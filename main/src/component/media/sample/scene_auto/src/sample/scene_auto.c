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
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <dlfcn.h>
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
#include "scene_media.h"
#include "sample_comm.h"
#include <stdatomic.h>
#include "ot_proc.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define scene_thread_create(thread_info, thread_func, thread_arg) \
            if (thread_info.thread_flag == OT_FALSE) { \
                thread_info.thread_flag = OT_TRUE; \
                pthread_attr_init(&thread_info.thread_attr); \
                ot_s32 ret = pthread_create(&thread_info.thread, &thread_info.thread_attr, thread_func, (void*)thread_arg); \
                ot_scenecomm_check_return(ret, OT_SCENE_EINTER); \
            } \


#define scene_thread_destroy(thread_info) \
        if (thread_info.thread_flag == OT_TRUE) { \
            thread_info.thread_flag = OT_FALSE; \
            int* thread_result; \
            pthread_join(thread_info.thread, (void**)&thread_result); \
            pthread_attr_destroy(&thread_info.thread_attr); \
        } \

#define scene_set_thread_info(pipe_id, thread_type, thread_tag) \
    snprintf_truncated_s(g_scene_info_thr[pipe_id][thread_type].thread_name, SCENE_THREAD_MAX_NAME, "%d_%s", pipe_id, thread_tag); \
    prctl(PR_SET_NAME, (unsigned long)(uintptr_t)g_scene_info_thr[pipe_id][thread_type].thread_name, 0, 0, 0); \
    g_scene_info_thr[pipe_id][thread_type].thread_id = syscall(SYS_gettid); \
    g_scene_info_thr[pipe_id][thread_type].enable = OT_TRUE;

#define SCENE_PROC_DIR_NAME "sample_scene"
#define SCENE_PROC_ENTRY_NAME "debug_info"
#define SCENE_THREAD_MAX_NAME 50 

typedef struct {
    ot_vi_pipe_id pipe_id;
    ot_u32 chn_id;    
} scene_pipe_chn_id;

typedef struct {
    ot_bool is_init;
    ot_proc_entry_attr proc_attr;
} scene_proc;

typedef enum {
    SCENE_PROC_THREAD_TYPE_NORMAL = 0,
    SCENE_PROC_THREAD_TYPE_DEFLICKER,
    SCENE_PROC_THREAD_TYPE_CLUT,
    SCENE_PROC_THREAD_TYPE_DEHAZE,
    SCENE_PROC_THREAD_TYPE_MCE,
    SCENE_PROC_THREAD_TYPE_ABNR,
    SCENE_PROC_THREAD_TYPE_FPPIPE,
    SCENE_PROC_THREAD_TYPE_VENC,
    SCENE_PROC_THREAD_TYPE_SAVE,
    SCENE_PROC_THREAD_TYPE_FRAME_VP2VO,
    SCENE_PROC_THREAD_TYPE_FRAME_IQT0,
    SCENE_PROC_THREAD_TYPE_FRAME_IQT1,
    SCENE_PROC_THREAD_TYPE_FRAME_IQT2,
    SCENE_PROC_THREAD_TYPE_FRAME_IQT3,
    SCENE_PROC_THREAD_TYPE_BUTT,
} scene_proc_thread_type;

typedef struct {
    ot_bool enable;
    pid_t thread_id;
    char thread_name[SCENE_THREAD_MAX_NAME];
    ot_u64 proc_time;
    ot_u64 period_time;
} scene_proc_thread_info;

typedef struct {
    ot_u32 pipe_num;
    ot_vi_pipe_handle pipe_hdl[OT_SCENE_PIPE_MAX_NUM];
    ot_eis_handle vproc_pipe_hdl[OT_SCENE_PIPE_MAX_NUM];
    ot_eis_handle vproc_grp_hdl[OT_SCENE_PIPE_MAX_NUM];
    ot_eis_handle vo_inport_hdl[OT_SCENE_PIPE_MAX_NUM];
} scene_handle;

static scene_handle g_scene_handle = {0};

static scene_pipe_chn_id g_frame_pool_id[OT_SCENE_PIPE_MAX_NUM][OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {0};
static scene_pipe_chn_id g_iqt_id[OT_SCENE_PIPE_MAX_NUM][OT_EIS_VPROC_GRP_CHN_MAX_NUM];

static scene_proc g_scene_proc = {0};
static scene_proc_thread_info g_scene_info_thr[OT_SCENE_PIPE_MAX_NUM][SCENE_PROC_THREAD_TYPE_BUTT] = {0};

/* notes scene thread state */
static scene_thread_state g_scene_thread_state = {0};

/* notes scene media param */
static scene_mediapipe_attr g_scene_media_attr = {0};

static ot_scene_param g_sceneparam = {0};

static scene_frame_pool g_vproc_pipe_frame_pool[OT_SCENE_PIPE_MAX_NUM] = {0};
static scene_frame_pool g_vproc_chn_frame_pool[OT_SCENE_PIPE_MAX_NUM][OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {0};

static FILE *g_save_fp[OT_SCENE_PIPE_MAX_NUM] = {0};


static ot_s32 scene_stop_thread_media(ot_void)
{
    for (ot_s32 i = 0; i < g_scene_handle.pipe_num; i++) {
        ot_vi_pipe_handle pipe_hdl = g_scene_handle.pipe_hdl[i]; 
        ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
        ot_vi_pipe_id master_pipe_id = 0;
        ot_s32 pipe_num = 0;
        ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
        if (ret != OT_SUCCESS) {
            continue;
        }
        master_pipe_id = pipe_id[0];
        scene_thread_destroy(g_scene_thread_state.thread_vp2vo[master_pipe_id]);        
        scene_thread_destroy(g_scene_thread_state.thread_venc[master_pipe_id]);
        scene_thread_destroy(g_scene_thread_state.thread_save[master_pipe_id]);
        scene_thread_destroy(g_scene_thread_state.thread_frame_pool_pipe[master_pipe_id]);        
        for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {     
            scene_thread_destroy(g_scene_thread_state.thread_frame_pool_chn[master_pipe_id][j]);               
        }   
            
    }
    return OT_SUCCESS;
}

static ot_s32 scene_stop_thread_scene_auto(ot_void)
{
    for (ot_s32 i = 0; i < g_scene_handle.pipe_num; i++) {
        ot_vi_pipe_handle pipe_hdl = g_scene_handle.pipe_hdl[i]; 
        ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
        ot_vi_pipe_id master_pipe_id = 0;
        ot_s32 pipe_num = 0;
        ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
        if (ret != OT_SUCCESS) {
            continue;
        }
        master_pipe_id = pipe_id[0];
        scene_thread_destroy(g_scene_thread_state.thread_normal[master_pipe_id]);
        scene_thread_destroy(g_scene_thread_state.thread_clut[master_pipe_id]);               
        scene_thread_destroy(g_scene_thread_state.thread_dehaze[master_pipe_id]);
        scene_thread_destroy(g_scene_thread_state.thread_mce[master_pipe_id]);
        scene_thread_destroy(g_scene_thread_state.thread_abnr[master_pipe_id]);
        scene_thread_destroy(g_scene_thread_state.thread_deflicker[master_pipe_id]);              
        for (ot_s32 j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
            scene_thread_destroy(g_scene_thread_state.thread_iqt[OT_EIS_VPROC_GRP_CHN_MAX_NUM * master_pipe_id + j]);
        }
    }
    
    return OT_SUCCESS;
}

static ot_s32 scene_stop_auto_thread(ot_void)
{
    ot_s32 ret = scene_stop_thread_scene_auto();
    ot_scenecomm_check_return(ret, OT_FAILURE);

    ret = scene_stop_thread_media();
    ot_scenecomm_check_return(ret, OT_FAILURE);

    return OT_SUCCESS;
}

__inline static ot_u64 get_sys_time_by_msec(ot_void)
{
    struct timeval time;
    gettimeofday(&time, NULL);

    return (ot_u64)((time.tv_sec * 1000LLU) + time.tv_usec / 1000);
}

static scene_mediapipe_attr_param* scene_get_mediapipe_attr_param(ot_vi_pipe_handle pipe_hdl)
{
    ot_s32 index = 0;
    ot_s32 ret = ot_scene_get_param_index(pipe_hdl, &index);
    ot_scenecomm_check_return(ret, OT_NULL);

    return &g_scene_media_attr.mediapipe_param.mediapipe_attr[index];
}

ot_void *scene_frame_pool_pipe_auto_thread(ot_void *arg)
{        
    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);
    
    ot_vi_pipe_handle pipe_hdl = {0};
    ot_s32 ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);
    
    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    ot_eis_handle vproc_pipe_hdl = g_scene_handle.vproc_pipe_hdl[pipe_id]; 
    ret = sample_scene_fp_init(&g_vproc_pipe_frame_pool[pipe_id], SCENE_FRAME_POOL_TYPE_VPROC_PIPE_DELAY);
    ot_scenecomm_check_return(ret, OT_NULL);
    
    sleep(1);
    ot_u64 time[4] = {0};
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_FPPIPE, "FPPipe");

    while (g_scene_thread_state.thread_frame_pool_pipe[pipe_id].thread_flag) {
        time[0] = get_sys_time_by_msec();
        ret = sample_scene_fp_set_frame(&g_vproc_pipe_frame_pool[pipe_id], vproc_pipe_hdl);
        time[1] = get_sys_time_by_msec();
        
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FPPIPE].proc_time = time[1] - time[0];
        usleep(attr_param->thr_sl_param.thr_usleep_fp_pipe);
        if (ret != OT_SUCCESS) {            
            continue;
        }
        time[2] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FPPIPE].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();        
    }
    
    sample_scene_fp_deinit(&g_vproc_pipe_frame_pool[pipe_id], vproc_pipe_hdl);
    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FPPIPE].enable = OT_FALSE;
    return OT_NULL;
}

ot_void *scene_frame_pool_chn_auto_thread(ot_void *arg)
{
    if (arg == OT_NULL) {
        scene_loge("the arg of scene_frame_pool_chn_auto_thread is null\n");
        return OT_NULL;
    }

    prctl(PR_SET_NAME, (unsigned long)(uintptr_t)"FPChn", 0, 0, 0);

    ot_s32 ret = OT_FALSE;
    scene_pipe_chn_id id = *((scene_pipe_chn_id*)arg);
    
    ot_scenecomm_check_return(id.pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);
    ot_scenecomm_check_return(id.chn_id >= OT_EIS_VPROC_GRP_CHN_MAX_NUM, OT_NULL);

    ot_vi_pipe_handle pipe_hdl = {0};
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(id.pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    scene_frame_pool *pool = &g_vproc_chn_frame_pool[id.pipe_id][id.chn_id];
    ret = sample_scene_fp_init(pool, SCENE_FRAME_POOL_TYPE_VPROC_CHN);
    ot_scenecomm_check_return(ret, OT_NULL);

    while (g_scene_thread_state.thread_frame_pool_chn[id.pipe_id][id.chn_id].thread_flag) {
        ret = sample_scene_fp_set_frame(pool, g_scene_media_attr.vproc_attr[id.pipe_id].chn_hdl[id.chn_id]);
        usleep(attr_param->thr_sl_param.thr_usleep_fp_chn);
        if (ret != OT_SUCCESS) {            
            continue;
        }
    }

    sample_scene_fp_deinit(pool, g_scene_media_attr.vproc_attr[id.pipe_id].chn_hdl[id.chn_id]);

    return OT_NULL;
}

ot_void *scene_normal_auto_thread(ot_void *arg)
{
    ot_s32 ret = OT_FALSE;
    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);

    ot_vi_pipe_handle pipe_hdl = {0};
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    ret = ot_scene_dynamic_normal_init(pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);
        
    sleep(1);
    ot_u64 time[4] = {0};
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_NORMAL, "Normal");
    while (g_scene_thread_state.thread_normal[pipe_id].thread_flag) {
        time[0] = get_sys_time_by_msec();        
        ret = ot_scene_dynamic_normal_set_param(pipe_hdl, g_scene_handle.vproc_grp_hdl[pipe_id]);
        time[1] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_NORMAL].proc_time = time[1] - time[0];
        usleep(attr_param->thr_sl_param.thr_usleep_normal);
        if (ret != OT_SUCCESS) {            
            continue;
        } 
        time[2] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_NORMAL].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();
    }

    ot_scene_dynamic_normal_deinit(pipe_hdl);
    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_NORMAL].enable = OT_FALSE;
    return OT_NULL;
}

void *scene_clut_auto_thread(ot_void *arg)
{
    ot_s32 ret = OT_FALSE;
    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);

	ot_vi_pipe_handle pipe_hdl = {0};
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    ret = ot_scene_dynamic_clut_init(pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    ot_u64 time[4] = {0};
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_CLUT, "CLUT");  
    while (g_scene_thread_state.thread_clut[pipe_id].thread_flag) {
        time[0] = get_sys_time_by_msec();
        ret = ot_scene_dynamic_clut_set_param(pipe_hdl);
        time[1] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_CLUT].proc_time = time[1] - time[0];
        usleep(attr_param->thr_sl_param.thr_usleep_clut);
        if (ret != OT_SUCCESS) {
            continue;
        } 
        
        time[2] = get_sys_time_by_msec();
		g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_CLUT].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();
    }

    ot_scene_dynamic_clut_deinit(pipe_hdl);
    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_CLUT].enable = OT_FALSE;
    return OT_NULL;
}

void *scene_venc_auto_thread(ot_void *arg)
{            
    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);
        
    ot_vi_pipe_handle pipe_hdl = {0};
    ot_s32 ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    ot_u32 chn_id = attr_param->mediapipe_param.venc_chn_id;
    ot_scenecomm_expr_true_return(chn_id >= OT_EIS_VPROC_GRP_CHN_MAX_NUM, OT_NULL);

    ot_eis_venc_image_frame venc_frame = { 0 };    
    ot_u64 last_pts = 0;
    
    ot_eis_handle venc_hdl = g_scene_media_attr.venc_attr[pipe_id].venc_hdl;
    ot_s32 milli_sec = 100;
    scene_frame_pool *pool = &g_vproc_chn_frame_pool[pipe_id][chn_id];
    sleep(1);
    ot_u64 time[4] = {0};
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_VENC, "Venc");
    while (g_scene_thread_state.thread_venc[pipe_id].thread_flag) {        
        ot_eis_img_frame* frame = sample_scene_fp_get_frame(pool, last_pts, milli_sec);
        if (frame == OT_NULL) {
            continue;
        }
        last_pts = frame->pts;
        venc_frame.frame = frame;
        time[0] = get_sys_time_by_msec();  
        ret = ot_eis_venc_send_frame(venc_hdl, &venc_frame, milli_sec);
        time[1] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_VENC].proc_time = time[1] - time[0];
        sample_scene_fp_free(pool);
        if (ret != OT_SUCCESS) {                        
            continue;
        }
        time[2] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_VENC].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();        
    }

    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_VENC].enable = OT_FALSE;
    return OT_NULL;
}

void *scene_save_auto_thread(ot_void *arg)
{
    ot_s32 ret = OT_FALSE;    
    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;    
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);

    sample_venc_attr *venc_attr = &g_scene_media_attr.venc_attr[pipe_id];
    ot_eis_handle venc_hdl = venc_attr->venc_hdl;
        
    ot_vi_pipe_handle pipe_hdl = {0};
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    ret = sample_scene_media_set_filename(venc_attr, g_save_fp, pipe_id);
    ot_scenecomm_check_return(ret, OT_NULL);

    ot_u32 max_file_size = attr_param->mediapipe_param.venc_max_file_size_m * 1024 * 1024;
    ot_u64 time[4] = {0};
    ot_u32 jpeg_acquire_cnt = 0;
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_SAVE, "Save");
    while (g_scene_thread_state.thread_save[pipe_id].thread_flag) {
        time[0] = get_sys_time_by_msec();
        ret = sample_scene_media_save_stream(venc_attr, g_save_fp, pipe_id, &jpeg_acquire_cnt, max_file_size);
        time[1] = get_sys_time_by_msec();        
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_SAVE].proc_time = time[1] - time[0];
        if (ret == 1) {
            sample_print("sample_scene_media_save_stream chn[%d] JPEG done!\n", (ot_s32)((intptr_t)venc_hdl));
            break;
        }
        time[2] = get_sys_time_by_msec();  
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_SAVE].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();        
    }

    if (g_save_fp[pipe_id]) {
        fclose(g_save_fp[pipe_id]);
    }

    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_SAVE].enable = OT_FALSE;
    return OT_NULL;
}


void *scene_deflicker_auto_thread(ot_void *arg)
{
    ot_u64 last_pts = 0;
    ot_u32 last_idx = 0;
    
    ot_eis_img_frame* frame = OT_NULL;
    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;    
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);

    sleep(2);

    ot_vi_pipe_handle pipe_hdl = {0};
    ot_s32 milli_sec = 100;
    ot_s32 ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);        

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_return(attr_param == OT_NULL, OT_NULL); 
    
    ret = ot_scene_dynamic_deflicker_init(pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    sample_scene_fp_set_debug(&g_vproc_pipe_frame_pool[pipe_id], attr_param->debug_param.debug_deflicker_enable);
    
    ot_u64 time[4] = {0};
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_DEFLICKER, "Deflicker");  
    while (g_scene_thread_state.thread_deflicker[pipe_id].thread_flag) {        
        frame = sample_scene_fp_get_frame(&g_vproc_pipe_frame_pool[pipe_id], last_pts, milli_sec);
        if (frame == OT_NULL) {
            continue;
        }
        
        if (attr_param->debug_param.debug_deflicker_enable && 
            (last_idx != 0 && last_idx != frame->index - 2)) {
            printf("deflicker frame loss. last:%d cur:%d\n", last_idx, frame->index);
        }

        last_pts = frame->pts;
        last_idx = frame->index;
        time[0] = get_sys_time_by_msec();
        ret = ot_scene_dynamic_deflicker_set_param(pipe_hdl, frame);
        time[1] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_DEFLICKER].proc_time = time[1] - time[0];        
        sample_scene_fp_free(&g_vproc_pipe_frame_pool[pipe_id]);

        if (ret != OT_SUCCESS) {
            continue;
        }        
        
        time[2] = get_sys_time_by_msec();   
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_DEFLICKER].period_time = time[2] - time[3]; 
        time[3] = get_sys_time_by_msec();                        
    }

    ot_scene_dynamic_deflicker_deinit(pipe_hdl);
    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_DEFLICKER].enable = OT_FALSE;
    return OT_NULL;
}

ot_void *scene_dehaze_auto_thread(ot_void *arg)
{
    ot_s32 ret = OT_FALSE;

    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);
    
    ot_vi_pipe_handle pipe_hdl = {0};
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    ret = ot_scene_dynamic_dehaze_init(pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    ot_u64 time[4] = {0};

    sleep(1);
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_DEHAZE, "Dehaze");
    while (g_scene_thread_state.thread_dehaze[pipe_id].thread_flag) {
        time[0] = get_sys_time_by_msec();
        ret = ot_scene_dynamic_dehaze_set_param(pipe_hdl);
        time[1] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_DEHAZE].proc_time = time[1] - time[0];
        usleep(attr_param->thr_sl_param.thr_usleep_dehaze); 
        if (ret != OT_SUCCESS) {
            continue;
        }        
        time[2] = get_sys_time_by_msec();   
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_DEHAZE].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();     
    }

    ot_scene_dynamic_dehaze_deinit(pipe_hdl);
    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_DEHAZE].enable = OT_FALSE;
    return OT_NULL;
}

ot_void *scene_mce_auto_thread(ot_void *arg)
{
    ot_s32 ret = OT_FALSE;
    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);

    ot_vi_pipe_handle pipe_hdl = {0};
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    ot_u8 mce_chn_id = attr_param->mce_param.mce_chn_id;
    ot_scenecomm_expr_true_return(mce_chn_id >= OT_EIS_VPROC_GRP_CHN_MAX_NUM, OT_NULL);

    ret = ot_scene_dynamic_mce_init(pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);
    
    ot_u64 last_pts = 0;
    ot_s32 milli_sec = 100;
    sleep(1);
    ot_u64 time[4] = {0};
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_MCE, "MCE"); 
    while (g_scene_thread_state.thread_mce[pipe_id].thread_flag) {   
        ot_eis_img_frame* frame = sample_scene_fp_get_frame(&g_vproc_chn_frame_pool[pipe_id][mce_chn_id], last_pts, milli_sec);
        if (frame == OT_NULL) {
            continue;
        }
        
        last_pts = frame->pts;
        time[0] = get_sys_time_by_msec();
        ret = ot_scene_dynamic_mce_set_param(pipe_hdl, frame);
        time[1] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_MCE].proc_time = time[1] - time[0];
        sample_scene_fp_free(&g_vproc_chn_frame_pool[pipe_id][mce_chn_id]);
        if (ret != OT_SUCCESS) {
            continue;
        }
        
        time[2] = get_sys_time_by_msec();  
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_MCE].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();        
    }

    ot_scene_dynamic_mce_deinit(pipe_hdl);
    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_MCE].enable = OT_FALSE;
    return OT_NULL;
}

ot_void *scene_abnr_auto_thread(ot_void *arg)
{    
    ot_s32 ret = OT_FALSE;

    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);
    
    ot_vi_pipe_handle pipe_hdl = {0};
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    ret = ot_scene_dynamic_abnr_init(pipe_hdl, attr_param->mediapipe_param.vi_sns_type);
    ot_scenecomm_check_return(ret, OT_NULL);

    ot_u64 time[4] = {0};
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_ABNR, "ABNR");   
    while (g_scene_thread_state.thread_abnr[pipe_id].thread_flag) {    
        time[0] = get_sys_time_by_msec();    
        ret = ot_scene_dynamic_abnr_set_param(pipe_hdl);
        time[1] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_ABNR].proc_time = time[1] - time[0];
        usleep(attr_param->thr_sl_param.thr_usleep_abnr); 
        if (ret != OT_SUCCESS) {
            continue;
        }
                
        time[2] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_ABNR].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();	
    }

    ot_scene_dynamic_abnr_deinit(pipe_hdl);
    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_ABNR].enable = OT_FALSE;
    return OT_NULL;
}

ot_void *scene_vp2vo_auto_thread(ot_void *arg)
{
    ot_s32 ret = OT_FALSE;
    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);

    ot_vi_pipe_handle pipe_hdl = {0};
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);    

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);

    ot_s32 chn_id = attr_param->vp2vo_param.vp2vo_chn_id[0];
    ot_scenecomm_expr_true_return(chn_id >= OT_EIS_VPROC_GRP_CHN_MAX_NUM, OT_NULL);

    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_FRAME_VP2VO, "VP2VO");   
    
    ot_u64 last_pts = 0;
    ot_s32 milli_sec = 100;
    sleep(1);
    ot_u64 time[4] = {0};
    while (g_scene_thread_state.thread_vp2vo[pipe_id].thread_flag) {
        ot_eis_img_frame* frame = sample_scene_fp_get_frame(&g_vproc_chn_frame_pool[pipe_id][chn_id], last_pts, milli_sec);
        if (frame == OT_NULL) {
            continue;
        }
        
        last_pts = frame->pts;
        time[0] = get_sys_time_by_msec();
        ret = ot_eis_vo_in_port_send_frame(g_scene_handle.vo_inport_hdl[pipe_id], frame);
        time[1] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FRAME_VP2VO].proc_time = time[1] - time[0];
        sample_scene_fp_free(&g_vproc_chn_frame_pool[pipe_id][chn_id]);
        if (ret != OT_SUCCESS) {
            continue;
        }
        
        time[2] = get_sys_time_by_msec();  
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FRAME_VP2VO].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();  
    }

    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FRAME_VP2VO].enable = OT_FALSE;
    return OT_NULL;
}

ot_void *scene_iqt_auto_on_one_thread(ot_void *arg)
{
    ot_s32 ret = OT_FALSE;
    ot_vi_pipe_id pipe_id = (ot_u32)(long)arg;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);
        
    ot_vi_pipe_handle pipe_hdl = {0};    
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);    

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);
    
    ot_s32 milli_sec = 100;
    ot_eis_img_attr frame_attr = {0};
    ot_scene_iqt_attr iqt_attr = {0};
    ot_u64 last_pts[OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {0};
    ot_u64 time[OT_EIS_VPROC_GRP_CHN_MAX_NUM][4] = {0};
    ot_eis_handle task_hdl[OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {0};

    int i = 0;
    for (i = 0; i < OT_EIS_VPROC_GRP_CHN_MAX_NUM; i++) {
        if (attr_param->vp2vo_param.vp2vo_chn_id[i] < 0) {
            continue;
        }

        char thread_tag[SCENE_THREAD_MAX_NAME];
        snprintf_truncated_s(thread_tag, SCENE_THREAD_MAX_NAME, "%d_%s", i, "IQT");
        scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_FRAME_IQT0+i, thread_tag);

        ret = sample_scene_get_vproc_frame_attr(g_scene_media_attr.vproc_attr[pipe_id].chn_hdl[i], &frame_attr);
        if (ret != OT_SUCCESS) {
            goto err_exit;
        }

        iqt_attr.width = frame_attr.width;
        iqt_attr.height = frame_attr.height;
        iqt_attr.pixel_format = frame_attr.pixel_fmt;
        ret = ot_scene_dynamic_iqt_init(pipe_hdl, g_scene_media_attr.vproc_attr[pipe_id].chn_hdl[i], &iqt_attr, &task_hdl[i]);
        if (ret != OT_SUCCESS) {
            goto err_exit;
        }
    }

    sleep(1);
        
    while (g_scene_thread_state.thread_iqt[pipe_id * OT_EIS_VPROC_GRP_CHN_MAX_NUM].thread_flag) {
        for (int vp_chn_id = 0; vp_chn_id < OT_EIS_VPROC_GRP_CHN_MAX_NUM; vp_chn_id++) {
            if (attr_param->vp2vo_param.vp2vo_chn_id[vp_chn_id] < 0) {
                continue;
            }

            ot_eis_img_frame* frame = sample_scene_fp_get_frame(&g_vproc_chn_frame_pool[pipe_id][vp_chn_id], last_pts[vp_chn_id], milli_sec);
            if (frame == OT_NULL) {
                continue;
            }
            
            last_pts[vp_chn_id] = frame->pts;
            time[vp_chn_id][0] = get_sys_time_by_msec();
            ret = ot_scene_dynamic_iqt_set_param(pipe_hdl, task_hdl[vp_chn_id], frame);
            if (ret != OT_SUCCESS) {
                printf("ot_scene_dynamic_iqt_set_param failed with %#x\n", ret);
                sample_scene_fp_free(&g_vproc_chn_frame_pool[pipe_id][vp_chn_id]);
                continue;
            }        
            time[vp_chn_id][1] = get_sys_time_by_msec();
            g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FRAME_IQT0+vp_chn_id].proc_time = time[vp_chn_id][1] - time[vp_chn_id][0];

            if (vp_chn_id == 0) {
                ret = ot_eis_vo_in_port_send_frame(g_scene_handle.vo_inport_hdl[pipe_id], frame);  
            }

            sample_scene_fp_free(&g_vproc_chn_frame_pool[pipe_id][vp_chn_id]);
            if (ret != OT_SUCCESS) {
                continue;
            }
            
            time[vp_chn_id][2] = get_sys_time_by_msec();  
            g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FRAME_IQT0+vp_chn_id].period_time = time[vp_chn_id][2] - time[vp_chn_id][3];
            time[vp_chn_id][3] = get_sys_time_by_msec();  
        }        
    }

err_exit:
    for (int j = i - 1; j >= 0; --j) {
        if (attr_param->vp2vo_param.vp2vo_chn_id[j] < 0) {
            continue;
        }

        ot_scene_dynamic_iqt_deinit(task_hdl[j]);
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FRAME_IQT0+j].enable = OT_FALSE;
    }
    return OT_NULL;
}

ot_void *scene_iqt_auto_thread(ot_void *arg)
{
    ot_s32 ret = OT_FALSE;
    scene_pipe_chn_id *pipe_chn_id = (scene_pipe_chn_id*)arg;
    ot_scenecomm_expr_true_return(pipe_chn_id == OT_NULL, OT_NULL);

    ot_vi_pipe_id pipe_id = pipe_chn_id->pipe_id;
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_NULL);
    
    ot_u32 vp_chn_id = pipe_chn_id->chn_id;
    ot_scenecomm_expr_true_return(vp_chn_id >= OT_EIS_VPROC_GRP_CHN_MAX_NUM, OT_NULL);

    ot_vi_pipe_handle pipe_hdl = {0};    
    ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(pipe_id, &pipe_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);    

    scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
    ot_scenecomm_check_pointer_return(attr_param, OT_NULL);
    
    ot_s32 milli_sec = 100;
    ot_eis_img_attr frame_attr = {0};
    ot_scene_iqt_attr iqt_attr = {0};
    ot_u64 last_pts = {0};
    ot_u64 time[4] = {0};
    ot_eis_handle task_hdl = {0};

    ret = sample_scene_get_vproc_frame_attr(g_scene_media_attr.vproc_attr[pipe_id].chn_hdl[vp_chn_id], &frame_attr);
    ot_scenecomm_check_return(ret, OT_NULL);    

    iqt_attr.width = frame_attr.width;
    iqt_attr.height = frame_attr.height;
    iqt_attr.pixel_format = frame_attr.pixel_fmt;
    ret = ot_scene_dynamic_iqt_init(pipe_hdl, g_scene_media_attr.vproc_attr[pipe_id].chn_hdl[vp_chn_id], &iqt_attr, &task_hdl);
    ot_scenecomm_check_return(ret, OT_NULL);    
    
    char thread_tag[SCENE_THREAD_MAX_NAME];
    snprintf_truncated_s(thread_tag, SCENE_THREAD_MAX_NAME, "%d_%s", vp_chn_id, "IQT");
    scene_set_thread_info(pipe_id, SCENE_PROC_THREAD_TYPE_FRAME_IQT0+vp_chn_id, thread_tag);

    sleep(1);
        
    while (g_scene_thread_state.thread_iqt[pipe_id * OT_EIS_VPROC_GRP_CHN_MAX_NUM + vp_chn_id].thread_flag) {        
        ot_eis_img_frame* frame = sample_scene_fp_get_frame(&g_vproc_chn_frame_pool[pipe_id][vp_chn_id], last_pts, milli_sec);
        if (frame == OT_NULL) {
            continue;
        }
        
        last_pts = frame->pts;
        time[0] = get_sys_time_by_msec();
        ret = ot_scene_dynamic_iqt_set_param(pipe_hdl, task_hdl, frame);
        if (ret != OT_SUCCESS) {
            printf("ot_scene_dynamic_iqt_set_param failed with %#x\n", ret);
            sample_scene_fp_free(&g_vproc_chn_frame_pool[pipe_id][vp_chn_id]);
            continue;
        }        
        time[1] = get_sys_time_by_msec();
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FRAME_IQT0+vp_chn_id].proc_time = time[1] - time[0];

        if (vp_chn_id == 0) {
            ret = ot_eis_vo_in_port_send_frame(g_scene_handle.vo_inport_hdl[pipe_id], frame);  
        }
            
        sample_scene_fp_free(&g_vproc_chn_frame_pool[pipe_id][vp_chn_id]);
        if (ret != OT_SUCCESS) {
            continue;
        }
        
        time[2] = get_sys_time_by_msec();  
        g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FRAME_IQT0+vp_chn_id].period_time = time[2] - time[3];
        time[3] = get_sys_time_by_msec();                
    }

    ot_scene_dynamic_iqt_deinit(task_hdl);
    g_scene_info_thr[pipe_id][SCENE_PROC_THREAD_TYPE_FRAME_IQT0+vp_chn_id].enable = OT_FALSE;
    return OT_NULL;
}

static ot_s32 scene_set_mediapipe(ot_scene_mediapipe *scene_mediapipe)
{
    ot_scenecomm_check_pointer_return(scene_mediapipe, OT_SCENE_ENONPTR);    
    
    (ot_void)memcpy_s(&g_scene_media_attr.mediapipe_param, sizeof(ot_scene_mediapipe), 
            scene_mediapipe, sizeof(ot_scene_mediapipe));
    return OT_SUCCESS;
}

static ot_s32 scene_set_sceneparam(const ot_scene_param *sceneparam)
{
    ot_scenecomm_check_pointer_return(sceneparam, OT_SCENE_ENONPTR);    
    
    (ot_void)memcpy_s(&g_sceneparam, sizeof(g_sceneparam), sceneparam, sizeof(ot_scene_param));
    return OT_SUCCESS;
}

static ot_s32 scene_proc_show(ot_proc_show_buffer_attr * buffer, ot_void *priv_data)
{
    ot_char* pad_string = "----------------------------------------------------------------------";
    ot_proc_entry_show(buffer, "%s sample_scene %s\n", pad_string, pad_string);
    ot_proc_entry_show(buffer,"%19s %19s %19s %19s\n", "thread_name", "thread_id", "proc_time(ms)", "period_time(ms)");

    for (int i = 0; i < OT_SCENE_PIPE_MAX_NUM; i++) {
        for (int j = 0; j < SCENE_PROC_THREAD_TYPE_BUTT; j++) {
            if (g_scene_info_thr[i][j].enable == OT_FALSE) {
                continue;
            }

            ot_proc_entry_show(buffer, "%19s %19lu %19lu %19lu\n", g_scene_info_thr[i][j].thread_name, g_scene_info_thr[i][j].thread_id, 
                            g_scene_info_thr[i][j].proc_time, g_scene_info_thr[i][j].period_time);
        }
    }

    ot_char* pad_string1 = "=====================================================================";
    ot_proc_entry_show(buffer, "%s frame info %s\n", pad_string1, pad_string1);
    ot_proc_entry_show(buffer,"%19s %19s %19s %19s %19s\n", "id", "src_fps", "dst_fps", "w", "h");

    char id_name[20] = {0};
    ot_s32 milli_sec = 10;
    for (ot_s32 i = 0; i < g_scene_handle.pipe_num; i++) {
        ot_vi_pipe_handle pipe_hdl = g_scene_handle.pipe_hdl[i]; 
        ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
        ot_vi_pipe_id master_pipe_id = 0;
        ot_s32 pipe_num = 0;
        ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
        if (ret != OT_SUCCESS) {
            continue;
        }
        master_pipe_id = pipe_id[0];

        scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);
        if (attr_param == OT_NULL) {
            continue;
        }

        ot_eis_vi_exp_info exp_info = {0};
        ret = ot_eis_vi_ae_query_info(pipe_hdl, &exp_info);
        ot_double fps = 0;
        if (ret == OT_SUCCESS) {
            fps = exp_info.fps / 100.0;
        }

        ot_eis_img_size img_size = {0};
        sample_comm_vi_get_size_by_sns_type(attr_param->mediapipe_param.vi_sns_type, &img_size);
        sprintf(id_name, "pipe_%d", master_pipe_id);
        ot_proc_entry_show(buffer, "%19s %19.2f %19.2f %19lu %19lu\n", id_name, fps, fps, img_size.width, img_size.height);
        for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
            if (attr_param->mediapipe_param.vproc_chn_enable[j] == OT_TRUE) {
                ot_eis_vproc_chn_attr chn_attr = {0};
                ot_eis_vproc_chn_get_attr(g_scene_media_attr.vproc_attr[master_pipe_id].chn_hdl[j], &chn_attr);
                ot_u32 wdith = chn_attr.image_attr.width;
                ot_u32 height = chn_attr.image_attr.height;
                if (attr_param->mediapipe_param.vproc_chn_frame_pool_enable[j] == OT_TRUE) {
                    ot_eis_img_frame* frame = sample_scene_fp_get_frame(&g_vproc_chn_frame_pool[master_pipe_id][j], 0, milli_sec);
                    if (frame != OT_NULL) {
                        wdith = frame->attr.width;
                        height = frame->attr.height;
                        sample_scene_fp_free(&g_vproc_chn_frame_pool[master_pipe_id][j]);
                    }
                }

                sprintf(id_name, "chn_%d", j);
                ot_proc_entry_show(buffer, "%19s %19d %19d %19lu %19lu\n", id_name, chn_attr.frc.src_frame_rate,
                        chn_attr.frc.dst_frame_rate, wdith, height);
            }
        }
    }

    return OT_SUCCESS;
}

static ot_s32 scene_proc_init()
{
    ot_s32 ret = OT_FAILURE;

    if (g_scene_proc.is_init) {
        return OT_SUCCESS;
    }

    ret = ot_proc_init();
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = ot_proc_dir_add(SCENE_PROC_DIR_NAME);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    g_scene_proc.proc_attr.dir_name = SCENE_PROC_DIR_NAME;
    g_scene_proc.proc_attr.name = SCENE_PROC_ENTRY_NAME;
    g_scene_proc.proc_attr.proc_show = scene_proc_show;
    g_scene_proc.proc_attr.proc_cmd = OT_NULL;
    g_scene_proc.proc_attr.priv_data = OT_NULL;
    ret = ot_proc_entry_create(&g_scene_proc.proc_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    g_scene_proc.is_init = OT_TRUE;

    return OT_SUCCESS;
}

static void scene_proc_deinit()
{
    if (g_scene_proc.is_init == OT_FALSE) {
        return ;
    }

    ot_proc_entry_destroy(&g_scene_proc.proc_attr);
    ot_proc_dir_remove(SCENE_PROC_DIR_NAME);
    ot_proc_deinit();

    g_scene_proc.is_init = OT_FALSE;
}

ot_s32 sample_scene_init(const ot_scene_param *scene_param, ot_scene_mediapipe *scene_mediapipe)
{
    ot_s32 ret = OT_FALSE;
    ot_scenecomm_check_pointer_return(scene_param, OT_SCENE_ENONPTR);
    ot_scenecomm_check_pointer_return(scene_mediapipe, OT_SCENE_ENONPTR);

    ret = ot_scene_init(scene_param);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    // mediapipe param
    ret = scene_set_mediapipe(scene_mediapipe);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = scene_set_sceneparam(scene_param);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    // proc
    scene_proc_init();

    return OT_SUCCESS;
}

ot_s32 sample_scene_start_mediapipe(ot_void)
{
    ot_s32 ret = OT_FALSE;

    ret = sample_scene_media_start(&g_scene_media_attr);
    if (ret != OT_SUCCESS) {
        scene_loge("start mediapipe failed\n");
        return OT_SCENE_EINTER;
    }

    g_scene_handle.pipe_num = g_scene_media_attr.mediapipe_param.mediapipe_comm.mediapipe_num;

    for (int i = 0; i < g_scene_handle.pipe_num; i++) {
        g_scene_handle.pipe_hdl[i] = g_scene_media_attr.vi_cfg[i].pipe_info[0].pipe_hdl;
        g_scene_handle.vproc_grp_hdl[i] = g_scene_media_attr.vproc_attr[i].grp_hdl;         
        g_scene_handle.vproc_pipe_hdl[i] = g_scene_media_attr.vproc_attr[i].pipe_hdl[0]; 
        g_scene_handle.vo_inport_hdl[i] = g_scene_media_attr.vo_attr->in_port_handle[i]; 
    }
    
    ret = ot_scene_set_state(g_scene_handle.pipe_hdl, g_scene_handle.pipe_num);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    // Set the compress mode of 3dnr
    for (int i = 0; i < g_scene_handle.pipe_num; i++) {
        ot_u8 comp_mode_3dnr = g_scene_media_attr.mediapipe_param.mediapipe_attr[i].mediapipe_param.comp_mode_3dnr;
        if (comp_mode_3dnr == 0) {
            continue;
        }

        ot_scene_ctrl_param_3dnr param_3dnr = {0};
        ret = ot_scene_ctrl_3dnr_get_param(g_scene_handle.pipe_hdl[i], g_scene_handle.vproc_grp_hdl[i], &param_3dnr);
        if (ret != OT_SUCCESS) {
            continue;
        }
        param_3dnr.enable = g_sceneparam.pipe_param[i].static_threednr.enable;
        param_3dnr.compress_mode = (ot_eis_img_compress_mode)comp_mode_3dnr;
        ot_scene_ctrl_3dnr_set_param(g_scene_handle.pipe_hdl[i], g_scene_handle.vproc_grp_hdl[i], &param_3dnr);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_start_thread_media()
{
    for (int i = 0; i < g_scene_handle.pipe_num; i++) {
        ot_vi_pipe_handle pipe_hdl = g_scene_handle.pipe_hdl[i]; 
        ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
        ot_vi_pipe_id master_pipe_id = 0;
        ot_s32 pipe_num = 0;
        ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
        if (ret != OT_SUCCESS) {
            continue;
        }
        master_pipe_id = pipe_id[0];

        scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);
        if (attr_param == OT_NULL) {
            continue;
        }

        // the frame pool of vproc pipe
        if (attr_param->mediapipe_param.vproc_pipe_frame_pool_enable == OT_TRUE) {
            scene_thread_create(g_scene_thread_state.thread_frame_pool_pipe[master_pipe_id], scene_frame_pool_pipe_auto_thread, (long)master_pipe_id);
        }

        // the frame pool of vproc chn
        for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
            if (attr_param->mediapipe_param.vproc_chn_enable[j] == OT_FALSE ||
                attr_param->mediapipe_param.vproc_chn_frame_pool_enable[j] == OT_FALSE
            ) {
                continue;
            }

            if (g_scene_thread_state.thread_frame_pool_chn[master_pipe_id][j].thread_flag == OT_FALSE) {
                g_frame_pool_id[master_pipe_id][j].pipe_id = master_pipe_id;
                g_frame_pool_id[master_pipe_id][j].chn_id = j;

                scene_thread_create(g_scene_thread_state.thread_frame_pool_chn[master_pipe_id][j],
                                scene_frame_pool_chn_auto_thread, &g_frame_pool_id[master_pipe_id][j]);
            }
        }

        if (attr_param->mediapipe_param.save_video_enable) {
            scene_thread_create(g_scene_thread_state.thread_venc[master_pipe_id], scene_venc_auto_thread, (long)master_pipe_id);
            scene_thread_create(g_scene_thread_state.thread_save[master_pipe_id], scene_save_auto_thread, (long)master_pipe_id);
        }
    }

    return OT_SUCCESS;
}


static ot_s32 scene_start_thread_scene_auto()
{
    for (ot_s32 i = 0; i < g_scene_handle.pipe_num; i++) {
        ot_vi_pipe_handle pipe_hdl = g_scene_handle.pipe_hdl[i]; 
        ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
        ot_vi_pipe_id master_pipe_id = 0;
        ot_s32 pipe_num = 0;
        ot_s32 ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
        if (ret != OT_SUCCESS) {
            continue;
        }
        master_pipe_id = pipe_id[0];

        ret = ot_scene_static_vi_set_param(pipe_hdl, g_scene_handle.vproc_grp_hdl[i]);
        if (ret != OT_SUCCESS) {
            continue;
        }
        
        scene_mediapipe_attr_param *attr_param = scene_get_mediapipe_attr_param(pipe_hdl);    
        if (attr_param == OT_NULL) {
            continue;
        }

        if (attr_param->thr_en_param.thr_enable_normal) {
            scene_thread_create(g_scene_thread_state.thread_normal[master_pipe_id], scene_normal_auto_thread, (long)master_pipe_id);
        }

        if (attr_param->thr_en_param.thr_enable_clut) {
            scene_thread_create(g_scene_thread_state.thread_clut[master_pipe_id], scene_clut_auto_thread, (long)master_pipe_id);
        }

        if (attr_param->thr_en_param.thr_enable_dehaze) {
            scene_thread_create(g_scene_thread_state.thread_dehaze[master_pipe_id], scene_dehaze_auto_thread, (long)master_pipe_id);
        }

        if (attr_param->thr_en_param.thr_enable_mce) {
            scene_thread_create(g_scene_thread_state.thread_mce[master_pipe_id], scene_mce_auto_thread, (long)master_pipe_id);
        }

        if (attr_param->thr_en_param.thr_enable_abnr == OT_TRUE) {        
            scene_thread_create(g_scene_thread_state.thread_abnr[master_pipe_id], scene_abnr_auto_thread, (long)master_pipe_id);  
        } 

        if (attr_param->thr_en_param.thr_enable_vp2vo == OT_TRUE) {        
            scene_thread_create(g_scene_thread_state.thread_vp2vo[master_pipe_id], scene_vp2vo_auto_thread, (long)master_pipe_id);  
        } 

        if (attr_param->thr_en_param.thr_enable_iqt == OT_TRUE) { 
            if (attr_param->vp2vo_param.vp2vo_on_one_thread) {                
                scene_thread_create(g_scene_thread_state.thread_iqt[master_pipe_id * OT_EIS_VPROC_GRP_CHN_MAX_NUM], scene_iqt_auto_on_one_thread, (long)master_pipe_id);  
            } else {
                for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
                    if (attr_param->vp2vo_param.vp2vo_chn_id[j] < 0) {
                        continue;
                    }
                    g_iqt_id[master_pipe_id][j].pipe_id = master_pipe_id;
                    g_iqt_id[master_pipe_id][j].chn_id = j;
                    scene_thread_create(g_scene_thread_state.thread_iqt[master_pipe_id * OT_EIS_VPROC_GRP_CHN_MAX_NUM + j], 
                        scene_iqt_auto_thread, (void*)&g_iqt_id[master_pipe_id][j]);  
                }
            }                       
        } 

        if (attr_param->thr_en_param.thr_enable_deflicker) {
            scene_thread_create(g_scene_thread_state.thread_deflicker[master_pipe_id], scene_deflicker_auto_thread, (long)master_pipe_id);
        }

        
    }    
    
    return OT_SUCCESS;
}

ot_s32 sample_scene_start_thread(ot_void)
{
    ot_s32 ret = OT_FALSE;

    ret = scene_start_thread_media();
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    ret = scene_start_thread_scene_auto();
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 sample_scene_refresh_param(const ot_scene_param *scene_param)
{
    ot_s32 ret = OT_FALSE;
    ot_scenecomm_check_pointer_return(scene_param, OT_SCENE_ENONPTR);

    ret = scene_stop_thread_scene_auto();
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    scene_logi("sceneauto threads exit.\n");

    scene_proc_deinit();
    scene_proc_init();

    ret = ot_scene_set_param(scene_param);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    for (ot_s32 i = 0; i < g_scene_handle.pipe_num; i++) {
        ot_scene_static_vi_set_param(g_scene_handle.pipe_hdl[i], g_scene_handle.vproc_grp_hdl[i]);
    }
    
    ret = scene_start_thread_scene_auto();
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 sample_scene_pause(ot_bool pause)
{
    ot_s32 ret = OT_FAILURE;
    ot_vi_pipe_handle pipe_hdl = g_scene_handle.pipe_hdl[0]; 
    if (g_scene_handle.pipe_num > 1) {
        printf("Please input the pipe id: ");

        for (int i = 0; i < g_scene_handle.pipe_num; i++) {
        	ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
        	ot_vi_pipe_id master_pipe_id = 0;
        	ot_s32 pipe_num = 0;
        	ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
        	if (ret != OT_SUCCESS) {
        	    continue;
        	}
        	master_pipe_id = pipe_id[0];
        
            printf("%d ", (int)master_pipe_id);
        }
        
        printf("\n");

        ot_char input[10];
        ot_s32 choice = -1;
        (ot_void)fgets(input, sizeof(input), stdin);
        ret = sscanf_s(input, "%d", &choice);
        if (ret != 1) {
            printf("pipe id error:%d\n", choice);
            return OT_FAILURE;
        }

        if (choice < 0 || choice >= OT_SCENE_PIPE_MAX_NUM) {
            printf("invalid pipe id:%d\n", choice);
            return OT_FAILURE;
        }

        ret = ot_eis_vi_get_pipe_hdl_by_pipe_id(choice, &pipe_hdl);
        if (ret != OT_SUCCESS) {
            printf("invalid pipe id:%d\n", choice);
            return OT_FAILURE;
        }
    }
    

    ret = ot_scene_pause(pipe_hdl, pause);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    return OT_SUCCESS;
}

ot_s32 sample_scene_deinit(ot_void)
{
    ot_s32 ret = OT_FALSE;
    scene_logi("sceneauto deinit.\n");
    ret = scene_stop_auto_thread();
    if (ret != OT_SUCCESS) {
        scene_loge("scene_stop_auto_thread fail!\n");
        return OT_SCENE_EINTER;
    }
    
    sleep(1); // wait for abnr
    scene_logi("sceneauto threads exit.\n");

    ret = sample_scene_media_release(&g_scene_media_attr);
    if (ret != OT_SUCCESS) {
        scene_loge("release mediapipe failed\n");
        return OT_SCENE_EINTER;
    }

    scene_logi("sceneauto mediapipe release.\n");

    ret = ot_scene_deinit();
    if (ret != OT_SUCCESS) {
        scene_loge("ot_scene_deinit failed\n");
        return OT_SCENE_EINTER;
    }

    scene_proc_deinit();

    scene_logi("sceneauto exit success.\n");

    return OT_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
