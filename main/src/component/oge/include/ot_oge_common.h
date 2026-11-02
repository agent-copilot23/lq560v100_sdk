/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef OT_OGE_COMMON_H
#define OT_OGE_COMMON_H

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#ifndef ot_phys_addr_t
    typedef unsigned long long  ot_phys_addr_t;
#endif

#ifndef ot_oge_handle
    typedef ot_s32  ot_oge_handle;
#endif

/* oge start err no. */
#define OT_ERR_OGE_BASE ((ot_s32)(((0x80UL + 0x20UL) << 24) | (100 << 16) | (4 << 13) | 1))

typedef enum {
    OT_ERR_OGE_DEV_NOT_OPEN = OT_ERR_OGE_BASE, /* oge device not open yet */
    OT_ERR_OGE_DEV_OPEN_FAILED,                /* open oge device failed */
    OT_ERR_OGE_NULL_PTR,                       /* input parameters contain null ptr */
    OT_ERR_OGE_NO_MEM,                         /* malloc failed  */
    OT_ERR_OGE_INVALID_HANDLE,                 /* invalid job handle */
    OT_ERR_OGE_INVALID_PARAM,                  /* invalid parameter */
    OT_ERR_OGE_NOT_ALIGNED,                    /* aligned error for position, stride, width */
    OT_ERR_OGE_MINIFICATION,                   /* invalid minification */
    OT_ERR_OGE_CLIP_AREA,                      /* clip area and operation area have no intersection */
    OT_ERR_OGE_JOB_TIMEOUT,                    /* blocked job wait timeout */
    OT_ERR_OGE_UNSUPPORTED_OPERATION,          /* unsupported operation */
    OT_ERR_OGE_QUERY_TIMEOUT,                  /* query time out */
    OT_ERR_OGE_INTERRUPT,                      /* blocked job was interrupted */
    OT_ERR_OGE_BUTT,
}ot_oge_err_code;

/* RGB and packet YUV formats and semi-planar YUV format */
typedef enum {
    OT_OGE_COLOR_FORMAT_RGB444 = 0,       /* RGB444 format */
    OT_OGE_COLOR_FORMAT_BGR444,           /* BGR444 format */
    OT_OGE_COLOR_FORMAT_RGB555,           /* RGB555 format */
    OT_OGE_COLOR_FORMAT_BGR555,           /* BGR555 format */
    OT_OGE_COLOR_FORMAT_RGB565,           /* RGB565 format */
    OT_OGE_COLOR_FORMAT_BGR565,           /* BGR565 format */
    OT_OGE_COLOR_FORMAT_RGB888,           /* RGB888 format */
    OT_OGE_COLOR_FORMAT_BGR888,           /* BGR888 format */
    OT_OGE_COLOR_FORMAT_ARGB4444,         /* ARGB4444 format */
    OT_OGE_COLOR_FORMAT_ABGR4444,         /* ABGR4444 format */
    OT_OGE_COLOR_FORMAT_RGBA4444,         /* RGBA4444 format */
    OT_OGE_COLOR_FORMAT_BGRA4444,         /* BGRA4444 format */
    OT_OGE_COLOR_FORMAT_ARGB1555,         /* ARGB1555 format */
    OT_OGE_COLOR_FORMAT_ABGR1555,         /* ABGR1555 format */
    OT_OGE_COLOR_FORMAT_RGBA1555,         /* RGBA1555 format */
    OT_OGE_COLOR_FORMAT_BGRA1555,         /* BGRA1555 format */
    OT_OGE_COLOR_FORMAT_ARGB8565,         /* ARGB8565 format */
    OT_OGE_COLOR_FORMAT_ABGR8565,         /* ABGR8565 format */
    OT_OGE_COLOR_FORMAT_RGBA8565,         /* RGBA8565 format */
    OT_OGE_COLOR_FORMAT_BGRA8565,         /* BGRA8565 format */
    OT_OGE_COLOR_FORMAT_ARGB8888,         /* ARGB8888 format */
    OT_OGE_COLOR_FORMAT_ABGR8888,         /* ABGR8888 format */
    OT_OGE_COLOR_FORMAT_RGBA8888,         /* RGBA8888 format */
    OT_OGE_COLOR_FORMAT_BGRA8888,         /* BGRA8888 format */
    OT_OGE_COLOR_FORMAT_RABG8888,         /* RABG8888 format */
    /* 1-bit palette format without alpha component. Each pixel occupies one bit. */
    OT_OGE_COLOR_FORMAT_CLUT1,
    /* 2-bit palette format without alpha component. Each pixel occupies two bits. */
    OT_OGE_COLOR_FORMAT_CLUT2,
    /* 4-bit palette format without alpha component. Each pixel occupies four bits. */
    OT_OGE_COLOR_FORMAT_CLUT4,
    /* 8-bit palette format without alpha component. Each pixel occupies eight bits. */
    OT_OGE_COLOR_FORMAT_CLUT8,
    /* 4-bit palette format with alpha component. Each pixel occupies 8 bit. */
    OT_OGE_COLOR_FORMAT_ACLUT44,
    /* 8-bit palette format with alpha component. Each pixel occupies 16 bit. */
    OT_OGE_COLOR_FORMAT_ACLUT88,
    OT_OGE_COLOR_FORMAT_A1,                 /* Alpha format. Each pixel occupies one bit. */
    OT_OGE_COLOR_FORMAT_A8,                 /* Alpha format. Each pixel occupies eight bits. */
    OT_OGE_COLOR_FORMAT_YCbCr888,           /* 33：YCbCr888 */
    OT_OGE_COLOR_FORMAT_AYCbCr8888,         /* 34：AYCbCr8888 */
    OT_OGE_COLOR_FORMAT_YCbCr422,           /* 35：PKYUYV */
    OT_OGE_COLOR_FORMAT_PKGYVYU,            /* 36：PKYVYU */
    OT_OGE_COLOR_FORMAT_PKGUYVY,            /* 37：PKUYVY */
    OT_OGE_COLOR_FORMAT_PKGVYUY,            /* 38：PKVYUY */
    OT_OGE_COLOR_FORMAT_PKGVUYY,            /* 39：PKVUYY */
    OT_OGE_COLOR_FORMAT_PKGYYUV,            /* 40：PKYYUV */
    OT_OGE_COLOR_FORMAT_PKGUVYY,            /* 41：PKUVYY */
    OT_OGE_COLOR_FORMAT_PKGYYVU,            /* 42：PKYYVU */
    OT_OGE_COLOR_FORMAT_JPG_YCbCr400MBP,    /* 43：SP400 */
    OT_OGE_COLOR_FORMAT_JPG_YCbCr422MBHP,   /* 44：SP422h(1*2)(Pixel width is even) */
    OT_OGE_COLOR_FORMAT_JPG_YCbCr422MBVP,   /* 45：SP422V(2*1)(Pixel width is even) */
    OT_OGE_COLOR_FORMAT_MP1_YCbCr420MBP,    /* 46：SP420(cd hi The pixel width and height are even numbers.) */
    OT_OGE_COLOR_FORMAT_MP2_YCbCr420MBP,    /* 47：SP420(cd hi The pixel width and height are even numbers.) */
    OT_OGE_COLOR_FORMAT_MP2_YCbCr420MBI,    /* 48：SP420(cd hi The pixel width and height are even numbers.) */
    OT_OGE_COLOR_FORMAT_JPG_YCbCr420MBP,    /* 49：SP420(cd hi The pixel width and height are even numbers.) */
    OT_OGE_COLOR_FORMAT_JPG_YCbCr444MBP,    /* 50：SP444(cd hi The pixel width and height are even numbers.) */
    OT_OGE_COLOR_FORMAT_MAX                 /* 51：End of enumeration */
} ot_oge_color_format;

/* Definition of the semi-planar YUV format */
typedef enum {
    OT_OGE_MB_COLOR_FORMAT_JPG_YCbCr400MBP = 0, /* Semi-planar YUV400 format, for JPG decoding */
    /* Semi-planar YUV422 format, horizontal sampling, for JPG decoding */
    OT_OGE_MB_COLOR_FORMAT_JPG_YCbCr422MBHP,
    OT_OGE_MB_COLOR_FORMAT_JPG_YCbCr422MBVP, /* Semi-planar YUV422 format, vertical sampling, for JPG decoding */
    OT_OGE_MB_COLOR_FORMAT_MP1_YCbCr420MBP,  /* Semi-planar YUV420 format */
    OT_OGE_MB_COLOR_FORMAT_MP2_YCbCr420MBP,  /* Semi-planar YUV420 format */
    OT_OGE_MB_COLOR_FORMAT_MP2_YCbCr420MBI,  /* Semi-planar YUV420 format */
    OT_OGE_MB_COLOR_FORMAT_JPG_YCbCr420MBP,  /* Semi-planar YUV420 format, for JPG pictures */
    OT_OGE_MB_COLOR_FORMAT_JPG_YCbCr444MBP,  /* Semi-planar YUV444 format, for JPG pictures */
    OT_OGE_MB_COLOR_FORMAT_MAX
} ot_oge_mb_color_format;

/* Structure of the bitmap information set by customers */
typedef struct {
    ot_phys_addr_t phys_addr; /* Header address of a bitmap or the Y component */
    ot_u32 phys_len;
    ot_oge_color_format color_format; /* Color format */
    ot_u32 height; /* Bitmap height */
    ot_u32 width; /* Bitmap width */
    ot_u32 stride; /* Stride of a bitmap or the Y component */
    ot_bool is_ycbcr_clut; /* Whether the CLUT is in the YCbCr space. */
    ot_bool alpha_max_is_255; /* The maximum alpha value of a bitmap is 255 or 128. */
    ot_bool support_alpha_ex_1555; /* Whether to enable the alpha extension of an ARGB1555 bitmap. */
    ot_u8 alpha0;        /* Values of alpha0 and alpha1, used as the ARGB1555 format */
    ot_u8 alpha1;        /* Values of alpha0 and alpha1, used as the ARGB1555 format */
    ot_phys_addr_t cbcr_phys_addr;    /* Address of the CbCr component, pilot */
    ot_u32 cbcr_phys_len;
    ot_u32 cbcr_stride;  /* Stride of the CbCr component, pilot */
    /* <Address of the color look-up table (CLUT), for color extension or color correction */
    ot_phys_addr_t clut_phys_addr;
    ot_u32 clut_phys_len;
} ot_oge_surface;

/* Definition of the semi-planar YUV data */
typedef struct {
    ot_oge_mb_color_format mb_color_format; /* YUV format */
    ot_phys_addr_t y_addr;             /* Physical address of the Y component */
    ot_u32 y_len;
    ot_u32 y_width;            /* Width of the Y component */
    ot_u32 y_height;           /* Height of the Y component */
    ot_u32 y_stride;           /* Stride of the Y component, indicating bytes in each row */
    ot_phys_addr_t cbcr_phys_addr;      /* Width of the UV component */
    ot_u32 cbcr_phys_len;
    ot_u32 cbcr_stride;        /* Stride of the UV component, indicating the bytes in each row */
} ot_oge_mb_surface;

/* Definition of the OGE rectangle */
typedef struct {
    ot_s32 pos_x;   /* Horizontal coordinate */
    ot_s32 pos_y;   /* Vertical coordinate */
    ot_u32 width;  /* Width */
    ot_u32 height; /* Height */
} ot_oge_rect;

/* dma module */
typedef struct {
    ot_oge_surface *dst_surface;
    ot_oge_rect *dst_rect;
} ot_oge_none_src;

/* single source */
typedef struct {
    ot_oge_surface *src_surface;
    ot_oge_surface *dst_surface;
    ot_oge_rect *src_rect;
    ot_oge_rect *dst_rect;
} ot_oge_single_src;

/* mb source */
typedef struct {
    ot_oge_mb_surface *mb_surface;
    ot_oge_surface *dst_surface;
    ot_oge_rect *src_rect;
    ot_oge_rect *dst_rect;
} ot_oge_mb_src;

/* double source */
typedef struct {
    ot_oge_surface *bg_surface;
    ot_oge_surface *fg_surface;
    ot_oge_surface *dst_surface;
    ot_oge_rect *bg_rect;
    ot_oge_rect *fg_rect;
    ot_oge_rect *dst_rect;
} ot_oge_double_src;

/* Logical operation type */
typedef enum {
    OT_OGE_ALPHA_BLENDING_NONE = 0x0,     /* No alpha and raster of operation (ROP) blending */
    OT_OGE_ALPHA_BLENDING_BLEND = 0x1,    /* Alpha blending */
    OT_OGE_ALPHA_BLENDING_ROP = 0x2,      /* ROP blending */
    OT_OGE_ALPHA_BLENDING_COLORIZE = 0x4, /* Colorize operation */
    OT_OGE_ALPHA_BLENDING_MAX = 0x8       /* End of enumeration */
} ot_oge_alpha_blending;

/* Definition of ROP codes */
typedef enum {
    OT_OGE_ROP_BLACK = 0,   /* Blackness */
    OT_OGE_ROP_NOTMERGEPEN, /* ~(S2 | S1) */
    OT_OGE_ROP_MASKNOTPEN,  /* ~S2&S1 */
    OT_OGE_ROP_NOTCOPYPEN,  /* ~S2 */
    OT_OGE_ROP_MASKPENNOT,  /* S2&~S1 */
    OT_OGE_ROP_NOT,         /* ~S1 */
    OT_OGE_ROP_XORPEN,      /* S2^S1 */
    OT_OGE_ROP_NOTMASKPEN,  /* ~(S2 & S1) */
    OT_OGE_ROP_MASKPEN,     /* S2&S1 */
    OT_OGE_ROP_NOTXORPEN,   /* ~(S2^S1) */
    OT_OGE_ROP_NOP,         /* S1 */
    OT_OGE_ROP_MERGENOTPEN, /* ~S2|S1 */
    OT_OGE_ROP_COPYPEN,     /* S2 */
    OT_OGE_ROP_MERGEPENNOT, /* S2|~S1 */
    OT_OGE_ROP_MERGEPEN,    /* S2|S1 */
    OT_OGE_ROP_WHITE,       /* Whiteness */
    OT_OGE_ROP_MAX
} ot_oge_rop_mode;

/* Definition of the blit mirror */
typedef enum {
    OT_OGE_MIRROR_NONE = 0,   /* No mirror */
    OT_OGE_MIRROR_HOR, /* Horizontal mirror */
    OT_OGE_MIRROR_VER,   /* Vertical mirror */
    OT_OGE_MIRROR_BOTH,       /* Horizontal and vertical mirror */
    OT_OGE_MIRROR_MAX
} ot_oge_mirror_mode;

/* Clip operation type */
typedef enum {
    OT_OGE_CLIP_MODE_NONE = 0, /* No clip */
    OT_OGE_CLIP_MODE_INSIDE,   /* Clip the data within the rectangle to output and discard others */
    OT_OGE_CLIP_MODE_OUTSIDE,  /* Clip the data outside the rectangle to output and discard others */
    OT_OGE_CLIP_MODE_MAX
} ot_oge_clip_mode;

/* Scaling mode for the macroblock */
typedef enum {
    OT_OGE_MB_RESIZE_NONE = 0,       /* No scaling */
    OT_OGE_MB_RESIZE_QUALITY_LOW,    /* Low-quality scaling */
    OT_OGE_MB_RESIZE_QUALITY_MIDDLE, /* Medium-quality scaling */
    OT_OGE_MB_RESIZE_QUALITY_HIGH,   /* High-quality scaling */
    OT_OGE_MB_RESIZE_MAX
} ot_oge_mb_resize;

/* Definition of fill colors */
typedef struct {
    ot_oge_color_format color_format; /* OGE pixel format */
    ot_u32 color_value;         /* Fill colors that vary according to pixel formats */
} ot_oge_fill_color;

/* Definition of colorkey modes */
typedef enum {
    OT_OGE_COLORKEY_MODE_NONE = 0,   /* No colorkey */
    /* When performing the colorkey operation on the foreground bitmap,
    you need to perform this operation before the CLUT for color extension
    and perform this operation after the CLUT for color correction. */
    OT_OGE_COLORKEY_MODE_FG,
    OT_OGE_COLORKEY_MODE_BG, /* Perform the colorkey operation on the background bitmap */
    OT_OGE_COLORKEY_MODE_MAX
} ot_oge_colorkey_mode;

/* Definition of colorkey range */
typedef struct {
    ot_u8 min_component;   /* Minimum value of a component */
    ot_u8 max_component;   /* Maximum value of a component */
    ot_u8 is_component_out;    /* The colorkey of a component is within or beyond the range. */
    ot_u8 is_component_ignore; /* Whether to ignore a component. */
    ot_u8 component_mask;  /* Component mask */
} ot_oge_colorkey_component;

/* Definition of colorkey values */
typedef union {
    struct {
        ot_oge_colorkey_component alpha; /* Alpha component */
        ot_oge_colorkey_component red;   /* Red component */
        ot_oge_colorkey_component green; /* Green component */
        ot_oge_colorkey_component blue;  /* Blue component */
    } argb_colorkey;                     /* AUTO:ot_oge_colorkey_mode:OT_OGE_COLORKEY_MODE_NONE; */
    struct {
        ot_oge_colorkey_component alpha; /* Alpha component */
        ot_oge_colorkey_component y;     /* Y component */
        ot_oge_colorkey_component cb;    /* Cb component */
        ot_oge_colorkey_component cr;    /* Cr component */
    } ycbcr_colorkey;                    /* AUTO:ot_oge_colorkey_mode:OT_OGE_COLORKEY_MODE_FG; */
    struct {
        ot_oge_colorkey_component alpha; /* Alpha component */
        ot_oge_colorkey_component clut;  /* Palette component */
    } clut_colorkey;                     /* AUTO:ot_oge_colorkey_mode:OT_OGE_COLORKEY_MODE_BG; */
} ot_oge_colorkey;

/* Definition of alpha output sources */
typedef enum {
    OT_OGE_OUT_ALPHA_FROM_NORM = 0,    /* Output from the result of alpha blending or anti-flicker */
    OT_OGE_OUT_ALPHA_FROM_BG,  /* Output from the background bitmap */
    OT_OGE_OUT_ALPHA_FROM_FG,  /* Output from the foreground bitmap */
    OT_OGE_OUT_ALPHA_FROM_GLOBALALPHA, /* Output from the global alpha */
    OT_OGE_OUT_ALPHA_FROM_MAX
} ot_oge_out_alpha_from;

/* Definition of filtering */
typedef enum {
    OT_OGE_FILTER_MODE_COLOR = 0, /* Filter the color */
    OT_OGE_FILTER_MODE_ALPHA,     /* Filter the alpha channel */
    OT_OGE_FILTER_MODE_BOTH,      /* Filter the color and alpha channel */
    OT_OGE_FILTER_MODE_NONE,      /* No filter */
    OT_OGE_FILTER_MODE_MAX
} ot_oge_filter_mode;

/* blend mode */
typedef enum {
    OT_OGE_BLEND_ZERO = 0x0,
    OT_OGE_BLEND_ONE,
    OT_OGE_BLEND_SRC2COLOR,
    OT_OGE_BLEND_INVSRC2COLOR,
    OT_OGE_BLEND_SRC2ALPHA,
    OT_OGE_BLEND_INVSRC2ALPHA,
    OT_OGE_BLEND_SRC1COLOR,
    OT_OGE_BLEND_INVSRC1COLOR,
    OT_OGE_BLEND_SRC1ALPHA,
    OT_OGE_BLEND_INVSRC1ALPHA,
    OT_OGE_BLEND_SRC2ALPHASAT,
    OT_OGE_BLEND_MAX
} ot_oge_blend_mode;

/* Alpha blending command. You can set parameters or select Porter or Duff. */
/* pixel = (source * fs + destination * fd),
   sa = source alpha,
   da = destination alpha */
typedef enum {
    OT_OGE_BLEND_CMD_NONE = 0x0, /* fs: sa      fd: 1.0-sa */
    OT_OGE_BLEND_CMD_CLEAR,      /* fs: 0.0     fd: 0.0 */
    OT_OGE_BLEND_CMD_SRC,        /* fs: 1.0     fd: 0.0 */
    OT_OGE_BLEND_CMD_SRCOVER,    /* fs: 1.0     fd: 1.0-sa */
    OT_OGE_BLEND_CMD_DSTOVER,    /* fs: 1.0-da  fd: 1.0 */
    OT_OGE_BLEND_CMD_SRCIN,      /* fs: da      fd: 0.0 */
    OT_OGE_BLEND_CMD_DSTIN,      /* fs: 0.0     fd: sa */
    OT_OGE_BLEND_CMD_SRCOUT,     /* fs: 1.0-da  fd: 0.0 */
    OT_OGE_BLEND_CMD_DSTOUT,     /* fs: 0.0     fd: 1.0-sa */
    OT_OGE_BLEND_CMD_SRCATOP,    /* fs: da      fd: 1.0-sa */
    OT_OGE_BLEND_CMD_DSTATOP,    /* fs: 1.0-da  fd: sa */
    OT_OGE_BLEND_CMD_ADD,        /* fs: 1.0     fd: 1.0 */
    OT_OGE_BLEND_CMD_XOR,        /* fs: 1.0-da  fd: 1.0-sa */
    OT_OGE_BLEND_CMD_DST,        /* fs: 0.0     fd: 1.0 */
    OT_OGE_BLEND_CMD_CONFIG,     /* You can set the parameteres. */
    OT_OGE_BLEND_CMD_MAX
} ot_oge_blend_cmd;

/* Options for the alpha blending operation */
typedef struct {
    ot_bool global_alpha_en;       /* Global alpha enable */
    ot_bool pixel_alpha_en;        /* Pixel alpha enable */
    ot_bool src1_alpha_premulti;       /* Src1 alpha premultiply enable */
    ot_bool src2_alpha_premulti;       /* Src2 alpha premultiply enable */
    ot_oge_blend_cmd blend_cmd;        /* Alpha blending command */
    /* Src1 blending mode select. It is valid when blend_cmd is set to OT_OGE_BLEND_CMD_CONFIG. */
    ot_oge_blend_mode src1_blend_mode;
    /* Src2 blending mode select. It is valid when blend_cmd is set to OT_OGE_BLEND_CMD_CONFIG. */
    ot_oge_blend_mode src2_blend_mode;
} ot_oge_blend_opt;

/* CSC parameter option */
typedef struct {
    ot_bool src_csc_user_en;  /* User-defined ICSC parameter enable */
    ot_bool src_csc_param_reload_en; /* User-defined ICSC parameter reload enable */
    ot_bool dst_csc_user_en;  /* User-defined OCSC parameter enable */
    ot_bool dst_csc_param_reload_en; /* User-defined OCSC parameter reload enable */
    ot_phys_addr_t src_csc_param_addr; /* ICSC parameter address. The address must be 128-bit aligned. */
    ot_s32 src_csc_param_len;
    ot_phys_addr_t dst_csc_param_addr; /* OCSC parameter address. The address must be 128-bit aligned. */
    ot_s32 dst_csc_param_len;
} ot_oge_csc_opt;

/* Definition of blit operation options */
typedef struct {
    ot_oge_alpha_blending alpha_blending_cmd; /* Logical operation type */

    ot_oge_rop_mode rop_color; /* ROP type of the color space */

    ot_oge_rop_mode rop_alpha; /* ROP type of the alpha component */

    ot_oge_colorkey_mode colorkey_mode; /* Colorkey mode */

    ot_oge_colorkey colorkey_value; /* Colorkey value */

    ot_oge_clip_mode clip_mode; /* Perform the clip operation within or beyond the area */

    ot_oge_rect clip_rect; /* Definition of the clipping area */

    ot_bool resize; /* Whether to scale */

    ot_oge_filter_mode filter_mode; /* Filtering mode during scaling */

    ot_oge_mirror_mode mirror; /* Mirror type */

    ot_bool clut_reload; /* Whether to reload the CLUT */

    ot_u8 global_alpha; /* Global alpha value */

    ot_oge_out_alpha_from out_alpha_from; /* Source of the output alpha */

    ot_u32 color_resize; /* Colorize value */

    ot_oge_blend_opt blend_opt;

    ot_oge_csc_opt csc_opt;
    ot_bool is_compress;
    ot_bool is_decompress;
} ot_oge_opt;

/* Definition of macroblock operation options */
typedef struct {
    ot_oge_clip_mode clip_mode; /* Clip mode */

    ot_oge_rect clip_rect; /* Definition of the clipping area */

    ot_oge_mb_resize resize_en; /* Scaling information */

    /* If the alpha value is not set, the maximum alpha value is output by default. */
    ot_bool is_set_out_alpha;

    ot_u8 out_alpha; /* Global alpha for operation */
} ot_oge_mb_opt;

/* Definition of the pattern filling operation */
typedef struct {
    ot_oge_alpha_blending alpha_blending_cmd; /* Logical operation type */

    ot_oge_rop_mode rop_color; /* ROP type of the color space */

    ot_oge_rop_mode rop_alpha; /* ROP type of the alpha component */

    ot_oge_colorkey_mode colorkey_mode; /* Colorkey mode */

    ot_oge_colorkey colorkey_value; /* Colorkey value */

    ot_oge_clip_mode clip_mode; /* Clip mode */

    ot_oge_rect clip_rect; /* Clipping area */

    ot_bool clut_reload; /* Whether to reload the CLUT */

    ot_u8 global_alpha; /* Global alpha */

    ot_oge_out_alpha_from out_alpha_from; /* Source of the output alpha */

    ot_u32 color_resize; /* Colorize value */

    ot_oge_blend_opt blend_opt; /* Options of the blending operation */

    ot_oge_csc_opt csc_opt; /* CSC parameter option */
} ot_oge_pattern_fill_opt;

/* Definition of rotation directions */
typedef enum {
    OT_OGE_ROTATE_CLOCKWISE_90 = 0, /* Rotate 90 degree clockwise */
    OT_OGE_ROTATE_CLOCKWISE_180,    /* Rotate 180 degree clockwise */
    OT_OGE_ROTATE_CLOCKWISE_270,    /* Rotate 270 degree clockwise */
    OT_OGE_ROTATE_MAX
} ot_oge_rotate_angle;

/* Definition of corner_rect */
typedef struct {
    ot_u32 width;
    ot_u32 height;
    ot_u32 inner_color;
    ot_u32 outer_color;
} ot_oge_corner_rect_info;

typedef struct {
    ot_oge_rect *corner_rect_region;
    ot_oge_corner_rect_info *corner_rect_info;
} ot_oge_corner_rect;

typedef struct {
    ot_s32 start_x;
    ot_s32 start_y;
    ot_s32 end_x;
    ot_s32 end_y;
    ot_u32 thick;
    ot_u32 color;
} ot_oge_line;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* OT_OGE_COMMON_H */
