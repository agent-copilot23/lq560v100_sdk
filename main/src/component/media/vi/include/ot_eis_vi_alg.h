#ifndef OT_EIS_VI_ALG_H
#define OT_EIS_VI_ALG_H

#include "ot_eis_type.h"
#include "ot_eis_video.h"
#include "ot_eis_vi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef ot_s32 ot_slave_dev_id;

/* MACRO DEFINITION */
#define OT_EIS_VI_STRIPING_MAX_NUM         3
#define OT_EIS_VI_STATIC_DP_COUNT_NORMAL   2048
#define OT_EIS_VI_STATIC_DP_COUNT_MAX      (OT_EIS_VI_STATIC_DP_COUNT_NORMAL * OT_EIS_VI_STRIPING_MAX_NUM)

#define OT_EIS_VI_AE_ZONE_ROW              15
#define OT_EIS_VI_AE_ZONE_COLUMN           17
#define OT_EIS_VI_MG_ZONE_ROW              15
#define OT_EIS_VI_MG_ZONE_COLUMN           17

#define OT_EIS_VI_AWB_ZONE_ORIG_ROW        32
#define OT_EIS_VI_AWB_ZONE_ORIG_COLUMN     32
#define OT_EIS_VI_AWB_ZONE_NUM             (OT_EIS_VI_AWB_ZONE_ORIG_ROW * OT_EIS_VI_AWB_ZONE_ORIG_COLUMN)
#define OT_EIS_VI_AWB_ZONE_STITCH_MAX      (OT_EIS_VI_AWB_ZONE_NUM * OT_EIS_VI_MAX_STITCH_NUM)

#define OT_EIS_VI_GAMMA_NODE_NUM           1025

#define OT_EIS_VI_AUTO_ISO_NUM             16
#define OT_EIS_VI_SHARPEN_LUMA_NUM         32
#define OT_EIS_VI_SHARPEN_GAIN_NUM         32

#define OT_EIS_VI_MAX_SNS_REGS             32
#define OT_EIS_VI_LSC_GRID_COL             33
#define OT_EIS_VI_LSC_GRID_ROW             33
#define OT_EIS_VI_LSC_GRID_POINTS          (OT_EIS_VI_LSC_GRID_COL * OT_EIS_VI_LSC_GRID_ROW)

#define OT_EIS_VI_MLSC_X_HALF_GRID_NUM       ((OT_EIS_VI_LSC_GRID_COL - 1) / 2)
#define OT_EIS_VI_MLSC_Y_HALF_GRID_NUM       ((OT_EIS_VI_LSC_GRID_ROW - 1) / 2)
#define OT_EIS_VI_MLSC_GAIN_LUT_NUM          2
#define OT_EIS_VI_ACS_LIGHT_NUM              32
#define OT_EIS_VI_ACS_CHN_NUM                2

#define OT_EIS_VI_LBLC_GRID_COL                      33
#define OT_EIS_VI_LBLC_GRID_ROW                      33
#define OT_EIS_VI_LBLC_GRID_POINTS                   (OT_EIS_VI_LBLC_GRID_COL * OT_EIS_VI_LBLC_GRID_ROW)

#define OT_EIS_VI_WDR_MAX_FRAME_NUM        4

#define OT_EIS_VI_CSC_DC_NUM               3
#define OT_EIS_VI_CSC_COEF_NUM             9

#define OT_EIS_VI_AI_MAX_STEP_FNO_NUM      1024

#define OT_EIS_VI_BAYERNR_LUT_LENGTH       33
#define OT_EIS_VI_BAYERNR_LUT_LENGTH1      32
#define OT_EIS_VI_RGB_CHN_NUM              3

#define OT_EIS_VI_BAYER_CHN_NUM            4

#define OT_EIS_VI_DRC_LMIX_NODE_NUM        33

#define OT_EIS_VI_DEHAZE_LUT_SIZE          256
#define OT_EIS_VI_WDR_RATIO_NUM            10

#define OT_EIS_VI_CAC_EXP_RATIO_NUM        16
#define OT_EIS_VI_CAC_THR_NUM              2
#define OT_EIS_VI_CAC_CURVE_NUM            3

#define OT_EIS_VI_BSHP_CURVE_NUM           8
#define OT_EIS_VI_BSHP_THD_NUM             2

#define OT_EIS_VI_DO_NOT_NEED_SWITCH_IMAGEMODE (-2)

#define OT_EIS_VI_CCM_MATRIX_SIZE          9
#define OT_EIS_VI_CCM_MATRIX_NUM           7

#define OT_EIS_VI_EXP_RATIO_NUM            3

#define OT_ERR_EIS_VI_NOT_INIT         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_NO_INIT)
#define OT_ERR_EIS_VI_MEM_NOT_INIT     OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_ALG_MEM_NOT_INIT)
#define OT_ERR_EIS_VI_SNS_UNREGISTER   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_SNS_UNREGISTER)
#define OT_ERR_EIS_VI_INVALID_ADDR     OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_INVALID_ADDR)
#define OT_ERR_EIS_VI_NO_INT           OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_NO_INT)
#define OT_ERR_EIS_VI_ALG_NOT_INIT     OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_ALG_NOT_INIT)

typedef struct {
    ot_eis_img_size        sns_size;          /* RO; width and height of the image output from the sensor */
    ot_float               frame_rate;        /* RW; Range: [0, 0xFFFF]; for frame rate */
    ot_eis_vi_bayer_format bayer_format;      /* RW; Range:[0,3] ;Format:2.0;the format of the input bayer image */
    ot_eis_wdr_mode        wdr_mode;          /* RO; WDR mode */
} ot_eis_vi_public_attr;

/* Slave mode sensor sync signal generate module */
typedef struct {
    union {
        struct {
            ot_u32  bit16_reserved      :  16;
            ot_u32  bit_h_inv           :  1;
            ot_u32  bit_v_inv           :  1;
            ot_u32  bit12_reserved      :  12;
            ot_u32  bit_h_enable        :  1;
            ot_u32  bit_v_enable        :  1;
        } bits;
        ot_u32 bytes;
    } cfg;

    ot_u32  vs_time;
    ot_u32  hs_time;
    ot_u32  vs_cyc;
    ot_u32  hs_cyc;
    ot_u32  hs_dly_cyc;
    ot_u32  slave_mode_time;
} ot_eis_vi_slave_sns_sync_attr;

typedef union {
    ot_u64  key;
    struct {
        ot_u64  bit_bypass_isp_d_gain        : 1;   /* RW;[0] */
        ot_u64  bit_bypass_anti_false_color  : 1;   /* RW;[1] */
        ot_u64  bit_bypass_crosstalk_removal : 1;   /* RW;[2] */
        ot_u64  bit_bypass_dpc            : 1;   /* RW;[3] */
        ot_u64  bit_bypass_nr             : 1;   /* RW;[4] */
        ot_u64  bit_bypass_dehaze         : 1;   /* RW;[5] */
        ot_u64  bit_bypass_wb_gain        : 1;   /* RW;[6] */
        ot_u64  bit_bypass_mesh_shading   : 1;   /* RW;[7] */
        ot_u64  bit_bypass_drc            : 1;   /* RW;[8] */
        ot_u64  bit_bypass_demosaic       : 1;   /* RW;[9] */
        ot_u64  bit_bypass_color_matrix   : 1;   /* RW;[10] */
        ot_u64  bit_bypass_gamma          : 1;   /* RW;[11] */
        ot_u64  bit_bypass_fswdr          : 1;   /* RW;[12] */
        ot_u64  bit_bypass_ca             : 1;   /* RW;[13] */
        ot_u64  bit_bypass_csc            : 1;   /* RW;[14] */
        ot_u64  bit_bypass_radial_crop    : 1;   /* RW;[15] */
        ot_u64  bit_bypass_sharpen        : 1;   /* RW;[16] */
        ot_u64  bit_bypass_bayer_sharpen  : 1;   /* RW;[17] */
        ot_u64  bit_bypass_cac            : 1;   /* RW;[18]; */
        ot_u64  bit2_chn_select           : 2;   /* RW;[19:20] */
        ot_u64  bit_bypass_ldci           : 1;   /* RW;[21] */
        ot_u64  bit_bypass_pregamma       : 1;   /* RW;[22], not support */
        ot_u64  bit_bypass_ae_stat_fe     : 1;   /* RW;[23] */
        ot_u64  bit_bypass_ae_stat_be     : 1;   /* RW;[24] */
        ot_u64  bit_bypass_mg_stat        : 1;   /* RW;[25] */
        ot_u64  bit_bypass_af_stat_fe     : 1;   /* RW;[26] */
        ot_u64  bit_bypass_af_stat_be     : 1;   /* RW;[27] */
        ot_u64  bit_bypass_awb_stat       : 1;   /* RW;[28] */
        ot_u64  bit_bypass_clut           : 1;   /* RW;[29] */
        ot_u64  bit_bypass_rgbir          : 1;   /* RW;[30], not support */
        ot_u64  bit_bypass_lblc           : 1;   /* RW;[31] */
        ot_u64  bit_reserved32            : 32;  /* H; [32:63] */
    };
} ot_eis_vi_alg_bypass_ctrl;

/*
 * Defines the WDR merge mode.
 * 0 = WDR mode is used
 * 1 = Fusion mode is used
 */
typedef enum {
    OT_EIS_VI_MERGE_WDR_MODE      = 0,
    OT_EIS_VI_MERGE_FUSION_MODE   = 1,
    OT_EIS_VI_MERGE_BUTT
} ot_eis_vi_wdr_merge_mode;

typedef struct {
    ot_u8 md_thr_low_gain;    /* RW;Range:[0x0,0xFF];Format:8.0 */
    ot_u8 md_thr_hig_gain;    /* RW;Range:[0x0,0xFF];Format:8.0 */
} ot_eis_vi_wdr_md_manual_attr;

typedef struct {
    ot_u8 md_thr_low_gain[OT_EIS_VI_WDR_RATIO_NUM][OT_EIS_VI_AUTO_ISO_NUM];    /* RW;Range:[0x0,0xFF];Format:8.0 */
    ot_u8 md_thr_hig_gain[OT_EIS_VI_WDR_RATIO_NUM][OT_EIS_VI_AUTO_ISO_NUM];    /* RW;Range:[0x0,0xFF];Format:8.0 */
} ot_eis_vi_wdr_md_auto_attr;

typedef struct {
    ot_bool short_expo_chk;     /* RW;Range:[0x0,0x1];Format:1.0; */
    ot_u16  short_check_threshold;   /* RW;Range:[0x0,0xFFF];Format:12.0 */
    ot_bool md_ref_flicker;
    ot_u8   mdt_still_threshold;     /* RW;Range:[0x0,0xFE];Format:8.0 */
    ot_u8   mdt_full_threshold;      /* RW;Range:[0x0,0xFE];Format:8.0 */
    ot_u8   mdt_long_blend;    /* RW;Range:[0x0,0xFE] */
    ot_eis_op_mode op_type;
    ot_eis_vi_wdr_md_manual_attr manual_attr;
    ot_eis_vi_wdr_md_auto_attr   auto_attr;
} ot_eis_vi_wdr_mdt_attr;

typedef struct {
    ot_bool motion_comp;        /* RW;Range:[0, 0x1];Format:1.0; enable/disable motion compensation */
    ot_u16  short_threshold;    /* RW;Range:[0, 0xFFF];Format:12.0; data above this threshold will be taken from
                                   short exposure only. */
    ot_u16  long_threshold;     /* RW;Range:[0, 0xFFF];Format:12.0; limited range:[0x0, short_threshold],
                                   data below this threshold will be taken from long exposure only. */
    ot_bool force_long;         /* RW;Range:[0, 1];Format:1.0; enable/disable force long */
    ot_u16  force_long_low_threshold; /* RW;Range:[0, 0xFFF];Format:12.0; data above this threshold will force to choose
                                         long frame only */
    ot_u16  force_long_hig_threshold; /* RW;Range:[0, 0xFFF];Format:12.0; data below this threshold will force to choose
                                         long frame only */
    ot_eis_vi_wdr_mdt_attr wdr_mdt;
} ot_eis_vi_wdr_combine_attr;

typedef struct {
    ot_bool fusion_blend_en;
    ot_u8   fusion_blend_wgt; /* RW; Range: [0, 16] */
    ot_u16  fusion_threshold[OT_EIS_VI_WDR_MAX_FRAME_NUM]; /* RW; Range: [0x0, 0xFFF] */
    ot_bool  fusion_force_gray_en;
    ot_u8   fusion_force_blend_threshold; /* RW; Range: [0, 15] */
} ot_eis_vi_fusion_attr;

typedef struct {
    ot_eis_vi_wdr_merge_mode   wdr_merge_mode;
    ot_eis_vi_wdr_combine_attr wdr_combine_attr;
    ot_eis_vi_fusion_attr      fusion_attr;
} ot_eis_vi_wdr_attr;

typedef struct {
    ot_u8 asymmetry;    /* RW; Range:[  1,  30]; Parameter0 of DRC asymmetry tone mapping curve */
    ot_u8 second_pole;  /* RW; Range:[150, 210]; Parameter1 of DRC asymmetry tone mapping curve */
    ot_u8 stretch;      /* RW; Range:[ 30,  60]; Parameter2 of DRC asymmetry tone mapping curve */
    ot_u8 compress;     /* RW; Range:[100, 200]; Parameter3 of DRC asymmetry tone mapping curve */
} ot_eis_vi_drc_asymmetry_curve_attr;

typedef struct {
    ot_u16  strength;  /* RW; Range:[0x0, 0x3FF]; DRC manual strength */
} ot_eis_vi_drc_manual_attr;

typedef struct {
    ot_u16  strength;      /* RW; Range:[0x0, 0x3FF]; Base strength for strength adaptation in auto mode.
                              The actual DRC strength is calculated automatically by firmware:
                              1) linear mode, auto strength = f1(strength, histogram)
                              2) sensor built-in WDR mode: auto strength = f2(strength, histogram)
                              3) FS WDR mode: auto strength = f3(strength, exp_ratio) */
    ot_u16  strength_max;  /* RW; Range:[0x0, 0x3FF]; uppper limit of firmware calculated strength */
    ot_u16  strength_min;  /* RW; Range:[0x0, 0x3FF]; lower limit of firmware calculated strength */
} ot_eis_vi_drc_auto_attr;

/* DRC curve type: 0 = Asymmetry curve, 1 = User-defined curve */
typedef enum {
    OT_EIS_VI_DRC_CURVE_ASYMMETRY = 0x0,
    OT_EIS_VI_DRC_CURVE_USER,
    OT_EIS_VI_DRC_CURVE_BUTT
} ot_eis_vi_drc_curve_select;

#define OT_EIS_VI_DRC_CC_NODE_NUM                    33
#define OT_EIS_VI_DRC_TM_NODE_NUM                    200
typedef struct {
    ot_bool enable;
    ot_eis_vi_drc_curve_select curve_select; /* RW; Range:[0x0, 0x1]; Tone mapping curve selection */
    ot_u8  purple_reduction_strength; /* RW; Range:[0x0, 0x80]; Purple detection and reduction strength */
    ot_u8  bright_gain_limit; /* RW; Range:[0x0, 0xF]; Bright area gain limit */
    ot_u8  bright_gain_limit_step; /* RW; Range:[0x0, 0xF]; Bright area gain high limit step */
    ot_u8  dark_gain_limit_luma; /* RW; Range:[0x0, 0x85]; Dark area luma gain limit */
    ot_u8  dark_gain_limit_chroma; /* RW; Range:[0x0, 0x85]; Dark area chroma gain limit */
    ot_u8  contrast_ctrl; /* RW; Range:[0x0, 0xF]; Format:4.0; Contrast control, smaller value yields better contrast */
    ot_u8  rim_reduction_strength;  /* RW; Range:[0x0, 0x40]; Gradient reversal (rim) reduction strength */
    ot_u8  rim_reduction_threshold; /* RW; Range:[0x0, 0x80]; Gradient reversal (rim) reduction threshold */
    ot_u16 color_correction_lut[OT_EIS_VI_DRC_CC_NODE_NUM]; /* RW; Range:[0x0, 0x400];
                                                           LUT of color correction coefficients */
    ot_u16 tone_mapping_value[OT_EIS_VI_DRC_TM_NODE_NUM]; /* RW; Range:[0x0, 0xffff]; LUT of user-defined curve */
    ot_u8  spatial_filter_coef; /* RW; Range:[0x0, 0x5]; Spatial filter coefficients */
    ot_u8  range_filter_coef; /* RW; Range:[0x0, 0xA]; Range filter coefficients */
    ot_u8  detail_adjust_coef; /* RW; Range:[0x0, 0xF]; Detail adjust coefficient */
    ot_u8  local_mixing_bright[OT_EIS_VI_DRC_LMIX_NODE_NUM]; /* RW; Range:[0x0, 0x80];
                                                             LUT of enhancement coefficients for positive details */
    ot_u8  local_mixing_dark[OT_EIS_VI_DRC_LMIX_NODE_NUM]; /* RW; Range:[0x0, 0x80];
                                                           LUT of enhancement coefficients for negative details */
    ot_u8  high_saturation_color_ctrl; /* RW; Range:[0x0, 0xF]; High saturation color control */
    ot_u8  global_color_ctrl; /* RW; Range:[0x0, 0xF]; global color control */

    ot_bool shoot_reduction_en; /* RW; Range:[0x0, 0x1]; shoot reduction enable */

    ot_eis_op_mode op_type;
    ot_eis_vi_drc_manual_attr manual_attr;
    ot_eis_vi_drc_auto_attr   auto_attr;
    ot_eis_vi_drc_asymmetry_curve_attr asymmetry_curve;
} ot_eis_vi_drc_attr;

#define OT_EIS_VI_EXPANDER_NODE_NUM      257
#define OT_EIS_VI_EXPANDER_POINT_NUM_MAX (OT_EIS_VI_EXPANDER_NODE_NUM - 1)

typedef struct {
    ot_bool  enable;
    ot_u8    bit_depth_in;        /* RW;Range:[0xC, 0x14];Format:5.0;The Bit depth of input.
                                     bit_depth_in must be an even number */
    ot_u8    bit_depth_out;       /* RW;Range:[0xC, 0x14];Format:5.0;The Bit depth of output.
                                     bit_depth_out must be an even number */
    ot_u16   knee_point_num;      /* Range:[1, 256]; Format:9.0; the number of knee point */
    ot_eis_img_point knee_point_coord[OT_EIS_VI_EXPANDER_POINT_NUM_MAX]; /* RW; Range: x:[0x0, 0x100], y:[0x0, 0x100000];
                                                                 The coordinate of knee point.
                                                                 The coordinate must be monotonically increasing */
} ot_eis_vi_expander_attr;

typedef struct {
    ot_u8 wgt;   /* RW;Range: [0x0, 0xFF];
                    Format:1.7;Weight of Gaussian distribution */
    ot_u8 sigma; /* RW;Range: [0x1, 0xFF];Format:0.8;Sigma of Gaussian distribution */
    ot_u8 mean;  /* RW;Range: [0x0, 0xFF];Format:0.8;Mean of Gaussian distribution */
} ot_eis_vi_ldci_gauss_coef_attr;

typedef struct {
    ot_eis_vi_ldci_gauss_coef_attr  he_pos_wgt;
    ot_eis_vi_ldci_gauss_coef_attr  he_neg_wgt;
} ot_eis_vi_ldci_he_wgt_attr;

typedef struct {
    ot_eis_vi_ldci_he_wgt_attr  he_wgt;
    ot_u16                   blc_ctrl;  /* RW;Range: [0x0, 0x1FF];Format:9.0;Restrain dark region */
} ot_eis_vi_ldci_manual_attr;

typedef struct {
    ot_eis_vi_ldci_he_wgt_attr  he_wgt[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16                   blc_ctrl[OT_EIS_VI_AUTO_ISO_NUM]; /* RW;Range: [0x0, 0x1FF];Format:9.0;auto mode,
                                                               Restrain dark region */
} ot_eis_vi_ldci_auto_attr;

typedef struct {
    ot_bool                 enable;          /* RW;Range:[0x0,0x1];Format:1.0; */
    ot_u8                   gauss_lpf_sigma; /* RW;Range: [0x1, 0xFF];Format:0.8;
                                                Coefficient of Gaussian low-pass filter */
    ot_eis_op_mode              op_type;
    ot_eis_vi_ldci_manual_attr manual_attr;
    ot_eis_vi_ldci_auto_attr   auto_attr;
    ot_u16                  tpr_incr_coef;   /* RW;Range: [0x0, 0x100];Format:0.9;
                                                Increase Coefficient of temporal filter */
    ot_u16                  tpr_decr_coef;   /* RW;Range: [0x0, 0x100];Format:0.9;
                                                Decrease Coefficient of temporal filter */
} ot_eis_vi_ldci_attr;

// ldcihlc
typedef struct {
    ot_u8                   strength;        /* RW:Range: [0x0, 0xFF]                   */
} ot_eis_vi_ldcihlc_manual_attr;

typedef struct {
    ot_u8                   strength[OT_EIS_VI_AUTO_ISO_NUM];
} ot_eis_vi_ldcihlc_auto_attr;

typedef struct {
    ot_bool                         enable;
    ot_u8                           threshold;      /* RW:Range: [0x20, 0xFF] */

    ot_eis_op_mode           op_type;
    ot_eis_vi_ldcihlc_manual_attr   manual_attr;
    ot_eis_vi_ldcihlc_auto_attr     auto_attr;

} ot_eis_vi_ldcihlc_attr;

/*
  defines CA type
  0 = enable Ca module
  1 = enable Cp module
 */
typedef enum {
    OT_EIS_VI_CA_ENABLE = 0x0,
    OT_EIS_VI_CP_ENABLE,
    OT_EIS_VI_CA_BUTT
} ot_eis_vi_ca_type;

#define OT_EIS_VI_CA_YRATIO_LUT_LENGTH               256
typedef struct {
    ot_u32  y_ratio_lut[OT_EIS_VI_CA_YRATIO_LUT_LENGTH];  /* RW;Range:[0,2047];Format:1.11 */
    ot_s32  iso_ratio[OT_EIS_VI_AUTO_ISO_NUM];            /* RW;Range:[0,2047];Format:1.10 */
    ot_u32  y_sat_lut[OT_EIS_VI_CA_YRATIO_LUT_LENGTH];  /* RW;Range:[0,2047];Format:1.11 */
} ot_eis_vi_ca_lut;

typedef struct {
    ot_u8   cp_lut_y[OT_EIS_VI_CA_YRATIO_LUT_LENGTH]; /* RW;Range:[0,255];Format:8.0; */
    ot_u8   cp_lut_u[OT_EIS_VI_CA_YRATIO_LUT_LENGTH]; /* RW;Range:[0,255];Format:8.0; */
    ot_u8   cp_lut_v[OT_EIS_VI_CA_YRATIO_LUT_LENGTH]; /* RW;Range:[0,255];Format:8.0; */
} ot_eis_vi_cp_lut;

typedef struct {
    ot_bool enable;             /* RW;Range:[0x0,0x1];Format:1.0 */
    ot_eis_vi_ca_type ca_cp_en;
    ot_eis_vi_ca_lut  ca;
    ot_eis_vi_cp_lut  cp;
} ot_eis_vi_ca_attr;

/* CSC */
typedef struct {
    ot_s16 in_dc[OT_EIS_VI_CSC_DC_NUM]; /* RW; Range:[-1024, 1023]; Format: 11.0;
                                            Input dc component for csc matrix */
    ot_s16 out_dc[OT_EIS_VI_CSC_DC_NUM]; /* RW; Range:[-1024, 1023]; Format: 11.0;
                                             Output dc component for csc matrix */
    ot_s16 coef[OT_EIS_VI_CSC_COEF_NUM];  /* RW; Range:[-4096, 4095]; Format: 5.10; 3x3 coefficients for csc matrix */
} ot_eis_vi_csc_matrix;

typedef struct {
    ot_bool enable;                 /* RW; Range:[0, 1];Format:1.0; Enable/Disable CSC Function */
    ot_eis_img_color_space color_gamut; /* RW; Range: [0, 3]; Color gamut type; Not Support COLOR_GAMUT_BT2020 */
    ot_u8   hue;              /* RW; Range:[0, 100];Format:8.0; Csc Hue Value */
    ot_u8   luma;             /* RW; Range:[0, 100];Format:8.0; Csc Luma Value */
    ot_u8   contr;            /* RW; Range:[0, 100];Format:8.0; Csc Contrast Value */
    ot_u8   satu;             /* RW; Range:[0, 100];Format:8.0; Csc Saturation Value */
    ot_bool limited_range_en; /* RW; Range: [0x0, 0x1];
                                 Enable/Disable: Enable Limited range output mode(default full range output) */
    ot_bool ext_csc_en;       /* RW; Range: [0x0, 0x1]; Enable/Disable: Enable extended luma range */
    ot_bool ct_mode_en;       /* RW; Range: [0x0, 0x1]; Enable/Disable: Enable ct mode */
    ot_eis_vi_csc_matrix csc_matrix;     /* RW; Color Space Conversion matrix */
} ot_eis_vi_csc_attr;

/* CLUT */
typedef struct {
    ot_bool enable; /* RW; Range:[0, 1];Format:1.0; Enable/Disable CLUT Function */
    ot_u32  gain_r; /* RW; Range:[0, 4095];Format:12.0 */
    ot_u32  gain_g; /* RW; Range:[0, 4095];Format:12.0 */
    ot_u32  gain_b; /* RW; Range:[0, 4095];Format:12.0 */
} ot_eis_vi_clut_attr;

/* CLUT LUT */
#define OT_EIS_VI_CLUT_LUT_LENGTH                    5508
typedef struct {
    ot_u32 lut[OT_EIS_VI_CLUT_LUT_LENGTH];  /* RW; Range: [0, 1073741823] */
} ot_eis_vi_clut_lut;

/*
 * Defines the type of static defect pixel calibration
 * 0 = bright defect pixel calibration
 * 1 = dark defect pixel calibration
 */
typedef enum {
    OT_EIS_VI_STATIC_DP_BRIGHT = 0x0,
    OT_EIS_VI_STATIC_DP_DARK,
    OT_EIS_VI_STATIC_DP_BUTT
} ot_eis_vi_static_dp_type;

/*
 * Defines the DPC correction or detection status
 * 0 = initial status, no calibration
 * 1 = The static defect pixel calibration ends normally
 * 2 = The static defect pixel calibration ends due to timeout.
 */
typedef enum {
    OT_EIS_VI_DPC_CALIBRATE_STATE_INIT     = 0,
    OT_EIS_VI_DPC_CALIBRATE_STATE_SUCCESS  = 1,
    OT_EIS_VI_DPC_CALIBRATE_STATE_TIMEOUT  = 2,
    OT_EIS_VI_DPC_CALIBRATE_STATE_BUTT
} ot_eis_vi_dpc_calibrate_status;

typedef struct {
    ot_bool enable_detect;                /* RW; Range: [0, 1];Format 1.0;Set 'OT_TRUE'to start static defect-pixel
                                             calibration, and firmware will set 'OT_FALSE' when finished */
    ot_eis_vi_static_dp_type static_dp_type; /* RW; Range: [0, 1];Format 1.0;Select static bright/dark defect-pixel
                                             calibration */
    ot_u8  start_thresh;  /* RW; Range: [1, 255]; Format 8.0;Start threshold for static defect-pixel calibraiton */
    ot_u16 count_max;     /* RW; Range:(0, 6144];
                           Format 14.0; limited Range: (0, OT_EIS_VI_STATIC_DP_COUNT_NORMAL*BlkNum],
                           Limit of max number of static defect-pixel calibraiton */
    ot_u16 count_min;     /* RW; Range: [0, 6144);Format 14.0; limited Range: [0, count_max),
                             Limit of min number of static defect-pixel calibraiton */
    ot_u16 time_limit;    /* RW; Range: [0x0, 1600];Format 11.0;Time limit for static defect-pixel calibraiton */

    ot_u32 table[OT_EIS_VI_STATIC_DP_COUNT_MAX];  /* R; Range: [0, 0x1FFF1FFF];Format 29.0;
                                           Static defect-pixel calibraiton table,
                                           0~12 bits represents the X coordinate of the defect pixel,
                                           16~28 bits represent the Y coordinate of the defect pixel */
    ot_u8  finish_thresh;   /* R; Range: [0, 255];Format 8.0; Finish threshold for static defect-pixel calibraiton */
    ot_u16 count;          /* R; Range:[0, 6144];Format 14.0;
                              Finish number for static defect-pixel calibraiton */
    ot_eis_vi_dpc_calibrate_status status;  /* R; Range: [0, 2];Format 2.0;Status of static defect-pixel calibraiton */
} ot_eis_vi_dpc_static_calibrate_attr;

typedef struct {
    ot_bool enable;       /* RW; Range: [0, 1];Format 1.0;Enable/disable the static defect-pixel module */
    ot_u16  bright_count; /* RW; Range:[0, 6144];Format 14.0;
                             limited Range: [0, OT_EIS_VI_STATIC_DP_COUNT_NORMAL*BlkNum],
                             When used as input(W), indicate the number of static bright defect pixels;
                             As output(R),indicate the number of static bright and dark defect pixels */
    ot_u16  dark_count;   /* RW; Range:[0, 6144];Format 14.0;
                             limited Range: [0, OT_EIS_VI_STATIC_DP_COUNT_NORMAL*BlkNum],
                             When used as input(W), indicate the number of static dark defect pixels;
                             As output(R), invalid value 0. */
    ot_u32  bright_table[OT_EIS_VI_STATIC_DP_COUNT_MAX]; /* RW; Range: [0x0, 0x1FFF1FFF];Format 29.0;
                                                  0~12 bits represents the X coordinate of the defect pixel,
                                                  16~28 bits represent the Y coordinate of the defect pixel
                                                  Notice: When used as input(W), indicate static bright defect pixels
                                                  table; As output(R), indicate static bright and dark defect pixels
                                                  table. */

    ot_u32  dark_table[OT_EIS_VI_STATIC_DP_COUNT_MAX]; /* RW; Range: [0x0, 0x1FFF1FFF];Format 29.0;
                                                0~12 bits represents the X coordinate of the defect pixel,
                                                16~28 bits represent the Y coordinate of the defect pixel
                                                Notice: When used as input(W), indicate static dark defect pixels table;
                                                As output(R), invalid value */
    ot_bool show;                  /* RW; Range: [0, 1];Format 1.0;RW;highlight static defect pixel */
} ot_eis_vi_dpc_static_cor_attr;

typedef struct {
    ot_u8  strength;           /* RW; Range: [0, 255];Format:8.0;Dynamic DPC strength. */
    ot_u8  blend_ratio;        /* RW; Range: [0, 128];Format:1.7;Blending ratio required for DPC */
} ot_eis_vi_dpc_dynamic_manual_attr;

typedef struct {
    ot_u8  strength[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; Range: [0, 255];Format:8.0;Dynamic DPC strength. */
    ot_u8  blend_ratio[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 128];Format:1.7;Blending ratio required for DPC */
} ot_eis_vi_dpc_dynamic_auto_attr;

typedef struct {
    ot_bool sup_twinkle_en;     /* RW; Range: [0, 1];Format 1.0;Enable/disable the twinkle suppression  module */
    ot_s8   soft_thr;           /* RW; Range: [0, 127];Format s8.0;twinkle suppression threshold */
    ot_u8   soft_slope;         /* RW; Range: [0, 255];Format 8.0;Correction controlling parameter of the pixels whose
                                   deviation is smaller than soft_thr. */
    ot_eis_op_mode op_type;         /* RW; Range: [0, 1];Format 1.0;Working mode of dynamic DPC */
    ot_eis_vi_dpc_dynamic_manual_attr manual_attr;
    ot_eis_vi_dpc_dynamic_auto_attr   auto_attr;
    ot_u8 bright_strength;      /* RW; Range: [0, 127];Format 8.0, local DPC strength for bright area */
    ot_u8 dark_strength;        /* RW; Range: [0, 127];Format 8.0, local DPC strength for dark area */
} ot_eis_vi_dpc_frame_dynamic_attr;

typedef struct {
    ot_bool enable;             /* RW; Range: [0, 1];Format 1.0;Enable/disable the dynamic defect-pixel module */
    ot_eis_vi_dpc_frame_dynamic_attr frame_dynamic[OT_EIS_VI_WDR_MAX_FRAME_NUM]; /* dynamic attr for short/long frames */
} ot_eis_vi_dpc_dynamic_cor_attr;

typedef struct {
    ot_bool enable;    /* RW; Range:[0, 1];Format:1.0; OT_TRUE: enable lblc; OT_FALSE: disable lblc */
    ot_u16  strength;  /* RW; Range:[0, 1023]; The strength of the lblc correction */
} ot_eis_vi_lblc_attr;

typedef struct {
    ot_u16  offset_r;  /* RW; Range:[0, 16383]; average value of the R channel Mesh BLC. */
    ot_u16  offset_gr; /* RW; Range:[0, 16383]; average value of the Gr channel Mesh BLC. */
    ot_u16  offset_gb; /* RW; Range:[0, 16383]; average value of the Gb channel Mesh BLC. */
    ot_u16  offset_b;  /* RW; Range:[0, 16383]; average value of the B channel Mesh BLC. */
    ot_u16  mesh_blc_r[OT_EIS_VI_LBLC_GRID_POINTS];  /* RW; Range:[0, 16383]; Member used to store
                                                    the calibration data of the R channel Mesh BLC. */
    ot_u16  mesh_blc_gr[OT_EIS_VI_LBLC_GRID_POINTS]; /* RW; Range:[0, 16383]; Member used to store
                                                    the calibration data of the Gr channel Mesh BLC. */
    ot_u16  mesh_blc_gb[OT_EIS_VI_LBLC_GRID_POINTS]; /* RW; Range:[0, 16383]; Member used to store
                                                    the calibration data of the Gb channel Mesh BLC. */
    ot_u16  mesh_blc_b[OT_EIS_VI_LBLC_GRID_POINTS];  /* RW; Range:[0, 16383]; Member used to store
                                                    the calibration data of the B channel Mesh BLC. */
} ot_eis_vi_lblc_lut_attr;

typedef struct {
    ot_bool enable;        /* RW; Range:[0, 1];Format:1.0; OT_TRUE: enable lsc; OT_FALSE: disable lsc */
    ot_u16  mesh_strength; /* RW; Range:[0, 16383]; The strength of the mesh shading correction */
    ot_u16  blend_ratio; /* RW; Range:[0, 256];Format:9.0; the blendratio of the two mesh gain lookup-table */
} ot_eis_vi_shading_attr;

typedef struct {
    ot_u16  r_gain[OT_EIS_VI_LSC_GRID_POINTS];  /* RW; Range:[0, 1023];Member used to store the calibration data of the
                                                R channel required for LSC. */
    ot_u16  gr_gain[OT_EIS_VI_LSC_GRID_POINTS]; /* RW; Range:[0, 1023];Member used to store the calibration data of the
                                                Gr channel required for LSC. */
    ot_u16  gb_gain[OT_EIS_VI_LSC_GRID_POINTS]; /* RW; Range:[0, 1023];Member used to store the calibration data of the
                                                Gb channel required for LSC. */
    ot_u16  b_gain[OT_EIS_VI_LSC_GRID_POINTS];  /* RW; Range:[0, 1023];Member used to store the calibration data of the
                                                B channel required for LSC. */
} ot_eis_vi_shading_gain_lut;

typedef struct {
    ot_u8   mesh_scale;        /* RW; Range:[0, 7];Format:3.0;Three bit value that selects the scale and precision for
                                  the 10 bit gain coefficients stored in mesh tables */
    ot_u16  x_grid_width[OT_EIS_VI_MLSC_X_HALF_GRID_NUM]; /* RW; Range:[4, 255],
                                                          Member used to store the width data of each GRID partition */
    ot_u16  y_grid_width[OT_EIS_VI_MLSC_Y_HALF_GRID_NUM]; /* RW; Range:[4, 255];
                                                        Member used to store the height data of each GRID partition */
    ot_eis_vi_shading_gain_lut lsc_gain_lut[OT_EIS_VI_MLSC_GAIN_LUT_NUM];
} ot_eis_vi_shading_lut_attr;

typedef struct {
    ot_bool enable;       /* RW; Range:[0,1];Format:1.0;Acs Enable */
    ot_u16  y_strength;   /* RW; Range:[0,256];Format:8.0;Acs correction strength for y shading */
    ot_u16  run_interval; /* RW; Range:[1,255];Format:16.0;Acs Run Interval */
    ot_bool lock_enable;  /* RW; Range:[0,1];Format:1.0;Lock Gain Lut Enable */
} ot_eis_vi_acs_attr;

typedef struct {
    ot_u16  dering_strength[OT_EIS_VI_AUTO_ISO_NUM];        /* RW; Range:[0, 1023];Format:10.0; dering strength. */
    ot_u8   dering_thresh[OT_EIS_VI_AUTO_ISO_NUM];          /* RW; Range:[0, 255];Format:8.0; dering threshold. */
    ot_u8   dering_static_strength[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 32];Format:6.0; dering static strength */
    ot_u8   dering_motion_strength[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 32];Format:6.0; dering moving strength */
} ot_eis_vi_nr_dering_auto_attr;

typedef struct {
    ot_u16  dering_strength;        /* RW; Range:[0, 1023];Format:10.0; dering strength. */
    ot_u8   dering_thresh;          /* RW; Range:[0, 255];Format:8.0; dering threshold. */
    ot_u8   dering_static_strength; /* RW; Range:[0, 32];Format:6.0; dering static strength. */
    ot_u8   dering_motion_strength; /* RW; Range:[0, 32];Format:6.0; dering moving strength. */
} ot_eis_vi_nr_dering_manual_attr;

typedef struct {
    ot_eis_vi_nr_dering_auto_attr   dering_auto;
    ot_eis_vi_nr_dering_manual_attr dering_manual;
} ot_eis_vi_nr_dering_attr;

typedef struct {
    ot_u16 sfm0_coarse_strength[OT_EIS_VI_BAYER_CHN_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 864];Format:10.0;
                                                                                sfm0 filter coarse strength */
    ot_u8  sfm0_detail_prot[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range:[0, 31];Format:5.0; sfm0 filter detail protect rate */
    ot_u16 sfm1_strength[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; Range:[0, 1023];Format:16.0; sfm1 filter strength. */
    ot_u8  sfm1_adp_strength[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 16];Format:5.0; sfm1 filter adaptive strength. */
    ot_u8  sfm6_strength[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; Range:[0, 64];Format:7.0; sfm6 hybrid filter strength. */
    ot_u8  sfm7_strength[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; Range:[0, 64];Format:7.0; sfm7 hybrid filter strength. */
    ot_u8  sth[OT_EIS_VI_AUTO_ISO_NUM];               /* RW; Range:[0, 255];Format:8.0; hybrid filter threshold. */
    ot_u8  fine_strength[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; Range:[0, 128];Format:8.0; denoise rate. */
    ot_u16 coring_wgt[OT_EIS_VI_AUTO_ISO_NUM]; /* RW;Range:[0, 3200];Format:12.0;Strength of reserving the random noise. */
    ot_u8  coring_mot_ratio[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range:[0, 63];Format:6.0; coring on moving area. */
} ot_eis_vi_nr_snr_auto_attr;

typedef struct {
    ot_u16    sfm0_coarse_strength[OT_EIS_VI_BAYER_CHN_NUM]; /* RW; Range:[0,864];Format:10.0; sfm0 coarse strength */
    ot_u8     sfm0_detail_prot;  /* RW; Range:[0, 31];Format:5.0; sfm0 filter detail protect rate */
    ot_u16    sfm1_strength;     /* RW; Range:[0, 1023];Format:16.0; sfm1 filter strength. */
    ot_u8     sfm1_adp_strength; /* RW; Range:[0, 16];Format:5.0; sfm1 filter adaptive strength. */
    ot_u8     sfm6_strength;     /* RW; Range:[0, 64];Format:7.0; sfm6 hybrid filter strength. */
    ot_u8     sfm7_strength;     /* RW; Range:[0, 64];Format:7.0; sfm7 hybrid filter strength. */
    ot_u8     sth;               /* RW; Range:[0, 255];Format:8.0; hybrid filter threshold. */
    ot_u8     fine_strength;     /* RW; Range:[0, 128];Format:8.0; denoise rate. */
    ot_u16    coring_wgt;        /* RW;Range:[0, 3200];Format:12.0;Strength of reserving the random noise. */
    ot_u8     coring_mot_ratio;  /* RW; Range:[0, 63];Format:6.0; coring on moving area. */
} ot_eis_vi_nr_snr_manual_attr;

typedef struct {
    ot_eis_vi_nr_snr_auto_attr   snr_auto;
    ot_eis_vi_nr_snr_manual_attr snr_manual;
} ot_eis_vi_nr_snr_attr_v0;

typedef enum {
    OT_EIS_VI_NR_SNR_V0,
} ot_eis_vi_nr_snr_mode;

typedef struct {
    ot_eis_vi_nr_snr_mode snr_version;
    union {
        ot_eis_vi_nr_snr_attr_v0 snr_attr; /* AUTO:ot_eis_vi_nr_snr_mode:OT_EIS_VI_NR_SNR_V0 */
    };
} ot_eis_vi_nr_snr_attr;

typedef struct {
    ot_u8   md_mode[OT_EIS_VI_AUTO_ISO_NUM];       /* RW; Range:[0, 1];Format:1.0; motion detection win size. */
    ot_u8   md_size_ratio[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 32];Format:6.0; motion detection size blend ratio. */
    ot_u8   md_anti_flicker_strength[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 64];Format:7.0;
                                                               motion detection anti flicker strength. */
    ot_u8   md_static_ratio[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 64];Format:7.0; motion detection static area ratio */
    ot_u8   md_motion_ratio[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 64];Format:7.0; motion detection motion area ratio */
    ot_u8   md_static_fine_strength[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range:[0, 255];Format:8.0;
                                                              motion detection background tnr strength  */
    ot_u8   tfs[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 255];Format:8.0; tnr absolute strength. */
    ot_bool user_define_md[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range:[0, 1];Format:1.0; user define motion detection mode. */
    ot_s16  user_define_slope[OT_EIS_VI_AUTO_ISO_NUM];        /* RW; Range:[-32768, 32767];Format:8.8; user define motion
                                                              detection strength change with luma. */
    ot_u16  user_define_dark_thresh[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range:[0, 65535];Format:8.8; user define motion
                                                              detection strength in dark area. */
    ot_u8   user_define_color_thresh[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 64];Format:7.0; user define motion
                                                              detection strength in color area. */
    ot_u8   sfr_r[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 128];Format:8.0; sfr rate r */
    ot_u8   sfr_g[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 128];Format:8.0; sfr rate g */
    ot_u8   sfr_b[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 128];Format:8.0; sfr rate b */
} ot_eis_vi_nr_tnr_auto_attr;

typedef struct {
    ot_u8   md_mode;                  /* RW; Range:[0, 1];Format:1.0; motion detection win size. */
    ot_u8   md_size_ratio;            /* RW; Range:[0, 32];Format:6.0; motion detection size blend ratio. */
    ot_u8   md_anti_flicker_strength; /* RW; Range:[0, 64];Format:7.0; motion detection anti flicker strength. */
    ot_u8   md_static_ratio;          /* RW; Range:[0, 64];Format:7.0; motion detection static area ratio */
    ot_u8   md_motion_ratio;          /* RW; Range:[0, 64];Format:7.0; motion detection motion area ratio */
    ot_u8   md_static_fine_strength;  /* RW; Range:[0, 255];Format:8.0; motion detection background tnr strength  */
    ot_u8   tfs;                      /* RW; Range:[0, 255];Format:8.0; tnr absolute strength. */
    ot_bool user_define_md;           /* RW; Range:[0, 1];Format:1.0; user define motion detection mode. */
    ot_s16  user_define_slope;        /* RW; Range:[-32768, 32767];Format:8.8; user define motion detection strength
                                         change with luma. */
    ot_u16  user_define_dark_thresh;  /* RW; Range:[0, 65535];Format:8.8; user define motion detection strength in
                                         dark area. */
    ot_u8   user_define_color_thresh; /* RW; Range:[0, 64];Format:7.0; user define motion detection strength in
                                         color area. */
    ot_u8   sfr_r;                    /* RW; Range:[0, 128];Format:8.0; sfr rate r */
    ot_u8   sfr_g;                    /* RW; Range:[0, 128];Format:8.0; sfr rate g */
    ot_u8   sfr_b;                    /* RW; Range:[0, 128];Format:8.0; sfr rate b */
} ot_eis_vi_nr_tnr_manual_attr;

typedef struct {
    ot_eis_vi_nr_tnr_auto_attr   tnr_auto;
    ot_eis_vi_nr_tnr_manual_attr tnr_manual;
} ot_eis_vi_nr_tnr_attr;

typedef struct {
    ot_u8 snr_sfm0_wdr_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM];    /* RW; Range:[0, 64];Format:7.0; wdr sfm0 strength */
    ot_u8 snr_sfm0_fusion_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM]; /* RW; Range:[0, 64];Format:7.0; fusion sfm0 strength */
    ot_u8 snr_wdr_sfm6_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM];    /* RW; Range:[0, 64];Format:7.0; wdr sfm6 strength */
    ot_u8 snr_wdr_sfm7_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM];    /* RW; Range:[0, 64];Format:7.0; wdr sfm7 strength */
    ot_u8 snr_fusion_sfm6_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM]; /* RW; Range:[0, 64];Format:7.0; fusion sfm6 strength */
    ot_u8 snr_fusion_sfm7_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM]; /* RW; Range:[0, 64];Format:7.0; fusion sfm7 strength */
    ot_u8 md_wdr_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM];          /* RW; Range:[0, 64];Format:7.0; wdr md tnr strength */
    ot_u8 md_fusion_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM];       /* RW; Range:[0, 64];Format:7.0; fusion md tnr strength */
} ot_eis_vi_nr_wdr_attr;

typedef struct {
    ot_bool    enable;     /* RW; Range:[0, 1];Format:1.0; bayernr enable  */
    ot_eis_op_mode op_type;    /* RW; Range:[0, 1];Format:1.0; bayernr operation mode  */
    ot_bool    tnr_en;     /* RW; Range:[0, 1];Format:1.0; bayernr tnr enable */
    ot_bool    lsc_nr_en;  /* RW; Range:[0, 1];Format:1.0; lsc nr enable. */
    ot_u8      lsc_ratio1; /* RW; Range:[0, 15];Format:4.0; lsc nr ratio1. */
    ot_u16     coring_ratio[OT_EIS_VI_BAYERNR_LUT_LENGTH]; /* RW;Range:[0, 1023];Format:10.0;
                                                            Strength of reserving the random noise according to luma. */
    ot_u16     mix_gain[OT_EIS_VI_BAYERNR_LUT_LENGTH1];    /* RW;Range:[0, 128];Format:8.0; mixing rate based on luma. */

    ot_eis_vi_nr_snr_attr   snr_cfg;
    ot_eis_vi_nr_tnr_attr   tnr_cfg;
    ot_eis_vi_nr_wdr_attr   wdr_cfg;
    ot_eis_vi_nr_dering_attr   dering_cfg;
} ot_eis_vi_nr_attr;

/*
  Defines the type of the gamma curve
  0 = Default curve
  1 = Default BT.709 curve (Gamma Curve)
  2 = Default SMPTE.2084 curve (PQ Curve), Not Support
  3 = User defined Gamma curve, LUT must be correct
*/
typedef enum {
    OT_EIS_VI_GAMMA_CURVE_DEFAULT = 0x0,
    OT_EIS_VI_GAMMA_CURVE_SRGB,
    OT_EIS_VI_GAMMA_CURVE_HDR,    /* Not Support */
    OT_EIS_VI_GAMMA_CURVE_USER_DEFINE,
    OT_EIS_VI_GAMMA_CURVE_BUTT
} ot_eis_vi_gamma_curve_type;

typedef struct {
    ot_bool   enable;                     /* RW; Range:[0, 1]; Format:1.0;Enable/Disable Gamma Function */
    ot_u16    table[OT_EIS_VI_GAMMA_NODE_NUM];    /* RW; Range:[0, 4095]; Format:12.0;Gamma LUT nodes value */

    ot_eis_vi_gamma_curve_type curve_type;    /* RW; Range:[0, 3]; Format:2.0;Gamma curve type */
} ot_eis_vi_gamma_attr;

typedef struct {
    ot_u8  luma_wgt[OT_EIS_VI_SHARPEN_LUMA_NUM]; /* RW; Range: [0, 31];
                                               Format:0.5;Adjust the sharpen strength according to luma.
                                               Sharpen strength will be weaker when it decrease. */
    ot_u16 texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM]; /* RW; Range: [0, 4095]; Format:7.5;Undirectional sharpen
                                                         strength for texture and detail enhancement */
    ot_u16 edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM];    /* RW; Range: [0, 4095]; Format:7.5;
                                           Directional sharpen strength for edge enhancement */
    ot_u16 texture_freq;       /* RW; Range: [0, 4095];Format:6.6; Texture frequency adjustment.
                                  Texture and detail will be finer when it increase */
    ot_u16 edge_freq;          /* RW; Range: [0, 4095];Format:6.6; Edge frequency adjustment.
                                  Edge will be narrower and thiner when it increase */
    ot_u8  over_shoot;         /* RW; Range: [0, 127]; Format:7.0;u8OvershootAmt */
    ot_u8  under_shoot;        /* RW; Range: [0, 127]; Format:7.0;u8UndershootAmt */

    ot_u16 motion_texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM]; /* RW; Range: [0, 4095];Format:7.5.
                                                                Not support */
    ot_u16 motion_edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM]; /* RW; Range: [0, 4095]; Format:7.5.
                                                             Not support */
    ot_u16 motion_texture_freq; /* RW; Range: [0, 4095]; Format:6.6. Not support */
    ot_u16 motion_edge_freq;   /* RW; Range: [0, 4095]; Format:6.6. Not support */
    ot_u8  motion_over_shoot;  /* RW; Range: [0, 127];  Format:7.0. Not support */
    ot_u8  motion_under_shoot; /* RW; Range: [0, 127];  Format:7.0. Not support */

    ot_u8  shoot_sup_strength; /* RW; Range: [0, 255]; Format:8.0;overshoot and undershoot suppression strength, the
                                  amplitude and width of shoot will be decrease when shootSupSt increase */
    ot_u8  shoot_sup_adj;      /* RW; Range: [0, 15]; Format:4.0;overshoot and undershoot suppression adjusting,
                                  adjust the edge shoot suppression strength */
    ot_u8  detail_ctrl;        /* RW; Range: [0, 255]; Format:8.0;Different sharpen strength for detail and edge.
                                  When it is bigger than 128, detail sharpen strength will be stronger than edge */
    ot_u8  detail_ctrl_threshold; /* RW; Range: [0, 255]; Format:8.0; The threshold of DetailCtrl, it is used to
                                  distinguish detail and edge. */
    ot_u8  edge_filt_strength;      /* RW; Range: [0, 63]; Format:6.0;The strength of edge filtering. */
    ot_u8  edge_filt_max_cap;  /* RW; Range: [0, 47]; Format:6.0;The max capacity of edge filtering. */
    ot_u8  r_gain;             /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Red Area */
    ot_u8  g_gain;             /* RW; Range: [0, 255]; Format:8.0; Sharpen Gain for Green Area */
    ot_u8  b_gain;             /* RW; Range: [0, 31];   Format:5.0;Sharpen Gain for Blue Area */
    ot_u8  skin_gain;          /* RW; Range: [0, 31]; Format:5.0;Sharpen Gain for Skin Area */
    ot_u16 max_sharp_gain;     /* RW; Range: [0, 0x7FF]; Format:8.3; Maximum sharpen gain */
} ot_eis_vi_sharpen_manual_attr;

typedef struct {
    ot_u8  luma_wgt[OT_EIS_VI_SHARPEN_LUMA_NUM][OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range:[0, 31]; Format:0.5;
                                                                       Adjust the sharpen strength according to luma.
                                                                    Sharpen strength will be weaker when it decrease */
    ot_u16 texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range: [0, 4095]; Format:7.5;
                                                                          Undirectional sharpen strength for texture
                                                                          and detail enhancement */
    ot_u16 edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 4095]; Format:7.5;
                                                                   Directional sharpen strength for edge enhancement */
    ot_u16 texture_freq[OT_EIS_VI_AUTO_ISO_NUM];   /* RW; Range: [0, 4095]; Format:6.6;Texture frequency adjustment.
                                                   Texture and detail will be finer when it increase */
    ot_u16 edge_freq[OT_EIS_VI_AUTO_ISO_NUM];      /* RW; Range: [0, 4095]; Format:6.6;Edge frequency adjustment.
                                                   Edge will be narrower and thiner when it increase */
    ot_u8  over_shoot[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; Range: [0, 127];  Format:7.0 */
    ot_u8  under_shoot[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; Range: [0, 127];  Format:7.0 */

    ot_u16 motion_texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 4095];
                                                                             Format:7.5. Not support */
    ot_u16 motion_edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 4095]; Format:7.5.
                                                                                  Not support  */
    ot_u16 motion_texture_freq[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 4095]; Format:6.6. Not support */
    ot_u16 motion_edge_freq[OT_EIS_VI_AUTO_ISO_NUM];   /* RW; Range: [0, 4095]; Format:6.6. Not support */
    ot_u8  motion_over_shoot[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range: [0, 127];  Format:7.0. Not support */
    ot_u8  motion_under_shoot[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 127];  Format:7.0. Not support */

    ot_u8  shoot_sup_strength[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range: [0, 255]; Format:8.0;overshoot and undershoot
                                                   suppression strength, the amplitude and width of shoot will be
                                                   decrease when shootSupSt increase */
    ot_u8  shoot_sup_adj[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range: [0, 15]; Format:4.0;overshoot and undershoot
                                                   suppression adjusting, adjust the edge shoot suppression strength */
    ot_u8  detail_ctrl[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; Range: [0, 255]; Format:8.0;Different sharpen strength for
                                                   detail and edge. When it is bigger than 128, detail sharpen
                                                   strength will be stronger than edge. */
    ot_u8  detail_ctrl_threshold[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 255]; Format:8.0; The threshold of DetailCtrl,
                                                    it is used to distinguish detail and edge. */
    ot_u8  edge_filt_strength[OT_EIS_VI_AUTO_ISO_NUM];   /* RW; Range: [0, 63]; Format:6.0;
                                                    The strength of edge filtering. */
    ot_u8  edge_filt_max_cap[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 47]; Format:6.0;
                                                      The max capacity of edge filtering. */
    ot_u8  r_gain[OT_EIS_VI_AUTO_ISO_NUM];        /* RW; Range: [0, 31]; Format:5.0; Sharpen Gain for Red Area */
    ot_u8  g_gain[OT_EIS_VI_AUTO_ISO_NUM];        /* RW; Range: [0, 255]; Format:8.0; Sharpen Gain for Green Area */
    ot_u8  b_gain[OT_EIS_VI_AUTO_ISO_NUM];        /* RW; Range: [0, 31]; Format:5.0; Sharpen Gain for Blue Area */
    ot_u8  skin_gain[OT_EIS_VI_AUTO_ISO_NUM];      /* RW; Range: [0, 31]; Format:5.0;Sharpen Gain for Skin Area */
    ot_u16 max_sharp_gain[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 0x7FF]; Format:8.3; Maximum sharpen gain */
} ot_eis_vi_sharpen_auto_attr;

typedef enum {
    OT_EIS_VI_SHARPEN_NORMAL = 0,
    OT_EIS_VI_SHARPEN_OFF,
    OT_EIS_VI_SHARPEN_DIFF_NORM,
    OT_EIS_VI_SHARPEN_DIFF,
    OT_EIS_VI_SHARPEN_TOTAL,
    OT_EIS_VI_SHARPEN_TEXTURE,
    OT_EIS_VI_SHARPEN_EDGE,
    OT_EIS_VI_SHARPEN_TOTAL_FREQ,
    OT_EIS_VI_SHARPEN_TEXTURE_FREQ,
    OT_EIS_VI_SHARPEN_EDGE_FREQ,
    OT_EIS_VI_SHARPEN_BUTT
} ot_eis_vi_sharpen_detail_map;

typedef struct {
    ot_bool enable;    /* RW; Range:[0, 1]; Format:1.0;Enable/Disable sharpen module */
    ot_bool motion_en;  /* RW; Range:[0, 1]; Format:1.0;Enable/Disable motion sharpen module.
                           Not support */
    ot_u8   motion_threshold0; /* RW; Range: [0, 15]; Format:8.0. Not support */
    ot_u8   motion_threshold1; /* RW; Range: [0, 15]; Format:8.0. Not support */
    ot_u16  motion_gain0;      /* RW; Range: [0, 256]; Format:16.0. Not support */
    ot_u16  motion_gain1;      /* RW; Range: [0, 256]; Format:16.0. Not support */
    ot_u8   skin_umin;   /* RW; Range: [0, 255];  Format:8.0; U min value of the range of skin area */
    ot_u8   skin_vmin;   /* RW; Range: [0, 255];  Format:8.0; V min value of the range of skin area */
    ot_u8   skin_umax;   /* RW; Range: [0, 255];  Format:8.0; U max value of the range of skin area */
    ot_u8   skin_vmax;   /* RW; Range: [0, 255];  Format:8.0; V max value of the range of skin area */
    ot_eis_op_mode op_type;
    ot_eis_vi_sharpen_detail_map  detail_map;
    ot_eis_vi_sharpen_manual_attr manual_attr;
    ot_eis_vi_sharpen_auto_attr   auto_attr;
} ot_eis_vi_sharpen_attr;

/* Crosstalk Removal */
typedef struct {
    ot_bool  enable;      /* RW; Range: [0, 1];Format:1.0;Enable/disable the crosstalk removal module */
    ot_u8    slope;       /* RW; Range: [0, 12]; Crosstalk slope value. */
    ot_u8    sensi_slope; /* RW; Range: [0, 12]; Crosstalk sensitivity. */
    ot_u16   sensi_threshold; /* RW; Range: [0, 4095]; Crosstalk sensitivity threshold. */
    ot_u16   strength[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; Range: [0,256];Crosstalk strength value. */
    ot_u16   threshold[OT_EIS_VI_AUTO_ISO_NUM];   /* RW; Range: [0, 4095]; Crosstalk threshold. */
    ot_u8    filter_mode[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 31];Format:5.0; Crosstalk filter mode. */
    ot_u16   np_offset[OT_EIS_VI_AUTO_ISO_NUM];   /* RW; Range: [512, 4095]; Not support */
} ot_eis_vi_cr_attr;

typedef struct {
    ot_u8    threshold;     /* RW;Range:[0x0,0x20];Format:6.0;Threshold for antifalsecolor */
    ot_u8    strength;      /* RW;Range:[0x0,0x1F];Format:5.0;Strength of antifalsecolor */
} ot_eis_vi_anti_false_color_manual_attr;

typedef struct {
    ot_u8  threshold[OT_EIS_VI_AUTO_ISO_NUM];    /* RW;Range:[0x0,0x20];Format:6.0;Threshold for antifalsecolor */
    ot_u8  strength[OT_EIS_VI_AUTO_ISO_NUM];     /* RW;Range:[0x0,0x1F];Format:5.0;Strength of antifalsecolor */
} ot_eis_vi_anti_false_color_auto_attr;

typedef struct {
    ot_bool  enable;                               /* RW;Range:[0x0,0x1];Format:1.0; AntiFalseColor Enable */
    ot_eis_op_mode op_type;
    ot_eis_vi_anti_false_color_manual_attr manual_attr;
    ot_eis_vi_anti_false_color_auto_attr   auto_attr;
} ot_eis_vi_anti_false_color_attr;

typedef struct {
    ot_u8   nddm_strength;           /* RW; Range:[0x0, 0xFF]; Format:4.4; Non-direction strength */
    ot_u8   nddm_mf_detail_strength; /* RW; Range:[0x0, 0x7f];
                                          Format:3.4; Non-direction medium frequent detail enhance  strength */
    ot_u8   nddm_hf_detail_strength; /* RW; Range:[0x0, 0x10]; Format:2.2;
                                          Non-direction high frequent detail enhance strength */
    ot_u8   detail_smooth_range;     /* RW; Range:[0x1, 0x7]; Format:4.0; Detail smooth range */
    ot_u8   color_noise_f_threshold; /* RW; Range:[0x0,0xff]; Format:8.0; Threshold of color noise cancel */
    ot_u8   color_noise_f_strength;  /* RW; Range:[0x0, 0x8]; Format:4.0; Strength of color noise cancel */
    ot_u8   color_noise_y_threshold; /* RW; Range:[0x0, 0xF];
                                      Format:8.0; Range of color denoise luma, related to luminance and saturation */
    ot_u8   color_noise_y_strength;  /* RW; Range:[0x0, 0x3F]; Format:8.0;Format:8.0; Strength of color denoise luma */
} ot_eis_vi_demosaic_manual_attr;

typedef struct {
    ot_u8   nddm_strength[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; Range:[0x0, 0xFF]; Format:4.4; Non-direction strength */
    ot_u8   nddm_mf_detail_strength[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0x7f]; Format:3.4;
                                                               Non-direction medium frequent detail enhance strength */
    ot_u8   nddm_hf_detail_strength[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0x10]; Format:2.2; Non-direction
                                                               high frequent detail enhance strength */
    ot_u8   detail_smooth_range[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; Range:[0x1, 0x7]; Format:4.0; Detail smooth range */
    ot_u8   color_noise_f_threshold[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0xff]; Format:8.0;
                                                               Threshold of color noise cancel */
    ot_u8   color_noise_f_strength[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range:[0x0, 0x8]; Format:4.0; Strength of color
                                                               noise cancel */
    ot_u8   color_noise_y_threshold[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range:[0x0, 0xF]; Format:8.0;
                                                    Range of color denoise luma, related to luminance and saturation */
    ot_u8   color_noise_y_strength[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; Range:[0x0, 0x3F]; Format:8.0;Format:8.0;
                                                               Strength of color denoise luma */
} ot_eis_vi_demosaic_auto_attr;

typedef struct {
    ot_bool enable;        /* RW; Range:[0, 1]; Format:1.0;Enable/Disable demosaic module */
    ot_eis_op_mode op_type;
    ot_u16     ai_detail_strength;    /* RW; Range:[0x0, 0x1FF]; Format:9.0; ai detail strength */
    ot_eis_vi_demosaic_manual_attr manual_attr;
    ot_eis_vi_demosaic_auto_attr   auto_attr;
} ot_eis_vi_demosaic_attr;

/* Defines the attributes of the black level */
typedef enum {
    OT_EIS_VI_BLACK_LEVEL_MODE_AUTO    = 0,
    OT_EIS_VI_BLACK_LEVEL_MODE_MANUAL  = 1,
    OT_EIS_VI_BLACK_LEVEL_MODE_BUTT
} ot_eis_vi_black_level_mode;

typedef struct {
    ot_u16 black_level[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_BAYER_CHN_NUM]; /* RW; Range: [0x0, 0x3FFF];Format:14.0;
                                                 sensor Black level values that correspond to the black levels of the
                                                 R,Gr, Gb, and B components respectively. */
} ot_eis_vi_black_level_manual_attr;

typedef struct {
    ot_bool  user_black_level_en;
    ot_u16   user_black_level[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_BAYER_CHN_NUM];  /* RW; Range: [0x0, 0x3FFF];Format:14.0;
                                                 user define Black level values that correspond to the black levels of
                                                 the R,Gr, Gb, and B components respectively. */
    ot_eis_vi_black_level_mode         black_level_mode;
    ot_eis_vi_black_level_manual_attr  manual_attr;
} ot_eis_vi_black_level_attr;

/*
  Defines the FPN removal type
  0 = Frame mode correction
  1 = Line mode correction
 */
typedef enum {
    OT_EIS_VI_FPN_TYPE_FRAME = 0,
    OT_EIS_VI_FPN_TYPE_LINE = 1,
    OT_EIS_VI_FPN_TYPE_BUTT
} ot_eis_vi_fpn_type;

/*
  Defines the FPN frame output mode
  0: the pixel value algin with the least significant bit (right-most bit)
  1: the pixel value algin with the most significant bit (left-most bit)
 */
typedef enum {
    OT_EIS_VI_FPN_OUT_MODE_NORM = 0,
    OT_EIS_VI_FPN_OUT_MODE_HIGH,
    OT_EIS_VI_FPN_OUT_MODE_BUTT
} ot_eis_vi_fpn_out_mode;

/* Defines the information about calibrated black frames */
typedef struct {
    ot_u32              iso;             /* RW;Range:[0x64,0xFFFFFFFF];Format:32.0;FPN CALIBRATE ISO */
    ot_u32              offset[OT_EIS_VI_STRIPING_MAX_NUM]; /* RW;Range:[0, 0x3FFF];Format:14.0;
                                                          FPN frame u32Offset (agv pixel value),
                                                          only higher 12bits valid  */
    ot_u32              frm_size;         /* RW;FPN frame size (exactly frm size or compress len) */
    ot_eis_video_frame fpn_frame;         /* FPN frame info, 8bpp,10bpp,12bpp,16bpp. Compression or not */
} ot_eis_vi_fpn_frame_info;

/* Defines the calibration attribute of the FPN removal module */
typedef struct {
    ot_u32                 threshold;  /* RW;Range:[1,0xFFF];Format:12.0;pix value > threshold means defective pixel */
    ot_u32                 frame_num;  /* RW;Range:[1, 16];Format:5.0;Number of frames to be calibrated.
                                          The value range is {1, 2, 4, 8, 16},that is, the integer exponentiation of 2 */
    ot_eis_vi_fpn_type        fpn_type;   /* frame mode or line mode */
    ot_eis_vi_fpn_frame_info  fpn_cali_frame;
    ot_eis_vi_fpn_out_mode    fpn_mode;   /* FPN frame output mode, for normal use, set to 0 */
} ot_eis_vi_fpn_calibrate_attr;

/* Defines the manual correction attribute of the FPN removal module */
typedef struct {
    ot_u32  strength;         /* RW;Range:[0,1023];Format:10.0;Manual correction strength */
} ot_eis_vi_fpn_manual_attr;

/* Defines the automatic correction attribute of the FPN removal module */
typedef struct {
    ot_u32 strength;          /* RW;Range:[0,1023];Format:10.0;Auto correction strength */
} ot_eis_vi_fpn_auto_attr;

/* Defines the correction attribute of the FPN removal module */
typedef struct {
    ot_bool                   enable;   /* RW;Range:[0,1];Format:1.0; */
    ot_bool                   aibnr_mode; /* RW;Range:[0,1];Format:1.0; */
    ot_eis_op_mode     op_type;  /* manual or auto mode */
    ot_eis_vi_fpn_type        fpn_type;
    ot_eis_vi_fpn_frame_info  fpn_frm_info;
    ot_eis_vi_fpn_manual_attr manual_attr;
    ot_eis_vi_fpn_auto_attr   auto_attr;
} ot_eis_vi_fpn_cor_attr;

/* Defines the manual dehaze attribute */
typedef struct {
    ot_u8 strength;  /* RW;Range:[0,0xFF];Format:8.0;Manual dehaze strength */
} ot_eis_vi_dehaze_manual_attr;

/* Defines the automatic dehaze attribute */
typedef struct {
    ot_u8 strength; /* RW;Range:[0,0xFF];Format:8.0;Weighted coefficient for automatic dehaze strength. */
} ot_eis_vi_dehaze_auto_attr;

/* Defines the dehaze attribute */
typedef struct {
    ot_bool  enable;           /* RW;Range:[0,1];Format:1.0; */
    ot_bool  user_lut_en;    /* RW;Range:[0,1];0:Auto Lut 1:User Lut */
    ot_u8    dehaze_lut[OT_EIS_VI_DEHAZE_LUT_SIZE];
    ot_eis_op_mode        op_type;
    ot_eis_vi_dehaze_manual_attr manual_attr;
    ot_eis_vi_dehaze_auto_attr   auto_attr;
    ot_u16       tmprflt_incr_coef; /* RW, Range: [0x0, 0x80].filter increase coefficient. */
    ot_u16       tmprflt_decr_coef; /* RW, Range: [0x0, 0x80].filter decrease coefficient. */
} ot_eis_vi_dehaze_attr;

/* Defines purple fringing correction attribute */
typedef struct {
    ot_u16  edge_threshold[OT_EIS_VI_CAC_THR_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; range: [0, 4095];
                                                                            format:12.0; cac edge threshold */
    ot_u16  edge_gain[OT_EIS_VI_AUTO_ISO_NUM];           /* RW; range: [0, 1023]; format:10.0; cac edge gain */
    ot_u16  cac_rb_strength[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; range: [0, 31]; format:5.0; cac strength on rb */
    ot_u16  purple_alpha[OT_EIS_VI_AUTO_ISO_NUM];        /* RW; range: [0, 63]; format:6.0; cac purple alpha */
    ot_u16  edge_alpha[OT_EIS_VI_AUTO_ISO_NUM];          /* RW; range: [0, 63]; format:6.0; cac edge  alpha */
    ot_u16  satu_low_threshold[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; range: [0, 4095]; format:12.0; cac purple saturation
                                                         low threshold */
    ot_u16  satu_high_threshold[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; range: [0, 16383]; format:14.0; cac purple saturation
                                                         high threshold */
} ot_eis_vi_cac_acac_auto_attr;

typedef struct {
    ot_u16  edge_threshold[OT_EIS_VI_CAC_THR_NUM];  /* RW; range: [0, 4095]; format:12.0; cac edge threshold */
    ot_u16  edge_gain;              /* RW; range: [0, 1023]; format:10.0; cac edge gain */
    ot_u16  cac_rb_strength;        /* RW; range: [0, 31]; format:5.0; cac strength on rb */
    ot_u16  purple_alpha;           /* RW; range: [0, 63]; format:6.0; cac purple alpha */
    ot_u16  edge_alpha;             /* RW; range: [0, 63]; format:6.0; cac edge alpha */
    ot_u16  satu_low_threshold;     /* RW; range: [0, 4095]; format:12.0; cac purple saturation low threshold */
    ot_u16  satu_high_threshold;    /* RW; range: [0, 16383]; format:14.0; cac purple saturation high threshold */
} ot_eis_vi_cac_acac_manual_attr;

typedef struct {
    ot_eis_vi_cac_acac_manual_attr     acac_manual;
    ot_eis_vi_cac_acac_auto_attr       acac_auto;
} ot_eis_vi_cac_acac_attr;

typedef struct {
    ot_u8   de_purple_cr_strength[OT_EIS_VI_CAC_EXP_RATIO_NUM]; /* RW; range: [0, 8]; format:4.0;
                                                         correction strength of the R channel */
    ot_u8   de_purple_cb_strength[OT_EIS_VI_CAC_EXP_RATIO_NUM]; /* RW; range: [0, 8]; format:4.0;
                                                         correction strength of the B channel */
} ot_eis_vi_cac_lcac_auto_attr;

typedef struct {
    ot_u8   de_purple_cr_strength;  /* RW; range: [0, 8]; format:4.0; correction strength of the R channel */
    ot_u8   de_purple_cb_strength;  /* RW; range: [0, 8]; format:4.0; correction strength of the B channel */
} ot_eis_vi_cac_lcac_manual_attr;

typedef struct {
    ot_u16  purple_detect_range;        /* RW; range: [0, 410]; format:9.0; purple fringing detection scope */
    ot_u16  var_threshold;              /* RW; range: [0, 4095]; format:12.0; edge detection threshold */
    ot_u16  r_detect_threshold[OT_EIS_VI_CAC_CURVE_NUM]; /* RW; range: [0,4095]; format:12.0; component R detection thd */
    ot_u16  g_detect_threshold[OT_EIS_VI_CAC_CURVE_NUM]; /* RW; range: [0,4095]; format:12.0; component G detection thd */
    ot_u16  b_detect_threshold[OT_EIS_VI_CAC_CURVE_NUM]; /* RW; range: [0,4095]; format:12.0; component B detection thd */
    ot_eis_vi_cac_lcac_manual_attr     lcac_manual;
    ot_eis_vi_cac_lcac_auto_attr       lcac_auto;
} ot_eis_vi_cac_lcac_attr;

typedef struct {
    ot_bool enable;                     /* RW; range: [0, 1]; format:1.0; cac enable */
    ot_eis_op_mode op_type;                 /* RW; range: [0, 1]; format:1.0; purple fringing correction working mode */
    ot_u8   detect_mode;                /* RW; range: [0, 2]; format:2.0; cac edge detect mode */
    ot_s16  purple_upper_limit;         /* RW; range: [-2048,2047]; format:s12.0; cac purple high limitation */
    ot_s16  purple_lower_limit;         /* RW; range: [-2048,2047]; format:s12.0; cac purple low limitation */
    ot_eis_vi_cac_acac_attr     acac_cfg;
    ot_eis_vi_cac_lcac_attr     lcac_cfg;
} ot_eis_vi_cac_attr;


typedef struct {
    ot_u8  mf_strength[OT_EIS_VI_BSHP_CURVE_NUM];   /* RW; range:[0, 63];mf_strength. */
    ot_u8  hf_strength[OT_EIS_VI_BSHP_CURVE_NUM];   /* RW; range:[0, 63];hf_strength. */
    ot_u8  dark_strength[OT_EIS_VI_BSHP_CURVE_NUM]; /* RW; range:[0, 63];dark_strength. */
    ot_u8  mf_gain;                /* RW; range:[0, 15];format:4.0; mf gain */
    ot_u8  hf_gain;                /* RW; range:[0, 15];format:4.0; hf gain */
    ot_u8  dark_gain;              /* RW; range:[0, 15];format:4.0; dark gain */
    ot_u16 overshoot;              /* RW; range:[0, 255];format:8.0; over shoot */
    ot_u16 undershoot;             /* RW; range:[0, 255];format:8.0; under shoot */
} ot_eis_vi_bayer_sharpen_manual_attr;

typedef struct {
    ot_u8  mf_strength[OT_EIS_VI_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM];   /* RW; range:[0, 63];mf_strength. */
    ot_u8  hf_strength[OT_EIS_VI_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM];   /* RW; range:[0, 63];hf_strength. */
    ot_u8  dark_strength[OT_EIS_VI_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; range:[0, 63];dark_strength. */
    ot_u8  mf_gain[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; range:[0, 15];format:4.0; mf gain */
    ot_u8  hf_gain[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; range:[0, 15];format:4.0; hf gain */
    ot_u8  dark_gain[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; range:[0, 15];format:4.0; dark gain */
    ot_u16 overshoot[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; range:[0, 255];format:8.0; over shoot */
    ot_u16 undershoot[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; range:[0, 255];format:8.0; under shoot */
} ot_eis_vi_bayer_sharpen_auto_attr;

typedef struct {
    ot_bool enable;                                 /* RW; range:[0, 1]; format:1.0;en/disable bayershrpen module */
    ot_u16  dark_threshold[OT_EIS_VI_BSHP_THD_NUM];    /* RW; range:[0, 4095];format:12.0; dark threshold */
    ot_u16  texture_threshold[OT_EIS_VI_BSHP_THD_NUM]; /* RW; range:[0, 4095];format:12.0; texture threshold */
    ot_eis_op_mode op_type;
    ot_eis_vi_bayer_sharpen_manual_attr manual_attr;
    ot_eis_vi_bayer_sharpen_auto_attr   auto_attr;
} ot_eis_vi_bayer_sharpen_attr;

/* Defines the Radial Crop attribute */
typedef struct {
    ot_bool  enable;       /* RW;Range:[0, 1];Format:1.0; enable/disable ridial crop */
    ot_eis_img_point center_coord; /* RW;the coordinate of central pixel */
    ot_u32   radius;       /* RW;Range:[0, 11586];Format:14.0; when the distance to central pixel is greater than
                              u32Radius, the pixel value becomes 0. */
} ot_eis_vi_rc_attr;

typedef struct {
    ot_u16 texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM]; /* R; range: [0, 4095]; Format:7.5;Undirectional sharpen
                                                         strength for texture and detail enhancement */
    ot_u16 edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM];    /* R; range: [0, 4095]; Format:7.5;Directional sharpen strength
                                                         for edge enhancement */
    ot_u16 texture_freq;         /* R; range: [0, 4095];Format:6.6; Texture frequency adjustment.
                                    Texture and detail will be finer when it increase */
    ot_u16 edge_freq;            /* R; range: [0, 4095];Format:6.6; Edge frequency adjustment.
                                    Edge will be narrower and thiner when it increase */
    ot_u8  over_shoot;           /* R; range: [0, 127]; Format:7.0;u8OvershootAmt */
    ot_u8  under_shoot;          /* R; range: [0, 127]; Format:7.0;u8UndershootAmt */
    ot_u8  shoot_sup_strength;   /* R; range: [0, 255]; Format:8.0;overshoot and undershoot suppression strength,
                                    the amplitude and width of shoot will be decrease when shootSupSt increase */

    ot_u8   nr_lsc_ratio;                     /* R;Range:[0x0, 0xf];Format:4.0; lsc nr ratio */
    ot_u16  coarse_strength[OT_EIS_VI_BAYER_CHN_NUM];    /* R;Range:[0x0, 0x360];Format:10.0;
                                                 Coarse Strength of noise reduction */
    ot_u8   wdr_frame_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM]; /* R;Range:[0x0, 0x40];Format:7.0;
                                                 Coarse strength of each frame in wdr mode */
    ot_u8   fine_strength;          /* R;Range:[0x0,0x80];Format:8.0;Strength of Luma noise reduction */
    ot_u16  coring_wgt;             /* R;Range:[0x0,0xC80];Format:12.0;Strength of reserving the random noise */

    ot_u16 dehaze_strength_actual; /* R;Range:[0,0xFF];Format:8.0;actual dehaze strength */
    ot_u16 drc_strength_actual;    /* R;Range: [0x0, 0x3FF];
                                    Strength of dynamic range compression.
                                    Higher values lead to higher differential gain between shadows and highlights. */
    ot_u32 wdr_exp_ratio_actual[OT_EIS_VI_WDR_MAX_FRAME_NUM - 1]; /* R; Range:[0x40, 0x4000]; Format:26.6;
                                                         0x40 means 1 times.
                                                         When enExpRatioType is AUTO, u32ExpRatio is invalid.
                                                         When enExpRatioType is MANUAL, u32ExpRatio is
                                                         quotient of long exposure time / short exposure time. */
    ot_bool wdr_switch_finish;       /* R; Range:[0, 1];Format:1.0;OT_TRUE: WDR switch is finished */
    ot_bool res_switch_finish;       /* R; Range:[0, 1];Format:1.0;OT_TRUE: Resolution switch is finished */
    ot_u16  black_level_actual[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_BAYER_CHN_NUM]; /* R; Range: [0x0, 0x3FFF];Format:14.0;
                                             Actual Black level values that correspond to the black levels of
                                             the R,Gr, Gb, and B components respectively. */

    ot_u16  sns_black_level[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_BAYER_CHN_NUM]; /* R; Range: [0x0, 0x3FFF];Format:14.0;
                                                      sesnor Black level values that correspond to the black levels of
                                                      the R,Gr, Gb, and B components respectively. */
} ot_eis_vi_alg_state_info;

/* statistics structs */
typedef union {
    ot_u32  key;
    struct {
        ot_u32  bit1_fe_ae_global_stat         : 1;   /* [0] */
        ot_u32  bit1_fe_ae_local_stat          : 1;   /* [1] */
        ot_u32  bit1_fe_ae_stitch_global_stat  : 1;   /* [2] */
        ot_u32  bit1_fe_ae_stitch_local_stat   : 1;   /* [3] */
        ot_u32  bit1_be_ae_global_stat         : 1;   /* [4] */
        ot_u32  bit1_be_ae_local_stat          : 1;   /* [5] */
        ot_u32  bit1_be_ae_stitch_global_stat  : 1;   /* [6] */
        ot_u32  bit1_be_ae_stitch_local_stat   : 1;   /* [7] */
        ot_u32  bit1_awb_stat1                 : 1;   /* [8] */
        ot_u32  bit1_awb_stat2                 : 1;   /* [9] */
        ot_u32  bit2_reserved0                 : 2;   /* [10:11] */
        ot_u32  bit1_fe_af_stat                : 1;   /* [12] */
        ot_u32  bit1_be_af_stat                : 1;   /* [13] */
        ot_u32  bit2_reserved1                 : 2;   /* [14:15] */
        ot_u32  bit1_dehaze                    : 1;   /* [16] */
        ot_u32  bit1_mg_stat                   : 1;   /* [17] */
        ot_u32  bit1_extend_stats              : 1;   /* [18] */
        ot_u32  bit13_reserved                 : 13;   /* [19:31] */
    };
} ot_eis_vi_stats_ctrl;

typedef struct {
    ot_bool enable;  /* RW; Range: [0,1];  Format:1.0; crop enable. */
    ot_u16  x;       /* RW; Format:13.0; image crop start x */
    ot_u16  y;       /* RW; Format:13.0; image crop start y */
    ot_u16  width;   /* RW; Format:14.0; image crop width */
    ot_u16  height;   /* RW; Format:14.0; image crop height */
} ot_eis_vi_stats_crop;

typedef struct {
    ot_u16 grid_y_pos[OT_EIS_VI_AE_ZONE_ROW + 1];   /* R */
    ot_u16 grid_x_pos[OT_EIS_VI_AE_ZONE_COLUMN + 1]; /* R */
    ot_u8  status;                        /* R;0:not update, 1: update,others:reserved */
} ot_eis_vi_ae_grid_info;

typedef struct {
    ot_u16 grid_y_pos[OT_EIS_VI_MG_ZONE_ROW + 1];    /* R */
    ot_u16 grid_x_pos[OT_EIS_VI_MG_ZONE_COLUMN + 1]; /* R */
    ot_u8  status;                         /* R;0:not update, 1: update,others:reserved */
} ot_eis_vi_mg_grid_info;

typedef struct {
    ot_u16 grid_y_pos[OT_EIS_VI_AWB_ZONE_ORIG_ROW  + 1];   /* R */
    ot_u16 grid_x_pos[OT_EIS_VI_AWB_ZONE_ORIG_COLUMN + 1]; /* R */
    ot_u8  status;                               /* R;0:not update, 1: update,others:reserved */
} ot_eis_vi_awb_grid_info;

/*
  The position of AE histogram in BE pipeline
  0 = After DG;
  1 = After static WB;
  2 = After DRC;
 */
typedef enum {
    OT_EIS_VI_AE_STATS_POS_AFTER_DG = 0,
    OT_EIS_VI_AE_STATS_POS_AFTER_WB,
    OT_EIS_VI_AE_STATS_POS_AFTER_DRC,
    OT_EIS_VI_AE_STATS_POS_BUTT
} ot_eis_vi_ae_stats_pos;

/*
  FourPlaneMode enable
  0 : Disabled;
  1 : Enabled
*/
typedef enum {
    OT_EIS_VI_AE_FOUR_PLANE_MODE_DISABLE = 0,
    OT_EIS_VI_AE_FOUR_PLANE_MODE_ENABLE,
    OT_EIS_VI_AE_FOUR_PLANE_MODE_BUTT
} ot_eis_vi_ae_four_plane_mode;

typedef enum {
    OT_EIS_VI_AE_HIST_SKIP_EVERY_PIXEL = 0,
    OT_EIS_VI_AE_HIST_SKIP_EVERY_2ND_PIXEL,
    OT_EIS_VI_AE_HIST_SKIP_EVERY_3RD_PIXEL,
    OT_EIS_VI_AE_HIST_SKIP_EVERY_4TH_PIXEL,
    OT_EIS_VI_AE_HIST_SKIP_EVERY_5TH_PIXEL,
    OT_EIS_VI_AE_HIST_SKIP_EVERY_8TH_PIXEL,
    OT_EIS_VI_AE_HIST_SKIP_EVERY_9TH_PIXEL,
    OT_EIS_VI_AE_HIST_SKIP_BUTT
} ot_eis_vi_ae_hist_skip;

typedef enum {
    OT_EIS_VI_AE_HIST_START_FORM_FIRST_COLUMN = 0,
    OT_EIS_VI_AE_HIST_START_FORM_SECOND_COLUMN,
    OT_EIS_VI_AE_HIST_OFFSET_X_BUTT
} ot_eis_vi_ae_hist_offset_x;

typedef enum {
    OT_EIS_VI_AE_HIST_START_FORM_FIRST_ROW = 0,
    OT_EIS_VI_AE_HIST_START_FORM_SECOND_ROW,
    OT_EIS_VI_AE_HIST_OFFSET_Y_BUTT
} ot_eis_vi_ae_hist_offset_y;

/* Defines the mode configuration for the sampling points during global histogram statistics. */
typedef struct {
    ot_eis_vi_ae_hist_skip hist_skip_x;  /* RW; Range:[0, 6]; Format:4.0;
                                         Histogram decimation in horizontal direction: 0=every pixel;
                                         1=every 2nd pixel; 2=every 3rd pixel; 3=every 4th pixel; 4=every 5th pixel;
                                         5=every 8th pixel ; 6+=every 9th pixel */
    ot_eis_vi_ae_hist_skip hist_skip_y;  /* RW; Range:[0, 6]; Format:4.0;
                                         Histogram decimation in vertical direction: 0=every pixel; 1=every 2nd pixel;
                                         2=every 3rd pixel; 3=every 4th pixel; 4=every 5th pixel; 5=every 8th pixel;
                                         6+=every 9th pixel */
    ot_eis_vi_ae_hist_offset_x hist_offset_x;  /* RW; Range:[0, 1]; Format:1.0;
                                               0= start from the first column; 1=start from second column */
    ot_eis_vi_ae_hist_offset_y hist_offset_y;  /* RW; Range:[0, 1]; Format:1.0;
                                               0= start from the first row; 1= start from second row */
} ot_eis_vi_ae_hist_config;

typedef enum {
    OT_EIS_VI_AE_NORMAL = 0,
    OT_EIS_VI_AE_ROOT,
    OT_EIS_VI_AE_STAT_MODE_BUTT
} ot_eis_vi_ae_stat_mode;

/* config of statistics structs */
typedef struct {
    ot_eis_vi_ae_stats_pos       ae_stats_pos;       /* RW; Range:[0, 2]; Format:2.0;
                                                     The position of AE histogram in BE pipeline */
    ot_eis_vi_ae_hist_config     hist_config;
    ot_eis_vi_ae_four_plane_mode four_plane_mode;    /* RW; Range:[0, 1]; Format:2.0;Four Plane Mode Enable */
    ot_eis_vi_ae_stat_mode       hist_mode;          /* RW; Range:[0, 1]; Format:2.0;AE Hist Rooting Mode */
    ot_eis_vi_ae_stat_mode       aver_mode;          /* RW; Range:[0, 1]; Format:2.0;AE Aver Rooting Mode */
    ot_eis_vi_ae_stat_mode       max_gain_mode;      /* RW; Range:[0, 1]; Format:2.0;Max Gain Rooting Mode */
    ot_eis_vi_stats_crop         crop;               /* RW; AE input image crop for module;
                                                        x: limited range:[0, image_width - 256],
                                                        y: limited range:[0, image_height - 120],
                                                        width: limited range:[256, image_width],
                                                        height: limited range:[120, image_height] */
    ot_eis_vi_stats_crop         fe_crop;            /* RW; AE input image crop for FE module;
                                                        x: limited range:[0, image_width - 256],
                                                        y: limited range:[0, image_height - 120],
                                                        width: limited range:[256, image_width],
                                                        height: limited range:[120, image_height] */
    ot_u8 weight[OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN]; /* RW; Range:[0x0, 0xF]; Format:4.0; AE weighting table */
} ot_eis_vi_ae_stats_cfg;

#define OT_EIS_VI_HIST_NUM 1024
typedef struct {
    ot_u32 fe_hist1024[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_HIST_NUM];  /* R; Range: [0x0, 0xFFFFFFFF]; Format:32.0;
                                                             Global 1024 bins histogram of FE */
    ot_u16 fe_global_avg[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_BAYER_CHN_NUM]; /* R; Range: [0x0, 0xFFFF]; Format:16.0;
                                                                 Global average value of FE */
    ot_u16 fe_zone_avg[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN][OT_EIS_VI_BAYER_CHN_NUM]; /* R;
                              Range: [0x0, 0xFFFF]; Format:16.0; Zone average value of FE */
    ot_u32 be_estimate_hist1024[OT_EIS_VI_HIST_NUM]; /* R; Range: [0x0, 0xFFFFFFFF]; Format:32.0;
                                                          Estimated Global 1024 bins histogram of BE */
    ot_u32 be_hist1024[OT_EIS_VI_HIST_NUM];         /* R; Range: [0x0, 0xFFFFFFFF]; Format:32.0;
                                                          Global 1024 bins histogram of BE */
    ot_u16 be_global_avg[OT_EIS_VI_BAYER_CHN_NUM];  /* R; Range: [0x0, 0xFFFF]; Format:16.0;
                                                          Global average value of BE */
    ot_u16 be_zone_avg[OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN][OT_EIS_VI_BAYER_CHN_NUM]; /* R; Range: [0x0, 0xFFFF];
                                                                               Format:16.0; Zone average value of BE */
    ot_eis_vi_ae_grid_info fe_grid_info;
    ot_eis_vi_ae_grid_info be_grid_info;
    ot_u64 pts;
} ot_eis_vi_ae_stats;

typedef struct {
    ot_u16 zone_avg[OT_EIS_VI_MG_ZONE_ROW][OT_EIS_VI_MG_ZONE_COLUMN][OT_EIS_VI_BAYER_CHN_NUM]; /* R; Range: [0x0, 0xFF];
                                                                                      Format:8.0; Zone average value */
    ot_eis_vi_mg_grid_info grid_info;
} ot_eis_vi_mg_stats;

typedef enum {
    OT_EIS_VI_AWB_STATS_POS_AFTER_DG = 0,
    OT_EIS_VI_AWB_STATS_POS_AFTER_EXPANDER,
    OT_EIS_VI_AWB_STATS_POS_AFTER_DRC,
    OT_EIS_VI_AWB_STATS_POS_BUTT
} ot_eis_vi_awb_stats_pos;

/* Defines the AWB statistics configuration */
typedef struct {
    ot_eis_vi_awb_stats_pos awb_stats_pos; /* RW; Range: [0x0, 0x2]; Position of AWB statistics in pipeline */
    ot_u16 zone_row;           /* RW; Range: [0x1, 0x20]; Vertical Blocks,
                                  limited range:[ceil(image_height / 1024), min(32, image_height / 15)] */
    ot_u16 zone_col;           /* RW; Range: [0x1, 0x20]; Horizontal Blocks,
                                  limited range:[max(BlkNum, ceil(width / 1024)), min(32, image_width / 20)] */
    ot_u16 white_level;        /* RW; Range: [0x0, 0xFFFF];Upper limit of valid data for white region,
                                  for Bayer statistics, [0x0, 0x3FF] for RGB statistics */
    ot_u16 black_level;        /* RW; Range: [0x0, 0xFFFF];limited range: [0x0, u16WhiteLevel],
                                  Lower limit of valid data for white region .
                                  for Bayer statistics, bitwidth is 12, for RGB statistics, bitwidth is 10 */
    ot_u16 cb_max;             /* RW; Range: [0x0, 0xFFF];Maximum value of B/G for white region */
    ot_u16 cb_min;             /* RW; Range: [0x0, 0xFFF];
                                  limited range: [0x0, u16CbMax]Minimum value of B/G for white region */
    ot_u16 cr_max;             /* RW; Range: [0x0, 0xFFF];Maximum value of R/G for white region */
    ot_u16 cr_min;             /* RW; Range: [0x0, 0xFFF];
                                  limited range: [0x0, u16CrMax],Minimum value of R/G for white region */
    ot_eis_vi_stats_crop crop;   /* RW; Crops the input image of the AWB module;
                                  x: limited range:[0, image_width - zone_col * 20],
                                  y: limited range:[0, image_height - zone_row * 15],
                                  width: limited range:[zone_col * 20, image_width],
                                  height: limited range:[zone_row * 15, image_height] */
} ot_eis_vi_wb_stats_cfg;

typedef struct {
    ot_u16 global_r;          /* R; Range: [0x0, 0xFFFF];Global WB output Average R */
    ot_u16 global_g;          /* R; Range: [0x0, 0xFFFF];Global WB output Average G */
    ot_u16 global_b;          /* R; Range: [0x0, 0xFFFF];Global WB output Average B */
    ot_u16 count_all;         /* R; Range: [0x0, 0xFFFF];normalized number of Gray points */

    ot_u16 zone_avg_r[OT_EIS_VI_AWB_ZONE_NUM];            /* R; Range: [0x0, 0xFFFF];Zone Average R */
    ot_u16 zone_avg_g[OT_EIS_VI_AWB_ZONE_NUM];            /* R; Range: [0x0, 0xFFFF];Zone Average G */
    ot_u16 zone_avg_b[OT_EIS_VI_AWB_ZONE_NUM];            /* R; Range: [0x0, 0xFFFF];Zone Average B */
    ot_u16 zone_count_all[OT_EIS_VI_AWB_ZONE_NUM];        /* R; Range: [0x0, 0xFFFF];normalized number of Gray points */
    ot_eis_vi_awb_grid_info grid_info;
    ot_u64 pts;
} ot_eis_vi_wb_stats;

#define OT_EIS_VI_DEHAZE_ZONE_COLUMN        24
#define OT_EIS_VI_DEHAZE_ZONE_ROW           16

typedef struct {
    ot_u32 local_air[OT_EIS_VI_DEHAZE_ZONE_ROW][OT_EIS_VI_DEHAZE_ZONE_COLUMN];
    ot_u16 dark_chn[OT_EIS_VI_DEHAZE_ZONE_ROW][OT_EIS_VI_DEHAZE_ZONE_COLUMN];
    ot_u64 pts;
} ot_eis_vi_dehaze_stats;

typedef struct {
    ot_eis_vi_stats_ctrl      ctrl;
    ot_eis_vi_stats_ctrl      update;
    ot_eis_vi_ae_stats_cfg    ae_cfg;
    ot_eis_vi_wb_stats_cfg    wb_cfg;
} ot_eis_vi_stats_cfg;

/* Smart Info */
#define OT_EIS_VI_PEOPLE_CLASS_MAX   2
typedef struct {
    ot_bool  enable;                /* R; Range:[0, 1]; enable smart roi info */
    ot_bool  available;             /* R; Range:[0, 1]; whether roi area is available */
    ot_u8    luma;                  /* R; Range:[0x00, 0xFF]; luma of roi area */
} ot_eis_vi_people_roi;

#define OT_EIS_VI_TUNNEL_CLASS_MAX   2
typedef struct {
    ot_bool  enable;                /* R; Range:[0, 1]; enable tunnel roi info */
    ot_bool  available;             /* R; Range:[0, 1]; whether roi area is available */
    ot_u32   tunnel_area_ratio;     /* R; Range:[0, 10000]; the proportion of tunnels in the whole picture */
    ot_u32   tunnel_exp_perf;       /* R; Range:[0, 10000]; the performance of tunnels exposure result */
} ot_eis_vi_tunnel_roi;

#define OT_EIS_VI_FACE_NUM 5
typedef struct {
    ot_bool  enable;                     /* R; Range:[0, 1]; enable fast face roi info */
    ot_bool  available;                  /* R; Range:[0, 1]; whether roi area is available */
    ot_u64   frame_pts;                  /* pts of face info */
    ot_eis_img_rect face_rect[OT_EIS_VI_FACE_NUM];  /* face roi rect */
} ot_eis_vi_face_roi;

typedef struct {
    ot_eis_vi_people_roi  people_roi[OT_EIS_VI_PEOPLE_CLASS_MAX]; /* people ae info */
    ot_eis_vi_tunnel_roi  tunnel_roi[OT_EIS_VI_TUNNEL_CLASS_MAX]; /* tunnels ae info */
    ot_eis_vi_face_roi face_roi;                               /* fast face ae info */
} ot_eis_vi_smart_info;


/* sensor communication bus */
typedef union {
    ot_s8   i2c_dev;       /* AUTO:ot_eis_vi_sns_type:OT_EIS_VI_SNS_TYPE_I2C */
    struct {
        ot_s8  bit4_ssp_dev       : 4;
        ot_s8  bit4_ssp_cs        : 4;
    } ssp_dev;             /* AUTO:ot_eis_vi_sns_type:OT_EIS_VI_SNS_TYPE_SSP */
} ot_eis_vi_sns_commbus;

typedef struct {
    ot_bool update;        /* RW; Range: [0x0, 0x1]; Format:1.0;
                              OT_TRUE: The sensor registers are written,
                              OT_FALSE: The sensor registers are not written */
    ot_u8   delay_frame_num; /* RW; Number of delayed frames for the sensor register */
    ot_u8   interrupt_pos;       /* RW;Position where the configuration of the sensor register takes effect */
                                /* 0x0,very short frame start interrupt, 0x1:very short frame end interrupt,
                                   0x10,short frame start interrupt, 0x11:short frame end interrupt,
                                   0x20,middle frame start interrupt, 0x21:middle frame end interrupt,
                                   0x30,long frame start interrupt, 0x31:long frame end interrupt            */
    ot_u8   dev_addr;      /* RW;Sensor device address */
    ot_u32  reg_addr;      /* RW;Sensor register address */
    ot_u32  addr_byte_num; /* RW;Bit width of the sensor register address */
    ot_u32  data;          /* RW;Sensor register data */
    ot_u32  data_byte_num; /* RW;Bit width of sensor register data */
} ot_eis_vi_i2c_data;

typedef struct {
    ot_bool update;             /* RW; Range: [0x0, 0x1]; Format:1.0;
                                   OT_TRUE: The sensor registers are written,
                                   OT_FALSE: The sensor registers are not written */
    ot_u8   delay_frame_num;      /* RW; Number of delayed frames for the sensor register */
    ot_u8   interrupt_pos;      /* RW;Position where the configuration of the sensor register takes effect
                                 0x0,very short frame start interrupt, 0x1:very short frame end interrupt
                                 0x10,short frame start interrupt, 0x11:short frame end interrupt
                                 0x20,middle frame start interrupt, 0x21:middle frame end interrupt
                                 0x30,long frame start interrupt, 0x31:long frame end interrupt            */

    ot_u32  dev_addr;           /* RW;Sensor device address */
    ot_u32  dev_addr_byte_num;  /* RW;Bit width of the sensor device address */
    ot_u32  reg_addr;           /* RW;Sensor register address */
    ot_u32  reg_addr_byte_num;  /* RW;Bit width of the sensor register address */
    ot_u32  data;               /* RW;Sensor register data */
    ot_u32  data_byte_num;      /* RW;Bit width of sensor register data */
} ot_eis_vi_ssp_data;

typedef struct {
    ot_eis_vi_sns_type sns_type;      /* RW; Range:[0, 1]; Format: 1.0;
                                      OT_EIS_VI_SNS_I2C_TYPE: I2C interface type between the sensor and VI,
                                      OT_EIS_VI_SNS_SSP_TYPE: SPI interface type between the sensor and VI */
    ot_u32  reg_num;               /* RW;Number of registers required when exposure results are written to the sensor.
                                      The member value cannot be dynamically changed */
    ot_u8   cfg2_valid_delay_max;  /* RW;Maximum number of delayed frames from the time when all sensor registers are
                                      configured to the time when configurations take effect, which is used to ensure
                                      the synchronization between sensor registers and VI registers */

    ot_u32  exp_distance[OT_EIS_VI_WDR_MAX_FRAME_NUM - 1]; /* RW; wdr sensor exposure distance,
                                                            not support  */
    ot_eis_vi_sns_commbus  com_bus;
    union {
        ot_eis_vi_i2c_data i2c_data[OT_EIS_VI_MAX_SNS_REGS];  /* AUTO: ot_eis_vi_sns_type:OT_EIS_VI_SNS_I2C_TYPE */
        ot_eis_vi_ssp_data ssp_data[OT_EIS_VI_MAX_SNS_REGS];  /* AUTO: ot_eis_vi_sns_type:OT_EIS_VI_SNS_SSP_TYPE */
    };

    struct {
        ot_bool update;
        ot_u8   delay_frame_num;
        ot_u32  slave_vs_time;      /* RW;time of vsync. Unit: inck clock cycle */
        ot_u32  slave_bind_dev;
    } slv_sync;

    ot_bool config;
} ot_eis_vi_sns_regs_info;

/*
 * Defines the VI FSWDR operating mode
 * 0 = Normal FSWDR mode
 * 1 = Long frame mode, only effective in LINE_WDR,
 * when running in this mode FSWDR module only output the long frame data
 */
typedef enum {
    OT_EIS_VI_FSWDR_NORMAL_MODE          = 0x0,
    OT_EIS_VI_FSWDR_LONG_FRAME_MODE      = 0x1,
    OT_EIS_VI_FSWDR_AUTO_LONG_FRAME_MODE = 0x2, /* Auto long frame mode, only effective in LINE_WDR,  When running in this
                                             mode, normal WDR and long frame mode would auto switch */
    OT_EIS_VI_FSWDR_MODE_BUTT
} ot_eis_vi_fswdr_mode;

/* Defines the F value of the VI iris */
typedef enum {
    OT_EIS_VI_IRIS_F_NO_32_0 = 0,
    OT_EIS_VI_IRIS_F_NO_22_0,
    OT_EIS_VI_IRIS_F_NO_16_0,
    OT_EIS_VI_IRIS_F_NO_11_0,
    OT_EIS_VI_IRIS_F_NO_8_0,
    OT_EIS_VI_IRIS_F_NO_5_6,
    OT_EIS_VI_IRIS_F_NO_4_0,
    OT_EIS_VI_IRIS_F_NO_2_8,
    OT_EIS_VI_IRIS_F_NO_2_0,
    OT_EIS_VI_IRIS_F_NO_1_4,
    OT_EIS_VI_IRIS_F_NO_1_0,

    OT_EIS_VI_IRIS_F_NO_BUTT,
} ot_eis_vi_iris_f_no;

#define OT_EIS_VI_AE_ROUTE_MAX_NODES                    16
typedef struct {
    ot_u32  int_time;         /* RW; Range:(0x0, 0xFFFFFFFF]; Format:32.0;
                                 sensor exposure time (unit: us), it's related to the specific sensor */
    ot_u32  sys_gain;         /* RW; Range:[0x400, 0xFFFFFFFF]; Format:22.10;
                                 system gain (unit: times, 10bit precision), it's related to the specific sensor and
                                 ISP Dgain range */
    ot_eis_vi_iris_f_no iris_fno;  /* RW; Range:[0, 10]; Format:4.0;
                                   the F number of the iris's aperture, only support for Piris */
    ot_u32  iris_fno_lin;       /* RW; Range:[0x1, 0x400]; Format:32.0;
                                   the equivalent gain of F number of the iris's aperture, only support for Piris */
} ot_eis_vi_ae_route_node;

typedef struct {
    ot_u32 total_num;     /* RW; Range:[0, 0x10]; Format:8.0; total node number of AE route */
    ot_eis_vi_ae_route_node route_node[OT_EIS_VI_AE_ROUTE_MAX_NODES];
} ot_eis_vi_ae_route;

#define OT_EIS_VI_AE_ROUTE_EX_MAX_NODES                 16
typedef struct {
    ot_u32  int_time;     /* RW; Range:(0x0, 0xFFFFFFFF]; Format:32.0;
                             sensor exposure time (unit: us), it's related to the specific sensor */
    ot_u32  a_gain;       /* RW; Range:[0x400, 0x3FFFFF]; Format:22.10;
                             sensor analog gain (unit: times, 10bit precision), it's related to the specific sensor */
    ot_u32  d_gain;       /* RW; Range:[0x400, 0x3FFFFF]; Format:22.10;
                             sensor digital gain (unit: times, 10bit precision), it's related to the specific sensor */
    ot_u32  isp_d_gain;   /* RW; Range:[0x400, 0x40000]; Format:22.10;
                             ISP digital gain (unit: times, 10bit precision) */
    ot_eis_vi_iris_f_no  iris_fno;  /* RW; Range:[0, 10]; Format:4.0;
                                    the F number of the iris's aperture, only support for Piris */
    ot_u32  iris_fno_lin;  /* RW; Range:[0x1, 0x400]; Format:32.0;
                              the equivalent gain of F number of the iris's aperture, only support for Piris */
} ot_eis_vi_ae_route_ex_node;

typedef struct {
    ot_u32 total_num;       /* RW; Range:[0, 0x10]; Format:8.0; total node number of extend AE route */
    ot_eis_vi_ae_route_ex_node route_ex_node[OT_EIS_VI_AE_ROUTE_EX_MAX_NODES];
} ot_eis_vi_ae_route_ex;


typedef struct {
    ot_u16 red_cast_gain;        /* RW; Range: [0x100, 0x180]; Format:4.8; R channel gain after CCM */
    ot_u16 green_cast_gain;      /* RW; Range: [0x100, 0x180]; Format:4.8; G channel gain after CCM */
    ot_u16 blue_cast_gain;       /* RW; Range: [0x100, 0x180]; Format:4.8; B channel gain after CCM */
} ot_eis_vi_color_tone_attr;

#define OT_EIS_VI_BAYER_CALIBRATION_PARA_NUM 16
typedef struct {
    ot_double calibration_coef[OT_EIS_VI_BAYER_CALIBRATION_PARA_NUM];
} ot_eis_vi_noise_calibration;

typedef struct {
    /* CCD sensitivity equivalent to ag-hr film speedrate */
    ot_u32 iso_speed_ratings;
    /* exposure time (reciprocal of shutter speed). */
    ot_u32 exposure_time;
    /* exposure bias (compensation) value of taking picture */
    ot_u32 exposure_bias_value;
    /*
     * exposure program that the camera used when image was taken. '1' means manual control,
     * '2' program normal, '3' aperture priority, '4' shutter priority, '5' program creative (slow program),
     * '6' program action(high-speed program), '7' portrait mode, '8' landscape mode.
     */
    ot_u8 exposure_program;
    /* the actual F-number (F-stop) of lens when the image was taken */
    ot_u32 f_number;
    /* maximum aperture value of lens. */
    ot_u32 max_aperture_value;
    /*
     * indicates the exposure mode set when the image was shot.
     * 0 = auto exposure, 1 = manual exposure, 2 = auto bracket
     */
    ot_u8 exposure_mode;
    /*
     * indicates the white balance mode set when the image was shot.
     * 0 = auto white balance, 1 = manual white balance.
     */
    ot_u8 white_balance;
} ot_eis_vi_dcf_update_info;

typedef struct {
    ot_bool enable;
} ot_eis_vi_dither_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_VI_ALG_H */
