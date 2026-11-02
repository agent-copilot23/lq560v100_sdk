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
#include "ot_eis_errno.h"
#include "sample_pke.h"
#include "sample_security.h"
#include "sample_symc_rootkey.h"

/* ecc verify */
static ot_s32 run_ecc_verify_sample(ot_u8 *image, ot_u32 len, ot_u8 *signature, ot_u8 *pb_key)
{
    ot_s32 ret = OT_FAILURE;
    ot_crypto_handle hash_handle;
    ot_bool is_on_curve = OT_FALSE;
    ot_u8 x[ECC_256_KEY_LENGTH] = {0};
    ot_u8 y[ECC_256_KEY_LENGTH] = {0};
    ot_u8 r[ECC_256_KEY_LENGTH] = {0};
    ot_u8 s[ECC_256_KEY_LENGTH] = {0};
    ot_u8 *input_hash_data;
    crypto_hash_attr hash_attr = {0};
    crypto_buf_attr src_buf = {0};
    ot_u32 output_hash_len = 0;

    input_hash_data = malloc(SHA256_HASH_LEN);
    sample_chk_expr_return(input_hash_data != OT_NULL, OT_TRUE);
    memcpy_s(x, sizeof(x), (ot_void *)pb_key, sizeof(x));
    memcpy_s(y, sizeof(y), (ot_void *)(pb_key + sizeof(x)), sizeof(y));
    memcpy_s(r, sizeof(r), signature, sizeof(r));
    memcpy_s(s, sizeof(s), signature + sizeof(r), sizeof(s));

    drv_pke_ecc_point output_pub_key = {
        .x = x,
        .y = y,
    };
    drv_pke_data input_hash = {
        .data = input_hash_data,
        .length =SHA256_HASH_LEN
    };
    drv_pke_ecc_sig output_sig = {
        .r = r,
        .s = s,
    };

    output_pub_key.length = ECC_256_KEY_LENGTH;
    output_sig.length = ECC_256_KEY_LENGTH;

    /* 1. hash init */
    sample_chk_expr_goto(ot_omi_cipher_hash_init(), OT_SUCCESS, __EXIT_FREE__);
    /* 2. hash create */
    hash_attr.is_long_term = OT_FALSE;
    hash_attr.hash_type = CRYPTO_HASH_TYPE_SHA256;
    sample_chk_expr_goto(ot_omi_cipher_hash_create(&hash_handle, &hash_attr), OT_SUCCESS, __HASH_DEINIT__);
    /* 3. hash update */
    src_buf.virt_addr = image;
    sample_chk_expr_goto(ot_omi_cipher_hash_update(hash_handle, &src_buf, len), OT_SUCCESS,
        __HASH_DEINIT__);
    /* 4. hash finish */
    sample_chk_expr_goto(ot_omi_cipher_hash_finish(hash_handle, input_hash_data,ECC_256_KEY_LENGTH,
        &output_hash_len), OT_SUCCESS, __HASH_DEINIT__);
    /* 5. pke init */
    sample_chk_expr_goto(ot_omi_cipher_pke_init(), OT_SUCCESS, __HASH_DEINIT__);

    sample_chk_expr_goto(ot_omi_cipher_pke_check_dot_on_curve(DRV_PKE_ECC_TYPE_RFC5639_P256, &output_pub_key, &is_on_curve),
        OT_SUCCESS, __PKE_DEINIT__);
    /* 9. ECC Verify. */
    sample_chk_expr_goto(ot_omi_cipher_pke_ecdsa_verify(DRV_PKE_ECC_TYPE_RFC5639_P256, &output_pub_key,
        &input_hash, &output_sig), OT_SUCCESS, __PKE_DEINIT__);
    ret = OT_SUCCESS;
__PKE_DEINIT__:
    ot_omi_cipher_pke_deinit();
__HASH_DEINIT__:
    ot_omi_cipher_hash_deinit();
    memset_s(x, ECC_256_KEY_LENGTH, 0, ECC_256_KEY_LENGTH);
    memset_s(y, ECC_256_KEY_LENGTH, 0, ECC_256_KEY_LENGTH);
__EXIT_FREE__:
    free(input_hash_data);
    input_hash_data = OT_NULL;
    return ret;
}

/* SM2 verify */
static ot_s32 run_sm2_verify_sample(ot_u8 *image, ot_u32 len, ot_u8 *signature, ot_u8 *pb_key)
{
    ot_s32 ret = OT_FAILURE;
    ot_bool is_on_curve = OT_FALSE;
    ot_u8 x[ECC_SM2_KEY_LENGTH] = {0};
    ot_u8 y[ECC_SM2_KEY_LENGTH] = {0};
    ot_u8 r[ECC_SM2_KEY_LENGTH] = {0};
    ot_u8 s[ECC_SM2_KEY_LENGTH] = {0};
    ot_u8 sm2_id_buf[MAX_ECC_LENGTH] = {0};
    ot_u8 *input_hash_data;
    ot_u8 id_buf[16] = "1234567812345678";

    input_hash_data = malloc(SM3_HASH_LEN);
    sample_chk_expr_return(input_hash_data != OT_NULL, OT_TRUE);
    memcpy_s(sm2_id_buf,MAX_ECC_LENGTH,id_buf,sizeof(id_buf));

    memcpy_s(x, sizeof(x), (ot_void *)pb_key, sizeof(x));
    memcpy_s(y, sizeof(y), (ot_void *)(pb_key + sizeof(x)), sizeof(y));
    memcpy_s(r, sizeof(r), signature, sizeof(r));
    memcpy_s(s, sizeof(s), signature + sizeof(r), sizeof(s));

    drv_pke_ecc_point output_pub_key = {
        .x = x,
        .y = y,
    };
    drv_pke_msg msg = {
        .data = image,
        .length = len,
        .buf_sec = DRV_PKE_BUF_NONSECURE
    };
    drv_pke_data input_hash = {
        .data = input_hash_data,
        .length = SM3_HASH_LEN
    };
    drv_pke_ecc_sig output_sig = {
        .r = r,
        .s = s,
    };
    drv_pke_data sm2_id = {
        .data = sm2_id_buf,
        .length = sizeof(id_buf)
    };

    output_pub_key.length = ECC_SM2_KEY_LENGTH;
    output_sig.length = ECC_SM2_KEY_LENGTH;

    /* 1. pke init */
    sample_chk_expr_goto(ot_omi_cipher_pke_init(), OT_SUCCESS, __EXIT_FREE__);
    /* 2. Check Dot */
    sample_chk_expr_goto(ot_omi_cipher_pke_check_dot_on_curve(DRV_PKE_ECC_TYPE_SM2, &output_pub_key, &is_on_curve),
        OT_SUCCESS, __PKE_DEINIT__);
    /* 3. DSA Hash */
    sample_chk_expr_goto(ot_omi_cipher_pke_sm2_dsa_hash(&sm2_id, &output_pub_key, &msg, &input_hash),
        OT_SUCCESS, __PKE_DEINIT__);
    /* 4. ECC Verify. */
    sample_chk_expr_goto(ot_omi_cipher_pke_ecdsa_verify(DRV_PKE_ECC_TYPE_SM2, &output_pub_key,
        &input_hash, &output_sig), OT_SUCCESS, __PKE_DEINIT__);
    ret = OT_SUCCESS;
__PKE_DEINIT__:
    ot_omi_cipher_pke_deinit();
    memset_s(x, ECC_SM2_KEY_LENGTH, 0, ECC_SM2_KEY_LENGTH);
    memset_s(y, ECC_SM2_KEY_LENGTH, 0, ECC_SM2_KEY_LENGTH);
__EXIT_FREE__:
    free(input_hash_data);
    input_hash_data = OT_NULL;
    return ret;
}

ot_s32 sample_pke_verify(ot_u8 *image, ot_u32 len, ot_u8 *signature, ot_u8 *pb_key, ot_u32 algo_type)
{
    ot_s32 ret = OT_FAILURE;
    ot_s8 *pke_name = OT_NULL;
    if (algo_type == SB_ALOG_GM) {
        pke_name = "SM2";
        ret = run_sm2_verify_sample(image,len,signature,pb_key);
    }else if (algo_type == SB_ALOG_NM) {
        pke_name = "brainpoolP256r1-sha256";
        ret = run_ecc_verify_sample(image,len,signature,pb_key);
    }else {
        printf("Error: unknown algorithm\n");
        return OT_FAILURE;
    }
    if (ret != OT_SUCCESS) {
        sample_err("%s verify failed\n", pke_name);
        return ret;
    }
    sample_log("%s verify success\n", pke_name);
    return OT_SUCCESS;
}