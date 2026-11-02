#ifndef __SMR_COMMON__
#define __SMR_COMMON__

#include "ot_eis_type.h"
#include "ot_eis_errno.h"
#include "ot_eis_mod.h"


#ifdef __cplusplus
extern "C" {
#endif

#define OT_SMR_REGION_NAME_LEN_MAX (32)
#define OT_SMR_CHUNK_NAME_MAX (32)

typedef struct {
    ot_s8 region_name[OT_SMR_REGION_NAME_LEN_MAX];
    ot_s8 chunk_name[OT_SMR_CHUNK_NAME_MAX];
    ot_u64 len;
    ot_u32 align;
    ot_bool cached;
} ot_smr_alloc_attr;

typedef enum {
    OT_SMR_NOT_INIT = 0x20,
    OT_SMR_DEPEND_NOT_READY,
    OT_SMR_BUSY,
    OT_SMR_BUTT
} ot_smr_err_code;

#define OT_ERR_SMR_DEFINE(errno)      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_SMR, errno)

#define OT_ERR_SMR_NULL_PTR           OT_ERR_SMR_DEFINE(OT_EIS_ERRNO_NULL_PTR)
#define OT_ERR_SMR_ILLEGAL_PAARAM     OT_ERR_SMR_DEFINE(OT_EIS_ERRNO_ILLEGAL_PARAM)
#define OT_ERR_SMR_NO_MEM             OT_ERR_SMR_DEFINE(OT_EIS_ERRNO_NO_MEM)
#define OT_ERR_SMR_NOT_INIT           OT_ERR_SMR_DEFINE(OT_SMR_NOT_INIT)
#define OT_ERR_SMR_DEPEND_NOT_READY   OT_ERR_SMR_DEFINE(OT_SMR_DEPEND_NOT_READY)
#define OT_ERR_SMR_NOT_SUPPORT        OT_ERR_SMR_DEFINE(OT_EIS_ERRNO_NO_SUPPORT)
#define OT_ERR_SMR_BUSY               OT_ERR_SMR_DEFINE(OT_SMR_BUSY)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
