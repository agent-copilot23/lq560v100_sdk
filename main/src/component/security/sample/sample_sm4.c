/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include "securec.h"

#include "sample_utils.h"
#include "ot_omi_cipher.h"
#include "ot_omi_km.h"
#include "ot_omi_otp.h"
#include "ot_eis_errno.h"
#include "sample_func.h"

#define MAX_DATA_LEN        128
#define TEST_DATA_LEN       32

typedef struct {
    const ot_char *name;
    ot_u8 key[MAX_KEY_LEN];
    ot_u32 key_len;
    ot_u8 src_data[MAX_DATA_LEN];
    ot_u32 data_len;
    crypto_symc_attr symc_attr;
    crypto_symc_ctrl_t symc_ctrl;
} symc_data_t;

/* sm4 cbc/ctr */
static symc_data_t g_sm4_data[] = {
    {
        .name = "SM4-CBC-128BITS",
        .key_len  = SM4_KEY_LEN, .data_len = TEST_DATA_LEN,
        .symc_attr = {
            .symc_type = CRYPTO_SYMC_TYPE_NORMAL,
            .is_long_term = OT_FALSE,
        },
        .symc_ctrl = {
            .symc_alg = CRYPTO_SYMC_ALG_SM4,
            .work_mode = CRYPTO_SYMC_WORK_MODE_CBC,
            .symc_key_length = CRYPTO_SYMC_KEY_128BIT,
            .symc_bit_width = CRYPTO_SYMC_BIT_WIDTH_128BIT,
            .iv_length = IV_LEN,
        },
    },
    {
        .name = "SM4-CTR-128BITS",
        .key_len = SM4_KEY_LEN, .data_len = TEST_DATA_LEN,
        .symc_attr = {
            .symc_type = CRYPTO_SYMC_TYPE_NORMAL,
            .is_long_term = OT_FALSE,
        },
        .symc_ctrl = {
            .symc_alg = CRYPTO_SYMC_ALG_SM4,
            .work_mode = CRYPTO_SYMC_WORK_MODE_CTR,
            .symc_key_length = CRYPTO_SYMC_KEY_128BIT,
            .symc_bit_width = CRYPTO_SYMC_BIT_WIDTH_128BIT,
            .iv_length = IV_LEN,
        },
    },
};

static ot_s32 cipher_set_clear_key(crypto_handle keyslot_crypto_handle, ot_u8 *key, ot_u32 keylen)
{
    ot_s32 ret = OT_SUCCESS;
    crypto_handle klad_handle = 0;
    ot_u32 offset = 0x12;
    ot_u8 tee_enable = 0;
    km_klad_attr klad_attr = {
        .key_cfg = {
            .engine = KM_CRYPTO_ALG_SM4,
            .decrypt_support = OT_TRUE,
            .encrypt_support = OT_TRUE
        }
    };
    km_klad_clear_key clear_key = {
        .key = key,
        .key_size = keylen
    };
    (ot_void)ot_omi_otp_init();
    (ot_void)ot_omi_otp_read_byte(offset, &tee_enable);
    (ot_void)ot_omi_otp_deinit();
    if (tee_enable == 0x42) {
        klad_attr.key_sec_cfg.key_sec = KM_KLAD_SEC_ENABLE;
        klad_attr.key_sec_cfg.master_only_enable = OT_TRUE;
        klad_attr.key_sec_cfg.dest_buf_sec_support = OT_TRUE;
        klad_attr.key_sec_cfg.src_buf_sec_support = OT_TRUE;
        klad_attr.key_sec_cfg.src_buf_non_sec_support = OT_FALSE;
        klad_attr.key_sec_cfg.dest_buf_non_sec_support = OT_FALSE;
    } else {
        klad_attr.key_sec_cfg.key_sec = KM_KLAD_SEC_DISABLE;
        klad_attr.key_sec_cfg.master_only_enable = OT_FALSE;
        klad_attr.key_sec_cfg.dest_buf_sec_support = OT_FALSE;
        klad_attr.key_sec_cfg.dest_buf_non_sec_support = OT_TRUE;
        klad_attr.key_sec_cfg.src_buf_sec_support = OT_FALSE;
        klad_attr.key_sec_cfg.src_buf_non_sec_support = OT_TRUE;
    }

    /* 1. klad create handle */
    sample_chk_expr_return(ot_omi_klad_create(&klad_handle), OT_SUCCESS);

    /* 2. klad set attr for clear key */
    sample_chk_expr_goto(ot_omi_klad_set_attr(klad_handle, &klad_attr), OT_SUCCESS, __KLAD_DESTORY__);

    /* 3. attach klad handle & kslot handle */
    sample_chk_expr_goto(ot_omi_klad_attach(klad_handle, KM_KLAD_DEST_TYPE_MCIPHER, keyslot_crypto_handle),
        OT_SUCCESS, __KLAD_DESTORY__);

    /* 4. set clear key */
    sample_chk_expr_goto(ot_omi_klad_set_clear_key(klad_handle, &clear_key), OT_SUCCESS, __KLAD_DETACH__);

__KLAD_DETACH__:
    ot_omi_klad_detach(klad_handle, KM_KLAD_DEST_TYPE_MCIPHER, keyslot_crypto_handle);
__KLAD_DESTORY__:
    ot_omi_klad_destroy(klad_handle);
    return ret;
}

/* phy address crypto data using specific chn */
static ot_s32 sample_one_pack_crypto(symc_data_t *data)
{
    ot_s32 ret = OT_SUCCESS;
    ot_crypto_handle symc_handle = 0;
    crypto_handle keyslot_crypto_handle = 0;
    crypto_buf_attr src_buf = {0};
    crypto_buf_attr dst_buf = {0};
    ot_u32 length = data->data_len;
    ot_void *src_virt_addr = OT_NULL;
    ot_void *dst_virt_addr = OT_NULL;

    sample_chk_expr_goto(cipher_alloc(&src_buf, (ot_void **)&src_virt_addr, length), OT_SUCCESS, CIPHER_FREE);
    sample_chk_expr_goto(cipher_alloc(&dst_buf, (ot_void **)&dst_virt_addr, length), OT_SUCCESS, CIPHER_FREE);

    /* 1. cipher init */
    sample_chk_expr_goto(ot_omi_cipher_symc_init(), OT_SUCCESS, CIPHER_FREE);

    /* 2. km init */
    sample_chk_expr_goto(ot_omi_km_init(), OT_SUCCESS, CIPHER_DEINIT);

    /* 3. cipher create handle */
    sample_chk_expr_goto(ot_omi_cipher_symc_create(&symc_handle, &data->symc_attr), OT_SUCCESS, KM_DEINIT);

    /* 4. create keyslot handle */
    sample_chk_expr_goto(ot_omi_keyslot_create(&keyslot_crypto_handle, KM_KEYSLOT_TYPE_MCIPHER), OT_SUCCESS,
        CIPHER_DESTROY);

    /* 5. attach cipher handle & kslot handle */
    sample_chk_expr_goto(ot_omi_cipher_symc_attach(symc_handle, (ot_crypto_handle)keyslot_crypto_handle), OT_SUCCESS,
        KEYSLOT_DESTROY);

    /* 6. set clear key */
    sample_chk_expr_goto(cipher_set_clear_key(keyslot_crypto_handle, data->key, data->key_len), OT_SUCCESS, KEYSLOT_DESTROY);

    /* 7. encrypt */
    /* 7.1 set config for encrypt */
    sample_chk_expr_goto(ot_omi_cipher_symc_set_config(symc_handle, &data->symc_ctrl), OT_SUCCESS, KEYSLOT_DESTROY);

    /* 7.2. encrypt */
    sample_chk_expr_goto_with_ret(memcpy_s(src_virt_addr, length, data->src_data, length),
        EOK, ret, OT_FAILURE, KEYSLOT_DESTROY);
    (ot_void)memset_s(dst_virt_addr, length, 0, length);
    sample_chk_expr_goto(ot_omi_cipher_symc_encrypt(symc_handle, &src_buf, &dst_buf, length), OT_SUCCESS,
        KEYSLOT_DESTROY);

    /* 8. decrypt */
    /* 8.1 set config for decrypt */
    sample_chk_expr_goto(ot_omi_cipher_symc_set_config(symc_handle, &data->symc_ctrl), OT_SUCCESS, KEYSLOT_DESTROY);

    /* 8.2. decrypt */
    sample_chk_expr_goto_with_ret(memcpy_s(src_virt_addr, length, dst_virt_addr, length), EOK, ret,
        OT_FAILURE, KEYSLOT_DESTROY);
    (ot_void)memset_s(dst_virt_addr, length, 0, length);
    sample_chk_expr_goto(ot_omi_cipher_symc_decrypt(symc_handle, &src_buf, &dst_buf, length), OT_SUCCESS,
        KEYSLOT_DESTROY);

    /* 9. compare */
    sample_chk_expr_goto_with_ret(memcmp(dst_virt_addr, data->src_data, length),
        0, ret, OT_FAILURE, KEYSLOT_DESTROY);

KEYSLOT_DESTROY:
    ot_omi_keyslot_destroy(keyslot_crypto_handle);
CIPHER_DESTROY:
    ot_omi_cipher_symc_destroy(symc_handle);
KM_DEINIT:
    ot_omi_km_deinit();
CIPHER_DEINIT:
    ot_omi_cipher_symc_deinit();
CIPHER_FREE:
    cipher_free(&src_buf, src_virt_addr);
    cipher_free(&dst_buf, dst_virt_addr);
    return ret;
}

static ot_s32 sample_sm4(ot_void)
{
    ot_u32 i;
    ot_s32 ret;
    ot_u32 num = (ot_u32)(sizeof(g_sm4_data) / sizeof(g_sm4_data[0]));
    for (i = 0; i < num; i++) {
        ret = sample_one_pack_crypto(&g_sm4_data[i]);
        if (ret != OT_SUCCESS) {
            sample_err("************ test SM4 one pack %s failed ************\n", g_sm4_data[i].name);
            return ret;
        }
        sample_log("************ test SM4 one pack %s success ************\n", g_sm4_data[i].name);
    }
    return OT_SUCCESS;
}

static ot_s32 data_init(ot_void)
{
    ot_u32 i;
    ot_u32 num = 0;
    /* 1. init g_sm4_data */
    num = (ot_u32)(sizeof(g_sm4_data) / sizeof(g_sm4_data[0]));
    for (i = 0; i < num; i++) {
        sample_chk_expr_return(get_random_data(g_sm4_data[i].key, sizeof(g_sm4_data[i].key)), OT_SUCCESS);
        sample_chk_expr_return(get_random_data(g_sm4_data[i].symc_ctrl.iv, sizeof(g_sm4_data[i].symc_ctrl.iv)),
            OT_SUCCESS);
        sample_chk_expr_return(get_random_data(g_sm4_data[i].src_data, sizeof(g_sm4_data[i].src_data)), OT_SUCCESS);
    }
    return OT_SUCCESS;
}

static ot_void data_deinit(ot_void)
{
    ot_u32 i;
    ot_u32 num = 0;
    /* 1. clear the key in g_sm4_data */
    num = (ot_u32)(sizeof(g_sm4_data) / sizeof(g_sm4_data[0]));
    for (i = 0; i < num; i++) {
        memset_s(g_sm4_data[i].key, sizeof(g_sm4_data[i].key), 0, sizeof(g_sm4_data[i].key));
    }
}

ot_s32 sample_sm4_clearkey(ot_void)
{
    ot_s32 ret;
    sample_chk_expr_return(data_init(), OT_SUCCESS);
    sample_log("************ test SM4 clearkey ************\n");
    ret = sample_sm4();
    if (ret != OT_SUCCESS) {
        return ret;
    }
    sample_log("************ test SM4 clearkey succeed ************\n");
    data_deinit();
    return OT_SUCCESS;
}
