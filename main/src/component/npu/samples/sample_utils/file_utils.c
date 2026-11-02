#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#if defined(__linux__)
#include <unistd.h>
#include <dirent.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <direct.h>
#include <io.h>
#endif

static ot_u32 get_size_by_dtype(uint32_t elem_size, ot_avp_data_type dtype);

#define WRITE_FILE_DATA(TYPE)                                                                   \
    static ot_u32 write_file_##TYPE##_data(const char* file_path, ot_u8* data_buf, ot_avp_npu_shape dims, ot_u32 stride, ot_avp_data_type dtype)   \
    {                                                                                               \
        FILE* file = fopen(file_path, "w");                                                         \
        if (file == NULL) {                                                                         \
            printf("file: %s open failed\n", file_path);                                            \
            return -1;                                                                              \
        }                                                                                           \
        ot_u32 loop_times = 1;                                                                      \
        for (ot_u32 i = 0; i < dims.dim_size - 1; i++) {                                            \
            loop_times *= dims.dims[i];                                                             \
        }                                                                                           \
        ot_s32 last_dim = dims.dims[dims.dim_size - 1];                                             \
        ot_u32 out_size = get_size_by_dtype(loop_times*last_dim, dtype);                            \
        ot_u8* out_data = (ot_u8*)malloc(out_size);                                                 \
        ot_ulong line_size = get_size_by_dtype(last_dim, dtype);                                    \
        for (ot_s32 loop = 0; loop < loop_times; loop++) {                                          \
            memcpy(out_data + loop * line_size, data_buf + loop*stride, line_size);                 \
        }                                                                                           \
        TYPE* type_buf = (TYPE*)out_data;                                                           \
        ot_u32 elem_size = out_size / get_size_by_dtype(1, dtype);                                  \
        float* float_out = (float*)malloc(elem_size * sizeof(float));                               \
        for (int i = 0; i < elem_size; i++) {                                                       \
            float_out[i] = type_buf[i];                                                             \
        }                                                                                           \
        fwrite(float_out, sizeof(float), elem_size, file);                                          \
        free(out_data);                                                                             \
        fclose(file);                                                                               \
        free(float_out);                                                                            \
        return 0;                                                                                   \
    }                                                                                               \

WRITE_FILE_DATA(uint8_t)
WRITE_FILE_DATA(int8_t)
WRITE_FILE_DATA(uint16_t)
WRITE_FILE_DATA(int16_t)
WRITE_FILE_DATA(uint32_t)
WRITE_FILE_DATA(int32_t)
WRITE_FILE_DATA(float)

static ot_u32 write_file_data_buf(const char* file_path, ot_u8* data_buf, ot_avp_npu_shape dims, ot_u32 stride, ot_avp_data_type dtype)
{
    FILE* file = fopen(file_path, "w");
    if (file == NULL) {
        printf("file: %s open failed\n", file_path);
        return -1;
    }
    ot_u32 loop_times = 1;
    for (ot_u32 i = 0; i < dims.dim_size - 1; i++) {
        loop_times *= dims.dims[i];
    }
    ot_s32 last_dim = dims.dims[dims.dim_size - 1];
    ot_u32 out_size = get_size_by_dtype(loop_times*last_dim, dtype);
    ot_u8* out_data = (ot_u8*)malloc(out_size);
    ot_ulong line_size = get_size_by_dtype(last_dim, dtype);
    for (ot_s32 loop = 0; loop < loop_times; loop++) {
        memcpy(out_data + loop * line_size, data_buf + loop*stride, line_size);
    }
    fwrite(out_data, sizeof(uint8_t), out_size, file);
    free(out_data);
    fclose(file);
    return 0;
}


ot_u32 get_dtype_bits(ot_avp_data_type dtype)
{
    switch (dtype) {
        case OT_AVP_DTYPE_INT8:
            return 8;
        case OT_AVP_DTYPE_INT16:
            return 16;
        case OT_AVP_DTYPE_INT32:
            return 32;
        case OT_AVP_DTYPE_INT64:
            return 64;
        case OT_AVP_DTYPE_UINT8:
            return 8;
        case OT_AVP_DTYPE_UINT16:
            return 16;
        case OT_AVP_DTYPE_UINT32:
            return 32;
        case OT_AVP_DTYPE_UINT64:
            return 64;
        case OT_AVP_DTYPE_F32:
            return 32;
        case OT_AVP_DTYPE_F64:
            return 64;
        case OT_AVP_DTYPE_BOOL:
            return 8;
        case OT_AVP_DTYPE_INT10:
            return 10;
        case OT_AVP_DTYPE_UINT10:
            return 10;
        case OT_AVP_DTYPE_INT12:
            return 12;
        case OT_AVP_DTYPE_UINT12:
            return 12;
        case OT_AVP_DTYPE_F16:
            return 16;
        default:
            return 0;
    }
}

static ot_u32 get_size_by_dtype(uint32_t elem_size, ot_avp_data_type dtype)
{
    ot_u32 data_bits = get_dtype_bits(dtype);
    return ceil(1.0*elem_size*data_bits / 8);
}


static const char* filename_suffix(const char* file_path, char split)
{
    int len = strlen(file_path);
    for (int i = len-1; i >= 0; i--) {
        if (file_path[i] == split) {
            return file_path + i + 1;
        }
    }
    return NULL;
}

static ot_u32 get_bin_size(const char* path)
{
    ot_u32 size = 0;
    FILE *fp = fopen(path, "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
    }
    return size;
}

static ot_u32 read_file_data_with_stride(const char* file_path, ot_u8* buf, ot_avp_npu_shape dims, ot_u32 stride, ot_avp_data_type dtype)
{
    ot_u32 size = get_bin_size(file_path);
    FILE *infile;
    if ((infile = fopen(file_path, "rb")) == NULL) {
        printf("can't open path: %s", file_path);
        return -1;
    }
    fseek(infile, 0, SEEK_SET);
    ot_u8* data = (ot_u8*)malloc(size);
    ot_ulong read_size = fread(data, 1, size, infile);
    if (read_size != size) {
        free(data);
        fclose(infile);
        printf("read size error");
        return -1;
    }
    fclose(infile);

    ot_ulong loop_times = 1;
    for (ot_s32 loop = 0; loop < dims.dim_size - 1; loop++) {
        loop_times *= dims.dims[loop];
    }

    ot_ulong dim_val = dims.dims[dims.dim_size - 1];
    ot_ulong line_size = get_size_by_dtype(dim_val, dtype);
    for (ot_s32 loop = 0; loop < loop_times; loop++) {
        if (loop * line_size + line_size > size) {
            printf("file data is not enough to fill buff.\n");
            free(data);
            return -1;
        }
        memcpy(buf + loop * stride, data + loop * line_size, line_size);
    }
    free(data);
    return 0;
}

ot_s32 load_data_from_file(const char* file_path, ot_u8* data_buf, ot_avp_npu_shape dims, ot_ulong stride, ot_avp_data_type dtype)
{
    const char* suffix = filename_suffix(file_path, '.');
    if (suffix == NULL) {
        printf("invalid file suffix\n");
        return -1;
    }
    ot_s32 ret = 0;
    ot_u32 data_bits = get_dtype_bits(dtype);
    if (data_bits <= 0) {
        printf("invalid dtype: %d", dtype);
        return -1;
    }
    if (strcmp(suffix, "txt") == 0) {
        printf("do not support xxx.txt input\n");
        return -1;
    } else {
        ret = read_file_data_with_stride(file_path, data_buf, dims, stride, dtype);
    }
    return ret;
}

ot_s32 dump_data_to_file(const char* output_file_path, ot_u8* data_buf, ot_avp_npu_shape dims, ot_ulong stride, ot_avp_data_type dtype)
{
    ot_u32 data_bits = get_dtype_bits(dtype);
    if (data_bits <= 0) {
        printf("invalid dtype: %d", dtype);
        return -1;
    }
    switch (dtype) {
        case OT_AVP_DTYPE_F32:
            return write_file_float_data(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_UINT8:
            return write_file_uint8_t_data(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_INT8:
            return write_file_int8_t_data(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_UINT16:
            return write_file_uint16_t_data(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_INT16:
            return write_file_int16_t_data(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_UINT32:
            return write_file_uint32_t_data(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_INT32:
            return write_file_int32_t_data(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_BOOL:
            return write_file_uint8_t_data(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_UINT10:
            return write_file_data_buf(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_UINT12:
            return write_file_data_buf(output_file_path, data_buf, dims, stride, dtype);
        case OT_AVP_DTYPE_F16:
            return write_file_data_buf(output_file_path, data_buf, dims, stride, dtype);
        default:
            printf("unsupport dtype %d\n", dtype);
            return -1;
    }
    return 0;
}


ot_s32 get_input_file(const char* input_path, char** input_file, int input_num)
{
    struct stat st;
    if (stat(input_path, &st) != 0) {
        printf("input path is not exit, input path:%s\n", input_path);
        return -1;
    }

    if (S_ISDIR(st.st_mode)) {
        // directory
        int file_num = 0;
#ifdef __linux__
        DIR *dir = opendir(input_path);
        if (dir == NULL) {
            printf("open directory failed\n");
            return -1;
        }
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 ||
                    strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            char file_name[FILE_NAME_MAX] = {0};
            get_file_name(entry->d_name, file_name);
            int input_index = atoi(file_name);
            if (input_index >= input_num) {
                printf("file name [%d] should < input_num[%d]\n", input_index, input_num);
                return -1;
            }
            int ret = snprintf(input_file[input_index], FILE_PATH_MAX, "%s/%s", input_path, entry->d_name);
            if (ret < 0) {
                printf("the length of input file path is too long, max length: [%d]\n", FILE_PATH_MAX);
                return -1;
            }
            file_num++;
        }
        closedir(dir);
#else
        char tmp_path[FILE_PATH_MAX] = "";
        gen_win_path(input_path, tmp_path, FILE_PATH_MAX);
        WIN32_FIND_DATA fileInfo;
        HANDLE hFind = FindFirstFile(tmp_path, &fileInfo);
        if (INVALID_HANDLE_VALUE == hFind) {
            printf("failed to find files in this directory:%s\n", tmp_path);
            return -1;
        }
        while (FindNextFile(hFind, &fileInfo) != 0) {
            if (strcmp(fileInfo.cFileName, ".") == 0 ||
                strcmp(fileInfo.cFileName, "..") == 0) {
                continue;
            }
            char file_name[FILE_NAME_MAX] = {0};
            get_file_name(fileInfo.cFileName, file_name);
            int input_index = atoi(file_name);
            if (input_index >= input_num) {
                printf("file name [%d] should < input_num[%d]\n", input_index, input_num);
                return -1;
            }
            int ret = snprintf(input_file[input_index], FILE_PATH_MAX, "%s%c%s", input_path, PATH_SEPARATOR, fileInfo.cFileName);
            if (ret < 0) {
                printf("the length of input file path is too long, max length: [%d]\n", FILE_PATH_MAX);
                return -1;
            }
            file_num++;
        }
        FindClose(hFind);
#endif
        if (file_num != input_num) {
            printf("input file num [%d] is not match model input num [%d]\n", file_num, input_num);
            return -1;
        }
    } else {
        // input file
        if (input_num != 1) {
            printf("multi-input model only support directory as -i parameter\n");
            return -1;
        }
        strncpy(input_file[0], input_path, FILE_PATH_MAX);
        if (stat(input_file[0], &st) != 0) {
            printf("file %s is not exit\n", input_file[0]);
            return -1;
        }
        return 0;
    }
    return 0;
}

void mkdir_dir(char* dir_path)
{
    char command[FILE_PATH_MAX] = {0};
#ifdef _MSC_VER
    snprintf(command, FILE_PATH_MAX, "rd /s /q %s", dir_path);
    if (0 == _access(dir_path, 0)) {
        system(command);
        if (_mkdir(dir_path) != 0) {
            printf("create directory:%s failed\n", dir_path);
            return;
        }
    } else {
        if (_mkdir(dir_path) != 0) {
            printf("create directory:%s failed\n", dir_path);
            return;
        }
    }
#else
    int ret = snprintf(command, FILE_PATH_MAX, "rm -rf %s", dir_path);
    if (ret < 0) {
        printf("mkdir directory fail, dir path is too long\n");
        return;
    }
    system(command);
    if (mkdir(dir_path, 0755) != 0) {
        printf("create directory:%s failed", dir_path);
    }
#endif
}

void get_file_name(const char* file_path, char* file_name)
{
    // dir/dir/file_name.bin  -> file_name
    char* pos = strrchr(file_path, PATH_SEPARATOR);
    if (pos == NULL) {
        strncpy(file_name, file_path, FILE_NAME_MAX);
    } else {
        strncpy(file_name, pos+1, FILE_NAME_MAX);
    }
    char* suffix_pos = strrchr(file_name, '.');
    if (suffix_pos != NULL) {
        *suffix_pos = '\0';
    }
}

bool is_directory(char* file_path)
{
    struct stat st;
    if (stat(file_path, &st) != 0) {
        printf("input path is not exit, input path:%s\n", file_path);
        return -1;
    }
    if (S_ISDIR(st.st_mode)) {
        return true;
    }
    return false;
}

void gen_win_path(const char* src, char* dst, int dst_len)
{
    strncpy(dst, src, dst_len);
    int len = strlen(dst);
    dst[len] = '\\';
    dst[len+1] = '*';
    return;
}
