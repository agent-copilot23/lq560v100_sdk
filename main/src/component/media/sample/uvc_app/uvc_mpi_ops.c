/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */
#include <stdint.h>
#include "uvc_mpi_ops.h"
#include "ot_stream.h"
#include "uvc_media.h"
#include <stdio.h>
#include "sample_comm.h"
#include "uvc_ict_ops.h"
#include "ot_eis_vi_ae_api.h"


static uint16_t sample_uvc_pu_sharpness_get(uint32_t dev_no)
{
    return 0;
}

static uint16_t sample_uvc_pu_gamma_get(uint32_t dev_no)
{
    ot_eis_vi_exp_info exp_info = {0};
    ot_s32 ret;
    uint64_t total_gain;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    if (ctx == OT_NULL) {
        printf("%s sample_uvc_get_media_ctx failed, dev_no:%u\n", __func__, dev_no);
        return 0;
    }

    ret = ot_eis_vi_ae_query_info(ctx->vi_cfg.pipe_info[0].pipe_hdl, &exp_info);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_query_info %d\n", __func__, ret);
        return 0;
    }

    /* a_gain/d_gain/isp_d_gain are 22.10; total_gain keeps 10-bit precision. */
    total_gain = (uint64_t)exp_info.a_gain * exp_info.d_gain * exp_info.isp_d_gain / 1024 / 1024;

    printf("%s total_gain:%llu a_gain:%u d_gain:%u isp_d_gain:%u\n",
           __func__, (unsigned long long)total_gain, exp_info.a_gain, exp_info.d_gain, exp_info.isp_d_gain);

    if (total_gain > 0xFFFF) {
        total_gain = 0xFFFF;
    }

    return (uint16_t)total_gain;
}

static uint32_t sample_uvc_pu_white_balance_component_get(uint32_t dev_no)
{
    ot_eis_vi_exp_info exp_info = {0};
    ot_s32 ret;
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    if (ctx == OT_NULL) {
        printf("%s sample_uvc_get_media_ctx failed, dev_no:%u\n", __func__, dev_no);
        return 0;
    }

    ret = ot_eis_vi_ae_query_info(ctx->vi_cfg.pipe_info[0].pipe_hdl, &exp_info);
    if (ret != OT_SUCCESS) {
        printf("%s ot_eis_vi_ae_query_info %d\n", __func__, ret);
        return 0;
    }

    printf("%s exp_time:%u\n", __func__, exp_info.exp_time);

    return exp_info.exp_time;
}

static uint8_t sample_uvc_pu_white_balance_component_auto_get(uint32_t dev_no)
{
    return 0;
}

static uint16_t sample_uvc_pu_digital_multiplier_get(uint32_t dev_no)
{
    return 0;
}

static uint16_t  sample_uvc_pu_digital_multiplier_limit_get(uint32_t dev_no)
{
    return 0;
}

static uint8_t sample_uvc_pu_hue_auto_get(uint32_t dev_no)
{
    return 0;
}

static uint8_t  sample_uvc_analog_video_standard_get(uint32_t dev_no)
{
    return 0;
}

static uint8_t  sample_uvc_analog_lock_status_get(uint32_t dev_no)
{
    return 0;
}

static uint8_t  sample_uvc_pu_contrast_auto_get(uint32_t dev_no)
{
    return 0;
}

static ot_void (uint32_t dev_no, uint16_t v)
{
    printf("func:%s data:0x%x\n", __func__, v);
}

static ot_void sample_uvc_pu_gamma_set(uint32_t dev_no, uint16_t v)
{
    printf("func:%s data:0x%x\n", __func__, v);
}

static ot_void sample_uvc_pu_white_balance_component_set(uint32_t dev_no, uint32_t v)
{
    printf("func:%s data:0x%x\n", __func__, v);
}

static ot_void sample_uvc_pu_white_balance_component_auto_set(uint32_t dev_no, uint8_t v)
{
    printf("func:%s data:0x%x\n", __func__, v);
}

static ot_void sample_uvc_pu_digital_multiplier_set(uint32_t dev_no, uint16_t v)
{
    printf("func:%s data:0x%x\n", __func__, v);
}

static ot_void sample_uvc_pu_digital_multiplier_limit_set(uint32_t dev_no, uint16_t v)
{
    printf("func:%s data:0x%x\n", __func__, v);
}

static ot_void sample_uvc_pu_hue_auto_set(uint32_t dev_no, uint8_t v)
{
    printf("func:%s data:0x%x\n", __func__, v);
}

static ot_void sample_uvc_pu_contrast_auto_set(uint32_t dev_no, uint8_t v)
{
    printf("func:%s data:0x%x\n", __func__, v);
}

static uint8_t sample_uvc_ct_scanning_mode_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_ct_exposure_auto_priority_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_ct_exposure_relative_time_get(uint32_t dev_no)
{
    return 0x5;
}

//2026.10.22 修改获取曝光率 focus曝光率 exp:exposure曝光
//static uint16_t sample_uvc_ct_focus_absolute_get(uint32_t dev_no)
//{
//    return 0x5;
//}
static uint16_t sample_uvc_ct_focus_absolute_get(uint32_t dev_no)
{
    ot_eis_vi_exp_info exp_info = {0};
    uvc_media_ctx *ctx = sample_uvc_get_media_ctx(dev_no);

    if(ctx == OT_NULL){
        return 0;
    }
    if(ot_eis_vi_ae_query_info(ctx->vi_cfg.pipe_info[0].pipe_hdl,&exp_info) != OT_SUCCESS){
        return 0;
    }
    return (uint16_t)((exp_info.exp_time / 10) & 0xFFFF);
}

static uint16_t sample_uvc_ct_focus_relative_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_ct_focus_sample_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_ct_focus_auto_get(uint32_t dev_no)
{
    return 0x5;
}

static uint16_t sample_uvc_ct_iris_absolute_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_ct_iris_relative_get(uint32_t dev_no)
{
    return 0x5;
}

static uint16_t sample_uvc_ct_zoom_absolute_get(uint32_t dev_no)
{
    return 0x5;
}

static uint32_t sample_uvc_ct_zoom_relative_get(uint32_t dev_no)
{
    return 0x5;
}

static uint64_t sample_uvc_ct_pantilt_absolute_get(uint32_t dev_no)
{
    return 0x5;
}

static uint64_t sample_uvc_ct_pantilt_relative_get(uint32_t dev_no)
{
    return 0x5;
}

static uint16_t sample_uvc_ct_roll_absolute_get(uint32_t dev_no)
{
    return 0x5;
}

static uint16_t sample_uvc_ct_roll_relative_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_ct_privacy_get(uint32_t dev_no)
{
    return 0x5;
}

static ot_void sample_uvc_ct_window_get(uint32_t dev_no, ct_window_params *params)
{
    memset_s(params, sizeof(ct_window_params), 0x0, sizeof(ct_window_params));
    params->window_top = 0x5;
}

static ot_void sample_uvc_ct_region_of_interest_get(uint32_t dev_no, region_of_interest_params *params)
{
    memset_s(params, sizeof(region_of_interest_params), 0x0, sizeof(region_of_interest_params));
    params->roi_top = 0x5;
}

static ot_void sample_uvc_ct_scanning_mode_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_exposure_auto_priority_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_exposure_relative_time_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_focus_absolute_set(uint32_t dev_no, uint16_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_focus_relative_set(uint32_t dev_no, uint8_t focus_relative, uint8_t speed)
{
    printf("v:0x%x %x \n", speed, focus_relative);
}

static ot_void sample_uvc_ct_focus_sample_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_focus_auto_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_iris_absolute_set(uint32_t dev_no, uint16_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_iris_relative_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_zoom_absolute_set(uint32_t dev_no, uint16_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_zoom_relative_set(uint32_t dev_no, uint16_t zoom, uint16_t digital_zoom, uint16_t speed)
{
    printf("v:0x%x %x %x \n", speed, digital_zoom, zoom);
}

static ot_void sample_uvc_ct_pantilt_absolute_set(uint32_t dev_no, uint32_t pan_absolute, uint32_t tilt_absolute)
{
    printf("v:0x%x %x \n", tilt_absolute, pan_absolute);
}

static ot_void sample_uvc_ct_pantilt_relative_set(uint32_t dev_no, uint8_t pan_relative, uint8_t pan_speed, uint8_t tilt_relative, uint8_t tilt_speed)
{
    printf("v:0x%x %x %x %x \n", tilt_speed, tilt_relative, pan_speed, pan_relative);
}

static ot_void sample_uvc_ct_roll_absolute_set(uint32_t dev_no, uint16_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_roll_relative_set(uint32_t dev_no, uint8_t roll_relative, uint8_t speed)
{
    printf("v:0x%x %x \n", speed, roll_relative);
}

static ot_void sample_uvc_ct_privacy_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x \n", v);
}

static ot_void sample_uvc_ct_window_set(uint32_t dev_no, ct_window_params *params)
{
    printf("v:0x%x %x %x %x %x %x \n", params->num_step_units, params->num_steps, params->window_right, params->window_bottom, params->window_left, params->window_top);
}

static ot_void sample_uvc_ct_region_of_interest_set(uint32_t dev_no, region_of_interest_params *params)
{
    printf("v:0x%x %x %x %x %x \n", params->auto_controls, params->roi_right, params->roi_bottom, params->roi_left, params->roi_top);
}

static uint16_t sample_uvc_eu_select_layer_get(uint32_t dev_no)
{
    return 0x5;
}

static uint64_t sample_uvc_eu_profile_toolset_get(uint32_t dev_no)
{
    return 0x5;
}

static uint32_t sample_uvc_eu_video_resolution_get(uint32_t dev_no)
{
    return 0x5;
}

static uint32_t sample_uvc_eu_min_frame_interval_get(uint32_t dev_no)
{
    return 0x5;
}

static uint32_t sample_uvc_eu_slice_mode_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_eu_rate_ctrl_mode_get(uint32_t dev_no)
{
    return 0x5;
}

static uint32_t sample_uvc_eu_average_bitrate_get(uint32_t dev_no)
{
    return 0x5;
}

static uint32_t sample_uvc_eu_cpb_size_get(uint32_t dev_no)
{
    return 0x5;
}

static uint32_t sample_uvc_eu_peak_bit_rate_get(uint32_t dev_no)
{
    return 0x5;
}

static uint64_t sample_uvc_eu_quantization_params_get(uint32_t dev_no)
{
    return 0x5;
}

static uint32_t sample_uvc_eu_sync_ref_frame_get(uint32_t dev_no)
{
    return 0x5;
}

static uint16_t sample_uvc_eu_ltr_buffer_get(uint32_t dev_no)
{
    return 0x5;
}

static uint16_t sample_uvc_eu_ltr_picture_get(uint32_t dev_no)
{
    return 0x5;
}

static uint16_t sample_uvc_eu_ltr_validation_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_eu_level_idc_limit_get(uint32_t dev_no)
{
    return 0x5;
}
static uint64_t sample_uvc_eu_sei_payloadtype_get(uint32_t dev_no)
{
    return 0x5;
}

static uint16_t sample_uvc_eu_qp_range_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_eu_priority_get(uint32_t dev_no)
{
    return 0x5;
}

static uint8_t sample_uvc_eu_start_or_stop_layer_get(uint32_t dev_no)
{
    return 0x5;
}

static uint16_t sample_uvc_eu_error_resiliency_get(uint32_t dev_no)
{
    return 0x5;
}

static ot_void sample_uvc_eu_select_layer_set(uint32_t dev_no, uint16_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_profile_toolset_set(uint32_t dev_no, uint16_t profile, uint16_t constrained_toolset, uint16_t settings)
{
    printf("v:0x%x %x %x\n", settings, constrained_toolset, profile);
}

static ot_void sample_uvc_eu_video_resolution_set(uint32_t dev_no, uint16_t width, uint16_t height)
{
    printf("v:0x%x %x\n", height, width);
}

static ot_void sample_uvc_eu_min_frame_interval_set(uint32_t dev_no, uint32_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_slice_mode_set(uint32_t dev_no, uint16_t slice_mode, uint16_t slice_config)
{
    printf("v:0x%x %x\n", slice_config, slice_mode);
}

static ot_void sample_uvc_eu_rate_ctrl_mode_set(uint32_t dev_no, uint16_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_average_bitrate_set(uint32_t dev_no, uint32_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_cpb_size_set(uint32_t dev_no, uint32_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_peak_bit_rate_set(uint32_t dev_no, uint32_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_quantization_params_set(uint32_t dev_no, uint16_t qp_prime_i, uint16_t qp_prime_p, uint16_t qp_prime_b)
{
    printf("v:0x%x %x %x\n", qp_prime_b, qp_prime_p, qp_prime_i);
}

static ot_void sample_uvc_eu_sync_ref_frame_set(uint32_t dev_no, uint8_t sync_frame_type, uint8_t sync_frame_interval, uint8_t gradual_decode_refresh)
{
    printf("v:0x%x %x %x\n", gradual_decode_refresh, sync_frame_interval, sync_frame_type);
}

static ot_void sample_uvc_eu_ltr_buffer_set(uint32_t dev_no, uint8_t num_host_control_ltr_buffer, uint8_t trust_mode)
{
    printf("v:0x%x %x\n", trust_mode, num_host_control_ltr_buffer);
}

static ot_void sample_uvc_eu_ltr_picture_set(uint32_t dev_no, uint8_t put_at_pos_in_ltr_buffer, uint8_t ltr_mode)
{
    printf("v:0x%x %x\n", ltr_mode, put_at_pos_in_ltr_buffer);
}

static ot_void sample_uvc_eu_ltr_validation_set(uint32_t dev_no, uint16_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_level_idc_limit_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_sei_payloadtype_set(uint32_t dev_no, uint64_t v)
{
    printf("v:0x%lx\n", v);
}

static ot_void sample_uvc_eu_qp_range_set(uint32_t dev_no, uint8_t min_qp, uint8_t max_qp)
{
    printf("v:0x%x %x\n", max_qp, min_qp);
}

static ot_void sample_uvc_eu_priority_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_start_or_stop_layer_set(uint32_t dev_no, uint8_t v)
{
    printf("v:0x%x\n", v);
}

static ot_void sample_uvc_eu_error_resiliency_set(uint32_t dev_no, uint16_t v)
{
    printf("v:0x%x\n", v);
}

/* Stream Control Operation Functions End */
static struct processing_unit_ops gs_pu_ops = {
    .backlight_compensation_get = sample_uvc_pu_backlight_compensation_get,
    .brightness_get = sample_uvc_pu_brightness_get,
    .contrast_get = sample_uvc_pu_contrast_get,
    .gain_get = sample_uvc_pu_gain_get,
    .power_line_frequency_get = sample_uvc_pu_power_line_frequency_get,
    .hue_get = sample_uvc_pu_hue_get,
    .saturation_get = sample_uvc_pu_saturation_get,
    .sharpness_get = sample_uvc_pu_sharpness_get,
    .gamma_get = sample_uvc_pu_gamma_get,
    .white_balance_temperature_get = sample_uvc_pu_white_balance_temperature_get,
    .white_balance_temperature_auto_get = sample_uvc_pu_white_balance_temperature_auto_get,
    .white_balance_component_get = sample_uvc_pu_white_balance_component_get,
    .white_balance_component_auto_get = sample_uvc_pu_white_balance_component_auto_get,
    .digital_multiplier_get = sample_uvc_pu_digital_multiplier_get,
    .digital_multiplier_limit_get = sample_uvc_pu_digital_multiplier_limit_get,
    .hue_auto_get = sample_uvc_pu_hue_auto_get,
    .analog_video_standard_get = sample_uvc_analog_video_standard_get,
    .analog_lock_status_get = sample_uvc_analog_lock_status_get,
    .contrast_auto_get = sample_uvc_pu_contrast_auto_get,

    .backlight_compensation_set = sample_uvc_pu_backlight_compensation_set,
    .brightness_set = sample_uvc_pu_brightness_set,
    .contrast_set = sample_uvc_pu_contrast_set,
    .gain_set = sample_uvc_pu_gain_set,
    .power_line_frequency_set = sample_uvc_pu_power_line_frequency_set,
    .hue_set = sample_uvc_pu_hue_set,
    .saturation_set = sample_uvc_pu_saturation_set,
    .sharpness_set = sample_uvc_pu_sharpness_set,
    .gamma_set = sample_uvc_pu_gamma_set,
    .white_balance_temperature_set = sample_uvc_pu_white_balance_temperature_set,
    .white_balance_temperature_auto_set = sample_uvc_pu_white_balance_temperature_auto_set,
    .white_balance_component_set = sample_uvc_pu_white_balance_component_set,
    .white_balance_component_auto_set = sample_uvc_pu_white_balance_component_auto_set,
    .digital_multiplier_set = sample_uvc_pu_digital_multiplier_set,
    .digital_multiplier_limit_set = sample_uvc_pu_digital_multiplier_limit_set,
    .hue_auto_set = sample_uvc_pu_hue_auto_set,
    .contrast_auto_set = sample_uvc_pu_contrast_auto_set,
};

static struct camera_terminal_ops gs_ct_ops = {
    .exposure_absolute_time_get = sample_uvc_ct_exposure_absolute_time_get,
    .exposure_auto_mode_get = sample_uvc_ct_exposure_auto_mode_get,
    .scanning_mode_get = sample_uvc_ct_scanning_mode_get,
    .exposure_auto_priority_get = sample_uvc_ct_exposure_auto_priority_get,
    .exposure_relative_time_get = sample_uvc_ct_exposure_relative_time_get,
    .focus_absolute_get = sample_uvc_ct_focus_absolute_get,
    .focus_relative_get = sample_uvc_ct_focus_relative_get,
    .focus_sample_get = sample_uvc_ct_focus_sample_get,
    .focus_auto_get = sample_uvc_ct_focus_auto_get,
    .iris_absolute_get = sample_uvc_ct_iris_absolute_get,
    .iris_relative_get = sample_uvc_ct_iris_relative_get,
    .zoom_absolute_get = sample_uvc_ct_zoom_absolute_get,
    .zoom_relative_get = sample_uvc_ct_zoom_relative_get,
    .pantilt_absolute_get = sample_uvc_ct_pantilt_absolute_get,
    .pantilt_relative_get = sample_uvc_ct_pantilt_relative_get,
    .roll_absolute_get = sample_uvc_ct_roll_absolute_get,
    .roll_relative_get = sample_uvc_ct_roll_relative_get,
    .ct_privacy_get = sample_uvc_ct_privacy_get,
    .ct_window_get = sample_uvc_ct_window_get,
    .region_of_interest_get = sample_uvc_ct_region_of_interest_get,

    .exposure_absolute_time_set = sample_uvc_ct_exposure_absolute_time_set,
    .exposure_auto_mode_set = sample_uvc_ct_exposure_auto_mode_set,
    .scanning_mode_set = sample_uvc_ct_scanning_mode_set,
    .exposure_auto_priority_set = sample_uvc_ct_exposure_auto_priority_set,
    .exposure_relative_time_set = sample_uvc_ct_exposure_relative_time_set,
    .focus_absolute_set = sample_uvc_ct_focus_absolute_set,
    .focus_relative_set = sample_uvc_ct_focus_relative_set,
    .focus_sample_set = sample_uvc_ct_focus_sample_set,
    .focus_auto_set = sample_uvc_ct_focus_auto_set,
    .iris_absolute_set = sample_uvc_ct_iris_absolute_set,
    .iris_relative_set = sample_uvc_ct_iris_relative_set,
    .zoom_absolute_set = sample_uvc_ct_zoom_absolute_set,
    .zoom_relative_set = sample_uvc_ct_zoom_relative_set,
    .pantilt_absolute_set = sample_uvc_ct_pantilt_absolute_set,
    .pantilt_relative_set = sample_uvc_ct_pantilt_relative_set,
    .roll_absolute_set = sample_uvc_ct_roll_absolute_set,
    .roll_relative_set = sample_uvc_ct_roll_relative_set,
    .ct_privacy_set = sample_uvc_ct_privacy_set,
    .ct_window_set = sample_uvc_ct_window_set,
    .region_of_interest_set = sample_uvc_ct_region_of_interest_set,
};

static struct encoding_unit_ops gs_eu_ops = {
    .select_layer_get = sample_uvc_eu_select_layer_get,
    .profile_toolset_get = sample_uvc_eu_profile_toolset_get,
    .video_resolution_get = sample_uvc_eu_video_resolution_get,
    .min_frame_interval_get = sample_uvc_eu_min_frame_interval_get,
    .slice_mode_get = sample_uvc_eu_slice_mode_get,
    .rate_ctrl_mode_get = sample_uvc_eu_rate_ctrl_mode_get,
    .average_bitrate_get = sample_uvc_eu_average_bitrate_get,
    .cpb_size_get = sample_uvc_eu_cpb_size_get,
    .peak_bit_rate_get = sample_uvc_eu_peak_bit_rate_get,
    .quantization_params_get = sample_uvc_eu_quantization_params_get,
    .sync_ref_frame_get = sample_uvc_eu_sync_ref_frame_get,
    .ltr_buffer_get = sample_uvc_eu_ltr_buffer_get,
    .ltr_picture_get = sample_uvc_eu_ltr_picture_get,
    .ltr_validation_get = sample_uvc_eu_ltr_validation_get,
    .level_idc_limit_get = sample_uvc_eu_level_idc_limit_get,
    .sei_payloadtype_get = sample_uvc_eu_sei_payloadtype_get,
    .qp_range_get = sample_uvc_eu_qp_range_get,
    .priority_get = sample_uvc_eu_priority_get,
    .start_or_stop_layer_get = sample_uvc_eu_start_or_stop_layer_get,
    .error_resiliency_get = sample_uvc_eu_error_resiliency_get,

    .select_layer_set = sample_uvc_eu_select_layer_set,
    .profile_toolset_set = sample_uvc_eu_profile_toolset_set,
    .video_resolution_set = sample_uvc_eu_video_resolution_set,
    .min_frame_interval_set = sample_uvc_eu_min_frame_interval_set,
    .slice_mode_set = sample_uvc_eu_slice_mode_set,
    .rate_ctrl_mode_set = sample_uvc_eu_rate_ctrl_mode_set,
    .average_bitrate_set = sample_uvc_eu_average_bitrate_set,
    .cpb_size_set = sample_uvc_eu_cpb_size_set,
    .peak_bit_rate_set = sample_uvc_eu_peak_bit_rate_set,
    .quantization_params_set = sample_uvc_eu_quantization_params_set,
    .sync_ref_frame_set = sample_uvc_eu_sync_ref_frame_set,
    .ltr_buffer_set = sample_uvc_eu_ltr_buffer_set,
    .ltr_picture_set = sample_uvc_eu_ltr_picture_set,
    .ltr_validation_set = sample_uvc_eu_ltr_validation_set,
    .level_idc_limit_set = sample_uvc_eu_level_idc_limit_set,
    .sei_payloadtype_set = sample_uvc_eu_sei_payloadtype_set,
    .qp_range_set = sample_uvc_eu_qp_range_set,
    .priority_set = sample_uvc_eu_priority_set,
    .start_or_stop_layer_set = sample_uvc_eu_start_or_stop_layer_set,
    .error_resiliency_set = sample_uvc_eu_error_resiliency_set
};

static struct stream_control_ops gs_sc_ops = {
    .init         = sample_uvc_sc_init,
    .deinit       = sample_uvc_sc_deinit,
    .startup      = sample_uvc_sc_startup,
    .shutdown     = sample_uvc_sc_shutdown,
    .set_idr      = sample_uvc_sc_set_idr,
    .set_property = sample_uvc_sc_set_property,
    .get_send     = sample_uvc_sc_get_send,
};

void camera_register_uvc_mpi_ops(void)
{
    printf("\n@@@@@ UVC App Sample @@@@@\n\n");

    ot_stream_register_mpi_ops(&gs_sc_ops, &gs_pu_ops, &gs_ct_ops, &gs_eu_ops, NULL);
}
