/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#ifndef MEDIA_MEM_H
#define MEDIA_MEM_H

#include <linux/version.h>
#include "smr.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
#define smr_map_down(mm)    down_read(&(mm)->mmap_lock)
#define smr_map_up(mm)      up_read(&(mm)->mmap_lock)
#else
#define smr_map_down(mm)    down_read(&(mm)->mmap_sem)
#define smr_map_up(mm)      up_read(&(mm)->mmap_sem)
#endif

unsigned long smr_strtoul_ex(const char *s, char **ep, unsigned int base);

ot_mmb_t *media_mem_get_mmb_by_handle(const void *mem_handle);
ot_mmb_t *media_mem_get_mmb_by_phys_and_size(unsigned long phys_addr, unsigned long size);

void media_mem_clear_pid_in_all_mmb(int pid);

static inline int media_mem_is_check_pid(void)
{
    return ot_smr_get_mem_process_isolation();
}

/* function returns the index of pid array */
static inline int find_pid_in_mmb(const ot_mmb_t *mmb, int pid)
{
    int i;
    for (i = 0; i < mmb->pid_num; ++i) {
        if (mmb->pid[i] == pid) {
            return i;
        }
    }
    return -1;
}

static inline void clear_pid_in_mmb(ot_mmb_t *mmb, int pos)
{
    int cur = pos;

    for (mmb->pid_num--; cur < mmb->pid_num; ++cur) {
        mmb->pid[cur] = mmb->pid[cur + 1];
    }
    mmb->pid[mmb->pid_num] = -1;
}

#endif  /* MEDIA_MEM_H */
