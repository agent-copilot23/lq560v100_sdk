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

typedef struct {
    const ot_char *mac_name;
    ot_u8 message[SYMC_MAC_DATA_LEN];
    ot_u32 message_length;
    ot_u8 key[MAX_KEY_LEN];
    ot_u32 key_length;
    crypto_symc_mac_attr mac_attr;
} symc_mac_data_t;

static symc_mac_data_t g_mac_data[] = {
    {
        .mac_name = "AES-128-CBC-MAC", .message_length = SYMC_MAC_DATA_LEN, .key_length = AES128_KEY_LEN,
        .mac_attr = {
            .symc_alg = CRYPTO_SYMC_ALG_AES,
            .symc_key_length = CRYPTO_SYMC_KEY_128BIT,
            .work_mode = CRYPTO_SYMC_WORK_MODE_CBC_MAC,
            .is_long_term = OT_FALSE
        },
    },
    {
        .mac_name = "AES-128-CMAC", .message_length = SYMC_MAC_DATA_LEN, .key_length = AES128_KEY_LEN,
        .mac_attr = {
            .symc_alg = CRYPTO_SYMC_ALG_AES,
            .symc_key_length = CRYPTO_SYMC_KEY_128BIT,
            .work_mode = CRYPTO_SYMC_WORK_MODE_CMAC,
            .is_long_term = OT_TRUE
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
            .engine = KM_CRYPTO_ALG_AES,
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
static ot_s32 sample_symc_mac_run(symc_mac_data_t *data)
{
    ot_s32 ret = OT_SUCCESS;
    ot_crypto_handle symc_handle;
    crypto_handle keyslot_crypto_handle;
    crypto_buf_attr src_buf = {0};
    ot_u8 mac[SYMC_MAC_LEN] = {0};
    ot_u32 mac_length = SYMC_MAC_LEN;

    /* 1. cipher init */
    sample_chk_expr_return(ot_omi_cipher_symc_init(), OT_SUCCESS);

    /* 2. km init */
    sample_chk_expr_goto(ot_omi_km_init(), OT_SUCCESS, CIPHER_DEINIT);

    /* 3. create keyslot handle */
    sample_chk_expr_goto(ot_omi_keyslot_create(&keyslot_crypto_handle, KM_KEYSLOT_TYPE_MCIPHER), OT_SUCCESS,
        KM_DEINIT);

    /* 4. set clear key */
    sample_chk_expr_goto(cipher_set_clear_key(keyslot_crypto_handle, data->key, data->key_length), OT_SUCCESS,
        KEYSLOT_DESTROY);
    data->mac_attr.keyslot_chn = (ot_crypto_handle)keyslot_crypto_handle;

    /* 5. set mac attr */
    sample_chk_expr_goto(ot_omi_cipher_mac_start(&symc_handle, &data->mac_attr), OT_SUCCESS, KEYSLOT_DESTROY);

    /* 6. mac process */
    src_buf.virt_addr = data->message;
    sample_chk_expr_goto(ot_omi_cipher_mac_update(symc_handle, &src_buf, data->message_length), OT_SUCCESS,
        KEYSLOT_DESTROY);

    /* 7. finish mac calculation and get the mac value */
    sample_chk_expr_goto(ot_omi_cipher_mac_finish(symc_handle, mac, &mac_length), OT_SUCCESS, KEYSLOT_DESTROY);

KEYSLOT_DESTROY:
    ot_omi_keyslot_destroy(keyslot_crypto_handle);
KM_DEINIT:
    ot_omi_km_deinit();
CIPHER_DEINIT:
    ot_omi_cipher_symc_deinit();
    return ret;
}

static ot_s32 sample_symc_mac_cal(ot_void)
{
    ot_u32 i;
    ot_s32 ret;
    ot_u32 num = (ot_u32)(sizeof(g_mac_data) / sizeof(g_mac_data[0]));
    for (i = 0; i < num; i++) {
        ret = sample_symc_mac_run(&g_mac_data[i]);
        if (ret != OT_SUCCESS) {
            sample_err("************ test symc mac %s failed ************\n", g_mac_data[i].mac_name);
            return ret;
        }
        sample_log("************ test symc mac %s success ************\n", g_mac_data[i].mac_name);
    }
    return OT_SUCCESS;
}

static ot_s32 data_init(ot_void)
{
    ot_u32 i;
    ot_u32 num = 0;
    /* 1. init g_mac_data */
    num = (ot_u32)(sizeof(g_mac_data) / sizeof(g_mac_data[0]));
    for (i = 0; i < num; i++) {
        sample_chk_expr_return(get_random_data(g_mac_data[i].key, sizeof(g_mac_data[i].key)), OT_SUCCESS);
        sample_chk_expr_return(get_random_data(g_mac_data[i].message, sizeof(g_mac_data[i].message)), OT_SUCCESS);
    }
    return OT_SUCCESS;
}

static ot_void data_deinit(ot_void)
{
    ot_u32 i;
    ot_u32 num = 0;
    /* 1. clear the key in g_mac_data */
    num = (ot_u32)(sizeof(g_mac_data) / sizeof(g_mac_data[0]));
    for (i = 0; i < num; i++) {
        memset_s(g_mac_data[i].key, sizeof(g_mac_data[i].key), 0, sizeof(g_mac_data[i].key));
    }
}

ot_s32 sample_symc_mac(ot_void)
{
    ot_s32 ret;
    sample_chk_expr_return(data_init(), OT_SUCCESS);
    sample_log("************ test symc mac ************\n");
    ret = sample_symc_mac_cal();
    if (ret != OT_SUCCESS) {
        return ret;
    }
    sample_log("************ test symc mac succeed ************\n");
    data_deinit();
    return OT_SUCCESS;
}
