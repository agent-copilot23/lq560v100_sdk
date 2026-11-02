/***********************************************************************************
*  Copyright (C), 2022 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
*  ********************************************************************************
*  Description   : declear the error number for os level
*  Created       : 2022/11/11
*  Last Modified : 2022/11/11
*  Group         : SDK Team
*  ********************************************************************************/

#ifndef OT_EIS_MOD_ID_H
#define OT_EIS_MOD_ID_H

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef enum {
    OT_EIS_MODULE_VI   = 0x0,
    OT_EIS_MODULE_VPROC,
    OT_EIS_MODULE_VO,
    OT_EIS_MODULE_SMR,
    OT_EIS_MODULE_VRB,
    OT_EIS_MODULE_BUFPOOL,
    OT_EIS_MODULE_PIPLINE,
    OT_EIS_MODULE_MIPIRX,
    OT_EIS_MODULE_VENC,
    OT_EIS_MODULE_VDEC,
    OT_EIS_MODULE_AO,
    OT_EIS_MODULE_MIPITX,
    OT_EIS_MODULE_OGE,
    OT_EIS_MODULE_GFB,
    OT_EIS_MODULE_AIISP,
    OT_EIS_MODULE_MEDIASYS,
    OT_EIS_MODULE_VI_SERDES,
    OT_EIS_MODULE_VO_SERDES,
    OT_EIS_MODULE_BC,
    OT_EIS_MODULE_SCENE_AUTO,
    OT_EIS_MODULE_DPU_RECT,
    OT_EIS_MODULE_DPU_MATCH,
    OT_EIS_MODULE_BOARD_INFO,
    OT_EIS_MODULE_INVALID

} ot_eis_module_id;

typedef enum {
    OT_EIS_MINOR_NUM_VI = 100,
    OT_EIS_MINOR_NUM_VO,
    OT_EIS_MINOR_NUM_VPROC,
    OT_EIS_MINOR_NUM_BUTT
} ot_eis_minor_num;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
