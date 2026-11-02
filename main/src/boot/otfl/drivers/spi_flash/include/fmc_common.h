#ifndef __FMC_COMMON_H__
#define __FMC_COMMON_H__

#include "../../share_drivers/share_drivers.h"

#define ECC_TYPE_0BIT				0x0
#define ECC_TYPE_8BIT				0x1
#define ECC_TYPE_16BIT				0x2
#define ECC_TYPE_24BIT				0x3
#define ECC_TYPE_28BIT				0x4
#define ECC_TYPE_40BIT				0x5
#define ECC_TYPE_64BIT				0x6

#define PAGE_SIZE_2KB				0x0
#define PAGE_SIZE_4KB				0x1
#define PAGE_SIZE_8KB				0x2
#define PAGE_SIZE_16KB				0x3

/* FMC REG */
#define FMC_CFG					0x00
#define fmc_cfg_spi_nand_sel(_type)		(((_size) & 0x3) << 11)
#define fmc_cfg_spi_nor_addr_mode(_mode)	((_mode) << 10)
#define fmc_cfg_block_size(_size)		(((_size) & 0x3) << 8)
#define fmc_cfg_ecc_type(_type)			(((_type) & 0x7) << 5)
#define fmc_cfg_page_size(_size)		(((_size) & 0x3) << 3)
#define fmc_cfg_flash_sel(_type)		(((_type) & 0x3) << 1)
#define fmc_cfg_op_mode(_mode)			((_mode) & 0x1)
#define OP_MODE_BOOT				0x0
#define OP_MODE_NORMAL				0x1
#define FLASH_TYPE_SPI_NOR			0x0
#define FLASH_TYPE_SPI_NAND			0x1
#define FLASH_TYPE_NAND				0x2
#define FLASH_TYPE_DEFAULT			0x3

#define SPI_NAND_MFR_OTHER			0x0
#define SPI_NAND_MFR_WINBOND			0x1
#define SPI_NAND_MFR_ESMT			0x2
#define SPI_NAND_MFR_MICRON			0x3

#define SPI_NAND_SEL_SHIFT			11
#define SPI_NAND_SEL_MASK			(0x3 << SPI_NAND_SEL_SHIFT)

#define SPI_NOR_ADDR_MODE_3_BYTES		0x0
#define SPI_NOR_ADDR_MODE_4_BYTES		0x1

#define SPI_NOR_ADDR_MODE_SHIFT			10
#define SPI_NOR_ADDR_MODE_MASK			(0x1 << SPI_NOR_ADDR_MODE_SHIFT)

#define BLOCK_SIZE_64_PAGE			0x0
#define BLOCK_SIZE_128_PAGE			0x1
#define BLOCK_SIZE_256_PAGE			0x2
#define BLOCK_SIZE_512_PAGE			0x3

#define _64_PAGES				64
#define _128_PAGES				128
#define _256_PAGES				256
#define _512_PAGES				512

#define EB_NORMAL				28
#define EB_2K_16_BIT				4
#define EB_4K_16_BIT				12

#define BLOCK_SIZE_MASK				(0x3 << 8)

#define ECC_TYPE_SHIFT				5
#define ECC_TYPE_MASK				(0x7 << ECC_TYPE_SHIFT)

#define PAGE_SIZE_SHIFT				3
#define PAGE_SIZE_MASK				(0x3 << PAGE_SIZE_SHIFT)

#define FLASH_TYPE_SPI_NOR			0x0
#define FLASH_TYPE_SPI_NAND			0x1
#define FLASH_TYPE_NAND				0x2
#define FLASH_TYPE_DEFAULT			0x3

#define FLASH_SEL_SHIFT				1
#define FLASH_SEL_MASK				(0x3 << FLASH_SEL_SHIFT)

#define OP_MODE_BOOT				0x0
#define OP_MODE_NORMAL				0x1

#define OP_MODE_MASK				0x1

#define FMC_GLOBAL_CFG				0x04
#define FMC_GLOBAL_CFG_WP_ENABLE		(1 << 6)
#define FMC_GLOBAL_CFG_RANDOMIZER_EN		(1 << 2)

#define FMC_SPI_TIMING_CFG			0x08
#define timing_cfg_tcsh(_n)			(((_n) & 0xf) << 8)
#define timing_cfg_tcss(_n)			(((_n) & 0xf) << 4)
#define timing_cfg_tshsl(_n)			((_n) & 0xf)

#define CS_HOLD_TIME				0x6
#define CS_SETUP_TIME				0x6
#define CS_DESELECT_TIME			0xf

#define FMC_PND_PWIDTH_CFG			0x0c
#define pwidth_cfg_rw_hcnt(_n)			(((_n) & 0xf) << 8)
#define pwidth_cfg_r_lcnt(_n)			(((_n) & 0xf) << 4)
#define pwidth_cfg_w_lcnt(_n)			((_n) & 0xf)

#ifdef CONFIG_NAND_EDO_MODE
#define RW_H_WIDTH				0x3
#define R_L_WIDTH				0x2
#define W_L_WIDTH				0x2
#define NAND_EDO_MODE_SHIFT		9
#define NAND_EDO_MODE_MASK		(1<<NAND_EDO_MODE_SHIFT)
#define set_nand_edo_mode_en(reg)	((reg) | NAND_EDO_MODE_MASK)
#else
#define RW_H_WIDTH				0xa
#define R_L_WIDTH				0xa
#define W_L_WIDTH				0xa
#endif

#define FMC_INT					0x18
#define FMC_INT_AHB_OP				(1 << 7)
#define FMC_INT_WR_LOCK				(1 << 6)
#define FMC_INT_DMA_ERR				(1 << 5)
#define FMC_INT_ERR_ALARM			(1 << 4)
#define FMC_INT_ERR_INVALID			(1 << 3)
#define FMC_INT_ERR_VALID			(1 << 2)
#define FMC_INT_OP_FAIL				(1 << 1)
#define FMC_INT_OP_DONE				(1 << 0)

#define FMC_INT_EN				0x1c
#define FMC_INT_EN_AHB_OP			(1 << 7)
#define FMC_INT_EN_WR_LOCK			(1 << 6)
#define FMC_INT_EN_DMA_ERR			(1 << 5)
#define FMC_INT_EN_ERR_ALARM			(1 << 4)
#define FMC_INT_EN_ERR_INVALID			(1 << 3)
#define FMC_INT_EN_ERR_VALID			(1 << 2)
#define FMC_INT_EN_OP_FAIL			(1 << 1)
#define FMC_INT_EN_OP_DONE			(1 << 0)

#define FMC_INT_CLR				0x20
#define FMC_INT_CLR_AHB_OP			(1 << 7)
#define FMC_INT_CLR_WR_LOCK			(1 << 6)
#define FMC_INT_CLR_DMA_ERR			(1 << 5)
#define FMC_INT_CLR_ERR_ALARM			(1 << 4)
#define FMC_INT_CLR_ERR_INVALID			(1 << 3)
#define FMC_INT_CLR_ERR_VALID			(1 << 2)
#define FMC_INT_CLR_OP_FAIL			(1 << 1)
#define FMC_INT_CLR_OP_DONE			(1 << 0)

#define FMC_INT_CLR_ALL				0xff

#define FMC_CMD					0x24
#define fmc_cmd_cmd2(_cmd)			(((_cmd) & 0xff) << 8)
#define fmc_cmd_cmd1(_cmd)			((_cmd) & 0xff)

#define FMC_ADDRH				0x28
#define fmc_addrh_set(_addr)			((_addr) & 0xff)

#define FMC_ADDRL				0x2c
#define fmc_addrl_block_h_mask(_page)		(((_page) & 0xffff) << 16)
#define fmc_addrl_block_l_mask(_page)		((_page) & 0xffc0)

#define READ_ID_ADDR				0x00
#define PROTECT_ADDR				0xa0
#define FEATURE_ADDR				0xb0
#define STATUS_ADDR				0xc0

#define FMC_OP_CFG				0x30
#define op_cfg_fm_cs(_cs)			((_cs) << 11)
#define op_cfg_force_cs_en(_en)			((_en) << 10)
#define op_cfg_mem_if_type(_type)		(((_type) & 0x7) << 7)
#define op_cfg_addr_num(_addr)			(((_addr) & 0x7) << 4)
#define op_cfg_dummy_num(_dummy)		((_dummy) & 0xf)
#define OP_CFG_OEN_EN				(0x1 << 13)

#define IF_TYPE_STD				0x0
#define IF_TYPE_DUAL				0x1
#define IF_TYPE_DIO				0x2
#define IF_TYPE_QUAD				0x3
#define IF_TYPE_QIO				0x4
#define IF_TYPE_DTR				0x4

#define IF_TYPE_SHIFT				7
#define IF_TYPE_MASK				(0x7 << IF_TYPE_SHIFT)

#define READ_ID_ADDR_NUM			1
#define FEATURES_OP_ADDR_NUM			1
#define STD_OP_ADDR_NUM				3

#define FMC_SPI_OP_ADDR				0x34

#define FMC_DATA_NUM				0x38
#define fmc_data_num_cnt(_n)			((_n) & 0x3fff)

#define SPI_NOR_SR_LEN				1 /* Status Register length */
#define SPI_NOR_CR_LEN				1 /* Config Register length */
#define FEATURES_DATA_LEN			1
#define READ_OOB_BB_LEN				1
#define MAX_SPI_NOR_ID_LEN			8
#define MAX_NAND_ID_LEN				8
#define MAX_SPI_NAND_ID_LEN			3

#define PROTECT_BRWD_MASK			(1 << 7)
#define PROTECT_BP3_MASK			(1 << 6)
#define PROTECT_BP2_MASK			(1 << 5)
#define PROTECT_BP1_MASK			(1 << 4)
#define PROTECT_BP0_MASK			(1 << 3)

#define any_bp_enable(_val) ((PROTECT_BP3_MASK & _val) || \
		(PROTECT_BP2_MASK & _val) || (PROTECT_BP1_MASK & _val) || \
		(PROTECT_BP0_MASK & _val))

#define ALL_BP_MASK				(PROTECT_BP3_MASK \
						| PROTECT_BP2_MASK \
						| PROTECT_BP1_MASK \
						| PROTECT_BP0_MASK)

#define FEATURE_ECC_ENABLE			(1 << 4)
#define FEATURE_QE_ENABLE			(1 << 0)

/* read nand id or nand status, return from nand data length */
#define MAX_NANDINFO_LEN			0x10

#define FMC_OP					0x3c
#define fmc_op_dummy_en(_en)			((_en) << 8)
#define fmc_op_cmd1_en(_en)			((_en) << 7)
#define fmc_op_addr_en(_en)			((_en) << 6)
#define fmc_op_write_data_en(_en)		((_en) << 5)
#define fmc_op_cmd2_en(_en)			((_en) << 4)
#define fmc_op_wait_ready_en(_en)		((_en) << 3)
#define fmc_op_read_data_en(_en)		((_en) << 2)
#define fmc_op_read_status_en(_en)		((_en) << 1)
#define FMC_OP_REG_OP_START			1

#define FMC_DMA_LEN				0x40
#define fmc_dma_len_set(_len)			((_len) & 0x0fffffff)
#define byte_to_mb(_x)				((_x) >> 20)

#define FMC_DMA_AHB_CTRL			0x48
#define FMC_DMA_AHB_CTRL_DMA_PP_EN		(1 << 3)
#define FMC_DMA_AHB_CTRL_BURST16_EN		(1 << 2)
#define FMC_DMA_AHB_CTRL_BURST8_EN		(1 << 1)
#define FMC_DMA_AHB_CTRL_BURST4_EN		1

#define ALL_BURST_ENABLE			(FMC_DMA_AHB_CTRL_BURST16_EN \
						| FMC_DMA_AHB_CTRL_BURST8_EN \
						| FMC_DMA_AHB_CTRL_BURST4_EN)

#define FMC_DMA_ADDR_OFFSET			4096

#define FMC_DMA_SADDR_D0			0x4c

#define FMC_DMA_SADDR_D1			0x50

#define FMC_DMA_SADDR_D2			0x54

#define FMC_DMA_SADDR_D3			0x58

#define FMC_DMA_SADDR_OOB			0x5c

#define FMC_DMA_SADDRH_D0			0x200
#define FMC_DMA_SADDRH_SHIFT		0x3LL
#define FMC_DMA_SADDRH_MASK			(FMC_DMA_SADDRH_SHIFT << 32)

#define FMC_DMA_SADDRH_OOB			0x210

#define FMC_DMA_BLK_SADDR			0x60
#define fmc_dma_blk_saddr_set(_addr)		((_addr) & 0xffffff)

#define FMC_DMA_BLK_LEN				0x64
#define fmc_dma_blk_len_set(_len)		((_len) & 0xffff)

#define FMC_OP_CTRL				0x68
#define op_ctrl_rd_opcode(_code)		(((_code) & 0xff) << 16)
#define op_ctrl_wr_opcode(_code)		(((_code) & 0xff) << 8)
#define op_ctrl_rd_op_sel(_op)			(((_op) & 0x3) << 4)
#define op_ctrl_dma_op(_type)			((_type) << 2)
#define op_ctrl_rw_op(_op)			((_op) << 1)
#define OP_CTRL_DMA_OP_READY			1

#define RD_OP_READ_ALL_PAGE			0x0
#define RD_OP_READ_OOB				0x1
#define RD_OP_BLOCK_READ			0x2

#define RD_OP_SHIFT				4
#define RD_OP_MASK				(0x3 << RD_OP_SHIFT)

#define OP_TYPE_DMA				0x0
#define OP_TYPE_REG				0x1

#define RW_OP_READ				0x0
#define RW_OP_WRITE				0x1

#define FMC_OP_PARA				0x70
#define FMC_OP_PARA_RD_OOB_ONLY			(1 << 1)

#define FMC_BOOT_SET				0x74
#define FMC_BOOT_SET_DEVICE_ECC_EN		(1 << 3)
#define FMC_BOOT_SET_BOOT_QUAD_EN		(1 << 1)

#define FMC_STATUS				0xac

#define GET_OP					0
#define SET_OP					1

#define STATUS_ECC_MASK				(0x3 << 4)
#define STATUS_P_FAIL_MASK			(1 << 3)
#define STATUS_E_FAIL_MASK			(1 << 2)
#define STATUS_WEL_MASK				(1 << 1)
#define STATUS_OIP_MASK				(1 << 0)

#define FMC_VERSION				0xbc

/* FMC IP version */
#define FMC_VER_100				0x100

#define FMC_DISABLE					0
#define FMC_ENABLE					1

#define FMC_BASE_ADDR    0x10000000

#define FMC_WR_REG(reg, val)    ((*(volatile unsigned int *)(FMC_BASE_ADDR + reg))=val)
#define FMC_RD_REG(reg)         (*(volatile unsigned int *)(FMC_BASE_ADDR + reg))

#define SPI_CMD_RDSR			0x05 /* Read Status Register */
#define SPI_CMD_WREN			0x06 /* Write Enable */
#define SPI_CMD_SE_64K			0xD8 /* 64KB sector Erase */
#define SPI_CMD_SE_64K4B 0xDC /* 64KB sector Erase 4byte mode */
#define SPI_CMD_SE_128K	 		0xD8 /* 128KB sector Erase */

#define SPI_CMD_GET_FEATURES		0x0F /* Get Features */
#define SPI_CMD_SET_FEATURE		0x1F /* Set Feature */
#define SPI_CMD_READ_QUAD	0x6B  /* 4 IO read cache only date */

#define SPI_CMD_READ_STD	0x03  /* Standard read cache */
#define SPI_CMD_READ_STD4B	0x13  /* Standard read cache 4byte mode */
#define SPI_CMD_READ_FAST	0x0B  /* Higher speed read cache */
#define SPI_CMD_READ_FAST4B	0x0C  /* Higher speed read cache 4byte mode */
#define SPI_CMD_READ_DUAL	0x3B  /* 2 IO read cache only date */
#define SPI_CMD_READ_DUAL4B	0x3C  /* 2 IO read cache only date 4byte mode */
#define SPI_CMD_READ_DUAL_ADDR	0xBB  /* 2 IO read cache date&addr */
#define SPI_CMD_READ_DUAL_ADDR4B 0xBC /* 2 IO read cache date&addr 4byte mode */
#define SPI_CMD_READ_QUAD	0x6B  /* 4 IO read cache only date */
#define SPI_CMD_READ_QUAD4B	0x6C  /* 4 IO read cache only date 4byte mode */
#define SPI_CMD_READ_QUAD_ADDR	0xEB  /* 4 IO read cache date&addr */
#define SPI_CMD_READ_QUAD_ADDR4B 0xEC /* 4 IO read cache date&addr 4byte mode */
#define SPI_CMD_READ_QUAD_DTR	0xED /* 4DTR MODE */
#define SPI_CMD_READ_QUAD_DTR4B	0xEE /* 4DTR MODE 4byte mode */
#define SPI_CMD_READ_QUAD_DTR4B_WINBOND	0xEC /* 4DTR MODE */

#define SPI_CMD_WRITE_STD	0x02 /* Standard page program */
#define SPI_CMD_WRITE_STD4B	0x12 /* Standard page program 4byte mode */
#define SPI_CMD_WRITE_DUAL	0xA2 /* 2 IO program only date */
#define SPI_CMD_WRITE_DUAL4B	0xA2 /* 2 IO program only date 4byte mode */
#define SPI_CMD_WRITE_DUAL_ADDR	 0xD2 /* 2 IO program date&addr */
#define SPI_CMD_WRITE_DUAL_ADDR4B 0xD2 /* 2 IO program date&addr 4byte mode */
#define SPI_CMD_WRITE_QUAD	0x32 /* 4 IO program only date */
#define SPI_CMD_WRITE_QUAD4B	0x34 /* 4 IO program only date 4byte mode */
#define SPI_CMD_WRITE_QUAD_ADDR	 0x38 /* 4 IO program date&addr */
#define SPI_CMD_WRITE_QUAD_ADDR4B 0x3E /* 4 IO program date&addr 4byte mode */

#define FMC100_DMA_WR_MAX_SIZE		4096
#define FMC100_DMA_WR_MASK			(FMC100_DMA_WR_MAX_SIZE - 1)
#define FMC100_DMA_RD_MAX_SIZE		(0x200000UL)
#define FMC100_DMA_RD_MASK			(FMC100_DMA_RD_MAX_SIZE - 1)
#define FMC100_REG_RD_MAX_SIZE		(0x4000UL)
#define FMC100_REG_RD_MASK			(FMC100_REG_RD_MAX_SIZE - 1)

#define SPI_NOR_SR_WIP_MASK 			(1 << 0)

enum OP {
	READ = 1,
	WRITE,
	ERASE,
};

#define FMC_WAIT_TIMEOUT	100000  /* origin: 4s equals 400000*10us; now: 1s */

#define fmc_cmd_wait_cpu_finish() \
	do { \
		unsigned val, timeout = FMC_WAIT_TIMEOUT; \
		do { \
			udelay(10); \
			val = FMC_RD_REG(FMC_OP); \
			--timeout; \
		} while ((val & FMC_OP_REG_OP_START) && timeout); \
		if (!timeout) \
			log_serial_puts((const signed char *)"\n\rError: Wait cmd cpu finish timeout!\n"); \
	} while (0)

#define fmc_dma_wait_int_finish() \
	do { \
		unsigned val, timeout = FMC_WAIT_TIMEOUT; \
		do { \
			udelay(10); \
			val = FMC_RD_REG(FMC_INT); \
			--timeout; \
		} while ((!(val & FMC_INT_OP_DONE) && timeout)); \
		if (!timeout) { \
			log_serial_puts((const signed char *)"\n\rError: Wait dma int finish timeout!\n"); \
		} \
	} while (0)

#endif /* __FMC_COMMON_H__ */
