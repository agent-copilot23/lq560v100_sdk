#include "../include/fmc_common.h"

// #define SUPPORT_4BYTE_ADDRESS

#ifdef SUPPORT_4BYTE_ADDRESS
#define ADDRESS_CYCLE    4
#else
#define ADDRESS_CYCLE    3
#endif

#define ERASE_SIZE    (64 * 1024)

static unsigned char gs_cs = 0;

static void spi_general_get_flash_register(unsigned char cmd, unsigned char *status)
{
    unsigned int reg;

    reg = op_cfg_fm_cs(gs_cs) | OP_CFG_OEN_EN;
    FMC_WR_REG(FMC_OP_CFG, reg);

    if (cmd == SPI_CMD_RDSR) {
        reg = fmc_op_read_status_en(FMC_ENABLE) | FMC_OP_REG_OP_START;
        goto cmd_config_done;
    }

    FMC_WR_REG(FMC_CMD, cmd);

    reg = fmc_data_num_cnt(SPI_NOR_CR_LEN);

    FMC_WR_REG(FMC_DATA_NUM, reg);

    reg = fmc_op_cmd1_en(FMC_ENABLE) | fmc_op_read_data_en(FMC_ENABLE) |
        FMC_OP_REG_OP_START;

cmd_config_done:
    FMC_WR_REG(FMC_OP, reg);
    fmc_cmd_wait_cpu_finish();

    if (cmd == SPI_CMD_RDSR)
        *status = FMC_RD_REG(FMC_STATUS);
    else
        *status = (*(volatile unsigned int *)FMC_BASE_ADDR);
}

/*
 *	Read status[C0H]:[0]bit OIP, judge whether the device is busy or not
 */
static int spi_general_wait_ready(void)
{
    unsigned char status;
    /* need a big number,so move left 20 bit */
    unsigned int deadline = 1 << 20;

    do {
        spi_general_get_flash_register(SPI_CMD_RDSR, &status);
        if (!(status & SPI_NOR_SR_WIP_MASK))
            return EXT_SUCCESS;

        udelay(1); /* delay 1 us */
    } while (deadline--);

    return EXT_FAILURE;
}

static int spi_nor_write_en(void)
{
    int ret = EXT_FAILURE;
    unsigned char status;
    unsigned int reg;

    spi_general_get_flash_register(SPI_CMD_RDSR, &status);
    if (status & STATUS_WEL_MASK) {
        return EXT_SUCCESS;
    }

    reg = FMC_RD_REG(FMC_GLOBAL_CFG);
    if (reg & FMC_GLOBAL_CFG_WP_ENABLE) {
        reg &= ~FMC_GLOBAL_CFG_WP_ENABLE;
        FMC_WR_REG(FMC_GLOBAL_CFG, reg);
    }

    reg = fmc_cmd_cmd1(SPI_CMD_WREN);
    FMC_WR_REG(FMC_CMD, reg);

    reg = op_cfg_fm_cs(gs_cs) | OP_CFG_OEN_EN;
    FMC_WR_REG(FMC_OP_CFG, reg);

    reg = fmc_op_cmd1_en(FMC_ENABLE) | FMC_OP_REG_OP_START;
    FMC_WR_REG(FMC_OP, reg);

    fmc_cmd_wait_cpu_finish();

    ret = spi_general_wait_ready();
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    spi_general_get_flash_register(SPI_CMD_RDSR, &status);
    if (status & STATUS_WEL_MASK) {
        // log_serial_puts((const s8 *)"Write Enable success.\n");
    } else {
        return EXT_FAILURE;
    }

    return EXT_SUCCESS;
}

static int spi_nor_erase_one_block(unsigned int offset)
{
    int ret = EXT_FAILURE;
    unsigned int regval;

    ret = spi_general_wait_ready();
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    ret = spi_nor_write_en();
    if (ret != EXT_SUCCESS) {
        return ret;
    }

#ifdef SUPPORT_4BYTE_ADDRESS
    regval = fmc_cmd_cmd1(SPI_CMD_SE_64K4B);
#else
    regval = fmc_cmd_cmd1(SPI_CMD_SE_64K);
#endif

    FMC_WR_REG(FMC_CMD, regval);

    regval = offset;
    FMC_WR_REG(FMC_ADDRL, regval);

    regval = op_cfg_fm_cs(gs_cs) |
        OP_CFG_OEN_EN |
        op_cfg_mem_if_type(IF_TYPE_STD) |
        op_cfg_addr_num(ADDRESS_CYCLE) |
        op_cfg_dummy_num(0);
    FMC_WR_REG(FMC_OP_CFG, regval);

    regval = fmc_op_cmd1_en(FMC_ENABLE) |
        fmc_op_addr_en(FMC_ENABLE) |
        FMC_OP_REG_OP_START;
    FMC_WR_REG(FMC_OP, regval);

    fmc_cmd_wait_cpu_finish();

    return EXT_SUCCESS;
}

int spi_nor_erase(unsigned int length, unsigned int offset)
{
    int ret = EXT_FAILURE;

    if(length % ERASE_SIZE) {
        return EXT_FAILURE;
    }

    while (length) {
        ret = spi_nor_erase_one_block(offset);
        if (ret != EXT_SUCCESS) {
            return ret;
        }

        offset += ERASE_SIZE;
        length -= ERASE_SIZE;
    }

    return EXT_SUCCESS;
}

static void fmc100_dma_transfer(unsigned int spi_start_addr, unsigned char* const dma_buffer,
        unsigned char rw_op, unsigned int size)
{
    unsigned char if_type = 0;
    unsigned char dummy = 0;
    unsigned char w_cmd = 0;
    unsigned char r_cmd = 0;
    unsigned int regval;

    regval = FMC_INT_CLR_ALL;
    FMC_WR_REG(FMC_INT_CLR, regval);

    regval = spi_start_addr;
    FMC_WR_REG(FMC_ADDRL, regval);

    if (rw_op == RW_OP_WRITE) {
        if_type = IF_TYPE_STD;
        dummy = 0;
#ifdef SUPPORT_4BYTE_ADDRESS
        w_cmd = SPI_CMD_WRITE_STD4B;
#else
        w_cmd = SPI_CMD_WRITE_STD;
#endif
    } else if (rw_op == RW_OP_READ) {
        if_type = IF_TYPE_STD;
        dummy = 1;
#ifdef SUPPORT_4BYTE_ADDRESS
        r_cmd = SPI_CMD_READ_FAST4B;
#else
        r_cmd = SPI_CMD_READ_FAST;
#endif
    }

    regval = op_cfg_fm_cs(gs_cs) | OP_CFG_OEN_EN |
        op_cfg_mem_if_type(if_type) | op_cfg_addr_num(ADDRESS_CYCLE) |
        op_cfg_dummy_num(dummy);
    FMC_WR_REG(FMC_OP_CFG, regval);

    regval = fmc_dma_len_set(size);
    FMC_WR_REG(FMC_DMA_LEN, regval);

    /* get hight 32 bits */
    regval = (((unsigned long)dma_buffer & FMC_DMA_SADDRH_MASK) >> 32);
    FMC_WR_REG(FMC_DMA_SADDRH_D0, regval);

    regval = (unsigned int)((unsigned long)dma_buffer);
    FMC_WR_REG(FMC_DMA_SADDR_D0, regval);

    regval = op_ctrl_rd_opcode(r_cmd) | op_ctrl_wr_opcode(w_cmd) |
        op_ctrl_rw_op(rw_op) | OP_CTRL_DMA_OP_READY;

    FMC_WR_REG(FMC_OP_CTRL, regval);

    fmc_dma_wait_int_finish();
}

static int dma_cycle_op(unsigned char rw_op, unsigned int from, unsigned int len, const void *buf)
{
    int ret = EXT_FAILURE;
    int op_len;
    unsigned int num;

    if (rw_op == RW_OP_READ) {
        op_len = FMC100_DMA_RD_MAX_SIZE;
    } else {
        op_len = FMC100_DMA_WR_MAX_SIZE;
    }

    while (len) {
        num = ((len >= op_len) ? op_len : len);

        ret = spi_general_wait_ready();
        if (ret != EXT_SUCCESS) {
            return ret;
        }

        fmc100_dma_transfer(from, (unsigned char *)buf, rw_op, num);
        from += num;
        buf  += num;
        len  -= num;
    }

    return EXT_SUCCESS;
}

int spi_nor_write(const unsigned char *buf, unsigned int length, unsigned int offset)
{
    int ret = EXT_FAILURE;

    ret = spi_general_wait_ready();
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    ret = spi_nor_write_en();
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    ret = dma_cycle_op(RW_OP_WRITE, offset, length, buf);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    return EXT_SUCCESS;
}

int spi_nor_read(unsigned char* const buf, unsigned int length, unsigned int offset)
{
    int ret = EXT_FAILURE;

    ret = spi_general_wait_ready();
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    ret = dma_cycle_op(RW_OP_READ, offset, length, buf);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    return EXT_SUCCESS;
}

int spi_nor_init(void)
{
    unsigned int reg;

    /* normal mode */
    reg = FMC_RD_REG(FMC_CFG);
    if ((reg & OP_MODE_MASK) == OP_MODE_BOOT) {
        reg |= fmc_cfg_op_mode(OP_MODE_NORMAL);
#ifdef SUPPORT_4BYTE_ADDRESS
        reg |= fmc_cfg_spi_nor_addr_mode(SPI_NOR_ADDR_MODE_4_BYTES);
#endif
        FMC_WR_REG(FMC_CFG, reg);
    }

    reg = timing_cfg_tcsh(CS_HOLD_TIME) |
        timing_cfg_tcss(CS_SETUP_TIME) |
        timing_cfg_tshsl(CS_DESELECT_TIME);
    FMC_WR_REG(FMC_SPI_TIMING_CFG, reg);

    return EXT_SUCCESS;
}

void spi_nor_deinit(void)
{
    int reg;

    reg = FMC_RD_REG(FMC_CFG);
    reg &= ~0x1;
    reg |= fmc_cfg_op_mode(OP_MODE_BOOT);
    FMC_WR_REG(FMC_CFG, reg);
}

