#include "ot_semu_memmap.h"
#include "semu_log.h"
#include <stdio.h>

#define PHYS_ADDR 0x50000000
#define PHYS_ADDR_LEN 0x1000

ot_s32 semu_sample_map(ot_void)
{
    ot_void* buffer = OT_NULL;
    ot_ulong mapped_addr = 0ul;
    ot_ulong mapped_size = 0ul;

    buffer = ot_semu_map(PHYS_ADDR, PHYS_ADDR_LEN, &mapped_addr, &mapped_size);
    if (buffer == OT_NULL) {
        semu_error("ot_semu_map failed!\n");
        return -1;
    }

    semu_info("0x50000000:0x%08x\n", *(ot_u32*)buffer);
    semu_info("0x50000004:0x%08x\n", *(ot_u32*)(buffer + 4));

    *(ot_u32*)buffer = 0x01010101;
    *(ot_u32*)(buffer + 4) = 0x01010101;

    semu_info("after write\n");
    semu_info("0x50000000:0x%08x\n", *(ot_u32*)buffer);
    semu_info("0x50000004:0x%08x\n", *(ot_u32*)(buffer + 4));

    ot_semu_unmap(mapped_addr, mapped_size);
    return 0;
}