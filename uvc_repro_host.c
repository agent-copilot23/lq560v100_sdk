/*
 * uvc_repro_host.c
 * ------------------------------------------------------------------
 * host(Linux PC) 侧最小复现程序：复现 UVC gadget "第一次开流正常、
 * STROFF 后第二次 STREAMON，约 5 秒后 VIDIOC_DQBUF 返回 ENODEV(19)"。
 *
 * 跑在哪一侧：PC/Linux host，对板子(gadget设备端)看到的 /dev/videoX。
 * 板子本身不调 V4L2_CID_*，本程序只模拟上位机行为。
 *
 * 编译（host x86 PC，或在板子上自环 /dev/video0 也可）：
 *   gcc -O2 -Wall uvc_repro_host.c -o uvc_repro_host
 *
 * 常用运行：
 *   ./uvc_repro_host -d /dev/video0 -f MJPG -W 1280 -H 720 -t 8
 *   ./uvc_repro_host -d /dev/video0 -f YUYV -W 1280 -H 720 -t 8
 *   # 分离实验：第二次(及第一次)不下发任何参数，只开关流，看是否仍 ENODEV
 *   ./uvc_repro_host -d /dev/video0 -f MJPG --no-set-ctrl
 *
 * 判读：
 *   - 第二次 DQBUF 返回 -1 且 errno=19(ENODEV) = USB 设备已消失，复现成功。
 *   - 只卡住不出帧、DQBUF 一直阻塞/超时，而不报 ENODEV = 管线没起但设备还在(缺陷②)。
 *   - 加 --no-set-ctrl 后正常 => 问题在参数下发；仍报错 => 问题在二次启停本身。
 * ------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <linux/videodev2.h>
#include <linux/v4l2-controls.h>

#define N_BUFS 4
#define DEF_TIMEOUT_MS 1000

struct buffer { void *start; size_t length; };

static struct buffer bufs[N_BUFS];
static int g_fd = -1;
static struct timespec g_t0;
static const char *device = "/dev/video0";

/* 相对启动的毫秒时间戳，便于对照"约5秒" */
static long now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec - g_t0.tv_sec) * 1000L + (ts.tv_nsec - g_t0.tv_nsec) / 1000000L;
}

static void log_ts(const char *tag)
{
    printf("[%7ld ms] %s\n", now_ms(), tag);
}

static int xioctl(int fd, unsigned long req, void *arg)
{
    int r;
    do { r = ioctl(fd, req, arg); } while (r == -1 && errno == EINTR);
    return r;
}

/* ---------- 设置单个控制，打印成功/失败 ---------- */
static int set_ctrl(int id, const char *name, int32_t val)
{
    struct v4l2_control c;
    memset(&c, 0, sizeof(c));
    c.id = id;
    c.value = val;
    if (xioctl(g_fd, VIDIOC_S_CTRL, &c) == -1) {
        printf("    S_CTRL %-22s id=0x%08x val=%d FAILED: %s(errno=%d)\n",
               name, id, val, strerror(errno), errno);
        return -1;
    }
    printf("    S_CTRL %-22s id=0x%08x val=%d OK\n", name, id, val);
    return 0;
}

/* 客户原始的 5 个参数（按客户给的顺序） */
static void apply_customer_ctrls(void)
{
    log_ts("--- 下发客户 5 个参数 ---");
    set_ctrl(V4L2_CID_GAIN,            "GAIN",               5);
    set_ctrl(V4L2_CID_EXPOSURE_AUTO,   "EXPOSURE_AUTO",     V4L2_EXPOSURE_AUTO_MODE_MANUAL); /* =1 手动 */
    set_ctrl(V4L2_CID_EXPOSURE_ABSOLUTE,"EXPOSURE_ABSOLUTE",220);
    set_ctrl(V4L2_CID_CONTRAST,        "CONTRAST",           100);
    set_ctrl(V4L2_CID_BACKLIGHT_COMPENSATION,"BACKLIGHT_COMP",2);
}

static int setup_format(uint32_t pixfmt, int w, int h)
{
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(g_fd, VIDIOC_G_FMT, &fmt) == -1)
        printf("G_FMT: %s\n", strerror(errno));

    fmt.fmt.pix.width = w;
    fmt.fmt.pix.height = h;
    fmt.fmt.pix.pixelformat = pixfmt;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;
    if (xioctl(g_fd, VIDIOC_S_FMT, &fmt) == -1) {
        printf("S_FMT(0x%08x %dx%d) FAILED: %s(errno=%d)\n",
               pixfmt, w, h, strerror(errno), errno);
        return -1;
    }
    printf("S_FMT OK: 0x%08x %ux%d, size=%u\n",
           fmt.fmt.pix.pixelformat, fmt.fmt.pix.width,
           fmt.fmt.pix.height, fmt.fmt.pix.sizeimage);
    return 0;
}

static void free_buffers(void)
{
    struct v4l2_requestbuffers rb;
    memset(&rb, 0, sizeof(rb));
    rb.count = 0;
    rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rb.memory = V4L2_MEMORY_MMAP;
    xioctl(g_fd, VIDIOC_REQBUFS, &rb); /* count=0 释放内核缓冲 */
    for (int i = 0; i < N_BUFS; i++) {
        if (bufs[i].start && bufs[i].start != MAP_FAILED)
            munmap(bufs[i].start, bufs[i].length);
        bufs[i].start = NULL;
        bufs[i].length = 0;
    }
}

static int reqbufs_mmap(void)
{
    free_buffers();
    struct v4l2_requestbuffers rb;
    memset(&rb, 0, sizeof(rb));
    rb.count = N_BUFS;
    rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    rb.memory = V4L2_MEMORY_MMAP;
    if (xioctl(g_fd, VIDIOC_REQBUFS, &rb) == -1) {
        printf("REQBUFS FAILED: %s(errno=%d)\n", strerror(errno), errno);
        return -1;
    }
    printf("REQBUFS count=%u\n", rb.count);

    for (int i = 0; i < N_BUFS; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (xioctl(g_fd, VIDIOC_QUERYBUF, &buf) == -1) {
            printf("QUERYBUF[%d] FAILED: %s\n", i, strerror(errno));
            return -1;
        }
        bufs[i].length = buf.length;
        bufs[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
                             MAP_SHARED, g_fd, buf.m.offset);
        if (bufs[i].start == MAP_FAILED) {
            printf("mmap[%d] FAILED: %s\n", i, strerror(errno));
            return -1;
        }
    }
    return 0;
}

static int queue_all(void)
{
    for (int i = 0; i < N_BUFS; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (xioctl(g_fd, VIDIOC_QBUF, &buf) == -1) {
            printf("QBUF[%d] FAILED: %s(errno=%d)\n", i, strerror(errno), errno);
            return -1;
        }
    }
    return 0;
}

static int stream_on(void)
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    log_ts(">>> VIDIOC_STREAMON");
    if (xioctl(g_fd, VIDIOC_STREAMON, &type) == -1) {
        printf("STREAMON FAILED: %s(errno=%d)\n", strerror(errno), errno);
        return -1;
    }
    return 0;
}

static int stream_off(void)
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    log_ts("<<< VIDIOC_STREAMOFF");
    if (xioctl(g_fd, VIDIOC_STREAMOFF, &type) == -1)
        printf("STREAMOFF FAILED: %s(errno=%d)\n", strerror(errno), errno);
    return 0;
}

/*
 * 取帧循环。
 * round=1: 取到第一帧后下发参数，然后继续取 duration_s 秒。
 * round=2: 只取帧，逐帧打印，重点抓 ENODEV(19) / POLLHUP。
 * 返回 0 正常结束，-1 检测到设备消失/出错。
 */
static int capture_round(int round, int duration_s, int set_ctrl, int max_frames)
{
    int frames = 0;
    long deadline = now_ms() + duration_s * 1000L;
    int first_frame = 1;

    while (1) {
        if (max_frames > 0 && frames >= max_frames) break;
        if (now_ms() >= deadline) break;

        struct pollfd pfd = { .fd = g_fd, .events = POLLIN };
        int pr = poll(&pfd, 1, DEF_TIMEOUT_MS);
        if (pr == -1) {
            printf("poll err: %s(errno=%d)\n", strerror(errno), errno);
            return -1;
        }
        if (pr == 0) {
            printf("[%7ld ms] round%d poll 超时(1000ms 无帧)\n", now_ms(), round);
            continue;
        }
        if (pfd.revents & (POLLHUP | POLLERR)) {
            printf("[%7ld ms] round%d poll 报告 POLLHUP/POLLERR=0x%x -> 设备已掉线\n",
                   now_ms(), round, pfd.revents);
            return -1;
        }

        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if (xioctl(g_fd, VIDIOC_DQBUF, &buf) == -1) {
            /* 这里就是客户报错点：errno=19 ENODEV */
            printf("[%7ld ms] round%d DQBUF FAILED: %s(errno=%d)  <== 复现点\n",
                   now_ms(), round, strerror(errno), errno);
            struct stat st;
            if (stat(device, &st) != 0)
                printf("         设备节点 %s 已不存在: %s\n", device, strerror(errno));
            return -1;
        }

        frames++;
        if (round == 1 && first_frame) {
            first_frame = 0;
            printf("[%7ld ms] round1 收到第一帧 (seq=%u), 之后下发参数\n",
                   now_ms(), buf.sequence);
            if (set_ctrl) apply_customer_ctrls();
        }
        if (round == 2 && (frames % 30 == 1))
            printf("[%7ld ms] round2 正常第 %d 帧 seq=%u\n", now_ms(), frames, buf.sequence);

        /* 归还缓冲 */
        if (xioctl(g_fd, VIDIOC_QBUF, &buf) == -1) {
            printf("QBUF(归还) FAILED: %s(errno=%d)\n", strerror(errno), errno);
            return -1;
        }
    }
    printf("[%7ld ms] round%d 结束, 共 %d 帧\n", now_ms(), round, frames);
    return 0;
}

static void usage(const char *p)
{
    printf("Usage: %s -d /dev/videoX [-f MJPG|YUYV] [-W w] [-H h] [-t seconds] "
           "[--no-set-ctrl] [-n max_frames_per_round]\n", p);
}

int main(int argc, char **argv)
{
    uint32_t pixfmt = V4L2_PIX_FMT_MJPEG;
    int w = 1280, h = 720, duration_s = 8, set_ctrl = 1, max_frames = 0;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-d") && i + 1 < argc) device = argv[++i];
        else if (!strcmp(argv[i], "-f") && i + 1 < argc) {
            const char *f = argv[++i];
            if (!strcmp(f, "YUYV")) pixfmt = V4L2_PIX_FMT_YUYV;
            else pixfmt = V4L2_PIX_FMT_MJPEG;
        }
        else if (!strcmp(argv[i], "-W") && i + 1 < argc) w = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-H") && i + 1 < argc) h = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-t") && i + 1 < argc) duration_s = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-n") && i + 1 < argc) max_frames = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--no-set-ctrl")) set_ctrl = 0;
        else { usage(argv[0]); return 1; }
    }

    clock_gettime(CLOCK_MONOTONIC, &g_t0);

    g_fd = open(device, O_RDWR);
    if (g_fd == -1) {
        printf("open %s FAILED: %s(errno=%d)\n", device, strerror(errno), errno);
        return 1;
    }
    printf("open %s OK, pixfmt=0x%08x %dx%d, 每轮 %ds, set_ctrl=%d\n",
           device, pixfmt, w, h, duration_s, set_ctrl);

    struct v4l2_capability cap;
    if (xioctl(g_fd, VIDIOC_QUERYCAP, &cap) == 0)
        printf("CAP: driver=%s card=%s bus=%s, streaming=%s\n",
               cap.driver, cap.card, cap.bus_info,
               (cap.capabilities & V4L2_CAP_STREAMING) ? "YES" : "NO");

    /* ====== 第一轮：第一次开流 + 取帧 + 设参 ====== */
    if (setup_format(pixfmt, w, h) == -1) goto out;
    if (reqbufs_mmap() == -1) goto out;
    if (queue_all() == -1) goto out;
    if (stream_on() == -1) goto out;
    capture_round(1, duration_s, set_ctrl, max_frames);
    stream_off();
    free_buffers(); /* 先释放第一轮内核缓冲，避免第二轮 S_FMT 报 EBUSY(16) */

    /* ====== 第二轮：关闭后重新开流，复现 ENODEV ====== */
    log_ts("===== 第二次 STREAMON 开始 =====");
    /* 正确顺序：释放旧缓冲 -> S_FMT -> REQBUFS -> QBUF -> STREAMON */
    free_buffers();
    if (setup_format(pixfmt, w, h) == -1) goto out;
    if (reqbufs_mmap() == -1) goto out;
    if (queue_all() == -1) goto out;
    if (stream_on() == -1) goto out;
    if (capture_round(2, duration_s, set_ctrl, max_frames) == -1) {
        printf(">>> 第二次开流后复现异常(设备消失/掉线)\n");
    }

out:
    stream_off();
    free_buffers();
    close(g_fd);
    return 0;
}
