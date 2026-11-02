/***********************************************************************************
*  Copyright (C), 2022 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
*  ********************************************************************************
*  Description   : declear the error number for os level
*  Created       : 2022/11/11
*  Last Modified : 2022/11/11
*  Group         : SDK Team
*  ********************************************************************************/

#ifndef OT_EIS_MIPI_RX_H
#define OT_EIS_MIPI_RX_H


#include "ot_eis_type.h"
#include "ot_eis_errno.h"
#include "ot_eis_mipi_rx_errno.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#define OT_MIPIRX_MAX_LANE_NUM (4)

#define OT_MIPIRX_WDR_VC_NUM (4)

//support 4 for LQ560 Plus;  2 for Mini
#define OT_MIPIRX_MAX_DEV_NUM (4)

//mipi-rx extend data type num
#define OT_MIPIRX_MAX_EXT_DT_NUM (3)

//sensor reset source, support 4 for LQ560 Plus;  2 for Mini
#define OT_EIS_MAX_SNS_RESET_SRC_NUM (4)

//sensor clock source, support 4 for LQ560 Plus;  2 for Mini
#define OT_EIS_MAX_SNS_CLK_SRC_NUM (4)

typedef ot_u32 ot_eis_sensor_clk_src;

typedef ot_u32 ot_eis_sensor_reset_src;

typedef ot_u32 ot_eis_mipirx_dev_no;

typedef enum
{
    OT_MIPIRX_LANE_LAYOUT_MODE_0 = 0, /* mini&plus:8lane; lite:4lane */
    OT_MIPIRX_LANE_LAYOUT_MODE_1,     /* mini&plus:4lane + 4lane; lite: 2lane +2lane */
    OT_MIPIRX_LANE_LAYOUT_MODE_2,     /* mini&plus:4lane + 2lane +2lane; lite:not support */
    OT_MIPIRX_LANE_LAYOUT_MODE_3,     /* mini&plus:2lane + 2lane + 2lane + 2lane; lite:not support */
    OT_MIPIRX_LANE_LAYOUT_MODE_4,     /* mini&plus:2lane + 2lane + 4lane; lite:not support */
    OT_MIPIRX_LANE_LAYOUT_MODE_INVALID
} ot_eis_mipirx_lane_layout;

typedef struct
{
    ot_eis_mipirx_lane_layout divide_mode; //only for mipi-rx dev
} ot_eis_vi_if_init_attr;

typedef enum
{
    OT_VI_INTF_TYPE_MIPI = 0,
    OT_VI_INTF_TYPE_INVALID
} ot_eis_vi_if_type;

//PPC: pixel per clock
typedef enum
{
    OT_VI_INTF_PPC_X1 = 1, //pixel 1 per clock
    OT_VI_INTF_PPC_X2 = 2, //pixels 2  per clock
    OT_VI_INTF_PPC_INVALID
} ot_eis_vi_if_data_rate;

typedef enum
{
    OT_MIPIRX_DT_RAW_8 = 0, //data type raw 8bit
    OT_MIPIRX_DT_RAW_10,
    OT_MIPIRX_DT_RAW_12,
    OT_MIPIRX_DT_RAW_14,
    OT_MIPIRX_DT_RAW_16,
    OT_MIPIRX_DT_YUV422_8BIT,
    OT_MIPIRX_DT_YUV422_2_RAW16,
    OT_MIPIRX_DT_INVALID
} ot_eis_mipirx_data_fmt;

typedef enum
{
    OT_MIPIRX_WDR_NONE = 0,
    OT_MIPIRX_WDR_VC,  //virtual channel
    OT_MIPIRX_WDR_DT,  //data type
    OT_MIPIRX_WDR_DOL,  //sensor dol
    OT_MIPIRX_WDR_INVALID
} ot_eis_mipirx_wdr_type;

typedef struct {
    ot_u32 type_num;
    ot_u32 data_bit_width[OT_MIPIRX_MAX_EXT_DT_NUM];
    ot_u32 data_type[OT_MIPIRX_MAX_EXT_DT_NUM];
} ot_eis_mipirx_ext_data_attr;

typedef struct {
    union {
        ot_eis_mipirx_ext_data_attr data_attr;
    };
} ot_eis_vi_intf_ext_attr;

typedef struct
{
    ot_eis_mipirx_dev_no devno;
    ot_eis_mipirx_data_fmt data_fmt;
    ot_eis_mipirx_wdr_type wdr_mode;
    ot_u32 sns_num; // 最大值为 4
    ot_s16 lane_id[OT_MIPIRX_MAX_LANE_NUM];
    union {
        ot_s16 wdr_data_type[OT_MIPIRX_WDR_VC_NUM];// only used with wdr mode: vc/dt/dol
    };
} ot_eis_mipirx_dev_attr;


typedef struct
{
    ot_eis_vi_if_type if_type;
    ot_eis_vi_if_data_rate data_rate;
    ot_s32 img_width;
    ot_s32 img_height;
    union {
        ot_eis_mipirx_dev_attr mipi_attr;
    };
} ot_eis_vi_if_attr;  //vi interface



ot_s32 ot_eis_vi_if_init(const ot_eis_vi_if_init_attr* attr);

ot_s32 ot_eis_vi_if_create(ot_eis_handle* vi_if_hdl, const ot_eis_vi_if_attr* if_attr);

ot_s32 ot_eis_vi_if_destroy(ot_eis_handle vi_if_hdl);

ot_s32 ot_eis_vi_if_set_ext_attr(ot_eis_handle vi_if_hdl, const ot_eis_vi_intf_ext_attr* ext_data);

ot_s32 ot_eis_vi_if_get_ext_attr(ot_eis_handle vi_if_hdl, ot_eis_vi_intf_ext_attr* ext_data);

ot_s32 ot_eis_vi_if_start(ot_eis_handle vi_if_hdl);

ot_s32 ot_eis_vi_if_stop(ot_eis_handle vi_if_hdl);

//only used if sensor clk source from soc
ot_s32 ot_eis_vi_if_sensor_clk_enable(ot_eis_sensor_clk_src clk_src, ot_bool enable);

//only used if sensor reset pin is connected to soc
ot_s32 ot_eis_vi_if_sensor_enable(ot_eis_sensor_reset_src clk_reset_src, ot_bool enable);

ot_s32 ot_eis_vi_if_deinit(ot_void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
