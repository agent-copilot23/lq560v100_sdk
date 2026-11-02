/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */

#ifndef OT_OMI_CIPHER_H
#define OT_OMI_CIPHER_H

#include "crypto_type.h"
#include "crypto_hash_struct.h"
#include "crypto_kdf_struct.h"
#include "crypto_pke_struct.h"
#include "crypto_symc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* HASH */
ot_s32 ot_omi_cipher_hash_init(ot_void);

ot_s32 ot_omi_cipher_hash_deinit(ot_void);

ot_s32 ot_omi_cipher_hash_create(ot_crypto_handle *omi_hash_handle, const crypto_hash_attr *hash_attr);

ot_s32 ot_omi_cipher_hash_update(ot_crypto_handle omi_hash_handle, const crypto_buf_attr *src_buf, const ot_u32 len);

ot_s32 ot_omi_cipher_hash_finish(ot_crypto_handle omi_hash_handle, ot_u8 *virt_addr, ot_u32 buffer_len, ot_u32 *result_len);

ot_s32 ot_omi_cipher_hash_get(ot_crypto_handle omi_hash_handle, crypto_hash_clone_ctx *hash_clone_ctx);

ot_s32 ot_omi_cipher_hash_set(ot_crypto_handle omi_hash_handle, const crypto_hash_clone_ctx *hash_clone_ctx);

ot_s32 ot_omi_cipher_hash_destroy(ot_crypto_handle omi_hash_handle);

/* PBKDF2 */
ot_s32 ot_omi_cipher_pbkdf2(const crypto_kdf_pbkdf2_param *param, ot_u8 *out, const ot_u32 out_len);

/* PKE */
ot_s32 ot_omi_cipher_pke_init(ot_void);

ot_s32 ot_omi_cipher_pke_deinit(ot_void);

ot_s32 ot_omi_cipher_pke_ecc_gen_key(drv_pke_ecc_curve_type curve_type, const drv_pke_data *input_priv_key,
    const drv_pke_data *output_priv_key, const drv_pke_ecc_point *output_pub_key);

ot_s32 ot_omi_cipher_pke_ecc_gen_ecdh_key(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *input_pub_key,
    const drv_pke_data *input_priv_key, const drv_pke_data *output_shared_key);

ot_s32 ot_omi_cipher_pke_ecdsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);

ot_s32 ot_omi_cipher_pke_ecdsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_data *hash, const drv_pke_ecc_sig *sig);

ot_s32 ot_omi_cipher_pke_eddsa_sign(drv_pke_ecc_curve_type curve_type, const drv_pke_data *priv_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);

ot_s32 ot_omi_cipher_pke_eddsa_verify(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, const drv_pke_ecc_sig *sig);

ot_s32 ot_omi_cipher_pke_check_dot_on_curve(drv_pke_ecc_curve_type curve_type, const drv_pke_ecc_point *pub_key,
    ot_bool *is_on_curve);

ot_s32 ot_omi_cipher_pke_sm2_dsa_hash(const drv_pke_data *sm2_id, const drv_pke_ecc_point *pub_key,
    const drv_pke_msg *msg, drv_pke_data *hash);

ot_s32 ot_omi_cipher_pke_sm2_public_encrypt(const drv_pke_ecc_point *pub_key, const drv_pke_data *plain_text,
    drv_pke_data *cipher_text);

ot_s32 ot_omi_cipher_pke_sm2_private_decrypt(const drv_pke_data *priv_key, const drv_pke_data *cipher_text,
    drv_pke_data *plain_text);

ot_s32 ot_omi_cipher_pke_rsa_sign(const drv_pke_rsa_priv_key *priv_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, const drv_pke_data *input_hash,
    drv_pke_data *sign);

ot_s32 ot_omi_cipher_pke_rsa_verify(const drv_pke_rsa_pub_key *pub_key, drv_pke_rsa_scheme scheme,
    drv_pke_hash_type hash_type, drv_pke_data *input_hash, const drv_pke_data *sig);

ot_s32 ot_omi_cipher_pke_rsa_public_encrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_pub_key *pub_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);

ot_s32 ot_omi_cipher_pke_rsa_private_decrypt(drv_pke_rsa_scheme scheme, drv_pke_hash_type hash_type,
    const drv_pke_rsa_priv_key *priv_key, const drv_pke_data *input, const drv_pke_data *label,
    drv_pke_data *output);

/* SYMC */
ot_s32 ot_omi_cipher_symc_init(ot_void);

ot_s32 ot_omi_cipher_symc_deinit(ot_void);

ot_s32 ot_omi_cipher_symc_create(ot_crypto_handle *symc_handle, const crypto_symc_attr *symc_attr);

ot_s32 ot_omi_cipher_symc_destroy(ot_crypto_handle symc_handle);

ot_s32 ot_omi_cipher_symc_set_config(ot_crypto_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl);

ot_s32 ot_omi_cipher_symc_attach(ot_crypto_handle symc_handle, ot_crypto_handle keyslot_crypto_handle);

ot_s32 ot_omi_cipher_symc_encrypt(ot_crypto_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, ot_u32 length);

ot_s32 ot_omi_cipher_symc_decrypt(ot_crypto_handle symc_handle, const crypto_buf_attr *src_buf,
    const crypto_buf_attr *dst_buf, ot_u32 length);

ot_s32 ot_omi_cipher_symc_encrypt_multi(ot_crypto_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, ot_u32 pack_num);

ot_s32 ot_omi_cipher_symc_decrypt_multi(ot_crypto_handle symc_handle, const crypto_symc_ctrl_t *symc_ctrl,
    const crypto_symc_pack *src_buf_pack, const crypto_symc_pack *dst_buf_pack, ot_u32 pack_num);

ot_s32 ot_omi_cipher_symc_get_tag(ot_crypto_handle symc_handle, ot_u8 *tag, ot_u32 tag_length);

ot_s32 ot_omi_cipher_mac_start(ot_crypto_handle *symc_handle, const crypto_symc_mac_attr *mac_attr);

ot_s32 ot_omi_cipher_mac_update(ot_crypto_handle symc_handle, const crypto_buf_attr *src_buf, ot_u32 length);

ot_s32 ot_omi_cipher_mac_finish(ot_crypto_handle symc_handle, ot_u8 *mac, ot_u32 *mac_length);

/* TRNG */
ot_s32 ot_omi_cipher_trng_get_random(ot_u32 *randnum);

ot_s32 ot_omi_cipher_trng_get_multi_random(ot_u32 size, ot_u8 *randnum);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif