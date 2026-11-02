#ifndef __OT_EIS_VDEC_H__
#define __OT_EIS_VDEC_H__

#include "ot_eis_video.h"
#include "ot_eis_common_vdec.h"


#ifdef __cplusplus
extern "C" {
#endif


ot_s32 ot_eis_vdec_chn_create(ot_eis_vdec_chn_handle *chn, const ot_eis_vdec_chn_attr *attr);
ot_s32 ot_eis_vdec_chn_destroy(ot_eis_vdec_chn_handle chn);

ot_s32 ot_eis_vdec_chn_start(ot_eis_vdec_chn_handle chn);
ot_s32 ot_eis_vdec_chn_stop(ot_eis_vdec_chn_handle chn);

ot_s32 ot_eis_vdec_chn_reset(ot_eis_vdec_chn_handle chn);	

ot_s32 ot_eis_vdec_chn_get_stream_info(ot_eis_vdec_chn_handle chn,  ot_eis_vdec_stream_info *stream_info);

ot_s32 ot_eis_vdec_chn_send_stream(ot_eis_vdec_chn_handle chn, ot_eis_vdec_stream *stream_buf, ot_s32 milli_sec);

ot_s32 ot_eis_vdec_chn_status(ot_eis_vdec_chn_handle chn, ot_eis_vdec_chn_info *chn_info);
ot_s32 ot_eis_vdec_chn_acquire_frame(ot_eis_vdec_chn_handle chn, ot_eis_vdec_pic_frame *frame, ot_s32 milli_sec);
ot_s32 ot_eis_vdec_chn_release_frame(ot_eis_vdec_chn_handle chn, const ot_eis_vdec_pic_frame *frame);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif