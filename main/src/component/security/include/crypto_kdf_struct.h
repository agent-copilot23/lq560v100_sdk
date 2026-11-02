/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */

#ifndef CRYPTO_KDF_STRUCT_H
#define CRYPTO_KDF_STRUCT_H

#include "crypto_common_struct.h"
#include "crypto_hash_struct.h"

typedef struct {
    crypto_hash_type hash_type;
    ot_u8 *password;
    ot_u32 plen;
    ot_u8 *salt;
    ot_u32 slen;
    ot_u16 count;
} crypto_kdf_pbkdf2_param;

#endif