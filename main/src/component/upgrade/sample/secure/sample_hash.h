/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef SAMPLE_HASH_H
#define SAMPLE_HASH_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include "securec.h"
#include "ot_omi_cipher.h"
#include "ot_omi_km.h"
#include "ot_omi_otp.h"
#include "ot_eis_errno.h"
#include "sample_utils.h"

typedef struct {
    const ot_s8 *hash_name;
    ot_u8 hash_value[MAX_HASH_LEN];
    ot_u32 hash_len;
    crypto_hash_attr hash_attr;
} hash_data_t;

ot_s32 sample_hash_from_file(const ot_s8 *path, ot_u32 algo_type, ot_u8 *out_hash, ot_u32 hash_len);
#endif /* !SAMPLE_HASH_H*/
