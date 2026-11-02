/***********************************************************************************
*  Copyright (C), 2022 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
*  ********************************************************************************
*  Description   : declear the error number for mipi rx
*  Created       : 2022/11/11
*  Last Modified : 2022/11/11
*  Group         : SDK Team
*  ********************************************************************************/

#ifndef OT_EIS_MIPI_RX_ERRNO_H
#define OT_EIS_MIPI_RX_ERRNO_H

#include "ot_eis_errno.h"
#include "ot_eis_mod.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef enum {
    OT_EIS_ERR_VI_IF_NO_DEV = 0x100,
    OT_EIS_ERR_VI_IF_HDL_NO_EXIST,
    OT_EIS_ERR_VI_IF_STATE_ERR,
    OT_EIS_ERR_VI_IF_OP_FAIL,
    OT_EIS_ERR_VI_IF_MAX
} ot_eis_vi_if_errno;


#define OT_MIPIRX_ERR_NOT_INIT   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERRNO_NO_INIT)

#define OT_MIPIRX_ERR_ALREADY_INIT   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERRNO_ALREADY_INIT)

#define OT_MIPIRX_ERR_STATE_ERR   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERR_VI_IF_STATE_ERR)

#define OT_MIPIRX_ERR_ILLEGAL_PARAM   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERRNO_ILLEGAL_PARAM)

#define OT_MIPIRX_ERR_NO_DEV   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERR_VI_IF_NO_DEV)

#define OT_MIPIRX_ERR_RES_BUSY   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERRNO_BUSY)

#define OT_MIPIRX_ERR_NO_MEM   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERRNO_NO_MEM)

#define OT_MIPIRX_ERR_NULL_PTR   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERRNO_NULL_PTR)

#define OT_MIPIRX_ERR_HDL_NO_EXIST   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERR_VI_IF_HDL_NO_EXIST)

#define OT_MIPIRX_ERR_OP_FAIL   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MIPIRX, OT_EIS_ERR_VI_IF_OP_FAIL)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
