#ifndef OT_EIS_IQT_H
#define OT_EIS_IQT_H

#include "ot_eis_type.h"
#include "ot_avp_npu_rts.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#define OT_EIS_IQT_MAX_TASK_NUM 8
#define OT_EIS_IQT_NR_THRES_SIZE 7
#define OT_EIS_IQT_DFL_HIST_THRES_SIZE 12
#define OT_EIS_IQT_DFL_EXP_THRES_SIZE 4
#define OT_EIS_IQT_DFL_AVG_THRES_SIZE 4
#define OT_EIS_IQT_DFL_BLD_WEIGHT_SIZE 3
#define OT_EIS_IQT_ECO_THRES_SIZE 2
#define OT_EIS_IQT_MAX_RESOLUTION_TYPE_NUM 2

typedef struct {
    ot_eis_img_size input_size;
    ot_eis_img_pixel_format pixel_format;
} ot_eis_iqt_input_params;

typedef struct {
    ot_u32                 thresholds[OT_EIS_IQT_NR_THRES_SIZE];
} ot_eis_iqt_nr_params;

typedef struct {
    ot_u8   strength;
    ot_u16  start_th;
    ot_u32  exposure_th[OT_EIS_IQT_DFL_EXP_THRES_SIZE];
    ot_u32  avglight_th[OT_EIS_IQT_DFL_AVG_THRES_SIZE];
    ot_u32  hist_refer[OT_EIS_IQT_DFL_HIST_THRES_SIZE];
    ot_u32  hist_delta[OT_EIS_IQT_DFL_HIST_THRES_SIZE];
    ot_u16  hist_weight[OT_EIS_IQT_DFL_HIST_THRES_SIZE];
    ot_u16  blending_weight[OT_EIS_IQT_DFL_BLD_WEIGHT_SIZE];
} ot_eis_iqt_dfl_params;

typedef struct {
    ot_u8                   mode;
    ot_bool                 manual_enable;
    ot_u8                   ynr_manual_strength;
    ot_u8                   cnr_manual_strength;
    ot_u8                   dfl_manual_strength;
    ot_u32                  eco_thresholds[OT_EIS_IQT_ECO_THRES_SIZE];
    ot_eis_iqt_nr_params    nr_params;
    ot_eis_iqt_dfl_params   dfl_params;
    ot_eis_iqt_input_params input_params;
    ot_avp_npu_mdl_config   mdl_config;
} ot_eis_iqt_params;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_IQT_H */