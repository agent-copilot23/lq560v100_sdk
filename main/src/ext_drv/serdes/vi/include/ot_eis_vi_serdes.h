#ifndef OT_EIS_VI_SERDES_H
#define OT_EIS_VI_SERDES_H

#include "ot_eis_type.h"
#include "ot_eis_vi_serdes_errno.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#define OT_EIS_VI_SERDES_ID_MAX     4
#define OT_EIS_VI_SERDES_HDL_MAX    8
#define OT_EIS_VI_SERDES_NAME_MAX   32
#define OT_EIS_VI_SERDES_PORT_MAX   4
#define OT_EIS_VI_SERDES_VC_ID_MAX  4

typedef ot_eis_handle ot_eis_vi_serdes_handle;

typedef enum {
    OT_VI_SERDES_BUS_TYPE_I2C,
    OT_VI_SERDES_BUS_TYPE_INVALID,
} ot_eis_vi_serdes_bus_type;

typedef struct {
    ot_eis_vi_serdes_bus_type bus_type; // i2c or ...
    ot_s8 bus_id; // i2c bus
} ot_eis_vi_serdes_bus_info;

typedef enum {
    OT_VI_SERDES_PORT_0,
    OT_VI_SERDES_PORT_1,
    OT_VI_SERDES_PORT_2,
    OT_VI_SERDES_PORT_3,
    OT_VI_SERDES_PORT_INVALID,
} ot_eis_vi_serdes_port;

// addr is 8bit, must be even.
typedef struct {
    ot_u8 serial_addr; // serializer addr
} ot_eis_vi_serdes_one_port_attr;

typedef struct {
    ot_eis_vi_serdes_port serdes_port;

    // addr is 8bit, must be even.
    ot_u8 serial_src_addr; // serializer source addr
    ot_u8 serial_dst_addr; // serializer destination addr, the src addr couldn't be equal to dst addr
    ot_u8 sns_src_addr; // sensor source addr
    ot_u8 sns_dst_addr; // sensor destination addr, the src addr couldn't be equal to dst addr
    // vc_id's value must be less than 4, According to ot_eis_vi_serdes_wdr_mode take coordinate values
    ot_u8 vc_id[OT_EIS_VI_SERDES_VC_ID_MAX];
} ot_eis_vi_serdes_multi_port_attr;

typedef enum {
    OT_VI_SERDES_DATA_FMT_RAW_8BIT,
    OT_VI_SERDES_DATA_FMT_RAW_10BIT,
    OT_VI_SERDES_DATA_FMT_RAW_12BIT,
    OT_VI_SERDES_DATA_FMT_RAW_14BIT,
    OT_VI_SERDES_DATA_FMT_RAW_16BIT,
    OT_VI_SERDES_DATA_FMT_YUV422_8BIT,
    OT_VI_SERDES_DATA_FMT_INVALID,
} ot_eis_vi_serdes_data_fmt;

typedef enum {
    OT_VI_SERDES_WDR_TYPE_NONE,
    OT_VI_SERDES_WDR_TYPE_VC,
    OT_VI_SERDES_WDR_TYPE_DT,
    OT_VI_SERDES_WDR_TYPE_DOL,
    OT_VI_SERDES_WDR_TYPE_INVALID,
} ot_eis_vi_serdes_wdr_type;

typedef enum {
    OT_VI_SERDES_WDR_MODE_NONE,
    OT_VI_SERDES_WDR_MODE_2TO1,
    OT_VI_SERDES_WDR_MODE_3TO1,
    OT_VI_SERDES_WDR_MODE_4TO1,
    OT_VI_SERDES_WDR_MODE_INVALID,
} ot_eis_vi_serdes_wdr_mode;

typedef struct {
    ot_eis_vi_serdes_bus_info bus_info;

    ot_u8 deserial_addr; // deserializer addr
    ot_u32 freq_mhz; // deserializer output freq

    ot_u8 sns_nums; // sensor numbers
    union {
        ot_eis_vi_serdes_one_port_attr one_port_attr; // sns_nums is equal to 1
        ot_eis_vi_serdes_multi_port_attr multi_port_attr[OT_EIS_VI_SERDES_PORT_MAX]; // sns_nums is larger to 1
    };

    ot_u8 sns_lanes; // sensor lane numbers
    ot_eis_vi_serdes_data_fmt data_fmt; // data type, raw8/10 ...
    ot_eis_vi_serdes_wdr_type wdr_type; // wdr, linear ...
    ot_eis_vi_serdes_wdr_mode wdr_mode; // 2to1, 3to1 ...

    ot_void *priv_data; // User-defined parameters   //  bit0:x3f bit1:is_sc220
} ot_eis_vi_serdes_attr;

ot_s32 ot_eis_vi_serdes_init(ot_void);
ot_s32 ot_eis_vi_serdes_deinit(ot_void);

ot_s32 ot_eis_vi_serdes_create(ot_eis_vi_serdes_handle *serdes_hdl,
    const ot_s8 *serdes_name, const ot_eis_vi_serdes_attr *serdes_attr); // serdes_attr through transmission, convenient user expansion
ot_s32 ot_eis_vi_serdes_destroy(ot_eis_vi_serdes_handle serdes_hdl);

ot_s32 ot_eis_vi_serdes_start(ot_eis_vi_serdes_handle serdes_hdl);
ot_s32 ot_eis_vi_serdes_stop(ot_eis_vi_serdes_handle serdes_hdl);

ot_s32 ot_eis_vi_serdes_restart(ot_eis_vi_serdes_handle serdes_hdl, ot_eis_vi_serdes_port port_id);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OT_EIS_VI_SERDES_H */