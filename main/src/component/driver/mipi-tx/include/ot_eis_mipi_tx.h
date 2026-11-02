/***********************************************************************************
*  Copyright (C), 2022 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
*  ********************************************************************************
*  Description   : declear the error number for os level
*  Created       : 2022/11/11
*  Last Modified : 2022/11/11
*  Group         : SDK Team
*  ********************************************************************************/

#ifndef OT_EIS_MIPI_TX_H
#define OT_EIS_MIPI_TX_H

#include "ot_eis_type.h"
#include "ot_eis_errno.h"
#include "ot_eis_mipi_tx_errno.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#define OT_MIPITX_MAX_LANE_NUM   4
#define OT_MIPITX_LANE_NUM_PER_DEV 4
#define OT_MIPITX_DISABLE_LANE_ID (-1)
#define OT_MIPITX_SET_DATA_SIZE 800
#define OT_MIPITX_GET_DATA_SIZE 160

#define OT_MIPITX_ALIGN_NUM          8
#define OT_MIPITX_ATTRIBUTE             __attribute__((aligned(OT_MIPITX_ALIGN_NUM)))

typedef enum {
    OT_MIPITX_OUT_MODE_CSI            = 0x0,              /* csi mode */
    OT_MIPITX_OUT_MODE_DSI_VIDEO      = 0x1,              /* dsi video mode */
    OT_MIPITX_OUT_MODE_DSI_CMD        = 0x2,              /* dsi command mode */

    OT_MIPITX_OUT_MODE_BUTT
} ot_eis_mipitx_out_mode;


typedef enum {
    OT_MIPITX_WORK_MODE_LP     = 0x0,              /* LP(Low Power) work mode */
    OT_MIPITX_WORK_MODE_HS     = 0x1,              /* HS(High Speed) work mode */

    OT_MIPITX_WORK_MODE_BUTT
} ot_eis_mipitx_work_mode;


typedef enum {
    OT_MIPITX_BURST_MODE                      = 0x0,
    OT_MIPITX_NON_BURST_MODE_SYNC_PULSES      = 0x1,
    OT_MIPITX_NON_BURST_MODE_SYNC_EVENTS      = 0x2,

    OT_MIPITX_VIDEO_DATA_MODE_BUTT
} ot_eis_mipitx_video_mode;

typedef enum {
    OT_MIPITX_OUT_FORMAT_RGB_16BIT          = 0x0, /* DSI */
    OT_MIPITX_OUT_FORMAT_RGB_18BIT          = 0x1,
    OT_MIPITX_OUT_FORMAT_RGB_18BIT_LOOSELY  = 0x2,
    OT_MIPITX_OUT_FORMAT_RGB_24BIT          = 0x3,
    OT_MIPITX_OUT_FORMAT_YUV420_12BIT       = 0x4,
    OT_MIPITX_OUT_FORMAT_YUV422_16BIT       = 0x5,

    OT_MIPITX_OUT_FORMAT_YUV420_8BIT_NORMAL = 0x6, /* CSI */
    OT_MIPITX_OUT_FORMAT_YUV420_8BIT_LEGACY = 0x7,
    OT_MIPITX_OUT_FORMAT_YUV422_8BIT        = 0x8,
    OT_MIPITX_OUT_FORMAT_RGB_888            = 0x9,
    OT_MIPITX_OUT_FORMAT_RAW_8BIT           = 0xa,
    OT_MIPITX_OUT_FORMAT_RAW_10BIT          = 0xb,
    OT_MIPITX_OUT_FORMAT_RAW_12BIT          = 0xc,
    OT_MIPITX_OUT_FORMAT_RAW_14BIT          = 0xd,
    OT_MIPITX_OUT_FORMAT_RAW_16BIT          = 0xe,
    OT_MIPITX_OUT_FORMAT_BUTT
} ot_eis_mipitx_out_format;


typedef struct {
    ot_u16  vact;
    ot_u16  vbp;
    ot_u16  vfp;

    ot_u16  hact;
    ot_u16  hbp;
    ot_u16  hfp;

    ot_u16  hpw;
    ot_u16  vpw;
} ot_eis_mipitx_sync_info;


typedef enum {
    OT_MIPITX_CLK_LANE_CONTINUE     = 0x0,
    OT_MIPITX_CLK_LANE_NON_CONTINUE = 0x1,
    OT_MIPITX_CLK_LANE_BUTT
} ot_eis_mipitx_continue_mode;


typedef struct {
    ot_u32        devno;                   /* device number */
    ot_s16               lane_id[OT_MIPITX_MAX_LANE_NUM];   /* lane_id: -1 - disable */
    ot_eis_mipitx_out_mode          out_mode;                /* output mode: CSI/DSI_VIDEO/DSI_CMD */
    ot_eis_mipitx_video_mode video_mode;
    ot_eis_mipitx_out_format out_format;
    ot_eis_mipitx_sync_info         sync_info;
    ot_u32        phy_data_rate;           /* Mbps */
    ot_u32        pixel_clk;               /* KHz */
    ot_eis_mipitx_continue_mode     clklane_continue_mode;
} ot_eis_mipitx_combo_dev_attr;

typedef struct {
    ot_u32        devno;                   /* device number */
    ot_eis_mipitx_work_mode work_mode;               /* work mode: low power mode, high speed mode. */
    ot_u8       lp_clk_en;               /* low power clock enable. */
    ot_u16      data_type;               /* data type */
    ot_u16      cmd_size;
    const ot_u8 OT_MIPITX_ATTRIBUTE *cmd;
} ot_eis_mipitx_cmd_info_attr;

typedef struct {
    ot_u32        devno;          /* device number */
    ot_eis_mipitx_work_mode work_mode;      /* work mode: low power mode, high speed mode. */
    ot_u8       lp_clk_en;      /* low power clock enable. */
    ot_u16      data_type;      /* DSI data type */
    ot_u16      data_param;     /* data param,low 8 bit:first param.high 8 bit:second param, set 0 if not use */
    ot_u16      get_data_size;  /* read data size */
    ot_u8       OT_MIPITX_ATTRIBUTE *get_data;      /* read data memory address, should  malloc by user */
} ot_eis_mipitx_get_cmd_info_attr;


typedef struct {
    ot_u32 devno;
    ot_u8 pn_swap[OT_MIPITX_LANE_NUM_PER_DEV];
} ot_eis_mipitx_combo_dev_pn_attr;


typedef struct {
    ot_eis_mipitx_combo_dev_attr dev_attr;
} ot_eis_vo_if_attr;

ot_s32 ot_eis_vo_if_init(ot_void);

ot_s32 ot_eis_vo_if_deinit(ot_void);

ot_s32 ot_eis_vo_if_create(ot_eis_handle* vo_if_hdl);

ot_s32 ot_eis_vo_if_destroy(ot_eis_handle vo_if_hdl);

ot_s32 ot_eis_vo_if_config(ot_eis_handle vo_if_hdl, const ot_eis_vo_if_attr* if_attr);

ot_s32 ot_eis_vo_if_start(ot_eis_handle vo_if_hdl);

ot_s32 ot_eis_vo_if_stop(ot_eis_handle vo_if_hdl);

ot_s32 ot_eis_vo_if_set_cmd_info(ot_eis_handle vo_if_hdl, const ot_eis_mipitx_cmd_info_attr* cmd_info_attr);

ot_s32 ot_eis_vo_if_get_cmd_info(ot_eis_handle vo_if_hdl, ot_eis_mipitx_get_cmd_info_attr* cmd_info_attr);

ot_s32 ot_eis_vo_if_dev_pn_swap(ot_eis_handle vo_if_hdl, const ot_eis_mipitx_combo_dev_pn_attr* swap_attr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
