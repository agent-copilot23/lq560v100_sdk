/******************************************************************************
 Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : vo port settings
Created       : 2023/7/24
Last Modified : 2023/7/24
******************************************************************************/

#ifndef OT_EIS_VO_IN_PORT
#define OT_EIS_VO_IN_PORT

#include "ot_eis_vo_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



/**
 * @brief 创建vo port
 *
 * @param handle 输出port handle信息
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_in_port_create(ot_eis_handle *handle, const ot_eis_vo_in_port_attr *attr);

/**
 * @brief 销毁vo的输入port
 *
 * @param handle port handle标识
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_in_port_destroy(ot_eis_handle handle);

/**
 * @brief 用户态送帧
 *
 * @param handle port handle
 * @param frame frame信息
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_in_port_send_frame(ot_eis_handle handle, const ot_eis_video_frame *frame);

/**
 * @brief 使能并设置mirror方向
 *
 * @param handle in_port句柄
 * @param mirror_attr mirror属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_in_port_set_mirror(ot_eis_handle handle, const ot_eis_vo_mirror_attr *mirror_attr);

/**
 * @brief 获取mirror方向
 *
 * @param handle in_port句柄
 * @param mirror_attr mirror属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_in_port_get_mirror(ot_eis_handle handle, ot_eis_vo_mirror_attr *mirror_attr);

/**
 * @brief 设置in port旋转属性
 *
 * @param handle in_port句柄
 * @param rot_attr rot属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_in_port_set_rotation(ot_eis_handle handle, const ot_eis_vo_rot_attr *rot_attr);

/**
 * @brief 获取in port旋转属性
 *
 * @param handle in_port句柄
 * @param rot_attr rot属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_in_port_get_rotation(ot_eis_handle handle, ot_eis_vo_rot_attr *rot_attr);

/**
 * @brief 获取in port的frame
 * 
 * @param handle handle标识
 * @param frame frame信息
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_in_port_get_frame(ot_eis_handle handle, ot_eis_video_frame *frame_info);

/**
 * @brief 释放in port的frame
 * 
 * @param handle handle标识
 * @param frame frame信息
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_in_port_release_frame(ot_eis_handle handle, const ot_eis_video_frame *frame_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif