#include "platform.h"
#include "../include/fmc_common.h"
#include "../include/nfc_common.h"

#define DMA_RAM_BUF 0x0403E000
#define PAGE_NUM_PER_BLOCK 64

static bool gs_is_quad_boot = FALSE;
static unsigned char gs_cs = 0;
static unsigned int gs_page_size = 0;
static unsigned int gs_page_shift = 0;
static unsigned int gs_block_size = 0;
static unsigned int gs_phy_erase_shift = 0;
static unsigned long gs_dma_buf = 0;
static unsigned char *gs_ddr_buf = (unsigned char *)DMA_RAM_BUF;

static void fmc100_ecc0_switch(unsigned char op, unsigned int fmc_cfg_recover)
{
    unsigned int config;
    unsigned int fmc_cfg;
    unsigned int fmc_cfg_ecc0;

    fmc_cfg = FMC_RD_REG(FMC_CFG);

    if (op == FMC_ENABLE) {
        fmc_cfg &= ~ECC_TYPE_MASK;
        fmc_cfg_ecc0 = fmc_cfg | (ECC_TYPE_0BIT << ECC_TYPE_SHIFT);
        config = fmc_cfg_ecc0;
    } else if (op == FMC_DISABLE) {
        config = fmc_cfg_recover;
    } else {
        return;
    }

    FMC_WR_REG(FMC_CFG, config);
}

static void spi_nand_set_cmd(unsigned char op, unsigned char addr, unsigned char value)
{
    unsigned int reg;

    reg = fmc_cmd_cmd1(op ? SPI_CMD_SET_FEATURE : SPI_CMD_GET_FEATURES);
    FMC_WR_REG(FMC_CMD, reg);

    FMC_WR_REG(FMC_ADDRL, addr);

    reg = op_cfg_fm_cs(gs_cs) | OP_CFG_OEN_EN | op_cfg_addr_num(FEATURES_OP_ADDR_NUM);
    FMC_WR_REG(FMC_OP_CFG, reg);

    reg = fmc_data_num_cnt(FEATURES_DATA_LEN);
    FMC_WR_REG(FMC_DATA_NUM, reg);

    reg = fmc_op_cmd1_en(FMC_ENABLE) | fmc_op_addr_en(FMC_ENABLE) | FMC_OP_REG_OP_START;

    if (op == SET_OP) {
        reg |= fmc_op_write_data_en(FMC_ENABLE);
        *(volatile unsigned char *)(SPI_BASE_ADDR) = value;
    } else {
        reg |= fmc_op_read_data_en(FMC_ENABLE);
    }

    FMC_WR_REG(FMC_OP, reg);
    fmc_cmd_wait_cpu_finish();
}

static char spi_nand_feature_op(unsigned char op, unsigned char addr, unsigned int *const value)
{
    unsigned int reg;
    unsigned char regval;
    unsigned int fmc_cfg_recover;

    if (value == NULL) {
        return EXT_FAILURE;
    }

    if ((op == GET_OP) && (addr == STATUS_ADDR)) {
        reg = op_cfg_fm_cs(gs_cs) | OP_CFG_OEN_EN;
        FMC_WR_REG(FMC_OP_CFG, reg);
        reg = fmc_op_read_status_en(FMC_ENABLE) | FMC_OP_REG_OP_START;
        FMC_WR_REG(FMC_OP, reg);
        fmc_cmd_wait_cpu_finish();
        *value = FMC_RD_REG(FMC_STATUS);
        return EXT_SUCCESS;
    }

    fmc_cfg_recover = FMC_RD_REG(FMC_CFG);
    fmc100_ecc0_switch(FMC_ENABLE, 0);
    regval = *value & 0xff;
    spi_nand_set_cmd(op, addr, regval);
    if (op == GET_OP) {
        *value = *(volatile unsigned char *)(SPI_BASE_ADDR);
    }

    fmc100_ecc0_switch(FMC_DISABLE, fmc_cfg_recover);
    return EXT_SUCCESS;
}

/*
 * Read status[C0H]:[0]bit OIP, judge whether the device is busy or not
 */
static int spi_general_wait_ready(unsigned char l_cmd)
{
    unsigned int status;
    /* just get a big number, so move left 12 bits */
    unsigned int deadline = 1 << 12;

    do {
        if (spi_nand_feature_op(GET_OP, STATUS_ADDR, &status) != EXT_SUCCESS) {
            log_serial_puts((const signed char  *)"get feature failed!\n ");
            return EXT_FAILURE;
        }
        if (!(status & STATUS_OIP_MASK)) {
            if ((l_cmd == NAND_CMD_PAGEPROG) && (status & STATUS_P_FAIL_MASK))
                return status;
            return EXT_SUCCESS;
        }

        udelay(1); /* delay 1 us */
    } while (deadline--);

    log_serial_puts((const signed char  *)"spi_general_wait_ready failed!\n ");
    return EXT_FAILURE;
}

static void write_enable_fmc_op(void)
{
    unsigned int reg;

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
}

/*
 * Send write enable cmd to SPI Nand, status[C0H]:[2]bit WEL must be set 1
 */
static int spi_nand_write_enable(void)
{
    int ret = EXT_FAILURE;
    unsigned int reg;

    ret = spi_nand_feature_op(GET_OP, STATUS_ADDR, &reg);
    if (ret != EXT_SUCCESS) {
        return EXT_FAILURE;
    }

    if (reg & STATUS_WEL_MASK) {
        return EXT_SUCCESS;
    }

    write_enable_fmc_op();
    return EXT_SUCCESS;
}

static void set_addr_reg(unsigned int page_addr)
{
    unsigned int reg;
    unsigned char pages_per_block_shift;
    unsigned int block_num;
    unsigned int block_num_h;
    unsigned int page_num;

    pages_per_block_shift = gs_phy_erase_shift - gs_page_shift;
    block_num = page_addr >> pages_per_block_shift;
    block_num_h = block_num >> REG_CNT_HIGH_BLOCK_NUM_SHIFT;
    reg = fmc_addrh_set(block_num_h);
    FMC_WR_REG(FMC_ADDRH, reg);

    page_num = page_addr - (block_num << pages_per_block_shift);
    reg = ((block_num & REG_CNT_BLOCK_NUM_MASK) << REG_CNT_BLOCK_NUM_SHIFT) |
        ((page_num & REG_CNT_PAGE_NUM_MASK) << REG_CNT_PAGE_NUM_SHIFT);
    FMC_WR_REG(FMC_ADDRL, reg);
}

static void set_cs_addr_reg(enum OP op, unsigned int page_addr)
{
    unsigned int reg;
    unsigned char iftype = 0;
    unsigned char dummy = 0;

    reg = FMC_INT_CLR_ALL;
    FMC_WR_REG(FMC_INT_CLR, reg);

    switch(op){
        case READ:
            if (gs_is_quad_boot) {
                iftype = IF_TYPE_QIO;
                dummy  = 2;
            } else {
                iftype = IF_TYPE_STD;
                dummy  = 1;
            }
            break;
        case WRITE:
            if (gs_is_quad_boot) {
                iftype = IF_TYPE_QUAD;
            } else {
                iftype = IF_TYPE_STD;
            }
            break;
        default:
            iftype = IF_TYPE_STD;
            break;
    }

    reg = op_cfg_fm_cs(gs_cs) |
        OP_CFG_OEN_EN |
        op_cfg_mem_if_type(iftype) |
        op_cfg_dummy_num(dummy);
    FMC_WR_REG(FMC_OP_CFG, reg);

    set_addr_reg(page_addr);
}

static void set_dma_addr_reg(unsigned long buf)
{
    unsigned int reg;
    unsigned long dma_oob;

    FMC_WR_REG(FMC_DMA_SADDR_D0, buf);

    /* get hight 32 bits */
    reg = ((unsigned long)buf & FMC_DMA_SADDRH_MASK) >> 32;
    FMC_WR_REG(FMC_DMA_SADDRH_D0, reg);

    dma_oob = (unsigned long)buf + gs_page_size;
    FMC_WR_REG(FMC_DMA_SADDR_OOB, dma_oob);

    /* get hight 32 bits */
    reg = (((unsigned long)dma_oob) & FMC_DMA_SADDRH_MASK) >> 32;
    FMC_WR_REG(FMC_DMA_SADDRH_OOB, reg);
}

static void erase_fmc_op(unsigned int page_addr)
{
    unsigned int reg;

    reg = FMC_INT_CLR_ALL;
    FMC_WR_REG(FMC_INT_CLR, reg);

    reg = SPI_CMD_SE_128K;
    FMC_WR_REG(FMC_CMD, fmc_cmd_cmd1(reg));

    reg = fmc_addrl_block_h_mask(page_addr >> 16) | fmc_addrl_block_l_mask(page_addr);
    FMC_WR_REG(FMC_ADDRL, reg);

    reg = op_cfg_fm_cs(gs_cs) | OP_CFG_OEN_EN |
        op_cfg_mem_if_type(IF_TYPE_STD)    |
        op_cfg_addr_num(STD_OP_ADDR_NUM)   |
        op_cfg_dummy_num(0);

    FMC_WR_REG(FMC_OP_CFG, reg);

    reg = fmc_op_cmd1_en(FMC_ENABLE) | fmc_op_addr_en(FMC_ENABLE) | FMC_OP_REG_OP_START;
    FMC_WR_REG(FMC_OP, reg);

    fmc_cmd_wait_cpu_finish();
}

int spi_nand_erase(unsigned int eraseblock, unsigned int length)
{
    int ret = EXT_FAILURE;
    unsigned int page_addr;

    while (length) {
        ret = spi_nand_write_enable();
        if (ret != EXT_SUCCESS) {
            return ret;
        }

        page_addr = eraseblock >> gs_page_shift;
        erase_fmc_op(page_addr);

        ret = spi_general_wait_ready(NAND_CMD_ERASE1);
        if (ret != EXT_SUCCESS) {
            return ret;
        }

        length -= gs_block_size;
        eraseblock += gs_block_size;
    }

    return ret;
}

static void pageprog_fmc_op(void)
{
    unsigned int reg;

    if (gs_is_quad_boot) {
        reg = op_ctrl_wr_opcode(SPI_CMD_WRITE_QUAD) | op_ctrl_dma_op(OP_TYPE_DMA) |
            op_ctrl_rw_op(RW_OP_WRITE) | OP_CTRL_DMA_OP_READY;
    } else {
        reg = op_ctrl_wr_opcode(SPI_CMD_WRITE_STD) | op_ctrl_dma_op(OP_TYPE_DMA) |
            op_ctrl_rw_op(RW_OP_WRITE) | OP_CTRL_DMA_OP_READY;
    }

    FMC_WR_REG(FMC_OP_CTRL, reg);

    fmc_dma_wait_int_finish();
}

static int spi_nand_page_prog(unsigned int offset)
{
    int ret = EXT_FAILURE;
    unsigned int page_addr;

    ret = spi_nand_write_enable();
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    page_addr = offset >> gs_page_shift;
    set_cs_addr_reg(WRITE, page_addr);

    set_dma_addr_reg(gs_dma_buf);
    pageprog_fmc_op();

    spi_general_wait_ready(NAND_CMD_PAGEPROG);

    return ret;
}

int spi_nand_write(const unsigned char *buf, unsigned int length, unsigned int offset)
{
    int ret = EXT_FAILURE;
    const unsigned char *buf_ptr = buf;
    unsigned int remain = length;
    unsigned int rlen = 0;

    while (remain) {
        rlen = (remain > gs_page_size) ? gs_page_size : remain;
        (void)memcpy_s((void *)gs_dma_buf, rlen, buf_ptr, rlen);

        ret = spi_nand_page_prog(offset);
        if (ret != EXT_SUCCESS) {
            return ret;
        }

        remain -= rlen;

        if (remain != 0) {
            offset += rlen;
            buf_ptr += rlen;
        } else {
            break;
        }
    }

    return ret;
}

static void spi_nand_page_read(unsigned int offset, bool read_page)
{
    unsigned short op_cfg;
    unsigned int page_addr;
    unsigned int reg;

    page_addr = offset >> gs_page_shift;

    if (read_page) {
        op_cfg = op_ctrl_rd_op_sel(RD_OP_READ_ALL_PAGE);
    } else {
        op_cfg = op_ctrl_rd_op_sel(RD_OP_READ_OOB);
    }

    set_cs_addr_reg(READ, page_addr);
    set_dma_addr_reg(gs_dma_buf);

    if (gs_is_quad_boot) {
        reg = op_ctrl_rd_opcode(SPI_CMD_READ_QUAD_ADDR) |
            op_cfg | op_ctrl_dma_op(OP_TYPE_DMA) |
            op_ctrl_rw_op(RW_OP_READ) | OP_CTRL_DMA_OP_READY;
        FMC_WR_REG(FMC_OP_CTRL, reg);
    } else {
        reg = op_ctrl_rd_opcode(SPI_CMD_READ_FAST) |
            op_cfg | op_ctrl_dma_op(OP_TYPE_DMA) |
            op_ctrl_rw_op(RW_OP_READ) | OP_CTRL_DMA_OP_READY;
        FMC_WR_REG(FMC_OP_CTRL, reg);
    }

    fmc_dma_wait_int_finish();
}

int spi_nand_read(unsigned char *buf, unsigned int length, unsigned int offset)
{
    unsigned char *buf_ptr = buf;
    unsigned int remain = length;
    unsigned int ofs = offset;
    unsigned int page_offset = 0;
    unsigned int rlen = 0;

    while (remain) {
        spi_nand_page_read(ofs, TRUE);

        rlen = (ofs % gs_page_size) ? (gs_page_size - ofs % gs_page_size) : gs_page_size;
        rlen = (remain > rlen) ? rlen : remain;
        page_offset = ofs & (gs_page_size - 1);

        (void)memcpy_s(buf_ptr, rlen, (void *)(gs_dma_buf + page_offset), rlen);

        remain -= rlen;
        if (remain != 0) {
            buf_ptr += rlen;
            ofs += rlen;
        } else {
            break;
        }
    }

    return EXT_SUCCESS;
}

/*
 * Send set features cmd to SPI Nand, feature[B0H]:[0]bit QE would be set
 */
static int spi_general_qe_enable(void)
{
    unsigned int reg;
    int op = gs_is_quad_boot;
    int ret = EXT_FAILURE;

    ret = spi_nand_feature_op(GET_OP, FEATURE_ADDR, &reg);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    if ((reg & FEATURE_QE_ENABLE) == op) {
        return op;
    }

    if (op == FMC_ENABLE) {
        reg |= FEATURE_QE_ENABLE;
    }
    else {
        reg &= ~FEATURE_QE_ENABLE;
    }

    ret = spi_nand_feature_op(SET_OP, FEATURE_ADDR, &reg);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    spi_general_wait_ready(NAND_CMD_READ0);

    ret = spi_nand_feature_op(GET_OP, FEATURE_ADDR, &reg);
    if (ret != EXT_SUCCESS) {
        log_serial_puts((const signed char *)"Error: Get feature reg failed\n");
        return ret;
    }

    if ((reg & FEATURE_QE_ENABLE) == op) {
        log_serial_puts((const signed char *)"SPI Nand enable Quad succeed!\n");
    }
    else {
        log_serial_puts((const signed char *)"SPI Nand enable Quad failed!\n");
    }

    return op;
}

static int spi_nand_disable_wr_protect(unsigned int* const reg)
{
    int ret = EXT_FAILURE;

    ret = spi_nand_feature_op(GET_OP, PROTECT_ADDR, reg);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    if (any_bp_enable(*reg)) {
        *reg &= ~ALL_BP_MASK;
        ret = spi_nand_feature_op(SET_OP, PROTECT_ADDR, reg);
        if (ret != EXT_SUCCESS) {
            log_serial_puts((const signed char *)"set protect reg failed!\n");
            return ret;
        }

        spi_general_wait_ready(NAND_CMD_READ0);

        ret = spi_nand_feature_op(GET_OP, PROTECT_ADDR, reg);
        if (ret != EXT_SUCCESS) {
            log_serial_puts((const signed char *)"get protect reg failed!\n");
            return ret;
        }
        if (any_bp_enable(*reg)) {
            log_serial_puts((const signed char *)"Error: Write protection disable failed!\n");
        }
    }

    return ret;
}

static int spi_nand_disable_inner_ecc(unsigned int* const reg)
{
    int ret = EXT_FAILURE;

    ret = spi_nand_feature_op(GET_OP, FEATURE_ADDR, reg);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    if (*reg & FEATURE_ECC_ENABLE) {
        *reg &= ~FEATURE_ECC_ENABLE;
        ret = spi_nand_feature_op(SET_OP, FEATURE_ADDR, reg);
        if (ret != EXT_SUCCESS) {
            return ret;
        }

        spi_general_wait_ready(NAND_CMD_READ0);

        ret = spi_nand_feature_op(GET_OP, FEATURE_ADDR, reg);
        if (ret != EXT_SUCCESS) {
            return ret;
        }

        if (*reg & FEATURE_ECC_ENABLE) {
            log_serial_puts((const signed char *)"Error: Write protection disable failed!\n");
        }
    }

    return ret;
}

void spi_nand_page_size(unsigned int *page_size)
{
    unsigned int reg_fmc_cfg;

    reg_fmc_cfg = FMC_RD_REG(FMC_CFG);

    if (reg_fmc_cfg & PAGE_SIZE_MASK) {
        *page_size = (unsigned int)(SPI_NAND_PAGE_SIZE_4K);
    } else {
        *page_size = (unsigned int)(SPI_NAND_PAGE_SIZE_2K);
    }
}

void spi_nand_block_size(unsigned int *block_size)
{
    unsigned int reg_fmc_cfg;
    unsigned int page_size = 0;

    spi_nand_page_size(&page_size);
    reg_fmc_cfg = FMC_RD_REG(FMC_CFG);

    if(reg_fmc_cfg & BLOCK_SIZE_MASK) {
        *block_size = page_size * (unsigned int)(_128_PAGES);
    } else {
        *block_size = page_size * (unsigned int)(_64_PAGES);
    }
}

bool is_nand(void)
{
    unsigned int reg_fmc_cfg;

    reg_fmc_cfg = FMC_RD_REG(FMC_CFG);
    if (reg_fmc_cfg & FLASH_SEL_MASK) {
        return TRUE;
    } else {
        return FALSE;
    }
}

bool is_bad_block(unsigned int offset)
{
    unsigned char *oob = (unsigned char *)(gs_dma_buf + gs_page_size);

    /* First Page */
    spi_nand_page_read(offset, FALSE);
    if (oob[0] != 0xFF || oob[1] != 0xFF) {
        return TRUE;
    }

    /* Last Page */
    spi_nand_page_read(offset + (PAGE_NUM_PER_BLOCK - 1) * gs_page_size, FALSE);
    if (oob[0] != 0xFF || oob[1] != 0xFF) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int spi_nand_init(void)
{
    int ret = EXT_FAILURE;
    unsigned int reg;
    unsigned int boot_sel;
    unsigned long align_mask = FMC_DMA_ALIGN - 1;

    reg = FMC_RD_REG(FMC_GLOBAL_CFG);
    if (reg & FMC_GLOBAL_CFG_WP_ENABLE) {
        reg &= ~FMC_GLOBAL_CFG_WP_ENABLE;
        FMC_WR_REG(FMC_GLOBAL_CFG, reg);
    }

    /* page size */
    spi_nand_page_size(&gs_page_size);

    if (gs_page_size == SPI_NAND_PAGE_SIZE_4K) {
        gs_page_shift = 12;      /* ffs(pagesize) - 1 */
        gs_phy_erase_shift = 18; /* ffs(erasesize) - 1, ffs(0x8) = 4*/
    } else if (gs_page_size == SPI_NAND_PAGE_SIZE_2K) {
        gs_page_shift = 11;
        gs_phy_erase_shift = 17;
    }

    /* block size */
    spi_nand_block_size(&gs_block_size);

    /* STD or QUAD */
    boot_sel = FMC_RD_REG(FMC_BOOT_SET);
    if (boot_sel & FMC_BOOT_SET_BOOT_QUAD_EN) {
        gs_is_quad_boot = TRUE;
    } else {
        gs_is_quad_boot = FALSE;
    }

    (void)memset_s(gs_ddr_buf, FMC100_BUFFER_LEN + FMC_DMA_ALIGN, 0xff, FMC100_BUFFER_LEN + FMC_DMA_ALIGN);

    /* DMA need 32 bytes alignment */
    gs_dma_buf = (unsigned long)(gs_ddr_buf + align_mask) & ~align_mask;

    /* normal mode */
    reg = FMC_RD_REG(FMC_CFG);
    if ((reg & OP_MODE_MASK) == OP_MODE_BOOT) {
        reg |= fmc_cfg_op_mode(OP_MODE_NORMAL);
        FMC_WR_REG(FMC_CFG, reg);
    }

    reg = timing_cfg_tcsh(CS_HOLD_TIME) |
        timing_cfg_tcss(CS_SETUP_TIME) |
        timing_cfg_tshsl(CS_DESELECT_TIME);
    FMC_WR_REG(FMC_SPI_TIMING_CFG, reg);

    reg = ALL_BURST_ENABLE;
    FMC_WR_REG(FMC_DMA_AHB_CTRL, reg);

    if (gs_is_quad_boot) {
        spi_general_qe_enable();
    }

    /* Disable write protection */
    ret = spi_nand_disable_wr_protect(&reg);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    /* Disable chip internal ECC */
    ret = spi_nand_disable_inner_ecc(&reg);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    return ret;
}

void spi_nand_deinit(void)
{
    int reg;

    reg = FMC_RD_REG(FMC_CFG);
    reg &= ~0x1;
    reg |= fmc_cfg_op_mode(OP_MODE_BOOT);
    FMC_WR_REG(FMC_CFG, reg);
}
