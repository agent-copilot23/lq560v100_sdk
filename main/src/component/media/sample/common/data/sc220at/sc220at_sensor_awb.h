/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef sc220at_SENSOR_AWB_H
#define sc220at_SENSOR_AWB_H

#include "ot_eis_vi_awb.h"
#include "ot_eis_vi_sns.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static const ot_eis_vi_awb_sensor_default g_sc220at_awb_cmos =
{
    4950, /* wb_ref_temp */
    {429, 256, 256, 555}, /* gain_offset */
    {47,120, -89, 323, -38452, 9467434}, /* wb_para */
    0, /* golden_rgain */
    0, /* golden_bgain */
    0, /* sample_rgain */
    0, /* sample_bgain */
    {
        1, /* valid */
        {128, 128, 128, 128, 124, 120, 116, 112, 108, 104, 100, 94, 90, 90, 90, 90}, /* saturation */
    },
    {
        6, /* ccm_tab_num */
    {
    {
                7500, /* ccm_tab_color_temp */
                {0x01B2, 0x80A7, 0x800B, 0x8049, 0x0188, 0x803F, 0x8003, 0x80A7, 0x1AA}, /* ccm_tab_color_ccm */
    },
    {
                6500, /* ccm_tab_color_temp */
                {0x01B2, 0x80A7, 0x800B, 0x8049, 0x0188, 0x803F, 0x8003, 0x80A7, 0x1AA}, /* ccm_tab_color_ccm */
    },
    {
                5000, /* ccm_tab_color_temp */
                {0x01B2, 0x80A7, 0x800B, 0x8049, 0x0188, 0x803F, 0x8003, 0x80A7, 0x1AA}, /* ccm_tab_color_ccm */
    },
    {
                4000, /* ccm_tab_color_temp */
                {0x1A4, 0x8087, 0x801D, 0x8048, 0x18A, 0x8042, 0x8009, 0x80B5, 0x1BE}, /* ccm_tab_color_ccm */
    },
    {
                2856, /* ccm_tab_color_temp */
                {0x190, 0x80A1, 0x11, 0x8044, 0x14D, 0x8009, 0x4, 0x80C1, 0x1BD}, /* ccm_tab_color_ccm */
    },
    {
                2300, /* ccm_tab_color_temp */
                {0x190, 0x80A1, 0x11, 0x8044, 0x14D, 0x8009, 0x4, 0x80C1, 0x1BD}, /* ccm_tab_color_ccm */
    },
    {
                2300, /* ccm_tab_color_temp */
                {0x190, 0x80A1, 0x11, 0x8044, 0x14D, 0x8009, 0x4, 0x80C1, 0x1BD}, /* ccm_tab_color_ccm */
    },
    },
    },
    0, /* init_rgain */
    0, /* init_ggain */
    0, /* init_bgain */
    1, /* awb_run_interval */
    {0, 0, 0, 0, 0}, /* init_ccm */
    {
        1, /* preawb_attr_enable */
        1, /* preawb_attr reverse enable */
        {2400, 2800, 4000, 5000, 6500, 7500}, /* preawb_attr_color_temp_array */
        {67051, 66168, 64001, 63188, 61240, 60919}, /* preawb_attr_ratio_r_array */
        {58385, 52546, 68416, 69240, 70212, 71045}, /* preawb_attr_ratio_b_array */
    },
};



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* sc220at_SENSOR_AWB_H */
