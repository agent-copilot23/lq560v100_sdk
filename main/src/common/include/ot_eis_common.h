/*
  Copyright (c), 2021-2022, Oritek Tech. Co., Ltd.
 */

#ifndef OT_EIS_COMMON_H
#define OT_EIS_COMMON_H

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */


#define OT_EIS_ALIGN_NUM (8)

#define OT_EIS_VERSION_NAME_MAX_LEN (64)

#define ALIGN_ATTR __attribute__((aligned(OT_EIS_ALIGN_NUM)))

typedef struct {
    ot_s8 version[OT_EIS_VERSION_NAME_MAX_LEN];
} ot_eis_sdk_version;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* __OT_EIS_COMMON_H__ */
