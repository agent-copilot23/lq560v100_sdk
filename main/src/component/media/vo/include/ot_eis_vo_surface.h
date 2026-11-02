/******************************************************************************
 Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : vo surface settings
Created       : 2023/5/30
Last Modified : 2023/5/30
******************************************************************************/

#ifndef OT_EIS_VO_SURFACE
#define OT_EIS_VO_SURFACE

#include "ot_eis_vo_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/**
 * @brief vo surface创建
 *
 * @param surface类型枚举
 * @param surface_handle 输出surface句柄
 * @param attr surface属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_create(ot_eis_vo_surface surface, ot_eis_handle *surface_handle, const ot_eis_vo_surface_attr *attr);

/**
 * @brief vo surface销毁
 *
 * @param surface类型枚举
 * @param surface_handle 输出surface句柄
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_destroy(ot_eis_handle surface_handle);

/**
 * @brief 将surface与port绑定，可重复调用以更新属性
 *
 * @param surface_handle surface句柄
 * @param port_handle port句柄
 * @param attr bind属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_bind(ot_eis_handle surface_handle, ot_eis_handle port_handle, const ot_eis_vo_surface_bind_attr *attr);

/**
 * @brief 将surface与port解绑
 *
 * @param surface_handle surface句柄
 * @param port_handle port句柄
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_unbind(ot_eis_handle surface_handle, ot_eis_handle port_handle);

/**
 * @brief 设置surface在display中的显示order
 *
 * @param surface_handle surface句柄
 * @param order order枚举
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_set_order(ot_eis_handle surface_handle, ot_eis_vo_surface_order order);

/**
 * @brief 获取surface在display中的显示order
 *
 * @param surface_handle surface句柄
 * @param order 输出order枚举
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_get_order(ot_eis_handle surface_handle, ot_eis_vo_surface_order *order);

/**
 * @brief 设置surface的mirror属性
 *
 * @param surface_handle surface句柄
 * @param mirror_attr mirror属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_set_mirror(ot_eis_handle surface_handle, const ot_eis_vo_mirror_attr *mirror_attr);

/**
 * @brief 获取surface的mirror属性
 *
 * @param surface_handle surface句柄
 * @param mirror_attrer 输出mirror_attr属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_get_mirror(ot_eis_handle surface_handle, ot_eis_vo_mirror_attr *mirror_attr);

/**
 * @brief 设置surface的crop参数
 *
 * @param handle surface句柄
 * @param crop_attr attr属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_set_crop(ot_eis_handle handle, const ot_eis_vo_crop_attr *crop_attr);

/**
 * @brief 获取surface的crop参数
 *
 * @param handle surface句柄
 * @param crop_attr attr属性
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_get_crop(ot_eis_handle handle, ot_eis_vo_crop_attr *crop_attr);

/**
 * @brief 设置surface 色彩空间转换
 *
 * @param handle surface句柄
 * @param attr csc参数
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_set_csc(ot_eis_handle handle, const ot_eis_vo_csc_attr *attr);

/**
 * @brief 设置surface 色彩空间转换
 *
 * @param handle surface句柄
 * @param attr
 * @return ot_s32
 */
ot_s32 ot_eis_vo_surface_get_csc(ot_eis_handle handle, ot_eis_vo_csc_attr *attr);

/**
 * @brief 设置surface 色彩空间转换
 *
 * @param handle surface句柄
 * @param attr cgc属性
 * @return ot_s32
 */

/**
 * @brief 获取surface的frame
 * 
 * @param surface_handle surface handle标识
 * @param frame frame信息
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_get_frame(ot_eis_handle surface_handle, ot_eis_video_frame *frame_info);

/**
 * @brief 释放surface的frame
 * 
 * @param surface_handle surface handle标识
 * @param frame frame信息
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_surface_release_frame(ot_eis_handle surface_handle, const ot_eis_video_frame *frame_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif