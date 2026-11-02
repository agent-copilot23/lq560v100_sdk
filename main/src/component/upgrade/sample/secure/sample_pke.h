/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef SAMPLE_PKE_H
#define SAMPLE_PKE_H
#include "sample_pke.h"
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

/* PKE data. */
typedef struct {
    const ot_s8 *pke_name;
    drv_pke_ecc_curve_type curve_type;
    ot_u32 key_len;
    ot_u32 hash_len;
    crypto_hash_type hash_type;
} pke_data;

ot_s32 sample_pke_verify(ot_u8 *image, ot_u32 len, ot_u8 *signature, ot_u8 *pb_key, ot_u32 algo_type);
#endif /* !SAMPLE_PKE_H*/
