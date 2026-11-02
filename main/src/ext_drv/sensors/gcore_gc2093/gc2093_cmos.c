/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include "ot_eis_errno.h"
#include "ot_eis_vi_sns.h"
#include "gc2093_cmos_ex.h"
#include "gc2093_cmos.h"
#include "ot_eis_vi_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_awb_api.h"

#define GC2093_ID                    2093
#define SENSOR_GC2093_WIDTH          1920
#define SENSOR_GC2093_HEIGHT         1080

#define higher_4bits(x) (((x) & 0xf0000) >> 16)
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)

#define MAX2(x, y)       ((x) > (y) ? (x) : (y))
#define MIN2(x, y)       ((x) < (y) ? (x) : (y))

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define gc2093_sensor_set_ctx(pipe, ctx)   ((g_gc2093_sns_state[pipe]) = (ctx))
#define gc2093_sensor_reset_ctx(pipe)      (g_gc2093_sns_state[pipe] = OT_NULL)

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

static ot_u32 gc2093_again[OT_EIS_VI_MAX_PIPE_NUM]={[0 ...(OT_EIS_VI_MAX_PIPE_NUM -1)]=1024};

ot_eis_sns_bus_info g_gc2093_bus_info[OT_EIS_VI_MAX_PIPE_NUM] = {
        [0] = {
            .com_bus.i2c_dev = 0,
            .bus_addr = GC2093_I2C_ADDR
        },
        [1 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = {
            .com_bus.i2c_dev = -1,
            .bus_addr = GC2093_I2C_ADDR
        }
};

ot_eis_sns_state *g_gc2093_sns_state[OT_EIS_VI_MAX_PIPE_NUM] = {OT_NULL};

static ot_bool blc_clamp_info[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = OT_TRUE};

ot_eis_sns_bus_info *gc2093_get_bus_info(ot_vi_pipe_id vi_pipe)
{
    return &g_gc2093_bus_info[vi_pipe];
}

ot_eis_sns_state *gc2093_get_ctx(ot_vi_pipe_id vi_pipe)
{
    return g_gc2093_sns_state[vi_pipe];
}

ot_void gc2093_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en)
{
    blc_clamp_info[vi_pipe] = clamp_en;
}

const gc2093_video_mode_tbl g_gc2093_mode_tbl[GC2093_MODE_BUTT] = {
    {GC2093_VMAX_2M_30FPS_10BIT_LINEAR,   GC2093_FULL_LINES_MAX,          30, 2.07,
     1920, 1080, 0, OT_EIS_WDR_MODE_NONE,       "GC2093_2M_30FPS_10BIT_LINEAR_MODE"},
    {GC2093_VMAX_2M_30FPS_10BIT_2TO1_WDR,  GC2093_FULL_LINES_MAX_2TO1_WDR, 30, 2.29,
     1920, 1080, 0, OT_EIS_WDR_MODE_2To1_LINE,  "GC2093_2M_30FPS_10BIT_2TO1_VC_MODE"},
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* Gc2093 Register Address */
#define GC2093_EXPO_H_ADDR          0x0003
#define GC2093_EXPO_L_ADDR          0x0004
#define GC2093_SHORT_EXPO_H_ADDR    0x0001
#define GC2093_SHORT_EXPO_L_ADDR    0x0002

#define GC2093_AGAIN_H_ADDR         0x00b4
#define GC2093_AGAIN_L_ADDR         0x00b3
#define GC2093_COL_AGAIN_H_ADDR		0x00b8
#define GC2093_COL_AGAIN_L_ADDR		0x00b9
#define GC2093_AGAIN_MAG1			0x0155
#define GC2093_AGAIN_HOLD			0x031d
#define GC2093_AGAIN_MAG2			0x00c2
#define GC2093_AGAIN_MAG3			0x00cf
#define GC2093_AGAIN_MAG4			0x00d9

#define GC2093_DePink_A			    0x0183
#define GC2093_DePink_B			    0x0187
#define GC2093_DePink_C			    0x0032

#define GC2093_DGAIN_H_ADDR         0x00b1
#define GC2093_DGAIN_L_ADDR         0x00b2

#define GC2093_VMAX_H_ADDR          0x0041
#define GC2093_VMAX_L_ADDR          0x0042
#define GC2093_MARGIN               16

#define GC2093_AGAIN_30X            30960
#define GC2093_AGAIN_36X            36672
#define GC2093_AGAIN_50X            51824

#define gc2093_err_mode_print(sns_image_mode, sns_state) \
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
    ae_sns_dft->full_lines_max = GC2093_FULL_LINES_MAX;
    ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 30); /* 1000000000ns,  */

    ae_sns_dft->int_time_accu.accu_type = OT_EIS_VI_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1;
    ae_sns_dft->int_time_accu.offset = 0;

    ae_sns_dft->again_accu.accu_type = OT_EIS_VI_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = OT_EIS_VI_AE_ACCURACY_LINEAR;
    ae_sns_dft->dgain_accu.accuracy = 0.015625; /* accuracy: 0.015625 */

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 2 << ae_sns_dft->isp_dgain_shift; /* max 128 */
    if (g_lines_per500ms[vi_pipe] == 0) {
        ae_sns_dft->lines_per500ms = sns_state->fl_std * 30 / 2; /* , div 2 */
    } else {
        ae_sns_dft->lines_per500ms = g_lines_per500ms[vi_pipe];
    }
    ae_sns_dft->max_iris_fno = OT_EIS_VI_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_fno = OT_EIS_VI_IRIS_F_NO_5_6;

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
    ae_sns_dft->max_again = 74976; /* max 74976 */
    ae_sns_dft->min_again = 1024;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 64; /* max 1024 */
    ae_sns_dft->min_dgain = 64;  /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->init_ae_speed=64;
    ae_sns_dft->init_ae_tolerance=5;
    ae_sns_dft->ae_exp_mode = OT_EIS_VI_AE_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 148859; /* init 76151 */
    ae_sns_dft->init_int_time = g_init_int_time[vi_pipe];
    ae_sns_dft->init_again = g_init_again[vi_pipe];
    ae_sns_dft->init_dgain = g_init_dgain[vi_pipe];
    ae_sns_dft->init_isp_dgain = g_init_isp_dgain[vi_pipe];

    ae_sns_dft->max_int_time = sns_state->fl_std - 2; /* sub 2 */
    ae_sns_dft->min_int_time = 1; /* min int 1 */
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
    ae_sns_dft->max_int_time = sns_state->fl_std - 2; /* sub 2 */
    ae_sns_dft->min_int_time = 2; /* min_int_time 2 */
    // ae_sns_dft->int_time_accu.offset = -0.115; /* -0.115 line for stagger */

    ae_sns_dft->max_int_time_target = 65535; /* max 65535 */
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->max_again = 65536; /* max 113168 */
    ae_sns_dft->min_again = 1024;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 64; /* max 1024 */
    ae_sns_dft->min_dgain = 64;  /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->ae_compensation = 24;

    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 16462; /* init 16462 */

    if (g_fswdr_mode[vi_pipe] == OT_EIS_VI_FSWDR_LONG_FRAME_MODE) {
        ae_sns_dft->ae_compensation = 56; /* ae_compensation 56 */
        ae_sns_dft->ae_exp_mode = OT_EIS_VI_AE_EXP_HIGHLIGHT_PRIOR;
    } else {

        ae_sns_dft->max_dgain_target = 1024;
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
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
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

static ot_void cmos_config_vmax(ot_eis_sns_state *sns_state, ot_u32 vmax)
{
    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(vmax);
    } else {
        sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(vmax);
    }

    return;
}

/* the function of sensor set fps */
static ot_bool cmos_2to1_vmax_limit(ot_u32 *vmax, ot_u32 full_line, ot_u32 step, ot_bool fps_up)
{
    if (fps_up) {
        if ((*vmax) + step < full_line) {
            (*vmax) = (full_line - step);
            return OT_FALSE;
        }
    } else {
        if ((*vmax) > full_line + step) {
            (*vmax) = (full_line + step);
            return OT_FALSE;
        }
    }
    return OT_TRUE;
}

static ot_void cmos_fps_set(ot_vi_pipe_id vi_pipe, ot_float fps, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    ot_u32 lines, lines_max, vmax;
    ot_float max_fps, min_fps;
    ot_bool achieve_fps_ok;
    ot_eis_sns_state *sns_state = OT_NULL;
    static ot_bool is_first[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = 1};

    sns_check_pointer_void_return(ae_sns_dft);
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines = g_gc2093_mode_tbl[sns_state->img_mode].ver_lines;
    lines_max = g_gc2093_mode_tbl[sns_state->img_mode].max_ver_lines;
    max_fps = g_gc2093_mode_tbl[sns_state->img_mode].max_fps;
    min_fps = g_gc2093_mode_tbl[sns_state->img_mode].min_fps;
    if ((fps > max_fps) || (fps < min_fps)) {
        sns_err_trace("Not support Fps: %f\n", fps);
        return;
    }

    achieve_fps_ok = OT_TRUE;
    vmax = (ot_u32)(lines * max_fps / sns_div_0_to_1_float(fps));

    vmax = (vmax > lines_max) ? lines_max : vmax;
    cmos_config_vmax(sns_state, vmax);

    sns_state->fl_std = vmax;
    ae_sns_dft->lines_per500ms = lines * max_fps / 2; /* div 2 */

    g_lines_per500ms[vi_pipe] = ae_sns_dft->lines_per500ms;
    // ae_sns_dft->fps = (achieve_fps_ok == OT_TRUE) ? fps : (lines * max_fps / vmax);
    ae_sns_dft->fps =  fps ;
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->max_int_time = sns_state->fl_std - 2; /* sub 10 */
    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];

    ae_sns_dft->hmax_times =
	(ot_u32)((1000000000) / (sns_state->fl_std * sns_div_0_to_1_float(fps))); /* 1000000000ns */
    return;
}

static ot_void cmos_slow_framerate_set(ot_vi_pipe_id vi_pipe, ot_u32 full_lines, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    ot_u32 lines_max;
    ot_u32 vmax;
    ot_eis_sns_state *sns_state = OT_NULL;
    sns_check_pointer_void_return(ae_sns_dft);
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    lines_max = g_gc2093_mode_tbl[sns_state->img_mode].max_ver_lines;

    vmax = full_lines;
    vmax = (vmax > lines_max) ? lines_max : vmax;
    sns_state->fl[0] = vmax;

    switch (sns_state->wdr_mode) {
        case OT_EIS_WDR_MODE_NONE:
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(sns_state->fl[0]);
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(sns_state->fl[0]);
            break;
        case OT_EIS_WDR_MODE_2To1_LINE:
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].data = low_8bits(sns_state->fl[0]);
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].data = high_8bits(sns_state->fl[0]);
            break;
        default:
            break;
    }

    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->max_int_time = sns_state->fl[0] - 2; /* max_int_time: Flstd - 10 */

    return;
}

static ot_void cmos_inttime_update_linear(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;
    ot_u32 TimeVal;

    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);
    TimeVal=(int_time > GC2093_FULL_LINES_MAX) ? GC2093_FULL_LINES_MAX : int_time;
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = low_8bits(TimeVal);
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = high_8bits(TimeVal);

    return;
}

static ot_void cmos_inttime_update_2to1_line(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;
    static ot_bool is_first[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = 1};

    static ot_u32 short_int_time[OT_EIS_VI_MAX_PIPE_NUM] = {0};
    static ot_u32 long_int_time[OT_EIS_VI_MAX_PIPE_NUM] = {0};

    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (is_first[vi_pipe]) { /* short exposure */
        short_int_time[vi_pipe] = int_time;
        if(short_int_time[vi_pipe]<=4){
          sns_state->regs_info[0].i2c_data[DePink_A].data = 0X01;
          sns_state->regs_info[0].i2c_data[DePink_B].data = 0X50;
          sns_state->regs_info[0].i2c_data[DePink_C].data = 0Xfd;
        }else{
          sns_state->regs_info[0].i2c_data[DePink_A].data = 0X01;
          sns_state->regs_info[0].i2c_data[DePink_B].data = 0X50;
          sns_state->regs_info[0].i2c_data[DePink_C].data = 0Xf8;
        }
        is_first[vi_pipe] = OT_FALSE;
    } else { /* long exposure */
        sns_state->wdr_int_time[1] = int_time;
        long_int_time[vi_pipe] = int_time;

        sns_state->regs_info[0].i2c_data[EXPO_H_IDX].data = high_8bits(long_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[EXPO_L_IDX].data = low_8bits(long_int_time[vi_pipe]);

        sns_state->regs_info[0].i2c_data[SHORT_EXPO_H_IDX].data = high_8bits(short_int_time[vi_pipe]);
        sns_state->regs_info[0].i2c_data[SHORT_EXPO_L_IDX].data = low_8bits(short_int_time[vi_pipe]);

        is_first[vi_pipe] = OT_TRUE;
    }

    return;
}

/* while vi notify ae to update sensor regs, ae call these funcs. */
static ot_void cmos_inttime_update(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        cmos_inttime_update_2to1_line(vi_pipe, int_time);
    } else {
        cmos_inttime_update_linear(vi_pipe, int_time);
    }

    return;
}
static ot_u32 regValTable[25][7]={
    //   0xb3 0xb8 0xb9 0x155 0xc2 0xcf 0xd9
    {0x00, 0x01, 0x00, 0x08, 0x10, 0x08, 0x0a},
    {0x10, 0x01, 0x0c, 0x08, 0x10, 0x08, 0x0a},
    {0x20, 0x01, 0x1b, 0x08, 0x11, 0x08, 0x0c},
    {0x30, 0x01, 0x2c, 0x08, 0x12, 0x08, 0x0e},
    {0x40, 0x01, 0x3f, 0x08, 0x14, 0x08, 0x12},
    {0x50, 0x02, 0x16, 0x08, 0x15, 0x08, 0x14},
    {0x60, 0x02, 0x35, 0x08, 0x17, 0x08, 0x18},
    {0x70, 0x03, 0x16, 0x08, 0x18, 0x08, 0x1a},
    {0x80, 0x04, 0x02, 0x08, 0x1a, 0x08, 0x1e},
    {0x90, 0x04, 0x31, 0x08, 0x1b, 0x08, 0x20},
    {0xa0, 0x05, 0x32, 0x08, 0x1d, 0x08, 0x24},
    {0xb0, 0x06, 0x35, 0x08, 0x1e, 0x08, 0x26},
    {0xc0, 0x08, 0x04, 0x08, 0x20, 0x08, 0x2a},
    {0x5a, 0x09, 0x19, 0x08, 0x1e, 0x08, 0x2a},
    {0x83, 0x0b, 0x0f, 0x08, 0x1f, 0x08, 0x2a},
    {0x93, 0x0d, 0x12, 0x08, 0x21, 0x08, 0x2e},
    {0x84, 0x10, 0x00, 0x0b, 0x22, 0x08, 0x30},
    {0x94, 0x12, 0x3a, 0x0b, 0x24, 0x08, 0x34},
    {0x5d, 0x1a, 0x02, 0x0b, 0x26, 0x08, 0x34},
    {0x9b, 0x1b, 0x20, 0x0b, 0x26, 0x08, 0x34},
    {0x8c, 0x20, 0x0f, 0x0b, 0x26, 0x08, 0x34},
    {0x9c, 0x26, 0x07, 0x12, 0x26, 0x08, 0x34},
    {0xB6, 0x36, 0x21, 0x12, 0x26, 0x08, 0x34},
    {0xad, 0x37, 0x3a, 0x12, 0x26, 0x08, 0x34},
    {0xbd, 0x3d, 0x02, 0x12, 0x26, 0x08, 0x34},
};
#define GAIN_NODE_NUM 25
static ot_u32 g_gain_table[GAIN_NODE_NUM] = {
    1024, 1216, 1456, 1712, 2000, 2352, 2832, 3376, 3968, 4752, 5696, 6800, 8064,
	9584, 11344, 13376, 15648, 18448, 26352, 26416, 30960, 36672, 51824, 63344, 74976,
};

static ot_void cmos_again_calc_table(ot_vi_pipe_id vi_pipe, ot_u32 *again_lin, ot_u32 *again_db)
{
    int i;
    ot_u32 pregain;
    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);
    if (*again_lin >= g_gain_table[GAIN_NODE_NUM - 1]) {
        *again_lin = g_gain_table[GAIN_NODE_NUM - 1];
        *again_db = GAIN_NODE_NUM - 1;
    }else{
        for (i = 1; i < GAIN_NODE_NUM; i++) {
            if (*again_lin < g_gain_table[i]) {
                *again_lin = g_gain_table[i - 1];
                *again_db = i - 1;
                break;
            }
        }
    }
    gc2093_again[vi_pipe] = *again_lin;

    return;
}

static ot_void cmos_dgain_calc_table(ot_vi_pipe_id vi_pipe, ot_u32 *dgain_lin, ot_u32 *dgain_db)
{
    return;
}

static ot_void cmos_gains_update(ot_vi_pipe_id vi_pipe, ot_u32 again, ot_u32 dgain)
{
    ot_eis_sns_state *sns_state = OT_NULL;
    ot_u32 dgainHigh_reg,dgainLow_reg;
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    // dgain = dgain * 1024 /g_gain_table[again] ;

    dgainHigh_reg = (dgain >> 6);
    dgainLow_reg = (dgain & 0x3f) << 2 ;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].data = regValTable[again][0];  //b3
    sns_state->regs_info[0].i2c_data[COL_AGAIN_H_IDX].data = regValTable[again][1];  //b8
    sns_state->regs_info[0].i2c_data[COL_AGAIN_L_IDX].data = regValTable[again][2];  //b9
    sns_state->regs_info[0].i2c_data[AGAIN_MAG1_IDX].data = regValTable[again][3];  //b155
    sns_state->regs_info[0].i2c_data[AGAIN_HOLD_IDX].data = 0x2D;  //031d
    sns_state->regs_info[0].i2c_data[AGAIN_MAG2_IDX].data = regValTable[again][4];  //c2
    sns_state->regs_info[0].i2c_data[AGAIN_MAG3_IDX].data = regValTable[again][5];  //cf
    sns_state->regs_info[0].i2c_data[AGAIN_MAG4_IDX].data = regValTable[again][6];  //d9
    sns_state->regs_info[0].i2c_data[AGAIN_REL_IDX].data = 0x28;  //031d

    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].data = dgainHigh_reg;   //b1
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].data = dgainLow_reg;    //b2

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
    int_time->int_time_max[0] = MAX2((ot_s32)int_time->pre_int_time[0] - step, time_max);    /* index 0 */
    int_time->int_time_min[0] = MAX2((ot_s32)int_time->pre_int_time[0] - step, time_min);    /* index 0 */
    int_time->int_time_max[1] = time_max;                                                       /* index 2 */
    int_time->int_time_min[1] = time_min;
}


static ot_void cmos_get_inttime_max_2to1_line(ot_vi_pipe_id vi_pipe, ot_u32 *ratio,
                                              ot_eis_vi_ae_int_time_range *int_time, ot_u32 *lf_max_int_time)
{
    ot_u32 short_max0, i, short_max, short_time_min_limit;
    ot_eis_sns_state *sns_state = OT_NULL;
    time_step step = {{0}};
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);
    static ot_u32 pre_ratio[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = 0X40};
    short_time_min_limit = 1; /* short_time_min_limit 2 */

    if (g_fswdr_mode[vi_pipe] == OT_EIS_VI_FSWDR_LONG_FRAME_MODE) {
        short_max0 = sns_state->fl[1] - 6 - step.dec[0] - sns_state->wdr_int_time[0];
        short_max = sns_state->fl[0] - 8 - step.dec[0];

        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        int_time->int_time_max[0] = short_time_min_limit;
        int_time->int_time_min[0] = short_time_min_limit;
        int_time->int_time_max[1] = short_max;
        int_time->int_time_min[1] = short_time_min_limit;
        cmos_set_2to1_long_frame_range(int_time, short_time_min_limit, short_max, 90); /* step 90 */
    } else {
        short_max0 = ((sns_state->fl[1] - 16 - sns_state->wdr_int_time[0]) * 0x40) / sns_div_0_to_1(ratio[0]);
        short_max = ((sns_state->fl[0] - 16 ) * 0x40) / (ratio[0] + 0x40);
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        short_max = (short_max > (sns_state->fl[0]-1088-20-20)) ? (sns_state->fl[0]-1088-20-20) : short_max;
        short_max = (short_max == 0) ? 1 : short_max;

        *lf_max_int_time = sns_state->fl[0] - GC2093_MARGIN - step.inc[0];

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
            cmos_step_limit(int_time, &step, sns_state->fl[1], 2); /* frame number is 2 */
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
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
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
#define CALIBRATE_STATIC_TEMP                         5319
#define CALIBRATE_STATIC_WB_R_GAIN                    447
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    398

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1                              84
#define CALIBRATE_AWB_P2                              37
#define CALIBRATE_AWB_Q1                              (-135)
#define CALIBRATE_AWB_A1                              148
#define CALIBRATE_AWB_B1                              45537
#define CALIBRATE_AWB_C1                              (-85308)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static ot_s32 cmos_get_awb_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_sensor_default *awb_sns_dft)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(awb_sns_dft);
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
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
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
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

    isp_def->sns_mode.sns_id = GC2093_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;

    return OT_SUCCESS;
}

static ot_s32 cmos_get_vi_black_level(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_black_level *black_level)
{
    ot_s32  i;
    ot_eis_sns_state *sns_state = OT_NULL;
    const ot_eis_vi_sns_black_level *cmos_blc_def = OT_NULL;

    sns_check_pointer_return(black_level);
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {
        cmos_blc_def = &g_cmos_blc;
    } else {
        cmos_blc_def = &g_cmos_blc_wdr;
    }

    (ot_void)memcpy_s(black_level, sizeof(ot_eis_vi_sns_black_level), cmos_blc_def, sizeof(ot_eis_vi_sns_black_level));

    /* Don't need to update black level when iso change */
    black_level->auto_attr.update = OT_TRUE;

    if (black_level->auto_attr.update==OT_TRUE) {
        if(gc2093_again[vi_pipe]>GC2093_AGAIN_30X){
            if ((gc2093_again[vi_pipe]>GC2093_AGAIN_30X) && (gc2093_again[vi_pipe]<=GC2093_AGAIN_36X)) {
                for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
                    black_level->auto_attr.black_level[i][0] = 0x420;
                    black_level->auto_attr.black_level[i][1] = 0x420;
                    black_level->auto_attr.black_level[i][2] = 0x420; /* index 2 */
                    black_level->auto_attr.black_level[i][3] = 0x420; /* index 3 */
                }
            } else if (gc2093_again[vi_pipe]>GC2093_AGAIN_36X) {
                for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
                    black_level->auto_attr.black_level[i][0] = 0x430;
                    black_level->auto_attr.black_level[i][1] = 0x430;
                    black_level->auto_attr.black_level[i][2] = 0x430; /* index 2 */
                    black_level->auto_attr.black_level[i][3] = 0x430; /* index 3 */
                }
            }
        } else {
          for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
            black_level->auto_attr.black_level[i][0] = 0x400;
            black_level->auto_attr.black_level[i][1] = 0x400;
            black_level->auto_attr.black_level[i][2] = 0x400; /* index 2 */
            black_level->auto_attr.black_level[i][3] = 0x400; /* index 3 */
            }
        }
    } else {
        for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
            black_level->auto_attr.black_level[i][0] = 0x400;
            black_level->auto_attr.black_level[i][1] = 0x400;
            black_level->auto_attr.black_level[i][2] = 0x400; /* index 2 */
            black_level->auto_attr.black_level[i][3] = 0x400; /* index 3 */
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

    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        return;
    } else {
        if (sns_state->img_mode == GC2093_2M_30FPS_10BIT_LINEAR_MODE) {
            full_lines_5fps = (GC2093_VMAX_2M_30FPS_10BIT_LINEAR * 30) / 5; /* , 5fps */
        } else {
            return;
        }
    }

    return;
}

static ot_s32 cmos_set_wdr_mode(ot_vi_pipe_id vi_pipe, ot_u8 mode)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    gc2093_sensor_get_ctx(vi_pipe, sns_state);
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
    sns_state->regs_info[0].com_bus.i2c_dev = g_gc2093_bus_info[vi_pipe].com_bus.i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = REG_MAX_IDX; // TODO: REG_MAX_IDX;

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        sns_state->regs_info[0].reg_num = REG_MAX_IDX; // WDR_REG_MAX_IDX;
        sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    }

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = OT_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = g_gc2093_bus_info[vi_pipe].bus_addr;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = GC2093_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = GC2093_DATA_BYTE;
    }

    /* Linear Mode Regs */
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].reg_addr = GC2093_EXPO_L_ADDR;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].reg_addr = GC2093_EXPO_H_ADDR;

    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].reg_addr = GC2093_AGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[COL_AGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[COL_AGAIN_H_IDX].reg_addr = GC2093_COL_AGAIN_H_ADDR;
    sns_state->regs_info[0].i2c_data[COL_AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[COL_AGAIN_L_IDX].reg_addr = GC2093_COL_AGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG1_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG1_IDX].reg_addr = GC2093_AGAIN_MAG1;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG2_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG2_IDX].reg_addr = GC2093_AGAIN_MAG2;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG3_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG3_IDX].reg_addr = GC2093_AGAIN_MAG3;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG4_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG4_IDX].reg_addr = GC2093_AGAIN_MAG4;

    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].reg_addr = GC2093_DGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].reg_addr = GC2093_DGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].reg_addr = GC2093_VMAX_L_ADDR;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].reg_addr = GC2093_VMAX_H_ADDR;

    return;
}

static ot_void cmos_2to1_line_wdr_sns_reg_info_init(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_L_IDX].reg_addr = GC2093_EXPO_L_ADDR;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[EXPO_H_IDX].reg_addr = GC2093_EXPO_H_ADDR;
    sns_state->regs_info[0].i2c_data[SHORT_EXPO_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SHORT_EXPO_L_IDX].reg_addr = GC2093_SHORT_EXPO_L_ADDR;
    sns_state->regs_info[0].i2c_data[SHORT_EXPO_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[SHORT_EXPO_H_IDX].reg_addr = GC2093_SHORT_EXPO_H_ADDR;

    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_L_IDX].reg_addr = GC2093_AGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[COL_AGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[COL_AGAIN_H_IDX].reg_addr = GC2093_COL_AGAIN_H_ADDR;
    sns_state->regs_info[0].i2c_data[COL_AGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[COL_AGAIN_L_IDX].reg_addr = GC2093_COL_AGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG1_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG1_IDX].reg_addr = GC2093_AGAIN_MAG1;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG2_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG2_IDX].reg_addr = GC2093_AGAIN_MAG2;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG3_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG3_IDX].reg_addr = GC2093_AGAIN_MAG3;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG4_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[AGAIN_MAG4_IDX].reg_addr = GC2093_AGAIN_MAG4;
    //DePink
    sns_state->regs_info[0].i2c_data[DePink_A].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DePink_A].reg_addr = GC2093_DePink_A;
    sns_state->regs_info[0].i2c_data[DePink_B].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DePink_B].reg_addr = GC2093_DePink_B;
    sns_state->regs_info[0].i2c_data[DePink_C].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DePink_C].reg_addr = GC2093_DePink_C;

    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_L_IDX].reg_addr = GC2093_DGAIN_L_ADDR;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[DGAIN_H_IDX].reg_addr = GC2093_DGAIN_H_ADDR;

    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[VMAX_L_IDX].reg_addr = GC2093_VMAX_L_ADDR;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[VMAX_H_IDX].reg_addr = GC2093_VMAX_H_ADDR;

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
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 0;
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 0;
        } else if (sns_state->fl[0] < sns_state->fl[1]) {
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 0; /* delay 2 */
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 0; /* delay 2 */
        } else {
        }
    } else {
        if (sns_state->fl[0] > sns_state->fl[1]) {
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 0;
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 0;
        } else if (sns_state->fl[0] < sns_state->fl[1]) {
            sns_state->regs_info[0].i2c_data[VMAX_L_IDX].delay_frame_num = 0; /* delay 1 */
            sns_state->regs_info[0].i2c_data[VMAX_H_IDX].delay_frame_num = 0; /* delay 1 */
        } else {
        }
    }

    return;
}

static ot_s32 cmos_get_sns_regs_info(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_regs_info *sns_regs_info)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(sns_regs_info);
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if ((sns_state->sync_init == OT_FALSE) || (sns_regs_info->config == OT_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, sns_state);

        if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
            /* VC 2t1 Mode Regs */
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
        case GC2093_2M_30FPS_10BIT_LINEAR_MODE:
            sns_state->fl_std = GC2093_VMAX_2M_30FPS_10BIT_LINEAR;
            break;
        case GC2093_2M_30FPS_10BIT_2TO1_VC_MODE:
            sns_state->fl_std = GC2093_VMAX_2M_30FPS_10BIT_2TO1_WDR;
            break;
        default:
            sns_state->fl_std = GC2093_VMAX_2M_30FPS_10BIT_LINEAR;
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
    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    image_mode = sns_state->img_mode;

    for (i = 0; i < GC2093_MODE_BUTT; i++) {
        if (sns_image_mode->fps <= g_gc2093_mode_tbl[i].max_fps &&
            sns_image_mode->width <= g_gc2093_mode_tbl[i].width &&
            sns_image_mode->height <= g_gc2093_mode_tbl[i].height &&
            sns_state->wdr_mode == g_gc2093_mode_tbl[i].wdr_mode) {
            image_mode = (gc2093_res_mode)i;
            break;
        }
    }

    if (i >= GC2093_MODE_BUTT) {
        gc2093_err_mode_print(sns_image_mode, sns_state);
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

    gc2093_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    sns_state->init      = OT_FALSE;
    sns_state->sync_init = OT_FALSE;
    sns_state->img_mode  = GC2093_2M_30FPS_10BIT_LINEAR_MODE;
    sns_state->wdr_mode  = OT_EIS_WDR_MODE_NONE;
    sns_state->fl_std    = GC2093_VMAX_2M_30FPS_10BIT_LINEAR;
    sns_state->fl[0]     = GC2093_VMAX_2M_30FPS_10BIT_LINEAR;
    sns_state->fl[1]     = GC2093_VMAX_2M_30FPS_10BIT_LINEAR;

    (ot_void)memset_s(&sns_state->regs_info[0], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));
    (ot_void)memset_s(&sns_state->regs_info[1], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));

    return;
}

static ot_s32 sensor_ctx_init(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    gc2093_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == OT_NULL) {
        sns_state_ctx = (ot_eis_sns_state *)malloc(sizeof(ot_eis_sns_state));
        if (sns_state_ctx == OT_NULL) {
            sns_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_FAILURE;
        }
    }

    (ot_void)memset_s(sns_state_ctx, sizeof(ot_eis_sns_state), 0, sizeof(ot_eis_sns_state));

    gc2093_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return OT_SUCCESS;
}

static ot_void sensor_ctx_exit(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    gc2093_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    gc2093_sensor_reset_ctx(vi_pipe);

    return;
}

static ot_eis_vi_sns_register_func g_gc2093_register_to_vi_func = {
    .pfn_sns_init                = gc2093_init,
    .pfn_sns_exit                = gc2093_exit,
    .pfn_sns_global_init         = sensor_global_init,
    .pfn_sns_set_image_mode      = cmos_set_image_mode,
    .pfn_sns_set_wdr_mode        = cmos_set_wdr_mode,
    .pfn_sns_get_vi_alg_default  = cmos_get_vi_alg_default,
    .pfn_sns_get_vi_black_level  = cmos_get_vi_black_level,
    .pfn_sns_get_blc_clamp_info  = cmos_get_blc_clamp_info,
    .pfn_sns_set_pixel_detect    = cmos_set_pixel_detect,
    .pfn_sns_get_reg_info        = cmos_get_sns_regs_info,
};

static ot_eis_vi_ae_sensor_register_func g_gc2093_register_to_ae_func = {
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

static ot_eis_vi_awb_sensor_register_func g_gc2093_register_to_awb_func = {
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

    ret = ot_eis_vi_sensor_register_to_vi(pipe_hdl, &g_gc2093_register_to_vi_func, GC2093_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to vi failed!\n");
        goto free_sns_ctx;
    }

    ret = ot_eis_vi_sensor_register_to_ae(pipe_hdl, &g_gc2093_register_to_ae_func, GC2093_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to ae lib failed!\n");
        goto sns_unreg_to_vi;
    }

    ret = ot_eis_vi_sensor_register_to_awb(pipe_hdl, &g_gc2093_register_to_awb_func, GC2093_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to awb lib failed!\n");
        goto sns_unreg_to_ae;
    }

    return OT_SUCCESS;

sns_unreg_to_ae:
    ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, GC2093_ID);
sns_unreg_to_vi:
    ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, GC2093_ID);
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

    ret = ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, GC2093_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }
    ret = ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, GC2093_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_eis_vi_sensor_unregister_to_awb(pipe_hdl, GC2093_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to awb lib failed!\n");
        return ret;
    }

    sensor_ctx_exit(master_pipe_id);

    return OT_SUCCESS;
}

static ot_s32 gc2093_set_bus_info(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_bus_info *sns_bus_info)
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

    memcpy_s(&g_gc2093_bus_info[master_pipe_id], sizeof(ot_eis_sns_bus_info), sns_bus_info, sizeof(ot_eis_sns_bus_info));

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

ot_eis_sns_obj g_sns_gc2093_obj = {
    .pfn_sns_register              = sensor_register_callback,
    .pfn_sns_unregister            = sensor_unregister_callback,
    .pfn_sns_standby               = gc2093_standby,
    .pfn_sns_restart               = gc2093_restart,
    .pfn_sns_set_mirror_flip       = OT_NULL,
    .pfn_sns_set_blc_clamp         = gc2093_blc_clamp,
    .pfn_sns_set_bus_info          = gc2093_set_bus_info,
    .pfn_sns_set_init              = sensor_set_init
};
