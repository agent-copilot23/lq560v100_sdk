/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#include <string.h>
#include "sample_utils.h"
#include "ot_smr.h"
#include "ot_omi_cipher.h"
#include "ot_eis_errno.h"

#define BYTES_IN_WORD   4

ot_void cipher_free(const crypto_buf_attr *buf_attr, const void *virt_addr)
{
    if (buf_attr->phys_addr != 0 && virt_addr != OT_NULL) {
        ot_smr_free(buf_attr->phys_addr);
    }
}

ot_s32 cipher_alloc(crypto_buf_attr *buf_attr, void **virt_addr, unsigned int size)
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
    buf_attr->phys_addr = (unsigned long) phys_addr;
    return OT_SUCCESS;
}

ot_s32 get_random_data(ot_u8 *buffer, ot_u32 size)
{
    if (size % BYTES_IN_WORD != 0) {
        sample_err("invalid size!\n");
        return OT_FAILURE;
    }
    if (buffer == OT_NULL) {
        sample_err("invalid buffer!\n");
        return OT_FAILURE;
    }
    ot_s32 ret;
    ot_u32 random_num = 0;
    ot_u32 i, j;
    ot_u32 times = size / BYTES_IN_WORD;
    for (i = 0; i < times; i++) {
        ret = ot_omi_cipher_trng_get_random(&random_num);
        if (ret != OT_SUCCESS) {
            sample_err("ot_omi_cipher_trng_get_random error!\n");
            return ret;
        }
        for (j = 0; j < BYTES_IN_WORD; j++) {
            buffer[i * BYTES_IN_WORD + j] = ((ot_u8 *)(&random_num))[j];
        }
    }
    return ret;
}

static ot_s32 get_rsa3072_key(drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_priv_key *priv_key)
{
    to_be_processed(pub_key);
    to_be_processed(priv_key);
    sample_err("Please implement this function to get rsa3072 key!!!\n");
    return OT_FAILURE;
}

static ot_s32 get_rsa4096_key(drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_priv_key *priv_key)
{
    to_be_processed(pub_key);
    to_be_processed(priv_key);
    sample_err("Please implement this function to get rsa4096 key!!!\n");
    return OT_FAILURE;
}

ot_s32 get_rsa_key(drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_priv_key *priv_key, ot_u32 key_len)
{
    ot_s32 ret = OT_SUCCESS;
    if (pub_key == OT_NULL || priv_key == OT_NULL) {
        return OT_FAILURE;
    }
    switch (key_len) {
        case RSA_3072_KEY_LEN:
            sample_chk_expr_return(get_rsa3072_key(pub_key, priv_key), OT_SUCCESS);
            break;
        case RSA_4096_KEY_LEN:
            sample_chk_expr_return(get_rsa4096_key(pub_key, priv_key), OT_SUCCESS);
            break;
        default:
            sample_err("Unsupported key length!!!\n");
            return OT_FAILURE;
    }
    priv_key->n_len = key_len;
    priv_key->d_len = key_len;
    pub_key->len = key_len;
    return ret;
}

ot_void destroy_rsa_key(drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_priv_key *priv_key, ot_u32 key_len)
{
    to_be_processed(pub_key);
    to_be_processed(priv_key);
    to_be_processed(key_len);
    sample_err("Please ensure that the key is cleared after use!!!\n");
}
