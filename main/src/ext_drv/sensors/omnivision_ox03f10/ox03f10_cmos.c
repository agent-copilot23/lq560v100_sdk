/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include "ot_eis_errno.h"
#include "ot_eis_vi_sns.h"
#include "ox03f10_cmos_ex.h"
#include "ox03f10_cmos.h"
#include "ot_eis_vi_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_awb_api.h"
#include "ot_eis_vi_3a.h"
#include "math.h"

#define OX03F10_ID                    3
#define SENSOR_OX03F10_WIDTH          1920
#define SENSOR_OX03F10_HEIGHT         1536

#define OX03F10_RATIO                 64

#define high_h_h_8bits(x) (((x) & 0xff000000) >> 24)
#define high_h_8bits(x)  (((x) & 0x00ff0000)>>16)
#define higher_2bits(x) (((x) & 0x30000) >> 16)
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)
#define high_8_in_16_bit(x)  (((x) & 0xff00) >> 8)
#define low_8_in_16_bit(x)  ((x) & 0x00ff)
#define high_4_in_8_bit(x)  (((x) & 0xf0)>>4)
#define low_4_in_8_bit(x)  ((x) & 0x0f)
#define high_4_in_14_bit(x)  (((x) & 0x3c00)>>10)
#define mid_8_in_14_bit(x)  (((x) & 0x03fc)>>2)
#define low_2_in_14_bit(x)  ((x) & 0x0003)

#define MAX2(x, y)       ((x) > (y) ? (x) : (y))
#define MIN2(x, y)       ((x) < (y) ? (x) : (y))
#define shutter_min_row (4)
#define shutter_max_row_offset (15)      //min offset: 12 + shutter_max_row_vs
#define shutter_max_row_vs (3)      //max 35, usually use:0 - 3
#define clip3(x, min, max)         ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define OX03F10_AWB_REVERSE                    (0x5003)
#define OX03F10_SPD_B_GAIN_H                   (0x5680)
#define OX03F10_SPD_B_GAIN_L                   (0x5681)
#define OX03F10_SPD_GB_GAIN_H                  (0x5682)
#define OX03F10_SPD_GB_GAIN_L                  (0x5683)
#define OX03F10_SPD_GR_GAIN_H                  (0x5684)
#define OX03F10_SPD_GR_GAIN_L                  (0x5685)
#define OX03F10_SPD_R_GAIN_H                   (0x5686)
#define OX03F10_SPD_R_GAIN_L                   (0x5687)
#define OX03F10_nCT_H                          (0x5B76)
#define OX03F10_nCT_L                          (0x5B77)

#define OX03F10_HCG_B_GAIN_H                   (0x5280)
#define OX03F10_HCG_B_GAIN_L                   (0x5281)
#define OX03F10_HCG_GB_GAIN_H                  (0x5282)
#define OX03F10_HCG_GB_GAIN_L                  (0x5283)
#define OX03F10_HCG_GR_GAIN_H                  (0x5284)
#define OX03F10_HCG_GR_GAIN_L                  (0x5285)
#define OX03F10_HCG_R_GAIN_H                   (0x5286)
#define OX03F10_HCG_R_GAIN_L                   (0x5287)

#define OX03F10_LCG_B_GAIN_H                   (0x5480)
#define OX03F10_LCG_B_GAIN_L                   (0x5481)
#define OX03F10_LCG_GB_GAIN_H                  (0x5482)
#define OX03F10_LCG_GB_GAIN_L                  (0x5483)
#define OX03F10_LCG_GR_GAIN_H                  (0x5484)
#define OX03F10_LCG_GR_GAIN_L                  (0x5485)
#define OX03F10_LCG_R_GAIN_H                   (0x5486)
#define OX03F10_LCG_R_GAIN_L                   (0x5487)

#define OX03F10_VS_B_GAIN_H                   (0x5880)
#define OX03F10_VS_B_GAIN_L                   (0x5881)
#define OX03F10_VS_GB_GAIN_H                  (0x5882)
#define OX03F10_VS_GB_GAIN_L                  (0x5883)
#define OX03F10_VS_GR_GAIN_H                  (0x5884)
#define OX03F10_VS_GR_GAIN_L                  (0x5885)
#define OX03F10_VS_R_GAIN_H                   (0x5886)
#define OX03F10_VS_R_GAIN_L                   (0x5887)

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define ox03f10_sensor_set_ctx(pipe, ctx)   ((g_ox03f10_sns_state[pipe]) = (ctx))
#define ox03f10_sensor_reset_ctx(pipe)      (g_ox03f10_sns_state[pipe] = OT_NULL)

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

ot_eis_sns_bus_info g_ox03f10_bus_info[OT_EIS_VI_MAX_PIPE_NUM] = {
        [0] = {
            .com_bus.i2c_dev = 0,
            .bus_addr = OX03F10_I2C_ADDR
        },
        [1 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = {
            .com_bus.i2c_dev = -1,
            .bus_addr = OX03F10_I2C_ADDR
        }
};

ot_eis_sns_state *g_ox03f10_sns_state[OT_EIS_VI_MAX_PIPE_NUM] = {OT_NULL};

static ot_bool blc_clamp_info[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = OT_TRUE};

ot_eis_sns_reg_table *ox03f_get_ext_sensor_table(ot_vi_pipe_id vi_pipe)
{
    return &g_ext_table[vi_pipe][0];
}

ot_u8 ox03f_get_ext_sensor_flag(ot_vi_pipe_id vi_pipe)
{
    return g_ext_sensor_table_flag[vi_pipe];
}

ot_eis_sns_bus_info *ox03f10_get_bus_info(ot_vi_pipe_id vi_pipe)
{
    return &g_ox03f10_bus_info[vi_pipe];
}

ot_eis_sns_state *ox03f10_get_ctx(ot_vi_pipe_id vi_pipe)
{
    return g_ox03f10_sns_state[vi_pipe];
}

ot_void ox03f10_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en)
{
    blc_clamp_info[vi_pipe] = clamp_en;
}

const ox03f10_video_mode_tbl g_ox03f10_mode_tbl[OX03F10_MODE_BUTT] = {
    {OX03F10_VMAX_1920X1936_60FPS_LINEAR,   OX03F10_FULL_LINES_MAX,          30, 0.8,
     1920, 1536, 0, OT_EIS_WDR_MODE_NONE,       "OX03F10_SENSOR_1920X1936_60FPS_LINEAR_MODE"},
    {OX03F10_VMAX_1920X1936_60FPS_BUILT_IN_WDR,  OX03F10_FULL_LINES_MAX_BUILT_IN_WDR, 60, 5,
     1920, 1536, 0, OT_EIS_WDR_MODE_BUILT_IN,   "OX03F10_SENSOR_1920X1936_60FPS_BUILT_IN_MODE"},
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* OX03F10 Register Address */
#define OX03F10_SHT_DCG_H                       (0x3501) /*  SHT_DCG[15:8]  */
#define OX03F10_SHT_DCG_L                       (0x3502) /*  SHT_DCG[7:0]  */
#define OX03F10_SHT_SPD_H                       (0x3541) /*  SHT_SPD[15:8]  */
#define OX03F10_SHT_SPD_L                       (0x3542) /*  SHT_SPD[7:0]  */
#define OX03F10_SHT_VS_H                        (0x35c1) /*  SHT_VS[15:8]  */
#define OX03F10_SHT_VS_L                        (0x35c2) /*  SHT_VS[7:0]  */

#define OX03F10_AGAIN_HCG_H                     (0x3508) /*  AGAIN_HCG_H[7:4]   REG[3:0]*/
#define OX03F10_AGAIN_HCG_L                     (0x3509) /*  AGAIN_HCG_H[3:0]   REG[7:4]*/
#define OX03F10_AGAIN_LCG_H                     (0x3588) /*  AGAIN_LCG_H[7:4]   REG[3:0]*/
#define OX03F10_AGAIN_LCG_L                     (0x3589) /*  AGAIN_LCG_H[3:0]   REG[7:4]*/
#define OX03F10_AGAIN_SPD_H                     (0x3548) /*  AGAIN_SPD_H[7:4]   REG[3:0]*/
#define OX03F10_AGAIN_SPD_L                     (0x3549) /*  AGAIN_SPD_H[3:0]   REG[7:4]*/
#define OX03F10_AGAIN_VS_H                      (0x35c8) /*  AGAIN_VS_H[7:4]   REG[3:0]*/
#define OX03F10_AGAIN_VS_L                      (0x35c9) /*  AGAIN_VS_H[3:0]   REG[7:4]*/

#define OX03F10_DGAIN_HCG_H                     (0x350A) /*  DGAIN_HCG_H[13:10]  REG[3:0]*/
#define OX03F10_DGAIN_HCG_M                     (0x350B) /*  DGAIN_HCG_M[9:2]   REG[7:0]*/
#define OX03F10_DGAIN_HCG_L                     (0x350C) /*  DGAIN_HCG_L[1:0]   REG[7:6]*/
#define OX03F10_DGAIN_LCG_H                     (0x358A) /*  DGAIN_LCG_H[13:10]  REG[3:0]*/
#define OX03F10_DGAIN_LCG_M                     (0x358B) /*  DGAIN_LCG_M[9:2]   REG[7:0]*/
#define OX03F10_DGAIN_LCG_L                     (0x358C) /*  DGAIN_LCG_L[1:0]   REG[7:6]*/
#define OX03F10_DGAIN_SPD_H                     (0x354A) /*  DGAIN_SPD_H[13:10]  REG[3:0]*/
#define OX03F10_DGAIN_SPD_M                     (0x354B) /*  DGAIN_SPD_M[9:2]   REG[7:0]*/
#define OX03F10_DGAIN_SPD_L                     (0x354C) /*  DGAIN_SPD_L[1:0]   REG[7:6]*/

#define OX03F10_DGAIN_VS_H                      (0x35CA) /*  DGAIN_VS_H[13:10]  REG[3:0]*/
#define OX03F10_DGAIN_VS_M                      (0x35CB) /*  DGAIN_VS_M[9:2]   REG[7:0]*/
#define OX03F10_DGAIN_VS_L                      (0x35CC) /*  DGAIN_VS_L[1:0]   REG[7:6]*/



#define OX03F10_VMAX_H                           (0x380e) /*  VMAX_H[15:8]   REG[7:0]*/
#define OX03F10_VMAX_L                           (0x380f) /*  VMAX_L[7:0]   REG[7:0]*/


#define ox03f10_err_mode_print(sns_image_mode, sns_state) \
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
    ae_sns_dft->full_lines_max = OX03F10_FULL_LINES_MAX;
    ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 60); /* 1000000000ns, 60fps */


    ae_sns_dft->again_accu.accu_type = OT_EIS_VI_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = OT_EIS_VI_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = 1;

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 12 << ae_sns_dft->isp_dgain_shift; /* max 2 */
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

    ae_sns_dft->max_again = 15890; /* max 15872 */     //15872 - > 15890
    ae_sns_dft->min_again = 10240; /* min 1024 */          //10x-15x
    ae_sns_dft->max_again_target = 15890;
    ae_sns_dft->min_again_target = 1024;

    ae_sns_dft->max_dgain = 6144; /* max 15872 */         //15.5x
    ae_sns_dft->min_dgain = 1024; /* min 1024 */
    ae_sns_dft->max_dgain_target = 16368;
    ae_sns_dft->min_dgain_target = 1024;

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 255 << ae_sns_dft->isp_dgain_shift; /* max 255 */

    ae_sns_dft->ae_compensation = 0x1E;                                                  //TBD
    ae_sns_dft->ae_exp_mode = OT_EIS_VI_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 76151; /* init 76151 */

    ae_sns_dft->max_int_time = sns_state->fl_std - shutter_max_row_offset; /* sub 47 */
    ae_sns_dft->min_int_time = shutter_min_row; /* min 4 */
    ae_sns_dft->max_int_time_target = 65535; /* max 65535 */
    ae_sns_dft->min_int_time_target = shutter_min_row; /* min 1 */
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
    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
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
        sns_state->regs_info[0].i2c_data[12].data = high_8_in_16_bit(vmax);
        sns_state->regs_info[0].i2c_data[13].data = low_8_in_16_bit(vmax);
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
    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines = g_ox03f10_mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = g_ox03f10_mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = g_ox03f10_mode_tbl[sns_state->img_mode].max_fps;
    min_fps = g_ox03f10_mode_tbl[sns_state->img_mode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        sns_err_trace("Not support Fps: %f\n", fps);
        return;
    }

    achieve_fps_flag = OT_TRUE;
    vmax = (ot_u32)((lines * max_fps + (fps/2) ) / sns_div_0_to_1_float(fps));  //round
    vmax = (vmax > lines_max) ? lines_max : vmax;
    cmos_config_vmax(sns_state, vmax);

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
    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines_max = g_ox03f10_mode_tbl[sns_state->img_mode].max_ver_lines;

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

    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
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
    ot_u32 int_dcg;

    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);


    int_dcg = int_time; /* mode 2 */       //unit: double lines

    //0 for 0.5 double lines , 1/2/3 for 1.5/2.5/3.5 double lines

    int_dcg = MIN2(int_dcg, 0xFFFFF);

    int_dcg = MIN2(MAX2(int_dcg, shutter_min_row), sns_state->fl[0] - shutter_max_row_offset); /* min 2, sub 47 */

    sns_state->regs_info[0].i2c_data[0].data = high_8_in_16_bit(int_dcg);

    sns_state->regs_info[0].i2c_data[1].data = low_8_in_16_bit(int_dcg);

    sns_state->regs_info[0].i2c_data[2].data = high_8_in_16_bit(int_dcg);
    sns_state->regs_info[0].i2c_data[3].data = low_8_in_16_bit(int_dcg);

    return;
}

/* while vi notify ae to update sensor regs, ae call these funcs. */
static ot_void cmos_inttime_update(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
        cmos_inttime_update_built_in_wdr(vi_pipe, int_time);
    } else {
        cmos_inttime_update_linear(vi_pipe, int_time);
    }

    return;
}
#define AGAIN_NODE_NUM    64
#define DGAIN_NODE_NUM   960
static ot_u32 g_again_table[AGAIN_NODE_NUM] = {
1024,1088,1152,1216,1280,1344,1408,1472,1536,1600,1664,1728,1792,1856,1920,1984,2048,2176,2304,2432,2560,2688,2816,2944,3072,3200,3328,3456,3584,3712,3840,3968,4096,4352,4608,4864,5120,5376,5632,5888,6144,6400,6656,6912,7168,7424,7680,7936,8192,8704,9216,9728,10240,10752,11264,11776,12288,12800,13312,13824,14336,14848,15360,15872};
static ot_u32 g_dgain_table[DGAIN_NODE_NUM] = {
1024,1040,1056,1072,1088,1104,1120,1136,1152,1168,1184,1200,1216,1232,1248,1264,1280,1296,1312,1328,1344,1360,1376,1392,1408,1424,1440,1456,1472,1488,1504,1520,1536,1552,1568,1584,1600,1616,1632,1648,1664,1680,1696,1712,1728,1744,1760,1776,1792,1808,1824,1840,1856,1872,1888,1904,1920,1936,1952,1968,1984,2000,2016,2032,2048,2064,2080,2096,2112,2128,2144,2160,2176,2192,2208,2224,2240,2256,2272,2288,2304,2320,2336,2352,2368,2384,2400,2416,2432,2448,2464,2480,2496,2512,2528,2544,2560,2576,2592,2608,2624,2640,2656,2672,2688,2704,2720,2736,2752,2768,2784,2800,2816,2832,2848,2864,2880,2896,2912,2928,2944,2960,2976,2992,3008,3024,3040,3056,3072,3088,3104,3120,3136,3152,3168,3184,3200,3216,3232,3248,3264,3280,3296,3312,3328,3344,3360,3376,3392,3408,3424,3440,3456,3472,3488,3504,3520,3536,3552,3568,3584,3600,3616,3632,3648,3664,3680,3696,3712,3728,3744,3760,3776,3792,3808,3824,3840,3856,3872,3888,3904,3920,3936,3952,3968,3984,4000,4016,4032,4048,4064,4080,4096,4112,4128,4144,4160,4176,4192,4208,4224,4240,4256,4272,4288,4304,4320,4336,4352,4368,4384,4400,4416,4432,4448,4464,4480,4496,4512,4528,4544,4560,4576,4592,4608,4624,4640,4656,4672,4688,4704,4720,4736,4752,4768,4784,4800,4816,4832,4848,4864,4880,4896,4912,4928,4944,4960,4976,4992,5008,5024,5040,5056,5072,5088,5104,5120,5136,5152,5168,5184,5200,5216,5232,5248,5264,5280,5296,5312,5328,5344,5360,5376,5392,5408,5424,5440,5456,5472,5488,5504,5520,5536,5552,5568,5584,5600,5616,5632,5648,5664,5680,5696,5712,5728,5744,5760,5776,5792,5808,5824,5840,5856,5872,5888,5904,5920,5936,5952,5968,5984,6000,6016,6032,6048,6064,6080,6096,6112,6128,6144,6160,6176,6192,6208,6224,6240,6256,6272,6288,6304,6320,6336,6352,6368,6384,6400,6416,6432,6448,6464,6480,6496,6512,6528,6544,6560,6576,6592,6608,6624,6640,6656,6672,6688,6704,6720,6736,6752,6768,6784,6800,6816,6832,6848,6864,6880,6896,6912,6928,6944,6960,6976,6992,7008,7024,7040,7056,7072,7088,7104,7120,7136,7152,7168,7184,7200,7216,7232,7248,7264,7280,7296,7312,7328,7344,7360,7376,7392,7408,7424,7440,7456,7472,7488,7504,7520,7536,7552,7568,7584,7600,7616,7632,7648,7664,7680,7696,7712,7728,7744,7760,7776,7792,7808,7824,7840,7856,7872,7888,7904,7920,7936,7952,7968,7984,8000,8016,8032,8048,8064,8080,8096,8112,8128,8144,8160,8176,8192,8208,8224,8240,8256,8272,8288,8304,8320,8336,8352,8368,8384,8400,8416,8432,8448,8464,8480,8496,8512,8528,8544,8560,8576,8592,8608,8624,8640,8656,8672,8688,8704,8720,8736,8752,8768,8784,8800,8816,8832,8848,8864,8880,8896,8912,8928,8944,8960,8976,8992,9008,9024,9040,9056,9072,9088,9104,9120,9136,9152,9168,9184,9200,9216,9232,9248,9264,9280,9296,9312,9328,9344,9360,9376,9392,9408,9424,9440,9456,9472,9488,9504,9520,9536,9552,9568,9584,9600,9616,9632,9648,9664,9680,9696,9712,9728,9744,9760,9776,9792,9808,9824,9840,9856,9872,9888,9904,9920,9936,9952,9968,9984,10000,10016,10032,10048,10064,10080,10096,10112,10128,10144,10160,10176,10192,10208,10224,10240,10256,10272,10288,10304,10320,10336,10352,10368,10384,10400,10416,10432,10448,10464,10480,10496,10512,10528,10544,10560,10576,10592,10608,10624,10640,10656,10672,10688,10704,10720,10736,10752,10768,10784,10800,10816,10832,10848,10864,10880,10896,10912,10928,10944,10960,10976,10992,11008,11024,11040,11056,11072,11088,11104,11120,11136,11152,11168,11184,11200,11216,11232,11248,11264,11280,11296,11312,11328,11344,11360,11376,11392,11408,11424,11440,11456,11472,11488,11504,11520,11536,11552,11568,11584,11600,11616,11632,11648,11664,11680,11696,11712,11728,11744,11760,11776,11792,11808,11824,11840,11856,11872,11888,11904,11920,11936,11952,11968,11984,12000,12016,12032,12048,12064,12080,12096,12112,12128,12144,12160,12176,12192,12208,12224,12240,12256,12272,12288,12304,12320,12336,12352,12368,12384,12400,12416,12432,12448,12464,12480,12496,12512,12528,12544,12560,12576,12592,12608,12624,12640,12656,12672,12688,12704,12720,12736,12752,12768,12784,12800,12816,12832,12848,12864,12880,12896,12912,12928,12944,12960,12976,12992,13008,13024,13040,13056,13072,13088,13104,13120,13136,13152,13168,13184,13200,13216,13232,13248,13264,13280,13296,13312,13328,13344,13360,13376,13392,13408,13424,13440,13456,13472,13488,13504,13520,13536,13552,13568,13584,13600,13616,13632,13648,13664,13680,13696,13712,13728,13744,13760,13776,13792,13808,13824,13840,13856,13872,13888,13904,13920,13936,13952,13968,13984,14000,14016,14032,14048,14064,14080,14096,14112,14128,14144,14160,14176,14192,14208,14224,14240,14256,14272,14288,14304,14320,14336,14352,14368,14384,14400,14416,14432,14448,14464,14480,14496,14512,14528,14544,14560,14576,14592,14608,14624,14640,14656,14672,14688,14704,14720,14736,14752,14768,14784,14800,14816,14832,14848,14864,14880,14896,14912,14928,14944,14960,14976,14992,15008,15024,15040,15056,15072,15088,15104,15120,15136,15152,15168,15184,15200,15216,15232,15248,15264,15280,15296,15312,15328,15344,15360,15376,15392,15408,15424,15440,15456,15472,15488,15504,15520,15536,15552,15568,15584,15600,15616,15632,15648,15664,15680,15696,15712,15728,15744,15760,15776,15792,15808,15824,15840,15856,15872,15888,15904,15920,15936,15952,15968,15984,16000,16016,16032,16048,16064,16080,16096,16112,16128,16144,16160,16176,16192,16208,16224,16240,16256,16272,16288,16304,16320,16336,16352,16368};


static ot_void cmos_again_calc_table(ot_vi_pipe_id vi_pipe, ot_u32 *again_lin, ot_u32 *again_db)
{
    int i;

    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);

    if (*again_lin >= g_again_table[AGAIN_NODE_NUM - 1]) {
        *again_lin = g_again_table[AGAIN_NODE_NUM - 1];
        *again_db = (g_again_table[AGAIN_NODE_NUM - 1] >> 6);                        //10bit -> 4bit fraction
        return;
    }

    for (i = 1; i < AGAIN_NODE_NUM; i++) {
        if (*again_lin < g_again_table[i]) {
            *again_lin = g_again_table[i - 1];
            *again_db = (g_again_table[i - 1]>>6);                    //10bit -> 4bit fraction
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
        *dgain_db = g_dgain_table[DGAIN_NODE_NUM - 1];
        return;
    }

    for (i = 1; i < DGAIN_NODE_NUM; i++) {
        if (*dgain_lin < g_dgain_table[i]) {
            *dgain_lin = g_dgain_table[i - 1];
            *dgain_db = g_dgain_table[i - 1];
            break;
        }
    }
    return;
}

static ot_u32 cmos_again_calc_by_again(ot_u32 again_ref,float ratio)
{
    int i;
    float again_temp;
    ot_u32 again_target;

    again_temp = ((float)(again_ref<<6))/ratio;
    again_target = (ot_u32)again_temp;

    if (again_target >= g_again_table[AGAIN_NODE_NUM - 1]) {
        again_target = (g_again_table[AGAIN_NODE_NUM - 1]>>6);                        //10bit -> 4bit fraction
        return;
    }

    for (i = 1; i < AGAIN_NODE_NUM; i++) {
        if (again_target < g_again_table[i]) {
            again_target = (g_again_table[i - 1]>>6);                    //10bit -> 4bit fraction
            break;
        }
    }
    return again_target;
}
static ot_void cmos_gains_update(ot_vi_pipe_id vi_pipe, ot_u32 again, ot_u32 dgain)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);


    sns_state->regs_info[0].i2c_data[8].data = 1;
    sns_state->regs_info[0].i2c_data[9].data = 0;
    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE)
        {  //hum
        sns_state->regs_info[0].i2c_data[6].data = low_8bits(again); /* index 3 */
        sns_state->regs_info[0].i2c_data[7].data = high_8bits(again); /* index 4 */
		}
		else if (sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
            //a gain
            sns_state->regs_info[0].i2c_data[4].data = (high_4_in_8_bit(again))&(0x0f); /* high 4 place in low 4 */
            sns_state->regs_info[0].i2c_data[5].data = ((low_4_in_8_bit(again))<<4); /* low 4 place in high 4 */

            //d gain
            sns_state->regs_info[0].i2c_data[6].data = high_4_in_14_bit(dgain); /* 14bit high 4 place in low 4 */
            sns_state->regs_info[0].i2c_data[7].data = mid_8_in_14_bit(dgain);  /* 14bit mid 8*/
            ot_u32 again_base = 16;    //again 16x (4 fraction)
            ot_u32 dgain_base = 1024;  //dgain 1024x (10 fraction)
            ot_u32 lcg_again_default = 1024;   //default 1.0   get again db (1024 base)
            ot_u32 lcg_dgain_default = 1024;   //default 1.0
            ot_u32 spd_again_default = 4352;   //default 4.25
            ot_u32 spd_dgain_default = 1024;   //default 1.0
            ot_float hcg_lcg_reg = 16.0;        //too small ng
            ot_float again_max_reg = 15.5;
            ot_u32 hcg_lcg_ratio = (ot_u32)(hcg_lcg_reg * again_base * dgain_base);   // ratio * 1024
            ot_u32 again_max = again_max_reg * 1024; // get again db (1024 base)

            ot_u32 lcg_again,lcg_dgain,spd_again,spd_dgain,total_gain;
            ot_u32 lcg_again_db,lcg_dgain_db,spd_again_db,spd_dgain_db;

            total_gain = again * dgain;
            if(total_gain < hcg_lcg_ratio)
            {
                lcg_again = lcg_again_default;
                lcg_dgain = lcg_dgain_default;
                spd_again = spd_again_default;
                spd_dgain = spd_dgain_default;
            }

            else
            {

                lcg_again = (total_gain * 1024)/hcg_lcg_ratio;               // get again db (1024 base)
                lcg_dgain = lcg_dgain_default;
                spd_again = (lcg_again * spd_again_default)/lcg_again_default;
                spd_dgain = spd_dgain_default;
                if(lcg_again>again_max)      //again max
                {
                    lcg_dgain = (lcg_again * 1024)/again_max;   //get dgain db (1024 base)
                    lcg_again = again_max;
                }
                if(spd_again>again_max)      //again max
                {
                    spd_dgain = (spd_again * 1024)/again_max;   //get dgain db (1024 base)
                    spd_again = again_max;
                }
            }

            cmos_again_calc_table(vi_pipe,&lcg_again,&lcg_again_db);
            cmos_dgain_calc_table(vi_pipe,&lcg_dgain,&lcg_dgain_db);
            cmos_again_calc_table(vi_pipe,&spd_again,&spd_again_db);
            cmos_dgain_calc_table(vi_pipe,&spd_dgain,&spd_dgain_db);
            //a gain
            sns_state->regs_info[0].i2c_data[8].data = (high_4_in_8_bit(lcg_again_db))&(0x0f); /* high 4 place in low 4 */
            sns_state->regs_info[0].i2c_data[9].data = ((low_4_in_8_bit(lcg_again_db))<<4); /* low 4 place in high 4 */
            //a gain
            sns_state->regs_info[0].i2c_data[10].data = (high_4_in_8_bit(spd_again_db))&(0x0f); /* high 4 place in low 4 */
            sns_state->regs_info[0].i2c_data[11].data = ((low_4_in_8_bit(spd_again_db))<<4); /* low 4 place in high 4 */
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
    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    switch (sns_state->wdr_mode) {
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
#define CALIBRATE_STATIC_WB_R_GAIN                    498
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    450

/* Calibration results for Auto WB Planck */                       //AWB calibration
#define CALIBRATE_AWB_P1                              (50)
#define CALIBRATE_AWB_P2                              (109)
#define CALIBRATE_AWB_Q1                              (-96)
#define CALIBRATE_AWB_A1                              (376)
#define CALIBRATE_AWB_B1                              -60488
#define CALIBRATE_AWB_C1                              (11598881)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static ot_s32 cmos_get_awb_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_sensor_default *awb_sns_dft)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(awb_sns_dft);
    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (ot_void)memset_s(awb_sns_dft, sizeof(ot_eis_vi_awb_sensor_default), 0, sizeof(ot_eis_vi_awb_sensor_default));

    if (g_ext_awb_flag[vi_pipe] == 1) {
        //g_ext_sns_awb include wb_cali_param & ccm & preawb param
        (ot_void)memcpy_s(awb_sns_dft, sizeof(ot_eis_vi_awb_sensor_default), &g_ext_sns_awb[vi_pipe],
                          sizeof(ot_eis_vi_awb_sensor_default));

        awb_sns_dft->init_rgain = g_init_wb_gain[vi_pipe][0]; /* 0: Rgain */
        awb_sns_dft->init_ggain = g_init_wb_gain[vi_pipe][1]; /* 1: Ggain */
        awb_sns_dft->init_bgain = g_init_wb_gain[vi_pipe][2]; /* 2: Bgain */
        awb_sns_dft->sample_rgain = g_sample_r_gain[vi_pipe];
        awb_sns_dft->sample_bgain = g_sample_b_gain[vi_pipe];
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
        awb_sns_dft->preawb_attr.enable = 1;
        ot_u16 color_temp_array[OT_EIS_VI_AWB_PREAWB_LUT_SIZE]     = { 2400, 2800, 4000, 5000, 6500, 7500 };
        ot_u32 preawb_ratio_r_array[OT_EIS_VI_AWB_PREAWB_LUT_SIZE] = { 67051, 66168, 64001, 63188, 61240, 60919 };
        ot_u32 preawb_ratio_b_array[OT_EIS_VI_AWB_PREAWB_LUT_SIZE] = { 58385, 52546, 68416, 69240, 70212, 71045 };
        for (int i = 0; i < OT_EIS_VI_AWB_PREAWB_LUT_SIZE; i++) {
            awb_sns_dft->preawb_attr.color_temp_array[i] = color_temp_array[i];
            awb_sns_dft->preawb_attr.ratio_r_array[i] = preawb_ratio_r_array[i];
            awb_sns_dft->preawb_attr.ratio_b_array[i] = preawb_ratio_b_array[i];
        }
    }
    return OT_SUCCESS;
}

#define PREAWB_GAIN_PREC   10
#define PREAWB_GAIN_MAX 16383
#define PREAWB_GAIN_MIN 0
#define AWB_INFO_GAIN_PREC 8
static ot_s32 cmos_set_awb_preawb(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_preawb_info *preawb_info, ot_eis_vi_awb_info *awb_info)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    ot_u16 awb_rgain, awb_bgain, awb_rgain_tune, awb_bgain_tune;
    ot_u16 color_temp, awb_rgain_i, awb_rgain_f, awb_bgain_i, awb_bgain_f;
    ot_u16 awb_rgain_tune_i, awb_rgain_tune_f, awb_bgain_tune_i, awb_bgain_tune_f;

    if (preawb_info->enable) {
        awb_rgain = awb_info->r_gain << (PREAWB_GAIN_PREC - AWB_INFO_GAIN_PREC);  // 0~16383, 4.10
        awb_bgain = awb_info->b_gain << (PREAWB_GAIN_PREC - AWB_INFO_GAIN_PREC);  // 0~16383, 4.10
        awb_rgain_tune = clip3((((ot_u32)awb_rgain * (ot_u32)preawb_info->rg_ratio) >>
                                        (OT_EIS_VI_AWB_PREAWB_RATIO_PREC)),
                                        PREAWB_GAIN_MIN, PREAWB_GAIN_MAX); // 0~16383, 4.10
        awb_bgain_tune = clip3((((ot_u32)awb_bgain * (ot_u32)preawb_info->bg_ratio) >>
                                        (OT_EIS_VI_AWB_PREAWB_RATIO_PREC)),
                                        PREAWB_GAIN_MIN, PREAWB_GAIN_MAX); // 0~16383, 4.10

        awb_rgain_i = awb_rgain >> PREAWB_GAIN_PREC;
        awb_rgain_f = awb_rgain % (1 << PREAWB_GAIN_PREC);
        awb_bgain_i = awb_bgain >> PREAWB_GAIN_PREC;
        awb_bgain_f = awb_bgain % (1 << PREAWB_GAIN_PREC);
        awb_rgain_tune_i = awb_rgain_tune >> PREAWB_GAIN_PREC;
        awb_rgain_tune_f = awb_rgain_tune % (1 << PREAWB_GAIN_PREC);
        awb_bgain_tune_i = awb_bgain_tune >> PREAWB_GAIN_PREC;
        awb_bgain_tune_f = awb_bgain_tune % (1 << PREAWB_GAIN_PREC);

    } else {
        awb_rgain_i = 1;
        awb_rgain_f = 0;
        awb_bgain_i = 1;
        awb_bgain_f = 0;
        awb_rgain_tune_i = 1;
        awb_rgain_tune_f = 0;
        awb_bgain_tune_i = 1;
        awb_bgain_tune_f = 0;

    }
    color_temp = awb_info->color_temp;

    // ct
    sns_state->regs_info[0].i2c_data[14].data = low_8bits(color_temp);
    sns_state->regs_info[0].i2c_data[15].data = high_8bits(color_temp);
    // spd
    sns_state->regs_info[0].i2c_data[16].data = low_8bits((int)((awb_bgain_tune_i << 2) + (awb_bgain_tune_f >> 8)));
    sns_state->regs_info[0].i2c_data[17].data = low_8bits((int)(awb_bgain_tune_f % 256));
    sns_state->regs_info[0].i2c_data[18].data = low_8bits((int)((awb_rgain_tune_i << 2) + (awb_rgain_tune_f >> 8)));
    sns_state->regs_info[0].i2c_data[19].data = low_8bits((int)(awb_rgain_tune_f % 256));
    // hcg
    sns_state->regs_info[0].i2c_data[20].data = low_8bits((int)((awb_bgain_i << 2) + (awb_bgain_f >> 8)));
    sns_state->regs_info[0].i2c_data[21].data = low_8bits((int)(awb_bgain_f % 256));
    sns_state->regs_info[0].i2c_data[22].data = low_8bits((int)((awb_rgain_i << 2) + (awb_rgain_f >> 8)));
    sns_state->regs_info[0].i2c_data[23].data = low_8bits((int)(awb_rgain_f % 256));
    // lcg
    sns_state->regs_info[0].i2c_data[24].data = low_8bits((int)((awb_bgain_i << 2) + (awb_bgain_f >> 8)));
    sns_state->regs_info[0].i2c_data[25].data = low_8bits((int)(awb_bgain_f % 256));
    sns_state->regs_info[0].i2c_data[26].data = low_8bits((int)((awb_rgain_i << 2) + (awb_rgain_f >> 8)));
    sns_state->regs_info[0].i2c_data[27].data = low_8bits((int)(awb_rgain_f % 256));
    // vs
    sns_state->regs_info[0].i2c_data[28].data = low_8bits((int)((awb_bgain_i << 2) + (awb_bgain_f >> 8)));
    sns_state->regs_info[0].i2c_data[29].data = low_8bits((int)(awb_bgain_f % 256));
    sns_state->regs_info[0].i2c_data[30].data = low_8bits((int)((awb_rgain_i << 2) + (awb_rgain_f >> 8)));
    sns_state->regs_info[0].i2c_data[31].data = low_8bits((int)(awb_rgain_f % 256));

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
    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
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
        isp_def->key.bit1_lsc      = 1;
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

    isp_def->sns_mode.sns_id = OX03F10_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;

    return OT_SUCCESS;
}

static ot_s32 cmos_get_vi_black_level(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_black_level *black_level)
{
    ot_s32  i;
    ot_eis_sns_state *sns_state = OT_NULL;
    const ot_eis_vi_sns_black_level *cmos_blc_def = OT_NULL;

    sns_check_pointer_return(black_level);
    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
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

    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->img_mode == OX03F10_SENSOR_1920X1936_60FPS_LINEAR_MODE) {
            full_lines_5fps = (OX03F10_VMAX_1920X1936_60FPS_LINEAR * 60) / 5; /* 30fps, 5fps */
        } else {
            return;
        }
    }


    return;
}

static ot_s32 cmos_set_wdr_mode(ot_vi_pipe_id vi_pipe, ot_u8 mode)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
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
    sns_state->regs_info[0].com_bus.i2c_dev = g_ox03f10_bus_info[vi_pipe].com_bus.i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = 32; // TODO

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
        sns_state->regs_info[0].reg_num = 32;
        sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    }

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = OT_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = g_ox03f10_bus_info[vi_pipe].bus_addr;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = OX03F10_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = OX03F10_DATA_BYTE;
    }

    return;
}

static ot_void cmos_linear_sns_reg_info_init(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
    return;
}

static ot_void cmos_built_in_sns_reg_info_init(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
	// shutter related
    sns_state->regs_info[0].i2c_data[0].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[0].reg_addr = OX03F10_SHT_DCG_H;
    sns_state->regs_info[0].i2c_data[1].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[1].reg_addr = OX03F10_SHT_DCG_L;
    sns_state->regs_info[0].i2c_data[2].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[2].reg_addr = OX03F10_SHT_SPD_H;
    sns_state->regs_info[0].i2c_data[3].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[3].reg_addr = OX03F10_SHT_SPD_L;

	// hcg gain
    sns_state->regs_info[0].i2c_data[4].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[4].reg_addr = OX03F10_AGAIN_HCG_H;
    sns_state->regs_info[0].i2c_data[5].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[5].reg_addr = OX03F10_AGAIN_HCG_L;

	// dgain related
    sns_state->regs_info[0].i2c_data[6].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[6].reg_addr = OX03F10_DGAIN_HCG_H;
    sns_state->regs_info[0].i2c_data[7].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[7].reg_addr = OX03F10_DGAIN_HCG_M;

	// lcg gain
    sns_state->regs_info[0].i2c_data[8].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[8].reg_addr = OX03F10_AGAIN_LCG_H;
    sns_state->regs_info[0].i2c_data[9].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[9].reg_addr = OX03F10_AGAIN_LCG_L;

	// spd gain
    sns_state->regs_info[0].i2c_data[10].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[10].reg_addr = OX03F10_AGAIN_SPD_H;
    sns_state->regs_info[0].i2c_data[11].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[11].reg_addr = OX03F10_AGAIN_SPD_L;

    // VTS related    be init to 0 ,error
    sns_state->regs_info[0].i2c_data[12].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[12].reg_addr = OX03F10_VMAX_H;
    sns_state->regs_info[0].i2c_data[13].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[13].reg_addr = OX03F10_VMAX_L;
    // preawb
    // ct
    sns_state->regs_info[0].i2c_data[14].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[14].reg_addr = OX03F10_nCT_L;
    sns_state->regs_info[0].i2c_data[15].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[15].reg_addr = OX03F10_nCT_H;
    // spd
    sns_state->regs_info[0].i2c_data[16].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[16].reg_addr = OX03F10_SPD_B_GAIN_H;
    sns_state->regs_info[0].i2c_data[17].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[17].reg_addr = OX03F10_SPD_B_GAIN_L;
    sns_state->regs_info[0].i2c_data[18].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[18].reg_addr = OX03F10_SPD_R_GAIN_H;
    sns_state->regs_info[0].i2c_data[19].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[19].reg_addr = OX03F10_SPD_R_GAIN_L;
    // hcg
    sns_state->regs_info[0].i2c_data[20].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[20].reg_addr = OX03F10_HCG_B_GAIN_H;
    sns_state->regs_info[0].i2c_data[21].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[21].reg_addr = OX03F10_HCG_B_GAIN_L;
    sns_state->regs_info[0].i2c_data[22].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[22].reg_addr = OX03F10_HCG_R_GAIN_H;
    sns_state->regs_info[0].i2c_data[23].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[23].reg_addr = OX03F10_HCG_R_GAIN_L;
    // lcg
    sns_state->regs_info[0].i2c_data[24].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[24].reg_addr = OX03F10_LCG_B_GAIN_H;
    sns_state->regs_info[0].i2c_data[25].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[25].reg_addr = OX03F10_LCG_B_GAIN_L;
    sns_state->regs_info[0].i2c_data[26].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[26].reg_addr = OX03F10_LCG_R_GAIN_H;
    sns_state->regs_info[0].i2c_data[27].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[27].reg_addr = OX03F10_LCG_R_GAIN_L;
    // vs
    sns_state->regs_info[0].i2c_data[28].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[28].reg_addr = OX03F10_VS_B_GAIN_H;
    sns_state->regs_info[0].i2c_data[29].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[29].reg_addr = OX03F10_VS_B_GAIN_L;
    sns_state->regs_info[0].i2c_data[30].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[30].reg_addr = OX03F10_VS_R_GAIN_H;
    sns_state->regs_info[0].i2c_data[31].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[31].reg_addr = OX03F10_VS_R_GAIN_L;
    // awb reverse

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
    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
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
        case OX03F10_SENSOR_1920X1936_60FPS_LINEAR_MODE:
            sns_state->fl_std = OX03F10_VMAX_1920X1936_60FPS_LINEAR;
            break;
        case OX03F10_SENSOR_1920X1936_60FPS_BUILT_IN_MODE:
            sns_state->fl_std = OX03F10_VMAX_1920X1936_60FPS_BUILT_IN_WDR;
            break;
        default:
            sns_state->fl_std = OX03F10_VMAX_1920X1936_60FPS_LINEAR;
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
    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < OX03F10_MODE_BUTT; i++) {
        if (sns_image_mode->fps <= g_ox03f10_mode_tbl[i].max_fps &&
            sns_image_mode->width <= g_ox03f10_mode_tbl[i].width &&
            sns_image_mode->height <= g_ox03f10_mode_tbl[i].height &&
            sns_state->wdr_mode == g_ox03f10_mode_tbl[i].wdr_mode) {
            image_mode = (ox03f10_res_mode)i;
            break;
        }
    }

    if (i >= OX03F10_MODE_BUTT) {
        ox03f10_err_mode_print(sns_image_mode, sns_state);
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

    ox03f10_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = OT_FALSE;
    sns_state->sync_init = OT_FALSE;
    sns_state->img_mode  = OX03F10_SENSOR_1920X1936_60FPS_LINEAR_MODE;
    sns_state->wdr_mode  = OT_EIS_WDR_MODE_NONE;
    sns_state->fl_std    = OX03F10_VMAX_1920X1936_60FPS_LINEAR;
    sns_state->fl[0]     = OX03F10_VMAX_1920X1936_60FPS_LINEAR;
    sns_state->fl[1]     = OX03F10_VMAX_1920X1936_60FPS_LINEAR;

    (ot_void)memset_s(&sns_state->regs_info[0], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));
    (ot_void)memset_s(&sns_state->regs_info[1], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));

    return;
}

static ot_s32 sensor_ctx_init(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    ox03f10_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == OT_NULL) {
        sns_state_ctx = (ot_eis_sns_state *)malloc(sizeof(ot_eis_sns_state));
        if (sns_state_ctx == OT_NULL) {
            sns_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_FAILURE;
        }
    }

    (ot_void)memset_s(sns_state_ctx, sizeof(ot_eis_sns_state), 0, sizeof(ot_eis_sns_state));

    ox03f10_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return OT_SUCCESS;
}

static ot_void sensor_ctx_exit(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    ox03f10_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    ox03f10_sensor_reset_ctx(vi_pipe);

    return;
}

static ot_eis_vi_sns_register_func g_ox03f10_register_to_vi_func = {
    .pfn_sns_init                = ox03f10_init,
    .pfn_sns_exit                = ox03f10_exit,
    .pfn_sns_global_init         = sensor_global_init,
    .pfn_sns_set_image_mode      = cmos_set_image_mode,
    .pfn_sns_set_wdr_mode        = cmos_set_wdr_mode,
    .pfn_sns_get_vi_alg_default  = cmos_get_vi_alg_default,
    .pfn_sns_get_vi_black_level  = cmos_get_vi_black_level,
    .pfn_sns_get_blc_clamp_info  = cmos_get_blc_clamp_info,
    .pfn_sns_set_pixel_detect    = cmos_set_pixel_detect,
    .pfn_sns_get_reg_info        = cmos_get_sns_regs_info,
};

static ot_eis_vi_ae_sensor_register_func g_ox03f10_register_to_ae_func = {
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

static ot_eis_vi_awb_sensor_register_func g_ox03f10_register_to_awb_func = {
    .pfn_sns_get_awb_default = cmos_get_awb_default,
    .pfn_sns_set_awb_preawb = cmos_set_awb_preawb,
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

    ret = ot_eis_vi_sensor_register_to_vi(pipe_hdl, &g_ox03f10_register_to_vi_func, OX03F10_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to vi failed!\n");
        goto free_sns_ctx;
    }

    ret = ot_eis_vi_sensor_register_to_ae(pipe_hdl, &g_ox03f10_register_to_ae_func, OX03F10_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to ae lib failed!\n");
        goto sns_unreg_to_vi;
    }

    ret = ot_eis_vi_sensor_register_to_awb(pipe_hdl, &g_ox03f10_register_to_awb_func, OX03F10_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to awb lib failed!\n");
        goto sns_unreg_to_ae;
    }

    return OT_SUCCESS;

sns_unreg_to_ae:
    ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, OX03F10_ID);
sns_unreg_to_vi:
    ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, OX03F10_ID);
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

    ret = ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, OX03F10_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }
    ret = ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, OX03F10_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_eis_vi_sensor_unregister_to_awb(pipe_hdl, OX03F10_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to awb lib failed!\n");
        return ret;
    }

    sensor_ctx_exit(master_pipe_id);

    return OT_SUCCESS;
}

static ot_s32 ox03f10_set_bus_info(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_bus_info *sns_bus_info)
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

    memcpy_s(&g_ox03f10_bus_info[master_pipe_id], sizeof(ot_eis_sns_bus_info), sns_bus_info, sizeof(ot_eis_sns_bus_info));

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

static ot_s32 ox03f_set_alg_default(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_sns_alg_default *sns_alg_def)
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

static ot_s32 ox03f_set_awb_default(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_sensor_default *sns_awb_def)
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

static ot_s32 ox03f_set_reg_table(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_reg_table *reg_table, ot_s32 table_cnt)
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

ot_eis_sns_obj g_sns_ox03f10_obj = {
    .pfn_sns_register              = sensor_register_callback,
    .pfn_sns_unregister            = sensor_unregister_callback,
    .pfn_sns_standby               = ox03f10_standby,
    .pfn_sns_restart               = ox03f10_restart,
    .pfn_sns_set_mirror_flip       = OT_NULL,
    .pfn_sns_set_blc_clamp         = ox03f10_blc_clamp,
    .pfn_sns_set_bus_info          = ox03f10_set_bus_info,
    .pfn_sns_set_init              = sensor_set_init,
    .pfn_sns_set_alg_default       = ox03f_set_alg_default,
    .pfn_sns_set_awb_default       = ox03f_set_awb_default,
    .pfn_sns_set_reg_table         = ox03f_set_reg_table,
};
