/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef SAMPLE_SECURITY_H
#define SAMPLE_SECURITY_H

#include "sample_security.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

#define HASH_LEN                        32
#define SIG_LEN                         64
#define ECC_256_KEY_LEN                 64
#define PROTECT_KEY_LEN                 16
#define IV_LEN                          16

#define UPGRADE_CODE_ENC                0x3C7896E1
#define IMAGE_MAGIC                     0x4AA4D33D
#define SYSTEM_PUBKEY_SIZE              64

/* Code area info, size is 0x200 */

typedef struct {
    ot_u32 magic;          /* 4 */
    ot_u32 struct_version; /* 4 */
    ot_u32 struct_length;  /* 4 */
    ot_u32 signature_length;  /* 4 */
    ot_u32 version_ext;       /* 4 */
    ot_u32 mask_version_ext;  /* 4 */
    ot_u32 code_area_len;   /* 4 */
    ot_u8 code_hash[HASH_LEN];      /* 32 */
    ot_u32 code_enc_flag;  /* 4 */
    ot_u8 protectionkey_l1_enc[PROTECT_KEY_LEN]; /* 16 */
    ot_u8 protectionkey_l2_enc[PROTECT_KEY_LEN]; /*16 */
    ot_u8 iv[IV_LEN];               /* 16 */
    ot_u8 reserved[0x110];
    ot_u8 signature[SIG_LEN];       /* 64 */
    ot_u8 signature_ext[SIG_LEN];   /* 64 */
} upgrade_code_info;

typedef struct {
    upgrade_code_info code_info;
    ot_s8* buffer;
    ot_s32 buffer_size;
    ot_s8 public_key[ECC_256_KEY_LEN];
} upgrade_security_info;

ot_s32 sample_security_verify_image(const ot_s8 *filePath);
ot_s32 sample_security_get_system_image_version(ot_u32 *image_ver);
#endif /* !SAMPLE_SECURITY_H*/
