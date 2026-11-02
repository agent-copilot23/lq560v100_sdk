#ifndef __OT_AVP_TENSOR_COMMON_H__
#define __OT_AVP_TENSOR_COMMON_H__

#include "ot_eis_type.h"

#define OT_AVP_MAX_TENSOR_NAME_LEN   (256)
#define OT_AVP_MAX_DIM_SIZE          (16)

typedef enum {
    OT_AVP_DTYPE_F32 = 0,
    OT_AVP_DTYPE_F64,

    OT_AVP_DTYPE_INT8,
    OT_AVP_DTYPE_INT16,
    OT_AVP_DTYPE_INT32,
    OT_AVP_DTYPE_INT64,

    OT_AVP_DTYPE_UINT8,
    OT_AVP_DTYPE_UINT16,
    OT_AVP_DTYPE_UINT32,
    OT_AVP_DTYPE_UINT64,

    OT_AVP_DTYPE_F16,
    OT_AVP_DTYPE_INT4,
    OT_AVP_DTYPE_UINT4,

    OT_AVP_DTYPE_BOOL,
    OT_AVP_DTYPE_INT10,
    OT_AVP_DTYPE_UINT10,
    OT_AVP_DTYPE_INT12,
    OT_AVP_DTYPE_UINT12,

    OT_AVP_DTYPE_BUTT
} ot_avp_data_type;

typedef enum {
    OT_AVP_ATTR_INT = 0,
    OT_AVP_ATTR_INTS,
    OT_AVP_ATTR_FLOAT,
    OT_AVP_ATTR_FLOATS,
    OT_AVP_ATTR_STR,
    OT_AVP_ATTR_BUTT,
} ot_avp_attr_type;

typedef struct ot_avp_shape {
    ot_s32 dims[OT_AVP_MAX_DIM_SIZE];
    ot_u8 dim_size;
} ot_avp_shape;

typedef enum {
    OT_AVP_DF_NCHW,
    OT_AVP_DF_NHWC,
    OT_AVP_DF_ND,
    OT_AVP_DF_NCD,
    OT_AVP_DF_ANY,

    OT_AVP_DF_BUTT
} ot_avp_npu_dformat;

typedef ot_avp_shape ot_avp_npu_shape;
typedef ot_avp_shape ot_avp_stride;

typedef struct{
    ot_u64 virt_addr;
    ot_u64 phys_addr;
    ot_u64 len;

    ot_avp_stride stride;
    ot_avp_data_type dtype;
    ot_avp_shape shape;
    ot_avp_npu_dformat format;

    ot_s8 name[OT_AVP_MAX_TENSOR_NAME_LEN];
} ot_avp_tensor;

typedef enum {
    OT_AVP_TENSOR_TYPE_OFFLINE = 0,
    OT_AVP_TENSOR_TYPE_ONLINE,
} ot_avp_tensor_type;

#endif