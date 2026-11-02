/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef SCENE_THREAD_H
#define SCENE_THREAD_H

#include "ot_scene_setparam.h"
#include "ot_scene.h"
#include "scene_media.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    ot_bool thread_flag;
    pthread_t thread;
    pthread_attr_t thread_attr;
} scene_thread;

/* scene thread state */
typedef struct {
    scene_thread thread_deflicker[OT_SCENE_PIPE_MAX_NUM];
    scene_thread thread_abnr[OT_SCENE_PIPE_MAX_NUM];    
    scene_thread thread_normal[OT_SCENE_PIPE_MAX_NUM];
    scene_thread thread_clut[OT_SCENE_PIPE_MAX_NUM];
    scene_thread thread_dehaze[OT_SCENE_PIPE_MAX_NUM];
    scene_thread thread_mce[OT_SCENE_PIPE_MAX_NUM];
    scene_thread thread_venc[OT_SCENE_PIPE_MAX_NUM];
    scene_thread thread_save[OT_SCENE_PIPE_MAX_NUM];
    scene_thread thread_iqt[OT_SCENE_PIPE_MAX_NUM * OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    scene_thread thread_vp2vo[OT_SCENE_PIPE_MAX_NUM];    
    scene_thread thread_frame_pool_pipe[OT_SCENE_PIPE_MAX_NUM];
    scene_thread thread_frame_pool_chn[OT_SCENE_PIPE_MAX_NUM][OT_EIS_VPROC_GRP_CHN_MAX_NUM];
} scene_thread_state;

ot_s32 sample_scene_init(const ot_scene_param *scene_param, ot_scene_mediapipe *scene_mediapipe);
ot_s32 sample_scene_start_mediapipe(ot_void);
ot_s32 sample_scene_start_thread(ot_void);
ot_s32 sample_scene_deinit(ot_void);
ot_s32 sample_scene_refresh_param(const ot_scene_param *scene_param);
ot_s32 sample_scene_pause(ot_bool pause);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef SCENE_THREAD_H */
