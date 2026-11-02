/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */

#ifndef OT_OMI_OTP_H
#define OT_OMI_OTP_H

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

ot_s32 ot_omi_otp_init(ot_void);

ot_s32 ot_omi_otp_deinit(ot_void);

ot_s32 ot_omi_otp_read_word(ot_u32 offset, ot_u32 *data);

ot_s32 ot_omi_otp_read_byte(ot_u32 offset, ot_u8 *data);

ot_s32 ot_omi_otp_write_byte(ot_u32 offset, ot_u8 data);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif