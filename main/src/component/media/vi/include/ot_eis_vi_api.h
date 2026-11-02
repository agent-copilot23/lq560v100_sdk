#ifndef OT_EIS_VI_API_H
#define OT_EIS_VI_API_H

#include "ot_eis_type.h"
#include "ot_eis_vi.h"

#include "ot_eis_sns_ctrl.h"
#include "ot_eis_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

ot_s32 ot_eis_vi_init(ot_void);
ot_s32 ot_eis_vi_deinit(ot_void);

/* dev interface */
ot_s32 ot_eis_vi_dev_create(ot_vi_dev_handle *dev_hdl, const ot_eis_vi_dev_attr *dev_attr);
ot_s32 ot_eis_vi_dev_destroy(ot_vi_dev_handle dev_hdl);

/* pipe interface */
ot_s32 ot_eis_vi_pipe_create(const ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX], ot_vi_pipe_handle *pipe_hdl, const ot_eis_vi_pipe_attr *pipe_attr);
ot_s32 ot_eis_vi_pipe_destroy(ot_vi_pipe_handle pipe_hdl);

/* chn interface */
ot_s32 ot_eis_vi_chn_create(ot_vi_chn_id chn_id, ot_vi_chn_handle *chn_hdl, const ot_eis_vi_chn_attr *chn_attr);
ot_s32 ot_eis_vi_chn_destroy(ot_vi_chn_handle chn_hdl);

/* bind pipe & dev */
ot_s32 ot_eis_vi_pipe_attach_dev(ot_vi_pipe_handle pipe_hdl, ot_vi_dev_handle dev_hdl);
ot_s32 ot_eis_vi_pipe_detach_dev(ot_vi_pipe_handle pipe_hdl, ot_vi_dev_handle dev_hdl);

/* bind pipe and chn */
ot_s32 ot_eis_vi_pipe_attach_chn(ot_vi_pipe_handle pipe_hdl, ot_vi_chn_handle chn_hdl);
ot_s32 ot_eis_vi_pipe_detach_chn(ot_vi_pipe_handle pipe_hdl, ot_vi_chn_handle chn_hdl);

ot_s32 ot_eis_vi_sensor_register(ot_vi_pipe_handle pipe_hdl, const ot_eis_sns_obj *sns_obj);
ot_s32 ot_eis_vi_sensor_unregister(ot_vi_pipe_handle pipe_hdl);

/* start & stop channel */
ot_s32 ot_eis_vi_pipe_init(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_eis_vi_pipe_deinit(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_eis_vi_pipe_start(ot_vi_pipe_handle pipe_hdl);
ot_s32 ot_eis_vi_pipe_stop(ot_vi_pipe_handle pipe_hdl);

/* low delay */
ot_s32 ot_eis_vi_pipe_set_low_delay_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_low_delay_info *low_delay_info);
ot_s32 ot_eis_vi_pipe_get_low_delay_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_low_delay_info *low_delay_info);
ot_s32 ot_eis_vi_chn_set_low_delay_attr(ot_vi_chn_handle chn_hdl, const ot_eis_low_delay_info *low_delay_info);
ot_s32 ot_eis_vi_chn_get_low_delay_attr(ot_vi_chn_handle chn_hdl, ot_eis_low_delay_info *low_delay_info);

/* dfx:dump pipe frame */
ot_s32 ot_eis_vi_pipe_enable_dump(ot_vi_pipe_handle pipe_hdl, ot_bool enable, ot_u32 depth);
ot_s32 ot_eis_vi_pipe_get_frame(ot_vi_pipe_handle pipe_hdl, ot_eis_video_frame frame_info[], ot_u32 frame_num,
                                ot_s32 milli_sec);
ot_s32 ot_eis_vi_pipe_release_frame(ot_vi_pipe_handle pipe_hdl, const ot_eis_video_frame frame_info[],
                                    ot_u32 frame_num);

/* dfx:dump chn frame */
ot_s32 ot_eis_vi_chn_enable_dump(ot_vi_chn_handle chn_hdl, ot_bool enable, ot_u32 depth);
ot_s32 ot_eis_vi_chn_get_frame(ot_vi_chn_handle chn_hdl, ot_eis_video_frame *frame_info, ot_s32 milli_sec);
ot_s32 ot_eis_vi_chn_release_frame(ot_vi_chn_handle chn_hdl, const ot_eis_video_frame *frame_info);

/* dfx:send frame */
ot_s32 ot_eis_vi_pipe_send_frame(ot_vi_pipe_handle pipe_hdl, const ot_eis_video_frame frame[],
                                 ot_u32 frame_num, ot_s32 mill_sec);

ot_s32 ot_eis_vi_get_chn_attr(ot_vi_chn_handle chn_hdl, ot_eis_vi_chn_attr *chn_attr);
ot_s32 ot_eis_vi_query_pipe_status(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_pipe_status *pipe_status);
ot_s32 ot_eis_vi_query_chn_status(ot_vi_chn_handle chn_hdl, ot_eis_vi_chn_status *chn_status);

ot_s32 ot_eis_vi_get_pipe_id_by_pipe_hdl(ot_vi_pipe_handle pipe_hdl, ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX], ot_s32 *pipe_num);
ot_s32 ot_eis_vi_get_pipe_hdl_by_pipe_id(ot_s32 pipe_id, ot_vi_pipe_handle *pipe_hdl);

ot_s32 ot_eis_vi_get_chn_id_by_chn_hdl(const ot_vi_chn_handle chn_hdl, ot_vi_chn_id *chn_id);
ot_s32 ot_eis_vi_get_chn_hdl_by_ids(const ot_vi_pipe_id pipe_id, const ot_vi_chn_id chn_id, ot_vi_chn_handle *chn_hdl);

ot_s32 ot_eis_vi_pipe_set_quickboot_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_vi_quickboot_attr *quickboot);
ot_s32 ot_eis_vi_pipe_get_quickboot_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_quickboot_attr *quickboot);

/* vi 3dnr */
ot_s32 ot_eis_vi_pipe_set_3dnr_attr(ot_vi_pipe_handle pipe_hdl, const ot_eis_3dnr_attr *attr);
ot_s32 ot_eis_vi_pipe_get_3dnr_attr(ot_vi_pipe_handle pipe_hdl, ot_eis_3dnr_attr *attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_VI_API_H */
