/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef OT_BC_ERROR_H
#define OT_BC_ERROR_H
#include "ot_eis_type.h"
#include "ot_eis_errno.h"
#include "ot_eis_mod.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OT_BC_ERR(ERR)                  OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_BC,ERR)
#define OT_BC_ERROR_ILLEGAL_PARAM       OT_BC_ERR(OT_EIS_ERRNO_ILLEGAL_PARAM)
#define OT_BC_ERROR_UNEXIST             OT_BC_ERR(OT_EIS_ERRNO_NO_EXIST)
#define OT_BC_ERROR_NOT_INIT            OT_BC_ERR(OT_EIS_ERRNO_NO_INIT)
/*invalid slot info*/
#define OT_BC_ERROR_NOT_SUPPORT_CFG     OT_BC_ERR(OT_EIS_ERRNO_BUTT + 1)
/*checksum failed*/
#define OT_BC_ERROR_INVALID_CRC_BLK0              OT_BC_ERR(OT_EIS_ERRNO_BUTT + 2)
#define OT_BC_ERROR_INVALID_CRC_ALL_BLKS          OT_BC_ERR(OT_EIS_ERRNO_BUTT + 3)
#define OT_BC_ERROR_WRITE_BLK0                    OT_BC_ERR(OT_EIS_ERRNO_BUTT + 4)
#define OT_BC_ERROR_WRITE_BLK1                    OT_BC_ERR(OT_EIS_ERRNO_BUTT + 5)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !OT_BC_ERROR_H*/