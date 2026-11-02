/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <assert.h>
#include <linux/fb.h>
#include "securec.h"
#include "ot_omi_cipher.h"
#include "ot_omi_km.h"
#include "ot_omi_otp.h"
#include "ot_eis_errno.h"
#include "sample_hash.h"
#include "sample_utils.h"

ot_s32 sample_hash_from_file(const ot_s8 *path, ot_u32 algo_type, ot_u8 *out_hash, ot_u32 hash_len)
{
    ot_s32 ret = OT_FAILURE;
    ot_crypto_handle hash_handle;
    crypto_buf_attr src_buf = {0};
    ot_u32 result_len = 0;
    ot_u8 *buff = OT_NULL;
    ot_u32 buff_size = 256*1024;
    ot_u32 offset = 0;
    ot_s32 readed = 0;
    ot_s32 this_read = 0;
    ot_s32 this_wrote = 0;

    buff = malloc(buff_size);
    if (buff == OT_NULL) {
        sample_err("malloc fail\n");
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    FILE *fb_read = fopen(path, "rb");
    if (fb_read == OT_NULL) {
        sample_err("Couldn't open file '%s'!\n", path);
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    struct stat file_info;
    if (fstat(fileno(fb_read), &file_info) != 0) {
        sample_err("Couldn't get info for file '%s'!\n", path);
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    ot_u64  file_size = file_info.st_size;

    crypto_hash_attr hash_attr = {
        .is_long_term = OT_FALSE
    };

    if (algo_type == SB_ALOG_GM) {
        hash_attr.hash_type = CRYPTO_HASH_TYPE_SM3;
    } else if(algo_type == SB_ALOG_NM) {
        hash_attr.hash_type = CRYPTO_HASH_TYPE_SHA256;
    } else {
        printf("Unknown algorithm\n");
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    /* 1. hash init */
    sample_chk_expr_return(ot_omi_cipher_hash_init(), OT_SUCCESS);

    /* 2. hash create */
    sample_chk_expr_goto(ot_omi_cipher_hash_create(&hash_handle, &hash_attr), OT_SUCCESS, __HASH_DEINIT__);

    do {
        if (fseek(fb_read, offset, SEEK_SET) != 0) {
            ret = OT_FAILURE;
            goto __HASH_DEINIT__;
        }
        ot_s32 size = file_size - readed > buff_size ? buff_size : file_size - readed;
        this_read = fread(buff, 1, size,fb_read);

        this_wrote = 0;
        if (this_read > 0) {

            src_buf.virt_addr = buff;
            /* 3. hash update */
            sample_chk_expr_goto(ot_omi_cipher_hash_update(hash_handle, &src_buf, this_read), OT_SUCCESS,
            __HASH_DEINIT__);

            offset += this_read;
            readed += this_read;
        } else {
            if (readed == 0) {
                sample_err("read image for %s failed\n", path);
                ret = OT_FAILURE;
                goto __HASH_DEINIT__;
            }
        }
    } while(this_read > 0 && readed < file_size);

    /* 4. hash finish */
    sample_chk_expr_goto(ot_omi_cipher_hash_finish(hash_handle, out_hash, hash_len, &result_len), OT_SUCCESS,
        __HASH_DEINIT__);
    ret = OT_SUCCESS;
__HASH_DEINIT__:
    ot_omi_cipher_hash_deinit();
CLEANUP:
    if (buff!= OT_NULL) {
        memset_s(buff, buff_size, 0, buff_size);
        free(buff);
    }
    if (fb_read != OT_NULL) {
        fclose(fb_read);
    }
    return ret;
}