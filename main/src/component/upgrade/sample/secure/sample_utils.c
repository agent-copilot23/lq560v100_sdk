/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#include <string.h>
#include "sample_utils.h"
#include "ot_smr.h"
#include "ot_omi_cipher.h"
#include "ot_eis_errno.h"

ot_void cipher_free(const crypto_buf_attr *buf_attr, const ot_void *virt_addr)
{
    if (buf_attr->phys_addr != 0 && virt_addr != OT_NULL) {
        ot_smr_free(buf_attr->phys_addr);
    }
}

ot_s32 cipher_alloc(crypto_buf_attr *buf_attr, ot_void **virt_addr, ot_u32 size)
{
    ot_s32 ret;
    ot_phys_addr phys_addr;
    ot_smr_alloc_attr attr;

    memset(&attr, 0x0, sizeof(attr));
    attr.len = size;
    attr.cached = OT_FALSE;
    ret = ot_smr_alloc(&attr, &phys_addr, virt_addr);
    if (ret != OT_SUCCESS) {
        sample_err("ot_smr_alloc failed\n");
        return OT_FAILURE;
    }
    buf_attr->phys_addr = (ot_ulong) phys_addr;
    return OT_SUCCESS;
}