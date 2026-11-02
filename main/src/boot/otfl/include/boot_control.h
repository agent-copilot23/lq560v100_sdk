/*
 * Copyright (c) Oritek Technologies Co., Ltd. 2020-2023. All rights reserved.
 */
#ifndef __BOOT_CONTROL_H__
#define __BOOT_CONTROL_H__

#include "ot_type.h"

enum {
    BOOT_SLOT_A = 0,
    BOOT_SLOT_B,
    BOOT_SLOT_INVALID = 0xFF,
};

enum {
    RESET_RETRY = 0,
    RESET_WFI,
};

ot_s32 boot_control_get_boot_slot_num(ot_u32 channel_type);
ot_u8 boot_control_get_active_slot(void);
ot_u8 boot_control_get_reset_state(void);

#endif /* __BOOT_CONTROL_H__ */
