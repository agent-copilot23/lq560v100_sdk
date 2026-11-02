/*
 * Copyright (c) Oritek Technologies CO., Ltd. 2020-2023. All rights reserved.
 */
#include "hal_i2c.h"
#include "hal_i2c_common.h"
#include "share_drivers.h"

static bsp_platform_i2c gs_i2c_platform_data[I2C_NUM] = {0};
static int gs_i2c_host_cfg[I2C_NUM] = {0, 1, 2, 3, 4, 5, 6, 7};
static i2c_driver_data gs_i2c_data[I2C_NUM] = {
	{I2C0_REG_BASE, CLK_LIMIT_DEFAULT, I2C_INTERRUPT_NUM},
	{I2C1_REG_BASE, CLK_LIMIT_DEFAULT, I2C_INTERRUPT_NUM},
	{I2C2_REG_BASE, CLK_LIMIT_DEFAULT, I2C_INTERRUPT_NUM},
	{I2C3_REG_BASE, CLK_LIMIT_DEFAULT, I2C_INTERRUPT_NUM},
	{I2C4_REG_BASE, CLK_LIMIT_DEFAULT, I2C_INTERRUPT_NUM},
	{I2C5_REG_BASE, CLK_LIMIT_DEFAULT, I2C_INTERRUPT_NUM},
	{I2C6_REG_BASE, CLK_LIMIT_DEFAULT, I2C_INTERRUPT_NUM},
	{I2C7_REG_BASE, CLK_LIMIT_DEFAULT, I2C_INTERRUPT_NUM},
};

static void bsp_i2c_disable(const i2c_driver_data *i2c);
static void bsp_i2c_cfg_irq(const i2c_driver_data *i2c, unsigned int flag);
static unsigned int bsp_i2c_clr_irq(const i2c_driver_data *i2c);


static void i2c2_pin_mux(void)
{
	void *iocfg4_base = (void *)IOMUX4_REG_BASE;

	writel(0x1002, (iocfg4_base + 0x14)); /* I2C2_SDA */
	writel(0x1002, (iocfg4_base + 0x1C)); /* I2C2_SCL */
}

static void bsp_i2c_rescue(const i2c_driver_data *i2c)
{
	unsigned int val;
	unsigned int time_cnt;
	unsigned int index;

	bsp_i2c_disable(i2c);
	bsp_i2c_cfg_irq(i2c, 0);
	bsp_i2c_clr_irq(i2c);

	val = (0x1 << GPIO_MODE_SHIFT) | (0x1 << FORCE_SCL_OEN_SHIFT) |
			(0x1 << FORCE_SDA_OEN_SHIFT);
	writel(val, (i2c->reg_base + BSP_I2C_CTRL2));

	time_cnt = 0U;
	do {
		for (index = 0U; index < 9U; index++) {
			val = (0x1 << GPIO_MODE_SHIFT) | 0x1;
			writel(val, (i2c->reg_base + BSP_I2C_CTRL2));
			udelay(5);
			val = (0x1 << GPIO_MODE_SHIFT) |
				(0x1 << FORCE_SCL_OEN_SHIFT) |
				(0x1 << FORCE_SDA_OEN_SHIFT);
			writel(val, (i2c->reg_base + BSP_I2C_CTRL2));
			udelay(5);
		}

		time_cnt++;
		if (time_cnt > I2C_WAIT_TIMEOUT) {
			goto disable_rescue;
		}

		val = readl((i2c->reg_base + BSP_I2C_CTRL2));
	} while (!(val & (0x1 << CHECK_SDA_IN_SHIFT)));

	val = (0x1 << GPIO_MODE_SHIFT) | (0x1 << FORCE_SCL_OEN_SHIFT) |
		(0x1 << FORCE_SDA_OEN_SHIFT);
	writel(val, (i2c->reg_base + BSP_I2C_CTRL2));

	val = (0x1 << GPIO_MODE_SHIFT) | (0x1 << FORCE_SCL_OEN_SHIFT);
	writel(val, (i2c->reg_base + BSP_I2C_CTRL2));

	udelay(10);
	val = (0x1 << GPIO_MODE_SHIFT) | (0x1 << FORCE_SCL_OEN_SHIFT) |
		(0x1 << FORCE_SDA_OEN_SHIFT);
	writel(val, (i2c->reg_base + BSP_I2C_CTRL2));

disable_rescue:
	val = (0x1 << FORCE_SCL_OEN_SHIFT) | 0x1;
	writel(val, (i2c->reg_base + BSP_I2C_CTRL2));
}

static void bsp_i2c_disable(const i2c_driver_data *i2c)
{
	unsigned int val = readl((i2c->reg_base + BSP_I2C_GLB));
	val &= ~GLB_EN_MASK;
	writel(val, (i2c->reg_base + BSP_I2C_GLB));
}

static void bsp_i2c_disable_irq(const i2c_driver_data *i2c, unsigned int flag)
{
	unsigned int val = 0U;

	val = readl((i2c->reg_base + BSP_I2C_INTR_EN));
	val &= ~flag;
	writel(val, (i2c->reg_base + BSP_I2C_INTR_EN));
}

static unsigned int bsp_i2c_clr_irq(const i2c_driver_data *i2c)
{
	unsigned int val = 0U;

	val = readl((i2c->reg_base + BSP_I2C_INTR_STAT));
	writel(INTR_ALL_MASK, (i2c->reg_base + BSP_I2C_INTR_RAW));

	return val;
}

static void bsp_i2c_set_freq(i2c_driver_data *i2c)
{
	unsigned int val = 0U;
	unsigned int freq = i2c->freq;
	unsigned int clk_rate = i2c->clk;
	unsigned int max_freq = clk_rate >> 1;

	if (freq > max_freq) {
		i2c->freq = max_freq;
		freq = i2c->freq;
	}

	if (freq <= 100000) {
		val = clk_rate / (freq * 2);
		writel(val, (i2c->reg_base + BSP_I2C_SCL_H));
		writel(val, (i2c->reg_base + BSP_I2C_SCL_L));
	} else {
		val = (clk_rate * 36) / (freq * 100);
		writel(val, (i2c->reg_base + BSP_I2C_SCL_H));
		val = (clk_rate * 64) / (freq * 100);
		writel(val, (i2c->reg_base + BSP_I2C_SCL_L));
	}

	val = readl((i2c->reg_base + BSP_I2C_GLB));
	val &= ~GLB_SDA_HOLD_MASK;
	val |= ((0xa << GLB_SDA_HOLD_SHIFT) & GLB_SDA_HOLD_MASK);
	writel(val, (i2c->reg_base + BSP_I2C_GLB));
}

static void bsp_i2c_set_water(const i2c_driver_data *i2c)
{
	writel(I2C_TXF_WATER, (i2c->reg_base + BSP_I2C_TX_WATER));
	writel(I2C_RXF_WATER, (i2c->reg_base + BSP_I2C_RX_WATER));
}

static void bsp_i2c_enable_clk(unsigned char i2c_num)
{
	const unsigned int clk_start_bit = 4U;      /* 4: i2c clk start bit */
	const unsigned int rst_start_bit = 0U;      /* 0: i2c rst start bit */
	const unsigned int cksel_start_bit = 12U;   /* 0: i2c clock select start bit */
	const unsigned int enable_clck = 1U;
	const unsigned int enable_rst = 0U;
	const unsigned int enable_cksel = 0U;	    /* 0: 50MHz, 1: 100MHz */

	write_reg_bit(enable_clck, clk_start_bit, (unsigned long int)(I2C_CRG_REG_BASE + i2c_num * 8));
	write_reg_bit(enable_rst, rst_start_bit, (unsigned long int)(I2C_CRG_REG_BASE + i2c_num * 8));
	write_reg_bit(enable_cksel, cksel_start_bit, (unsigned long int)(I2C_CRG_REG_BASE + i2c_num * 8));
}

static void bsp_i2c_init_cfg(i2c_driver_data *i2c, unsigned char i2c_num)
{
	bsp_i2c_enable_clk(i2c_num);
	bsp_i2c_disable(i2c);
	bsp_i2c_disable_irq(i2c, INTR_ALL_MASK);
	bsp_i2c_set_freq(i2c);
	bsp_i2c_set_water(i2c);
}

static void bsp_i2c_cmdreg_set(const i2c_driver_data *i2c, unsigned int cmd, unsigned int *offset)
{
	writel(cmd, (i2c->reg_base + BSP_I2C_CMD_BASE + (*offset) * 4));
	(*offset)++;
}

static void bsp_i2c_cfg_cmd(const i2c_driver_data *i2c)
{
	i2c_msg *msg = i2c->msgs;
	unsigned int offset = 0U;

	if (i2c->msg_idx == 0U)
		bsp_i2c_cmdreg_set(i2c, CMD_TX_S, &offset);
	else
		bsp_i2c_cmdreg_set(i2c, CMD_TX_RS, &offset);

	if (msg->flags & I2C_M_TEN) {
		if (i2c->msg_idx == 0U) {
			bsp_i2c_cmdreg_set(i2c, CMD_TX_D1_2, &offset);
			bsp_i2c_cmdreg_set(i2c, CMD_TX_D1_1, &offset);
		} else {
			bsp_i2c_cmdreg_set(i2c, CMD_TX_D1_2, &offset);
		}
	} else {
		bsp_i2c_cmdreg_set(i2c, CMD_TX_D1_1, &offset);
	}

	if (msg->flags & I2C_M_IGNORE_NAK)
		bsp_i2c_cmdreg_set(i2c, CMD_IGN_ACK, &offset);
	else
		bsp_i2c_cmdreg_set(i2c, CMD_RX_ACK, &offset);

	if (msg->flags & I2C_M_RD) {
		if (msg->len >= 2) {
			writel(offset, (i2c->reg_base + BSP_I2C_DST1));
			writel(msg->len - 2, (i2c->reg_base + BSP_I2C_LOOP1));
			bsp_i2c_cmdreg_set(i2c, CMD_RX_FIFO, &offset);
			bsp_i2c_cmdreg_set(i2c, CMD_TX_ACK, &offset);
			bsp_i2c_cmdreg_set(i2c, CMD_JMP1, &offset);
		}
		bsp_i2c_cmdreg_set(i2c, CMD_RX_FIFO, &offset);
		bsp_i2c_cmdreg_set(i2c, CMD_TX_NACK, &offset);
	} else {
		writel(offset, (i2c->reg_base + BSP_I2C_DST1));
		writel(msg->len - 1, (i2c->reg_base + BSP_I2C_LOOP1));
		bsp_i2c_cmdreg_set(i2c, CMD_UP_TXF, &offset);
		bsp_i2c_cmdreg_set(i2c, CMD_TX_FIFO, &offset);

		if (msg->flags & I2C_M_IGNORE_NAK) {
			bsp_i2c_cmdreg_set(i2c, CMD_IGN_ACK, &offset);
		} else {
			bsp_i2c_cmdreg_set(i2c, CMD_RX_ACK, &offset);
		}
		bsp_i2c_cmdreg_set(i2c, CMD_JMP1, &offset);
	}

	if ((i2c->msg_idx == (i2c->msg_num - 1)) || (msg->flags & I2C_M_STOP)) {
		bsp_i2c_cmdreg_set(i2c, CMD_TX_P, &offset);
	}

	bsp_i2c_cmdreg_set(i2c, CMD_EXIT, &offset);
}

static void bsp_i2c_enable(const i2c_driver_data *i2c)
{
	unsigned int val = 0U;

	val = readl((i2c->reg_base + BSP_I2C_GLB));
	val |= GLB_EN_MASK;
	writel(val, (i2c->reg_base + BSP_I2C_GLB));
}

static void bsp_i2c_set_addr(const i2c_driver_data *i2c)
{
	i2c_msg *msg = i2c->msgs;
	unsigned int addr;

	if (msg->flags & I2C_M_TEN) {
		addr = ((msg->addr & 0x300) << 1) | 0xf000;
		if (msg->flags & I2C_M_RD) {
			addr |= 1 << 8;
		}

		addr |= msg->addr & 0xff;
	} else {
		addr = (msg->addr & 0x7f) << 1;
		if (msg->flags & I2C_M_RD) {
			addr |= 1;
		}
	}

	writel(addr, (i2c->reg_base + BSP_I2C_DATA1));
}

static void bsp_i2c_start_cmd(const i2c_driver_data *i2c)
{
	unsigned int val = 0U;

	val = readl((i2c->reg_base + BSP_I2C_CTRL1));
	val |= CTRL1_CMD_START_MASK;
	writel(val, (i2c->reg_base + BSP_I2C_CTRL1));
}

static int bsp_i2c_wait_rx_noempty(const i2c_driver_data *i2c)
{
	unsigned int time_cnt = 0;
	unsigned int val = 0U;

	do {
		val = readl((i2c->reg_base + BSP_I2C_STAT));
		if (val & STAT_RXF_NOE_MASK) {
			return 0;
		}
		udelay(50);
		time_cnt++;
	} while (time_cnt < I2C_TIMEOUT_COUNT);

	bsp_i2c_rescue(i2c);

	return -1;
}

static int bsp_i2c_wait_tx_nofull(const i2c_driver_data *i2c)
{
	unsigned int time_cnt = 0U;
	unsigned int val = 0U;

	do {
		val = readl((i2c->reg_base + BSP_I2C_STAT));
		if (val & STAT_TXF_NOF_MASK) {
			return 0;
		}
		udelay(50);
		time_cnt++;
	} while (time_cnt < I2C_TIMEOUT_COUNT);

	bsp_i2c_rescue(i2c);

	return -1;
}

static int bsp_i2c_wait_idle(const i2c_driver_data *i2c)
{
	unsigned int time_cnt = 0;
	unsigned int val;

	do {
		val = readl((i2c->reg_base + BSP_I2C_INTR_RAW));
		if (val & (INTR_ABORT_MASK)) {
			return -1;
		}

		if (val & INTR_CMD_DONE_MASK) {
			return 0;
		}
		udelay(50); /* delay:50 us */
		time_cnt++;
	} while (time_cnt < I2C_WAIT_TIMEOUT);

	bsp_i2c_rescue(i2c);

	return -1;
}

static int bsp_i2c_polling_xfer_one_msg(i2c_driver_data *i2c)
{
	int status;
	unsigned int val;
	unsigned int msg_buf_ptr = 0U;
	i2c_msg *msg = i2c->msgs;

	bsp_i2c_enable(i2c);
	bsp_i2c_clr_irq(i2c);
	bsp_i2c_set_addr(i2c);
	bsp_i2c_cfg_cmd(i2c);
	bsp_i2c_start_cmd(i2c);
	if (msg->flags & I2C_M_RD) {
		while (msg_buf_ptr < msg->len) {
			status = bsp_i2c_wait_rx_noempty(i2c);
			if (status) {
				log_serial_puts((const signed char *)"bsp_i2c_wait_rx_noempty status error\n");
				goto end;
			}
			val = readl((i2c->reg_base + BSP_I2C_RXF));
			msg->buf[msg_buf_ptr] = val;
			msg_buf_ptr++;
		}
	} else {
		while (msg_buf_ptr < msg->len) {
			status = bsp_i2c_wait_tx_nofull(i2c);
			if (status) {
				log_serial_puts((const signed char *)"bsp_i2c_wait_tx_nofull status error\n");
				goto end;
			}
			val = msg->buf[msg_buf_ptr];
			writel(val, (i2c->reg_base + BSP_I2C_TXF));
			msg_buf_ptr++;
		}
	}

	status = bsp_i2c_wait_idle(i2c);
	if (status) {
		log_serial_puts((const signed char *)"bsp_i2c_wait_idle status error\n");
	}

end:
	bsp_i2c_disable(i2c);

	return status;
}

static void bsp_i2c_cfg_irq(const i2c_driver_data *i2c, unsigned int flag)
{
	writel(flag, (i2c->reg_base + BSP_I2C_INTR_EN));
}

static int bsp_i2c_xfer(unsigned char i2c_num, i2c_msg *msgs, unsigned int num)
{
	int status;
	i2c_driver_data *i2c = &gs_i2c_data[i2c_num];

	if (msgs == NULL) {
		log_serial_puts((const signed char *)"[error] msg pointer is null.\n");
		return -1;
	}

	i2c->msgs = msgs;
	i2c->msg_num = num;
	i2c->msg_idx = 0U;

	while (i2c->msg_idx < i2c->msg_num) {
		status = bsp_i2c_polling_xfer_one_msg(i2c);
		 if (status) {
			break;
		 }
		i2c->msgs++;
		i2c->msg_idx++;
	}

	if (!status || i2c->msg_idx > 0) {
		status = i2c->msg_idx;
	}

	return status;
}

static int bsp_i2c_check_enable(unsigned char i2c_num)
{
	if (i2c_num >= I2C_NUM) {
		return -1;
	}

	return gs_i2c_host_cfg[i2c_num];
}

static int bsp_i2c_recv(const i2c_client *client, unsigned char *buf, unsigned int count)
{
	int ret;
	i2c_msg msg[I2C_MESSAGE_SIZE] = {0};
	unsigned char recv_buf[I2C_BUF_SIZE] = {0};
	unsigned char i2c_num = client->i2c_num;

	if (client->reg_width == 2) {
		recv_buf[0] = (client->reg_addr >> 8) & 0xff;
		recv_buf[1] = client->reg_addr & 0xff;
	} else {
		recv_buf[0] = client->reg_addr & 0xff;
	}

	msg[0].addr = client->dev_addr;
	msg[0].flags = 0;
	msg[0].len = client->reg_width;
	msg[0].buf = recv_buf;

	msg[1].addr = client->dev_addr;
	msg[1].flags = 0;
	msg[1].flags |= I2C_M_RD;
	msg[1].len = count;
	msg[1].buf = buf;

	ret = bsp_i2c_xfer(i2c_num, msg, 2);
	if (ret < 0) {
		return -1;
	}

	return 0;
}

static int bsp_i2c_send(unsigned char i2c_num, unsigned short dev_addr, unsigned char *buf, unsigned int count)
{
	int msgs_count;
	i2c_msg msg = {0};

	msg.addr = dev_addr;
	msg.flags = 0;
	msg.len = count;
	msg.buf = buf;
	msgs_count = bsp_i2c_xfer(i2c_num, &msg, 1);

	return msgs_count;
}

static int bsp_i2c_init(unsigned char i2c_num)
{
	int ret;
	i2c_driver_data *i2c = NULL;
	bsp_platform_i2c *hpi = NULL;

	ret = bsp_i2c_check_enable(i2c_num);
	if (ret < 0) {
		log_serial_puts((const signed char *)"bsp_i2c_init error\n");
		return -1;
	}

	i2c = &gs_i2c_data[i2c_num];
	hpi = &gs_i2c_platform_data[i2c_num];
	i2c->clk = get_bus_clk();
	i2c->private = (void *)(hpi);
	i2c->irq = 0;
	bsp_i2c_init_cfg(i2c, i2c_num);

	return 0;
}

int hal_i2c_recv(const i2c_client *client, unsigned char *buf, unsigned int count)
{
	if ((client == NULL) || (buf == NULL)) {
		log_serial_puts((const signed char *)"[error] i2c recv param is null.\n");
		return -1;
	}

	if (client->i2c_num >= I2C_NUM) {
		log_serial_puts((const signed char *)"[error] i2c num is invalid.\n");
		return -1;
	}

	if (count > I2C_BUF_SIZE) {
		log_serial_puts((const signed char *)"[error] buf count should lees than 8.\n");
		return -1;
	}

	return bsp_i2c_recv(client, buf, count);
}

int hal_i2c_send(unsigned char i2c_num, unsigned short dev_addr, unsigned char *buf, unsigned int count)
{
	int ret;

	if (i2c_num >= I2C_NUM) {
		log_serial_puts((const signed char *)"[error] i2c num is invalid.\n");
		return -1;
	}

	if (buf == NULL) {
		log_serial_puts((const signed char *)"[error] i2c send param is null.\n");
		return -1;
	}

	if (count > I2C_BUF_SIZE) {
		log_serial_puts((const signed char *)"[error] buf count should lees than(8).\n");
		return -1;
	}

	ret = bsp_i2c_send(i2c_num, dev_addr, buf, count);
	if (ret < 0) {
		log_serial_puts((const signed char *)"[error] bsp i2c send error.\n");
		return ret;
	}

	return 0;
}

int hal_i2c_init(unsigned char i2c_num)
{
	int ret;

	i2c2_pin_mux();
	if (i2c_num >= I2C_NUM) {
		log_serial_puts((const signed char *)"[error] i2c num is invalid.\n");
		return -1;
	}

	ret = bsp_i2c_init(i2c_num);
	if (ret < 0) {
		log_serial_puts((const signed char *)"[error] i2c init failed.\n");
		return ret;
	}

	return 0;
}
