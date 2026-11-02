#ifndef SAMPLE_AI_H
#define SAMPLE_AI_H

#include <stdio.h>
#include "ot_eis_type.h"
#include "ot_avp_cve.h"
#include "ot_avp_npu_rts.h"
#include "sample_comm.h"

#undef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#undef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))

#define RETURN_NONE
#define CVE_MAX_FILE_PATH 256
#define OT_AVP_CVE_MASK_NUM 25
#define SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM 64
#define SAMPLE_NPU_MAX_BBOX_NUM 100     // 该值为topk，可在yml配置。sample模型配置100
#define SAMPLE_NPU_MAX_NMS_OUT_NUM 16

#define CHECK_RETURN(val, ret)                  \
    do {                                        \
        if (val) {                              \
            return ret;                         \
        }                                       \
    } while(0);

#define CHECK_RETURN_WITH_LOG(val, ret, ...)    \
    do {                                        \
        if (val) {                              \
            printf(__VA_ARGS__);             \
            return ret;                         \
        }                                       \
    } while(0);

#define CHECK_GOTO(val, label)                  \
    do {                                        \
        if (val) {                              \
            goto label;                         \
        }                                       \
    } while(0);

#define CHECK_GOTO_WITH_LOG(val, label, ...)    \
    do {                                        \
        if (val) {                              \
            printf(__VA_ARGS__);             \
            goto label;                         \
        }                                       \
    } while(0);

typedef struct sample_rect {
    ot_float x1; // 左上角橫坐标
    ot_float y1; // 左上角纵坐标
    ot_float x2; // 右上角橫坐标
    ot_float y2; // 右上角纵坐标
    ot_float conf;
    ot_u32 class_id;
} sample_rect;

typedef struct {
    ot_u32 x; // 左上角橫坐标
    ot_u32 y; // 左上角纵坐标
    ot_u32 width;
    ot_u32 height;
    ot_u32 classId; // 类别, [1, 80]
} ot_avp_img_rect;

typedef struct {
    ot_s8 image_path[256];
    ot_avp_npu_dataset* input_dataset;
    ot_avp_npu_dataset* output_dataset;
    ot_avp_npu_shape input_shape;
    ot_avp_npu_shape output_shape;
    ot_u32 input_num;
    ot_u32 output_num;
    ot_void *input_data[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];
    ot_void *output_data[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];

    ot_u32 input_stride[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];
    ot_u32 input_buffsize[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];
    ot_u32 input_data_size[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];
    ot_avp_npu_shape input_dim[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];

    ot_u32 output_stride[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];
    ot_u32 output_buffsize[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];
    ot_u32 output_data_size[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];
    ot_avp_npu_shape output_dim[SAMPLE_NPU_MAX_INPUT_OUTPUT_NUM];

} sample_npu_model_info;

typedef struct {
    ot_eis_handle yolov5_model_hdl;
    ot_eis_handle mobilenetv2_model_hdl;
    sample_npu_model_info yolov5_model_info;
    sample_npu_model_info mobilenetv2_model_info;

    ot_avp_img_rect *rect;
    ot_eis_vproc_rect *vproc_rect;
    ot_eis_osd_attr *osd_attr;
    ot_eis_img_frame osd_fram[3]; // 0: person, 1: car, 2: unknown
    ot_u32 bbox_num;
} sample_npu_attr;

typedef struct {
    ot_eis_handle buf_pool_hdl;
    ot_avp_cve_img* resize_in;
    ot_avp_cve_img* resize_out;
    ot_u32 img_num;
} sample_cve_attr;


ot_s32 sample_ai_init();
ot_void sample_ai_deinit();

ot_s32 sample_ai_process();

ot_s32 sample_ai_create_data();
ot_void sample_ai_destory_data();
ot_void sample_ai_reset_data();

ot_void sample_npu_yolov5_data_trans();
ot_void sample_cve_crop_and_resize(ot_eis_video_frame* in_frame);
ot_void sample_npu_mobilenetv2_execute();

sample_npu_attr* get_sample_npu_attr();
sample_cve_attr* get_sample_cve_attr();

ot_u64 sample_ai_get_time_ms();
#endif