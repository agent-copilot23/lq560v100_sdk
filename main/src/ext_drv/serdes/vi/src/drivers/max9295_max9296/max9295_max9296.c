#include "ot_eis_vi_serdes.h"
#include "serdes_obj_mgt.h"

#define MAX9296_BRD_BUS_ID          6

#define MAX9295_REG_SIZE            2
#define MAX9295_DATA_SIZE           1

#define MAX9296_REG_SIZE            2
#define MAX9296_DATA_SIZE           1
#define MAX9296_FREQ_100MHZ_MAX     20

#define MAX2008X_BUS_ID             0
#define MAX2008X_I2C_ADDR           0x50
#define MAX2008X_REG_SIZE           1
#define MAX2008X_DATA_SIZE          1

#define I2C_DETECT_CNT              200

#define SENSOR_NUMBERS_1            1
#define SENSOR_NUMBERS_2            2

typedef enum {
    SERDES_PIPE_X,
    SERDES_PIPE_Y,
    SERDES_PIPE_Z,
    SERDES_PIPE_U,
} serdes_pipe;

typedef struct {
    ot_bool is_created;
    ot_bool is_started;
    ot_eis_vi_serdes_attr serdes_attr;
} vi_serdes_drv_ctrl;
static vi_serdes_drv_ctrl g_serdes_drv_ctrl[OT_EIS_VI_SERDES_ID_MAX];

#define max9295_i2c_write(bus_id, dev_addr, reg_addr, data)       \
    serdes_i2c_write(bus_id, dev_addr, reg_addr, MAX9295_REG_SIZE, data, MAX9295_DATA_SIZE)

#define max9295_i2c_read(bus_id, dev_addr, reg_addr, data)        \
    serdes_i2c_read(bus_id, dev_addr, reg_addr, MAX9295_REG_SIZE, data, MAX9295_DATA_SIZE)

#define max9296_i2c_write(bus_id, dev_addr, reg_addr, data)       \
    serdes_i2c_write(bus_id, dev_addr, reg_addr, MAX9296_REG_SIZE, data, MAX9296_DATA_SIZE)

#define max9296_i2c_read(bus_id, dev_addr, reg_addr, data)        \
    serdes_i2c_read(bus_id, dev_addr, reg_addr, MAX9296_REG_SIZE, data, MAX9296_DATA_SIZE)

#define max2008x_i2c_write(reg_addr, data)              \
    serdes_i2c_write(MAX2008X_BUS_ID, MAX2008X_I2C_ADDR, reg_addr, MAX2008X_REG_SIZE, data, MAX2008X_DATA_SIZE)

#define max2008x_i2c_read(reg_addr, data)               \
    serdes_i2c_read(MAX2008X_BUS_ID, MAX2008X_I2C_ADDR, reg_addr, MAX2008X_REG_SIZE, data, MAX2008X_DATA_SIZE)

#define max9296_freq_100mhz_roundup(freq_mhz)           \
    (((freq_mhz) % 100) ? (((freq_mhz) / 100) + 1) : (freq_mhz) / 100)

static ot_void vi_serdes_x3f_adapt(ot_u8 bus_id, ot_u8 serial_addr, ot_bool *is_x3f)
{
    if ((is_x3f != OT_NULL) && (*is_x3f == OT_TRUE)) {
        max9295_i2c_write(bus_id, serial_addr, 0x02CA, 0x80); // gpio4 0
        max9295_i2c_write(bus_id, serial_addr, 0x056F, 0x1e);
        max9295_i2c_write(bus_id, serial_addr, 0x03F1, 0x05); // gpio2 pin select for pclk output
        max9295_i2c_write(bus_id, serial_addr, 0x03F0, 0x59); // generation pll, 24MHz
        serdes_mdelay(20);
        max9295_i2c_write(bus_id, serial_addr, 0x02CA, 0x90);
    } else {
        max9295_i2c_write(bus_id, serial_addr,  0x0383, 0x00); // pixel mode
        max9295_i2c_write(bus_id, serial_addr,  0x0003, 0x03); // select RCLK to gpio4
        max9295_i2c_write(bus_id, serial_addr,  0x0006, 0xb0); // RCLK output enable
        max9295_i2c_write(bus_id, serial_addr,  0x03f0, 0x59); // generation pll, 24MHz
        max9295_i2c_write(bus_id, serial_addr,  0x0570, 0x0c);
    }
}

static ot_s32 max9295_max9296_data_type(ot_eis_vi_serdes_data_fmt data_fmt, ot_u8 *data_type)
{
    switch (data_fmt) {
        case OT_VI_SERDES_DATA_FMT_RAW_8BIT:    *data_type = 0x2A; break;
        case OT_VI_SERDES_DATA_FMT_RAW_10BIT:   *data_type = 0x2B; break;
        case OT_VI_SERDES_DATA_FMT_RAW_12BIT:   *data_type = 0x2C; break;
        case OT_VI_SERDES_DATA_FMT_RAW_14BIT:   *data_type = 0x2D; break;
        case OT_VI_SERDES_DATA_FMT_RAW_16BIT:   *data_type = 0x2E; break;
        case OT_VI_SERDES_DATA_FMT_YUV422_8BIT: *data_type = 0x1E; break;
        default:
            serdes_err("Invalid data format %d.\n", data_fmt);
            return OT_VI_SERDES_ERR_INVALID_PARAM;
    }
    return OT_SUCCESS;
}

static ot_bool max9296_check_is_exist(ot_s8 bus_id, ot_u8 dev_addr)
{
    ot_s32 cnt;

    cnt = I2C_DETECT_CNT;
    for (cnt = I2C_DETECT_CNT; cnt >= 0; cnt--) {
        ot_u16 data = 0x00;
        max9296_i2c_read(bus_id, dev_addr, 0x00, &data);
        if (data == dev_addr) {
            return OT_TRUE;
        }
        serdes_mdelay(1);
    }
    serdes_err("MAX9296 don't exist.\n");
    return OT_FALSE;
}

static ot_bool max9295_check_is_exist(ot_s8 bus_id, ot_u8 dev_addr)
{
    ot_s32 cnt;

    cnt = I2C_DETECT_CNT;

    for (cnt = I2C_DETECT_CNT; cnt >= 0; cnt--) {
        ot_u16 data = 0x00;
        max9295_i2c_read(bus_id, dev_addr, 0x00, &data);
        if (data == dev_addr) {
            return OT_TRUE;
        }
        serdes_mdelay(1);
    }
    serdes_err("MAX9295 0x%02X don't exist.\n", dev_addr);
    return OT_FALSE;
}

static ot_bool max9295_check_and_change_addr(ot_s8 bus_id, ot_u8 src_addr, ot_u8 dst_addr)
{
    ot_s32 cnt;

    cnt = I2C_DETECT_CNT;
    for (cnt = I2C_DETECT_CNT; cnt >= 0; cnt--) {
        ot_u16 data = 0x00;
        max9295_i2c_read(bus_id, src_addr, 0x00, &data);
        if (data == src_addr) {
            max9295_i2c_write(bus_id, src_addr, 0x0000, dst_addr); // change i2c addr
            return OT_TRUE;
        }
        max9295_i2c_read(bus_id, dst_addr, 0x00, &data);
        if (data == dst_addr) {
            return OT_TRUE;
        }
        serdes_mdelay(1);
    }
    serdes_err("MAX9295 0x%02X or 0x%02X don't exist.\n", src_addr, dst_addr);
    return OT_FALSE;
}


// serializer power supply is controlled by MAX2008X
static ot_void max9295_power_ctrl(ot_bool enable)
{
    ot_u16 data;
    max2008x_i2c_read(0x01, &data);
    if (enable) {
        max2008x_i2c_write(0x01, data | 0x0f);
    } else {
        max2008x_i2c_write(0x01, data & 0xf0);
    }
}

static ot_s32 max9295_linear_two_sns_cfg(const ot_eis_vi_serdes_attr *serdes_attr,
    ot_eis_vi_serdes_port serdes_port, ot_u8 data_type)
{
    ot_u8 tmp;
    ot_u8 bus_id = serdes_attr->bus_info.bus_id;
    const ot_eis_vi_serdes_multi_port_attr *port_attr = &(serdes_attr->multi_port_attr[serdes_port]);
    ot_bool is_x3f = OT_FALSE;

    if (max9295_check_is_exist(bus_id, port_attr->serial_src_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    if (max9295_check_and_change_addr(bus_id,
        port_attr->serial_src_addr, port_attr->serial_dst_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    if (max9295_check_is_exist(bus_id, port_attr->serial_dst_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    // config source id
    tmp = (serdes_port == OT_VI_SERDES_PORT_0) ? 0x41 : 0x42;
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x007B, tmp);
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x0083, tmp);
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x008B, tmp);
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x0093, tmp);
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x00A3, tmp);
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x00AB, tmp);

    // reset sensor i2c addr
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x0042, port_attr->sns_dst_addr);
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x0043, port_attr->sns_src_addr);

    // set PipeZ stream ID
    tmp = (serdes_port == OT_VI_SERDES_PORT_0) ? 0x11 : 0x12;
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x005B, tmp);

    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x0002, 0x43); // enable PipeZ
    max9295_i2c_write(bus_id,
        port_attr->serial_dst_addr, 0x0318, 0x40 | data_type); // data type
    max9295_i2c_write(bus_id,
        port_attr->serial_dst_addr, 0x0331, 0x03 | ((serdes_attr->sns_lanes - 1) << 4)); // lane nums
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x02BE, 0x00); // GPIO0 0, sensor reset
    serdes_mdelay(20);
    max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x02BE, 0x10); // GPIO0 1, sensor unreset
    serdes_mdelay(20); // wait sensor work stability

    if(serdes_attr->priv_data != OT_NULL) {
        is_x3f = (*((ot_u8*)serdes_attr->priv_data)) & 0x01;
    }

    vi_serdes_x3f_adapt(bus_id, port_attr->serial_dst_addr, &is_x3f);
    return OT_SUCCESS;
}

static ot_void max9296_pipe_remap(const ot_eis_vi_serdes_attr *serdes_attr,
    serdes_pipe pipe, ot_u8 src_vc, ot_u8 dst_vc, ot_u8 data_type)
{
    ot_u16 addr_offset = 0x40 * pipe;
    ot_u8 bus_id, deserial_addr;

    bus_id = serdes_attr->bus_info.bus_id;
    deserial_addr = serdes_attr->deserial_addr;
    max9296_i2c_write(bus_id, deserial_addr, 0x040B + addr_offset, 0x07);
    max9296_i2c_write(bus_id, deserial_addr, 0x040C + addr_offset, 0x00);
    max9296_i2c_write(bus_id, deserial_addr, 0x040D + addr_offset, (src_vc << 6) | data_type); // MAP DT
    max9296_i2c_write(bus_id, deserial_addr, 0x040E + addr_offset, (dst_vc << 6) | data_type);
    max9296_i2c_write(bus_id, deserial_addr, 0x040F + addr_offset, (src_vc << 6) | 0x00); // MAP FS
    max9296_i2c_write(bus_id, deserial_addr, 0x0410 + addr_offset, (dst_vc << 6) | 0x00);
    max9296_i2c_write(bus_id, deserial_addr, 0x0411 + addr_offset, (src_vc << 6) | 0x01); // MAP FE
    max9296_i2c_write(bus_id, deserial_addr, 0x0412 + addr_offset, (dst_vc << 6) | 0x01);
    max9296_i2c_write(bus_id, deserial_addr, 0x042D + addr_offset, 0x15); // Map PHY B
}

static ot_bool seders_sns_is_sc220(ot_void *priv_data)
{
    ot_bool sns_sc220 = OT_FALSE;

    if(priv_data == OT_NULL) {
        return OT_FALSE;
    }

    sns_sc220 = ((*((ot_u8*)priv_data)) & (0x02)) == (0x02) ? OT_TRUE : OT_FALSE;
    return sns_sc220;
}

static ot_s32 max9295_max9296_linear_two_sns_start(const ot_eis_vi_serdes_attr *serdes_attr)
{
    ot_s32 i, ret;
    ot_u32 freq_100mhz;
    ot_u8 data_type, bus_id, deserial_addr;

    bus_id = serdes_attr->bus_info.bus_id;
    deserial_addr = serdes_attr->deserial_addr;
    if (max9296_check_is_exist(bus_id, deserial_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    ret = max9295_max9296_data_type(serdes_attr->data_fmt, &data_type);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    // max9296 supports a maximum of 2.0Gbps/lane
    freq_100mhz = max9296_freq_100mhz_roundup(serdes_attr->freq_mhz);
    serdes_chk_data_gt_return(freq_100mhz, MAX9296_FREQ_100MHZ_MAX, OT_VI_SERDES_ERR_OVER_SIZE);

    max9296_i2c_write(bus_id, deserial_addr, 0x0313, 0x00); // disable mipi output

    if(seders_sns_is_sc220(serdes_attr->priv_data)) {
        max9296_i2c_write(bus_id, deserial_addr, 0x0001, 0x01); // 3Gbps
    } else {
        max9296_i2c_write(bus_id, deserial_addr, 0x0001, 0x02); // 6Gbps
    }

    for (i = 0; i < serdes_attr->sns_nums; i++) {
        ot_eis_vi_serdes_port serdes_port = serdes_attr->multi_port_attr[i].serdes_port;

        serdes_chk_data_gt_return(serdes_port, OT_VI_SERDES_PORT_2, OT_EIS_ERRNO_ILLEGAL_PARAM);

        max9296_i2c_write(bus_id, deserial_addr, 0x0010, 0x21 + serdes_port); // link port
        serdes_mdelay(50);
        ret = max9295_linear_two_sns_cfg(serdes_attr, serdes_port, data_type);
        if (ret != OT_SUCCESS) {
            serdes_err("MAX9295 port%d config failed.\n", i);
            return ret;
        }
    }

    max9296_i2c_write(bus_id, deserial_addr, 0x0010, 0x23); // link port0 & port1
    max9296_i2c_write(bus_id, deserial_addr, 0x0050, 0x01); // PipeX stream ID 1
    max9296_i2c_write(bus_id, deserial_addr, 0x0051, 0x02); // PipeY stream ID 2

    // PipeX VC0 -> PHY B vc_id
    max9296_pipe_remap(serdes_attr, SERDES_PIPE_X, 0,
        serdes_attr->multi_port_attr[0].vc_id[0], data_type);

    // PipeY VC0 -> PHY B vc_id
    max9296_pipe_remap(serdes_attr, SERDES_PIPE_Y, 0,
        serdes_attr->multi_port_attr[1].vc_id[0], data_type);

    max9296_i2c_write(bus_id, deserial_addr, 0x1D00, 0xF4); // phy reset
    max9296_i2c_write(bus_id, deserial_addr, 0x0320, 0x20 + freq_100mhz); // (1 ~ 0x1F) * 100Mhz
    max9296_i2c_write(bus_id, deserial_addr, 0x1D00, 0xF5);

    max9296_i2c_write(bus_id, deserial_addr, 0x0332, 0x30); // Turn on PHY0 and PHY1
    if (bus_id == MAX9296_BRD_BUS_ID) {
        max9296_i2c_write(bus_id, deserial_addr, 0x0333, 0xB4); // set lane mapping
        max9296_i2c_write(bus_id, deserial_addr, 0x0335, 0x3F); // set lane polarity
    }
    max9296_i2c_write(bus_id, deserial_addr, 0x044A, 0xD0); // 0xD0 - 4lane, 0x50 - 2lane
    max9296_i2c_write(bus_id, deserial_addr, 0x0002, 0x33); // enable PipeX & PipeY

    max9296_i2c_write(bus_id, deserial_addr, 0x0313, 0x02); // enable mipi output
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_linear_two_sns_stop(const ot_eis_vi_serdes_attr *serdes_attr)
{
    ot_s32 i;
    ot_u8 bus_id, deserial_addr;
    const ot_eis_vi_serdes_multi_port_attr *port_attr = OT_NULL;

    bus_id = serdes_attr->bus_info.bus_id;
    deserial_addr = serdes_attr->deserial_addr;
    for (i = serdes_attr->sns_nums - 1; i >= 0; i--) {
        port_attr = &(serdes_attr->multi_port_attr[i]);
        if (max9295_check_is_exist(bus_id, port_attr->serial_dst_addr) == OT_TRUE) {
            max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x02BE, 0x00); // GPIO0 0
            max9295_i2c_write(bus_id, port_attr->serial_dst_addr, 0x0010, 0x80);
        }
        serdes_mdelay(50);
    }
    max9296_i2c_write(bus_id, deserial_addr, 0x0010, 0x80);
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_linear_one_sns_start(const ot_eis_vi_serdes_attr *serdes_attr)
{
    ot_s32 ret;
    ot_u32 freq_100mhz;
    ot_u8 data_type, bus_id, deserial_addr, serial_addr;
    ot_bool is_x3f = OT_FALSE;

    bus_id = serdes_attr->bus_info.bus_id;
    serial_addr = serdes_attr->one_port_attr.serial_addr;
    deserial_addr = serdes_attr->deserial_addr;
    if (max9296_check_is_exist(bus_id, deserial_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    ret = max9295_max9296_data_type(serdes_attr->data_fmt, &data_type);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    // max9296 supports a maximum of 2.0Gbps/lane
    freq_100mhz = max9296_freq_100mhz_roundup(serdes_attr->freq_mhz);
    serdes_chk_data_gt_return(freq_100mhz, MAX9296_FREQ_100MHZ_MAX, OT_VI_SERDES_ERR_OVER_SIZE);

    max9296_i2c_write(bus_id, deserial_addr, 0x0313, 0x00); // disable mipi output

    if(seders_sns_is_sc220(serdes_attr->priv_data)) {
        max9296_i2c_write(bus_id, deserial_addr, 0x0001, 0x01); // 3Gbps
    } else {
        max9296_i2c_write(bus_id, deserial_addr, 0x0001, 0x02); // 6Gbps
    }

    max9296_i2c_write(bus_id, deserial_addr, 0x1D00, 0xF4); // phy reset
    max9296_i2c_write(bus_id, deserial_addr, 0x0320, 0x20 + freq_100mhz); // (1 ~ 0x1F) * 100Mhz
    max9296_i2c_write(bus_id, deserial_addr, 0x1D00, 0xF5);

    max9296_i2c_write(bus_id, deserial_addr, 0x0332, 0x30); // Turn on PHY0 and PHY1
    if (bus_id == MAX9296_BRD_BUS_ID) {
        max9296_i2c_write(bus_id, deserial_addr, 0x0333, 0xB4); // set lane mapping
        max9296_i2c_write(bus_id, deserial_addr, 0x0335, 0x3F); // set lane polarity
    }
    max9296_i2c_write(bus_id, deserial_addr, 0x044A, 0xD0); // 0xD0 - 4lane, 0x50 - 2lane
    max9296_i2c_write(bus_id, deserial_addr, 0x0051, 0x02); // swap pipeline Y/Z
    max9296_i2c_write(bus_id, deserial_addr, 0x0052, 0x01);
    max9296_i2c_write(bus_id, deserial_addr, 0x0002, 0x23); // enable pipeline Y

    serdes_mdelay(50); // wait max9295 power up stability
    if (max9295_check_is_exist(bus_id, serial_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    max9295_i2c_write(bus_id,
        serial_addr, 0x0331, 0x03 | ((serdes_attr->sns_lanes - 1) << 4)); // 2lane
    max9295_i2c_write(bus_id, serial_addr, 0x0318, 0x40 | data_type); // data type
    max9295_i2c_write(bus_id, serial_addr, 0x02BE, 0x00); // GPIO0 0, sensor reset
    serdes_mdelay(20);
    max9295_i2c_write(bus_id, serial_addr, 0x02BE, 0x10); // GPIO0 1, sensor unreset
    serdes_mdelay(20); // wait sensor work stability

    if(serdes_attr->priv_data != OT_NULL) {
        is_x3f = (*((ot_u8*)serdes_attr->priv_data)) & 0x01;
    }

    vi_serdes_x3f_adapt(bus_id, serial_addr, &is_x3f);
    max9296_i2c_write(bus_id, deserial_addr, 0x0313, 0x02); // enable mipi output
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_linear_one_sns_stop(const ot_eis_vi_serdes_attr *serdes_attr)
{
    ot_u8 bus_id, deserial_addr, serial_addr;

    bus_id = serdes_attr->bus_info.bus_id;
    serial_addr = serdes_attr->one_port_attr.serial_addr;
    deserial_addr = serdes_attr->deserial_addr;

    max9295_i2c_write(bus_id, serial_addr, 0x02BE, 0x00); // GPIO0 0
    max9295_i2c_write(bus_id, serial_addr, 0x0010, 0x80);
    max9296_i2c_write(bus_id, deserial_addr, 0x0010, 0x80);
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_wdr_vc_2to1_one_sns_start(const ot_eis_vi_serdes_attr *serdes_attr)
{
    ot_s32 ret;
    ot_u32 freq_100mhz;
    ot_u8 data_type, bus_id, deserial_addr, serial_addr;

    bus_id = serdes_attr->bus_info.bus_id;
    serial_addr = serdes_attr->one_port_attr.serial_addr;
    deserial_addr = serdes_attr->deserial_addr;

    if (max9296_check_is_exist(bus_id, deserial_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    ret = max9295_max9296_data_type(serdes_attr->data_fmt, &data_type);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    // max9296 supports a maximum of 2.0Gbps/lane
    freq_100mhz = max9296_freq_100mhz_roundup(serdes_attr->freq_mhz);
    serdes_chk_data_gt_return(freq_100mhz, MAX9296_FREQ_100MHZ_MAX, OT_VI_SERDES_ERR_OVER_SIZE);

    // INSTRUCTIONS FOR DESERIALIZER MAX9296A
    max9296_i2c_write(bus_id, deserial_addr, 0x0313, 0x00); // CSI output disabled

    if(seders_sns_is_sc220(serdes_attr->priv_data)) {
        max9296_i2c_write(bus_id, deserial_addr, 0x0001, 0x01); // 3Gbps
    } else {
        max9296_i2c_write(bus_id, deserial_addr, 0x0001, 0x02); // 6Gbps
    }

    max9296_i2c_write(bus_id, deserial_addr, 0x0050, 0x01); // PipeX ID1
    max9296_i2c_write(bus_id, deserial_addr, 0x0051, 0x02); // PipeY ID2
    // PipeX VC0 -> PHY B VC0
    max9296_pipe_remap(serdes_attr, SERDES_PIPE_X, 0, 0, data_type);
    // PipeY VC1 -> PHY B VC1
    max9296_pipe_remap(serdes_attr, SERDES_PIPE_Y, 1, 1, data_type);
    // Double Mode Configuration
    max9296_i2c_write(bus_id, deserial_addr, 0x0473, 0x04); // Alternate memory map enabled for 10bit DT
    // MIPI DPHY Configuratio
    max9296_i2c_write(bus_id, deserial_addr, 0x0330, 0x04); // PHY 1x4
    max9296_i2c_write(bus_id, deserial_addr, 0x044A, 0xD0); // 4lane
    if (bus_id == MAX9296_BRD_BUS_ID) {
        max9296_i2c_write(bus_id, deserial_addr, 0x0333, 0xB4); // set lane mapping
        max9296_i2c_write(bus_id, deserial_addr, 0x0335, 0x3F); // set lane polarity
    }
    max9296_i2c_write(bus_id, deserial_addr, 0x1D00, 0xF4); // phy reset
    max9296_i2c_write(bus_id, deserial_addr, 0x0320, 0x20 + freq_100mhz); // 0xF * 100Mhz
    max9296_i2c_write(bus_id, deserial_addr, 0x1D00, 0xF5);

    // INSTRUCTIONS FOR GMSL-A SERIALIZER MAX9295A
    serdes_mdelay(50); // wait max9295 power up stability
    if (max9295_check_is_exist(bus_id, serial_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }
    max9295_i2c_write(bus_id, serial_addr, 0x02BE, 0x00); // GPIO0 0, sensor reset
    serdes_mdelay(20);
    max9295_i2c_write(bus_id, serial_addr, 0x02BE, 0x10); // GPIO0 1, sensor unreset
    serdes_mdelay(20); // wait sensor work stability
    max9295_i2c_write(bus_id, serial_addr, 0x0331, 0x03 | ((serdes_attr->sns_lanes - 1) << 4)); // lane nums
    max9295_i2c_write(bus_id, serial_addr, 0x0002, 0x33); // Enable Video X/Y
    max9295_i2c_write(bus_id, serial_addr, 0x0308, 0x7F); // Enable portB
    max9295_i2c_write(bus_id, serial_addr, 0x0309, 0x01); // PipeX VC0
    max9295_i2c_write(bus_id, serial_addr, 0x030B, 0x02); // PipeY VC1
    max9295_i2c_write(bus_id, serial_addr, 0x0311, 0x35); // Start Video X/Y
    max9295_i2c_write(bus_id, serial_addr, 0x0314, 0x40 + data_type); // PipeX DT: 0x6B
    max9295_i2c_write(bus_id, serial_addr, 0x0316, 0x40 + data_type); // PipeY DT: 0x6B
    // Double Mode Configuration
    max9295_i2c_write(bus_id, serial_addr, 0x0313, 0x01); // Send 10-bit pixels as 20-bit
    max9295_i2c_write(bus_id, serial_addr, 0x031C, 0x34); // Software override enabled
    max9295_i2c_write(bus_id, serial_addr, 0x0313, 0x03); // Send 10-bit pixels as 20-bit
    max9295_i2c_write(bus_id, serial_addr, 0x031D, 0x34); // Software override enabled
    // Pipe Configuration
    max9295_i2c_write(bus_id, serial_addr, 0x0053, 0x11); // PipeX ID1
    max9295_i2c_write(bus_id, serial_addr, 0x0057, 0x12); // PipeY ID2
    max9296_i2c_write(bus_id, deserial_addr, 0x0313, 0x02); // CSI output enabled
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_wdr_vc_2to1_one_sns_stop(const ot_eis_vi_serdes_attr *serdes_attr)
{
    return max9295_max9296_linear_one_sns_stop(serdes_attr);
}

static ot_s32 max9295_max9296_get_idle_id(ot_void)
{
    ot_s32 i;

    for (i = 0; i < OT_EIS_VI_SERDES_ID_MAX; i++) {
        if (g_serdes_drv_ctrl[i].is_created == OT_FALSE) {
            break;
        }
    }
    return i;
}

static ot_s32 max9295_max9296_create(ot_s32 *serdes_id, const ot_serdes_attr *serdes_attr)
{
    ot_s32 id;
    vi_serdes_drv_ctrl *drv_ctrl = OT_NULL;

    id = max9295_max9296_get_idle_id();
    serdes_chk_data_ge_return(id, OT_EIS_VI_SERDES_ID_MAX, OT_VI_SERDES_ERR_OVER_SIZE);

    drv_ctrl = &(g_serdes_drv_ctrl[id]);

    drv_ctrl->is_created = OT_TRUE;
    drv_ctrl->is_started = OT_FALSE;
    memcpy(&(drv_ctrl->serdes_attr), (ot_eis_vi_serdes_attr *)serdes_attr, sizeof(ot_eis_vi_serdes_attr));
    *serdes_id = id;
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_destroy(ot_s32 serdes_id)
{
    vi_serdes_drv_ctrl *drv_ctrl = OT_NULL;

    serdes_chk_data_ge_return(serdes_id, OT_EIS_VI_SERDES_ID_MAX, OT_VI_SERDES_ERR_OVER_SIZE);

    drv_ctrl = &(g_serdes_drv_ctrl[serdes_id]);

    if (drv_ctrl->is_created == OT_FALSE) {
        serdes_err("Serdes id don't create.\n");
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    if (drv_ctrl->is_started == OT_TRUE) {
        serdes_err("Serdes id don't stop.\n");
        return OT_VI_SERDES_ERR_NO_START;
    }

    memset(drv_ctrl, 0, sizeof(vi_serdes_drv_ctrl));
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_start_by_scene(const ot_eis_vi_serdes_attr *serdes_attr)
{
    ot_s32 ret;

    if ((serdes_attr->wdr_type == OT_VI_SERDES_WDR_TYPE_NONE) ||
        (serdes_attr->wdr_type == OT_VI_SERDES_WDR_TYPE_DOL)) {
        if (serdes_attr->sns_nums == SENSOR_NUMBERS_1) {
            ret = max9295_max9296_linear_one_sns_start(serdes_attr);
            if (ret != OT_SUCCESS) {
                serdes_err("start one linear sensor failed.\n");
                return ret;
            }
        } else if (serdes_attr->sns_nums == SENSOR_NUMBERS_2) {
            ret = max9295_max9296_linear_two_sns_start(serdes_attr);
            if (ret != OT_SUCCESS) {
                serdes_err("start two linear sensor failed.\n");
                return ret;
            }
        } else {
            serdes_err("Invalid sensor numbers %d.\n", serdes_attr->sns_nums);
            return OT_VI_SERDES_ERR_INVALID_PARAM;
        }
    } else if ((serdes_attr->wdr_type == OT_VI_SERDES_WDR_TYPE_VC) &&
        (serdes_attr->wdr_mode == OT_VI_SERDES_WDR_MODE_2TO1)) {
        ret = max9295_max9296_wdr_vc_2to1_one_sns_start(serdes_attr);
        if (ret != OT_SUCCESS) {
            serdes_err("start one wdr 2to1 sensor failed.\n");
            return ret;
        }
    } else {
        serdes_err("Invalid wdr type.\n");
        return OT_VI_SERDES_ERR_INVALID_PARAM;
    }
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_stop_by_scene(const ot_eis_vi_serdes_attr *serdes_attr)
{
    if ((serdes_attr->wdr_type == OT_VI_SERDES_WDR_TYPE_NONE) ||
        (serdes_attr->wdr_type == OT_VI_SERDES_WDR_TYPE_DOL)) {
        if (serdes_attr->sns_nums == SENSOR_NUMBERS_1) {
            return max9295_max9296_linear_one_sns_stop(serdes_attr);
        } else if (serdes_attr->sns_nums == SENSOR_NUMBERS_2) {
            return max9295_max9296_linear_two_sns_stop(serdes_attr);
        } else {
            serdes_err("Invalid sensor numbers %d.\n",serdes_attr->sns_nums);
            return OT_VI_SERDES_ERR_INVALID_PARAM;
        }
    } else if ((serdes_attr->wdr_type == OT_VI_SERDES_WDR_TYPE_VC) &&
        (serdes_attr->wdr_mode == OT_VI_SERDES_WDR_MODE_2TO1)) {
        return max9295_max9296_wdr_vc_2to1_one_sns_stop(serdes_attr);
    } else {
        serdes_err("Invalid wdr type.\n");
        return OT_VI_SERDES_ERR_INVALID_PARAM;
    }
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_start(ot_s32 serdes_id)
{
    ot_s32 ret;
    vi_serdes_drv_ctrl *drv_ctrl = OT_NULL;

    serdes_chk_data_ge_return(serdes_id, OT_EIS_VI_SERDES_ID_MAX, OT_VI_SERDES_ERR_OVER_SIZE);

    drv_ctrl = &(g_serdes_drv_ctrl[serdes_id]);

    if (drv_ctrl->is_created == OT_FALSE) {
        serdes_err("Serdes id don't create.\n");
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    if (drv_ctrl->is_started == OT_TRUE) {
        serdes_err("Serdes id don't start.\n");
        return OT_VI_SERDES_ERR_NO_STOP;
    }

    if (drv_ctrl->serdes_attr.bus_info.bus_id == -1) {
        drv_ctrl->is_started = OT_TRUE;
        return OT_SUCCESS;
    }

    if (drv_ctrl->serdes_attr.bus_info.bus_id == MAX9296_BRD_BUS_ID) {
        max9295_power_ctrl(OT_TRUE);
    }

    ret = max9295_max9296_start_by_scene(&(drv_ctrl->serdes_attr));
    if (ret != OT_SUCCESS) {
        if (drv_ctrl->serdes_attr.bus_info.bus_id == MAX9296_BRD_BUS_ID) {
            max9295_power_ctrl(OT_FALSE);
        }
        serdes_err("The serdes start failed.\n");
        return ret;
    }
    drv_ctrl->is_started = OT_TRUE;
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_stop(ot_s32 serdes_id)
{
    ot_s32 ret;
    vi_serdes_drv_ctrl *drv_ctrl = OT_NULL;

    serdes_chk_data_ge_return(serdes_id, OT_EIS_VI_SERDES_ID_MAX, OT_VI_SERDES_ERR_OVER_SIZE);

    drv_ctrl = &(g_serdes_drv_ctrl[serdes_id]);

    if (drv_ctrl->is_created == OT_FALSE) {
        serdes_err("Serdes id don't create.\n");
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    if (drv_ctrl->is_started == OT_FALSE) {
        serdes_err("Serdes id had stoped.\n");
        return OT_VI_SERDES_ERR_NO_START;
    }

    if (drv_ctrl->serdes_attr.bus_info.bus_id == -1) {
        drv_ctrl->is_started = OT_FALSE;
        return OT_SUCCESS;
    }

    ret = max9295_max9296_stop_by_scene(&(drv_ctrl->serdes_attr));
    if (ret != OT_SUCCESS) {
        serdes_err("The serdes stop failed.\n");
        return ret;
    }

    if (drv_ctrl->serdes_attr.bus_info.bus_id == MAX9296_BRD_BUS_ID) {
        max9295_power_ctrl(OT_FALSE);
    }
    drv_ctrl->is_started = OT_FALSE;
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_linear_one_sns_restart(const ot_eis_vi_serdes_attr *serdes_attr)
{
    ot_s32 ret;
    ot_u8 data_type, bus_id, deserial_addr, serial_addr;
    ot_bool is_x3f = OT_FALSE;

    bus_id = serdes_attr->bus_info.bus_id;
    serial_addr = serdes_attr->one_port_attr.serial_addr;
    deserial_addr = serdes_attr->deserial_addr;
    if (max9296_check_is_exist(bus_id, deserial_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    ret = max9295_max9296_data_type(serdes_attr->data_fmt, &data_type);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    if (max9295_check_is_exist(bus_id, serial_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    max9295_i2c_write(bus_id,
        serial_addr, 0x0331, 0x03 | ((serdes_attr->sns_lanes - 1) << 4)); // 2lane
    max9295_i2c_write(bus_id, serial_addr, 0x0318, 0x40 | data_type); // data type
    max9295_i2c_write(bus_id, serial_addr, 0x02BE, 0x00); // GPIO0 0, sensor reset
    serdes_mdelay(20);
    max9295_i2c_write(bus_id, serial_addr, 0x02BE, 0x10); // GPIO0 1, sensor unreset
    serdes_mdelay(20); // wait sensor work stability

    if(serdes_attr->priv_data != OT_NULL) {
        is_x3f = (*((ot_u8*)serdes_attr->priv_data)) & 0x01;
    }

    vi_serdes_x3f_adapt(bus_id, serial_addr, &is_x3f);
    max9296_i2c_write(bus_id, deserial_addr, 0x0313, 0x02); // enable mipi output
    return OT_SUCCESS;
}

static ot_s32 max9295_max9296_wdr_vc_2to1_one_sns_restart(const ot_eis_vi_serdes_attr *serdes_attr, ot_u32 port_id)
{
    ot_s32 ret;
    ot_u8 data_type, bus_id, deserial_addr, serial_addr;

    bus_id = serdes_attr->bus_info.bus_id;
    serial_addr = serdes_attr->one_port_attr.serial_addr;
    deserial_addr = serdes_attr->deserial_addr;

    if (max9296_check_is_exist(bus_id, deserial_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }

    ret = max9295_max9296_data_type(serdes_attr->data_fmt, &data_type);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    if (max9295_check_is_exist(bus_id, serial_addr) == OT_FALSE) {
        return OT_VI_SERDES_ERR_NO_EXIST;
    }
    max9295_i2c_write(bus_id, serial_addr, 0x02BE, 0x00); // GPIO0 0, sensor reset
    serdes_mdelay(20);
    max9295_i2c_write(bus_id, serial_addr, 0x02BE, 0x10); // GPIO0 1, sensor unreset
    serdes_mdelay(20); // wait sensor work stability
    max9295_i2c_write(bus_id, serial_addr, 0x0331, 0x03 | ((serdes_attr->sns_lanes - 1) << 4)); // lane nums
    max9295_i2c_write(bus_id, serial_addr, 0x0002, 0x33); // Enable Video X/Y
    max9295_i2c_write(bus_id, serial_addr, 0x0308, 0x7F); // Enable portB
    max9295_i2c_write(bus_id, serial_addr, 0x0309, 0x01); // PipeX VC0
    max9295_i2c_write(bus_id, serial_addr, 0x030B, 0x02); // PipeY VC1
    max9295_i2c_write(bus_id, serial_addr, 0x0311, 0x35); // Start Video X/Y
    max9295_i2c_write(bus_id, serial_addr, 0x0314, 0x40 + data_type); // PipeX DT: 0x6B
    max9295_i2c_write(bus_id, serial_addr, 0x0316, 0x40 + data_type); // PipeY DT: 0x6B
    // Double Mode Configuration
    max9295_i2c_write(bus_id, serial_addr, 0x0313, 0x01); // Send 10-bit pixels as 20-bit
    max9295_i2c_write(bus_id, serial_addr, 0x031C, 0x34); // Software override enabled
    max9295_i2c_write(bus_id, serial_addr, 0x0313, 0x03); // Send 10-bit pixels as 20-bit
    max9295_i2c_write(bus_id, serial_addr, 0x031D, 0x34); // Software override enabled
    // Pipe Configuration
    max9295_i2c_write(bus_id, serial_addr, 0x0053, 0x11); // PipeX ID1
    max9295_i2c_write(bus_id, serial_addr, 0x0057, 0x12); // PipeY ID2
    max9296_i2c_write(bus_id, deserial_addr, 0x0313, 0x02); // CSI output enabled
    return OT_SUCCESS;

}

static ot_s32 max9295_max9296_restart(ot_s32 serdes_id, ot_u32 port_id)
{
    ot_s32 ret = OT_FAILURE;
    ot_eis_vi_serdes_attr *vi_serdes_attr = OT_NULL;
    ot_u8 sns_nums = 0;

    serdes_chk_data_ge_return(serdes_id, OT_EIS_VI_SERDES_ID_MAX, OT_VI_SERDES_ERR_OVER_SIZE);

    vi_serdes_attr = &(g_serdes_drv_ctrl[serdes_id].serdes_attr);
    sns_nums = vi_serdes_attr->sns_nums;

    if ((vi_serdes_attr->wdr_type == OT_VI_SERDES_WDR_TYPE_NONE) ||
        (vi_serdes_attr->wdr_type == OT_VI_SERDES_WDR_TYPE_DOL)) {
        if (sns_nums == SENSOR_NUMBERS_1) {
            ret = max9295_max9296_linear_one_sns_restart(vi_serdes_attr);
            if (ret != OT_SUCCESS) {
                serdes_err("restart one linear sensor failed.\n");
                return ret;
            }
        } else {
            serdes_err("Invalid sensor numbers %d.\n", sns_nums);
            return OT_VI_SERDES_ERR_INVALID_PARAM;
        }
    } else if ((vi_serdes_attr->wdr_type == OT_VI_SERDES_WDR_TYPE_VC) &&
        (vi_serdes_attr->wdr_mode == OT_VI_SERDES_WDR_MODE_2TO1)) {
        ret = max9295_max9296_wdr_vc_2to1_one_sns_restart(vi_serdes_attr, port_id);
        if (ret != OT_SUCCESS) {
            serdes_err("restart one wdr 2to1 sensor failed.\n");
            return ret;
        }
    } else {
        serdes_err("Invalid wdr type.\n");
        return OT_VI_SERDES_ERR_INVALID_PARAM;
    }
    return OT_SUCCESS;
}


ot_serdes_obj g_max9295_max9296_obj = {
    .serdes_name = "MAX9295,MAX9296",
    .create      = max9295_max9296_create,
    .destroy     = max9295_max9296_destroy,
    .start       = max9295_max9296_start,
    .stop        = max9295_max9296_stop,
    .restart     = max9295_max9296_restart,
};

const ot_serdes_obj *max9295_max9296_get_vi_serdes_obj(ot_void)
{
    return &g_max9295_max9296_obj;
}
