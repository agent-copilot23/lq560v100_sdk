#ifndef OT_GFB_COMMON_H
#define OT_GFB_COMMON_H

#include <linux/fb.h>

#include "ot_eis_type.h"
#include "ot_eis_mod.h"
#include "ot_eis_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OT_GFB_MAX_FB_NUM 5

#define OT_ERR_GFB_BASE ((ot_s32)(OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_GFB, 1)))

typedef enum {
    OT_ERR_GFB_NOT_INIT = OT_ERR_GFB_BASE, /* oge device not open yet */
    OT_ERR_GFB_DEV_OPEN_FAILED,            /* open oge device failed */
    OT_ERR_GFB_NULL_PTR,                   /* input parameters contain null ptr */
    OT_ERR_GFB_PARAM_ILLEGAL,
    OT_ERR_GFB_IOCTL_FAILED,
    OT_ERR_GFB_BUTT,
}ot_gfb_err_code;

typedef struct {
    ot_bool enable;         /* colorkey enable flag */
    ot_u32 value;           /* colorkey value, maybe contains alpha */
} ot_eis_fb_colorkey;

/* Alpha info */
typedef struct {
    ot_bool pixel_alpha;   /*  pixel alpha enable flag */
    ot_bool global_alpha_en;  /*  global alpha enable flag */
    ot_u8 alpha0;         /*  alpha0 value, used in ARGB1555 */
    ot_u8 alpha1;         /*  alpha1 value, used in ARGB1555 */
    ot_u8 global_alpha;    /*  global alpha value */
} ot_eis_fb_alpha;

typedef struct {
    ot_s32 x_pos;         /* <  horizontal position */
    ot_s32 y_pos;         /* <  vertical position */
} ot_eis_fb_point;

typedef enum {
    OT_EIS_FB_FORMAT_RGB565 = 0,
    OT_EIS_FB_FORMAT_RGB888,              /*  RGB888 24bpp */

    OT_EIS_FB_FORMAT_KRGB444,       /*  RGB444 16bpp */
    OT_EIS_FB_FORMAT_KRGB555,       /*  RGB555 16bpp */
    OT_EIS_FB_FORMAT_KRGB888,       /*  RGB888 32bpp */

    OT_EIS_FB_FORMAT_ARGB4444,      /* ARGB4444 */
    OT_EIS_FB_FORMAT_ARGB1555,      /* ARGB1555 */
    OT_EIS_FB_FORMAT_ARGB8888,      /* ARGB8888 */
    OT_EIS_FB_FORMAT_ARGB8565,      /* ARGB8565 */

    OT_EIS_FB_FORMAT_RGBA4444,      /* ARGB4444 */
    OT_EIS_FB_FORMAT_RGBA5551,      /* RGBA5551 */
    OT_EIS_FB_FORMAT_RGBA5658,      /* RGBA5658 */
    OT_EIS_FB_FORMAT_RGBA8888,      /* RGBA8888 */

    OT_EIS_FB_FORMAT_BGR565,        /* BGR565 */
    OT_EIS_FB_FORMAT_BGR888,        /* BGR888 */
    OT_EIS_FB_FORMAT_ABGR4444,      /* ABGR4444 */
    OT_EIS_FB_FORMAT_ABGR1555,      /* ABGR1555 */
    OT_EIS_FB_FORMAT_ABGR8888,      /* ABGR8888 */
    OT_EIS_FB_FORMAT_ABGR8565,      /* ABGR8565 */
    OT_EIS_FB_FORMAT_KBGR444,       /* BGR444 16bpp */
    OT_EIS_FB_FORMAT_KBGR555,       /* BGR555 16bpp */
    OT_EIS_FB_FORMAT_KBGR888,       /* BGR888 32bpp */

    OT_EIS_FB_FORMAT_1BPP,          /* clut1 */
    OT_EIS_FB_FORMAT_2BPP,          /* clut2 */
    OT_EIS_FB_FORMAT_4BPP,          /* clut4 */
    OT_EIS_FB_FORMAT_8BPP,          /* clut8 */
    OT_EIS_FB_FORMAT_ACLUT44,       /* AClUT44 */
    OT_EIS_FB_FORMAT_ACLUT88,         /* ACLUT88 */
    OT_EIS_FB_FORMAT_PUYVY,         /* UYVY */
    OT_EIS_FB_FORMAT_PYUYV,         /* YUYV */
    OT_EIS_FB_FORMAT_PYVYU,         /* YVYU */
    OT_EIS_FB_FORMAT_YUV888,        /* YUV888 */
    OT_EIS_FB_FORMAT_AYUV8888,      /* AYUV8888 */
    OT_EIS_FB_FORMAT_YUVA8888,      /* YUVA8888 */
    OT_EIS_FB_FORMAT_BUTT
} ot_eis_fb_color_format;

/* surface info */
typedef struct {
    ot_phys_addr  phys_addr;     /* start physical address */
    ot_u32  width;       /* width pixels */
    ot_u32  height;      /* height pixels */
    ot_u32  pitch;       /* line pixels */
    ot_eis_fb_color_format format; /* color format */
} ot_eis_fb_surface;

typedef struct {
    ot_s32 x;
    ot_s32 y;
    ot_s32 width;
    ot_s32 height;
} ot_eis_fb_rect;

/* refresh surface info */
typedef struct {
    ot_eis_fb_surface canvas;
    ot_eis_fb_rect update_rect;       /* refresh region */
} ot_eis_fb_buf;

typedef struct {
    ot_bool is_key_rgb;
    ot_bool is_key_alpha;      /* whether support colorkey alpha */
    ot_bool is_global_alpha;   /* whether support global alpha */
    ot_bool is_cmap;          /* whether support color map */
    ot_bool has_cmap_reg;    /* whether has color map register */
    ot_bool is_color_format[OT_EIS_FB_FORMAT_BUTT]; /* support which color format */
    ot_bool is_vo_scale;       /* support vo scale */
    /* whether support a certain layer */
    ot_bool is_layer_support;
    ot_u32  max_width;    /* the max pixels per line */
    ot_u32  max_height;   /* the max lines */
    ot_u32  min_width;    /* the min pixels per line */
    ot_u32  min_height;   /* the min lines */
    ot_u32  ver_deflicker_level;   /* vertical deflicker level, 0 means vertical deflicker is unsupporteded. */
    ot_u32  hor_deflicker_level;   /* horizontal deflicker level, 0 means horizontal deflicker is unsupporteded. */
    ot_bool  is_decompress;
    ot_bool  is_premul;
    ot_bool  is_ghdr;         /* new feature. is ghdr supported. */
    ot_bool  is_osb;   /* new feature. is smart rect supported */
} ot_eis_fb_capability;

typedef enum {
    OT_EIS_FB_LAYER_BUF_DOUBLE = 0x0,       /* 2 display buf in fb */
    OT_EIS_FB_LAYER_BUF_ONE    = 0x1,       /* 1 display buf in fb */
    OT_EIS_FB_LAYER_BUF_NONE   = 0x2,       /* no display buf in fb,the buf user refreshed will be directly set to VO */
    OT_EIS_FB_LAYER_BUF_DOUBLE_IMMEDIATE = 0x3, /* 2 display buf in fb, each refresh will be displayed */
    OT_EIS_FB_LAYER_BUF_BUTT
} ot_eis_fb_layer_buf;

/* antiflicker level */
/* Auto means fb will choose a appropriate antiflicker level automatically according to the color info of map */
typedef enum {
    OT_EIS_FB_LAYER_ANTIFLICKER_NONE = 0x0,  /* no antiflicker */
    OT_EIS_FB_LAYER_ANTIFLICKER_LOW = 0x1,   /* low level */
    OT_EIS_FB_LAYER_ANTIFLICKER_MID = 0x2, /* middle level */
    OT_EIS_FB_LAYER_ANTIFLICKER_HIGH = 0x3, /* high level */
    OT_EIS_FB_LAYER_ANTIFLICKER_AUTO = 0x4, /* auto */
    OT_EIS_FB_LAYER_ANTIFLICKER_BUTT
} ot_eis_fb_layer_antiflicker_level;

/* layer info maskbit */
typedef enum {
    OT_EIS_FB_LAYER_MASK_BUF_MODE = 0x1,           /* buf mode bitmask */
    OT_EIS_FB_LAYER_MASK_ANTIFLICKER_MODE = 0x2,  /* antiflicker mode bitmask */
    OT_EIS_FB_LAYER_MASK_POS = 0x4,               /* the position bitmask */
    OT_EIS_FB_LAYER_MASK_CANVAS_SIZE = 0x8,      /* canvassize bitmask */
    OT_EIS_FB_LAYER_MASK_DISPLAY_SIZE = 0x10,       /* displaysize bitmask */
    OT_EIS_FB_LAYER_MASK_SCREEN_SIZE = 0x20,     /* screensize bitmask */
    OT_EIS_FB_LAYER_MASK_MUL = 0x40,           /* pre-mult bitmask */
    OT_EIS_FB_LAYER_MASK_BUTT
} ot_eis_fb_layer_info_maskbit;

/* layer info */
typedef struct {
    ot_eis_fb_layer_buf buf_mode;
    ot_eis_fb_layer_antiflicker_level antiflicker_level;
    ot_s32 x_pos;           /*  the x pos of origin point in screen */
    ot_s32 y_pos;           /*  the y pos of origin point in screen */
    ot_u32 canvas_width;    /*  the width of canvas buffer */
    ot_u32 canvas_height;   /*  the height of canvas buffer */
    /* the width of display buf in fb.for 0 buf, there is no display buf in fb, so it's effectless */
    ot_u32 display_width;
    ot_u32 display_height;  /*  the height of display buf in fb. */
    ot_u32 screen_width;    /*  the width of screen */
    ot_u32 screen_height;   /*  the height of screen */
    ot_bool is_premul;      /*  The data drawn in buf is premul data or not */
    ot_u32 mask;           /*  param modify mask bit */
} ot_eis_fb_layer_info;

typedef enum {
    OT_EIS_FB_MIRROR_NONE = 0x0,
    OT_EIS_FB_MIRROR_HOR = 0x1,
    OT_EIS_FB_MIRROR_VER = 0x2,
    OT_EIS_FB_MIRROR_BOTH = 0x3,
    OT_EIS_FB_MIRROR_INVALID
} ot_eis_fb_mirror_mode;

typedef enum {
    OT_EIS_FB_ROTATE_NONE = 0x0,
    OT_EIS_FB_ROTATE_90 = 0x1,
    OT_EIS_FB_ROTATE_180 = 0x2,
    OT_EIS_FB_ROTATE_270 = 0x3,
    OT_EIS_FB_ROTATE_INVALID
} ot_eis_fb_rotate_mode;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
