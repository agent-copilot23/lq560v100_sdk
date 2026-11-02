/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#ifndef OT_EIS_VI_AWB_H
#define OT_EIS_VI_AWB_H

#include "ot_eis_type.h"
#include "ot_eis_vi_alg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

/*
  Defines the AWB algorithm type
  0 = Improved gray world algorithm.
  1 = AWB algorithm that classifies the statistics and re-filters the white blocks
  2 = AWB algorithm that opt outdoor performance for dv
 */
typedef enum {
    OT_EIS_VI_AWB_ALG_LOWCOST = 0,
    OT_EIS_VI_AWB_ALG_ADVANCE = 1,
    OT_EIS_VI_AWB_ALG_NATURA  = 2,
    OT_EIS_VI_AWB_ALG_BUTT
} ot_eis_vi_awb_alg_type;

/* Defines the AWB gain range */
typedef struct {
    ot_bool enable;
    ot_eis_op_mode   op_type;

    ot_u16 high_rg_limit; /* RW; Range:[0x0, 0xFFF]; Format:4.8;
                             in Manual Mode, user define the Max Rgain of High Color Temperature */
    ot_u16 high_bg_limit; /* RW; Range:[0x0, 0xFFF]; Format:4.8;
                             in Manual Mode, user define the Min Bgain of High Color Temperature */
    ot_u16 low_rg_limit;  /* RW; Range:[0x0, 0xFFF]; Format:4.8; limited range:[0x0, high_rg_limit),
                             in Manual Mode, user define the Min Rgain of Low Color Temperature */
    ot_u16 low_bg_limit;  /* RW; Range:[0, 0xFFF]; Format:4.8; limited Range:(high_bg_limit, 0xFFF],
                             in Manual Mode, user define the Max Bgain of Low Color Temperature */
} ot_eis_vi_awb_ct_limit_attr;

typedef struct {
    ot_bool enable;                         /* RW; Range:[0, 1]; Format:1.0;
                                               If enabled, statistic parameter cr, cb will change according to iso */

    ot_u16  cr_max[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0xFFF]; cr_max[i] >= cr_min[i] */
    ot_u16  cr_min[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0xFFF] */
    ot_u16  cb_max[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0xFFF]; cb_max[i] >= cb_min[i] */
    ot_u16  cb_min[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0xFFF] */
} ot_eis_vi_awb_cbcr_track_attr;

#define OT_EIS_VI_AWB_LUM_HIST_NUM                          6
/* Defines the parameters of the luminance histogram statistics for white balance */
typedef struct {
    ot_bool enable;                        /* RW; Range:[0, 1]; Format:1.0;
                                              If enabled, zone weight to awb is combined with zone luma */
    ot_eis_op_mode   op_type;              /* In auto mode, the weight distribution follows Gaussian distribution */
    ot_u8   hist_thresh[OT_EIS_VI_AWB_LUM_HIST_NUM]; /* RW; Range:[0x0, 0xFF]; Format:8.0;
                                              In manual mode, user define luma thresh, thresh[0] is 0,
                                              thresh[5] is 0xFF,  thresh[i] greater or equal to  thresh[i-1] */
    ot_u16  hist_wt[OT_EIS_VI_AWB_LUM_HIST_NUM];     /* RW; Range:[0x0, 0xFFFF]; Format:16.0;
                                              user can define luma weight in both manual and auto mode. */
} ot_eis_vi_awb_lum_histgram_attr;

#define OT_EIS_VI_AWB_CURVE_PARA_NUM                        6
typedef struct {
    ot_bool enable;               /* RW; Range:[0x0, 0x1]; Format:1.0;If AWB is disabled, static wb gain will be used,
                                     otherwise auto wb gain will be used */

    ot_u16 ref_color_temp;        /* RW; Range:[0x0, 0xFFFF]; Format:16.0; Calibration Information */
    ot_u16 static_wb[OT_EIS_VI_BAYER_CHN_NUM];   /* RW; Range:[0x0, 0xFFF]; Format:12.0;Calibration Information */
    ot_s32 curve_para[OT_EIS_VI_AWB_CURVE_PARA_NUM]; /* RW; Format:32.0;Calibration Information,
                                              limited Range:as32CurvePara[3] != 0, as32CurvePara[4]==128 */

    ot_eis_vi_awb_alg_type       alg_type;

    ot_u8  rg_strength;     /* RW; Range: [0x0, 0xFF]; Format:8.0; AWB Strength of R Channel */
    ot_u8  bg_strength;     /* RW; Range: [0x0, 0xFF]; Format:8.0; AWB Strength of B Channel */
    ot_u16 speed;           /* RW; Range: [0x0, 0xFFF]; Format:12.0; Convergence speed of AWB  */
    ot_u16 zone_sel;        /* RW; Range: [0, 255]; Format:8.0; A value of 0 or 0xFF means global AWB,
                               A value between 0 and 0xFF means zoned AWB */
    ot_u16 high_color_temp; /* RW; Range: [1, 65535]; Format:16.0; AWB max temperature, Recommended: [8500, 10000] */
    ot_u16 low_color_temp;  /* RW; Range: [0, 65535]; Format:16.0; Limited Range:[0, high_color_temp),
                               AWB min temperature, Recommended: [2000, 2500] */
    ot_eis_vi_awb_ct_limit_attr ct_limit;
    ot_bool shift_limit_en;  /* RW; Range: [0, 1]; Format:1.0; If enabled, when the statistic information is out of
                                range, it should be project back */
    ot_u8  shift_limit;      /* RW; Range: [0x0, 0xFF];  Format:8.0; planckian curve range, Recommended: [0x30, 0x50] */
    ot_bool gain_norm_en;    /* RW; Range: [0, 1]; Format:1.0; if enabled, the min of RGB gain is fixed. */
    ot_bool natural_cast_en; /* RW, Range: [0, 1]; Format:1.0; if enabled, the color performance will be natural in
                                lowlight and low color temperature */

    ot_eis_vi_awb_cbcr_track_attr   cb_cr_track;
    ot_eis_vi_awb_lum_histgram_attr luma_hist;
    ot_bool awb_zone_wt_en;      /* RW, Range: [0, 1]; Format:1.0; if enabled, user can set weight for each zones */
    ot_u8   zone_wt[OT_EIS_VI_AWB_ZONE_NUM];   /* RW; Range: [0, 255]; Format:8.0;Zone Wt Table */
} ot_eis_vi_wb_auto_attr;

typedef struct {
    ot_u16 r_gain;            /* RW; Range: [0x0, 0xFFF]; Format:4.8; Multiplier for R  color channel  */
    ot_u16 gr_gain;           /* RW; Range: [0x0, 0xFFF]; Format:4.8; Multiplier for Gr color channel */
    ot_u16 gb_gain;           /* RW; Range: [0x0, 0xFFF]; Format:4.8; Multiplier for Gb color channel */
    ot_u16 b_gain;            /* RW; Range: [0x0, 0xFFF]; Format:4.8; Multiplier for B  color channel */
} ot_eis_vi_wb_manual_attr;

typedef enum {
    OT_EIS_VI_ALG_AWB_GW      = 0,
    OT_EIS_VI_ALG_AWB_SPEC    = 1,
    OT_EIS_VI_ALG_BUTT
} ot_eis_vi_awb_alg;

typedef struct {
    ot_bool bypass;              /* RW; Range: [0, 1];  Format:1.0; If enabled, awb will be bypassed */
    ot_u8   awb_run_interval;    /* RW; Range: [0x1, 0xFF];  Format:8.0; set the AWB run interval */
    ot_eis_op_mode   op_type;
    ot_eis_vi_wb_manual_attr  manual_attr;
    ot_eis_vi_wb_auto_attr    auto_attr;
    ot_eis_vi_awb_alg         alg_type;
} ot_eis_vi_awb_attr;

/* Defines the information about a separate illuminant */
typedef struct {
    ot_u16 white_r_gain;  /* RW;Range:[0x0, 0xFFF]; Format:4.8; G/R of White points at this light source */
    ot_u16 white_b_gain;  /* RW;Range:[0x0, 0xFFF]; Format:4.8; G/B of White points at this light source */
    ot_u16 exp_quant;     /* RW;shutter time * again * dgain >> 4, Not support Now */
    ot_u8  light_status;  /* RW;Range:[0,2]; Format:2.0; idle  1:add light source  2:delete sensitive color */
    ot_u8  radius;        /* RW;Range:[0x0, 0xFF];  Format:8.0; Radius of light source, */
} ot_eis_vi_awb_extra_light_source_info;

typedef enum {
    OT_EIS_VI_AWB_SCENE_MODE_INDOOR  = 0,
    OT_EIS_VI_AWB_SCENE_MODE_OUTDOOR = 1,
    OT_EIS_VI_AWB_SCENE_MODE_BUTT
} ot_eis_vi_awb_scene_mode_status;

typedef struct {
    ot_bool enable;                 /* RW; Range:[0, 1]; Format:1.0; Outdoor/Indoor scenario determination enable */
    ot_eis_op_mode   op_type;
    ot_eis_vi_awb_scene_mode_status scene_status; /* RW; User should config indoor or outdoor status in Manual Mode */
    ot_u32 out_thresh;         /* RW;Format:32.0;shutter time(in us) to judge indoor or outdoor */
    ot_u16 low_start;          /* RW;Format:16.0;5000K is recommend */
    ot_u16 low_stop;           /* RW;Format:16.0;limited range:(0, low_start),
                                  4500K is recommend, should be smaller than low_start */
    ot_u16 high_start;         /* RW;Format:16.0;limited range:(low_start, 0xFFFF],
                                  6500K is recommend, should be larger than low_start */
    ot_u16 high_stop;          /* RW;Format:16.0;limited range:(high_start, 0xFFFF],
                                  8000K is recommend, should be larger than high_start */
    ot_bool green_enhance_en;  /* RW; Range:[0, 1]; Format:1.0;If this is enabled, Green channel will be enhanced
                                  based on the ratio of green plant */
    ot_u8   out_shift_limit;   /* RW; Range:[0, 0xFF]; Format:8.0;Max white point zone distance to Planckian Curve */
} ot_eis_vi_awb_in_out_attr;

/*
  Defines the AWB policy in the multi-illuminant scenario
  0 = Automatic saturation adjustment in the multi-illuminant scenario
  1 = Automatic ccm adjustment in the multi-illuminant scenario
 */
typedef enum {
    OT_EIS_VI_AWB_MULTI_LS_SAT = 0,
    OT_EIS_VI_AWB_MULTI_LS_CCM = 1,
    OT_EIS_VI_AWB_MULTI_LS_BUTT
} ot_eis_vi_awb_multi_ls_type;

#define OT_EIS_VI_AWB_LS_NUM                                4
#define OT_EIS_VI_AWB_MULTI_CT_NUM                          8
/* extended AWB attributes */
typedef struct {
    ot_u8  tolerance;        /* RW; Range:[0x0, 0xFF]; Format:8.0; AWB adjust tolerance */
    ot_u8  zone_radius;      /* RW; Range:[0x0, 0xFF]; Format:8.0; radius of AWB blocks */
    ot_u16 curve_l_limit;    /* RW; Range:[0x0, 0x100]; Format:9.0; Left limit of AWB Curve,
                                recommend for indoor 0xE0, outdoor 0xE0 */
    ot_u16 curve_r_limit;    /* RW; Range:[0x100, 0xFFF]; Format:12.0; Right Limit of AWB Curve,
                                recommend for indoor 0x130, outdoor 0x120 */

    ot_bool  extra_light_en; /* RW; Range:[0, 1]; Format:1.0; Enable special light source function */
    ot_eis_vi_awb_extra_light_source_info  light_info[OT_EIS_VI_AWB_LS_NUM];
    ot_eis_vi_awb_in_out_attr              in_or_out;

    ot_bool multi_light_source_en;           /* RW; Range:[0, 1]; Format:1.0; If enabled, awb will do special process
                                                in multi light source environment */
    ot_eis_vi_awb_multi_ls_type multi_ls_type;  /* Saturation or CCM Tuning */
    ot_u16  multi_ls_scaler;                 /* RW; Range:[0x0, 0x100]; Format:12.0;
                                                In saturation type, it means the max saturation it can achieve,
                                                in ccm type, it means the strength of multi process. */
    ot_u16  multi_ct_bin[OT_EIS_VI_AWB_MULTI_CT_NUM];  /* RW; Range:[0, 0XFFFF]; Format:16.0;
                                                AWB Support divide the color temperature range by 8 bins */
    ot_u16  multi_ct_wt[OT_EIS_VI_AWB_MULTI_CT_NUM];   /* RW; Range:[0x0, 0x400];Weight for different color temperature,
                                                same value of 8 means CT weight doesn't work, */

    ot_bool fine_tun_en;       /* RW; Range:[0x0, 0x1]; Format:1.0;If enabled, skin color scene will be optimized */
    ot_u8   fine_tun_strength; /* RW; Range:[0x0, 0xFF]; Format:8.0;
                                  larger value means better performance of skin color scene,
                                  but will increase error probability in low color temperature scene */
} ot_eis_vi_awb_ex_attr;

typedef struct {
    ot_bool sat_en;               /* RW; Range: [0, 1]; Format:1.0;
                                     If sat_en=1, the active CCM = SatMatrix * ManualMatrix,
                                     else the active CCM =  ManualMatrix */
    ot_u16  ccm[OT_EIS_VI_CCM_MATRIX_SIZE]; /* RW; Range: [0x0, 0xFFFF]; Format:8.8; Manul CCM matrix, */
} ot_eis_vi_color_matrix_manual_attr;

typedef struct {
    ot_u16 color_temp;           /* RW; Range: [500, 30000]; Format:16.0; the current color temperature */
    ot_u16 ccm[OT_EIS_VI_CCM_MATRIX_SIZE]; /* RW; Range: [0x0, 0xFFFF]; Format:8.8;CCM matrixes for different
                                           color temperature */
} ot_eis_vi_color_matrix_param;

typedef struct {
    ot_bool iso_act_en;    /* RW; Range: [0, 1]; Format:1.0; if enabled, CCM will bypass in low light */
    ot_bool temp_act_en;   /* RW; Range: [0, 1]; Format:1.0; if enabled, CCM will bypass when color temperature is
                              larger than 8000K or less than 2500K */
    ot_u16  ccm_tab_num;   /* RW; Range: [0x3, 0x7]; Format:16.0; The number of CCM matrixes */
    ot_eis_vi_color_matrix_param ccm_tab[OT_EIS_VI_CCM_MATRIX_NUM];
} ot_eis_vi_color_matrix_auto_attr;

typedef struct {
    ot_eis_op_mode op_type;
    ot_eis_vi_color_matrix_manual_attr manual_attr;
    ot_eis_vi_color_matrix_auto_attr   auto_attr;
} ot_eis_vi_color_matrix_attr;

typedef struct {
    ot_u8 sat;           /* RW; Range: [0, 0xFF]; Format:8.0; set the manual saturation of CCM */
} ot_eis_vi_saturation_manual_attr;

typedef struct {
    ot_u8 sat[OT_EIS_VI_AUTO_ISO_NUM];           /* RW; Range: [0, 0xFF]; should be decreased with increasing ISO */
} ot_eis_vi_saturation_auto_attr;

typedef struct {
    ot_eis_op_mode op_type;
    ot_eis_vi_saturation_manual_attr manual_attr;
    ot_eis_vi_saturation_auto_attr   auto_attr;
} ot_eis_vi_saturation_attr;

typedef struct {
    ot_u16 r_gain;               /* R; Range: [0x0, 0xFFF]; Format:4.8;AWB result of R color channel */
    ot_u16 gr_gain;              /* R; Range: [0x0, 0xFFF]; Format:4.8; AWB result of Gr color channel */
    ot_u16 gb_gain;              /* R; Range: [0x0, 0xFFF]; Format:4.8; AWB result of Gb color channel */
    ot_u16 b_gain;               /* R; Range: [0x0, 0xFFF]; Format:4.8; AWB result of B color channel */
    ot_u16 saturation;           /* R; Range: [0x0, 0xFF];Format:8.0;Current saturation */
    ot_u16 color_temp;           /* R; Range: [0x0, 0xFFFF];Format:16.0;Detect color temperature, maybe out of color
                                    cemeprature range */
    ot_u16 ccm[OT_EIS_VI_CCM_MATRIX_SIZE]; /* R; Range: [0x0, 0xFFFF];Format:16.0;Current color correction matrix */

    ot_u16 ls0_ct;               /* R; Range: [0x0, 0xFFFF];Format:16.0;color tempearture of primary light source */
    ot_u16 ls1_ct;               /* R; Range: [0x0, 0xFFFF];Format:16.0;color tempearture of secondary light source */
    ot_u16 ls0_area;             /* R; Range: [0x0, 0xFF];Format:8.0;area of primary light source */
    ot_u16 ls1_area;             /* R; Range: [0x0, 0xFF];Format:8.0;area of secondary light source */
    ot_u8  multi_degree;         /* R; Range: [0x0, 0xFF];0 means uniform light source, larger value means multi
                                    light source */
    ot_u16 active_shift;         /* R; Range;[0x0,0xFF] */
    ot_u32 first_stable_time;    /* R, Range: [0x0, 0xFFFFFFFF];Format:32.0;AWB first stable frame number */
    ot_eis_vi_awb_scene_mode_status scene_status;  /* R; indoor or outdoor status */
    ot_s16 bv;                                   /* R; Range;[-32768, 32767]; Bv value */
} ot_eis_vi_awb_info;

/* mode switch attr */
typedef struct {
    ot_u32 night_exp_thresh;      /* RW; Range;[0, 4294967295]; if exp > night_exp_thresh, awb switch to night mode */
    ot_u32 skin_tune_exp_thresh;  /* RW; Range;[0, 4294967295];if exp < night_exp_thresh, awb enlarge zone_radius for better behavior against skin */

    ot_u16 out_switch_luma_thresh; /* RW; Range;[0, 65535]; luma thresh for switching mode to outdoor */
    ot_u16 out_switch_lowct_num;   /* RW; Range;[0, 1024]; statistic the number of light source during 1500k~3000k, for more accurate switching mode to outdoor */
} ot_eis_vi_awb_mode_switch_attr;

#define OT_EIS_VI_AWB_RP_LS_NUM     8

/* plck weight attr */
typedef struct {
    ot_bool enable;         /* RW; Range;[0, 1]; Bv value */
    ot_u8 strength;         /* RW; Range;[1, 255]; Bv value */
} ot_eis_vi_awb_plck_weight_attr;

/* reproject light source */
typedef struct {
    ot_bool enable;                                         /* RW; Range;[0, 1]; */
    ot_u8 rp_valid[OT_EIS_VI_AWB_RP_LS_NUM];                /* RW; Range;[0, 1]; status of the reproject zone */
    ot_u16 rp_tgt_rgain[OT_EIS_VI_AWB_RP_LS_NUM];           /* RW; Range;[0, 4095]; target project rgain */
    ot_u16 rp_tgt_bgain[OT_EIS_VI_AWB_RP_LS_NUM];           /* RW; Range;[0, 4095]; target project bgaine */
    ot_u8 rp_ls_radius[OT_EIS_VI_AWB_RP_LS_NUM];            /* RW; Range;[0, 255]; radius of the reproject zone */
    ot_u16 rp_zone_rgain[OT_EIS_VI_AWB_RP_LS_NUM];          /* RW; Range;[0, 4095]; rgain of reproject zone */
    ot_u16 rp_zone_bgain[OT_EIS_VI_AWB_RP_LS_NUM];          /* RW; Range;[0, 4095]; bgain of reproject zone */
    ot_u8 rp_boundry_ratio[OT_EIS_VI_AWB_RP_LS_NUM];        /* RW; Range;[0, 255]; center boundry of reproject zone */
    ot_u32 rp_exp_range_left[OT_EIS_VI_AWB_RP_LS_NUM];      /* RW; Range;[0, 4294967295]; exp left thresh */
    ot_u32 rp_exp_range_right[OT_EIS_VI_AWB_RP_LS_NUM];     /* RW; Range;[0, 4294967295]; exp right thresh */
    ot_u16 rp_ct_range_left[OT_EIS_VI_AWB_RP_LS_NUM];       /* RW; Range;[0, 65535]; ct left thresh*/
    ot_u16 rp_ct_range_right[OT_EIS_VI_AWB_RP_LS_NUM];      /* RW; Range;[0, 65535]; ct right thresh */
} ot_eis_vi_awb_ls_reproject_attr;

/* define anti blue interfere attr */
typedef struct {
    ot_u16 blue_discard_ct_thresh_indoor;
    ot_u16 blue_discard_ct_thresh_outdoor;
    ot_u16 blue_moment_ctn_ct_thresh;
    ot_u16 blue_moment_auto_outdoor_num_thresh;
    ot_u8 blue_moment_sft;
} ot_eis_vi_awb_anti_blue_interfere_attr;

#define OT_EIS_VI_AWB_PREAWB_LUT_SIZE 6
#define OT_EIS_VI_AWB_PREAWB_RATIO_PREC 16

/* pre-awb attr */
typedef struct {
    ot_bool enable;                                             /* RW; Range;[0, 1]; */
    ot_bool awb_reverse_en;                                     /* RW; Range;[0, 1]; */
    ot_u16 color_temp_array[OT_EIS_VI_AWB_PREAWB_LUT_SIZE];     /* RW; Range;[0, 65535]; calib preawb ct array */
    ot_u32 ratio_r_array[OT_EIS_VI_AWB_PREAWB_LUT_SIZE];        /* RW; Range;[0, 1048575]; calib preawb rgain ratio array */
    ot_u32 ratio_b_array[OT_EIS_VI_AWB_PREAWB_LUT_SIZE];        /* RW; Range;[0, 1048575]; calib preawb bgain ratio array */
} ot_eis_vi_awb_preawb_para;

/* pre-awb info */
typedef struct {
    ot_bool enable;     /* RW; Range;[0, 1]; */
    ot_u32 rg_ratio;    /* RW; Range;[0, 1048575]; preawb rgain ratio */
    ot_u32 bg_ratio;    /* RW; Range;[0, 1048575]; preawb bgain ratio */
} ot_eis_vi_awb_preawb_info;

/* sensor's interface to awb */
typedef struct {
    ot_u16 color_temp;              /* RW;  range:[2000,10000]; format:16.0; the current color temperature */
    ot_u16 ccm[OT_EIS_VI_CCM_MATRIX_SIZE];    /* RW;  range: [0x0, 0xFFFF]; format:8.8;
                                       CCM matrixes for different color temperature */
} ot_eis_vi_awb_ccm_tab;

typedef struct {
    ot_u16  ccm_tab_num;                   /* RW;  range: [0x3, 0x7]; format:16.0; the number of CCM matrixes */
    ot_eis_vi_awb_ccm_tab ccm_tab[OT_EIS_VI_CCM_MATRIX_NUM];
} ot_eis_vi_awb_ccm;

typedef struct {
    ot_bool valid;

    ot_u8   saturation[OT_EIS_VI_AUTO_ISO_NUM];   /* RW;adjust saturation, different iso with different saturation */
} ot_eis_vi_awb_agc_table;

typedef struct {
    ot_u16  wb_ref_temp;       /* RW;reference color temperature for WB  */
    ot_u16  gain_offset[OT_EIS_VI_BAYER_CHN_NUM];  /* RW; gain offset for white balance */
    ot_s32  wb_para[OT_EIS_VI_AWB_CURVE_PARA_NUM];      /* RW; parameter for wb curve,p1,p2,q1,a1,b1,c1 */

    ot_u16  golden_rgain;      /* rgain for the golden sample */
    ot_u16  golden_bgain;      /* bgain for the golden sample */
    ot_u16  sample_rgain;      /* rgain for the current sample */
    ot_u16  sample_bgain;      /* bgain for the current sample */
    ot_eis_vi_awb_agc_table agc_tbl;
    ot_eis_vi_awb_ccm ccm;
    ot_u16    init_rgain;           /* init WB gain */
    ot_u16    init_ggain;
    ot_u16    init_bgain;
    ot_u8     awb_run_interval;       /* RW;AWB run interval */
    ot_u16    init_ccm[OT_EIS_VI_CCM_MATRIX_SIZE];

    // preawb attr
    ot_eis_vi_awb_preawb_para preawb_attr;
} ot_eis_vi_awb_sensor_default;

typedef struct {
    ot_s32 (*pfn_sns_get_awb_default)(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_sensor_default *awb_sns_dft);
    ot_s32 (*pfn_sns_set_awb_preawb)(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_preawb_info *preawb_info, ot_eis_vi_awb_info *awb_info);
} ot_eis_vi_awb_sensor_register_func;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif
