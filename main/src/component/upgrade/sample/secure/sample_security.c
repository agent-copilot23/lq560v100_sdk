#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/stat.h>
#include "ot_smr.h"
#include "ot_bc.h"
#include "sample_security.h"
#include "sample_hash.h"
#include "sample_symc_rootkey.h"
#include "sample_pke.h"
#include "sample_upgrade.h"

#define  LINE_SIZE 4096

ot_s32 sample_security_get_system_pubkey(ot_u8 *verify_pubkey)
{
    if (verify_pubkey == OT_NULL) {
        printf("verify_pubkey is OT_NULL\n");
        return OT_FAILURE;
    }

    FILE *file = fopen("/proc/cmdline", "r");
    if (file == OT_NULL) {
        printf("fail to get verify_pubkey\n");
        return OT_FAILURE;
    }

    ot_s8 line[LINE_SIZE] = { 0 };
    while (fgets(line, sizeof(line), file)) {
        ot_s8 *ptr = strstr(line, "verify_pubkey=");
        if (ptr != OT_NULL) {
            ptr += strlen("verify_pubkey=0x");
            for (ot_s32 i = 0; i < SYSTEM_PUBKEY_SIZE; i++) {
                if (sscanf(ptr + i * 2, "%2hhx", &verify_pubkey[i]) != 1) {
                    printf("read verify_pubkey fail\n");
                    fclose(file);
                    return OT_FAILURE;
                }
            }
        }
    }
    fclose(file);
    return OT_SUCCESS;
}

ot_s32 sample_security_get_system_image_version(ot_u32 *image_ver)
{
    if (image_ver == OT_NULL) {
        printf("image_ver is OT_NULL\n");
        return OT_FAILURE;
    }

    FILE *file = fopen("/proc/cmdline", "r");
    if (file == OT_NULL) {
        printf("fail to get verify_pubkey\n");
        return OT_FAILURE;
    }

    ot_s8 line[LINE_SIZE] = { 0 };
    while (fgets(line, sizeof(line), file)) {
        ot_s8 *ptr = strstr(line, "image_ver=");
            if (ptr != OT_NULL) {
                sscanf(ptr, "image_ver=0x%x", image_ver);
            }
    }
    fclose(file);
    return OT_SUCCESS;
}

static ot_s32 sample_security_get_code_info(const ot_s8 *path, upgrade_security_info *upgrade_info)
{
    struct stat file_info;
    size_t read_len = 0;
    FILE *fb = fopen(path, "rb");
    if (fb == OT_NULL) {
        printf("Couldn't open file '%s'!\n", path);
        return OT_FAILURE;
    }

    if (fstat(fileno(fb), &file_info) != 0) {
        printf("Couldn't get info for file '%s'!\n", path);
        fclose(fb);
        return OT_FAILURE;
    }
    ot_u64 file_size = (ot_u64)file_info.st_size;

    read_len = fread(&upgrade_info->code_info, 1, sizeof(upgrade_info->code_info), fb);
    if (read_len != sizeof(upgrade_info->code_info)) {
        printf("Error reading from file");
        fclose(fb);
        return OT_FAILURE;
    }
    if (upgrade_info->code_info.magic != IMAGE_MAGIC) {
        printf("image format err, magic: 0x%x, target: 0x %x\n\r", upgrade_info->code_info.magic, IMAGE_MAGIC);
        return OT_FAILURE;
    }
    upgrade_info->buffer_size = file_size - sizeof(upgrade_info->code_info);
    fclose(fb);
    return OT_SUCCESS;
}

static ot_s32 sample_security_save_code_image(const ot_s8 *path, ot_u32 read_offset, ot_s32 read_size, const ot_s8 *save_path)
{
    ot_s32 ret = OT_FAILURE;
    ot_u8 *buff = OT_NULL;
    ot_u32 buff_size = 256*1024;
    ot_u32 offset = read_offset;
    ot_u32 write_offset = 0;
    ot_s32 readed = 0;
    ot_s32 this_read = 0;
    ot_s32 this_wrote = 0;
    buff = malloc(buff_size);
    if (buff == OT_NULL) {
        printf("malloc fail\n");
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    FILE *fb_read = fopen(path, "rb");
    if (fb_read == OT_NULL) {
        printf("Couldn't open file '%s'!\n", path);
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    struct stat file_info;
    if (fstat(fileno(fb_read), &file_info) != 0) {
        printf("Couldn't get info for file '%s'!\n", path);
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    ot_u64 file_size = file_info.st_size;
    if (file_size < read_offset) {
        printf(" read offset is out of range\n");
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    file_size = file_size - read_offset > read_size ? read_size : file_size - read_offset;

    FILE *fb_write = fopen(save_path, "wb");
    if (fb_read == OT_NULL) {
        printf("Couldn't open file '%s'!\n", path);
        ret = OT_FAILURE;
        goto CLEANUP;
    }
    do {
        if (fseek(fb_read, offset, SEEK_SET) != 0) {
            ret = OT_FAILURE;
            goto CLEANUP;
        }
         if (fseek(fb_write, write_offset, SEEK_SET) != 0) {
            ret = OT_FAILURE;
            goto CLEANUP;
        }
        ot_s32 size = file_size - readed > buff_size ? buff_size : file_size - readed;
        this_read = fread(buff, 1, size,fb_read);
        this_wrote = 0;
        if (this_read > 0) {
            this_wrote = fwrite(buff, 1, this_read, fb_write);
            if (this_wrote < this_read) {
                printf("write %s failed\n", save_path);
                ret = OT_FAILURE;
                goto CLEANUP;
            }
            offset += this_read;
            write_offset += this_read;
            readed += this_read;
        } else {
            if (readed == 0) {
                printf("read image for %s failed\n", save_path);
                ret = OT_FAILURE;
                goto CLEANUP;
            }
        }
    } while(this_read > 0 && readed < file_size);
    ret = OT_SUCCESS;
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

static ot_void sample_security_remove_file(const ot_s8 *path)
{
    ot_s32 ret = OT_FAILURE;
    if (access(path,F_OK) == 0) {
        ret = unlink(path);
        if (ret != 0) {
            printf("unlink %s failed:%s\n", path, strerror(errno));
        }
    }
}

ot_s32 sample_security_verify_image(const ot_s8 *filePath)
{
    ot_s32 ret = OT_FAILURE;
    upgrade_security_info upgrade_info = {0};
    ret = ot_smr_init();
    if(ret != OT_SUCCESS) {
        printf("ot_smr_init failed\n");
        goto out;
    }
    ret = sample_security_get_code_info(filePath,&upgrade_info);
    if (ret != OT_SUCCESS) {
        printf("sample_security_get_code_info failed\n");
        goto out;
    }
    ot_s32 offset = sizeof(upgrade_info.code_info);
    ot_s32 image_len = upgrade_info.code_info.code_area_len;
    ot_u32 algo_type = ot_bc_get_secure_algo();

    if(upgrade_info.code_info.code_enc_flag != UPGRADE_CODE_ENC) {
        ret = sample_symc_rootkey_decrypt_from_file(filePath,offset,image_len,UPGRADE_PACKAGE_FILE,
                                                    upgrade_info.code_info.protectionkey_l1_enc,
                                                    upgrade_info.code_info.protectionkey_l2_enc,
                                                    upgrade_info.code_info.iv,algo_type);
        if(ret != OT_SUCCESS) {
            printf("sample_symc_rootkey_decrypt_handle failed\n");
            sample_security_remove_file(UPGRADE_PACKAGE_FILE);
            goto out;
        }
    } else {
        printf("not enc image,choose to skip\n");
        ret = sample_security_save_code_image(filePath,offset,image_len,UPGRADE_PACKAGE_FILE);
        if (ret != OT_SUCCESS) {
            printf("sample_security_save_code_image failed\n");
            sample_security_remove_file(UPGRADE_PACKAGE_FILE);
            goto out;
        }
    }

    if (ot_bc_is_secure_enable() == OT_TRUE) {
        ot_u32 version = 0;
        ret = ot_bc_get_system_version(&version);
        if( ret != OT_SUCCESS) {
            printf("ot_bc_get_system_version failed\n");
            goto out;
        }
        printf("otp version:%u,code version:%u \n",version,upgrade_info.code_info.version_ext);
        if ( version > upgrade_info.code_info.version_ext) {
            printf("Version too low, upgrade prohibited\n");
            ret = OT_FAILURE;
            goto out;
        }

        ret = sample_security_get_system_pubkey(upgrade_info.public_key);
        if( ret != OT_SUCCESS) {
            printf("sample_security_get_system_pubkey failed\n");
            goto out;
        }
        ot_u32 size = sizeof(upgrade_info.code_info) - sizeof(upgrade_info.code_info.signature) - sizeof(upgrade_info.code_info.signature_ext);
        ret = sample_pke_verify((ot_u8 *)&upgrade_info.code_info,size,
                                upgrade_info.code_info.signature,
                                upgrade_info.public_key,algo_type);
        if(ret != OT_SUCCESS) {
            printf("sample_pke_verify failed\n");
            goto out;
        }

        ot_u8 out_hash[MAX_HASH_LEN] = {0};
        ret = sample_hash_from_file(UPGRADE_PACKAGE_FILE,algo_type,out_hash,sizeof(out_hash));
        if (ret != OT_SUCCESS) {
            printf("sample_hash_finish failed\n");
            sample_security_remove_file(UPGRADE_PACKAGE_FILE);
            goto out;
        }
        if (memcmp(out_hash,upgrade_info.code_info.code_hash,sizeof(upgrade_info.code_info.code_hash)) == 0) {
            printf("image hash success \n");
        } else {
            printf("image hash fail \n");
            sample_security_remove_file(UPGRADE_PACKAGE_FILE);
            ret = OT_FAILURE;
            goto out;
        }
    } else {
        printf("verify not enable, choose to skip\n");
        ret = OT_SUCCESS;
        goto out;
    }

out:
    if (upgrade_info.buffer != OT_NULL) {
        printf("free upgrade info buffer \n");
        free(upgrade_info.buffer);
    }
    ot_smr_deinit();
    return ret;
}