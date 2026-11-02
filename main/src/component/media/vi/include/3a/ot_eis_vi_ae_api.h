/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#ifndef OT_EIS_VI_AE_API_H
#define OT_EIS_VI_AE_API_H

#include "ot_eis_type.h"
#include "ot_eis_vi_alg.h"
#include "ot_eis_vi_ae.h"
#include "ot_eis_vi_sns.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* The interface of ae lib register to vi. */
ot_s32 ot_eis_vi_ae_register(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_eis_vi_ae_unregister(ot_vi_pipe_handle pipe_hdl);

/* The callback function of sensor register to ae lib. */
ot_s32 ot_eis_vi_sensor_register_to_ae(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_ae_sensor_register_func *pregister,
                                       ot_s32 sns_id);
ot_s32 ot_eis_vi_sensor_unregister_to_ae(ot_vi_pipe_handle pipe_hdl, ot_s32 sensor_id);

ot_s32 ot_eis_vi_ae_set_exposure_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_exposure_attr *exp_attr);
ot_s32 ot_eis_vi_ae_get_exposure_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_exposure_attr *exp_attr);

ot_s32 ot_eis_vi_ae_set_wdr_exposure_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_wdr_exposure_attr *wdr_exp_attr);
ot_s32 ot_eis_vi_ae_get_wdr_exposure_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_wdr_exposure_attr *wdr_exp_attr);

ot_s32 ot_eis_vi_ae_set_route_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_ae_route *ae_route_attr);
ot_s32 ot_eis_vi_ae_get_route_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_ae_route *ae_route_attr);

ot_s32 ot_eis_vi_ae_set_route_sf_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_ae_route *ae_route_sf_attr);
ot_s32 ot_eis_vi_ae_get_route_sf_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_ae_route *ae_route_sf_attr);

ot_s32 ot_eis_vi_ae_query_info(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_exp_info *exp_info);

ot_s32 ot_eis_vi_ae_set_route_ex_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_ae_route_ex *ae_route_attr_ex);
ot_s32 ot_eis_vi_ae_get_route_ex_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_ae_route_ex *ae_route_attr_ex);

ot_s32 ot_eis_vi_ae_set_route_sf_ex_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_ae_route_ex *ae_route_sf_attr_ex);
ot_s32 ot_eis_vi_ae_get_route_sf_ex_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_ae_route_ex *ae_route_sf_attr_ex);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
