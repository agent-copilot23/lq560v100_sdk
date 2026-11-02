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

#include "imx582_cmos.h"
#include "ot_eis_vi_api.h"

#include "imx582_fme_cfg_param.h"
#include "imx582_8k6k_5fps_raw10.h"
#include "imx582_4k3k_30fps_raw10.h"
#include "imx582_2160p_30fps_raw10.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
#define I2C_BUS_ID_DEF       -1

static int g_fd[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = -1};

int imx582_i2c_init(ot_vi_pipe_id vi_pipe)
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
    ot_eis_sns_bus_info *imx582businfo = OT_NULL;
    imx582businfo = imx582_get_bus_info(vi_pipe);
    dev_num = imx582businfo->com_bus.i2c_dev;
    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        sns_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return OT_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_EIS_I2C_SLAVE_FORCE, (imx582businfo->bus_addr >> 1));
    if (ret < 0) {
        sns_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return OT_SUCCESS;
}

int imx582_i2c_exit(ot_vi_pipe_id vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return OT_SUCCESS;
    }
    return OT_FAILURE;
}

ot_s32 imx582_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = IMX582_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = IMX582_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = IMX582_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        sns_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];

    if (IMX582_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (addr >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = addr & 0xff;
    } else {
        buf[idx++] = addr & 0xff;
    }

    if (IMX582_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (data >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = data & 0xff;
    } else {
        buf[idx++] = data & 0xff;
    }

    ot_eis_sns_bus_info *bus_info = imx582_get_bus_info(vi_pipe);
    ret = write(g_fd[vi_pipe], buf, IMX582_ADDR_BYTE + IMX582_DATA_BYTE);
    if (ret < 0) {
        sns_err_trace("i2c%d devaddr 0x%x write 0x%x data 0x%x failed!\n",
            bus_info->com_bus.i2c_dev, bus_info->bus_addr, addr, data);
        return OT_FAILURE;
    }
#endif
    return OT_SUCCESS;
}

ot_s32 imx582_read_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 *data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];
    ot_u8 buf_read[I2C_BUF_NUM];

    if (IMX582_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (addr >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = addr & 0xff;
    } else {
        buf[idx++] = addr & 0xff;
    }

    ret = write(g_fd[vi_pipe], buf, IMX582_ADDR_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    ret = read(g_fd[vi_pipe], buf_read, IMX582_DATA_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    if (IMX582_DATA_BYTE == 2) {  /* 2 byte */
        ot_u32 high = buf_read[0];
        ot_u32 low = buf_read[1];
        *data = (high << 8) + low;
    } else {
        *data = buf_read[0];
    }

    return OT_SUCCESS;
}

void imx582_prog(ot_vi_pipe_id vi_pipe, const ot_u32 *rom)
{
    return;
}

void imx582_standby(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

void imx582_restart(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

ot_void imx582_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp)
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
    imx582_set_blc_clamp_value(master_pipe_id, blc_clamp.blc_clamp_en);
    return;
}

void imx582_default_reg_init(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
    ot_s32 ret = OT_SUCCESS;
    ot_eis_sns_state *pastimx582 = OT_NULL;

    pastimx582 = imx582_get_ctx(vi_pipe);
    for (i = 0; i < pastimx582->regs_info[0].reg_num; i++) {
        ret += c(vi_pipe,
            pastimx582->regs_info[0].i2c_data[i].reg_addr,
            pastimx582->regs_info[0].i2c_data[i].data);
    }
    if (ret != OT_SUCCESS) {
        sns_err_trace("write register failed!\n");
    }
    return;
}

static void imx582_cfg_i2c_seq(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
	ot_eis_sns_state *sns_state = OT_NULL;
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);
    
    switch (sns_state->img_mode) {
    case IMX582_SENSOR_4K3K_30FPS_LINEAR_MODE:
        for (i = 0; i < sizeof(g_imx582_4k3k_30fps_seq) / sizeof(g_imx582_4k3k_30fps_seq[0]); i++) {
            imx582_write_register(vi_pipe, g_imx582_4k3k_30fps_seq[i][0], g_imx582_4k3k_30fps_seq[i][1]);
        }
        printf("===============================================================\n");
        printf("== vi_pipe:%d imx582 10bit linear 4k3k 30fps init success! ==\n", vi_pipe);
        printf("===============================================================\n");
        break;
    case IMX582_SENSOR_2160P_30FPS_LINEAR_MODE:
        for (i = 0; i < sizeof(g_imx582_2160p_30fps_seq) / sizeof(g_imx582_2160p_30fps_seq[0]); i++) {
            imx582_write_register(vi_pipe, g_imx582_2160p_30fps_seq[i][0], g_imx582_2160p_30fps_seq[i][1]);
        }
        printf("===============================================================\n");
        printf("== vi_pipe:%d imx582 10bit linear 2160p 30fps init success! ==\n", vi_pipe);
        printf("===============================================================\n");
        break;
    case IMX582_SENSOR_8K6K_5FPS_LINEAR_MODE:
        for (i = 0; i < sizeof(g_imx582_8k6k_5fps_seq) / sizeof(g_imx582_8k6k_5fps_seq[0]); i++) {
            imx582_write_register(vi_pipe, g_imx582_8k6k_5fps_seq[i][0], g_imx582_8k6k_5fps_seq[i][1]);
        }
        printf("===============================================================\n");
        printf("== vi_pipe:%d imx582 10bit linear 8k6k 5fps init success! ==\n", vi_pipe);
        printf("===============================================================\n");
        break;
    default:
        printf("===============================================================\n");
        printf("== vi_pipe:%d size error! ==\n", vi_pipe);
        printf("===============================================================\n");
        break;
    }
}

static void imx582_fme(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
    if (imx582_get_ext_sensor_flag(vi_pipe) == 1) {
        ot_eis_sns_reg_table *sns_table = imx582_get_ext_sensor_table(vi_pipe);
        ot_eis_sns_reg_table  *fme_table = sns_table + 4;
        for (i = 0; i < fme_table->data_len; i++) {
            imx582_write_register(vi_pipe, (fme_table->reg_data + i)->addr, (fme_table->reg_data + i)->data);
        }
    } else {
        for (i = 0; i < sizeof(g_fme_param) / sizeof(g_fme_param[0]); i++) {
            imx582_write_register(vi_pipe, g_fme_param[i][0], g_fme_param[i][1]);
        }
    }
}

ot_s32 imx582_get_i2c_dev_addr(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *pastimx582 = imx582_get_ctx(vi_pipe);
    ot_eis_sns_bus_info *imx582businfo = imx582_get_bus_info(vi_pipe);

    if (pastimx582->init != OT_TRUE || imx582businfo->bus_addr == 0)
        return OT_FAILURE;
    return imx582businfo->bus_addr;
}

#define IMX582_LOOP_CNT 200
static ot_bool imx582_check_exist(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_u32 data = 0;
    for (int i = 0; i < IMX582_LOOP_CNT; i++) {
        ret = imx582_read_register(vi_pipe, 0x0100, &data);
        if (ret == OT_SUCCESS) {
            return OT_TRUE;
        }
        usleep(1000); // 1ms, total 200ms
    }
    sns_err_trace("imx582 pipe%d don't exist\n", vi_pipe);
    return OT_FALSE;
}

void imx582_init(ot_vi_pipe_id vi_pipe) 
{
    ot_bool          init;
    ot_s32 ret;
    ot_eis_sns_state *pastimx582 = OT_NULL;

    pastimx582 = imx582_get_ctx(vi_pipe);
    init       = pastimx582->init;

    ret = imx582_i2c_init(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("i2c init failed!\n");
        return;
    }
    /* When sensor first init, config all registers */
    if ((init == OT_FALSE) && (imx582_check_exist(vi_pipe) == OT_TRUE)) {
        imx582_cfg_i2c_seq(vi_pipe);
    }
    imx582_fme(vi_pipe);              //sensor FULL ME mode
    imx582_write_register(vi_pipe, 0x0100, 0x01); // imx582 streaming

    pastimx582->init = OT_TRUE;
    return;
}

void imx582_exit(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_eis_sns_state *pastimx582 = OT_NULL;

    ret = imx582_i2c_exit(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("imx582 exit failed!\n");
    }

    pastimx582 = imx582_get_ctx(vi_pipe);
    pastimx582->init = OT_FALSE;
    return;
}
