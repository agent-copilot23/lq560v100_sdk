/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef SCENE_FRAME_POOL_H
#define SCENE_FRAME_POOL_H

#include "ot_scene_setparam.h"
#include "ot_scene.h"
#include <stdatomic.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SCENE_FP_MAX_TMP_FRAME_LEN 6
#define SCENE_FP_MAX_BUF_LEN (SCENE_FP_MAX_TMP_FRAME_LEN - 1)
#define SCENE_FRAME_POOL_INVALID_IDX -1

typedef enum {
    SCENE_FRAME_POOL_TYPE_VPROC_PIPE,
    SCENE_FRAME_POOL_TYPE_VPROC_CHN,
    SCENE_FRAME_POOL_TYPE_VPROC_PIPE_DELAY,
    SCENE_FRAME_POOL_TYPE_BUTT,
} scene_frame_pool_type;

typedef struct {
    ot_bool init;
    ot_bool debug;
    ot_u32 buf_len;        
    atomic_int rp_lock_r;
    atomic_int rp_lock_w;
    pthread_rwlock_t rwlock;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    scene_frame_pool_type frame_type;
    ot_eis_img_frame* frame_ptr[SCENE_FP_MAX_BUF_LEN]; // write idx:0, read idx:buf_len-1
    ot_bool frame_used[SCENE_FP_MAX_TMP_FRAME_LEN];
    ot_eis_img_frame frame[SCENE_FP_MAX_TMP_FRAME_LEN];
} scene_frame_pool;

ot_s32 sample_scene_fp_set_frame(scene_frame_pool *pool, ot_eis_handle hdl);
ot_eis_img_frame* sample_scene_fp_get_frame(scene_frame_pool *pool, ot_u64 pts, ot_s32 timeout_ms);
ot_s32 sample_scene_fp_init(scene_frame_pool *pool, scene_frame_pool_type type);
ot_s32 sample_scene_fp_deinit(scene_frame_pool *pool, ot_eis_handle hdl);
void sample_scene_fp_free(scene_frame_pool *pool);
ot_s32 sample_scene_fp_set_debug(scene_frame_pool *pool, ot_bool debug);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef SCENE_FRAME_POOL_H */
