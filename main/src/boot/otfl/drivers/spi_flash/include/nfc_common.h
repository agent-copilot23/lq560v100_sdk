#ifndef __NFC_COMMON_H__
#define __NFC_COMMON_H__

#define MTD_MAX_ECCPOS_ENTRIES_LARGE	680
#define MTD_MAX_OOBFREE_ENTRIES_LARGE	32

#define REG_CNT_HIGH_BLOCK_NUM_SHIFT        10

#define REG_CNT_BLOCK_NUM_MASK          0x3ff
#define REG_CNT_BLOCK_NUM_SHIFT         22

#define REG_CNT_PAGE_NUM_MASK           0x3f
#define REG_CNT_PAGE_NUM_SHIFT          16

#define SPI_NAND_PAGE_SIZE_2K           2048
#define SPI_NAND_PAGE_SIZE_4K           4096

#define SPI_NAND_MAX_PAGESIZE           4096
#define SPI_NAND_MAX_OOBSIZE            256

#define FMC100_BUFFER_LEN (SPI_NAND_MAX_PAGESIZE + SPI_NAND_MAX_OOBSIZE)
/* DMA address align with 32 bytes. */
#define FMC_DMA_ALIGN				32
/*
 * Standard NAND flash commands
 */
#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_RNDOUT		5
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_RNDIN		0x85
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_PARAM		0xec
#define NAND_CMD_GET_FEATURES	0xee
#define NAND_CMD_SET_FEATURES	0xef
#define NAND_CMD_RESET		0xff
#endif /* __NFC_COMMON_H__ */
