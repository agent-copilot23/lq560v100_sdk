#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "ot_eis_errno.h"
#include "sample_comm.h"

#define I2C_DEV_FILE_NUM    32
#define I2C_BUF_NUM         8
#define I2C_DATA_1BYTE      1
#define I2C_REG_ADDR_1BYTE  1
#define I2C_BIT_8SHIFT      8
#define I2C_MSG_CNT         2

#define I2C_DETECT_CNT        200

#define I2C_BUS_ID            3
#define LIGHTSENSOR_REG_SIZE  1
#define LIGHTSENSOR_DATA_SIZE 2

#define LIGHTSENSOR_ID            0x3001
#define LIGHTSENSOR_MAX_NUM       2
#define LIGHTSENSOR_RESULT_REG    0x00
#define LIGHTSENSOR_CONFIG_REG    0x01
#define LIGHTSENSOR_STOP_MODE     0xC810
#define LIGHTSENSOR_CONTINUE_100MS_MODE 0xC410
#define LIGHTSENSOR_CONTINUE_800MS_MODE 0xCE10

static sample_light_sensor g_light_sensor_attr[LIGHTSENSOR_MAX_NUM] = {0};

static ot_s32 lightsensor_i2c_open(ot_s8 bus_id, ot_u8 dev_addr)
{
    ot_s32 fd = -1;
    ot_s8 dev_file[I2C_DEV_FILE_NUM] = {0};

    (ot_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", bus_id);
    fd = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        sample_print("Open %s error!\n", dev_file);
        return OT_EIS_ERRNO_NO_SUPPORT;
    }

    if (ioctl(fd, I2C_SLAVE_FORCE, dev_addr >> 1) < 0) {
        sample_print("I2C_SLAVE_FORCE error!\n");
        close(fd);
        return OT_EIS_ERRNO_NO_SUPPORT;
    }
    return fd;
}

static ot_s32 lightsensor_i2c_write(ot_s8 bus_id,
    ot_u8 dev_addr, ot_u16 reg_addr, ot_u8 reg_byte, ot_u16 data, ot_u8 data_byte)
{
    ot_s32 fd = -1;
    ot_u32 idx = 0;
    ot_u8 buf[I2C_BUF_NUM];

    fd = lightsensor_i2c_open(bus_id, dev_addr);
    if (fd < 0) {
        sample_print("lightsensor i2c open error!\n");
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
        sample_print("i2c%d 0x%02x write 0x%04x data 0x%02x failed!\n", bus_id, dev_addr, reg_addr, data);
        return OT_EIS_ERRNO_NO_SUPPORT;
    }
    close(fd);
    return OT_SUCCESS;
}

static ot_s32 lightsensor_i2c_read(ot_s8 bus_id,
    ot_u8 dev_addr, ot_u16 reg_addr, ot_u8 reg_byte, ot_u16 *data, ot_u8 data_byte)
{
    ot_s32 fd = -1;
    ot_u32 idx = 0;
    ot_u8 buf[I2C_BUF_NUM] = {0};
    static struct i2c_rdwr_ioctl_data rdwr;
    static struct i2c_msg msg[I2C_MSG_CNT] = {0};

    fd = lightsensor_i2c_open(bus_id, dev_addr);
    if (fd < 0) {
        sample_print("lightsensor i2c open error!\n");
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
    rdwr.msgs[1].len   = data_byte;
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

static ot_bool lightsensor_check_is_exist(ot_u8 dev_addr)
{
    ot_s32 cnt;

    cnt = I2C_DETECT_CNT;
    for (cnt = I2C_DETECT_CNT; cnt >= 0; cnt--) {
        ot_u16 data = 0x00;
        lightsensor_i2c_read(I2C_BUS_ID, dev_addr, 0x7F, LIGHTSENSOR_REG_SIZE,
                            &data, LIGHTSENSOR_DATA_SIZE); // 7E:厂商ID：0x5449，7F:设备ID：0x3001
        if (data == LIGHTSENSOR_ID) {
            return OT_TRUE;
        }
        usleep(1 * 1000);
    }
    printf("LIGHT SENSOR 0x%02X don't exist.\n", dev_addr);
    return OT_FALSE;
}

ot_void sample_comm_lightsensor_start(sample_light_sensor *light_sensor_attr)
{
    if (light_sensor_attr->is_used) {
        if (lightsensor_check_is_exist(light_sensor_attr->port_attr[0].dst_addr) == OT_FALSE) {
            printf("light sensor0 don't exist.\n");
            return;
        }
        memcpy(&g_light_sensor_attr, light_sensor_attr, sizeof(sample_light_sensor));
        lightsensor_i2c_write(I2C_BUS_ID, light_sensor_attr->port_attr[0].dst_addr,
                            LIGHTSENSOR_CONFIG_REG, LIGHTSENSOR_REG_SIZE,
                            LIGHTSENSOR_CONTINUE_100MS_MODE, LIGHTSENSOR_DATA_SIZE);
        if (light_sensor_attr->light_sensor_num == 2) {
            if (lightsensor_check_is_exist(light_sensor_attr->port_attr[1].dst_addr) == OT_FALSE) {
                printf("light sensor1 don't exist.\n");
                return;
            }
            lightsensor_i2c_write(I2C_BUS_ID, light_sensor_attr->port_attr[1].dst_addr,
                                LIGHTSENSOR_CONFIG_REG, LIGHTSENSOR_REG_SIZE,
                                LIGHTSENSOR_CONTINUE_100MS_MODE, LIGHTSENSOR_DATA_SIZE);
        }
    }
}

ot_void sample_comm_lightsensor_stop(sample_light_sensor *light_sensor_attr)
{
    if (light_sensor_attr->is_used) {
        memcpy(&g_light_sensor_attr, light_sensor_attr, sizeof(sample_light_sensor));
        lightsensor_i2c_write(I2C_BUS_ID, light_sensor_attr->port_attr[0].dst_addr,
                        LIGHTSENSOR_CONFIG_REG, LIGHTSENSOR_REG_SIZE,
                        LIGHTSENSOR_STOP_MODE, LIGHTSENSOR_DATA_SIZE);
        if (light_sensor_attr->light_sensor_num == 2) {
            lightsensor_i2c_write(I2C_BUS_ID, light_sensor_attr->port_attr[1].dst_addr,
                                LIGHTSENSOR_CONFIG_REG, LIGHTSENSOR_REG_SIZE,
                                LIGHTSENSOR_STOP_MODE, LIGHTSENSOR_DATA_SIZE);
        }
    }
}

ot_u16 sample_comm_lightsensor_get_lux(sample_light_sensor_id port_id)
{
    ot_u16 data;
    ot_u32 real_lux;
    lightsensor_i2c_read(I2C_BUS_ID, g_light_sensor_attr->port_attr[port_id].dst_addr,
                        LIGHTSENSOR_RESULT_REG, LIGHTSENSOR_REG_SIZE,
                        &data, LIGHTSENSOR_DATA_SIZE);
    real_lux = pow(2, (data >> 12) & 0xf) * ((data & 0xfff));
    return (real_lux * 100 / 83865);
}