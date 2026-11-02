/* Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "ot_eis_errno.h"

#ifdef OT_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "ot_eis_i2c.h"
#endif

#include "gc2093_cmos.h"
#include "ot_eis_vi_api.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

static int g_fd[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = -1};

int gc2093_i2c_init(ot_vi_pipe_id vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        return OT_SUCCESS;
    }
#ifdef OT_GPIO_I2C
    g_fd[vi_pipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[vi_pipe] < 0) {
        sns_err_trace("Open gpioi2c_ex error!\n");
        return OT_FAILURE;
    }
#else
    int ret;
    char dev_file[I2C_DEV_FILE_NUM] = {0};
    ot_u8 dev_num;
    ot_eis_sns_bus_info *gc2093businfo = gc2093_get_bus_info(vi_pipe);
    dev_num = gc2093businfo->com_bus.i2c_dev;
    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        sns_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return OT_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_EIS_I2C_SLAVE_FORCE, (gc2093businfo->bus_addr >> 1));
    if (ret < 0) {
        sns_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return OT_SUCCESS;
}

int gc2093_i2c_exit(ot_vi_pipe_id vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return OT_SUCCESS;
    }
    return OT_FAILURE;
}

static void delay_ms(int ms);
ot_s32 gc2093_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }
    delay_ms(1);

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = GC2093_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = GC2093_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = GC2093_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        sns_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];

    if (GC2093_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (GC2093_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ot_eis_sns_bus_info *gc2093businfo = gc2093_get_bus_info(vi_pipe);
    ret = write(g_fd[vi_pipe], buf, GC2093_ADDR_BYTE + GC2093_DATA_BYTE);
    if (ret < 0) {
        sns_err_trace("i2c%x bus_id 0x%02x write 0x%04x data 0x%02x failed!\n",
            gc2093businfo->com_bus.i2c_dev, gc2093businfo->bus_addr, addr, data);
        return OT_FAILURE;
    }

#endif
    return OT_SUCCESS;
}

ot_s32 gc2093_read_register(ot_vi_pipe_id vi_pipe, ot_u16 addr, ot_u32 *data)
{
    ot_s32 ret;
    ot_u32 idx = 0;
    ot_u8 buf[I2C_BUF_NUM] = {0};
    ot_eis_sns_bus_info *bus_info = gc2093_get_bus_info(vi_pipe);

    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

    buf[idx++]         = ((addr >> 8) & 0xff);
    buf[idx++]         = ((addr >> 0) & 0xff);
    ret = write(g_fd[vi_pipe], buf, GC2093_ADDR_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    ret = read(g_fd[vi_pipe], buf, GC2093_DATA_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    *data = buf[0];
    return OT_SUCCESS;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000); /* 1ms: 1000us */
    return;
}

void gc2093_prog(ot_vi_pipe_id vi_pipe, const ot_u32 *rom)
{
    return;
}

void gc2093_standby(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

void gc2093_restart(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

ot_void gc2093_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp)
{
    ot_s32 ret = OT_SUCCESS;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    master_pipe_id = vi_pipe[0];

    gc2093_set_blc_clamp_value(master_pipe_id, blc_clamp.blc_clamp_en);

//    if (blc_clamp.blc_clamp_en == OT_TRUE) {
//        ret += gc2093_write_register(vi_pipe, 0x4001, 0xeb);  /* clamp on */
//    } else {
//        ret += gc2093_write_register(vi_pipe, 0x4001, 0xea);  /* clamp off */
//    }

    if (ret != OT_SUCCESS) {
        sns_err_trace("write register failed!\n");
    }
    return;
}

static void gc2093_linear_2m30_10bit_init(ot_vi_pipe_id vi_pipe);
static void gc2093_vc_wdr_2to1_2m30_10bit_init(ot_vi_pipe_id vi_pipe);

static void gc2093_default_reg_init(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
    ot_s32 ret = OT_SUCCESS;
    ot_eis_sns_state *pastgc2093 = OT_NULL;

    pastgc2093 = gc2093_get_ctx(vi_pipe);
    for (i = 0; i < pastgc2093->regs_info[0].reg_num; i++) {
        ret += gc2093_write_register(vi_pipe,
            pastgc2093->regs_info[0].i2c_data[i].reg_addr,
            pastgc2093->regs_info[0].i2c_data[i].data);
    }

    if (ret != OT_SUCCESS) {
        sns_err_trace("write register failed!\n");
    }

    return;
}

static ot_bool gc2093_get_quickboot_flag(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_vi_pipe_handle pipe_hdl;
    ot_eis_vi_quickboot_attr quickboot_attr = { 0 };

    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(vi_pipe, &pipe_hdl);
    if (ret != OT_SUCCESS) {
        sns_err_trace("get pipe_hdl failed!\n");
        return OT_FALSE;
    }

    ret = ot_eis_vi_pipe_get_quickboot_attr(pipe_hdl, &quickboot_attr);
    if (ret != OT_SUCCESS) {
        sns_err_trace("get quickboot attr failed!\n");
        return OT_FALSE;
    }

    return quickboot_attr.enable;
}

#define GC2093_LOOP_CNT 200
static ot_bool gc2093_check_exist(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_u32 data = 0;

    for (int i = 0; i < GC2093_LOOP_CNT; i++) {
        ret = gc2093_read_register(vi_pipe, 0x003e, &data);
        if (ret == OT_SUCCESS) {
            return OT_TRUE;
        }
        usleep(1000); // 1ms, total 200ms
    }
    sns_err_trace("gc2093 pipe%d don't exist\n", vi_pipe);
    return OT_FALSE;
}

void gc2093_init(ot_vi_pipe_id vi_pipe)
{
    ot_bool          quickboot_en;
    ot_eis_wdr_mode  wdr_mode;
    ot_u8            img_mode;
    ot_s32           ret;
    ot_eis_sns_state *pastgc2093 = OT_NULL;

    pastgc2093 = gc2093_get_ctx(vi_pipe);
    wdr_mode    = pastgc2093->wdr_mode;
    img_mode    = pastgc2093->img_mode;

    ret = gc2093_i2c_init(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("i2c init failed!\n");
        return;
    }

    quickboot_en = gc2093_get_quickboot_flag(vi_pipe);

    if (!quickboot_en && (pastgc2093->init == OT_FALSE) && (gc2093_check_exist(vi_pipe) == OT_TRUE)) {
        if (OT_EIS_WDR_MODE_2To1_LINE == wdr_mode) {
            if (GC2093_2M_30FPS_10BIT_2TO1_VC_MODE == img_mode) {  /* GC2093_VMAX_2M_30FPS_10BIT_2TO1_WDR */
                gc2093_vc_wdr_2to1_2m30_10bit_init(vi_pipe);
            } else {
                sns_err_trace("Invalid image mode!\n");
                return;
            }
        } else if (OT_EIS_WDR_MODE_NONE == wdr_mode) {
            gc2093_linear_2m30_10bit_init(vi_pipe);
        } else {
            sns_err_trace("Invalid sensor mode!\n");
            return;
        }
    }

    pastgc2093->init = OT_TRUE;
    return;
}

void gc2093_exit(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_eis_sns_state *pastgc2093 = OT_NULL;

    ret = gc2093_i2c_exit(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("GC2093 exit failed!\n");
    }

    pastgc2093 = gc2093_get_ctx(vi_pipe);
    pastgc2093->init = OT_FALSE;
    return;
}

static void gc2093_linear_2m30_10bit_init(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret = OT_SUCCESS;

    /****system****/
    ret += gc2093_write_register(vi_pipe, 0x03fe, 0xf0);
    ret += gc2093_write_register(vi_pipe, 0x03fe, 0xf0);
    ret += gc2093_write_register(vi_pipe, 0x03fe, 0xf0);
    ret += gc2093_write_register(vi_pipe, 0x03fe, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x03f2, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x03f3, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x03f4, 0x36);
    ret += gc2093_write_register(vi_pipe, 0x03f5, 0xc0);
    ret += gc2093_write_register(vi_pipe, 0x03f6, 0x0B);
    ret += gc2093_write_register(vi_pipe, 0x03f7, 0x11);
    ret += gc2093_write_register(vi_pipe, 0x03f8, 0x36);
    ret += gc2093_write_register(vi_pipe, 0x03f9, 0x42);
    ret += gc2093_write_register(vi_pipe, 0x03fc, 0x8e);
    /****CISCTL & ANALOG****/
    ret += gc2093_write_register(vi_pipe, 0x0087, 0x18);
    ret += gc2093_write_register(vi_pipe, 0x00ee, 0x30);
    ret += gc2093_write_register(vi_pipe, 0x00d0, 0xbf);
    ret += gc2093_write_register(vi_pipe, 0x01a0, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x01a4, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x01a5, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x01a6, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x01af, 0x09);
    ret += gc2093_write_register(vi_pipe, 0x0003, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0004, 0x65);
    ret += gc2093_write_register(vi_pipe, 0x0005, 0x05);
    ret += gc2093_write_register(vi_pipe, 0x0006, 0xa0);
    ret += gc2093_write_register(vi_pipe, 0x0007, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0008, 0x11);
    ret += gc2093_write_register(vi_pipe, 0x0009, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x000a, 0x02);
    ret += gc2093_write_register(vi_pipe, 0x000b, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x000c, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x000d, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x000e, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x000f, 0x07);
    ret += gc2093_write_register(vi_pipe, 0x0010, 0x8c);
    ret += gc2093_write_register(vi_pipe, 0x0013, 0x15);
    ret += gc2093_write_register(vi_pipe, 0x0019, 0x0c);
    ret += gc2093_write_register(vi_pipe, 0x0041, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0042, 0xe2);
    ret += gc2093_write_register(vi_pipe, 0x0053, 0x60);
    ret += gc2093_write_register(vi_pipe, 0x008d, 0x92);
    ret += gc2093_write_register(vi_pipe, 0x0090, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x00c7, 0xe1);
    ret += gc2093_write_register(vi_pipe, 0x001b, 0x73);
    ret += gc2093_write_register(vi_pipe, 0x0028, 0x0d);
    ret += gc2093_write_register(vi_pipe, 0x0029, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x002b, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x002e, 0x23);
    ret += gc2093_write_register(vi_pipe, 0x0037, 0x03);
    ret += gc2093_write_register(vi_pipe, 0x0043, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0044, 0x30);
    ret += gc2093_write_register(vi_pipe, 0x004a, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x004b, 0x28);
    ret += gc2093_write_register(vi_pipe, 0x0055, 0x30);
    ret += gc2093_write_register(vi_pipe, 0x0066, 0x3f);
    ret += gc2093_write_register(vi_pipe, 0x0068, 0x3f);
    ret += gc2093_write_register(vi_pipe, 0x006b, 0x44);
    ret += gc2093_write_register(vi_pipe, 0x0077, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0078, 0x20);
    ret += gc2093_write_register(vi_pipe, 0x007c, 0xa1);
    ret += gc2093_write_register(vi_pipe, 0x00ce, 0x7c);
    ret += gc2093_write_register(vi_pipe, 0x00d3, 0xd4);
    ret += gc2093_write_register(vi_pipe, 0x00e6, 0x50);
    /*gain*/
    ret += gc2093_write_register(vi_pipe, 0x00b6, 0xc0);
    ret += gc2093_write_register(vi_pipe, 0x00b0, 0x68);
    ret += gc2093_write_register(vi_pipe, 0x00b3, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x00b8, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x00b9, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x00b1, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x00b2, 0x00);
    /*isp*/
    ret += gc2093_write_register(vi_pipe, 0x0101, 0x0c);
    ret += gc2093_write_register(vi_pipe, 0x0102, 0x89);
    ret += gc2093_write_register(vi_pipe, 0x0104, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x0107, 0xa6);
    ret += gc2093_write_register(vi_pipe, 0x0108, 0xa9);
    ret += gc2093_write_register(vi_pipe, 0x0109, 0xa8);
    ret += gc2093_write_register(vi_pipe, 0x010a, 0xa7);
    ret += gc2093_write_register(vi_pipe, 0x010b, 0xff);
    ret += gc2093_write_register(vi_pipe, 0x010c, 0xff);
    ret += gc2093_write_register(vi_pipe, 0x010f, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0158, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0428, 0x86);
    ret += gc2093_write_register(vi_pipe, 0x0429, 0x86);
    ret += gc2093_write_register(vi_pipe, 0x042a, 0x86);
    ret += gc2093_write_register(vi_pipe, 0x042b, 0x68);
    ret += gc2093_write_register(vi_pipe, 0x042c, 0x68);
    ret += gc2093_write_register(vi_pipe, 0x042d, 0x68);
    ret += gc2093_write_register(vi_pipe, 0x042e, 0x68);
    ret += gc2093_write_register(vi_pipe, 0x042f, 0x68);
    ret += gc2093_write_register(vi_pipe, 0x0430, 0x4f);
    ret += gc2093_write_register(vi_pipe, 0x0431, 0x68);
    ret += gc2093_write_register(vi_pipe, 0x0432, 0x67);
    ret += gc2093_write_register(vi_pipe, 0x0433, 0x66);
    ret += gc2093_write_register(vi_pipe, 0x0434, 0x66);
    ret += gc2093_write_register(vi_pipe, 0x0435, 0x66);
    ret += gc2093_write_register(vi_pipe, 0x0436, 0x66);
    ret += gc2093_write_register(vi_pipe, 0x0437, 0x66);
    ret += gc2093_write_register(vi_pipe, 0x0438, 0x62);
    ret += gc2093_write_register(vi_pipe, 0x0439, 0x62);
    ret += gc2093_write_register(vi_pipe, 0x043a, 0x62);
    ret += gc2093_write_register(vi_pipe, 0x043b, 0x62);
    ret += gc2093_write_register(vi_pipe, 0x043c, 0x62);
    ret += gc2093_write_register(vi_pipe, 0x043d, 0x62);
    ret += gc2093_write_register(vi_pipe, 0x043e, 0x62);
    ret += gc2093_write_register(vi_pipe, 0x043f, 0x62);
    /*dark sun*/
    ret += gc2093_write_register(vi_pipe, 0x0123, 0x08);
    ret += gc2093_write_register(vi_pipe, 0x0123, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0120, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x0121, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0122, 0x65);
    ret += gc2093_write_register(vi_pipe, 0x0124, 0x03);
    ret += gc2093_write_register(vi_pipe, 0x0125, 0xff);
    ret += gc2093_write_register(vi_pipe, 0x001a, 0x8c);
    ret += gc2093_write_register(vi_pipe, 0x00c6, 0xe0);
    /*blk*/
    ret += gc2093_write_register(vi_pipe, 0x0026, 0x30);
    ret += gc2093_write_register(vi_pipe, 0x0142, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0149, 0x1e);
    ret += gc2093_write_register(vi_pipe, 0x014a, 0x0f);
    ret += gc2093_write_register(vi_pipe, 0x014b, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0155, 0x07);
    ret += gc2093_write_register(vi_pipe, 0x0414, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0415, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0416, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0417, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x04e0, 0x18);
    /*window*/
    ret += gc2093_write_register(vi_pipe, 0x0192, 0x02);
    ret += gc2093_write_register(vi_pipe, 0x0194, 0x03);
    ret += gc2093_write_register(vi_pipe, 0x0195, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0196, 0x38);
    ret += gc2093_write_register(vi_pipe, 0x0197, 0x07);
    ret += gc2093_write_register(vi_pipe, 0x0198, 0x80);
    /****DVP & MIPI****/
    ret += gc2093_write_register(vi_pipe, 0x019a, 0x06);
    ret += gc2093_write_register(vi_pipe, 0x007b, 0x2a);
    ret += gc2093_write_register(vi_pipe, 0x0023, 0x2d);
    ret += gc2093_write_register(vi_pipe, 0x0201, 0x27);
    ret += gc2093_write_register(vi_pipe, 0x0202, 0x56);
    ret += gc2093_write_register(vi_pipe, 0x0203, 0x8e);
    ret += gc2093_write_register(vi_pipe, 0x0212, 0x80);
    ret += gc2093_write_register(vi_pipe, 0x0213, 0x07);
    ret += gc2093_write_register(vi_pipe, 0x0215, 0x12);
    ret += gc2093_write_register(vi_pipe, 0x003e, 0x91);

    printf("======================================================================\n");
    printf("== vi_pipe:%d gc2093 24MClk 2M30fps(MIPI) 10bit linear init success! ==\n", vi_pipe);
    printf("======================================================================\n");
}

static void gc2093_vc_wdr_2to1_2m30_10bit_init(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret = OT_SUCCESS;

    ret += gc2093_write_register(vi_pipe, 0x03fe, 0xf0);
    ret += gc2093_write_register(vi_pipe, 0x03fe, 0xf0);
    ret += gc2093_write_register(vi_pipe, 0x03fe, 0xf0);
    ret += gc2093_write_register(vi_pipe, 0x03fe, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x03f2, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x03f3, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x03f4, 0x36);
    ret += gc2093_write_register(vi_pipe, 0x03f5, 0xc0);
    ret += gc2093_write_register(vi_pipe, 0x03f6, 0x0B);
    ret += gc2093_write_register(vi_pipe, 0x03f7, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x03f8, 0x63);
    ret += gc2093_write_register(vi_pipe, 0x03f9, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x03fc, 0x8e);
    /**** CISCTL & ANALOG ****/
    ret += gc2093_write_register(vi_pipe, 0x0087, 0x18);
    ret += gc2093_write_register(vi_pipe, 0x00ee, 0x30);
    ret += gc2093_write_register(vi_pipe, 0x00d0, 0xbf);
    ret += gc2093_write_register(vi_pipe, 0x01a0, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x01a4, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x01a5, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x01a6, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x01af, 0x09);
    ret += gc2093_write_register(vi_pipe, 0x0001, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0002, 0x02);
    ret += gc2093_write_register(vi_pipe, 0x0003, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0004, 0x02);
    ret += gc2093_write_register(vi_pipe, 0x0005, 0x02);
    ret += gc2093_write_register(vi_pipe, 0x0006, 0x94);
    ret += gc2093_write_register(vi_pipe, 0x0007, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0008, 0x11);
    ret += gc2093_write_register(vi_pipe, 0x0009, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x000a, 0x02);
    ret += gc2093_write_register(vi_pipe, 0x000b, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x000c, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x000d, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x000e, 0x40);
    ret += gc2093_write_register(vi_pipe, 0x000f, 0x07);
    ret += gc2093_write_register(vi_pipe, 0x0010, 0x8c);
    ret += gc2093_write_register(vi_pipe, 0x0013, 0x15);
    ret += gc2093_write_register(vi_pipe, 0x0019, 0x0c);
    ret += gc2093_write_register(vi_pipe, 0x0041, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0042, 0xE2);
    ret += gc2093_write_register(vi_pipe, 0x0053, 0x60);
    ret += gc2093_write_register(vi_pipe, 0x008d, 0x92);
    ret += gc2093_write_register(vi_pipe, 0x0090, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x00c7, 0xe1);
    ret += gc2093_write_register(vi_pipe, 0x001b, 0x73);
    ret += gc2093_write_register(vi_pipe, 0x0028, 0x0d);
    ret += gc2093_write_register(vi_pipe, 0x0029, 0x24);
    ret += gc2093_write_register(vi_pipe, 0x002b, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x002e, 0x23);
    ret += gc2093_write_register(vi_pipe, 0x0037, 0x03);
    ret += gc2093_write_register(vi_pipe, 0x0043, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0044, 0x28);
    ret += gc2093_write_register(vi_pipe, 0x004a, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x004b, 0x20);
    ret += gc2093_write_register(vi_pipe, 0x0055, 0x28);
    ret += gc2093_write_register(vi_pipe, 0x0066, 0x3f);
    ret += gc2093_write_register(vi_pipe, 0x0068, 0x3f);
    ret += gc2093_write_register(vi_pipe, 0x006b, 0x44);
    ret += gc2093_write_register(vi_pipe, 0x0077, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0078, 0x20);
    ret += gc2093_write_register(vi_pipe, 0x007c, 0xa1);
    ret += gc2093_write_register(vi_pipe, 0x00ce, 0x7c);
    ret += gc2093_write_register(vi_pipe, 0x00d3, 0xd4);
    ret += gc2093_write_register(vi_pipe, 0x00e6, 0x50);
    /* gain */
    ret += gc2093_write_register(vi_pipe, 0x00b6, 0xc0);
    ret += gc2093_write_register(vi_pipe, 0x00b0, 0x68);
    /* isp */
    ret += gc2093_write_register(vi_pipe, 0x0101, 0x0c);
    ret += gc2093_write_register(vi_pipe, 0x0102, 0x89);
    ret += gc2093_write_register(vi_pipe, 0x0104, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x010e, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x010f, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0158, 0x00);
    /* dark sun */
    ret += gc2093_write_register(vi_pipe, 0x0123, 0x08);
    ret += gc2093_write_register(vi_pipe, 0x0123, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0120, 0x01);
    ret += gc2093_write_register(vi_pipe, 0x0121, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0122, 0xd8);
    ret += gc2093_write_register(vi_pipe, 0x0124, 0x03);
    ret += gc2093_write_register(vi_pipe, 0x0125, 0xff);
    ret += gc2093_write_register(vi_pipe, 0x001a, 0x8c);
    ret += gc2093_write_register(vi_pipe, 0x00c6, 0xe0);
    /* blk */
    ret += gc2093_write_register(vi_pipe, 0x0026, 0x30);
    ret += gc2093_write_register(vi_pipe, 0x0142, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0149, 0x1e);
    ret += gc2093_write_register(vi_pipe, 0x014a, 0x0f);
    ret += gc2093_write_register(vi_pipe, 0x014b, 0x00);
    ret += gc2093_write_register(vi_pipe, 0x0155, 0x07);
    ret += gc2093_write_register(vi_pipe, 0x0414, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0415, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0416, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0417, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0454, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0455, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0456, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x0457, 0x78);
    ret += gc2093_write_register(vi_pipe, 0x04e0, 0x18);
    /* window */
    ret += gc2093_write_register(vi_pipe, 0x0192, 0x02);
    ret += gc2093_write_register(vi_pipe, 0x0194, 0x03);
    ret += gc2093_write_register(vi_pipe, 0x0195, 0x04);
    ret += gc2093_write_register(vi_pipe, 0x0196, 0x38);
    ret += gc2093_write_register(vi_pipe, 0x0197, 0x07);
    ret += gc2093_write_register(vi_pipe, 0x0198, 0x80);
    /**** DVP & MIPI ****/
    ret += gc2093_write_register(vi_pipe, 0x019a, 0x06);
    ret += gc2093_write_register(vi_pipe, 0x007b, 0x2a);
    ret += gc2093_write_register(vi_pipe, 0x0023, 0x2d);
    ret += gc2093_write_register(vi_pipe, 0x0201, 0x27);
    ret += gc2093_write_register(vi_pipe, 0x0202, 0x56);
    ret += gc2093_write_register(vi_pipe, 0x0203, 0xce); // try 0xce or 0x8e
    ret += gc2093_write_register(vi_pipe, 0x0212, 0x80);
    ret += gc2093_write_register(vi_pipe, 0x0213, 0x07);
    ret += gc2093_write_register(vi_pipe, 0x0215, 0x10);
    ret += gc2093_write_register(vi_pipe, 0x003e, 0x91);
    /**** HDR EN ****/
    ret += gc2093_write_register(vi_pipe, 0x0027, 0x71);
    ret += gc2093_write_register(vi_pipe, 0x0215, 0x92);
    ret += gc2093_write_register(vi_pipe, 0x024d, 0x01);

    gc2093_default_reg_init(vi_pipe);

    if (ret != OT_SUCCESS) {
        sns_err_trace("gc2093 write register failed!\n");
        return;
    }

    printf("===========================================================================\n");
    printf("== vi_pipe:%d gc2093 24MClk 2M30fps(MIPI) 10bit vc-wdr 2to1 init success! ==\n", vi_pipe);
    printf("===========================================================================\n");
}
