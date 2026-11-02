/******************************************************************************
 Copyright (C), 2022 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : the application interface struct and macro code etc.
Created       : 2022/11/16
Last Modified :
******************************************************************************/
#ifndef OT_AVP_CVE_COMMON_H
#define OT_AVP_CVE_COMMON_H

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef ot_avp_handle
typedef void* ot_avp_handle;
#endif

#define OT_AVP_CVE_DEFAULT_ALIGN            16
#define OT_AVP_CVE_MAX_TIMEOUT              60000       // ms
#define OT_AVP_CVE_MAX_WIDTH                4096
#define OT_AVP_CVE_MAX_HEIGHT               4096
#define OT_AVP_CVE_MIN_WIDTH                16
#define OT_AVP_CVE_MIN_HEIGHT               16

#define OT_AVP_CVE_IMG_MAX_CHN_NUM          3      // max addr channel num
#define OT_AVP_CVE_MAX_COEF_NUM             25
#define OT_AVP_CVE_ST_MAX_CORNER_NUM        2500
#define OT_AVP_CVE_ST_MAX_ROI_NUM           32
#define OT_AVP_CVE_MAX_PYR_LEVEL_NUM        9
#define OT_MAX_FRAME_BUFF_PART          3
#define OT_AVP_CVE_HIST_NUM 256
#define OT_AVP_CVE_MAP_NUM 256

typedef enum {
    OT_AVP_CVE_INVOKE_SYNC,             /* 同步模式 */
    OT_AVP_CVE_INVOKE_TRIGGER,          /* 异步模式, 不关心该任务是否完成 */
    OT_AVP_CVE_INVOKE_TRIGGER_AND_WAIT, /* 异步模式, 希望及时得到该任务完成的信息, 需配合wait接口使用 */
} ot_avp_cve_invoke_mode;

typedef enum {
    OT_IMAGE_FORMAT_BAYER_8BPP = 0,
    OT_IMAGE_FORMAT_BAYER_10BPP,
    OT_IMAGE_FORMAT_BAYER_12BPP,
    OT_IMAGE_FORMAT_BAYER_14BPP,
    OT_IMAGE_FORMAT_BAYER_16BPP,
    OT_IMAGE_FORMAT_BAYER_24BPP,

    OT_IMAGE_FORMAT_RGB_444 = 20,
    OT_IMAGE_FORMAT_RGB_555,
    OT_IMAGE_FORMAT_RGB_565,
    OT_IMAGE_FORMAT_RGB_888,
    OT_IMAGE_FORMAT_RGB_888_PLANAR,
    OT_IMAGE_FORMAT_RGB_101010,

    OT_IMAGE_FORMAT_BGR_444,
    OT_IMAGE_FORMAT_BGR_555,
    OT_IMAGE_FORMAT_BGR_565,
    OT_IMAGE_FORMAT_BGR_888,
    OT_IMAGE_FORMAT_BGR_888_PLANAR,
    OT_IMAGE_FORMAT_BGR_101010,

    OT_IMAGE_FORMAT_GBR_888,
    OT_IMAGE_FORMAT_BRG_888,
    OT_IMAGE_FORMAT_GRB_888,

    OT_IMAGE_FORMAT_ARGB_1555 = 60,
    OT_IMAGE_FORMAT_ARGB_4444,
    OT_IMAGE_FORMAT_ARGB_8888,
    OT_IMAGE_FORMAT_ARGB_2101010,

    OT_IMAGE_FORMAT_ABGR_1555,
    OT_IMAGE_FORMAT_ABGR_4444,
    OT_IMAGE_FORMAT_ABGR_8888,

    OT_IMAGE_FORMAT_RGBA_1555,
    OT_IMAGE_FORMAT_RGBA_4444,
    OT_IMAGE_FORMAT_RGBA_8888,

    OT_IMAGE_FORMAT_BGRA_1555,
    OT_IMAGE_FORMAT_BGRA_4444,
    OT_IMAGE_FORMAT_BGRA_8888,

    OT_IMAGE_FORMAT_XRGB_1555 = 100,
    OT_IMAGE_FORMAT_XRGB_4444,
    OT_IMAGE_FORMAT_XRGB_8888,

    OT_IMAGE_FORMAT_XBGR_1555,
    OT_IMAGE_FORMAT_XBGR_4444,
    OT_IMAGE_FORMAT_XBGR_8888,

    OT_IMAGE_FORMAT_BGRX_1555,
    OT_IMAGE_FORMAT_BGRX_4444,
    OT_IMAGE_FORMAT_BGRX_8888,

    OT_IMAGE_FORMAT_RGBX_1555,
    OT_IMAGE_FORMAT_RGBX_4444,
    OT_IMAGE_FORMAT_RGBX_8888,

    OT_IMAGE_FORMAT_YUV_444 = 140,
    OT_IMAGE_FORMAT_YUV_444_PLANAR,

    OT_IMAGE_FORMAT_YUV_422_PLANAR  = 160,
    OT_IMAGE_FORMAT_YUV_422_SEMIPLANAR,
    OT_IMAGE_FORMAT_YUV_422_PACKED_YUYV,
    OT_IMAGE_FORMAT_YUV_422_PACKED_YVYU,
    OT_IMAGE_FORMAT_YUV_422_PACKED_YYUV,
    OT_IMAGE_FORMAT_YUV_422_PACKED_YYVU,
    OT_IMAGE_FORMAT_YUV_422_PACKED_UYVY,
    OT_IMAGE_FORMAT_YUV_422_PACKED_UVYY,
    OT_IMAGE_FORMAT_YUV_422_PACKED_VYUY,
    OT_IMAGE_FORMAT_YUV_422_PACKED_VUYY,

    OT_IMAGE_FORMAT_YUV_422_PACKED_Y1UY0V,
    OT_IMAGE_FORMAT_YUV_422_PACKED_Y1VY0U,
    OT_IMAGE_FORMAT_YUV_422_PACKED_UY1VY0,
    OT_IMAGE_FORMAT_YUV_422_PACKED_VY1UY0,
    OT_IMAGE_FORMAT_YUV_422_PACKED_Y1Y0UV,
    OT_IMAGE_FORMAT_YUV_422_PACKED_Y1Y0VU,
    OT_IMAGE_FORMAT_YUV_422_PACKED_UVY1Y0,
    OT_IMAGE_FORMAT_YUV_422_PACKED_VUY1Y0,

    OT_IMAGE_FORMAT_YUV_420_PLANAR = 180,
    OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR,
    OT_IMAGE_FORMAT_YUV_420_PLANAR_10BIT,
    OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR_10BIT,
    OT_IMAGE_FORMAT_YUV_420_PLANAR_10BIT_PACKED,

    OT_IMAGE_FORMAT_YVU_422_PLANAR = 200,
    OT_IMAGE_FORMAT_YVU_422_SEMIPLANAR,

    OT_IMAGE_FORMAT_YVU_420_PLANAR = 220,
    OT_IMAGE_FORMAT_YVU_420_SEMIPLANAR,

    OT_IMAGE_FORMAT_YUV_400 = 240,

    OT_IMAGE_FORMAT_U8C1 = 260,
    OT_IMAGE_FORMAT_U8C2_PLANAR,
    OT_IMAGE_FORMAT_U8C2_PACKED,
    OT_IMAGE_FORMAT_U8C3_PLANAR,
    OT_IMAGE_FORMAT_U8C3_PACKED,

    OT_IMAGE_FORMAT_S8C1,
    OT_IMAGE_FORMAT_S8C2_PLANAR,
    OT_IMAGE_FORMAT_S8C2_PACKED,
    OT_IMAGE_FORMAT_S8C3_PLANAR,
    OT_IMAGE_FORMAT_S8C3_PACKED,

    OT_IMAGE_FORMAT_U16C1,
    OT_IMAGE_FORMAT_S16C1,
    OT_IMAGE_FORMAT_U32C1,
    OT_IMAGE_FORMAT_S32C1,
    OT_IMAGE_FORMAT_U64C1,
    OT_IMAGE_FORMAT_S64C1,

    OT_IMAGE_FORMAT_BITMAP = 280,

    OT_IMAGE_FORMAT_BUTT
} ot_image_pixel_format;

typedef struct {
    ot_u64 phys_addr;  /* the physical addr of memory */
    ot_u64 virt_addr;  /* the virtual addr of memory */
    ot_u32 size;       /* the size of memory by byte */
} ot_mem_info;

typedef struct {
    ot_u32 width;
    ot_u32 height;
} ot_avp_cve_size;

typedef ot_mem_info ot_avp_cve_mem_1d;          /* 1D data */
typedef ot_mem_info ot_avp_cve_src_mem_1d;      /* 1D data */
typedef ot_mem_info ot_avp_cve_dst_mem_1d;      /* 1D data */

typedef ot_mem_info ot_avp_cve_hist_data;       /* hist data */
typedef ot_mem_info ot_avp_cve_dst_hist_data;   /* hist data */
typedef ot_mem_info ot_avp_cve_dst_hist_data;   /* hist data */

typedef ot_mem_info ot_avp_cve_lut_data;        /* lut data */
typedef ot_mem_info ot_avp_cve_src_lut_data;    /* lut data */
typedef ot_mem_info ot_avp_cve_dst_lut_data;    /* lut data */

typedef ot_mem_info ot_src_mem_info;
typedef ot_mem_info ot_dst_mem_info;


typedef struct {
    ot_u64 phys_addr;  /* the physical addr of the data */
    ot_u64 virt_addr;  /* the virtual addr of the data */
    ot_u32 stride;     /* the stride of 2D data by byte */
    ot_u16 width;      /* the width of 2D data by byte */
    ot_u16 height;     /* the height of 2D data by byte */
    ot_u32 capacity;
} ot_data_info;

typedef ot_data_info ot_avp_cve_src_mem_2d;  /* 2D data */
typedef ot_data_info ot_avp_cve_dst_mem_2d;  /* 2D data */
typedef ot_data_info ot_avp_cve_mem_2d;      /* 2D data */

/* image broder type, do not change the value */
typedef enum {
    OT_IMG_BORDER_CONSTANT  = 0,   /* iiiiii  | abcdefgh | iiiiii  with specified  'i' */
    OT_IMG_BORDER_EDGE = 1,
    OT_IMG_BORDER_BUTT
} ot_img_border_type;

/* image data */
typedef struct {
    ot_u64 phys_addr[OT_AVP_CVE_IMG_MAX_CHN_NUM]; /* the physical addr of image */
    ot_u64 virt_addr[OT_AVP_CVE_IMG_MAX_CHN_NUM]; /* the virtual addr of image */
    ot_u32 stride[OT_AVP_CVE_IMG_MAX_CHN_NUM];    /* the stride of image */
    ot_u16 width;                          /* the width of image */
    ot_u16 height;                         /* the height of image */
    ot_image_pixel_format type;                  /* the type of image */
    ot_u32 capacity;                       /* the memory size of image */
} ot_avp_cve_img;

typedef ot_avp_cve_img ot_avp_cve_src_img;  /* image data */
typedef ot_avp_cve_img ot_avp_cve_dst_img;  /* image data */

typedef enum {
    OT_AVP_CVE_DMA_2D_MODE_DIRECT_CP = 0,
    OT_AVP_CVE_DMA_2D_MODE_MASK_H24V24_CP,
    OT_AVP_CVE_DMA_2D_MODE_MASK_H24V32_CP,
    OT_AVP_CVE_DMA_2D_MODE_MASK_H32V24_CP,
    OT_AVP_CVE_DMA_2D_MODE_MASK_H32V32_CP,
    OT_AVP_CVE_DMA_2D_MODE_3BYTE_SET,
    OT_AVP_CVE_DMA_2D_MODE_8BYTE_SET,
    OT_AVP_CVE_DMA_2D_MODE_BUTT
} ot_avp_cve_dma_2d_mode;

typedef struct {
    ot_avp_cve_dma_2d_mode mode;
    ot_u32 h_mask;                 /* h_mask val, only used for mask mode */
    ot_u32 v_mask;                 /* v_mask val, only used for mask mode */
    ot_u64 val;                    /* set val, only used for set mode */
} ot_avp_cve_dma_2d_ctrl;

typedef enum {
    OT_KERNEL_SIZE_MODE_3x3 = 3,
    OT_KERNEL_SIZE_MODE_5x5 = 5,
    OT_KERNEL_SIZE_MODE_7x7 = 7,
    OT_KERNEL_SIZE_MODE_9x9 = 9,
    OT_KERNEL_SIZE_MODE_11x11 = 11,
    OT_KERNEL_SIZE_MODE_BUTT
} ot_kernel_size_mode;

/* 卷积核相关参数 */
typedef struct {
    ot_kernel_size_mode ksize_mode;     /* 卷积核类型 */
    ot_s16 *coef;                       /* 卷积核系数 */
} ot_img_kernel_attr;

/* 边界填充相关参数 */
typedef struct {
    ot_img_border_type pad_mode;            /* 边界填充模式 */
    ot_u8 pad_val;                          /* 只应用于OT_IMG_BORDER_CONSTANT模式 */
} ot_img_border_attr;

/* 归一化参数  */
typedef struct {
    ot_s16 scale;   /* used for normalization */
} ot_img_norm_attr;

typedef struct {
    ot_img_norm_attr norm_attr;
    ot_img_kernel_attr kernel_attr;
    ot_img_border_attr border_attr;
} ot_avp_cve_filter_ctrl;

/* soble输出模式 */
typedef enum {
    OT_AVP_CVE_SOBEL_OUT_BOTH,
    OT_AVP_CVE_SOBEL_OUT_SINGLE,
} ot_avp_cve_sobel_out_mode;

typedef struct {
    ot_avp_cve_sobel_out_mode out_mode;
    ot_img_kernel_attr kernel_attr;
} ot_avp_cve_sobel_ctrl;

typedef enum {
    OT_AVP_CVE_OUI_MAG_ANG_BOTH,
    OT_AVP_CVE_OUT_MAG_ONLY,
    OT_AVP_CVE_OUT_BUTT,
} ot_avp_cve_mag_ang_out_mode;

typedef struct {
    ot_avp_cve_mag_ang_out_mode out_mode;
    ot_u16 thresh;
    ot_img_kernel_attr kernel_attr;
} ot_avp_cve_mag_and_ang_ctrl;

// do not change ot_avp_cve_thresh_mode order
typedef enum {
    OT_AVP_CVE_THRESH_BINARY = 0,   // 二进制阈值化
    OT_AVP_CVE_THRESH_TURNC,        // 截断阈值化
    OT_AVP_CVE_THRESH_TO_MIN,       // 阈值化为min_val
    OT_AVP_CVE_THRESH_MIN_MID_MAX,  // 分段阈值化
    OT_AVP_CVE_THRESH_ORI_MID_MAX,  // 分段阈值化
    OT_AVP_CVE_THRESH_MIN_MID_ORI,  // 分段阈值化
    OT_AVP_CVE_THRESH_MIN_ORI_MAX,  // 分段阈值化
    OT_AVP_CVE_THRESH_ORI_MID_ORI,  // 分段阈值化

    OT_AVP_CVE_THRESH_U16_TERNARY = 9,
    OT_AVP_CVE_THRESH_U16_CLIP,

    OT_AVP_CVE_THRESH_S16_TERNARY_U8 = 19,
    OT_AVP_CVE_THRESH_S16_TERNARY_S8,
    OT_AVP_CVE_THRESH_S16_CLIP_U8,
    OT_AVP_CVE_TRHESH_S16_CLIP_S8,

    OT_AVP_CVE_THRESH_BUTT
} ot_avp_cve_thresh_mode;

typedef struct {
    ot_avp_cve_thresh_mode mode;

    ot_s32 low_thr;
    ot_s32 high_thr;
    /* min, mid, max_val 不需要满足变量命名中的大小关系。只代表分段阈值化的三段值 */
    ot_s16 min_val;
    ot_s16 mid_val;
    ot_s16 max_val;
} ot_avp_cve_thresh_ctrl;

typedef enum {
    OT_AVP_CVE_MORPH_DILATE,
    OT_AVP_CVE_MORPH_ERODE,
    OT_AVP_CVE_MORPH_MODE_BUTT
} ot_avp_cve_morph_mode;

typedef struct {
    ot_avp_cve_morph_mode morph_mode;
    ot_kernel_size_mode ksize_mode;
    ot_s16 *coef0;
} ot_avp_cve_morph_ctrl;

typedef struct {
    ot_avp_cve_img src1;
    ot_avp_cve_img src2;
} ot_avp_cve_add_src_img;

typedef struct {
    ot_s16 x; /* weight: x of xA+yB */
    ot_s16 y; /* weight: y of xA+yB */
} ot_avp_cve_add_ctrl;

typedef enum {
    OT_AVP_CVE_SUB_MODE_SHIFT,
    OT_AVP_CVE_SUB_MODE_SHIFT_ABS,
} ot_avp_cve_sub_mode;


typedef struct {
    ot_avp_cve_sub_mode sub_mode;
} ot_avp_cve_sub_ctrl;

typedef enum {
    OT_AVP_CVE_INTEG_SUM,
    OT_AVP_CVE_INTEG_SQSUM,
} ot_avp_cve_integ_calmode;


typedef struct {
    ot_avp_cve_integ_calmode cal_mode;
} ot_avp_cve_integ_ctrl;

typedef struct {
    ot_kernel_size_mode ksize_mode;
} ot_avp_cve_median_filter_ctrl;

typedef enum {
    OT_AVP_CVE_MIN_FILTER,
    OT_AVP_CVE_MAX_FILTER,
} ot_avp_cve_min_max_filter_mode;

typedef struct {
    ot_avp_cve_min_max_filter_mode mode;
    ot_img_kernel_attr kernel_attr;
} ot_avp_cve_min_max_filter_ctrl;

/* do not change the order */
typedef enum {
    OT_AVP_CVE_SAD_BLOCK_4X4,
    OT_AVP_CVE_SAD_BLOCK_8X8,
    OT_AVP_CVE_SAD_BLOCK_16X16,
    OT_AVP_CVE_SAD_BLOCK_BUTT
} ot_avp_cve_sad_block_mode;

typedef enum {
    OT_AVP_CVE_SAD_OUT_BOTH,
    OT_AVP_CVE_SAD_OUT_SAD_ONLY,
    OT_AVP_CVE_SAD_OUT_THRESH_ONLY,
    OT_AVP_CVE_SAD_OUT_BUTT
} ot_avp_cve_sad_out_mode;

typedef struct {
    ot_avp_cve_sad_out_mode out_mode;
    ot_avp_cve_sad_block_mode block_mode;
    ot_u16 thresh_val;
    ot_u8 max_val;
    ot_u8 min_val;
} ot_avp_cve_sad_ctrl;

typedef enum {
    OT_AVP_CVE_RESIZE_MODE_BILINEAR, /* 双线性插值模式 */
    OT_AVP_CVE_RESIZE_MODE_BUTT
} ot_avp_cve_resize_mode;

typedef struct {
    ot_avp_cve_resize_mode alg_mode;
    ot_u16 batch_num;           /* 批量处理的图像数量 */
    ot_mem_info mem;            /* 辅助内存, mem->size = batch_num * 48 */
} ot_avp_cve_resize_ctrl;

typedef struct {
    ot_avp_cve_lut_data lut;     /* size = 256 */
} ot_avp_cve_equalize_hist_ctrl;

typedef struct {
    ot_u32 hist[OT_AVP_CVE_HIST_NUM];
    ot_u8 map[OT_AVP_CVE_MAP_NUM];
} ot_avp_cve_equalize_hist_lut_mem;


typedef enum {

    OT_CSC_BT601_RGB_FR2YCbCr_LR, // 输出模式：LR：Limite Range
    OT_CSC_BT601_RGB_LR2YCbCr_FR,
    OT_CSC_BT601_YCbCr_FR2RGB_FR,
    OT_CSC_BT601_YCbCr_LR2RGB_LR,

    OT_CSC_BT709_RGB_FR2YCbCr_LR,
    OT_CSC_BT709_RGB_LR2YCbCr_FR,
    OT_CSC_BT709_YCbCr_FR2RGB_FR,
    OT_CSC_BT709_YCbCr_LR2RGB_LR,

    OT_CSC_MODE_BUTT,
} ot_avp_cve_csc_mode;


typedef struct {
    ot_avp_cve_csc_mode csc_mode; /* 不同转换模式, 对应不同的转换系数 */
} ot_avp_cve_csc_ctrl;


typedef struct {
    ot_s32 x;
    ot_s32 y;
} ot_avp_cve_point_s25q7;

typedef struct {
    ot_u16 x; /* RW;The X coordinate of the point */
    ot_u16 y; /* RW;The Y coordinate of the point */
} ot_avp_cve_point_u16;

typedef struct {
    ot_u16 x;
    ot_u16 y;
    ot_u16 lambda;
    ot_u16 reserved;
} ot_avp_cve_point_lambda;

typedef enum {
    OT_AVP_CVE_ST_OUT_POINT_U16   = 1,
    OT_AVP_CVE_ST_OUT_BUTT
} ot_avp_cve_st_out_mode;

typedef struct {
    ot_u32 corner_num;                  /* 有效角点数目 */
    union {
        ot_avp_cve_point_u16 point_u16[OT_AVP_CVE_ST_MAX_CORNER_NUM];
    };                                  /* 有效角点坐标数组虚拟地址 */
} ot_avp_cve_st_corner_info;

typedef struct {
    ot_s32 x;
    ot_s32 y;
    ot_u32 width;
    ot_u32 height;
} ot_avp_cve_rect;

typedef struct {
    ot_avp_cve_st_out_mode out_mode;

    ot_u8 roi_num;                               /* 感兴趣区域个数 */
    ot_avp_cve_rect roi_rect[OT_AVP_CVE_ST_MAX_ROI_NUM]; /* 感兴趣区域 */

    /* for candidate corner */
    ot_u16 quality_level;   /* 角点质量控制参数 */

    /* for select corners */
    ot_u16 max_corner_num;  /* 最大可以检测的角点数目 */
    ot_u8  min_distance;    /* 相邻角点最小距离 */

    ot_mem_info aux_mem;    /* 辅助内存 */
} ot_avp_cve_st_corner_ctrl;


/* lk optical flow pyramid */
typedef enum {
    OT_AVP_CVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_NONE = 0,   /* status和err都不输出 */
    OT_AVP_CVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_STATUS,     /* 只输出status */
    OT_AVP_CVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH,       /* 输出status和err */
    OT_AVP_CVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BUTT,
} ot_avp_cve_lk_optical_flow_pyr_out_mode;

typedef struct {
    ot_avp_cve_lk_optical_flow_pyr_out_mode out_mode;
    ot_u8 level_num;       /* 金字塔层数 */

    ot_u16 points_num;     /* 特征点数目 */

    ot_bool use_init_flow; /* 是否使用初始化光流计算 */

    ot_u32 min_eig_thr;    /* 最小特征值阈值 */
    ot_u8 iteration_cnt;   /* 迭代次数 */
    ot_u16 epsilon;        /* 迭代收敛条件：dx^2 + dy^2 < epsilon  */
} ot_avp_cve_lk_optical_flow_pyr_ctrl;

typedef ot_mem_info ot_avp_cve_lk_optical_flow_status_info; /* 光流next_pts特征点对应的跟踪状态信息，1：成功，0：失败 */
typedef ot_mem_info ot_avp_cve_lk_optical_flow_error_info;  /* 光流next_pts特征点对应的跟踪状态相似度误差估计 */
typedef ot_mem_info ot_avp_cve_lk_optical_flow_point_info;  /* 光流next_pts特征点的坐标 */
typedef struct {
    ot_avp_cve_lk_optical_flow_status_info status;    /* size >= sizeof(ot_u8) * ctrl->points_num */
    ot_avp_cve_lk_optical_flow_error_info error;      /* size >= sizeof(ot_u32) * ctrl->points_num */
    ot_avp_cve_lk_optical_flow_point_info next_pts;   /* size >= sizeof(ot_avp_cve_point_s25q7) * ctrl->points_num */
} ot_avp_cve_lk_optical_flow_pyr_dst_info;

typedef struct {
    ot_avp_cve_img prev_pyr[OT_AVP_CVE_MAX_PYR_LEVEL_NUM];       /* 前一帧图像的金字塔图像数组, 数组大小由ctrl->level_num控制 */
    ot_avp_cve_lk_optical_flow_point_info prev_pts;
                                /* 前一帧图像金字塔第0层(prev_pyr[0])的初始特征点数组.
                                   size >= sizeof(ot_avp_cve_point_s25q7) * ctrl->points_num */
    ot_avp_cve_img next_pyr[OT_AVP_CVE_MAX_PYR_LEVEL_NUM];       /* 下一帧图像的金字塔图像数组, 数组大小由ctrl->level_num控制 */
} ot_avp_cve_lk_optical_flow_pyr_src_info;




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif