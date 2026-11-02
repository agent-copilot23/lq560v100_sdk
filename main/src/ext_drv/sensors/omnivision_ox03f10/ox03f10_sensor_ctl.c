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

#include "ox03f10_cmos.h"
#include "ot_eis_vi_api.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
#define I2C_BUS_ID_DEF       -1

static int g_fd[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = -1};

int ox03f10_i2c_init(ot_vi_pipe_id vi_pipe)
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
    ot_eis_sns_bus_info *ox03f10businfo = ox03f10_get_bus_info(vi_pipe);
    ot_u8 dev_num = ox03f10businfo->com_bus.i2c_dev;
    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        sns_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return OT_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_EIS_I2C_SLAVE_FORCE, (ox03f10businfo->bus_addr >> 1));
    if (ret < 0) {
        sns_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return OT_SUCCESS;
}

int ox03f10_i2c_exit(ot_vi_pipe_id vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return OT_SUCCESS;
    }
    return OT_FAILURE;
}

ot_s32 ox03f10_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = OX03F10_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = OX03F10_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = OX03F10_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        sns_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];

    if (OX03F10_ADDR_BYTE == 2) {  /* 2 byte */
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

    ot_eis_sns_bus_info *bus_info = ox03f10_get_bus_info(vi_pipe);
    ret = write(g_fd[vi_pipe], buf, OX03F10_ADDR_BYTE + OX03F10_DATA_BYTE);
    if (ret < 0) {
        sns_err_trace("i2c%d devaddr 0x%x write 0x%x data 0x%x failed!\n",
            bus_info->com_bus.i2c_dev, bus_info->bus_addr, addr, data);
        return OT_FAILURE;
    }

#endif
    return OT_SUCCESS;
}

ot_s32 ox03f10_read_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 *data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];
    ot_u8 buf_read[I2C_BUF_NUM];

    if (OX03F10_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx++] = (addr >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = addr & 0xff;
    } else {
        buf[idx++] = addr & 0xff;
    }

    ret = write(g_fd[vi_pipe], buf, OX03F10_ADDR_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    ret = read(g_fd[vi_pipe], buf_read, OX03F10_DATA_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    if (OX03F10_DATA_BYTE == 2) {  /* 2 byte */
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

void ox03f10_prog(ot_vi_pipe_id vi_pipe, const ot_u32 *rom)
{
    return;
}

void ox03f10_standby(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

void ox03f10_restart(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

static void ox03f10_fme(ot_vi_pipe_id vi_pipe)              //for sensor fme ae_mode      in sensor_init:ox03f_init
{
	ot_u32 i;
    if (ox03f_get_ext_sensor_flag(vi_pipe) == 1) {
        ot_eis_sns_reg_table *sns_table = ox03f_get_ext_sensor_table(vi_pipe);
        ot_eis_sns_reg_table  *fme_table = sns_table + 4;

        for (i = 0; i < fme_table->data_len; i++) {
            ox03f10_write_register(vi_pipe, (fme_table->reg_data + i)->addr, (fme_table->reg_data + i)->data);
        }
    }else {
	#include "ox03f10_fme_cfg_param.h"
        for (i = 0; i < sizeof(g_fme_param) / sizeof(g_fme_param[0]); i++)
        {
            ox03f10_write_register(vi_pipe, g_fme_param[i][0], g_fme_param[i][1]);
        }
	}
}

ot_void ox03f10_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp)
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
    ox03f10_set_blc_clamp_value(master_pipe_id, blc_clamp.blc_clamp_en);
    return;
}

void ox03f10_default_reg_init(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
    ot_s32 ret = OT_SUCCESS;
    ot_eis_sns_state *pastox03f10 = OT_NULL;

    pastox03f10 = ox03f10_get_ctx(vi_pipe);
    for (i = 0; i < pastox03f10->regs_info[0].reg_num; i++) {
        ret += ox03f10_write_register(vi_pipe,
            pastox03f10->regs_info[0].i2c_data[i].reg_addr,
            pastox03f10->regs_info[0].i2c_data[i].data);
    }
    if (ret != OT_SUCCESS) {
        sns_err_trace("write register failed!\n");
    }
    return;
}

static void ox03f10_cfg_i2c_seq(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
#if !defined(OX03F10_DATA_TYPE_RAW16) && defined(OX03F10_RATE_60FPS)  // 1920x1536 raw12 60fps
#include "ox03f10_60fps_pwl12.h"
    printf("Load 60fps PWL12 1920x1536 sequence!\n");
#elif defined(OX03F10_DATA_TYPE_RAW16) && defined(OX03F10_RATE_60FPS)  // 1920x1536 raw16 60fps
#include "ox03f10_60fps_pwl16.h"
    printf("Load 60fps PWL16 1920x1536 sequence!\n");
#else
#error "Please select correct sequence!"
#endif
    ox03f10_write_register(vi_pipe, 0x0103, 0x01);
    ox03f10_write_register(vi_pipe, 0x0107, 0x01);
    delay_ms(10);

    for (i = 0; i < sizeof(g_ox03f10_seq) / sizeof(g_ox03f10_seq[0]); i++) {
        ox03f10_write_register(vi_pipe, g_ox03f10_seq[i][0], g_ox03f10_seq[i][1]);
    }
}

ot_s32 ox03f10_get_i2c_dev_addr(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *pastox03f10 = ox03f10_get_ctx(vi_pipe);
    ot_eis_sns_bus_info *ox03f10businfo = ox03f10_get_bus_info(vi_pipe);

    if (pastox03f10->init != OT_TRUE || ox03f10businfo->bus_addr == 0)
        return OT_FAILURE;
    return ox03f10businfo->bus_addr;
}

#define OX03F10_LOOP_CNT 200
static ot_bool ox03f10_check_exist(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_u32 data = 0;

    for (int i = 0; i < OX03F10_LOOP_CNT; i++) {
        ret = ox03f10_read_register(vi_pipe, 0x0100, &data);
        if (ret == OT_SUCCESS) {
            return OT_TRUE;
        }
        usleep(1000); // 1ms, total 200ms
    }
    sns_err_trace("ox03f10 pipe%d don't exist\n", vi_pipe);
    return OT_FALSE;
}

void ox03f10_init(ot_vi_pipe_id vi_pipe)
{
    ot_bool          init;
    ot_s32 ret;
    ot_eis_sns_state *pastox03f10 = OT_NULL;

    pastox03f10 = ox03f10_get_ctx(vi_pipe);
    init       = pastox03f10->init;

    ret = ox03f10_i2c_init(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("i2c init failed!\n");
        return;
    }
    /* When sensor first init, config all registers */
    if ((init == OT_FALSE) && (ox03f10_check_exist(vi_pipe) == OT_TRUE)) {
        ox03f10_cfg_i2c_seq(vi_pipe);
    }

    ox03f10_fme(vi_pipe);              //sensor FULL ME mode
    ox03f10_write_register(vi_pipe, 0x0100, 0x01); // ox03f10 streaming
    pastox03f10->init = OT_TRUE;
    return;
}

void ox03f10_exit(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_eis_sns_state *pastox03f10 = OT_NULL;

    ret = ox03f10_i2c_exit(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("ox03f10 exit failed!\n");
    }

    pastox03f10 = ox03f10_get_ctx(vi_pipe);
    pastox03f10->init = OT_FALSE;
    return;
}
