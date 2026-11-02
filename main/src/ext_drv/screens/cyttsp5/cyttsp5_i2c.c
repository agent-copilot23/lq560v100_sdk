/*
 * cyttsp5_i2c.c
 * Cypress TrueTouch(TM) Standard Product V5 I2C Module.
 * For use with Cypress Txx5xx parts.
 * Supported parts include:
 * TMA5XX
 *
 * Copyright (C) 2012-2013 Cypress Semiconductor
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, and only version 2, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contact Cypress Semiconductor at www.cypress.com <ttdrivers@cypress.com>
 *
 */

#include "cyttsp5_regs.h"
#include <linux/miscdevice.h>
#include <linux/i2c.h>

#define CY_I2C_DATA_SIZE  (2 * 256)

#define CYTTSP5_TS_NAME			"Cyttsp5-TS"
#define CYTTSP5_TS_ADDR			0x24
#define CYTTSP5_TS_I2C_BUS_ID	3
#define CYTTSP5_GPIO_IRQ_NUM	88	// gpio11_0
#define CYTTSP5_GPIO_RST_NUM	0	// gpio0_0

static struct i2c_client* g_cyttsp5_ts_client;

static int g_bus_id = CYTTSP5_TS_I2C_BUS_ID;
static int g_irq = CYTTSP5_GPIO_IRQ_NUM;

module_param_named(bus_id, g_bus_id, int, S_IRUSR);
module_param_named(irq, g_irq, int, S_IRUSR);

static int cyttsp5_i2c_read_default(struct device *dev, void *buf, int size)
{
	struct i2c_client *client = to_i2c_client(dev);
	int rc;

	if (!buf || !size || size > CY_I2C_DATA_SIZE)
		return -EINVAL;

	rc = i2c_master_recv(client, buf, size);

	return (rc < 0) ? rc : rc != size ? -EIO : 0;
}

static int cyttsp5_i2c_read_default_nosize(struct device *dev, u8 *buf, u32 max)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct i2c_msg msgs[2];
	u8 msg_count = 1;
	int rc;
	u32 size;

	if (!buf)
		return -EINVAL;

	msgs[0].addr = client->addr;
	msgs[0].flags = (client->flags & I2C_M_TEN) | I2C_M_RD;
	msgs[0].len = 2;
	msgs[0].buf = buf;
	rc = i2c_transfer(client->adapter, msgs, msg_count); // this fills msgs[0].buf?
	if (rc < 0 || rc != msg_count)
		return (rc < 0) ? rc : -EIO;

	size = get_unaligned_le16(&buf[0]);
	if (!size || size == 2)
		return 0;

	if (size > max)
		return -EINVAL;

	rc = i2c_master_recv(client, buf, size);

	return (rc < 0) ? rc : rc != (int)size ? -EIO : 0;
}

static int cyttsp5_i2c_write_read_specific(struct device *dev, u8 write_len,
		u8 *write_buf, u8 *read_buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct i2c_msg msgs[2];
	u8 msg_count = 1;
	int rc;

	if (!write_buf || !write_len)
		return -EINVAL;

	msgs[0].addr = client->addr;
	msgs[0].flags = client->flags & I2C_M_TEN;
	msgs[0].len = write_len;
	msgs[0].buf = write_buf;
	rc = i2c_transfer(client->adapter, msgs, msg_count);

	if (rc < 0 || rc != msg_count)
		return (rc < 0) ? rc : -EIO;
	else
		rc = 0;

	if (read_buf)
		rc = cyttsp5_i2c_read_default_nosize(dev, read_buf,
				CY_I2C_DATA_SIZE);

	return rc;
}

static struct cyttsp5_bus_ops cyttsp5_i2c_bus_ops = {
	.bustype = BUS_I2C,
	.read_default = cyttsp5_i2c_read_default,
	.read_default_nosize = cyttsp5_i2c_read_default_nosize,
	.write_read_specific = cyttsp5_i2c_write_read_specific,
};

static const struct file_operations cyttsp5_ts_ops = {
	.owner = THIS_MODULE,
};

static struct miscdevice cyttsp5_ts_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &cyttsp5_ts_ops,
	.name = CYTTSP5_TS_NAME,
};

static struct i2c_board_info cyttsp5_ts_info = {
	I2C_BOARD_INFO("Cyttsp5-TS", CYTTSP5_TS_ADDR),
};

static int i2c_client_init(void)
{
	struct i2c_adapter* i2c_adap;

	i2c_adap = i2c_get_adapter(g_bus_id);
	if(i2c_adap == NULL) {
		pr_err("i2c get adapter failed\n");
		return -1;
	}

	g_cyttsp5_ts_client = i2c_new_client_device(i2c_adap, &cyttsp5_ts_info);
	i2c_put_adapter(i2c_adap);

	return 0;
}


static void i2c_client_exit(void)
{
	i2c_unregister_device(g_cyttsp5_ts_client);
}

static int cyttsp5_i2c_probe(void)
{
	int ret;

	ret = misc_register(&cyttsp5_ts_dev);
	if (ret < 0) {
		pr_err("misc register cyttsp5 touchscreen failed\n");
		return -1;
	}

	ret = i2c_client_init();
	if (ret < 0) {
		pr_err("misc register cyttsp5 touchscreen failed\n");
		return -1;
	}

	if (!i2c_check_functionality(g_cyttsp5_ts_client->adapter, I2C_FUNC_I2C)) {
		pr_err("I2C functionality not Supported\n");
		return -EIO;
	}

	return cyttsp5_probe(&cyttsp5_i2c_bus_ops, &g_cyttsp5_ts_client->dev, g_irq,
			  CY_I2C_DATA_SIZE);
}

static int cyttsp5_i2c_remove(struct i2c_client *client)
{

	struct cyttsp5_core_data *cd = i2c_get_clientdata(g_cyttsp5_ts_client);

	cyttsp5_release(cd);
	i2c_client_exit();
	misc_deregister(&cyttsp5_ts_dev);

	return 0;
}

static int __init cyttsp5_i2c_init(void)
{
	int rc = cyttsp5_i2c_probe();

	pr_info("%s: Cypress TTSP v5 I2C Driver (Built %s) rc=%d\n",
		 __func__, CY_DRIVER_DATE, rc);
	return rc;
}
module_init(cyttsp5_i2c_init);

static void __exit cyttsp5_i2c_exit(void)
{
	cyttsp5_i2c_remove(g_cyttsp5_ts_client);
}
module_exit(cyttsp5_i2c_exit);

MODULE_ALIAS("i2c:cyttsp5");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Cypress TrueTouch(R) Standard Product I2C driver");
MODULE_AUTHOR("Cypress Semiconductor <ttdrivers@cypress.com>");
