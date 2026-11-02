#ifndef __OT_AVP_CUSTOM_UTILS_H__
#define __OT_AVP_CUSTOM_UTILS_H__

#include "ot_avp_tensor_common.h"
#include "ot_avp_npu_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* ot_avp_custom_handle;

typedef struct ot_avp_attr ot_avp_attr;

typedef ot_bool (*ot_avp_checksupported_func)(ot_avp_custom_handle);

typedef ot_s32 (*ot_avp_infershape_func)(ot_avp_custom_handle);

typedef ot_s32 (*ot_avp_compute_func)(ot_avp_custom_handle);

//------------get tensor info-----------
ot_s32 ot_avp_custom_get_input_num(ot_avp_custom_handle handle, ot_u32 *tensor_num);

ot_s32 ot_avp_custom_get_input_shape_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_avp_npu_shape *shape);

ot_s32 ot_avp_custom_get_input_dtype_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_avp_data_type *dtype);

ot_s32 ot_avp_custom_get_input_format_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_avp_npu_dformat *format);

ot_u8* ot_avp_custom_get_input_buf_by_index(ot_avp_custom_handle handle, ot_u32 index);

ot_s32 ot_avp_custom_get_input_buf_len_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_u64 *buf_len);

ot_s32 ot_avp_custom_get_output_num(ot_avp_custom_handle handle, ot_u32 *tensor_num);

ot_s32 ot_avp_custom_get_output_shape_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_avp_npu_shape *shape);

ot_s32 ot_avp_custom_get_output_dtype_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_avp_data_type *dtype);

ot_s32 ot_avp_custom_get_output_format_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_avp_npu_dformat *format);

ot_u8* ot_avp_custom_get_output_buf_by_index(ot_avp_custom_handle handle, ot_u32 index);

ot_s32 ot_avp_custom_get_output_buf_len_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_u64 *buf_len);

//-------------set tensor info-------------
ot_s32 ot_avp_custom_set_output_shape_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_avp_npu_shape shape);

ot_s32 ot_avp_custom_set_output_dtype_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_avp_data_type dtype);

ot_s32 ot_avp_custom_set_output_format_by_index(ot_avp_custom_handle handle, ot_u32 index, ot_avp_npu_dformat format);

//---------------get attr info--------------
ot_avp_attr* ot_avp_custom_get_attr_by_name(ot_avp_custom_handle handle, const char *name, ot_u32 name_len);

ot_s32 ot_avp_custom_get_attr_dtype(ot_avp_attr *attr, ot_avp_attr_type *dtype);

ot_s32 ot_avp_custom_get_attr_float(ot_avp_attr *attr, ot_float *value);

ot_s32 ot_avp_custom_get_attr_floats_size(ot_avp_attr *attr, ot_u64 *size);

ot_s32 ot_avp_custom_get_attr_floats_by_index(ot_avp_attr *attr, ot_u64 index, ot_float *value);

ot_s32 ot_avp_custom_get_attr_int(ot_avp_attr *attr, ot_s64 *value);

ot_s32 ot_avp_custom_get_attr_ints_size(ot_avp_attr *attr, ot_u64 *size);

ot_s32 ot_avp_custom_get_attr_ints_by_index(ot_avp_attr *attr, ot_u64 index, ot_s64 *value);

ot_s32 ot_avp_custom_get_attr_string(ot_avp_attr *attr, char *array, ot_u64 array_len);

#ifdef __cplusplus
};
#endif

#endif