/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef OT_SCENE_LOADPARAM_H
#define OT_SCENE_LOADPARAM_H

#include "ot_eis_vproc_common.h"
#include "ot_scene_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SCENE_VRB_BUF_BLK_NUM 3

typedef struct {
    ot_s8* name;
    ot_u32 value;
} scene_enum_mapping;

typedef struct {      
    ot_u32 mediapipe_num;
    ot_u8 vi_vproc_mode_type;    
    ot_u8 vo_display_dev;
    ot_float vo_fps;
    ot_u32 vrb_buf_blks[SCENE_VRB_BUF_BLK_NUM];
} scene_mediapipe_comm_param;

typedef struct {
    ot_bool vi_with_sensor;
    ot_u8 vi_sns_type;
    ot_u8 vi_dev_id;
    ot_bool vproc_pipe_frame_pool_enable;
    ot_bool vproc_chn_frame_pool_enable[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_bool vproc_chn_enable[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u8 vproc_chn_frc_src[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u8 vproc_chn_frc_dst[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u32 vproc_chn_img_w[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u32 vproc_chn_img_h[OT_EIS_VPROC_GRP_CHN_MAX_NUM];    
    ot_bool vproc_chn_crop_enable[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u8 vproc_chn_crop_mode[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u16 vproc_chn_crop_x[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u16 vproc_chn_crop_y[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u32 vproc_chn_crop_w[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u32 vproc_chn_crop_h[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_bool vproc_chn_mirror_enable[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_bool vproc_chn_rotation_enable[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u8 vproc_chn_rotation[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    
    ot_bool save_video_enable;
    ot_u8 venc_chn_id;
    ot_u8 venc_codec_format;    
    ot_u32 venc_stream_buf_size;  
    ot_u8 venc_gop;               
    ot_u8 venc_rc_mode;          
    ot_u32 venc_statis_time;      
    ot_u32 venc_max_bit_rate;     
    ot_u32 venc_short_statis_time;
    ot_u32 venc_long_statis_time; 
    ot_u32 venc_long_max_bit_rate;
    ot_u32 venc_long_min_bit_rate;
    ot_u32 venc_gop_mode;         
    ot_u32 venc_qp_delta_ip;      
    ot_u32 venc_max_file_size_m;      

    ot_u8 comp_mode_vi_pipe;
    ot_u8 comp_mode_vi_chn;
    ot_u8 comp_mode_vproc_pipe[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u8 comp_mode_vproc_chn[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_u8 comp_mode_3dnr;
} scene_mediapipe_param;

typedef struct {
    ot_u8 mce_chn_id;
} scene_mce_param;

typedef struct {
    ot_u8 debug_deflicker_enable;
} scene_debug_param;

typedef struct {
    ot_s32 vp2vo_chn_id[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_bool vp2vo_on_one_thread;
} scene_vp2vo_param;

typedef struct {
    ot_bool thr_enable_normal;
    ot_bool thr_enable_mce;
    ot_bool thr_enable_deflicker;
    ot_bool thr_enable_clut;
    ot_bool thr_enable_dehaze;
    ot_bool thr_enable_abnr;
    ot_bool thr_enable_iqt;
    ot_bool thr_enable_vp2vo;
} scene_thr_enable_param;

typedef struct {
    ot_u32 thr_usleep_normal;
    ot_u32 thr_usleep_clut;
    ot_u32 thr_usleep_dehaze;
    ot_u32 thr_usleep_abnr;
    ot_u32 thr_usleep_fp_pipe;
    ot_u32 thr_usleep_fp_chn;
} scene_thr_usleep_param;

typedef struct {
    scene_thr_enable_param thr_en_param;
    scene_thr_usleep_param thr_sl_param;
    scene_mediapipe_param mediapipe_param;
    scene_mce_param mce_param;
    scene_debug_param debug_param;
    scene_vp2vo_param vp2vo_param;
} scene_mediapipe_attr_param;

typedef struct {
    scene_mediapipe_comm_param mediapipe_comm; 
    scene_mediapipe_attr_param mediapipe_attr[OT_SCENE_PIPE_MAX_NUM];
} ot_scene_mediapipe;


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef OT_SCENE_LOADPARAM_H */
