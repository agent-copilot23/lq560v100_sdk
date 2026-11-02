/******************************************************************************
 Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : vo chn settings
Created       : 2023/5/31
Last Modified : 2023/5/31
******************************************************************************/

#ifndef OT_EIS_VO_COMMON
#define OT_EIS_VO_COMMON

#include "ot_eis_type.h"
#include "ot_eis_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

//max supported surface num
#define OT_VO_MAX_SURF_NUM 2

#define OT_VO_MAX_PORT_NUM 4

#define OT_VO_MAX_DISP_NUM 1

//max binded surface num by display
#define OT_VO_MAX_BIND_SURF_NUM 2

//max binded port num by surface
#define OT_VO_MAX_BIND_PORT_NUM 4

#define OT_VO_MIN_IN_PORT_WIDTH           32
#define OT_VO_MIN_IN_PORT_HEIGHT          32

#define OT_VO_MAX_IN_PORT_IMG_WIDTH       16384
#define OT_VO_MAX_IN_PORT_IMG_HEIGHT      8192

#define OT_VO_MAX_IN_PORT_WIDTH           4096
#define OT_VO_MAX_IN_PORT_HEIGHT          4096

#define OT_VO_MIN_SURFACE_WIDTH           32
#define OT_VO_MIN_SURFACE_HEIGHT          32

#define OT_VO_MAX_SURFACE_WIDTH           4096
#define OT_VO_MAX_SURFACE_HEIGHT          4096

#define OT_VO_MIN_DISPLAY_WIDTH           32
#define OT_VO_MIN_DISPLAY_HEIGHT          32

#define OT_VO_MAX_DISPLAY_WIDTH           4096
#define OT_VO_MAX_DISPLAY_HEIGHT          4096

#define OT_VO_BYPASS_BUF_LEN              0
#define OT_VO_MIN_BUF_LEN                 3
#define OT_VO_MAX_BUF_LEN                 15

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif