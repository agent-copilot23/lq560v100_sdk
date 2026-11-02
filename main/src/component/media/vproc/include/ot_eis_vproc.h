#ifndef __OT_EIS_VPROC_H__
#define __OT_EIS_VPROC_H__

#include "ot_eis_type.h"
#include "ot_eis_video.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 输入：无
 * 输出：无
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：vproc 初始化函数，调用一次
 */
ot_s32 ot_eis_vproc_init(ot_void);
/*
 * 输入：无
 * 输出：无
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：vproc 去初始化函数，调用一次，退出vproc模块
 */
ot_s32 ot_eis_vproc_deinit(ot_void);

/**
 * 输入：输入帧信息
 * 输出：输出帧信息
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：将帧从源拷贝到目标
 */
ot_s32 ot_eis_frame_dma_copy(const ot_eis_img_frame *src, ot_eis_img_frame *dst);
#ifdef __cplusplus
}
#endif

#endif