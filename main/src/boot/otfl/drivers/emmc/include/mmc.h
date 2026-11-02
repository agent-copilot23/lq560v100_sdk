#ifndef __MMC_H__
#define __MMC_H__

#define MMC_CARD_BUSY    (1 << 31)    /* Card Power up status bit */

/* read or write flags */
#define MMC_DATA_WRITE       0
#define MMC_DATA_READ       1

/* Standard MMC commands (4.1)           type  argument     response */
/* class 1 */
#define MMC_GO_IDLE_STATE         0   /* bc                          */
#define MMC_SEND_OP_COND          1   /* bcr  [31:0] OCR         R3  */
#define MMC_ALL_SEND_CID          2   /* bcr                     R2  */
#define MMC_SET_RELATIVE_ADDR     3   /* ac   [31:16] RCA        R1  */
#define MMC_SWITCH                6   /* ac   [31:0] See below   R1b */
#define MMC_SELECT_CARD           7   /* ac   [31:16] RCA        R1  */
#define MMC_SEND_EXT_CSD          8   /* adtc                    R1  */
#define MMC_SEND_CSD              9   /* ac   [31:16] RCA        R2  */
#define MMC_STOP_TRANSMISSION    12   /* ac                      R1b */

/* class 2 */
#define MMC_SET_BLOCKLEN         16   /* ac   [31:0] block len   R1  */
#define MMC_READ_SINGLE_BLOCK    17   /* adtc [31:0] data addr   R1  */
#define MMC_READ_MULTIPLE_BLOCK  18   /* adtc [31:0] data addr   R1  */

/* class 4 */
#define MMC_SET_BLOCK_COUNT      23   /* adtc [31:0] data addr   R1  */
#define MMC_WRITE_BLOCK          24   /* adtc [31:0] data addr   R1  */
#define MMC_WRITE_MULTIPLE_BLOCK 25   /* adtc                    R1  */

#define MMC_RSP_PRESENT     (1 << 0)
#define MMC_RSP_136       (1 << 1)        /* 136 bit response */
#define MMC_RSP_CRC          (1 << 2)        /* expect valid crc */
#define MMC_RSP_BUSY        (1 << 3)        /* card may send busy */
#define MMC_RSP_OPCODE      (1 << 4)        /* response contains opcode */

/* Response */
#define MMC_RSP_NONE    (0)
#define MMC_RSP_R1      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2      (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3      (MMC_RSP_PRESENT)
#define MMC_RSP_R4      (MMC_RSP_PRESENT)
#define MMC_RSP_R5      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

/*
 * MMC_SWITCH access modes
 */
#define MMC_SWITCH_MODE_WRITE_BYTE    0x03    /* Set target to value */

/*
 * EXT_CSD fields
 */
#define EXT_CSD_PART_CONFIG		179	/* R/W */
#define EXT_CSD_BUS_WIDTH        183    /* R/W */
#define EXT_CSD_HS_TIMING        185    /* R/W */

/*
 * EXT_CSD field definitions
 */
#define EXT_CSD_CMD_SET_NORMAL        (1<<0)
#define EXT_CSD_PART_CONFIG_ACC_MASK	(0x7)

#define EXT_CSD_BUS_WIDTH_1    0    /* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4    1    /* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8    2    /* Card is in 8 bit mode */

#define EXT_CSD_TIMING_BC	0	/* Backwards compatility */
#define EXT_CSD_TIMING_HS	1	/* High speed */

#define MMC_VDD_165_195    (1 << 7) /* VDD voltage 1.65 - 1.95 */

#define OCR_HCS            (1 << 30)
#define OCR_BUSY           (1 << 31)
#endif /* __MMC_H__ */
