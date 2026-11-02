#include <stdio.h>
#include "abs_ops_impl.h"
#include "ot_avp_custom_utils.h"
#include <math.h>
#include <stdlib.h>

ot_s32 abs_infershape(ot_avp_custom_handle custom_op)
{
    printf("call extern cpu abs infershape\n");
    ot_avp_npu_shape in0_shape;
    ot_avp_data_type in0_dtype;
    ot_avp_npu_dformat in0_format;
    // get inputs info
    ot_avp_custom_get_input_shape_by_index(custom_op, 0, &in0_shape);
    ot_avp_custom_get_input_dtype_by_index(custom_op, 0, &in0_dtype);
    ot_avp_custom_get_input_format_by_index(custom_op, 0, &in0_format);

    // update outputs info
    ot_avp_custom_set_output_shape_by_index(custom_op, 0, in0_shape);
    ot_avp_custom_set_output_dtype_by_index(custom_op, 0, in0_dtype);
    ot_avp_custom_set_output_format_by_index(custom_op, 0, in0_format);
    return 0;
}

ot_bool abs_check_supported_float(ot_avp_custom_handle custom_op)
{
    printf("call extern cpu abs check supported\n");
    ot_avp_data_type in0_dtype;
    ot_s32 ret = ot_avp_custom_get_input_dtype_by_index(custom_op, 0, &in0_dtype);
    if (in0_dtype != OT_AVP_DTYPE_F32 || ret != 0) {
        return false;
    }

    return true;
}

ot_s32 abs_compute_float(ot_avp_custom_handle custom_op)
{
    printf("call extern cpu abs compute\n");

    // get inputs tensor info
    ot_u64 in0_buf_len = 0;
    ot_avp_custom_get_input_buf_len_by_index(custom_op, 0, &in0_buf_len);
    ot_u64 in0_elem_len = in0_buf_len / sizeof(ot_float);
    ot_float* in0_buf = (ot_float*)ot_avp_custom_get_input_buf_by_index(custom_op, 0);
    if (in0_buf == NULL) {
        return -1;
    }

    // get outputs tensor info
    ot_float* out0_buf = (ot_float*)ot_avp_custom_get_output_buf_by_index(custom_op, 0);
    if (out0_buf == NULL) {
        return -1;
    }

    // compute
    for (ot_u32 i = 0; i < in0_elem_len; i++) {
        out0_buf[i] = fabs(in0_buf[i]);
    }
    return 0;
}

ot_bool abs_check_supported_int32(ot_avp_custom_handle custom_op)
{
    printf("call extern cpu abs check supported\n");
    ot_avp_data_type in0_dtype;
    ot_s32 ret = ot_avp_custom_get_input_dtype_by_index(custom_op, 0, &in0_dtype);
    if (in0_dtype != OT_AVP_DTYPE_INT32 || ret != 0) {
        return false;
    }

    return true;
}

ot_s32 abs_compute_int32(ot_avp_custom_handle custom_op)
{
    printf("call extern cpu abs compute\n");

    // get inputs tensor info
    ot_u64 in0_buf_len = 0;
    ot_avp_custom_get_input_buf_len_by_index(custom_op, 0, &in0_buf_len);
    ot_u64 in0_elem_len = in0_buf_len / sizeof(ot_s32);
    ot_s32* in0_buf = (ot_s32*)ot_avp_custom_get_input_buf_by_index(custom_op, 0);
    if (in0_buf == NULL) {
        return -1;
    }

    // get outputs tensor info
    ot_s32* out0_buf = (ot_s32*)ot_avp_custom_get_output_buf_by_index(custom_op, 0);
    if (out0_buf == NULL) {
        return -1;
    }

    // compute
    for (ot_u32 i = 0; i < in0_elem_len; i++) {
        out0_buf[i] = abs(in0_buf[i]);
    }
    return 0;
}