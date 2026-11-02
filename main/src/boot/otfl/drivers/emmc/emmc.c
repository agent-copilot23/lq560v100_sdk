#include <string.h>
#include "types.h"
#include "common.h"
#include "platform.h"
#include "../share_drivers/share_drivers.h"
#include "../uart/serial_pl011.h"
#include "./include/cmd.h"
#include "./include/mmc.h"
#include "./include/emmc_common.h"

#define EMMC_BUF 0x0403E000
#define EMMC_TIMEOUT 3000
#define MAX_SDMA_BUFFER (512U * 1024U)
#define SYSSTAT (0x11020018U)
#define SFC_EMMC_BOOT_MASK (0x800U)

/* 1-Byte Read Register: */
#define ot_readb(addr) (*((volatile unsigned char *)(addr)))
/* 2-Byte Read Register: */
#define ot_readw(addr) (*((volatile unsigned short *)(addr)))
/* 4-Byte Read Register: */
#define ot_readl(addr) (*((volatile unsigned int *)(addr)))

/* 1-Byte Write Register: */
#define ot_writeb(value, addr) (*((volatile unsigned char *)(addr)) = value)
/* 2-Byte Write Register: */
#define ot_writew(value, addr) (*((volatile unsigned short *)(addr)) = value)
/* 4-Byte Write Register: */
#define ot_writel(value, addr) (*((volatile unsigned int *)(addr)) = value)

static int sdhci_send_cmd(struct mmc_command *cmd);

static int mmc_set_block_size(unsigned short block_len)
{
    struct mmc_command cmd = {
        .opcode = MMC_SET_BLOCKLEN,
        .arg = block_len,
        .flags = MMC_RSP_R1,
        .rsvd = {0U,0U,0U},
        .resp = {0U,0U,0U,0U},
        .data = NULL
    };

    return sdhci_send_cmd(&cmd);
}

static int mmc_set_block_count(unsigned int block_count)
{
    struct mmc_command cmd = {
        .opcode = MMC_SET_BLOCK_COUNT,
        .arg = block_count,
        .flags = MMC_RSP_R1,
        .rsvd = {0U,0U,0U},
        .resp = {0U,0U,0U,0U},
        .data = NULL
    };

    return sdhci_send_cmd(&cmd);
}

static void sdhci_cmd_set(struct mmc_command *cmd)
{
    unsigned short val = 0U;

    switch (cmd->flags) {
    case MMC_RSP_NONE:
        val = SDHCI_CMD_RESP_NONE;
        break;
    case MMC_RSP_R1:
    case MMC_RSP_R3:
        val = SDHCI_CMD_RESP_SHORT;
        break;
    case MMC_RSP_R1B:
        val = SDHCI_CMD_RESP_SHORT_BUSY;
        break;
    case MMC_RSP_R2:
        val = SDHCI_CMD_RESP_LONG;
        break;
    default:
        return;
        break;
    }

    val |= (unsigned short)(cmd->opcode << 8U);

    if(cmd->data) {
        val |= SDHCI_CMD_DATA;
    }

    ot_writew(val, SDHCI_BASE_ADDR + SDHCI_COMMAND);
}

static int sdhci_check_int_status(unsigned int mask)
{
    unsigned int reg = 0U;

    unsigned int timer_out = EMMC_TIMEOUT * timer_get_divider();

    timer_start();

    for (;;) {
        reg = ot_readl(SDHCI_BASE_ADDR + SDHCI_INT_STATUS);

        if (reg & mask) {
            break;
        }
        if (timer_get_val() > timer_out) {
            log_serial_puts((const signed char *)"emmc_write int time out\n");
            return EXT_FAILURE;
        }
        if (reg & SDHCI_INT_ERROR_MASK) {
            log_serial_puts((const signed char *)"emmc_write int error\n");
            return EXT_FAILURE;
        }
    }

    return EXT_SUCCESS;
}

static int sdhci_send_cmd(struct mmc_command *cmd)
{
    int err;
    unsigned short mode = 0U;
    unsigned int stat= 0U;
    unsigned int timeout = 100000U;
    unsigned int start_addr = (uintptr_t)(cmd->data->data_buf);
    unsigned int sysstat;

    sysstat = ot_readl(SYSSTAT);
    if (sysstat & SFC_EMMC_BOOT_MASK) {
        ot_writeb((unsigned char)(SDHCI_CTRL_8BITBUS | SDHCI_CTRL_SDMA), SDHCI_BASE_ADDR + SDHCI_HOST_CONTROL);
    } else {
        ot_writeb((unsigned char)(SDHCI_CTRL_4BITBUS | SDHCI_CTRL_SDMA), SDHCI_BASE_ADDR + SDHCI_HOST_CONTROL);
    }

    ot_writew((unsigned char)(SDHCI_CTRL_UHS_SDR12), SDHCI_BASE_ADDR + SDHCI_HOST_CONTROL2);

    if (cmd->data) {
        ot_writew((unsigned short)(cmd->data->blk_cnt), SDHCI_BASE_ADDR + SDHCI_BLOCK_COUNT);
        ot_writew((unsigned short)(BLOCK_SIZE | (0x7U << 12)), SDHCI_BASE_ADDR + SDHCI_BLOCK_SIZE);    /* 0x7:512KB buffer boundary */

        mmc_set_block_size(BLOCK_SIZE);
        mmc_set_block_count(cmd->data->blk_cnt);

        mode |= (unsigned short)(SDHCI_TRNS_DMA | SDHCI_TRNS_BLK_CNT_EN);
        if (cmd->data->blk_cnt > 1U) {
            mode |= (unsigned short)(SDHCI_TRNS_MULTI);
        }

        if (cmd->data->flags == MMC_DATA_READ) {
            ot_writew((unsigned short)(SDHCI_TRNS_READ) | mode, SDHCI_BASE_ADDR + SDHCI_TRANSFER_MODE);
        } else {
            ot_writew((unsigned short)(mode), SDHCI_BASE_ADDR + SDHCI_TRANSFER_MODE);
        }

        ot_writel(start_addr, SDHCI_BASE_ADDR + SDHCI_DMA_ADDRESS);
    }

    ot_writel((unsigned int)SDHCI_INT_MASK, SDHCI_BASE_ADDR + SDHCI_INT_STATUS);
    ot_writel(cmd->arg, SDHCI_BASE_ADDR + SDHCI_ARGUMENT);

    sdhci_cmd_set(cmd);

    err = sdhci_check_int_status(SDHCI_INT_RESPONSE);
    if (err != EXT_SUCCESS) {
        return err;
    }

    ot_writel((unsigned int)SDHCI_INT_RESPONSE, SDHCI_BASE_ADDR + SDHCI_INT_STATUS);

    if (cmd->flags != MMC_RSP_NONE) {
        cmd->resp[0] = ot_readl(SDHCI_BASE_ADDR + SDHCI_RESPONSE);
    }

    if(cmd->data) {
        do {
            stat = ot_readl(SDHCI_BASE_ADDR + SDHCI_INT_STATUS);
            if (stat & SDHCI_INT_DMA_END) {
                ot_writel((unsigned int)SDHCI_INT_DMA_END, SDHCI_BASE_ADDR + SDHCI_INT_STATUS);
                start_addr &= ~(MAX_SDMA_BUFFER - 1U);
                start_addr += MAX_SDMA_BUFFER;
                ot_writel((uintptr_t)start_addr, SDHCI_BASE_ADDR + SDHCI_DMA_ADDRESS);
            }

            if (timeout-- > 0U) {
                udelay(10U);
            } else {
                return EXT_FAILURE;
            }
        } while (!(stat & SDHCI_INT_DATA_END));
        ot_writel((unsigned int)SDHCI_INT_DATA_END, SDHCI_BASE_ADDR + SDHCI_INT_STATUS);
    }

    return EXT_SUCCESS;
}

static int mmc_write_block(const unsigned int *data_buf, unsigned int block, unsigned int block_count)
{
    struct mmc_data data = {
        .blk_cnt = block_count,
        .blk_sz = BLOCK_SIZE,
        .flags = MMC_DATA_WRITE,
        .rsvd = 0U,
        .data_buf = (unsigned int *)data_buf
    };

    struct mmc_command cmd = {
        .opcode = block_count > 1U ? MMC_WRITE_MULTIPLE_BLOCK : MMC_WRITE_BLOCK,
        .arg = block,
        .flags = MMC_RSP_R1,
        .rsvd = {0U,0U,0U},
        .resp = {0U,0U,0U,0U},
        .data = &data
    };

   return sdhci_send_cmd(&cmd);
}

static int mmc_read_block(unsigned int *data_buf, unsigned int block, unsigned int block_count)
{
    struct mmc_data data = {
        .blk_cnt = block_count,
        .blk_sz = BLOCK_SIZE,
        .flags = MMC_DATA_READ,
        .rsvd = 0U,
        .data_buf = data_buf
    };

    struct mmc_command cmd = {
        .opcode = block_count > 1U ? MMC_READ_MULTIPLE_BLOCK : MMC_READ_SINGLE_BLOCK,
        .arg = block,
        .flags = MMC_RSP_R1,
        .rsvd = {0U,0U,0U},
        .resp = {0U,0U,0U,0U},
        .data = &data
    };

   return sdhci_send_cmd(&cmd);
}

int emmc_write(const unsigned int *buf, unsigned int offset, unsigned int length)
{
    int ret = EXT_FAILURE;
    const unsigned int *buf_ptr = buf;
    unsigned int block = offset / BLOCK_SIZE;
    unsigned int rlen = length & (BLOCK_SIZE - 1);
    unsigned int block_count = (length - rlen) / BLOCK_SIZE;
    unsigned int ofs = 0;

    if (block_count >= 1) {
        ret = mmc_write_block(buf, block, block_count);
        if (ret != EXT_SUCCESS) {
            return ret;
        }

        block += block_count;
    }

    if (rlen) {
        ofs = (block_count == 0) ? 0 : (block_count * BLOCK_SIZE);
        (void)memset_s((void *)EMMC_BUF, BLOCK_SIZE, 0xFF, BLOCK_SIZE);
        (void)memcpy_s((void *)EMMC_BUF, rlen, (unsigned char *)buf_ptr + ofs, rlen);
        ret = mmc_write_block((unsigned int *)EMMC_BUF, block, 1);
        if (ret != EXT_SUCCESS) {
            return ret;
        }
    }

    return EXT_SUCCESS;
}

int emmc_read(unsigned int *buf, unsigned int offset, unsigned int length)
{
    int ret = EXT_FAILURE;
    unsigned int i;
    unsigned int block = offset / BLOCK_SIZE;
    unsigned int block_count = (length + BLOCK_SIZE - 1) / BLOCK_SIZE;
    unsigned int rlen = 0;
    unsigned int remain = length;
    unsigned int ofs = offset;
    unsigned int *buf_ptr = buf;

    for (i = 0; i < block_count; i++) {
        ret = mmc_read_block((unsigned int *)EMMC_BUF, block + i, 1);
        if (ret != EXT_SUCCESS) {
            return ret;
        }

        rlen = (ofs % BLOCK_SIZE) ? (BLOCK_SIZE - ofs % BLOCK_SIZE) : BLOCK_SIZE;
        rlen = (remain > rlen) ? rlen : remain;

        (void)memcpy_s(buf_ptr, rlen, (unsigned int *)EMMC_BUF + ofs % BLOCK_SIZE, rlen);

        buf_ptr = (unsigned int *)((unsigned char *)buf_ptr + rlen);
        ofs += rlen;
        remain -= rlen;
    }

    return EXT_SUCCESS;
}
