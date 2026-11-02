/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include "ot_eis_errno.h"
#include "ot_eis_vi_sns.h"
#include "imx623_cmos_ex.h"
#include "imx623_cmos.h"
#include "imx623_cfg.h"
#include "ot_eis_vi_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_awb_api.h"


#define IMX623_ID                    8

#define high_h_h_8bits(x) (((x) & 0xff000000) >> 24)
#define high_h_8bits(x)  (((x) & 0x00ff0000)>>16)
#define higher_2bits(x) (((x) & 0x30000) >> 16)
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)

#define MAX2(x, y)       ((x) > (y) ? (x) : (y))
#define MIN2(x, y)       ((x) < (y) ? (x) : (y))

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define imx623_sensor_set_ctx(pipe, ctx)   ((g_imx623_sns_state[pipe]) = (ctx))
#define imx623_sensor_reset_ctx(pipe)      (g_imx623_sns_state[pipe] = OT_NULL)

static ot_eis_vi_fswdr_mode g_fswdr_mode[OT_EIS_VI_MAX_PIPE_NUM] = {
    [0 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = OT_EIS_VI_FSWDR_NORMAL_MODE
};

static ot_u32 g_max_time_get_cnt[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_u32 g_init_exposure[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_init_int_time[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_init_again[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_init_dgain[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_init_isp_dgain[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_lines_per500ms[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_u8 g_ae_stat_pos[OT_EIS_VI_MAX_PIPE_NUM] = {0};

static ot_u16 g_init_wb_gain[OT_EIS_VI_MAX_PIPE_NUM][OT_EIS_VI_RGB_CHN_NUM] = {{0}};
static ot_u16 g_sample_r_gain[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_u16 g_sample_b_gain[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_bool g_quick_start_en[OT_EIS_VI_MAX_PIPE_NUM] = {OT_FALSE};

static ot_bool g_ae_route_ex_valid[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_eis_vi_ae_route g_init_ae_route[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_vi_ae_route_ex g_init_ae_route_ex[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_vi_ae_route g_init_ae_route_sf[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_vi_ae_route_ex g_init_ae_route_sf_ex[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};

static ot_eis_vi_sns_alg_default  g_ext_sns_alg[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_vi_awb_sensor_default g_ext_sns_awb[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_sns_reg_table  g_ext_table[OT_EIS_VI_MAX_PIPE_NUM][5] = {{0}};
static ot_u8  g_ext_alg_flag[OT_EIS_VI_MAX_PIPE_NUM] = { 0 };
static ot_u8  g_ext_awb_flag[OT_EIS_VI_MAX_PIPE_NUM] = { 0 };
static ot_u8  g_ext_sensor_table_flag[OT_EIS_VI_MAX_PIPE_NUM] = { 0 };

ot_eis_sns_bus_info g_imx623_bus_info[OT_EIS_VI_MAX_PIPE_NUM] = {
        [0] = {
            .com_bus.i2c_dev = 0,
            .bus_addr = IMX623_I2C_ADDR
        },
        [1 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = {
            .com_bus.i2c_dev = -1,
            .bus_addr = IMX623_I2C_ADDR
        }
};

ot_eis_sns_state *g_imx623_sns_state[OT_EIS_VI_MAX_PIPE_NUM] = {OT_NULL};

static ot_bool blc_clamp_info[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = OT_TRUE};

ot_eis_sns_reg_table *imx623_get_ext_sensor_table(ot_vi_pipe_id vi_pipe)
{
    return &g_ext_table[vi_pipe][0];
}

ot_u8 imx623_get_ext_sensor_flag(ot_vi_pipe_id vi_pipe)
{
    return g_ext_sensor_table_flag[vi_pipe];
}

ot_eis_sns_bus_info *imx623_get_bus_info(ot_vi_pipe_id vi_pipe)
{
    return &g_imx623_bus_info[vi_pipe];
}

ot_eis_sns_state *imx623_get_ctx(ot_vi_pipe_id vi_pipe)
{
    return g_imx623_sns_state[vi_pipe];
}

ot_void imx623_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en)
{
    blc_clamp_info[vi_pipe] = clamp_en;
}

const imx623_video_mode_tbl g_imx623_mode_tbl[IMX623_MODE_BUTT] = {
    {IMX623_VMAX_1552P30_LINEAR,   IMX623_FULL_LINES_MAX,          30, 0.8,
     1936, 1552, 0, OT_EIS_WDR_MODE_NONE,       "IMX623_SENSOR_1552P_30FPS_LINEAR_MODE"},
    {IMX623_VMAX_1552P30_BUILT_IN_WDR,  IMX623_FULL_LINES_MAX_BUILT_IN_WDR, 60, 0.5,
     1936, 1552, 0, OT_EIS_WDR_MODE_BUILT_IN,  "IMX623_SENSOR_1552P_30FPS_WDR_built_in_MODE"},
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* IMX623 Register Address */
#define IMX623_SHT_SP1_LL                       (0x9c08) /*  SHT_SP1[7:0]  */
#define IMX623_SHT_SP1_LH                       (0x9c09) /*  SHT_SP1[15:8]  */
#define IMX623_SHT_SP1_HL                       (0x9c0a) /*  SHT_SP1[23:16]  */
#define IMX623_SHT_SP1_HH                       (0x9c0b) /*  SHT_SP1[31:24]  */

#define IMX623_SHT_SP2_LL                       (0x9c10) /*  SHT_SP2[7:0]  */
#define IMX623_SHT_SP2_LH                       (0x9c11) /*  SHT_SP2[15:8]  */
#define IMX623_SHT_SP2_HL                       (0x9c12) /*  SHT_SP2[23:16]  */
#define IMX623_SHT_SP2_HH                       (0x9c13) /*  SHT_SP2[31:24]  */
#define IMX623_AGAIN_SP1_H_L                   (0x9c18) /*  AGAIN_SP1_H[7:0] */
#define IMX623_AGAIN_SP1_H_H                   (0x9c19) /*  AGAIN_SP1_H[9:8] */
#define IMX623_AGAIN_SP1_L_L                   (0x9c1a) /*  AGAIN_SP1_L[7:0] */
#define IMX623_AGAIN_SP1_L_H                   (0x9c1b) /*  AGAIN_SP1_L[9:8] */

#define IMX623_AGAIN_SP2_H_L                   (0x9c1c) /*  AGAIN_SP2_H[7:0] */
#define IMX623_AGAIN_SP2_H_H                   (0x9c1d) /*  AGAIN_SP2_H[9:8] */
#define IMX623_AGAIN_SP2_L_L                   (0x9c1e) /*  AGAIN_SP2_L[7:0] */
#define IMX623_AGAIN_SP2_L_H                   (0x9c1f) /*  AGAIN_SP2_L[9:8] */
#define IMX623_DGAIN_SP12_L                    (0x9c20) /*  DGAIN_SP12[7:0] */
#define IMX623_DGAIN_SP12_H                    (0x9c21) /*  DGAIN_SP12[9:8] */

#define IMX623_DGAIN_SP1_H_L                   (0xae84) /*  DGAIN_SP1_H[7:0] */
#define IMX623_DGAIN_SP1_H_H                   (0xae85) /*  DGAIN_SP1_H[15:8] */
#define IMX623_DGAIN_SP1_L_L                   (0xae88) /*  DGAIN_SP1_L[7:0] */
#define IMX623_DGAIN_SP1_L_H                   (0xae89) /*  DGAIN_SP1_L[15:8] */

#define IMX623_DGAIN_SP2_H_L                   (0xae8c) /*  DGAIN_SP2_H[7:0] */
#define IMX623_DGAIN_SP2_H_H                   (0xae8d) /*  DGAIN_SP2_H[16:8] */
#define IMX623_DGAIN_SP2_L_L                   (0xae90) /*  DGAIN_SP2_L[7:0] */
#define IMX623_DGAIN_SP2_L_H                   (0xae91) /*  DGAIN_SP2_L[16:8] */

#define IMX623_VMAX_OFFSET_L                   (0x8a70) /* VMAX_OFFSET[7:0] */
#define IMX623_VMAX_OFFSET_M                   (0x8a71) /* VMAX_OFFSET[15:8] */
#define IMX623_VMAX_OFFSET_H                   (0x8a72) /* VMAX_OFFSET[17:16] */

#define IMX623_VMAX_L                           (0x8a74) /* VMAX_OFFSET[7:0] */
#define IMX623_VMAX_M                           (0x8a75) /* VMAX_OFFSET[15:8] */
#define IMX623_VMAX_H                           (0x8a76) /* VMAX_OFFSET[17:16] */


#define imx623_err_mode_print(sns_image_mode, sns_state) \
do { \
    sns_err_trace("Not support! Width:%u, Height:%u, Fps:%f, WDRMode:%d\n", \
                  (sns_image_mode)->width, \
                  (sns_image_mode)->height, \
                  (sns_image_mode)->fps, \
                  (sns_state)->wdr_mode); \
} while (0)

static ot_void cmos_get_ae_comm_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft,
    const ot_eis_sns_state *sns_state)
{
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->full_lines_max = IMX623_FULL_LINES_MAX;
    ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 60); /* 1000000000ns, 60fps */


    ae_sns_dft->again_accu.accu_type = OT_EIS_VI_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = OT_EIS_VI_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = 1;

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 4 << ae_sns_dft->isp_dgain_shift; /* max 2 */
    if (g_lines_per500ms[vi_pipe] == 0) {
        ae_sns_dft->lines_per500ms = sns_state->fl_std * 60 / 2; /* 60fps, div 2 */
    } else {
        ae_sns_dft->lines_per500ms = g_lines_per500ms[vi_pipe];
    }
    ae_sns_dft->max_iris_fno = OT_EIS_VI_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_fno = OT_EIS_VI_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid = OT_FALSE;
    ae_sns_dft->ae_route_attr.total_num = 0;
    ae_sns_dft->ae_route_attr_ex.total_num = 0;
    ae_sns_dft->quick_start.quick_start_enable = g_quick_start_en[vi_pipe];
    ae_sns_dft->quick_start.black_frame_num = 0;
    ae_sns_dft->ae_stat_pos = g_ae_stat_pos[vi_pipe]; /* 1 use be stat to AE */
    return;
}

static ot_void cmos_get_ae_linear_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft,
    const ot_eis_sns_state *sns_state)
{
    ae_sns_dft->int_time_accu.accu_type = OT_EIS_VI_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1; /* accuracy 1 */
    ae_sns_dft->int_time_accu.offset = 0;
    ae_sns_dft->max_again = 32768; /* max 32768 */
    ae_sns_dft->min_again = 1024;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 2048; /* max 257217 */
    ae_sns_dft->min_dgain = 1024; /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 4 << ae_sns_dft->isp_dgain_shift; /* max 255 */
    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode = OT_EIS_VI_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 76151; /* init 76151 */
    ae_sns_dft->init_int_time = g_init_int_time[vi_pipe];
    ae_sns_dft->init_again = g_init_again[vi_pipe];
    ae_sns_dft->init_dgain = g_init_dgain[vi_pipe];
    ae_sns_dft->init_isp_dgain = g_init_isp_dgain[vi_pipe];

    ae_sns_dft->max_int_time = sns_state->fl_std - 8; /* sub 8 */
    ae_sns_dft->min_int_time = 4; /* min 2 */
    ae_sns_dft->max_int_time_target = 65535; /* max int 65535 */
    ae_sns_dft->min_int_time_target = 1;

    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[vi_pipe];
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_eis_vi_ae_route),
                      &g_init_ae_route[vi_pipe],  sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_eis_vi_ae_route_ex),
                      &g_init_ae_route_ex[vi_pipe], sizeof(ot_eis_vi_ae_route_ex));

    return;
}

static ot_void cmos_get_ae_built_in_wdr_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft,
    const ot_eis_sns_state *sns_state)
{
    ae_sns_dft->int_time_accu.accu_type = OT_EIS_VI_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1; /* accuracy 1 */
    ae_sns_dft->int_time_accu.offset = 0;

    ae_sns_dft->max_again = 32768; /* max 32768 */
    ae_sns_dft->min_again = 8134; /* min 1024 */          //18db
    ae_sns_dft->max_again_target = 32768;
    ae_sns_dft->min_again_target = 1024;

    ae_sns_dft->max_dgain = 6144; /* max 257217 */
    ae_sns_dft->min_dgain = 1024; /* min 1024 */
    ae_sns_dft->max_dgain_target = 16384;
    ae_sns_dft->min_dgain_target = 1024;

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 255 << ae_sns_dft->isp_dgain_shift; /* max 255 */

    ae_sns_dft->ae_compensation = 0x1E;                                                  //TBD
    ae_sns_dft->ae_exp_mode = OT_EIS_VI_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 76151; /* init 76151 */

    ae_sns_dft->max_int_time = sns_state->fl_std - 8; /* sub 8 */
    ae_sns_dft->min_int_time = 4; /* min 4 */
    ae_sns_dft->max_int_time_target = 65535; /* max 65535 */
    ae_sns_dft->min_int_time_target = 1; /* min 1 */
    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[vi_pipe];
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_eis_vi_ae_route),
                      &g_init_ae_route[vi_pipe],  sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_eis_vi_ae_route_ex),
                      &g_init_ae_route_ex[vi_pipe], sizeof(ot_eis_vi_ae_route_ex));
    return;
}

static ot_s32 cmos_get_ae_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(ae_sns_dft);
    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (ot_void)memset_s(&ae_sns_dft->ae_route_attr, sizeof(ot_eis_vi_ae_route), 0, sizeof(ot_eis_vi_ae_route));

    cmos_get_ae_comm_default(vi_pipe, ae_sns_dft, sns_state);

    switch (sns_state->wdr_mode) {
        case OT_EIS_WDR_MODE_NONE:   /* linear mode */
            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
            break;

        case OT_EIS_WDR_MODE_BUILT_IN:
            cmos_get_ae_built_in_wdr_default(vi_pipe, ae_sns_dft, sns_state);
            break;
        //case OT_EIS_WDR_MODE_BUILT_IN:
            //cmos_get_ae__default(vi_pipe, ae_sns_dft, sns_state);
            //break;

        default:
            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
            break;
    }

    return OT_SUCCESS;
}

static ot_void cmos_config_vmax(ot_eis_sns_state *sns_state, ot_u32 vmax)
{
    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[24].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[25].data = high_8bits(vmax);
        sns_state->regs_info[0].i2c_data[26].data = higher_2bits(vmax); /* array index 7 */
    }  else if(sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
        sns_state->regs_info[0].i2c_data[18].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[19].data = high_8bits(vmax);
        sns_state->regs_info[0].i2c_data[20].data = higher_2bits(vmax); /* array index 13 */
    } else {
			printf("invalid wdr_mode:%d\n",sns_state->wdr_mode);
		}

    return;
}

static ot_void cmos_fps_set(ot_vi_pipe_id vi_pipe, ot_float fps, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    ot_u32 lines, lines_max, vmax,vmax_offset;
    ot_bool achieve_fps_flag;
    ot_float max_fps, min_fps;
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_void_return(ae_sns_dft);
    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines = g_imx623_mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = g_imx623_mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = g_imx623_mode_tbl[sns_state->img_mode].max_fps;
    min_fps = g_imx623_mode_tbl[sns_state->img_mode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        sns_err_trace("Not support Fps: %f\n", fps);
        return;
    }

    achieve_fps_flag = OT_TRUE;
    vmax = (ot_u32)(lines * max_fps / sns_div_0_to_1_float(fps));
    vmax = (vmax > lines_max) ? lines_max : vmax;
    vmax = vmax + vmax % 2; /* mod 2 */
    vmax_offset = vmax - g_imx623_mode_tbl[sns_state->img_mode].ver_lines;
    cmos_config_vmax(sns_state, vmax_offset);

    sns_state->fl_std = vmax;
    ae_sns_dft->lines_per500ms = lines * max_fps / 2; /* div 2 */
    g_lines_per500ms[vi_pipe] = ae_sns_dft->lines_per500ms;

    ae_sns_dft->fps = lines * max_fps * 0x40 / vmax / 0x40;
    ae_sns_dft->fps = (achieve_fps_flag) ? fps : (ae_sns_dft->fps);
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->max_int_time = sns_state->fl_std - 8; /* sub 8 */
    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times =
        (ot_u32)((1000000000) / (sns_state->fl_std * sns_div_0_to_1_float(fps))); /* 1000000000ns */

    return;
}

static ot_void cmos_slow_framerate_set(ot_vi_pipe_id vi_pipe, ot_u32 full_lines, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    ot_u32 lines_max, vmax,vmax_offset;
    ot_eis_sns_state *sns_state = OT_NULL;
    sns_check_pointer_void_return(ae_sns_dft);
    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines_max = g_imx623_mode_tbl[sns_state->img_mode].max_ver_lines;

    vmax = full_lines;
    vmax = (vmax > lines_max) ? lines_max : vmax;
    sns_state->fl[0] = vmax;
    vmax_offset = vmax - lines_max;

    switch (sns_state->wdr_mode) {
        case OT_EIS_WDR_MODE_NONE:
            sns_state->regs_info[0].i2c_data[24].data = vmax_offset & 0xFF; /* index 5 */
            sns_state->regs_info[0].i2c_data[25].data = (vmax_offset & 0xFF00) >> 8; /* index 6, shift 8 */
            sns_state->regs_info[0].i2c_data[26].data = (vmax_offset & 0xF0000) >> 16; /* index 7, shift 16 */
            break;
        case OT_EIS_WDR_MODE_BUILT_IN:
            sns_state->regs_info[0].i2c_data[18].data = vmax_offset & 0xFF; /* index 5 */
            sns_state->regs_info[0].i2c_data[19].data = (vmax_offset & 0xFF00) >> 8; /* index 6, shift 8 */
            sns_state->regs_info[0].i2c_data[20].data = (vmax_offset & 0xF0000) >> 16; /* index 7, shift 16 */
            break;
        default:
            break;
    }

    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->max_int_time = sns_state->fl[0] - 8; /* max_int_time: Flstd - 8 */

    return;
}

static ot_void cmos_inttime_update_linear(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;
    ot_u32 value;

    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    value = int_time + int_time % 2; /* mode 2 */

    value = MIN2(value, 0xFFFFF);
    value = MIN2(MAX2(value, 4), sns_state->fl[0] - 8); /* min 4, sub 8 */
    sns_state->regs_info[0].i2c_data[0].data = low_8bits(value);
    sns_state->regs_info[0].i2c_data[1].data = high_8bits(value);
    sns_state->regs_info[0].i2c_data[2].data = high_h_8bits(value);
    sns_state->regs_info[0].i2c_data[3].data = high_h_h_8bits(value);
    sns_state->regs_info[0].i2c_data[4].data = sns_state->regs_info[0].i2c_data[0].data;
    sns_state->regs_info[0].i2c_data[5].data = sns_state->regs_info[0].i2c_data[1].data;
    sns_state->regs_info[0].i2c_data[6].data = sns_state->regs_info[0].i2c_data[2].data;
    sns_state->regs_info[0].i2c_data[7].data = sns_state->regs_info[0].i2c_data[3].data;

    return;
}

static ot_void cmos_inttime_update_built_in_wdr(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;
    ot_u32 value;

    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    value = int_time + int_time % 2; /* mode 2 */

    value = MIN2(value, 0xFFFFF);
    value = MIN2(MAX2(value, 4), sns_state->fl[0] - 8); /* min 4, sub 8 */

    sns_state->regs_info[0].i2c_data[0].data = low_8bits(value);
    sns_state->regs_info[0].i2c_data[1].data = high_8bits(value);
    sns_state->regs_info[0].i2c_data[2].data = high_h_8bits(value);
    sns_state->regs_info[0].i2c_data[3].data = high_h_h_8bits(value);
    sns_state->regs_info[0].i2c_data[4].data = sns_state->regs_info[0].i2c_data[0].data;
    sns_state->regs_info[0].i2c_data[5].data = sns_state->regs_info[0].i2c_data[1].data;
    sns_state->regs_info[0].i2c_data[6].data = sns_state->regs_info[0].i2c_data[2].data;
    sns_state->regs_info[0].i2c_data[7].data = sns_state->regs_info[0].i2c_data[3].data;

    return;
}

/* while vi notify ae to update sensor regs, ae call these funcs. */
static ot_void cmos_inttime_update(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
        cmos_inttime_update_built_in_wdr(vi_pipe, int_time);
    } else {
        cmos_inttime_update_linear(vi_pipe, int_time);
    }

    return;
}

#define AGAIN_NODE_NUM    261
#define DGAIN_NODE_NUM   481
static ot_u32 g_again_table[AGAIN_NODE_NUM] = {
    1024,1036,1048,1060,1072,1085,1097,1110,1123,1136,1149,1162,1176,1189,1203,1217,1231,1245,
 1260,1274,1289,1304,1319,1334,1350,1366,1381,1397,1414,1430,1446,1463,1480,1497,1515,1532,
 1552,1568,1586,1604,1623,1642,1661,1680,1699,1719,1739,1759,1780,1800,1821,1842,1863,1885,
 1907,1929,1951,1974,1997,2020,2043,2067,2091,2115,2139,2164,2189,2215,2240,2266,2292,2319,
 2346,2373,2400,2428,2456,2485,2514,2543,2572,2602,2632,2663,2693,2725,2756,2788,2820,2853,
 2886,2919,2953,2987,3022,3057,3092,3128,3164,3201,3238,3276,3314,3352,3391,3430,3470,3510,
 3551,3592,3633,3675,3718,3761,3805,3849,3893,3938,3984,4030,4077,4124,4172,4220,4269,4318,
 4368,4419,4470,4522,4574,4627,4681,4735,4790,4845,4901,4958,5015,5073,5132,5192,5252,5313,
 5374,5436,5499,5563,5627,5692,5758,5825,5893,5961,6030,6100,6170,6242,6314,6387,6461,6536,
 6611,6688,6766,6844,6923,7003,7084,7166,7249,7333,7418,7504,7591,7679,7768,7858,7949,8041,
 8134,8228,8323,8420,8517,8616,8716,8817,8919,9022,9126,9232,9339,9447,9557,9667,9779,9892,
 10007,10123,10240,10359,10479,10600,10723,10847,10972,11099,11228,11358,11489,11623,11757,11893,
 12031,12170,12311,12454,12598,12744,12891,13041,13192,13344,13499,13655,13813,13973,14135,14299,
 14464,14632,14801,14973,15146,15321,15499,15678,15860,16044,16229,16800,17390,18001,18634,19289,
 19966,20668,21394,22146,22925,23730,24564,25427,26321,27246,28203,29194,30220,31282,32382,};
static ot_u32 g_dgain_table[DGAIN_NODE_NUM] = {
1024,1036,1048,1060,1072,1085,1097,1110,1123,1136,1149,1162,1176,1189,1203,1217,1231,1245,
1260,1274,1289,1304,1319,1334,1350,1366,1381,1397,1414,1430,1446,1463,1480,1497,1515,1532,
1552,1568,1586,1604,1623,1642,1661,1680,1699,1719,1739,1759,1780,1800,1821,1842,1863,1885,
1907,1929,1951,1974,1997,2020,2043,2067,2091,2115,2139,2164,2189,2215,2240,2266,2292,2319,
2346,2373,2400,2428,2456,2485,2514,2543,2572,2602,2632,2663,2693,2725,2756,2788,2820,2853,
2886,2919,2953,2987,3022,3057,3092,3128,3164,3201,3238,3276,3314,3352,3391,3430,3470,3510,
3551,3592,3633,3675,3718,3761,3805,3849,3893,3938,3984,4030,4077,4124,4172,4220,4269,4318,
4368,4419,4470,4522,4574,4627,4681,4735,4790,4845,4901,4958,5015,5073,5132,5192,5252,5313,
5374,5436,5499,5563,5627,5692,5758,5825,5893,5961,6030,6100,6170,6242,6314,6387,6461,6536,
6611,6688,6766,6844,6923,7003,7084,7166,7249,7333,7418,7504,7591,7679,7768,7858,7949,8041,
8134,8228,8323,8420,8517,8616,8716,8817,8919,9022,9126,9232,9339,9447,9557,9667,9779,9892,
10007,10123,10240,10359,10479,10600,10723,10847,10972,11099,11228,11358,11489,11623,11757,11893,
12031,12170,12311,12454,12598,12744,12891,13041,13192,13344,13499,13655,13813,13973,14135,14299,
14464,14632,14801,14973,15146,15321,15499,15678,15860,16044,16229,16417,16607,16800,16994,17191,
17390,17591,17795,18001,18210,18420,18634,18850,19068,19289,19512,19738,19966,20198,20431,20668,
20907,21149,21394,21642,21893,22146,22403,22662,22925,23190,23458,23730,24005,24283,24564,24848,
25136,25427,25722,26020,26321,26626,26934,27246,27561,27880,28203,28530,28860,29194,29532,29874,
30220,30570,30924,31282,31645,32011,32382,32757,33136,33520,33908,34300,34698,35099,35506,35917,
36333,36754,37179,37610,38045,38486,38931,39382,39838,40300,40766,41238,41716,42199,42687,43182,
43682,44188,44699,45217,45740,46270,46806,47348,47896,48451,49012,49579,50153,50734,51322,51916,
52517,53125,53740,54363,54992,55629,56273,56925,57584,58251,58925,59607,60298,60996,61702,62417,
63139,63870,64610,65358,66115,66881,67655,68438,69231,70033,70843,71664,72494,73333,74182,75041,
75910,76789,77678,78578,79488,80408,81339,82281,83234,84198,85173,86159,87157,88166,89187,90219,
91264,92321,93390,94471,95565,96672,97791,98924,100069,101228,102400,103586,104785,105999,107226,
108468,109724,110994,112279,113580,114895,116225,117571,118932,120310,121703,123112,124537,125980,
127438,128914,130407,131917,133444,134989,136553,138134,139733,141351,142988,144644,146319,148013,
149727,151461,153215,154989,156783,158599,160435,162293,164172,166073,167996,169942,171910,173900,
175914,177951,180011,182096,184204,186337,188495,190678,192886,195119,197379,199664,201976,204315,
206681,209074,211495,213944,216421,218927,221462,224027,226621,229245,231900,234585,237301,240049,
242829,245640,248485,251362,254273,257217,};

static ot_void cmos_again_calc_table(ot_vi_pipe_id vi_pipe, ot_u32 *again_lin, ot_u32 *again_db)
{
    int i;

    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);

    if (*again_lin >= g_again_table[AGAIN_NODE_NUM - 1]) {
        *again_lin = g_again_table[AGAIN_NODE_NUM - 1];
        *again_db = 300;                                    //0.1+0.3 db      max 30db
        return;
    }

    for (i = 1; i < AGAIN_NODE_NUM; i++) {
        if (*again_lin < g_again_table[i]) {
            *again_lin = g_again_table[i - 1];
            if((i-1)<241)                                    //0.1db step
            {
                *again_db = i - 1;
            }
            else
            {
                *again_db = ((i - 1)-240)*3 + 240;          //0.3db step
            }
            break;
        }
    }
    return;
}

static ot_void cmos_dgain_calc_table(ot_vi_pipe_id vi_pipe, ot_u32 *dgain_lin, ot_u32 *dgain_db)
{
    int i;

    sns_check_pointer_void_return(dgain_lin);
    sns_check_pointer_void_return(dgain_db);

    if (*dgain_lin >= g_dgain_table[DGAIN_NODE_NUM - 1]) {
        *dgain_lin = g_dgain_table[DGAIN_NODE_NUM - 1];
        *dgain_db = DGAIN_NODE_NUM - 1;
        return;
    }

    for (i = 1; i < DGAIN_NODE_NUM; i++) {
        if (*dgain_lin < g_dgain_table[i]) {
            *dgain_lin = g_dgain_table[i - 1];
            *dgain_db = i - 1;
            break;
        }
    }
    return;
}


static ot_void cmos_gains_update(ot_vi_pipe_id vi_pipe, ot_u32 again, ot_u32 dgain)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);



    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {  //hum
 		sns_state->regs_info[0].i2c_data[8].data = low_8bits(again); /* index 3 */
        sns_state->regs_info[0].i2c_data[9].data = high_8bits(again); /* index 4 */
 		sns_state->regs_info[0].i2c_data[16].data = low_8bits(dgain); /* index 3 */
        sns_state->regs_info[0].i2c_data[17].data = high_8bits(dgain); /* index 4 */
		}
		else if (sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
 		sns_state->regs_info[0].i2c_data[8].data = low_8bits(again); /* index 3 */
        sns_state->regs_info[0].i2c_data[9].data = high_8bits(again); /* index 4 */
 		// sns_state->regs_info[0].i2c_data[10].data = sns_state->regs_info[0].i2c_data[8].data;
        // sns_state->regs_info[0].i2c_data[11].data = sns_state->regs_info[0].i2c_data[9].data;
 		// sns_state->regs_info[0].i2c_data[12].data = sns_state->regs_info[0].i2c_data[8].data;
        // sns_state->regs_info[0].i2c_data[13].data = sns_state->regs_info[0].i2c_data[9].data;
 		// sns_state->regs_info[0].i2c_data[14].data = sns_state->regs_info[0].i2c_data[8].data;
        // sns_state->regs_info[0].i2c_data[15].data = sns_state->regs_info[0].i2c_data[9].data;

 		sns_state->regs_info[0].i2c_data[16].data = low_8bits(dgain); /* index 3 */
        sns_state->regs_info[0].i2c_data[17].data = high_8bits(dgain); /* index 4 */
		}

    return;
}

static ot_void cmos_clip_ratio_range(ot_u32 *a_exp_ratio, const ot_eis_vi_ae_int_time_range *int_time,
    ot_u32 wdr_frame_num)
{
    ot_u32 i, ratio_min, ratio_max;
    for (i = 0; i < wdr_frame_num - 1; ++i) {
        ratio_max = int_time->int_time_max[i + 1] * 0x40 / int_time->int_time_min[i];
        ratio_min = int_time->int_time_min[i + 1] * 0x40 / int_time->int_time_max[i];
        a_exp_ratio[i] = MIN2(MAX2(a_exp_ratio[i], ratio_min), ratio_max);
        a_exp_ratio[i] = MIN2(MAX2(a_exp_ratio[i], 0x40), 0x4000);
    }
}

static ot_void cmos_step_limit(ot_eis_vi_ae_int_time_range *int_time,
    const time_step *step, ot_u32 full_lines, ot_u32 wdr_frame_num)
{
    ot_u32 i;
    ot_u32 max_int_time = 0;
    for (i = 0; i < wdr_frame_num; ++i) {
        if (int_time->pre_int_time[i] == 0) {
            return ;
        }
    }
    for (i = 0; i < wdr_frame_num; ++i) {
        if (step->inc[i] > 0) {
            int_time->int_time_max[i] = MIN2(int_time->pre_int_time[i] + step->inc[i], int_time->int_time_max[i]);
        }
        if (step->dec[i] > 0) {
            if (int_time->pre_int_time[i] > step->dec[i]) {
                int_time->int_time_min[i] = MAX2(int_time->pre_int_time[i] - step->dec[i], int_time->int_time_min[i]);
            } else {
                int_time->int_time_min[i] = MAX2(int_time->int_time_min[i], 0);
            }
        }
        if (int_time->int_time_min[i] > int_time->int_time_max[i]) {
            int_time->int_time_max[i] = int_time->int_time_min[i];
        }
        /* make sure LEF > SEF1 and SEF1 > SEF2 */
        if (i > 0) {
            int_time->int_time_max[i] = MAX2(int_time->int_time_max[i], int_time->int_time_max[i - 1]);
            int_time->int_time_min[i] = MAX2(int_time->int_time_min[i], int_time->int_time_min[i - 1]);
        }
    }
    /* make sure max_int_time < full_lines */
    for (i = 0; i < wdr_frame_num; ++i) {
        max_int_time += int_time->int_time_max[i];
    }
    if (max_int_time > full_lines) {
        max_int_time = max_int_time - full_lines;
        for (i = 0; i < wdr_frame_num; ++i) {
            if (int_time->int_time_max[i] - int_time->int_time_min[i] > max_int_time) {
                int_time->int_time_max[i] = int_time->int_time_max[i] - max_int_time;
                return;
            }
        }
    }
    return;
}

static ot_void cmos_get_inttime_max(ot_vi_pipe_id vi_pipe, ot_u16 man_ratio_enable, ot_u32 *ratio,
    ot_eis_vi_ae_int_time_range *int_time, ot_u32 *lf_max_int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_void_return(ratio);
    sns_check_pointer_void_return(int_time);
    sns_check_pointer_void_return(lf_max_int_time);
    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    switch (sns_state->wdr_mode) {
        //case OT_EIS_WDR_MODE_2To1_LINE:
            //cmos_get_inttime_max_2to1_line(vi_pipe, ratio, int_time, lf_max_int_time);
            //break;
        default:
            break;
    }

    return;
}

/* Only used in LINE_WDR mode */
static ot_void cmos_ae_fswdr_attr_set(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_fswdr_attr *ae_fswdr_attr)
{
    sns_check_pointer_void_return(ae_fswdr_attr);

    g_fswdr_mode[vi_pipe] = ae_fswdr_attr->fswdr_mode;
    g_max_time_get_cnt[vi_pipe] = 0;

    return;
}
/* awb static param for Fuji Lens New IR_Cut */                  //AWB calibration
#define CALIBRATE_STATIC_TEMP                         4950
#define CALIBRATE_STATIC_WB_R_GAIN                    429
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    555

/* Calibration results for Auto WB Planck */                       //AWB calibration
#define CALIBRATE_AWB_P1                              (60)
#define CALIBRATE_AWB_P2                              (115)
#define CALIBRATE_AWB_Q1                              (-81)
#define CALIBRATE_AWB_A1                              (118824)
#define CALIBRATE_AWB_B1                              128
#define CALIBRATE_AWB_C1                              (-69882)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static ot_s32 cmos_get_awb_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_sensor_default *awb_sns_dft)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(awb_sns_dft);
    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);
    (ot_void)memset_s(awb_sns_dft, sizeof(ot_eis_vi_awb_sensor_default), 0, sizeof(ot_eis_vi_awb_sensor_default));

    if (g_ext_awb_flag[vi_pipe] == 1) {
        (ot_void)memcpy_s(awb_sns_dft, sizeof(ot_eis_vi_awb_sensor_default), &g_ext_sns_awb[vi_pipe],
                          sizeof(ot_eis_vi_awb_sensor_default));

        awb_sns_dft->init_rgain = g_init_wb_gain[vi_pipe][0]; /* 0: Rgain */
        awb_sns_dft->init_ggain = g_init_wb_gain[vi_pipe][1]; /* 1: Ggain */
        awb_sns_dft->init_bgain = g_init_wb_gain[vi_pipe][2]; /* 2: Bgain */
        awb_sns_dft->sample_rgain = g_sample_r_gain[vi_pipe];
        awb_sns_dft->sample_bgain = g_sample_b_gain[vi_pipe];
        awb_sns_dft->preawb_attr.enable = 0;
    } else {
        awb_sns_dft->wb_ref_temp = CALIBRATE_STATIC_TEMP; /* wb_ref_temp 4950 */

        awb_sns_dft->gain_offset[0] = CALIBRATE_STATIC_WB_R_GAIN;
        awb_sns_dft->gain_offset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
        awb_sns_dft->gain_offset[2] = CALIBRATE_STATIC_WB_GB_GAIN; /* index 2 */
        awb_sns_dft->gain_offset[3] = CALIBRATE_STATIC_WB_B_GAIN; /* index 3 */

        awb_sns_dft->wb_para[0] = CALIBRATE_AWB_P1;
        awb_sns_dft->wb_para[1] = CALIBRATE_AWB_P2;
        awb_sns_dft->wb_para[2] = CALIBRATE_AWB_Q1; /* index 2 */
        awb_sns_dft->wb_para[3] = CALIBRATE_AWB_A1; /* index 3 */
        awb_sns_dft->wb_para[4] = CALIBRATE_AWB_B1; /* index 4 */
        awb_sns_dft->wb_para[5] = CALIBRATE_AWB_C1; /* index 5 */

        awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
        awb_sns_dft->golden_bgain = GOLDEN_BGAIN;

        awb_sns_dft->awb_run_interval = 1;

        switch (sns_state->wdr_mode) {
            case OT_EIS_WDR_MODE_NONE:
                (ot_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_eis_vi_awb_ccm), &g_awb_ccm, sizeof(ot_eis_vi_awb_ccm));
                (ot_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_eis_vi_awb_agc_table),
                                  &g_awb_agc_table, sizeof(ot_eis_vi_awb_agc_table));
                break;
            case OT_EIS_WDR_MODE_BUILT_IN:
                (ot_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_eis_vi_awb_ccm), &g_awb_ccm_built_in, sizeof(ot_eis_vi_awb_ccm));
                (ot_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_eis_vi_awb_agc_table),
                                  &g_awb_agc_table, sizeof(ot_eis_vi_awb_agc_table));

                break;
            default:
                (ot_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_eis_vi_awb_ccm), &g_awb_ccm, sizeof(ot_eis_vi_awb_ccm));
                (ot_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_eis_vi_awb_agc_table),
                                  &g_awb_agc_table, sizeof(ot_eis_vi_awb_agc_table));
                break;
        }
        awb_sns_dft->init_rgain = g_init_wb_gain[vi_pipe][0]; /* 0: Rgain */
        awb_sns_dft->init_ggain = g_init_wb_gain[vi_pipe][1]; /* 1: Ggain */
        awb_sns_dft->init_bgain = g_init_wb_gain[vi_pipe][2]; /* 2: Bgain */
        awb_sns_dft->sample_rgain = g_sample_r_gain[vi_pipe];
        awb_sns_dft->sample_bgain = g_sample_b_gain[vi_pipe];
        awb_sns_dft->preawb_attr.enable = 0;
    }

    return OT_SUCCESS;
}

static void cmos_get_isp_linear_default(ot_eis_vi_sns_alg_default *isp_def, ot_vi_pipe_id vi_pipe)
{
    isp_def->key.bit1_demosaic         = 1;
    isp_def->demosaic                  = &g_cmos_demosaic;
    isp_def->key.bit1_sharpen          = 1;
    isp_def->sharpen                   = &g_cmos_yuv_sharpen;
    isp_def->key.bit1_drc              = 1;
    isp_def->drc                       = &g_cmos_drc;
    isp_def->key.bit1_bayer_nr         = 1;
    isp_def->bayer_nr                  = &g_cmos_bayer_nr;
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color          = &g_cmos_anti_false_color;
    isp_def->key.bit1_cac              = 1;
    isp_def->cac                       = &g_cmos_cac;
    isp_def->key.bit1_bshp             = 1;
    isp_def->bshp                      = &g_cmos_bayershp;
    isp_def->key.bit1_ldci             = 1;
    isp_def->ldci                      = &g_cmos_ldci;
    isp_def->key.bit1_gamma            = 1;
    isp_def->gamma                     = &g_cmos_gamma;
#ifdef CONFIG_OT_ISP_CR_SUPPORT
    isp_def->key.bit1_ge               = 1;
    isp_def->ge                        = &g_cmos_ge;
#endif
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze;
    isp_def->key.bit1_ca = 0;
    (ot_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_eis_vi_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_eis_vi_noise_calibration));
    return;
}

static void cmos_get_isp_built_in_default(ot_eis_vi_sns_alg_default *isp_def, ot_vi_pipe_id vi_pipe)
{
    isp_def->key.bit1_demosaic         = 1;
    isp_def->demosaic                  = &g_cmos_demosaic_built_in;
    isp_def->key.bit1_sharpen          = 1;
    isp_def->sharpen                   = &g_cmos_yuv_sharpen_built_in;
    isp_def->key.bit1_drc              = 1;
    isp_def->drc                       = &g_cmos_drc_built_in;
    isp_def->key.bit1_bayer_nr         = 1;
    isp_def->bayer_nr                  = &g_cmos_bayer_nr_built_in;
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color          = &g_cmos_anti_false_color;
    isp_def->key.bit1_ldci             = 1;
    isp_def->ldci                      = &g_cmos_ldci_built_in;
    isp_def->key.bit1_gamma            = 1;
    isp_def->gamma                     = &g_cmos_gamma;
    isp_def->key.bit1_expander         = 1;
    isp_def->expander                  = &g_cmos_expander;

#ifdef CONFIG_OT_ISP_CR_SUPPORT
        isp_def->key.bit1_ge               = 1;
        isp_def->ge                        = &g_cmos_ge;
#endif
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze;
    (ot_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_eis_vi_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_eis_vi_noise_calibration));
    return;
}


static ot_s32 cmos_get_vi_alg_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_alg_default *isp_def)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(isp_def);
    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (ot_void)memset_s(isp_def, sizeof(ot_eis_vi_sns_alg_default), 0, sizeof(ot_eis_vi_sns_alg_default));

    if (g_ext_alg_flag[vi_pipe] == 1) {
        isp_def->key.bit1_clut             = g_ext_sns_alg[vi_pipe].key.bit1_clut;
        isp_def->clut                      = g_ext_sns_alg[vi_pipe].clut;
        isp_def->key.bit1_dpc              = g_ext_sns_alg[vi_pipe].key.bit1_dpc;
        isp_def->dpc                       = g_ext_sns_alg[vi_pipe].dpc;
        isp_def->key.bit1_wdr              = g_ext_sns_alg[vi_pipe].key.bit1_wdr;
        isp_def->wdr                       = g_ext_sns_alg[vi_pipe].wdr;
        isp_def->key.bit1_lsc              = g_ext_sns_alg[vi_pipe].key.bit1_lsc;
        isp_def->lsc                       = g_ext_sns_alg[vi_pipe].lsc;
        isp_def->key.bit1_acs              = g_ext_sns_alg[vi_pipe].key.bit1_acs;
        isp_def->acs                       = g_ext_sns_alg[vi_pipe].acs;
        isp_def->key.bit1_demosaic         = g_ext_sns_alg[vi_pipe].key.bit1_demosaic;
        isp_def->demosaic                  = g_ext_sns_alg[vi_pipe].demosaic;
        isp_def->key.bit1_sharpen          = g_ext_sns_alg[vi_pipe].key.bit1_sharpen;
        isp_def->sharpen                   = g_ext_sns_alg[vi_pipe].sharpen;
        isp_def->key.bit1_drc              = g_ext_sns_alg[vi_pipe].key.bit1_drc;
        isp_def->drc                       = g_ext_sns_alg[vi_pipe].drc;
        isp_def->key.bit1_bayer_nr         = g_ext_sns_alg[vi_pipe].key.bit1_bayer_nr;
        isp_def->bayer_nr                  = g_ext_sns_alg[vi_pipe].bayer_nr;
        isp_def->key.bit1_anti_false_color = g_ext_sns_alg[vi_pipe].key.bit1_anti_false_color;
        isp_def->anti_false_color          = g_ext_sns_alg[vi_pipe].anti_false_color;
        isp_def->key.bit1_ldci             = g_ext_sns_alg[vi_pipe].key.bit1_ldci;
        isp_def->ldci                      = g_ext_sns_alg[vi_pipe].ldci;
        isp_def->key.bit1_gamma            = g_ext_sns_alg[vi_pipe].key.bit1_gamma;
        isp_def->gamma                     = g_ext_sns_alg[vi_pipe].gamma;
        isp_def->key.bit1_expander         = g_ext_sns_alg[vi_pipe].key.bit1_expander;
        isp_def->expander                  = g_ext_sns_alg[vi_pipe].expander;
        isp_def->key.bit1_cac         = g_ext_sns_alg[vi_pipe].key.bit1_cac;
        isp_def->cac                  = g_ext_sns_alg[vi_pipe].cac;
        isp_def->key.bit1_bshp         = g_ext_sns_alg[vi_pipe].key.bit1_bshp;
        isp_def->bshp                  = g_ext_sns_alg[vi_pipe].bshp;
        isp_def->key.bit1_lblc         = g_ext_sns_alg[vi_pipe].key.bit1_lblc;
        isp_def->lblc                  = g_ext_sns_alg[vi_pipe].lblc;
#ifdef CONFIG_OT_ISP_CA_SUPPORT
        isp_def->key.bit1_ca      = g_ext_sns_alg[vi_pipe].key.bit1_ca;
        isp_def->key.ca           = g_ext_sns_alg[vi_pipe].ca;
#endif
#ifdef CONFIG_OT_ISP_CR_SUPPORT
        isp_def->key.bit1_ge               = g_ext_sns_alg[vi_pipe].key.bit1_ge;
        isp_def->ge                        = g_ext_sns_alg[vi_pipe].ge;
#endif
        isp_def->key.bit1_dehaze           = g_ext_sns_alg[vi_pipe].key.bit1_dehaze;
        isp_def->dehaze                    = g_ext_sns_alg[vi_pipe].dehaze;
        (ot_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_eis_vi_noise_calibration),
                          &g_ext_sns_alg[vi_pipe].noise_calibration, sizeof(ot_eis_vi_noise_calibration));
    } else {
        isp_def->key.bit1_clut    = 1;
        isp_def->clut             = &g_cmos_clut;
        isp_def->key.bit1_dpc     = 1;
        isp_def->dpc              = &g_cmos_dpc;
        isp_def->key.bit1_wdr     = 1;
        isp_def->wdr              = &g_cmos_wdr;
        isp_def->key.bit1_lsc      = 0;
        isp_def->lsc               = &g_cmos_lsc;
        isp_def->key.bit1_acs      = 0;
        isp_def->acs               = &g_cmos_acs;
#ifdef CONFIG_OT_ISP_CA_SUPPORT
        isp_def->key.bit1_ca      = 0;
#endif
        switch (sns_state->wdr_mode) {
            case OT_EIS_WDR_MODE_NONE:
                cmos_get_isp_linear_default(isp_def, vi_pipe);
                break;
            case OT_EIS_WDR_MODE_BUILT_IN:
                cmos_get_isp_built_in_default(isp_def, vi_pipe);
                break;
            default:
                cmos_get_isp_linear_default(isp_def, vi_pipe);
                break;
        }
    }

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;
    isp_def->sns_mode.sns_id = IMX623_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;

    return OT_SUCCESS;
}

static ot_s32 cmos_get_vi_black_level(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_black_level *black_level)
{
    ot_s32  i;
    ot_eis_sns_state *sns_state = OT_NULL;
    const ot_eis_vi_sns_black_level *cmos_blc_def = OT_NULL;

    sns_check_pointer_return(black_level);
    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if (g_ext_alg_flag[vi_pipe] == 1) {
        cmos_blc_def = g_ext_sns_alg[vi_pipe].sns_blc;
    } else {
        if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE || sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
            cmos_blc_def = &g_cmos_blc;
        } else {
            cmos_blc_def = &g_cmos_blc_wdr;
        }
    }

    (ot_void)memcpy_s(black_level, sizeof(ot_eis_vi_sns_black_level), cmos_blc_def, sizeof(ot_eis_vi_sns_black_level));

    return OT_SUCCESS;
}

static ot_s32 cmos_get_blc_clamp_info(ot_vi_pipe_id vi_pipe, ot_bool *blc_clamp_en)
{
    sns_check_pointer_return(blc_clamp_en);

    *blc_clamp_en = blc_clamp_info[vi_pipe];

    return OT_SUCCESS;
}

static ot_void cmos_set_pixel_detect(ot_vi_pipe_id vi_pipe, ot_bool enable)
{
    ot_u32 full_lines_5fps;
    ot_eis_sns_state *sns_state = OT_NULL;

    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->img_mode == IMX623_SENSOR_1552P_30FPS_LINEAR_MODE) {
            full_lines_5fps = (IMX623_VMAX_1552P30_LINEAR * 60) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }

//    if (enable) { /* setup for ISP pixel calibration mode */
//        imx623_write_register(vi_pipe, IMX623_AGAIN_L_ADDR, 0x80);
//        imx623_write_register(vi_pipe, IMX623_AGAIN_H_ADDR, 0x00);
//
//        imx623_write_register(vi_pipe, IMX623_DGAIN_L_ADDR, 0x00);
//        imx623_write_register(vi_pipe, IMX623_DGAIN_H_ADDR, 0x04);
//
//        imx623_write_register(vi_pipe, IMX623_VMAX_L_ADDR, low_8bits(full_lines_5fps));
//        imx623_write_register(vi_pipe, IMX623_VMAX_H_ADDR, high_8bits(full_lines_5fps));
//
//        imx623_write_register(vi_pipe, IMX623_EXPO_L_ADDR, low_8bits(full_lines_5fps - 10));
//        imx623_write_register(vi_pipe, IMX623_EXPO_H_ADDR, high_8bits(full_lines_5fps - 10));
//    } else { /* setup for ISP 'normal mode' */
//        sns_state->fl_std = (sns_state->fl_std > IMX623_FULL_LINES_MAX) ? IMX623_FULL_LINES_MAX : sns_state->fl_std;
//        imx623_write_register(vi_pipe, IMX623_VMAX_L_ADDR, low_8bits(sns_state->fl_std));
//        imx623_write_register(vi_pipe, IMX623_VMAX_H_ADDR, high_8bits(sns_state->fl_std));
//        sns_state->sync_init = OT_FALSE;
//    }

    return;
}

static ot_s32 cmos_set_wdr_mode(ot_vi_pipe_id vi_pipe, ot_u8 mode)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init = OT_FALSE;

    switch (mode & 0x3F) {
        case OT_EIS_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_EIS_WDR_MODE_NONE;
            printf("linear mode\n");
            break;
        case OT_EIS_WDR_MODE_BUILT_IN:
            sns_state->wdr_mode = OT_EIS_WDR_MODE_BUILT_IN;
            printf("built-in mode\n");
            break;



        default:
            sns_err_trace("NOT support this mode!\n");
            return OT_FAILURE;
    }

    (ot_void)memset_s(sns_state->wdr_int_time, sizeof(sns_state->wdr_int_time), 0, sizeof(sns_state->wdr_int_time));

    return OT_SUCCESS;
}

static ot_void cmos_comm_sns_reg_info_init(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
    ot_u32 i;
    sns_state->regs_info[0].sns_type = OT_EIS_VI_SNS_TYPE_I2C;
    sns_state->regs_info[0].com_bus.i2c_dev = g_imx623_bus_info[vi_pipe].com_bus.i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = 26; // TODO

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
        sns_state->regs_info[0].reg_num = 20;
        sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    }

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = OT_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = g_imx623_bus_info[vi_pipe].bus_addr;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = I2C_REG_ADDR_2BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = I2C_DATA_BYTE;
    }

    /* Linear Mode Regs */
//    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].delay_frame_num = 0;
//    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].reg_addr = IMX623_EXPO_L_ADDR;
//    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].delay_frame_num = 0;
//    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].reg_addr = IMX623_EXPO_H_ADDR;
//
//    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].delay_frame_num = 0;
//    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].reg_addr = IMX623_AGAIN_L_ADDR;
//    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].delay_frame_num = 0;
//    sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].reg_addr = IMX623_AGAIN_H_ADDR;
//
//    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].delay_frame_num = 0;
//    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].reg_addr = IMX623_DGAIN_L_ADDR;
//    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].delay_frame_num = 0;
//    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].reg_addr = IMX623_DGAIN_H_ADDR;
//
//    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 1;
//    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].reg_addr = IMX623_VMAX_L_ADDR;
//    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 1;
//    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].reg_addr = IMX623_VMAX_H_ADDR;

    return;
}

static ot_void cmos_linear_sns_reg_info_init(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
	// shutter related
    sns_state->regs_info[0].i2c_data[0].delay_frame_num = 0;           //SP1
    sns_state->regs_info[0].i2c_data[0].reg_addr = IMX623_SHT_SP1_LL;
    sns_state->regs_info[0].i2c_data[1].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[1].reg_addr = IMX623_SHT_SP1_LH;
    sns_state->regs_info[0].i2c_data[2].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[2].reg_addr = IMX623_SHT_SP1_HL;
    sns_state->regs_info[0].i2c_data[3].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[3].reg_addr = IMX623_SHT_SP1_HH;

    sns_state->regs_info[0].i2c_data[4].delay_frame_num = 0;            //SP2
    sns_state->regs_info[0].i2c_data[4].reg_addr = IMX623_SHT_SP2_LL;
    sns_state->regs_info[0].i2c_data[5].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[5].reg_addr = IMX623_SHT_SP2_LH;
    sns_state->regs_info[0].i2c_data[6].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[6].reg_addr = IMX623_SHT_SP2_HL;
    sns_state->regs_info[0].i2c_data[7].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[7].reg_addr = IMX623_SHT_SP2_HH;

	// again related
    sns_state->regs_info[0].i2c_data[8].delay_frame_num = 0;             //SP1_H
    sns_state->regs_info[0].i2c_data[8].reg_addr = IMX623_AGAIN_SP1_H_L;
    sns_state->regs_info[0].i2c_data[9].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[9].reg_addr = IMX623_AGAIN_SP1_H_H;

    sns_state->regs_info[0].i2c_data[10].delay_frame_num = 0;             //SP1_L
    sns_state->regs_info[0].i2c_data[10].reg_addr = IMX623_AGAIN_SP1_L_L;
    sns_state->regs_info[0].i2c_data[11].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[11].reg_addr = IMX623_AGAIN_SP1_L_H;
    sns_state->regs_info[0].i2c_data[12].delay_frame_num = 0;             //SP2_H
    sns_state->regs_info[0].i2c_data[12].reg_addr = IMX623_AGAIN_SP2_H_L;
    sns_state->regs_info[0].i2c_data[13].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[13].reg_addr = IMX623_AGAIN_SP2_H_H;
    sns_state->regs_info[0].i2c_data[14].delay_frame_num = 0;             //SP2_L
    sns_state->regs_info[0].i2c_data[14].reg_addr = IMX623_AGAIN_SP2_L_L;
    sns_state->regs_info[0].i2c_data[15].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[15].reg_addr = IMX623_AGAIN_SP2_L_H;

    sns_state->regs_info[0].i2c_data[10].data = 0x2a;                                //SP1_L SP2_H SP2_L again common
    sns_state->regs_info[0].i2c_data[11].data = 0x00;
    sns_state->regs_info[0].i2c_data[12].data = 0x26;
    sns_state->regs_info[0].i2c_data[13].data = 0x01;
    sns_state->regs_info[0].i2c_data[14].data = 0x2a;
    sns_state->regs_info[0].i2c_data[15].data = 0x00;

	// dgain related
    sns_state->regs_info[0].i2c_data[16].delay_frame_num = 0;             //SP1_H
    sns_state->regs_info[0].i2c_data[16].reg_addr = IMX623_DGAIN_SP1_H_L;
    sns_state->regs_info[0].i2c_data[17].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[17].reg_addr = IMX623_DGAIN_SP1_H_H;
    sns_state->regs_info[0].i2c_data[18].delay_frame_num = 0;             //SP1_L
    sns_state->regs_info[0].i2c_data[18].reg_addr = IMX623_DGAIN_SP1_L_L;
    sns_state->regs_info[0].i2c_data[19].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[19].reg_addr = IMX623_DGAIN_SP1_L_H;
    sns_state->regs_info[0].i2c_data[20].delay_frame_num = 0;             //SP2_H
    sns_state->regs_info[0].i2c_data[20].reg_addr = IMX623_DGAIN_SP2_H_L;
    sns_state->regs_info[0].i2c_data[21].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[21].reg_addr = IMX623_DGAIN_SP2_H_H;
    sns_state->regs_info[0].i2c_data[22].delay_frame_num = 0;             //SP2_L
    sns_state->regs_info[0].i2c_data[22].reg_addr = IMX623_DGAIN_SP2_L_L;
    sns_state->regs_info[0].i2c_data[23].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[23].reg_addr = IMX623_DGAIN_SP2_L_H;

    // Vmax offset
    sns_state->regs_info[0].i2c_data[24].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[24].reg_addr = IMX623_VMAX_OFFSET_L;
    sns_state->regs_info[0].i2c_data[25].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[25].reg_addr = IMX623_VMAX_OFFSET_M;
    sns_state->regs_info[0].i2c_data[26].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[26].reg_addr = IMX623_VMAX_OFFSET_H;
    return;
}

static ot_void cmos_built_in_sns_reg_info_init(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
	// shutter related
    sns_state->regs_info[0].i2c_data[0].delay_frame_num = 0;           //SP1
    sns_state->regs_info[0].i2c_data[0].reg_addr = IMX623_SHT_SP1_LL;
    sns_state->regs_info[0].i2c_data[1].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[1].reg_addr = IMX623_SHT_SP1_LH;
    sns_state->regs_info[0].i2c_data[2].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[2].reg_addr = IMX623_SHT_SP1_HL;
    sns_state->regs_info[0].i2c_data[3].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[3].reg_addr = IMX623_SHT_SP1_HH;

    sns_state->regs_info[0].i2c_data[4].delay_frame_num = 0;            //SP2
    sns_state->regs_info[0].i2c_data[4].reg_addr = IMX623_SHT_SP2_LL;
    sns_state->regs_info[0].i2c_data[5].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[5].reg_addr = IMX623_SHT_SP2_LH;
    sns_state->regs_info[0].i2c_data[6].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[6].reg_addr = IMX623_SHT_SP2_HL;
    sns_state->regs_info[0].i2c_data[7].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[7].reg_addr = IMX623_SHT_SP2_HH;

	// again related
    sns_state->regs_info[0].i2c_data[8].delay_frame_num = 0;             //SP1_H
    sns_state->regs_info[0].i2c_data[8].reg_addr = IMX623_AGAIN_SP1_H_L;
    sns_state->regs_info[0].i2c_data[9].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[9].reg_addr = IMX623_AGAIN_SP1_H_H;

    sns_state->regs_info[0].i2c_data[10].delay_frame_num = 0;             //SP1_L
    sns_state->regs_info[0].i2c_data[10].reg_addr = IMX623_AGAIN_SP1_L_L;
    sns_state->regs_info[0].i2c_data[11].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[11].reg_addr = IMX623_AGAIN_SP1_L_H;
    sns_state->regs_info[0].i2c_data[12].delay_frame_num = 0;             //SP2_H
    sns_state->regs_info[0].i2c_data[12].reg_addr = IMX623_AGAIN_SP2_H_L;
    sns_state->regs_info[0].i2c_data[13].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[13].reg_addr = IMX623_AGAIN_SP2_H_H;
    sns_state->regs_info[0].i2c_data[14].delay_frame_num = 0;             //SP2_L
    sns_state->regs_info[0].i2c_data[14].reg_addr = IMX623_AGAIN_SP2_L_L;
    sns_state->regs_info[0].i2c_data[15].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[15].reg_addr = IMX623_AGAIN_SP2_L_H;

    sns_state->regs_info[0].i2c_data[10].data = 0x2a;                                //SP1_L SP2_H SP2_L again common
    sns_state->regs_info[0].i2c_data[11].data = 0x00;
    sns_state->regs_info[0].i2c_data[12].data = 0x26;
    sns_state->regs_info[0].i2c_data[13].data = 0x01;
    sns_state->regs_info[0].i2c_data[14].data = 0x2a;
    sns_state->regs_info[0].i2c_data[15].data = 0x00;

	// dgain related
    sns_state->regs_info[0].i2c_data[16].delay_frame_num = 0;             //SP12
    sns_state->regs_info[0].i2c_data[16].reg_addr = IMX623_DGAIN_SP12_L;
    sns_state->regs_info[0].i2c_data[17].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[17].reg_addr = IMX623_DGAIN_SP12_H;

    // Vmax_offset
    sns_state->regs_info[0].i2c_data[18].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[18].reg_addr = IMX623_VMAX_OFFSET_L;
    sns_state->regs_info[0].i2c_data[19].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[19].reg_addr = IMX623_VMAX_OFFSET_M;
    sns_state->regs_info[0].i2c_data[20].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[20].reg_addr = IMX623_VMAX_OFFSET_H;
    return;
}

static ot_void cmos_sns_reg_info_update(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
    ot_u32 i;

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        if (sns_state->regs_info[0].i2c_data[i].data ==
            sns_state->regs_info[1].i2c_data[i].data) {
            sns_state->regs_info[0].i2c_data[i].update = OT_FALSE;
        } else {
            sns_state->regs_info[0].i2c_data[i].update = OT_TRUE;
        }
    }
    return;
}

static ot_s32 cmos_get_sns_regs_info(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_regs_info *sns_regs_info)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(sns_regs_info);
    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if ((sns_state->sync_init == OT_FALSE) || (sns_regs_info->config == OT_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, sns_state);

        if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {

            cmos_linear_sns_reg_info_init(vi_pipe, sns_state);
        }
        if (sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
            cmos_built_in_sns_reg_info_init(vi_pipe, sns_state);
        }
        sns_state->sync_init = OT_TRUE;
    } else {
        cmos_sns_reg_info_update(vi_pipe, sns_state);
    }

    sns_regs_info->config = OT_FALSE;
    (ot_void)memcpy_s(sns_regs_info, sizeof(ot_eis_vi_sns_regs_info),
                      &sns_state->regs_info[0], sizeof(ot_eis_vi_sns_regs_info));
    (ot_void)memcpy_s(&sns_state->regs_info[1], sizeof(ot_eis_vi_sns_regs_info),
                      &sns_state->regs_info[0], sizeof(ot_eis_vi_sns_regs_info));
    sns_state->fl[1] = sns_state->fl[0];

    return OT_SUCCESS;
}

static ot_void cmos_config_image_mode_param(ot_vi_pipe_id vi_pipe, ot_u8 sns_image_mode,
    ot_eis_sns_state *sns_state)
{
    switch (sns_image_mode) {
        case IMX623_SENSOR_1552P_30FPS_LINEAR_MODE:
            sns_state->fl_std = IMX623_VMAX_1552P30_LINEAR;
            break;
        case IMX623_SENSOR_1552P_30FPS_WDR_built_in_MODE:
            sns_state->fl_std = IMX623_VMAX_1552P30_BUILT_IN_WDR;
            break;
        default:
            sns_state->fl_std = IMX623_VMAX_1552P30_LINEAR;
            break;
    }

    return;
}

static ot_s32 cmos_set_image_mode(ot_vi_pipe_id vi_pipe, const ot_eis_vi_sns_image_mode *sns_image_mode)
{
    ot_u32 i;
    ot_u8 image_mode;
    ot_eis_sns_state *sns_state = OT_NULL;
    sns_check_pointer_return(sns_image_mode);
    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < IMX623_MODE_BUTT; i++) {
        if (sns_image_mode->fps <= g_imx623_mode_tbl[i].max_fps &&
            sns_image_mode->width <= g_imx623_mode_tbl[i].width &&
            sns_image_mode->height <= g_imx623_mode_tbl[i].height &&
            sns_state->wdr_mode == g_imx623_mode_tbl[i].wdr_mode) {
            image_mode = (imx623_res_mode)i;
            break;
        }
    }

    if (i >= IMX623_MODE_BUTT) {
        imx623_err_mode_print(sns_image_mode, sns_state);
        return OT_FAILURE;
    }

    cmos_config_image_mode_param(vi_pipe, image_mode, sns_state);

    if ((sns_state->init == OT_TRUE) && (image_mode == sns_state->img_mode)) {
        return OT_EIS_VI_DO_NOT_NEED_SWITCH_IMAGEMODE; /* Don't need to switch image_mode */
    }

    sns_state->sync_init = OT_FALSE;
    sns_state->img_mode = image_mode;
    sns_state->fl[0] = sns_state->fl_std;
    sns_state->fl[1] = sns_state->fl[0];

    return OT_SUCCESS;
}

static ot_void sensor_global_init(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx623_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = OT_FALSE;
    sns_state->sync_init = OT_FALSE;
    sns_state->img_mode  = IMX623_SENSOR_1552P_30FPS_LINEAR_MODE;
    sns_state->wdr_mode  = OT_EIS_WDR_MODE_NONE;
    sns_state->fl_std    = IMX623_VMAX_1552P30_LINEAR;
    sns_state->fl[0]     = IMX623_VMAX_1552P30_LINEAR;
    sns_state->fl[1]     = IMX623_VMAX_1552P30_LINEAR;

    (ot_void)memset_s(&sns_state->regs_info[0], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));
    (ot_void)memset_s(&sns_state->regs_info[1], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));

    return;
}

static ot_s32 sensor_ctx_init(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    imx623_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == OT_NULL) {
        sns_state_ctx = (ot_eis_sns_state *)malloc(sizeof(ot_eis_sns_state));
        if (sns_state_ctx == OT_NULL) {
            sns_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_FAILURE;
        }
    }

    (ot_void)memset_s(sns_state_ctx, sizeof(ot_eis_sns_state), 0, sizeof(ot_eis_sns_state));

    imx623_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return OT_SUCCESS;
}

static ot_void sensor_ctx_exit(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    imx623_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    imx623_sensor_reset_ctx(vi_pipe);

    return;
}

static ot_eis_vi_sns_register_func g_imx623_register_to_vi_func = {
    .pfn_sns_init                = imx623_init,
    .pfn_sns_exit                = imx623_exit,
    .pfn_sns_global_init         = sensor_global_init,
    .pfn_sns_set_image_mode      = cmos_set_image_mode,
    .pfn_sns_set_wdr_mode        = cmos_set_wdr_mode,
    .pfn_sns_get_vi_alg_default  = cmos_get_vi_alg_default,
    .pfn_sns_get_vi_black_level  = cmos_get_vi_black_level,
    .pfn_sns_get_blc_clamp_info  = cmos_get_blc_clamp_info,
    .pfn_sns_set_pixel_detect    = cmos_set_pixel_detect,
    .pfn_sns_get_reg_info        = cmos_get_sns_regs_info,
};

static ot_eis_vi_ae_sensor_register_func g_imx623_register_to_ae_func = {
    .pfn_sns_get_ae_default         = cmos_get_ae_default,
    .pfn_sns_cfg_fps                = cmos_fps_set,
    .pfn_sns_cfg_slow_framerate     = cmos_slow_framerate_set,
    .pfn_sns_update_inttime         = cmos_inttime_update,
    .pfn_sns_update_gains           = cmos_gains_update,
    .pfn_sns_calc_table_again       = cmos_again_calc_table,
    .pfn_sns_calc_table_dgain       = cmos_dgain_calc_table,
    .pfn_sns_get_inttime_max        = cmos_get_inttime_max,
    .pfn_sns_set_fswdr_attr         = cmos_ae_fswdr_attr_set,
    .pfn_sns_set_quick_start_status = OT_NULL,
    .pfn_sns_convert_exp_param      = OT_NULL,
    .pfn_sns_get_thermo_default     = OT_NULL,
} ;

static ot_eis_vi_awb_sensor_register_func g_imx623_register_to_awb_func = {
    .pfn_sns_get_awb_default = cmos_get_awb_default,
    .pfn_sns_set_awb_preawb = NULL,
};

static ot_s32 sensor_register_callback(ot_vi_pipe_handle pipe_hdl)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    ret = sensor_ctx_init(master_pipe_id);
    if (ret != OT_SUCCESS) {
        return OT_FAILURE;
    }

    ret = ot_eis_vi_sensor_register_to_vi(pipe_hdl, &g_imx623_register_to_vi_func, IMX623_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to vi failed!\n");
        goto free_sns_ctx;
    }

    ret = ot_eis_vi_sensor_register_to_ae(pipe_hdl, &g_imx623_register_to_ae_func, IMX623_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to ae lib failed!\n");
        goto sns_unreg_to_vi;
    }

    ret = ot_eis_vi_sensor_register_to_awb(pipe_hdl, &g_imx623_register_to_awb_func, IMX623_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to awb lib failed!\n");
        goto sns_unreg_to_ae;
    }

    return OT_SUCCESS;

sns_unreg_to_ae:
    ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, IMX623_ID);
sns_unreg_to_vi:
    ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, IMX623_ID);
free_sns_ctx:
    sensor_ctx_exit(master_pipe_id);
    return ret;
}

static ot_s32 sensor_unregister_callback(ot_vi_pipe_handle pipe_hdl)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    ret = ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, IMX623_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }
    ret = ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, IMX623_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_eis_vi_sensor_unregister_to_awb(pipe_hdl, IMX623_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to awb lib failed!\n");
        return ret;
    }

    sensor_ctx_exit(master_pipe_id);

    return OT_SUCCESS;
}

static ot_s32 imx623_set_bus_info(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_bus_info *sns_bus_info)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    memcpy_s(&g_imx623_bus_info[master_pipe_id], sizeof(ot_eis_sns_bus_info), sns_bus_info, sizeof(ot_eis_sns_bus_info));

    return OT_SUCCESS;
}

static ot_s32 sensor_set_init(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_init_attr *init_attr)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    sns_check_pointer_return(init_attr);

    g_init_exposure[master_pipe_id]  = init_attr->exposure;
    g_init_int_time[master_pipe_id]  = init_attr->exp_time;
    g_init_again[master_pipe_id]     = init_attr->a_gain;
    g_init_dgain[master_pipe_id]     = init_attr->d_gain;
    g_init_isp_dgain[master_pipe_id] = init_attr->ispd_gain;
    g_lines_per500ms[master_pipe_id] = init_attr->lines_per500ms;
    g_init_wb_gain[master_pipe_id][0] = init_attr->wb_r_gain; /* 0: rgain */
    g_init_wb_gain[master_pipe_id][1] = init_attr->wb_g_gain; /* 1: ggain */
    g_init_wb_gain[master_pipe_id][2] = init_attr->wb_b_gain; /* 2: bgain */
    g_sample_r_gain[master_pipe_id] = init_attr->sample_r_gain;
    g_sample_b_gain[master_pipe_id] = init_attr->sample_b_gain;
    g_quick_start_en[master_pipe_id] = init_attr->quick_start_en;
    g_ae_stat_pos[master_pipe_id]       = init_attr->ae_stat_pos;
    g_ae_route_ex_valid[master_pipe_id] = init_attr->ae_route_ex_valid;
    (ot_void)memcpy_s(&g_init_ae_route[master_pipe_id], sizeof(ot_eis_vi_ae_route),
                      &init_attr->ae_route, sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&g_init_ae_route_ex[master_pipe_id], sizeof(ot_eis_vi_ae_route_ex),
                      &init_attr->ae_route_ex, sizeof(ot_eis_vi_ae_route_ex));
    (ot_void)memcpy_s(&g_init_ae_route_sf[master_pipe_id], sizeof(ot_eis_vi_ae_route),
                      &init_attr->ae_route_sf, sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&g_init_ae_route_sf_ex[master_pipe_id], sizeof(ot_eis_vi_ae_route_ex),
                      &init_attr->ae_route_sf_ex, sizeof(ot_eis_vi_ae_route_ex));

    return OT_SUCCESS;
}

static ot_s32 imx623_set_alg_default(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_sns_alg_default *sns_alg_def)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    if (sns_alg_def == OT_NULL) {
        return OT_FAILURE;
    }
    g_ext_sns_alg[master_pipe_id] = *sns_alg_def;
    g_ext_alg_flag[master_pipe_id] = 1;

    return OT_SUCCESS;
}

static ot_s32 imx623_set_awb_default(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_sensor_default *sns_awb_def)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    if(sns_awb_def == OT_NULL) {
        return OT_FAILURE;
    }
    g_ext_sns_awb[master_pipe_id] = *sns_awb_def;
    g_ext_awb_flag[master_pipe_id] = 1;
    return OT_SUCCESS;
}

static ot_s32 imx623_set_reg_table(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_reg_table *reg_table, ot_s32 table_cnt)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    g_ext_table[master_pipe_id][0] = *reg_table;
    g_ext_table[master_pipe_id][1] = *(reg_table + 1);
    g_ext_table[master_pipe_id][2] = *(reg_table + 2);
    g_ext_table[master_pipe_id][3] = *(reg_table + 3);
    g_ext_table[master_pipe_id][4] = *(reg_table + 4);
    g_ext_sensor_table_flag[master_pipe_id] = 1;
    return OT_SUCCESS;
}

ot_eis_sns_obj g_sns_imx623_obj = {
    .pfn_sns_register              = sensor_register_callback,
    .pfn_sns_unregister            = sensor_unregister_callback,
    .pfn_sns_standby               = imx623_standby,
    .pfn_sns_restart               = imx623_restart,
    .pfn_sns_set_mirror_flip       = OT_NULL,
    .pfn_sns_set_blc_clamp         = imx623_blc_clamp,
    .pfn_sns_set_bus_info          = imx623_set_bus_info,
    .pfn_sns_set_init              = sensor_set_init,
    .pfn_sns_set_alg_default       = imx623_set_alg_default,
    .pfn_sns_set_awb_default       = imx623_set_awb_default,
    .pfn_sns_set_reg_table         = imx623_set_reg_table,
};