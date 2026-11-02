#ifndef OT_EIS_VI_AE_H
#define OT_EIS_VI_AE_H

#include "ot_eis_type.h"
#include "ot_eis_vi_alg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* Defines the maximum exposure time or gain and minimum exposure time or gain */
typedef struct {
    ot_u32 max;  /* RW;Range:[0,0xFFFFFFFF];Format:32.0;Maximum value */
    ot_u32 min;  /* RW;Range:[0,0xFFFFFFFF];Format:32.0;limited Range:[0,u32Max],Minimum value */
} ot_eis_vi_ae_range;

/*
  Defines the AE exposure policy mode
  0 = Highlight first exposure mode
  1 = Lowlight first exposure mode
 */
typedef enum {
    OT_EIS_VI_AE_EXP_HIGHLIGHT_PRIOR = 0,
    OT_EIS_VI_AE_EXP_LOWLIGHT_PRIOR  = 1,
    OT_EIS_VI_AE_STRATEGY_MODE_BUTT
} ot_eis_vi_ae_strategy;

/*
  Defines the AE mode
  0 = Automatic frame rate reduction mode (slow shutter mode)
  1 = Constant frame rate mode
 */
typedef enum {
    OT_EIS_VI_AE_MODE_SLOW_SHUTTER = 0,
    OT_EIS_VI_AE_MODE_FIX_FRAME_RATE  = 1,
    OT_EIS_VI_AE_MODE_BUTT
} ot_eis_vi_ae_mode;

/*
  Defines the anti-flicker mode.
  0 = The epxosure time is fixed to be the multiplies of 1/(2*frequency) sec,
    it may lead to over exposure in the high-luminance environments.
  1 = The anti flicker may be closed to avoid over exposure in the high-luminance environments.
 */
typedef enum {
    OT_EIS_VI_ANTIFLICKER_NORMAL_MODE = 0x0,
    OT_EIS_VI_ANTIFLICKER_AUTO_MODE   = 0x1,
    OT_EIS_VI_ANTIFLICKER_MODE_BUTT
} ot_eis_vi_antiflicker_mode;

/* Defines the anti-flicker attribute */
typedef struct {
    ot_bool enable;      /* RW; Range:[0, 1]; Format:1.0; */
    ot_u8   frequency;   /* RW; Range:[0, 255]; Format:8.0;
                            usually this value is 50 or 60  which is the frequency of the AC power supply */
    ot_eis_vi_antiflicker_mode  mode;
} ot_eis_vi_antiflicker;

/* Defines the exposure delay attribute */
typedef struct {
    ot_u16 black_delay_frame;    /* RW; Range:[0, 65535]; Format:16.0; AE black delay frame count */
    ot_u16 white_delay_frame;    /* RW; Range:[0, 65535]; Format:16.0; AE white delay frame count */
} ot_eis_vi_ae_delay;

typedef struct {
    ot_bool enable; /* RW; Range:[0, 1]; Format:1.0; */

    ot_u8   luma_diff; /* RW; Range:[0, 100]; Format:8.0; if subflicker mode enable, current luma is less than AE
                          compensation plus LumaDiff, AE will keep min antiflicker shutter time
                          (for example: 1/100s or 1/120s) to avoid flicker. while current luma is larger than AE
                          compensation plus the LumaDiff, AE will reduce shutter time to avoid over-exposure and
                          introduce flicker in the pircture */
} ot_eis_vi_subflicker;

typedef struct {
    ot_eis_op_mode exp_time_op_type;
    ot_eis_op_mode a_gain_op_type;
    ot_eis_op_mode d_gain_op_type;
    ot_eis_op_mode ispd_gain_op_type;

    ot_u32 exp_time;   /* RW; Range:[0x0, 0xFFFFFFFF]; Format:32.0;
                          sensor exposure time (unit: us), it's related to the specific sensor */
    ot_u32 a_gain;     /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                          sensor analog gain (unit: times, 10bit precision), it's related to the specific sensor */
    ot_u32 d_gain;     /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                          sensor digital gain (unit: times, 10bit precision), it's related to the specific sensor */
    ot_u32 isp_d_gain; /* RW; Range:[0x400, 0x40000]; Format:22.10;
                          ISP digital gain(unit: times, 10bit precision), it's related to the ISP digital gain range */
} ot_eis_vi_me_attr;

typedef struct {
    /* base parameter */
    ot_eis_vi_ae_range exp_time_range;   /* RW; Range:[0x0, 0xFFFFFFFF]; Format:32.0; sensor exposure time (unit: us),
                                         it's related to the specific sensor */
    ot_eis_vi_ae_range a_gain_range;     /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                         sensor analog gain (unit: times, 10bit precision), it's related to the
                                         specific sensor */
    ot_eis_vi_ae_range d_gain_range;     /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                         sensor digital gain (unit: times, 10bit precision), it's related to the
                                         specific sensor */
    ot_eis_vi_ae_range ispd_gain_range;  /* RW; Range:[0x400, 0x40000]; Format:22.10;
                                         ISP digital gain (unit: times, 10bit precision), it's related to the
                                         ISP digital gain range */
    ot_eis_vi_ae_range sys_gain_range;   /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                         system gain (unit: times, 10bit precision), it's related to the specific sensor
                                         and ISP Dgain range */
    ot_u32 gain_threshold;            /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                         Gain threshold for slow shutter mode (unit: times, 10bit precision) */

    ot_u8   speed;                   /* RW; Range:[0x0, 0xFF]; Format:8.0;
                                        AE adjust step for dark scene to bright scene switch */
    ot_u16  black_speed_bias;        /* RW; Range:[0x0, 0xFFFF]; Format:16.0;
                                        AE adjust step bias for bright scene to dark scene switch */
    ot_u8   tolerance;               /* RW; Range:[0x0, 0xFF]; Format:8.0; AE adjust tolerance */
    ot_u8   compensation;            /* RW; Range:[0x0, 0xFF]; Format:8.0; AE compensation */
    ot_u16  ev_bias;                 /* RW; Range:[0x0, 0xFFFF]; Format:16.0; AE EV bias */
    ot_eis_vi_ae_strategy ae_strategy_mode;  /* RW; Range:[0, 1]; Format:1.0; Support Highlight prior or Lowlight prior */
    ot_u16  hist_ratio_slope;       /* RW; Range:[0x0, 0xFFFF]; Format:16.0; AE hist ratio slope */
    ot_u8   max_hist_offset;        /* RW; Range:[0x0, 0xFF]; Format:8.0; Max hist offset */

    ot_eis_vi_ae_mode     ae_mode;     /* RW; Range:[0, 1]; Format:1.0; AE mode(slow shutter/fix framerate)(onvif) */
    ot_eis_vi_antiflicker antiflicker;
    ot_eis_vi_subflicker  subflicker;
    ot_eis_vi_ae_delay    ae_delay_attr;

    ot_bool manual_exp_value;     /* RW; Range:[0, 1]; Format:1.0; manual exposure value or not */
    ot_u32  exp_value;            /* RW; Range:(0x0, 0xFFFFFFFF]; Format:32.0; manual exposure value */

    ot_eis_vi_fswdr_mode fswdr_mode; /* RW; Range:[0, 2]; Format:2.0; FSWDR running mode */
    ot_bool wdr_quick;            /* RW; Range:[0, 1]; Format:1.0; WDR use delay strategy or not;
                                     If is true, WDR AE adjust will be faster */

    ot_u16  iso_cal_coef;     /* RW; Range:[0x0, 0xFFFF]; Format:8.8;
                                 The coefficient between Standard ISO and origin ISO (unit: times, 8bit precision) */
} ot_eis_vi_ae_attr;

/*
 * Defines the prior frame of ISP
 * 0 = long frame
 * 1 = short frame
 */
typedef enum {
    OT_EIS_VI_LONG_FRAME      = 0,
    OT_EIS_VI_SHORT_FRAME     = 1,
    OT_EIS_VI_PRIOR_FRAME_BUTT
} ot_eis_vi_prior_frame;

typedef struct {
    ot_bool    bypass;      /* RW; Range:[0, 1]; Format:1.0; */
    ot_eis_op_mode op_type;
    ot_u8     ae_run_interval;   /* RW; Range:[0x1, 0xFF]; Format:8.0; set the AE run interval */
    ot_bool   hist_stat_adjust;  /* RW; Range:[0, 1]; Format:1.0;
                                    OT_TRUE: 256 bin histogram statistic config will adjust when large red or blue
                                    area detected.
                                    OT_FALSE: 256 bin histogram statistic config will not change */
    ot_bool   ae_route_ex_valid;   /* RW; Range:[0, 1]; Format:1.0; use extend AE route or not */
    ot_eis_vi_me_attr   manual_attr;
    ot_eis_vi_ae_attr   auto_attr;
    ot_eis_vi_prior_frame prior_frame; /* RW; Range:[0, 1]; Format:1.0; AE prior frame */
    ot_bool ae_gain_sep_cfg; /* RW; Range:[0, 1]; Format:1.0; long and short frame gain separately configure or not */
    ot_bool advance_ae; /* RW; Range:[0, 1]; Format:1.0;  open advance AE or not */
} ot_eis_vi_exposure_attr;

typedef struct {
    ot_eis_op_mode     exp_ratio_type;   /* RW; Range:[0, 1]; Format:1.0;
                                        OT_EIS_OP_MODE_AUTO: The exp_ratio used in VI is generated by firmware;
                                        OT_EIS_OP_MODE_MANUAL: The exp_ratio used in VI is set by exp_ratio */
    ot_u32 exp_ratio[OT_EIS_VI_EXP_RATIO_NUM]; /* RW; Range:[0x40, 0x4000]; Format:26.6; 0x40 means 1 times.
                                        When exp_ratio_type is OT_EIS_OP_MODE_AUTO, exp_ratio is invalid.
                                        When exp_ratio_type is OT_EIS_OP_MODE_MANUAL, exp_ratio is quotient of
                                        long exposure time / short exposure time. */
    ot_u32 exp_ratio_max;           /* RW; Range:[0x40, 0x4000]; Format:26.6; 0x40 means 1 times.
                                       When exp_ratio_type is OT_EIS_OP_MODE_AUTO, exp_ratio_max is max(upper limit) of
                                       exp_ratio generated by firmware.
                                       When exp_ratio_type is OT_EIS_OP_MODE_MANUAL, exp_ratio_max is invalid. */
    ot_u32 exp_ratio_min;          /* RW; Range:[0x40, 0x4000]; Format:26.6; limited Range:[0x40, exp_ratio_max],
                                      0x40 means 1 times.
                                      When exp_ratio_type is OT_EIS_OP_MODE_AUTO, exp_ratio_min is min(lower limit) of
                                      exp_ratio generated by firmware.
                                      When exp_ratio_type is OT_EIS_OP_MODE_MANUAL, exp_ratio_min is invalid. */
    ot_u16 tolerance;              /* RW; Range:[0x0, 0xFF]; Format:8.0;et the dynamic range tolerance.
                                      Format: unsigned 6.2-bit fixed-point. 0x4 means 1dB. */
    ot_u16 speed;                  /* RW; Range:[0x0, 0xFF]; Format:8.0; exposure ratio adjust speed */
    ot_u16 ratio_bias;             /* RW; Range:[0x0, 0xFFFF]; Format:16.0; exposure ratio bias */
    ot_u16 high_light_target;      /* RW; Range:[0x0, 0x400]; Format:10.0; the high light target. */
    ot_u16 exp_coef_min;       /* RW; Range:[0x0, 0x400]; Format:6.10; 0x400 means 1 times; min exposure coefficient */
} ot_eis_vi_wdr_exposure_attr;

/* AI structs */
/*
  Defines the VI iris type
  0 = DC iris
  1 = P iris
 */
typedef enum {
    OT_EIS_VI_IRIS_TYPE_DC = 0,
    OT_EIS_VI_IRIS_TYPE_P,

    OT_EIS_VI_IRIS_TYPE_BUTT,
} ot_eis_vi_iris_type;

typedef struct {
    ot_bool step_fno_table_change;    /* W; Range:[0, 1]; Format:1.0; Step-F number mapping table change or not */
    ot_bool zero_is_max;              /* RW; Range:[0, 1]; Format:1.0; Step 0 corresponds to max aperture or not,
                                         it's related to the specific iris */
    ot_u16  total_step;               /* RW; Range:[1, 1024]; Format:16.0; Total steps of  Piris's aperture,
                                         it's related to the specific iris */
    ot_u16  step_count;               /* RW; Range:[1, 1024]; Format:16.0; Used steps of Piris's aperture.
                                         when Piris's aperture is too small, the F number precision is not enough,
                                         so not all the steps can be used. It's related to the specific iris */
    ot_u16 step_fno_table[OT_EIS_VI_AI_MAX_STEP_FNO_NUM]; /* RW; Range:[1, 1024]; Format:16.0; Step-F number mapping table.
                                                    F1.0 is expressed as 1024, F32 is expressed as 1,
                                                    it's related to the specific iris */
    ot_eis_vi_iris_f_no  max_iris_fno_target;  /* RW; Range:[0, 10]; Format:4.0;
                                               Max F number of Piris's aperture, it's related to the specific iris */
    ot_eis_vi_iris_f_no  min_iris_fno_target;  /* RW; Range:[0, 10]; Format:4.0;
                                               Min F number of Piris's aperture, it's related to the specific iris */

    ot_bool fno_ex_valid;           /* RW; Range:[0, 1]; Format:1.0; use equivalent gain to present FNO or not */
    ot_u32  max_iris_fno_target_linear;  /* RW; Range:[1, 1024]; Format:16.0;
                                            Max equivalent gain of F number of Piris's aperture,
                                            only used when bFNOExValid is true, it's related to the specific iris */
    ot_u32  min_iris_fno_target_linear;  /* RW; Range:[1, 1024]; Format:16.0;
                                            Min equivalent gain of F number of Piris's aperture,
                                            only used when bFNOExValid is true, it's related to the specific iris */
} ot_eis_vi_piris_attr;

typedef struct {
    ot_u32  exp_time;                   /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    ot_u32  short_exp_time;             /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    ot_u32  median_exp_time;            /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    ot_u32  long_exp_time;              /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    ot_u32  a_gain;                     /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    ot_u32  d_gain;                     /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    ot_u32  a_gain_sf;                  /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    ot_u32  d_gain_sf;                  /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    ot_u32  isp_d_gain;                 /* R; Range:[0x400, 0xFFFFFFFF]; Format:22.10; */
    ot_u32  exposure;                   /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    ot_bool exposure_is_max;            /* R; Range:[0, 1]; Format:1.0; */
    ot_s16  hist_error;                 /* R; Range:[-32768, 32767]; Format:16.0; */
    ot_u32  ae_hist1024_value[OT_EIS_VI_HIST_NUM]; /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0;
                                            1024 bins histogram for channel 1 */

    ot_u8  ave_lum;                 /* R; Range:[0x0, 0xFF]; Format:8.0; */
    ot_u32 lines_per500ms;          /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; exposure lines per 500ms */
    ot_u32 piris_fno;               /* R; Range:[0x0, 0x400]; Format:32.0; */
    ot_u32 fps;                     /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; actual fps */
    ot_u32 iso;                     /* R; Range:[0x64, 0xFFFFFFFF]; Format:32.0; */
    ot_u32 isosf;                   /* R; Range:[0x64, 0xFFFFFFFF]; Format:32.0; */
    ot_u32 iso_calibrate;           /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; */
    ot_u32 ref_exp_ratio;           /* R; Range:[0x40, 0x4000]; Format:26.6; */
    ot_u16 wdr_exp_coef;            /* R; Range:[0x0, 0x400]; Format:6.10; 0x400 means 1 times */
    ot_u32 first_stable_time;       /* R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; AE first stable time for quick start */
    ot_u32 quick_star_iso;          /* R; Range:[0x64, 0xFFFFFFFF]; Format:32.0; */
    ot_eis_vi_ae_route ae_route;       /* R; Actual AE route */
    ot_eis_vi_ae_route_ex ae_route_ex; /* R; Actual AE route_ex */
    ot_eis_vi_ae_route ae_route_sf;    /* R; Actual AE route_sf */
    ot_eis_vi_ae_route_ex ae_route_sf_ex; /* R; Actual AE route_sf_ex */
} ot_eis_vi_exp_info;

typedef struct {
    ot_bool enable;               /* RW; Range:[0, 1]; Format:1.0; smart ae enable or not */
    ot_bool ir_mode;              /* RW; Range:[0, 1]; Format:1.0; smart ae IR mode or not */
    ot_eis_op_mode   smart_exp_type;  /* RW; Range:[0, 1]; Format:1.0;
                                       OT_EIS_OP_MODE_AUTO: The exp_coef used in VI is  generated by firmware;
                                       OT_EIS_OP_MODE_MANUAL: The exp_coef used in VI is set by exp_coef */
    ot_u16  exp_coef;          /* RW; Range:[0x0, 0xFFFF]; Format:6.10; 0x400 means 1 times.
                                  When enExpHDRLvType is OT_EIS_OP_MODE_AUTO, exp_coef is invalid.
                                  When enExpHDRLvType is OT_EIS_OP_MODE_MANUAL, exp_coef is the quotient of exposure. */
    ot_u8   luma_target;       /* RW; Range:[0x0, 0xFF]; Format:8.0; luma target of smart ae. */
    ot_u16  exp_coef_max;      /* RW; Range:[0x0, 0xFFFF]; Format:6.10; 0x400 means 1 times.
                                  When enExpHDRLvType is OT_EIS_OP_MODE_AUTO, u32ExpCoefvMax is max(upper limit) of ExpCoef
                                  generated by firmware.
                                  When enExpHDRLvType is OT_EIS_OP_MODE_MANUAL, u32ExpCoefMax is invalid. */
    ot_u16  exp_coef_min;      /* RW; Range:[0x0, 0xFFFF]; Format:6.10; 0x400 means 1 times.
                                  When enExpHDRLvType is OT_EIS_OP_MODE_AUTO, u32ExpCoefMax is min(lower limit) of ExpCoef
                                  generated by firmware.
                                  When enExpHDRLvType is OT_EIS_OP_MODE_MANUAL, u32ExpCoefMin is invalid. */
    ot_u8   smart_interval;       /* RW; Range:[0x1, 0xFF]; Format:8.0; smart ae run interval */
    ot_u8   smart_speed;          /* RW; Range:[0x0, 0xFF]; Format:8.0; smart ae adjust step */
    ot_u16  smart_delay_num;      /* RW; Range:[0x0, 0x400]; Format:16.0; smart ae adjust delay frame num */
} ot_eis_vi_smart_exposure_attr;

/* sensor's interface to ae
 * eg: 0.35db, accu_type=OT_EIS_VI_AE_ACCURACY_DB, accuracy=0.35
 *  and the multiply of 0.35db is power(10, (0.35/20))
 *  eg: 1/16, 2/16, 3/16 multiplies, accu_type=OT_EIS_VI_AE_ACCURACY_LINEAR, accuracy=0.0625
 *  eg: 1,2,4,8,16 multiplies, accu_type=OT_EIS_VI_AE_ACCURACY_DB, accuracy=6
 */
typedef enum {
    OT_EIS_VI_AE_ACCURACY_DB = 0,
    OT_EIS_VI_AE_ACCURACY_LINEAR,
    OT_EIS_VI_AE_ACCURACY_TABLE,

    OT_EIS_VI_AE_ACCURACY_BUTT,
} ot_eis_vi_ae_accuracy_type;

typedef struct {
    ot_eis_vi_ae_accuracy_type accu_type;
    ot_float   accuracy;
    ot_float   offset;
} ot_eis_vi_ae_accuracy;

typedef struct {
    ot_bool quick_start_enable;
    ot_u8   black_frame_num;
    ot_bool ir_mode_en;
    ot_u32  init_exposure_ir;
    ot_u32  iso_thr_ir;
    ot_u16  ir_cut_delay_time;
} ot_eis_vi_quick_start_param;

typedef struct {
    ot_u8   ae_compensation;

    ot_u32  lines_per500ms;
    ot_u32  flicker_freq;
    ot_float fps;
    ot_u32  hmax_times; /* unit is ns */
    ot_u32  init_exposure;
    ot_u32  init_int_time;
    ot_u32  init_again;
    ot_u32  init_dgain;
    ot_u32  init_isp_dgain;
    ot_u32  init_ae_speed;
    ot_u32  init_ae_tolerance;

    ot_u32  full_lines_std;
    ot_u32  full_lines_max;
    ot_u32  full_lines;
    ot_u32  binning_full_lines;
    ot_u32  max_int_time;     /* RW;unit is line */
    ot_u32  min_int_time;
    ot_u32  max_int_time_target;
    ot_u32  min_int_time_target;
    ot_eis_vi_ae_accuracy int_time_accu;

    ot_u32  max_again;
    ot_u32  min_again;
    ot_u32  max_again_target;
    ot_u32  min_again_target;
    ot_eis_vi_ae_accuracy again_accu;

    ot_u32  max_dgain;
    ot_u32  min_dgain;
    ot_u32  max_dgain_target;
    ot_u32  min_dgain_target;
    ot_eis_vi_ae_accuracy dgain_accu;

    ot_u32  max_isp_dgain_target;
    ot_u32  min_isp_dgain_target;
    ot_u32  isp_dgain_shift;

    ot_u32  max_int_time_step;
    ot_bool max_time_step_enable;
    ot_u32  max_inc_time_step[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32  max_dec_time_step[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32  lf_max_short_time;
    ot_u32  lf_min_exposure;

    ot_eis_vi_ae_route ae_route_attr;
    ot_bool ae_route_ex_valid;
    ot_eis_vi_ae_route_ex ae_route_attr_ex;

    ot_eis_vi_ae_route ae_route_sf_attr;
    ot_eis_vi_ae_route_ex ae_route_sf_attr_ex;

    ot_u16 man_ratio_enable;
    ot_u32 arr_ratio[OT_EIS_VI_EXP_RATIO_NUM];

    ot_eis_vi_iris_type  iris_type;
    ot_eis_vi_piris_attr piris_attr;
    ot_eis_vi_iris_f_no  max_iris_fno;  /* RW; Range:[0, 10]; Format:4.0;
                                        Max F number of Piris's aperture, it's related to the specific iris */
    ot_eis_vi_iris_f_no  min_iris_fno;  /* RW; Range:[0, 10]; Format:4.0;
                                        Min F number of Piris's aperture, it's related to the specific iris */

    ot_eis_vi_ae_strategy ae_exp_mode;

    ot_u16 iso_cal_coef;
    ot_u8  ae_run_interval;
    ot_u32 exp_ratio_max;
    ot_u32 exp_ratio_min;
    ot_bool diff_gain_support;
    ot_eis_vi_quick_start_param quick_start;
    ot_eis_vi_prior_frame prior_frame;
    ot_bool ae_gain_sep_cfg;
    ot_bool lhcg_support;
    ot_u32 sns_lhcg_exp_ratio;

    ot_u8 ae_stat_pos;          /* RW; Range:[0, 1]; Format:1.0 */
} ot_eis_vi_ae_sensor_default;

typedef struct {
    ot_eis_vi_fswdr_mode fswdr_mode;
} ot_eis_vi_ae_fswdr_attr;

typedef struct {
    ot_u32 reg_addr;
    ot_u32 reg_value;
} ot_eis_vi_ae_param_reg;

typedef struct {
    ot_u32 tar_fps;
    ot_u32 exp_time;
    ot_u32 exp_again;
    ot_u32 exp_dgain;
    ot_u32 exp_isp_dgain;
    ot_eis_vi_ae_param_reg time_reg[10]; /* 10 */
    ot_eis_vi_ae_param_reg again_reg[10]; /* 10 */
    ot_eis_vi_ae_param_reg dgain_reg[10]; /* 10 */
} ot_eis_vi_ae_convert_param;

typedef struct {
    ot_u32 int_time_min[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32 int_time_max[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32 pre_int_time[OT_EIS_VI_WDR_MAX_FRAME_NUM];
} ot_eis_vi_ae_int_time_range;

typedef struct {
    ot_u8   dark_throw_ratio;    /* RW; Range:[1, 100]; Format:8.0; */
    ot_u8   bright_throw_ratio;  /* RW; Range:[1, 100]; Format:8.0; */
    ot_u32  bright_compensation; /* RW; Range:[0, 4095]; Format:32.0; */
} ot_eis_vi_thermo_sensor_default;

typedef struct {
    ot_s32 (*pfn_sns_get_ae_default)(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft);

    /* the function of sensor set fps */
    ot_void (*pfn_sns_cfg_fps)(ot_vi_pipe_id vi_pipe, ot_float f32_fps, ot_eis_vi_ae_sensor_default *ae_sns_dft);
    ot_void (*pfn_sns_cfg_slow_framerate)(ot_vi_pipe_id vi_pipe, ot_u32 full_lines, ot_eis_vi_ae_sensor_default *ae_sns_dft);

    /* while vi notify ae to update sensor regs, ae call these funcs. */
    ot_void (*pfn_sns_update_inttime)(ot_vi_pipe_id vi_pipe, ot_u32 int_time);
    ot_void (*pfn_sns_update_gains)(ot_vi_pipe_id vi_pipe, ot_u32 again, ot_u32 dgain);

    ot_void (*pfn_sns_calc_table_again)(ot_vi_pipe_id vi_pipe, ot_u32 *again_lin, ot_u32 *again_db);
    ot_void (*pfn_sns_calc_table_dgain)(ot_vi_pipe_id vi_pipe, ot_u32 *dgain_lin, ot_u32 *dgain_db);

    ot_void (*pfn_sns_get_inttime_max)(ot_vi_pipe_id vi_pipe, ot_u16 man_ratio_enable, ot_u32 *ratio,
                                       ot_eis_vi_ae_int_time_range *int_time, ot_u32 *lf_max_int_time);

    /* long frame mode set */
    ot_void (*pfn_sns_set_fswdr_attr)(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_fswdr_attr *ae_fswdr_attr);
    ot_void (*pfn_sns_set_quick_start_status)(ot_vi_pipe_id vi_pipe, ot_bool quick_start_status);
    ot_void (*pfn_sns_convert_exp_param)(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_convert_param *exp_param);
    ot_s32 (*pfn_sns_get_thermo_default)(ot_vi_pipe_id vi_pipe, ot_eis_vi_thermo_sensor_default *thermo_sns_dft);
} ot_eis_vi_ae_sensor_register_func;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif