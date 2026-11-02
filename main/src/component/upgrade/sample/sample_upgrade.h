/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef SAMPLE_UPGRADE_H
#define SAMPLE_UPGRADE_H
#include "ot_bc.h"
#include "sample_upgrade_config.h"



#define NAME_LEN    64
struct upgrade_part{
    char part_name[NAME_LEN];
    char image_name[NAME_LEN];
};

struct upgrade_control{
    ot_bool quit;
    ot_bool stree_test;
    ot_u32 stage;
    ot_u32 progress;
    ot_u32 current_slot;
    ot_u32 upgrade_part_num;
    struct upgrade_part parts[UPGRADE_MAX_PART_NUM];
};

typedef ot_s32 (*upgrade_stage_process)(ot_void *control);
typedef ot_s32 (*upgrade_stage_entry)(ot_void* control);
typedef ot_s32 (*upgrade_stage_exit)(ot_void* control);
typedef struct {
    upgrade_stage_entry entry;
    upgrade_stage_entry process;
    upgrade_stage_entry exit;
}upgrade_stage_handler;

typedef enum{
    NEED_OTA,
    HANDEL_PACKAGE,
    FLASH_PARTITIONS,
    VEFIFY_PARTITIONS,
    VERIFY_BY_BOOT
} upgrade_state;
ot_void sample_upgrade_get_suffix(ot_bc_slot slot, ot_s8 *suffix, ot_u32 len);
ot_u32 sample_upgrade_get_about_to_upgrade_slot(struct upgrade_control *control);
ot_s32 sample_upgrade_process_package(ot_void *control);
ot_s32 sample_upgrade_get_partinfo_imageinfo_from_config(ot_void *control);
#endif /* !SAMPLE_UPGRADE_H */