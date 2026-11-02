#ifndef OT_EIS_VI_SNS_H
#define OT_EIS_VI_SNS_H

#include "ot_eis_type.h"
#include "ot_eis_vi.h"
#include "ot_eis_vi_alg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    ot_bool update;
    ot_u16  black_level[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_BAYER_CHN_NUM]; /* 14bits */
} ot_eis_vi_black_level_auto_attr;

typedef struct {
    ot_bool  user_black_level_en;
    ot_u16   user_black_level[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_BAYER_CHN_NUM];
    ot_eis_vi_black_level_mode         black_level_mode;
    ot_eis_vi_black_level_manual_attr  manual_attr;
    ot_eis_vi_black_level_auto_attr    auto_attr;
} ot_eis_vi_sns_black_level;

typedef struct {
    ot_s32            sns_id;
} ot_eis_vi_sns_attr_info;

typedef struct {
    ot_u32  max_width;
    ot_u32  max_height;
} ot_eis_vi_sns_max_resolution;

typedef struct {
    ot_eis_vi_shading_attr     lsc_attr;
    ot_eis_vi_shading_lut_attr lsc_lut;
} ot_eis_vi_sns_lsc;

typedef struct {
    ot_eis_vi_lblc_attr     lblc_attr;
    ot_eis_vi_lblc_lut_attr lblc_lut;
} ot_eis_vi_cmos_lblc;

typedef struct {
    ot_eis_vi_clut_attr clut_attr;
    ot_eis_vi_clut_lut clut_lut;
} ot_eis_vi_sns_clut;

typedef struct {
    ot_u32  sns_id;
    ot_u8   sns_mode;
} ot_eis_vi_sns_mode;

typedef struct {
    ot_u32   exp_ratio[OT_EIS_VI_EXP_RATIO_NUM];
} ot_eis_vi_sns_wdr_switch_attr;

typedef struct {
    ot_u16 g_param_high_ct[OT_EIS_VI_LSC_GRID_POINTS];
    ot_u16 g_param_low_ct[OT_EIS_VI_LSC_GRID_POINTS];
} ot_eis_vi_acs_y_shading_lut;

typedef struct {
    ot_float avg_rg_map[OT_EIS_VI_LSC_GRID_POINTS];
    ot_float avg_bg_map[OT_EIS_VI_LSC_GRID_POINTS];
    ot_float prof_rg_map[OT_EIS_VI_LSC_GRID_POINTS];
    ot_float prof_bg_map[OT_EIS_VI_LSC_GRID_POINTS];
} ot_eis_vi_acs_color_shading_lut;

typedef struct {
    ot_s16   light_index[OT_EIS_VI_ACS_LIGHT_NUM * OT_EIS_VI_ACS_CHN_NUM];
    ot_float model_ar_min;
    ot_float model_ar_step;
    ot_float model_ab_min;
    ot_float model_ab_step;
    ot_s16   light_type_g_high;
    ot_s16   light_type_g_low;
} ot_eis_vi_acs_calib_param;

typedef struct {
    ot_eis_vi_acs_attr               acs_attr;
    ot_eis_vi_acs_calib_param        acs_calib_param;
    ot_eis_vi_acs_y_shading_lut      acs_y_shading_lut;
    ot_eis_vi_acs_color_shading_lut  acs_color_shading_lut;
} ot_eis_vi_cmos_acs;

typedef union {
    ot_u64  key;
    struct {
        ot_u64  bit1_drc              : 1;   /* [0] */
        ot_u64  bit1_demosaic         : 1;   /* [1] */
        ot_u64  bit1_reserved         : 1;   /* [2] */
        ot_u64  bit1_gamma            : 1;   /* [3] */
        ot_u64  bit1_sharpen          : 1;   /* [4] */
        ot_u64  bit1_ldci             : 1;   /* [5] */
        ot_u64  bit1_dpc              : 1;   /* [6] */
        ot_u64  bit1_lsc              : 1;   /* [7] */
        ot_u64  bit1_ge               : 1;   /* [8] */
        ot_u64  bit1_anti_false_color : 1;   /* [9] */
        ot_u64  bit1_bayer_nr         : 1;   /* [10] */
        ot_u64  bit1_ca               : 1;   /* [11] */
        ot_u64  bit1_expander         : 1;   /* [12] */
        ot_u64  bit1_clut             : 1;   /* [13] */
        ot_u64  bit1_wdr              : 1;   /* [14] */
        ot_u64  bit1_dehaze           : 1;   /* [15] */
        ot_u64  bit1_cac              : 1;   /* [16] */
        ot_u64  bit1_acs              : 1;   /* [17] */
        ot_u64  bit1_bshp             : 1;   /* [18] */
        ot_u64  bit1_lblc             : 1;   /* [19] */
        ot_u64  bit44_reserved        : 44;   /* [20:63] */
    };
} ot_eis_vi_sns_alg_key;

typedef struct {
    ot_eis_vi_sns_alg_key                key;
    const ot_eis_vi_drc_attr              *drc;
    const ot_eis_vi_demosaic_attr         *demosaic;
    const ot_eis_vi_gamma_attr            *gamma;
    const ot_eis_vi_sharpen_attr          *sharpen;
    const ot_eis_vi_ldci_attr             *ldci;
    const ot_eis_vi_dpc_dynamic_cor_attr  *dpc;
    const ot_eis_vi_sns_lsc               *lsc;
    const ot_eis_vi_cr_attr               *ge;
    const ot_eis_vi_anti_false_color_attr *anti_false_color;
    const ot_eis_vi_nr_attr               *bayer_nr;
    const ot_eis_vi_ca_attr               *ca;
    const ot_eis_vi_expander_attr         *expander;
    const ot_eis_vi_sns_clut             *clut;
    const ot_eis_vi_wdr_attr             *wdr;
    const ot_eis_vi_dehaze_attr           *dehaze;
    const ot_eis_vi_cac_attr              *cac;
    const ot_eis_vi_bayer_sharpen_attr    *bshp;
    const ot_eis_vi_cmos_acs              *acs;
    const ot_eis_vi_cmos_lblc             *lblc;
    const ot_eis_vi_sns_black_level       *sns_blc;
    ot_eis_vi_noise_calibration     noise_calibration;
    ot_eis_vi_sns_max_resolution sns_max_resolution;
    ot_eis_vi_sns_mode           sns_mode;
    ot_eis_vi_sns_wdr_switch_attr       wdr_switch_attr;
} ot_eis_vi_sns_alg_default;

typedef struct {
    ot_void (*pfn_sns_init)(ot_vi_pipe_id vi_pipe);
    ot_void (*pfn_sns_exit)(ot_vi_pipe_id vi_pipe);
    ot_void (*pfn_sns_global_init)(ot_vi_pipe_id vi_pipe);
    ot_s32 (*pfn_sns_set_image_mode)(ot_vi_pipe_id vi_pipe, const ot_eis_vi_sns_image_mode *sns_image_mode);
    ot_s32 (*pfn_sns_set_wdr_mode)(ot_vi_pipe_id vi_pipe, ot_u8 mode);

    /* the algs get data which is associated with sensor, except 3a */
    ot_s32 (*pfn_sns_get_vi_alg_default)(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_alg_default *def);
    ot_s32 (*pfn_sns_get_vi_black_level)(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_black_level *black_level);
    ot_s32 (*pfn_sns_get_blc_clamp_info)(ot_vi_pipe_id vi_pipe, ot_bool *clamp_en);
    ot_s32 (*pfn_sns_get_reg_info)(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_regs_info *sns_regs_info);

    /* the function of sensor set pixel detect */
    ot_void (*pfn_sns_set_pixel_detect)(ot_vi_pipe_id vi_pipe, ot_bool enable);
    ot_s32 (*pfn_sns_get_awb_gains)(ot_vi_pipe_id vi_pipe, ot_u32 *sns_awb_gain);
} ot_eis_vi_sns_register_func;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_VI_SNS_H */
