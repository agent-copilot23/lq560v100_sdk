#ifndef OT_SEMU_MEMMAP_H
#define OT_SEMU_MEMMAP_H

#include "ot_eis_type.h"
#include "ot_eis_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

ot_void *ot_semu_map(ot_ulong phys, ot_ulong size, ot_ulong *mapped_addr, ot_ulong *mapped_size);
ot_void ot_semu_unmap(ot_ulong mapped_addr, ot_ulong size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif