/*
 * Copyright (c) Oritek Technologies Co., Ltd. 2020-2023. All rights reserved.
 */
#include "types.h"
#include "platform.h"
#include "lib.h"
#include "flash_map.h"
#include "common.h"
#include "cipher.h"
#include "checkup.h"
#include "share_drivers.h"
#include "err_print.h"
#include "spi_nand.h"
#include "spi_nor.h"
#include "emmc.h"
#include "ot_partition_unpack.h"
#include "boot_control.h"

static ot_u32 ree_flash_offset = 0;
static ot_u32 uboot_part_offset = 0;
static ot_u32 uboot_part_length = 0;
static ot_u32 nand_block_size = 0;

static ot_s32 get_uboot_part_flash_offset(u32 *start, u32 *length)
{
    ot_partition_bmp_info_t *bmp_info = OT_NULL;
    ot_partition_info_t *part_info = OT_NULL;
    ot_s8 *uboot_part = OT_NULL;

    if (uboot_part_offset == 0) {
        bmp_info = (ot_partition_bmp_info_t *)(uintptr_t)get_partition_area_addr();

        if (is_ab_boot_enable()) {
            ot_u8 active_slot = boot_control_get_active_slot();
            if (active_slot != BOOT_SLOT_B) {
                uboot_part = (ot_s8 *)"uboot_a";
            } else {
                uboot_part = (ot_s8 *)"uboot_b";
            }
        } else {
            uboot_part = (ot_s8 *)"uboot";
        }

        ot_s32 ret = ot_partition_get_partition_info_by_name(bmp_info, &part_info, uboot_part);
        if (ret != EXT_SEC_SUCCESS) {
            log_serial_puts((ot_s8 *)"\r\nuboot part name:");
            log_serial_puts((ot_s8 *)uboot_part);
            log_serial_puts((ot_s8 *)"not found.\r\n");
            return EXT_FAILURE;
        }

        uboot_part_offset = (ot_u32)(part_info->start_addr);
        uboot_part_length = (ot_u32)(part_info->length);
    }

    if (start) {
        *start = uboot_part_offset;
    }

    if (length) {
        *length = uboot_part_length;
    }

    return EXT_SUCCESS;
}

static u32 gs_flash_init = FALSE;
static u32 gs_last_rblock = 0; /* logic block */
static u32 gs_bad_count = 0;

static int nand_flash_read(u32 dest, u32 count, u32 offset)
{
    ot_u32 block_size = 0;
    ot_u32 uboot_part_start = 0;
    ot_u32 cur_block = 0;
    ot_u32 scan_count = 0;
    ot_u32 phy_block = 0;
    ot_u32 length = 0;
    ot_s32 ret = EXT_FAILURE;
    block_size = nand_block_size;
    ret = get_uboot_part_flash_offset(&uboot_part_start, &length);
    if (ret != EXT_SUCCESS) {
        return EXT_FAILURE;
    }

    /* uboot分区起始地址和长度必须按block对齐, 该机制由烧写工具烧写检查保证 */
    if (((uboot_part_start & (block_size - 1)) != 0) ||
        ((length & (block_size - 1)) != 0)) {
        return EXT_FAILURE;
    }

    u32 block_l = uboot_part_start / block_size;
    u32 block_h = block_l + length / block_size; /* [block_l, block_h)*/
    if (gs_flash_init != TRUE) {
        gs_last_rblock = block_l; /* logic block */
        /* 第1次读需要获取第1个好块位置, 统计第1个好块前面的坏块数量 */
        phy_block = gs_last_rblock;
        gs_bad_count = 0;
        do {
            if ((phy_block < block_l) || (phy_block >= block_h)) {
                return EXT_FAILURE;
            }
            if (is_bad_block(phy_block * block_size)) {
                gs_bad_count++;
            } else {
                break;
            }
            phy_block++;
        } while(1);
        gs_flash_init = TRUE;
    }
    cur_block = offset / block_size; /*logic block */
    phy_block = gs_last_rblock + gs_bad_count; /* logic -> phy block, has been scan, good block */
    if (cur_block < gs_last_rblock) { /* 该接口只支持递增block读 */
        return EXT_FAILURE;
    }

    if (cur_block > gs_last_rblock) {
        /* 当前读取块大于上一次读取的块, 扫描2者之间的坏块 */
        scan_count = cur_block - gs_last_rblock;
        do {
            phy_block++; /* next block */
            if ((phy_block < block_l) || (phy_block >= block_h)) {
                return EXT_FAILURE;
            }
            if (is_bad_block(phy_block * block_size)) {
                gs_bad_count++;
            } else {
                scan_count--;
            }
        } while(scan_count != 0);
    }

    /* cur block is good block */
    u32 remain = count;
    u32 tmp = block_size - (offset & (block_size - 1));
    u32 rlen = (remain > tmp) ? tmp : remain;
    u32 rcnt = 0;
    spi_nand_read((u8 *)(uintptr_t)dest, rlen, phy_block * block_size + (offset & (block_size - 1)));
    remain -= rlen;
    rcnt = rlen;

    while (remain != 0) {
        /* 读连续块扫描, 坏块则跳过 */
        phy_block++;
        if ((phy_block < block_l) || (phy_block >= block_h)) {
            return EXT_FAILURE;
        }
        if (is_bad_block(phy_block * block_size)) {
            gs_bad_count++;
        } else {
            rlen = (remain > block_size) ? block_size : remain;
            spi_nand_read((u8 *)(uintptr_t)(dest + rcnt), rlen, phy_block * block_size);
            remain -= rlen;
            rcnt += rlen;
        }
    }

    gs_last_rblock = (offset + count) / block_size;

    return EXT_SUCCESS;
}

static int flash_read(u32 dest, u32 count, u32 offset)
{
    if (is_nand()) {
        return nand_flash_read(dest, count, offset);
    } else {
        return spi_nor_read((u8 *)(uintptr_t)dest, count, offset);
    }
}

static int get_ree_key_flash_offset(u32 *addr)
{
    otfl_code_info otfl_info;
    u32 channel_type = get_data_channel_type();
    int ret;
    if (channel_type == BOOT_SEL_SDIO) {
        *addr = OTFL_CODE_OFFSET + get_otfl_code_area_len();
        return EXT_SUCCESS;
    }

    if (ree_flash_offset != 0) {
        *addr = ree_flash_offset;
        return EXT_SUCCESS;
    }

    u32 start_addr;
    ret = get_uboot_part_flash_offset(&start_addr, NULL);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    if (channel_type == BOOT_SEL_FLASH) {
        if (flash_read((u32)(uintptr_t)(&otfl_info), sizeof(otfl_code_info), start_addr + OTFL_CODE_INFO_OFFSET) != EXT_SUCCESS) {
            return EXT_FAILURE;
        }
    } else if (channel_type == BOOT_SEL_EMMC) {
        if (emmc_read((void *)(uintptr_t)&otfl_info, start_addr + OTFL_CODE_INFO_OFFSET, sizeof(otfl_code_info)) != EXT_SUCCESS) {
            return EXT_FAILURE;
        }
    }
    ree_flash_offset = start_addr + OTFL_CODE_OFFSET + otfl_info.code_area_len;
    *addr = ree_flash_offset;

    return EXT_SUCCESS;
}

static int get_param_info_flash_offset(u32 *addr)
{
    u32 offset;
    int ret = get_ree_key_flash_offset(&offset);
    if (ret != EXT_SUCCESS) {
        return ret;
    }
    *addr = offset + REE_BOOT_KEY_AREA_SIZE;

    return EXT_SUCCESS;
}

static int get_first_param_data_flash_offset(u32 *addr)
{
    u32 offset;
    int ret = get_param_info_flash_offset(&offset);
    if (ret != EXT_SUCCESS) {
        return ret;
    }
    *addr = offset + PARM_AREA_INFO_SIZE + FILL_LEN_256BYTE; /* 512byte align for sd/emmc */

    return EXT_SUCCESS;
}

static int get_cur_param_data_flash_offset(u32 board_index, u32 *addr)
{
	u32 para_len = get_ddr_param_len();
	para_area_info *para_info = (para_area_info *)(uintptr_t)get_ddr_param_info_addr();

    u32 offset;
    int ret = get_first_param_data_flash_offset(&offset);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

	*addr = offset + para_info->board_index_hash_table[board_index] * para_len;

    return EXT_SUCCESS;
}

static int get_uboot_info_flash_offset(u32 *addr)
{
	u32 ddr_para_len;
	u32 ddr_para_cnt;
    u32 offset;
	ddr_para_len = get_ddr_param_len();
	ddr_para_cnt = get_ddr_param_cnt();
    int ret = get_first_param_data_flash_offset(&offset);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

	*addr = offset + ddr_para_cnt * ddr_para_len;

    return EXT_SUCCESS;
}

static int get_uboot_code_flash_offset(u32 *addr)
{
    u32 offset;
    int ret = get_uboot_info_flash_offset(&offset);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    *addr = offset + UBOOT_CODE_INFO_SIZE;

    return EXT_SUCCESS;
}

int get_ree_key_and_paras_info_from_device(const backup_img_params_s *backup_params, u32 channel_type)
{
    u32 offset;
    u32 dst;
    u32 size;
    dst = get_ree_key_area_addr();
    int ret = get_ree_key_flash_offset(&offset);
    if (ret != EXT_SUCCESS) {
        return ret;
    }
    size = REE_BOOT_KEY_AREA_SIZE + PARM_AREA_INFO_SIZE + FILL_LEN_256BYTE;
    if (channel_type == BOOT_SEL_FLASH) {
        offset += backup_params->offset_addr;
        return flash_read(dst, size, offset);
    } else if (channel_type == BOOT_SEL_EMMC) {
        offset += backup_params->offset_addr;
        return emmc_read((void *)(uintptr_t)dst, offset, size);
    } else if (channel_type == BOOT_SEL_SDIO) {
        if (!self_sdio_check()) {
            err_print(OTFL_SDIO_CHECK_ERROR);
            return EXT_SEC_FAILURE;
        }
        enable_sdio_dma();
        set_sdio_pos(offset);
        (void)copy_from_sdio((void *)(uintptr_t)dst, size);
        return EXT_SEC_SUCCESS;
    }
    return EXT_SEC_FAILURE;
}

static int read_from_emmc(u32 dst, u32 first_offset, u32 cur_offset, u32 size, u32 board_index)
{
	u32 cycle;
	int ret = EXT_SEC_FAILURE;
	u32 para_cnt = get_ddr_param_cnt();
	u32 para_len = get_ddr_param_len();
	para_area_info *para_info = (para_area_info *)(uintptr_t)get_ddr_param_info_addr();
	u32 dst_tmp = dst + para_len;
	u32 emmc_offset = cur_offset;
	if (mmc_get_cur_mode() == BOOT_MODE) {
		emmc_offset = first_offset;
		cycle = para_info->board_index_hash_table[board_index];
		while (cycle) {
			ret = mmc_read((void *)(uintptr_t)dst_tmp, emmc_offset, para_len, READ_DATA_BY_CPU);
			if (ret != EXT_SEC_SUCCESS)
				return EXT_SEC_FAILURE;
			emmc_offset += para_len;
			cycle--;
		}
	}
	ret = mmc_read((void *)(uintptr_t)dst, emmc_offset, size, READ_DATA_BY_CPU);
	if (ret != EXT_SEC_SUCCESS)
		return EXT_SEC_FAILURE;
	if (mmc_get_cur_mode() == BOOT_MODE) {
		emmc_offset += para_len;
		cycle = para_cnt - para_info->board_index_hash_table[board_index] - 1;
		while (cycle) {
			ret = mmc_read((void *)(uintptr_t)(dst_tmp), emmc_offset, para_len, READ_DATA_BY_CPU);
			if (ret != EXT_SEC_SUCCESS)
				return EXT_SEC_FAILURE;
			emmc_offset += para_len;
			cycle--;
		}
	}
	return EXT_SEC_SUCCESS;
}

int get_paras_data_from_flash(const backup_img_params_s *backup_params, u32 board_index, u32 channel_type)
{
    /* u32 first_offset; */
    u32 cur_offset;
    u32 para_len = get_ddr_param_len();
    u32 dst;
    u32 size;
    dst = get_ddr_param_data_addr();
    int ret = get_cur_param_data_flash_offset(board_index, &cur_offset);
    if (ret != EXT_SUCCESS) {
        return EXT_FAILURE;
    }
    size = para_len;
    if (channel_type == BOOT_SEL_FLASH) {
        cur_offset += backup_params->offset_addr;
        return flash_read(dst, size, cur_offset);
    } else if (channel_type == BOOT_SEL_EMMC) {
        cur_offset += backup_params->offset_addr;
        return emmc_read((u32 *)(uintptr_t)dst, cur_offset, size);
    } else if (channel_type == BOOT_SEL_SDIO) {
        enable_sdio_dma();
        set_sdio_pos(cur_offset);
        return copy_from_sdio((void *)(uintptr_t)dst, size);
    }
    return EXT_SEC_FAILURE;
}

int get_uboot_info_from_flash(const backup_img_params_s *backup_params, u32 channel_type)
{
    u32 offset;
    u32 dst;
    u32 size;
    dst = get_uboot_info_download_ddr_addr();
    int ret = get_uboot_info_flash_offset(&offset);
    if (ret != EXT_SUCCESS) {
        return ret;
    }
    size = UBOOT_CODE_INFO_SIZE;
    if (channel_type == BOOT_SEL_FLASH) {
        offset += backup_params->offset_addr;
        return flash_read(dst, size, offset);
    } else if (channel_type == BOOT_SEL_EMMC) {
        offset += backup_params->offset_addr;
        return emmc_read((void *)(uintptr_t)dst, offset, size);
    } else if (channel_type == BOOT_SEL_SDIO) {
        enable_sdio_dma();
        set_sdio_pos(offset);
        return copy_from_sdio((void *)(uintptr_t)dst, size);
    }
    return EXT_SEC_FAILURE;
}


#define EDA_LODA_LEN 	1024
int get_uboot_code_from_flash(const backup_img_params_s *backup_params,  u32 channel_type)
{
    u32 offset;
    u32 dst;
    u32 size;
    uboot_code_info *uboot_info = (uboot_code_info *)get_uboot_info_download_ddr_addr();
    dst = uboot_info->uboot_entry_point;
    int ret = get_uboot_code_flash_offset(&offset);
    if (ret != EXT_SUCCESS) {
        return ret;
    }

    size = uboot_info->code_area_len;
#ifdef CFG_EDA_VERIFY
    size = EDA_LODA_LEN;
#endif
    if (channel_type == BOOT_SEL_FLASH) {
        offset += backup_params->offset_addr;
        return flash_read(dst, size, offset);
    } else if (channel_type == BOOT_SEL_EMMC) {
        offset += backup_params->offset_addr;
        return emmc_read((void *)(uintptr_t)dst, offset, size);
    } else if (channel_type == BOOT_SEL_SDIO) {
        enable_sdio_dma();
        set_sdio_pos(offset);
        return copy_from_sdio((void *)(uintptr_t)dst, size);
    }
    return EXT_SEC_FAILURE;
}

/* fix page 4k, block 256kB offset */
#define MISC_DATA_PAGE_SIZE        4096
#define MISC_DATA_BLOCK_SIZE       (MISC_DATA_PAGE_SIZE * 64)
int get_misc_data_from_flash(u32 channel_type, ot_u8 block_index, ot_u8 page_index, ot_misc_record_t *record)
{
    s32 ret = EXT_FAILURE;
    ot_partition_bmp_info_t *bmp_info = (ot_partition_bmp_info_t *)(uintptr_t)get_partition_area_addr();
    ot_partition_info_t *part_info;
    u32 misc_offset;
    u32 record_offset;

    ret = ot_partition_get_partition_info_by_name(bmp_info, &part_info, (ot_s8 *)"misc");
    if (ret != EXT_SUCCESS) {
        log_serial_puts((ot_s8 *)"\r\n Error: misc area not found.\r\n");
        return EXT_FAILURE;
    }

    misc_offset = part_info->start_addr;
    record_offset = misc_offset + block_index * MISC_DATA_BLOCK_SIZE + page_index * MISC_DATA_PAGE_SIZE;
    if (channel_type == BOOT_SEL_FLASH) {
        if (is_nand()) {
            return spi_nand_read((ot_u8 *)record, sizeof(ot_misc_record_t), record_offset);
        } else {
            return spi_nor_read((ot_u8 *)record, sizeof(ot_misc_record_t), record_offset);
        }
    } else if (channel_type == BOOT_SEL_EMMC) {
        return emmc_read((ot_u32 *)record, record_offset, sizeof(ot_misc_record_t));
    }

    return EXT_FAILURE;
}


int write_misc_data_to_flash(u32 channel_type, ot_u8 block_index, ot_misc_record_t *record, u8 num)
{
    s32 ret = EXT_FAILURE;
    ot_partition_bmp_info_t *bmp_info = (ot_partition_bmp_info_t *)(uintptr_t)get_partition_area_addr();
    ot_partition_info_t *part_info;
    u32 misc_offset;
    u32 record_offset;
    u32 block_size;

    ret = ot_partition_get_partition_info_by_name(bmp_info, &part_info, (ot_s8 *)"misc");
    if (ret != EXT_SUCCESS) {
        log_serial_puts((ot_s8 *)"\r\n Error: misc area not found.\r\n");
        return EXT_FAILURE;
    }

    misc_offset = part_info->start_addr;
    record_offset = misc_offset + block_index * MISC_DATA_BLOCK_SIZE;
    if (channel_type == BOOT_SEL_FLASH) {
        if (is_nand()) {
            block_size = nand_block_size;
            spi_nand_erase(record_offset, block_size);
            for (u8 i = 0; i < num; i++) {
                spi_nand_write((ot_u8 *)record, sizeof(ot_misc_record_t), record_offset);
                record_offset += MISC_DATA_PAGE_SIZE;
            }
        } else {
            block_size = 64*1024;
            spi_nor_erase(block_size, record_offset);
            for (u8 i = 0; i < num; i++) {
                spi_nor_write((ot_u8 *)record, sizeof(ot_misc_record_t), record_offset);
                record_offset += MISC_DATA_PAGE_SIZE;
            }
        }
    } else if (channel_type == BOOT_SEL_EMMC) {
        for (u8 i = 0; i < num; i++) {
            emmc_write((ot_u32 *)record, record_offset, sizeof(ot_misc_record_t));
            record_offset += MISC_DATA_PAGE_SIZE;
        }
    } else {
        return EXT_FAILURE;
    }

    return EXT_SUCCESS;
}

ot_bool is_ab_boot_enable(ot_void)
{
    ot_partition_info_t *part_info = OT_NULL;
    ot_bool enable = OT_FALSE;
    ot_partition_bmp_info_t *bmp_info = (ot_partition_bmp_info_t *)(uintptr_t)get_partition_area_addr();
    ot_partition_get_partition_info_by_name(bmp_info, &part_info,
                                                  (ot_s8 *)"misc");
    if (part_info != OT_NULL) {
        enable = OT_TRUE;
    }
    return enable;
}

int flash_device_init(void)
{
    if (is_nand()) {
        spi_nand_init();
        spi_nand_block_size(&nand_block_size);
    } else {
        return spi_nor_init();
    }

    return EXT_SEC_SUCCESS;
}
