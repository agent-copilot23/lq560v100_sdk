/******************************************************************************
 Copyright (C), 2022 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : the cve error code.
Created       : 2022/11/16
Last Modified :
******************************************************************************/
#ifndef OT_AVP_CVE_ERRNO_H
#define OT_AVP_CVE_ERRNO_H

#include "ot_eis_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define CVE_MOD (0x81)

#define HW_TIMEOUT_ERRCODE          (36)
#define INVALID_HANDLE_ID_ERRCODE   (37)
#define DEV_EXIST_ERRCODE           (38)
#define DEV_UNEXIST_ERRCODE         (39)
#define OPEN_DEV_FAIL_ERRCODE       (40)
#define TASK_RUNNING_ERRCODE        (41)

typedef enum {
    OT_AVP_CVE_ERR_FAIL    = OT_FAILURE,                                                         /* Indicate the execution is failed */
    OT_AVP_CVE_SUCCESS     = OT_SUCCESS,                                                         /* Indicate the execution is successful */

    OT_AVP_CVE_ERR_BAD_ADDR          = OT_EIS_ERRNO_DEFINE(CVE_MOD, OT_EIS_ERRNO_INVALID_ADDR),  /* Indicate the input phys addr or virt addr is illegal */
    OT_AVP_CVE_ERR_INVALID_HANDLE_ID = OT_EIS_ERRNO_DEFINE(CVE_MOD, INVALID_HANDLE_ID_ERRCODE),  /* Indicate the handle is invalid */
    OT_AVP_CVE_ERR_DEV_EXIST         = OT_EIS_ERRNO_DEFINE(CVE_MOD, DEV_EXIST_ERRCODE),          /* Indicate the device exists */
    OT_AVP_CVE_ERR_DEV_UNEXIST       = OT_EIS_ERRNO_DEFINE(CVE_MOD, DEV_UNEXIST_ERRCODE),        /* Indicate the device not exist */
    OT_AVP_CVE_ERR_OPEN_DEV_FAIL     = OT_EIS_ERRNO_DEFINE(CVE_MOD, OPEN_DEV_FAIL_ERRCODE),      /* Indicate driver open device fail */
    OT_AVP_CVE_ERR_HW_TIMEOUT        = OT_EIS_ERRNO_DEFINE(CVE_MOD, HW_TIMEOUT_ERRCODE),         /* Indicate task hardware execute timeout */
    OT_AVP_CVE_ERR_SW_TIMEOUT        = OT_EIS_ERRNO_DEFINE(CVE_MOD, OT_EIS_ERRNO_TIMEOUT),       /* Indicate task software execute timeout */
    OT_AVP_CVE_ERR_INVALID_ARGUMENTS = OT_EIS_ERRNO_DEFINE(CVE_MOD, OT_EIS_ERRNO_ILLEGAL_PARAM), /* Indicate some arguments are invalid */
    OT_AVP_CVE_ERR_NULL_PTR          = OT_EIS_ERRNO_DEFINE(CVE_MOD, OT_EIS_ERRNO_NULL_PTR),      /* Indicate using a NULL pointer */
    OT_AVP_CVE_ERR_NO_MEMORY         = OT_EIS_ERRNO_DEFINE(CVE_MOD, OT_EIS_ERRNO_NO_MEM),        /* Indicate there is no enough system memory */
    OT_AVP_CVE_ERR_TASK_RUNNING      = OT_EIS_ERRNO_DEFINE(CVE_MOD, TASK_RUNNING_ERRCODE),       /* Indicate task not done */
} ot_avp_cve_status_e;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif