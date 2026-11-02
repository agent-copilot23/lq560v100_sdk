#ifndef OT_EIS_VIDEO_H
#define OT_EIS_VIDEO_H

#include "ot_eis_type.h"
#include "ot_eis_common.h"
#include "ot_eis_mod.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OT_EIS_MAX_FRAME_BUFF_PART 3

typedef enum {
    OT_EIS_IMAGE_FORMAT_BAYER_8BPP = 0,
    OT_EIS_IMAGE_FORMAT_BAYER_10BPP,
    OT_EIS_IMAGE_FORMAT_BAYER_12BPP,
    OT_EIS_IMAGE_FORMAT_BAYER_14BPP,
    OT_EIS_IMAGE_FORMAT_BAYER_16BPP,
    OT_EIS_IMAGE_FORMAT_RGB_444 = 20,
    OT_EIS_IMAGE_FORMAT_RGB_555,
    OT_EIS_IMAGE_FORMAT_RGB_565,
    OT_EIS_IMAGE_FORMAT_RGB_888,
    OT_EIS_IMAGE_FORMAT_BGR_444,
    OT_EIS_IMAGE_FORMAT_BGR_555,
    OT_EIS_IMAGE_FORMAT_BGR_565,
    OT_EIS_IMAGE_FORMAT_BGR_888,
    OT_EIS_IMAGE_FORMAT_ARGB_1555 = 60,
    OT_EIS_IMAGE_FORMAT_ARGB_4444,
    OT_EIS_IMAGE_FORMAT_ARGB_8888,
    OT_EIS_IMAGE_FORMAT_ABGR_1555,
    OT_EIS_IMAGE_FORMAT_ABGR_4444,
    OT_EIS_IMAGE_FORMAT_ABGR_8888,
    OT_EIS_IMAGE_FORMAT_YUV_422_SEMIPLANAR = 160,
    OT_EIS_IMAGE_FORMAT_YUV_422_PACKED_YUYV,
    OT_EIS_IMAGE_FORMAT_YUV_422_PACKED_YVYU,
    OT_EIS_IMAGE_FORMAT_YUV_422_PACKED_YYUV,
    OT_EIS_IMAGE_FORMAT_YUV_422_PACKED_YYVU,
    OT_EIS_IMAGE_FORMAT_YUV_422_PACKED_UYVY,
    OT_EIS_IMAGE_FORMAT_YUV_422_PACKED_UVYY,
    OT_EIS_IMAGE_FORMAT_YUV_422_PACKED_VYUY,
    OT_EIS_IMAGE_FORMAT_YUV_422_PACKED_VUYY,
    OT_EIS_IMAGE_FORMAT_YUV_420_SEMIPLANAR = 180,
    OT_EIS_IMAGE_FORMAT_YVU_422_PLANAR = 200,
    OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR,
    OT_EIS_IMAGE_FORMAT_YVU_420_PLANAR = 220,
    OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
    OT_EIS_IMAGE_FORMAT_YUV_400 = 240,

    OT_EIS_IMAGE_FORMAT_BUTT
} ot_eis_img_pixel_format;

typedef enum {
    OT_EIS_PIXEL_BIT_DEPTH_8 = 8,
    OT_EIS_PIXEL_BIT_DEPTH_10 = 10,
    OT_EIS_PIXEL_BIT_DEPTH_12 = 12,
    OT_EIS_PIXEL_BIT_DEPTH_16 = 16,
    OT_EIS_PIXEL_BIT_DEPTH_BUTT
} ot_eis_img_pixel_bit_depth;

typedef struct {
    ot_phys_addr misc_attr_phys_addr;        /* default allocated buffer */
    ot_phys_addr jpeg_dcf_phys_addr;
    ot_phys_addr isp_attr_phys_addr;
    ot_phys_addr low_delay_phys_addr;
    ot_phys_addr bnr_mot_phys_addr;
    ot_phys_addr rsv1_phys_addr;
    ot_phys_addr rsv2_phys_addr;
    ot_phys_addr isp_phys_addr;

    ot_void* ALIGN_ATTR misc_attr_virt_addr;       /* misc info */
    ot_void* ALIGN_ATTR jpeg_dcf_virt_addr;        /* jpeg_dcf, used in JPEG DCF */
    ot_void* ALIGN_ATTR isp_attr_virt_addr;        /* isp_frame_info, used in ISP debug, when get raw and send raw */
    ot_void* ALIGN_ATTR low_delay_virt_addr;       /* used in low delay */
    ot_void* ALIGN_ATTR bnr_mot_virt_addr;         /* used for 3dnr from bnr mot */
    ot_void* ALIGN_ATTR rsv1_virt_addr;
    ot_void* ALIGN_ATTR rsv2_virt_addr;
    ot_void* ALIGN_ATTR isp_virt_addr;
} ot_eis_img_extra_attr;

typedef enum {
    OT_EIS_IMAGE_LAYOUT_LINEAR, /* nature frame line */
    OT_EIS_IMAGE_LAYOUT_TILE_64x16,
    OT_EIS_IMAGE_LAYOUT_BUTT
} ot_eis_img_layout;

typedef enum {
    OT_EIS_IMAGE_COMPRESS_MODE_NONE = 0x0,   /* no compress */
    OT_EIS_IMAGE_COMPRESS_MODE_SEG,           /* compress unit is 256x1 bytes as a segment. */
    OT_EIS_IMAGE_COMPRESS_MODE_LINE,          /* compress unit is the whole line. */
    OT_EIS_IMAGE_COMPRESS_MODE_FRAME,         /* compress unit is the whole frame. YUV for 3DNR */
    OT_EIS_IMAGE_COMPRESS_MODE_BUTT
} ot_eis_img_compress_mode;

typedef enum {
    OT_EIS_IMAGE_COLOR_SPACE_BT601 = 0x0,   /* color space 601*/
    OT_EIS_IMAGE_COLOR_SPACE_BT709,
    OT_EIS_IMAGE_COLOR_SPACE_BT2020,
    OT_EIS_IMAGE_COLOR_SPACE_USER,
    OT_EIS_IMAGE_COLOR_SPACE_BUTT
} ot_eis_img_color_space;

typedef struct {
    ot_phys_addr phys_addr[OT_EIS_MAX_FRAME_BUFF_PART];  //图像数据物理地址
    ot_void* virt_addr[OT_EIS_MAX_FRAME_BUFF_PART]; //图像数据虚拟地址
    ot_u32 stride[OT_EIS_MAX_FRAME_BUFF_PART]; //图像数据跨距
} ot_eis_img_buffer;

typedef ot_eis_img_buffer ot_eis_img_compress_header;

typedef struct {
    ot_u32 width; //图像宽度
    ot_u32 height; //图像高度
    ot_eis_img_pixel_format pixel_fmt; //图像像素格式
    ot_eis_img_layout layout;      // 图像块排布       确定当前支持的layout模式
    ot_eis_img_pixel_bit_depth bit_depth; //图像像素位宽
    ot_eis_img_compress_mode compress_mode;  //图像压缩模式
    // ot_eis_img_color_space color_space;
} ot_eis_img_attr;

typedef struct {
    ot_u32 index;    //图像帧序列号
    ot_u64 pts;    //帧时间戳
    ot_eis_img_attr attr;    //帧图像属性
    ot_eis_img_buffer buff;    // 帧数据保存地址信息
    ot_eis_img_compress_header header;    //压缩头的buffer
    ot_eis_module_id mod_id;
    ot_eis_handle pool_hdl;
    ot_eis_img_extra_attr extra_attr;
    ot_eis_img_color_space color_space;
    ot_u32 frame_flag;
} ot_eis_img_frame;

typedef ot_eis_img_frame ot_eis_video_frame;

typedef struct {
    ot_u32 x;          //区域横坐标像素起始位置
    ot_u32 y;          //区域纵坐标像素起始位置
    ot_u32 width;      //区域宽度像素值
    ot_u32 height;     //区域高度像素值
} ot_eis_img_rect;

typedef struct {
    ot_s32 x;
    ot_s32 y;
} ot_eis_img_point;

typedef struct {
    ot_u32 width;
    ot_u32 height;
} ot_eis_img_size;

typedef enum {
    OT_EIS_WDR_MODE_NONE = 0,
    OT_EIS_WDR_MODE_BUILT_IN,
    OT_EIS_WDR_MODE_RESERVED,

    OT_EIS_WDR_MODE_2To1_LINE,
    OT_EIS_WDR_MODE_2To1_FRAME,

    OT_EIS_WDR_MODE_3To1_LINE,
    OT_EIS_WDR_MODE_3To1_FRAME,

    OT_EIS_WDR_MODE_4To1_LINE,
    OT_EIS_WDR_MODE_4To1_FRAME,

    OT_EIS_WDR_MODE_BUTT,
} ot_eis_wdr_mode;

typedef struct {
    ot_s32  src_frame_rate;        /* source 帧率 */
    ot_s32  dst_frame_rate;        /* dest 帧率 */
} ot_eis_frame_rate_ctrl;

typedef struct {
    ot_bool enable;         /* RW; low delay enable. */
    ot_u32  line_cnt;       /* RW; range: [16, 8192]; low delay shoreline. */
    ot_bool one_buf_en;     /* RW; one buffer for low delay enable. */
} ot_eis_low_delay_info;

typedef enum {
    OT_EIS_COORD_ABS = 0,                          /* Absolute coordinate. */
    OT_EIS_COORD_RATIO,                            /* Ratio coordinate. */
    OT_EIS_COORD_BUTT
} ot_eis_coord_type;

typedef enum  {
    OT_EIS_OP_MODE_AUTO   = 0,
    OT_EIS_OP_MODE_MANUAL = 1,
    OT_EIS_OP_MODE_BUTT
} ot_eis_op_mode;

typedef struct {
    /* ies0~3 for different frequency response. */
    ot_u8 ies0;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u8 ies1;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u8 ies2;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u8 ies3;     /* RW; Range: [0, 255]; The gains of edge and texture enhancement. */
    ot_u16 iedz;    /* RW; Range: [0, 999]; The threshold to control the generated artifacts. */
    ot_u8 o_sht;    /* RW; Range: [0, 255]; The gains to control the overshoot. */
    ot_u8 u_sht;    /* RW; Range: [0, 255]; The gains to control the undershoot. */
} ot_eis_nr_iey;

typedef struct {
    struct {
        ot_u16 spn    : 4;  /* RW; Range: [0,   5]; The selection of filters to be mixed for NO.6 filter. */
        ot_u16 sbn    : 4;  /* RW; Range: [0,   5]; The selection of filters to be mixed for NO.6 filter. */
        ot_u16 pbr    : 5;  /* RW; Range: [0,  16]; The mix ratio between spn and sbr. */
        ot_u16 j_mode : 2;  /* RW; Range: [0,   2]; The selection modes for the blending of spatial filters. */
        ot_u16 reserved0   : 1;  /* RW; Reserved */
    };

    ot_u8 sfr6[4];     /* RW; Range: [0, 31]; The relative NR strength for NO.6 filter. (Effective when jmode = 2). */
    ot_u8 sbr6[2];     /* RW; Range: [0, 15]; The control of overshoot and undershoot. */

    ot_u8 sfs1;        /* RW; Range: [0, 255]; The NR strength parameters for NO.1 filter. */
    ot_u8 sbr1;        /* RW; Range: [0, 255]; The NR strength parameters for NO.1 filter. */
    ot_u8 sfs2;        /* RW; Range: [0, 255]; The NR strength parameters for NO.2 filter. */
    ot_u8 sft2;        /* RW; Range: [0, 255]; The NR strength parameters for NO.2 filter. */
    ot_u8 sbr2;        /* RW; Range: [0, 255]; The NR strength parameters for NO.2 filter. */
    ot_u8 sfs4;        /* RW; Range: [0, 255]; The NR strength parameters for NO.3 and NO.4 filters. */
    ot_u8 sft4;        /* RW; Range: [0, 255]; The NR strength parameters for NO.3 and NO.4 filters. */
    ot_u8 sbr4;        /* RW; Range: [0, 255]; The NR strength parameters for NO.3 and NO.4 filters. */
    /*
     * sth1_0, sth2_0, sth3_0; Range: [0, 511]; The thresholds for protection of edges on foreground from blurring.
     * sth1_1, sth2_1, sth3_1; Range: [0, 511]; The thresholds for protection of edges on background from blurring.
     * bri_idx0, bri_idx1; Range: [0, 7]; The filter selection based on brightness
     * for foreground and background, respectively.
     */
    ot_u16 sth1_0;  /* Range: [0, 511] */
    ot_u16 sth2_0;  /* Range: [0, 511] */
    ot_u16 sth3_0;  /* Range: [0, 511] */
    ot_u16 sth1_1;  /* Range: [0, 511] */
    ot_u16 sth2_1;  /* Range: [0, 511] */
    ot_u16 sth3_1;  /* Range: [0, 511] */

    /*
     * sfn0_0~sfn3_0; Range: [0, 6]; Filter selection for different foreground image areas based on sth1_0~sth3_0.
     * sfn0_1~sfn3_1; Range: [0, 6]; Filter selection for different background image areas based on sth1_1~sth3_1.
     */
    struct {
        ot_u16 sfn0_0 : 4;
        ot_u16 sfn1_0 : 4;
        ot_u16 sfn2_0 : 4;
        ot_u16 sfn3_0 : 4;
    };
    struct {
        ot_u16 sfn0_1 : 4;
        ot_u16 sfn1_1 : 4;
        ot_u16 sfn2_1 : 4;
        ot_u16 sfn3_1 : 4;
    };
    ot_u8 bri_str[17]; /* RW ; Range: [0, 255];  Spatial NR strength based on brightness. */
    struct {
        ot_u8 bri_idx0 : 3;     /* RW; Range: [0, 6]; The filter selection based on brightness */
        ot_u8 bri_idx1 : 3;     /* RW; Range: [0, 6]; The filter selection based on brightness */
        ot_u8 reserved1 : 2;
    };
    ot_u8 k_mode;               /* RW; Range: [0, 2]; mode selection of k filter */
    ot_u8 sbs_k[33];            /* RW; Range: [0, 255]; the filter strength of k filter depending on brightness */
    ot_u8 sds_k[33];            /* RW; Range: [0, 255]; the filter strength of k filter depending on brightness */
    ot_u8 reserved2;
} ot_eis_nr_sfy;

typedef struct {
    struct {
        ot_u16 tfs0   : 4;   /* RW; Range: [0,  15]; The NR strength for temporal filtering. must be 0 for vproc */
        ot_u16 tdz0   : 10;  /* RW; Range: [0, 999]; Protection of the weak texture area from temporal filtering. */
        ot_u16 ref_en : 1;   /* RW; Range: [0,  1];  The switch the reference frame. */
        ot_u16 reserved0  : 1;
    };
    struct {
        ot_u16 tfs1      : 4;   /* RW; Range: [0,  15]; The NR strength for temporal filtering. */
        ot_u16 tdz1      : 10;  /* RW; Range: [0, 999]; Protection of the weak texture area from temporal filtering. */
        ot_u16 math_mode : 1;   /* RW; Range: [0,   1]; The motion detection mode. must be 1 for vproc */
        ot_u16 reserved1     : 1;
    };
    ot_u8   tss0;       /* RW; Range: [0,  15]; The ratio for blending spatial NR with the temporal NR results. */
    ot_u8   tss1;       /* RW; Range: [0,  15]; The ratio for blending spatial NR with the temporal NR results. */

    ot_u16 auto_math;   /* RW; Range: [0, 999]; The motion threshold for the Level 0 denoise. */
    ot_u8 tfr0[6];      /* RW; Range: [0,  31]; The temporal NR strength control for background (static) area. */
    ot_u8 tfr1[6];      /* RW; Range: [0,  31]; The temporal NR strength control for background (static) area. */
} ot_eis_nr_tfy;

typedef struct {
    struct {
        ot_u16 math0 : 10;   /* RW; Range: [0, 999]; The threshold for motion detection. */
        ot_u16 mate0 : 4;    /* RW; Range: [0,   8]; The motion index for smooth image area. */
        ot_u16 adv_math : 2; /* RW; Range: [0,   2]; The mode selection for motion detection. */
    };
    struct {
        ot_u16 math1 : 10;  /* RW; Range: [0, 999]; The threshold for motion detection. */
        ot_u16 mate1 : 4;   /* RW; Range: [0,   8]; The motion index for smooth image area. */
        ot_u16 reserved0 : 2;
    };
    ot_u8   mabw0;          /* RW; Range: [5,   9]; The window size for motion detection. */
    ot_u8   mabw1;          /* RW; Range: [5,   9]; The window size for motion detection. */

    ot_u16 artc;           /* RW; Range: [0, 255]; The artifact control for motion */
    ot_u16 mcth;           /* RW; Range: [0, 999]; The sensitivity for motion */
    ot_u16 dci_w;          /* RW; Range: [0, 999]; The temporal gain for dci gain. must be 0 for vproc */
} ot_eis_nr_mdy;

typedef struct {
    ot_u8 sfc_enhance;     /* RW; Range: [0, 255];  The chroma NR strength for sfc. */
    ot_u8 sfc_ext;         /* RW; Range: [0, 255];  The extra chroma NR strength for sfc. */
    ot_u8 trc;             /* RW; Range: [0, 255];  The temporal chroma NR strength. */
    ot_u8 reserved0;
    struct {
        ot_u16 sfc   : 8;   /* RW; Range: [0, 255];  The motion threshold for chroma. */
        ot_u16 tfc   : 6;   /* RW; Range: [0, 63];   The control of temporal chroma NR strength. */
        ot_u16 reserved1 : 2;
    };
    ot_u8 tpc;              /* RW; Range: [0, 1];  Chroma denoise mode. must be 1 in vproc */
    ot_u8 pre_sfc;          /* RW; Range: [0, 31];  pre-chroma denoise. */
} ot_eis_nr_nrc0;

typedef struct {
    ot_u8 sfs2;      /* RW; Range: [0, 255];  The NR strength parameters for NO.2 filter. */
    ot_u8 sft2;      /* RW; Range: [0, 255];  The NR strength parameters for NO.2 filter. */
    ot_u8 sbr2;      /* RW; Range: [0, 255];  The NR strength parameters for NO.2 filter. */
    struct {
        ot_u16 pbr0  : 4;    /* RW; Range: [0, 15]; The mix ratio. */
        ot_u16 pbr1  : 4;    /* RW; Range: [0, 15]; The mix ratio. */
        ot_u16 reserved0 : 8;
    };
} ot_eis_nr_nrc1;

typedef struct {
    ot_eis_nr_iey  iey[5];
    ot_eis_nr_sfy  sfy[5];
    ot_eis_nr_mdy  mdy[2];
    ot_eis_nr_tfy  tfy[3];
    ot_eis_nr_nrc0 nrc0;
    ot_eis_nr_nrc1 nrc1;

    struct {
        ot_u8 limit_range_en : 1;  /* RW; Range: [0, 1]; The switch for limit range mode. */
        ot_u8 nry0_en        : 1;  /* RW; Range: [0, 1]; The switch for luma denoise. */
        ot_u8 nry1_en        : 1;  /* RW; Range: [0, 1]; The switch for luma denoise. */
        ot_u8 nry2_en        : 1;  /* RW; Range: [0, 1]; The switch for luma denoise. */
        ot_u8 nry3_en        : 1;  /* RW; Range: [0, 1]; The switch for luma denoise. */
        ot_u8 nrc0_en        : 1;  /* RW; Range: [0, 1]; The switch for chroma denoise. */
        ot_u8 nrc1_en        : 1;  /* RW; Range: [0, 1]; The switch for chroma denoise. */
        ot_u8 reserved0      : 1;
    };
} ot_eis_nr_cfg;

typedef struct {
    ot_eis_nr_cfg nr_param;
} ot_eis_nr_param_manual;

typedef struct {
    ot_u32 param_num;               /* RW; Range: [1, 16] */
    ot_u32 ALIGN_ATTR *iso;
    ot_eis_nr_cfg ALIGN_ATTR *nr_param;
} ot_eis_nr_param_auto;

typedef enum {
    OT_EIS_NR_TYPE_VIDEO_NORM = 0,
    OT_EIS_NR_TYPE_SNAP_NORM,
    OT_EIS_NR_TYPE_VIDEO_SPATIAL,
    OT_EIS_NR_TYPE_BUTT
} ot_eis_nr_type;

typedef struct {
    ot_eis_op_mode nr_mode;
    ot_eis_nr_param_manual nr_manual;
    ot_eis_nr_param_auto nr_auto;
} ot_eis_3dnr_cfg;

typedef struct {
    ot_eis_3dnr_cfg nr_cfg;
} ot_eis_3dnr_param;

typedef struct {
    ot_bool enable;
    ot_eis_nr_type nr_type;
    ot_eis_3dnr_param param;
    ot_eis_img_compress_mode compress_mode;  //3dnr 压缩模式
} ot_eis_3dnr_attr;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
