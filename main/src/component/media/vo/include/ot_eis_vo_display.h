/******************************************************************************
 Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : vo display panel settings
Created       : 2023/6/5
Last Modified : 2023/6/5
******************************************************************************/
#ifndef OT_EIS_VO_DISPLAY
#define OT_EIS_VO_DISPLAY

#include "ot_eis_vo_type.h"
#include "ot_eis_vo_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/**
 * @brief 创建display
 *
 * @param display枚举
 * @param 输出display句柄
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_create(ot_eis_vo_display display, ot_eis_handle *display_handle,
	const ot_eis_vo_display_attr *display_attr);

/**
 * @brief 销毁display panel
 *
 * @param display枚举
 * @param 输出display句柄
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_destroy(ot_eis_handle display_handle);

/**
 * @brief 将display与surface绑定，可重复调用以更新属性
 *
 * @param display_handle display句柄
 * @param surface_handle surface句柄
 * @param attr bind属性
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_bind(ot_eis_handle display_handle, ot_eis_handle surface_handle,
	const ot_eis_vo_display_bind_attr *attr);

/**
 * @brief 将display与surface解绑
 *
 * @param display_handle display句柄
 * @param surface_handle surface句柄
 * @param attr bind属性
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_unbind(ot_eis_handle display_handle, ot_eis_handle surface_handle);

/**
 * @brief 启动display panel
 *
 * @param display_handle display handle
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_start(ot_eis_handle display_handle);

/**
 * @brief 停止display panel
 *
 * @param display_handle display handle
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_stop(ot_eis_handle display_handle);

/**
 * @brief 设置display上mipi接口的参数
 *
 * @param display_handle display handle
 * @param mipi_param mipi接口参数
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_set_mipi_param(ot_eis_handle display_handle, const ot_eis_vo_mipi_param *mipi_param);

/**
 * @brief 获取display上mipi接口的参数
 *
 * @param display_handle display handle
 * @param mipi_param mipi接口参数
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_get_mipi_param(ot_eis_handle display_handle, ot_eis_vo_mipi_param *mipi_param);

/**
 * @brief 设置display上bt接口的参数
 *
 * @param display_handle display handle
 * @param bt_param bt接口参数
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_set_bt_param(ot_eis_handle display_handle, const ot_eis_vo_bt_param *bt_param);

/**
 * @brief 获取display上bt接口的参数
 *
 * @param display_handle display handle
 * @param bt_param bt接口参数
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_get_bt_param(ot_eis_handle display_handle, const ot_eis_vo_bt_param *bt_param);

/**
 * @brief 设置display上rgb接口的参数
 *
 * @param display_handle display handle
 * @param rgb_param rgb接口参数
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_set_rgb_param(ot_eis_handle display_handle, const ot_eis_vo_rgb_param *rgb_param);

/**
 * @brief 获取display上rgb接口的参数
 *
 * @param display_handle display handle
 * @param rgb_param rgb接口参数
 * @return ot_s32 结果 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_display_get_rgb_param(ot_eis_handle display_handle, ot_eis_vo_rgb_param *rgb_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif