/*
 * Copyright (c) Oritek Technologies CO., Ltd. 2020-2023. All rights reserved.
 */
#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

#include "platform.h"
#include "lib.h"

typedef struct {
    unsigned char i2c_num;
    unsigned short dev_addr;
    unsigned long int reg_addr;
    unsigned int reg_width;
} i2c_client;

int hal_i2c_recv(const i2c_client *client, unsigned char *buf, unsigned int count);
int hal_i2c_send(unsigned char i2c_num, unsigned short dev_addr,
                 unsigned char *buf, unsigned int count);
int hal_i2c_init(unsigned char i2c_num);

#endif /* __HAL_I2C_H__ */
