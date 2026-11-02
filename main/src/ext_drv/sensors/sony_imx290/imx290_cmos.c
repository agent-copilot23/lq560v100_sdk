/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include "ot_eis_errno.h"
#include "ot_eis_vi_sns.h"
#include "imx290_cmos_ex.h"
#include "imx290_cmos.h"
#include "ot_eis_vi_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_awb_api.h"

#define IMX290_ID                    290
#define SENSOR_IMX290_WIDTH          1920
#define SENSOR_IMX290_HEIGHT         1080
#define BLACK_LEVEL_2TO1_WDR_R       0x400
#define BLACK_LEVEL_2TO1_WDR_GR      0x400
#define BLACK_LEVEL_2TO1_WDR_GB      0x400
#define BLACK_LEVEL_2TO1_WDR_B       0x400

/* for dol_2 */
#define LEF_MAX (IMX290_VMAX_2M_60FPS_10BIT_2TO1_WDR*2 -2)    /*tbd VMAX or FSC*/
#define SHS1_MIN 2    //min 2
#define RHS1_MIN 5    //2*N+5
#define IMX290_MARGIN 2  //tbd
#define BRL 1109

#define higher_4bits(x) (((x) & 0xf0000) >> 16)
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)
#define high_8_in_24bits(x) (((x) & 0xff0000) >> 16)
#define mid_8_in_24bits(x) (((x) & 0x00ff00) >> 8)
#define low_8_in_24bits(x) ((x) & 0x0000ff)

#define MAX2(x, y)       ((x) > (y) ? (x) : (y))
#define MIN2(x, y)       ((x) < (y) ? (x) : (y))

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define imx290_sensor_set_ctx(pipe, ctx)   ((g_imx290_sns_state[pipe]) = (ctx))
#define imx290_sensor_reset_ctx(pipe)      (g_imx290_sns_state[pipe] = OT_NULL)

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

typedef struct{
    ot_u32 RHS1_max;
}imx290_state_2to1_wdr;

imx290_state_2to1_wdr g_imx290_state = {
    11
};

ot_eis_sns_bus_info g_imx290_bus_info[OT_EIS_VI_MAX_PIPE_NUM] = {
        [0] = {
            .com_bus.i2c_dev = 0,
            .bus_addr = IMX290_I2C_ADDR
        },
        [1 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = {
            .com_bus.i2c_dev = -1,
            .bus_addr = IMX290_I2C_ADDR
        }
};

ot_eis_sns_state *g_imx290_sns_state[OT_EIS_VI_MAX_PIPE_NUM] = {OT_NULL};

static ot_bool blc_clamp_info[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = OT_TRUE};

ot_eis_sns_bus_info *imx290_get_bus_info(ot_vi_pipe_id vi_pipe)
{
    return &g_imx290_bus_info[vi_pipe];
}

ot_eis_sns_state *imx290_get_ctx(ot_vi_pipe_id vi_pipe)
{
    return g_imx290_sns_state[vi_pipe];
}

ot_void imx290_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en)
{
    blc_clamp_info[vi_pipe] = clamp_en;
}

const imx290_video_mode_tbl g_imx290_mode_tbl[IMX290_MODE_BUTT] = {
    {IMX290_VMAX_2M_60FPS_12BIT_LINEAR,   IMX290_FULL_LINES_MAX,          30, 1.06,
     1920, 1080, 0, OT_EIS_WDR_MODE_NONE,       "IMX290_2M_60FPS_12BIT_LINEAR_MODE"},
    {IMX290_VMAX_2M_30FPS_12BIT_2TO1_WDR,  IMX290_FULL_LINES_MAX_2TO1_WDR, 30, 1.0,
     1920, 1080, 0, OT_EIS_WDR_MODE_2To1_LINE,  "IMX290_2M_30FPS_12BIT_2TO1_DOL_MODE"},
    {IMX290_VMAX_2M_60FPS_10BIT_2TO1_WDR,  IMX290_FULL_LINES_MAX_2TO1_WDR, 60, 30.1,
     1920, 1080, 0, OT_EIS_WDR_MODE_2To1_LINE,  "IMX290_2M_60FPS_10BIT_2TO1_DOL_MODE"},
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* IMX290 Register Address */
/* IMX290 exposure time */
#define IMX290_SHS1_L_ADDR          0x3020
#define IMX290_SHS1_M_ADDR          0x3021
#define IMX290_SHS1_H_ADDR          0x3022
#define IMX290_SHS2_L_ADDR          0x3024
#define IMX290_SHS2_M_ADDR          0x3025
#define IMX290_SHS2_H_ADDR          0x3026
#define IMX290_RHS1_L_ADDR          0x3030
#define IMX290_RHS1_M_ADDR          0x3031
#define IMX290_RHS1_H_ADDR          0x3032
/* IMX290 gain */
#define IMX290_LEF_GAIN_ADDR        0x3014
#define IMX290_SEF1_GAIN_ADDR       0x30F2
/* IMX290 VMAX */
#define IMX290_VMAX_L_ADDR          0x3018
#define IMX290_VMAX_M_ADDR          0x3019
#define IMX290_VMAX_H_ADDR          0x301A
/* IMX290 Y_OUT_SIZE */
#define IMX290_Y_OUT_SIZE_L_ADDR    0x3418
#define IMX290_Y_OUT_SIZE_H_ADDR    0x3419


#define imx290_err_mode_print(sns_image_mode, sns_state) \
do { \
    sns_err_trace("Not support! Width:%u, Height:%u, Fps:%f, WDRMode:%d\n", \
                  (sns_image_mode)->width, \
                  (sns_image_mode)->height, \
                  (sns_image_mode)->fps, \
                  (sns_state)->wdr_mode); \
} while (0)

static ot_void update_imx290_state_2to1_wdr(ot_u32 vmax)    //vmax 60 or 120, tbd
{
    g_imx290_state.RHS1_max = vmax * 2 - (BRL*2) - 21;
    g_imx290_state.RHS1_max = MAX2(g_imx290_state.RHS1_max,RHS1_MIN);
}

static ot_void cmos_get_ae_comm_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft,
    const ot_eis_sns_state *sns_state)
{
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->full_lines_max = IMX290_FULL_LINES_MAX;
    if (IMX290_2M_60FPS_10BIT_2TO1_DOL_MODE == sns_state->img_mode)
    {
        ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 60); /* 1000000000ns,  */
    }
    else if (IMX290_2M_30FPS_12BIT_2TO1_DOL_MODE == sns_state->img_mode)
    {
        ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 30); /* 1000000000ns,  */
    }

    ae_sns_dft->int_time_accu.accu_type = OT_EIS_VI_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1;
    ae_sns_dft->int_time_accu.offset = 0;

    ae_sns_dft->again_accu.accu_type = OT_EIS_VI_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = OT_EIS_VI_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = 1;

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 2 << ae_sns_dft->isp_dgain_shift; /* max 128 */

    if (g_lines_per500ms[vi_pipe] == 0) {
        if (IMX290_2M_60FPS_10BIT_2TO1_DOL_MODE == sns_state->img_mode)
        {
            ae_sns_dft->lines_per500ms = sns_state->fl_std * 60 / 2; /* , div 2 ,500ms*/
        }
        else if (IMX290_2M_30FPS_12BIT_2TO1_DOL_MODE == sns_state->img_mode)
        {
            ae_sns_dft->lines_per500ms = sns_state->fl_std * 30 / 2; /* , div 2 ,500ms*/
        }
    } else {
        ae_sns_dft->lines_per500ms = g_lines_per500ms[vi_pipe];
    }

    ae_sns_dft->max_iris_fno = OT_EIS_VI_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_fno = OT_EIS_VI_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid = OT_FALSE;   /*default false*/
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
    ae_sns_dft->max_again = 15872; /* max 15872 */
    ae_sns_dft->min_again = 1024;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 2047; /* max 2047 */
    ae_sns_dft->min_dgain = 128;  /* min 128 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode = OT_EIS_VI_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 76151; /* init 76151 */
    ae_sns_dft->init_int_time = g_init_int_time[vi_pipe];
    ae_sns_dft->init_again = g_init_again[vi_pipe];
    ae_sns_dft->init_dgain = g_init_dgain[vi_pipe];
    ae_sns_dft->init_isp_dgain = g_init_isp_dgain[vi_pipe];

    ae_sns_dft->max_int_time = sns_state->fl_std - 10; /* sub 10 */
    ae_sns_dft->min_int_time = 8; /* min int 8 */
    ae_sns_dft->max_int_time_target = 65535; /* max int 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[vi_pipe];
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_eis_vi_ae_route),
                      &g_init_ae_route[vi_pipe],  sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_eis_vi_ae_route_ex),
                      &g_init_ae_route_ex[vi_pipe], sizeof(ot_eis_vi_ae_route_ex));

    return;
}

static ot_void cmos_get_ae_2to1_line_wdr_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft,
    const ot_eis_sns_state *sns_state)
{
    ae_sns_dft->max_int_time = LEF_MAX; /* sub 10 */  /*tbd VMX or FSC*/
    ae_sns_dft->min_int_time = 2; /* min_int_time 2 */
    ae_sns_dft->int_time_accu.offset = 0; /* -0.115 line for stagger */   /*if not 0,exp_ratio not accu*/

    ae_sns_dft->max_int_time_target = 65535; /* max 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->max_again = 323820; /* max 323820 */ /*again 31.62x ,default d gain 10x max*/ /*tbd*/
    ae_sns_dft->min_again = 1024;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 1024; /* max 1024 */
    ae_sns_dft->min_dgain = 1024;  /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->max_isp_dgain_target = 4 << ae_sns_dft->isp_dgain_shift; /* max 4 << shift */
    ae_sns_dft->diff_gain_support = OT_TRUE;            /*supprot diff gain,default ture*/

    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 52000; /* init 52000 */

    if (g_fswdr_mode[vi_pipe] == OT_EIS_VI_FSWDR_LONG_FRAME_MODE) {    /*default long frame?*/
        ae_sns_dft->ae_compensation = 56; /* ae_compensation 56 */
        ae_sns_dft->ae_exp_mode = OT_EIS_VI_AE_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation = 32; /* ae_compensation 32 */
        ae_sns_dft->ae_exp_mode = OT_EIS_VI_AE_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable = OT_TRUE;
        ae_sns_dft->arr_ratio[0] = 0x400;
        ae_sns_dft->arr_ratio[1] = 0x40;
        ae_sns_dft->arr_ratio[2] = 0x40; /* array index 2 */
    }
    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[vi_pipe];
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_eis_vi_ae_route),
                      &g_init_ae_route[vi_pipe],  sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_eis_vi_ae_route_ex),
                      &g_init_ae_route_ex[vi_pipe],  sizeof(ot_eis_vi_ae_route_ex));
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_sf_attr, sizeof(ot_eis_vi_ae_route),
                      &g_init_ae_route_sf[vi_pipe], sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_sf_attr_ex, sizeof(ot_eis_vi_ae_route_ex),
                      &g_init_ae_route_sf_ex[vi_pipe],  sizeof(ot_eis_vi_ae_route_ex));
    return;
}

static ot_s32 cmos_get_ae_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(ae_sns_dft);
    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (ot_void)memset_s(&ae_sns_dft->ae_route_attr, sizeof(ot_eis_vi_ae_route), 0, sizeof(ot_eis_vi_ae_route));

    cmos_get_ae_comm_default(vi_pipe, ae_sns_dft, sns_state);

    switch (sns_state->wdr_mode) {
        case OT_EIS_WDR_MODE_NONE:   /* linear mode */
            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
            break;

        case OT_EIS_WDR_MODE_2To1_LINE:
            cmos_get_ae_2to1_line_wdr_default(vi_pipe, ae_sns_dft, sns_state);
            break;

        default:
            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
            break;
    }

    return OT_SUCCESS;
}

static ot_void cmos_config_vmax(ot_eis_sns_state *sns_state, ot_u32 vmax)    /*set VMAX*/
{
    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8_in_24bits(vmax);
        sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8_in_24bits(vmax);
    } else {
        sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].data = low_8_in_24bits(vmax);
        sns_state->regs_info[0].i2c_data[WDR_VMAX_M_IDX].data = mid_8_in_24bits(vmax);
        sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].data = high_8_in_24bits(vmax);
    }

    return;
}

static ot_void cmos_fps_set(ot_vi_pipe_id vi_pipe, ot_float fps, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    ot_u32 lines, lines_max, vmax;
    ot_float max_fps, min_fps;
    ot_bool achieve_fps_ok;
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_void_return(ae_sns_dft);
    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines = g_imx290_mode_tbl[sns_state->img_mode].ver_lines;     /*get limit setting*/
    lines_max = g_imx290_mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = g_imx290_mode_tbl[sns_state->img_mode].max_fps;
    min_fps = g_imx290_mode_tbl[sns_state->img_mode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        sns_err_trace("Not support Fps: %f\n", fps);
        return;
    }

    achieve_fps_ok = OT_TRUE;
    vmax = (ot_u32)(lines * max_fps / sns_div_0_to_1_float(fps));   /*120 or 60, 60fps->vmax 1125*/

    vmax = (vmax > lines_max) ? lines_max : vmax;

    cmos_config_vmax(sns_state, vmax);

    if(sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE)
    {
        sns_state->fl_std = vmax * 2;  //X2   vmax->60fps(for sensor) , full_line = 2* VMAX(for AE)
        ae_sns_dft->lines_per500ms = lines * max_fps; /* div 2 & plus 2*/
        update_imx290_state_2to1_wdr(vmax);
    }
    else
    {
        sns_state->fl_std = vmax;  //linear
        ae_sns_dft->lines_per500ms = lines * max_fps / 2; /* div 2 */
    }
    g_lines_per500ms[vi_pipe] = ae_sns_dft->lines_per500ms;
    ae_sns_dft->fps = fps;  //when ae_sns_dft->fps != fps,set fps again
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->max_int_time = sns_state->fl_std - 2; /* sub 10 */   /*when update vmax,change max_int_time*/
    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times =
        (ot_u32)((1000000000) / (sns_state->fl_std * sns_div_0_to_1_float(fps))); /* 1000000000ns */ /*change hmax_time*/

    return;
}

static ot_void cmos_slow_framerate_set(ot_vi_pipe_id vi_pipe, ot_u32 full_lines, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    printf("not support cmos_slow_framerate_set\n");
    return;
}

static ot_void cmos_inttime_update_linear(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;
    ot_u32 value;

    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    value = int_time;

    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = low_8bits(value);
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = high_8bits(value);

    return;
}

static ot_void cmos_inttime_update_2to1_line(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;
    static ot_bool is_first[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = 1};

    static ot_u32 short_int_time[OT_EIS_VI_MAX_PIPE_NUM] = {0};
    static ot_u32 long_int_time[OT_EIS_VI_MAX_PIPE_NUM] = {0};

    static ot_u32 RHS1_time[OT_EIS_VI_MAX_PIPE_NUM] = {0};
    static ot_u32 SHS1_time[OT_EIS_VI_MAX_PIPE_NUM] = {0};
    static ot_u32 SHS2_time[OT_EIS_VI_MAX_PIPE_NUM] = {0};
    static ot_u32 short_int_time_max[OT_EIS_VI_MAX_PIPE_NUM] = {0};

    ot_u32 SNS_YOUTSIE;

    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (is_first[vi_pipe]) { /* short exposure */
        short_int_time_max[vi_pipe] = g_imx290_state.RHS1_max - 3;
        int_time = MAX2(int_time,1);   //MIN 1?
        int_time = MIN2(int_time,short_int_time_max[vi_pipe]);
        sns_state->wdr_int_time[0] = int_time;
        short_int_time[vi_pipe] = int_time;
        is_first[vi_pipe] = OT_FALSE;
    } else { /* long exposure */
        sns_state->wdr_int_time[1] = int_time;
        long_int_time[vi_pipe] = int_time;
        SHS2_time[vi_pipe] = sns_state->fl_std - 1 -long_int_time[vi_pipe];

        /*SHS1: 2/3*/
        SHS1_time[vi_pipe] = (short_int_time[vi_pipe] % 2)+2;
        RHS1_time[vi_pipe] = short_int_time[vi_pipe] + SHS1_time[vi_pipe] + 1;

        SNS_YOUTSIE = (1109 + (RHS1_time[vi_pipe] - 1) / 2 ) * 2; /* (1109 + (RHS1 - 1) / 2 ) * 2 */
        SNS_YOUTSIE = (SNS_YOUTSIE >= 0x1FFF) ? 0x1FFF : SNS_YOUTSIE;

        sns_state->regs_info[0].i2c_data[RHS1_L_IDX].data = low_8_in_24bits(RHS1_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[RHS1_M_IDX].data = mid_8_in_24bits(RHS1_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[RHS1_H_IDX].data = high_8_in_24bits(RHS1_time[vi_pipe]);

        sns_state->regs_info[0].i2c_data[SHS1_L_IDX].data = low_8_in_24bits(SHS1_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[SHS1_M_IDX].data = mid_8_in_24bits(SHS1_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[SHS1_H_IDX].data = high_8_in_24bits(SHS1_time[vi_pipe]);

        sns_state->regs_info[0].i2c_data[SHS2_L_IDX].data = low_8_in_24bits(SHS2_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[SHS2_M_IDX].data = mid_8_in_24bits(SHS2_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[SHS2_H_IDX].data = high_8_in_24bits(SHS2_time[vi_pipe]);

        sns_state->regs_info[0].i2c_data[Y_OUT_SIZE_L_IDX].data = low_8bits(SNS_YOUTSIE);
        sns_state->regs_info[0].i2c_data[Y_OUT_SIZE_H_IDX].data = high_8bits(SNS_YOUTSIE);

        is_first[vi_pipe] = OT_TRUE;
    }

    return;
}

/* while vi notify ae to update sensor regs, ae call these funcs. */
static ot_void cmos_inttime_update(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        cmos_inttime_update_2to1_line(vi_pipe, int_time);
    } else {
        cmos_inttime_update_linear(vi_pipe, int_time);
    }

    return;
}

#define GAIN_NODE_NUM 241
static ot_u32 g_gain_table[GAIN_NODE_NUM] = {
    1024,     1060,     1097,     1136,     1176,     1217,     1260,    1304,    1350,    1397,    1446,    1497,    1550,    1604,    1661,    1719,    1780,    1842,    1907,    1974,
    2043,     2115,     2189,     2266,     2346,     2428,     2514,    2602,    2693,    2788,    2886,    2987,    3092,    3201,    3314,    3430,    3551,    3675,    3805,    3938,
    4077,     4220,     4368,     4522,     4681,     4845,     5015,    5192,    5374,    5563,    5758,    5961,    6170,    6387,    6611,    6844,    7084,    7333,    7591,    7858,
    8134,     8420,     8716,     9022,     9339,     9667,     10007,   10359,   10723,   11099,   11489,   11893,   12311,   12744,   13192,   13655,   14135,   14632,   15146,   15678,
    16229,    16800,    17390,    18001,    18634,    19289,    19966,   20668,   21394,   22146,   22925,   23730,   24564,   25427,   26321,   27246,   28203,   29194,   30220,   31282,
    32382,    33520,    34698,    35917,    37179,    38486,    39838,   41238,   42687,   44188,   45740,   47348,   49012,   50734,   52517,   54363,   56273,   58251,   60298,   62417,
    64610,    66881,    69231,    71664,    74182,    76789,    79488,   82281,   85173,   88166,   91264,   94471,   97791,   101228,  104785,  108468,  112279,  116225,  120310,  124537,
    128914,   133444,   138134,   142988,   148013,   153215,   158599,  164172,  169942,  175914,  182096,  188495,  195119,  201976,  209074,  216421,  224027,  231900,  240049,  248485,
    257217,   266256,   275613,   285299,   295325,   305703,   316446,  327567,  339078,  350994,  363329,  376097,  389314,  402995,  417157,  431817,  446992,  462700,  478961,  495793,
    513216,   531251,   549921,   569246,   589250,   609958,   631393,  653582,  676550,  700326,  724936,  750412,  776783,  804081,  832338,  861589,  891867,  923209,  955652,  989236,
    1024000,  1059986,  1097236,  1135795,  1175709,  1217026,  1259795, 1304067, 1349895, 1397333, 1446438, 1497269, 1549887, 1604353, 1660734, 1719095, 1779508, 1842044, 1906777, 1973786,
    2043149,  2114949,  2189273,  2266209,  2345848,  2428287,  2513622, 2601956, 2693394, 2788046, 2886024, 2987445, 3092431, 3201105, 3313599, 3430046, 3550585, 3675361, 3804521, 3938220, 40766172
};

static ot_void cmos_again_calc_table(ot_vi_pipe_id vi_pipe, ot_u32 *again_lin, ot_u32 *again_db)
{
    int i;

    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);

    if (*again_lin >= g_gain_table[GAIN_NODE_NUM - 1]) {
        *again_lin = g_gain_table[GAIN_NODE_NUM - 1];
        *again_db = (GAIN_NODE_NUM - 1);  //1 means 0.3db,mean linear table, equal to sensor meaning;
        return;
    }

    for (i = 1; i < GAIN_NODE_NUM; i++) {
        if (*again_lin < g_gain_table[i]) {
            *again_lin = g_gain_table[i - 1];
            *again_db = (i - 1);
            break;
        }
    }
    return;
}

static ot_void cmos_gains_regs_set(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state, ot_u32 again_reg, ot_u32 dgain_reg)
{
    static ot_bool first_gain[OT_EIS_VI_MAX_PIPE_NUM] = { [0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = 1 };

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].data = (again_reg & 0xff);
        sns_state->regs_info[0].i2c_data[AGAIN_H_IDX].data = ((again_reg & 0x3f00) >> 8); /* shift 8 */
        sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].data = (dgain_reg & 0xff);
        sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].data = ((dgain_reg & 0x3f00) >> 8); /* shift 8 */
    } else if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        if (first_gain[vi_pipe] == OT_TRUE) { /* long frame */
            sns_state->regs_info[0].i2c_data[LEF_GAIN_IDX].data = again_reg;

            first_gain[vi_pipe] = OT_FALSE;
        } else {                              /* short frame */
            sns_state->regs_info[0].i2c_data[SEF1_GAIN_IDX].data = again_reg;

            first_gain[vi_pipe] = OT_TRUE;
        }
    }

    return;
}

static ot_void cmos_gains_update(ot_vi_pipe_id vi_pipe, ot_u32 again, ot_u32 dgain)
{
    ot_eis_sns_state *sns_state = OT_NULL;
    ot_u32 again_reg, dgain_reg;

    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    cmos_gains_regs_set(vi_pipe, sns_state, again, dgain);

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
static ot_void cmos_set_2to1_long_frame_range(ot_eis_vi_ae_int_time_range *int_time,
                                              ot_s32 time_min, ot_s32 time_max, ot_s32 step)
{
    int_time->int_time_max[0] = MAX2((ot_s32)int_time->pre_int_time[0] - step, time_min);    /* index 0 */
    int_time->int_time_min[0] = MAX2((ot_s32)int_time->pre_int_time[0] - step, time_min);    /* index 0 */
    int_time->int_time_max[1] = time_max;                                                       /* index 2 */
    int_time->int_time_min[1] = time_min;                                                       /* index 2 */
}


static ot_void cmos_get_inttime_max_2to1_line(ot_vi_pipe_id vi_pipe, ot_u32 *ratio,
                                              ot_eis_vi_ae_int_time_range *int_time, ot_u32 *lf_max_int_time)
{
    ot_u32 short_max0, i, short_max, short_time_min_limit;
    ot_eis_sns_state *sns_state = OT_NULL;
    time_step step = {{0}};
    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);
    static ot_u32 pre_ratio[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = 0X40};
    short_time_min_limit = 2; /* short_time_min_limit 2 */
    if (g_fswdr_mode[vi_pipe] == OT_EIS_VI_FSWDR_LONG_FRAME_MODE) {
        short_max0 = sns_state->fl[1] - IMX290_MARGIN - step.dec[0] - sns_state->wdr_int_time[0];
        short_max = sns_state->fl[0] - IMX290_MARGIN - step.dec[0];
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        int_time->int_time_max[0] = short_time_min_limit;
        int_time->int_time_min[0] = short_time_min_limit;
        int_time->int_time_max[1] = short_max;
        int_time->int_time_min[1] = short_time_min_limit;
        cmos_set_2to1_long_frame_range(int_time, short_time_min_limit, short_max, 90); /* step 90 */
    } else {
        short_max0 = g_imx290_state.RHS1_max - SHS1_MIN - 1;    //S_max = RHS1_max - 3
        short_max = ((sns_state->fl[0] - IMX290_MARGIN) * 0x40) / (ratio[0] + 0x40);    //L+S less than one frame
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        short_max = (short_max == 0) ? 1 : short_max;

        // *lf_max_int_time = sns_state->fl[0] - IMX290_MARGIN - step.inc[0];
        *lf_max_int_time = sns_state->fl[0] - IMX290_MARGIN - RHS1_MIN;

        if (short_max >= short_time_min_limit) {
            int_time->int_time_max[0] = short_max;
            int_time->int_time_max[1] = (int_time->int_time_max[0] * ratio[0]) >> 6; /* shift 6 */
            int_time->int_time_min[0] = short_time_min_limit;
            int_time->int_time_min[1] = (int_time->int_time_min[0] * ratio[0]) >> 6; /* shift 6 */
        } else {
            short_max = short_time_min_limit;
            int_time->int_time_max[0] = short_max;
            int_time->int_time_max[1] = (int_time->int_time_max[0] * 0xFFF) >> 6; /* shift 6 */
            int_time->int_time_min[0] = int_time->int_time_max[0];
            int_time->int_time_min[1] = int_time->int_time_max[1];
        }

        if (ratio[0] != pre_ratio[vi_pipe]) {
            cmos_clip_ratio_range(ratio, (const ot_eis_vi_ae_int_time_range *)int_time, 2); /* frame number is 2 */
        }
    }
    pre_ratio[vi_pipe] = ratio[0];
    return;
}

static ot_void cmos_get_inttime_max(ot_vi_pipe_id vi_pipe, ot_u16 man_ratio_enable, ot_u32 *ratio,
    ot_eis_vi_ae_int_time_range *int_time, ot_u32 *lf_max_int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_void_return(ratio);
    sns_check_pointer_void_return(int_time);
    sns_check_pointer_void_return(lf_max_int_time);
    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    switch (sns_state->wdr_mode) {
        case OT_EIS_WDR_MODE_2To1_LINE:
            cmos_get_inttime_max_2to1_line(vi_pipe, ratio, int_time, lf_max_int_time);
            break;
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
/* awb static param for Fuji Lens New IR_Cut */
#define CALIBRATE_STATIC_TEMP                         5000
#define CALIBRATE_STATIC_WB_R_GAIN                    427
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    480

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              69
#define CALIBRATE_AWB_P2                              96
#define CALIBRATE_AWB_Q1                              (-91 )
#define CALIBRATE_AWB_A1                              306
#define CALIBRATE_AWB_B1                              (-26926)
#define CALIBRATE_AWB_C1                              (7220654)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static ot_s32 cmos_get_awb_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_sensor_default *awb_sns_dft)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(awb_sns_dft);
    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (ot_void)memset_s(awb_sns_dft, sizeof(ot_eis_vi_awb_sensor_default), 0, sizeof(ot_eis_vi_awb_sensor_default));
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

        case OT_EIS_WDR_MODE_2To1_FRAME:
        case OT_EIS_WDR_MODE_2To1_LINE:
            (ot_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_eis_vi_awb_ccm), &g_awb_ccm_wdr, sizeof(ot_eis_vi_awb_ccm));
            (ot_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_eis_vi_awb_agc_table),
                              &g_awb_agc_table_wdr, sizeof(ot_eis_vi_awb_agc_table));

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

    return OT_SUCCESS;
}

static void cmos_get_isp_linear_default(ot_eis_vi_sns_alg_default *isp_def)
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

static void cmos_get_isp_wdr_default(ot_eis_vi_sns_alg_default *isp_def)
{
    isp_def->key.bit1_dpc            = 1;
    isp_def->dpc                     = &g_cmos_dpc_wdr;
    isp_def->key.bit1_demosaic       = 1;
    isp_def->demosaic                = &g_cmos_demosaic_wdr;
    isp_def->key.bit1_sharpen        = 1;
    isp_def->sharpen                 = &g_cmos_yuv_sharpen_wdr;
    isp_def->key.bit1_drc            = 1;
    isp_def->drc                     = &g_cmos_drc_wdr;
    isp_def->key.bit1_gamma          = 1;
    isp_def->gamma                   = &g_cmos_gamma_wdr;
#ifdef CONFIG_OT_ISP_PREGAMMA_SUPPORT
    isp_def->key.bit1_pregamma       = 1;
    isp_def->pregamma                = &g_cmos_pregamma;
#endif
    isp_def->key.bit1_bayer_nr       = 1;
    isp_def->bayer_nr                = &g_cmos_bayer_nr_wdr;
#ifdef CONFIG_OT_ISP_CR_SUPPORT
    isp_def->key.bit1_ge             = 1;
    isp_def->ge                      = &g_cmos_ge_wdr;
#endif
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color = &g_cmos_anti_false_color_wdr;
    isp_def->key.bit1_cac = 1;
    isp_def->cac = &g_cmos_cac_wdr;
    isp_def->key.bit1_bshp= 1;
    isp_def->bshp = &g_cmos_bayershp_wdr;
    isp_def->key.bit1_ldci = 1;
    isp_def->ldci = &g_cmos_ldci_wdr;
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze_wdr;
    (ot_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_eis_vi_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_eis_vi_noise_calibration));
    return;
}

static ot_s32 cmos_get_vi_alg_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_alg_default *isp_def)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(isp_def);
    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (ot_void)memset_s(isp_def, sizeof(ot_eis_vi_sns_alg_default), 0, sizeof(ot_eis_vi_sns_alg_default));
#ifdef CONFIG_OT_ISP_CA_SUPPORT
    isp_def->key.bit1_ca      = 0;
#endif
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

    switch (sns_state->wdr_mode) {
        case OT_EIS_WDR_MODE_NONE:
            cmos_get_isp_linear_default(isp_def);
            break;
        case OT_EIS_WDR_MODE_2To1_LINE:
            cmos_get_isp_wdr_default(isp_def);
            break;
        default:
            cmos_get_isp_linear_default(isp_def);
            break;
    }

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x400;

    isp_def->sns_mode.sns_id = IMX290_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;

    return OT_SUCCESS;
}

static ot_s32 cmos_get_vi_black_level(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_black_level *black_level)
{
    ot_s32  i;
    ot_eis_sns_state *sns_state = OT_NULL;
    const ot_eis_vi_sns_black_level *cmos_blc_def = OT_NULL;

    sns_check_pointer_return(black_level);
    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {
        cmos_blc_def = &g_cmos_blc;
    } else {
        cmos_blc_def = &g_cmos_blc_wdr;
    }

    (ot_void)memcpy_s(black_level, sizeof(ot_eis_vi_sns_black_level), cmos_blc_def, sizeof(ot_eis_vi_sns_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = OT_FALSE;

    /* black level of linear mode */
    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {
        for (i = 0; i < OT_EIS_VI_BAYER_CHN_NUM; i++) {
            black_level->auto_attr.black_level[0][i] = 0x400;
        }
    } else { /* black level of DOL mode */
        for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
            black_level->auto_attr.black_level[i][0] = BLACK_LEVEL_2TO1_WDR_R;
            black_level->auto_attr.black_level[i][1] = BLACK_LEVEL_2TO1_WDR_GR;
            black_level->auto_attr.black_level[i][2] = BLACK_LEVEL_2TO1_WDR_GB; /* index 2 */
            black_level->auto_attr.black_level[i][3] = BLACK_LEVEL_2TO1_WDR_B; /* index 3 */
        }
    }

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

    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->img_mode == IMX290_2M_60FPS_12BIT_LINEAR_MODE) {
            full_lines_5fps = (IMX290_VMAX_2M_60FPS_12BIT_LINEAR * 30) / 5; /* , 5fps */
        } else {
            return;
        }
    }

    if (enable) { /* setup for ISP pixel calibration mode */
        imx290_write_register(vi_pipe, IMX290_VMAX_L_ADDR, low_8bits(full_lines_5fps));
        imx290_write_register(vi_pipe, IMX290_VMAX_H_ADDR, high_8bits(full_lines_5fps));
    } else { /* setup for ISP 'normal mode' */
        sns_state->fl_std = (sns_state->fl_std > IMX290_FULL_LINES_MAX) ? IMX290_FULL_LINES_MAX : sns_state->fl_std;
        imx290_write_register(vi_pipe, IMX290_VMAX_L_ADDR, low_8bits(sns_state->fl_std));
        imx290_write_register(vi_pipe, IMX290_VMAX_H_ADDR, high_8bits(sns_state->fl_std));
        sns_state->sync_init = OT_FALSE;
    }

    return;
}

static ot_s32 cmos_set_wdr_mode(ot_vi_pipe_id vi_pipe, ot_u8 mode)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init = OT_FALSE;

    switch (mode & 0x3F) {
        case OT_EIS_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_EIS_WDR_MODE_NONE;
            printf("linear mode\n");
            break;

        case OT_EIS_WDR_MODE_2To1_LINE:
            sns_state->wdr_mode = OT_EIS_WDR_MODE_2To1_LINE;
            printf("2TO1 line WDR mode\n");
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
    sns_state->regs_info[0].com_bus.i2c_dev = g_imx290_bus_info[vi_pipe].com_bus.i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = WDR_REG_MAX_IDX; // TODO: REG_MAX_IDX;

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        sns_state->regs_info[0].reg_num = WDR_REG_MAX_IDX;
        sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    }

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {      /*define dev addr,dev byte num*/
        sns_state->regs_info[0].i2c_data[i].update = OT_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = g_imx290_bus_info[vi_pipe].bus_addr;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = IMX290_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = IMX290_DATA_BYTE;
    }
    return;
}

static ot_void cmos_2to1_line_wdr_sns_reg_info_init(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
    sns_state->regs_info[0].i2c_data[SHS1_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SHS1_L_IDX].reg_addr = IMX290_SHS1_L_ADDR;
    sns_state->regs_info[0].i2c_data[SHS1_M_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SHS1_M_IDX].reg_addr = IMX290_SHS1_M_ADDR;
    sns_state->regs_info[0].i2c_data[SHS1_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SHS1_H_IDX].reg_addr = IMX290_SHS1_H_ADDR;

    sns_state->regs_info[0].i2c_data[SHS2_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SHS2_L_IDX].reg_addr = IMX290_SHS2_L_ADDR;
    sns_state->regs_info[0].i2c_data[SHS2_M_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SHS2_M_IDX].reg_addr = IMX290_SHS2_M_ADDR;
    sns_state->regs_info[0].i2c_data[SHS2_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SHS2_H_IDX].reg_addr = IMX290_SHS2_H_ADDR;

    sns_state->regs_info[0].i2c_data[RHS1_L_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[RHS1_L_IDX].reg_addr = IMX290_RHS1_L_ADDR;
    sns_state->regs_info[0].i2c_data[RHS1_M_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[RHS1_M_IDX].reg_addr = IMX290_RHS1_M_ADDR;
    sns_state->regs_info[0].i2c_data[RHS1_H_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[RHS1_H_IDX].reg_addr = IMX290_RHS1_H_ADDR;

    sns_state->regs_info[0].i2c_data[LEF_GAIN_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[LEF_GAIN_IDX].reg_addr = IMX290_SEF1_GAIN_ADDR;

    sns_state->regs_info[0].i2c_data[SEF1_GAIN_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SEF1_GAIN_IDX].reg_addr = IMX290_LEF_GAIN_ADDR;

    sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[WDR_VMAX_L_IDX].reg_addr = IMX290_VMAX_L_ADDR;
    sns_state->regs_info[0].i2c_data[WDR_VMAX_M_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[WDR_VMAX_M_IDX].reg_addr = IMX290_VMAX_M_ADDR;
    sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[WDR_VMAX_H_IDX].reg_addr = IMX290_VMAX_H_ADDR;

    sns_state->regs_info[0].i2c_data[Y_OUT_SIZE_L_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[Y_OUT_SIZE_L_IDX].reg_addr = IMX290_Y_OUT_SIZE_L_ADDR;
    sns_state->regs_info[0].i2c_data[Y_OUT_SIZE_H_IDX].delay_frame_num = 1;
    sns_state->regs_info[0].i2c_data[Y_OUT_SIZE_H_IDX].reg_addr = IMX290_Y_OUT_SIZE_H_ADDR;
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

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        if (sns_state->fl[0] > sns_state->fl[1]) {
        } else if (sns_state->fl[0] < sns_state->fl[1]) {
        } else {
        }
    } else {
        if (sns_state->fl[0] > sns_state->fl[1]) {
        } else if (sns_state->fl[0] < sns_state->fl[1]) {
        } else {
        }
    }

    return;
}

static ot_s32 cmos_get_sns_regs_info(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_regs_info *sns_regs_info)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(sns_regs_info);
    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if ((sns_state->sync_init == OT_FALSE) || (sns_regs_info->config == OT_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, sns_state);

        if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
            /* DOL 2t1 Mode Regs */
            cmos_2to1_line_wdr_sns_reg_info_init(vi_pipe, sns_state);
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
        case IMX290_2M_60FPS_12BIT_LINEAR_MODE:
            sns_state->fl_std = IMX290_VMAX_2M_60FPS_12BIT_LINEAR;
            break;
        case IMX290_2M_60FPS_10BIT_2TO1_DOL_MODE:
            sns_state->fl_std = IMX290_VMAX_2M_60FPS_10BIT_2TO1_WDR;
            break;
        case IMX290_2M_30FPS_12BIT_2TO1_DOL_MODE:
            sns_state->fl_std = IMX290_VMAX_2M_30FPS_12BIT_2TO1_WDR;
            break;
        default:
            sns_state->fl_std = IMX290_VMAX_2M_60FPS_12BIT_LINEAR;
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
    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < IMX290_MODE_BUTT; i++) {
        if (sns_image_mode->fps <= g_imx290_mode_tbl[i].max_fps &&
            sns_image_mode->width <= g_imx290_mode_tbl[i].width &&
            sns_image_mode->height <= g_imx290_mode_tbl[i].height &&
            sns_state->wdr_mode == g_imx290_mode_tbl[i].wdr_mode) {
            image_mode = (imx290_res_mode)i;
            break;
        }
    }

    if (i >= IMX290_MODE_BUTT) {
        imx290_err_mode_print(sns_image_mode, sns_state);
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

    imx290_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = OT_FALSE;
    sns_state->sync_init = OT_FALSE;
    sns_state->img_mode  = IMX290_2M_60FPS_12BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_EIS_WDR_MODE_NONE;
    sns_state->fl_std    = IMX290_VMAX_2M_60FPS_12BIT_LINEAR;
    sns_state->fl[0]     = IMX290_VMAX_2M_60FPS_12BIT_LINEAR;
    sns_state->fl[1]     = IMX290_VMAX_2M_60FPS_12BIT_LINEAR;

    (ot_void)memset_s(&sns_state->regs_info[0], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));
    (ot_void)memset_s(&sns_state->regs_info[1], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));

    return;
}

static ot_s32 sensor_ctx_init(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    imx290_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == OT_NULL) {
        sns_state_ctx = (ot_eis_sns_state *)malloc(sizeof(ot_eis_sns_state));
        if (sns_state_ctx == OT_NULL) {
            sns_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_FAILURE;
        }
    }

    (ot_void)memset_s(sns_state_ctx, sizeof(ot_eis_sns_state), 0, sizeof(ot_eis_sns_state));

    imx290_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return OT_SUCCESS;
}

static ot_void sensor_ctx_exit(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    imx290_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    imx290_sensor_reset_ctx(vi_pipe);

    return;
}

static ot_eis_vi_sns_register_func g_imx290_register_to_vi_func = {
    .pfn_sns_init                = imx290_init,
    .pfn_sns_exit                = imx290_exit,
    .pfn_sns_global_init         = sensor_global_init,
    .pfn_sns_set_image_mode      = cmos_set_image_mode,
    .pfn_sns_set_wdr_mode        = cmos_set_wdr_mode,
    .pfn_sns_get_vi_alg_default  = cmos_get_vi_alg_default,
    .pfn_sns_get_vi_black_level  = cmos_get_vi_black_level,
    .pfn_sns_get_blc_clamp_info  = cmos_get_blc_clamp_info,
    .pfn_sns_set_pixel_detect    = cmos_set_pixel_detect,
    .pfn_sns_get_reg_info        = cmos_get_sns_regs_info,
};

static ot_eis_vi_ae_sensor_register_func g_imx290_register_to_ae_func = {
    .pfn_sns_get_ae_default         = cmos_get_ae_default,
    .pfn_sns_cfg_fps                = cmos_fps_set,
    .pfn_sns_cfg_slow_framerate     = cmos_slow_framerate_set,
    .pfn_sns_update_inttime         = cmos_inttime_update,
    .pfn_sns_update_gains           = cmos_gains_update,
    .pfn_sns_calc_table_again       = cmos_again_calc_table,
    .pfn_sns_calc_table_dgain       = OT_NULL,
    .pfn_sns_get_inttime_max        = cmos_get_inttime_max,
    .pfn_sns_set_fswdr_attr         = cmos_ae_fswdr_attr_set,
    .pfn_sns_set_quick_start_status = OT_NULL,
    .pfn_sns_convert_exp_param      = OT_NULL,
    .pfn_sns_get_thermo_default     = OT_NULL,
} ;

static ot_eis_vi_awb_sensor_register_func g_imx290_register_to_awb_func = {
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

    ret = ot_eis_vi_sensor_register_to_vi(pipe_hdl, &g_imx290_register_to_vi_func, IMX290_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to vi failed!\n");
        goto free_sns_ctx;
    }

    ret = ot_eis_vi_sensor_register_to_ae(pipe_hdl, &g_imx290_register_to_ae_func, IMX290_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to ae lib failed!\n");
        goto sns_unreg_to_vi;
    }

    ret = ot_eis_vi_sensor_register_to_awb(pipe_hdl, &g_imx290_register_to_awb_func, IMX290_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to awb lib failed!\n");
        goto sns_unreg_to_ae;
    }

    return OT_SUCCESS;

sns_unreg_to_ae:
    ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, IMX290_ID);
sns_unreg_to_vi:
    ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, IMX290_ID);
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

    ret = ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, IMX290_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }
    ret = ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, IMX290_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_eis_vi_sensor_unregister_to_awb(pipe_hdl, IMX290_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to awb lib failed!\n");
        return ret;
    }

    sensor_ctx_exit(master_pipe_id);

    return OT_SUCCESS;
}

static ot_s32 imx290_set_bus_info(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_bus_info *sns_bus_info)
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

    memcpy_s(&g_imx290_bus_info[master_pipe_id], sizeof(ot_eis_sns_bus_info), sns_bus_info, sizeof(ot_eis_sns_bus_info));

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

ot_eis_sns_obj g_sns_imx290_obj = {
    .pfn_sns_register              = sensor_register_callback,
    .pfn_sns_unregister            = sensor_unregister_callback,
    .pfn_sns_standby               = imx290_standby,
    .pfn_sns_restart               = imx290_restart,
    .pfn_sns_set_mirror_flip       = OT_NULL,
    .pfn_sns_set_blc_clamp         = imx290_blc_clamp,
    .pfn_sns_set_bus_info          = imx290_set_bus_info,
    .pfn_sns_set_init              = sensor_set_init
};
