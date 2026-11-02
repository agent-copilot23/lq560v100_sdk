/******************************************************************************
 Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : vo dev settings
Created       : 2023/5/30
Last Modified : 2023/5/30
******************************************************************************/
#ifndef __OT_EIS_VO_SYS__
#define __OT_EIS_VO_SYS__

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/**
 * @brief 初始化vo子系统
 * 
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_init(ot_void);

/**
 * @brief 重置vo子系统
 * 
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_reset(ot_void);

/**
 * @brief 注销vo子系统
 * 
 * @return ot_s32 0：成功；非0：失败
 */
ot_s32 ot_eis_vo_deinit(ot_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
