/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */

#ifndef OT_OMI_KM_H
#define OT_OMI_KM_H

#include "crypto_km_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

ot_s32 ot_omi_km_init(ot_void);
ot_s32 ot_omi_km_deinit(ot_void);

/* Keyslot. */
ot_s32 ot_omi_keyslot_create(crypto_handle *omi_keyslot_crypto_handle, km_keyslot_type keyslot_type);
ot_s32 ot_omi_keyslot_destroy(crypto_handle omi_keyslot_crypto_handle);

/* Klad. */
ot_s32 ot_omi_klad_create(crypto_handle *omi_klad_handle);
ot_s32 ot_omi_klad_destroy(crypto_handle omi_klad_handle);

ot_s32 ot_omi_klad_attach(crypto_handle omi_klad_handle, km_klad_dest_type klad_type,
    crypto_handle omi_keyslot_crypto_handle);
ot_s32 ot_omi_klad_detach(crypto_handle omi_klad_handle, km_klad_dest_type klad_type,
    crypto_handle omi_keyslot_crypto_handle);

ot_s32 ot_omi_klad_set_attr(crypto_handle omi_klad_handle, const km_klad_attr *attr);
ot_s32 ot_omi_klad_get_attr(crypto_handle omi_klad_handle, km_klad_attr *attr);

ot_s32 ot_omi_klad_set_session_key(crypto_handle omi_klad_handle, const km_klad_session_key *key);
ot_s32 ot_omi_klad_set_content_key(crypto_handle omi_klad_handle, const km_klad_content_key *key);

ot_s32 ot_omi_klad_set_clear_key(crypto_handle omi_klad_handle, const km_klad_clear_key *key);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif