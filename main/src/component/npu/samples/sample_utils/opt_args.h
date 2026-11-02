#ifndef __SAMPLE_UTILS_OPT_ARGS_H
#define __SAMPLE_UTILS_OPT_ARGS_H
#include "ot_avp_npu_common.h"

typedef struct {
    char* model_path;
    char* input_path;
    char* output_path;
    ot_bool perf_mode;
    ot_bool dump_layer;
    ot_bool is_folder;
    ot_u32 repeat;
    ot_u32 core_ids;
} arg_config;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

int parse_optarg(int argc, char** argv, arg_config* opt_arg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif