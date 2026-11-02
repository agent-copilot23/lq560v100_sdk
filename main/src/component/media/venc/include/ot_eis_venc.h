#ifndef __OT_EIS_VENC_H__
#define __OT_EIS_VENC_H__

#include "ot_eis_type.h"
#include "ot_eis_video.h"
#include "ot_eis_common_venc.h"

#ifdef __cplusplus
extern "C" {
#endif


ot_s32 ot_eis_venc_chn_create(ot_eis_venc_chn_handle *chn, const ot_eis_venc_chn_attr *attr);
ot_s32 ot_eis_venc_chn_destroy(ot_eis_venc_chn_handle chn);

ot_s32 ot_eis_venc_chn_start(ot_eis_venc_chn_handle chn);
ot_s32 ot_eis_venc_chn_stop(ot_eis_venc_chn_handle chn);
ot_s32 ot_eis_venc_chn_reset(ot_eis_venc_chn_handle chn);

ot_s32 ot_eis_venc_send_frame(ot_eis_venc_chn_handle chn, const ot_eis_venc_image_frame *frame, ot_s32 milli_sec);

ot_s32 ot_eis_venc_chn_query_status(ot_eis_venc_chn_handle chn, ot_eis_venc_chn_status *status);
ot_s32 ot_eis_venc_chn_acquire_stream(ot_eis_venc_chn_handle chn, ot_eis_venc_stream *stream, ot_s32 milli_sec);
ot_s32 ot_eis_venc_chn_release_stream(ot_eis_venc_chn_handle chn, const ot_eis_venc_stream *stream);

ot_s32 ot_eis_venc_chn_require_idr(ot_eis_venc_chn_handle chn);

ot_s32 ot_eis_venc_set_video_roi_attr(ot_eis_venc_chn_handle chn, const ot_eis_venc_video_roi_attr *attr);
ot_s32 ot_eis_venc_get_video_roi_attr(ot_eis_venc_chn_handle chn, ot_eis_venc_video_roi_attr *attr);
ot_s32 ot_eis_venc_set_jpeg_roi_attr(ot_eis_venc_chn_handle chn, const ot_eis_venc_jpeg_roi_attr *attr);
ot_s32 ot_eis_venc_get_jpeg_roi_attr(ot_eis_venc_chn_handle chn, ot_eis_venc_jpeg_roi_attr *attr);

ot_s32 ot_eis_venc_init();
ot_s32 ot_eis_venc_exit();



#ifdef __cplusplus
}
#endif

#endif		//__OT_EIS_VENC_H__