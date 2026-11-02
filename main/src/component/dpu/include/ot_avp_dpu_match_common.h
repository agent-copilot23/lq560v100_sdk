/******************************************************************************
 Copyright (C), 2024 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : the application interface struct and macro code etc.
Created       : 2024/09/23
Last Modified :
******************************************************************************/
#ifndef OT_AVP_DPU_MATCH_COMMON_H
#define OT_AVP_DPU_MATCH_COMMON_H

#include "ot_eis_type.h"
#include "ot_eis_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define OT_AVP_DPU_MATCH_MAX_PIPE_NUM 2
#define OT_AVP_DPU_MATCH_MAX_CHN_NUM 1

typedef struct {
    ot_u64 phys_addr;
    ot_u64 virt_addr;
    ot_u32 size;
} ot_avp_dpu_match_mem_info;

typedef struct {
    ot_eis_img_frame src_frame[OT_AVP_DPU_MATCH_MAX_PIPE_NUM]; /* src_frame[0]:left frame,src_frame[1]:right frame */
    ot_eis_img_frame rect_frame[OT_AVP_DPU_MATCH_MAX_PIPE_NUM]; /* rect_frame[0]:left frame,rect_frame[1]:right frame */
} ot_avp_dpu_match_src_frame_info;

typedef struct {
    ot_eis_img_frame disp_frame; /* disparity frame */
    ot_eis_img_frame conf_frame; /* confidence frame */
    ot_eis_img_frame depth_frame; /* depth frame */
} ot_avp_dpu_match_dst_frame_info;

typedef enum {
    OT_AVP_DPU_MATCH_MASK_DEFAULT_MODE = 0x0,
    OT_AVP_DPU_MATCH_MASK_1X1_MODE = 0x1,
    OT_AVP_DPU_MATCH_MASK_3X3_MODE = 0x2,
    OT_AVP_DPU_MATCH_MASK_5X5_MODE = 0x3,
    OT_AVP_DPU_MATCH_MASK_7X7_MODE = 0x4,
    OT_AVP_DPU_MATCH_MASK_9X9_MODE = 0x5,
    OT_AVP_DPU_MATCH_MASK_11X11_MODE = 0x6,
    OT_AVP_DPU_MATCH_MASK_13X13_MODE = 0x7,
    OT_AVP_DPU_MATCH_MASK_15X15_MODE = 0x8,
    OT_AVP_DPU_MATCH_MASK_17X17_MODE = 0x9,
    OT_AVP_DPU_MATCH_MASK_MODE_INVALID
} ot_avp_dpu_match_mask_mode;

typedef enum {
    OT_AVP_DPU_MATCH_SPEED_ACCURACY_MODE_SPEED = 0x0,
    OT_AVP_DPU_MATCH_SPEED_ACCURACY_MODE_ACCURACY = 0x1,
    OT_AVP_DPU_MATCH_SPEED_ACCURACY_MODE_INVALID
} ot_avp_dpu_match_speed_accuracy_mode;

typedef enum {
    OT_AVP_DPU_MATCH_DISP_FROM_MATCH = 0x0,
    OT_AVP_DPU_MATCH_DISP_FROM_USER = 0x1,  /* only disparity to depth */
    OT_AVP_DPU_MATCH_DISP_FROM_INVALID
} ot_avp_dpu_match_disp_from;

typedef enum {
    OT_AVP_DPU_MATCH_BG_MODE_NONE = 0x0,
    OT_AVP_DPU_MATCH_BG_MODE_LOW_LIGHT = 0x1,
    OT_AVP_DPU_MATCH_BG_MODE_NORMAL_LIGHT = 0x2,
    OT_AVP_DPU_MATCH_BG_MODE_HIGH_LIGHT = 0x3,
    OT_AVP_DPU_MATCH_BG_MODE_INVALID
} ot_avp_dpu_match_bg_mode;

typedef enum {
    OT_AVP_DPU_MATCH_OPTI_MODE_DEFAULT = 0x0,
    OT_AVP_DPU_MATCH_OPTI_MODE_ADAPTIVE = 0x1,
    OT_AVP_DPU_MATCH_OPTI_MODE_INVALID
} ot_avp_dpu_match_opti_mode;

typedef enum {
    OT_AVP_DPU_MATCH_DISP_VERSION_V1 = 0,
    OT_AVP_DPU_MATCH_DISP_VERSION_INVALID
} ot_avp_dpu_match_disp_version;

typedef enum {
    OT_AVP_DPU_MATCH_DEPTH_VERSION_V1 = 0,
    OT_AVP_DPU_MATCH_DEPTH_VERSION_INVALID
} ot_avp_dpu_match_depth_version;

typedef struct {
    ot_bool is_proc_disp;
    ot_bool is_output_disp;
    ot_bool is_output_conf;
    ot_bool is_post_proc;
    ot_bool is_filter_disp_by_conf; /* confidence filter disparity switch */
    ot_bool is_time_domain_filter;
    ot_bool is_enable_disp_subpixel; /* calculate subpixel disparity or not. */
    ot_avp_dpu_match_mask_mode mask_mode; /* aggregation mask mode. */
    ot_avp_dpu_match_speed_accuracy_mode speed_accuracy_mode; /* adjust speed-accuracy trade-off. */
    ot_avp_dpu_match_bg_mode  bg_mode; /* backgroud mode */
    ot_avp_dpu_match_opti_mode opti_mode; /* update the correlation value of first_penalty_coef and second_penalty_coef */
    ot_u16 disp_num; /* the number of disparity, it must be the multiple of 16. range:[16,224] */
    ot_s16 disp_start_pos; /* minimum disparity, it must be the multiple of 2. range:[-224, 224] */
    ot_u16 first_penalty_coef; /* First penalty coefficient. Range: [0, 127] */
    ot_u16 second_penalty_coef; /* Secode penalty coefficient. Range: [0, 127] */
    ot_u8 fg_init_cost_zero_thr;  /* the foreground overexposure area filter threshold,[1, 16] */
    ot_u8 local_feature; /* census cost rate, [0, 8] */
    ot_u8 conf_filter_disp_thr; /* confidece filter disparity threshold, [0,15] */
    ot_u8 aggregate_coef; /* aggregate coefficient.[0, 8] */
    ot_u8 unique_ratio; /* uniqueness ratio. [0, 31] */
    ot_u8 rsv[3];
} ot_avp_dpu_match_disp_param_v1;

typedef struct {
    ot_avp_dpu_match_disp_version version;
    union {
        ot_avp_dpu_match_disp_param_v1 v1; /* v1 param,AUTO:ot_avp_dpu_match_disp_version:OT_AVP_DPU_MATCH_DISP_VERSION_V1 */
    };
} ot_avp_dpu_match_disp_param;

typedef struct {
    ot_bool is_output_depth; /* output depth switch */
    ot_avp_dpu_match_disp_from disp_from; /* disparity source */
    ot_u32 fb; /* fixed-point fb，the decimal bits wide is fb_norm_bits，fixed-point:{24.8,22.10,20.12} */
    ot_u16 z0; /* z0 */
    ot_u16 min_depth_thr; /* min valid depth value */
    ot_u16 max_depth_thr; /* max valid depth value */
    ot_u16 invalid_depth; /* invalid depth  */
    ot_s8 disp_coef; /* the coefficient of disparity to depth, {-1, 1} */
    ot_u8 fb_norm_bits; /* the decimal bits wide of fb, {8,10,12} */
    ot_u8 depth_prec; /* the decimal bits wide of depth, {0,2,4} .one-to-one correspondence fb_norm_bits. */
    ot_u8 rsv;
} ot_avp_dpu_match_depth_param_v1;

typedef struct {
    ot_avp_dpu_match_depth_version version;
    union {
        ot_avp_dpu_match_depth_param_v1 v1; /* v1 param,AUTO:ot_dpu_match_depth_version:OT_AVP_DPU_MATCH_DEPTH_VERSION_V1 */
    };
} ot_avp_dpu_match_depth_param;

typedef struct {
    ot_eis_img_size left_image_size; /* left image size. */
    ot_eis_img_size right_image_size; /* right image size. */
    ot_s32 invalid_disp;  /* output this value,when cannot match valid disparity, [-225, 223] << 6 */
    ot_u32 depth; /* the depth of user image queue for getting match output image,
                     it can not be changed dynamic. range:[0,8] */
    ot_bool is_need_src_frame; /* the flag of getting source videoframe.it will effect when bind dpu rect. */
    ot_avp_dpu_match_mem_info assist_buf; /* assistance buffer. */
    ot_eis_frame_rate_ctrl frame_rate; /* grp frame rate control. */
    ot_avp_dpu_match_disp_param disp_param; /*  disparity param */
    ot_avp_dpu_match_depth_param depth_param; /* disparity to depth param */
} ot_avp_dpu_match_grp_attr;

typedef struct {
    ot_eis_img_size image_size; /* output image size. */
} ot_avp_dpu_match_chn_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
