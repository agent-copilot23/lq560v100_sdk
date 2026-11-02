/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef OT_EIS_SCENE_AUTO_H
#define OT_EIS_SCENE_AUTO_H

#include "ot_eis_type.h"
#include "ot_eis_mod.h"
#include "ot_eis_errno.h"
#include "ot_eis_video.h"
#include "ot_eis_vi_alg.h"
#include "ot_eis_scene_errno.h"
#include "ot_avp_npu_rts.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OT_EIS_SCENE_CLUT_GAIN_LEN 3
#define OT_EIS_SCENE_DRC_ADJ_LEN 4

typedef struct {
    ot_u32 min_exposure;
    ot_u32 max_exposure;
    ot_u32 low_exp_adj_count;
    ot_u32 low_exp_adj_ltoh[OT_EIS_SCENE_DRC_ADJ_LEN];         //constant 4 tbd
    ot_u32 low_exp_adj_ratio[OT_EIS_SCENE_DRC_ADJ_LEN];
    ot_u32 high_exp_adj_count;
    ot_u32 high_exp_adj_ltoh[OT_EIS_SCENE_DRC_ADJ_LEN];         //constant 4 tbd
    ot_u32 high_exp_adj_ratio[OT_EIS_SCENE_DRC_ADJ_LEN];
} ot_eis_scene_drc_adj_attr;

typedef struct {
    ot_u32 gain[OT_EIS_SCENE_CLUT_GAIN_LEN];
    ot_u32 lut[OT_EIS_VI_CLUT_LUT_LENGTH];
} ot_eis_scene_clut_table;

typedef struct {
    ot_u32 deflicker_min_freq; 
    ot_u32 deflicker_sensor_T;
    ot_u32 deflicker_running_interval;
    ot_u32 deflicker_exposure_l_thresh;
    ot_u32 deflicker_exposure_h_thresh;   
    ot_u32 deflicker_reset_ratio_en;
    ot_u32 deflicker_reset_ratio_u;
    ot_u32 deflicker_reset_ratio_d;
    ot_u32 deflicker_reset_filter;
} ot_eis_scene_deflicker_attr;

#define OT_EIS_SCENE_MCE_STATUS_SIZE 64
typedef struct {    
    ot_bool status_lists[OT_EIS_VI_MAX_PIPE_NUM][OT_EIS_SCENE_MCE_STATUS_SIZE];
    ot_u32 iso_outside_mce[OT_EIS_VI_MAX_PIPE_NUM];
    ot_u8 last_trans_coeff[OT_EIS_VI_MAX_PIPE_NUM];
    ot_u16 ct_retreat_ctn[OT_EIS_VI_MAX_PIPE_NUM];
} ot_eis_scene_clut_param;

typedef struct {
    ot_bool mce_enable;       
    ot_u32 mce_start_iso;            /* RW; control working iso range; */
    ot_u32 mce_stop_iso;             /* RW; control working iso range; */
    ot_u16 mce_start_ct;             /* RW; control working ct range; */
    ot_u16 mce_stop_ct;              /* RW; control working ct range; */
    ot_u16 mce_retreat_ct_thresh;    /* RW; control working ct range; */
    ot_u16 mce_retreat_ct_list_size; /* RW; control working ct range; */
    ot_u8 mce_alpha_thresh_top;      /* RW; stable clut when alpha exceeds this thresh; */
    ot_u8 mce_alpha_thresh_bot;      /* RW; stable clut when alpha is less than this thresh; */
    ot_u8 mce_list_size;             /* RW; the size of the status list, bigger the size is, slower the color change and more smooth; */
} ot_eis_scene_mce_attr;

typedef struct {
    ot_u32 haze_level;
} ot_eis_scene_haze_level_info;

typedef enum {
    OT_EIS_SCENE_SENSOR_NONE,
    OT_EIS_SCENE_SENSOR_IMX623,
    OT_EIS_SCENE_SENSOR_OX03C10,
    OT_EIS_SCENE_SENSOR_OX03F10,
    OT_EIS_SCENE_SENSOR_BUTT,
} ot_eis_scene_sensor;

#define OT_EIS_SCENE_ABNR_THR_SIZE 12
typedef struct {
    ot_u32  thresh_cnt;
    ot_u64  thresh[OT_EIS_SCENE_ABNR_THR_SIZE];
    ot_eis_scene_sensor sensor;
    ot_eis_img_rect roi_rect;
} ot_eis_scene_abnr_attr;


#define OT_EIS_SCENE_IQT_NR_THR_SIZE 7
#define OT_EIS_SCENE_IQT_DFL_HIST_THR_SIZE 12
#define OT_EIS_SCENE_IQT_DFL_HIST_THRES_SIZE 12
#define OT_EIS_SCENE_IQT_DFL_EXP_THRES_SIZE 4
#define OT_EIS_SCENE_IQT_DFL_AVG_THRES_SIZE 4
#define OT_EIS_SCENE_IQT_DFL_BLD_WEIGHT_SIZE 3
#define OT_EIS_SCENE_IQT_ECO_THRES_SIZE 2

typedef struct {
    ot_eis_img_size input_size;
    ot_eis_img_pixel_format pixel_format;
} ot_eis_scene_input_attr;

typedef struct {
    ot_u32 thresholds[OT_EIS_SCENE_IQT_NR_THR_SIZE];
} ot_eis_scene_nr_attr;

typedef struct {
    ot_u8   strength;
    ot_u16  start_th;
    ot_u32  exposure_th[OT_EIS_SCENE_IQT_DFL_EXP_THRES_SIZE];
    ot_u32  avglight_th[OT_EIS_SCENE_IQT_DFL_AVG_THRES_SIZE];
    ot_u32  hist_refer[OT_EIS_SCENE_IQT_DFL_HIST_THR_SIZE];
    ot_u32  hist_delta[OT_EIS_SCENE_IQT_DFL_HIST_THR_SIZE];
    ot_u16  hist_weight[OT_EIS_SCENE_IQT_DFL_HIST_THR_SIZE];
    ot_u16  blending_weight[OT_EIS_SCENE_IQT_DFL_BLD_WEIGHT_SIZE];
} ot_eis_scene_dfl_attr;

typedef struct {
    ot_u8                   mode;
    ot_bool                 manual_enable;
    ot_u8                   ynr_manual_strength;
    ot_u8                   cnr_manual_strength;
    ot_u8                   dfl_manual_strength;
    ot_u32                  eco_thresholds[OT_EIS_SCENE_IQT_ECO_THRES_SIZE];
    ot_eis_scene_nr_attr    nr_attr;
    ot_eis_scene_dfl_attr   dfl_attr; 
    ot_eis_scene_input_attr input_attr;
    ot_avp_npu_mdl_config   mdl_config;
} ot_eis_scene_iqt_attr;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef OT_EIS_SCENE_AUTO_H */
