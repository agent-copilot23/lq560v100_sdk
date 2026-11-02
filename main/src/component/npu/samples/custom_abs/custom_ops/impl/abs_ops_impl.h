#ifndef __EXTERN_CUSTOM_OPSETS_IMPL_ABS_OPS_IMPL_H
#define __EXTERN_CUSTOM_OPSETS_IMPL_ABS_OPS_IMPL_H

#include "ot_avp_custom_utils.h"
#include "ot_eis_type.h"

#ifdef __cplusplus
extern "C" {
#endif

ot_s32 abs_infershape(ot_avp_custom_handle custom_op);
ot_bool abs_check_supported_float(ot_avp_custom_handle custom_op);
ot_s32 abs_compute_float(ot_avp_custom_handle custom_op);

ot_bool abs_check_supported_int32(ot_avp_custom_handle custom_op);
ot_s32 abs_compute_int32(ot_avp_custom_handle custom_op);

#ifdef __cplusplus
};
#endif

#endif