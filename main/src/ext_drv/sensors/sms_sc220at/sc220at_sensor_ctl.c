/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
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

#include "sc220at_cmos.h"
#include "ot_eis_vi_api.h"
#include "sc220at_30fps_raw12.h"
#include "sc220at_30fps_yuv422.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
#define I2C_BUS_ID_DEF       -1

static int g_fd[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = -1};

int sc220at_i2c_init(ot_vi_pipe_id vi_pipe)
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
    ot_eis_sns_bus_info *sc220at_businfo = OT_NULL;
    sc220at_businfo = sc220at_get_bus_info(vi_pipe);
    dev_num = sc220at_businfo->com_bus.i2c_dev;
    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        sns_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return OT_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_EIS_I2C_SLAVE_FORCE, (sc220at_businfo->bus_addr >> 1));
    if (ret < 0) {
        sns_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return OT_SUCCESS;
}


int sc220at_i2c_exit(ot_vi_pipe_id vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return OT_SUCCESS;
    }
    return OT_FAILURE;
}

ot_s32 sc220at_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = SC220AT_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = SC220AR_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = SC220AT_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        sns_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];

    if (SC220AT_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (addr >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = addr & 0xff;
    } else {
        buf[idx++] = addr & 0xff;
    }

    if (I2C_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (data >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = data & 0xff;
    } else {
        buf[idx++] = data & 0xff;
    }

    ot_eis_sns_bus_info *bus_info = sc220at_get_bus_info(vi_pipe);
    ret = write(g_fd[vi_pipe], buf, SC220AT_ADDR_BYTE + SC220AT_DATA_BYTE);
    if (ret < 0) {
        sns_err_trace("i2c%d devaddr 0x%x write 0x%x data 0x%x failed!\n",
            bus_info->com_bus.i2c_dev, bus_info->bus_addr, addr, data);
        return OT_FAILURE;
    }

#endif
    return OT_SUCCESS;
}

ot_s32 sc220at_read_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 *data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];
    ot_u8 buf_read[I2C_BUF_NUM];

    if (SC220AT_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (addr >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = addr & 0xff;
    } else {
        buf[idx++] = addr & 0xff;
    }

    ret = write(g_fd[vi_pipe], buf, SC220AT_ADDR_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    ret = read(g_fd[vi_pipe], buf_read, SC220AT_DATA_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    if (SC220AT_DATA_BYTE == 2) {  /* 2 byte */
        ot_u32 high = buf_read[0];
        ot_u32 low = buf_read[1];
        *data = (high << 8) + low;
    } else {
        *data = buf_read[0];
    }

    return OT_SUCCESS;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000); /* 1ms: 1000us */
}

void sc220at_prog(ot_vi_pipe_id vi_pipe, const ot_u32 *rom)
{
    return;
}

void sc220at_standby(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

void sc220at_restart(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

static void sc220at_fme(ot_vi_pipe_id vi_pipe)              //for sensor fme ae_mode      in sensor_init:ox03f_init
{
    ot_u32 i;
    if (sc220at_get_ext_sensor_flag(vi_pipe) == 1) {
        ot_eis_sns_reg_table *sns_table = sc220at_get_ext_sensor_table(vi_pipe);
        ot_eis_sns_reg_table  *fme_table = sns_table + 4;

        for (i = 0; i < fme_table->data_len; i++) {
            sc220at_write_register(vi_pipe, (fme_table->reg_data + i)->addr, (fme_table->reg_data + i)->data);
        }
    }else {
#include "sc220at_fme_cfg_param.h"
        printf("Load sc220at_fme_cfg_param.h!\n");
        for (i = 0; i < sizeof(g_fme_param) / sizeof(g_fme_param[0]); i++){
            sc220at_write_register(vi_pipe, g_fme_param[i][0], g_fme_param[i][1]);
        }
    }
}

ot_void sc220at_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp)
{
    ot_s32 ret = OT_SUCCESS;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return;
    }
    master_pipe_id = vi_pipe[0];
    sc220at_set_blc_clamp_value(vi_pipe, blc_clamp.blc_clamp_en);
    return;
}

void sc220at_default_reg_init(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
    ot_s32 ret = OT_SUCCESS;
    ot_eis_sns_state *pastsc220at = OT_NULL;

    pastsc220at = sc220at_get_ctx(vi_pipe);
    for (i = 0; i < pastsc220at->regs_info[0].reg_num; i++) {
        ret += sc220at_write_register(vi_pipe,
            pastsc220at->regs_info[0].i2c_data[i].reg_addr,
            pastsc220at->regs_info[0].i2c_data[i].data);
    }
    if (ret != OT_SUCCESS) {
        sns_err_trace("write register failed!\n");
    }
    return;
}

static void sc220at_cfg_i2c_seq_raw12(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;

    printf("Load 30fps raw12 1080p sequence!\n");
    for (i = 0; i < sizeof(g_sc220at_seq_raw) / sizeof(g_sc220at_seq_raw[0]); i++) {
        sc220at_write_register(vi_pipe, g_sc220at_seq_raw[i][0], g_sc220at_seq_raw[i][1]);
    }
}

static void sc220at_cfg_i2c_seq_yuv422(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;

    printf("Load 30fps YUV422 1080p sequence!\n");
    for (i = 0; i < sizeof(g_sc220at_seq_yuv) / sizeof(g_sc220at_seq_yuv[0]); i++) {
        sc220at_write_register(vi_pipe, g_sc220at_seq_yuv[i][0], g_sc220at_seq_yuv[i][1]);
    }
}

ot_s32 sc220at_get_i2c_dev_addr(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *pastsc220at = sc220at_get_ctx(vi_pipe);
    ot_eis_sns_bus_info *sc220atbusinfo = sc220at_get_bus_info(vi_pipe);

    if (pastsc220at->init != OT_TRUE || sc220atbusinfo->bus_addr == 0)
        return OT_FAILURE;
    return sc220atbusinfo->bus_addr;
}
static ot_bool sc220at_get_quickboot_flag(ot_vi_pipe_id vi_pipe)
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

#define SC220AT_LOOP_CNT 200
static ot_bool sc220at_check_exist(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_u32 data = 0;

    for (int i = 0; i < SC220AT_LOOP_CNT; i++) {
        ret = sc220at_read_register(vi_pipe, 0x2103, &data);
        if (ret == OT_SUCCESS) {
            return OT_TRUE;
        }
        usleep(1000); // 1ms, total 200ms
    }
    sns_err_trace("sc220at pipe%d don't exist\n", vi_pipe);
    return OT_FALSE;
}

void sc220at_init(ot_vi_pipe_id vi_pipe)
{
    ot_bool init;
    ot_s32 ret;
    ot_eis_sns_state *pastsc220at = OT_NULL;
    ot_u8 image_mode;
    ot_bool quickboot_en;

    pastsc220at = sc220at_get_ctx(vi_pipe);
    init = pastsc220at->init;

    ret = sc220at_i2c_init(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("i2c init failed!\n");
        return;
    }

    quickboot_en = sc220at_get_quickboot_flag(vi_pipe);
    image_mode = pastsc220at->img_mode;

    if (image_mode == SC220AT_SENSOR_1080P_30FPS_YUV422)
    {
        printf("sc220at_init image_mode = ");
        printf("%d\n",image_mode);
    }

    /* When sensor first init, config all registers */
    if ((init == OT_FALSE) && (quickboot_en == OT_FALSE) && (sc220at_check_exist(vi_pipe) == OT_TRUE)) {
        if (image_mode < SC220AT_MODE_BUTT) {
            sleep(1); // wait until the harden sequence is completed

            sc220at_fme(vi_pipe);              //sensor FULL ME mode
            printf("sc220at_init sc220at_fme = \n");
            sc220at_write_register(vi_pipe, 0x2103, 0x01); // sc220at soft reset
            usleep(1000); // 1ms
            sc220at_write_register(vi_pipe, 0x2103, 0x00);

            if (image_mode == SC220AT_SENSOR_1080P_30FPS_YUV422) {
                sc220at_cfg_i2c_seq_yuv422(vi_pipe);
            } else if (image_mode == SC220AT_SENSOR_1080P_30FPS_RAW12_WDR_built_in_MODE) {
                sc220at_cfg_i2c_seq_raw12(vi_pipe);
            }
        }
    }

    pastsc220at->init = OT_TRUE;
    return;
}

void sc220at_exit(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_eis_sns_state *pastsc220at = OT_NULL;

    ret = sc220at_i2c_exit(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sc220at exit failed!\n");
    }

    pastsc220at = sc220at_get_ctx(vi_pipe);
    pastsc220at->init = OT_FALSE;

    return;
}
