/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "sample_utils.h"
#define FORMAT_BUFF_SIZE    (128*1024)
ot_s8* sample_upgrade_read_all(const ot_s8 *file_path)
{
    FILE *file = OT_NULL;
    ot_s32 length = 0;
    ot_s8 *content = OT_NULL;
    size_t read_chars = 0;

    /* open in read binary mode */
    file = fopen(file_path, "rb");
    if (file == OT_NULL) {
        debug("fopen %s failed\n", file_path);
        goto cleanup;
    }

    /* get the length */
    if (fseek(file, 0, SEEK_END) != 0) {
        debug("fseek %s end\n", file_path);
        goto cleanup;
    }
    length = ftell(file);
    if (length < 0) {
        debug("ftell %s length failed\n", file_path);
        goto cleanup;
    }
    if (fseek(file, 0, SEEK_SET) != 0) {
        debug("ftell %s orig failed\n", file_path);
        goto cleanup;
    }

    /* allocate content buffer */
    content = (char*)malloc((size_t)length + sizeof(""));
    if (content == OT_NULL) {
        debug("malloc %d failed\n", length);
        goto cleanup;
    }

    /* read the file into memory */
    read_chars = fread(content, sizeof(char), (size_t)length, file);
    if ((ot_s32)read_chars != length) {
        free(content);
        error("readed:%d length:%d\n",
                    read_chars,
                    length);
        content = OT_NULL;
        goto cleanup;
    }
    content[read_chars] = '\0';


cleanup:
    if (file != OT_NULL) {
        fclose(file);
    }

    return content;
}

ot_s32 sample_upgrade_read_file(const ot_s8 *path, ot_u32 file_offset, ot_u8 *buff, ot_u32 buff_len)
{
    ot_s32 ret = OT_FAILURE;
    FILE *file = OT_NULL;
    ot_u32 to_read = 0;
    ot_u32 readed = 0;
    CHECK_RET(!sample_upgrade_is_path_exist(path) || (buff == OT_NULL), ret);

    /* open in read binary mode */
    file = fopen(path, "rb");
    if (file == OT_NULL) {
        goto cleanup;
    }
    if (fseek(file, file_offset, SEEK_SET) != 0) {
        goto cleanup;
    }
    to_read = buff_len;
    do {
        ot_u32 this_read = 0;
        ot_u32 this_to_read = 0;
        this_to_read = to_read-readed;
        this_read = fread(buff + readed, 1, to_read-readed, file);
        readed += this_read;

        if (this_read <= 0) {
            break;
        }
    } while (readed < to_read);
    ret = readed;
cleanup:
    if (file != OT_NULL) {
        fclose(file);
    }
    return ret;
}

ot_s32 sample_upgrade_write_file(const ot_s8 *path, ot_u32 file_offset,  ot_u8 *buff, ot_u32 buff_len)
{
    ot_s32 ret = OT_FAILURE;
    FILE *file = OT_NULL;
    ot_u32 to_write = 0;
    ot_u32 wrote = 0;
    CHECK_RET(!sample_upgrade_is_path_exist(path) || (buff == OT_NULL), ret);
    /* open in read binary mode */
    file = fopen(path, "wb");
    if (file == OT_NULL) {
        goto cleanup;
    }
    if (fseek(file, file_offset, SEEK_SET) != 0) {
        goto cleanup;
    }
    to_write = buff_len;
    do {
        ot_u32 this_write = 0;
        ot_u32 this_to_write = 0;
        this_to_write = to_write-wrote;
        this_write = fwrite(buff + wrote, 1, to_write-wrote, file);
        wrote += this_write;

        if (this_write <= 0) {
            break;
        }
    } while (wrote < to_write);
    ret = wrote;
cleanup:
    if (file != OT_NULL) {
        fflush(file);
        fsync(fileno(file));
        fclose(file);
    }
    return ret;
}

ot_void sample_upgrade_del_file(const char *path)
{
    ot_s32 ret = OT_FAILURE;
    if (sample_upgrade_is_path_exist(path)) {
        ret = unlink(path);
        if (ret != 0) {
            error("unlink %s failed:%s\n", path, strerror(errno));
        }
    }
}

ot_void sample_upgrade_save_bin(const ot_s8 *path, const ot_u8 *content, ot_u32 size)
{
    FILE *file = OT_NULL;
    ot_u32 writed = 0;
	debug("begin\n");
    /* open in read binary mode */
    file = fopen(path, "wb");
    if (file == OT_NULL) {
        error("fopen %s failed:%s \n",path, strerror(errno));
        goto cleanup;
    }

    if (content == OT_NULL || (size == 0)) {
        goto cleanup;
    }

    do {
        writed += fwrite(content + writed, 1, size-writed, file);
        debug("writed:%d\n", writed);
    } while (writed < size);

cleanup:
    if (file != OT_NULL) {
		/*fclose = fflush and close fd*/
        fflush(file);
        fsync(fileno(file));
        fclose(file);
    }
	debug("end\n");
}

ot_bool sample_upgrade_is_path_exist(const ot_s8 *path)
{
    if (0 == access(path, F_OK)) {
        return OT_TRUE;
    }
    return OT_FALSE;
}

ot_bool sample_upgrade_get_bool_setting(ot_s8 *n)
{
    ot_bool ret = OT_FALSE;
    ot_s8 *set = OT_NULL;
    if (n == OT_NULL) {
        return ret;
    }
    set = getenv(n);
    if (set != OT_NULL) {
        ret = OT_TRUE;
    }
    return ret;
}

ot_s32 sample_format_file(const ot_s8 *file_path)
{
    FILE *file = OT_NULL;
    ot_u64 length = 0;
    ot_s8 *content = OT_NULL;
    ot_u64 wrote = 0;
    ot_s32 this_wrote = 0;
    ot_s32 ret = OT_FAILURE;
    /* open in read binary mode */
    file = fopen(file_path, "rwb");
    if (file == OT_NULL) {
        debug("fopen %s failed\n", file_path);
        goto cleanup;
    }

    /* get the length */
    if (fseek(file, 0, SEEK_END) != 0) {
        debug("fseek %s end\n", file_path);
        goto cleanup;
    }
    length = ftell(file);
    if (length < 0) {
        debug("ftell %s length failed\n", file_path);
        goto cleanup;
    }
    if (fseek(file, 0, SEEK_SET) != 0) {
        debug("fseek %s orig failed\n", file_path);
        goto cleanup;
    }

    /* allocate content buffer */
    content = (char *)malloc(FORMAT_BUFF_SIZE);
    if (content == OT_NULL) {
        debug("malloc %d failed\n", FORMAT_BUFF_SIZE);
        goto cleanup;
    }
    memset(content, 0xff, FORMAT_BUFF_SIZE);

    while (wrote < length) {
        this_wrote = sample_upgrade_write_file(file_path, wrote, content,
                                                FORMAT_BUFF_SIZE);
        if (this_wrote > 0) {
            wrote += this_wrote;
        } else {
            break;
        }
    }

    if (content != OT_NULL) {
        free(content);
    }
    if (wrote != length){
        error("format %s failed\n", file_path);
        ret = OT_FAILURE;
    } else {
        ret = OT_SUCCESS;
    }
cleanup:
    if (file != OT_NULL) {
        fclose(file);
    }

    return ret;
}