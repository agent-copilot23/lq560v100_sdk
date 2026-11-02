#include "ot_avp_npu_rts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(__linux__)
#include <unistd.h>
#include <dirent.h>
#define LIB_PATH "/../lib"
#elif defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <direct.h>
#define LIB_PATH "\\..\\bin"
#endif
#include "file_utils.h"
#include "opt_args.h"
#include <time.h>
#include <float.h>

#ifndef SIMULATOR
#include "ot_smr.h"
#ifndef OT_AVP_NPU_V200
#include "ot_eis_media_sys.h"
#endif
#endif


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    void* buf;
    uint32_t buf_len;
} data_buf;


typedef struct {
    char* input_path;
    char* output_path;
    int input_num;
    int output_num;
    ot_avp_npu_dataset* input_dataset;
    ot_avp_npu_dataset* output_dataset;
    arg_config* opt_arg;
    data_buf* input_buf;
    data_buf* output_buf;
} exec_args;

ot_s32 execute_model(ot_avp_handle handle, exec_args* exec_args)
{
    arg_config* opt_arg = exec_args->opt_arg;
    ot_u32 input_num = exec_args->input_num;
    ot_u32 output_num = exec_args->output_num;
    ot_avp_npu_dataset* input_dataset = exec_args->input_dataset;
    ot_avp_npu_dataset* output_dataset = exec_args->output_dataset;
    data_buf* input_data = exec_args->input_buf;
    data_buf* output_data = exec_args->output_buf;
    ot_s32 ret;
    char** input_file = (char**)malloc(input_num*sizeof(char*));
    for (int i = 0; i < input_num; i++) {
        input_file[i] = (char*)malloc(FILE_PATH_MAX);
        memset(input_file[i], 0, FILE_PATH_MAX);
    }
    if (exec_args->input_path != NULL) {
        ret = get_input_file(exec_args->input_path, input_file, input_num);
        if (ret != 0) {
            printf("get input file failed\n");
            goto exec_finish;
        }
    } else if (exec_args->output_path != NULL) {
        printf("can not find input args, please reference ortm_run_model -m xxx.otrm -i xxx -o xxx\n");
        goto exec_finish;
    }
    for (int i = 0; i < exec_args->input_num; i++) {
        if (exec_args->input_path != NULL) {
            ot_avp_data_type dtype;
            ot_avp_npu_get_input_dtype_by_index(handle, i, &dtype);
            ot_avp_npu_shape shape;
            ot_avp_npu_get_input_shape_by_index(handle, i, &shape);
            ot_u32 stride = ot_avp_npu_get_input_default_stride(handle, i);
            if (stride == 0) {
                printf("stride can't be 0\n");
                goto exec_finish;
            }
            printf("load input data: %s\n", input_file[i]);
            ret = load_data_from_file(input_file[i], input_data[i].buf, shape, stride, dtype);
            if (ret != 0) {
                printf("load data from file %s failed\n", input_file[i]);
                goto exec_finish;
            }
        }
    }
    for (int i = 0; i < output_num; i++) {
        memset((ot_u8*)output_data[i].buf, 0, output_data[i].buf_len);
    }
    // execute model
#ifdef SIMULATOR
    if (ot_avp_npu_execute(handle, input_dataset, output_dataset) != 0) {
        printf("run model error\n");
        goto exec_finish;
    }
#else
    struct timespec start, end;
    double max_time = 0, min_time = DBL_MAX, sum_time = 0;
    for (int r = 0; r < opt_arg->repeat; r++) {
        clock_gettime(CLOCK_REALTIME, &start);
        if (ot_avp_npu_execute(handle, input_dataset, output_dataset) != 0) {
            printf("run model error\n");
            goto exec_finish;
        }
        clock_gettime(CLOCK_REALTIME, &end);
        long long exec_time_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        double time_ms = exec_time_ns / 1000000.0;
        sum_time += time_ms;
        max_time = MAX(max_time, time_ms);
        min_time = MIN(min_time, time_ms);
    }
    printf("\n==== oritek execute result ====\n");
    printf("max_time: %.3lf ms, min_time: %.3lf ms, avg_time: %.3f ms, repeat: %d\n\n", max_time, min_time, sum_time / opt_arg->repeat, opt_arg->repeat);
#endif
    // save results
    if (exec_args->output_path) {
        char input_file_name[FILE_NAME_MAX] = {0};
        // get input[0] filename as output_path
        get_file_name(input_file[0], input_file_name);
        for (int i = 0; i < output_num; i++) {
            ot_avp_data_type dtype;
            ot_avp_npu_get_output_dtype_by_index(handle, i, &dtype);
            ot_avp_npu_shape shape;
            ot_avp_npu_get_output_shape_by_index(handle, i, &shape);
            ot_u32 stride = ot_avp_npu_get_output_default_stride(handle, i);
            // dump to file
            char output_file[FILE_PATH_MAX] = {0};
            int ret = 0;
            if (dtype == OT_AVP_DTYPE_F16 || dtype == OT_AVP_DTYPE_UINT10 || dtype == OT_AVP_DTYPE_INT12) {
                ret = snprintf(output_file, FILE_PATH_MAX, "%s%c%s_%d.bin", exec_args->output_path, PATH_SEPARATOR, input_file_name, i);
            } else {
                ret = snprintf(output_file, FILE_PATH_MAX, "%s%c%s_%d.float.bin", exec_args->output_path, PATH_SEPARATOR, input_file_name, i);
            }
            if (ret < 0) {
                printf("dump output fail, output file is too long\n");
                goto exec_finish;
            }
            printf("dump output path: %s\n", output_file);
            if (dump_data_to_file(output_file, (ot_u8*)output_data[i].buf, shape, stride, dtype) != 0) {
                printf("dump output:%s failed\n", output_file);
            }
        }
    }

exec_finish:
    for (int i = 0; i < input_num; i++) {
        if (input_file[i] != NULL) {
            free(input_file[i]);
        }
    }
    free(input_file);
    return 0;
}

ot_s32 exec(ot_avp_handle handle, arg_config* opt_arg)
{
    ot_s32 ret = -1;
    ot_u32 input_num, output_num;
    ot_avp_npu_get_input_num(handle, &input_num);
    ot_avp_npu_get_output_num(handle, &output_num);

    // create input/output dataset
    ot_avp_npu_dataset* input_dataset = ot_avp_npu_create_dataset();
    ot_avp_npu_dataset* output_dataset = ot_avp_npu_create_dataset();
    data_buf* input_data = (data_buf*)malloc(sizeof(data_buf)* input_num);
    data_buf* output_data = (data_buf*)malloc(sizeof(data_buf) * output_num);
    memset(input_data, 0, sizeof(data_buf)* input_num);
    memset(output_data, 0, sizeof(data_buf) * output_num);

    for (int i = 0; i < input_num; i++) {
        ot_avp_data_type dtype;
        ot_avp_npu_get_input_dtype_by_index(handle, i, &dtype);
        ot_avp_npu_shape shape;
        ot_avp_npu_get_input_shape_by_index(handle, i, &shape);
        ot_u32 stride = ot_avp_npu_get_input_default_stride(handle, i);
        if (stride == 0) {
            printf("stride can't be 0\n");
            goto destory_dataset;
        }
        ot_u32 buf_size = ot_avp_npu_get_input_size_by_index(handle, i);
        if (buf_size == 0) {
            printf("buffer size can't be 0\n");
            goto destory_dataset;
        }
        ret = ot_avp_npu_malloc(&input_data[i].buf, buf_size);
        if (ret != 0) {
            printf("malloc failed");
            goto destory_dataset;
        }
        memset(input_data[i].buf, 0, buf_size);
        input_data[i].buf_len = buf_size;
        ot_avp_npu_add_buffer(input_dataset, input_data[i].buf, 0, input_data[i].buf_len, stride);
    }
    // create output buffer
    for (int i = 0; i < output_num; i++) {
        ot_avp_data_type dtype;
        ot_avp_npu_get_output_dtype_by_index(handle, i, &dtype);
        ot_avp_npu_shape shape;
        ot_avp_npu_get_output_shape_by_index(handle, i, &shape);
        ot_u32 buf_size = ot_avp_npu_get_output_size_by_index(handle, i);
        if (buf_size == 0) {
            printf("buffer size can't be 0\n");
            goto destory_dataset;
        }
        ot_u32 stride = ot_avp_npu_get_output_default_stride(handle, i);
        if (stride == 0) {
            printf("stride can't be 0\n");
            goto destory_dataset;
        }
        ret = ot_avp_npu_malloc(&output_data[i].buf, buf_size);
        if (ret != 0) {
            printf("malloc failed\n");
            goto destory_dataset;
        }
        memset(output_data[i].buf, 0, buf_size);
        output_data[i].buf_len = buf_size;
        ot_avp_npu_add_buffer(output_dataset, output_data[i].buf, 0, buf_size, stride);
    }
    if (opt_arg->output_path) {
        mkdir_dir(opt_arg->output_path);
    }
    exec_args exec_cfg;
    exec_cfg.input_dataset = input_dataset;
    exec_cfg.output_dataset = output_dataset;
    exec_cfg.input_num = input_num;
    exec_cfg.output_num = output_num;
    exec_cfg.opt_arg = opt_arg;
    exec_cfg.output_path = opt_arg->output_path;
    exec_cfg.input_buf = input_data;
    exec_cfg.output_buf = output_data;
    if (opt_arg->perf_mode || opt_arg->input_path == NULL) {
        // perf mode
        exec_cfg.input_path = NULL;
        exec_cfg.output_path = NULL;
        ret = execute_model(handle, &exec_cfg);
        if (ret != 0) {
            printf("execute model fail");
            goto destory_dataset;
        }
    } else if (opt_arg->is_folder) {
        // two-level folder
#ifdef __linux__
        struct dirent* entry;
        DIR *dir = opendir(opt_arg->input_path);
        if (dir == NULL) {
            printf("open dir fail, path:%s\n", opt_arg->input_path);
            goto destory_dataset;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            char input_path[FILE_PATH_MAX] = {0};
            ret = snprintf(input_path, FILE_PATH_MAX, "%s/%s", opt_arg->input_path, entry->d_name);
            if (ret < 0) {
                printf("the length of input file path is too long, max length: [%d]\n", FILE_PATH_MAX);
                goto destory_dataset;
            }
            printf("input_path: %s\n", input_path);
            if (!is_directory(input_path)) {
                printf("input path is not directory, path: %s\n", input_path);
                goto destory_dataset;
            }
            exec_cfg.input_path = input_path;
            char output_path[FILE_PATH_MAX] = {0};
            ret = snprintf(output_path, FILE_PATH_MAX, "%s/%s", opt_arg->output_path, entry->d_name);
            if (ret < 0) {
                printf("the length of output file path is too long, max length: [%d]\n", FILE_PATH_MAX);
                goto destory_dataset;
            }
            mkdir_dir(output_path);
            exec_cfg.output_path = output_path;
            ret = execute_model(handle, &exec_cfg);
            if (ret != 0) {
                printf("execute model fail");
                goto destory_dataset;
            }
        }
#else
        char tmp_path[FILE_PATH_MAX] = "";
        gen_win_path(opt_arg->input_path, tmp_path, FILE_PATH_MAX);
        printf("find the floder:%s\n", tmp_path);
        WIN32_FIND_DATA fileInfo;
        HANDLE hFind = FindFirstFile(tmp_path, &fileInfo);
        if (INVALID_HANDLE_VALUE == hFind) {
            printf("failed to find files in this directory:%s\n", tmp_path);
            goto destory_dataset;
        }
        while (FindNextFile(hFind, &fileInfo) != 0) {
            if (strcmp(fileInfo.cFileName, ".") == 0
                || strcmp(fileInfo.cFileName, "..") == 0) {
                continue;
            }
            /*---------------- input path ----------------*/
            char input_path[FILE_PATH_MAX] = {0};
            ret = snprintf(input_path, FILE_PATH_MAX, "%s%c%s", opt_arg->input_path, PATH_SEPARATOR, fileInfo.cFileName);
            if (ret < 0) {
                printf("the length of input file path is too long, max length: [%d]\n", FILE_PATH_MAX);
                goto destory_dataset;
            }
            if (!is_directory(input_path)) {
                printf("input path is not directory, path: %s\n", input_path);
                goto destory_dataset;
            }
            exec_cfg.input_path = input_path;
            /*---------------- output path ----------------*/
            char output_path[FILE_PATH_MAX] = {0};
            ret = snprintf(output_path, FILE_PATH_MAX, "%s\\%s", opt_arg->output_path, fileInfo.cFileName);
            if (ret < 0) {
                printf("the length of output file path is too long, max length: [%d]\n", FILE_PATH_MAX);
                goto destory_dataset;
            }
            mkdir_dir(output_path);
            exec_cfg.output_path = output_path;
            ret = execute_model(handle, &exec_cfg);
            if (ret != 0) {
                printf("execute model fail");
                goto destory_dataset;
            }
        }
        FindClose(hFind);
#endif
    } else {
        // one-level folder
        if (input_num > 1) {
            // multi-input model
            exec_cfg.input_path = opt_arg->input_path;
            ret = execute_model(handle, &exec_cfg);
            if (ret != 0) {
                printf("execute model fail");
                goto destory_dataset;
            }
        } else {
            // single-input model
            struct stat st;
            if (stat(opt_arg->input_path, &st) != 0) {
                printf("input path is not exit, input path:%s\n", opt_arg->input_path);
                goto destory_dataset;
            }
            if (S_ISDIR(st.st_mode)) {
                // multiple image execute
#ifdef __linux__
                struct dirent *entry;
                DIR *dir = opendir(opt_arg->input_path);
                while ((entry = readdir(dir)) != NULL) {
                    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                        continue;
                    }
                    char input_path[FILE_PATH_MAX] = {0};
                    ret = snprintf(input_path, FILE_PATH_MAX, "%s/%s", opt_arg->input_path, entry->d_name);
                    if (ret < 0) {
                        printf("the length of input file path is too long, max length: [%d]\n", FILE_PATH_MAX);
                        goto destory_dataset;
                    }
                    exec_cfg.input_path = input_path;
                    ret = execute_model(handle, &exec_cfg);
                    if (ret != 0) {
                        printf("execute model fail");
                        goto destory_dataset;
                    }
                }
#else
                char tmp_path[FILE_PATH_MAX] = "";
                gen_win_path(opt_arg->input_path, tmp_path, FILE_PATH_MAX);
                WIN32_FIND_DATA fileInfo;
                HANDLE hFind = FindFirstFile(tmp_path, &fileInfo);
                if (INVALID_HANDLE_VALUE == hFind) {
                    printf("failed to find files in this directory:%s\n", tmp_path);
                    goto destory_dataset;
                }
                while (FindNextFile(hFind, &fileInfo) != 0) {
                    if (strcmp(fileInfo.cFileName, ".") == 0
                        || strcmp(fileInfo.cFileName, "..") == 0) {
                        continue;
                    }
                    char input_path[FILE_PATH_MAX] = "";
                    ret = snprintf(input_path, FILE_PATH_MAX, "%s%c%s", opt_arg->input_path, PATH_SEPARATOR, fileInfo.cFileName);
                    if (ret < 0) {
                        printf("the length of input file path is too long, max length: [%d]\n", FILE_PATH_MAX);
                        goto destory_dataset;
                    }
                    exec_cfg.input_path = input_path;
                    printf("exec_cfg input_path:%s\n", exec_cfg.input_path);
                    ret = execute_model(handle, &exec_cfg);
                    if (ret != 0) {
                        printf("execute model fail");
                        goto destory_dataset;
                     }
                }
                FindClose(hFind);
#endif
            } else {
                // single image execute
                exec_cfg.input_path = opt_arg->input_path;
                ret = execute_model(handle, &exec_cfg);
                if (ret != 0) {
                    printf("execute model fail");
                    goto destory_dataset;
                }
            }
        }
    }

destory_dataset:
    ot_avp_npu_destroy_dataset(input_dataset);
    ot_avp_npu_destroy_dataset(output_dataset);
    // free buffer
    for (int i = 0; i < input_num; i++) {
        if (input_data[i].buf != NULL) {
            ot_avp_npu_free(input_data[i].buf);
        }
    }
    for (int i = 0; i < output_num; i++) {
        if (output_data[i].buf != NULL) {
            ot_avp_npu_free(output_data[i].buf);
        }
    }
    free(input_data);
    free(output_data);
    return ret;
}

int main(int argc, char** argv) {
    arg_config opt_arg = {NULL, NULL, NULL, false, false, false, 1, 0};
    if (-1 == parse_optarg(argc, argv, &opt_arg)) {
        fprintf(stderr, "parse arguments failed, exiting.\n");
        return -1;
    }

    extra_config extra_npu_config[2];
    char* layer_path_key   = "DUMP_PATH";
    char* layer_path_value = "LayerDump";
    memcpy(&(extra_npu_config[0].key), layer_path_key, strlen(layer_path_key) + 1);
    memcpy(&(extra_npu_config[0].value), layer_path_value, strlen(layer_path_value) + 1);

#ifdef SIMULATOR
    char* quant_path_key = "QUANT_DUMP_PATH";
    char* quant_path_value = "QuantDump";
    memcpy(&(extra_npu_config[1].key), quant_path_key, strlen(quant_path_key) + 1);
    memcpy(&(extra_npu_config[1].value), quant_path_value, strlen(quant_path_value) + 1);
    opt_arg.repeat = 1;
#else

#ifdef OT_AVP_NPU_V200
    ot_smr_attr smr_attr = {0};
    const char *region_name = (const char *)"anony";

    strcpy((char *)smr_attr.regions[0].region_name, region_name);
    smr_attr.regions[0].addr = (ot_phys_addr)0x80000000;
    smr_attr.regions[0].len = 0x30000000;  // 256M * 3 = 768M
    smr_attr.allocator = OT_SMR_ALLOCATOR_BBAA;
    smr_attr.num = 1;

    if (ot_smr_config(&smr_attr) != 0) {
        printf("SMR already config!\n");
    }
#else
    ot_eis_media_init();
#endif
    ot_smr_init();
#endif

    ot_avp_npu_config config = {
        .core_ids = 0,
        .dump_layer = opt_arg.dump_layer,   // dump layer
        .perf_mode = opt_arg.perf_mode,  // perf_mode
        .log_level = 2,   // log level info
        .extra_config_num = 2,
        .extra_configs = extra_npu_config,
    };

    if (opt_arg.model_path == NULL) {
        printf("get model path failed, please set parameter [-m]\n");
        return -1;
    }
    printf("ortm_run_model start\n");
    ot_avp_handle handle = NULL;
    uint32_t ret = ot_avp_npu_init(&config);
    if (ret != 0) {
        printf("npu init failed\n");
        goto fail;
    }

    // load model
    ret = ot_avp_npu_load_model(opt_arg.model_path, &handle);
    if (ret != 0) {
        printf("open model failed\n");
        goto fail;
    }

    ot_u32 runtime_buf_size = 0;
    ot_avp_npu_get_runtime_buffer_size(handle, &runtime_buf_size);

    void *runtime_buf = NULL;
    if (runtime_buf_size != 0 ) {
        ret = ot_avp_npu_malloc(&runtime_buf, runtime_buf_size);
        if (ret != 0) {
            printf("malloc runtime buf fail\n");
            goto destory_buffer;
        }
        ret = ot_avp_npu_set_runtime_buffer(handle, runtime_buf, runtime_buf_size);
        if (ret != 0) {
            printf("ot_avp_npu_set_runtime_buffer fail\n");
            goto destory_buffer;
        }
    }
    if (opt_arg.core_ids != 0) {
        ret = ot_avp_npu_bind_model_to_core(handle, opt_arg.core_ids);
        if (ret != 0) {
            printf("ot_avp_npu_bind_model_to_core fail\n");
            goto destory_buffer;
        }
    }
    ret = exec(handle, &opt_arg);
    if (ret != 0) {
        printf("model execute fail\n");
    }
    printf("ortm_run_model end\n");
destory_buffer:
    if (runtime_buf != NULL) {
        ot_avp_npu_free(runtime_buf);
    }
fail:
    ot_avp_npu_unload_model(handle);
    ot_avp_npu_deinit();
#ifndef SIMULATOR
    ot_smr_deinit();
#ifndef OT_AVP_NPU_V200
    ot_eis_media_deinit();
#endif
#endif
    return 0;
}
