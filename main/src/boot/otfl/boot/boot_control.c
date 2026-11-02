/*
 * Copyright (c) Oritek Technologies Co., Ltd. 2020-2023. All rights reserved.
 */
#include "types.h"
#include "spi_nand.h"
#include "flash_map.h"
#include "boot_control.h"
#include "share_drivers.h"
#include "crc32.h"

static ot_misc_record_t gs_misc_record;
static u8 gs_active_slot = BOOT_SLOT_INVALID;

#define MISC_RECORD_VERSION_DEFINE(majorH,majorL,minorH,minorL)         ((minorL << 24) | (minorH << 16) | (majorL << 8) | (majorH << 0))
#define CUR_MISC_RECORD_VERSION                                         MISC_RECORD_VERSION_DEFINE('0','0','1','0')

#define MAX_RECORD_NUM                  4

#define ACTIVE_STATE                    15
#define NACTIVE_STATE                   14

static ot_s32 get_misc_record_data_from_block(u32 channel_type, u8 b_index, u8 r_index, ot_misc_record_t *misc_record)
{
    ot_s32 ret = EXT_FAILURE;
    ot_u32 crc32_cal;

    if (get_misc_data_from_flash(channel_type, b_index, r_index, misc_record) != EXT_SUCCESS) {
        log_serial_puts((const s8 *)"\r\nget misc data failed\r\n");
        return ret;
    }

    crc32_cal = crc32((ot_u8 *)misc_record, sizeof(*misc_record) - sizeof(ot_u32));
    if ((crc32_cal == misc_record->crc32) &&
        (misc_record->magic0 == BOOT_CTRL_MAGIC) &&
        (misc_record->magic1 == BOOT_CTRL_MAGIC)) {
        if (misc_record->version == CUR_MISC_RECORD_VERSION) {
            ret = EXT_SUCCESS;
        }
    }

    return ret;
}

ot_s32 boot_control_get_boot_slot_num(ot_u32 channel_type)
{
    ot_s32 ret = EXT_FAILURE;

    ot_bool update_misc = OT_FALSE;

    ot_u8 record_num = 0;
    ot_u8 b_index = 0;
    ot_u8 r_index = 0;

    do {
        b_index = (record_num >> 1) & 1;
        r_index = record_num & 1;
        ret = get_misc_record_data_from_block(channel_type, b_index, r_index, &gs_misc_record);
        if (ret == EXT_SUCCESS) {
            /* In True Table, Valid slot state is support, NA State is error. */
            /*State 0: program init*/
            /*State 3: SlotB update SlotA Initial APP state */
            /*State 4: SlotB update SlotA success */
            if ((gs_misc_record.slots[0].priority == ACTIVE_STATE) &&
                (gs_misc_record.slots[0].retries_remain > 0) &&
                (gs_misc_record.slots[0].is_boot_ok >= 0) &&
                (gs_misc_record.slots[1].priority == NACTIVE_STATE) &&
                (gs_misc_record.slots[1].retries_remain > 0) &&
                (gs_misc_record.slots[1].is_boot_ok != 0)) {
                gs_active_slot = BOOT_SLOT_A;
                break;
            }
            /*State 1: SlotA update SlotB Initial APP state */
            /*State 2: SlotA update SlotB success */
            if ((gs_misc_record.slots[0].priority == NACTIVE_STATE) &&
                (gs_misc_record.slots[0].retries_remain > 0) &&
                (gs_misc_record.slots[0].is_boot_ok != 0) && 
                (gs_misc_record.slots[1].priority == ACTIVE_STATE) &&
                (gs_misc_record.slots[1].retries_remain > 0) &&
                (gs_misc_record.slots[1].is_boot_ok >= 0)) {
                gs_active_slot = BOOT_SLOT_B;
                break;
            }

            /*State 1-1: SlotA update SlotB Error state, bakckup to SlotA */
            if ((gs_misc_record.slots[0].priority == NACTIVE_STATE) &&
                (gs_misc_record.slots[0].retries_remain > 0) &&
                (gs_misc_record.slots[0].is_boot_ok != 0) &&
                (gs_misc_record.slots[1].priority == ACTIVE_STATE) &&
                (gs_misc_record.slots[1].retries_remain == 0) &&
                (gs_misc_record.slots[1].is_boot_ok == 0)) {
                gs_active_slot = BOOT_SLOT_A;
                break;
            }

            /*State 3-1: SlotB update SlotA Error state, bakckup to SlotB */
            if ((gs_misc_record.slots[0].priority == ACTIVE_STATE) &&
                (gs_misc_record.slots[0].retries_remain == 0) &&
                (gs_misc_record.slots[0].is_boot_ok == 0) &&
                (gs_misc_record.slots[1].priority == NACTIVE_STATE) &&
                (gs_misc_record.slots[1].retries_remain > 0) &&
                (gs_misc_record.slots[1].is_boot_ok != 0)) {
                gs_active_slot = BOOT_SLOT_B;
                break;
            }
        }
        record_num++;
    } while(record_num < MAX_RECORD_NUM);

    if (record_num >= MAX_RECORD_NUM) {
        return EXT_FAILURE;
    }

    if (gs_misc_record.slots[gs_active_slot].is_boot_ok == 0) {
        gs_misc_record.slots[gs_active_slot].retries_remain--;
        gs_misc_record.crc32 = crc32((ot_u8 *)(&gs_misc_record), sizeof(gs_misc_record) - sizeof(ot_u32));
        update_misc = OT_TRUE;
    }

    if (update_misc || (record_num >= 2)) {
        if (record_num >= 2) {
            /* Record data from block1, Update block 1 */
            log_serial_puts((const s8 *)"\r\nupdate misc to block1\r\n");
            ret = write_misc_data_to_flash(channel_type, 1, &gs_misc_record, 2);
            if (ret != EXT_SUCCESS) {
                return ret;
            }
        }

        /* default update block0 */
        log_serial_puts((const s8 *)"\r\nupdate misc to block0\r\n");
        ret = write_misc_data_to_flash(channel_type, 0, &gs_misc_record, 2);
        if (ret != EXT_SUCCESS) {
            return ret;
        }
    }

    return EXT_SUCCESS;
}

ot_u8 boot_control_get_active_slot(void)
{
    return gs_active_slot;
}

ot_u8 boot_control_get_reset_state(void)
{
    if (gs_active_slot == BOOT_SLOT_INVALID) {
        return RESET_WFI;
    }
    if ((gs_misc_record.slots[0].retries_remain > 0) ||
        gs_misc_record.slots[1].retries_remain > 0) {
        if (gs_misc_record.slots[gs_active_slot].is_boot_ok == 0) {
            return RESET_RETRY;
        }
    }

    return RESET_WFI;
}

