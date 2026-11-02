/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
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

#include "imx623_cmos.h"
#include "imx623_cfg.h"
#include "ot_eis_vi_api.h"

#define I2C_DEV_FILE_NUM        16
#define I2C_BUF_NUM             8
#define I2C_BUS_ID_DEF          -1

static int g_fd[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = -1};

int imx623_i2c_init(ot_vi_pipe_id vi_pipe)
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
    ot_eis_sns_bus_info *imx623businfo = OT_NULL;
    imx623businfo = imx623_get_bus_info(vi_pipe);
    dev_num = imx623businfo->com_bus.i2c_dev;
    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        sns_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return OT_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_EIS_I2C_SLAVE_FORCE, (imx623businfo->bus_addr >> 1));
    if (ret < 0) {
        sns_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return OT_SUCCESS;
}

int imx623_i2c_exit(ot_vi_pipe_id vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return OT_SUCCESS;
    }
    return OT_FAILURE;
}

ot_s32 imx623_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = IMX623_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = I2C_REG_ADDR_2BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = I2C_DATE_1BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        sns_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];

    if (I2C_REG_ADDR_2BYTE == 2) {  /* 2 byte */
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

    ot_eis_sns_bus_info *bus_info = imx623_get_bus_info(vi_pipe);
    ret = write(g_fd[vi_pipe], buf, I2C_REG_ADDR_2BYTE + I2C_DATA_BYTE);
    if (ret < 0) {
        sns_err_trace("i2c%d devaddr 0x%x write 0x%x data 0x%x failed!\n",
            bus_info->com_bus.i2c_dev, bus_info->bus_addr, addr, data);
        return OT_FAILURE;
    }

#endif
    return OT_SUCCESS;
}

ot_s32 imx623_read_register(ot_vi_pipe_id vi_pipe, ot_u16 addr, ot_u32 *data)
{
    ot_s32 ret;
    ot_u32 idx = 0;
    ot_u8 buf[I2C_BUF_NUM] = {0};
    ot_eis_sns_bus_info *bus_info = imx623_get_bus_info(vi_pipe);

    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

    buf[idx++]         = ((addr >> 8) & 0xff);
    buf[idx++]         = ((addr >> 0) & 0xff);
    ret = write(g_fd[vi_pipe], buf, I2C_REG_ADDR_2BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    ret = read(g_fd[vi_pipe], buf, I2C_DATA_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    *data = buf[0];
    return OT_SUCCESS;
}

void imx623_prog(ot_vi_pipe_id vi_pipe, const ot_u32 *rom)
{
    return;
}

void imx623_standby(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

void imx623_restart(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

ot_void imx623_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp)
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
    imx623_set_blc_clamp_value(master_pipe_id, blc_clamp.blc_clamp_en);
    return;
}

void imx623_default_reg_init(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
    ot_s32 ret = OT_SUCCESS;
    ot_eis_sns_state *pastimx623 = OT_NULL;

    pastimx623 = imx623_get_ctx(vi_pipe);
    for (i = 0; i < pastimx623->regs_info[0].reg_num; i++) {
        ret += imx623_write_register(vi_pipe,
            pastimx623->regs_info[0].i2c_data[i].reg_addr,
            pastimx623->regs_info[0].i2c_data[i].data);
    }

    if (ret != OT_SUCCESS) {
        sns_err_trace("write register failed!\n");
    }

    return;
}

static void imx623_cfg_i2c_seq(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;

#if !defined(IMX623_CHIP_TYPE_ES3) // es2 30fps raw12 1936x1552
#include "imx623_es2_30fps_raw12.h"
    printf("Load ES2 30fps RAW12!\n");
#elif defined(IMX623_RES_1280x720) // es3 30fps raw16 1280x720
#include "imx623_es3_30fps_raw16_720p.h"
    printf("Load ES3 30fps RAW16 1280x720!\n");
#elif defined(IMX623_FRAME_RATE_60FPS) // es3 60fps raw16 1936x1552
#include "imx623_es3_60fps_raw16.h"
    printf("Load ES3 60fps RAW16!\n");
#else // es3 30fps raw16 1936x1552
#include "imx623_es3_30fps_raw16.h"
    printf("Load ES3 30fps RAW16!\n");
#endif

    for (i = 0; i < sizeof(g_imx623_seq) / sizeof(g_imx623_seq[0]); i++)
        imx623_write_register(vi_pipe, g_imx623_seq[i][0], g_imx623_seq[i][1]);
}

#ifdef IMX623_CHIP_TYPE_ES3
static void imx623_es3_cali(ot_vi_pipe_id vi_pipe)              //for sensor calibration      in sensor_init:imx623_init
{
    ot_u32 i;
    if (imx623_get_ext_sensor_flag(vi_pipe) == 1) {
        ot_eis_sns_reg_table *sns_table = imx623_get_ext_sensor_table(vi_pipe);

        ot_eis_sns_reg_table  *dkshd_table = sns_table;
        ot_eis_sns_reg_table  *adj_table = sns_table + 1;
        ot_eis_sns_reg_table  *pxshd_table = sns_table + 2;
        ot_eis_sns_reg_table  *awb_table = sns_table + 3;

        for (i = 0; i < dkshd_table->data_len; i++) {
            imx623_write_register(vi_pipe, (dkshd_table->reg_data + i)->addr, (dkshd_table->reg_data + i)->data);
        }
        for (i = 0; i < adj_table->data_len; i++) {
            imx623_write_register(vi_pipe, (adj_table->reg_data + i)->addr, (adj_table->reg_data + i)->data);
        }
        for (i = 0; i < pxshd_table->data_len; i++) {
            imx623_write_register(vi_pipe, (pxshd_table->reg_data + i)->addr, (pxshd_table->reg_data + i)->data);
        }
        for (i = 0; i < awb_table->data_len; i++) {
            imx623_write_register(vi_pipe, (awb_table->reg_data + i)->addr, (awb_table->reg_data + i)->data);
        }
        printf("sensor cali param\n");
    } else {
        #include "imx623_es3_hj6158_cali.h"
        for (i = 0; i < sizeof(g_DKSHD_param) / sizeof(g_DKSHD_param[0]); i++) {
            imx623_write_register(vi_pipe, g_DKSHD_param[i][0], g_DKSHD_param[i][1]);
        }
        for (i = 0; i < sizeof(g_ADJ_param) / sizeof(g_ADJ_param[0]); i++) {
            imx623_write_register(vi_pipe, g_ADJ_param[i][0], g_ADJ_param[i][1]);
        }
        for (i = 0; i < sizeof(g_PXSHD_param) / sizeof(g_PXSHD_param[0]); i++) {
            imx623_write_register(vi_pipe, g_PXSHD_param[i][0], g_PXSHD_param[i][1]);
        }
        for (i = 0; i < sizeof(g_preAWB_param) / sizeof(g_preAWB_param[0]); i++) {
            imx623_write_register(vi_pipe, g_preAWB_param[i][0], g_preAWB_param[i][1]);
        }
        printf("sensor cali param\n");
    }
}
#endif

static void imx623_fme(ot_vi_pipe_id vi_pipe)              //for sensor fme ae_mode      in sensor_init:imx623_init
{
    ot_u32 i;

    if (imx623_get_ext_sensor_flag(vi_pipe) == 1) {
        ot_eis_sns_reg_table *sns_table = imx623_get_ext_sensor_table(vi_pipe);
        ot_eis_sns_reg_table  *fme_table = sns_table + 4;

        for (i = 0; i < fme_table->data_len; i++) {
            imx623_write_register(vi_pipe, (fme_table->reg_data + i)->addr, (fme_table->reg_data + i)->data);
        }
    }else {
        #include "imx623_fme_cfg_param.h"
        for (i = 0; i < sizeof(g_fme_param) / sizeof(g_fme_param[0]); i++) {
            imx623_write_register(vi_pipe, g_fme_param[i][0], g_fme_param[i][1]);
        }
    }
}

ot_s32 imx623_get_i2c_dev_addr(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *pastimx623 = imx623_get_ctx(vi_pipe);
    ot_eis_sns_bus_info *imx623businfo = imx623_get_bus_info(vi_pipe);

    if (pastimx623->init != OT_TRUE || imx623businfo->bus_addr == 0)
        return OT_FAILURE;
    return imx623businfo->bus_addr;
}

static ot_bool imx623_get_quickboot_flag(ot_vi_pipe_id vi_pipe)
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

#define IMX623_LOOP_CNT 200
static ot_bool imx623_check_exist(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_u32 data = 0;

    usleep(200000); // imx623 flush flash need 200ms
    for (int i = 0; i < IMX623_LOOP_CNT; i++) {
        ret = imx623_read_register(vi_pipe, 0xffff, &data);
        if (ret == OT_SUCCESS) {
            return OT_TRUE;
        }
        usleep(1000); // 1ms, total 200ms
    }
    sns_err_trace("imx623 pipe%d don't exist\n", vi_pipe);
    return OT_FALSE;
}

static ot_bool imx623_is_firmware(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_u32 reg_8b = 0;
    ot_u32 reg_8c = 0;
    ot_u32 reg_8d = 0;

    ret  = imx623_read_register(vi_pipe, 0x608b, &reg_8b);
    ret |= imx623_read_register(vi_pipe, 0x608c, &reg_8c);
    ret |= imx623_read_register(vi_pipe, 0x608d, &reg_8d);
    if (ret != OT_SUCCESS) {
        return OT_FALSE;
    }

    // if both 0x608b & 0x608c & 0x608d values is 0x01, it is firmware
    if (reg_8b == 0x01 && reg_8c == 0x01 && reg_8d == 0x01) {
        return OT_TRUE;
    } else {
        return OT_FALSE;
    }
}

void imx623_init(ot_vi_pipe_id vi_pipe)
{
    ot_bool          quickboot_en;
    ot_s32 ret;
    ot_eis_sns_state *pastimx623 = OT_NULL;

    pastimx623 = imx623_get_ctx(vi_pipe);

    ret = imx623_i2c_init(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("i2c init failed!\n");
        return;
    }

    quickboot_en = imx623_get_quickboot_flag(vi_pipe);

    /* When sensor first init, config all registers */
    if ((quickboot_en == OT_FALSE) &&
        (imx623_check_exist(vi_pipe) == OT_TRUE) && (imx623_is_firmware(vi_pipe) == OT_FALSE)) {
#ifdef IMX623_CHIP_TYPE_ES3
        imx623_cfg_i2c_seq(vi_pipe); // es3 only starts with i2c
#else
        ot_eis_sns_bus_info *imx623businfo = imx623_get_bus_info(vi_pipe);
        if (imx623businfo.bus_addr == IMX623_I2C_ADDR_I2C) // es2 distinguish between flash startup and i2c startup
            imx623_cfg_i2c_seq(vi_pipe);
#endif
#ifdef IMX623_CHIP_TYPE_ES3
        imx623_es3_cali(vi_pipe);         //es3 sensor calibration
#endif
        imx623_fme(vi_pipe);              //sensor FULL ME mode

        imx623_write_register(vi_pipe, 0x8A01, 0x80); // streaming
    }

    pastimx623->init = OT_TRUE;
    return;
}

void imx623_exit(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_eis_sns_state *pastimx623 = OT_NULL;

    ret = imx623_i2c_exit(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("imx623 exit failed!\n");
    }

    pastimx623 = imx623_get_ctx(vi_pipe);
    pastimx623->init = OT_FALSE;
    return;
}
