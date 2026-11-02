#ifndef __OT_EIS_COMMON_VENC_H__
#define __OT_EIS_COMMON_VENC_H__

#include "ot_eis_errno.h"
#include "ot_eis_mod.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OT_EIS_VENC_ERRNO_START = 0x50,
    OT_EIS_VENC_ERRNO_INVALID_CHN_ID,
    OT_EIS_VENC_ERRNO_CHN_EXIST,
    OT_EIS_VENC_ERRNO_NOT_CFG,
    OT_EIS_VENC_ERRNO_NOT_PERM,
    OT_EIS_VENC_ERRNO_NO_BUF,
    OT_EIS_VENC_ERRNO_NOT_READY,
    OT_EIS_VENC_ERRNO_ID_NOT_ENOUGH,
    OT_EIS_VENC_ERRNO_DATA_ERR,
    OT_EIS_VENC_ERRNO_SYS_RESOURCE_ERR,
    OT_EIS_VENC_ERRNO_FRAME_INFO_ERR,
    OT_EIS_VENC_ERRNO_FRAME_MANAGE_ERR,
    OT_EIS_VENC_ERRNO_INNER_ERR_BUTT
} ot_eis_venc_errno;

/* invalid channel ID */
#define OT_EIS_ERR_VENC_INVALID_CHN_ID OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_INVALID_CHN_ID)
/* at least one parameter is illagal ,eg, an illegal enumeration value  */
#define OT_EIS_ERR_VENC_ILLEGAL_PARAM OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_ERRNO_ILLEGAL_PARAM)
/* channel exists */
#define OT_EIS_ERR_VENC_EXIST         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_CHN_EXIST)
/* channel exists */
#define OT_EIS_ERR_VENC_UNEXIST       OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_ERRNO_NO_EXIST)
/* using a NULL pointer */
#define OT_EIS_ERR_VENC_NULL_PTR      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_ERRNO_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define OT_EIS_ERR_VENC_NOT_CFG    OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_NOT_CFG)
/* operation is not supported by NOW */
#define OT_EIS_ERR_VENC_NOT_SUPPORT   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_ERRNO_NO_SUPPORT)
/* operation is not permitted ,eg, try to change statuses attribute */
#define OT_EIS_ERR_VENC_NOT_PERM      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_NOT_PERM)
/* failure caused by malloc memory */
#define OT_EIS_ERR_VENC_NO_MEM         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_ERRNO_NO_MEM)
/* failure caused by malloc buffer */
#define OT_EIS_ERR_VENC_NO_BUF         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_NO_BUF)
/* no data in buffer */
#define OT_EIS_ERR_VENC_BUF_EMPTY     OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_ERRNO_BUF_EMPTY)
/* no buffer for new data */
#define OT_EIS_ERR_VENC_BUF_FULL      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_ERRNO_BUF_FULL)
/* system is not ready,had not initialed or loaded */
#define OT_EIS_ERR_VENC_SYS_NOT_READY  OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_NOT_READY)
/* system is busy */
#define OT_EIS_ERR_VENC_BUSY          OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_ERRNO_BUSY)
/* buf size not enough */
#define OT_EIS_ERR_VENC_SIZE_NOT_ENOUGH OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_ERRNO_OVER_SIZE)

/* channel handle not enough */
#define OT_EIS_ERR_VENC_CHN_NOT_ENOUGH             OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_ID_NOT_ENOUGH)
#define OT_EIS_ERR_VENC_QUERY_STATUS_DATA_ERROR    OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_DATA_ERR)
#define OT_EIS_ERR_VENC_SYS_RESOURCE_ERR           OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_SYS_RESOURCE_ERR)
#define OT_EIS_ERR_VENC_FRAME_INFO_ERR             OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_FRAME_INFO_ERR)
#define OT_EIS_ERR_VENC_FRAME_MANAGE_ERR           OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_FRAME_MANAGE_ERR)

/* error not defined */
#define OT_EIS_ERR_VENC_UNDEFINED_ERR              OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VENC, OT_EIS_VENC_ERRNO_INNER_ERR_BUTT)


typedef ot_eis_handle ot_eis_venc_chn_handle;

typedef enum{
    OT_EIS_VENC_CODEC_HEVC = 0,
    OT_EIS_VENC_CODEC_H264 = 1,
    OT_EIS_VENC_CODEC_JPEG = 2,
    OT_EIS_VENC_CODEC_BUTT
}ot_eis_venc_codec_format;

typedef struct {
    ot_eis_img_frame* frame;
}ot_eis_venc_image_frame;

typedef enum{
    OT_EIS_VENC_GOP_MODE_NORMAL,
    OT_EIS_VENC_GOP_MODE_DUAL,
    OT_EIS_VENC_GOP_MODE_BUTT,
}ot_eis_venc_gop_mode; 

typedef struct {
    ot_s32 qp_delta_ip;
}ot_eis_venc_gop_normal_attr;

typedef struct {
    ot_u32 sp_interval;
    ot_s32 qp_delta_sp;
    ot_s32 qp_delta_ip;
} ot_eis_venc_gop_dual_attr;

typedef struct {
    ot_eis_venc_gop_mode            gop_mode;
    union {
        ot_eis_venc_gop_normal_attr gop_normal;
        ot_eis_venc_gop_dual_attr   gop_dual;
    };
}ot_eis_venc_gop_attr;

typedef enum{
    OT_EIS_VENC_RC_MODE_CVBR,
    OT_EIS_VENC_RC_MODE_CBR,
    OT_EIS_VENC_RC_MODE_VBR,
    OT_EIS_VENC_RC_MODE_CQP,
    OT_EIS_VENC_RC_MODE_BUTT,
}ot_eis_venc_rc_mode; 

typedef struct {
    ot_u32 statis_time;
    ot_u32 max_bit_rate;
    ot_u32 short_statis_time;
    ot_u32 long_statis_time;
    ot_u32 long_max_bit_rate;
    ot_u32 long_min_bit_rate;
}ot_eis_venc_rc_cvbr;

typedef struct {
    ot_u32 statis_time;
    ot_u32 target_bit_rate;
}ot_eis_venc_rc_cbr;

typedef struct {
    ot_u32 target_bit_rate;
    ot_u32 statis_time;
}ot_eis_venc_rc_vbr;

typedef struct {
    ot_u32 qp_for_I;
    ot_u32 qp_for_P;
}ot_eis_venc_rc_cqp;

typedef struct {
    ot_u32                gop;
    ot_u32                output_fps;
    ot_u32                input_fps;
    ot_eis_venc_rc_mode   rc_mode;
    union {
        ot_eis_venc_rc_cvbr     cvbr_attr;
        ot_eis_venc_rc_cbr      cbr_attr;
        ot_eis_venc_rc_vbr      vbr_attr;
        ot_eis_venc_rc_cqp      cqp_attr;
    };
}ot_eis_venc_rc_attr;

typedef struct {
    ot_s32 x;
    ot_s32 y;
    ot_u32 width;
    ot_u32 height;
}ot_venc_rect;

typedef struct {
    ot_u32  max_pic_width;
    ot_u32  max_pic_height;
    ot_u32  out_width;
    ot_u32  out_height;
    ot_u32  stream_buf_size;
}ot_eis_venc_common_attr;

typedef struct{
    ot_eis_venc_rc_attr   rc_attr;
    ot_eis_venc_gop_attr  gop_attr;
}ot_eis_venc_video_attr;

typedef struct {
    ot_eis_venc_codec_format   codecFormat;
    ot_eis_venc_common_attr    common_attr;
    union {
        ot_eis_venc_video_attr     video_attr;
    };
}ot_eis_venc_chn_attr;

typedef struct {
    ot_u32  index;
    ot_bool enable;
    ot_bool is_abs_qp;
    ot_s32  qp_val;
    ot_venc_rect rect;
} ot_eis_venc_video_roi_attr;

typedef struct {
    ot_u32 index;
    ot_bool enable;
    ot_u32 bitrate_level;
    ot_venc_rect rect;
} ot_eis_venc_jpeg_roi_attr;

typedef struct {
    ot_u32      unCoded_frames;
    ot_u32      unTaken_streams;
}ot_eis_venc_chn_status;

typedef struct {
    ot_u32  ALIGN_ATTR  stream_lens;
    ot_void ALIGN_ATTR *stream_virt_addr;
    ot_u32              seq;
    ot_u64              pts;
}ot_eis_venc_stream;


#ifdef __cplusplus
}
#endif

#endif