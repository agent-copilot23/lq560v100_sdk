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
 * Processing Unit Operation Functions
 * ***************************************************** */
static void ot_stream_pu_backlight_compensation_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 50;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->backlight_compensation_get) {
            v = get_ot_stream()->mpi_pu_ops->backlight_compensation_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xFF);
            resp->data[1] = (v >> SHIFT_8BIT) & 0xFF;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x02;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x02;
             resp->data[1] = 0x00;
             break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_backlight_compensation_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }
    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->backlight_compensation_set) {
        get_ot_stream()->mpi_pu_ops->backlight_compensation_set(dev->dev_no, v);
    }
}

static void brightness_set_cur(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

static void brightness_get_cur(struct ot_uvc_dev *dev, struct uvc_request_data *resp, uint8_t v)
{
    if (get_ot_stream()->brightness_stall) {
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x04;
        dev->request_error_code.length = 1;
        get_ot_stream()->brightness_stall = OT_FALSE;
    } else {
        resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
        resp->data[0] = v;
        resp->data[1] = 0;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
    }
}

static void brightness_get_min(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    resp->data[0] = 0x00;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

static void brightness_get_max(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    resp->data[0] = 0x64;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

static void brightness_get_def(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    resp->data[0] = 0x32;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

static void brightness_get_res(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->data[0] = 0x01;
    resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

static void brightness_get_info(struct ot_uvc_dev *dev, struct uvc_request_data *resp)
{
    resp->data[0] = 0x03;
    resp->length = OT_UVC_RESP_LENGTH_BYTE;
    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;
}

static void ot_stream_pu_brightness_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 50;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->brightness_get) {
            v = get_ot_stream()->mpi_pu_ops->brightness_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            brightness_set_cur(dev, resp);
            break;
        case OT_UVC_GET_CUR:
            brightness_get_cur(dev, resp, v);
            break;
        case OT_UVC_GET_MIN:
            brightness_get_min(dev, resp);
            break;
        case OT_UVC_GET_MAX:
            brightness_get_max(dev, resp);
            break;
        case OT_UVC_GET_DEF:
            brightness_get_def(dev, resp);
            break;
        case OT_UVC_GET_RES:
            brightness_get_res(dev, resp);
            break;
        case OT_UVC_GET_INFO:
            brightness_get_info(dev, resp);
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_brightness_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);
    if (v > 0x64) {
        get_ot_stream()->brightness_stall = OT_TRUE;
    }

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->brightness_set) {
        get_ot_stream()->mpi_pu_ops->brightness_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_contrast_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 50; /* 50 is in range of 0~100  */

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->contrast_get) {
            v = get_ot_stream()->mpi_pu_ops->contrast_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x64;
            resp->data[1] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x32;
            resp->data[1] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x02;
             resp->data[1] = 0x00;
             break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x03;
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

static void ot_stream_pu_contrast_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->contrast_set) {
        get_ot_stream()->mpi_pu_ops->contrast_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_gain_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->gain_get) {
            v = get_ot_stream()->mpi_pu_ops->gain_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length  = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x18;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x03;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x02;
             resp->data[1] = 0x00;
             break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length  = 1;
            break;
    }
}

static void ot_stream_pu_gain_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->gain_set) {
        get_ot_stream()->mpi_pu_ops->gain_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_hue_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 50; /* 50 is in range of 0~100  */

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->hue_get) {
            v = get_ot_stream()->mpi_pu_ops->hue_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_DEF:
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x64;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x02;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_hue_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->hue_set) {
        get_ot_stream()->mpi_pu_ops->hue_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_power_line_frequency_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->power_line_frequency_get) {
            v = get_ot_stream()->mpi_pu_ops->power_line_frequency_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = v;
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x1;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x2;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x1;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x01;
             resp->data[1] = 0x00;
             break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x01;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_power_line_frequency_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->power_line_frequency_set) {
        get_ot_stream()->mpi_pu_ops->power_line_frequency_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_saturation_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 50; /* 50 is in range of 0~100  */

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->saturation_get) {
            v = get_ot_stream()->mpi_pu_ops->saturation_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x64;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x32;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x02;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_saturation_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->saturation_set) {
        get_ot_stream()->mpi_pu_ops->saturation_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_sharpness_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->sharpness_get) {
            v = get_ot_stream()->mpi_pu_ops->sharpness_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x01;
             resp->data[1] = 0x00;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_sharpness_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->sharpness_set) {
        get_ot_stream()->mpi_pu_ops->sharpness_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_resp_set(struct uvc_request_data *resp, uint32_t length, uint8_t data0, uint8_t data1)
{
    resp->length = length;
    resp->data[0] = data0;
    resp->data[1] = data1;
}

#define GAMMA_MAX 500
static void ot_stream_pu_gamma_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 100;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->gamma_get) {
            v = get_ot_stream()->mpi_pu_ops->gamma_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            if (get_ot_stream()->gamma_stall) {
                resp->length = -EL2HLT;
                dev->request_error_code.data[0] = 0x04;
                dev->request_error_code.length = 1;
                get_ot_stream()->gamma_stall = OT_FALSE;
            } else {
                ot_stream_pu_resp_set(resp, OT_UVC_RESP_LENGTH_HALF_WORD, (v & 0xff), ((v >> SHIFT_8BIT) & 0xff));
            }
            break;
        case OT_UVC_GET_MIN:
            ot_stream_pu_resp_set(resp, OT_UVC_RESP_LENGTH_HALF_WORD, 1 & 0xff, ((1 >> SHIFT_8BIT) & 0xff));
            break;
        case OT_UVC_GET_MAX:
            ot_stream_pu_resp_set(resp, OT_UVC_RESP_LENGTH_HALF_WORD,
                GAMMA_MAX & 0xff, ((GAMMA_MAX >> SHIFT_8BIT) & 0xff));
            break;
        case OT_UVC_GET_RES:
            ot_stream_pu_resp_set(resp, OT_UVC_RESP_LENGTH_HALF_WORD, 0x01, 0x0);
            break;
        case OT_UVC_GET_INFO:
            ot_stream_pu_resp_set(resp, OT_UVC_RESP_LENGTH_BYTE, 0x0, 0x0);
            break;
        case OT_UVC_GET_DEF:
            ot_stream_pu_resp_set(resp, OT_UVC_RESP_LENGTH_HALF_WORD, 0x01, 0x0);
            break;
        case OT_UVC_GET_LEN:
            ot_stream_pu_resp_set(resp, OT_UVC_RESP_LENGTH_HALF_WORD, 0x02, 0x0);
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_gamma_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);
    if (v < 1 || v > GAMMA_MAX) {
        get_ot_stream()->gamma_stall = OT_TRUE;
    }

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->gamma_set) {
        get_ot_stream()->mpi_pu_ops->gamma_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_white_balance_temperature_auto_ctrl(struct ot_uvc_dev *dev, uint8_t req,
    struct uvc_request_data *resp)
{
    uint8_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_temperature_auto_get) {
            v = get_ot_stream()->mpi_pu_ops->white_balance_temperature_auto_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = v;
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_DEF:
        case OT_UVC_GET_RES:
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x0;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x1;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x01;
             resp->data[1] = 0x00;
             break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_white_balance_temperature_auto_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_temperature_auto_set) {
        get_ot_stream()->mpi_pu_ops->white_balance_temperature_auto_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_white_balance_temperature_ctrl(struct ot_uvc_dev *dev, uint8_t req,
    struct uvc_request_data *resp)
{
    uint16_t v = 15000; /* white balance temperature 15000K */

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_temperature_get) {
            v = get_ot_stream()->mpi_pu_ops->white_balance_temperature_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            dev->request_error_code.data[0] = 0x06;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_DEF:
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 1500 & 0xff; /* 1500:min wbt,low 256 bit */
            resp->data[1] = 1500 / 256;  /* 1500:min wbt,high 256 bit */
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 15000 & 0xff; /* 15000:max wbt,low 256 bit */
            resp->data[1] = 15000 / 256;  /* 15000:max wbt,high 256 bit */
            break;
        case OT_UVC_GET_LEN:
            resp->data[0] = 0x02;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_RES:
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_INFO:
            resp->data[0] = 0x03;
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_white_balance_temperature_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_temperature_set) {
        get_ot_stream()->mpi_pu_ops->white_balance_temperature_set(dev->dev_no, v);
    }
}


static void ot_stream_pu_white_balance_component_ctrl(struct ot_uvc_dev *dev, uint8_t req,
    struct uvc_request_data *resp)
{
    uint32_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_component_get) {
            v = get_ot_stream()->mpi_pu_ops->white_balance_component_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            resp->data[0x0] = (v & 0xff);
            resp->data[0x1] = ((v >> SHIFT_8BIT) & 0xff);
            resp->data[0x2] = ((v >> SHIFT_16BIT) & 0xff);
            resp->data[0x3] = ((v >> SHIFT_24BIT) & 0xff);
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            (ot_void)memset_s(resp->data, sizeof(resp->data), 0, OT_UVC_RESP_LENGTH_WORD);
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            resp->data[0] = 0xff;
            resp->data[1] = 0x7f;
            resp->data[2] = 0xff;
            resp->data[3] = 0x7f;
            break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            (ot_void)memset_s(resp->data, sizeof(resp->data), 0, OT_UVC_RESP_LENGTH_WORD);
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x01;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             (ot_void)memset_s(resp->data,sizeof(resp->data),0,OT_UVC_RESP_LENGTH_WORD)
             break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_white_balance_component_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint32_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT) + (data->data[0x2] << SHIFT_16BIT) +
        (data->data[0x3] << SHIFT_24BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_component_set) {
        get_ot_stream()->mpi_pu_ops->white_balance_component_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_white_balance_component_auto_ctrl(struct ot_uvc_dev *dev, uint8_t req,
    struct uvc_request_data *resp)
{
    uint8_t v = 1;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_component_auto_get) {
            v = get_ot_stream()->mpi_pu_ops->white_balance_component_auto_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            if (get_ot_stream()->white_balance_component_auto_stall) {
                resp->length = -EL2HLT;
                dev->request_error_code.data[0] = 0x02;
                dev->request_error_code.length = 1;
                get_ot_stream()->white_balance_component_auto_stall = OT_FALSE;
            } else {
                resp->length = OT_UVC_RESP_LENGTH_BYTE;
                resp->data[0] = v;
                dev->request_error_code.data[0] = 0x00;
                dev->request_error_code.length = 1;
            }
            break;
        
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x01;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;

        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;

        case OT_UVC_GET_MIN:
        case OT_UVC_GET_RES:
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x01;
             resp->data[1] = 0x00;
             break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_white_balance_component_auto_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (v > 0x1) {
        get_ot_stream()->white_balance_component_auto_stall = OT_TRUE;
    }

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->white_balance_component_auto_set) {
        get_ot_stream()->mpi_pu_ops->white_balance_component_auto_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_digital_multiplier_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint16_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->digital_multiplier_get) {
            v = get_ot_stream()->mpi_pu_ops->digital_multiplier_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x02;
             resp->data[1] = 0x00;
             break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_digital_multiplier_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->digital_multiplier_set) {
        get_ot_stream()->mpi_pu_ops->digital_multiplier_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_digital_multiplier_limit_ctrl(struct ot_uvc_dev *dev, uint8_t req,
    struct uvc_request_data *resp)
{
    uint16_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->digital_multiplier_limit_get) {
            v = get_ot_stream()->mpi_pu_ops->digital_multiplier_limit_get(dev->dev_no);
        }
    }

    dev->request_error_code.data[0] = 0x00;
    dev->request_error_code.length = 1;

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            break;
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            resp->data[1] = ((v >> SHIFT_8BIT) & 0xff);
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x01;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = 0x00;
            resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x02;
             resp->data[1] = 0x00;
             break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_digital_multiplier_limit_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint16_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0] + (data->data[1] << SHIFT_8BIT);

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->digital_multiplier_limit_set) {
        get_ot_stream()->mpi_pu_ops->digital_multiplier_limit_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_hue_auto_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 1;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->hue_auto_get) {
            v = get_ot_stream()->mpi_pu_ops->hue_auto_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            if (get_ot_stream()->hue_auto_stall) {
                resp->length = -EL2HLT;
                dev->request_error_code.data[0] = 0x02;
                dev->request_error_code.length = 1;
                get_ot_stream()->hue_auto_stall = OT_FALSE;
            } else {
                resp->length = OT_UVC_RESP_LENGTH_BYTE;
                resp->data[0] = v;
                dev->request_error_code.data[0] = 0x00;
                dev->request_error_code.length = 1;
            }
            break;
        case OT_UVC_GET_MIN:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x80;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_LEN:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x01;
             resp->data[1] = 0x00;
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_RES:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x01;
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

static void ot_stream_pu_hue_auto_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (v > 0x1) {
        get_ot_stream()->hue_auto_stall = OT_TRUE;
    }

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->hue_auto_set) {
        get_ot_stream()->mpi_pu_ops->hue_auto_set(dev->dev_no, v);
    }
}

static void ot_stream_pu_analog_video_standard_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->analog_video_standard_get) {
            v = get_ot_stream()->mpi_pu_ops->analog_video_standard_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
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

static void ot_stream_pu_analog_lock_status_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 0;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->analog_lock_status_get) {
            v = get_ot_stream()->mpi_pu_ops->analog_lock_status_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_GET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
            resp->data[0] = (v & 0xff);
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
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

static void ot_stream_pu_contrast_auto_ctrl(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    uint8_t v = 1;

    if (get_ot_stream()->streaming[dev->dev_no] == 1) {
        if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->contrast_auto_get) {
            v = get_ot_stream()->mpi_pu_ops->contrast_auto_get(dev->dev_no);
        }
    }

    switch (req) {
        case OT_UVC_SET_CUR:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            break;
        case OT_UVC_GET_CUR:
            if (get_ot_stream()->contrast_auto_stall) {
                resp->length = -EL2HLT;
                dev->request_error_code.data[0] = 0x02;
                dev->request_error_code.length = 1;
                get_ot_stream()->contrast_auto_stall = OT_FALSE;
            } else {
                resp->length = OT_UVC_RESP_LENGTH_BYTE;
                resp->data[0] = v;
                dev->request_error_code.data[0] = 0x00;
                dev->request_error_code.length = 1;
            }
            break;
        case OT_UVC_GET_INFO:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MAX:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x01;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_MIN:
        case OT_UVC_GET_RES:
        case OT_UVC_GET_DEF:
            resp->length = OT_UVC_RESP_LENGTH_BYTE;
            resp->data[0] = 0x00;
            dev->request_error_code.data[0] = 0x00;
            dev->request_error_code.length = 1;
            break;
        case OT_UVC_GET_LEN:
             resp->length = OT_UVC_RESP_LENGTH_HALF_WORD;
             resp->data[0] = 0x01;
             resp->data[1] = 0x00;
             break;
        default:
            resp->length = -EL2HLT;
            dev->request_error_code.data[0] = 0x07;
            dev->request_error_code.length = 1;
            break;
    }
}

static void ot_stream_pu_contrast_auto_set(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    uint8_t v;

    if (get_ot_stream()->streaming[dev->dev_no] != 1) {
        return;
    }

    v = data->data[0];

    if (v > 1) {
        get_ot_stream()->contrast_auto_stall = OT_TRUE;
    }

    if (get_ot_stream()->mpi_pu_ops && get_ot_stream()->mpi_pu_ops->contrast_auto_set) {
        get_ot_stream()->mpi_pu_ops->contrast_auto_set(dev->dev_no, v);
    }
}

static void do_pu_ctrl_default(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp)
{
    ot_stream_event_default_control(req, resp);
    resp->length = -EL2HLT;
    dev->request_error_code.data[0] = 0x06;
    dev->request_error_code.length = 1;
}

typedef void (*do_pu_ctrl)(struct ot_uvc_dev *dev, uint8_t req, struct uvc_request_data *resp);
static do_pu_ctrl g_pu_ctrl_list[] = {
    [OT_UVC_PU_CONTROL_UNDEFINED] = do_pu_ctrl_default,
    [OT_UVC_PU_BACKLIGHT_COMPENSATION_CONTROL] = ot_stream_pu_backlight_compensation_ctrl,
    [OT_UVC_PU_BRIGHTNESS_CONTROL] = ot_stream_pu_brightness_ctrl,
    [OT_UVC_PU_CONTRAST_CONTROL] = ot_stream_pu_contrast_ctrl,
    [OT_UVC_PU_GAIN_CONTROL] = ot_stream_pu_gain_ctrl,
    [OT_UVC_PU_POWER_LINE_FREQUENCY_CONTROL] = ot_stream_pu_power_line_frequency_ctrl,
    [OT_UVC_PU_HUE_CONTROL] = ot_stream_pu_hue_ctrl,
    [OT_UVC_PU_SATURATION_CONTROL] = ot_stream_pu_saturation_ctrl,
    [OT_UVC_PU_SHARPNESS_CONTROL] = ot_stream_pu_sharpness_ctrl,
    [OT_UVC_PU_GAMMA_CONTROL] = ot_stream_pu_gamma_ctrl,
    [OT_UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL] = ot_stream_pu_white_balance_temperature_ctrl,
    [OT_UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL] = ot_stream_pu_white_balance_temperature_auto_ctrl,
    [OT_UVC_PU_WHITE_BALANCE_COMPONENT_CONTROL] = ot_stream_pu_white_balance_component_ctrl,
    [OT_UVC_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL] = ot_stream_pu_white_balance_component_auto_ctrl,
    [OT_UVC_PU_DIGITAL_MULTIPLIER_CONTROL] = ot_stream_pu_digital_multiplier_ctrl,
    [OT_UVC_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL] = ot_stream_pu_digital_multiplier_limit_ctrl,
    [OT_UVC_PU_HUE_AUTO_CONTROL] = ot_stream_pu_hue_auto_ctrl,
    [OT_UVC_PU_ANALOG_VIDEO_STANDARD_CONTROL] = ot_stream_pu_analog_video_standard_ctrl,
    [OT_UVC_PU_ANALOG_LOCK_STATUS_CONTROL] = ot_stream_pu_analog_lock_status_ctrl,
    [OT_UVC_PU_CONTRAST_AUTO_CONTROL] = ot_stream_pu_contrast_auto_ctrl,
    NULL,
};

void ot_stream_event_pu_control(struct ot_uvc_dev *dev, uint8_t req, uint8_t unit_id, uint8_t cs,
    struct uvc_request_data *resp)
{
    if (dev == NULL || resp == NULL) {
        err("dev or resp is NULL\n");
        return;
    }
    if (unit_id != OT_UVC_PROCESSING_UNIT_ID) {
        return;
    }

    if (cs <= OT_UVC_PU_CONTROL_UNDEFINED || cs > OT_UVC_PU_CONTRAST_AUTO_CONTROL) {
        g_pu_ctrl_list[OT_UVC_PU_CONTROL_UNDEFINED](dev, req, resp);
        return;
    }

    g_pu_ctrl_list[cs](dev, req, resp);
}

static void pu_data_do_nothing(struct ot_uvc_dev *dev, struct uvc_request_data *data)
{
    return;
}

typedef void (*do_pu_data)(struct ot_uvc_dev *dev, struct uvc_request_data *data);
static do_pu_data g_pu_data_list[] = {
    [OT_UVC_PU_CONTROL_UNDEFINED] = pu_data_do_nothing,
    [OT_UVC_PU_BACKLIGHT_COMPENSATION_CONTROL] = ot_stream_pu_backlight_compensation_set,
    [OT_UVC_PU_BRIGHTNESS_CONTROL] = ot_stream_pu_brightness_set,
    [OT_UVC_PU_CONTRAST_CONTROL] = ot_stream_pu_contrast_set,
    [OT_UVC_PU_GAIN_CONTROL] = ot_stream_pu_gain_set,
    [OT_UVC_PU_POWER_LINE_FREQUENCY_CONTROL] = ot_stream_pu_power_line_frequency_set,
    [OT_UVC_PU_HUE_CONTROL] = ot_stream_pu_hue_set,
    [OT_UVC_PU_SATURATION_CONTROL] = ot_stream_pu_saturation_set,
    [OT_UVC_PU_SHARPNESS_CONTROL] = ot_stream_pu_sharpness_set,
    [OT_UVC_PU_GAMMA_CONTROL] = ot_stream_pu_gamma_set,
    [OT_UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL] = ot_stream_pu_white_balance_temperature_set,
    [OT_UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL] = ot_stream_pu_white_balance_temperature_auto_set,
    [OT_UVC_PU_WHITE_BALANCE_COMPONENT_CONTROL] = ot_stream_pu_white_balance_component_set,
    [OT_UVC_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL] = ot_stream_pu_white_balance_component_auto_set,
    [OT_UVC_PU_DIGITAL_MULTIPLIER_CONTROL] = ot_stream_pu_digital_multiplier_set,
    [OT_UVC_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL] = ot_stream_pu_digital_multiplier_limit_set,
    [OT_UVC_PU_HUE_AUTO_CONTROL] = ot_stream_pu_hue_auto_set,
    [OT_UVC_PU_ANALOG_VIDEO_STANDARD_CONTROL] = pu_data_do_nothing,
    [OT_UVC_PU_ANALOG_LOCK_STATUS_CONTROL] = pu_data_do_nothing,
    [OT_UVC_PU_CONTRAST_AUTO_CONTROL] = ot_stream_pu_contrast_auto_set,
    NULL,
};

void ot_stream_event_pu_data(struct ot_uvc_dev *dev, int unit_id, int control, struct uvc_request_data *data)
{
    if (data == NULL) {
        err("data is NULL\n");
        return;
    }

    if (unit_id != OT_UVC_PROCESSING_UNIT_ID) {
        return;
    }

    if (control < OT_UVC_PU_CONTROL_UNDEFINED || control > OT_UVC_PU_CONTRAST_AUTO_CONTROL) {
        err("invalid event type!\n");
        return;
    }

    g_pu_data_list[control](dev, data);
}
/* Processing Unit Operation Functions End */
