/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/vendor/semu.h>
#include <asm/io.h>
#include <linux/io.h>
#include "osal_addr.h"
#include "ot_osal.h"
#include "securec.h"
#include "osal_inner.h"
#include "osal_interrupt.h"
#include "osal_interrupt_report.h"

#define osal_mod_get_delta_ptr(base, gap)         ((unsigned int *)((uintptr_t)base + (gap)))

static OSAL_LIST_HEAD(g_irq_mod_report_list);
static irq_report_mod_info irq_mod_vi = {0};
static irq_report_mod_info irq_mod_vo = {0};
static bool vi_err_exist = FALSE;
static bool vo_err_exist = FALSE;
static void* mod_vi_virt = NULL;
static void* mod_vo_virt = NULL;

bool osal_interrupt_mod_report_list(const char *name, signed int irq)
{
    if (name != NULL && (osal_strcmp(OSAL_INTERRUPT_MOD_VI_NAME, name) == 0 || osal_strcmp(OSAL_INTERRUPT_MOD_VO_NAME, name) == 0)) {
        return TRUE;
    }

    if(irq != -1 && (irq == irq_mod_vi.irq || irq == irq_mod_vo.irq)) {
        return TRUE;
    }
    return FALSE;
}

static void osal_interrupt_mod_vi_gamble_err(irq_report_vi_err_info vi_report[OSAL_VI_REPORT_ERR_NUM_MAX], int *cnt)
{
    int iter_index = 0;
    unsigned int *vi_chn_int_stat = NULL;
    unsigned int vi_chn_int_valu = 0;
    unsigned int *vi_port_int_stat = NULL;
    unsigned int vi_port_int_valu = 0;
    unsigned int *vi_cap_int_stat = NULL;
    unsigned int vi_cap_int_valu = 0;

    for(iter_index = 0; iter_index < OSAL_MOD_VI_CHN_NUM; iter_index++) {
        vi_chn_int_stat = osal_mod_get_delta_ptr(mod_vi_virt, MOD_VI_CH_INT_REG_GAP + iter_index * MOD_VI_CHN_INT_GAP);
        vi_chn_int_valu = *vi_chn_int_stat;
        if(vi_chn_int_valu & MOD_VI_CH_INT_FIELD_THROW_VAL) {
            vi_report[*cnt].vi_err_report_type = IRQ_VI_REPORT_CHN_INT_FILED_THROW;
            vi_report[*cnt].chn_id = iter_index;
            *cnt = (*cnt) + 1;
        }

        if(vi_chn_int_valu & MOD_VI_CH_INT_BUF_OVF_VAL) {
            vi_report[*cnt].vi_err_report_type = IRQ_VI_REPORT_CHN_INT_BUF_OVF;
            vi_report[*cnt].chn_id = iter_index;
            *cnt = (*cnt) + 1;
        }

        if(vi_chn_int_valu & MOV_VI_CH_INT_EM_VUF_OVF_VAL) {
            vi_report[*cnt].vi_err_report_type = IRQ_VI_REPORT_CHN_INT_EM_BUF_OVF;
            vi_report[*cnt].chn_id = iter_index;
            *cnt = (*cnt) + 1;
        }

        if(vi_chn_int_valu & MOD_VI_CH_INT_STT_BUF_OVF_VAL) {
            vi_report[*cnt].vi_err_report_type = IRQ_VI_REPORT_CHN_INT_STT_BUF_OVF;
            vi_report[*cnt].chn_id = iter_index;
            *cnt = (*cnt) + 1;
        }
    }

    for(iter_index = 0; iter_index < OSAL_MOD_VI_PORT_NUM; iter_index++) {
        vi_port_int_stat = osal_mod_get_delta_ptr(mod_vi_virt, MOD_VI_PT_INT_REG_GAP + iter_index * MOD_VI_PT_INT_GAP);
        vi_port_int_valu = *vi_port_int_stat;
        if(vi_port_int_valu & MOD_VI_PT_INT_HEIGHT_ERR_VAL) {
            vi_report[*cnt].vi_err_report_type = IRQ_VI_REPORT_PT_INT_HEIGHT_ERR;
            vi_report[*cnt].dev_id = iter_index;
            *cnt = (*cnt) + 1;
        }

        if(vi_port_int_valu & MOD_VI_PT_INT_WIDTH_ERR_VAL) {
            vi_report[*cnt].vi_err_report_type = IRQ_VI_REPORT_PT_INT_WIDTH_ERR;
            vi_report[*cnt].dev_id = iter_index;
            *cnt = (*cnt) + 1;
        }
    }

    vi_cap_int_stat = osal_mod_get_delta_ptr(mod_vi_virt, MOD_VI_CAP_INT0_REG_GAP);
    vi_cap_int_valu = *vi_cap_int_stat;
    if(vi_cap_int_valu & MOD_VI_CAP_INT0_DBG_VAL) {
        vi_report[*cnt].vi_err_report_type = IRQ_VI_REPORT_CAP_INT0_DBG;
        *cnt = (*cnt) + 1;
    }
}

static void osal_interrupt_get_reg_and_clear_bit_value(unsigned int *virt, unsigned int bit_value)
{
    *virt = bit_value;
}

static void osal_interrupt_mod_vi_send_err_and_clear(irq_report_vi_err_info vi_report[OSAL_VI_REPORT_ERR_NUM_MAX], int cnt)
{
    int iter_index = 0;
    semu_err_info info = {0};
    signed int chn_id = 0;
    signed int dev_id = 0;
    unsigned int *chn_virt = NULL;
    unsigned int *dev_virt = NULL;

    if(vi_err_exist && cnt == 0) {
        vi_err_exist = FALSE;
    }

    if(cnt > 0) {
        vi_err_exist = TRUE;
    }

    for(iter_index = 0; iter_index < cnt; iter_index++) {
        chn_id = vi_report[iter_index].chn_id;
        dev_id = vi_report[iter_index].dev_id;
        chn_virt = osal_mod_get_delta_ptr(mod_vi_virt, MOD_VI_CH_INT_REG_GAP + chn_id * MOD_VI_CHN_INT_GAP);
        dev_virt = osal_mod_get_delta_ptr(mod_vi_virt, MOD_VI_PT_INT_REG_GAP + dev_id * MOD_VI_PT_INT_GAP);

        switch (vi_report[iter_index].vi_err_report_type) {
        case IRQ_VI_REPORT_CHN_INT_FILED_THROW:
            info.vi.chn_id = chn_id;
            semu_send_err(SEMU_SM_VI_ERR_MONITOR, SEMU_VI_ERR_IN_FRAME_SEQ, &info);
            osal_interrupt_get_reg_and_clear_bit_value(chn_virt, MOD_VI_CH_INT_FIELD_THROW_VAL);
            break;
        case IRQ_VI_REPORT_CHN_INT_BUF_OVF:
            info.vi.chn_id = chn_id;
            semu_send_err(SEMU_SM_VI_ERR_MONITOR, SEMU_VI_ERR_FIFO_OVERFLOW, &info);
            osal_interrupt_get_reg_and_clear_bit_value(chn_virt, MOD_VI_CH_INT_BUF_OVF_VAL);
            break;
        case IRQ_VI_REPORT_CHN_INT_EM_BUF_OVF:
            info.vi.chn_id = chn_id;
            semu_send_err(SEMU_SM_VI_ERR_MONITOR, SEMU_VI_ERR_FIFO_OVERFLOW, &info);
            osal_interrupt_get_reg_and_clear_bit_value(chn_virt, MOV_VI_CH_INT_EM_VUF_OVF_VAL);
            break;
        case IRQ_VI_REPORT_CHN_INT_STT_BUF_OVF:
            info.vi.chn_id = chn_id;
            semu_send_err(SEMU_SM_VI_ERR_MONITOR, SEMU_VI_ERR_FIFO_OVERFLOW, &info);
            osal_interrupt_get_reg_and_clear_bit_value(chn_virt, MOD_VI_CH_INT_STT_BUF_OVF_VAL);
            break;
        case IRQ_VI_REPORT_PT_INT_HEIGHT_ERR:
            info.vi.dev_id = dev_id;
            semu_send_err(SEMU_SM_VI_ERR_MONITOR, SEMU_VI_ERR_RESOLUTION_CHANGE, &info);
            osal_interrupt_get_reg_and_clear_bit_value(dev_virt, MOD_VI_PT_INT_HEIGHT_ERR_VAL);
            break;
        case IRQ_VI_REPORT_PT_INT_WIDTH_ERR:
            info.vi.dev_id = dev_id;
            semu_send_err(SEMU_SM_VI_ERR_MONITOR, SEMU_VI_ERR_RESOLUTION_CHANGE, &info);
            osal_interrupt_get_reg_and_clear_bit_value(dev_virt, MOD_VI_PT_INT_WIDTH_ERR_VAL);
            break;
        case IRQ_VI_REPORT_CAP_INT0_DBG:
            semu_send_err(SEMU_SM_VI_ERR_MONITOR, SEMU_VI_ERR_BUS_ACCESS, &info);
            osal_interrupt_get_reg_and_clear_bit_value(osal_mod_get_delta_ptr(mod_vi_virt, MOD_VI_CAP_INT0_REG_GAP), MOD_VI_CAP_INT0_DBG_VAL);
            break;
        default:
            break;
        }
    }
}

static int osal_interrupt_mod_vi_process(int irq, void *dev_id)
{
    irq_report_vi_err_info vi_report[OSAL_VI_REPORT_ERR_NUM_MAX] = {0};
    int report_cnt = 0;
    int ret = OSAL_IRQ_HANDLED;

    osal_interrupt_mod_vi_gamble_err(vi_report, &report_cnt);

    if(irq_mod_vi.mod_process != NULL) {
        ret = irq_mod_vi.mod_process(irq, dev_id);
    }

    osal_interrupt_mod_vi_send_err_and_clear(vi_report, report_cnt);
    return ret;
}

static int osal_interrupt_mod_vi_process_thread(int irq, void *dev_id)
{
    if(irq_mod_vi.mod_process_thread != NULL) {
        irq_mod_vi.mod_process_thread(irq, dev_id);
    }

    return OSAL_IRQ_HANDLED;
}

static void osal_interrupt_mod_vo_gamble_err(irq_report_vo_err_info vo_report[OSAL_VO_REPORT_ERR_NUM_MAX], int *cnt)
{
    unsigned int *vo_ctrl_vo_tunl_stat = NULL;
    unsigned int vo_ctrl_vo_tunl_valu = 0;

    vo_ctrl_vo_tunl_stat = osal_mod_get_delta_ptr(mod_vo_virt, MOD_VO_CTRL_REG_GAP);

    vo_ctrl_vo_tunl_valu = *vo_ctrl_vo_tunl_stat;
    if(vo_ctrl_vo_tunl_valu & MOD_VO_CTRL_V0_TUNL_VAL) {
        vo_report[*cnt].vo_err_report_type = IRQ_VO_REPORT_CTRL_V0_TUNL_ERR;
        *cnt = (*cnt) + 1;
    }

    if(vo_ctrl_vo_tunl_valu & MOD_VO_CTRL_DHD0UF_VAL) {
        vo_report[*cnt].vo_err_report_type = IRQ_VO_REPORT_CTRL_DHD0BUF_ERR;
        *cnt = (*cnt) + 1;
    }
}

static void osal_interrupt_mod_vo_send_err_and_clear(irq_report_vo_err_info vo_report[OSAL_VO_REPORT_ERR_NUM_MAX], int cnt)
{
    int iter_index = 0;
    semu_err_info info = {0};

    if(vo_err_exist && cnt == 0) {
        vo_err_exist = FALSE;
    }

    if(cnt > 0) {
        vo_err_exist = TRUE;
    }

    for(iter_index = 0; iter_index < cnt; iter_index++) {
        switch (vo_report[iter_index].vo_err_report_type) {
        case IRQ_VO_REPORT_CTRL_V0_TUNL_ERR:
            semu_send_err(SEMU_SM_VO_ERR_MONITOR, SEMU_VO_ERR_LOW_DELAY, &info);
            osal_interrupt_get_reg_and_clear_bit_value(osal_mod_get_delta_ptr(mod_vo_virt, MOD_VO_CTRL_REG_GAP), MOD_VO_CTRL_V0_TUNL_VAL);
            break;
        case IRQ_VO_REPORT_CTRL_DHD0BUF_ERR:
            semu_send_err(SEMU_SM_VO_ERR_MONITOR, SEMU_VO_ERR_LOW_BANWDTH, &info);
            osal_interrupt_get_reg_and_clear_bit_value(osal_mod_get_delta_ptr(mod_vo_virt, MOD_VO_INT_STA_GAP), MOD_VO_CTRL_DHD0UF_VAL);
            break;
        default:
            break;
        }
    }
}

static int osal_interrupt_mod_vo_process(int irq, void *dev_id)
{
    int report_cnt = 0;
    irq_report_vo_err_info vo_report[OSAL_VO_REPORT_ERR_NUM_MAX] = {0};
    int ret = OSAL_IRQ_HANDLED;

    osal_interrupt_mod_vo_gamble_err(vo_report, &report_cnt);

    if(irq_mod_vo.mod_process != NULL) {
        ret = irq_mod_vo.mod_process(irq, dev_id);
    }

    osal_interrupt_mod_vo_send_err_and_clear(vo_report, report_cnt);
    return ret;
}

static int osal_interrupt_mod_vo_process_thread(int irq, void *dev_id)
{
    if(irq_mod_vo.mod_process_thread != NULL) {
        irq_mod_vo.mod_process_thread(irq, dev_id);
    }
    return OSAL_IRQ_HANDLED;
}

static int osal_interrupt_mod_get_handler(const char *name, osal_irq_handler *handler, osal_irq_handler* thread_fn, osal_irq_handler thread_fn_arg)
{
    if(handler == NULL || thread_fn == NULL) {
        return OSAL_FAILURE;
    }

    if(osal_strcmp(OSAL_INTERRUPT_MOD_VI_NAME, name) == 0) {
        *handler   = osal_interrupt_mod_vi_process;
        *thread_fn = (thread_fn_arg == NULL ? NULL : osal_interrupt_mod_vi_process_thread);
    } else if(osal_strcmp(OSAL_INTERRUPT_MOD_VO_NAME, name) == 0) {
        *handler   = osal_interrupt_mod_vo_process;
        *thread_fn = (thread_fn_arg == NULL ? NULL : osal_interrupt_mod_vo_process_thread);
    }
    return OSAL_SUCCESS;
}

static int osal_interrupt_vi_reg_addr_map(void)
{
    if(mod_vi_virt != NULL) {
        return OSAL_SUCCESS;
    }

    mod_vi_virt = (void *)osal_ioremap(MOD_VI_REG_START_ADDR, MOV_VI_REG_ACCSESS_SIZE);
    if(mod_vi_virt == NULL) {
        osal_log("mod vi virt ioremap fail!\n");
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

static int osal_interrupt_vo_reg_addr_map(void)
{

    if(mod_vo_virt != NULL) {
        return OSAL_SUCCESS;
    }

    mod_vo_virt = (void *)osal_ioremap(MOD_VO_REG_START_ADDR, MOD_VO_REG_ACCSS_SIZE);
    if(mod_vo_virt == NULL) {
        osal_log("mod vo virt ioremap fail!\n");
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;

}

void osal_interrupt_mod_reg_addr_unmap(void)
{
    if(mod_vi_virt != NULL) {
        osal_iounmap(mod_vi_virt, MOV_VI_REG_ACCSESS_SIZE);
        mod_vi_virt = NULL;
    }

    if(mod_vo_virt != NULL) {
        osal_iounmap(mod_vo_virt, MOD_VO_REG_ACCSS_SIZE);
        mod_vo_virt = NULL;
    }
}

int osal_interrupt_report_mod_register(unsigned int irq, osal_irq_handler handler, osal_irq_handler thread_fn,
    const char *name, void *dev, unsigned long flags)
{
    int ret;
    osal_irq_handler mod_handler;
    osal_irq_handler mod_thread_handler;
    irq_report_mod_info *irq_report_mod = NULL;

    if(osal_strcmp(OSAL_INTERRUPT_MOD_VI_NAME, name) == 0) {
        if(osal_interrupt_vi_reg_addr_map() != OSAL_SUCCESS) {
            return OSAL_FAILURE;
        }
        irq_report_mod = &irq_mod_vi;
    } else if(osal_strcmp(OSAL_INTERRUPT_MOD_VO_NAME, name) == 0) {
        if(osal_interrupt_vo_reg_addr_map() != OSAL_SUCCESS) {
            return OSAL_FAILURE;
        }
        irq_report_mod = &irq_mod_vo;
    } else {
        osal_log("mod[%s] unsupport register semu proxy! ret = %#x.\n", name, ret);
        return OSAL_FAILURE;
    }

    ret = osal_interrupt_mod_get_handler(name, &mod_handler, &mod_thread_handler, thread_fn);
    if (ret != OSAL_SUCCESS) {
        osal_log("osal report handler get mod:%s failed.\n", name);
        return OSAL_FAILURE;
    }

    ret = request_threaded_irq(irq, (irq_handler_t)mod_handler, (irq_handler_t)mod_thread_handler, flags, name, dev);
    if (ret != OSAL_SUCCESS) {
        osal_log("request_threaded_irq failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }

    irq_report_mod->irq = irq;
    irq_report_mod->mod_process = handler;
    irq_report_mod->mod_process_thread = thread_fn;
    return OSAL_SUCCESS;
}

int osal_interrupt_report_mod_unregister(unsigned int irq, void *dev)
{

    if(irq == irq_mod_vi.irq) {
        free_irq(irq_mod_vi.irq, dev);
        memset(&irq_mod_vi, 0x00, sizeof(irq_report_mod_info));
    } else if(irq == irq_mod_vo.irq) {
        free_irq(irq_mod_vo.irq, dev);
        memset(&irq_mod_vo, 0x00, sizeof(irq_report_mod_info));
    } else {
        return OSAL_FAILURE;
        }
    return OSAL_SUCCESS;
}
