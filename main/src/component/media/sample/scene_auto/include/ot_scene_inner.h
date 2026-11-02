/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef OT_SCENE_INNER_H
#define OT_SCENE_INNER_H
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* macro define */
#define OT_SCENE_PIPE_MAX_NUM OT_EIS_VI_MAX_PIPE_NUM            /* *< pipe max count */
#define OT_SCENE_PIPETYPE_NUM 4

typedef enum {
    SCENE_THREAD_TYPE_NORMAL = 0,
    SCENE_THREAD_TYPE_DEHAZE,
    SCENE_THREAD_PIPE_TYPE_BUTT
} scene_thread_type;

typedef enum {
    SCENE_DYNAMIC_CHANGE_TYPE_EXPOSURE = 0,
    SCENE_DYNAMIC_CHANGE_TYPE_ISO,
    SCENE_DYNAMIC_PIPE_TYPE_BUTT
} scene_dynamic_change_type;

typedef struct {
    ot_u64 exposure;
    ot_u32 iso;
} scene_exposure_info;

/* mainIsp state */
typedef struct {
    ot_u32 wdr_ratio;
    scene_exposure_info exp_last_normal;
    scene_exposure_info exp_last_dehaze;
    scene_exposure_info exp_cur;
} scene_exp_info_pool;

typedef ot_s32 (*scene_set_dynamic_by_param_cb)(ot_vi_pipe_handle pipe_hdl, ot_u64 param, ot_u64 last_param, ot_u8 pipe_param_index);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef OT_SCENE_INNER_H */
