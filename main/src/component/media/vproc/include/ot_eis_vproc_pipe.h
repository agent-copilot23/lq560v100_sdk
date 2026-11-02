#ifndef __OT_EIS_VPROC_PIPE_H__
#define __OT_EIS_VPROC_PIPE_H__

#include "ot_eis_video.h"
#include "ot_eis_vproc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 输入：attr pipe 的属性， 
 * 输出：pipe_hdl pipe 的handle
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：创建 vproc pipe, 返回 pipe handle
 */
ot_s32 ot_eis_vproc_pipe_create(ot_eis_handle* pipe_hdl, const ot_eis_vproc_pipe_attr* attr);

/*
 * 输入：pipe 的handle， 
 * 输出：无
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：vproc pipe 销毁，关闭通路 pipe, 需要在从 channel 解除绑定后才能关闭
 */
ot_s32 ot_eis_vproc_pipe_destroy(const ot_eis_handle pipe_hdl);

/*
 * 输入：pipe_hdl pipe 的 handle
 * 输入：attr pipe 的属性
 * 输出：无
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：设置 vproc pipe 属性, 在 channel start 前调用
 */
ot_s32 ot_eis_vproc_pipe_set_attr(const ot_eis_handle pipe_hdl, const ot_eis_vproc_pipe_attr* attr);

/*
 * 输入：pipe_hdl pipe 的 handle，
 * 输出：attr pipe 的属性
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：获取 vproc pipe 属性
 */
ot_s32 ot_eis_vproc_pipe_get_attr(const ot_eis_handle pipe_hdl, ot_eis_vproc_pipe_attr* attr);

/*
 * 输入：pipe_hdl pipe 的handle
 * 输入：frame_info 输入的帧信息数组
 * 输入：frame_num 输入帧的
 * 输出：无
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：app 调用此接口发送帧到 vproc pipe 输入队列
 */
ot_s32 ot_eis_vproc_pipe_send_frame(const ot_eis_handle pipe_hdl, const ot_eis_img_frame *frame_info, ot_u32 frame_num);

/*
 * 输入：pipe_hdl pipe 的handle
 * 输出：frame_info 输出的帧信息
 * 输入  timeout 输入帧的超时时间，-1表示阻塞模式
 * 输出：无
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：app 调用此接口从 vproc pipe 输入队列获取一帧
 */
ot_s32 ot_eis_vproc_pipe_acquire_frame(const ot_eis_handle pipe_hdl, ot_eis_img_frame* frame, ot_s32 timeout);

/*
 * 输入：pipe_hdl pipe 的handle
 * 输入：frame_info 输入的帧信息
 * 输出：无
 * 返回值：成功：0 ，错误： 错误码;
 * 功能：app 调用此接口发送帧到 vproc pipe 输入队列
 */
ot_s32 ot_eis_vproc_pipe_release_frame(const ot_eis_handle pipe_hdl, const ot_eis_img_frame *frame_info);

#ifdef __cplusplus
}
#endif

#endif