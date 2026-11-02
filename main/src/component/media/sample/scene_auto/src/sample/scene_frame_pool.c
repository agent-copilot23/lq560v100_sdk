/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include "ot_scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <sys/syscall.h>

#include "scene_frame_pool.h"
#include "scene_auto.h"
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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SCENE_FRAME_POOL_DEBUG 0

static ot_s32 get_free_frame_idx_from_pool(scene_frame_pool *pool)
{
    ot_scenecomm_check_pointer_return(pool, SCENE_FRAME_POOL_INVALID_IDX);
    ot_scenecomm_expr_true_return(pool->init == OT_FALSE, SCENE_FRAME_POOL_INVALID_IDX);

    for (int i = 0; i < SCENE_FP_MAX_TMP_FRAME_LEN; i++) {
        if (pool->frame_used[i] == OT_FALSE) {
            return i;
        }
    }
    return SCENE_FRAME_POOL_INVALID_IDX;
}

ot_s32 sample_scene_fp_set_debug(scene_frame_pool *pool, ot_bool debug)
{
    ot_scenecomm_check_pointer_return(pool, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(pool->init == OT_FALSE, OT_SCENE_ENOTINIT);
    pool->debug = debug;
    return OT_SUCCESS;
}

ot_eis_img_frame* sample_scene_fp_get_frame(scene_frame_pool *pool, ot_u64 pts, ot_s32 timeout_ms)
{
    ot_scenecomm_check_pointer_return(pool, OT_NULL);
    ot_scenecomm_expr_true_return(pool->init == OT_FALSE, OT_NULL);
    ot_scenecomm_expr_true_return(pool->buf_len > SCENE_FP_MAX_BUF_LEN, OT_NULL);
    
    ot_rwlock_rdlock(pool->rwlock);
    ot_eis_img_frame* read_ptr = pool->frame_ptr[pool->buf_len - 1];

    if (!(read_ptr != NULL && pool->rp_lock_w == 0 && read_ptr->pts != pts)) {
        // wait for new frame
        ot_rwlock_unlock(pool->rwlock);
        ot_cond_timedwait(pool->cond, pool->mutex, timeout_ms);
        ot_rwlock_rdlock(pool->rwlock);
        read_ptr = pool->frame_ptr[pool->buf_len - 1];
    }
        
    if (read_ptr != NULL && pool->rp_lock_w == 0 && read_ptr->pts != pts) {
        atomic_fetch_add(&pool->rp_lock_r, 1);
        ot_rwlock_unlock(pool->rwlock);
#if SCENE_FRAME_POOL_DEBUG        
        printf("--> get_frame pid:%lu rp_lock_w:%d prev_pts:%llu rd_pts:%llu index:%d 1.get succ\n", 
            syscall(SYS_gettid), pool->rp_lock_w, pts, read_ptr->pts, read_ptr->index);
#endif        
        return read_ptr;
    }
#if SCENE_FRAME_POOL_DEBUG     
    printf("--> get_frame pid:%lu rp_lock_w:%d prev_pts:%llu rd_pts:%llu 1.nothing\n", syscall(SYS_gettid), 
            pool->rp_lock_w, pts, read_ptr != OT_NULL ? read_ptr->pts : 0);
#endif            
    ot_rwlock_unlock(pool->rwlock);
    return OT_NULL;
}

static ot_s32 release_frame_to_pool(scene_frame_pool *pool, ot_eis_handle hdl, ot_eis_img_frame* frame)
{
    ot_scenecomm_check_pointer_return(pool, OT_SCENE_ENONPTR);
    ot_scenecomm_check_pointer_return(frame, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(pool->frame_type > SCENE_FRAME_POOL_TYPE_BUTT, OT_SCENE_EOUTOFRANGE);
    ot_scenecomm_expr_true_return(pool->init == OT_FALSE, OT_SCENE_ENOTINIT);

    if (pool->frame_type == SCENE_FRAME_POOL_TYPE_VPROC_PIPE ||
        pool->frame_type == SCENE_FRAME_POOL_TYPE_VPROC_PIPE_DELAY) {
        ot_eis_vproc_pipe_release_frame(hdl, frame);
    } else {
        ot_eis_vproc_chn_release_frame(hdl, frame);
    }
    
    for (int i = 0; i < SCENE_FP_MAX_TMP_FRAME_LEN; i++) {
        if (&pool->frame[i] == frame) {
            pool->frame_used[i] = OT_FALSE;
            break;
        }
    }

    return OT_SUCCESS;
}

void sample_scene_fp_free(scene_frame_pool *pool)
{
    if (pool == OT_NULL || 
        pool->init == OT_FALSE ||
        pool->buf_len > SCENE_FP_MAX_BUF_LEN) {
        return;
    };
    
    ot_rwlock_rdlock(pool->rwlock);
    if (pool->rp_lock_r > 0) {        
        atomic_fetch_sub(&pool->rp_lock_r, 1);
#if SCENE_FRAME_POOL_DEBUG
        printf("--> free_frame pid:%lu rd_pts:%llu rp_lock_r:%d 1.r_lock--\n", syscall(SYS_gettid), 
            pool->frame_ptr[pool->buf_len - 1] != OT_NULL ? pool->frame_ptr[pool->buf_len - 1]->pts : 0, pool->rp_lock_r);
#endif            
    }
    ot_rwlock_unlock(pool->rwlock);
}

static ot_bool scene_is_same_frame(scene_frame_pool *pool, ot_eis_img_frame* frame)
{
    ot_scenecomm_check_pointer_return(pool, OT_FALSE);
    ot_scenecomm_check_pointer_return(frame, OT_FALSE);
    ot_scenecomm_expr_true_return(pool->buf_len > SCENE_FP_MAX_BUF_LEN || pool->buf_len < 2, OT_SCENE_EOUTOFRANGE);

    for (int i = 0; i < pool->buf_len && i < SCENE_FP_MAX_BUF_LEN; i++) {
        if ((pool->frame_ptr[i] != OT_NULL && pool->frame_ptr[i]->pts == frame->pts)) {
#if SCENE_FRAME_POOL_DEBUG             
        printf("--> set_frame pid:%lu saved_pts:%llu cur_pts:%llu 2.same pts, release cur\n", syscall(SYS_gettid),
                pool->frame_ptr[i]->pts, frame->pts);
#endif             
            return OT_TRUE;
        }
    }

    return OT_FALSE;
}

static ot_s32 scene_get_frame_from_media(scene_frame_pool *pool, ot_eis_handle hdl, ot_eis_img_frame** frame)
{
    ot_scenecomm_check_pointer_return(pool, OT_SCENE_ENONPTR);
    ot_scenecomm_check_pointer_return(frame, OT_SCENE_ENONPTR);

    ot_s32 ret = OT_FAILURE;
    ot_s32 frame_timeout = 3000;
    ot_s32 idx = get_free_frame_idx_from_pool(pool);    
    ot_scenecomm_expr_true_return(idx == SCENE_FRAME_POOL_INVALID_IDX, OT_SCENE_BUF_FULL);

    if (pool->frame_type == SCENE_FRAME_POOL_TYPE_VPROC_PIPE ||
        pool->frame_type == SCENE_FRAME_POOL_TYPE_VPROC_PIPE_DELAY) {
        ret = ot_eis_vproc_pipe_acquire_frame(hdl, &pool->frame[idx], frame_timeout);
    } else {
        ret = ot_eis_vproc_chn_acquire_frame(hdl, &pool->frame[idx], frame_timeout);
    }
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

#if SCENE_FRAME_POOL_DEBUG         
        printf("--> set_frame pid:%lu hdl:%llu cur_pts:%llu index:%d 1.acquire\n", syscall(SYS_gettid), 
                (ot_u64)hdl, pool->frame[idx].pts, pool->frame[idx].index);
#endif    
    // check same frame
    if (scene_is_same_frame(pool, &pool->frame[idx])) {        
        release_frame_to_pool(pool, hdl, &pool->frame[idx]);         
        return OT_SCENE_SAME_FRAME;
    }
    
    *frame = &pool->frame[idx];
    pool->frame_used[idx] = OT_TRUE;
    return OT_SUCCESS;
}

static ot_s32 scene_save_frame_to_pool(scene_frame_pool *pool, ot_eis_handle hdl, ot_eis_img_frame* frame)
{
    ot_scenecomm_check_pointer_return(pool, OT_SCENE_ENONPTR);
    ot_scenecomm_check_pointer_return(frame, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(pool->buf_len > SCENE_FP_MAX_BUF_LEN || pool->buf_len < 2, OT_SCENE_EOUTOFRANGE);;
    
    // release write_ptr if not null
    if (pool->frame_ptr[0] != OT_NULL) { 
#if SCENE_FRAME_POOL_DEBUG               
        printf("--> set_frame pid:%lu hdl:%llu prev_pts:%llu cur_pts:%llu 3.diff pts and release prev\n", syscall(SYS_gettid), (ot_u64)hdl, 
                pool->frame_ptr[0]->pts, frame->pts);
#endif                    
        release_frame_to_pool(pool, hdl, pool->frame_ptr[0]);
        pool->frame_ptr[0] = OT_NULL;       
    }
    
    // save to write_ptr
    pool->frame_ptr[0] = frame;
        
    if (pool->debug && 
        pool->frame_ptr[1] != OT_NULL &&
        pool->frame_ptr[1]->index != pool->frame_ptr[0]->index - 2) {
        printf("vp frame loss. pid:%lu hdl:%llu last:%d cur:%d\n", syscall(SYS_gettid), (ot_u64)hdl, pool->frame_ptr[1]->index, pool->frame_ptr[0]->index);
    }
    
    // move to next buf ptr
    for (int i = pool->buf_len - 1; i > 0; i--) {
        if (i == pool->buf_len - 1) {
            // read_ptr
            if (pool->frame_ptr[i] == OT_NULL || pool->rp_lock_r == 0) {
                // read_ptr is NULL or unlock
                ot_rwlock_wrlock(pool->rwlock);                
                atomic_fetch_add(&pool->rp_lock_w, 1);
                
                if (pool->rp_lock_r == 0 && pool->frame_ptr[i] != OT_NULL) {
                    // release read_ptr before move
#if SCENE_FRAME_POOL_DEBUG                 
                    printf("--> set_frame pid:%lu hdl:%llu wrlock:%d 4.release\n", 
                            syscall(SYS_gettid), (ot_u64)hdl, pool->rp_lock_w);
#endif
                    release_frame_to_pool(pool, hdl, pool->frame_ptr[i]); 
                    pool->frame_ptr[i] = OT_NULL;
                } 
#if SCENE_FRAME_POOL_DEBUG                
                else {
                    printf("--> set_frame pid:%lu hdl:%llu 5.move directly \n", syscall(SYS_gettid), (ot_u64)hdl);  
                }
#endif
                pool->frame_ptr[i] = pool->frame_ptr[i - 1];
                pool->frame_ptr[i - 1] = OT_NULL;
                atomic_fetch_sub(&pool->rp_lock_w, 1); 
                ot_rwlock_unlock(pool->rwlock);

                // Wake up the waiting read thread
                ot_cond_signal(pool->cond, pool->mutex);
            }
        } else {
            // others buf ptr
            if (i == pool->buf_len - 2 && pool->frame_ptr[i] != OT_NULL) {
                // release second to last buf ptr before move
#if SCENE_FRAME_POOL_DEBUG                 
                printf("--> set_frame pid:%lu hdl:%llu rd_pts:%llu 4.release\n", 
                        syscall(SYS_gettid), (ot_u64)hdl, pool->frame_ptr[i]->pts);
#endif
                release_frame_to_pool(pool, hdl, pool->frame_ptr[i]); 
                pool->frame_ptr[i] = OT_NULL;
            }
#if SCENE_FRAME_POOL_DEBUG
            else {
                printf("--> set_frame pid:%lu hdl:%llu 5.move directly \n", syscall(SYS_gettid), (ot_u64)hdl);  
            }
#endif            
            pool->frame_ptr[i] = pool->frame_ptr[i - 1];
            pool->frame_ptr[i - 1] = OT_NULL;
        }
    }
    
    return OT_SUCCESS;
}

ot_s32 sample_scene_fp_set_frame(scene_frame_pool *pool, ot_eis_handle hdl)
{
    ot_scenecomm_check_pointer_return(pool, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(pool->frame_type >= SCENE_FRAME_POOL_TYPE_BUTT, OT_SCENE_EOUTOFRANGE);
    ot_scenecomm_expr_true_return(pool->init == OT_FALSE, OT_SCENE_ENOTINIT);
    ot_scenecomm_expr_true_return(pool->buf_len > SCENE_FP_MAX_BUF_LEN || pool->buf_len < 2, OT_SCENE_EOUTOFRANGE);

    ot_s32 ret = OT_FAILURE;
    ot_eis_img_frame* frame = OT_NULL;

    ret = scene_get_frame_from_media(pool, hdl, &frame);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    
    ret = scene_save_frame_to_pool(pool, hdl, frame);
    ot_scenecomm_check_return(ret, ret);
    
    return OT_SUCCESS;
}

ot_s32 sample_scene_fp_init(scene_frame_pool *pool, scene_frame_pool_type type)
{
    ot_scenecomm_check_pointer_return(pool, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(type >= SCENE_FRAME_POOL_TYPE_BUTT, OT_SCENE_EOUTOFRANGE);
    ot_scenecomm_expr_true_return(pool->init == OT_TRUE, OT_SCENE_EINITIALIZED);

    atomic_init(&pool->rp_lock_r, 0);
    atomic_init(&pool->rp_lock_w, 0);
    
    ot_cond_init(pool->cond, pool->mutex);
    
    if (type == SCENE_FRAME_POOL_TYPE_VPROC_PIPE_DELAY) {
        pool->buf_len = 3;
    } else {
        pool->buf_len = 2;
    }
    for (int i = 0; i < SCENE_FP_MAX_BUF_LEN; i++) {
        pool->frame_ptr[i] = OT_NULL;
    }
    pool->frame_type = type;    
    pool->init = OT_TRUE;

    return OT_SUCCESS;
}

ot_s32 sample_scene_fp_deinit(scene_frame_pool *pool, ot_eis_handle hdl)
{
    ot_scenecomm_check_pointer_return(pool, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(pool->init == OT_FALSE, OT_SCENE_ENOTINIT);

    for (int i = 0; i < SCENE_FP_MAX_TMP_FRAME_LEN; i++) {
        if (pool->frame_used[i]) {
            release_frame_to_pool(pool, hdl, &pool->frame[i]);            
        }
    }
    
    for (int i = 0; i < SCENE_FP_MAX_BUF_LEN; i++) {
        pool->frame_ptr[i] = OT_NULL;
    }
    ot_rwlock_destroy(pool->rwlock);
    ot_cond_destroy(pool->cond, pool->mutex);
    pool->init = OT_FALSE;
    return OT_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
