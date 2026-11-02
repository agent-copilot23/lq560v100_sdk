/***********************************************************************************
*  Copyright (C), 2022 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
*  ********************************************************************************
*  Description   : declear the error number for os level
*  Created       : 2022/11/11
*  Last Modified : 2022/11/11
*  Group         : SDK Team
*  ********************************************************************************/

#ifndef OT_EIS_ERRNO_H
#define OT_EIS_ERRNO_H

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef OT_SUCCESS
#define OT_SUCCESS              0
#define OT_FAILURE      (-1)
#endif

#define OT_EIS_ERRNO_ID_BASE  (0x60000000L)
#define OT_EIS_ERRNO_OS_ID    (OT_EIS_ERRNO_ID_BASE + 0x20000000L)

/*
 * |---------------------------------------------|
 * | 1 |   OS_ID   |   MOD_ID     |    ERR_NO    |
 * |---------------------------------------------|
 * |<--><--7bits----><----8bits---><---16bits--->|
 */
#define OT_EIS_ERRNO_DEFINE(mod, err_no) \
    ((ot_s32)((OT_EIS_ERRNO_OS_ID) | ((mod) << 16) | (err_no)))


typedef enum {
    OT_EIS_ERRNO_ILLEGAL_PARAM   = 0, /* at lease one parameter is illegal                      */
    OT_EIS_ERRNO_NO_EXIST        = 1, /* resource not exists                                    */
    OT_EIS_ERRNO_NULL_PTR        = 2, /* using a NULL pointer                                   */
    OT_EIS_ERRNO_NO_SUPPORT      = 3, /* operation or type is not supported by NOW              */
    OT_EIS_ERRNO_NO_ACCESS       = 4, /* operation is not permitted*/
    OT_EIS_ERRNO_NO_ENABLE      = 5,
    OT_EIS_ERRNO_NO_DISABLE     = 6,
    OT_EIS_ERRNO_NO_START       = 7,
    OT_EIS_ERRNO_NO_STOP        = 8,
    OT_EIS_ERRNO_NO_MEM          = 9, /* failure caused by malloc memory                       */
    OT_EIS_ERRNO_BUF_EMPTY       = 10, /* no data in buffer                                     */
    OT_EIS_ERRNO_BUF_FULL        = 11, /* no buffer for new data                                */
    OT_EIS_ERRNO_NO_INIT        = 12,
    OT_EIS_ERRNO_TIMEOUT         = 13,
    OT_EIS_ERRNO_INVALID_ADDR    = 14, /* bad address,  */
    OT_EIS_ERRNO_BUSY            = 15,
    OT_EIS_ERRNO_OVER_SIZE       = 16, /* buffer size is smaller than the actual size required  */
    OT_EIS_ERRNO_NO_SPACE        = 17,
    OT_EIS_ERRNO_ALREADY_INIT    = 18,
    OT_EIS_ERRNO_ALREADY_CREATE  = 19,

    OT_EIS_ERRNO_BUTT                    /* maximum code, private error code of all modules
                                      ** must be greater than it                               */
} ot_eis_err_code;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
