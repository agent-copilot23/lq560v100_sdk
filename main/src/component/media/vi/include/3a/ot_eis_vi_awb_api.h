#ifndef OT_EIS_VI_AWB_API_H
#define OT_EIS_VI_AWB_API_H

#include "ot_eis_type.h"
#include "ot_eis_vi_alg.h"
#include "ot_eis_vi_awb.h"
#include "ot_eis_vi_sns.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

/* The interface of awb lib register to vi. */
ot_s32 ot_eis_vi_awb_unregister(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_eis_vi_awb_register(ot_vi_pipe_handle pipe_hdl);

/* The callback function of sensor register to awb lib. */
ot_s32 ot_eis_vi_sensor_register_to_awb(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_sensor_register_func *awb_sns_register,
                                        ot_s32 sensor_id);
ot_s32 ot_eis_vi_sensor_unregister_to_awb(ot_vi_pipe_handle pipe_hdl, ot_s32 sensor_id);

ot_s32 ot_eis_vi_awb_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_awb_attr *wb_attr);
ot_s32 ot_eis_vi_awb_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_attr *wb_attr);

ot_s32 ot_eis_vi_awb_set_ex_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_awb_ex_attr *awb_ex_attr);
ot_s32 ot_eis_vi_awb_get_ex_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_ex_attr *awb_ex_attr);

ot_s32 ot_eis_vi_awb_set_ccm_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_color_matrix_attr *ccm_attr);
ot_s32 ot_eis_vi_awb_get_ccm_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_color_matrix_attr *ccm_attr);

ot_s32 ot_eis_vi_awb_set_saturation_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_saturation_attr *sat_attr);
ot_s32 ot_eis_vi_awb_get_saturation_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_saturation_attr *sat_attr);

ot_s32 ot_eis_vi_awb_set_mode_switch_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_awb_mode_switch_attr *mode_switch_attr);
ot_s32 ot_eis_vi_awb_get_mode_switch_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_mode_switch_attr *mode_switch_attr);

ot_s32 ot_eis_vi_awb_set_rp_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_awb_ls_reproject_attr *rp_attr);
ot_s32 ot_eis_vi_awb_get_rp_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_ls_reproject_attr *rp_attr);

ot_s32 ot_eis_vi_awb_set_plck_wt_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_awb_plck_weight_attr *plck_wt_attr);
ot_s32 ot_eis_vi_awb_get_plck_wt_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_plck_weight_attr *plck_wt_attr);

ot_s32 ot_eis_vi_awb_set_anti_blue_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_awb_anti_blue_interfere_attr *anti_blue_attr);
ot_s32 ot_eis_vi_awb_get_anti_blue_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_anti_blue_interfere_attr *anti_blue_attr);

ot_s32 ot_eis_vi_awb_query_info(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_info *awb_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif
