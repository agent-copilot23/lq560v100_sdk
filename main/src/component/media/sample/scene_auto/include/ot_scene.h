/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef OT_SCENE_H
#define OT_SCENE_H

#include "ot_scene_setparam.h"
#include "ot_scenecomm.h"
#include "ot_scene_inner.h"
#include "ot_eis_vi_ae.h"
#include "ot_scene_loadparam.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* error code define */
#define OT_SCENE_EINVAL OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 1)       /* Invalid argument */
#define OT_SCENE_ENOTINIT OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 2)     /* Not inited */
#define OT_SCENE_ENONPTR OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 3)      /* null pointer error */
#define OT_SCENE_EOUTOFRANGE OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 4)  /* out of range */
#define OT_SCENE_EINTER OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 5)       /* Internal error */
#define OT_SCENE_EINITIALIZED OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 6) /* Already initialized */
#define OT_SCENE_SAME_FRAME OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 7)   /* same frame */
#define OT_SCENE_NO_START OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 8)     /* no start */
#define OT_SCENE_NO_SUPPORT OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 9)     /* no support */
#define OT_SCENE_INVALID_PIPE_HDL OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 10) /* invalid pipe handle */
#define OT_SCENE_INVALID_CHN_HDL OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 11) /* invalid channel handle */
#define OT_SCENE_BUF_EMPTY OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 12) /* buffer empty */
#define OT_SCENE_BUF_FULL OT_SCENECOMM_ERR_ID(OT_SCENE_MOD_SCENE, 13) /* buffer empty */

typedef ot_s32 (*ot_scene_set_static_param)(ot_eis_handle hdl, ot_u8 index);

typedef struct {
    ot_scene_pipe_param pipe_param[OT_SCENE_PIPETYPE_NUM];
} ot_scene_param;

/* pipe type */
typedef enum {
    OT_SCENE_PIPE_TYPE_SNAP = 0,
    OT_SCENE_PIPE_TYPE_VIDEO,
    OT_SCENE_PIPE_TYPE_BUTT
} ot_scene_pipe_type;

/* scene state */
typedef struct {
    ot_bool scene_init;
    ot_u32 pipe_param_idx[OT_SCENE_PIPE_MAX_NUM];
    scene_exp_info_pool exp_info[OT_SCENE_PIPE_MAX_NUM];
    ot_scene_pipe_type pipe_type[OT_SCENE_PIPE_MAX_NUM];
} ot_scene_state;

typedef struct {
    ot_u8 ct_value;
} ot_scene_ctrl_param_clut;

typedef struct {
    ot_bool enable;
} ot_scene_ctrl_param_dehaze;

typedef struct {
    ot_bool enable;
} ot_scene_ctrl_param_deflicker;

typedef struct {
    ot_bool enable;
    ot_eis_img_compress_mode compress_mode;
} ot_scene_ctrl_param_3dnr;

typedef struct {
    ot_u32 width;
    ot_u32 height;
    ot_eis_img_pixel_format pixel_format;
} ot_scene_iqt_attr;

ot_s32 ot_scene_init(const ot_scene_param *scene_param);
ot_s32 ot_scene_deinit(ot_void);
ot_s32 ot_scene_set_param(const ot_scene_param *scene_param);
ot_s32 ot_scene_set_state(const ot_vi_pipe_handle pipe_hdl[], ot_u32 pipe_num);
ot_s32 ot_scene_get_param_index(ot_vi_pipe_handle pipe_hdl, ot_s32 *index);

ot_s32 ot_scene_ctrl_clut_get_param(ot_vi_pipe_handle pipe_hdl, ot_scene_ctrl_param_clut *ctrl_param);
ot_s32 ot_scene_ctrl_clut_set_param(ot_vi_pipe_handle pipe_hdl, const ot_scene_ctrl_param_clut *ctrl_param);
ot_s32 ot_scene_ctrl_dehaze_get_param(ot_vi_pipe_handle pipe_hdl, ot_scene_ctrl_param_dehaze *ctrl_param);
ot_s32 ot_scene_ctrl_dehaze_set_param(ot_vi_pipe_handle pipe_hdl, const ot_scene_ctrl_param_dehaze *ctrl_param);
ot_s32 ot_scene_ctrl_deflicker_get_param(ot_vi_pipe_handle pipe_hdl, ot_scene_ctrl_param_deflicker *ctrl_param);
ot_s32 ot_scene_ctrl_deflicker_set_param(ot_vi_pipe_handle pipe_hdl, const ot_scene_ctrl_param_deflicker *ctrl_param);
ot_s32 ot_scene_ctrl_3dnr_get_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_grp_hdl, ot_scene_ctrl_param_3dnr *ctrl_param);
ot_s32 ot_scene_ctrl_3dnr_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_grp_hdl, const ot_scene_ctrl_param_3dnr *ctrl_param);

ot_s32 ot_scene_static_vi_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_grp_hdl);

ot_s32 ot_scene_dynamic_normal_init(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_scene_dynamic_normal_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_grp_hdl);
ot_s32 ot_scene_dynamic_normal_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_scene_dynamic_clut_init(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_scene_dynamic_clut_set_param(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_scene_dynamic_clut_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_scene_dynamic_dehaze_init(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_scene_dynamic_dehaze_set_param(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_scene_dynamic_dehaze_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_scene_dynamic_abnr_init(ot_vi_pipe_handle pipe_hdl, ot_u32 sns_type);
ot_s32 ot_scene_dynamic_abnr_set_param(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_scene_dynamic_abnr_set_roi(ot_vi_pipe_handle pipe_hdl, ot_eis_img_point *roi);
ot_s32 ot_scene_dynamic_abnr_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_scene_dynamic_iqt_init(ot_vi_pipe_handle pipe_hdl, ot_eis_handle vproc_chn_hdl, const ot_scene_iqt_attr *attr, ot_eis_handle *task_hdl);
ot_s32 ot_scene_dynamic_iqt_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_handle task_hdl, ot_eis_video_frame *video_frame);
ot_s32 ot_scene_dynamic_iqt_deinit(ot_eis_handle task_hdl);

ot_s32 ot_scene_dynamic_mce_init(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_scene_dynamic_mce_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame *video_frame);
ot_s32 ot_scene_dynamic_mce_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_scene_dynamic_deflare_init(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_scene_dynamic_deflare_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame *video_frame);
ot_s32 ot_scene_dynamic_deflare_deinit(ot_vi_pipe_handle pipe_hdl);

ot_s32 ot_scene_dynamic_deflicker_init(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_scene_dynamic_deflicker_set_param(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame *video_frame);
ot_s32 ot_scene_dynamic_deflicker_deinit(ot_vi_pipe_handle pipe_hdl);

/*
 * Pause sceneauto when isp stopped(in playback state or when reset mediaRoute)
 * Note: when Pause is true, the sceneauto thread can's set param executed
 */
ot_s32 ot_scene_pause(ot_vi_pipe_handle pipe_hdl, ot_bool enable);

/* *
 * @param[in]  dir_name : sensor ini dir
 * @param[in]  scene_param : SceneParam
 * @param[in]  video_mode : VideoMode Param
 * @param[in]  scene_mediapipe : mediapipe Param
 * @return   0 success, non-zero error code.
 * @exception    None
 */
ot_s32 ot_scene_create_param(const ot_char *dir_name, ot_scene_param *scene_param, ot_scene_mediapipe *scene_mediapipe);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef OT_SCENE_H */
