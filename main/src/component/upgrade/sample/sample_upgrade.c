/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#include <unistd.h>
#include <sys/reboot.h>

#include "ot_bc.h"
#include "sample_utils.h"
#include "sample_upgrade_config.h"
#include "sample_upgrade.h"
#include "sample_security.h"


#define clear_upgrade_flag()   {sample_upgrade_del_file(UPGRADE_FLAG_FILE);}


static ot_s32 need_upgrade(ot_void *control);
static ot_s32 upgrade_parts(ot_void *control);
static ot_s32 upgrade_boot_record_and_reboot(ot_void *control);
static ot_s32 upgrade_parts_entry(ot_void *argv);
static ot_s32 upgrade_verify(ot_void *control);
static  upgrade_stage_handler upgrade_stage_handlers[] = {
    {.entry = NULL,
        .process = need_upgrade,
        .exit = NULL
        },
    {.entry = NULL,
        .process = upgrade_verify,
        .exit = NULL
        },
    {.entry = NULL,
        .process = sample_upgrade_process_package,
        .exit = NULL
        },
    {.entry = upgrade_parts_entry,
        .process = upgrade_parts,
        .exit = NULL
        },
    {.entry = NULL,
        .process = upgrade_boot_record_and_reboot,
        .exit = NULL
        },
};

static struct upgrade_control g_control = {0};

static ot_void get_settings(ot_void)
{
    g_control.stree_test = sample_upgrade_get_bool_setting("upgrade_stree_test");
    g_control.quit = sample_upgrade_get_bool_setting("upgrade_quit");
}

static ot_void set_upgrade_flag(ot_bc_slot slot)
{
    ot_u8 msg[2] = {0};
    msg[0] = 'a' + slot;
    msg[1] = '\0';
    sample_upgrade_save_bin(UPGRADE_FLAG_FILE, msg, 2);
}

static ot_bool is_upgrade_in_progress(ot_void)
{
    ot_bool ret = OT_FALSE;
    if (sample_upgrade_is_path_exist(UPGRADE_FLAG_FILE)) {
        ret = OT_TRUE;
    }
    return ret;
}

static ot_bool is_upgrade_done(ot_void)
{
    ot_bool ret = OT_FALSE;
    ot_s8 *msg = OT_NULL;
    ot_bc_slot upgrade_slot = 0;
    ot_bc_slot cur = 0;
    const ot_s8  *path = UPGRADE_FLAG_FILE;
    if (sample_upgrade_is_path_exist(path)) {
        msg = sample_upgrade_read_all(path);
        if (msg != OT_NULL) {
            if (msg[0] >= 'a') {
                upgrade_slot = msg[0] - 'a';
                ot_bc_get_cur_slot(&cur);
                if (upgrade_slot == cur) {
                    ret = OT_TRUE;
                    if ((ot_bc_is_secure_enable() == OT_TRUE)) {
                        ot_u32 system_ver;
                        ot_s32 image_ver;
                        ot_bc_get_system_version(&system_ver);
                        sample_security_get_system_image_version(&image_ver);
                        if (image_ver > system_ver) {
                            printf("image_ver: %d is greater than system_ver:%d ,update system version number\n", image_ver, system_ver);
                            ot_bc_set_system_version(image_ver);
                        }
                    }
                }
                free(msg);
            } else {
                free(msg);
                error("the upgrade flag abnormal, exit upgrade sample\n");
                exit(-1);
            }
        } else {
            error("the upgrade flag abnormal, exit upgrade sample\n");
            exit(-1);
        }
    } else {
        error("this should not happend. %s is not exist when call this\n", path);
    }
    debug("ret:%d \n", ret);
    return ret;
}

ot_u32 sample_upgrade_get_about_to_upgrade_slot(struct upgrade_control *control)
{
    ot_u32 current_slot = control->current_slot;
    ot_u32 next_slot = 0;
    ot_u32 num = 0;
    num = UPGRADE_SLOT_NUM;
    next_slot = (current_slot + 1)%2;
    debug("current:%d next:%d\n", current_slot, next_slot);
    return next_slot;
}

static ot_bool has_upgrade_package(ot_void)
{
    ot_bool ret = OT_FALSE;
    if (sample_upgrade_is_path_exist(UPGRADE_PACKAGE_IMG_FILE)) {
        ret = OT_TRUE;
    }
    debug("ret:%d \n", ret);
    return ret;
}

static ot_s32 clean_upgrade_package(ot_void)
{
    ot_s32 ret = OT_FAILURE;
    ot_u32 idx = 0;
    struct upgrade_part *part = NULL;
    struct upgrade_control *control = &g_control;
    /*keep package_file for stress test*/
    if (!g_control.stree_test && sample_upgrade_is_path_exist(UPGRADE_PACKAGE_FILE)) {
        sample_upgrade_del_file(UPGRADE_PACKAGE_FILE);
    }
    if (!g_control.stree_test && sample_upgrade_is_path_exist(UPGRADE_PACKAGE_IMG_FILE)) {
        sample_upgrade_del_file(UPGRADE_PACKAGE_IMG_FILE);
    }
    ret = sample_upgrade_get_partinfo_imageinfo_from_config(control);
    if (ret != OT_SUCCESS) {
        goto out;
    }

    for (idx = 0; idx < control->upgrade_part_num; idx++) {
        ot_s8 image_path[UPGRADE_PATH_LEN] = {0};
        part = &control->parts[idx];
        snprintf(image_path,UPGRADE_PATH_LEN, "%s/%s",UPGRADE_PATH, part->image_name);
        if (sample_upgrade_is_path_exist(image_path)) {
            sample_upgrade_del_file(image_path);
        }
    }
    if (sample_upgrade_is_path_exist(UPGRADE_CONFIG_FILE)) {
        sample_upgrade_del_file(UPGRADE_CONFIG_FILE);
    }

    sync();
out:
    debug("\n");
    return ret;
}

static ot_s32 need_upgrade(ot_void *argv)
{
    ot_s32 ret = OT_FAILURE;
    ot_bool is_boot_ok = OT_FALSE;
    ot_bool upgrade_now = OT_FALSE;
    struct upgrade_control *control = (struct upgrade_control *)argv;
    warn("start\n");
    if (has_upgrade_package()) {
        if (is_upgrade_in_progress()) {
            if (is_upgrade_done()) {
                clear_upgrade_flag();
                clean_upgrade_package();
                error("upgrade done\n");
            } else {
                error("last verify(boot) failed. try upgrade(flash parts) again\n");
                upgrade_now = OT_TRUE;
            }
        } else {
            ot_bc_slot next = sample_upgrade_get_about_to_upgrade_slot(control);
            set_upgrade_flag(next);
            error("start to upgrade(flash parts)\n");
            upgrade_now = OT_TRUE;
        }
    } else {
        clear_upgrade_flag();
    }

    if (upgrade_now == OT_TRUE) {
        error("upgrade now \n");
        ret = OT_SUCCESS;
    } else {
        ret = OT_FAILURE;
    }
    return ret;
}

static ot_s32 upgrade_parts_entry(ot_void *argv)
{
    ot_s32 ret = OT_FAILURE;
    struct upgrade_control *control = (struct upgrade_control *)argv;
    CHECK_RET(argv == NULL, ret);
    return ret;
}

static ot_s32 upgrade_parts(ot_void *argv)
{
    ot_s32 ret = OT_FAILURE;
    struct upgrade_control *control = (struct upgrade_control *)argv;
    struct upgrade_part *part = NULL;
    ot_u32 idx = 0;
    ot_u8 *buff = NULL;
    ot_u32 buff_size = 256*1024;
    ot_u32 offset = 0;
    ot_s32 readed = 0;
    ot_s32 this_read = 0;
    ot_s32 this_wrote = 0;
    ot_u32 next = 0;
    debug("start\n");
    CHECK_RET(argv == NULL, ret);

    buff = malloc(buff_size);
    if (buff == NULL) {
        error("malloc fail\n");
    }
    ret = sample_upgrade_get_partinfo_imageinfo_from_config(control);
    if (ret != OT_SUCCESS) {
        goto out;
    }

    for (idx = 0; idx < control->upgrade_part_num; idx++) {
        ot_s8 part_path[128] = {0};
        ot_s8 image_path[128] = {0};
        part = &control->parts[idx];
        snprintf(part_path,128, "/dev/by-name/%s",part->part_name);
        snprintf(image_path,128, "%s/%s",UPGRADE_PATH, part->image_name);
        if (!sample_upgrade_is_path_exist(part_path)) {
            error("not exist %s\n", part_path);
            return OT_FAILURE;
        }
        if (!sample_upgrade_is_path_exist(image_path)) {
            debug("not need to upgrade %s, no image\n", part_path);
            continue;
        }
        debug("flash %s start\n", part_path);
        sample_format_file(part_path);
        debug("format %s done\n", part_path);
        offset = 0;
        readed = 0;
        do {
            this_read = sample_upgrade_read_file(image_path, offset, buff, buff_size);
            this_wrote = 0;
            if (this_read > 0) {
                this_wrote = sample_upgrade_write_file(part_path, offset, buff, this_read);
                if (this_wrote < this_read) {
                    error("flash %s failed\n", part_path);
                    ret = OT_FAILURE;
                    return ret;
                }
                offset += this_read;
                readed += this_read;
            } else {
                if (readed == 0) {
                    error("read image for %s failed\n", part_path);
                    ret = OT_FAILURE;
                    return ret;
                }
            }
            debug("this_read:0x%x this_wrote:0x%x offset:0x%x readed:0x%x\n",
                    this_read,this_wrote,offset,readed);
        } while (this_read > 0);
        debug("flash %s done\n", part_path);
    }
    sync();
    sync();
    sync();
    ret = OT_SUCCESS;
out:
    debug("ret:%d", ret);
    if (buff != NULL) {
        free(buff);
    }
    return ret;
}

static ot_s32 upgrade_boot_record_and_reboot(ot_void *argv)
{
    ot_s32 ret = OT_FAILURE;
    struct upgrade_control *control = (struct upgrade_control *)argv;
    ot_u32 current_slot = 0;
    ot_u32 next_slot = 0;
    debug("start\n");
    CHECK_RET(argv == NULL, ret);

    current_slot = control->current_slot;

    next_slot = sample_upgrade_get_about_to_upgrade_slot(control);
    warn("\n\n\t\t\tabout to reboot to verify slot:%d\n", next_slot);

    if (OT_SUCCESS != ot_bc_switch_slot(next_slot)) {
        error("switch to next slot failed\n");
        return OT_FAILURE;
    }

    /*TODO HOW to handle reboot failed*/
    ret = reboot(RB_AUTOBOOT);
    if (ret == -1) {
        error("Reboot failed:%s\n", strerror(errno));
        return OT_FAILURE;
    } else {
        error("Rebooting...\n");
        return OT_SUCCESS;
    }
}

static ot_s32 upgrade_verify(ot_void *control)
{
    ot_s32 ret = OT_FAILURE;
    ret = sample_security_verify_image(UPGRADE_PACKAGE_IMG_FILE);
    if(ret == OT_SUCCESS) {
        error("verify %s success\n", UPGRADE_PACKAGE_IMG_FILE);
    } else {
        error("verify %s failed\n", UPGRADE_PACKAGE_IMG_FILE);
    }
    return ret;
}


static ot_s32 upgrade_process(ot_void)
{
    ot_s32 ret = OT_FAILURE;
    int i = 0;
    upgrade_stage_handler *stage_handler = NULL;
    while (g_control.quit != OT_TRUE) {
        get_settings();
        ret = ot_bc_init();
        if (OT_BC_ERROR_INVALID_CRC_BLK0 == ret) {
            warn("blk0 crc is invalid,");
        }
        /* not fatal error, start upgrade  */
        if ((ret == OT_SUCCESS) || (OT_BC_ERROR_INVALID_CRC_BLK0 == ret)) {
            ot_bc_get_cur_slot(&g_control.current_slot);
            ot_bc_markup_cur_slot();
            for (i = 0;i < ARRAY_SIZE(upgrade_stage_handlers); i++) {
                ret = OT_SUCCESS;
                stage_handler = &upgrade_stage_handlers[i];
                if (stage_handler->entry != NULL) {
                    stage_handler->entry(&g_control);
                }

                if ((stage_handler->process != NULL)) {
                    ret = stage_handler->process(&g_control);
                }

                if (ret != OT_SUCCESS) {
                    break;
                }

                if ((stage_handler->exit != NULL)) {
                    ret = stage_handler->exit(&g_control);
                }
            }
        } else {
            error("boot_control init error\n");
        }

        sleep(5);
    }
    ot_bc_deinit();
    return ret;
}

ot_void sample_upgrade_get_suffix(ot_bc_slot slot, ot_s8 *suffix, ot_u32 len)
{
    if (len < 2) {
        return;
    }
    suffix[0]='_';
    suffix[1]='a' + slot;
}

int main(int argc, const char  *argv[])
{
    upgrade_process();
    return 0;
}
