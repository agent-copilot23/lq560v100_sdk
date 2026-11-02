#include "serdes_comm.h"

#define I2C_DEV_FILE_NUM    32
#define I2C_BUF_NUM         8
#define I2C_DATA_1BYTE      1
#define I2C_REG_ADDR_1BYTE  1
#define I2C_BIT_8SHIFT      8
#define I2C_MSG_CNT         2

static ot_s32 serdes_i2c_open(ot_s8 bus_id, ot_u8 dev_addr)
{
    ot_s32 fd = -1;
    ot_s8 dev_file[I2C_DEV_FILE_NUM] = {0};

    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", bus_id);
    fd = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        serdes_err("Open %s error!\n", dev_file);
        return OT_EIS_ERRNO_NO_SUPPORT;
    }

    if (ioctl(fd, I2C_SLAVE_FORCE, dev_addr >> 1) < 0) {
        serdes_err("I2C_SLAVE_FORCE error!\n");
        close(fd);
        return OT_EIS_ERRNO_NO_SUPPORT;
    }
    return fd;
}

ot_s32 serdes_i2c_write(ot_s8 bus_id,
    ot_u8 dev_addr, ot_u16 reg_addr, ot_u8 reg_byte, ot_u16 data, ot_u8 data_byte)
{
    ot_s32 fd = -1;
    ot_u32 idx = 0;
    ot_u8 buf[I2C_BUF_NUM];

    fd = serdes_i2c_open(bus_id, dev_addr);
    if (fd < 0) {
        serdes_err("serdes i2c open error!\n");
        return OT_EIS_ERRNO_NO_SUPPORT;
    }

    if (reg_byte == I2C_REG_ADDR_1BYTE) {
        buf[idx++] = reg_addr & 0xff;
    } else {
        buf[idx++] = (reg_addr >> I2C_BIT_8SHIFT) & 0xff;
        buf[idx++] = reg_addr & 0xff;
    }
    if (data_byte == I2C_DATA_1BYTE) {
        buf[idx++] = data & 0xff;
    } else {
        buf[idx++] = (data >> I2C_BIT_8SHIFT) & 0xff;
        buf[idx++] = data & 0xff;
    }
    if (write(fd, buf, idx) < 0) {
        close(fd);
        serdes_err("i2c%d 0x%02x write 0x%04x data 0x%02x failed!\n", bus_id, dev_addr, reg_addr, data);
        return OT_EIS_ERRNO_NO_SUPPORT;
    }
    close(fd);
    return OT_SUCCESS;
}

ot_s32 serdes_i2c_read(ot_s8 bus_id,
    ot_u8 dev_addr, ot_u16 reg_addr, ot_u8 reg_byte, ot_u16 *data, ot_u8 data_byte)
{
    ot_s32 fd = -1;
    ot_u32 idx = 0;
    ot_u8 buf[I2C_BUF_NUM] = {0};
    static struct i2c_rdwr_ioctl_data rdwr;
    static struct i2c_msg msg[I2C_MSG_CNT] = {0};

    fd = serdes_i2c_open(bus_id, dev_addr);
    if (fd < 0) {
        serdes_err("serdes i2c open error!\n");
        return OT_EIS_ERRNO_NO_SUPPORT;
    }

    if (reg_byte == I2C_REG_ADDR_1BYTE) {
        buf[idx++] = reg_addr & 0xff;
    } else {
        buf[idx++] = (reg_addr >> 8) & 0xff;  /* shift 8 */
        buf[idx++] = reg_addr & 0xff;
    }
    rdwr.msgs          = &msg[0];
    rdwr.nmsgs         = I2C_MSG_CNT;
    rdwr.msgs[0].addr  = (dev_addr >> 1);
    rdwr.msgs[0].flags = 0;
    rdwr.msgs[0].len   = reg_byte;
    rdwr.msgs[0].buf   = buf;

    rdwr.msgs[1].addr  = (dev_addr >> 1);
    rdwr.msgs[1].flags = 0;
    rdwr.msgs[1].flags |= I2C_M_RD;
    rdwr.msgs[1].len   = 1;
    rdwr.msgs[1].buf   = buf;

    if (ioctl(fd, I2C_RDWR, &rdwr) < 0) {
        close(fd);
        return OT_EIS_ERRNO_NO_SUPPORT;
    }

    if (data_byte == I2C_DATA_1BYTE) {
        *data = buf[0];
    } else {
        *data = (buf[0] << I2C_BIT_8SHIFT) | buf[1];
    }
    close(fd);
    return OT_SUCCESS;
}

ot_void serdes_mdelay(ot_u32 ms)
{
    usleep(ms * 1000);
}
