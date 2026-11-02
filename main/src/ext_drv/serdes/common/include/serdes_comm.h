#ifndef SERDES_COMM_H
#define SERDES_COMM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "ot_eis_errno.h"
#include "ot_eis_list.h"
#include "securec.h"

typedef ot_void ot_serdes_attr;

#define serdes_lock_init(lock)       pthread_mutex_init(&(lock), OT_NULL)
#define serdes_lock(lock)            pthread_mutex_lock(&(lock))
#define serdes_unlock(lock)          pthread_mutex_unlock(&(lock))
#define serdes_lock_deinit(lock)     pthread_mutex_destroy((&lock))

#define SERDES_LANE_MAX     4

typedef enum {
    OT_SERDES_TYPE_VI,
    OT_SERDES_TYPE_VO,
    OT_SERDES_TYPE_BUTT,
} ot_serdes_type;

ot_void serdes_mdelay(ot_u32 ms);

ot_s32 serdes_i2c_read(ot_s8 bus_id,
    ot_u8 dev_addr, ot_u16 reg_addr, ot_u8 reg_byte, ot_u16 *data, ot_u8 data_byte);

ot_s32 serdes_i2c_write(ot_s8 bus_id,
    ot_u8 dev_addr, ot_u16 reg_addr, ot_u8 reg_byte, ot_u16 data, ot_u8 data_byte);

#define serdes_err(fmt, ...)                                                    \
    do {                                                                        \
        printf("[ERR] %s-%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);      \
    } while (0)

#define serdes_chk_ptr_null_return(ptr, errno)                                  \
    do {                                                                        \
        if ((ptr) == OT_NULL) {                                                 \
            serdes_err("%s is NULL.\n", # ptr);                                 \
            return (errno);                                                     \
        }                                                                       \
    } while (0)

#define serdes_chk_data_ge_return(data, max, errno)                             \
    do {                                                                        \
        if ((data) >= (max)) {                                                  \
            serdes_err("%s is greater than or equal to %d.\n", # data, max);    \
            return (errno);                                                     \
        }                                                                       \
    } while (0)

#define serdes_chk_data_gt_return(data, max, errno)                             \
    do {                                                                        \
        if ((data) > (max)) {                                                   \
            serdes_err("%s is greater than %d.\n", # data, max);                \
            return (errno);                                                     \
        }                                                                       \
    } while (0)

#define serdes_chk_data_le_return(data, min, errno)                             \
    do {                                                                        \
        if ((data) <= (min)) {                                                  \
            serdes_err("%s is less than or equal to %d.\n", # data, min);       \
            return (errno);                                                     \
        }                                                                       \
    } while (0)

#define serdes_chk_i2c_addr_odd_return(addr, errno)                             \
    do {                                                                        \
        if ((addr) & 0x01) {                                                    \
            serdes_err("The i2c addr 0x%02X is odd.\n", addr);                  \
            return (errno);                                                     \
        }                                                                       \
    } while (0)

#define serdes_chk_i2c_addr_equal_return(src_addr, dst_addr, errno)             \
    do {                                                                        \
        if ((src_addr) == (dst_addr)) {                                         \
            serdes_err("%s addr 0x%02X is equal to %s 0x%02X.\n",               \
                # src_addr, src_addr, # dst_addr, dst_addr);                    \
            return (errno);                                                     \
        }                                                                       \
    } while (0)

#endif /* SERDES_COMM_H */