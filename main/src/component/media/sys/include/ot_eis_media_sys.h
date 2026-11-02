#ifndef OT_EIS_MEDIA_SYS_H
#define OT_EIS_MEDIA_SYS_H

#include "ot_eis_media_sys_common.h"
#include "ot_eis_video.h"

#ifdef __cplusplus
extern "C" {
#endif

ot_s32 ot_eis_media_init(ot_void);

ot_s32 ot_eis_media_deinit(ot_void);

ot_s32 ot_eis_media_get_version(ot_eis_sdk_version *sdk_info);

ot_s32 ot_eis_media_get_cur_pts(ot_u64 *cur_pts);

ot_s32 ot_eis_media_init_pts_base(ot_u64 pts_base);

ot_s32 ot_eis_media_sync_pts(ot_u64 pts_base);

//mod_id为butt时候，设置all
ot_s32 ot_eis_media_log_level_set(const ot_eis_media_log_level *log_level);

ot_s32 ot_eis_media_log_level_get(ot_eis_media_log_level *log_level);

ot_s32 ot_eis_media_set_time_zone(ot_s32 time_zone);

ot_s32 ot_eis_media_get_time_zone(ot_s32 *time_zone);

ot_s32 ot_eis_media_set_vi_vproc_mode(const ot_eis_vi_vproc_mode *vi_vproc_mode);

ot_s32 ot_eis_media_get_vi_vproc_mode(ot_eis_vi_vproc_mode *vi_vproc_mode);

ot_s32 ot_eis_media_set_3dnr_pos(ot_eis_3dnr_pos pos);
ot_s32 ot_eis_media_get_3dnr_pos(ot_eis_3dnr_pos *pos);

ot_s32 ot_eis_media_get_img_buf_size(const ot_eis_img_attr *img_attr, ot_eis_buf_size_calc_cfg *calc_cfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
