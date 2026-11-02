#include "ot_avp_npu_rts.h"
#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ot_smr.h"
#include "sample_npu_common.h"

/*
 * 支持用法1：model_0 trigger + model_1 trigger + model_1 wait
 * 支持用法2：model_0 trigger + model_0 wait + model_1 trigger + model_1 wait
 */
ot_s32 sample_npu_async_inference_both_single_graph(int argc, char** argv)
{
    ot_avp_npu_config config = {
        .core_ids = 0,
        .dump_layer = false,   // dump layer
        .perf_mode = false,  // perf_mode
        .log_level = 2,   // log level
        .thread_num = 2,
    };
    ot_s32 ret = ot_avp_npu_init(&config);
    SAMPLE_NPU_CHECK_RETURN_WITH_LOG(ret != 0, ret, "npu init fail.\n");

    const int model_num = 2;
    model_task_info model_info[model_num];
    const char* model_path[] =
        {"../data/model/classification/resnet50_binary_b.ortm",
         "../data/model/classification/resnet50_binary_b.ortm"};

    // 模型加载, 创建输入输出数据
    for (int i = 0; i < model_num; i++) {
        memset(&model_info[i], 0, sizeof(model_task_info));
        if (ot_avp_npu_load_model(model_path[i], &model_info[i].handle) != 0) {
            printf("load model[%d] fail\n", i);
        }
        prepare_input_output_tensor(model_info[i].handle, &model_info[i]);
    }

    // 支持用法1：model_0 trigger + model_1 trigger + model_1 wait
    for (int i = 0; i < model_num; i++) {
        ret = ot_avp_npu_trigger(model_info[i].handle, model_info[i].input_dataset, model_info[i].output_dataset);
        if (ret != 0) {
            printf("trigger model[%d] fail.\n", i);
        }
    }
    ret = ot_avp_npu_wait(model_info[model_num - 1].handle, -1);
    if (ret != 0) {
        printf("wait model fail.\n");
    }

    // 支持用法2：model_0 trigger + model_0 wait + model_1 trigger + model_1 wait
    for (int i = 0; i < model_num; i++) {
        ret = ot_avp_npu_trigger(model_info[i].handle, model_info[i].input_dataset, model_info[i].output_dataset);
        if (ret != 0) {
            printf("trigger model[%d] fail.\n", i);
        }
        ret = ot_avp_npu_wait(model_info[i].handle, -1);
        if (ret != 0) {
            printf("wait model fail.\n");
        }
    }

    // 销毁输入输出数据, 模型卸载
    for (int i = 0; i < model_num; i++) {
        destory_input_output_tensor(model_info[i].handle, &model_info[i]);
        ot_avp_npu_unload_model(model_info[i].handle);
    }

    printf("%s end.\n", __FUNCTION__);

    ot_avp_npu_deinit();

    return ret;
}

/*
 * 支持用法1：model_0 trigger + model_0 wait + model_1 trigger + model_1 wait
 */
ot_s32 sample_npu_async_inference_mix_single_and_multi_graph(int argc, char** argv)
{
    ot_avp_npu_config config = {
        .core_ids = 0,
        .dump_layer = false,   // dump layer
        .perf_mode = false,  // perf_mode
        .log_level = 2,   // log level
        .thread_num = 2,
    };
    ot_s32 ret = ot_avp_npu_init(&config);
    SAMPLE_NPU_CHECK_RETURN_WITH_LOG(ret != 0, ret, "npu init fail.\n");

    const int model_num = 2;
    model_task_info model_info[model_num];
    const char* model_path[] =
        {"../data/model/classification/resnet50_binary_b.ortm",
         "../data/model/classification/resnet50_binary_b.ortm"};

    // 模型加载, 创建输入输出数据
    for (int i = 0; i < model_num; i++) {
        memset(&model_info[i], 0, sizeof(model_task_info));
        if (ot_avp_npu_load_model(model_path[i], &model_info[i].handle) != 0) {
            printf("load model[%d] fail\n", i);
        }
        prepare_input_output_tensor(model_info[i].handle, &model_info[i]);
    }

    // 支持用法1：model_0 trigger + model_0 wait + model_1 trigger + model_1 wait
    for (int i = 0; i < model_num; i++) {
        ret = ot_avp_npu_trigger(model_info[i].handle, model_info[i].input_dataset, model_info[i].output_dataset);
        if (ret != 0) {
            printf("trigger model[%d] fail.\n", i);
        }
        ret = ot_avp_npu_wait(model_info[i].handle, -1);
        if (ret != 0) {
            printf("wait model fail.\n");
        }
    }

    // 销毁输入输出数据, 模型卸载
    for (int i = 0; i < model_num; i++) {
        destory_input_output_tensor(model_info[i].handle, &model_info[i]);
        ot_avp_npu_unload_model(model_info[i].handle);
    }

    printf("%s end.\n", __FUNCTION__);

    ot_avp_npu_deinit();

    return ret;
}
