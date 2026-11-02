/******************************************************************************
 Copyright (C), 2024 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : the application interface struct and macro code etc.
Created       : 2024/09/23
Last Modified :
******************************************************************************/
#ifndef OT_AVP_DPU_RECT_COMMON_H
#define OT_AVP_DPU_RECT_COMMON_H

#include "ot_eis_type.h"
#include "ot_eis_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define OT_AVP_DPU_RECT_MAX_PIPE_NUM 2
#define OT_AVP_DPU_RECT_MAX_CHN_NUM 2
#define OT_AVP_DPU_RECT_UNWARP_IH3X_FIXED_NUM 3

typedef struct {
    ot_u64 phys_addr;
    ot_u64 virt_addr;
    ot_u32 size;
} ot_avp_dpu_rect_mem_info;

typedef struct {
    ot_eis_img_frame src_frame[OT_AVP_DPU_RECT_MAX_PIPE_NUM]; /* src_frame[0]:left frame,src_frame[1]:right frame */
    ot_eis_img_frame rect_frame[OT_AVP_DPU_RECT_MAX_CHN_NUM]; /* rect_frame[0]:left frame,rect_frame[1]:right frame */
} ot_avp_dpu_rect_frame_info;

typedef enum {
    OT_AVP_DPU_RECT_MODE_SINGLE = 0x0, /* only channel 0 work */
    OT_AVP_DPU_RECT_MODE_DOUBLE = 0x1, /* two channel work */
    OT_AVP_DPU_RECT_MODE_INVALID
} ot_avp_dpu_rect_mode;

typedef enum {
    OT_AVP_DPU_RECT_UPSAMPLE_SCALE_0 = 0x0, /* no scale */
    OT_AVP_DPU_RECT_UPSAMPLE_SCALE_1 = 0x1, /* scale 2^1 times */
    OT_AVP_DPU_RECT_UPSAMPLE_SCALE_2 = 0x2, /* scale 2^2 times */

    OT_AVP_DPU_RECT_UPSAMPLE_SCALE_INVALID
} ot_avp_dpu_rect_upsample_scale;

/* unwarp */
typedef struct {
    ot_bool is_unwarp; /* the flag of unwarp */
    int unwarp_ih3x_fixed[OT_AVP_DPU_RECT_UNWARP_IH3X_FIXED_NUM]; /* unwarp param .precision ：1.2.21 */
} ot_avp_dpu_rect_unwarp_attr;

/* depth */
typedef struct {
    ot_bool is_invalid_depth_proc; /* the flag of invalid depth proc */
    ot_u32  invalid_depth_thr;     /* invalid depth threshold */
} ot_avp_dpu_rect_depth_attr;

/* pipe */
typedef struct {
    ot_eis_img_size image_size; /* image size. */
    ot_s32 lut_id; /* rectification lut */
    ot_avp_dpu_rect_upsample_scale upsample_scale; /* unsample scale */
    ot_avp_dpu_rect_unwarp_attr unwarp_attr; /* unwarp */
    ot_avp_dpu_rect_depth_attr depth_attr; /* depth */
} ot_avp_dpu_rect_pipe_attr;

typedef struct {
    ot_avp_dpu_rect_mode rect_mode; /* rectification mode, it can not be changed dynamic */
    ot_u32 depth; /* the depth of user image queue for getting rectification output image,
                     * it can not be changed dynamic. range:[0,8]
                     */
    ot_bool is_need_src_frame; /* the flag of getting source video frame. */
    ot_eis_frame_rate_ctrl frame_rate; /* grp frame rate control. */
    ot_avp_dpu_rect_pipe_attr left_attr; /* left */
    ot_avp_dpu_rect_pipe_attr right_attr; /* right */
} ot_avp_dpu_rect_grp_attr;

typedef struct {
    ot_eis_img_size image_size; /* rectification output image size */
} ot_avp_dpu_rect_chn_attr;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
