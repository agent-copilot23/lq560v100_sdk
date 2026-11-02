#include "ot_eis_vo_serdes.h"
#include "serdes_obj_mgt.h"

#define TI941_REG_SIZE              1
#define TI941_DATA_SIZE             1

#define TI948_I2C_ADDR              0x58
#define TI948_I2C_ADDR_PORT0        0x5A
#define TI948_I2C_ADDR_PORT1        0x5C
#define TI948_REG_SIZE              1
#define TI948_DATA_SIZE             1

#define LIGHTSENSOR_NUM_MAX         2

#define I2C_DETECT_CNT              200

#define ti941_port1_addr(src_addr)  (src_addr + 2)

typedef enum {
    BRD_TYPE_TM0_VERA, // TM070JDHP08
    BRD_TYPE_TM0_VERB, // TM070JDHP08
    BRD_TYPE_TM1_VERA, // SM067MPA02
    BRD_TYPE_BOE0_VERA, // AV069HDT
} serdes_brd_type;

typedef enum {
    SERDES_PORT_ID0,
    SERDES_PORT_ID1,
    SERDES_PORT_INVALID,
} serdes_port_id;

typedef struct {
    ot_u8 src_addr;
    ot_u8 dst_addr;
} light_sensor_port_attr;

typedef struct {
    ot_bool is_used;
    ot_u8 light_sensor_num;
    light_sensor_port_attr port_attr[LIGHTSENSOR_NUM_MAX];
} light_sensor;

typedef struct {
    ot_bool is_created;
    ot_bool is_started;
    ot_eis_vo_serdes_attr serdes_attr;
} vo_serdes_drv_ctrl;
static vo_serdes_drv_ctrl g_serdes_drv_ctrl[OT_EIS_VO_SERDES_ID_MAX];

#define ti941_i2c_write(bus_id, dev_addr, reg_addr, data)   \
    serdes_i2c_write(bus_id, dev_addr, reg_addr, TI941_REG_SIZE, data, TI941_DATA_SIZE)

#define ti941_i2c_read(bus_id, dev_addr, reg_addr, data)    \
    serdes_i2c_read(bus_id, dev_addr, reg_addr, TI941_REG_SIZE, data, TI941_DATA_SIZE)

#define ti948_i2c_write(bus_id, dev_addr, reg_addr, data)   \
    serdes_i2c_write(bus_id, dev_addr, reg_addr, TI948_REG_SIZE, data, TI948_DATA_SIZE)

#define ti948_i2c_read(bus_id, dev_addr, reg_addr, data)    \
    serdes_i2c_read(bus_id, dev_addr, reg_addr, TI948_REG_SIZE, data, TI948_DATA_SIZE)

static ot_bool ti948_check_is_exist(ot_s8 bus_id, ot_u8 dev_addr)
{
    ot_s32 cnt;

    cnt = I2C_DETECT_CNT;
    for (cnt = I2C_DETECT_CNT; cnt >= 0; cnt--) {
        ot_u16 data = 0x00;
        ti948_i2c_read(bus_id, dev_addr, 0x00, &data);

        if (dev_addr == TI948_I2C_ADDR && data == dev_addr) { // TI948 I2C don't change
            return OT_TRUE;
        } else if (data == (dev_addr + 0x01)) { // TI948 I2C changed
            return OT_TRUE;
        }
        serdes_mdelay(1);
    }
    serdes_err("TI948 0x%02X don't exist.\n", dev_addr);
    return OT_FALSE;
}

static ot_bool ti941_check_is_exist(ot_s8 bus_id, ot_u8 dev_addr)
{
    ot_s32 cnt;

    cnt = I2C_DETECT_CNT;
    for (cnt = I2C_DETECT_CNT; cnt >= 0; cnt--) {
        ot_u16 data = 0x00;
        ti941_i2c_read(bus_id, dev_addr, 0x00, &data);
        if (data == dev_addr) {
            return OT_TRUE;
        }
        serdes_mdelay(1);
    }
    serdes_err("TI941 0x%02X don't exist.\n", dev_addr);
    return OT_FALSE;
}

static ot_s32 ti948_change_i2c_addr(ot_s8 bus_id, ot_u8 src_addr, ot_u8 dst_addr)
{
    ot_s32 cnt;

    cnt = I2C_DETECT_CNT;
    for (cnt = I2C_DETECT_CNT; cnt >= 0; cnt--) {
        ot_u16 data = 0x00;
        ti948_i2c_read(bus_id, src_addr, 0x00, &data);
        if (src_addr == TI948_I2C_ADDR && data == src_addr) { // TI948 I2C don't change
            ti948_i2c_write(bus_id, src_addr, 0x00, dst_addr);
            return OT_SUCCESS;
        }

        ti948_i2c_read(bus_id, dst_addr, 0x00, &data);  // TI948 I2C changed
        if (data == dst_addr) {
            return OT_SUCCESS;
        }
        serdes_mdelay(1);
    }
    return OT_VO_SERDES_ERR_NO_EXIST;
}

static serdes_brd_type vo_serdes_get_brd_type(ot_s8 bus_id, ot_u8 dev_addr)
{
    ot_u16 data;
    ti948_i2c_write(bus_id, dev_addr, 0x1A, 0x03); // GPIO9 input mode
    ti948_i2c_read(bus_id, dev_addr, 0x6F, &data);
    if ((data & 0x02) == 0x00) {
        return BRD_TYPE_TM0_VERA;
    }

    ti948_i2c_write(bus_id, dev_addr, 0x1F, 0x13); // GPIO3 input mode
    ti948_i2c_write(bus_id, dev_addr, 0x20, 0x33); // GPIO5/6 input mode
    ti948_i2c_read(bus_id, dev_addr, 0x6E, &data);
    if ((data & 0x08) == 0x08) { // GPIO3 High
        return BRD_TYPE_BOE0_VERA;
    }
    if ((data & 0x20) == 0x20) { // GPIO6 High
        return BRD_TYPE_TM1_VERA;
    }
    return BRD_TYPE_TM0_VERB;
}

static ot_s32 vo_serdes_ti948_start(ot_s8 bus_id, ot_u8 dev_addr)
{
    serdes_brd_type brd_type;

    if (ti948_check_is_exist(bus_id, dev_addr) == OT_FALSE) {
        serdes_err("TI948 don't exist.\n");
        return OT_VO_SERDES_ERR_NO_EXIST;
    }
    ti948_i2c_write(bus_id, dev_addr, 0x34, 0x01); // port0 sel

    brd_type = vo_serdes_get_brd_type(bus_id, dev_addr);
    ti948_i2c_write(bus_id, dev_addr, 0x1d, 0x19); // GPIO0 output 1
    if (dev_addr != TI948_I2C_ADDR_PORT1) { // main Ver.A port1 don't support remote control
        ti948_i2c_write(bus_id, dev_addr, 0x1E, 0x15); // GPIO1 remote control, GPIO2 output 0
    } else {
        ti948_i2c_write(bus_id, dev_addr, 0x1E, 0x15); // GPIO1 remote control, GPIO2 output 0, Ver.A config 0x19
    }
    ti948_i2c_write(bus_id, dev_addr, 0x1F, 0x19); // GPIO3 output 1
    if (brd_type == BRD_TYPE_TM0_VERB) {
        ti948_i2c_write(bus_id, dev_addr, 0x20, 0x11); // GPIO5/6 output 0
    } else {
        ti948_i2c_write(bus_id, dev_addr, 0x20, 0x99); // GPIO5/6 output 1
    }
    ti948_i2c_write(bus_id, dev_addr, 0x21, 0x91); // GPIO7 output 0, GPIO8 output 1
    if (brd_type == BRD_TYPE_TM0_VERA) {
        ti948_i2c_write(bus_id, dev_addr, 0x1A, 0x11); // GPIO9 output 0
    } else {
        ti948_i2c_write(bus_id, dev_addr, 0x1A, 0x19); // GPIO9 output 1
    }

    ti948_i2c_write(bus_id, dev_addr, 0x49, 0xe3); // output mode: replicate mode
    return OT_SUCCESS;
}
static ot_void vo_serdes_lightsensor_cfg(const ot_eis_vo_serdes_attr *serdes_attr,
    serdes_port_id port_id, ot_u8 serial_addr)
{
    ot_s8 bus_id = serdes_attr->bus_info.bus_id;
    light_sensor *light_sensor_attr = serdes_attr->priv_data;

    if (light_sensor_attr != OT_NULL && light_sensor_attr->is_used) {
        ti948_i2c_write(bus_id, serial_addr, 0x07,
            light_sensor_attr->port_attr[port_id].src_addr); // 3001 i2caddr
        ti948_i2c_write(bus_id, serial_addr, 0x08,
            light_sensor_attr->port_attr[port_id].dst_addr); // 3001 mapped addr
    }
}

static ot_s32 vo_verdes_two_port_start(const ot_eis_vo_serdes_attr *serdes_attr, serdes_port_id port_id)
{
    ot_s8 bus_id = serdes_attr->bus_info.bus_id;
    ot_u8 serial_addr = serdes_attr->serial_addr;
    const ot_eis_vo_serdes_multi_port_attr *port_attr = OT_NULL;

    if (port_id == SERDES_PORT_ID0) {
        ti941_i2c_write(bus_id, serial_addr, 0x1e, 0x01); // enable port 0
    } else {
        ti941_i2c_write(bus_id, serial_addr, 0x1e, 0x07); // enable port 0 & 1
        serial_addr = ti941_port1_addr(serial_addr);
    }
    ti941_i2c_write(bus_id, serial_addr, 0x03, 0x9a); // i2c cfg
    ti941_i2c_write(bus_id, serial_addr, 0x0e, 0x03); // GPIO1 input mode

    vo_serdes_lightsensor_cfg(serdes_attr, port_id, serial_addr);

    port_attr = &(serdes_attr->multi_port_attr[port_id]);
    if (ti948_change_i2c_addr(bus_id,
        port_attr->deserial_src_addr, port_attr->deserial_dst_addr + 0x01) != OT_SUCCESS) {
        serdes_err("TI948 change addr failed.\n");
        return OT_VO_SERDES_ERR_NO_EXIST;
    }
    serdes_mdelay(1);
    return vo_serdes_ti948_start(bus_id, port_attr->deserial_dst_addr);
}

static ot_s32 vo_verdes_two_port_stop(const ot_eis_vo_serdes_attr *serdes_attr, serdes_port_id port_id)
{
    ot_s8 bus_id = serdes_attr->bus_info.bus_id;
    const ot_eis_vo_serdes_multi_port_attr *port_attr = OT_NULL;

    port_attr = &(serdes_attr->multi_port_attr[port_id]);
    if (ti948_check_is_exist(bus_id, port_attr->deserial_dst_addr) == OT_TRUE) {
        ti948_i2c_write(bus_id, port_attr->deserial_dst_addr, 0x00, 0x00); // 948 addr back
    }
    if (ti948_check_is_exist(bus_id, port_attr->deserial_src_addr) == OT_TRUE) {
        ti948_i2c_write(bus_id, port_attr->deserial_src_addr, 0x01, 0x07); // reset 948
    }
    return OT_SUCCESS;
}

static ot_s32 vo_serdes_ti941_ti948_one_port_start(const ot_eis_vo_serdes_attr *serdes_attr)
{
    ot_s8 bus_id = serdes_attr->bus_info.bus_id;
    ot_u8 serial_addr = serdes_attr->serial_addr;
    ot_u8 deserial_addr = serdes_attr->one_port_attr.deserial_addr;

    // TI941 config
    if (ti941_check_is_exist(bus_id, serial_addr) == OT_FALSE) {
        serdes_err("TI941 don't exist.\n");
        return OT_VO_SERDES_ERR_NO_EXIST;
    }
    ti941_i2c_write(bus_id, serial_addr, 0x01, 0x08); // dsi disable
    ti941_i2c_write(bus_id, serial_addr, 0x1e, 0x01); // enable port0
    ti941_i2c_write(bus_id, serial_addr, 0x03, 0x9a); // i2c cfg
    ti941_i2c_write(bus_id, serial_addr, 0x0e, 0x03); // GPIO1 input mode
    ti941_i2c_write(bus_id, serial_addr, 0x4f, 0x8C); // continue clock
    ti941_i2c_write(bus_id, serial_addr, 0x56, 0x00); // DSI 参考时钟
    ti941_i2c_write(bus_id, serial_addr, 0x02, 0x07); // lane swap
    if (serdes_attr->video_mode != OT_VO_SERDES_BURST) {
        ti941_i2c_write(bus_id, serial_addr, 0x40, 0x04); // dsi port0
        ti941_i2c_write(bus_id, serial_addr, 0x41, 0x20); // Select DSI_CONFIG_0 register
        ti941_i2c_write(bus_id, serial_addr, 0x42, 0x6F); // set DSI_SYNC_PULSES=0

        ti941_i2c_write(bus_id, serial_addr, 0x41, 0x31); // Select DSI_CONFIG_0 register
        ti941_i2c_write(bus_id, serial_addr, 0x42, 0x03); // set DSI_SYNC_PULSES=0
    }
    ti941_i2c_write(bus_id, serial_addr, 0x01, 0x00); // dsi enable

    vo_serdes_lightsensor_cfg(serdes_attr, SERDES_PORT_ID0, serial_addr);
    serdes_mdelay(1);
    return vo_serdes_ti948_start(bus_id, deserial_addr);
}

static ot_s32 vo_serdes_ti941_ti948_one_port_stop(const ot_eis_vo_serdes_attr *serdes_attr)
{
    ot_s8 bus_id = serdes_attr->bus_info.bus_id;
    ot_u8 serial_addr = serdes_attr->serial_addr;
    ot_u8 deserial_addr = serdes_attr->one_port_attr.deserial_addr;

    if (ti948_check_is_exist(bus_id, deserial_addr) == OT_TRUE) {
        ti948_i2c_write(bus_id, deserial_addr, 0x01, 0x07); // reset 948
    }
    if (ti941_check_is_exist(bus_id, serial_addr) == OT_TRUE) {
        ti941_i2c_write(bus_id, serial_addr, 0x01, 0x0F); // reset 941
    }
    return OT_SUCCESS;
}

static ot_s32 vo_serdes_ti941_ti948_two_port_copy_start(const ot_eis_vo_serdes_attr *serdes_attr)
{
    ot_s8 bus_id = serdes_attr->bus_info.bus_id;
    ot_u8 serial_addr = serdes_attr->serial_addr;

    // TI941 config
    if (ti941_check_is_exist(bus_id, serial_addr) == OT_FALSE) {
        serdes_err("TI941 don't exist.\n");
        return OT_VO_SERDES_ERR_NO_EXIST;
    }
    ti941_i2c_write(bus_id, serial_addr, 0x01, 0x08); // dsi disable
    ti941_i2c_write(bus_id, serial_addr, 0x1e, 0x01); // enable port0

    ti941_i2c_write(bus_id, serial_addr, 0x02, 0x07); // lane swap
    ti941_i2c_write(bus_id, serial_addr, 0x4f, 0x8C); // continue clock
    ti941_i2c_write(bus_id, serial_addr, 0x5b, 0x04); // Auto single or copy mode
    ti941_i2c_write(bus_id, serial_addr, 0x56, 0x00); // DSI 参考时钟

    if (serdes_attr->video_mode != OT_VO_SERDES_BURST) {
        ti941_i2c_write(bus_id, serial_addr, 0x40, 0x04); // dsi port0
        ti941_i2c_write(bus_id, serial_addr, 0x41, 0x20); // Select DSI_CONFIG_0 register
        ti941_i2c_write(bus_id, serial_addr, 0x42, 0x6F); // set DSI_SYNC_PULSES=0

        ti941_i2c_write(bus_id, serial_addr, 0x41, 0x31); // Select DSI_CONFIG_0 register
        ti941_i2c_write(bus_id, serial_addr, 0x42, 0x03); // set DSI_SYNC_PULSES=0
    }

    // TI941 port 0 config
    vo_verdes_two_port_start(serdes_attr, SERDES_PORT_ID0);

    //  TI941 port 1 config
    vo_verdes_two_port_start(serdes_attr, SERDES_PORT_ID1);

    ti941_i2c_write(bus_id, serial_addr, 0x01, 0x00); // dsi enable
    return OT_SUCCESS;
}

static ot_s32 vo_serdes_ti941_ti948_two_port_copy_stop(const ot_eis_vo_serdes_attr *serdes_attr)
{
    ot_s8 bus_id = serdes_attr->bus_info.bus_id;
    ot_u8 serial_addr = serdes_attr->serial_addr;

    vo_verdes_two_port_stop(serdes_attr, SERDES_PORT_ID0);
    vo_verdes_two_port_stop(serdes_attr, SERDES_PORT_ID1);

    if (ti941_check_is_exist(bus_id, serial_addr) == OT_TRUE) {
        ti941_i2c_write(bus_id, serial_addr, 0x1E, 0x01); // 941 sel port 0
        ti941_i2c_write(bus_id, serial_addr, 0x01, 0x0F); // reset 941
    }
    return OT_SUCCESS;
}

static ot_s32 vo_serdes_ti941_ti948_two_port_sep_start(const ot_eis_vo_serdes_attr *serdes_attr)
{
    ot_s8 bus_id = serdes_attr->bus_info.bus_id;
    ot_u8 serial_addr = serdes_attr->serial_addr;

    // TI941 config
    if (ti941_check_is_exist(bus_id, serial_addr) == OT_FALSE) {
        serdes_err("TI941 don't exist.\n");
        return OT_VO_SERDES_ERR_NO_EXIST;
    }
    ti941_i2c_write(bus_id, serial_addr, 0x01, 0x08); // dsi disable
    ti941_i2c_write(bus_id, serial_addr, 0x1e, 0x01); // enable port0

    ti941_i2c_write(bus_id, serial_addr, 0x02, 0x07); // lane swap
    ti941_i2c_write(bus_id, serial_addr, 0x4f, 0x8C); // continue clock
    ti941_i2c_write(bus_id, serial_addr, 0x5b, 0x07); // force Splitter mode
    ti941_i2c_write(bus_id, serial_addr, 0x56, 0x80); // L/R Pixel Processing & DSI 参考时钟
    ti941_i2c_write(bus_id, serial_addr, 0x32, 0x00); // 2D Line Size 1280
    ti941_i2c_write(bus_id, serial_addr, 0x33, 0x05);

    ti941_i2c_write(bus_id, serial_addr, 0x40, 0x04); // Select DSI Port 0 digital registers
    ti941_i2c_write(bus_id, serial_addr, 0x41, 0x05); // Select DPHY_SKIP_TIMING register
    ti941_i2c_write(bus_id, serial_addr, 0x42, 0x30); // Write TSKIP_CNT value for 450 MHz DSI clock frequency

    ti941_i2c_write(bus_id, serial_addr, 0x41, 0x20); // Select DSI_CONFIG_0 register
    ti941_i2c_write(bus_id, serial_addr, 0x42, 0x6F); // set DSI_SYNC_PULSES=0

    // TI941 port 0 config
    vo_verdes_two_port_start(serdes_attr, SERDES_PORT_ID0);

    //  TI941 port 1 config
    vo_verdes_two_port_start(serdes_attr, SERDES_PORT_ID1);

    ti941_i2c_write(bus_id, serial_addr, 0x01, 0x00); // dsi enable
    return OT_SUCCESS;
}

static ot_s32 vo_serdes_ti941_ti948_two_port_sep_stop(const ot_eis_vo_serdes_attr *serdes_attr)
{
    return vo_serdes_ti941_ti948_two_port_copy_stop(serdes_attr);
}

static ot_s32 ti941_ti948_get_idle_id(ot_void)
{
    ot_s32 i;

    for (i = 0; i < OT_EIS_VO_SERDES_ID_MAX; i++) {
        if (g_serdes_drv_ctrl[i].is_created == OT_FALSE) {
            break;
        }
    }
    return i;
}

static ot_s32 ti941_ti948_create(ot_s32 *serdes_id, const ot_serdes_attr *serdes_attr)
{
    ot_s32 id;
    vo_serdes_drv_ctrl *drv_ctrl = OT_NULL;

    id = ti941_ti948_get_idle_id();
    serdes_chk_data_ge_return(id, OT_EIS_VO_SERDES_ID_MAX, OT_VO_SERDES_ERR_OVER_SIZE);

    drv_ctrl = &(g_serdes_drv_ctrl[id]);

    drv_ctrl->is_created = OT_TRUE;
    drv_ctrl->is_started = OT_FALSE;
    memcpy(&(drv_ctrl->serdes_attr), (ot_eis_vo_serdes_attr *)serdes_attr, sizeof(ot_eis_vo_serdes_attr));
    *serdes_id = id;
    return OT_SUCCESS;
}

static ot_s32 ti941_ti948_destroy(ot_s32 serdes_id)
{
    vo_serdes_drv_ctrl *drv_ctrl = OT_NULL;

    serdes_chk_data_ge_return(serdes_id, OT_EIS_VO_SERDES_ID_MAX, OT_VO_SERDES_ERR_OVER_SIZE);

    drv_ctrl = &(g_serdes_drv_ctrl[serdes_id]);

    if (drv_ctrl->is_created == OT_FALSE) {
        serdes_err("Serdes id don't create.\n");
        return OT_VO_SERDES_ERR_NO_EXIST;
    }

    if (drv_ctrl->is_started == OT_TRUE) {
        serdes_err("Serdes id don't stop.\n");
        return OT_VO_SERDES_ERR_NO_START;
    }

    memset(drv_ctrl, 0, sizeof(vo_serdes_drv_ctrl));
    return OT_SUCCESS;
}

static ot_s32 ti941_ti948_start(ot_s32 serdes_id)
{
    vo_serdes_drv_ctrl *drv_ctrl = OT_NULL;

    serdes_chk_data_ge_return(serdes_id, OT_EIS_VO_SERDES_ID_MAX, OT_VO_SERDES_ERR_OVER_SIZE);

    drv_ctrl = &(g_serdes_drv_ctrl[serdes_id]);

    if (drv_ctrl->is_created == OT_FALSE) {
        serdes_err("Serdes id don't create.\n");
        return OT_VO_SERDES_ERR_NO_EXIST;
    }

    if (drv_ctrl->is_started == OT_TRUE) {
        serdes_err("Serdes id had started.\n");
        return OT_VO_SERDES_ERR_NO_STOP;
    }

    if (drv_ctrl->serdes_attr.bus_info.bus_id == -1) {
        drv_ctrl->is_started = OT_TRUE;
        return OT_SUCCESS;
    }

    if (drv_ctrl->serdes_attr.fpdlink_mode == OT_VO_SERDES_FPDLINK_MODE_SINGLE) {
        vo_serdes_ti941_ti948_one_port_start(&(drv_ctrl->serdes_attr));
    } else if (drv_ctrl->serdes_attr.fpdlink_mode == OT_VO_SERDES_FPDLINK_MODE_COPY) {
        vo_serdes_ti941_ti948_two_port_copy_start(&(drv_ctrl->serdes_attr));
    } else if (drv_ctrl->serdes_attr.fpdlink_mode == OT_VO_SERDES_FPDLINK_MODE_SEPARATE) {
        vo_serdes_ti941_ti948_two_port_sep_start(&(drv_ctrl->serdes_attr));
    }

    drv_ctrl->is_started = OT_TRUE;
    return OT_SUCCESS;
}

static ot_s32 ti941_ti948_stop(ot_s32 serdes_id)
{
    vo_serdes_drv_ctrl *drv_ctrl = OT_NULL;

    serdes_chk_data_ge_return(serdes_id, OT_EIS_VO_SERDES_ID_MAX, OT_VO_SERDES_ERR_OVER_SIZE);

    drv_ctrl = &(g_serdes_drv_ctrl[serdes_id]);

    if (drv_ctrl->is_created == OT_FALSE) {
        serdes_err("Serdes id don't create.\n");
        return OT_VO_SERDES_ERR_NO_EXIST;
    }

    if (drv_ctrl->is_started == OT_FALSE) {
        serdes_err("Serdes id don't start.\n");
        return OT_VO_SERDES_ERR_NO_START;
    }

    if (drv_ctrl->serdes_attr.bus_info.bus_id == -1) {
        drv_ctrl->is_started = OT_FALSE;
        return OT_SUCCESS;
    }

    if (drv_ctrl->serdes_attr.fpdlink_mode == OT_VO_SERDES_FPDLINK_MODE_SINGLE) {
        vo_serdes_ti941_ti948_one_port_stop(&(drv_ctrl->serdes_attr));
    } else if (drv_ctrl->serdes_attr.fpdlink_mode == OT_VO_SERDES_FPDLINK_MODE_COPY) {
        vo_serdes_ti941_ti948_two_port_copy_stop(&(drv_ctrl->serdes_attr));
    } else if (drv_ctrl->serdes_attr.fpdlink_mode == OT_VO_SERDES_FPDLINK_MODE_SEPARATE) {
        vo_serdes_ti941_ti948_two_port_sep_stop(&(drv_ctrl->serdes_attr));
    }

    drv_ctrl->is_started = OT_FALSE;
    return OT_SUCCESS;
}

static ot_s32 ti941_ti948_restart(ot_s32 serdes_id, ot_u32 port_id)
{
    vo_serdes_drv_ctrl *drv_ctrl = OT_NULL;
    ot_eis_vo_serdes_multi_port_attr *port_attr = OT_NULL;
    ot_s8 bus_id;
    ot_u8 serial_addr;
    ot_u8 deserial_addr;

    serdes_chk_data_ge_return(serdes_id, OT_EIS_VO_SERDES_ID_MAX, OT_VO_SERDES_ERR_OVER_SIZE);

    drv_ctrl = &(g_serdes_drv_ctrl[serdes_id]);

    if (drv_ctrl->is_created == OT_FALSE) {
        serdes_err("Serdes id don't create.\n");
        return OT_VO_SERDES_ERR_NO_EXIST;
    }

    if (drv_ctrl->is_started != OT_TRUE) {
        serdes_err("Serdes id not started.\n");
        return OT_VO_SERDES_ERR_NO_START;
    }

    if (drv_ctrl->serdes_attr.bus_info.bus_id == -1) {
        return OT_SUCCESS;
    }

    bus_id = drv_ctrl->serdes_attr.bus_info.bus_id;
    deserial_addr = drv_ctrl->serdes_attr.one_port_attr.deserial_addr;
    serial_addr = drv_ctrl->serdes_attr.serial_addr;

    if (drv_ctrl->serdes_attr.fpdlink_mode != OT_VO_SERDES_FPDLINK_MODE_SINGLE) {
        port_attr = &(drv_ctrl->serdes_attr.multi_port_attr[port_id]);
        ti941_i2c_write(bus_id, serial_addr, 0x1e, 0x1 +port_id*6);
        if (ti948_change_i2c_addr(bus_id,
            port_attr->deserial_src_addr, port_attr->deserial_dst_addr + 0x01) != OT_SUCCESS) {
            serdes_err("TI948 change addr failed.\n");
            return OT_VO_SERDES_ERR_NO_EXIST;
        }
        ti941_i2c_write(bus_id, serial_addr, 0x1e, 0x07);
        deserial_addr = port_attr->deserial_dst_addr;
    }
    serdes_mdelay(1);
    vo_serdes_ti948_start(bus_id, deserial_addr);
    return OT_SUCCESS;
}

ot_serdes_obj g_ti941_ti948_obj = {
    .serdes_name = "TI941,TI948",
    .create      = ti941_ti948_create,
    .destroy     = ti941_ti948_destroy,
    .start       = ti941_ti948_start,
    .stop        = ti941_ti948_stop,
    .restart     = ti941_ti948_restart,
};

const ot_serdes_obj *ti941_ti948_get_vo_serdes_obj(ot_void)
{
    return &g_ti941_ti948_obj;
}
