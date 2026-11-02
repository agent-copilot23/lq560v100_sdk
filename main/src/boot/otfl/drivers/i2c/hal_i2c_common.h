/*
 * Copyright (c) Oritek Technologies CO., Ltd. 2020-2023. All rights reserved.
 */
#ifndef __HAL_I2C_COMMON_H__
#define __HAL_I2C_COMMON_H__

/*-----------------------------------------------------------------------------------
 * i2c register
 *-----------------------------------------------------------------------------------*/
#define I2C0_REG_BASE		(0x11060000U)
#define I2C1_REG_BASE		(0x11061000U)
#define I2C2_REG_BASE		(0x11062000U)
#define I2C3_REG_BASE		(0x11063000U)
#define I2C4_REG_BASE		(0x11064000U)
#define I2C5_REG_BASE		(0x11065000U)
#define I2C6_REG_BASE		(0x11066000U)
#define I2C7_REG_BASE		(0x11067000U)
#define I2C_NUM 8

#define BSP_I2C_GLB			(0x0U)
#define BSP_I2C_SCL_H		(0x4U)
#define BSP_I2C_SCL_L		(0x8U)
#define BSP_I2C_DATA1		(0x10U)
#define BSP_I2C_TXF			(0x20U)
#define BSP_I2C_RXF			(0x24U)
#define BSP_I2C_CMD_BASE	(0x30U)
#define BSP_I2C_LOOP1		(0xB0U)
#define BSP_I2C_DST1		(0xB4U)
#define BSP_I2C_TX_WATER	(0xC8U)
#define BSP_I2C_RX_WATER	(0xCCU)
#define BSP_I2C_CTRL1		(0xD0U)
#define BSP_I2C_CTRL2		(0xD4U)
#define BSP_I2C_STAT		(0xD8U)
#define BSP_I2C_INTR_RAW	(0xE0U)
#define BSP_I2C_INTR_EN		(0xE4U)
#define BSP_I2C_INTR_STAT	(0xE8U)

/* I2C Timing CMD Register -- BSP_I2C_CMD_BASE + n * 4 (n = 0, 1, 2, ... 31) */
#define CMD_EXIT			(0x0U)
#define CMD_TX_S			(0x1U)
#define CMD_TX_D1_2			(0x4U)
#define CMD_TX_D1_1			(0x5U)
#define CMD_TX_FIFO			(0x9U)
#define CMD_RX_FIFO			(0x12U)
#define CMD_RX_ACK			(0x13U)
#define CMD_IGN_ACK			(0x15U)
#define CMD_TX_ACK			(0x16U)
#define CMD_TX_NACK			(0x17U)
#define CMD_JMP1			(0x18U)
#define CMD_UP_TXF			(0x1dU)
#define CMD_TX_RS			(0x1eU)
#define CMD_TX_P			(0x1fU)

#define BIT(nr)			    (1UL << (nr))
/* I2C Global Config Register -- BSP_I2C_GLB */
#define GLB_EN_MASK 		BIT(0)
#define GLB_SDA_HOLD_MASK 	(0xffff00)
#define GLB_SDA_HOLD_SHIFT 	(8U)

/* I2C Control Register 1 -- BSP_I2C_CTRL1 */
#define CTRL1_CMD_START_MASK 	BIT(0)

/* I2C Status Register -- BSP_I2C_STAT */
#define STAT_RXF_NOE_MASK 	BIT(16) /* RX FIFO not empty flag */
#define STAT_TXF_NOF_MASK 	BIT(19) /* TX FIFO not full flag */

/*
 * I2C Interrupt status and mask Register --
 * BSP_I2C_INTR_RAW, BSP_I2C_STAT, BSP_I2C_INTR_STAT
 */
#define INTR_ABORT_MASK 	(BIT(0) | BIT(11))
#define INTR_RX_MASK 		BIT(2)
#define INTR_TX_MASK 		BIT(4)
#define INTR_CMD_DONE_MASK 	BIT(12)
#define INTR_USE_MASK 		(INTR_ABORT_MASK | INTR_RX_MASK | INTR_TX_MASK | INTR_CMD_DONE_MASK)
#define INTR_ALL_MASK 		(0xffffffffU)
#define I2C_TXF_DEPTH 		(64U)
#define I2C_RXF_DEPTH 		(64U)
#define I2C_TXF_WATER 		(32U)
#define I2C_RXF_WATER 		(32U)
/* for i2c rescue */
#define CHECK_SDA_IN_SHIFT 	(16U)
#define GPIO_MODE_SHIFT 	(8U)
#define FORCE_SCL_OEN_SHIFT (4U)
#define FORCE_SDA_OEN_SHIFT (0U)

#define PERI_CRG4256        (0x4280)
#define I2C_CRG_REG_BASE	(REG_BASE_CRG + PERI_CRG4256)
#define IOMUX4_REG_BASE     (0x11120200)

#define CLK_LIMIT_DEFAULT 	(400000)
#define GET_UINT32(uwAddr) 	(*((volatile unsigned int *)(uwAddr)))
#define WRITE_UINT32(uwValue, uwAddr)	(*((volatile unsigned int *)(uwAddr)) = (uwValue))

#define write_reg_bit(value, offset, addr) ({ \
												unsigned long t, mask;           \
												mask = 1 << (offset);            \
												t = GET_UINT32(addr);            \
												t &= ~mask;                      \
												t |= ((value) << (offset)) & mask; \
												WRITE_UINT32(t, addr);             \
											})
#define get_bus_clk() 		(50000000U)
#define I2C_WAIT_TIMEOUT 	(0x400)
#define I2C_TIMEOUT_COUNT 	(0x10000)
#define I2C_MESSAGE_SIZE 	(2)
#define I2C_BUF_SIZE 		(8)
#define I2C_INTERRUPT_NUM 	(0)

typedef struct {
	unsigned int msg_buf_ptr;
	int status;
#define I2C_WAIT_RESPOND (1 << 0)
} bsp_platform_i2c;

typedef struct {
	unsigned int freq;
	unsigned int clk;
} i2c_bsp_platform_data;

typedef struct {
	unsigned short addr; /* slave address */
	unsigned short flags;
#define I2C_M_TEN 0x0010
#define I2C_M_RD 0x0001
#define I2C_M_STOP 0x8000
#define I2C_M_NOSTART 0x4000
#define I2C_M_REV_DIR_ADDR 0x2000
#define I2C_M_IGNORE_NAK 0x1000
#define I2C_M_NO_RD_ACK 0x0800
#define I2C_M_RECV_LEN 0x0400
#define I2C_M_16BIT_DATA 0x0008
#define I2C_M_16BIT_REG 0x0002
	unsigned short len; /* msg length */
	unsigned char *buf; /* pointer to msg data */
} i2c_msg;

typedef struct {
	unsigned int reg_base;
	unsigned int freq;
	unsigned int irq;
	unsigned int clk;
	i2c_msg *msgs;
	unsigned int msg_num;
	unsigned int msg_idx;
	unsigned int lock;
	void *private;
} i2c_driver_data;

#endif /* __HAL_I2C_COMMON_H__ */
