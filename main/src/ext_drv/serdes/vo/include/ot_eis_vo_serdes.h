#ifndef OT_EIS_VO_SERDES_H
#define OT_EIS_VO_SERDES_H

#include "ot_eis_type.h"
#include "ot_eis_vo_serdes_errno.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#define OT_EIS_VO_SERDES_ID_MAX     4
#define OT_EIS_VO_SERDES_HDL_MAX    8
#define OT_EIS_VO_SERDES_NAME_MAX   32
#define OT_EIS_VO_SERDES_PORT_MAX   2

typedef ot_eis_handle ot_eis_vo_serdes_handle;

typedef enum {
    OT_VO_SERDES_BUS_TYPE_I2C,
    OT_VO_SERDES_BUS_TYPE_INVALID,
} ot_eis_vo_serdes_bus_type;

typedef struct {
    ot_eis_vo_serdes_bus_type bus_type;
    ot_s8 bus_id;
} ot_eis_vo_serdes_bus_info;

typedef enum {
    OT_VO_SERDES_FPDLINK_MODE_SINGLE,
    OT_VO_SERDES_FPDLINK_MODE_COPY,
    OT_VO_SERDES_FPDLINK_MODE_SEPARATE,
    OT_VO_SERDES_FPDLINK_MODE_INVALID,
} ot_eis_vo_serdes_fpdlink_mode;

typedef enum {
    OT_VO_SERDES_DATA_FMT_RGB_16BIT,
    OT_VO_SERDES_DATA_FMT_RGB_18BIT,
    OT_VO_SERDES_DATA_FMT_RGB_18BIT_LOOSELY,
    OT_VO_SERDES_DATA_FMT_RGB_24BIT,
    OT_VO_SERDES_DATA_FMT_YUV420_12BIT,
    OT_VO_SERDES_DATA_FMT_YUV422_16BIT,
    OT_VO_SERDES_DATA_FMT_INVALID,
} ot_eis_vo_serdes_data_fmt;

typedef enum {
    OT_VO_SERDES_CLK_MODE_CONTINUE,
    OT_VO_SERDES_CLK_MODE_NON_CONTINUE,
    OT_VO_SERDES_CLK_MODE_INVALID,
} ot_eis_vo_serdes_clk_mode;

typedef enum {
    OT_VO_SERDES_BURST,
    OT_VO_SERDES_NON_BURST_SYNC_PULSES,
    OT_VO_SERDES_NON_BURST_SYNC_EVENTS,
    OT_VO_SERDES_VIDEO_MODE_INVALID,
} ot_eis_vo_serdes_video_mode;

typedef enum {
    OT_VO_SERDES_PORT_0,
    OT_VO_SERDES_PORT_1,
    OT_VO_SERDES_PORT_INVALID,
} ot_eis_vo_serdes_port;

// addr is 8bit, must be even.
typedef struct {
    ot_u8 deserial_addr; // deserializer addr
} ot_eis_vo_serdes_one_port_attr;

typedef struct {
    ot_eis_vo_serdes_port serdes_port;
    ot_u8 deserial_src_addr; // deserializer addr, 0x0-0xff
    ot_u8 deserial_dst_addr; // deserializer change dst addr, 0x0-0xff, src_addr couldn't be equal to dst_addr
} ot_eis_vo_serdes_multi_port_attr;

typedef struct {
    ot_eis_vo_serdes_bus_info bus_info;
    ot_eis_vo_serdes_fpdlink_mode fpdlink_mode;
    ot_u8 serial_addr;
    union {
        // fpdlink_mode is OT_VO_SERDES_FPDLINK_MODE_SINGLE
        ot_eis_vo_serdes_one_port_attr one_port_attr;
        // fpdlink_mode is OT_VO_SERDES_FPDLINK_MODE_COPY or OT_VO_SERDES_FPDLINK_MODE_SEPARATE
        ot_eis_vo_serdes_multi_port_attr multi_port_attr[OT_EIS_VO_SERDES_PORT_MAX];
    };
    ot_eis_vo_serdes_data_fmt data_fmt;
    ot_eis_vo_serdes_clk_mode clk_mode;
    ot_eis_vo_serdes_video_mode video_mode;
    ot_void *priv_data; // User-defined parameters
} ot_eis_vo_serdes_attr;

ot_s32 ot_eis_vo_serdes_init(ot_void);
ot_s32 ot_eis_vo_serdes_deinit(ot_void);

ot_s32 ot_eis_vo_serdes_create(ot_eis_vo_serdes_handle *serdes_hdl,
    const ot_s8 *serdes_name, const ot_eis_vo_serdes_attr *serdes_attr); // serdes_attr through transmission, convenient user expansion
ot_s32 ot_eis_vo_serdes_destroy(ot_eis_vo_serdes_handle serdes_hdl);

ot_s32 ot_eis_vo_serdes_start(ot_eis_vo_serdes_handle serdes_hdl);
ot_s32 ot_eis_vo_serdes_stop(ot_eis_vo_serdes_handle serdes_hdl);
ot_s32 ot_eis_vo_serdes_restart(ot_eis_vo_serdes_handle serdes_hdl, ot_eis_vo_serdes_port port_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OT_EIS_VO_SERDES_H */