#include "sample_npu_common.h"



int ot_avp_create_tensor(ot_avp_tensor *tensor, ot_u32 size, ot_avp_npu_shape shape, ot_avp_data_type dtype, ot_u32 stride)
{
    tensor->len = size;
    tensor->shape = shape;
    tensor->dtype = dtype;
    tensor->stride.dims[0] = stride;

    ot_s32 ret = ot_avp_npu_malloc((ot_void **)&tensor->virt_addr, size);
    if (ret != 0) {
        return -1;
    }

    return 0;
}

int ot_avp_destroy_tensor(ot_avp_tensor *tensor)
{
    ot_avp_npu_free((ot_void *)tensor->virt_addr);

    memset(tensor, 0, sizeof(ot_avp_tensor));

    return 0;
}

void prepare_input_output_tensor(void *handle, model_task_info* info)
{
    ot_avp_npu_get_input_num(handle, &info->input_num);
    ot_avp_npu_get_output_num(handle, &info->output_num);
    info->input_dataset = ot_avp_npu_create_dataset();
    info->output_dataset = ot_avp_npu_create_dataset();

    for (ot_s32 i = 0; i < info->output_num; i++) {
        ot_ulong stride = ot_avp_npu_get_output_default_stride(handle, i);
        ot_ulong buffer_size = ot_avp_npu_get_output_size_by_index(handle, i);
        ot_avp_npu_shape shape;
        ot_avp_npu_get_output_shape_by_index(handle, i, &shape);
        ot_avp_data_type data_type;
        ot_avp_npu_get_output_dtype_by_index(handle, i, &data_type);

        ot_avp_create_tensor(&info->output_tensor[i], buffer_size, shape, data_type, stride);
        ot_avp_npu_add_buffer(info->output_dataset, (ot_u8 *)info->output_tensor[i].virt_addr,
            0, buffer_size, stride);
    }

    for (ot_s32 i = 0; i < info->input_num; i++) {
        ot_ulong stride = ot_avp_npu_get_input_default_stride(handle, i);
        ot_ulong buffer_size = ot_avp_npu_get_input_size_by_index(handle, i);
        ot_avp_npu_shape shape;
        ot_avp_npu_get_input_shape_by_index(handle, i, &shape);
        ot_avp_data_type data_type;
        ot_avp_npu_get_input_dtype_by_index(handle, i, &data_type);

        ot_avp_create_tensor(&info->input_tensor[i], buffer_size, shape, data_type, stride);
        ot_avp_npu_add_buffer(info->input_dataset, (ot_u8 *)info->input_tensor[i].virt_addr,
            0, buffer_size, stride);
    }
}

void destory_input_output_tensor(void *handle, model_task_info* info)
{
    ot_avp_npu_destroy_dataset(info->input_dataset);
    ot_avp_npu_destroy_dataset(info->output_dataset);
    for (ot_u32 i = 0; i < info->input_num; i++) {
        ot_avp_destroy_tensor(&info->input_tensor[i]);
    }
    for (ot_u32 i = 0; i < info->output_num; i++) {
        ot_avp_destroy_tensor(&info->output_tensor[i]);
    }
}