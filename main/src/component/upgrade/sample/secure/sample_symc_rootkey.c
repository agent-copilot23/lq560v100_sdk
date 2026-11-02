/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/stat.h>
#include "securec.h"
#include "ot_omi_cipher.h"
#include "ot_omi_km.h"
#include "ot_omi_otp.h"
#include "ot_eis_errno.h"
#include "sample_symc_rootkey.h"
#include "sample_utils.h"


static ot_s32 cipher_set_rootkey(crypto_handle keyslot_crypto_handle, symc_data_root_key_t *data)
{
    ot_s32 ret = OT_FAILURE;
    crypto_handle klad_handle = 0;
    ot_u32 offset = 0x12;
    ot_u8 tee_enable = 0;
    km_klad_attr klad_attr = {
        .klad_cfg = {
            .rootkey_type = data->rootkey_type
        },
        .key_cfg = {
            .engine = data->crypto_alg,
            .decrypt_support = OT_TRUE,
            .encrypt_support = OT_TRUE
        }
    };
    km_klad_session_key klad_session_key = {
        .level = KM_KLAD_LEVEL1,
        .alg = data->session_alg,
        .key_size = data->session_len,
        .key = data->session_key
    };

    km_klad_content_key klad_content_key = {
        .alg = data->content_alg,
        .key_size = data->content_len,
        .key = data->content_key
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

    /* 2. klad set attr for rootkey */
    sample_chk_expr_goto(ot_omi_klad_set_attr(klad_handle, &klad_attr), OT_SUCCESS, __KLAD_DESTORY__);

    /* 3. attach klad handle & kslot handle */
    sample_chk_expr_goto(ot_omi_klad_attach(klad_handle, KM_KLAD_DEST_TYPE_MCIPHER, keyslot_crypto_handle),
        OT_SUCCESS, __KLAD_DESTORY__);

    /* 4. set session key */
    sample_chk_expr_goto(ot_omi_klad_set_session_key(klad_handle, &klad_session_key), OT_SUCCESS, __KLAD_DETACH__);

    /* 5. set content key */
    sample_chk_expr_goto(ot_omi_klad_set_content_key(klad_handle, &klad_content_key), OT_SUCCESS, __KLAD_DETACH__);
    ret = OT_SUCCESS;
__KLAD_DETACH__:
    ot_omi_klad_detach(klad_handle, KM_KLAD_DEST_TYPE_MCIPHER, keyslot_crypto_handle);
__KLAD_DESTORY__:
    ot_omi_klad_destroy(klad_handle);
    return ret;
}

ot_s32 sample_symc_rootkey_decrypt_from_file(const ot_s8 *path, ot_u32 read_offset, ot_s32 read_size, const ot_s8 *save_path,
                                            ot_u8 *session_key, ot_u8 *content_key, ot_u8 *iv, ot_s32 algo_type)
{
    ot_s32 ret = OT_FAILURE;
    ot_crypto_handle symc_handle = 0;
    crypto_handle keyslot_crypto_handle = 0;
    crypto_buf_attr src_buf = {0};
    crypto_buf_attr dst_buf = {0};
    ot_void *src_virt_addr = OT_NULL;
    ot_void *dst_virt_addr = OT_NULL;

    ot_u8 *buff = OT_NULL;
    ot_u32 buff_size = 256*1024;
    ot_u32 offset = read_offset;
    ot_u32 write_offset = 0;
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
        sample_err( "Couldn't get info for file '%s'!\n", path);
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    ot_u64 file_size = file_info.st_size;
    if (file_size < read_offset) {
        sample_err(" read offset is out of range\n");
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    file_size = file_size - read_offset > read_size ? read_size : file_size - read_offset;

    FILE *fb_write = fopen(save_path, "wb");
    if (fb_read == OT_NULL) {
        sample_err("Couldn't open file '%s'!\n", path);
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    symc_data_root_key_t data= {
        .rootkey_type = KM_KLAD_KEY_TYPE_ABRK1,
        .session_len = SESSION_KEY_LEN,
        .content_len = CONTENT_KEY_LEN,

        .symc_attr = {
            .symc_type = CRYPTO_SYMC_TYPE_NORMAL,
            .is_long_term = OT_FALSE,
        },
        .symc_ctrl = {
            .work_mode = CRYPTO_SYMC_WORK_MODE_CBC,
            .symc_key_length = CRYPTO_SYMC_KEY_128BIT,
            .symc_bit_width = CRYPTO_SYMC_BIT_WIDTH_128BIT,
            .iv_length = IV_LEN,
        },
    };
    if (algo_type) {
        data.name = "SM4-CBC-128BITS";
        data.crypto_alg = KM_CRYPTO_ALG_SM4;
        data.session_alg = KM_KLAD_ALG_TYPE_SM4;
        data.content_alg = KM_KLAD_ALG_TYPE_SM4;
        data.symc_ctrl.symc_alg = CRYPTO_SYMC_ALG_SM4;
    } else {
        data.name = "AES-CBC-128BITS";
        data.crypto_alg = KM_CRYPTO_ALG_AES;
        data.session_alg = KM_KLAD_ALG_TYPE_AES;
        data.content_alg = KM_KLAD_ALG_TYPE_AES;
        data.symc_ctrl.symc_alg = CRYPTO_SYMC_ALG_AES;
    }
    sample_log("symc rootkey %s\n", data.name);
    memcpy_s(data.session_key, sizeof(data.session_key), session_key, SESSION_KEY_LEN);
    memcpy_s(data.content_key, sizeof(data.content_key), content_key, CONTENT_KEY_LEN);
    memcpy_s(data.symc_ctrl.iv, sizeof(data.symc_ctrl.iv), iv, IV_LEN);

    sample_chk_expr_goto(cipher_alloc(&src_buf, (ot_void **)&src_virt_addr, buff_size), OT_SUCCESS, CIPHER_FREE);
    sample_chk_expr_goto(cipher_alloc(&dst_buf, (ot_void **)&dst_virt_addr, buff_size), OT_SUCCESS, CIPHER_FREE);

    /* 1. cipher init */
    sample_chk_expr_goto(ot_omi_cipher_symc_init(), OT_SUCCESS, CIPHER_FREE);

    /* 2. km init */
    sample_chk_expr_goto(ot_omi_km_init(), OT_SUCCESS, CIPHER_DEINIT);

    /* 3. cipher create handle */
    sample_chk_expr_goto(ot_omi_cipher_symc_create(&symc_handle, &data.symc_attr), OT_SUCCESS, KM_DEINIT);

    /* 4. create keyslot handle */
    sample_chk_expr_goto(ot_omi_keyslot_create(&keyslot_crypto_handle, KM_KEYSLOT_TYPE_MCIPHER), OT_SUCCESS,
        CIPHER_DESTROY);

    /* 5. attach cipher handle & kslot handle */
    sample_chk_expr_goto(ot_omi_cipher_symc_attach(symc_handle, (ot_crypto_handle)keyslot_crypto_handle), OT_SUCCESS,
        KEYSLOT_DESTROY);

    /* 6. set clear key */
    sample_chk_expr_goto(cipher_set_rootkey(keyslot_crypto_handle, &data), OT_SUCCESS, KEYSLOT_DESTROY);

    /* 7. decrypt */
    /* 7.1 set config for decrypt */
    sample_chk_expr_goto(ot_omi_cipher_symc_set_config(symc_handle, &data.symc_ctrl), OT_SUCCESS, KEYSLOT_DESTROY);

    offset = read_offset;
    do {
        if (fseek(fb_read, offset, SEEK_SET) != 0) {
            ret = OT_FAILURE;
            goto KEYSLOT_DESTROY;
        }
         if (fseek(fb_write, write_offset, SEEK_SET) != 0) {
            ret = OT_FAILURE;
            goto KEYSLOT_DESTROY;
        }
        ot_s32 size = file_size - readed > buff_size ? buff_size : file_size - readed;
        this_read = fread(buff, 1, size,fb_read);
        this_wrote = 0;
        if (this_read > 0) {
            sample_chk_expr_goto_with_ret(memcpy_s(src_virt_addr, buff_size, buff, this_read), EOK, ret,
                OT_FAILURE, KEYSLOT_DESTROY);
            (ot_void)memset_s(buff, buff_size, 0, this_read);
            sample_chk_expr_goto(ot_omi_cipher_symc_decrypt(symc_handle, &src_buf, &dst_buf, this_read), OT_SUCCESS,
                KEYSLOT_DESTROY);
            memcpy_s(buff, buff_size, dst_virt_addr, this_read);

            this_wrote = fwrite(buff, 1, this_read, fb_write);
            if (this_wrote < this_read) {
                sample_log("write %s failed\n", save_path);
                ret = OT_FAILURE;
                goto KEYSLOT_DESTROY;
            }
            offset += this_read;
            write_offset += this_read;
            readed += this_read;
        } else {
            if (readed == 0) {
                sample_log("read image for %s failed\n", save_path);
                ret = OT_FAILURE;
                goto KEYSLOT_DESTROY;
            }
        }
    } while (this_read > 0 && readed < file_size);
    ret = OT_SUCCESS;
KEYSLOT_DESTROY:
    ot_omi_keyslot_destroy(keyslot_crypto_handle);
CIPHER_DESTROY:
    ot_omi_cipher_symc_destroy(symc_handle);
KM_DEINIT:
    ot_omi_km_deinit();
CIPHER_DEINIT:
    ot_omi_cipher_symc_deinit();
CIPHER_FREE:
    cipher_free(&src_buf, src_virt_addr);
    cipher_free(&dst_buf, dst_virt_addr);
CLEANUP:
    if (buff!= OT_NULL) {
        memset_s(buff, buff_size, 0, buff_size);
        free(buff);
    }
    if (fb_read != OT_NULL) {
        fclose(fb_read);
    }
    if (fb_write!= OT_NULL) {
        fflush(fb_write);
        fsync(fileno(fb_write));
        fclose(fb_write);
    }
    return ret;
}
