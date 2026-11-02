#include "ot_avp_npu_rts.h"
#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __GNUC__
#include <unistd.h>
#endif
#ifndef SIMULATOR
#include "ot_smr.h"
#ifndef OT_AVP_NPU_V200
#include "ot_eis_media_sys.h"
#endif
#endif

// simulator lib path
#if defined(_WIN32)
#define LIB_PATH "..\\..\\simulator\\bin"
#elif defined(__linux__)
#define LIB_PATH "../../simulator/lib"
#endif

int main(int argc, char** argv) {
#ifdef SIMULATOR
    ot_avp_npu_config config = {
        .core_ids = 0,
        .dump_layer = true,   // dump layer
        .perf_mode = false,  // perf_mode
        .log_level = 2,   // log level info

    };
#if defined(_WIN32)
    const char* model_path = "..\\resnet50_binary_s.ortm";
    const char* input_path = "..\\ILSVRC2012_val_00024327.bin";
#else
    const char* model_path = "resnet50_binary_s.ortm";
    const char* input_path = "ILSVRC2012_val_00024327.bin";
#endif
#else
    ot_avp_npu_config config = {
        .core_ids = 0,
        .dump_layer = false,   // dump layer
        .perf_mode = false,  // perf_mode
        .log_level = 2,   // log level
    };
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
    const char* model_path = "../data/model/classification/resnet50_binary_b.ortm";
    const char* input_path = "../data/ImageNet/binary/ILSVRC2012_val_00024327.bin";
#endif
    printf("resnet50 sample start\n");

    ot_avp_handle handle = NULL;
    uint32_t ret = ot_avp_npu_init(&config);
    if (ret != 0) {
        printf("npu init error\n");
        return ret;
    }

    // load model
    ret = ot_avp_npu_load_model(model_path, &handle);
    if (ret != 0) {
        printf("open model error\n");
        ot_avp_npu_deinit();
        return ret;
    }

    // get input and output information
    uint32_t input_num = 0;
    uint32_t output_num = 0;
    ot_avp_npu_get_input_num(handle, &input_num);
    ot_avp_npu_get_output_num(handle, &output_num);

    ot_avp_npu_shape input_shape;
    ot_avp_npu_shape output_shape;
    ot_avp_npu_get_input_shape_by_index(handle, 0, &input_shape);
    ot_avp_npu_get_output_shape_by_index(handle, 0, &output_shape);

    ot_avp_data_type input_dtype, output_dtype;
    ot_avp_npu_get_input_dtype_by_index(handle, 0, &input_dtype);
    ot_avp_npu_get_output_dtype_by_index(handle, 0, &output_dtype);

    ot_avp_npu_dataset* input_dataset = ot_avp_npu_create_dataset();
    ot_avp_npu_dataset* output_dataset = ot_avp_npu_create_dataset();

    // create buffer
    uint32_t input_len = ot_avp_npu_get_input_size_by_index(handle, 0);
    void* input_data = NULL;
    ot_avp_npu_malloc(&input_data, input_len);
    memset(input_data, 0, input_len);
    uint32_t input_stride = ot_avp_npu_get_input_default_stride(handle, 0);
    ot_avp_npu_add_buffer(input_dataset, input_data, 0, input_len, input_stride);
    load_data_from_file(input_path, input_data, input_shape, input_stride, input_dtype);

    uint32_t output_len = ot_avp_npu_get_output_size_by_index(handle, 0);

    void* output_data = NULL;
    ot_avp_npu_malloc(&output_data, output_len);
    memset(output_data, 0, output_len);
    uint32_t output_stride = ot_avp_npu_get_output_default_stride(handle, 0);
    ot_avp_npu_add_buffer(output_dataset, output_data, 0, output_len, output_stride);

    // execute model
    ret = ot_avp_npu_execute(handle, input_dataset, output_dataset);
    if (ret != 0) {
        printf("run model error\n");
    }

    dump_data_to_file("output.bin", output_data, output_shape, output_stride, output_dtype);
    printf("sample end\n");
    ot_avp_npu_destroy_dataset(input_dataset);
    ot_avp_npu_destroy_dataset(output_dataset);
    ot_avp_npu_unload_model(handle);
    ot_avp_npu_deinit();
    ot_avp_npu_free(input_data);
    ot_avp_npu_free(output_data);
#ifndef SIMULATOR
    ot_smr_deinit();
#ifndef OT_AVP_NPU_V200
    ot_eis_media_deinit();
#endif
#endif
    return 0;
}
