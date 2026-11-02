/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef OSAL_INTERRUPT_REPORT_H
#define OSAL_INTERRUPT_REPORT_H

#include "osal_interrupt.h"

#define OSAL_INTERRUPT_MOD_VI_NAME "VICAP_0_0"
#define OSAL_INTERRUPT_MOD_VO_NAME "VO Int"

#define OSAL_MOD_VI_CHN_NUM 4
#define OSAL_MOD_VI_PORT_NUM 4

#define MOD_VI_REG_START_ADDR 0x17400000
#define MOV_VI_REG_ACCSESS_SIZE 0x20000

#define MOD_VI_CH_INT_REG_GAP 0x100F0
#define MOD_VI_CHN_INT_GAP 0x1000
#define MOD_VI_CH_INT_FIELD_THROW_VAL 0x00000008
#define MOD_VI_CH_INT_BUF_OVF_VAL 0x00000004
#define MOV_VI_CH_INT_EM_VUF_OVF_VAL 0x00100000
#define MOD_VI_CH_INT_STT_BUF_OVF_VAL 0x00080000

#define MOD_VI_PT_INT_REG_GAP 0x10F0
#define MOD_VI_PT_INT_GAP 0x100
#define MOD_VI_PT_INT_HEIGHT_ERR_VAL 0x00000004
#define MOD_VI_PT_INT_WIDTH_ERR_VAL 0x00000002

#define MOD_VI_CAP_INT0_REG_GAP 0x00F0
#define MOD_VI_CAP_INT0_DBG_VAL 0x04000000

#define MOD_VO_REG_START_ADDR 0x17A00000
#define MOD_VO_REG_ACCSS_SIZE 0xFF

#define MOD_VO_CTRL_REG_GAP 0x0014
#define MOD_VO_CTRL_V0_TUNL_VAL 0x00100000

#define MOD_VO_INT_STA_GAP 0x0014
#define MOD_VO_CTRL_DHD0UF_VAL 0x00000008

#define OSAL_VI_REPORT_ERR_NUM_MAX (OSAL_MOD_VI_CHN_NUM*4 + OSAL_MOD_VI_PORT_NUM*2 + 1) //chn0-chn3,port0-port3
#define OSAL_VO_REPORT_ERR_NUM_MAX 2

typedef enum {
    IRQ_VI_REPORT_CHN_INT_FILED_THROW = 0,
    IRQ_VI_REPORT_CHN_INT_BUF_OVF,
    IRQ_VI_REPORT_CHN_INT_EM_BUF_OVF,
    IRQ_VI_REPORT_CHN_INT_STT_BUF_OVF,
    IRQ_VI_REPORT_PT_INT_HEIGHT_ERR,
    IRQ_VI_REPORT_PT_INT_WIDTH_ERR,
    IRQ_VI_REPORT_CAP_INT0_DBG,
    IRQ_VI_REPORT_BUTT
} irq_vi_err_report_type;

typedef enum {
    IRQ_VO_REPORT_CTRL_V0_TUNL_ERR = 0,
    IRQ_VO_REPORT_CTRL_DHD0BUF_ERR,
    IRQ_VO_REPORT_BUTT
} irq_vo_err_report_type;

typedef struct {
    unsigned int irq;
    osal_irq_handler mod_process;
    osal_irq_handler mod_process_thread;
} irq_report_mod_info;

typedef struct {
    irq_vi_err_report_type vi_err_report_type;
    signed int chn_id;
    signed int dev_id;
} irq_report_vi_err_info;

typedef struct {
    irq_vo_err_report_type vo_err_report_type;
} irq_report_vo_err_info;

bool osal_interrupt_mod_report_list(const char *name, signed int irq);

int osal_interrupt_report_mod_register(unsigned int irq, osal_irq_handler handler, osal_irq_handler thread_fn,
    const char *name, void *dev, unsigned long flags);

int osal_interrupt_report_mod_unregister(unsigned int irq, void *dev);

void osal_interrupt_mod_reg_addr_unmap(void);

#endif
