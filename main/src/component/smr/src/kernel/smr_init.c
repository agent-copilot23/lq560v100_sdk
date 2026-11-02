/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */

#include <linux/module.h>
#include "ot_osal.h"
#include "smr_init.h"

static int __init smr_init(void)
{
    if (mem_check_module_param() == -1) {
        return -1;
    }
    if (media_mem_init() != 0) {
        goto failed;
    }
    osal_printk("load ot_smr ....OK!\n");
    return 0;
failed:
    osal_printk("load ot_smr failed!\n");
    return -1;
}

static void __exit smr_exit(void)
{
    media_mem_exit();
    osal_printk("unload ot_smr ....OK!\n");
}

module_init(smr_init);
module_exit(smr_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
