/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef OT_OGE_H
#define OT_OGE_H


#include "ot_oge_common.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* API Declaration */
ot_s32 ot_oge_open(ot_void);
ot_void ot_oge_close(ot_void);
ot_s32 ot_oge_begin_job(ot_void);
ot_s32 ot_oge_end_job(ot_s32 handle, ot_bool is_sync, ot_bool is_block, ot_u32 time_out);
ot_s32 ot_oge_cancel_job(ot_s32 handle);
ot_s32 ot_oge_wait_the_task_done(ot_s32 handle);
ot_s32 ot_oge_wait_all_task_done(ot_void);
ot_s32 ot_oge_quick_fill(ot_s32 handle, const ot_oge_none_src *none_src, ot_u32 fill_data);
ot_s32 ot_oge_draw_corner_box(ot_s32 handle, const ot_oge_surface *dst_surface, const ot_oge_corner_rect *corner_rect,
                              ot_u32 num);
ot_s32 ot_oge_draw_line(ot_s32 handle, const ot_oge_surface *dst_surface, const ot_oge_line *line, ot_u32 num);
ot_s32 ot_oge_quick_copy(ot_s32 handle, const ot_oge_single_src *single_src);
ot_s32 ot_oge_quick_resize(ot_s32 handle, const ot_oge_single_src *single_src);
ot_s32 ot_oge_solid_draw(ot_s32 handle, const ot_oge_single_src *single_src, const ot_oge_fill_color *fill_color,
                         const ot_oge_opt *opt);
ot_s32 ot_oge_rotate(ot_s32 handle, const ot_oge_single_src *single_src, ot_oge_rotate_angle rotate);
ot_s32 ot_oge_bit_blit(ot_s32 handle, const ot_oge_double_src *double_src, const ot_oge_opt *opt);
ot_s32 ot_oge_pattern_fill(ot_s32 handle, const ot_oge_double_src *double_src, const ot_oge_pattern_fill_opt *fill_opt);
ot_s32 ot_oge_mb_blit(ot_s32 handle, const ot_oge_mb_src *mb_src, const ot_oge_mb_opt *opt);
ot_s32 ot_oge_set_alpha_threshold_value(ot_u8 threshold_value);
ot_s32 ot_oge_get_alpha_threshold_value(ot_u8 *threshold_value);
ot_s32 ot_oge_set_alpha_threshold_state(ot_bool threshold_en);
ot_s32 ot_oge_get_alpha_threshold_state(ot_bool *threshold_en);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OT_OGE_H */
