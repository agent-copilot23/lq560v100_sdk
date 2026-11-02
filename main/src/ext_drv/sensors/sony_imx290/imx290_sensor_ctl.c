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

#include "imx290_cmos.h"
#include "ot_eis_vi_api.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

static int g_fd[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = -1};

int imx290_i2c_init(ot_vi_pipe_id vi_pipe)
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
    ot_eis_sns_bus_info *imx290businfo = OT_NULL;
    imx290businfo = imx290_get_bus_info(vi_pipe);
    dev_num = imx290businfo->com_bus.i2c_dev;
    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        sns_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return OT_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_EIS_I2C_SLAVE_FORCE, (imx290businfo->bus_addr >> 1));
    if (ret < 0) {
        sns_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
#endif

    return OT_SUCCESS;
}

int imx290_i2c_exit(ot_vi_pipe_id vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return OT_SUCCESS;
    }
    return OT_FAILURE;
}

ot_s32 imx290_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data)
{
    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = IMX290_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = IMX290_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = IMX290_DATA_BYTE;

    ret = ioctl(g_fd[vi_pipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        sns_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    ot_u32 idx = 0;
    ot_s32 ret;
    ot_u8 buf[I2C_BUF_NUM];

    if (IMX290_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
    }

    if (IMX290_DATA_BYTE == 2) {  /* 2 byte */
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ot_eis_sns_bus_info *bus_info = imx290_get_bus_info(vi_pipe);
    ret = write(g_fd[vi_pipe], buf, IMX290_ADDR_BYTE + IMX290_DATA_BYTE);
    if (ret < 0) {
        sns_err_trace("i2c%d devaddr 0x%x write 0x%x data 0x%x failed!\n",
            bus_info->com_bus.i2c_dev, bus_info->bus_addr, addr, data);
        return OT_FAILURE;
    }

#endif
    return OT_SUCCESS;
}

ot_s32 imx290_read_register(ot_vi_pipe_id vi_pipe, ot_u16 addr, ot_u32 *data)
{
    ot_s32 ret;
    ot_u32 idx = 0;
    ot_u8 buf[I2C_BUF_NUM] = {0};
    ot_eis_sns_bus_info *bus_info = imx290_get_bus_info(vi_pipe);

    if (g_fd[vi_pipe] < 0) {
        return OT_SUCCESS;
    }

    buf[idx++]         = ((addr >> 8) & 0xff);
    buf[idx++]         = ((addr >> 0) & 0xff);
    ret = write(g_fd[vi_pipe], buf, IMX290_ADDR_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }

    ret = read(g_fd[vi_pipe], buf, IMX290_DATA_BYTE);
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

void imx290_prog(ot_vi_pipe_id vi_pipe, const ot_u32 *rom)
{
    return;
}

void imx290_standby(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

void imx290_restart(ot_vi_pipe_handle pipe_hdl)
{
    return;
}

ot_void imx290_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp)
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

    imx290_set_blc_clamp_value(master_pipe_id, blc_clamp.blc_clamp_en);

    if (blc_clamp.blc_clamp_en == OT_TRUE) {
        ret += imx290_write_register(master_pipe_id, 0x4001, 0xeb);  /* clamp on */
    } else {
        ret += imx290_write_register(master_pipe_id, 0x4001, 0xea);  /* clamp off */
    }

    if (ret != OT_SUCCESS) {
        sns_err_trace("write register failed!\n");
    }
    return;
}

static void imx290_dol_wdr_2to1_2m60_10bit_init(ot_vi_pipe_id vi_pipe);
static void imx290_dol_wdr_2to1_2m30_12bit_init(ot_vi_pipe_id vi_pipe);
void imx290_dol_wdr_2to1_2m60_10bit_779m_init(ot_vi_pipe_id vi_pipe);

static void imx290_default_reg_init(ot_vi_pipe_id vi_pipe)
{
    ot_u32 i;
    ot_s32 ret = OT_SUCCESS;
    ot_eis_sns_state *pastimx290 = OT_NULL;

    pastimx290 = imx290_get_ctx(vi_pipe);
    for (i = 0; i < pastimx290->regs_info[0].reg_num; i++) {
        ret += imx290_write_register(vi_pipe,
            pastimx290->regs_info[0].i2c_data[i].reg_addr,
            pastimx290->regs_info[0].i2c_data[i].data);
    }

    if (ret != OT_SUCCESS) {
        sns_err_trace("write register failed!\n");
    }

    return;
}

static ot_bool imx290_get_quickboot_flag(ot_vi_pipe_id vi_pipe)
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

#define IMX290_LOOP_CNT 200
static ot_bool imx290_check_exist(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_u32 data = 0;

    for (int i = 0; i < IMX290_LOOP_CNT; i++) {
        ret = imx290_read_register(vi_pipe, 0x3000, &data);
        if (ret == OT_SUCCESS) {
            return OT_TRUE;
        }
        usleep(1000); // 1ms, total 200ms
    }
    sns_err_trace("imx290 pipe%d don't exist\n", vi_pipe);
    return OT_FALSE;
}

void imx290_init(ot_vi_pipe_id vi_pipe)
{
    ot_bool          quickboot_en;
    ot_eis_wdr_mode  wdr_mode;
    ot_u8            img_mode;
    ot_s32           ret;
    ot_eis_sns_state *pastimx290 = OT_NULL;

    pastimx290 = imx290_get_ctx(vi_pipe);
    wdr_mode    = pastimx290->wdr_mode;
    img_mode    = pastimx290->img_mode;

    ret = imx290_i2c_init(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("i2c init failed!\n");
        return;
    }

    quickboot_en = imx290_get_quickboot_flag(vi_pipe);

    if (!quickboot_en && (pastimx290->init == OT_FALSE) && (imx290_check_exist(vi_pipe) == OT_TRUE)) {
        if (OT_EIS_WDR_MODE_2To1_LINE == wdr_mode) {
            if (IMX290_2M_60FPS_10BIT_2TO1_DOL_MODE == img_mode) {
                imx290_dol_wdr_2to1_2m60_10bit_init(vi_pipe);
                // imx290_dol_wdr_2to1_2m60_10bit_779m_init(vi_pipe); // consider mipi bandwidth
            } else if (IMX290_2M_30FPS_12BIT_2TO1_DOL_MODE == img_mode) {
                imx290_dol_wdr_2to1_2m30_12bit_init(vi_pipe);
            } else {
                sns_err_trace("Invalid image mode!\n");
                return;
            }
        } else {
            sns_err_trace("Invalid sensor mode!\n");
            return;
        }
    }

    pastimx290->init = OT_TRUE;
    return;
}

void imx290_exit(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    ot_eis_sns_state *pastimx290 = OT_NULL;

    ret = imx290_i2c_exit(vi_pipe);
    if (ret != OT_SUCCESS) {
        sns_err_trace("IMX290 exit failed!\n");
    }

    pastimx290 = imx290_get_ctx(vi_pipe);
    pastimx290->init = OT_FALSE;

    return;
}

static void imx290_dol_wdr_2to1_2m60_10bit_init(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret = OT_SUCCESS;

    ret += imx290_write_register(vi_pipe, 0x3000, 0x01);  /* standby */

    ret += imx290_write_register(vi_pipe, 0x3002, 0x00);  /* XTMSTA */
    ret += imx290_write_register(vi_pipe, 0x3005, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3007, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3009, 0x00);
    ret += imx290_write_register(vi_pipe, 0x300a, 0x40);
    ret += imx290_write_register(vi_pipe, 0x300c, 0x11);
    ret += imx290_write_register(vi_pipe, 0x300f, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3010, 0x21);
    ret += imx290_write_register(vi_pipe, 0x3012, 0x64);
    ret += imx290_write_register(vi_pipe, 0x3016, 0x09);
    ret += imx290_write_register(vi_pipe, 0x3018, 0x65);  /* VMAX */
    ret += imx290_write_register(vi_pipe, 0x3019, 0x04);  /* VMAX */
    ret += imx290_write_register(vi_pipe, 0x301c, 0x4C);  /* HMAX */
    ret += imx290_write_register(vi_pipe, 0x301d, 0x04);  /* HMAX */

    ret += imx290_write_register(vi_pipe, 0x3020, 0x02);  /* SHS1 */
    ret += imx290_write_register(vi_pipe, 0x3021, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3024, 0xC9);  /* SHS2 */
    ret += imx290_write_register(vi_pipe, 0x3025, 0x07);
    ret += imx290_write_register(vi_pipe, 0x3030, 0x0B);  /* RHS1 */
    ret += imx290_write_register(vi_pipe, 0x3031, 0x00);

    ret += imx290_write_register(vi_pipe, 0x3045, 0x05);
    ret += imx290_write_register(vi_pipe, 0x3046, 0x00);
    ret += imx290_write_register(vi_pipe, 0x304b, 0x0a);
    ret += imx290_write_register(vi_pipe, 0x305c, 0x18);
    ret += imx290_write_register(vi_pipe, 0x305d, 0x03);
    ret += imx290_write_register(vi_pipe, 0x305e, 0x20);
    ret += imx290_write_register(vi_pipe, 0x305f, 0x01);
    ret += imx290_write_register(vi_pipe, 0x3070, 0x02);
    ret += imx290_write_register(vi_pipe, 0x3071, 0x11);
    ret += imx290_write_register(vi_pipe, 0x309b, 0x10);
    ret += imx290_write_register(vi_pipe, 0x309c, 0x22);
    ret += imx290_write_register(vi_pipe, 0x30a2, 0x02);
    ret += imx290_write_register(vi_pipe, 0x30a6, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30a8, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30aa, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30ac, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30b0, 0x43);

    ret += imx290_write_register(vi_pipe, 0x3106, 0x11);
    ret += imx290_write_register(vi_pipe, 0x3119, 0x9e);
    ret += imx290_write_register(vi_pipe, 0x311c, 0x1e);
    ret += imx290_write_register(vi_pipe, 0x311e, 0x08);
    ret += imx290_write_register(vi_pipe, 0x3128, 0x05);
    ret += imx290_write_register(vi_pipe, 0x3129, 0x1d);
    ret += imx290_write_register(vi_pipe, 0x313d, 0x83);
    ret += imx290_write_register(vi_pipe, 0x3150, 0x03);
    ret += imx290_write_register(vi_pipe, 0x315e, 0x1a);
    ret += imx290_write_register(vi_pipe, 0x3164, 0x1a);
    ret += imx290_write_register(vi_pipe, 0x317c, 0x12);
    ret += imx290_write_register(vi_pipe, 0x317e, 0x00);
    ret += imx290_write_register(vi_pipe, 0x31ec, 0x37);

    ret += imx290_write_register(vi_pipe, 0x32b8, 0x50);
    ret += imx290_write_register(vi_pipe, 0x32b9, 0x10);
    ret += imx290_write_register(vi_pipe, 0x32ba, 0x00);
    ret += imx290_write_register(vi_pipe, 0x32bb, 0x04);
    ret += imx290_write_register(vi_pipe, 0x32c8, 0x50);
    ret += imx290_write_register(vi_pipe, 0x32c9, 0x10);
    ret += imx290_write_register(vi_pipe, 0x32ca, 0x00);
    ret += imx290_write_register(vi_pipe, 0x32cb, 0x04);

    ret += imx290_write_register(vi_pipe, 0x332c, 0xd3);
    ret += imx290_write_register(vi_pipe, 0x332d, 0x10);
    ret += imx290_write_register(vi_pipe, 0x332e, 0x0d);
    ret += imx290_write_register(vi_pipe, 0x3358, 0x06);
    ret += imx290_write_register(vi_pipe, 0x3359, 0xe1);
    ret += imx290_write_register(vi_pipe, 0x335a, 0x11);
    ret += imx290_write_register(vi_pipe, 0x3360, 0x1e);
    ret += imx290_write_register(vi_pipe, 0x3361, 0x61);
    ret += imx290_write_register(vi_pipe, 0x3362, 0x10);
    ret += imx290_write_register(vi_pipe, 0x33b0, 0x50);
    ret += imx290_write_register(vi_pipe, 0x33b2, 0x1a);
    ret += imx290_write_register(vi_pipe, 0x33b3, 0x04);

    ret += imx290_write_register(vi_pipe, 0x3405, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3407, 0x03);
    ret += imx290_write_register(vi_pipe, 0x3414, 0x0a);
    ret += imx290_write_register(vi_pipe, 0x3415, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3418, 0x9C);  /* Y_OUT_SIZE */
    ret += imx290_write_register(vi_pipe, 0x3419, 0x08);  /* Y_OUT_SIZE */
    ret += imx290_write_register(vi_pipe, 0x3441, 0x0a);
    ret += imx290_write_register(vi_pipe, 0x3442, 0x0a);
    ret += imx290_write_register(vi_pipe, 0x3443, 0x03);
    ret += imx290_write_register(vi_pipe, 0x3444, 0x20);
    ret += imx290_write_register(vi_pipe, 0x3445, 0x25);
    ret += imx290_write_register(vi_pipe, 0x3446, 0x77);
    ret += imx290_write_register(vi_pipe, 0x3447, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3448, 0x67);
    ret += imx290_write_register(vi_pipe, 0x3449, 0x00);
    ret += imx290_write_register(vi_pipe, 0x344a, 0x47);
    ret += imx290_write_register(vi_pipe, 0x344b, 0x00);
    ret += imx290_write_register(vi_pipe, 0x344c, 0x37);
    ret += imx290_write_register(vi_pipe, 0x344d, 0x00);
    ret += imx290_write_register(vi_pipe, 0x344e, 0x3f);
    ret += imx290_write_register(vi_pipe, 0x344f, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3450, 0xff);
    ret += imx290_write_register(vi_pipe, 0x3451, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3452, 0x3f);
    ret += imx290_write_register(vi_pipe, 0x3453, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3454, 0x37);
    ret += imx290_write_register(vi_pipe, 0x3455, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3472, 0xa0);
    ret += imx290_write_register(vi_pipe, 0x3473, 0x07);
    ret += imx290_write_register(vi_pipe, 0x347b, 0x23);
    ret += imx290_write_register(vi_pipe, 0x3480, 0x49);
    ret += imx290_write_register(vi_pipe, 0x3000, 0x00);  /* standby */

    imx290_default_reg_init(vi_pipe);

    delay_ms(1);
    ret += imx290_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    if (ret != OT_SUCCESS) {
        sns_err_trace("imx290 write register failed!\n");
        return;
    }

    printf("================================================================================\n");
    printf("== vi_pipe:%d imx290 37.125MClk 2M60fps(MIPI) 10bit dol-wdr 2to1 init success! ==\n", vi_pipe);
    printf("================================================================================\n");
    return;
}

void imx290_dol_wdr_2to1_2m60_10bit_779m_init(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret = OT_SUCCESS;

    ret += imx290_write_register(vi_pipe, 0x3000, 0x01);
    ret += imx290_write_register(vi_pipe, 0x3002, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3005, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3009, 0x00);
    ret += imx290_write_register(vi_pipe, 0x300A, 0x40);
    ret += imx290_write_register(vi_pipe, 0x300C, 0x11);
    ret += imx290_write_register(vi_pipe, 0x300F, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3010, 0x21);
    ret += imx290_write_register(vi_pipe, 0x3012, 0x64);
    ret += imx290_write_register(vi_pipe, 0x3016, 0x09);
    ret += imx290_write_register(vi_pipe, 0x3018, 0x65);
    ret += imx290_write_register(vi_pipe, 0x3019, 0x04);
    ret += imx290_write_register(vi_pipe, 0x301C, 0x4C);
    ret += imx290_write_register(vi_pipe, 0x301D, 0x04);
    ret += imx290_write_register(vi_pipe, 0x3020, 0x02);
    ret += imx290_write_register(vi_pipe, 0x3021, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3024, 0xC9);
    ret += imx290_write_register(vi_pipe, 0x3025, 0x07);
    ret += imx290_write_register(vi_pipe, 0x3030, 0x0B);
    ret += imx290_write_register(vi_pipe, 0x3031, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3045, 0x05);
    ret += imx290_write_register(vi_pipe, 0x3046, 0x00);
    ret += imx290_write_register(vi_pipe, 0x304B, 0x0A);
    ret += imx290_write_register(vi_pipe, 0x305C, 0x15);
    ret += imx290_write_register(vi_pipe, 0x305D, 0x03);
    ret += imx290_write_register(vi_pipe, 0x305E, 0x20);
    ret += imx290_write_register(vi_pipe, 0x305F, 0x01);
    ret += imx290_write_register(vi_pipe, 0x3070, 0x02);
    ret += imx290_write_register(vi_pipe, 0x3071, 0x11);
    ret += imx290_write_register(vi_pipe, 0x309B, 0x10);
    ret += imx290_write_register(vi_pipe, 0x309C, 0x22);
    ret += imx290_write_register(vi_pipe, 0x30A2, 0x02);
    ret += imx290_write_register(vi_pipe, 0x30A6, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30A8, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30AA, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30AC, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30B0, 0x43);
    ret += imx290_write_register(vi_pipe, 0x3106, 0x11);
    ret += imx290_write_register(vi_pipe, 0x3119, 0x9E);
    ret += imx290_write_register(vi_pipe, 0x311C, 0x1E);
    ret += imx290_write_register(vi_pipe, 0x311E, 0x08);
    ret += imx290_write_register(vi_pipe, 0x3128, 0x05);
    ret += imx290_write_register(vi_pipe, 0x3129, 0x1D);
    ret += imx290_write_register(vi_pipe, 0x313D, 0x83);
    ret += imx290_write_register(vi_pipe, 0x3150, 0x03);
    ret += imx290_write_register(vi_pipe, 0x315E, 0x12);
    ret += imx290_write_register(vi_pipe, 0x3164, 0x1A);
    ret += imx290_write_register(vi_pipe, 0x317C, 0x12);
    ret += imx290_write_register(vi_pipe, 0x317E, 0x00);
    ret += imx290_write_register(vi_pipe, 0x31A0, 0xB4);
    ret += imx290_write_register(vi_pipe, 0x31A1, 0x02);
    ret += imx290_write_register(vi_pipe, 0x31EC, 0x37);
    ret += imx290_write_register(vi_pipe, 0x32B8, 0x50);
    ret += imx290_write_register(vi_pipe, 0x32B9, 0x10);
    ret += imx290_write_register(vi_pipe, 0x32BA, 0x00);
    ret += imx290_write_register(vi_pipe, 0x32BB, 0x04);
    ret += imx290_write_register(vi_pipe, 0x32C8, 0x50);
    ret += imx290_write_register(vi_pipe, 0x32C9, 0x10);
    ret += imx290_write_register(vi_pipe, 0x32CA, 0x00);
    ret += imx290_write_register(vi_pipe, 0x32CB, 0x04);
    ret += imx290_write_register(vi_pipe, 0x332C, 0xD3);
    ret += imx290_write_register(vi_pipe, 0x332D, 0x10);
    ret += imx290_write_register(vi_pipe, 0x332E, 0x0D);
    ret += imx290_write_register(vi_pipe, 0x3358, 0x06);
    ret += imx290_write_register(vi_pipe, 0x3359, 0xE1);
    ret += imx290_write_register(vi_pipe, 0x335A, 0x11);
    ret += imx290_write_register(vi_pipe, 0x3360, 0x1E);
    ret += imx290_write_register(vi_pipe, 0x3361, 0x61);
    ret += imx290_write_register(vi_pipe, 0x3362, 0x10);
    ret += imx290_write_register(vi_pipe, 0x33B0, 0x50);
    ret += imx290_write_register(vi_pipe, 0x33B2, 0x1A);
    ret += imx290_write_register(vi_pipe, 0x33B3, 0x04);
    ret += imx290_write_register(vi_pipe, 0x3405, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3407, 0x03);
    ret += imx290_write_register(vi_pipe, 0x3414, 0x0A);
    ret += imx290_write_register(vi_pipe, 0x3415, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3418, 0x9C);
    ret += imx290_write_register(vi_pipe, 0x3419, 0x08);
    ret += imx290_write_register(vi_pipe, 0x3441, 0x0A);
    ret += imx290_write_register(vi_pipe, 0x3442, 0x0A);
    ret += imx290_write_register(vi_pipe, 0x3443, 0x03);
    ret += imx290_write_register(vi_pipe, 0x3444, 0x20);
    ret += imx290_write_register(vi_pipe, 0x3445, 0x25);
    ret += imx290_write_register(vi_pipe, 0x3446, 0x77);
    ret += imx290_write_register(vi_pipe, 0x3447, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3448, 0x67);
    ret += imx290_write_register(vi_pipe, 0x3449, 0x00);
    ret += imx290_write_register(vi_pipe, 0x344A, 0x37);
    ret += imx290_write_register(vi_pipe, 0x344B, 0x00);
    ret += imx290_write_register(vi_pipe, 0x344C, 0x37);
    ret += imx290_write_register(vi_pipe, 0x344D, 0x00);
    ret += imx290_write_register(vi_pipe, 0x344E, 0x37);
    ret += imx290_write_register(vi_pipe, 0x344F, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3450, 0xDF);
    ret += imx290_write_register(vi_pipe, 0x3451, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3452, 0x37);
    ret += imx290_write_register(vi_pipe, 0x3453, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3454, 0x2F);
    ret += imx290_write_register(vi_pipe, 0x3455, 0x00);
    ret += imx290_write_register(vi_pipe, 0x346A, 0x9C);
    ret += imx290_write_register(vi_pipe, 0x346B, 0x07);
    ret += imx290_write_register(vi_pipe, 0x3472, 0xA0);
    ret += imx290_write_register(vi_pipe, 0x3473, 0x07);
    ret += imx290_write_register(vi_pipe, 0x347B, 0x23);
    ret += imx290_write_register(vi_pipe, 0x3480, 0x49);
    ret += imx290_write_register(vi_pipe, 0x3000, 0x00);

    imx290_default_reg_init(vi_pipe);
    delay_ms(1);

    ret += imx290_write_register(vi_pipe, 0x3002, 0x00);  /* master mode start */
    if (ret != OT_SUCCESS) {
        sns_err_trace("imx290 write register failed!\n");
        return;
    }

    printf("=====================================================================================\n");
    printf("== vi_pipe:%d imx290 37.125MClk 2M60fps(MIPI) 10bit dol-wdr 2to1 779M init success! ==\n", vi_pipe);
    printf("=====================================================================================\n");
}

static void imx290_dol_wdr_2to1_2m30_12bit_init(ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret = OT_SUCCESS;
    ret += imx290_write_register(vi_pipe, 0x3000, 0x01);  /* standby */
    ret += imx290_write_register(vi_pipe, 0x3002, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3005, 0x01);
    ret += imx290_write_register(vi_pipe, 0x3007, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3009, 0x01);
    ret += imx290_write_register(vi_pipe, 0x300A, 0x00);
    ret += imx290_write_register(vi_pipe, 0x300B, 0x01);
    ret += imx290_write_register(vi_pipe, 0x300C, 0x11);
    ret += imx290_write_register(vi_pipe, 0x300F, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3010, 0x21);
    ret += imx290_write_register(vi_pipe, 0x3012, 0x64);
    ret += imx290_write_register(vi_pipe, 0x3016, 0x09);
    ret += imx290_write_register(vi_pipe, 0x3018, 0x65);
    ret += imx290_write_register(vi_pipe, 0x3019, 0x04);
    ret += imx290_write_register(vi_pipe, 0x301C, 0x98);
    ret += imx290_write_register(vi_pipe, 0x301D, 0x08);
    ret += imx290_write_register(vi_pipe, 0x3020, 0x02);
    ret += imx290_write_register(vi_pipe, 0x3021, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3024, 0xC9);
    ret += imx290_write_register(vi_pipe, 0x3025, 0x07);
    ret += imx290_write_register(vi_pipe, 0x3030, 0x0B);
    ret += imx290_write_register(vi_pipe, 0x3031, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3045, 0x05);
    ret += imx290_write_register(vi_pipe, 0x3046, 0x01);
    ret += imx290_write_register(vi_pipe, 0x304B, 0x0A);
    ret += imx290_write_register(vi_pipe, 0x305C, 0x18);
    ret += imx290_write_register(vi_pipe, 0x305D, 0x03);
    ret += imx290_write_register(vi_pipe, 0x305E, 0x20);
    ret += imx290_write_register(vi_pipe, 0x305F, 0x01);
    ret += imx290_write_register(vi_pipe, 0x3070, 0x02);
    ret += imx290_write_register(vi_pipe, 0x3071, 0x11);
    ret += imx290_write_register(vi_pipe, 0x309B, 0x10);
    ret += imx290_write_register(vi_pipe, 0x309C, 0x22);
    ret += imx290_write_register(vi_pipe, 0x30A2, 0x02);
    ret += imx290_write_register(vi_pipe, 0x30A6, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30A8, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30AA, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30AC, 0x20);
    ret += imx290_write_register(vi_pipe, 0x30B0, 0x43);
    ret += imx290_write_register(vi_pipe, 0x3106, 0x11);
    ret += imx290_write_register(vi_pipe, 0x3119, 0x9E);
    ret += imx290_write_register(vi_pipe, 0x311C, 0x1E);
    ret += imx290_write_register(vi_pipe, 0x311E, 0x08);
    ret += imx290_write_register(vi_pipe, 0x3128, 0x05);
    ret += imx290_write_register(vi_pipe, 0x3129, 0x00);
    ret += imx290_write_register(vi_pipe, 0x313D, 0x83);
    ret += imx290_write_register(vi_pipe, 0x3150, 0x03);
    ret += imx290_write_register(vi_pipe, 0x315E, 0x1A);
    ret += imx290_write_register(vi_pipe, 0x3164, 0x1A);
    ret += imx290_write_register(vi_pipe, 0x317C, 0x00);
    ret += imx290_write_register(vi_pipe, 0x317E, 0x00);
    ret += imx290_write_register(vi_pipe, 0x31EC, 0x0E);
    ret += imx290_write_register(vi_pipe, 0x32B8, 0x50);
    ret += imx290_write_register(vi_pipe, 0x32B9, 0x10);
    ret += imx290_write_register(vi_pipe, 0x32BA, 0x00);
    ret += imx290_write_register(vi_pipe, 0x32BB, 0x04);
    ret += imx290_write_register(vi_pipe, 0x32C8, 0x50);
    ret += imx290_write_register(vi_pipe, 0x32C9, 0x10);
    ret += imx290_write_register(vi_pipe, 0x32CA, 0x00);
    ret += imx290_write_register(vi_pipe, 0x32CB, 0x04);
    ret += imx290_write_register(vi_pipe, 0x332C, 0xD3);
    ret += imx290_write_register(vi_pipe, 0x332D, 0x10);
    ret += imx290_write_register(vi_pipe, 0x332E, 0x0D);
    ret += imx290_write_register(vi_pipe, 0x3358, 0x06);
    ret += imx290_write_register(vi_pipe, 0x3359, 0xE1);
    ret += imx290_write_register(vi_pipe, 0x335A, 0x11);
    ret += imx290_write_register(vi_pipe, 0x3360, 0x1E);
    ret += imx290_write_register(vi_pipe, 0x3361, 0x61);
    ret += imx290_write_register(vi_pipe, 0x3362, 0x10);
    ret += imx290_write_register(vi_pipe, 0x33B0, 0x50);
    ret += imx290_write_register(vi_pipe, 0x33B2, 0x1A);
    ret += imx290_write_register(vi_pipe, 0x33B3, 0x04);
    ret += imx290_write_register(vi_pipe, 0x3405, 0x10);
    ret += imx290_write_register(vi_pipe, 0x3407, 0x03);
    ret += imx290_write_register(vi_pipe, 0x3414, 0x0A);
    ret += imx290_write_register(vi_pipe, 0x3415, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3418, 0x9C);
    ret += imx290_write_register(vi_pipe, 0x3419, 0x08);
    ret += imx290_write_register(vi_pipe, 0x3441, 0x0C);
    ret += imx290_write_register(vi_pipe, 0x3442, 0x0C);
    ret += imx290_write_register(vi_pipe, 0x3443, 0x03);
    ret += imx290_write_register(vi_pipe, 0x3444, 0x20);
    ret += imx290_write_register(vi_pipe, 0x3445, 0x25);
    ret += imx290_write_register(vi_pipe, 0x3446, 0x57);
    ret += imx290_write_register(vi_pipe, 0x3447, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3448, 0x37);
    ret += imx290_write_register(vi_pipe, 0x3449, 0x00);
    ret += imx290_write_register(vi_pipe, 0x344A, 0x1F);
    ret += imx290_write_register(vi_pipe, 0x344B, 0x00);
    ret += imx290_write_register(vi_pipe, 0x344C, 0x1F);
    ret += imx290_write_register(vi_pipe, 0x344D, 0x00);
    ret += imx290_write_register(vi_pipe, 0x344E, 0x1F);
    ret += imx290_write_register(vi_pipe, 0x344F, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3450, 0x77);
    ret += imx290_write_register(vi_pipe, 0x3451, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3452, 0x1F);
    ret += imx290_write_register(vi_pipe, 0x3453, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3454, 0x17);
    ret += imx290_write_register(vi_pipe, 0x3455, 0x00);
    ret += imx290_write_register(vi_pipe, 0x3472, 0xA0);
    ret += imx290_write_register(vi_pipe, 0x3473, 0x07);
    ret += imx290_write_register(vi_pipe, 0x347B, 0x23);
    ret += imx290_write_register(vi_pipe, 0x3480, 0x49);
    ret += imx290_write_register(vi_pipe, 0x3000, 0x00);

    imx290_default_reg_init(vi_pipe);
    delay_ms(1);

    ret += imx290_write_register(vi_pipe, 0x3002, 0x00); /* master mode start */
    if (ret != OT_SUCCESS) {
        sns_err_trace("imx290 write register failed!\n");
        return;
    }

    printf("================================================================================\n");
    printf("== vi_pipe:%d imx290 37.125MClk 2M30fps(MIPI) 12bit dol-wdr 2to1 init success! ==\n", vi_pipe);
    printf("================================================================================\n");
}
