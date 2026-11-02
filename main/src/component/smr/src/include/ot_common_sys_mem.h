/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#ifndef OT_COMMON_SYS_MEM_H
#define OT_COMMON_SYS_MEM_H

#include "ot_eis_mod.h"
#include "ot_eis_type.h"
#include "ot_eis_errno.h"
#include "ot_smr_common.h"

#define OT_ERR_MEM_ILLEGAL_PARAM OT_ERR_SMR_ILLEGAL_PAARAM
#define OT_ERR_MEM_NULL_PTR      OT_ERR_SMR_NULL_PTR
#define OT_ERR_MEM_NOT_PERM      OT_ERR_SMR_NOT_SUPPORT
#define OT_ERR_MEM_NOT_READY     OT_ERR_SMR_DEPEND_NOT_READY
#define OT_ERR_MEM_BUSY          OT_ERR_SMR_BUSY

typedef struct {
    ot_phys_addr phys_addr;
    ot_bool is_cached;
} ot_sys_virt_mem_info;

typedef struct {
    ot_phys_addr phys_addr;
    ot_u64 offset;
    ot_void *mem_handle;
} ot_sys_mem_info;

#endif /* OT_COMMON_SYS_MEM_H */
