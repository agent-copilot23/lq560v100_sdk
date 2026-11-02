#ifndef OT_GFB_H
#define OT_GFB_H

#include "ot_gfb_common.h"

#ifdef __cplusplus
extern "C" {
#endif

ot_s32 ot_gfb_open(ot_s32 fb_id, ot_s32 *fb_fd);

ot_s32 ot_gfb_close(ot_s32 fb_fd);

ot_s32 ot_gfb_get_var_screen_attr(ot_s32 fb_fd, struct fb_var_screeninfo* var);

ot_s32 ot_gfb_set_var_screen_attr(ot_s32 fb_fd, const struct fb_var_screeninfo* var);

ot_s32 ot_gfb_set_color_map_attr(ot_s32 fb_fd, const struct fb_cmap* cmap);

ot_s32 ot_gfb_get_fix_screenf_info(ot_s32 fb_fd, struct fb_fix_screeninfo* fix);

ot_s32 ot_gfb_set_display_offset(ot_s32 fb_fd, const struct fb_var_screeninfo* var);

ot_s32 ot_gfb_mmap(ot_s32 fb_fd, ot_u32 smemlen, ot_void** virt_addr);

ot_s32 ot_gfb_munmap(ot_void* virt_addr, ot_u32 smemlen);

ot_s32 ot_gfb_set_colorkey_vaule(ot_s32 fb_fd, const ot_eis_fb_colorkey* fb_colorkey);

ot_s32 ot_gfb_get_colorkey_vaule(ot_s32 fb_fd, ot_eis_fb_colorkey* fb_colorkey);

ot_s32 ot_gfb_set_alpha_cfg(ot_s32 fb_fd, const ot_eis_fb_alpha* alpha);

ot_s32 ot_gfb_get_alpha_cfg(ot_s32 fb_fd, ot_eis_fb_alpha* alpha);

ot_s32 ot_gfb_set_screen_origin(ot_s32 fb_fd, const ot_eis_fb_point* point);

ot_s32 ot_gfb_get_screen_origin(ot_s32 fb_fd, ot_eis_fb_point* point);

ot_s32 ot_gfb_set_display_state(ot_s32 fb_fd, ot_bool display);

ot_s32 ot_gfb_get_display_state(ot_s32 fb_fd, ot_bool* display);

ot_s32 ot_gfb_refresh_display_info(ot_s32 fb_fd, const ot_eis_fb_buf* buf);

ot_s32 ot_gfb_get_capability(ot_s32 fb_fd, ot_eis_fb_capability* cap);

ot_s32 ot_gfb_set_layer_info(ot_s32 fb_fd, ot_eis_fb_layer_info* layer_info);

ot_s32 ot_gfb_get_layer_info(ot_s32 fb_fd, ot_eis_fb_layer_info* layer_info);

ot_s32 ot_gfb_set_mirror_mode(ot_s32 fb_fd, ot_eis_fb_mirror_mode *mirror_mode);

ot_s32 ot_gfb_get_mirror_mode(ot_s32 fb_fd, ot_eis_fb_mirror_mode *mirror_mode);

ot_s32 ot_gfb_set_rotate_mode(ot_s32 fb_fd, ot_eis_fb_rotate_mode *rotate_mode);

ot_s32 ot_gfb_get_rotate_mode(ot_s32 fb_fd, ot_eis_fb_rotate_mode *rotate_mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
