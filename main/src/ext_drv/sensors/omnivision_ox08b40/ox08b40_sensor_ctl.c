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

#include "ox08b40_cmos.h"
#include "ot_eis_vi_api.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
#define I2C_BUS_ID_DEF       -1

static int g_fd[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = -1};

int ox08b40_i2c_init(ot_vi_pipe_id vi_pipe)
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
    ot_eis_sns_bus_info *ox08b40businfo = OT_NULL;
    ox08b40businfo = ox08b40_get_bus_info(vi_pipe);
    dev_num = ox08b40businfo->com_bus.i2c_dev;
    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        sns_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return OT_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_EIS_I2C_SLAVE_FORCE, (ox08b40businfo->bus_addr >> 1));
    if (ret < 0) {
        sns_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return OT_SUCCESS;
}

int ox08b40_i2c_exit(ot_vi_pipe_id vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return OT_SUCCESS;
    }
    return OT_FAILURE;
}

ot_s32 ox08b40_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = OX08B40_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = OX08B40_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = OX08B40_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        sns_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];

    if (OX08B40_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (addr >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = addr & 0xff;
    } else {
        buf[idx++] = addr & 0xff;
    }

    if (OX08B40_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (data >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = data & 0xff;
    } else {
        buf[idx++] = data & 0xff;
    }

    ot_eis_sns_bus_info *bus_info = ox08b40_get_bus_info(vi_pipe);
    ret = write(g_fd[vi_pipe], buf, OX08B40_ADDR_BYTE + OX08B40_DATA_BYTE);
    if (ret < 0) {
        sns_err_trace("i2c%d devaddr 0x%x write 0x%x data 0x%x failed!\n",
            bus_info->com_bus.i2c_dev, bus_info->bus_addr, addr, data);
        return OT_FAILURE;
    }

#endif
    return OT_SUCCESS;
}

ot_s32 ox08b40_read_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 *data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];
    ot_u8 buf_read[I2C_BUF_NUM];

    if (OX08B40_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (addr >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = addr & 0xff;
    } else {
        buf[idx++] = addr & 0xff;
    }

    ret = write(g_fd[vi_pipe], buf, OX08B40_ADDR_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    ret = read(g_fd[vi_pipe], buf_read, OX08B40_DATA_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    if (OX08B40_DATA_BYTE == 2) {  /* 2 byte */
        ot_u32 high = buf_read[0];
        ot_u32 low = buf_read[1];
        *data = (high << 8) + low;
    } else {
        *data = buf_read[0];
    }

    return OT_SUCCESS;
}

void ox08b40_prog(ot_vi_pipe_id vi_pipe, const ot_u32 *rom)
{
    return;
}

void ox08b40_standby(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

void ox08b40_restart(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

ot_void ox08b40_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp)
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
    ox08b40_set_blc_clamp_value(master_pipe_id, blc_clamp.blc_clamp_en);
    return;
}

void ox08b40_default_reg_init(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
    ot_s32 ret = OT_SUCCESS;
    ot_eis_sns_state *pastox08b40 = OT_NULL;

    pastox08b40 = ox08b40_get_ctx(vi_pipe);
    for (i = 0; i < pastox08b40->regs_info[0].reg_num; i++) {
        ret += ox08b40_write_register(vi_pipe,
            pastox08b40->regs_info[0].i2c_data[i].reg_addr,
            pastox08b40->regs_info[0].i2c_data[i].data);
    }
    if (ret != OT_SUCCESS) {
        sns_err_trace("write register failed!\n");
    }
    return;
}

static void ox08b40_cfg_i2c_seq(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
#if !defined(OX08B40_DATA_TYPE_RAW16) && !defined(OX08B40_RATE_60FPS) // 2160p raw12 30fps
#include "ox08b40_30fps_pwl12.h"
    printf("Load 30fps PWL12 2160p sequence!\n");
#else
#error "Please select correct sequence!"
#endif

    for (i = 0; i < sizeof(g_ox08b40_seq) / sizeof(g_ox08b40_seq[0]); i++) {
        ox08b40_write_register(vi_pipe, g_ox08b40_seq[i][0], g_ox08b40_seq[i][1]);
    }
}

static void ox08b40_fme(ot_vi_pipe_id vi_pipe)              //for sensor fme ae_mode      in sensor_init:imx623_init
{
    ot_u32 i;
    if (ox08b40_get_ext_sensor_flag(vi_pipe) == 1) {
        ot_eis_sns_reg_table *sns_table = ox08b40_get_ext_sensor_table(vi_pipe);
        ot_eis_sns_reg_table  *fme_table = sns_table + 4;
        for (i = 0; i < fme_table->data_len; i++) {
            ox08b40_write_register(vi_pipe, (fme_table->reg_data + i)->addr, (fme_table->reg_data + i)->data);
        }
    }else {
    #include "ox08b40_fme_cfg_param.h"
        for (i = 0; i < sizeof(g_fme_param) / sizeof(g_fme_param[0]); i++)
        {
            ox08b40_write_register(vi_pipe, g_fme_param[i][0], g_fme_param[i][1]);
        }
    }
}

ot_s32 ox08b40_get_i2c_dev_addr(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *pastox08b40 = ox08b40_get_ctx(vi_pipe);
    ot_eis_sns_bus_info *ox08b40businfo = ox08b40_get_bus_info(vi_pipe);

    if (pastox08b40->init != OT_TRUE || ox08b40businfo->bus_addr == 0)
        return OT_FAILURE;
    return ox08b40businfo->bus_addr;
}

#define OX08B40_LOOP_CNT 200
static ot_bool ox08b40_check_exist(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_u32 data = 0;

    for (int i = 0; i < OX08B40_LOOP_CNT; i++) {
        ret = ox08b40_read_register(vi_pipe, 0x0100, &data);
        if (ret == OT_SUCCESS) {
            return OT_TRUE;
        }
        usleep(1000); // 1ms, total 200ms
    }
    sns_err_trace("ox08b40 pipe%d don't exist\n", vi_pipe);
    return OT_FALSE;
}

void ox08b40_init(ot_vi_pipe_id vi_pipe)
{
    ot_bool          init;
    ot_s32 ret;
    ot_eis_sns_state *pastox08b40 = OT_NULL;

    pastox08b40 = ox08b40_get_ctx(vi_pipe);
    init       = pastox08b40->init;

    ret = ox08b40_i2c_init(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("i2c init failed!\n");
        return;
    }
    /* When sensor first init, config all registers */
    if ((init == OT_FALSE) && (ox08b40_check_exist(vi_pipe) == OT_TRUE)) {
        ox08b40_cfg_i2c_seq(vi_pipe);
    }
    ox08b40_fme(vi_pipe);              //sensor FULL ME mode
    ox08b40_write_register(vi_pipe, 0x0100, 0x01); // ox08b40 streaming

    pastox08b40->init = OT_TRUE;
    return;
}

void ox08b40_exit(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_eis_sns_state *pastox08b40 = OT_NULL;

    ret = ox08b40_i2c_exit(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("ox08b40 exit failed!\n");
    }

    pastox08b40 = ox08b40_get_ctx(vi_pipe);
    pastox08b40->init = OT_FALSE;
    return;
}
