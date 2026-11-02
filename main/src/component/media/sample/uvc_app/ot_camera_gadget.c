/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */

#include <sys/time.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/select.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <linux/usb/ch9.h>
#include "securec.h"

#include "ot_camera.h"
#include "uvc.h"
#include "ot_stream.h"
#include "frame_cache.h"
#include "uvc_media.h"
#include "uvc_dump.h"

#undef OT_DEBUG

#ifdef OT_DEBUG
#define debug(fmt, args...) printf(fmt, ##args)
#else
#define debug(fmt, args...)
#endif

#define ERR_INFO printf
#define RIGHT_SHIFT_8BIT 8
#define UDC_PATH_LENGTH 389
#define GET_MAX_PACKET_SIZE_PATH 398

static struct ot_uvc_dev *g_ot_cd[OT_UVC_MAX_CHN_NUM] = {0};
static ot_void ot_uvc_get_buf_size(struct ot_uvc_dev *uvc_dev, ot_u32 width, ot_u32 height, ot_u32 fcc,
    ot_u32 *dw_max_video_frame_size);

static const ot_char *image_fmt_to_string(ot_u32 format)
{
    switch (format) {
        case VIDEO_IMG_FORMAT_H264:
            return "H264";
        case VIDEO_IMG_FORMAT_H265:
            return "H265";
        case VIDEO_IMG_FORMAT_MJPEG:
            return "MJPEG";
        case VIDEO_IMG_FORMAT_YUYV:
            return "YUYV";
        case VIDEO_IMG_FORMAT_YUV420:
            return "YUV420";
        case VIDEO_IMG_FORMAT_NV21:
            return "NV21";
        case VIDEO_IMG_FORMAT_NV12:
            return "NV12";
        default:
            return "unknown format";
    }
}

#ifdef OT_DEBUG
static const ot_char *get_code(ot_s32 code)
{
    if (code == 0x01) { // 0x01 == SET_CUR
        return "SET_CUR";
    } else if (code == 0x81) { // 0x81 == GET_CUR
        return "GET_CUR";
    } else if (code == 0x82) { // 0x82 == GET_MIN
        return "GET_MIN";
    } else if (code == 0x83) { // 0x83 == GET_MAX
        return "GET_MAX";
    } else if (code == 0x84) { // 0x84 == GET_RES
        return "GET_RES";
    } else if (code == 0x85) { // 0x85 == GET_LEN
        return "GET_LEN";
    } else if (code == 0x86) { // 0x86 == GET_INFO
        return "GET_INFO";
    } else if (code == 0x87) { // 0x87 == GET_DEF
        return "GET_DEF";
    } else {
        return "UNKNOWN";
    }
}

static const ot_char *get_intf_cs_string(ot_s32 code)
{
    if (code == 0x01) {
        return "PROB_CONTROL";
    } else if (code == 0x02) {
        return "COMMIT_CONTROL";
    } else {
        return "UNKNOWN";
    }
}
#endif
static ot_void clear_uvc_frame_nodes(struct ot_uvc_dev *uvc_dev)
{
    ot_s32 i;
    uvc_cache_t *uvc_cache = uvc_dev->uvc_cache;

    if (uvc_cache == NULL)
        return ;
    for (i = 0; i < MAX_SAVE_FRAME_NUMS; i++) {
        if (uvc_dev->frame_node[i]) {
            put_node_to_queue(uvc_cache->free_queue, uvc_dev->frame_node[i]);
            uvc_dev->frame_node[i] = NULL;
        }
    }
}
static struct ot_uvc_dev *ot_uvc_init(const ot_char *dev_name, ot_u32 dev_no)
{
    struct ot_uvc_dev *uvc_dev = NULL;
    struct video_ability cap;
    ot_s32 ret;
    ot_s32 fd;

    ot_char *regular_path = realpath(dev_name, NULL);
    if (regular_path == NULL) {
        ERR_INFO("unable to get real path: %s\n", dev_name);
        return NULL;
    }
    fd = open(regular_path, O_RDWR | O_NONBLOCK);
    free(regular_path);
    if (fd == -1) {
        ERR_INFO("v4l2 open failed(%s): %d\n", dev_name, errno);
        return NULL;
    }

    ret = ioctl(fd, VIDEO_IOCTL_QUERY_CAP, &cap);
    if (ret < 0) {
        ERR_INFO("unable to query device: %d\n", errno);
        close(fd);
        return NULL;
    }

    debug("open succeeded(%s:caps=0x%04x)\n", dev_name, cap.dw_caps);

    if (!(cap.dw_caps & 0x02)) {
        close(fd);
        return NULL;
    }

    debug("device is %s on bus %s\n", cap.a_card, cap.a_bus_info);

    uvc_dev = (struct ot_uvc_dev *)malloc(sizeof *uvc_dev);
    if (uvc_dev == NULL) {
        close(fd);
        return NULL;
    }

    (ot_void)memset_s(uvc_dev, sizeof(*uvc_dev), 0, sizeof(*uvc_dev));
    uvc_dev->i_fd = fd;
    uvc_dev->dev_no = dev_no;
    uvc_dev->dump_frame_id = 0;

    return uvc_dev;
}

static ot_void ot_uvc_video_fill_buffer_userptr(struct ot_uvc_dev *uvc_dev, struct video_cache *v_cache)
{
    ot_s32 s32_retry_count = 0;
    v_cache->dw_used = 0;

    switch (uvc_dev->dw_fcc) {
        case VIDEO_IMG_FORMAT_MJPEG:
        case VIDEO_IMG_FORMAT_H264:
        case VIDEO_IMG_FORMAT_H265:
        case VIDEO_IMG_FORMAT_NV21:
        case VIDEO_IMG_FORMAT_NV12:
        case VIDEO_IMG_FORMAT_YUYV:
        case VIDEO_IMG_FORMAT_YUV420: {
            uvc_cache_t *c_uvc_cache = uvc_cache_get(uvc_dev->dev_no);
            frame_node_t *c_node = 0;
            frame_queue_t *c_q = 0;
            frame_queue_t *c_fq = 0;
            do {
                if (c_uvc_cache != NULL) {
                    c_q = c_uvc_cache->ok_queue;
                    c_fq = c_uvc_cache->free_queue;
                    get_node_from_queue(c_q, &c_node);
                }

                if ((uvc_dev->frame_node[v_cache->dw_index] != 0) && (c_uvc_cache  != NULL)) {
                    // printf("###RAWDBG### fill idx=%u RECYCLE old node\n", v_cache->dw_index);
                    put_node_to_queue(c_fq, uvc_dev->frame_node[v_cache->dw_index]);
                    uvc_dev->frame_node[v_cache->dw_index] = OT_NULL;
                }

                if (c_node != 0) {
                    v_cache->dw_used = c_node->used;
                    if (c_node->is_smr && c_node->used > 0) {
                        ot_smr_flush_cache(c_node->phys, c_node->mem, c_node->used);
                    }
                    v_cache->n.ul_usr = (ot_ulong)c_node->mem;
                    v_cache->dw_len = c_node->length;
                    uvc_dev->frame_node[v_cache->dw_index] = c_node;

                    /* 48M RAW: QBUF 前最后一个用户态数据点 */
                    if (c_node->dump_id > 0) {
                        uvc_dump_frame_stage(uvc_dev->dev_no, c_node->dump_id,
                                             "before_qbuf", "16bits", "RGGB",
                                             uvc_dev->dw_width, uvc_dev->dw_height,
                                             uvc_dev->dw_width * 2, c_node->mem, c_node->used);
                        c_node->dump_id = 0;
                    }

                    /* 抽样 1/100: QBUF 前确认 buffer 内容(unpack 真数据 vs 全 0)+ 地址。坐实
                     * "节点在发送时刻有数据" -> 若 host 仍收到 0 即为 DMA/cache 一致性问题。 */
                    { static unsigned int raw_node_dbg = 0; if ((raw_node_dbg++ % 100) == 0) {
                        ot_u16 *pp = (ot_u16 *)c_node->mem;
                        printf("###RAWDBG### preQBUF idx=%u va=%p phys=0x%llx used=%u first8u16:%u %u %u %u %u %u %u %u\n",
                               v_cache->dw_index, c_node->mem,
                               (unsigned long long)c_node->phys, c_node->used,
                               pp[0], pp[1], pp[2], pp[3], pp[4], pp[5], pp[6], pp[7]);
                    }}
                    break;
                } else {
                    if (s32_retry_count == 0 || (s32_retry_count % 2000) == 0) {
                        // printf("###RAWDBG### fill idx=%u ok_queue EMPTY, retry=%d\n", v_cache->dw_index, s32_retry_count);
                    }
                    usleep(1000); // delay 1000 us
                    s32_retry_count++;
                }
            } while (s32_retry_count < 6000 && uvc_dev->i_streaming); // max 6000ms
        }
            break;
        default:
            break;
    }
    if (s32_retry_count >= 6000) {
        ERR_INFO("failed to get userptr c_node\n");
        if (uvc_dev->uvc_cache != NULL) {
            uvc_dev->uvc_cache->dbg_ok_empty++; /* RAWDBG: ok_queue 取空 6s 超时计数 */
        }
    }
}

static ot_s32 ot_uvc_video_process_user_ptr(struct ot_uvc_dev *uvc_dev)
{
    struct video_cache v_cache;
    ot_s32 ret, do_dequeue = 1;

    if (!uvc_dev->i_streaming)
        return 0;

    (ot_void)memset_s(&v_cache, sizeof(v_cache), 0, sizeof(v_cache));
    v_cache.dw_type = VIDEO_CACHE_TYPE_OPT;
    v_cache.dw_mem  = VIDEO_MM_USER;

    for (int i = 0; i < uvc_dev->dw_nbufs; i++) {
        if (uvc_dev->frame_node[i] == NULL) {
            do_dequeue = 0;
            v_cache.dw_index = i;
            break;
        }
    }
    /* printf("###RAWDBG### send: found_null_slot=%d idx=%u frame_node=[%d,%d,%d,%d]\n",
        !do_dequeue, v_cache.dw_index,
        uvc_dev->frame_node[0] != 0, uvc_dev->frame_node[1] != 0,
        uvc_dev->frame_node[2] != 0, uvc_dev->frame_node[3] != 0); */

    if (do_dequeue) {
        // printf("###RAWDBG### send: all %u slots full -> DQBUF\n", uvc_dev->dw_nbufs);
        ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_DQUEUE_BUF, &v_cache);
        // printf("###RAWDBG### send: DQBUF ret=%d idx=%u\n", ret, v_cache.dw_index);
        if (ret < 0) {
            return ret;
        }
        if (uvc_dev->uvc_cache != NULL) {
            uvc_dev->uvc_cache->dbg_dqbuf++; /* RAWDBG: DQBUF 回收成功计数 */
        }
    }

    ot_uvc_video_fill_buffer_userptr(uvc_dev, &v_cache);

    if (v_cache.dw_used > 0) {
        ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_QUEUE_BUF, &v_cache);
        if (ret >= 0 && uvc_dev->uvc_cache != NULL) {
            uvc_dev->uvc_cache->dbg_qbuf++; /* RAWDBG: QBUF 成功计数(每帧, 非 1/100) */
        }
        {   /* 抽样 1/100: 通路验证关键信号——QBUF 成败(errno 14 = SMR userptr pin 失败) */
            static unsigned int qbuf_cnt = 0;
            if ((qbuf_cnt++ % 100) == 0) {
                printf("###RAWDBG### QBUF idx=%u ret=%d errno=%d (1/100)\n",
                       v_cache.dw_index, ret, (ret < 0) ? errno : 0);
            }
        }
        if (ret < 0) {
            ERR_INFO("Unable to requeue buffer(1): %d\n", errno);
            return ret;
        }
    }

    return 0;
}

static ot_s32 ot_uvc_video_reqbufs_user_ptr(struct ot_uvc_dev *uvc_dev, ot_s32 nbufs)
{
    struct video_buf_request v_rb;
    ot_s32 s32_ret;

    uvc_dev->dw_nbufs = 0;

    (ot_void)memset_s(&v_rb, sizeof(v_rb), 0, sizeof(v_rb));
    v_rb.dw_cnt  = nbufs;
    v_rb.dw_type = VIDEO_CACHE_TYPE_OPT;
    v_rb.dw_mem  = VIDEO_MM_USER;

    s32_ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_REQUEST_BUF, &v_rb);
    if (s32_ret < 0) {
        ERR_INFO("Unable to allocate buffers: %d\n", errno);
        return s32_ret;
    }

    uvc_dev->dw_nbufs = v_rb.dw_cnt;

    return 0;
}

static ot_void ot_uvc_video_stream_user_ptr(struct ot_uvc_dev *uvc_dev)
{
    struct video_cache v_cache;
    ot_u32 i;
    ot_s32 cache_type = VIDEO_CACHE_TYPE_OPT;
    ot_s32 ret;

    for (i = 0; i < (uvc_dev->dw_nbufs); ++i) {
        uvc_dev->frame_node[i] = NULL;
    }

    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_STREAM_ON, &cache_type);
    if (ret < 0) {
        ERR_INFO("Unable to stream on: %d\n", errno);
        return;
    }

    uvc_dev->i_streaming = 1;

    return;
}

static ot_s32 ot_uvc_video_set_fmt(struct ot_uvc_dev *uvc_dev)
{
    struct video_fmt v_fmt;
    ot_s32 ret;

    (ot_void)memset_s(&v_fmt, sizeof(v_fmt), 0, sizeof(v_fmt));
    v_fmt.dw_type = VIDEO_CACHE_TYPE_OPT;
    v_fmt.fmt.pix.dw_width = uvc_dev->dw_width;
    v_fmt.fmt.pix.dw_height = uvc_dev->dw_height;
    v_fmt.fmt.pix.dw_fmt = uvc_dev->dw_fcc;
    v_fmt.fmt.pix.dw_fld = VIDEO_FLD_NOTHING;

    if ((uvc_dev->dw_fcc == VIDEO_IMG_FORMAT_MJPEG) ||
        (uvc_dev->dw_fcc == VIDEO_IMG_FORMAT_H264) ||
        (uvc_dev->dw_fcc == VIDEO_IMG_FORMAT_H265))
    {
        ot_uvc_get_buf_size(uvc_dev, uvc_dev->dw_width, uvc_dev->dw_height, uvc_dev->dw_fcc, &v_fmt.fmt.pix.dw_size);
    }

    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_SET_FORMAT, &v_fmt);
    if (ret < 0) {
        ERR_INFO("Unable to set format: %d\n", errno);
    }

    return ret;
}

static ot_void ot_uvc_stream_off(struct ot_uvc_dev *uvc_dev)
{
    ot_s32 cache_type = VIDEO_CACHE_TYPE_OPT;
    ot_s32 ret;

    if (uvc_dev->i_streaming == 0) {
        return;
    }

    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_STREAM_OFF, &cache_type);
    if (ret < 0) {
        ERR_INFO("Unable to stream off: %d\n", errno);
    }
    ot_stream_shutdown(uvc_dev->dev_no);
    debug("Stopping video stream.\n");
    
    uvc_dev->i_streaming = 0;
}

static ot_void ot_uvc_video_disable(struct ot_uvc_dev *uvc_dev)
{
    printf("4444400000001   %s  %d\n",__func__,__LINE__);
    ot_uvc_stream_off(uvc_dev);
    uvc_frame_nodes_free(uvc_dev->uvc_cache);
    clear_uvc_frame_nodes(uvc_dev);
    uvc_dev->dump_frame_id = 0;
}

static ot_void ot_uvc_video_enable(struct ot_uvc_dev *uvc_dev)
{
    encoder_property ep;
    ot_u32 max_video_frame_size = 0;

    ot_uvc_video_disable(uvc_dev);

    ep.format = uvc_dev->dw_fcc;
    ep.width = uvc_dev->dw_width;
    ep.height = uvc_dev->dw_height;
    ep.fps = uvc_dev->i_fps;
    ep.compsite = 0;

    ot_uvc_get_buf_size(uvc_dev, ep.width, ep.height, ep.format, &max_video_frame_size);
    uvc_frame_nodes_alloc(uvc_dev->uvc_cache, max_video_frame_size, sample_uvc_is_raw_fmt(&ep));
    /* 修复①: V4L2 槽数 ≤ 实际节点数。节点数 < 槽数时, "满槽才 DQBUF" 永不触发
     * (空槽永远存在) -> 已 QBUF 节点卡在内核不回收 -> free/ok 双队列耗空 -> 卡死。
     * 6000p RAW 仅 2 节点(192M/96M), 故槽数收到 2(而非 MAX_SAVE_FRAME_NUMS=4)。 */
    unsigned int node_cnt = uvc_queue_count(uvc_dev->uvc_cache->free_queue);
    ot_u32 req_nbufs = ((node_cnt > 0) && (node_cnt < MAX_SAVE_FRAME_NUMS)) ? node_cnt : MAX_SAVE_FRAME_NUMS;

    ot_stream_shutdown(uvc_dev->dev_no);
    ot_stream_set_enc_property(uvc_dev->dev_no, &ep);
    ot_stream_startup(uvc_dev->dev_no);
    ot_uvc_video_reqbufs_user_ptr(uvc_dev, req_nbufs);
    ot_uvc_video_stream_user_ptr(uvc_dev);
}

static ot_s32 is_contain_udc_sub_dir(const ot_char *path, ot_s32 path_size)
{
    DIR *p_dir = NULL;
    struct dirent *p_dirent = NULL;
    if (path_size < 0) {
        return -1;
    }

    p_dir = opendir(path);
    if (p_dir == NULL) {
        ERR_INFO("No such path: %s\n", path);
        return -1;
    }

    while ((p_dirent = readdir(p_dir)) != NULL) {
        if (strcmp(p_dirent->d_name, "UDC") == 0) {
            closedir(p_dir);
            return 0;
        }
    }

    closedir(p_dir);

    return -1;
}

static ot_s32 read_file(const ot_char *path, ot_u32 path_size, const ot_char *dest, ot_u32 size)
{
    FILE *input = NULL;
    ot_char *realpath_res = NULL;
    ot_s32 ret;

    realpath_res = realpath(path, NULL);
    if (realpath_res == NULL) {
        return -1;
    }

    input = fopen(realpath_res, "rw");
    free(realpath_res);
    realpath_res = NULL;
    if (input == NULL) {
        ERR_INFO("No such path: %s\n", path);
        return -1;
    }

    ret = fscanf_s(input, "%s", dest, size);
    if (ret != 1) {
        ERR_INFO("fscanf_s from %s fail\n", path);
        ret = -1;
    } else {
        ret = 0;
    }

    if (fclose(input) != 0) {
        ERR_INFO("close file failed!\n");
    }

    return ret;
}

static ot_s32 get_udc_node_name(ot_char *node_name, ot_u32 size)
{
    DIR *p_dir = NULL;
    struct dirent *p_dirent = NULL;
    const ot_char *path_tmp = "/sys/kernel/config/usb_gadget/";
    ot_char tmp[UDC_PATH_LENGTH] = {0};

    p_dir = opendir(path_tmp);
    if (p_dir == NULL) {
        ERR_INFO("No such path: %s\n", path_tmp);
        return -1;
    }

    while ((p_dirent = readdir(p_dir)) != NULL) {
        if (strcmp(p_dirent->d_name, ".") == 0 || strcmp(p_dirent->d_name, "..") == 0) {
            continue;
        }

        if (p_dirent->d_type == DT_DIR) {
            snprintf_truncated_s(tmp, UDC_PATH_LENGTH, "%s%s", path_tmp, p_dirent->d_name);

            if (is_contain_udc_sub_dir(tmp, UDC_PATH_LENGTH) == 0) {
                snprintf_truncated_s(tmp, UDC_PATH_LENGTH, "%s%s/UDC", path_tmp, p_dirent->d_name);
                closedir(p_dir);
                return read_file(tmp, UDC_PATH_LENGTH, node_name, size);
            }
        }
    }

    closedir(p_dir);

    return -1;
}

static ot_u32 get_max_payload_transfer_size(const struct ot_uvc_frame_info *frm_info)
{
    const ot_u32 high_speed_size = 3072;
    const ot_u32 full_speed_size = 1023;
    const ot_char *path_tmp = "/sys/class/udc/";

    ot_char tmp[128];
    ot_char target_path[GET_MAX_PACKET_SIZE_PATH]; // 398=256+128+14
    ot_u32 result = high_speed_size;

    if (get_udc_node_name(tmp, 128) != 0) { /* 128 bytes used for tmp */
        return result;
    }

    snprintf_truncated_s(target_path, GET_MAX_PACKET_SIZE_PATH, "%s%s/current_speed", path_tmp, tmp);

    if (read_file(target_path, GET_MAX_PACKET_SIZE_PATH, tmp, 128) != 0) { /* 128 bytes used for tmp */
        return result;
    }

    if (strcmp(tmp, "super-speed") == 0) {
        result = frm_info->ss_xfersize;
    } else if (strcmp(tmp, "high-speed") == 0) {
        result = frm_info->hs_xfersize;
    } else if (strcmp(tmp, "full-speed") == 0) {
        result = full_speed_size;
    } else {
        ERR_INFO("USB cable is not connected yet.\n");
    }

    return result;
}

static ot_void ot_uvc_get_buf_size(struct ot_uvc_dev *uvc_dev, ot_u32 width, ot_u32 height, ot_u32 fcc,
    ot_u32 *dw_max_video_frame_size)
{
    switch (fcc) {
        case VIDEO_IMG_FORMAT_YUYV:
            *dw_max_video_frame_size = width * height * 2; /* 2 bytes a pixel */
            break;
        case VIDEO_IMG_FORMAT_YUV420:
        case VIDEO_IMG_FORMAT_NV12:
        case VIDEO_IMG_FORMAT_NV21:
            *dw_max_video_frame_size = width * height * 3 / 2; /* 3 / 2,  1.5 byte a pixel */
            break;
        case VIDEO_IMG_FORMAT_MJPEG:
        case VIDEO_IMG_FORMAT_H264:
        case VIDEO_IMG_FORMAT_H265:
            if ((width <= 4000) && (height <= 3000)) {
                *dw_max_video_frame_size = MAX_PAYLOAD_IMAGE_SIZE / 2;
            }
            else {
                *dw_max_video_frame_size = MAX_PAYLOAD_IMAGE_SIZE;
            }
            
            break;
        default:
            printf("format error!\n");
            break;
    }
}

#ifdef UVC_STILL_IMAGE
static ot_u32 clamp(ot_u32 val, ot_u32 min, ot_u32 max);

static ot_u32 is_still_image(ot_u32 cs)
{
    return (cs == OT_UVC_VS_STILL_PROBE_CONTROL || cs == OT_UVC_VS_STILL_COMMIT_CONTROL ||
        cs == OT_UVC_VS_STILL_IMAGE_TRIGGER_CONTROL);
}

static ot_u32 ot_get_still_capture_method(ot_u32 dev_no)
{
    ot_u32 result = 0;
    ot_char char_tmp[128] = {0};
    ot_char file_path[128] = {0};
    (ot_void)snprintf_truncated_s(file_path, sizeof(file_path),
        "/sys/kernel/config/usb_gadget/camera/functions/uvc.usb%u/still_capture_method", dev_no);

    if (read_file(file_path, GET_MAX_PACKET_SIZE_PATH, char_tmp, 128) != 0) { /* 128 bytes used for char_tmp */
        printf("%d, read_file failed!\n", __LINE__);
        return result;
    }

    result = (ot_u32)strtol(char_tmp, NULL, 10); /* 10 base */
    return result;
}

static ot_u32 ot_get_still_maxpacket(ot_u32 dev_no)
{
    ot_u32 result = 32768;
    ot_char char_tmp[128] = {0};
    ot_char file_path[128] = {0};
    (ot_void)snprintf_truncated_s(file_path, sizeof(file_path),
        "/sys/kernel/config/usb_gadget/camera/functions/uvc.usb%u/still_maxpacket", dev_no);

    if (read_file(file_path, GET_MAX_PACKET_SIZE_PATH, char_tmp, 128) != 0) { /* 128 bytes used for char_tmp */
        printf("%d, read_file failed!\n", __LINE__);
        return result;
    }

    result = (ot_u32)strtol(char_tmp, NULL, 10); /* 10 base */
    return result;
}
static ot_void ot_uvc_handle_streaming_still_control(struct ot_uvc_dev *uvc_dev,
    struct ot_uvc_streaming_still_control *u_still_ctrl, ot_u8 i_frame, ot_u8 i_format)
{
    const struct ot_uvc_format_info *fmt_info = NULL;
    const struct ot_uvc_frame_info *frm_info = NULL;
    ot_u8 n_frm = 0;
    ot_u32 max_video_frame_size = 0;

    if (i_format >= array_size(ot_fmt)) {
        i_format = 0;
    }

    fmt_info = &ot_fmt[i_format];

    while (fmt_info->frames[n_frm].width != 0) {
        ++n_frm;
    }

    if (i_frame >= n_frm) {
        i_frame = 0;
    }

    frm_info = &fmt_info->frames[i_frame];

    (ot_void)memset_s(u_still_ctrl, sizeof(*u_still_ctrl), 0, sizeof(*u_still_ctrl));

    u_still_ctrl->b_format_index = i_format + 1;               /* Yuv: 1, Mjpeg: 2. */
    u_still_ctrl->b_frame_index = i_frame + 1;                 /* 360p: 1 720p: 2. */
    u_still_ctrl->b_compression_index = 0;

    ot_uvc_get_buf_size(uvc_dev, frm_info->width,  frm_info->height, fmt_info->fcc, &max_video_frame_size);
    (ot_void)memcpy_s(&u_still_ctrl->dw_max_video_frame_size, sizeof(ot_u32), &max_video_frame_size, sizeof(ot_u32));

    if (uvc_dev->dw_bulk != 0) {
        u_still_ctrl->dw_max_payload_transfer_size = uvc_dev->dw_bulk_size; /* This should be filled by the driver. */
    } else {
        u_still_ctrl->dw_max_payload_transfer_size = get_max_payload_transfer_size(frm_info);
    }

    if (ot_get_still_capture_method(uvc_dev->dev_no) == UVC_STILL_IMAGE_METHOD3)
        u_still_ctrl->dw_max_payload_transfer_size = ot_get_still_maxpacket(uvc_dev->dev_no);
}

static ot_void ot_uvc_handle_streaming_still_request(struct ot_uvc_dev *uvc_dev, ot_u8 req, ot_u8 cs,
    struct uvc_request_data *u_req_data)
{
    struct ot_uvc_streaming_still_control u_still_ctrl;
    struct ot_uvc_streaming_still_control *still_ctrl;

    u_still_ctrl = *(struct ot_uvc_streaming_still_control *)(u_req_data->data);
    u_req_data->length = 0x0B;
    const struct ot_uvc_format_info *format = &ot_fmt[uvc_dev->probe_still.b_format_index - 1];
    const struct ot_uvc_frame_info *frame = &format->frames[uvc_dev->probe_still.b_frame_index - 1];

    switch (req) {
        case OT_UVC_SET_CUR:
            uvc_dev->i_control = cs;
            u_still_ctrl.dw_max_payload_transfer_size =
                uvc_dev->dw_bulk ? uvc_dev->dw_bulk_size : get_max_payload_transfer_size(frame);
            if (ot_get_still_capture_method(uvc_dev->dev_no) == UVC_STILL_IMAGE_METHOD3)
                u_still_ctrl.dw_max_payload_transfer_size = ot_get_still_maxpacket(uvc_dev->dev_no);
            break;
        case OT_UVC_GET_CUR:
            still_ctrl = (cs == OT_UVC_VS_STILL_PROBE_CONTROL) ? &uvc_dev->probe_still : &uvc_dev->commit_still;
            (ot_void)memcpy_s(&u_still_ctrl, sizeof(u_still_ctrl), still_ctrl, sizeof(u_still_ctrl));
            u_still_ctrl.dw_max_payload_transfer_size =
                uvc_dev->dw_bulk ? uvc_dev->dw_bulk_size : get_max_payload_transfer_size(frame);
            if (ot_get_still_capture_method(uvc_dev->dev_no) == UVC_STILL_IMAGE_METHOD3)
                u_still_ctrl.dw_max_payload_transfer_size = ot_get_still_maxpacket(uvc_dev->dev_no);
            break;
        case OT_UVC_GET_MIN:
        case OT_UVC_GET_MAX:
        case OT_UVC_GET_DEF:
            ot_uvc_handle_streaming_still_control(uvc_dev, &u_still_ctrl, uvc_dev->probe_still.b_frame_index - 1, \
                uvc_dev->probe_still.b_format_index - 1);
            break;
        case OT_UVC_GET_RES:
            (ot_void)memset_s(u_req_data->data, sizeof(u_req_data->data), 0, sizeof(u_req_data->data));
            return;
        case OT_UVC_GET_LEN:
            u_req_data->data[0] = 0x00;
            u_req_data->data[1] = 0x0B;
            u_req_data->length = 0x2;
            return;
        case OT_UVC_GET_INFO:
            u_req_data->data[0] = 0x03;
            u_req_data->length = 1;
            return;
        default:
            return;
    }

    errno_t ret = memcpy_s(u_req_data->data, sizeof(u_req_data->data), &u_still_ctrl, sizeof(u_still_ctrl));
    if (ret != EOK) {
        ERR_INFO("copy back u_still_ctrl to req_data fail %d\n", ret);
    }
}

static struct ot_uvc_streaming_still_control *get_streaming_still_control(ot_s32 control_type,
    struct ot_uvc_dev *uvc_dev, const struct uvc_request_data *u_req_data)
{
    switch (control_type) {
        case OT_UVC_VS_STILL_PROBE_CONTROL:
            return &uvc_dev->probe_still;
        case OT_UVC_VS_STILL_COMMIT_CONTROL:
            return &uvc_dev->commit_still;
        case OT_UVC_VS_STILL_IMAGE_TRIGGER_CONTROL:
            return &uvc_dev->commit_still;
        default:
            return NULL;
    }
}

static ot_void do_ot_uvc_still_commit_trigger_event(struct ot_uvc_dev *uvc_dev,
    struct ot_uvc_streaming_still_control *u_still_target, const struct ot_uvc_frame_info *frm_info, ot_u32 fmt)
{
    int ret;
    struct video_framebuffer *still_image = &uvc_dev->still_image;

    if (uvc_dev->i_control == OT_UVC_VS_STILL_COMMIT_CONTROL) {
        still_image->fmt.height = frm_info->height;
        still_image->fmt.width = frm_info->width;
        still_image->fmt.pixelformat = fmt;
        still_image->fmt.sizeimage = u_still_target->dw_max_video_frame_size;
    } else if (uvc_dev->i_control == OT_UVC_VS_STILL_IMAGE_TRIGGER_CONTROL) {
        ret = sample_uvc_get_still_image(still_image);
        if (ret < 0) {
            printf("[sample] get still_image failed, ret: %d\n", ret);
        }

        if (still_image->base == NULL) {
            printf("[sample] get still image failed, addr is NULL!\n");
            return;
        }

        ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_SET_FRAME_BUF, still_image);
        if (ret < 0) {
            printf("[sample] VIDEO_IOCTL_SET_FRAME_BUF failed: %d\n", errno);
        }

        free(still_image->base);
        still_image->base = NULL;
    }
}

static ot_void do_ot_uvc_streaming_still_data_event(struct ot_uvc_dev *uvc_dev, struct uvc_request_data *u_req_data)
{
    struct ot_uvc_streaming_still_control *u_still_target = NULL;
    struct ot_uvc_streaming_still_control u_still_ctrl;
    const struct ot_uvc_format_info *fmt_info = NULL;
    const struct ot_uvc_frame_info *frm_info = NULL;
    ot_u32 i_fmt, i_frm;
    ot_u32 n_frm;

    if ((u_still_target = get_streaming_still_control(uvc_dev->i_control, uvc_dev, u_req_data)) == NULL) {
        return;
    }

    ot_s32 ret = memcpy_s(&u_still_ctrl, sizeof(u_still_ctrl), u_req_data->data, sizeof(u_still_ctrl));
    if (ret != EOK) {
        ERR_INFO("memcpy_s u_still_ctrl fail, ret %d\n", ret);
        return;
    }

    i_fmt = clamp((ot_u32)u_still_ctrl.b_format_index, 1U, (ot_u32)array_size(ot_fmt));
    fmt_info = &ot_fmt[i_fmt - 1];
    n_frm = 0;

    while (fmt_info->frames[n_frm].width != 0) {
        ++n_frm;
    }

    i_frm = clamp((ot_u32)u_still_ctrl.b_frame_index, 1U, n_frm);
    frm_info = &fmt_info->frames[i_frm - 1];

    u_still_target->b_format_index = (ot_u8)i_fmt;
    u_still_target->b_frame_index = (ot_u8)i_frm;

    switch (fmt_info->fcc) {
        case VIDEO_IMG_FORMAT_YUYV:
            u_still_target->dw_max_video_frame_size = frm_info->width * frm_info->height * 2; /* 2 bytes a pixel */
            break;
        case VIDEO_IMG_FORMAT_NV12:
        case VIDEO_IMG_FORMAT_NV21:
        case VIDEO_IMG_FORMAT_YUV420:
            u_still_target->dw_max_video_frame_size =
                frm_info->width * frm_info->height * 3 / 2; /* 3 / 2, 1.5 bytes a pixel */
            break;
        case VIDEO_IMG_FORMAT_MJPEG:
            if (uvc_dev->dw_img_size == 0) {
                printf("WARNING: MJPEG requested and no image loaded.\n");
            }
            u_still_target->dw_max_video_frame_size = uvc_dev->dw_img_size;
            break;
        default:
            printf("WARNING: wrong image format\n");
            break;
    }

    do_ot_uvc_still_commit_trigger_event(uvc_dev, u_still_target, frm_info, fmt_info->fcc);
}

#ifdef UVC_STILL_BUTTON_TEST
int button_pressed()
{
    struct ot_uvc_dev *uvc_dev = g_ot_cd[0];
    struct video_framebuffer tmp;
    int ret;

    if (!uvc_dev->i_streaming)
        return 0;

    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_BUTTON, (void*)&tmp);
    if (ret < 0)
        printf("VIDEO_IOCTL_BUTTON ret:%d errno:%d\n", ret, errno);

    return ret;
}
#endif
#endif

static ot_u32 ot_get_max_payload_transfer_size_for_bulk(ot_u32 dev_no)
{
    ot_u32 result = 1024;
    ot_char char_tmp[128] = {0};
    ot_char file_path[128] = {0};
    (ot_void)snprintf_truncated_s(file_path, sizeof(file_path),
        "/sys/kernel/config/usb_gadget/camera/functions/uvc.usb%u/streaming_maxpacket", dev_no);

    if (read_file(file_path, GET_MAX_PACKET_SIZE_PATH, char_tmp, 128) != 0) { /* 128 bytes used for char_tmp */
        printf("%d, read_file failed!\n", __LINE__);
        return result;
    }
    printf("%s, %s \n", __func__, char_tmp);
    result = (ot_u32)strtol(char_tmp, NULL, 10); /* 10 base */
    return result;
}

static ot_void ot_uvc_handle_streaming_control(struct ot_uvc_dev *uvc_dev, struct ot_uvc_streaming_control *u_str_ctrl,
    ot_u8 i_frame, ot_u8 i_format)
{
    const struct ot_uvc_format_info *fmt_info = NULL;
    const struct ot_uvc_frame_info *frm_info = NULL;
    ot_u8 n_frm = 0;
    ot_u32 max_video_frame_size = 0;

    if (i_format >= array_size(ot_fmt)) {
        i_format = 0;
    }

    debug("i_format = %d\n", i_format);
    fmt_info = &ot_fmt[i_format];

    while (fmt_info->frames[n_frm].width != 0) {
        ++n_frm;
    }

    if (i_frame >= n_frm) {
        i_frame = 0;
    }

    frm_info = &fmt_info->frames[i_frame];

    (ot_void)memset_s(u_str_ctrl, sizeof(*u_str_ctrl), 0, sizeof(*u_str_ctrl));

    u_str_ctrl->bm_hint = 1;
    u_str_ctrl->b_format_index = i_format + 1;               /* Yuv: 1, Mjpeg: 2. */
    u_str_ctrl->b_frame_index = i_frame + 1;                 /* 360p: 1 720p: 2. */
    u_str_ctrl->dw_frame_interval = frm_info->intervals[0]; /* Corresponding to the number of frame rate. */

    ot_uvc_get_buf_size(uvc_dev, frm_info->width, frm_info->height, fmt_info->fcc, &max_video_frame_size);
    (ot_void)memcpy_s(&u_str_ctrl->dw_max_video_frame_size, sizeof(ot_u32), &max_video_frame_size, sizeof(ot_u32));

    if (uvc_dev->dw_bulk != 0) {
        u_str_ctrl->dw_max_payload_transfer_size = uvc_dev->dw_bulk_size; /* This should be filled by the driver. */
    } else {
        u_str_ctrl->dw_max_payload_transfer_size = get_max_payload_transfer_size(frm_info);
    }
    u_str_ctrl->bm_framing_info = 3; /* 3 format mode */
    u_str_ctrl->b_prefered_version = 1;
    u_str_ctrl->b_max_version = 1;

    u_str_ctrl->dw_clock_frequency = 48000000; // 48000000 Hz
    u_str_ctrl->b_usage = 17; // 17 file-storage mode
    u_str_ctrl->b_msettings = 0x20;
    u_str_ctrl->b_max_number_of_ref_frames_plus1 = 1;
    u_str_ctrl->b_rate_control_modes = 0x22;
}

static ot_void ot_uvc_handle_standard_request(const struct ot_uvc_dev *uvc_dev,
    const struct usb_ctrlrequest *u_ctrl_req, const struct uvc_request_data *u_req_data)
{
    debug("camera standard request\n");
}


static void ot_uvc_eve_undef_control(struct ot_uvc_dev *uvc_dev, ot_u8 cs, struct uvc_request_data *u_req_data)
{
    switch (cs) {
        case OT_UVC_VC_REQUEST_ERROR_CODE_CONTROL:
            u_req_data->length = uvc_dev->request_error_code.length;
            u_req_data->data[0] = uvc_dev->request_error_code.data[0];
            break;
        default:
            uvc_dev->request_error_code.length = 1;
            uvc_dev->request_error_code.data[0] = 0x06;
            break;
    }
}

static ot_void ot_uvc_handle_control_request(struct ot_uvc_dev *uvc_dev, ot_u8 req, ot_u8 unit_id, ot_u8 cs,
    struct uvc_request_data *u_req_data)
{
    switch (unit_id) {
        case OT_UVC_ID_UNDEFINED:
            ot_uvc_eve_undef_control(uvc_dev, cs, u_req_data);
            break;
        case OT_UVC_CAMERA_TERMINAL_ID:
            ot_stream_event_ct_control(uvc_dev, req, unit_id, cs, u_req_data);
            break;
        case OT_UVC_PROCESSING_UNIT_ID:
            ot_stream_event_pu_control(uvc_dev, req, unit_id, cs, u_req_data);
            break;
        case OT_UVC_ENCODING_UNIT_ID:
            ot_stream_event_eu_control(uvc_dev, req, unit_id, cs, u_req_data);
            break;
        case OT_UVC_EXTENSION_UNIT_ID:
            ot_stream_event_xu_control(uvc_dev, req, unit_id, cs, u_req_data);
            break;
        default:
            uvc_dev->request_error_code.length = 1;
            uvc_dev->request_error_code.data[0] = 0x06;
    }
}

static ot_u16 ot_uvc_get_version(ot_u32 dev_no)
{
    ot_u16 result = 0x0110;
    ot_char char_tmp[128] = {0};
    ot_char file_path[128] = {0};
    (ot_void)snprintf_truncated_s(file_path, sizeof(file_path),
        "/sys/kernel/config/usb_gadget/camera/functions/uvc.usb%u/control/header/h/bcdUVC", dev_no);

    if (read_file(file_path, GET_MAX_PACKET_SIZE_PATH, char_tmp, 128) != 0) { /* 128 bytes used for char_tmp */
        printf("%d, read_file failed!\n", __LINE__);
        return result;
    }
    result = (ot_u16)strtol(char_tmp, NULL, 10); /* 10 base */
    return result;
}

static ot_s32 proc_still_image_request(struct ot_uvc_dev *uvc_dev, ot_u8 req,
    ot_u8 cs, struct uvc_request_data *u_req_data)
{
#ifdef UVC_STILL_IMAGE
    if (is_still_image(cs)) {
        ot_uvc_handle_streaming_still_request(uvc_dev, req, cs, u_req_data);
        return OT_SUCCESS;
    }
#endif

    return -1;
}

static ot_void ot_uvc_handle_streaming_request(struct ot_uvc_dev *uvc_dev,
    ot_u8 req, ot_u8 cs, struct uvc_request_data *u_req_data)
{
    struct ot_uvc_streaming_control u_str_ctrl;
    struct ot_uvc_streaming_control *str_ctl;

    if (proc_still_image_request(uvc_dev, req, cs, u_req_data) == 0)
        return;

    if ((cs != OT_UVC_VS_PROBE_CONTROL) && (cs != OT_UVC_VS_COMMIT_CONTROL)) {
        return;
    }

    u_str_ctrl = *(struct ot_uvc_streaming_control *)(u_req_data->data);
    u_req_data->length = (ot_uvc_get_version(uvc_dev->dev_no) == 0x0150) ? 0x30 : 0x22;
    const struct ot_uvc_format_info *format = &ot_fmt[uvc_dev->probe.b_format_index - 1];
    const struct ot_uvc_frame_info *frame = &format->frames[uvc_dev->probe.b_frame_index - 1];

    switch (req) {
        case OT_UVC_SET_CUR:
            uvc_dev->i_control = cs;
            u_str_ctrl.dw_max_payload_transfer_size =
                uvc_dev->dw_bulk ? uvc_dev->dw_bulk_size : get_max_payload_transfer_size(frame);
            break;
        case OT_UVC_GET_CUR:
            str_ctl = (cs == OT_UVC_VS_PROBE_CONTROL) ? &uvc_dev->probe : &uvc_dev->commit;
            (ot_void)memcpy_s(&u_str_ctrl, sizeof(u_str_ctrl), str_ctl, sizeof(u_str_ctrl));
            u_str_ctrl.dw_max_payload_transfer_size =
                uvc_dev->dw_bulk ? uvc_dev->dw_bulk_size : get_max_payload_transfer_size(frame);
            break;
        case OT_UVC_GET_MIN:
        case OT_UVC_GET_MAX:
        case OT_UVC_GET_DEF:
            ot_uvc_handle_streaming_control(uvc_dev, &u_str_ctrl, uvc_dev->probe.b_frame_index - 1, \
                uvc_dev->probe.b_format_index - 1);
            break;
        case OT_UVC_GET_RES:
            (ot_void)memset_s(u_req_data->data, sizeof(u_req_data->data), 0, sizeof(u_req_data->data));
            return;
        case OT_UVC_GET_LEN:
            u_req_data->data[0] = 0x00;
            u_req_data->data[1] =(ot_uvc_get_version(uvc_dev->dev_no) == 0x0150) ? 0x30 : 0x22;
            u_req_data->length = 0x2;
            return;
        case OT_UVC_GET_INFO:
            u_req_data->data[0] = 0x03;
            u_req_data->length = 1;
            return;
        default:
            return;
    }

    errno_t ret = memcpy_s(u_req_data->data, sizeof(u_req_data->data), &u_str_ctrl, sizeof(u_str_ctrl));
    if (ret != EOK) {
        ERR_INFO("copy back u_str_ctrl to req_data fail %d\n", ret);
    }
}

static ot_void set_probe_status(struct ot_uvc_dev *uvc_dev, ot_s32 cs, ot_s32 req)
{
    if (cs == 0x01) {
        switch (req) {
            case 0x01:
                uvc_dev->probe_status.b_set = 1;
                break;
            case 0x81:
                uvc_dev->probe_status.b_get = 1;
                break;
            case 0x82:
                uvc_dev->probe_status.b_min = 1;
                break;
            case 0x83:
                uvc_dev->probe_status.b_max = 1;
                break;
            case 0x84:
                break;
            case 0x85:
                break;
            case 0x86:
                break;
            default:
                break;
        }
    }
}

static ot_void ot_uvc_handle_class_request(struct ot_uvc_dev *uvc_dev, struct usb_ctrlrequest *u_ctrl_req,
    struct uvc_request_data *u_req_data)
{
    const ot_bool probe_status = OT_TRUE;

    if (probe_status) {
        ot_u8 recip_type = u_ctrl_req->bRequestType & USB_RECIP_MASK;
        switch (recip_type) {
            case USB_RECIP_INTERFACE:
                debug("Request type: USB_RECIP_INTERFACE\n");
                debug("Request: 0x%02x(%s)\n", u_ctrl_req->bRequest, (ot_char *)get_code(u_ctrl_req->bRequest));
                debug("Interface No.  %d\n", (u_ctrl_req->wIndex & 0xff));
                debug("Entity ID: %d\n", ((u_ctrl_req->wIndex & 0xff00) >> RIGHT_SHIFT_8BIT));
                set_probe_status(uvc_dev, (u_ctrl_req->wValue >> RIGHT_SHIFT_8BIT), u_ctrl_req->bRequest);
                break;
            case USB_RECIP_DEVICE:
                debug("request type :DEVICE\n");
                break;
            case USB_RECIP_ENDPOINT:
                debug("request type :ENDPOINT\n");
                break;
            case USB_RECIP_OTHER:
                debug("request type :OTHER\n");
                break;
            default:
                break;
        }
    }

    if ((u_ctrl_req->bRequestType & USB_RECIP_MASK) != USB_RECIP_INTERFACE) {
        return;
    }

    uvc_dev->i_control = (u_ctrl_req->wValue >> RIGHT_SHIFT_8BIT);
    uvc_dev->i_unit_id = ((u_ctrl_req->wIndex & 0xff00) >> RIGHT_SHIFT_8BIT);
    uvc_dev->i_intf_id = (u_ctrl_req->wIndex & 0xff);

    switch (u_ctrl_req->wIndex & 0xff) {
        case OT_UVC_INTF_CONTROL:
        case OT_UVC_INTF_CONTROL2:
            debug("Control Selector ID: 0x%02x\n", (u_ctrl_req->wValue >> RIGHT_SHIFT_8BIT));
            ot_uvc_handle_control_request(uvc_dev, u_ctrl_req->bRequest, u_ctrl_req->wIndex >> RIGHT_SHIFT_8BIT,
                u_ctrl_req->wValue >> RIGHT_SHIFT_8BIT, u_req_data);
            break;
        case OT_UVC_INTF_STREAMING:
        case OT_UVC_INTF_STREAMING2:
            debug("Control Selector ID: 0x%02x(%s)\n", (u_ctrl_req->wValue >> RIGHT_SHIFT_8BIT),
                    (ot_char *)get_intf_cs_string((u_ctrl_req->wValue >> RIGHT_SHIFT_8BIT)));
            ot_uvc_handle_streaming_request(uvc_dev, u_ctrl_req->bRequest, u_ctrl_req->wValue >> RIGHT_SHIFT_8BIT,
                u_req_data);
            break;
        default:
            break;
    }
}

static ot_void do_ot_uvc_setup_event(struct ot_uvc_dev *uvc_dev, struct usb_ctrlrequest *u_ctrl_req,
    struct uvc_request_data *u_req_data)
{
    uvc_dev->i_control = 0;
    uvc_dev->i_unit_id = 0;
    uvc_dev->i_intf_id = 0;

    switch (u_ctrl_req->bRequestType & USB_TYPE_MASK) {
        case USB_TYPE_STANDARD:
            ot_uvc_handle_standard_request(uvc_dev, u_ctrl_req, u_req_data);
            break;
        case USB_TYPE_CLASS:
            ot_uvc_handle_class_request(uvc_dev, u_ctrl_req, u_req_data);
            break;
        default:
            break;
    }
}

static ot_void handle_control_interface_data(struct ot_uvc_dev *uvc_dev, struct uvc_request_data *u_req_data)
{
    switch (uvc_dev->i_unit_id) {
        case OT_UVC_CAMERA_TERMINAL_ID:
            ot_stream_event_ct_data(uvc_dev, uvc_dev->i_unit_id, uvc_dev->i_control, u_req_data);
            break;
        case OT_UVC_PROCESSING_UNIT_ID:
            ot_stream_event_pu_data(uvc_dev, uvc_dev->i_unit_id, uvc_dev->i_control, u_req_data);
            break;
        case OT_UVC_ENCODING_UNIT_ID:
            ot_stream_event_eu_data(uvc_dev, uvc_dev->i_unit_id, uvc_dev->i_control, u_req_data);
            break;
        case OT_UVC_EXTENSION_UNIT_ID:
            ot_stream_event_xu_data(uvc_dev, uvc_dev->i_unit_id, uvc_dev->i_control, u_req_data);
            break;
        default:
            break;
    }
}

static ot_u32 clamp(ot_u32 val, ot_u32 min, ot_u32 max)
{
    ot_u32 res = val;
    if (res < min) {
        return min;
    }
    if (res > max) {
        return max;
    }
    return val;
}

static struct ot_uvc_streaming_control *get_streaming_control(ot_s32 control_type, struct ot_uvc_dev *uvc_dev,
    const struct uvc_request_data *u_req_data)
{
    switch (control_type) {
        case OT_UVC_VS_PROBE_CONTROL:
            debug("setting probe control, length = %d\n", u_req_data->length);
            return &uvc_dev->probe;
        case OT_UVC_VS_COMMIT_CONTROL:
            debug("setting commit control, length = %d\n", u_req_data->length);
            return &uvc_dev->commit;
        default:
            debug("setting unknown control, length = %d\n", u_req_data->length);
            return NULL;
    }
}

static ot_void do_ot_uvc_vs_commit(struct ot_uvc_dev *uvc_dev, struct ot_uvc_streaming_control *u_str_target,
    const struct ot_uvc_format_info *fmt_info, const struct ot_uvc_frame_info *frm_info)
{
    uvc_dev->dw_fcc = fmt_info->fcc;
    uvc_dev->dw_width = frm_info->width;
    uvc_dev->dw_height = frm_info->height;
    uvc_dev->i_fps = FRAME_INTERVAL_CALC_100NS / u_str_target->dw_frame_interval;

    ot_uvc_video_set_fmt(uvc_dev);

    if (uvc_dev->dw_bulk != 0) {
        ot_uvc_video_disable(uvc_dev);
        ot_uvc_video_enable(uvc_dev);
    }
    (ot_void)memset_s(&uvc_dev->probe_status, sizeof(uvc_dev->probe_status), 0, sizeof(uvc_dev->probe_status));
}

static ot_void do_ot_uvc_data_event(struct ot_uvc_dev *uvc_dev, struct uvc_request_data *u_req_data)
{
    struct ot_uvc_streaming_control *u_str_target = NULL;
    struct ot_uvc_streaming_control u_str_ctrl;
    const struct ot_uvc_format_info *fmt_info = NULL;
    const struct ot_uvc_frame_info *frm_info = NULL;
    const ot_u32 *intval = NULL;
    ot_u32 i_fmt, i_frm;
    ot_u32 n_frm = 0, max_frame_size = MAX_PAYLOAD_IMAGE_SIZE;

    if ((uvc_dev->i_unit_id != 0)
        && (uvc_dev->i_intf_id == OT_UVC_INTF_CONTROL || uvc_dev->i_intf_id == OT_UVC_INTF_CONTROL2)) {
        return handle_control_interface_data(uvc_dev, u_req_data);
    }

#ifdef UVC_STILL_IMAGE
    if (is_still_image(uvc_dev->i_control)) {
        return do_ot_uvc_streaming_still_data_event(uvc_dev, u_req_data);
    }
#endif

    if ((u_str_target = get_streaming_control(uvc_dev->i_control, uvc_dev, u_req_data)) == NULL) {
        return;
    }

    ot_s32 ret = memcpy_s(&u_str_ctrl, sizeof(u_str_ctrl), u_req_data->data, u_req_data->length);
    if (ret != EOK) {
        ERR_INFO("memcpy_s u_str_ctrl fail, ret %d\n", ret);
        return;
    }

    i_fmt = clamp((ot_u32)u_str_ctrl.b_format_index, 1U, (ot_u32)array_size(ot_fmt));

    fmt_info = &ot_fmt[i_fmt - 1];

    while (fmt_info->frames[n_frm].width != 0) {
        ++n_frm;
    }

    i_frm    = clamp((ot_u32)u_str_ctrl.b_frame_index, 1U, n_frm);
    frm_info = &fmt_info->frames[i_frm - 1];
    intval   = frm_info->intervals;

    while ((intval[0] < u_str_ctrl.dw_frame_interval) && intval[1]) {
        ++intval;
    }

    u_str_target->b_format_index = (ot_u8)i_fmt;
    u_str_target->b_frame_index  = (ot_u8)i_frm;

    ot_uvc_get_buf_size(uvc_dev, frm_info->width, frm_info->height, fmt_info->fcc, &max_frame_size);
    u_str_target->dw_max_video_frame_size = max_frame_size;
    u_str_target->dw_frame_interval = *intval;

    ERR_INFO("u_str_target->dw_frame_interval: %ld\n", u_str_target->dw_frame_interval);    //2026.10.16 添加的打印

    if (uvc_dev->i_control == OT_UVC_VS_COMMIT_CONTROL) {
        do_ot_uvc_vs_commit(uvc_dev, u_str_target, fmt_info, frm_info);
    }
}

static ot_void do_ot_uvc_event(struct ot_uvc_dev *uvc_dev)
{
    struct video_event v_event;
    struct ot_uvc_event *ot_eve = (struct ot_uvc_event *)(ot_void *)&v_event.u.a_data;
    struct uvc_request_data u_req_data;
    ot_s32 ret;

    debug("#############do_ot_uvc_event()#############\n");

    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_DQUEUE_EVENT, &v_event);
    if (ret < 0) {
        ERR_INFO("VIDEO_IOCTL_DQUEUE_EVENT failed: %d\n", errno);
        return;
    }

    (ot_void)memset_s(&u_req_data, sizeof(u_req_data), 0, sizeof(u_req_data));
    u_req_data.length = 32; /* 32 byte data length */

    switch (v_event.dw_type) {
        case OT_UVC_EVE_CON:
            ot_uvc_handle_streaming_control(uvc_dev, &uvc_dev->probe, 0, 0);
            ot_uvc_handle_streaming_control(uvc_dev, &uvc_dev->commit, 0, 0);
#ifdef UVC_STILL_IMAGE
            ot_uvc_handle_streaming_still_control(uvc_dev, &uvc_dev->probe_still, 0, 0);
            ot_uvc_handle_streaming_still_control(uvc_dev, &uvc_dev->commit_still, 0, 0);
#endif
            return;
        case OT_UVC_EVE_DISCON: //修改 2026.10.30
            printf("4440001     ############################\n");
            if (uvc_dev->dw_bulk == 0) {
                ot_uvc_video_disable(uvc_dev);
            }
            return;
        case OT_UVC_EVE_SETTING:
            printf("4440002     ############################\n");
            do_ot_uvc_setup_event(uvc_dev, &ot_eve->req, &u_req_data);
            break;
        case OT_UVC_EVE_DATA:
            printf("4440003     ############################\n");
            do_ot_uvc_data_event(uvc_dev, &ot_eve->data);
            return;
        case OT_UVC_EVE_STRON:
            printf("4440004     ############################\n");
            printf("###RAWDBG### event STREAMON dev=%u bulk=%u\n",
                   uvc_dev->dev_no, (unsigned)uvc_dev->dw_bulk);
            if (uvc_dev->dw_bulk != 1) {
                ot_uvc_video_enable(uvc_dev);
            }
            return;
        case OT_UVC_EVE_STROFF:
            printf("4440005     ############################\n");
            ot_uvc_video_disable(uvc_dev);
            return;
        default:
            break;
    }

    ret = ioctl(uvc_dev->i_fd, OT_UVC_IOC_SEND_RESPONSE, &u_req_data);
    if (ret < 0) {
        ERR_INFO("OT_UVC_IOC_S_EVENT failed: %d\n", errno);
    }
}

static ot_void ot_uvc_event_register(struct ot_uvc_dev *uvc_dev)
{
    struct video_event_descriptor v_event_desc;
    ot_s32 ret;

    (ot_void)memset_s(&v_event_desc, sizeof(v_event_desc), 0, sizeof(v_event_desc));

    v_event_desc.dw_type = OT_UVC_EVE_CON;
    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_DESC_EVENT, &v_event_desc);
    if (ret < 0) {
        ERR_INFO("Connect event failed: %d\n", errno);
    }

    v_event_desc.dw_type = OT_UVC_EVE_DISCON;
    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_DESC_EVENT, &v_event_desc);
    if (ret < 0) {
        ERR_INFO("Disconnect event failed: %d\n", errno);
    }

    v_event_desc.dw_type = OT_UVC_EVE_SETTING;
    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_DESC_EVENT, &v_event_desc);
    if (ret < 0) {
        ERR_INFO("Setup event failed: %d\n", errno);
    }

    v_event_desc.dw_type = OT_UVC_EVE_DATA;
    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_DESC_EVENT, &v_event_desc);
    if (ret < 0) {
        ERR_INFO("Data event failed: %d\n", errno);
    }

    v_event_desc.dw_type = OT_UVC_EVE_STRON;
    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_DESC_EVENT, &v_event_desc);
    if (ret < 0) {
        ERR_INFO("StreamOn event failed: %d\n", errno);
    }

    v_event_desc.dw_type = OT_UVC_EVE_STROFF;
    ret = ioctl(uvc_dev->i_fd, VIDEO_IOCTL_DESC_EVENT, &v_event_desc);
    if (ret < 0) {
        ERR_INFO("StreamOff event failed: %d\n", errno);
    }
}

static ot_void get_bulk_mode(ot_u32 *bulk_mode, ot_s32 dev_no)
{
    if (bulk_mode == NULL)
        return;

    ot_char char_tmp[128] = {0};
    ot_char file_path[128] = {0};
    *bulk_mode = 0;

    (ot_void)snprintf_truncated_s(file_path, sizeof(file_path),
        "/sys/kernel/config/usb_gadget/camera/functions/uvc.usb%d/streaming_transfer", dev_no);

    if (read_file(file_path, GET_MAX_PACKET_SIZE_PATH, char_tmp, 128) != 0) { /* 128 bytes used for char_tmp */
        printf("%d, read_file failed!\n", __LINE__);
        return;
    }
    printf("%s, %s \n", __func__, char_tmp);
    if (strcmp(char_tmp, "bulk") == 0) {
        *bulk_mode = 1;
    }
    return;
}

ot_s32 open_uvc_device(const ot_char *dev_path, ot_u32 dev_no)
{
    struct ot_uvc_dev *uvc_dev;

    uvc_dev = ot_uvc_init(dev_path, dev_no);
    if (uvc_dev == 0) {
        return -1;
    }

    uvc_dev->uvc_cache = uvc_cache_get(dev_no);

    uvc_dev->dw_img_size = MAX_PAYLOAD_IMAGE_SIZE; /* 2160*3840*2 */

    get_bulk_mode(&uvc_dev->dw_bulk, dev_no);
    if (uvc_dev->dw_bulk == 1) {
        uvc_dev->dw_bulk_size = ot_get_max_payload_transfer_size_for_bulk(uvc_dev->dev_no);
    }

    debug("set max-imagesize = %d, set bulkmode =%d, set bulksize = %d\n", uvc_dev->dw_img_size, uvc_dev->dw_bulk,
        uvc_dev->dw_bulk_size);

    ot_uvc_event_register(uvc_dev);
    g_ot_cd[dev_no] = uvc_dev;

    return 0;
}

ot_s32 close_uvc_device(ot_u32 dev_no)
{
    if (g_ot_cd[dev_no] != 0) {
        ot_uvc_video_disable(g_ot_cd[dev_no]);
        close(g_ot_cd[dev_no]->i_fd);
        free(g_ot_cd[dev_no]);
    }

    g_ot_cd[dev_no] = NULL;
    return 0;
}

ot_s32 run_uvc_data(ot_u32 dev_no)
{
    fd_set w_fds;
    ot_s32 ret = 0;
    struct timeval t_val;

    if (g_ot_cd[dev_no] == NULL) {
        return -1;
    }
    if (g_ot_cd[dev_no]->i_streaming == 1) {
        ret = ot_uvc_video_process_user_ptr(g_ot_cd[dev_no]);

        /* RAWDBG: 每秒汇总一次 —— 仅当启动参数带有 st 时开启。
         * 计数为每秒增量(get_raw 每秒应≈fps); 定位节点/槽卡死。 */
        if (uvc_get_rawdbg_enable()) {
            struct timespec ts_now;
            struct ot_uvc_dev *d = g_ot_cd[dev_no];
            uvc_cache_t *c = d->uvc_cache;
            static long last_sec[MAX_UVC_CACHE_NUMBER] = {0};
            const unsigned int interval = 1;
            clock_gettime(CLOCK_MONOTONIC, &ts_now);
            if (c != NULL && dev_no < MAX_UVC_CACHE_NUMBER &&
                (ts_now.tv_sec - last_sec[dev_no]) >= (long)interval) {
                unsigned int nb = d->dw_nbufs, si;
                char slot[MAX_SAVE_FRAME_NUMS + 1];
                last_sec[dev_no] = ts_now.tv_sec;
                if (nb > MAX_SAVE_FRAME_NUMS) {
                    nb = MAX_SAVE_FRAME_NUMS;
                }
                for (si = 0; si < nb; si++) {
                    slot[si] = d->frame_node[si] ? '1' : '0';
                }
                slot[si] = '\0';
                printf("###RAWDBG### +%us d%u: get_raw=%u unpack=%u free_drop=%u | "
                       "dqbuf=%u qbuf=%u ok_empty=%u | free=%u ok=%u nbufs=%u slot[%s]\n",
                       interval, (unsigned)dev_no, c->dbg_get_raw, c->dbg_unpack, c->dbg_free_drop,
                       c->dbg_dqbuf, c->dbg_qbuf, c->dbg_ok_empty,
                       uvc_queue_count(c->free_queue), uvc_queue_count(c->ok_queue),
                       (unsigned)d->dw_nbufs, slot);
                c->dbg_get_raw = c->dbg_unpack = c->dbg_free_drop = 0;
                c->dbg_dqbuf = c->dbg_qbuf = c->dbg_ok_empty = 0;
            }
        }
    }
    else {
        usleep(2000); /* 2 ms */
    }

    return ret;
}

ot_s32 run_uvc_device(ot_u32 dev_no)
{
    fd_set e_fds;
    ot_s32 ret;
    struct timeval t_val;

    if (g_ot_cd[dev_no] == NULL) {
        return -1;
    }

    t_val.tv_sec = 1;
    t_val.tv_usec = 0;

    FD_ZERO(&e_fds);
    FD_SET(g_ot_cd[dev_no]->i_fd, &e_fds);

    ret = select(g_ot_cd[dev_no]->i_fd + 1, NULL, NULL, &e_fds, &t_val);
    if (ret > 0) {
        if (FD_ISSET(g_ot_cd[dev_no]->i_fd, &e_fds)) {
            do_ot_uvc_event(g_ot_cd[dev_no]);
        }
    }

    return ret;
}

/* -------------------------------------------------------------------- */
struct ot_uvc_dev *get_uvc_device(ot_u32 dev_no)
{
    if (dev_no >= OT_UVC_MAX_CHN_NUM) {
        return NULL;
    }
    return g_ot_cd[dev_no];
}