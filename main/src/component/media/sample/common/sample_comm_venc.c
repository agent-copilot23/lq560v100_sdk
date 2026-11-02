/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdint.h>
#include "sample_comm.h"

ot_s32 sample_comm_venc_start(sample_venc_attr *attr)
{
    ot_s32 ret = OT_SUCCESS;
    if ((ret = ot_eis_venc_init()) != OT_SUCCESS) {
        sample_print("ot_eis_venc_init failed with %#x! ===\n", ret);
        return ret;
    }

    if ((ret = ot_eis_venc_chn_create(&attr->venc_hdl, &attr->venc_attr)) != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_create [%d] failed with %#x! ===\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return ret;
    }

    if ((ret = ot_eis_venc_chn_start(attr->venc_hdl)) != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_start [%d] failed with %#x! ===\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return OT_FAILURE;
    }
    return ret;
}

ot_s32 sample_comm_venc_stop(sample_venc_attr *attr)
{
    ot_s32 ret = OT_SUCCESS;
    ret = ot_eis_venc_chn_stop(attr->venc_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_stop vechn[%d] failed with %#x!\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
    }

    ret = ot_eis_venc_chn_destroy(attr->venc_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_destroy vechn[%d] failed with %#x!\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return OT_FAILURE;
    }

    ret = ot_eis_venc_exit();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_exit failed with %#x! ===\n", ret);
        return ret;
    }
    return ret;
}

