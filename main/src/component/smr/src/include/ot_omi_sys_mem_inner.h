/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */

#ifndef OT_OMI_SYS_MEM_INNER_H
#define OT_OMI_SYS_MEM_INNER_H

#include "ot_eis_type.h"

#ifdef __cplusplus
extern "C" {
#endif

ot_s32 ot_omi_sys_smr_check_phy_addr(ot_phys_addr phys_addr, ot_ulong size);

ot_s32 ot_omi_sys_smr_alloc_only(ot_phys_addr *phys_addr, const ot_char *smr_name,
    const ot_char *buf_name, ot_ulong size);

ot_s32 ot_omi_sys_smr_free_only(ot_phys_addr phys_addr, const ot_void *virt_addr);

ot_void *ot_omi_sys_smr_remap_nocache(ot_phys_addr phys_addr, ot_u32 size);

ot_void *ot_omi_sys_smr_remap_cached(ot_phys_addr phys_addr, ot_u32 size);

ot_s32 ot_omi_sys_smr_unmap(const ot_void *virt_addr);

#ifdef __cplusplus
}
#endif

#endif /* OT_OMI_SYS_MEM_INNER_H */
