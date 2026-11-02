#ifndef __OT_EIS_COMMON_VDEC_H__
#define __OT_EIS_COMMON_VDEC_H__

#include "ot_eis_errno.h"
#include "ot_eis_mod.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OT_EIS_VDEC_ERRNO_START = 0x50,
    OT_EIS_VDEC_ERRNO_INVALID_CHN_ID,
    OT_EIS_VDEC_ERRNO_CHN_EXIST,
    OT_EIS_VDEC_ERRNO_NOT_CFG,
    OT_EIS_VDEC_ERRNO_NOT_PERM,
    OT_EIS_VDEC_ERRNO_NO_BUF,
    OT_EIS_VDEC_ERRNO_NOT_READY,
    OT_EIS_VDEC_ERRNO_ID_NOT_ENOUGH,
    OT_EIS_VDEC_ERRNO_INNER_ERR_BUTT
} ot_eis_vdec_errno;


/*********************************************************************************************/
/* Invalid channel ID. */
#define OT_EIS_ERR_VDEC_INVALID_CHN_ID OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_VDEC_ERRNO_INVALID_CHN_ID)
/* At least one parameter is illegal ,eg, an illegal enumeration value. */
#define OT_EIS_ERR_VDEC_ILLEGAL_PARAM OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_ERRNO_ILLEGAL_PARAM)
/* Channel exists. */
#define OT_EIS_ERR_VDEC_EXIST         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_VDEC_ERRNO_CHN_EXIST)
/* Using a NULL pointer. */
#define OT_EIS_ERR_VDEC_NULL_PTR      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_ERRNO_NULL_PTR)
/* Try to enable or initialize system,device or channel, before configure attribute. */
#define OT_EIS_ERR_VDEC_NOT_CFG       OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_VDEC_ERRNO_NOT_CFG)
/* Operation is not supported by NOW. */
#define OT_EIS_ERR_VDEC_NOT_SUPPORT   OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_ERRNO_NO_SUPPORT)
/* Operation is not permitted ,eg, try to change statuses attribute. */
#define OT_EIS_ERR_VDEC_NOT_PERM      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_VDEC_ERRNO_NOT_PERM)
/* The channel is not existed. */
#define OT_EIS_ERR_VDEC_UNEXIST       OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_ERRNO_NO_EXIST)
/* Failure caused by malloc memory. */
#define OT_EIS_ERR_VDEC_NO_MEM         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_ERRNO_NO_MEM)
/* Failure caused by malloc buffer. */
#define OT_EIS_ERR_VDEC_NO_BUF         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_VDEC_ERRNO_NO_BUF)
/* No data in buffer. */
#define OT_EIS_ERR_VDEC_BUF_EMPTY     OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_ERRNO_BUF_EMPTY)
/* No buffer for new data. */
#define OT_EIS_ERR_VDEC_BUF_FULL      OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_ERRNO_BUF_FULL)
/* System is not ready,had not initialed or loaded. */
#define OT_EIS_ERR_VDEC_SYS_NOT_READY  OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_VDEC_ERRNO_NOT_READY)
/* System busy */
#define OT_EIS_ERR_VDEC_BUSY          OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_ERRNO_BUSY)

/* Bad address,  eg. used for copy_from_user & copy_to_user. */
#define OT_EIS_ERR_VDEC_BAD_ADDR       OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_ERRNO_INVALID_ADDR)

/* channel handle not enough */
#define OT_EIS_ERR_VDEC_CHN_NOT_ENOUGH OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_VDEC_ERRNO_ID_NOT_ENOUGH)

/* error not defined */
#define OT_EIS_ERR_VDEC_UNDEFINED_ERR  OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VDEC, OT_EIS_VDEC_ERRNO_INNER_ERR_BUTT)


typedef ot_eis_handle ot_eis_vdec_chn_handle;

typedef enum{
    OT_VDEC_ORDER_DISPLAY = 0,
    OT_VDEC_ORDER_DECODE
}ot_vdec_output_order;

typedef enum
{
    OT_VDEC_CODEC_HEVC = 0,
    OT_VDEC_CODEC_H264 = 1,
    OT_VDEC_CODEC_JPEG = 2,
    OT_VDEC_CODEC_BUTT
}ot_vdec_codec_format;

typedef struct {
    ot_vdec_codec_format    format;
    ot_u32                  width;  /* Max width of pic. */
    ot_u32                  height;  /* Max height of pic. */
    ot_u32                  stream_buf_size;
    ot_u32                  frame_buf_size;
    ot_u32                  frame_buf_cnt;
    ot_u32                  ref_frame_num;
    ot_vdec_output_order    output_order;
}ot_eis_vdec_chn_attr;

typedef struct {
    ot_u32              end_of_stream;
    ot_u64              pts;
    ot_u32              stream_len;
    ot_void* ALIGN_ATTR stream_virt_addr;
}ot_eis_vdec_stream;

typedef struct {
    ot_u32 x;
    ot_u32 y;
    ot_u32 width;
    ot_u32 height;
}ot_vdec_rect;

typedef struct {
  ot_u32                pic_width;                    /**< decoded picture width in pixels */
  ot_u32                pic_height;                   /**< decoded picture height in pixels */
}ot_eis_vdec_stream_info;

typedef struct {
    ot_eis_img_frame image;
}ot_eis_vdec_pic_frame;

typedef struct {
    ot_u32 unDecoded_streams;
    ot_u32 unTaken_frames;
}ot_eis_vdec_chn_info;


#ifdef __cplusplus
}
#endif

#endif