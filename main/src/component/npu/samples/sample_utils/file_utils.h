#ifndef __SAMPLE_UTILS_FILE_UTILS_H
#define __SAMPLE_UTILS_FILE_UTILS_H
#include "ot_avp_npu_rts.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(__linux__)
#define PATH_SEPARATOR '/'
#elif defined(_WIN32) || defined(_WIN64)
#define PATH_SEPARATOR '\\'
#define S_ISDIR(m) (((m) & 0170000) == (0040000))
#endif

#define FILE_PATH_MAX (384)
#define FILE_NAME_MAX (128)


ot_s32 load_data_from_file(const char* file_path, ot_u8* data_buf, ot_avp_npu_shape dims, ot_ulong stride, ot_avp_data_type dtype);

ot_s32 dump_data_to_file(const char* output_file_path, ot_u8* data_buf, ot_avp_npu_shape dims, ot_ulong stride, ot_avp_data_type dtype);

ot_s32 get_input_file(const char* output_path, char** input_file, int input_num);

void mkdir_dir(char* dir_path);

void get_file_name(const char* file_path, char* file_name);

bool is_directory(char* file_path);

void gen_win_path(const char* src, char* dst, int dst_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
