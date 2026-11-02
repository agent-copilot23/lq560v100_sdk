#ifndef OT_EIS_VI_ALG_API_H
#define OT_EIS_VI_ALG_API_H

#include "ot_eis_type.h"
#include "ot_eis_video.h"
#include "ot_eis_vi_alg.h"
#include "ot_eis_vi_sns.h"
#include "ot_eis_vi_3a.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

ot_s32 ot_eis_vi_sensor_register_to_vi(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_sns_register_func *sns_register,
                                       ot_s32 sensor_id);
ot_s32 ot_eis_vi_sensor_unregister_to_vi(ot_vi_pipe_handle pipe_hdl, ot_s32 sensor_id);

ot_s32 ot_eis_vi_ae_register_callback(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_ae_register_func *ae_register);
ot_s32 ot_eis_vi_ae_unregister_callback(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_eis_vi_awb_register_callback(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_awb_register_func *awb_register);
ot_s32 ot_eis_vi_awb_unregister_callback(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_eis_vi_public_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_public_attr *public_attr);
ot_s32 ot_eis_vi_public_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_public_attr *public_attr);

ot_s32 ot_eis_vi_query_alg_state_info(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_alg_state_info *state_info);

ot_s32 ot_eis_vi_ae_get_stats(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_ae_stats *ae_stat);
ot_s32 ot_eis_vi_mg_get_stats(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_mg_stats *mg_stat);
ot_s32 ot_eis_vi_wb_get_stats(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_wb_stats *wb_stat);
ot_s32 ot_eis_vi_dehaze_get_stats(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_dehaze_stats *dehaze_stat);

ot_s32 ot_eis_vi_stats_set_cfg(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_stats_cfg *stat_cfg);
ot_s32 ot_eis_vi_stats_get_cfg(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_stats_cfg *stat_cfg);

ot_s32 ot_eis_vi_wdr_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_wdr_attr *wdr_attr);
ot_s32 ot_eis_vi_wdr_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_wdr_attr *wdr_attr);

ot_s32 ot_eis_vi_drc_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_drc_attr *drc_attr);
ot_s32 ot_eis_vi_drc_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_drc_attr *drc_attr);

ot_s32 ot_eis_vi_dehaze_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_dehaze_attr *dehaze_attr);
ot_s32 ot_eis_vi_dehaze_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_dehaze_attr *dehaze_attr);

ot_s32 ot_eis_vi_expander_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_expander_attr *expander_attr);
ot_s32 ot_eis_vi_expander_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_expander_attr *expander_attr);

ot_s32 ot_eis_vi_ldci_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_ldci_attr *ldci_attr);
ot_s32 ot_eis_vi_ldci_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_ldci_attr *ldci_attr);

ot_s32 ot_eis_vi_ldcihlc_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_ldcihlc_attr *ldcihlc_attr);
ot_s32 ot_eis_vi_ldcihlc_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_ldcihlc_attr *ldcihlc_attr);

ot_s32 ot_eis_vi_alg_set_bypass_ctrl(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_alg_bypass_ctrl *bypass_ctrl);
ot_s32 ot_eis_vi_alg_get_bypass_ctrl(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_alg_bypass_ctrl *bypass_ctrl);

ot_s32 ot_eis_vi_dpc_set_calibrate_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_dpc_static_calibrate_attr *dp_calibrate);
ot_s32 ot_eis_vi_dpc_get_calibrate_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_dpc_static_calibrate_attr *dp_calibrate);

ot_s32 ot_eis_vi_dpc_set_static_cor_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_dpc_static_cor_attr *dp_static_attr);
ot_s32 ot_eis_vi_dpc_get_static_cor_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_dpc_static_cor_attr *dp_static_attr);

ot_s32 ot_eis_vi_dpc_set_dynamic_cor_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_dpc_dynamic_cor_attr *dp_dynamic_attr);
ot_s32 ot_eis_vi_dpc_get_dynamic_cor_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_dpc_dynamic_cor_attr *dp_dynamic_attr);

ot_s32 ot_eis_vi_lblc_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_lblc_attr *lblc_attr);
ot_s32 ot_eis_vi_lblc_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_lblc_attr *lblc_attr);

ot_s32 ot_eis_vi_lblc_set_lut_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_lblc_lut_attr *lblc_lut_attr);
ot_s32 ot_eis_vi_lblc_get_lut_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_lblc_lut_attr *lblc_lut_attr);

ot_s32 ot_eis_vi_mlsc_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_shading_attr *shading_attr);
ot_s32 ot_eis_vi_mlsc_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_shading_attr *shading_attr);

ot_s32 ot_eis_vi_mlsc_set_gain_lut_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_shading_lut_attr *shading_lut_attr);
ot_s32 ot_eis_vi_mlsc_get_gain_lut_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_shading_lut_attr *shading_lut_attr);

ot_s32 ot_eis_vi_acs_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_acs_attr *acs_attr);
ot_s32 ot_eis_vi_acs_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_acs_attr *acs_attr);

ot_s32 ot_eis_vi_nr_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_nr_attr *nr_attr);
ot_s32 ot_eis_vi_nr_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_nr_attr *nr_attr);

ot_s32 ot_eis_vi_color_tone_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_color_tone_attr *color_tone_attr);
ot_s32 ot_eis_vi_color_tone_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_color_tone_attr *color_tone_attr);

ot_s32 ot_eis_vi_gamma_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_gamma_attr *gamma_attr);
ot_s32 ot_eis_vi_gamma_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_gamma_attr *gamma_attr);

ot_s32 ot_eis_vi_cac_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_cac_attr *cac_attr);
ot_s32 ot_eis_vi_cac_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_cac_attr *cac_attr);

ot_s32 ot_eis_vi_bayer_sharpen_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_bayer_sharpen_attr *bshp_attr);
ot_s32 ot_eis_vi_bayer_sharpen_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_bayer_sharpen_attr *bshp_attr);

ot_s32 ot_eis_vi_rc_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_rc_attr *rc_attr);
ot_s32 ot_eis_vi_rc_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_rc_attr *rc_attr);

ot_s32 ot_eis_vi_yuv_sharpen_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_sharpen_attr *shp_attr);
ot_s32 ot_eis_vi_yuv_sharpen_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_sharpen_attr *shp_attr);

ot_s32 ot_eis_vi_crosstalk_removal_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_cr_attr *cr_attr);
ot_s32 ot_eis_vi_crosstalk_removal_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_cr_attr *cr_attr);

ot_s32 ot_eis_vi_ca_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_ca_attr *ca_attr);
ot_s32 ot_eis_vi_ca_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_ca_attr *ca_attr);

ot_s32 ot_eis_vi_anti_false_color_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_anti_false_color_attr *anti_false_color);
ot_s32 ot_eis_vi_anti_false_color_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_anti_false_color_attr *anti_false_color);

ot_s32 ot_eis_vi_demosaic_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_demosaic_attr *demosaic_attr);
ot_s32 ot_eis_vi_demosaic_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_demosaic_attr *demosaic_attr);

ot_s32 ot_eis_vi_black_level_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_black_level_attr *black_level);
ot_s32 ot_eis_vi_black_level_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_black_level_attr *black_level);

ot_s32 ot_eis_vi_clut_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_clut_attr *clut_attr);
ot_s32 ot_eis_vi_clut_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_clut_attr *clut_attr);
ot_s32 ot_eis_vi_clut_set_coeff(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_clut_lut *clut_lut);
ot_s32 ot_eis_vi_clut_get_coeff(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_clut_lut *clut_lut);

ot_s32 ot_eis_vi_csc_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_csc_attr *csc_attr);
ot_s32 ot_eis_vi_csc_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_csc_attr *csc_attr);

/* block, return fpn frame when this is function is called. */
ot_s32 ot_eis_vi_fpn_set_calibrate_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_fpn_calibrate_attr *calibrate_attr);
ot_s32 ot_eis_vi_fpn_set_cor_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_fpn_cor_attr *fpn_attr);
ot_s32 ot_eis_vi_fpn_get_cor_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_fpn_cor_attr *fpn_attr);

ot_s32 ot_eis_vi_set_sns_slave_attr(ot_slave_dev_id slave_dev, const ot_eis_vi_slave_sns_sync_attr *sns_sync_attr);
ot_s32 ot_eis_vi_get_sns_slave_attr(ot_slave_dev_id slave_dev, ot_eis_vi_slave_sns_sync_attr *sns_sync_attr);

ot_s32 ot_eis_vi_dither_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_dither_attr *dither_attr);
ot_s32 ot_eis_vi_dither_get_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_dither_attr *dither_attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_VI_ALG_API_H */
