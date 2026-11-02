/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */

#ifndef OT_OMI_SYS_MEM_H
#define OT_OMI_SYS_MEM_H

#include "ot_eis_type.h"
#include "ot_common_sys_mem.h"

#ifdef __cplusplus
extern "C" {
#endif

ot_void *ot_omi_sys_mmap(ot_phys_addr phys_addr, ot_u32 size);

ot_void *ot_omi_sys_mmap_cached(ot_phys_addr phys_addr, ot_u32 size);

ot_s32 ot_omi_sys_munmap(const ot_void *virt_addr, ot_u32 size);

/* alloc smr memory in user context */
ot_s32 ot_omi_sys_smr_alloc(ot_phys_addr *phys_addr, ot_void **virt_addr,
    const ot_char *mmb, const ot_char *zone, ot_u32 len);

/* alloc smr memory with cache in user context */
ot_s32 ot_omi_sys_smr_alloc_cached(ot_phys_addr *phys_addr, ot_void **virt_addr,
    const ot_char *mmb, const ot_char *zone, ot_u32 len);

/* free smr memory in user context */
ot_s32 ot_omi_sys_smr_free(ot_phys_addr phys_addr, const ot_void *virt_addr);

/* flush cache */
ot_s32 ot_omi_sys_flush_cache(ot_phys_addr phys_addr, ot_void *virt_addr, ot_u32 size);

/* get virtual meminfo according to virtual addr, should be in one process */
ot_s32 ot_omi_sys_get_virt_mem_info(const ot_void *virt_addr, ot_sys_virt_mem_info *mem_info);

ot_s32 ot_omi_sys_mem_share(const ot_void *mem_handle, ot_s32 pid);
ot_s32 ot_omi_sys_mem_unshare(const ot_void *mem_handle, ot_s32 pid);
ot_s32 ot_omi_sys_mem_share_all(const ot_void *mem_handle);
ot_s32 ot_omi_sys_mem_unshare_all(const ot_void *mem_handle);

ot_s32 ot_omi_sys_get_mem_info_by_virt(const ot_void *virt_addr, ot_sys_mem_info *mem_info);
ot_s32 ot_omi_sys_get_mem_info_by_phys(ot_phys_addr phys_addr, ot_sys_mem_info *mem_info);
ot_s32 ot_omi_sys_get_mem_info_by_handle(const ot_void *mem_handle, ot_sys_mem_info *mem_info);

#ifdef __cplusplus
}
#endif

#endif /* OT_OMI_SYS_MEM_H */
