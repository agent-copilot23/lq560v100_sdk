/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef SAMPLE_SYMC_ROOTKEY_H
#define SAMPLE_SYMC_ROOTKEY_H
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
    const ot_s8 *name;
    ot_u8 session_key[MAX_SESSION_KEY];
    ot_u32 session_len;
    ot_u8 content_key[MAX_CONTENT_KEY];
    ot_u32 content_len;
    ot_u32 rootkey_type;
    km_crypto_alg crypto_alg;
    km_klad_alg_type session_alg;
    km_klad_alg_type content_alg;
    crypto_symc_attr symc_attr;
    crypto_symc_ctrl_t symc_ctrl;
} symc_data_root_key_t;

ot_s32 sample_symc_rootkey_decrypt_from_file(const ot_s8 *path, ot_u32 read_offset, ot_s32 read_size, const ot_s8 *save_path,
                                            ot_u8 *session_key, ot_u8 *content_key, ot_u8 *iv, ot_s32 algo_type);
#endif /* !SAMPLE_SYMC_ROOTKEY_H*/
