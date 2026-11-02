/*
  Copyright (c), 2021-2022, Oritek Tech. Co., Ltd.
 */

#ifndef OT_EIS_MEDIA_AO_H
#define OT_EIS_MEDIA_AO_H

#include "ot_eis_type.h"
#include "ot_eis_common.h"
#include "ot_eis_common_ao.h"

#ifdef __cplusplus
extern "C" {
#endif

ot_s32 ot_eis_ao_init(ot_void);

ot_s32 ot_eis_ao_deinit(ot_void);

ot_s32 ot_eis_ao_set_pub_attr(ot_s32 ao_dev, const ot_eis_ao_attr *attr);

ot_s32 ot_eis_ao_get_pub_attr(ot_s32 ao_dev, ot_eis_ao_attr *attr);

ot_s32 ot_eis_ao_enable(ot_s32 ao_dev);
ot_s32 ot_eis_ao_disable(ot_s32 ao_dev);

ot_s32 ot_eis_ao_enable_chn(ot_s32 ao_dev, ot_s32 ao_chn);
ot_s32 ot_eis_ao_disable_chn(ot_s32 ao_dev, ot_s32 ao_chn);

ot_s32 ot_eis_ao_send_frame(ot_s32 ao_dev, ot_s32 ao_chn, const ot_eis_audio_frame *data, ot_s32 milli_sec);

ot_s32 ot_eis_ao_clr_chn_buf(ot_s32 ao_dev, ot_s32 ao_chn);

ot_s32 ot_eis_ao_query_chn_status(ot_s32 ao_dev, ot_s32 ao_chn, ot_eis_ao_chn_state *status);

ot_s32 ot_eis_ao_set_volume(ot_s32 ao_dev, ot_s32 volume_db);
ot_s32 ot_eis_ao_get_volume(ot_s32 ao_dev, ot_s32 *volume_db);

ot_s32 ot_eis_ao_set_mute(ot_s32 ao_dev, ot_bool enable, const ot_eis_audio_fade *fade);
ot_s32 ot_eis_ao_get_mute(ot_s32 ao_dev, ot_bool *enable, ot_eis_audio_fade *fade);

#ifdef __cplusplus
}
#endif

#endif
