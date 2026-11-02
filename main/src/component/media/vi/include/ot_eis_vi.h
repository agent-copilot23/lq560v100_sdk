#ifndef OT_EIS_VI_H
#define OT_EIS_VI_H

#include "ot_eis_type.h"
#include "ot_eis_mod.h"
#include "ot_eis_errno.h"
#include "ot_eis_video.h"
#include "ot_eis_mipi_rx.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define OT_EIS_VI_MAX_DEV_NUM                 4

#define OT_EIS_VI_MAX_FE_PIPE_NUM             4
#define OT_EIS_VI_MAX_PHY_PIPE_NUM            7  // 4 + 3, 3为无FE的物理pipe,只能走YUV mode,走虚拟通道分支
#define OT_EIS_VI_MAX_PIPE_NUM                (OT_EIS_VI_MAX_PHY_PIPE_NUM)
#define OT_EIS_VI_MAX_STITCH_NUM              OT_EIS_VI_MAX_FE_PIPE_NUM

#define OT_EIS_VI_MAX_PHYS_CHN_NUM            1
#define OT_EIS_VI_MAX_CHN_NUM                 1

#define OT_EIS_VI_DEV_MIN_WIDTH               120
#define OT_EIS_VI_DEV_MAX_WIDTH               8192
#define OT_EIS_VI_DEV_MIN_HEIGHT              88
#define OT_EIS_VI_DEV_MAX_HEIGHT              8192

#define OT_EIS_VI_PIPE_MIN_WIDTH              120
#define OT_EIS_VI_PIPE_MIN_HEIGHT             88
#define OT_EIS_VI_PIPE_MAX_HEIGHT             8192
#define OT_EIS_VI_PIPE_MAX_WIDTH_OFFLINE      8192
#define OT_EIS_VI_PIPE_MAX_WIDTH_ONLINE       4096

#define OT_EIS_VI_PHYS_CHN_MIN_WIDTH          120
#define OT_EIS_VI_PHYS_CHN_MIN_HEIGHT         88
#define OT_EIS_VI_PHYS_CHN_MAX_HEIGHT         8192
#define OT_EIS_VI_PHYS_CHN_MAX_WIDTH_OFFLINE  8192
#define OT_EIS_VI_PHYS_CHN_MAX_WIDTH_ONLINE   4096

#define OT_EIS_VI_EXT_CHN_MIN_WIDTH           32
#define OT_EIS_VI_EXT_CHN_MIN_HEIGHT          32
#define OT_EIS_VI_EXT_CHN_MAX_WIDTH           8192
#define OT_EIS_VI_EXT_CHN_MAX_HEIGHT          8192

#define OT_EIS_VI_MAX_VC_ID      4

#define OT_EIS_VI_PIPE_ID_NUM_MAX 2

typedef ot_eis_handle  ot_vi_pipe_handle;
typedef ot_eis_handle  ot_mipi_handle;
typedef ot_eis_handle  ot_vi_dev_handle;
typedef ot_eis_handle  ot_vi_chn_handle;

typedef ot_s32 ot_vi_dev_id;
typedef ot_s32 ot_vi_pipe_id;
typedef ot_s32 ot_vi_chn_id;

typedef enum {
    OT_ERRNO_EIS_VI_INVALID_DEV_ID        = 0x70,
    OT_ERRNO_EIS_VI_INVALID_PIPE_ID       = 0x71,
    OT_ERRNO_EIS_VI_INVALID_CHN_ID        = 0x72,
    OT_ERRNO_EIS_VI_INVALID_GRP_ID        = 0x73,
    OT_ERRNO_EIS_VI_NOT_CFG               = 0x74,
    OT_ERRNO_EIS_VI_NOT_READY             = 0x75,
    OT_ERRNO_EIS_VI_NOT_BINDED            = 0x76,
    OT_ERRNO_EIS_VI_BINDED                = 0x77,
    OT_ERRNO_EIS_VI_NOT_CREATED           = 0x78,
    OT_ERRNO_EIS_VI_CREATED               = 0x79,
    OT_ERRNO_EIS_VI_INVALID_DEV_HDL       = 0x7a,
    OT_ERRNO_EIS_VI_INVALID_PIPE_HDL      = 0x7b,
    OT_ERRNO_EIS_VI_INVALID_CHN_HDL       = 0x7c,
    OT_ERRNO_EIS_VI_HDL_RES_GET_FAIL      = 0x7d,

    OT_ERRNO_EIS_VI_ALG_MEM_NOT_INIT      = 0x80, /* memory not init */
    OT_ERRNO_EIS_VI_SNS_UNREGISTER        = 0x81, /* sensor unregister */
    OT_ERRNO_EIS_VI_NO_INT                = 0x82,
    OT_ERRNO_EIS_VI_ALG_NOT_INIT          = 0x83, /* alg not init */

    OT_ERRNO_EIS_VI_BUTT
} ot_eis_vi_err_code;

#define OT_ERR_EIS_VI_INVALID_DEV_ID  OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_INVALID_DEV_ID)
#define OT_ERR_EIS_VI_INVALID_PIPE_ID OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_INVALID_PIPE_ID)
#define OT_ERR_EIS_VI_INVALID_CHN_ID  OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_INVALID_CHN_ID)
#define OT_ERR_EIS_VI_INVALID_GRP_ID  OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_INVALID_GRP_ID)
#define OT_ERR_EIS_VI_ILLEGAL_PARAM   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_ILLEGAL_PARAM)
#define OT_ERR_EIS_VI_NULL_PTR        OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_NULL_PTR)
#define OT_ERR_EIS_VI_NOT_CFG         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_NOT_CFG)
#define OT_ERR_EIS_VI_NOT_SUPPORT     OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_NO_SUPPORT)
#define OT_ERR_EIS_VI_NOT_PERM        OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_NO_ACCESS)
#define OT_ERR_EIS_VI_NOT_ENABLE      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_NO_ENABLE)
#define OT_ERR_EIS_VI_NOT_DISABLE     OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_NO_DISABLE)
#define OT_ERR_EIS_VI_NO_MEM          OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_NO_MEM)
#define OT_ERR_EIS_VI_BUF_EMPTY       OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_BUF_EMPTY)
#define OT_ERR_EIS_VI_BUF_FULL        OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_BUF_FULL)
#define OT_ERR_EIS_VI_NOT_READY       OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_NOT_READY)
#define OT_ERR_EIS_VI_TIMEOUT         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_TIMEOUT)
#define OT_ERR_EIS_VI_BUSY            OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_EIS_ERRNO_BUSY)
#define OT_ERR_EIS_VI_NOT_BINDED      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_NOT_BINDED)
#define OT_ERR_EIS_VI_BINDED          OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_BINDED)

#define OT_ERR_EIS_VI_NOT_CREATED     OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_NOT_CREATED)
#define OT_ERR_EIS_VI_CREATED         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_CREATED)

#define OT_ERR_EIS_VI_INVALID_DEV_HDL    OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_INVALID_DEV_HDL)
#define OT_ERR_EIS_VI_INVALID_PIPE_HDL   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_INVALID_PIPE_HDL)
#define OT_ERR_EIS_VI_INVALID_CHN_HDL    OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_INVALID_CHN_HDL)
#define OT_ERR_EIS_VI_HDL_RES_GET_FAIL   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI, OT_ERRNO_EIS_VI_HDL_RES_GET_FAIL)

#define OT_EIS_VI_COMPONENT_MASK_NUM  2UL

/* Interface mode of video input */
typedef enum {
    OT_EIS_VI_INTF_MODE_MIPI_RAW = 0,       /* MIPI RAW mode */
    OT_EIS_VI_INTF_MODE_MIPI_YUV422,        /* MIPI YUV422 mode */

    OT_EIS_VI_INTF_MODE_BUTT
} ot_eis_vi_intf_mode;

/* Input data type */
typedef enum {
    OT_EIS_VI_DATA_TYPE_RAW = 0, /* input format is raw */
    OT_EIS_VI_DATA_TYPE_YUV,     /* input format is yuv */

    OT_EIS_VI_DATA_TYPE_BUTT
} ot_eis_vi_data_type;

/* Sequence of YUV data */
typedef enum {
    OT_EIS_VI_DATA_SEQ_UYVY = 0, /* The input sequence of YUV is UYVY */
    OT_EIS_VI_DATA_SEQ_VYUY,     /* The input sequence of YUV is VYUY */
    OT_EIS_VI_DATA_SEQ_YUYV,     /* The input sequence of YUV is YUYV */
    OT_EIS_VI_DATA_SEQ_YVYU,     /* The input sequence of YUV is YVYU */

    OT_EIS_VI_DATA_SEQ_BUTT
} ot_eis_vi_data_seq;

/* typedef vi inport attribute */
typedef struct {
    ot_eis_mipirx_dev_no  mipirx_dev_no;
    ot_eis_vi_intf_mode   intf_mode;
    ot_eis_vi_data_type   data_type;
    ot_eis_vi_data_seq    data_seq;
    ot_u32                component_mask[OT_EIS_VI_COMPONENT_MASK_NUM];
    ot_eis_img_size       in_size;
} ot_eis_vi_dev_attr;

#define OT_EIS_VI_FRAME_RATE_MAX           65535.0

typedef struct {
    ot_u16   width;
    ot_u16   height;
    ot_float fps;
    ot_u8    sns_mode;
} ot_eis_vi_sns_image_mode;

/* Defines the format of the input Bayer image */
typedef enum {
    OT_EIS_VI_BAYER_RGGB    = 0,
    OT_EIS_VI_BAYER_GRBG    = 1,
    OT_EIS_VI_BAYER_GBRG    = 2,
    OT_EIS_VI_BAYER_BGGR    = 3,
    OT_EIS_VI_BAYER_BUTT
} ot_eis_vi_bayer_format;

/*
* 0 = Communication between the sensor and the vi over the I2C interface
* 1 = Communication between the sensor and the vi over the SSP interface
*/
typedef enum {
    OT_EIS_VI_SNS_TYPE_I2C = 0,
    OT_EIS_VI_SNS_TYPE_SSP,

    OT_EIS_VI_SNS_TYPE_BUTT,
} ot_eis_vi_sns_type;

typedef struct {
    ot_eis_vi_sns_type type;
    ot_s8  bus_id;
    ot_u8  bus_addr;
} ot_eis_vi_sns_ctrl_bus_attr;

typedef enum {
    OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR = 0, /* RW; Frame source from pipe sensor */
    OT_EIS_VI_PIPE_FRAME_SOURCE_USER,       /* RW; User send frame to pipe BE */

    OT_EIS_VI_PIPE_FRAME_SOURCE_BUTT
} ot_eis_vi_pipe_frame_source;

typedef enum {
    OT_EIS_VI_PIPE_BYPASS_NONE = 0,
    OT_EIS_VI_PIPE_BYPASS_FE,
    OT_EIS_VI_PIPE_BYPASS_BE,

    OT_EIS_VI_PIPE_BYPASS_BUTT
} ot_eis_vi_pipe_bypass_mode;

typedef struct {
    ot_eis_vi_sns_image_mode image_mode;
    ot_eis_wdr_mode          wdr_mode;
    ot_eis_vi_bayer_format   bayer_format; //bayer_pattern
    ot_eis_vi_sns_ctrl_bus_attr bus_info;
    ot_u8 vc_id[OT_EIS_VI_MAX_VC_ID];

    ot_eis_vi_pipe_frame_source frame_source;
    ot_eis_vi_pipe_bypass_mode  bypass_mode;
    ot_bool                     isp_bypass;       /* RW;Range:[0, 1];isp bypass enable */
    ot_eis_img_pixel_format     pixel_format;
    ot_eis_img_compress_mode    compress_mode;    /* RW; Range:[0, 4];Compress mode. */
    ot_eis_frame_rate_ctrl      frame_rate_ctrl;  /* RW; Frame rate ctrl */
} ot_eis_vi_pipe_attr;

typedef struct {
    ot_bool  enable;          /* RW; Crop enable */
    ot_eis_img_rect  rect;    /* RW; Crop rectangular */
} ot_eis_vi_crop_info;

#define OT_EIS_VI_MAX_CHN_FRAME_DEPTH 8
typedef enum {
    OT_EIS_VI_CHN_WORK_MODE_PIPE,
    OT_EIS_VI_CHN_WORK_MODE_USER,
    OT_EIS_VI_CHN_WORK_MODE_BUTT
} ot_eis_vi_chn_work_mode;

typedef struct {
    ot_eis_img_size            size;            /* RW; channel out put size */
    ot_eis_img_pixel_format    pixel_format;    /* RW; pixel format */
    ot_eis_img_compress_mode   compress_mode;
    ot_eis_vi_crop_info        crop_info;
    ot_eis_frame_rate_ctrl     frame_rate_ctrl;  /* RW; Frame rate ctrl */

    /* for user_mode */
    ot_eis_vi_chn_work_mode work_mode;
    ot_u32            frame_queue_depth;           /* RW; range [0,8];depth */
    ot_eis_handle         pool_handle;
} ot_eis_vi_chn_attr;

typedef struct {
    ot_bool enable;
} ot_eis_vi_quickboot_attr;

typedef struct {
    ot_u32  frame_rate;      /* RO; Current frame rate */
    ot_u32  interrupt_cnt;   /* RO; frame interrupt count */
    ot_u32  lost_frame_cnt;  /* RO; lost frame count */
    ot_u32  vrb_fail_cnt;    /* RO; Video buffer acquire failure */
    ot_eis_img_size size;    /* RO; Current pipe FE output size */
} ot_eis_vi_pipe_status;

typedef struct {
    ot_u32  frame_rate;     /* RO; current frame rate */
    ot_u32  lost_frame_cnt; /* RO; lost frame count */
    ot_u32  vrb_fail_cnt;    /* RO; video buffer acquire failure */
    ot_eis_img_size size;    /* RO; chn output size */
} ot_eis_vi_chn_status;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_VI_H */
