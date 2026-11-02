/******************************************************************************
 Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : vo data type
Created       : 2023/5/31
Last Modified : 2023/5/31
******************************************************************************/
#ifndef OT_EIS_VO_TYPE
#define OT_EIS_VO_TYPE

#include "ot_eis_video.h"
#include "ot_eis_type.h"
#include "ot_eis_vo_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define OT_EIS_VO_GAMMA_PARA_NUM 257 /* gamma 257 parameters */

/**
 * @brief port类型枚举
 *
 */
typedef enum {
    OT_EIS_VO_PORT_USER = 0,
    OT_EIS_VO_PORT_PIPE,
    OT_EIS_VO_PORT_INVALID
} ot_eis_vo_port_type;

/**
 * @brief surface类型枚举
 *
 */
typedef enum {
    OT_EIS_VO_VIDEO_SURFACE_0 = 0,

    OT_EIS_VO_GRAPHIC_SURFACE_0 = 0x100,
    OT_EIS_VO_SURFACE_INVALID
} ot_eis_vo_surface;

/**
 * @brief 旋转角度枚举
 *
 */
typedef enum {
    OT_EIS_VO_ROTATION_0 = 0,
    OT_EIS_VO_ROTATION_90,
    OT_EIS_VO_ROTATION_180,
    OT_EIS_VO_ROTATION_270,
    OT_EIS_VO_ROTATION_INVALID
} ot_eis_vo_rotation_type;

typedef enum {
    OT_EIS_VO_MIRROR_X = 0,
    OT_EIS_VO_MIRROR_Y,
    OT_EIS_VO_MIRROR_XY,
    OT_EIS_VO_MIRROR_INVALID
} ot_eis_vo_mirror_type;


typedef enum {
    OT_EIS_VO_DISPLAY_0,
    OT_EIS_VO_DISPLAY_INVALID
} ot_eis_vo_display;

typedef enum {
    OT_EIS_VO_INTF_MIPI_0,
    OT_EIS_VO_INTF_BT656,
    OT_EIS_VO_INTF_BT1120,
    OT_EIS_VO_INTF_RGB_6BIT,
    OT_EIS_VO_INTF_RGB_8BIT,
    OT_EIS_VO_INTF_RGB_16BIT,
    OT_EIS_VO_INTF_RGB_18BIT,
    OT_EIS_VO_INTF_RGB_24BIT,
    OT_EIS_VO_INTF_INVALID
} ot_eis_vo_interface;

typedef enum {
    OT_EIS_VO_DISPLAY_320_480_60 = 0,
    OT_EIS_VO_DISPLAY_480_800_60,
    OT_EIS_VO_DISPLAY_480_864_60,
    OT_EIS_VO_DISPLAY_640_480_60,
    OT_EIS_VO_DISPLAY_720_480_60,
    OT_EIS_VO_DISPLAY_800_480_60,
    OT_EIS_VO_DISPLAY_1024_600_60,
    OT_EIS_VO_DISPLAY_1024_768_60,
    OT_EIS_VO_DISPLAY_1280_720_60,
    OT_EIS_VO_DISPLAY_1280_800_60,
    OT_EIS_VO_DISPLAY_1920_1080_30,
    OT_EIS_VO_DISPLAY_1920_1080_60,
    OT_EIS_VO_DISPLAY_3840_2160_60,
    OT_EIS_VO_DISPLAY_CUSTOMIZED,
    OT_EIS_VO_DISPLAY_SIZE_INVALID
} ot_eis_vo_display_size_type;

typedef enum {
    OT_EIS_VO_PRIORITY_0,
    OT_EIS_VO_PRIORITY_1,
    OT_EIS_VO_PRIORITY_INVALID
} ot_eis_vo_surface_order;

typedef struct {
    ot_eis_img_pixel_format format;
    ot_eis_img_layout layout;
    ot_u32 width;
    ot_u32 height;
} ot_eis_vo_pixel_attr;

/**
 * @brief vo port属性结构体
 *
 */
typedef struct {
    ot_eis_vo_pixel_attr pixel_attr;
} ot_eis_vo_in_port_attr;


typedef enum {
    OT_EIS_VO_CSC_MATRIX_BT601LIMIT_TO_BT601LIMIT = 0, /* Identity matrix.   from BT.601 limit to BT.601 limit */
    OT_EIS_VO_CSC_MATRIX_BT601FULL_TO_BT601LIMIT = 1,  /* Change color space from BT.601 full to BT.601 limit */
    OT_EIS_VO_CSC_MATRIX_BT709LIMIT_TO_BT601LIMIT = 2, /* Change color space from BT.709 limit to BT.601 limit */
    OT_EIS_VO_CSC_MATRIX_BT709FULL_TO_BT601LIMIT = 3,  /* Change color space from BT.709 full to BT.601 limit */

    OT_EIS_VO_CSC_MATRIX_BT601LIMIT_TO_BT709LIMIT = 4, /* Change color space from BT.601 limit to BT.709 limit */
    OT_EIS_VO_CSC_MATRIX_BT601FULL_TO_BT709LIMIT = 5,  /* Change color space from BT.601 full to BT.709 limit */
    OT_EIS_VO_CSC_MATRIX_BT709LIMIT_TO_BT709LIMIT = 6, /* Identity matrix.   from BT.709 limit to BT.709 limit */
    OT_EIS_VO_CSC_MATRIX_BT709FULL_TO_BT709LIMIT = 7,  /* Change color space from BT.709 full to BT.709 limit */

    OT_EIS_VO_CSC_MATRIX_BT601LIMIT_TO_BT601FULL = 8,  /* Change color space from BT.601 limit to BT.601 full */
    OT_EIS_VO_CSC_MATRIX_BT601FULL_TO_BT601FULL = 9,   /* Identity matrix.   from BT.601 full to BT.601 full */
    OT_EIS_VO_CSC_MATRIX_BT709LIMIT_TO_BT601FULL = 10,  /* Change color space from BT.709 limit to BT.601 full */
    OT_EIS_VO_CSC_MATRIX_BT709FULL_TO_BT601FULL = 11,   /* Change color space from BT.709 full to BT.601 full */

    OT_EIS_VO_CSC_MATRIX_BT601LIMIT_TO_BT709FULL = 12,  /* Change color space from BT.601 limit to BT.709 full */
    OT_EIS_VO_CSC_MATRIX_BT601FULL_TO_BT709FULL = 13,   /* Change color space from BT.601 full to BT.709 full */
    OT_EIS_VO_CSC_MATRIX_BT709LIMIT_TO_BT709FULL = 14,  /* Change color space from BT.709 limit to BT.709 full */
    OT_EIS_VO_CSC_MATRIX_BT709FULL_TO_BT709FULL = 15,   /* Identity matrix.   from BT.709 full to BT.709 full */

    OT_EIS_VO_CSC_MATRIX_BT601LIMIT_TO_RGBFULL = 16,    /* Change color space from BT.601 limit to RGB full */
    OT_EIS_VO_CSC_MATRIX_BT601FULL_TO_RGBFULL = 17,     /* Change color space from BT.601 full to RGB full */
    OT_EIS_VO_CSC_MATRIX_BT709LIMIT_TO_RGBFULL = 18,    /* Change color space from BT.709 limit to RGB full */
    OT_EIS_VO_CSC_MATRIX_BT709FULL_TO_RGBFULL = 19,     /* Change color space from BT.709 full to RGB full */

    OT_EIS_VO_CSC_MATRIX_BT601LIMIT_TO_RGBLIMIT = 20,   /* Change color space from BT.601 limit to RGB limit */
    OT_EIS_VO_CSC_MATRIX_BT601FULL_TO_RGBLIMIT = 21,    /* Change color space from BT.709 full to RGB limit */
    OT_EIS_VO_CSC_MATRIX_BT709LIMIT_TO_RGBLIMIT = 22,   /* Change color space from BT.601 limit to RGB limit */
    OT_EIS_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT = 23,    /* Change color space from BT.709 full to RGB limit */

    OT_EIS_VO_CSC_MATRIX_RGBFULL_TO_BT601LIMIT = 24,    /* Change color space from RGB full to BT.601 limit */
    OT_EIS_VO_CSC_MATRIX_RGBFULL_TO_BT601FULL = 25,     /* Change color space from RGB full to BT.601 full */
    OT_EIS_VO_CSC_MATRIX_RGBFULL_TO_BT709LIMIT = 26,    /* Change color space from RGB full to BT.709 limit */
    OT_EIS_VO_CSC_MATRIX_RGBFULL_TO_BT709FULL = 27,     /* Change color space from RGB full to BT.709 full */

    OT_EIS_VO_CSC_MATRIX_INVALID,
} ot_eis_vo_csc_matrix;

typedef struct {
    ot_eis_vo_csc_matrix csc_matrix; /* RW; CSC matrix */
    ot_u32 luma; /* RW; range: [0, 100]; luminance, default: 50 */
    ot_u32 contrast; /* RW; range: [0, 100]; contrast, default: 50 */
    ot_u32 hue; /* RW; range: [0, 100]; hue, default: 50 */
    ot_u32 saturation; /* RW; range: [0, 100]; saturation, default: 50 */
    ot_bool ex_csc_en; /* external luminance enable */
} ot_eis_vo_csc_attr;

/**
 * @brief vo 显示位置信息结构体
 *
 */
typedef struct{
    ot_u32 x;
    ot_u32 y;
    ot_u32 w;
    ot_u32 h;
} ot_eis_vo_rect;

typedef struct {
    ot_bool enable;

    ot_u32 width;
    ot_u32 height;
} ot_eis_vo_scale_attr;

typedef struct {
    ot_bool enable;
    ot_eis_vo_rect rect;
} ot_eis_vo_crop_attr;

typedef struct {
    ot_bool enable;
    ot_eis_vo_rotation_type type;
} ot_eis_vo_rot_attr;

typedef struct {
    ot_bool enable;
    ot_eis_vo_mirror_type type;
} ot_eis_vo_mirror_attr;

/**
 * @brief vo surface属性结构体
 *
 */
typedef struct {
    ot_u32 width;
    ot_u32 height;
    ot_u32 buf_len;
    ot_bool early_disp;
} ot_eis_vo_surface_attr;

typedef struct {
    ot_u32 priority;
    ot_u32 x;
    ot_u32 y;
    ot_u32 width;
    ot_u32 height;
    ot_bool show;
} ot_eis_vo_surface_bind_attr;

typedef struct {
    ot_u32 x;
    ot_u32 y;
    ot_eis_vo_surface_order order;
} ot_eis_vo_display_bind_attr;

/**
 * @brief rgb颜色值标识
 *
 */
typedef struct {
    ot_u8 a;
    ot_u8 r;
    ot_u8 g;
    ot_u8 b;
} ot_eis_vo_color;


typedef enum {
    OT_EIS_VO_CLK_SRC_PLL = 0,       /* Clock source type PLL */
    OT_EIS_VO_CLK_SRC_LCDMCLK = 1,   /* Clock source type LCDMCLK */
    OT_EIS_VO_CLK_SRC_PLL_FOUT4 = 2, /* Clock source type PLL FOUT4 */
    OT_EIS_VO_CLK_SRC_FIXED = 3,     /* Clock source type FIXED */

    OT_EIS_VO_CLK_SRC_INVALID,
} ot_eis_vo_clk_src;

typedef enum {
    OT_EIS_VO_FIXED_CLK_297M    = 0, /* Fixed clock source 297MHz */
    OT_EIS_VO_FIXED_CLK_148_5M  = 1, /* Fixed clock source 148.5MHz */
    OT_EIS_VO_FIXED_CLK_135M    = 2, /* Fixed clock source 135MHz */
    OT_EIS_VO_FIXED_CLK_108M    = 3, /* Fixed clock source 108MHz */
    OT_EIS_VO_FIXED_CLK_88M     = 4, /* Fixed clock source 88MHz */
    OT_EIS_VO_FIXED_CLK_74_25M  = 5, /* Fixed clock source 74.25MHz */
    OT_EIS_VO_FIXED_CLK_66M     = 6, /* Fixed clock source 66MHz */
    OT_EIS_VO_FIXED_CLK_INVALID,
} ot_eis_vo_fixed_clk;


typedef struct {
    ot_u32 fb_div;    /* RW, range: [0, 0xfff];  frequency double division */
    ot_u32 frac;      /* RW, range: [0, 0xffffff]; fractional division */
    ot_u32 ref_div;   /* RW, range: (0, 0x3f]; reference clock division */
    ot_u32 post_div1; /* RW, range: (0, 0x7]; level 1 post division */
    ot_u32 post_div2; /* RW, range: (0, 0x7]; level 2 post division */
} ot_eis_vo_pll;


typedef struct {
    ot_eis_vo_clk_src clk_src;
    union {
        ot_eis_vo_pll vo_pll;
        ot_u32 lcd_m_clk_div;
        ot_eis_vo_fixed_clk fixed_clk;
    };
} ot_eis_vo_user_sync_attr;

typedef struct {
    ot_u32 pixel_clk; /* RW, pixel clock(Hz) */
} ot_eis_vo_auto_user_sync_info;

typedef struct {
    ot_eis_vo_user_sync_attr user_sync_attr; /* RW; user synchronization timing attribute */
    ot_u32 pre_div;                      /* RW, range: [1, 32]; device previous division */
    ot_u32 dev_div;                      /* RW, range: [1, 4]; device clock division */
} ot_eis_vo_manual_user_sync_info;

typedef struct {
    ot_bool clk_reverse_en;
    ot_eis_op_mode op_mode;
    union {
        /*
         * RW; auto user synchronization info
         * AUTO:ot_op_mode:OT_OP_MODE_AUTO;
         */
        ot_eis_vo_auto_user_sync_info auto_user_sync_info;
        /*
         * RW; manual user synchronization info
         * AUTO:ot_op_mode:OT_OP_MODE_MANUAL;
         */
        ot_eis_vo_manual_user_sync_info manual_user_sync_info;
    };
                  /* RW, range: [0, 1]; whether to reverse clock  */
} ot_eis_vo_user_sync_info;

typedef struct {
    ot_bool syncm; /* RW; sync mode(0:timing,as BT.656; 1:signal,as LCD) */
    ot_bool iop; /* RW; interlaced or progressive display(0:i; 1:p) */
    ot_u8 intfb; /* RW; interlaced bit width while output */

    ot_u16 vact; /* RW; vertical active area */
    ot_u16 vbb; /* RW; vertical back blank porch */
    ot_u16 vfb; /* RW; vertical front blank porch */

    ot_u16 hact; /* RW; horizontal active area */
    ot_u16 hbb; /* RW; horizontal back blank porch */
    ot_u16 hfb; /* RW; horizontal front blank porch */
    ot_u16 hmid; /* RW; bottom horizontal active area */

    ot_u16 bvact; /* RW; bottom vertical active area */
    ot_u16 bvbb; /* RW; bottom vertical back blank porch */
    ot_u16 bvfb; /* RW; bottom vertical front blank porch */

    ot_u16 hpw; /* RW; horizontal pulse width */
    ot_u16 vpw; /* RW; vertical pulse width */

    ot_bool idv; /* RW; inverse data valid of output */
    ot_bool ihs; /* RW; inverse horizontal synchronization signal */
    ot_bool ivs; /* RW; inverse vertical synchronization signal */
} ot_eis_vo_sync_info;

typedef struct {
    ot_eis_vo_display_size_type type;
    ot_u32 frame_rate;
} ot_eis_vo_display_size_attr;

typedef struct {
    ot_u32 bg_color;
    ot_eis_vo_display_size_attr size;
    ot_eis_vo_interface interface;
    ot_eis_vo_sync_info sync_info;
    ot_eis_vo_user_sync_info clk_info;
    ot_u32 vtth;
} ot_eis_vo_display_attr;


typedef struct {
    ot_bool enable;
    ot_u32 value;
} ot_eis_vo_colorkey_attr;

typedef struct {
    ot_bool enable;  //alpha_en
    ot_bool alpha_chn_en;  /*  global alpha enable flag */
    ot_u8 alpha0;         /*  alpha0 value, used in ARGB1555 */
    ot_u8 alpha1;         /*  alpha1 value, used in ARGB1555 */
    ot_u8 global_alpha;    /*  global alpha value */
    ot_u8 reserved;
} ot_eis_vo_alpha_blending_attr;

typedef enum {
    OT_EIS_VO_DUMMY_ORDER_DUMMY_RGB = 0, /* if serial perd is four, send order is DRGB */
    OT_EIS_VO_DUMMY_ORDER_RGB_DUMMY = 1, /* if serial perd is four, send order is RGBD */
    OT_EIS_VO_DUMMY_ORDER_INVALID,
} ot_eis_vo_rgb_dummy_order;

typedef struct {
    ot_bool gamma_enable; /* RW: gamma enable */
    ot_u16 gamma_para[OT_EIS_VO_GAMMA_PARA_NUM]; /* RW; range */
} ot_eis_vo_gamma_info;

typedef struct {
    ot_eis_vo_csc_attr csc; /* RW, color space */
    ot_bool rgb_inverted_en; /* RW; component r g b inverted, rgb to bgr, default: 0, rgb */
    ot_bool bit_inverted_en; /* RW; data's bit inverted,
                                rgb6bit: bit[5:0] to bit[0:5],
                                rgb8bit: bit[7:0] to bit[0:7],
                                rgb16bit: bit[15:0] to bit[0:15],
                                rgb18bit: bit[17:0] to bit[0:17],
                                rgb24bit: bit[23:0] to bit[0:23],
                                default: 0, bit[5/7/15/17/23:0] */
    ot_bool rgb_dummy_out_en; /* RW; rgb dummy output enable */
    ot_eis_vo_rgb_dummy_order rgb_dummy_order; /* RW; rgb dummy order */
    ot_eis_vo_gamma_info gamma_info; /* RW; gamma info */
} ot_eis_vo_rgb_param;

typedef enum {
    OT_EIS_VO_CLK_EDGE_SINGLE = 0,  /* single-edge mode */
    OT_EIS_VO_CLK_EDGE_DUAL,        /* dual-edge mode */
    OT_EIS_VO_CLK_EDGE_INVALID
} ot_eis_vo_clk_edge;

typedef struct {
    ot_bool yc_inverted_en; /* RW; component y c inverted, yc to cy, default: 0, yc */
    ot_bool bit_inverted_en; /* RW; data's bit inverted, bt.656: bit[7:0] to bit[0:7],
                                bt.1120: bit[15:0] to bit[0:15],
                                default: 0, bit[7:0] or bit[15:0] */
    ot_eis_vo_clk_edge clk_edge; /* RW; clk edge */
} ot_eis_vo_bt_param;

typedef struct {
    ot_eis_vo_csc_attr csc; /* RW; color space */
    ot_eis_vo_gamma_info gamma_info; /* RW; gamma info */
} ot_eis_vo_mipi_param;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif