#ifndef __SMR_EXT_HEAD__
#define __SMR_EXT_HEAD__

#include "ot_smr_common.h"

#ifdef __cplusplus
extern "C" {
#endif

ot_s32 ot_smr_init(ot_void);

ot_s32 ot_smr_deinit(ot_void);

ot_s32 ot_smr_alloc(const ot_smr_alloc_attr* attr, ot_phys_addr* phy_addr, ot_void** virt_addr);

ot_s32 ot_smr_free(ot_phys_addr phy_addr);

ot_s32 ot_smr_mmap(ot_phys_addr phy_addr, ot_u32 size, ot_bool cached, ot_void** virt_addr);

ot_s32 ot_smr_munmap(ot_void* virt_addr, ot_u32 size);

ot_s32 ot_smr_virt2phy(ot_void* virt_addr, ot_phys_addr* phy_addr);

ot_bool ot_smr_is_cached(ot_void* virt_addr);

ot_s32 ot_smr_flush_cache(ot_phys_addr phy_addr, ot_void* virt_addr, ot_u32 size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
