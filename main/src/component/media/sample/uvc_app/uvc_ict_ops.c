#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "ot_stream.h"
#include "uvc_media.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_vi_awb_api.h"

#include "uvc_ict_ops.h"
#include "sample_comm.h"

#define USE_CONTRAST_AS_MJPEG_QFACTOR               (1)
#define USE_BACKLIGHT_COMPENSATION_AS_SENSOR_MODE   (1)

#if USE_BACKLIGHT_COMPENSATION_AS_SENSOR_MODE

#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ot_eis_i2c.h"

#define IMX582_I2C_ADDR          0x34
#define IMX582_ADDR_BYTE         2
#define IMX582_DATA_BYTE         1
#define IMX582_I2C_BUS           5

static unsigned short g_imx582_normal_seq[][2] = {
    { 0x0100, 0x00},  // Software Standby
    { 0x3040, 0x00},  // Set as input
    { 0x3041, 0x01},  // Master Mode
    { 0x3F71, 0x00},  // Output Disable
    // Strobe light timing setting
    { 0x0c12, 0x01},
    { 0x0c14, 0x00},
    { 0x0c15, 0x00},
    { 0x0c18, 0x00},
    { 0x0c19, 0x01},
    { 0x0c1a, 0x00},
    { 0x0c1b, 0x00},
    { 0x0100, 0x01},  //Software Start
};

static unsigned short g_imx582_master_seq[][2] = {
    { 0x0100, 0x00},  //Software Standby
    { 0x3040, 0x01},  //Output For Master Mode
    { 0x3041, 0x01},  //Master Mode
    { 0x3F71, 0x01},  //Output Enable
    { 0x3050, 0x01},
    { 0x4B82, 0x07},
    { 0x4B83, 0x00},  //XVS Out Polarity Low active
    // Strobe light timing setting
    { 0x0c12, 0x80},
    { 0x0c14, 0x06},
    { 0x0c15, 0xdc},
    { 0x0c18, 0x03},
    { 0x0c19, 0xe8},
    { 0x0c1a, 0x21},
    { 0x0c1b, 0x01},
    { 0x0100, 0x01},  //Software Start
};
#if 1
static unsigned short g_imx582_slave_seq[][2] = {
    { 0x0100, 0x00},  //Software Standby
    { 0x3040, 0x00},  //Input For Slave Mode
    { 0x3041, 0x00},  //Slave Mode
    { 0x3F71, 0x00},  //Output Disable
    { 0x3050, 0x01},
    { 0x4B85, 0x00},  //XVS In Polarity Low active
    // Strobe light timing setting
    { 0x0c12, 0x80},
    { 0x0c14, 0x06},
    { 0x0c15, 0xdc},
    { 0x0c18, 0x03},
    { 0x0c19, 0xe8},
    { 0x0c1a, 0x21},
    { 0x0c1b, 0x01},
    { 0x0100, 0x01},  //Software Start
};
#else
static unsigned short g_imx582_slave_seq[][2] = {
    { 0x0100, 0x00},  //Software Standby
    { 0x3F70, 0x01},
    { 0x3040, 0x00},  //Input For Slave Mode
    { 0x3041, 0x00},  //Slave Mode
    { 0x3F79, 0x00},  //  PRSH_LENGTH_LINES ≥ COARSE_INTEG_TIME+34
	{ 0x3F7A, 0x02},  //
	{ 0x3F7B, 0xFA},  //
	{ 0x3F68, 0x02},  //  line Delay from XVS  = PRSH_LENGTH_LINES
	{ 0x3F69, 0xFA},  //
	{ 0x3F6A, 0x00},  //  VTPXCK
	{ 0x3F6B, 0x00},  //
	{ 0x3F6E, 0x00},  //
	{ 0x3F6F, 0x00},  //
    { 0x3F71, 0x00},  //Output Disable
    { 0x0350, 0x01},
    { 0x4B85, 0x00},  //XVS In Polarity Low active
    // Strobe light timing setting
    { 0x0c12, 0x80},
    { 0x0c14, 0x06},
    { 0x0c15, 0xdc},
    { 0x0c18, 0x03},
    { 0x0c19, 0xe8},
    { 0x0c1a, 0x21},
    { 0x0c1b, 0x01},
    { 0x0100, 0x01},  //Software Start
};
#endif

#define I2C_DEV_PATH_LEN     16
#define I2C_BUF_LEN          8

static int g_mode[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = 0}; // 0: normal, 1: master, 2: slave

static int g_fd[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = -1};
static int gs_wb_temperature[OT_UVC_MAX_CHN_NUM] = {[0 ...(OT_UVC_MAX_CHN_NUM - 1)] = 6500};

static int imx582_i2c_init(ot_vi_pipe_id vi_pipe)
{
    int  ret;
    char dev_file[I2C_DEV_PATH_LEN] = {0};
    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", IMX582_I2C_BUS);

    g_fd[vi_pipe] = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        sns_err_trace("Open /dev/ot_i2c_drv-%u error!\n", IMX582_I2C_BUS);
        return OT_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], OT_EIS_I2C_SLAVE_FORCE, (IMX582_I2C_ADDR >> 1));
    if (ret < 0) {
        sns_err_trace("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }
    return OT_SUCCESS;
}

static int imx582_i2c_exit(ot_vi_pipe_id vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return OT_SUCCESS;
    }
    return OT_FAILURE;
}

static ot_s32 imx582_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data)
{
    ot_s32 ret = OT_SUCCESS;

    if (g_fd[vi_pipe] < 0) {
        return OT_FAILURE;
    }

    ot_u32 idx = 0;
    ot_u8  buf[I2C_BUF_LEN];

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

    ret = write(g_fd[vi_pipe], buf, IMX582_ADDR_BYTE + IMX582_DATA_BYTE);
    if (ret < 0) {
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

uint16_t sample_uvc_pu_backlight_compensation_get(uint32_t dev_no)
{
    ot_vi_pipe_id vi_pipe = 0;
    return g_mode[vi_pipe];
}

ot_void sample_uvc_pu_backlight_compensation_set(uint32_t dev_no, uint16_t v)
{
    ot_vi_pipe_id vi_pipe = 0;
    /* slave mode (v==2) disabled per business req: IMX582 must not use slave mode.
       clamp to normal(0)/master(1); case 2 (g_imx582_slave_seq) stays unreachable. */
    if (v > 2) {
        printf("func:%s invalid mode:0x%x (slave disabled)\n", __func__, v);
        return;
    }
    if (v == g_mode[vi_pipe]) {
        return;
    }

    imx582_i2c_init(vi_pipe);
    switch(v) {
        case 0:
        for (int i = 0; i < sizeof(g_imx582_normal_seq) / sizeof(g_imx582_normal_seq[0]); i++) {
            imx582_write_register(vi_pipe, g_imx582_normal_seq[i][0], g_imx582_normal_seq[i][1]);
        }
        break;
        case 1:
        for (int i = 0; i < sizeof(g_imx582_master_seq) / sizeof(g_imx582_master_seq[0]); i++) {
            imx582_write_register(vi_pipe, g_imx582_master_seq[i][0], g_imx582_master_seq[i][1]);
        }
        break;
        case 2:
        for (int i = 0; i < sizeof(g_imx582_slave_seq) / sizeof(g_imx582_slave_seq[0]); i++) {
            imx582_write_register(vi_pipe, g_imx582_slave_seq[i][0], g_imx582_slave_seq[i][1]);
        }
        break;
    }
    imx582_i2c_exit(vi_pipe);

    g_mode[vi_pipe] = v;
}
#else
uint16_t sample_uvc_pu_backlight_compensation_get(uint32_t dev_no)
{
    return 0;
}

ot_void sample_uvc_pu_backlight_compensation_set(uint32_t dev_no, uint16_t v)
{
    printf("func:%s data:0x%x\n", __func__, v);
}
#endif

ot_void sample_uvc_ct_exposure_auto_mode_set(uint32_t dev_no, uint8_t v)
{
    ot_eis_vi_exposure_attr attr;
    ot_s32 ret = OT_FAILURE;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_ae_get_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_get_exposure_attr %d\n", __func__, ret);
        return;
    }
    
    attr.op_type = (v == 0x08) ? OT_EIS_OP_MODE_AUTO : OT_EIS_OP_MODE_MANUAL;
    attr.manual_attr.exp_time_op_type  = attr.op_type;
    attr.manual_attr.a_gain_op_type    = attr.op_type;
    attr.manual_attr.d_gain_op_type    = attr.op_type;
    attr.manual_attr.ispd_gain_op_type = attr.op_type;
    ret = ot_eis_vi_ae_set_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_set_exposure_attr %d\n", __func__, ret);
        return;
    }
}

ot_void sample_uvc_ct_exposure_absolute_time_set(uint32_t dev_no, uint32_t v)
{
    ot_eis_vi_exposure_attr attr;
    ot_s32 ret = OT_FAILURE;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_ae_get_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_get_exposure_attr %d\n", __func__, ret);
        return ;
    }

    ot_u32 exp_time = v;

    printf("%s: requested exposure %u us, sensor minimum %u us, sensor maximum %u us\n",
    __func__, exp_time, attr.auto_attr.exp_time_range.min, attr.auto_attr.exp_time_range.max);

   
    if (exp_time < attr.auto_attr.exp_time_range.min) {
        printf("%s: requested exposure %u us is below sensor range, using sensor minimum %u us\n",
            __func__, exp_time, attr.auto_attr.exp_time_range.min);
        exp_time = attr.auto_attr.exp_time_range.min;
    } else if (exp_time > attr.auto_attr.exp_time_range.max) {
        printf("%s: requested exposure %u us exceeds sensor range, using sensor maximum %u us\n",
            __func__, exp_time, attr.auto_attr.exp_time_range.max);
        exp_time = attr.auto_attr.exp_time_range.max;
    }
    

    attr.manual_attr.exp_time_op_type = OT_EIS_OP_MODE_MANUAL;
    attr.manual_attr.a_gain_op_type    = attr.op_type;
    attr.manual_attr.d_gain_op_type    = attr.op_type;
    attr.manual_attr.ispd_gain_op_type = attr.op_type;
    attr.manual_attr.exp_time = v;
    ret = ot_eis_vi_ae_set_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_set_exposure_attr %d\n", __func__, ret);
        return;
    }
}

uint8_t sample_uvc_ct_exposure_auto_mode_get(uint32_t dev_no)
{
    ot_eis_vi_exposure_attr attr;
    ot_u8 ret;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_ae_get_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_get_exposure_attr %d\n", __func__, ret);
        return 0x8; // Default to auto mode
    }

    return (attr.op_type == OT_EIS_OP_MODE_AUTO ? 0x8 : 0x1);
}

uint32_t sample_uvc_ct_exposure_absolute_time_get(uint32_t dev_no)
{
    ot_eis_vi_exposure_attr attr;
    ot_s32 ret = OT_FAILURE;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_ae_get_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_get_exposure_attr %d\n", __func__, ret);
        return 0x05;
    }
    
    return (attr.manual_attr.exp_time_op_type == OT_EIS_OP_MODE_MANUAL) ? attr.manual_attr.exp_time : 2000;
}

uint16_t sample_uvc_pu_brightness_get(uint32_t dev_no)
{
    ot_eis_vi_csc_attr attr;
    ot_s32 ret = OT_FAILURE;

    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_csc_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return 50;
    }
    
    return attr.luma;
}

uint16_t sample_uvc_pu_contrast_get(uint32_t dev_no)
{
    ot_s32 ret = OT_FAILURE;
#if USE_CONTRAST_AS_MJPEG_QFACTOR
    ot_eis_venc_mjpeg_rc_param param;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);
    ret = ot_eis_venc_get_mjpeg_rc_param(ctx->ve_cfg.venc_hdl, &param);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_venc_get_mjpeg_rc_param %d\n", __func__, ret);
        return 50;
    }

    return param.cbr_param.max_qfactor;
#else
    ot_eis_vi_csc_attr attr;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_csc_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return 50;
    }
    return attr.contr;
#endif
}

ot_void sample_uvc_pu_contrast_set(uint32_t dev_no, uint16_t v)
{
    ot_s32 ret = OT_FAILURE;
#if USE_CONTRAST_AS_MJPEG_QFACTOR
    ot_eis_venc_mjpeg_rc_param param;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);
    v += 20;        //2026.10.30 这里的修改对比度的值，已经变成修改MJPG的压缩比，增加以下压缩的程度
    printf("555555500001   v=%lu\n",v);     //2026.10.30 这里的修改对比度的值，已经变成修改MJPG的压缩比，这里打印一下
    ret = ot_eis_venc_get_mjpeg_rc_param(ctx->ve_cfg.venc_hdl, &param);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_venc_get_mjpeg_rc_param %d\n", __func__, ret);
        return;
    }
    v = (v > 99) ? 99 : v;
    v = (v <  1) ?  1 : v;
    param.cbr_param.max_qfactor = v;
    param.cbr_param.min_qfactor = (v - 10 > 1) ? (v - 10) : 1;
    printf("555555500001   param.cbr_param.max_qfactor=%lu\n",param.cbr_param.max_qfactor);     //2026.10.30 这里的修改对比度的值，已经变成修改MJPG的压缩比，这里打印一下
    printf("555555500001   param.cbr_param.min_qfactor=%lu\n",param.cbr_param.min_qfactor);     //2026.10.30 这里的修改对比度的值，已经变成修改MJPG的压缩比，这里打印一下
    ret = ot_eis_venc_set_mjpeg_rc_param(ctx->ve_cfg.venc_hdl, &param);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_venc_set_mjpeg_rc_param %d\n", __func__, ret);
        return;
    }
#else
    ot_eis_vi_csc_attr attr;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_csc_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return;
    }
    attr.contr = (ot_u8)v;
    ret = ot_eis_vi_csc_set_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return;
    }
#endif
}

uint16_t sample_uvc_pu_gain_get(uint32_t dev_no)
{
    ot_eis_vi_exposure_attr attr;
    ot_s32 ret = OT_FAILURE;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_ae_get_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_get_exposure_attr %d\n", __func__, ret);
        return 0x01;
    }
    if (attr.op_type != OT_EIS_OP_MODE_MANUAL) {
        return 0x01;
    }

    ot_u32 a_gain = attr.manual_attr.a_gain / 0x400; 
    a_gain = (a_gain > 24) ? 24 : a_gain;

    return (ot_u16)a_gain;
}

uint16_t sample_uvc_pu_hue_get(uint32_t dev_no)
{
    ot_eis_vi_csc_attr attr;
    ot_s32 ret = OT_FAILURE;

    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_csc_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return 50;
    }
    return attr.hue;
}

uint8_t sample_uvc_pu_power_line_frequency_get(uint32_t dev_no)
{
    ot_eis_vi_exposure_attr attr;
    ot_u8 ret = OT_FAILURE;

    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_ae_get_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return 0;
    }

    if (attr.op_type != OT_EIS_OP_MODE_AUTO) {
        ret = 0;
    }
    else if (attr.auto_attr.antiflicker.enable == OT_FALSE) {
        ret = 0;
    }
    else if (attr.auto_attr.antiflicker.mode == OT_EIS_VI_ANTIFLICKER_AUTO_MODE) {
        ret = 3;
    }
    else if (attr.auto_attr.antiflicker.frequency == 50) {
        ret = 1;
    }
    else if (attr.auto_attr.antiflicker.frequency == 60) {
        ret = 2;
    }
    else {
        ret = 0;
    }

    return ret;
}

ot_void sample_uvc_pu_white_balance_temperature_auto_set(uint32_t dev_no, uint8_t v)
{
    ot_s32 ret = OT_FAILURE;
    ot_eis_vi_awb_attr attr;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_awb_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_awb_get_attr %d\n", __func__, ret);
        return;
    }

    attr.op_type = (v == 0) ? OT_EIS_OP_MODE_MANUAL : OT_EIS_OP_MODE_AUTO;
    ret = ot_eis_vi_awb_set_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_awb_set_attr %d\n", __func__, ret);
        return;
    }
}

extern ot_s32 ot_scene_set_manual_wb_gain_by_ct(ot_vi_pipe_handle pipe_hdl, ot_s32 ct);

ot_void sample_uvc_pu_white_balance_temperature_set(uint32_t dev_no, uint16_t v)
{
    ot_s32 ret = OT_FAILURE;
    ot_eis_vi_awb_attr attr;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_awb_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_awb_get_attr %d\n", __func__, ret);
        return;
    }
    if (attr.op_type == OT_EIS_OP_MODE_MANUAL) {
        ot_scene_set_manual_wb_gain_by_ct(ctx->vi_cfg.pipe_info[0].pipe_hdl, v);
        gs_wb_temperature[dev_no] = v;
    }
}


ot_void sample_uvc_pu_brightness_set(uint32_t dev_no, uint16_t v)
{
    ot_eis_vi_csc_attr attr;
    ot_s32 ret = OT_FAILURE;

    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_csc_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return;
    }
    attr.luma = (ot_u8)v;
    ret = ot_eis_vi_csc_set_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return;
    }
}


ot_void sample_uvc_pu_gain_set(uint32_t dev_no, uint16_t v)
{
    ot_eis_vi_exposure_attr attr;
    ot_s32 ret = OT_FAILURE;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_ae_get_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_get_exposure_attr %d\n", __func__, ret);
        return;
    }
    if (attr.op_type != OT_EIS_OP_MODE_MANUAL) {
        printf("%s attr.op_type != OT_EIS_OP_MODE_MANUAL\n", __func__);
        return;
    }
    attr.manual_attr.a_gain_op_type = OT_EIS_OP_MODE_MANUAL;
    attr.manual_attr.a_gain = 0x400 * v;
    attr.manual_attr.d_gain_op_type = OT_EIS_OP_MODE_MANUAL;
    attr.manual_attr.d_gain = 0x400;
    attr.manual_attr.ispd_gain_op_type = OT_EIS_OP_MODE_MANUAL;
    attr.manual_attr.isp_d_gain = 0x400;
    ret = ot_eis_vi_ae_set_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_set_exposure_attr %d\n", __func__, ret);
        return;
    }
}

ot_void sample_uvc_pu_hue_set(uint32_t dev_no, uint16_t v)
{
    ot_eis_vi_csc_attr attr;
    ot_s32 ret = OT_FAILURE;

    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_csc_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return;
    }
    attr.hue = (ot_u8)v;
    ret = ot_eis_vi_csc_set_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return;
    }
}

ot_void sample_uvc_pu_power_line_frequency_set(uint32_t dev_no, uint8_t v)
{
    ot_eis_vi_exposure_attr attr;
    ot_s32 ret = OT_FAILURE;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_ae_get_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_get_exposure_attr %d\n", __func__, ret);
        return;
    }

    switch (v) {
    case 0:
        attr.auto_attr.antiflicker.enable = OT_FALSE;
        break;
    case 1:
        attr.auto_attr.antiflicker.enable = OT_TRUE;
        attr.auto_attr.antiflicker.mode = OT_EIS_VI_ANTIFLICKER_NORMAL_MODE;
        attr.auto_attr.antiflicker.frequency = 50;
        break;
    case 2:
        attr.auto_attr.antiflicker.enable = OT_TRUE;
        attr.auto_attr.antiflicker.mode = OT_EIS_VI_ANTIFLICKER_NORMAL_MODE;
        attr.auto_attr.antiflicker.frequency = 60;
        break;
    case 3:
        attr.auto_attr.antiflicker.enable = OT_TRUE;
        attr.auto_attr.antiflicker.mode = OT_EIS_VI_ANTIFLICKER_AUTO_MODE;
        break;
    }

    ret = ot_eis_vi_ae_set_exposure_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_set_exposure_attr %d\n", __func__, ret);
        return;
    }
}

ot_void sample_uvc_pu_saturation_set(uint32_t dev_no, uint16_t v)
{
    ot_eis_vi_csc_attr attr;
    ot_s32 ret = OT_FAILURE;

    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_csc_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return;
    }
    attr.satu = (ot_u8)v;
    ret = ot_eis_vi_csc_set_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return;
    }
}


uint8_t sample_uvc_pu_white_balance_temperature_auto_get(uint32_t dev_no)
{
    ot_s32 ret = OT_FAILURE;
    ot_eis_vi_awb_attr attr;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_awb_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_awb_get_attr %d\n", __func__, ret);
        return 0x02;
    }

    return (attr.op_type == OT_EIS_OP_MODE_AUTO) ? 0x01 : 0x00;
}

uint16_t sample_uvc_pu_white_balance_temperature_get(uint32_t dev_no)
{
    return gs_wb_temperature[dev_no];
}


uint16_t sample_uvc_pu_saturation_get(uint32_t dev_no)
{
    ot_eis_vi_csc_attr attr;
    ot_s32 ret = OT_FAILURE;

    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    ret = ot_eis_vi_csc_get_attr(ctx->vi_cfg.pipe_info[0].pipe_hdl, &attr);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_csc_get_attr %d\n", __func__, ret);
        return 50;
    }
    
    return attr.satu;
}
