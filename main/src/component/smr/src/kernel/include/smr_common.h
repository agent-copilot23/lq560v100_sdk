/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */

#ifndef SMR_COMMON_H
#define SMR_COMMON_H

#include "smr.h"

#define SMR_GRAIN   PAGE_SIZE

#define smr_align2(x, g)        (((g) == 0) ? (0) : ((((x) + (g) - 1) / (g)) * (g)))
#define smr_grain_align(x)      smr_align2(x, SMR_GRAIN)

#ifdef CONFIG_OT_LOG_TRACE_SUPPORT
#define smr_printk(fmt...) osal_printk(fmt)
#else
#define smr_printk(fmt...)
#endif

#define smr_trace(level, fmt, params...)    \
    smr_printk(level "smr_userdev:%s: \n" fmt, __FUNCTION__, ##params)

#define smr_err_trace(fmt, params...)   smr_trace(KERN_ERR, fmt, ##params)
#define smr_warn_trace(fmt, params...)  smr_trace(KERN_WARNING, fmt, ##params)

#define SMR_DBG_LEVEL       0x0
#define SMR_INFO_LEVEL      0x1

/* do not print by default */
#define smr_trace_ctrl(level, fmt, params...)       \
    do {                                            \
        if (level & SMR_DBG_LEVEL) {                \
            smr_trace(KERN_INFO, fmt, ##params);    \
        }                                           \
    } while (0)

#define smr_debug_info(fmt, params...)  smr_trace_ctrl(SMR_INFO_LEVEL, fmt, ##params)

#define SMR_MEM_NAME  "smr"

#define SMR_PARAM_NUM   4

struct mmb_node {
    struct mmb_info mmb_info;
    int map_ref;
    int mmb_ref;
    struct osal_list_head list;
    ot_mmb_t *mmb;
} __attribute__((aligned(8)));

int ot_smr_register(ot_smr_t *zone);
int ot_smr_unregister(ot_smr_t *zone);

int ot_mmb_get(ot_mmb_t *mmb);
int ot_mmb_put(ot_mmb_t *mmb);

int ot_smr_get_anony(void);
struct osal_list_head *ot_smr_get_smr_list(void);

static inline int smr_default_filter(const ot_smr_t *smr, unsigned long gfp, const char *smr_name)
{
    if ((gfp != 0) && (smr->gfp != gfp)) {
        return -1;
    }
    if ((smr_name == NULL) || (*smr_name == '\0')) {
        if (ot_smr_get_anony() != 1) {
            return -1;
        }
        smr_name = "anonymous";
    }
    return (strcmp(smr_name, smr->name) == 0) ? 0 : -1;
}

ot_mmb_t *media_mem_get_mmb_by_phys(unsigned long phys_addr);
void media_mem_put_mmb(ot_mmb_t *mmb);

/* for smr userdev */
int smr_userdev_init(void);
void smr_userdev_exit(void);

#endif /* SMR_COMMON_H */
