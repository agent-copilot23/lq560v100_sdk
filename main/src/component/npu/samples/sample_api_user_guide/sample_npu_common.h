#ifndef SAMPLE_NPU_COMMON_H
#define SAMPLE_NPU_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ot_avp_npu_rts.h"

#ifndef OT_AVP_NPU_V200
#include "ot_eis_media_sys.h"
#endif

#define SAMPLE_NPU_CHECK_RETURN(val, ret)       \
    do {                                        \
        if (val) {                              \
            return ret;                         \
        }                                       \
    } while(0);

#define SAMPLE_NPU_CHECK_RETURN_WITH_LOG(val, ret, ...)    \
    do {                                        \
        if (val) {                              \
            printf(__VA_ARGS__);                \
            return ret;                         \
        }                                       \
    } while(0);

#define SAMPLE_NPU_CHECK_GOTO(val, label)       \
    do {                                        \
        if (val) {                              \
            goto label;                         \
        }                                       \
    } while(0);

#define SAMPLE_NPU_CHECK_GOTO_WITH_LOG(val, label, ...)    \
    do {                                        \
        if (val) {                              \
            printf(__VA_ARGS__);                \
            goto label;                         \
        }                                       \
    } while(0);

#define OT_TEST_MAX_INPUT_OUTPUT_NUM 64

typedef struct {
    void *handle;
    ot_u32 input_num;
    ot_u32 output_num;
    ot_avp_tensor input_tensor[OT_TEST_MAX_INPUT_OUTPUT_NUM];
    ot_avp_tensor output_tensor[OT_TEST_MAX_INPUT_OUTPUT_NUM];
    ot_avp_npu_dataset* input_dataset;
    ot_avp_npu_dataset* output_dataset;
} model_task_info;

int ot_avp_create_tensor(ot_avp_tensor *tensor, ot_u32 size, ot_avp_npu_shape shape, ot_avp_data_type dtype, ot_u32 stride);
int ot_avp_destroy_tensor(ot_avp_tensor *tensor);

void prepare_input_output_tensor(void *handle, model_task_info* info);
void destory_input_output_tensor(void *handle, model_task_info* info);


#endif