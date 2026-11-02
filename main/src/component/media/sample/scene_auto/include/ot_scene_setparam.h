/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */


#ifndef OT_SCENE_SETPARAM_H
#define OT_SCENE_SETPARAM_H

#include "ot_eis_video.h"
#include "ot_eis_vi_alg.h"
#include "ot_eis_vi.h"
#include "ot_eis_vi_awb.h"
#include "ot_eis_scene_auto.h"

#include "ot_eis_vproc.h"
#include "ot_eis_venc.h"
#include "ot_eis_media_sys_common.h"
#ifdef __cplusplus
extern "C" {
#endif
#pragma pack(4)

#define OT_SCENE_AE_EXPOSURE_MAX_COUNT 12
#define OT_SCENE_RATIO_STRENGTH_NUM 11
#define OT_SCENE_SHADING_EXPOSURE_MAX_COUNT 10
#define OT_SCENE_GAMMA_EXPOSURE_MAX_COUNT 10
#define OT_EIS_SCENE_DRC_ISO_MAX_COUNT 16
#define OT_EIS_SCENE_DRC_REF_RATIO_MAX_COUNT 12
#define OT_EIS_SCENE_ISO_STRENGTH_NUM 16
#define OT_EIS_SCENE_DRC_RATIO_MAX_COUNT 12
#define OT_SCENE_REF_EXP_RATIO_FRM 16
#define OT_SCENE_NR_LUT_LENGTH 33
#define OT_SCENE_NR_LUT_LENGTH1 32
#define OT_SCENE_NR_RATIO_MAX_COUNT 16
#define OT_SCENE_FALSECOLOR_EXPOSURE_MAX_COUNT 10
#define OT_SCENE_LDCI_EXPOSURE_MAX_COUNT 6
#define OT_SCENE_DEHAZE_EXPOSURE_MAX_COUNT 16
#define OT_SCENE_3DNR_MAX_COUNT 16
#define OT_SCENE_3DNR_IDX_LEN 17
#define ISP_AUTO_ISO_CA_NUM 16
#define ISP_SCENE_CA_MAX_BLEND_WEIGHT 256
#define ISP_SCENE_CA_SHIFT 8
#define ISP_AUTO_ISO_BLC_NUM 16
#define ISP_AUTO_ISO_DPC_NUM 16
#define ISP_AUTO_ISO_AWB_NUM 16
#define FPN_GAIN_ISO 100
#define FPN_GAIN_SHIFT 20
#define FPN_CORRECTION_STR 256
#define DIR_NAME_BUFF 200
#define ISP_AUTO_RATIO_FSWDR_NUM 9
#define AIBNR_ISO_NUM 2
#define VENC_MODE_ISO_NUM 2
#define OT_SCENE_BAYERNR_DENOISE_Y_LUT_NUM      16
#define OT_SCENE_BSHP_CURVE_NUM    8
#define OT_SCENE_ACAC_THR_NUM             2


#undef  _tmprt2_4x
#define _tmprt2_4x(ps, X, Y) ps[1].X, ps[1].Y,  ps[2].X, ps[2].Y,  ps[3].X, ps[3].Y,  ps[4].X, ps[4].Y

#undef  _t3_
#define _t3_(ie, n, X, Y, Z)            ie[n].X, ie[n].Y, ie[n].Z
#undef  _t4_
#define _t4_(ie, n, K, X, Y, Z)   ie[n].K, ie[n].X, ie[n].Y, ie[n].Z

#undef  _t4a_0_
#define _t4a_0_(ps, a, X)   ps[a].X[0x00],  ps[a].X[0x01], ps[a].X[0x02], ps[a].X[0x03]
#undef  _t4a_4_
#define _t4a_4_(ps, a, X)   ps[a].X[0x04],  ps[a].X[0x05], ps[a].X[0x06], ps[a].X[0x07]
#undef  _t4a_8_
#define _t4a_8_(ps, a, X)   ps[a].X[0x08],  ps[a].X[0x09], ps[a].X[0x0a], ps[a].X[0x0b]
#undef  _t4a_12_
#define _t4a_12_(ps, a, X)   ps[a].X[0x0c],  ps[a].X[0x0d], ps[a].X[0x0e], ps[a].X[0x0f]

#undef  _t4a_10_
#define _t4a_10_(ps, a, X)   ps[a].X[0x10],  ps[a].X[0x11], ps[a].X[0x12], ps[a].X[0x13]
#undef  _t4a_14_
#define _t4a_14_(ps, a, X)   ps[a].X[0x14],  ps[a].X[0x15], ps[a].X[0x16], ps[a].X[0x17]
#undef  _t4a_18_
#define _t4a_18_(ps, a, X)   ps[a].X[0x18],  ps[a].X[0x19], ps[a].X[0x1a], ps[a].X[0x1b]
#undef  _t4a_1c_
#define _t4a_1c_(ps, a, X)   ps[a].X[0x1c],  ps[a].X[0x1d], ps[a].X[0x1e], ps[a].X[0x1f]

#undef  _tmprt3z
#define _tmprt3z(ps, a, b, X, Y, Z)  _t3_(ps, a, X, Y, Z), _t3_(ps, b, X, Y, Z)
#undef  _tmprt3x
#define _tmprt3x(ps, X, Y, Z)  _t3_(ps, 1, X, Y, Z), _t3_(ps, 2, X, Y, Z), _t3_(ps, 3, X, Y, Z)

#undef  _tmprt3_4
#define _tmprt3_4(ps, X, Y, Z)  _t3_(ps, 1, X, Y, Z), _t3_(ps, 2, X, Y, Z), _t3_(ps, 3, X, Y, Z), _t3_(ps, 4, X, Y, Z)

#undef  _tmprt4_4
#define _tmprt4_4(ps, K, X, Y, Z) \
    _t4_(ps, 1, K, X, Y, Z), _t4_(ps, 2, K, X, Y, Z), _t4_(ps, 3, K, X, Y, Z), _t4_(ps, 4, K, X, Y, Z)

#undef  _tmprt4_2
#define _tmprt4_2(ps, K, X, Y, Z)  _t4_(ps, 1, K, X, Y, Z), _t4_(ps, 2, K, X, Y, Z)

#define scene_set_dynamic_drc_member_piso(member, temp_member) do {                                       \
        if ((iso_level == 0) || (iso_level == (iso_count - 1))) {                                         \
            temp_member = g_scene_pipe_param[index].dynamic_drc.member[iso_level];                        \
        } else {                                                                                          \
            temp_member = scene_interpulate(iso, iso_level_thresh[iso_level - 1],                         \
                g_scene_pipe_param[index].dynamic_drc.member[iso_level - 1], iso_level_thresh[iso_level], \
                g_scene_pipe_param[index].dynamic_drc.member[iso_level]);                                 \
        }                                                                                                 \
    } while (0)


#define scene_set_dynamic_drc_member_pratio(member, temp_member) do {                                           \
        if ((ratio_level == 0) || (ratio_level == (ratio_count - 1))) {                                         \
            temp_member = g_scene_pipe_param[index].dynamic_drc.member[ratio_level];                            \
        } else {                                                                                                \
            temp_member = scene_interpulate(wdr_ratio, ratio_level_thresh[ratio_level - 1],                     \
                g_scene_pipe_param[index].dynamic_drc.member[ratio_level - 1], ratio_level_thresh[ratio_level], \
                g_scene_pipe_param[index].dynamic_drc.member[ratio_level]);                                     \
        }                                                                                                       \
    } while (0)

#define scene_set_drc_member(target, member, temp_member, flag) do {                                        \
        if (flag == 0) {                                                                                    \
            scene_set_dynamic_drc_member_pratio(member, temp_member);                                       \
        } else {                                                                                            \
            scene_set_dynamic_drc_member_piso(member, temp_member);                                         \
        }                                                                                                   \
        target = scene_time_filter(target, temp_member, g_scene_pipe_param[index].dynamic_drc.interval, i); \
    } while (0)

#define scene_set_dynamic_drc_return(target, member, flag) do {                       \
        void *temp_member;                                                            \
        temp_member = (typeof(target) *)malloc(sizeof(target));                       \
        if (temp_member == NULL) {                                                    \
            return;                                                                   \
        }                                                                             \
        scene_set_drc_member(target, member, *((typeof(target) *)temp_member), flag); \
        free(temp_member);                                                            \
    } while (0)

#define scene_set_nrx_attr_interpulate(pack, member) do {                        \
        (pack)->nrx_attr->member = scene_interpulate((pack)->mid, (pack)->left,        \
            (pack)->nrx_param->threednr_value[(pack)->iso_level - 1].member, (pack)->right,  \
            (pack)->nrx_param->threednr_value[(pack)->iso_level].member);                  \
    } while (0)

typedef struct {
    ot_u8 index;
    ot_u32 iso_level;
    ot_u32 iso_count;
    ot_u32 ratio_level;
    ot_u32 ratio_count;
    ot_u32 interval;
    ot_u32 wdr_ratio;
    ot_u32 iso;
} set_isp_attr_param;

typedef enum {
    OT_SCENE_OP_TYPE_AUTO = 0,
    OT_SCENE_OP_TYPE_MANUAL = 1,
    OT_SCENE_OP_TYPE_BUTT
} ot_scene_op_type;

typedef struct {
    ot_u32 fps_max;
} ot_scene_fps_auto;

typedef struct {
    ot_u32 fps;
} ot_scene_fps_manual;

typedef struct {
    ot_bool enable;
    ot_scene_op_type op_type;
    ot_scene_fps_auto auto_s;
    ot_scene_fps_manual manual;
} ot_scene_fps;

typedef struct {
    ot_bool debug;
    ot_bool ae_weight_tab;
    ot_bool static_ae;
    ot_bool static_cac;
    ot_bool static_dpc;
    ot_bool static_wdr_exposure;
    ot_bool static_fswdr;
    ot_bool static_awb;
    ot_bool static_awbex;
    ot_bool static_drc;
    ot_bool static_dehaze;
    ot_bool static_ldci;
    ot_bool static_ldcihlc;
    ot_bool static_saturation;
    ot_bool static_ccm;
    ot_bool static_dm;
    ot_bool static_nr;
    ot_bool static_shading;
    ot_bool static_csc;
    ot_bool static_bayeshp;
    ot_bool static_sharpen;
    ot_bool static_clut;
    ot_bool static_ca;
    ot_bool static_cross_talk;
    ot_bool static_3dnr;
    ot_bool static_blc;
    ot_bool static_gamma;
    ot_bool static_mce;
    ot_bool static_back_light;

	ot_bool dynamic_vproc;
    ot_bool dynamic_ae;
    ot_bool dynamic_fps;
    ot_bool dynamic_wdr_exposure;
    ot_bool dynamic_fswdr;
    ot_bool dynamic_drc;
    ot_bool dynamic_dehaze;
    ot_bool dynamic_ldci;
    ot_bool dynamic_ldcihlc;
    ot_bool dynamic_nr;
    ot_bool dynamic_dpc;
    ot_bool dynamic_awb;
    ot_bool dynamic_blc;
    ot_bool dynamic_linear_ca;
    ot_bool dynamic_ca;
    ot_bool dynamic_shading;
    ot_bool dynamic_clut;
    ot_bool dynamic_gamma;
    ot_bool dynamic_false_color;
    ot_bool dynamic_3dnr;
    ot_bool dynamic_fpn;
    ot_bool dynamic_abnr;
    ot_bool dynamic_iqt;
    ot_bool dynamic_mce;
    ot_bool dynamic_back_light;
} ot_scene_module_state;

typedef struct {
    ot_eis_scene_mce_attr mce_attr;
} ot_scene_static_mce;


typedef struct {
    ot_u32 total_num;
    ot_u32 int_time[OT_EIS_VI_AE_ROUTE_MAX_NODES];
    ot_u32 sys_gain[OT_EIS_VI_AE_ROUTE_MAX_NODES];
} ot_scene_static_ae_route;

typedef struct {
    ot_u32 total_num;
    ot_u32 int_time[OT_EIS_VI_AE_ROUTE_MAX_NODES];
    ot_u32 again[OT_EIS_VI_AE_ROUTE_MAX_NODES];
    ot_u32 dgain[OT_EIS_VI_AE_ROUTE_MAX_NODES];
    ot_u32 isp_dgain[OT_EIS_VI_AE_ROUTE_MAX_NODES];
} ot_scene_static_ae_route_ex;

typedef struct {
    ot_u32 dev_num;
    ot_u32 dev_id[16];
    ot_u32 backlight_base;
    ot_u32 backlight_array[16];
} ot_scene_static_back_light;

typedef struct {
    ot_bool gamma_enable;
    ot_u16 gamma_table[OT_EIS_VI_GAMMA_NODE_NUM];
    ot_u8 curve_type;
} ot_scene_static_gamma;

typedef struct {
    ot_u8  op_type;
    ot_u16 manual_rgain;
    ot_u16 manual_gbgain;
    ot_u16 manual_grgain;
    ot_u16 manual_bgain;
    ot_u16 auto_static_wb[OT_EIS_VI_BAYER_CHN_NUM];
    ot_s32 auto_curve_para[OT_EIS_VI_AWB_CURVE_PARA_NUM];
    ot_u16 auto_speed;
    ot_u16 auto_low_color_temp;
    ot_u16 auto_cr_max[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16 auto_cr_min[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16 auto_cb_max[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16 auto_cb_min[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16 luma_hist_enable;
    ot_u16 luma_hist_op_type;
    ot_u16 luma_hist_thresh[OT_EIS_VI_AWB_LUM_HIST_NUM];
    ot_u16 hist_wt[OT_EIS_VI_AWB_LUM_HIST_NUM];
    ot_u16 awb_switch;
    ot_u16 black_level;
    ot_u16 auto_high_color_temp;

    ot_u8 awb_run_interval;
    ot_bool ct_limit_enable;
    ot_u8 ct_limit_op_type;
    ot_u16 high_rg_limit;
    ot_u16 high_bg_limit;
    ot_u16 low_rg_limit;
    ot_u16 low_bg_limit;
    ot_u8 shift_limit;

    ot_u32 night_exp_thresh;
    ot_u32 skin_office_exp_thresh;
    ot_u16 out_switch_luma_thresh;
    ot_u16 out_switch_lowct_num;

    ot_bool weight_enable;
    ot_u8 weight_strength;

    ot_bool rp_enable;
    ot_u8 rp_valid[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u16 rp_tgt_rgain[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u16 rp_tgt_bgain[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u8 rp_ls_radius[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u16 rp_zone_rgain[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u16 rp_zone_bgain[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u8 rp_boundry_ratio[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u32 rp_exp_range_left[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u32 rp_exp_range_right[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u16 rp_ct_range_left[OT_EIS_VI_AWB_RP_LS_NUM];
    ot_u16 rp_ct_range_right[OT_EIS_VI_AWB_RP_LS_NUM];
} ot_scene_static_awb;

typedef struct {
    ot_bool bypass;
    ot_u8 tolerance;
    ot_u8 in_or_out_enable;
    ot_u8 in_or_out_op_type;
    ot_u8 in_or_out_outdoor_status;
    ot_u8 in_or_out_green_enhance_en;
    ot_u8 out_shift_limit;
    ot_u32 out_thresh;
	ot_u16 low_start;
    ot_u16 low_stop;
    ot_u16 high_start;
    ot_u16 high_stop;
    ot_bool multi_light_source_en;
    ot_u16 multi_ctwt[OT_EIS_VI_AWB_MULTI_CT_NUM];

    ot_u8 zone_radius;
    ot_u16 curve_l_limit;
    ot_u16 curve_r_limit;
    ot_bool extra_light_en;
    ot_u16 white_r_gain[OT_EIS_VI_AWB_LS_NUM];
    ot_u16 white_b_gain[OT_EIS_VI_AWB_LS_NUM];
    ot_u16 exp_quant[OT_EIS_VI_AWB_LS_NUM];
    ot_u8 light_status[OT_EIS_VI_AWB_LS_NUM];
    ot_u8 radius[OT_EIS_VI_AWB_LS_NUM];
    ot_bool fine_tun_en;
    ot_u8 fine_tun_strength;

} ot_scene_static_awb_ex;

typedef struct {
    ot_bool enable;                     /* RW; range: [0, 1]; format:1.0; cac enable */
    ot_u8   op_type;                 /* RW; range: [0, 1]; format:1.0; purple fringing correction working mode */
    ot_u8   detect_mode;                /* RW; range: [0, 2]; format:2.0; cac edge detect mode */
    ot_s16  purple_upper_limit;         /* RW; range: [-2048,2047]; format:s12.0; cac purple high limitation */

    ot_s16  purple_lower_limit;         /* RW; range: [-2048,2047]; format:s12.0; cac purple low limitation */
    ot_u16  edge_threshold[OT_SCENE_ACAC_THR_NUM][OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16  edge_gain[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16  cac_rb_strength[OT_EIS_VI_AUTO_ISO_NUM];        /* RW; range: [0, 31]; format:5.0; cac strength on rb */
    ot_u16  purple_alpha[OT_EIS_VI_AUTO_ISO_NUM];           /* RW; range: [0, 63]; format:6.0; cac purple alpha */
    ot_u16  edge_alpha[OT_EIS_VI_AUTO_ISO_NUM];             /* RW; range: [0, 63]; format:6.0; cac edge alpha */
    ot_u16  satu_low_threshold[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; range: [0, 4095]; format:12.0;
                                                         cac purple saturation low threshold */
    ot_u16  satu_high_threshold[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; range: [0, 16383]; format:14.0;
                                                         cac purple saturation high threshold */

    ot_u8   de_purple_cr_strength[OT_EIS_VI_CAC_EXP_RATIO_NUM]; /* RW; range: [0, 8]; format:4.0;
                                                         correction strength of the R channel */
    ot_u8   de_purple_cb_strength[OT_EIS_VI_CAC_EXP_RATIO_NUM]; /* RW; range: [0, 8]; format:4.0; */
    ot_u16  purple_detect_range;        /* RW; range: [0, 410]; format:9.0; purple fringing detection scope */
    ot_u16  var_threshold;              /* RW; range: [0, 4095]; format:12.0; edge detection threshold */

    ot_u16 r_detect_threshold[OT_EIS_VI_CAC_CURVE_NUM]; /* RW;Range:[0,4095];Format:12.0;Component R detection threshold */
    ot_u16 g_detect_threshold[OT_EIS_VI_CAC_CURVE_NUM]; /* RW;Range:[0,4095];Format:12.0;Component G detection threshold */
    ot_u16 b_detect_threshold[OT_EIS_VI_CAC_CURVE_NUM]; /* RW;Range:[0,4095];Format:12.0;Component B detection threshold */
} ot_scene_static_cac;

typedef struct {
    ot_bool enable;
    ot_eis_op_mode op_type[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u16 strength[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u16 blend_ratio[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
} ot_scene_static_dpc;

typedef struct {
    ot_u8 exp_ratio_type;
    ot_u32 exp_ratio_max;
    ot_u32 exp_ratio_min;
    ot_u32 exp_ratio[OT_EIS_VI_EXP_RATIO_NUM];
    ot_u16 tolerance;
    ot_u32 ref_ratio_up;
    ot_u32 ref_ratio_dn;
    ot_u32 exp_thr;
    ot_u32 high_light_target;
    ot_u32 exp_coef_min;
} ot_scene_static_wdr_exposure;

typedef struct {
    ot_u8 ae_exposure_cnt;
    ot_u64 exp_ltoh_thresh[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u64 exp_htol_thresh[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u32 exp_compensation[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u32 max_hist_offset[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
} ot_scene_dynamic_wdr_exposure;

typedef struct {
    ot_eis_vi_wdr_merge_mode wdr_merge_mode;
    ot_bool fusion_blend_en;
} ot_scene_static_fswdr;

typedef struct {
    ot_bool enable;
    ot_u8 ldci_op_type;
    ot_u8 gauss_lpf_sigma;
    ot_u16 tpr_incr_coef;
    ot_u16 tpr_decr_coef;
    ot_u8 auto_he_pos_wgt[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 auto_he_pos_sigma[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 auto_he_pos_mean[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 auto_he_neg_wgt[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 auto_he_neg_sigma[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 auto_he_neg_mean[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 auto_blc_ctrl[OT_EIS_VI_AUTO_ISO_NUM];
} ot_scene_static_ldci;

typedef struct {
    ot_bool enable;
} ot_scene_static_ldcihlc;

typedef struct {
    ot_bool enable;
    ot_u8 dehaze_op_type;
    ot_bool user_lut_enable;
    ot_u16 tmprflt_incr_coef;
    ot_u16 tmprflt_decr_coef;
    ot_u8 dehaze_lut[OT_EIS_VI_DEHAZE_LUT_SIZE];
} ot_scene_static_dehaze;

typedef struct {
    ot_bool enable;
    ot_eis_op_mode op_type;
    ot_eis_vi_drc_curve_select curve_select; /* RW; Range:[0x0, 0x1]; Tone mapping curve selection */
    ot_u8 purple_reduction_strength;      /* RW; Range:[0x0, 0x80]; Purple detection and reduction strength */
    ot_u8 contrast_ctrl;  /* RW; Range:[0x0, 0xF]; Format:4.0; Contrast control, smaller value yields better contrast */
    ot_u8 blend_luma_max; /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */
    ot_u8 blend_luma_bright_min;         /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */
    ot_u8 blend_luma_bright_threshold;   /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */

    ot_u8 blend_luma_dark_min;           /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */
    ot_u8 blend_luma_dark_threshold;     /* RW; Range:[0x0, 0xFF]; Luma-based filter blending weight control */

    ot_u8 blend_detail_max;              /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */
    ot_u8 blend_detail_bright_min;       /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */
    ot_u8 blend_detail_bright_threshold; /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */

    ot_u8 blend_detail_dark_min;         /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */
    ot_u8 blend_detail_dark_threshold;   /* RW; Range:[0x0, 0xFF]; Detail-based filter blending weight control */

    ot_u8  global_color_ctrl;
    ot_u16 color_correction_lut[OT_EIS_VI_DRC_CC_NODE_NUM];
} ot_scene_static_drc;


typedef struct {
    ot_u8 ae_exposure_cnt;
    ot_u32 auto_exp_ratio_max[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u32 auto_exp_ratio_min[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u64 exp_ltoh_thresh[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u64 exp_htol_thresh[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u8 auto_compensation[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u8 auto_max_hist_offset[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u32  wdr_ratio_threshold;
    ot_bool l_advance_ae;
    ot_bool h_advance_ae;
} ot_scene_dynamic_ae;

typedef struct {
    ot_u8 fps_exposure_cnt;
    ot_u64 exp_ltoh_thresh[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u64 exp_htol_thresh[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
    ot_u64 fps_set[OT_SCENE_AE_EXPOSURE_MAX_COUNT];
} ot_scene_dynamic_fps;

typedef struct {
    ot_bool crop_ae_enable;
    ot_bool crop_ae_enable_once;
    ot_u8 ae_weight_inside;
    ot_u8 ae_weight_outside;
    ot_bool crop_awb_enable;
    ot_bool crop_awb_enable_once;
    ot_u8 awb_weight_inside;
    ot_u8 awb_weight_outside;
    ot_u32 crop_mode;
    ot_u32 rect_ae_x;
    ot_u32 rect_ae_y;
    ot_u32 rect_ae_width;
    ot_u32 rect_ae_height;
    ot_u32 rect_awb_x;
    ot_u32 rect_awb_y;
    ot_u32 rect_awb_width;
    ot_u32 rect_awb_height;
} ot_scene_dynamic_vproc;

typedef struct {
    ot_u32 ref_exp_ratio;
    ot_u32 ref_ratio_alpha;
    ot_u32 k;
} ot_eis_scene_dynamic_drc_coef;

typedef struct {
    ot_u32 enable_cnt;
    ot_u8 enable[OT_SCENE_LDCI_EXPOSURE_MAX_COUNT];
    ot_u64 enable_exp_thresh_ltoh[OT_SCENE_LDCI_EXPOSURE_MAX_COUNT];
    ot_u32 exp_thresh_cnt;
    ot_u8 manual_ldci_he_pos_wgt[OT_SCENE_LDCI_EXPOSURE_MAX_COUNT];
    ot_u64 exp_thresh_ltoh[OT_SCENE_LDCI_EXPOSURE_MAX_COUNT];
} ot_scene_dynamic_ldci;

typedef struct {
    ot_u8  threshold;
    ot_u8  op_type;
    ot_u8  strength_manual;
    ot_u8  strength_auto[OT_EIS_VI_AUTO_ISO_NUM];
} ot_scene_dynamic_ldcihlc;

typedef struct {
    ot_u32  abnr_thresh_cnt;
    ot_u64  abnr_thresh[OT_EIS_SCENE_ABNR_THR_SIZE];
} ot_scene_dynamic_abnr;

typedef struct {
    ot_eis_scene_iqt_attr iqt_attr;
} ot_scene_dynamic_iqt;

#define S_IDX_LEN   17

typedef struct {
    /* ies0~3 for different frequency response. */
    ot_u32 ies0;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u32 ies1;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u32 ies2;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u32 ies3;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u32 iedz;    /* RW; Range: [0, 999]; The threshold to control the generated artifacts. */
    ot_u32 o_sht;    /* RW; Range: [0, 255]; The gains to control the overshoot. */
    ot_u32 u_sht;    /* RW; Range: [0, 255]; The gains to control the undershoot. */
} t_v200_vproc_iey;

typedef struct {
    ot_u32 spn;  /* RW; Range: [0,   6]; The selection of filters to be mixed for NO.7 filter. */
    ot_u32 sbn;  /* RW; Range: [0,   6]; The selection of filters to be mixed for NO.7 filter. */
    ot_u32 pbr;  /* RW; Range: [0,  16]; The mix ratio between spn and sbr. */
    ot_u32 j_mode;  /* RW; Range: [0,   2]; The selection modes for the blending of spatial filters. */
    ot_u32 _rb_;  /* RW; Reserved */

    ot_u32 sfr6[4];     /* RW; Range: [0, 31]; The relative NR strength for NO.6 filter. (Effective when jmode = 2). */
    ot_u32 sbr6[2];     /* RW; Range: [0, 15]; The control of overshoot and undershoot. */

    ot_u32 sfs1;        /* RW; Range: [0, 255]; The NR strength parameters for NO.1 filter. */
    ot_u32 sbr1;        /* RW; Range: [0, 255]; The NR strength parameters for NO.1 filter. */
    ot_u32 sfs2;        /* RW; Range: [0, 255]; The NR strength parameters for NO.2 filter. */
    ot_u32 sft2;        /* RW; Range: [0, 255]; The NR strength parameters for NO.2 filter. */
    ot_u32 sbr2;        /* RW; Range: [0, 255]; The NR strength parameters for NO.2 filter. */
    ot_u32 sfs4;        /* RW; Range: [0, 255]; The NR strength parameters for NO.3 and NO.4 filters. */
    ot_u32 sft4;        /* RW; Range: [0, 255]; The NR strength parameters for NO.3 and NO.4 filters. */
    ot_u32 sbr4;        /* RW; Range: [0, 255]; The NR strength parameters for NO.3 and NO.4 filters. */
    /*
    * sth1_0, sth2_0, sth3_0; Range: [0, 511]; The thresholds for protection of edges on foreground from blurring.
    * sth1_1, sth2_1, sth3_1; Range: [0, 511]; The thresholds for protection of edges on background from blurring.
    * bri_idx0, bri_idx1; Range: [0, 7]; The filter selection based on brightness
    * for foreground and background, respectively.
    */
    ot_u32 sth1_0;
    ot_u32 sth2_0;
    ot_u32 sth3_0;
    ot_u32 sth1_1;
    ot_u32 sth2_1;
    ot_u32 sth3_1;

    /*
    * sfn0_0~sfn3_0; Range: [0, 6]; Filter selection for different foreground image areas based on sth1_0~sth3_0.
    * sfn0_1~sfn3_1; Range: [0, 6]; Filter selection for different background image areas based on sth1_1~sth3_1.
    */

    ot_u32 sfn0_0;
    ot_u32 sfn1_0;
    ot_u32 sfn2_0;
    ot_u32 sfn3_0;

    ot_u32 sfn0_1;
    ot_u32 sfn1_1;
    ot_u32 sfn2_1;
    ot_u32 sfn3_1;

    ot_u32 bri_str[S_IDX_LEN]; /* RW ; Range: [0, 255];  Spatial NR strength based on brightness. */

    ot_u32 bri_idx0;     /* RW; Range: [0, 6]; The filter selection based on brightness */
    ot_u32 bri_idx1;     /* RW; Range: [0, 6]; The filter selection based on brightness */
    ot_u32 reserved1;
    ot_u32 k_mode;
    ot_u32 sbs_k[33];
    ot_u32 sds_k[33];
    ot_u32 reserved2;
} t_v200_vproc_sfy;

typedef struct {
    /* ies0~3 for different frequency response. */
    ot_u32 ies0;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u32 ies1;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u32 ies2;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u32 ies3;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u32 aux_ies;  /* RW; Range: [0, 255]; The gains of edge enhancement for NO.2 filter. */

    ot_u32 iedz;    /* RW; Range: [0, 999]; The threshold to control the generated artifacts. */
    ot_u32 _rb_;

    ot_u32 o_sht;       /* RW; Range: [0, 255]; The gains to control the overshoot. */
    ot_u32 u_sht;       /* RW; Range: [0, 255]; The gains to control the undershoot. */
    ot_u32 aux_o_sht;   /* RW; Range: [0, 255]; The gains to control the overshoot. */
    ot_u32 aux_u_sht;   /* RW; Range: [0, 255]; The gains to control the undershoot. */
} t_v200_vproc_adv_iey;

typedef struct {
    ot_u32 spn6;  /* RW; Range: [0,   5]; The selection of filters to be mixed for NO.6 filter. */
    ot_u32 sbn6;  /* RW; Range: [0,   5]; The selection of filters to be mixed for NO.6 filter. */
    ot_u32 pbr6;  /* RW; Range: [0,  16]; The mix ratio between spn and sbr. */
    ot_u32 j_mode6;  /* RW; Range: [0,   2]; The selection modes for the blending of spatial filters. */
    ot_u32 _rb_;  /* RW; Reserved */

    ot_u32 sfr6[4];     /* RW; Range: [0, 31]; The relative NR strength for NO.6 filter. (Effective when jmode = 2). */
    ot_u32 sbr6[2];     /* RW; Range: [0, 15]; The control of overshoot and undershoot. */
    ot_u32 sfs1;        /* RW; Range: [0, 15]; The NR strength parameters for NO.1 filter. */
    ot_u32 sfs2;        /* RW; Range: [0, 255]; Currently no use, reserved. */
    ot_u32 bld1;       /* RW; Range: [0, 999];  The NR strength parameters for NO.1 filter. */

    ot_u32 spn3;  /* RW; Range: [0,   5]; The selection of filters to be mixed for NO.3 filter. */
    ot_u32 sbn3;  /* RW; Range: [0,   5]; The selection of filters to be mixed for NO.3 filter. */
    ot_u32 pbr3;  /* RW; Range: [0,  16]; The mix ratio between spn and sbr. */
    ot_u32 _rb0_;

    ot_u32 spn4;  /* RW; Range: [0,   5]; The selection of filters to be mixed for NO.4 filter. */
    ot_u32 sbn4;  /* RW; Range: [0,   5]; The selection of filters to be mixed for NO.4 filter. */
    ot_u32 pbr4;  /* RW; Range: [0,  16]; The mix ratio between spn and sbr. */
    ot_u32 _rb1_;

    ot_u32 sfs5;           /* RW; Range: [0, 255]; The NR strength parameters for NO.5 filters. */
    ot_u32 sft5;           /* RW; Range: [0, 255]; The NR strength parameters for NO.5 filters. */
    ot_u32 sbr5;           /* RW; Range: [0, 255]; The NR strength parameters for NO.5 filters. */
    ot_u32 reserved2;

    ot_u32  sth1;        /* RW; Range: [0, 511]; The thresholds for protection of edges from blurring. */
    ot_u32  sth2;        /* RW; Range: [0, 511]; The thresholds for protection of edges from blurring. */
    ot_u32  sth3;        /* RW; Range: [0, 511]; The thresholds for protection of edges from blurring. */

    ot_u32 sfn0; /* RW; Range: [0, 6]; Filter selection for different foreground image areas based on sth1. */
    ot_u32 sfn1; /* RW; Range: [0, 6]; Filter selection for different foreground image areas based on sth1. */
    ot_u32 sfn2; /* RW; Range: [0, 6]; Filter selection for different foreground image areas based on sth2. */
    ot_u32 sfn3; /* RW; Range: [0, 6]; Filter selection for different foreground image areas based on sth3. */

    ot_u32   bri_str[S_IDX_LEN];   /* RW; Range: [0, 255];  Spatial NR strength based on brightness. */
    ot_u32 bri_idx;                       /* RW; Range: [0, 6]; The filter selection based on brightness */

    ot_u32 k_mode;
    ot_u32 sbs_k[33];
    ot_u32 sds_k[33];
    ot_u32 reserved3;
} t_v200_vproc_adv_sfy;

typedef struct {
    ot_u32 tfs0;   /* RW; Range: [0,  15]; The NR strength for temporal filtering. */
    ot_u32 tdz0;  /* RW; Range: [0, 999]; Protection of the weak texture area from temporal filtering. */
    ot_u32 ref_en;   /* RW; Range: [0,  1];  The switch the reference frame. */
    ot_u32 _rb0_;

    ot_u32 tfs1;   /* RW; Range: [0,  15]; The NR strength for temporal filtering. */
    ot_u32 tdz1;  /* RW; Range: [0, 999]; Protection of the weak texture area from temporal filtering. */
    ot_u32 math_mode;   /* RW; Range: [0,   1]; The motion detection mode. */
    ot_u32 _rb1_;

    ot_u32   tss0;       /* RW; Range: [0,  15]; The ratio for blending spatial NR with the temporal NR results. */
    ot_u32   tss1;       /* RW; Range: [0,  15]; The ratio for blending spatial NR with the temporal NR results. */

    ot_u32 auto_math;   /* RW; Range: [0, 999]; The motion threshold for the Level 0 denoise. */
    ot_u32 tfr0[6];      /* RW; Range: [0,  31]; The temporal NR strength control for background (static) area. */
    ot_u32 tfr1[6];      /* RW; Range: [0,  31]; The temporal NR strength control for background (static) area. */
} t_v200_vproc_tfy;

typedef struct {
    ot_u32 math0;  /* RW; Range: [0, 999]; The threshold for motion detection. */
    ot_u32 mate0;   /* RW; Range: [0,   8]; The motion index for smooth image area. */
    ot_u32 adv_math; /* RW; Range: [0,   2]; The mode selection for motion detection. */
    ot_u32 math1;  /* RW; Range: [0, 999]; The threshold for motion detection. */
    ot_u32 mate1;   /* RW; Range: [0,   8]; The motion index for smooth image area. */
    ot_u32 _rb1_;
    ot_u32   mabw0;          /* RW; Range: [5,   9]; The window size for motion detection. */
    ot_u32   mabw1;          /* RW; Range: [5,   9]; The window size for motion detection. */

    ot_u32 artc;           /* RW; Range: [0, 255]; The artifact control for motion */
    ot_u32 mcth;           /* RW; Range: [0, 999]; The sensitivity for motion */
    ot_u32 dci_w;          /* RW; Range: [0, 999]; The temporal gain for dci gain */
} t_v200_vproc_mdy;

typedef struct {
    ot_u32 sfc_enhance;     /* RW; Range: [0, 255];  The chroma NR strength for sfc. */
    ot_u32 sfc_ext;         /* RW; Range: [0, 255];  The extra chroma NR strength for sfc. */
    ot_u32 trc;             /* RW; Range: [0, 255];  The temporal chroma NR strength. */
    ot_u32 reserved;
    ot_u32 sfc;   /* RW; Range: [0, 255];  The motion threshold for chroma. */
    ot_u32 tfc;   /* RW; Range: [0, 31];   The control of temporal chroma NR strength. */
    ot_u32 _rb0_;

    ot_u32 tpc;              /* RW; Range: [0, 31];  Chroma denoise mode. */
    ot_u32 pre_sfc;          /* RW; Range: [0, 31];  pre-chroma denoise. */
} t_v200_vproc_nrc0;

typedef struct {
    ot_u32 sfs2;      /* RW; Range: [0, 255];  The NR strength parameters for NO.2 filter. */
    ot_u32 sft2;      /* RW; Range: [0, 255];  The NR strength parameters for NO.2 filter. */
    ot_u32 sbr2;      /* RW; Range: [0, 255];  The NR strength parameters for NO.2 filter. */
    ot_u32 sfc3;      /* RW; Range: [0, 255];  The chroma NR strength for NO.3 filter. */
    ot_u32 sfc_ext3;  /* RW; Range: [0, 255];  The extra chroma NR strength for NO.3 filter. */
    /* sfr3_u, sfr3_v; Range: [0, 15]; The chroma NR strength control for U and V, respectively, for NO.3 filter. */

    ot_u32 sfr3_u;
    ot_u32 sfr3_v;

    ot_u32 spn0;    /* RW; Range: [0,  3];  The selection of filters to be mixed for NO.X filter. */
    ot_u32 sbn0;    /* RW; Range: [0,  3];  The selection of filters to be mixed for NO.X filter. */
    ot_u32 pbr0;    /* RW; Range: [0, 15];  The mix ratio for NO.7 filter. */
    ot_u32 spn1;    /* RW; Range: [0,  3];  The selection of filters to be mixed for NO.X filter. */
    ot_u32 sbn1;    /* RW; Range: [0,  3];  The selection of filters to be mixed for NO.X filter. */
    ot_u32 pbr1;    /* RW; Range: [0,  15]; The mix ratio for NO.X filter. */
} t_v200_vproc_nrc1;

typedef struct {
    t_v200_vproc_adv_iey adv_ie_y[2];
    t_v200_vproc_adv_sfy adv_sf_y[2];

    t_v200_vproc_iey  iey[5];
    t_v200_vproc_sfy  sfy[5];
    t_v200_vproc_mdy  mdy[2];
    t_v200_vproc_tfy  tfy[3];
    t_v200_vproc_nrc0 nrc0;
    t_v200_vproc_nrc1 nrc1;

    ot_u32 limit_range_en;  /* RW; Range: [0, 1]; The switch for limit range mode. */
    ot_u32 nry0_en;  /* RW; Range: [0, 1]; The switch for luma denoise. */
    ot_u32 nry1_en;  /* RW; Range: [0, 1]; The switch for luma denoise. */
    ot_u32 nry2_en;  /* RW; Range: [0, 1]; The switch for luma denoise. */
    ot_u32 nry3_en;  /* RW; Range: [0, 1]; The switch for luma denoise. */
    ot_u32 nrc0_en;  /* RW; Range: [0, 1]; The switch for chroma denoise. */
    ot_u32 nrc1_en;  /* RW; Range: [0, 1]; The switch for chroma denoise. */
    ot_u32 _rb_;

    ot_u32 adv_mode;                /* RW; Range: [0, 3]; */
} ot_scene_3dnr;


typedef struct {
    ot_bool enable;
    ot_u32 threed_nr_count;
    ot_u32 threed_nr_iso[OT_SCENE_3DNR_MAX_COUNT];
    ot_scene_3dnr threednr_value[OT_SCENE_3DNR_MAX_COUNT];
} ot_scene_static_3dnr;

typedef struct {
    ot_u8 ae_weight[OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN];
} ot_scene_static_statisticscfg;

typedef struct {
    ot_u8 auto_sat[OT_EIS_VI_AUTO_ISO_NUM];
} ot_scene_static_saturation;

typedef struct {
    ot_u8 ccm_op_type;
    ot_u16 manual_ccm[OT_EIS_VI_CCM_MATRIX_SIZE];
    ot_bool auto_iso_act_en;
    ot_bool auto_temp_act_en;
    ot_u32 total_num;
    ot_u16 auto_color_temp[OT_EIS_VI_CCM_MATRIX_NUM];
    ot_u16 auto_ccm[OT_EIS_VI_CCM_MATRIX_NUM][OT_EIS_VI_CCM_MATRIX_SIZE];
} ot_scene_static_ccm;

typedef struct {
    ot_bool enable;
    ot_u8 hue;
    ot_u8 luma;
    ot_u8 contrast;
    ot_u8 saturation;
    ot_eis_img_color_space color_gamut;
} ot_scene_static_csc;

typedef struct {
    ot_u8 nddm_strength[OT_EIS_VI_AUTO_ISO_NUM];           /* RW; Range:[0x0, 0xFF]; Format:4.4; Non-direction strength */
    ot_u8 nddm_mf_detail_strength[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0x7f];
                                                           Format:3.4; Non-direction medium frequent detail
                                                           enhance strength */
    ot_u8 nddm_hf_detail_strength[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0x0, 0x10]; Format:2.2; Non-direction
                                                           high frequent detail enhance strength */
    ot_u8 detail_smooth_range[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; Range: [0x1, 0x7];
                                                           Format:4.0; Detail smooth range */
    ot_u8 color_noise_f_threshold[OT_EIS_VI_AUTO_ISO_NUM]; /* RW;Range:[0x0, 0xff]; Format:8.0;
                                                           Threshold of color noise cancel */
    ot_u8 color_noise_f_strength[OT_EIS_VI_AUTO_ISO_NUM];  /* RW;Range:[0x0, 0x8]; Format:4.0; Strength of color
                                                           noise cancel */
    ot_u8 color_noise_y_threshold[OT_EIS_VI_AUTO_ISO_NUM]; /* RW;Range:[0x0, 0xFF]; Format:8.0;Range of
	                                                       color denoise luma, related to luminance and saturation */
    ot_u8 color_noise_y_strength[OT_EIS_VI_AUTO_ISO_NUM];  /* RW;Range: [0x0, 0xFF]; Format:8.0;Format:8.0;
                                                           Strength of color denoise luma */
} ot_scene_static_dm_auto;

typedef struct {
    ot_bool enable; /* RW; Range:[0, 1]; Format:1.0;Enable/Disable demosaic module */
    ot_eis_op_mode op_type;
    ot_scene_static_dm_auto dm_auto_cfg;
} ot_scene_static_demosaic;

typedef struct {
    ot_u16  dering_strength[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8   dering_thresh[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8   dering_static_strength[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8   dering_motion_strength[OT_EIS_VI_AUTO_ISO_NUM];
} ot_scene_static_bnr_dering_snr;

typedef struct {
    ot_u16 sfm0_coarse_strength[OT_EIS_VI_BAYER_CHN_NUM][OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8  sfm6_strength[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8  sfm7_strength[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8  sth[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8  sfm1_adp_strength[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16  sfm1_strength[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 sfm0_detail_prot[OT_EIS_VI_AUTO_ISO_NUM];

    ot_u8 fine_strength[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16 coring_wgt[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 coring_mot_ratio[OT_EIS_VI_AUTO_ISO_NUM];
} ot_scene_static_bnr_snr;

typedef struct {
    ot_bool md_mode[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 md_anti_flicker_strength[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 md_static_ratio[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 md_motion_ratio[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 md_static_fine_strength[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 tfs[OT_EIS_VI_AUTO_ISO_NUM];

    ot_bool user_define_md[OT_EIS_VI_AUTO_ISO_NUM];
    ot_s16 user_define_slope[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16 user_define_dark_thresh[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 user_define_color_thresh[OT_EIS_VI_AUTO_ISO_NUM];

    ot_u8 sfr_r[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 sfr_g[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8 sfr_b[OT_EIS_VI_AUTO_ISO_NUM];
} ot_scene_static_bnr_tnr;

typedef struct {
    ot_bool enable;        /* RW; Range:[0,1];Format:1.0; bayernr enable  */
    ot_eis_op_mode op_type;    /* RW; Range:[0,1];Format:1.0; bayernr operation mode  */
    ot_bool tnr_enable;    /* RW; Range:[0,1];Format:1.0; bayernr tnr enable */
    ot_bool lsc_nr_enable; /* RW; Range:[0,1];Format:1.0; lsc nr enable */
    ot_u8 lsc_ratio1;      /* RW; Range:[0,15];Format:4.0; lsc nr ratio1 */

    ot_u16 coring_ratio[OT_EIS_VI_BAYERNR_LUT_LENGTH];
    ot_u16 mix_gain[OT_EIS_VI_BAYERNR_LUT_LENGTH1];
    ot_scene_static_bnr_snr snr_cfg;
    ot_scene_static_bnr_tnr tnr_cfg;
    ot_scene_static_bnr_dering_snr  dering_snr_cfg;
} ot_scene_static_nr;

typedef struct {
    ot_u8 luma_wgt[OT_EIS_VI_SHARPEN_LUMA_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range:[0, 31]; Format:0.5;
                                                                     Adjust the sharpen strength according to luma.
                                                                     Sharpen strength will be weaker when it decrease */
    ot_u16 texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 4095]; Format:7.5;
                                                                              Undirectional sharpen strength for texture
                                                                              and detail enhancement */
    ot_u16 edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 4095]; Format:7.5; Directional
                                                                           sharpen strength for edge enhancement */
    ot_u16 texture_freq[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 4095]; Format:6.6;Texture frequency adjustment.
                                                 Texture and detail will be finer when it increase */
    ot_u16 edge_freq[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; Range: [0, 4095]; Format:6.6;Edge frequency adjustment.
                                                 Edge will be narrower and thiner when it increase */
    ot_u8 over_shoot[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; Range: [0, 127];  Format:7.0 */
    ot_u8 under_shoot[OT_EIS_VI_AUTO_ISO_NUM];   /* RW; Range: [0, 127];  Format:7.0 */

    ot_u8 shoot_sup_strength[OT_EIS_VI_AUTO_ISO_NUM]; /* overshoot and undershoot suppression strength */
    ot_u8 shoot_sup_adj[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 15]; Format:4.0;overshoot and undershoot
                                                 suppression adjusting, adjust the edge shoot suppression strength */
    ot_u8 detail_ctrl[OT_EIS_VI_AUTO_ISO_NUM];           /* RW; Range: [0, 255]; Format:8.0;Different sharpen strength for
                                                         detail and edge. When it is bigger than 128, detail sharpen
                                                         strength will be stronger than edge. */
    ot_u8 detail_ctrl_threshold[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; Range: [0, 255]; Format:8.0; The threshold of DetailCtrl,
                                                         it is used to distinguish detail and edge. */
    ot_u8 edge_filt_strength[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; Range: [0, 63]; Format:6.0;
                                                         The strength of edge filtering. */
    ot_u8 edge_filt_max_cap[OT_EIS_VI_AUTO_ISO_NUM];     /* RW; Range: [0, 47]; Format:6.0;
                                                         The max capacity of edge filtering. */
    ot_u8 r_gain[OT_EIS_VI_AUTO_ISO_NUM];                /* RW; Range: [0, 31]; Format:5.0; Sharpen Gain for Red Area */
    ot_u8 g_gain[OT_EIS_VI_AUTO_ISO_NUM];                /* RW; Range: [0, 255]; Format:8.0; Sharpen Gain for Green Area */
    ot_u8 b_gain[OT_EIS_VI_AUTO_ISO_NUM];                /* RW; Range: [0, 31]; Format:5.0; Sharpen Gain for Blue Area */
    ot_u8 skin_gain[OT_EIS_VI_AUTO_ISO_NUM];             /* RW; Range: [0, 31]; Format:5.0;Sharpen Gain for Skin Area */
    ot_u16 max_sharp_gain[OT_EIS_VI_AUTO_ISO_NUM];       /* RW; Range: [0, 0x7FF]; Format:8.3; Maximum sharpen gain */
    ot_u8 weak_detail_gain[OT_EIS_VI_AUTO_ISO_NUM];      /* RW; Range: [0, 127]; Format:7.0; sharpen Gain for weakdetail */
} ot_scene_static_sharpen_auto;

typedef struct {
    ot_bool enable;          /* RW; Range:[0, 1]; Format:1.0;Enable/Disable sharpen module */
    ot_u8 skin_umin;         /* RW; Range: [0, 255];  Format:8.0; U min value of the range of skin area */
    ot_u8 skin_vmin;         /* RW; Range: [0, 255];  Format:8.0; V min value of the range of skin area */
    ot_u8 skin_umax;         /* RW; Range: [0, 255];  Format:8.0; U max value of the range of skin area */
    ot_u8 skin_vmax;         /* RW; Range: [0, 255];  Format:8.0; V max value of the range of skin area */

    ot_scene_static_sharpen_auto sharpen_auto_cfg;
} ot_scene_static_sharpen;

typedef struct {
    ot_u8  mf_strength[OT_SCENE_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM];   /* RW; range:[0, 63];mf_strength. */
    ot_u8  hf_strength[OT_SCENE_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM];   /* RW; range:[0, 63];hf_strength. */
    ot_u8  dark_strength[OT_SCENE_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM]; /* RW; range:[0, 63];dark_strength. */
    ot_u8  mf_gain[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; range:[0, 15];format:4.0; mf gain */
    ot_u8  hf_gain[OT_EIS_VI_AUTO_ISO_NUM];    /* RW; range:[0, 15];format:4.0; hf gain */
    ot_u8  dark_gain[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; range:[0, 15];format:4.0; dark gain */
    ot_u16 overshoot[OT_EIS_VI_AUTO_ISO_NUM];  /* RW; range:[0, 255];format:8.0; over shoot */
    ot_u16 undershoot[OT_EIS_VI_AUTO_ISO_NUM]; /* RW; range:[0, 255];format:8.0; under shoot */
} ot_scene_static_bayershp_auto;

typedef struct {
    ot_bool enable; /* RW; range:[0, 1]; format:1.0;en/disable bayershp module */
    ot_u16  dark_threshold[OT_EIS_VI_BSHP_THD_NUM];    /* RW; range:[0, 4095];format:12.0; dark threshold */
    ot_u16  texture_threshold[OT_EIS_VI_BSHP_THD_NUM]; /* RW; range:[0, 4095];format:12.0; texture threshold */
    ot_eis_op_mode op_type;
    ot_scene_static_bayershp_auto bayershp_auto_attr;
} ot_scene_static_bayershp;

typedef struct {
    ot_bool enable;
} ot_scene_static_crosstalk;

typedef struct {
    ot_bool enable;
    ot_u32 y_ratio_lut[OT_EIS_VI_CA_YRATIO_LUT_LENGTH];
    ot_u32 y_sat_lut[OT_EIS_VI_CA_YRATIO_LUT_LENGTH];
    ot_u16 iso_ratio[ISP_AUTO_ISO_CA_NUM];
} ot_scene_static_ca;

typedef struct {
    ot_u32 ratio_count;
    ot_u32 ratio_level[OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u16 mix_gain[OT_SCENE_NR_LUT_LENGTH1][OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u8 md_fusion_frame_str_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u8 snr_sfm0_fusion_frame_str_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u8 md_wdr_frame_str_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u8 snr_sfm0_wdr_frame_str_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u8 snr_fusion_sfm6_sfm7_frame_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u8 snr_wdr_sfm6_sfm7_frame_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];

    ot_u32 coring_ratio_count;
    ot_u32 coring_ratio_iso[OT_SCENE_NR_RATIO_MAX_COUNT];
    ot_u16 coring_ratio[OT_SCENE_NR_LUT_LENGTH][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 snr_sfm0_wdr_frame_str[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 snr_sfm0_fusion_frame_str[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 md_wdr_frame_str[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 md_fusion_frame_str[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 snr_wdr_sfm6_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 snr_wdr_sfm7_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 snr_fusion_sfm6_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 snr_fusion_sfm7_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];

    /* used for different exp ratio */
    ot_u16 wdr_ratio_threshold;
    ot_u8  fine_strength_l[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u8  fine_strength_h[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16 coring_wgt_l[OT_EIS_VI_AUTO_ISO_NUM];
    ot_u16 coring_wgt_h[OT_EIS_VI_AUTO_ISO_NUM];
} ot_scene_dynamic_nr;

typedef struct {
    ot_u8 md_fusion_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u8 sfm0_fusion_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u8 md_wdr_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u8 sfm0_wdr_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u8 snr_fusion_sfm6_sfm7_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u8 snr_wdr_sfm6_sfm7_incr[OT_EIS_VI_WDR_MAX_FRAME_NUM];
} ot_scene_nr_para;

typedef struct {
    ot_u32 iso_count;
    ot_u32 iso_level[OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 denoise_y_alpha[OT_EIS_SCENE_ISO_STRENGTH_NUM];        /* RW; Range:[0, 255];Format:8.0; */
    ot_u8 denoise_y_mot_ratio[OT_EIS_SCENE_ISO_STRENGTH_NUM];    /* RW; Range:[0, 63];  Format:6.0; */
    ot_u8 denoise_y_fg_str_lut[OT_SCENE_BAYERNR_DENOISE_Y_LUT_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 denoise_y_bg_str_lut[OT_SCENE_BAYERNR_DENOISE_Y_LUT_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 denoise_y_mot_map_lut[OT_SCENE_BAYERNR_DENOISE_Y_LUT_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
} ot_scene_dynamic_ynet_para;

typedef struct {
    ot_u32 iso_count;
    ot_u32 ratio_count;
    ot_u32 iso_level[OT_EIS_SCENE_ISO_STRENGTH_NUM];                                 /* iso threshold count */
    ot_u32 ratio_level[OT_SCENE_RATIO_STRENGTH_NUM];                                 /* iso threshold count */
    ot_u32 blend_weight[OT_SCENE_RATIO_STRENGTH_NUM];                                 /* iso threshold count */
    ot_u32 ca_y_ratio_iso_lut[OT_EIS_SCENE_ISO_STRENGTH_NUM][OT_EIS_VI_CA_YRATIO_LUT_LENGTH]; /* cay_ratio_lut count */
    ot_u32 ca_y_ratio_ratio_lut[OT_SCENE_RATIO_STRENGTH_NUM][OT_EIS_VI_CA_YRATIO_LUT_LENGTH]; /* cay_ratio_lut count */
} ot_scene_dynamic_ca;

typedef struct {
    ot_u8  black_level_mode;
    ot_u32 blc_count;
    ot_u32 iso_thresh[ISP_AUTO_ISO_BLC_NUM];
    ot_u32 blc_r[ISP_AUTO_ISO_BLC_NUM];
    ot_u32 blc_gr[ISP_AUTO_ISO_BLC_NUM];
    ot_u32 blc_gb[ISP_AUTO_ISO_BLC_NUM];
    ot_u32 blc_b[ISP_AUTO_ISO_BLC_NUM];
} ot_scene_dynamic_blc;

typedef struct {
    ot_bool enable;
    ot_u32 backlight_base;                 //screen backlight base,[0 , base]
    ot_u32 array_cnt;            //array valid num,[1,16]
    ot_u32 backlight_min;        //screen backlight min,[0,10000]
    ot_u32 backlight_max;        //screen backlight max,[0,10000]
    ot_u32 backlight_mode;       //0: fix, 1:ctrl by light sensor, 2:ctrl by iso
    ot_u32 light_array[16];
    ot_u32 backlight_array_light[16];
    ot_u32 iso_array[16];
    ot_u32 backlight_array_iso[16];
    ot_u32 backlight_tolerance;    //base 10000,[0,10000]
    ot_u32 backlight_conv_ratio_inc;   //base 100,[0,100]
    ot_u32 backlight_conv_ratio_dec;   //base 100,[0,100]
    ot_u32 dev_num;
    ot_u32 dev_id[16];
    ot_u32 light_sensor_num;      // 1 or 2
    ot_u32 src_addr[16];          //0x44
    ot_u32 dst_addr[16];          //0x46,0x48
    ot_bool info_print;
    ot_bool img_bright_enable;
    ot_u32 img_array_cnt;
    ot_u32 img_light_array[16];
    ot_u32 img_bright_array[16];
    ot_u16 img_bright_base[OT_EIS_VI_DRC_TM_NODE_NUM];
} ot_scene_dynamic_back_light;

typedef struct {
    ot_u8  iso_count;
    ot_u32 iso_level[ISP_AUTO_ISO_AWB_NUM];

    ot_u16 auto_low_color_temp[ISP_AUTO_ISO_AWB_NUM];
    ot_u16 auto_high_color_temp[ISP_AUTO_ISO_AWB_NUM];

    ot_eis_op_mode in_out_op_mode[ISP_AUTO_ISO_AWB_NUM];
    ot_eis_vi_awb_scene_mode_status manual_in_out_status[ISP_AUTO_ISO_AWB_NUM];
    ot_u16 low_start[ISP_AUTO_ISO_AWB_NUM];
    ot_u16 low_stop[ISP_AUTO_ISO_AWB_NUM];
    ot_u16 high_start[ISP_AUTO_ISO_AWB_NUM];
    ot_u16 high_stop[ISP_AUTO_ISO_AWB_NUM];

    ot_bool focus_en[ISP_AUTO_ISO_AWB_NUM];
    ot_u16 zone_radius[ISP_AUTO_ISO_AWB_NUM];

    ot_u8 shift_limit[ISP_AUTO_ISO_AWB_NUM];
    ot_u8 out_shift_limit[ISP_AUTO_ISO_AWB_NUM];

    ot_u16 bd_ct_th_indoor[ISP_AUTO_ISO_AWB_NUM];
    ot_u16 bd_ct_th_outdoor[ISP_AUTO_ISO_AWB_NUM];
    ot_u16 bm_ctn_ct_th[ISP_AUTO_ISO_AWB_NUM];
    ot_u16 bm_au_out_num[ISP_AUTO_ISO_AWB_NUM];
    ot_u8 bm_sft[ISP_AUTO_ISO_AWB_NUM];
} ot_scene_dynamic_awb;

typedef struct {
    ot_u8  iso_count;
    ot_u32 iso_level[ISP_AUTO_ISO_DPC_NUM];
    ot_u8  sup_twinkle_en[OT_EIS_VI_WDR_MAX_FRAME_NUM][ISP_AUTO_ISO_DPC_NUM];
    ot_u8  soft_thr[OT_EIS_VI_WDR_MAX_FRAME_NUM][ISP_AUTO_ISO_DPC_NUM];
    ot_u8  soft_slope[OT_EIS_VI_WDR_MAX_FRAME_NUM][ISP_AUTO_ISO_DPC_NUM];
    ot_u8  ratio_count;
    ot_u32 ratio_level[ISP_AUTO_ISO_DPC_NUM];
    ot_u8  bright_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM][ISP_AUTO_ISO_DPC_NUM];
    ot_u8  dark_strength[OT_EIS_VI_WDR_MAX_FRAME_NUM][ISP_AUTO_ISO_DPC_NUM];
} ot_scene_dynamic_dpc;

typedef struct {
    ot_bool enable;
} ot_scene_static_shading;

typedef struct {
    ot_u32 exp_thresh_cnt;
    ot_u64 exp_thresh_ltoh[OT_SCENE_SHADING_EXPOSURE_MAX_COUNT];
    ot_u16 mesh_strength[OT_SCENE_SHADING_EXPOSURE_MAX_COUNT];
} ot_scene_dynamic_shading;

typedef struct {
    ot_u8 option;
    ot_u32 interval;
    ot_u32 total_num;
    ot_u8 iso_count;
    ot_u8 ratio_count;
    ot_u64 exp_thresh_ltoh[OT_SCENE_GAMMA_EXPOSURE_MAX_COUNT];
    ot_u64 exp_thresh_htol[OT_SCENE_GAMMA_EXPOSURE_MAX_COUNT];
    ot_u16 table[OT_SCENE_GAMMA_EXPOSURE_MAX_COUNT][OT_EIS_VI_GAMMA_NODE_NUM];
    ot_u16 gamma_idx[OT_EIS_SCENE_ISO_STRENGTH_NUM][OT_SCENE_RATIO_STRENGTH_NUM];
} ot_scene_dynamic_gamma;

typedef enum {
    OT_ISP_BLC_NORMAL = 0,
    OT_ISP_BLC_GLOBAL,
    OT_ISP_BLC_REAL_TIME,
    OT_ISP_BLC_MODE_BUTT
} ot_isp_blc_process_sel;


typedef struct {
    ot_bool enable;
    ot_isp_blc_process_sel blc_sel;
    ot_u16 user_offset;
} ot_scene_static_blc;

typedef struct {
    ot_u8 dehzae_level_enable;
    ot_u32 exp_thresh_cnt;
    ot_u64 exp_thresh_ltoh[OT_SCENE_DEHAZE_EXPOSURE_MAX_COUNT];
    ot_u8 manual_strength[OT_SCENE_DEHAZE_EXPOSURE_MAX_COUNT];
    ot_u32 wdr_ratio_threshold;
    ot_u8 manual_strengther[OT_SCENE_DEHAZE_EXPOSURE_MAX_COUNT];
    ot_u16 dehaze_level_cnt;
    ot_u64 dehaze_level_threshold[OT_SCENE_DEHAZE_EXPOSURE_MAX_COUNT];
    ot_u64 dehaze_level_strength[OT_SCENE_DEHAZE_EXPOSURE_MAX_COUNT];
    ot_u8 dehaze_level_print;

    ot_u8 dehaze_lut_tune_en;
    ot_u8 dehaze_lut_luma_cnt;
    ot_u64 dehaze_lut_luma_thr[4];
    ot_u8 dehaze_lut_1[OT_EIS_VI_DEHAZE_LUT_SIZE];
    ot_u8 dehaze_lut_2[OT_EIS_VI_DEHAZE_LUT_SIZE];
    ot_u8 dehaze_lut_3[OT_EIS_VI_DEHAZE_LUT_SIZE];
    ot_u8 dehaze_lut_4[OT_EIS_VI_DEHAZE_LUT_SIZE];

} ot_scene_dynamic_dehaze;

typedef struct {
    ot_u8 md_thr_low_gain[OT_SCENE_RATIO_STRENGTH_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 md_thr_high_gain[OT_SCENE_RATIO_STRENGTH_NUM][OT_EIS_SCENE_ISO_STRENGTH_NUM];
} ot_scene_wdr_combine_attr;

typedef struct {
    ot_u8  ratio_count;
    ot_u32 ratio_level[ISP_AUTO_RATIO_FSWDR_NUM];
    ot_u8  wdr_merge_mode[ISP_AUTO_RATIO_FSWDR_NUM];
    ot_u8  motion_comp[ISP_AUTO_RATIO_FSWDR_NUM];

    ot_u8  iso_count;
    ot_u32 ratio_thr;
    ot_u32 iso_ltoh_thresh[OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_bool fusion_blend_en[OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 fusion_blend_wgt[OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_bool fusion_blend_en_h[OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u8 fusion_blend_wgt_h[OT_EIS_SCENE_ISO_STRENGTH_NUM];
} ot_scene_dynamic_fswdr;

typedef struct {
        ot_bool enable;
    ot_u32 ratio_count;
    ot_u32 ratio_level[OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u32 iso_count;
    ot_u32 iso_level[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u32 interval;

    ot_u8 high_saturation_color_ctrl[OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u8 bright_gain_limit[OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];
    ot_u8 bright_gain_limit_step[OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];

    ot_u8 local_mixing_bright[OT_EIS_VI_DRC_LMIX_NODE_NUM][OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u8 local_mixing_dark[OT_EIS_VI_DRC_LMIX_NODE_NUM][OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u16 color_correction_lut[OT_EIS_VI_DRC_CC_NODE_NUM][OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];

    ot_u8 tone_mapping_wgt_x[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];

    ot_u8 spatial_filter_coef[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u8 range_filter_coef[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u8 detail_adjust_coef[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u8 rim_reduction_strength[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u8 rim_reduction_threshold[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u8 dark_gain_limit_luma[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u8 dark_gain_limit_chroma[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u8 global_color_ctrl[OT_EIS_SCENE_DRC_RATIO_MAX_COUNT]; /* global color control  for CSP curve */
    ot_bool shoot_reduction_en[OT_EIS_SCENE_DRC_RATIO_MAX_COUNT];

    ot_u16 manual_str[OT_EIS_SCENE_DRC_ISO_MAX_COUNT];
    ot_u32 ref_ratio_count;
    ot_u32 ref_ratio_ltoh[OT_EIS_SCENE_DRC_REF_RATIO_MAX_COUNT];
    ot_u32 ref_ratio_alpha[OT_EIS_SCENE_DRC_REF_RATIO_MAX_COUNT];
    ot_u32 tm_ratio_threshold;
    ot_u32 curve_index_count;
    ot_u32 curve_index_ltoh[6];         //constant 6 tbd

    ot_u32 curve_speed_p3;
    ot_u32 curve_speed_p4;
    ot_u16 tm_value_low_adj[OT_EIS_VI_DRC_TM_NODE_NUM];
    ot_u16 tm_value_low[OT_EIS_VI_DRC_TM_NODE_NUM];
    ot_u16 tm_value_high[OT_EIS_VI_DRC_TM_NODE_NUM];
    ot_u16 tm_val_higher[OT_EIS_VI_DRC_TM_NODE_NUM];
    ot_u16 tm_val_highest[OT_EIS_VI_DRC_TM_NODE_NUM];
    ot_u16 tm_val_highest_adj[OT_EIS_VI_DRC_TM_NODE_NUM];

    ot_eis_scene_drc_adj_attr  drc_adj;
} ot_scene_dynamic_drc;

typedef struct {
    ot_u32 total_num;
    ot_u32 false_color_exp_thresh[OT_SCENE_FALSECOLOR_EXPOSURE_MAX_COUNT];
    ot_u8 manual_strength[OT_SCENE_FALSECOLOR_EXPOSURE_MAX_COUNT];
} ot_scene_dynamic_false_color;

typedef struct {
    ot_u32 iso_count;
    ot_bool aibnr_mode;
    ot_u32 fpn_iso_thresh;
    ot_u32 iso_thresh[OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u32 fpn_offset[OT_EIS_SCENE_ISO_STRENGTH_NUM];
} ot_scene_dynamic_fpn;

#define SCENE_STATIC_CLUT_CNT 3
typedef struct {
    ot_bool enable;
    ot_u8   lut_grp;
    ot_eis_scene_clut_table clut_table[SCENE_STATIC_CLUT_CNT];
} ot_scene_static_clut;

#define SCENE_DYNAMIC_CLUT_CNT 2
typedef struct {
    ot_u32 iso_count;
    ot_u32 iso_level[OT_EIS_SCENE_ISO_STRENGTH_NUM];

    ot_u32 gain_r[OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u32 gain_g[OT_EIS_SCENE_ISO_STRENGTH_NUM];
    ot_u32 gain_b[OT_EIS_SCENE_ISO_STRENGTH_NUM];

    ot_eis_scene_clut_table clut_table[SCENE_DYNAMIC_CLUT_CNT];
} ot_scene_dynamic_clut;

typedef struct {
    ot_bool ae_route_ex_valid;
    ot_u8 ae_run_interval;
    ot_u32 auto_sys_gain_max;
    ot_u8 auto_speed;
    ot_u8 auto_tolerance;
    ot_u16 auto_black_delay_frame;
    ot_u16 auto_white_delay_frame;
    ot_u32 auto_exp_time_max;
    ot_bool auto_antiflicker_enable;
    ot_bool hist_stat_adjust;
    ot_u32 auto_exp_time_min;
    ot_u32 frame_rate;

    ot_u32 a_gain_min;
    ot_u32 a_gain_max;
    ot_u32 d_gain_min;
    ot_u32 d_gain_max;
    ot_u32 isp_d_gain_min;
    ot_u32 isp_d_gain_max;
    ot_u32 compensation;

    ot_eis_scene_deflicker_attr deflicker_attr;
} ot_scene_static_ae;

typedef struct {
    ot_u32 mce_iso_thresh;
} ot_scene_dynamic_mce;

typedef struct {
    ot_scene_module_state module_state;
    ot_scene_static_ae static_ae;
    ot_scene_static_ae_route static_ae_route;
    ot_scene_static_ae_route_ex static_ae_route_ex;
    ot_scene_static_statisticscfg static_statistics;
    ot_scene_static_wdr_exposure static_wdr_exposure;
    ot_scene_static_fswdr static_fswdr;
    ot_scene_static_awb static_awb;
    ot_scene_static_awb_ex static_awb_ex;
    ot_scene_static_ccm static_ccm;
    ot_scene_static_saturation static_saturation;
    ot_scene_static_clut static_clut;
    ot_scene_static_ldci static_ldci;
    ot_scene_static_ldcihlc static_ldcihlc;
    ot_scene_static_drc static_drc;
    ot_scene_static_nr static_nr;
    ot_scene_static_ca static_ca;
    ot_scene_static_blc static_blc;
    ot_scene_static_cac static_cac;
    ot_scene_static_dpc static_dpc;
    ot_scene_static_dehaze static_dehaze;
    ot_scene_static_shading static_shading;
    ot_scene_static_csc staic_csc;
    ot_scene_static_crosstalk static_crosstalk;
    ot_scene_static_demosaic static_dm;
    ot_scene_static_sharpen static_sharpen;
    ot_scene_static_bayershp static_bayershp;
    ot_scene_static_3dnr static_threednr;
    ot_scene_static_gamma static_gamma;
    ot_scene_static_mce static_mce;
    ot_scene_static_back_light static_back_light;

    ot_scene_dynamic_vproc dynamic_vproc;
    ot_scene_dynamic_ae dynamic_ae;
    ot_scene_dynamic_fps dynamic_fps;
    ot_scene_dynamic_wdr_exposure dynamic_wdr_exposure;
    ot_scene_dynamic_fswdr dynamic_fswdr;
    ot_scene_dynamic_dehaze dynamic_dehaze;
    ot_scene_dynamic_drc dynamic_drc;
    ot_scene_dynamic_gamma dynamic_gamma;
    ot_scene_dynamic_nr dynamic_nr;
    ot_scene_dynamic_ca dynamic_ca;
    ot_scene_dynamic_blc dynamic_blc;
    ot_scene_dynamic_dpc dynamic_dpc;
    ot_scene_dynamic_awb dynamic_awb;
    ot_scene_dynamic_shading dynamic_shading;
    ot_scene_dynamic_clut dynamic_clut;
    ot_scene_dynamic_ldci dynamic_ldci;
    ot_scene_dynamic_ldcihlc dynamic_ldcihlc;
    ot_scene_dynamic_false_color dynamic_false_color;
    ot_scene_dynamic_fpn dynamic_fpn;
    ot_scene_dynamic_abnr dynamic_abnr;
    ot_scene_dynamic_iqt dynamic_iqt;
    ot_scene_dynamic_mce dynamic_mce;
    ot_scene_dynamic_back_light dynamic_back_light;
} ot_scene_pipe_param;

typedef struct {
    ot_eis_scene_clut_param clut_attr;
    ot_eis_scene_clut_table ori_clut;
    ot_bool ori_clut_valid;
} ot_scene_clut_param;

typedef struct {
    ot_scene_3dnr *nrx_attr;
    const ot_scene_static_3dnr *nrx_param;
    ot_u32 iso_level;
    ot_u32 mid;
    ot_u32 left;
    ot_u32 right;
} _3dnr_nrx_pack;

typedef struct {
    ot_bool enable;
    ot_bool set_compress_mode;
    ot_eis_img_compress_mode compress_mode;
} ot_scene_3dnr_param;

ot_s32 ot_scene_set_static_ae(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_gamma(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_back_light(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_dynamic_fps(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_ae(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_vproc(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);
ot_s32 ot_scene_set_static_cac(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_dpc(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_wdr_exposure(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_fswdr(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_drc(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_dehaze(ot_vi_pipe_handle pipe_hdl, ot_u8 index);
ot_s32 ot_scene_set_static_dehaze_param(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_bool enable);

ot_s32 ot_scene_set_static_ldci(ot_vi_pipe_handle pipe_hdl, ot_u8 index);
ot_s32 ot_scene_set_static_ldcihlc(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_awb(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_awbex(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_mce(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_saturation(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_ccm(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_csc(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_nr(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_shading(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_ae_weight_table(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_3dnr(ot_eis_handle vi_pipe_hdl, ot_eis_handle vproc_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_3dnr_param(ot_eis_handle vi_pipe_hdl, ot_eis_handle vproc_hdl, ot_scene_3dnr_param *param, ot_u8 index);


ot_s32 ot_scene_set_static_deflicker_param(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_bool enable);

ot_s32 ot_scene_set_static_blc(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_dynamic_3dnr(ot_eis_handle vi_pipe_hdl, ot_eis_handle vproc_hdl, ot_u32 iso, ot_u8 index, ot_eis_3dnr_pos pos_3dnr);

ot_s32 ot_scene_set_static_demosaic(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_sharpen(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_bayershp(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_clut(ot_vi_pipe_handle pipe_hdl, ot_u8 index);
ot_s32 ot_scene_set_static_clut_param(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u8 ct_value);

ot_s32 ot_scene_set_static_ca(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_static_crosstalk(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_dynamic_photo_gamma(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_video_gamma(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_shading(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_clut(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index);
ot_s32 ot_scene_set_dynamic_clut_auto(ot_vi_pipe_handle pipe_hdl, ot_eis_scene_clut_param *clut_param, ot_u8 index);

ot_s32 ot_scene_set_dynamic_linear_ca(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index);

ot_s32 ot_scene_set_dynamic_back_light(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index);

ot_s32 ot_scene_set_dynamic_fpn(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index);

ot_s32 ot_scene_set_dynamic_nr(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 wdr_ratio, ot_u32 iso);

ot_s32 ot_scene_set_dynamic_ca(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 wdr_ratio, ot_u32 iso);

ot_s32 ot_scene_set_dynamic_dpc(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index);

ot_s32 ot_scene_set_dynamic_awb(ot_vi_pipe_handle pipe_hdl, ot_u64 iso, ot_u64 last_iso, ot_u8 index);

ot_s32 ot_scene_set_dynamic_dpc_ratio(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 wdr_ratio);

ot_s32 ot_scene_set_dynamic_ldci(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_ldcihlc(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_black_level(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_false_color(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_dehaze(ot_vi_pipe_handle pipe_hdl, ot_u64 exposure, ot_u64 last_exposure, ot_u8 index);

ot_s32 ot_scene_set_dynamic_fswdr(ot_vi_pipe_handle pipe_hdl, ot_u32 iso, ot_u32 last_iso, ot_u8 index, ot_u32 wdr_ratio);

ot_s32 ot_scene_set_dynamic_drc(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 wdr_ratio, ot_u32 iso);

ot_s32 ot_scene_set_pipe_param(const ot_scene_pipe_param *scene_pipe_param, ot_u32 num);

ot_s32 ot_scene_set_pause(ot_vi_pipe_handle pipe_hdl, ot_bool pause);

ot_void set_dir_name(const ot_char *dir_name);

ot_s32 ot_scene_release_fpn(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_dynamic_abnr(ot_vi_pipe_handle pipe_hdl, ot_u8 index);

ot_s32 ot_scene_set_dynamic_iqt(ot_vi_pipe_handle pipe_hdl, ot_eis_handle task_hdl, ot_eis_video_frame *video_frame, ot_u8 index);

ot_s32 ot_scene_set_dynamic_mce(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame *video_frame, ot_u8 index);

#pragma pack()
#ifdef __cplusplus
}
#endif

#endif
