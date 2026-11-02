/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef SCENE_MEDIA_H
#define SCENE_MEDIA_H

#include "ot_scene_setparam.h"
#include "ot_scene.h"

#include "ot_eis_vi_ae.h"
#include "ot_scene_inner.h"
#include "ot_scenecomm.h"
#include "ot_scene_setparam.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_awb_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_media_sys.h"
#include "ot_scenecomm_log.h"
#include "scene_setparam_inner.h"
#include "sample_comm.h"
#include "securec.h"
#include "ot_buffer_pool.h"
#include "ot_eis_vi_api.h"
#include "ot_eis_video.h"
#include "ot_avp_npu_rts.h"
#include "ot_eis_vi_awb_api.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
  ot_bool is_started;
  sample_vi_cfg vi_cfg[OT_SCENE_PIPE_MAX_NUM];
  ot_eis_handle pool_hdl[OT_SCENE_PIPE_MAX_NUM];
  sample_vo_attr *vo_attr;
  ot_eis_handle media_pipe_hdl;
  sample_vproc_attr vproc_attr[OT_SCENE_PIPE_MAX_NUM];  
  sample_venc_attr venc_attr[OT_SCENE_PIPE_MAX_NUM];
  ot_scene_mediapipe mediapipe_param;
} scene_mediapipe_attr;

ot_s32 sample_scene_media_start(scene_mediapipe_attr *mediapipe_attr);
ot_s32 sample_scene_media_release(scene_mediapipe_attr *mediapipe_attr);

ot_s32 sample_scene_media_set_filename(sample_venc_attr* attr, FILE *save_fp[], ot_u32 pipe_id);
ot_s32 sample_scene_media_save_stream(sample_venc_attr* attr, FILE *save_fp[], ot_u32 pipe_id, 
                ot_u32 *jpeg_acquire_cnt, ot_u32 max_file_size);
ot_s32 sample_scene_get_vproc_frame_attr(ot_eis_handle vproc_chn_hdl, ot_eis_img_attr *attr);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef SCENE_MEDIA_H */
