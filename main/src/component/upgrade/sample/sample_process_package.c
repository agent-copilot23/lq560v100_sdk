/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#include <unistd.h>
#include "ot_bc.h"
#include "sample_utils.h"
#include "sample_upgrade_config.h"
#include "sample_upgrade.h"
#include "unzip.h"

extern int do_extract(unzFile uf, int opt_extract_without_path, int opt_overwrite, const char* password);
static ot_s32 unzip_package(const ot_s8 *path)
{
    ot_s32 ret = OT_FAILURE;
    unzFile uf = NULL;
    debug("start\n");
    if (sample_upgrade_is_path_exist(path)) {
        chdir(UPGRADE_PATH);
        uf = unzOpen64(path);
        if (uf != OT_NULL) {
            ret = do_extract(uf, 1, 1, OT_NULL);
            if (ret != 0) {
                ret = OT_FAILURE;
            } else {
                ret = OT_SUCCESS;
            }
        } else {
            error("unzOpen failed\n");
        }
    } else {
        error("no %s\n", path);
    }
    return ret;
}

ot_s32 sample_upgrade_process_package(ot_void *argv)
{
    ot_s32 ret = OT_FAILURE;
    struct upgrade_control *control = (struct upgrade_control *)argv;
    debug("start\n");
    CHECK_RET(argv == NULL, ret);
    ret = unzip_package(UPGRADE_PACKAGE_FILE);
    return ret;
}