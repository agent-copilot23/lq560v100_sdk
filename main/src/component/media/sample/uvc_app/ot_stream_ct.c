/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */
 
#include <pthread.h>
#include "log.h"
#include "ot_stream.h"
#include "frame_cache.h"
#include "uvc.h"
#include "ot_ctrl.h"
#include "ot_camera.h"
#include "securec.h"

/* ******************************************************
 * Camera Terminal Operation Functions
 * ***************************************************** */
static void ot_stream_it_exposure_auto_mode_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0x03;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->exposure_auto_mode_get) {
            v = get_ot_stream()->mpi_ct_ops->exposure_auto_mode_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_CUR:
            if (get_ot_stream()->exposure_auto_stall) {
                resp->length = -EL2HLT;
                dev->request_error_code.data[0] = 0x04;
                dev->request_error_code.length = 1;
                get_ot_stream()->exposure_auto_stall = OT_FALSE;
            } else {
                resp->data[0] = v;
                resp->length = OT_UVC_RESP_LENGTH_BYTE;
                dev->request_error_code.data[0] = 0x00;
                dev->request_error_code.length = 1;
            }
            break;

        case OT_UVC_GET_MAX:
            resp->data[0] = 0x01;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x09;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x0B;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_exposure_auto_mode_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t v = 0x04;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];
    switch (v) {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x08:
            get_ot_stream()->exposure_auto_stall = OT_FALSE;
            break;
        default:
            printf("Unsupported exposure auto mode value: 0x%x\n", v);
            get_ot_stream()->exposure_auto_stall = OT_TRUE;
            return;
    }
    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->exposure_auto_mode_set) {
        get_ot_stream()->mpi_ct_ops->exposure_auto_mode_set(dev->dev_no, v);
    }
}

static void exposure_time_set_cur(struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_WORD;
    resp->data[0] = 0x0;
}

static void exposure_time_get_cur(struct uvc_request_data *resp, uint32_t v, struct ot_uvc_dev *dev)
{
    resp->length = OT_UVC_RESP_LENGTH_WORD;
    resp->data[0] = (v & 0xff);
    resp->data[1] = ((v >> SHIFT_8BIT)  & 0xff);
    resp->data[2] = ((v >> SHIFT_16BIT) & 0xff); /* [2]:data 16~23bit */
    resp->data[3] = ((v >> SHIFT_24BIT) & 0xff); /* [3]:data 24~31bit */
}

static void exposure_time_get_max(struct uvc_request_data *resp)
{
    uint32_t max_exp_us = 2000000; /* 2000000 us */
    resp->length = OT_UVC_RESP_LENGTH_WORD;
    resp->data[0] = (max_exp_us) & 0xff;                /* [0]:data 0~7bit  */
    resp->data[1] = (max_exp_us >>  SHIFT_8BIT) & 0xff; /* [1]:data 8~15bit  */
    resp->data[2] = (max_exp_us >> SHIFT_16BIT) & 0xff; /* [2]:data 16~23bit */
    resp->data[3] = (max_exp_us >> SHIFT_24BIT) & 0xff; /* [3]:data 24~31bit */
}

static void exposure_time_get_min(struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_WORD;
    resp->data[0] = 0x02; /* [0]:data 0~7bit */
    resp->data[1] = 0x0;  /* [1]:data 8~15bit */
    resp->data[2] = 0x0;  /* [2]:data 16~23bit */
    resp->data[3] = 0x0;  /* [3]:data 24~31bit */
}

static void exposure_time_get_res(struct uvc_request_data *resp)
{
    resp->data[0] = 0x02; /* [0]:data 0~7bit */
    resp->data[1] = 0x0;  /* [1]:data 8~15bit */
    resp->data[2] = 0x0;  /* [2]:data 16~23bit */
    resp->data[3] = 0x0;  /* [3]:data 24~31bit */
    resp->length = OT_UVC_RESP_LENGTH_WORD;
}

static void exposure_time_get_def(struct uvc_request_data *resp)
{
    resp->data[0] = 0x64;
    resp->data[1] = 0x0;  /* [1]:data 8~15bit */
    resp->data[2] = 0x0;  /* [2]:data 16~23bit */
    resp->data[3] = 0x0;  /* [3]:data 24~31bit */
    resp->length = OT_UVC_RESP_LENGTH_WORD;
}

static void exposure_time_get_info(struct uvc_request_data *resp)
{
    resp->data[0] = 0x3;
    resp->length = OT_UVC_RESP_LENGTH_BYTE;
}

static void ot_stream_it_exposure_absolute_time_ctrl(struct ot_uvc_dev *dev, uint8_t req,
    struct uvc_request_data *resp)
{
    uint32_t v = 2000; /* 2000 us */

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->exposure_absolute_time_get) {
            v = get_ot_stream()->mpi_ct_ops->exposure_absolute_time_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            exposure_time_set_cur(resp);
            break;
        case OT_UVC_GET_CUR:
            exposure_time_get_cur(resp, v, dev);
            break;
        case OT_UVC_GET_MAX:
            exposure_time_get_max(resp);
            break;
        case OT_UVC_GET_MIN:
            exposure_time_get_min(resp);
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x04;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_RES:
            exposure_time_get_res(resp);
            break;
        case OT_UVC_GET_DEF:
            exposure_time_get_def(resp);
            break;
        case OT_UVC_GET_INFO:
            exposure_time_get_info(resp);
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.length = 1;
            dev->request_error_code.data[0] = 0x07;
            break;
    }
}

static void ot_stream_it_exposure_absolute_time_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint32_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = 100 * (data->data[0] +               /* [0]: data 0~7bit */
        (data->data[1] << SHIFT_8BIT)  +     /* [1]: data 8~15bit */
        (data->data[2] << SHIFT_16BIT) +     /* [2]: data 16~23bit */
        (data->data[3] << SHIFT_24BIT));     /* [3]: data 24~31bit */

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->exposure_absolute_time_set) {
        get_ot_stream()->mpi_ct_ops->exposure_absolute_time_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_scanning_mode_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->scanning_mode_get) {
            v = get_ot_stream()->mpi_ct_ops->scanning_mode_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->data[0] = v;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_exposure_auto_priority_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0x05;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->exposure_auto_priority_get) {
            v = get_ot_stream()->mpi_ct_ops->exposure_auto_priority_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->data[0] = v;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_exposure_relative_time_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->exposure_relative_time_get) {
            v = get_ot_stream()->mpi_ct_ops->exposure_relative_time_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->data[0] = v;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_focus_absolute_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->focus_absolute_get) {
            v = get_ot_stream()->mpi_ct_ops->focus_absolute_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->data[1] = 0x1;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
        //2026.10.22 修改曝光问题
        //    resp->data[0] = v;
        //    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = v;
            resp->data[1] = v & 0xff;
            resp->length  = (v>>8) & 0xff;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->data[1] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MAX:
            //2026.10.22 修改曝光问题
            //resp->data[0] = 0xf;
            //resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0xff;
            resp->data[1] = 0xff;   /*65535 对应 exp_time/10 的16位满程量*/
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x4;
            //2026.10.22 修改曝光问题
            resp->data[1] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x1;
            //2026.10.22 修改曝光问题
            resp->data[1] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_focus_relative_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->focus_relative_get) {
            v = get_ot_stream()->mpi_ct_ops->focus_relative_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->data[0] = v;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x01;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x04;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_focus_sample_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->focus_sample_get) {
            v = get_ot_stream()->mpi_ct_ops->focus_sample_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->data[0] = v;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x4;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_focus_auto_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->focus_auto_get) {
            v = get_ot_stream()->mpi_ct_ops->focus_auto_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->data[0] = v;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x4;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_iris_absolute_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->iris_absolute_get) {
            v = get_ot_stream()->mpi_ct_ops->iris_absolute_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            (ot_void)memcpy_s(resp->data, sizeof(resp->data), &v, sizeof(uint16_t));
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x1;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x4;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_iris_relative_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->iris_relative_get) {
            v = get_ot_stream()->mpi_ct_ops->iris_relative_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->data[0] = v;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_zoom_absolute_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->zoom_absolute_get) {
            v = get_ot_stream()->mpi_ct_ops->zoom_absolute_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            (ot_void)memcpy_s(resp->data, sizeof(resp->data), &v, sizeof(uint16_t));
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x1;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x4;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_zoom_relative_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint32_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->zoom_relative_get) {
            v = get_ot_stream()->mpi_ct_ops->zoom_relative_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_CUR:
            (ot_void)memcpy_s(resp->data, sizeof(resp->data), &v, sizeof(uint32_t));
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x1;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x04;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_pantilt_absolute_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint64_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->pantilt_absolute_get) {
            v = get_ot_stream()->mpi_ct_ops->pantilt_absolute_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_8;
            break;
        case OT_UVC_GET_CUR:
            (ot_void)memcpy_s(resp->data, sizeof(resp->data), &v, sizeof(uint64_t));
            resp->length = OT_UVC_RESP_LENGTH_8;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_8;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_8;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x1;
            resp->length = OT_UVC_RESP_LENGTH_8;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x04;
            resp->length = OT_UVC_RESP_LENGTH_8;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_8;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_pantilt_relative_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint32_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->pantilt_relative_get) {
            v = get_ot_stream()->mpi_ct_ops->pantilt_relative_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_CUR:
            (ot_void)memcpy_s(resp->data, sizeof(resp->data), &v, sizeof(uint32_t));
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x1;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x04;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_roll_absolute_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->roll_absolute_get) {
            v = get_ot_stream()->mpi_ct_ops->roll_absolute_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            (ot_void)memcpy_s(resp->data, sizeof(resp->data), &v, sizeof(uint16_t));
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x1;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x04;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_roll_relative_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->roll_relative_get) {
            v = get_ot_stream()->mpi_ct_ops->roll_relative_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            (ot_void)memcpy_s(resp->data, sizeof(resp->data), &v, sizeof(uint16_t));
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x1;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x04;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_privacy_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0x5;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->ct_privacy_get) {
            v = get_ot_stream()->mpi_ct_ops->ct_privacy_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->data[0] = v;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x3;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_window_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    ct_window_params v;
    errno_t ret;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->ct_window_get) {
            get_ot_stream()->mpi_ct_ops->ct_window_get(dev->dev_no, &v);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_12;
            break;
        case OT_UVC_GET_CUR:
            ret = memcpy_s(resp->data, sizeof(resp->data), &v, sizeof(ct_window_params));
            if (ret != EOK) {
                printf("GET_CUR memcpy_s fail %#x\n", ret);
            }
            resp->length = OT_UVC_RESP_LENGTH_12;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_12;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_12;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x4;
            resp->length = OT_UVC_RESP_LENGTH_12;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_ct_region_of_interest_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    region_of_interest_params v;
    errno_t ret;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->region_of_interest_get) {
            get_ot_stream()->mpi_ct_ops->region_of_interest_get(dev->dev_no, &v);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
    switch (req) {
        case OT_UVC_SET_CUR:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_10;
            break;
        case OT_UVC_GET_CUR:
            ret = memcpy_s(resp->data, sizeof(resp->data), &v, sizeof(region_of_interest_params));
            if (ret != EOK) {
                printf("GET_CUR memcpy_s fail %#x\n", ret);
            }
            resp->length = OT_UVC_RESP_LENGTH_10;
            break;
        case OT_UVC_GET_MIN:
            resp->data[0] = 0x0;
            resp->length = OT_UVC_RESP_LENGTH_10;
            break;
        case OT_UVC_GET_MAX:
            resp->data[0] = 0xf;
            resp->length = OT_UVC_RESP_LENGTH_10;
            break;
        case OT_UVC_GET_DEF:
            resp->data[0] = 0x4;
            resp->length = OT_UVC_RESP_LENGTH_10;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

void do_ct_ctrl_defalut(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    ot_stream_event_default_control(req, resp);
    resp->length = -EL2HLT;
    dev->request_error_code.data[0] = 0x06;
    dev->request_error_code.length = 1;
    return;
}

typedef void (*do_ct_ctrl)(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp);
static do_ct_ctrl g_ct_ctrl_list[] = {
    [OT_UVC_CT_CONTROL_UNDEFINED] = do_ct_ctrl_defalut,
    [OT_UVC_CT_SCANNING_MODE_CONTROL] = ot_stream_ct_scanning_mode_ctrl,
    [OT_UVC_CT_AE_MODE_CONTROL] = ot_stream_it_exposure_auto_mode_ctrl,
    [OT_UVC_CT_AE_PRIORITY_CONTROL] = ot_stream_ct_exposure_auto_priority_ctrl,
    [OT_UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL] = ot_stream_it_exposure_absolute_time_ctrl,
    [OT_UVC_CT_EXPOSURE_TIME_RELATIVE_CONTROL] = ot_stream_ct_exposure_relative_time_ctrl,
    [OT_UVC_CT_FOCUS_ABSOLUTE_CONTROL] = ot_stream_ct_focus_absolute_ctrl,
    [OT_UVC_CT_FOCUS_RELATIVE_CONTROL] = ot_stream_ct_focus_relative_ctrl,
    [OT_UVC_CT_FOCUS_AUTO_CONTROL] = ot_stream_ct_focus_auto_ctrl,
    [OT_UVC_CT_IRIS_ABSOLUTE_CONTROL] = ot_stream_ct_iris_absolute_ctrl,
    [OT_UVC_CT_IRIS_RELATIVE_CONTROL] = ot_stream_ct_iris_relative_ctrl,
    [OT_UVC_CT_ZOOM_ABSOLUTE_CONTROL] = ot_stream_ct_zoom_absolute_ctrl,
    [OT_UVC_CT_ZOOM_RELATIVE_CONTROL] = ot_stream_ct_zoom_relative_ctrl,
    [OT_UVC_CT_PANTILT_ABSOLUTE_CONTROL] = ot_stream_ct_pantilt_absolute_ctrl,
    [OT_UVC_CT_PANTILT_RELATIVE_CONTROL] = ot_stream_ct_pantilt_relative_ctrl,
    [OT_UVC_CT_ROLL_ABSOLUTE_CONTROL] = ot_stream_ct_roll_absolute_ctrl,
    [OT_UVC_CT_ROLL_RELATIVE_CONTROL] = ot_stream_ct_roll_relative_ctrl,
    [OT_UVC_CT_PRIVACY_CONTROL] = ot_stream_ct_privacy_ctrl,
    [OT_UVC_CT_FOCUS_SIMPLE_CONTROL] = ot_stream_ct_focus_sample_ctrl,
    [OT_UVC_CT_WINDOW_CONTROL] = ot_stream_ct_window_ctrl,
    [OT_UVC_CT_REGION_OF_INTEREST_CONTROL] = ot_stream_ct_region_of_interest_ctrl,
    NULL,
};

void ot_stream_event_ct_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp)
{
    if (dev == NULL || resp == NULL) {
        err("dev or resp is NULL\n");
        return;
    }

    if (unit_id != OT_UVC_CAMERA_TERMINAL_ID) {
        return;
    }

    if (get_ot_stream()->exposure_auto_stall) {
        printf("Exposure Auto Control is Stalled\n");
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x04;
        dev->request_error_code.length = 1;
        get_ot_stream()->exposure_auto_stall = OT_FALSE;
        return;
    }

    if (cs <= OT_UVC_CT_CONTROL_UNDEFINED || cs > OT_UVC_CT_REGION_OF_INTEREST_CONTROL) {
        return g_ct_ctrl_list[OT_UVC_CT_CONTROL_UNDEFINED](dev, req, resp);
    }

    g_ct_ctrl_list[cs](dev, req, resp);
}

static void ot_stream_ct_scanning_mode_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->scanning_mode_set) {
        get_ot_stream()->mpi_ct_ops->scanning_mode_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_exposure_auto_priority_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->exposure_auto_priority_set) {
        get_ot_stream()->mpi_ct_ops->exposure_auto_priority_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_exposure_relative_time_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->exposure_relative_time_set) {
        get_ot_stream()->mpi_ct_ops->exposure_relative_time_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_focus_absolute_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint16_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->focus_absolute_set) {
        get_ot_stream()->mpi_ct_ops->focus_absolute_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_focus_relative_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t focus_relative = 0;
    uint8_t speed;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    focus_relative = data->data[0];
    speed = data->data[1];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->focus_relative_set) {
        get_ot_stream()->mpi_ct_ops->focus_relative_set(dev->dev_no, focus_relative, speed);
    }
}

static void ot_stream_ct_focus_sample_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->focus_sample_set) {
        get_ot_stream()->mpi_ct_ops->focus_sample_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_focus_auto_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->focus_auto_set) {
        get_ot_stream()->mpi_ct_ops->focus_auto_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_iris_absolute_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->iris_absolute_set) {
        get_ot_stream()->mpi_ct_ops->iris_absolute_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_iris_relative_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->iris_relative_set) {
        get_ot_stream()->mpi_ct_ops->iris_relative_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_zoom_absolute_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->zoom_absolute_set) {
        get_ot_stream()->mpi_ct_ops->zoom_absolute_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_zoom_relative_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint16_t zoom;
    uint16_t digital_zoom;
    uint16_t speed;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    zoom = data->data[0];
    digital_zoom = data->data[1];
    speed = data->data[0x2];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->zoom_relative_set) {
        get_ot_stream()->mpi_ct_ops->zoom_relative_set(dev->dev_no, zoom, digital_zoom, speed);
    }
}

static void ot_stream_ct_pantilt_absolute_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint32_t dw_pan_pbsolute;
    uint32_t dw_tilt_absolute;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    dw_pan_pbsolute = data->data[0] + (data->data[1] << SHIFT_8BIT) + (data->data[0x2] << SHIFT_16BIT) +
        (data->data[0x3] << SHIFT_24BIT);
    dw_tilt_absolute = data->data[0x4] + (data->data[0x5] << SHIFT_8BIT) + (data->data[0x6] << SHIFT_16BIT) +
        (data->data[0x7] << SHIFT_24BIT);

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->pantilt_absolute_set) {
        get_ot_stream()->mpi_ct_ops->pantilt_absolute_set(dev->dev_no, dw_pan_pbsolute, dw_tilt_absolute);
    }
}

static void ot_stream_ct_pantilt_relative_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t pan_relative;
    uint8_t pan_speed;
    uint8_t tilt_relative;
    uint8_t tilt_speed;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    pan_relative = data->data[0];
    pan_speed = data->data[1];
    tilt_relative = data->data[0x2];
    tilt_speed = data->data[0x3];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->pantilt_relative_set) {
        get_ot_stream()->mpi_ct_ops->pantilt_relative_set(dev->dev_no, pan_relative, pan_speed, tilt_relative,
            tilt_speed);
    }
}

static void ot_stream_ct_roll_absolute_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->roll_absolute_set) {
        get_ot_stream()->mpi_ct_ops->roll_absolute_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_roll_relative_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t roll_relative;
    uint8_t speed;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    roll_relative = data->data[0];
    speed = data->data[1];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->roll_relative_set) {
        get_ot_stream()->mpi_ct_ops->roll_relative_set(dev->dev_no, roll_relative, speed);
    }
}

static void ot_stream_ct_privacy_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->ct_privacy_set) {
        get_ot_stream()->mpi_ct_ops->ct_privacy_set(dev->dev_no, v);
    }
}

static void ot_stream_ct_window_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    ct_window_params v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v.window_top = data->data[0] + (data->data[1] << SHIFT_8BIT);
    v.window_left = data->data[0x2] + (data->data[0x3] << SHIFT_8BIT);
    v.window_bottom = data->data[0x4] + (data->data[0x5] << SHIFT_8BIT);
    v.window_right = data->data[0x6] + (data->data[0x7] << SHIFT_8BIT);
    v.num_steps = data->data[0x8] + (data->data[0x9] << SHIFT_8BIT);
    v.num_step_units = data->data[0xA] + (data->data[0xB] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->ct_window_set) {
        get_ot_stream()->mpi_ct_ops->ct_window_set(dev->dev_no, &v);
    }
}

static void ot_stream_ct_region_of_interest_set(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    region_of_interest_params v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v.roi_top = data->data[0] + (data->data[1] << SHIFT_8BIT);
    v.roi_left = data->data[0x2] + (data->data[0x3] << SHIFT_8BIT);
    v.roi_bottom = data->data[0x4] + (data->data[0x5] << SHIFT_8BIT);
    v.roi_right = data->data[0x6] + (data->data[0x7] << SHIFT_8BIT);
    v.auto_controls = data->data[0x8] + (data->data[0x9] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_ct_ops && get_ot_stream()->mpi_ct_ops->region_of_interest_set) {
        get_ot_stream()->mpi_ct_ops->region_of_interest_set(dev->dev_no, &v);
    }
}

static void ct_data_do_nothing(struct ot_uvc_dev *dev, const struct uvc_request_data *data)
{
    return;
}

typedef void (*do_ct_data)(struct ot_uvc_dev *dev, const struct uvc_request_data *data);
static do_ct_data g_ct_data_list[] = {
    [OT_UVC_CT_CONTROL_UNDEFINED] = ct_data_do_nothing,
    [OT_UVC_CT_SCANNING_MODE_CONTROL] = ot_stream_ct_scanning_mode_set,
    [OT_UVC_CT_AE_MODE_CONTROL] = ot_stream_ct_exposure_auto_mode_set,
    [OT_UVC_CT_AE_PRIORITY_CONTROL] = ot_stream_ct_exposure_auto_priority_set,
    [OT_UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL] = ot_stream_it_exposure_absolute_time_set,
    [OT_UVC_CT_EXPOSURE_TIME_RELATIVE_CONTROL] = ot_stream_ct_exposure_relative_time_set,
    [OT_UVC_CT_FOCUS_ABSOLUTE_CONTROL] = ot_stream_ct_focus_absolute_set,
    [OT_UVC_CT_FOCUS_RELATIVE_CONTROL] = ot_stream_ct_focus_relative_set,
    [OT_UVC_CT_FOCUS_AUTO_CONTROL] = ot_stream_ct_focus_auto_set,
    [OT_UVC_CT_IRIS_ABSOLUTE_CONTROL] = ot_stream_ct_iris_absolute_set,
    [OT_UVC_CT_IRIS_RELATIVE_CONTROL] = ot_stream_ct_iris_relative_set,
    [OT_UVC_CT_ZOOM_ABSOLUTE_CONTROL] = ot_stream_ct_zoom_absolute_set,
    [OT_UVC_CT_ZOOM_RELATIVE_CONTROL] = ot_stream_ct_zoom_relative_set,
    [OT_UVC_CT_PANTILT_ABSOLUTE_CONTROL] = ot_stream_ct_pantilt_absolute_set,
    [OT_UVC_CT_PANTILT_RELATIVE_CONTROL] = ot_stream_ct_pantilt_relative_set,
    [OT_UVC_CT_ROLL_ABSOLUTE_CONTROL] = ot_stream_ct_roll_absolute_set,
    [OT_UVC_CT_ROLL_RELATIVE_CONTROL] = ot_stream_ct_roll_relative_set,
    [OT_UVC_CT_PRIVACY_CONTROL] = ot_stream_ct_privacy_set,
    [OT_UVC_CT_FOCUS_SIMPLE_CONTROL] = ot_stream_ct_focus_sample_set,
    [OT_UVC_CT_WINDOW_CONTROL] = ot_stream_ct_window_set,
    [OT_UVC_CT_REGION_OF_INTEREST_CONTROL] = ot_stream_ct_region_of_interest_set,
    NULL,
};

void ot_stream_event_ct_data(struct ot_uvc_dev *dev, int unit_id, int control, struct uvc_request_data *data)
{
    if (dev == NULL || data == NULL) {
        err("dev or data is NULL\n");
        return;
    }
    if (unit_id != OT_UVC_CAMERA_TERMINAL_ID) {
        return;
    }

    if (control < OT_UVC_CT_CONTROL_UNDEFINED || control > OT_UVC_CT_REGION_OF_INTEREST_CONTROL) {
        err("invalid event type!\n");
        return;
    }

    g_ct_data_list[control](dev, data);
}


/* Camera Terminal Operation Functions End */
