/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef OT_EIS_SCENE_AUTO_API_H
#define OT_EIS_SCENE_AUTO_API_H

#include "ot_eis_scene_auto.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ot_s32 ot_eis_scene_drc_get_adj(ot_vi_pipe_handle pipe_hdl, const ot_eis_scene_drc_adj_attr *adj_attr, ot_u32 exposure, ot_u32 *drc_adj);

ot_s32 ot_eis_scene_haze_level_init(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_eis_scene_haze_level_get_info(ot_vi_pipe_handle pipe_hdl, ot_eis_scene_haze_level_info *info);
ot_s32 ot_eis_scene_haze_level_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_eis_scene_clut_load_table(ot_eis_scene_clut_table *clut, const ot_u8 table_data[], ot_u32 table_len);
ot_s32 ot_eis_scene_clut_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_scene_clut_param *clut_param, 
            const ot_eis_scene_clut_table clut_table[], ot_u8 clut_len);

ot_s32 ot_eis_scene_mce_init(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_eis_scene_mce_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame *video_frame);
ot_s32 ot_eis_scene_mce_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_eis_scene_mce_set_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_scene_mce_attr *attr);


ot_s32 ot_eis_scene_deflicker_init(ot_vi_pipe_handle pipe_hdl, const ot_eis_scene_deflicker_attr *deflicker_attr);
ot_s32 ot_eis_scene_deflicker_set_param(ot_vi_pipe_handle pipe_hdl, const ot_eis_video_frame *video_frame);
ot_s32 ot_eis_scene_deflicker_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_eis_scene_abnr_init(ot_vi_pipe_handle pipe_hdl, ot_eis_scene_abnr_attr *abnr_attr);
ot_s32 ot_eis_scene_abnr_set_param(ot_vi_pipe_handle pipe_hdl, ot_u64 iso);
ot_s32 ot_eis_scene_abnr_set_roi(ot_vi_pipe_handle pipe_hdl, ot_eis_img_point *roi);
ot_s32 ot_eis_scene_abnr_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_eis_scene_iqt_init(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_chn_hdl, const ot_eis_scene_iqt_attr *iqt_attr, ot_eis_handle *task_hdl);
ot_s32 ot_eis_scene_iqt_process(ot_eis_handle task_hdl, ot_eis_video_frame *video_frame);
ot_s32 ot_eis_scene_iqt_deinit(ot_eis_handle task_hdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef OT_EIS_SCENE_AUTO_API_H */
