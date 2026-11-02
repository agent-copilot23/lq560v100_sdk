/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "securec.h"
#include "ot_scene_setparam.h"
#include "ot_confaccess.h"
#include "ot_scenecomm.h"
#include "ot_scene_loadparam.h"
#include "ot_scene.h"
#include "ot_eis_scene_auto_api.h"

#include "ot_eis_media_sys_common.h"
#include "sample_comm.h"
#include "ot_eis_common_venc.h"
#include "ini.h"

#ifdef __cplusplus
extern "C" {
#endif

/* param config file path */
#define SCENE_INIPARAM "sceneparamini"
#define SCENE_INI_SCENEMODE "scene_param_"
#define SCENE_INIPARAM_MODULE_NAME_LEN 64
#define SCENE_INIPARAM_NODE_NAME_LEN 128
#define SCENETOOL_MAX_FILESIZE 512

#define SCENE_INI_MEDIAPIPE "scene_mediapipe"

#define scene_iniparam_check_load_return(ret, name) do { \
        if ((ret) != OT_SUCCESS) {                       \
            scene_loge(" Load [%s] failed\n", name);     \
            return OT_FAILURE;                           \
        }                                                \
    } while (0)

#define scene_copy_array(dest, src, size, type) do {         \
        for (ot_u32 index_ = 0; index_ < (size); index_++) { \
            (dest)[index_] = (type)(src)[index_];            \
        }                                                    \
    } while (0)

#define scene_load_array(module, node, array, size, type) do {                                    \
        char *got_string_ = NULL;                                                                 \
        ot_s32 ret_ = ot_confaccess_get_string(SCENE_INIPARAM, module, node, NULL, &got_string_); \
        scene_iniparam_check_load_return(ret_, node);                                             \
        if (got_string_ != OT_NULL) {                                                             \
            scene_get_numbers_in_one_line(got_string_);                                           \
            scene_copy_array(array, g_line_data, size, type);                                     \
            ot_scenecomm_safe_free(got_string_);                                                  \
        }                                                                                         \
    } while (0)

#define scene_load_int(module, node, dest, type) do {                             \
        ret = ot_confaccess_get_int(SCENE_INIPARAM, module, node, 0, &get_value); \
        scene_iniparam_check_load_return(ret, node);                              \
        dest = (type)get_value;                                                   \
    } while (0)

#define scene_load_float(module, node, dest, type) do {                             \
        ot_double get_value = 0;                                                    \
        ret = ot_confaccess_get_double(SCENE_INIPARAM, module, node, 0, &get_value); \
        scene_iniparam_check_load_return(ret, node);                              \
        dest = (type)get_value;                                                   \
    } while (0)


#define scene_load_str(module, node, dest, size) do {                             \
        char *got_string_ = NULL;                                                                 \
        ot_s32 ret_ = ot_confaccess_get_string(SCENE_INIPARAM, module, node, NULL, &got_string_); \
        scene_iniparam_check_load_return(ret_, node);                                             \
        if (got_string_ != OT_NULL) {                                                             \
            int min_size = size > strlen(got_string_) ? strlen(got_string_) : size;               \
            (ot_void)memcpy_s(&dest, min_size, got_string_, min_size);                            \
            ot_scenecomm_safe_free(got_string_);                                                  \
        }                                                                                         \
    } while (0)

#define SCENE_COMPARE_IGNORE_CASE(s1, s2) strcasecmp(s1, s2)

#define scene_load_enum(module, node, dest) do {                             \
        char *got_string_ = NULL;                                                                 \
        ot_s32 ret_ = ot_confaccess_get_string(SCENE_INIPARAM, module, node, NULL, &got_string_); \
        scene_iniparam_check_load_return(ret_, node);                                             \
        if (got_string_ != OT_NULL) {                                                             \
            ot_u32 map_idx = 0;                                                                   \
            while (g_enum_map[map_idx].name != NULL) {                                            \
                if (SCENE_COMPARE_IGNORE_CASE(got_string_, g_enum_map[map_idx].name) == 0) {      \
                    dest = g_enum_map[map_idx].value;                                             \
                }                                                                                 \
                map_idx++;                                                                        \
            }                                                                                     \
            ot_scenecomm_safe_free(got_string_);                                                  \
        }                                                                                         \
    } while (0)

#define scene_set_enum_mapping_tag(tag) {#tag, tag}

/* param */
static ot_s64 g_line_data[6000]; /* 5000 line num */

ot_char g_ini_dir[SCENETOOL_MAX_FILESIZE] = {0};
ot_char g_relative_path[SCENETOOL_MAX_FILESIZE] = {0};

static scene_enum_mapping g_enum_map[] = {

    // sample_vo_display_mode
    scene_set_enum_mapping_tag(ONE_IN_PORT_MIPI_AV069HDT_1280X720),
    scene_set_enum_mapping_tag(ONE_IN_PORT_MIPI_768),
    scene_set_enum_mapping_tag(TWO_IN_PORT_MIPI_768),
    scene_set_enum_mapping_tag(ONE_IN_PORT_MIPI_800X1280),
    scene_set_enum_mapping_tag(TWO_IN_PORT_MIPI_AV069HDT_2560X720),
    scene_set_enum_mapping_tag(TWO_IN_PORT_MIPI_2560X768),

    // sample_sns_type
    scene_set_enum_mapping_tag(OV_OX08B40_MIPI_8M_30FPS_12BIT),
    scene_set_enum_mapping_tag(OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT),
    scene_set_enum_mapping_tag(OV_OX03C_MIPI_3M_60FPS_16BIT),
    scene_set_enum_mapping_tag(OV_OX03F10_MIPI_3M_60FPS_12BIT),
    scene_set_enum_mapping_tag(OV_OX03F10_MIPI_3M_60FPS_16BIT),
    scene_set_enum_mapping_tag(OV_OX05B1S_MIPI_5M_60FPS_10BIT),
    scene_set_enum_mapping_tag(SONY_IMX623_MIPI_3M_60FPS_16BIT),
    scene_set_enum_mapping_tag(SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1),
    scene_set_enum_mapping_tag(SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1),
    scene_set_enum_mapping_tag(GC_GC2093_MIPI_2M_30FPS_10BIT),
    scene_set_enum_mapping_tag(GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1),
    scene_set_enum_mapping_tag(SMS_SC220AT_MIPI_2M_30FPS_YUV422),
    scene_set_enum_mapping_tag(SMS_SC220AT_MIPI_2M_30FPS_RAW12),

    {NULL, 0},
};

static ot_s32 scene_get_numbers_in_one_line(const ot_char *input_line)
{
    const ot_char *vr_begin = input_line;
    const ot_char *vr_end = vr_begin;
    ot_u32 part_count = 0;
    ot_char    part[20] = {0}; /* 20 buffer len */
    size_t whole_count = 0;
    size_t length = strlen(input_line);
    ot_u64 hex_value;
    errno_t err;

    ot_s32 i = 0;
    ot_bool is_hex_num = OT_FALSE;
    (ot_void)memset_s(g_line_data, sizeof(g_line_data), 0, sizeof(g_line_data));
    while ((vr_end != NULL)) {
        if ((whole_count > length) || (whole_count == length)) {
            break;
        }

        while ((*vr_end != '|') && (*vr_end != '\0') && (*vr_end != ',')) {
            if (*vr_end == 'x') {
                is_hex_num = OT_TRUE;
            }
            vr_end++;
            part_count++;
            whole_count++;
        }

        err = memcpy_s(part, sizeof(part) - 1, vr_begin, part_count);
        if (err != EOK) {
            break;
        }

        if (is_hex_num == OT_TRUE) {
            ot_char *end_ptr = NULL;
            hex_value = (ot_u64)strtoll(part + 2, &end_ptr, 16); /* 16 Hexadecimal, 2 offset */
            g_line_data[i] = hex_value;
        } else {
            g_line_data[i] = (ot_s64)strtoll(part, NULL, 10); /* base 10 */
        }

        (ot_void)memset_s(part, sizeof(part), 0, sizeof(part));
        part_count = 0;
        vr_end++;
        vr_begin = vr_end;
        whole_count++;
        i++;
    }
    return i;
}

static ot_s32 scene_load_module_state_configs(const ot_char *module, ot_scene_module_state *module_state)
{
    ot_s32 ret, get_value;
    scene_load_int(module, "module_state:static_csc", module_state->static_csc, ot_bool);
    scene_load_int(module, "module_state:static_crosstalk", module_state->static_cross_talk, ot_bool);
    scene_load_int(module, "module_state:static_demosaic", module_state->static_dm, ot_bool);
    scene_load_int(module, "module_state:static_bayer_sharpen", module_state->static_bayeshp, ot_bool);
    scene_load_int(module, "module_state:static_sharpen", module_state->static_sharpen, ot_bool);
    scene_load_int(module, "module_state:static_3dnr", module_state->static_3dnr, ot_bool);
    scene_load_int(module, "module_state:static_blc", module_state->static_blc, ot_bool);
    scene_load_int(module, "module_state:static_mce", module_state->static_mce, ot_bool);
    scene_load_int(module, "module_state:dynamic_vproc", module_state->dynamic_vproc, ot_bool);
    scene_load_int(module, "module_state:dynamic_ae", module_state->dynamic_ae, ot_bool);
    scene_load_int(module, "module_state:dynamic_fps", module_state->dynamic_fps, ot_bool);
    scene_load_int(module, "module_state:dynamic_wdr_exposure", module_state->dynamic_wdr_exposure, ot_bool);
    scene_load_int(module, "module_state:dynamic_wdr", module_state->dynamic_fswdr, ot_bool);
    scene_load_int(module, "module_state:dynamic_blc", module_state->dynamic_blc, ot_bool);
    scene_load_int(module, "module_state:dynamic_dehaze", module_state->dynamic_dehaze, ot_bool);
    scene_load_int(module, "module_state:dynamic_drc", module_state->dynamic_drc, ot_bool);
    scene_load_int(module, "module_state:dynamic_gamma", module_state->dynamic_gamma, ot_bool);
    scene_load_int(module, "module_state:dynamic_nr", module_state->dynamic_nr, ot_bool);
    scene_load_int(module, "module_state:dynamic_dpc", module_state->dynamic_dpc, ot_bool);
    scene_load_int(module, "module_state:dynamic_awb", module_state->dynamic_awb, ot_bool);
    scene_load_int(module, "module_state:dynamic_linear_ca", module_state->dynamic_linear_ca, ot_bool);
    scene_load_int(module, "module_state:dynamic_ca", module_state->dynamic_ca, ot_bool);
    scene_load_int(module, "module_state:dynamic_shading", module_state->dynamic_shading, ot_bool);
    scene_load_int(module, "module_state:dynamic_clut", module_state->dynamic_clut, ot_bool);
    scene_load_int(module, "module_state:dynamic_ldci", module_state->dynamic_ldci, ot_bool);
    scene_load_int(module, "module_state:dynamic_ldcihlc", module_state->dynamic_ldcihlc, ot_bool);
    scene_load_int(module, "module_state:dynamic_false_color", module_state->dynamic_false_color, ot_bool);
    scene_load_int(module, "module_state:dynamic_3dnr", module_state->dynamic_3dnr, ot_bool);
    scene_load_int(module, "module_state:dynamic_abnr", module_state->dynamic_abnr, ot_bool);
    scene_load_int(module, "module_state:dynamic_iqt", module_state->dynamic_iqt, ot_bool);
    scene_load_int(module, "module_state:dynamic_mce", module_state->dynamic_mce, ot_bool);
    scene_load_int(module, "module_state:dynamic_back_light", module_state->dynamic_back_light, ot_bool);
    return OT_SUCCESS;
}

static ot_s32 scene_load_module_state(const ot_char *module, ot_scene_module_state *module_state)
{
    ot_s32 ret, get_value;
    ot_scenecomm_check_pointer_return(module_state, OT_FAILURE);

    scene_load_int(module, "module_state:debug", module_state->debug, ot_bool);
    scene_load_int(module, "module_state:static_ae", module_state->static_ae, ot_bool);
    scene_load_int(module, "module_state:ae_weight_table", module_state->ae_weight_tab, ot_bool);
    scene_load_int(module, "module_state:static_back_light", module_state->static_back_light, ot_bool);
    scene_load_int(module, "module_state:static_wdr_exposure", module_state->static_wdr_exposure, ot_bool);
    scene_load_int(module, "module_state:static_wdr", module_state->static_fswdr, ot_bool);
    scene_load_int(module, "module_state:static_awb", module_state->static_awb, ot_bool);
    scene_load_int(module, "module_state:static_awb_ex", module_state->static_awbex, ot_bool);
    scene_load_int(module, "module_state:static_ccm", module_state->static_ccm, ot_bool);
    scene_load_int(module, "module_state:static_saturation", module_state->static_saturation, ot_bool);
    scene_load_int(module, "module_state:static_clut", module_state->static_clut, ot_bool);
    scene_load_int(module, "module_state:static_ldci", module_state->static_ldci, ot_bool);
    scene_load_int(module, "module_state:static_ldcihlc", module_state->static_ldcihlc, ot_bool);
    scene_load_int(module, "module_state:static_drc", module_state->static_drc, ot_bool);
    scene_load_int(module, "module_state:static_nr", module_state->static_nr, ot_bool);
    scene_load_int(module, "module_state:static_ca", module_state->static_ca, ot_bool);
    scene_load_int(module, "module_state:static_cac", module_state->static_cac, ot_bool);
    scene_load_int(module, "module_state:static_dpc", module_state->static_dpc, ot_bool);
    scene_load_int(module, "module_state:static_dehaze", module_state->static_dehaze, ot_bool);
    scene_load_int(module, "module_state:static_shading", module_state->static_shading, ot_bool);
    scene_load_int(module, "module_state:dynamic_fpn", module_state->dynamic_fpn, ot_bool);
    scene_load_int(module, "module_state:static_gamma", module_state->static_gamma, ot_bool);
    ret = scene_load_module_state_configs(module, module_state);
    ot_scenecomm_check_return(ret, ret);

    return OT_SUCCESS;
}

static ot_s32 scene_load_ae_weight_tab(const ot_char *module, ot_scene_static_statisticscfg *static_statistics)
{
    ot_scenecomm_check_pointer_return(static_statistics, OT_FAILURE);
    ot_u32 row;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = {0};

    for (row = 0; row < OT_EIS_VI_AE_ZONE_ROW; row++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_ae_weight:ae_weight_%u", row);
        scene_load_array(module, node, static_statistics->ae_weight[row], OT_EIS_VI_AE_ZONE_COLUMN, ot_u8);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_mce(const ot_char *module, ot_scene_static_mce *static_mce)
{
    ot_s32 ret, get_value;
    ot_scenecomm_check_pointer_return(static_mce, OT_FAILURE);

    scene_load_int(module, "static_mce:mce_enable", static_mce->mce_attr.mce_enable, ot_bool);
    scene_load_int(module, "static_mce:mce_start_iso", static_mce->mce_attr.mce_start_iso, ot_u32);
    scene_load_int(module, "static_mce:mce_stop_iso", static_mce->mce_attr.mce_stop_iso, ot_u32);
    scene_load_int(module, "static_mce:mce_start_ct", static_mce->mce_attr.mce_start_ct, ot_u16);
    scene_load_int(module, "static_mce:mce_stop_ct", static_mce->mce_attr.mce_stop_ct, ot_u16);
    scene_load_int(module, "static_mce:mce_retreat_ct_thresh", static_mce->mce_attr.mce_retreat_ct_thresh, ot_u16);
    scene_load_int(module, "static_mce:mce_retreat_ct_list_size", static_mce->mce_attr.mce_retreat_ct_list_size, ot_u16);
    scene_load_int(module, "static_mce:mce_alpha_thresh_top", static_mce->mce_attr.mce_alpha_thresh_top, ot_u8);
    scene_load_int(module, "static_mce:mce_alpha_thresh_bot", static_mce->mce_attr.mce_alpha_thresh_bot, ot_u8);
    scene_load_int(module, "static_mce:mce_list_size", static_mce->mce_attr.mce_list_size, ot_u8);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_ae(const ot_char *module, ot_scene_static_ae *static_ae)
{
    ot_scenecomm_check_pointer_return(static_ae, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_ae:ae_run_interval", static_ae->ae_run_interval, ot_u8);
    scene_load_int(module, "static_ae:ae_route_ex_valid", static_ae->ae_route_ex_valid, ot_bool);
    scene_load_int(module, "static_ae:auto_sys_gain_max", static_ae->auto_sys_gain_max, ot_u32);
    scene_load_int(module, "static_ae:auto_exp_time_max", static_ae->auto_exp_time_max, ot_u32);
    scene_load_int(module, "static_ae:auto_speed", static_ae->auto_speed, ot_u8);
    scene_load_int(module, "static_ae:auto_tolerance", static_ae->auto_tolerance, ot_u8);
    scene_load_int(module, "static_ae:auto_black_delay_frame", static_ae->auto_black_delay_frame, ot_u16);
    scene_load_int(module, "static_ae:auto_white_delay_frame", static_ae->auto_white_delay_frame, ot_u16);
    scene_load_int(module, "static_ae:auto_antiflicker_enable", static_ae->auto_antiflicker_enable, ot_bool);
    scene_load_int(module, "static_ae:hist_stat_adjust", static_ae->hist_stat_adjust, ot_bool);
    scene_load_int(module, "static_ae:auto_exp_time_min", static_ae->auto_exp_time_min, ot_u32);
    scene_load_int(module, "static_ae:frame_rate", static_ae->frame_rate, ot_u32);

    scene_load_int(module, "static_ae:deflicker_sensor_T", static_ae->deflicker_attr.deflicker_sensor_T, ot_u32);
    scene_load_int(module, "static_ae:deflicker_min_freq", static_ae->deflicker_attr.deflicker_min_freq, ot_u32);
    scene_load_int(module, "static_ae:deflicker_running_interval", static_ae->deflicker_attr.deflicker_running_interval, ot_u32);
    scene_load_int(module, "static_ae:deflicker_exposure_l_thresh", static_ae->deflicker_attr.deflicker_exposure_l_thresh, ot_u32);
    scene_load_int(module, "static_ae:deflicker_exposure_h_thresh", static_ae->deflicker_attr.deflicker_exposure_h_thresh, ot_u32);
    scene_load_int(module, "static_ae:deflicker_reset_ratio_en", static_ae->deflicker_attr.deflicker_reset_ratio_en, ot_u32);
    scene_load_int(module, "static_ae:deflicker_reset_ratio_u", static_ae->deflicker_attr.deflicker_reset_ratio_u, ot_u32);
    scene_load_int(module, "static_ae:deflicker_reset_ratio_d", static_ae->deflicker_attr.deflicker_reset_ratio_d, ot_u32);
    scene_load_int(module, "static_ae:deflicker_reset_filter", static_ae->deflicker_attr.deflicker_reset_filter, ot_u32);
    scene_load_int(module, "static_ae:a_gain_min", static_ae->a_gain_min, ot_u32);
    scene_load_int(module, "static_ae:a_gain_max", static_ae->a_gain_max, ot_u32);
    scene_load_int(module, "static_ae:d_gain_min", static_ae->d_gain_min, ot_u32);
    scene_load_int(module, "static_ae:d_gain_max", static_ae->d_gain_max, ot_u32);
    scene_load_int(module, "static_ae:isp_d_gain_min", static_ae->isp_d_gain_min, ot_u32);
    scene_load_int(module, "static_ae:isp_d_gain_max", static_ae->isp_d_gain_max, ot_u32);
    scene_load_int(module, "static_ae:compensation", static_ae->compensation, ot_u32);
    return OT_SUCCESS;
}

static ot_s32 scene_load_static_ae_route_ex(const ot_char *module, ot_scene_static_ae_route_ex *static_ae_route_ex)
{
    ot_scenecomm_check_pointer_return(static_ae_route_ex, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_ae_route_ex:total_num", static_ae_route_ex->total_num, ot_u32);

    scene_load_array(module, "static_ae_route_ex:int_time", static_ae_route_ex->int_time, static_ae_route_ex->total_num,
        ot_u32);

    scene_load_array(module, "static_ae_route_ex:again", static_ae_route_ex->again, static_ae_route_ex->total_num,
        ot_u32);

    scene_load_array(module, "static_ae_route_ex:dgain", static_ae_route_ex->dgain, static_ae_route_ex->total_num,
        ot_u32);

    scene_load_array(module, "static_ae_route_ex:isp_dgain", static_ae_route_ex->isp_dgain, static_ae_route_ex->total_num,
        ot_u32);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_back_light(const ot_char *module, ot_scene_static_back_light *static_back_light)
{
    ot_scenecomm_check_pointer_return(static_back_light, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_back_light:dev_num", static_back_light->dev_num, ot_u32);

    scene_load_array(module, "static_back_light:dev_id", static_back_light->dev_id, static_back_light->dev_num,
        ot_u32);

    scene_load_int(module, "static_back_light:backlight_base", static_back_light->backlight_base, ot_u32);
    scene_load_array(module, "static_back_light:backlight_array", static_back_light->backlight_array, static_back_light->dev_num,
        ot_u32);

    return OT_SUCCESS;
}

// static ot_s32 scene_load_static_gamma(const ot_char *module, ot_scene_static_gamma *static_gamma)
// {
//     ot_scenecomm_check_pointer_return(static_gamma, OT_FAILURE);
//     ot_s32 ret;
//     ot_s32 get_value;
//     ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

//     scene_load_int(module, "static_gamma:gamma_enable", static_gamma->gamma_enable, ot_bool);
//     scene_load_int(module, "static_gamma:curve_type", static_gamma->curve_type, ot_u8);
//     snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_gamma:gamma_table");
//     scene_load_array(module, node, static_gamma->gamma_table, OT_EIS_VI_GAMMA_NODE_NUM, ot_u16);
//     return OT_SUCCESS;
// }

static ot_s32 scene_load_static_wdr_exposure(const ot_char *module, ot_scene_static_wdr_exposure *static_wdr_exposure)
{
    ot_scenecomm_check_pointer_return(static_wdr_exposure, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_wdr_exposure:exp_ratio_type", static_wdr_exposure->exp_ratio_type, ot_u8);
    scene_load_int(module, "static_wdr_exposure:exp_ratio_max", static_wdr_exposure->exp_ratio_max, ot_u32);
    scene_load_int(module, "static_wdr_exposure:exp_ratio_min", static_wdr_exposure->exp_ratio_min, ot_u32);

    scene_load_array(module, "static_wdr_exposure:exp_ratio", static_wdr_exposure->exp_ratio, OT_EIS_VI_EXP_RATIO_NUM,
        ot_u32);

    scene_load_int(module, "static_wdr_exposure:tolerance", static_wdr_exposure->tolerance, ot_u16);
    scene_load_int(module, "static_wdr_exposure:ref_ratio_up", static_wdr_exposure->ref_ratio_up, ot_u32);
    scene_load_int(module, "static_wdr_exposure:ref_ratio_dn", static_wdr_exposure->ref_ratio_dn, ot_u32);
    scene_load_int(module, "static_wdr_exposure:exp_thr", static_wdr_exposure->exp_thr, ot_u32);

    scene_load_int(module, "static_wdr_exposure:high_light_target", static_wdr_exposure->high_light_target, ot_u32);
    scene_load_int(module, "static_wdr_exposure:exp_coef_min", static_wdr_exposure->exp_coef_min, ot_u32);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_fswdr(const ot_char *module, ot_scene_static_fswdr *static_fs_wdr)
{
    ot_scenecomm_check_pointer_return(static_fs_wdr, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_wdr:wdr_merge_mode", static_fs_wdr->wdr_merge_mode, ot_u16);
    scene_load_int(module, "static_wdr:fusion_blend_en", static_fs_wdr->fusion_blend_en, ot_bool);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_awb(const ot_char *module, ot_scene_static_awb *static_awb)
{
    ot_scenecomm_check_pointer_return(static_awb, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_array(module, "static_awb:auto_static_wb", static_awb->auto_static_wb, OT_EIS_VI_BAYER_CHN_NUM, ot_u16);

    scene_load_array(module, "static_awb:auto_curve_para", static_awb->auto_curve_para, OT_EIS_VI_AWB_CURVE_PARA_NUM,
        ot_s32);

    scene_load_int(module, "static_awb:op_type", static_awb->op_type, ot_u8);
    scene_load_int(module, "static_awb:manual_rgain", static_awb->manual_rgain, ot_u16);
    scene_load_int(module, "static_awb:manual_gbgain", static_awb->manual_gbgain, ot_u16);
    scene_load_int(module, "static_awb:manual_grgain", static_awb->manual_grgain, ot_u16);
    scene_load_int(module, "static_awb:manual_bgain", static_awb->manual_bgain, ot_u16);
    scene_load_int(module, "static_awb:auto_speed", static_awb->auto_speed, ot_u16);
    scene_load_int(module, "static_awb:auto_low_color_temp", static_awb->auto_low_color_temp, ot_u16);

    scene_load_array(module, "static_awb:auto_cr_max", static_awb->auto_cr_max, OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_awb:auto_cr_min", static_awb->auto_cr_min, OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_awb:auto_cb_max", static_awb->auto_cb_max, OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_awb:auto_cb_min", static_awb->auto_cb_min, OT_EIS_VI_AUTO_ISO_NUM, ot_u16);

    scene_load_int(module, "static_awb:luma_hist_enable", static_awb->luma_hist_enable, ot_u16);
    scene_load_int(module, "static_awb:luma_hist_op_type", static_awb->luma_hist_op_type, ot_u16);
    scene_load_int(module, "static_awb:awb_switch", static_awb->awb_switch, ot_u16);
    scene_load_int(module, "static_awb:black_level", static_awb->black_level, ot_u16);
    scene_load_int(module, "static_awb:auto_high_color_temp", static_awb->auto_high_color_temp, ot_u16);

    scene_load_array(module, "static_awb:luma_hist_thresh", static_awb->luma_hist_thresh, OT_EIS_VI_AWB_LUM_HIST_NUM, ot_u16);
    scene_load_array(module, "static_awb:luma_hist_wt", static_awb->hist_wt, OT_EIS_VI_AWB_LUM_HIST_NUM, ot_u16);

    scene_load_int(module, "static_awb:awb_run_interval", static_awb->awb_run_interval, ot_u8);
    scene_load_int(module, "static_awb:ct_limit_enable", static_awb->ct_limit_enable, ot_bool);
    scene_load_int(module, "static_awb:ct_limit_op_type", static_awb->ct_limit_op_type, ot_u8);
    scene_load_int(module, "static_awb:high_rg_limit", static_awb->high_rg_limit, ot_u16);
    scene_load_int(module, "static_awb:high_bg_limit", static_awb->high_bg_limit, ot_u16);
    scene_load_int(module, "static_awb:low_rg_limit", static_awb->low_rg_limit, ot_u16);
    scene_load_int(module, "static_awb:low_bg_limit", static_awb->low_bg_limit, ot_u16);
    scene_load_int(module, "static_awb:shift_limit", static_awb->shift_limit, ot_u8);

    scene_load_int(module, "static_awb:night_exp_thresh", static_awb->night_exp_thresh, ot_u32);
    scene_load_int(module, "static_awb:skin_office_exp_thresh", static_awb->skin_office_exp_thresh, ot_u32);
    scene_load_int(module, "static_awb:out_switch_luma_thresh", static_awb->out_switch_luma_thresh, ot_u16);
    scene_load_int(module, "static_awb:out_switch_lowct_num", static_awb->out_switch_lowct_num, ot_u16);

    scene_load_int(module, "static_awb:weight_enable", static_awb->weight_enable, ot_bool);
    scene_load_int(module, "static_awb:weight_strength", static_awb->weight_strength, ot_u8);

    scene_load_int(module, "static_awb:rp_enable", static_awb->rp_enable, ot_bool);
    scene_load_array(module, "static_awb:rp_valid", static_awb->rp_valid, OT_EIS_VI_AWB_RP_LS_NUM, ot_u8);
    scene_load_array(module, "static_awb:rp_tgt_rgain", static_awb->rp_tgt_rgain, OT_EIS_VI_AWB_RP_LS_NUM, ot_u16);
    scene_load_array(module, "static_awb:rp_tgt_bgain", static_awb->rp_tgt_bgain, OT_EIS_VI_AWB_RP_LS_NUM, ot_u16);
    scene_load_array(module, "static_awb:rp_ls_radius", static_awb->rp_ls_radius, OT_EIS_VI_AWB_RP_LS_NUM, ot_u8);
    scene_load_array(module, "static_awb:rp_zone_rgain", static_awb->rp_zone_rgain, OT_EIS_VI_AWB_RP_LS_NUM, ot_u16);
    scene_load_array(module, "static_awb:rp_zone_bgain", static_awb->rp_zone_bgain, OT_EIS_VI_AWB_RP_LS_NUM, ot_u16);
    scene_load_array(module, "static_awb:rp_boundry_ratio", static_awb->rp_boundry_ratio, OT_EIS_VI_AWB_RP_LS_NUM, ot_u8);
    scene_load_array(module, "static_awb:rp_exp_range_left", static_awb->rp_exp_range_left, OT_EIS_VI_AWB_RP_LS_NUM, ot_u32);
    scene_load_array(module, "static_awb:rp_exp_range_right", static_awb->rp_exp_range_right, OT_EIS_VI_AWB_RP_LS_NUM, ot_u32);
    scene_load_array(module, "static_awb:rp_ct_range_left", static_awb->rp_ct_range_left, OT_EIS_VI_AWB_RP_LS_NUM, ot_u16);
    scene_load_array(module, "static_awb:rp_ct_range_right", static_awb->rp_ct_range_right, OT_EIS_VI_AWB_RP_LS_NUM, ot_u16);
    return OT_SUCCESS;
}

static ot_s32 scene_load_static_awb_ex(const ot_char *module, ot_scene_static_awb_ex *static_awb_ex)
{
    ot_scenecomm_check_pointer_return(static_awb_ex, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_awb_ex:bypass", static_awb_ex->bypass, ot_bool);
    scene_load_int(module, "static_awb_ex:tolerance", static_awb_ex->tolerance, ot_u8);
    scene_load_int(module, "static_awb_ex:in_or_out_enable", static_awb_ex->in_or_out_enable, ot_u8);
    scene_load_int(module, "static_awb_ex:in_or_out_op_type", static_awb_ex->in_or_out_op_type, ot_u8);
    scene_load_int(module, "static_awb_ex:in_or_out_outdoor_status", static_awb_ex->in_or_out_outdoor_status, ot_u8);
    scene_load_int(module, "static_awb_ex:in_or_out_green_enhance_en", static_awb_ex->in_or_out_green_enhance_en, ot_u8);
    scene_load_int(module, "static_awb_ex:out_shift_limit", static_awb_ex->out_shift_limit, ot_u8);
    scene_load_int(module, "static_awb_ex:out_thresh", static_awb_ex->out_thresh, ot_u32);
	scene_load_int(module, "static_awb_ex:low_start", static_awb_ex->low_start, ot_u16);
    scene_load_int(module, "static_awb_ex:low_stop", static_awb_ex->low_stop, ot_u16);
    scene_load_int(module, "static_awb_ex:high_start", static_awb_ex->high_start, ot_u16);
    scene_load_int(module, "static_awb_ex:high_stop", static_awb_ex->high_stop, ot_u16);
    scene_load_int(module, "static_awb_ex:multi_light_source_en", static_awb_ex->multi_light_source_en, ot_u16);

    scene_load_array(module, "static_awb_ex:multi_ctwt", static_awb_ex->multi_ctwt, OT_EIS_VI_AWB_MULTI_CT_NUM, ot_u16);

    scene_load_int(module, "static_awb_ex:zone_radius", static_awb_ex->zone_radius, ot_u8);
    scene_load_int(module, "static_awb_ex:curve_l_limit", static_awb_ex->curve_l_limit, ot_u16);
    scene_load_int(module, "static_awb_ex:curve_r_limit", static_awb_ex->curve_r_limit, ot_u16);
    scene_load_int(module, "static_awb_ex:extra_light_en", static_awb_ex->extra_light_en, ot_bool);

    scene_load_array(module, "static_awb_ex:white_r_gain", static_awb_ex->white_r_gain, OT_EIS_VI_AWB_LS_NUM, ot_u16);
    scene_load_array(module, "static_awb_ex:white_b_gain", static_awb_ex->white_b_gain, OT_EIS_VI_AWB_LS_NUM, ot_u16);
    scene_load_array(module, "static_awb_ex:exp_quant", static_awb_ex->exp_quant, OT_EIS_VI_AWB_LS_NUM, ot_u16);
    scene_load_array(module, "static_awb_ex:light_status", static_awb_ex->light_status, OT_EIS_VI_AWB_LS_NUM, ot_u8);
    scene_load_array(module, "static_awb_ex:radius", static_awb_ex->radius, OT_EIS_VI_AWB_LS_NUM, ot_u8);

    scene_load_int(module, "static_awb_ex:fine_tun_en", static_awb_ex->fine_tun_en, ot_bool);
    scene_load_int(module, "static_awb_ex:fine_tun_strength", static_awb_ex->fine_tun_strength, ot_u8);

    return OT_SUCCESS;
}


static ot_s32 scene_load_static_cmm(const ot_char *module, ot_scene_static_ccm *static_ccm)
{
    ot_scenecomm_check_pointer_return(static_ccm, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;
    ot_u32 i;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = {0};

    scene_load_int(module, "static_ccm:ccm_op_type", static_ccm->ccm_op_type, ot_u8);

    scene_load_array(module, "static_ccm:manual_ccm", static_ccm->manual_ccm, OT_EIS_VI_CCM_MATRIX_SIZE, ot_u16);

    scene_load_int(module, "static_ccm:auto_iso_act_en", static_ccm->auto_iso_act_en, ot_u32);
    scene_load_int(module, "static_ccm:auto_temp_act_en", static_ccm->auto_temp_act_en, ot_u32);
    scene_load_int(module, "static_ccm:total_num", static_ccm->total_num, ot_u32);

    scene_load_array(module, "static_ccm:auto_color_temp", static_ccm->auto_color_temp, static_ccm->total_num, ot_u16);

    for (i = 0; i < static_ccm->total_num; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_ccm:auto_ccm_%u", i);
        scene_load_array(module, node, static_ccm->auto_ccm[i], OT_EIS_VI_CCM_MATRIX_SIZE, ot_u16);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_saturation(const ot_char *module, ot_scene_static_saturation *static_saturation)
{
    ot_scenecomm_check_pointer_return(static_saturation, OT_FAILURE);
    scene_load_array(module, "static_saturation:auto_sat", static_saturation->auto_sat, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    return OT_SUCCESS;
}

static ot_s32 scene_read_clut_table(ot_eis_scene_clut_table *clut, const ot_char *path)
{
    ot_s32 ret;
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        scene_loge("open static clut table failed:%s\n", path);
        return OT_FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    unsigned int table_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char *table_data = (unsigned char *)malloc(table_len);
    if (table_data == NULL) {
        fclose(fp);
        scene_loge("read static clut table failed, malloc failed\n");
        return OT_FAILURE;
    }

    fread(table_data, table_len, 1, fp);
    fclose(fp);

    ret = ot_eis_scene_clut_load_table(clut, table_data, table_len);
    if (ret != OT_SUCCESS) {
        free(table_data);
        return OT_FAILURE;
    }

    free(table_data);
    return OT_SUCCESS;
}

static ot_s32 scene_load_static_clut(const ot_char *module, ot_scene_static_clut *static_clut)
{
    ot_s32 ret;
    ot_s32 get_value;
    char table_path[SCENETOOL_MAX_FILESIZE] = {0};
    char filename[SCENETOOL_MAX_FILESIZE] = {0};

    ot_scenecomm_check_pointer_return(static_clut, OT_FAILURE);

    scene_load_int(module, "static_clut:enable", static_clut->enable, ot_bool);
    scene_load_int(module, "static_clut:lut_grp", static_clut->lut_grp, ot_u8);

    if (static_clut->enable) {
        scene_load_str(module, "static_clut:clut_table_1", filename, SCENETOOL_MAX_FILESIZE);
        snprintf_truncated_s(table_path, SCENETOOL_MAX_FILESIZE, "%s/%s/%s", g_ini_dir, g_relative_path, filename);
        ret = scene_read_clut_table(&static_clut->clut_table[0], table_path);
        ot_scenecomm_check_return(ret, OT_FAILURE);

        scene_load_str(module, "static_clut:clut_table_2", filename, SCENETOOL_MAX_FILESIZE);
        snprintf_truncated_s(table_path, SCENETOOL_MAX_FILESIZE, "%s/%s/%s", g_ini_dir, g_relative_path, filename);
        ret = scene_read_clut_table(&static_clut->clut_table[1], table_path);
        ot_scenecomm_check_return(ret, OT_FAILURE);

        scene_load_str(module, "static_clut:clut_table_3", filename, SCENETOOL_MAX_FILESIZE);
        snprintf_truncated_s(table_path, SCENETOOL_MAX_FILESIZE, "%s/%s/%s", g_ini_dir, g_relative_path, filename);
        ret = scene_read_clut_table(&static_clut->clut_table[2], table_path);
        ot_scenecomm_check_return(ret, OT_FAILURE);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_ldci(const ot_char *module, ot_scene_static_ldci *static_ldci)
{
    ot_scenecomm_check_pointer_return(static_ldci, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_ldci:enable", static_ldci->enable, ot_bool);
    scene_load_int(module, "static_ldci:ldci_op_type", static_ldci->ldci_op_type, ot_u8);
    scene_load_int(module, "static_ldci:gauss_lpf_sigma", static_ldci->gauss_lpf_sigma, ot_u8);
    scene_load_int(module, "static_ldci:tpr_incr_coef", static_ldci->tpr_incr_coef, ot_u8);
    scene_load_int(module, "static_ldci:tpr_decr_coef", static_ldci->tpr_decr_coef, ot_u8);

    scene_load_array(module, "static_ldci:auto_he_pos_wgt", static_ldci->auto_he_pos_wgt, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);

    scene_load_array(module, "static_ldci:auto_he_pos_sigma", static_ldci->auto_he_pos_sigma, OT_EIS_VI_AUTO_ISO_NUM,
        ot_u8);

    scene_load_array(module, "static_ldci:auto_he_pos_mean", static_ldci->auto_he_pos_mean, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);

    scene_load_array(module, "static_ldci:auto_he_neg_wgt", static_ldci->auto_he_neg_wgt, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);

    scene_load_array(module, "static_ldci:auto_he_neg_sigma", static_ldci->auto_he_neg_sigma, OT_EIS_VI_AUTO_ISO_NUM,
        ot_u8);

    scene_load_array(module, "static_ldci:auto_he_neg_mean", static_ldci->auto_he_neg_mean, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);

    scene_load_array(module, "static_ldci:auto_blc_ctrl", static_ldci->auto_blc_ctrl, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_ldcihlc(const ot_char *module, ot_scene_static_ldcihlc *static_ldcihlc)
{
    ot_scenecomm_check_pointer_return(static_ldcihlc, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;
    scene_load_int(module, "static_ldcihlc:enable", static_ldcihlc->enable, ot_bool);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_drc(const ot_char *module, ot_scene_static_drc *static_drc)
{
    ot_scenecomm_check_pointer_return(static_drc, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_drc:enable", static_drc->enable, ot_bool);
    scene_load_int(module, "static_drc:curve_select", static_drc->curve_select, ot_eis_vi_drc_curve_select);
    scene_load_int(module, "static_drc:op_type", static_drc->op_type, ot_eis_op_mode);
    scene_load_int(module, "static_drc:purple_reduction_strength", static_drc->purple_reduction_strength, ot_u8);
    scene_load_int(module, "static_drc:contrast_ctrl", static_drc->contrast_ctrl, ot_u8);
    scene_load_int(module, "static_drc:blend_luma_max", static_drc->blend_luma_max, ot_u8);
    scene_load_int(module, "static_drc:blend_luma_bright_min", static_drc->blend_luma_bright_min, ot_u8);
    scene_load_int(module, "static_drc:blend_luma_bright_threshold", static_drc->blend_luma_bright_threshold, ot_u8);
    scene_load_int(module, "static_drc:blend_luma_dark_min", static_drc->blend_luma_dark_min, ot_u8);
    scene_load_int(module, "static_drc:blend_luma_dark_threshold", static_drc->blend_luma_dark_threshold, ot_u8);
    scene_load_int(module, "static_drc:blend_detail_max", static_drc->blend_detail_max, ot_u8);
    scene_load_int(module, "static_drc:blend_detail_bright_min", static_drc->blend_detail_bright_min, ot_u8);
    scene_load_int(module, "static_drc:blend_detail_bright_threshold", static_drc->blend_detail_bright_threshold, ot_u8);
    scene_load_int(module, "static_drc:blend_detail_dark_min", static_drc->blend_detail_dark_min, ot_u8);
    scene_load_int(module, "static_drc:blend_detail_dark_threshold", static_drc->blend_detail_dark_threshold, ot_u8);
    scene_load_int(module, "static_drc:global_color_ctrl", static_drc->global_color_ctrl, ot_u8);
    scene_load_array(module, "static_drc:color_correction_lut", static_drc->color_correction_lut,
        OT_EIS_VI_DRC_CC_NODE_NUM, ot_u16);
    return OT_SUCCESS;
}


static ot_s32 scene_load_static_post_snr_arrays(const ot_char *module, ot_scene_static_nr *static_nr)
{
    scene_load_array(module, "static_nr:dering_strength", static_nr->dering_snr_cfg.dering_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_nr:dering_thresh", static_nr->dering_snr_cfg.dering_thresh,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:dering_static_strength", static_nr->dering_snr_cfg.dering_static_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:dering_motion_strength", static_nr->dering_snr_cfg.dering_motion_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_nr_arrays(const ot_char *module, ot_scene_static_nr *static_nr)
{
    ot_s32 ret;
    scene_load_array(module, "static_nr:sfm6_strength", static_nr->snr_cfg.sfm6_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:sfm7_strength", static_nr->snr_cfg.sfm7_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:sth", static_nr->snr_cfg.sth,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:sfm1_adp_strength", static_nr->snr_cfg.sfm1_adp_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:sfm1_strength", static_nr->snr_cfg.sfm1_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_nr:sfm0_detail_prot", static_nr->snr_cfg.sfm0_detail_prot,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:fine_strength", static_nr->snr_cfg.fine_strength, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:coring_wgt", static_nr->snr_cfg.coring_wgt, OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_nr:coring_mot_ratio", static_nr->snr_cfg.coring_mot_ratio,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:md_mode", static_nr->tnr_cfg.md_mode, OT_EIS_VI_AUTO_ISO_NUM, ot_bool);
    scene_load_array(module, "static_nr:md_anti_flicker_strength", static_nr->tnr_cfg.md_anti_flicker_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:md_static_ratio", static_nr->tnr_cfg.md_static_ratio,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:md_motion_ratio", static_nr->tnr_cfg.md_motion_ratio,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:md_static_fine_strength", static_nr->tnr_cfg.md_static_fine_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:tnr_tfs", static_nr->tnr_cfg.tfs, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:user_define_md", static_nr->tnr_cfg.user_define_md, OT_EIS_VI_AUTO_ISO_NUM,
        ot_bool);
    scene_load_array(module, "static_nr:user_define_slope", static_nr->tnr_cfg.user_define_slope,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_nr:user_define_dark_thresh", static_nr->tnr_cfg.user_define_dark_thresh,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_nr:user_define_color_thresh", static_nr->tnr_cfg.user_define_color_thresh,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:sfr_r", static_nr->tnr_cfg.sfr_r, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:sfr_g", static_nr->tnr_cfg.sfr_g, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_nr:sfr_b", static_nr->tnr_cfg.sfr_b, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);

    ret = scene_load_static_post_snr_arrays(module, static_nr);
    ot_scenecomm_check_return(ret, ret);

    return OT_SUCCESS;
}
static ot_s32 scene_load_static_nr(const ot_char *module, ot_scene_static_nr *static_nr)
{
    ot_scenecomm_check_pointer_return(static_nr, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;
    ot_u32 i;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = {0};

    scene_load_int(module, "static_nr:enable", static_nr->enable, ot_bool);
    scene_load_int(module, "static_nr:op_type", static_nr->op_type, ot_eis_op_mode);
    scene_load_int(module, "static_nr:tnr_enable", static_nr->tnr_enable, ot_bool);
    scene_load_int(module, "static_nr:lsc_nr_enable", static_nr->lsc_nr_enable, ot_bool);
    scene_load_int(module, "static_nr:lsc_ratio1", static_nr->lsc_ratio1, ot_bool);

    scene_load_array(module, "static_nr:coring_ratio", static_nr->coring_ratio, OT_EIS_VI_BAYERNR_LUT_LENGTH, ot_u16);
    scene_load_array(module, "static_nr:mix_gain", static_nr->mix_gain, OT_EIS_VI_BAYERNR_LUT_LENGTH1, ot_u16);

    for (i = 0; i < OT_EIS_VI_BAYER_CHN_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_nr:sfm0_coarse_strength_%u", i);
        scene_load_array(module, node, static_nr->snr_cfg.sfm0_coarse_strength[i], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    }

    ret = scene_load_static_nr_arrays(module, static_nr);
    ot_scenecomm_check_return(ret, ret);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_ca(const ot_char *module, ot_scene_static_ca *static_ca)
{
    ot_scenecomm_check_pointer_return(static_ca, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_ca:enable", static_ca->enable, ot_bool);

    scene_load_array(module, "static_ca:iso_ratio", static_ca->iso_ratio, ISP_AUTO_ISO_CA_NUM, ot_u16);
    scene_load_array(module, "static_ca:y_ratio_lut", static_ca->y_ratio_lut, OT_EIS_VI_CA_YRATIO_LUT_LENGTH, ot_u32);
    scene_load_array(module, "static_ca:y_sat_lut", static_ca->y_sat_lut, OT_EIS_VI_CA_YRATIO_LUT_LENGTH, ot_u32);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_cac(const ot_char *module, ot_scene_static_cac *static_cac)
{
    ot_scenecomm_check_pointer_return(static_cac, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_cac:enable", static_cac->enable, ot_bool);
    scene_load_int(module, "static_cac:op_type", static_cac->op_type, ot_eis_op_mode);
    scene_load_int(module, "static_cac:detect_mode", static_cac->detect_mode, ot_bool);
    scene_load_int(module, "static_cac:purple_upper_limit", static_cac->purple_upper_limit, ot_s16);
    scene_load_int(module, "static_cac:purple_lower_limit", static_cac->purple_lower_limit, ot_s16);
    scene_load_int(module, "static_cac:purple_detect_range", static_cac->purple_detect_range, ot_u16);
    scene_load_int(module, "static_cac:var_threshold", static_cac->var_threshold, ot_u16);

    scene_load_array(module, "static_cac:edge_threshold_0", static_cac->edge_threshold[0],
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_cac:edge_threshold_1", static_cac->edge_threshold[1],
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_cac:edge_gain", static_cac->edge_gain,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_cac:cac_rb_strength", static_cac->cac_rb_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_cac:purple_alpha", static_cac->purple_alpha,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_cac:edge_alpha", static_cac->edge_alpha,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_cac:satu_low_threshold", static_cac->satu_low_threshold,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_cac:satu_high_threshold", static_cac->satu_high_threshold,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);

    scene_load_array(module, "static_cac:de_purple_cr_strength",
        static_cac->de_purple_cr_strength, OT_EIS_VI_CAC_EXP_RATIO_NUM, ot_u8);
    scene_load_array(module, "static_cac:de_purple_cb_strength",
        static_cac->de_purple_cb_strength, OT_EIS_VI_CAC_EXP_RATIO_NUM, ot_u8);
    scene_load_array(module, "static_cac:r_detect_threshold", static_cac->r_detect_threshold,
        OT_EIS_VI_CAC_CURVE_NUM, ot_u16);
    scene_load_array(module, "static_cac:g_detect_threshold", static_cac->g_detect_threshold,
        OT_EIS_VI_CAC_CURVE_NUM, ot_u16);
    scene_load_array(module, "static_cac:b_detect_threshold", static_cac->b_detect_threshold,
        OT_EIS_VI_CAC_CURVE_NUM, ot_u16);

    return OT_SUCCESS;
}


static ot_s32 scene_load_static_dpc(const ot_char *module, ot_scene_static_dpc *static_dpc)
{
    ot_scenecomm_check_pointer_return(static_dpc, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_dpc:enable", static_dpc->enable, ot_bool);

    scene_load_array(module, "static_dpc:op_type", static_dpc->op_type, OT_EIS_VI_WDR_MAX_FRAME_NUM, ot_eis_op_mode);
    scene_load_array(module, "static_dpc:strength_0", static_dpc->strength[0], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_dpc:blend_ratio_0", static_dpc->blend_ratio[0], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_dpc:strength_1", static_dpc->strength[1], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_dpc:blend_ratio_1", static_dpc->blend_ratio[1], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_dpc:strength_2", static_dpc->strength[2], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_dpc:blend_ratio_2", static_dpc->blend_ratio[2], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_dpc:strength_3", static_dpc->strength[3], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_dpc:blend_ratio_3", static_dpc->blend_ratio[3], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_dehaze(const ot_char *module, ot_scene_static_dehaze *static_de_haze)
{
    ot_scenecomm_check_pointer_return(static_de_haze, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_dehaze:enable", static_de_haze->enable, ot_bool);
    scene_load_int(module, "static_dehaze:dehaze_op_type", static_de_haze->dehaze_op_type, ot_u8);
    scene_load_int(module, "static_dehaze:user_lut_enable", static_de_haze->user_lut_enable, ot_bool);
    scene_load_int(module, "static_dehaze:tmprflt_incr_coef", static_de_haze->tmprflt_incr_coef, ot_bool);
    scene_load_int(module, "static_dehaze:tmprflt_decr_coef", static_de_haze->tmprflt_decr_coef, ot_bool);

    scene_load_array(module, "static_dehaze:dehaze_lut", static_de_haze->dehaze_lut, OT_EIS_VI_DEHAZE_LUT_SIZE, ot_u8);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_shading(const ot_char *module, ot_scene_static_shading *static_shading)
{
    ot_scenecomm_check_pointer_return(static_shading, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_shading:enable", static_shading->enable, ot_bool);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_csc(const ot_char *module, ot_scene_static_csc *static_csc)
{
    ot_scenecomm_check_pointer_return(static_csc, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_csc:enable", static_csc->enable, ot_bool);
    scene_load_int(module, "static_csc:hue", static_csc->hue, ot_u8);
    scene_load_int(module, "static_csc:luma", static_csc->luma, ot_u8);
    scene_load_int(module, "static_csc:contrast", static_csc->contrast, ot_u8);
    scene_load_int(module, "static_csc:saturation", static_csc->saturation, ot_u8);
    scene_load_int(module, "static_csc:color_gamut", static_csc->color_gamut, ot_eis_img_color_space);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_cross_talk(const ot_char *module, ot_scene_static_crosstalk *static_cross_talk)
{
    ot_scenecomm_check_pointer_return(static_cross_talk, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_crosstalk:enable", static_cross_talk->enable, ot_bool);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_demosaic(const ot_char *module, ot_scene_static_demosaic *static_demosaic)
{
    ot_scenecomm_check_pointer_return(static_demosaic, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_dm:enable", static_demosaic->enable, ot_bool);
    scene_load_int(module, "static_dm:op_type", static_demosaic->op_type, ot_eis_op_mode);

    scene_load_array(module, "static_dm:nddm_strength", static_demosaic->dm_auto_cfg.nddm_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_dm:nddm_mf_detail_strength", static_demosaic->dm_auto_cfg.nddm_mf_detail_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_dm:nddm_hf_detail_strength", static_demosaic->dm_auto_cfg.nddm_hf_detail_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_dm:detail_smooth_range", static_demosaic->dm_auto_cfg.detail_smooth_range,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_dm:color_noise_f_threshold", static_demosaic->dm_auto_cfg.color_noise_f_threshold,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_dm:color_noise_f_strength", static_demosaic->dm_auto_cfg.color_noise_f_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_dm:color_noise_y_threshold", static_demosaic->dm_auto_cfg.color_noise_y_threshold,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_dm:color_noise_y_strength", static_demosaic->dm_auto_cfg.color_noise_y_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_bayershp(const ot_char *module, ot_scene_static_bayershp *static_bayershp)
{
    ot_scenecomm_check_pointer_return(static_bayershp, OT_FAILURE);
    ot_s32 ret;
    ot_u32 i;
    ot_s32 get_value;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

    scene_load_int(module, "static_bayer_sharpen:enable", static_bayershp->enable, ot_bool);
    scene_load_int(module, "static_bayer_sharpen:op_type", static_bayershp->op_type, ot_eis_op_mode);

    scene_load_array(module, "static_bayer_sharpen:dark_threshold",
        static_bayershp->dark_threshold, OT_EIS_VI_BSHP_THD_NUM, ot_u16);
    scene_load_array(module, "static_bayer_sharpen:texture_threshold",
        static_bayershp->texture_threshold, OT_EIS_VI_BSHP_THD_NUM, ot_u16);

    for (i = 0; i < OT_SCENE_BSHP_CURVE_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_bayer_sharpen:mf_strength_%u", i);
        scene_load_array(module, node, static_bayershp->bayershp_auto_attr.mf_strength[i],
            OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    }

    for (i = 0; i < OT_SCENE_BSHP_CURVE_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_bayer_sharpen:hf_strength_%u", i);
        scene_load_array(module, node, static_bayershp->bayershp_auto_attr.hf_strength[i],
            OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    }

    for (i = 0; i < OT_SCENE_BSHP_CURVE_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_bayer_sharpen:dark_strength_%u", i);
        scene_load_array(module, node, static_bayershp->bayershp_auto_attr.dark_strength[i],
            OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    }

    scene_load_array(module, "static_bayer_sharpen:mf_gain",
        static_bayershp->bayershp_auto_attr.mf_gain, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_bayer_sharpen:hf_gain",
        static_bayershp->bayershp_auto_attr.hf_gain, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_bayer_sharpen:dark_gain", static_bayershp->bayershp_auto_attr.dark_gain,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_bayer_sharpen:overshoot", static_bayershp->bayershp_auto_attr.overshoot,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_bayer_sharpen:undershoot", static_bayershp->bayershp_auto_attr.undershoot,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);

    return OT_SUCCESS;
}


static ot_s32 scene_load_static_sharpen_arrays(const ot_char *module, ot_scene_static_sharpen *static_sharpen)
{
    scene_load_array(module, "static_sharpen:texture_freq", static_sharpen->sharpen_auto_cfg.texture_freq,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_sharpen:edge_freq", static_sharpen->sharpen_auto_cfg.edge_freq,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "static_sharpen:over_shoot", static_sharpen->sharpen_auto_cfg.over_shoot,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:under_shoot", static_sharpen->sharpen_auto_cfg.under_shoot,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:shoot_sup_strength", static_sharpen->sharpen_auto_cfg.shoot_sup_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:shoot_sup_adj", static_sharpen->sharpen_auto_cfg.shoot_sup_adj,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:detail_ctrl", static_sharpen->sharpen_auto_cfg.detail_ctrl,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:detail_ctrl_threshold",
        static_sharpen->sharpen_auto_cfg.detail_ctrl_threshold, OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:edge_filt_strength", static_sharpen->sharpen_auto_cfg.edge_filt_strength,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:edge_filt_max_cap", static_sharpen->sharpen_auto_cfg.edge_filt_max_cap,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:r_gain", static_sharpen->sharpen_auto_cfg.r_gain,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:g_gain", static_sharpen->sharpen_auto_cfg.g_gain,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:b_gain", static_sharpen->sharpen_auto_cfg.b_gain,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:skin_gain", static_sharpen->sharpen_auto_cfg.skin_gain,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    scene_load_array(module, "static_sharpen:max_sharp_gain", static_sharpen->sharpen_auto_cfg.max_sharp_gain,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u16);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_sharpen(const ot_char *module, ot_scene_static_sharpen *static_sharpen)
{
    ot_scenecomm_check_pointer_return(static_sharpen, OT_FAILURE);
    ot_s32 ret;
    ot_u32 i;
    ot_s32 get_value;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

    scene_load_int(module, "static_sharpen:enable", static_sharpen->enable, ot_bool);
    scene_load_int(module, "static_sharpen:skin_umin", static_sharpen->skin_umin, ot_u8);
    scene_load_int(module, "static_sharpen:skin_vmin", static_sharpen->skin_vmin, ot_u8);
    scene_load_int(module, "static_sharpen:skin_umax", static_sharpen->skin_umax, ot_u8);
    scene_load_int(module, "static_sharpen:skin_vmax", static_sharpen->skin_vmax, ot_u8);

    for (i = 0; i < OT_EIS_VI_SHARPEN_GAIN_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_sharpen:luma_wgt_%u", i);
        scene_load_array(module, node, static_sharpen->sharpen_auto_cfg.luma_wgt[i], OT_EIS_VI_AUTO_ISO_NUM, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_SHARPEN_GAIN_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_sharpen:texture_strength_%u", i);
        scene_load_array(module, node, static_sharpen->sharpen_auto_cfg.texture_strength[i],
            OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    }

    for (i = 0; i < OT_EIS_VI_SHARPEN_GAIN_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_sharpen:edge_strength_%u", i);
        scene_load_array(module, node, static_sharpen->sharpen_auto_cfg.edge_strength[i], OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    }

    ret = scene_load_static_sharpen_arrays(module, static_sharpen);
    ot_scenecomm_check_return(ret, ret);

    return OT_SUCCESS;
}

static ot_s32 scene_load_static_blc(const ot_char *module, ot_scene_static_blc *static_blc)
{
    ot_scenecomm_check_pointer_return(static_blc, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "static_blc:enable", static_blc->enable, ot_bool);
    scene_load_int(module, "static_blc:user_offset", static_blc->user_offset, ot_u16);

    return OT_SUCCESS;
}


static ot_s32 scene_load_dynamic_vproc(const ot_char *module, ot_scene_dynamic_vproc *dynamic_vproc)
{
    ot_scenecomm_check_pointer_return(dynamic_vproc, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_vproc:crop_ae_enable", dynamic_vproc->crop_ae_enable, ot_bool);
    scene_load_int(module, "dynamic_vproc:crop_ae_enable_once", dynamic_vproc->crop_ae_enable_once, ot_bool);
    scene_load_int(module, "dynamic_vproc:ae_weight_inside", dynamic_vproc->ae_weight_inside, ot_u8);
    scene_load_int(module, "dynamic_vproc:ae_weight_outside", dynamic_vproc->ae_weight_outside, ot_u8);
    scene_load_int(module, "dynamic_vproc:crop_awb_enable", dynamic_vproc->crop_awb_enable, ot_bool);
    scene_load_int(module, "dynamic_vproc:crop_awb_enable_once", dynamic_vproc->crop_awb_enable_once, ot_bool);
    scene_load_int(module, "dynamic_vproc:awb_weight_inside", dynamic_vproc->awb_weight_inside, ot_u8);
    scene_load_int(module, "dynamic_vproc:awb_weight_outside", dynamic_vproc->awb_weight_outside, ot_u8);

    return OT_SUCCESS;
}
static ot_s32 scene_load_dynamic_ae(const ot_char *module, ot_scene_dynamic_ae *dynamic_ae)
{
    ot_scenecomm_check_pointer_return(dynamic_ae, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_ae:ae_exposure_cnt", dynamic_ae->ae_exposure_cnt, ot_u8);

    scene_load_array(module, "dynamic_ae:exp_ltoh_thresh", dynamic_ae->exp_ltoh_thresh,
        dynamic_ae->ae_exposure_cnt, ot_u64);
    scene_load_array(module, "dynamic_ae:exp_htol_thresh", dynamic_ae->exp_htol_thresh,
        dynamic_ae->ae_exposure_cnt, ot_u64);
    scene_load_array(module, "dynamic_ae:auto_compensation", dynamic_ae->auto_compensation,
        dynamic_ae->ae_exposure_cnt, ot_u8);
    scene_load_array(module, "dynamic_ae:auto_max_hist_offset", dynamic_ae->auto_max_hist_offset,
        dynamic_ae->ae_exposure_cnt, ot_u8);
    scene_load_array(module, "dynamic_ae:auto_exp_ratio_max", dynamic_ae->auto_exp_ratio_max,
        dynamic_ae->ae_exposure_cnt, ot_u32);
    scene_load_array(module, "dynamic_ae:auto_exp_ratio_min", dynamic_ae->auto_exp_ratio_min,
        dynamic_ae->ae_exposure_cnt, ot_u32);

    scene_load_int(module, "dynamic_ae:wdr_ratio_threshold", dynamic_ae->wdr_ratio_threshold, ot_u32);
    scene_load_int(module, "dynamic_ae:l_advance_ae", dynamic_ae->l_advance_ae, ot_bool);
    scene_load_int(module, "dynamic_ae:h_advance_ae", dynamic_ae->h_advance_ae, ot_bool);

    return OT_SUCCESS;
}


static ot_s32 scene_load_dynamic_fps(const ot_char *module, ot_scene_dynamic_fps *dynamic_fps)
{
    ot_scenecomm_check_pointer_return(dynamic_fps, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_fps:fps_exposure_cnt", dynamic_fps->fps_exposure_cnt, ot_u8);

    scene_load_array(module, "dynamic_fps:exp_ltoh_thresh", dynamic_fps->exp_ltoh_thresh,
        dynamic_fps->fps_exposure_cnt, ot_u64);
    scene_load_array(module, "dynamic_fps:exp_htol_thresh", dynamic_fps->exp_htol_thresh,
        dynamic_fps->fps_exposure_cnt, ot_u64);

    scene_load_array(module, "dynamic_fps:fps_set", dynamic_fps->fps_set,
        dynamic_fps->fps_exposure_cnt, ot_u64);

    return OT_SUCCESS;
}


static ot_s32 scene_load_dynamic_dehaze(const ot_char *module, ot_scene_dynamic_dehaze *dynamic_dehaze)
{
    ot_scenecomm_check_pointer_return(dynamic_dehaze, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_dehaze:dehzae_level_enable", dynamic_dehaze->dehzae_level_enable, ot_u8);
    scene_load_int(module, "dynamic_dehaze:exp_thresh_cnt", dynamic_dehaze->exp_thresh_cnt, ot_u32);
    scene_load_int(module, "dynamic_dehaze:dehaze_level_cnt", dynamic_dehaze->dehaze_level_cnt, ot_u16);
    scene_load_int(module, "dynamic_dehaze:dehaze_level_print", dynamic_dehaze->dehaze_level_print, ot_u8);

    scene_load_array(module, "dynamic_dehaze:exp_thresh_ltoh", dynamic_dehaze->exp_thresh_ltoh,
        dynamic_dehaze->exp_thresh_cnt, ot_u64);
    scene_load_array(module, "dynamic_dehaze:manual_strength", dynamic_dehaze->manual_strength,
        dynamic_dehaze->exp_thresh_cnt, ot_u8);

    scene_load_int(module, "dynamic_dehaze:wdr_ratio_threshold", dynamic_dehaze->wdr_ratio_threshold, ot_u32);
    scene_load_array(module, "dynamic_dehaze:manual_strengther", dynamic_dehaze->manual_strengther,
        dynamic_dehaze->exp_thresh_cnt, ot_u8);

    scene_load_array(module, "dynamic_dehaze:dehaze_level_threshold", dynamic_dehaze->dehaze_level_threshold,
        dynamic_dehaze->dehaze_level_cnt, ot_u64);
    scene_load_array(module, "dynamic_dehaze:dehaze_level_strength", dynamic_dehaze->dehaze_level_strength,
        dynamic_dehaze->dehaze_level_cnt, ot_u64);

    scene_load_int(module, "dynamic_dehaze:dehaze_lut_tune_en", dynamic_dehaze->dehaze_lut_tune_en, ot_u8);
    scene_load_int(module, "dynamic_dehaze:dehaze_lut_luma_cnt", dynamic_dehaze->dehaze_lut_luma_cnt, ot_u8);
    scene_load_array(module, "dynamic_dehaze:dehaze_lut_luma_thr", dynamic_dehaze->dehaze_lut_luma_thr,
        dynamic_dehaze->dehaze_lut_luma_cnt, ot_u64);
    scene_load_array(module, "dynamic_dehaze:dehaze_lut_1", dynamic_dehaze->dehaze_lut_1, OT_EIS_VI_DEHAZE_LUT_SIZE, ot_u8);
    scene_load_array(module, "dynamic_dehaze:dehaze_lut_2", dynamic_dehaze->dehaze_lut_2, OT_EIS_VI_DEHAZE_LUT_SIZE, ot_u8);
    scene_load_array(module, "dynamic_dehaze:dehaze_lut_3", dynamic_dehaze->dehaze_lut_3, OT_EIS_VI_DEHAZE_LUT_SIZE, ot_u8);
    scene_load_array(module, "dynamic_dehaze:dehaze_lut_4", dynamic_dehaze->dehaze_lut_4, OT_EIS_VI_DEHAZE_LUT_SIZE, ot_u8);

    return OT_SUCCESS;
}


static ot_s32 scene_load_dynamic_drc_arrays(const ot_char *module, ot_scene_dynamic_drc *dynamic_drc)
{
    ot_u32 i;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

    for (i = 0; i < OT_EIS_VI_DRC_LMIX_NODE_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_drc:local_mixing_bright_%u", i);
        scene_load_array(module, node, dynamic_drc->local_mixing_bright[i], dynamic_drc->iso_count, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_drc:local_mixing_dark_%u", i);
        scene_load_array(module, node, dynamic_drc->local_mixing_dark[i], dynamic_drc->iso_count, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_drc:color_correction_lut_%u", i);
        scene_load_array(module, node, dynamic_drc->color_correction_lut[i], dynamic_drc->ratio_count, ot_u16);
    }

    scene_load_array(module, "dynamic_drc:tone_mapping_wgt_x", dynamic_drc->tone_mapping_wgt_x,
        dynamic_drc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_drc:spatial_filter_coef", dynamic_drc->spatial_filter_coef,
        dynamic_drc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_drc:range_filter_coef", dynamic_drc->range_filter_coef,
        dynamic_drc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_drc:detail_adjust_coef", dynamic_drc->detail_adjust_coef,
        dynamic_drc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_drc:rim_reduction_strength", dynamic_drc->rim_reduction_strength,
        dynamic_drc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_drc:rim_reduction_threshold", dynamic_drc->rim_reduction_threshold,
        dynamic_drc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_drc:dark_gain_limit_luma", dynamic_drc->dark_gain_limit_luma,
        dynamic_drc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_drc:dark_gain_limit_chroma", dynamic_drc->dark_gain_limit_chroma,
        dynamic_drc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_drc:global_color_ctrl", dynamic_drc->global_color_ctrl,
        dynamic_drc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_drc:shoot_reduction_en", dynamic_drc->shoot_reduction_en,
        dynamic_drc->iso_count, ot_bool);

    scene_load_array(module, "dynamic_drc:manual_str", dynamic_drc->manual_str, dynamic_drc->iso_count, ot_u16);
    scene_load_array(module, "dynamic_drc:tm_value_low_adj",   dynamic_drc->tm_value_low_adj,  OT_EIS_VI_DRC_TM_NODE_NUM, ot_u16);
    scene_load_array(module, "dynamic_drc:tm_value_low",   dynamic_drc->tm_value_low,  OT_EIS_VI_DRC_TM_NODE_NUM, ot_u16);
    scene_load_array(module, "dynamic_drc:tm_value_high",  dynamic_drc->tm_value_high, OT_EIS_VI_DRC_TM_NODE_NUM, ot_u16);
    scene_load_array(module, "dynamic_drc:tm_val_higher",  dynamic_drc->tm_val_higher, OT_EIS_VI_DRC_TM_NODE_NUM, ot_u16);
    scene_load_array(module, "dynamic_drc:tm_val_highest", dynamic_drc->tm_val_highest, OT_EIS_VI_DRC_TM_NODE_NUM, ot_u16);
    scene_load_array(module, "dynamic_drc:tm_val_highest_adj", dynamic_drc->tm_val_highest_adj, OT_EIS_VI_DRC_TM_NODE_NUM, ot_u16);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_drc(const ot_char *module, const ot_scene_module_state *module_state,
    ot_scene_dynamic_drc *dynamic_drc)
{
    ot_scenecomm_check_pointer_return(dynamic_drc, OT_FAILURE);
    if (module_state->dynamic_drc == 0) {
        return OT_SUCCESS;
    }
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_drc:ratio_count", dynamic_drc->ratio_count, ot_u32);
    scene_load_array(module, "dynamic_drc:ratio_level", dynamic_drc->ratio_level, dynamic_drc->ratio_count, ot_u32);
    scene_load_array(module, "dynamic_drc:high_saturation_color_ctrl", dynamic_drc->high_saturation_color_ctrl,
        dynamic_drc->ratio_count, ot_u8);

    scene_load_array(module, "dynamic_drc:bright_gain_limit", dynamic_drc->bright_gain_limit,
        dynamic_drc->ratio_count, ot_u8);
    scene_load_array(module, "dynamic_drc:bright_gain_limit_step", dynamic_drc->bright_gain_limit_step,
        dynamic_drc->ratio_count, ot_u8);

    scene_load_int(module, "dynamic_drc:ref_ratio_count", dynamic_drc->ref_ratio_count, ot_u32);

    scene_load_array(module, "dynamic_drc:ref_ratio_ltoh", dynamic_drc->ref_ratio_ltoh,
        dynamic_drc->ref_ratio_count, ot_u32);
    scene_load_array(module, "dynamic_drc:ref_ratio_alpha", dynamic_drc->ref_ratio_alpha,
        dynamic_drc->ref_ratio_count, ot_u32);

    scene_load_int(module, "dynamic_drc:tm_ratio_threshold", dynamic_drc->tm_ratio_threshold, ot_u32);

    scene_load_int(module, "dynamic_drc:iso_count", dynamic_drc->iso_count, ot_u32);

    scene_load_array(module, "dynamic_drc:iso_level", dynamic_drc->iso_level, dynamic_drc->iso_count, ot_u32);

    scene_load_int(module, "dynamic_drc:interval", dynamic_drc->interval, ot_u32);
    scene_load_int(module, "dynamic_drc:curve_speed_p3", dynamic_drc->curve_speed_p3, ot_u32);
    scene_load_int(module, "dynamic_drc:curve_speed_p4", dynamic_drc->curve_speed_p4, ot_u32);
    scene_load_int(module, "dynamic_drc:enable", dynamic_drc->enable, ot_bool);

    scene_load_int(module, "dynamic_drc:curve_index_count", dynamic_drc->curve_index_count, ot_u32);
    scene_load_array(module, "dynamic_drc:curve_index_ltoh", dynamic_drc->curve_index_ltoh,
        dynamic_drc->curve_index_count, ot_u32);

    scene_load_int(module, "dynamic_drc:min_exposure", dynamic_drc->drc_adj.min_exposure, ot_u32);
    scene_load_int(module, "dynamic_drc:low_exp_adj_count", dynamic_drc->drc_adj.low_exp_adj_count, ot_u32);
    scene_load_array(module, "dynamic_drc:low_exp_adj_ltoh", dynamic_drc->drc_adj.low_exp_adj_ltoh,
        dynamic_drc->drc_adj.low_exp_adj_count, ot_u32);
    scene_load_array(module, "dynamic_drc:low_exp_adj_ratio", dynamic_drc->drc_adj.low_exp_adj_ratio,
        dynamic_drc->drc_adj.low_exp_adj_count, ot_u32);

    scene_load_int(module, "dynamic_drc:max_exposure", dynamic_drc->drc_adj.max_exposure, ot_u32);
    scene_load_int(module, "dynamic_drc:high_exp_adj_count", dynamic_drc->drc_adj.high_exp_adj_count, ot_u32);
    scene_load_array(module, "dynamic_drc:high_exp_adj_ltoh", dynamic_drc->drc_adj.high_exp_adj_ltoh,
        dynamic_drc->drc_adj.high_exp_adj_count, ot_u32);
    scene_load_array(module, "dynamic_drc:high_exp_adj_ratio", dynamic_drc->drc_adj.high_exp_adj_ratio,
        dynamic_drc->drc_adj.high_exp_adj_count, ot_u32);

    ret = scene_load_dynamic_drc_arrays(module, dynamic_drc);
    ot_scenecomm_check_return(ret, ret);

    return OT_SUCCESS;
}


static ot_s32 scene_load_dynamic_gamma(const ot_char *module, ot_scene_dynamic_gamma *dynamic_gamma)
{
    ot_scenecomm_check_pointer_return(dynamic_gamma, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;
    ot_u32 i;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

    scene_load_int(module, "dynamic_gamma:total_num", dynamic_gamma->total_num, ot_u32);
    scene_load_int(module, "dynamic_gamma:interval", dynamic_gamma->interval, ot_u32);

    scene_load_array(module, "dynamic_gamma:exp_thresh_ltoh", dynamic_gamma->exp_thresh_ltoh,
        dynamic_gamma->total_num, ot_u64);

    scene_load_array(module, "dynamic_gamma:exp_thresh_htol", dynamic_gamma->exp_thresh_htol,
        dynamic_gamma->total_num, ot_u64);

    /* table */
    for (i = 0; i < dynamic_gamma->total_num; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_gamma:table_%u", i);
        scene_load_array(module, node, dynamic_gamma->table[i], OT_EIS_VI_GAMMA_NODE_NUM, ot_u16);
    }
    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_nr_arrays_part1(const ot_char *module, ot_scene_dynamic_nr *dynamic_nr)
{
    ot_u32 i;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

    for (i = 0; i < OT_SCENE_NR_LUT_LENGTH1; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:mix_gain_%u", i);
        scene_load_array(module, node, dynamic_nr->mix_gain[i], dynamic_nr->ratio_count, ot_u16);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:snr_sfm0_wdr_frame_str_%u", i);
        scene_load_array(module, node, dynamic_nr->snr_sfm0_wdr_frame_str[i], dynamic_nr->coring_ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:snr_sfm0_fusion_frame_str_%u", i);
        scene_load_array(module, node, dynamic_nr->snr_sfm0_fusion_frame_str[i], dynamic_nr->coring_ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:md_wdr_frame_str_%u", i);
        scene_load_array(module, node, dynamic_nr->md_wdr_frame_str[i], dynamic_nr->coring_ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:md_fusion_frame_str_%u", i);
        scene_load_array(module, node, dynamic_nr->md_fusion_frame_str[i], dynamic_nr->coring_ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:snr_wdr_sfm6_strength_%u", i);
        scene_load_array(module, node, dynamic_nr->snr_wdr_sfm6_strength[i], dynamic_nr->coring_ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:snr_wdr_sfm7_strength_%u", i);
        scene_load_array(module, node, dynamic_nr->snr_wdr_sfm7_strength[i], dynamic_nr->coring_ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:snr_fusion_sfm6_strength_%u", i);
        scene_load_array(module, node, dynamic_nr->snr_fusion_sfm6_strength[i], dynamic_nr->coring_ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:snr_fusion_sfm7_strength_%u", i);
        scene_load_array(module, node, dynamic_nr->snr_fusion_sfm7_strength[i], dynamic_nr->coring_ratio_count, ot_u8);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_nr_arrays(const ot_char *module, ot_scene_dynamic_nr *dynamic_nr)
{
    ot_u32 i;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:md_fusion_frame_str_incr_%u", i);
        scene_load_array(module, node, dynamic_nr->md_fusion_frame_str_incr[i], dynamic_nr->ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:snr_sfm0_fusion_frame_str_incr_%u", i);
        scene_load_array(module, node, dynamic_nr->snr_sfm0_fusion_frame_str_incr[i], dynamic_nr->ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:md_wdr_frame_str_incr_%u", i);
        scene_load_array(module, node, dynamic_nr->md_wdr_frame_str_incr[i], dynamic_nr->ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:snr_sfm0_wdr_frame_str_incr_%u", i);
        scene_load_array(module, node, dynamic_nr->snr_sfm0_wdr_frame_str_incr[i], dynamic_nr->ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN,
            "dynamic_nr:snr_fusion_sfm6_sfm7_frame_incr_%u", i);
        scene_load_array(module, node,
        dynamic_nr->snr_fusion_sfm6_sfm7_frame_incr[i], dynamic_nr->ratio_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:snr_wdr_sfm6_sfm7_frame_incr_%u", i);
        scene_load_array(module, node, dynamic_nr->snr_wdr_sfm6_sfm7_frame_incr[i], dynamic_nr->ratio_count, ot_u8);
    }

    for (i = 0; i < OT_SCENE_NR_LUT_LENGTH; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_nr:coring_ratio_%u", i);
        scene_load_array(module, node, dynamic_nr->coring_ratio[i], dynamic_nr->coring_ratio_count, ot_u16);
    }

    scene_load_dynamic_nr_arrays_part1(module, dynamic_nr);

    return OT_SUCCESS;
}
static ot_s32 scene_load_dynamic_nr(const ot_char *module, ot_scene_dynamic_nr *dynamic_nr)
{
    ot_scenecomm_check_pointer_return(dynamic_nr, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_nr:coring_ratio_count", dynamic_nr->coring_ratio_count, ot_u32);
    scene_load_array(module, "dynamic_nr:coring_ratio_iso", dynamic_nr->coring_ratio_iso,
                     dynamic_nr->coring_ratio_count, ot_u32);
    scene_load_int(module, "dynamic_nr:ratio_count", dynamic_nr->ratio_count, ot_u32);
    scene_load_array(module, "dynamic_nr:ratio_level", dynamic_nr->ratio_level, dynamic_nr->ratio_count, ot_u32);
    ret = scene_load_dynamic_nr_arrays(module, dynamic_nr);
    ot_scenecomm_check_return(ret, ret);

    scene_load_int(module, "dynamic_nr:wdr_ratio_threshold", dynamic_nr->wdr_ratio_threshold, ot_u16);

    scene_load_array(module, "dynamic_nr:coring_wgt_l", dynamic_nr->coring_wgt_l, OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "dynamic_nr:coring_wgt_h", dynamic_nr->coring_wgt_h, OT_EIS_VI_AUTO_ISO_NUM, ot_u16);

    scene_load_array(module, "dynamic_nr:fine_strength_l", dynamic_nr->fine_strength_l, OT_EIS_VI_AUTO_ISO_NUM, ot_u16);
    scene_load_array(module, "dynamic_nr:fine_strength_h", dynamic_nr->fine_strength_h, OT_EIS_VI_AUTO_ISO_NUM, ot_u16);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_back_light(const ot_char *module, ot_scene_dynamic_back_light *dynamic_back_light)
{
    ot_scenecomm_check_pointer_return(dynamic_back_light, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_back_light:enable", dynamic_back_light->enable, ot_bool);
    scene_load_int(module, "dynamic_back_light:backlight_base", dynamic_back_light->backlight_base, ot_u32);
    scene_load_int(module, "dynamic_back_light:array_cnt", dynamic_back_light->array_cnt, ot_u32);
    scene_load_int(module, "dynamic_back_light:backlight_min", dynamic_back_light->backlight_min, ot_u32);
    scene_load_int(module, "dynamic_back_light:backlight_max", dynamic_back_light->backlight_max, ot_u32);
    scene_load_int(module, "dynamic_back_light:backlight_mode", dynamic_back_light->backlight_mode, ot_u32);

    scene_load_array(module, "dynamic_back_light:light_array", dynamic_back_light->light_array,
                     dynamic_back_light->array_cnt, ot_u32);
    scene_load_array(module, "dynamic_back_light:backlight_array_light", dynamic_back_light->backlight_array_light,
                     dynamic_back_light->array_cnt, ot_u32);

    scene_load_array(module, "dynamic_back_light:iso_array", dynamic_back_light->iso_array,
                     dynamic_back_light->array_cnt, ot_u32);
    scene_load_array(module, "dynamic_back_light:backlight_array_iso", dynamic_back_light->backlight_array_iso,
                     dynamic_back_light->array_cnt, ot_u32);

    scene_load_int(module, "dynamic_back_light:backlight_tolerance", dynamic_back_light->backlight_tolerance, ot_u32);

    scene_load_int(module, "dynamic_back_light:backlight_conv_ratio_inc", dynamic_back_light->backlight_conv_ratio_inc, ot_u32);
    scene_load_int(module, "dynamic_back_light:backlight_conv_ratio_dec", dynamic_back_light->backlight_conv_ratio_dec, ot_u32);
    scene_load_int(module, "dynamic_back_light:dev_num", dynamic_back_light->dev_num, ot_u32);
    scene_load_array(module, "dynamic_back_light:dev_id", dynamic_back_light->dev_id,
                     dynamic_back_light->dev_num, ot_u32);

    scene_load_int(module, "dynamic_back_light:light_sensor_num", dynamic_back_light->light_sensor_num, ot_u32);
    scene_load_array(module, "dynamic_back_light:src_addr", dynamic_back_light->src_addr,
                     dynamic_back_light->light_sensor_num, ot_u32);
    scene_load_array(module, "dynamic_back_light:dst_addr", dynamic_back_light->dst_addr,
                     dynamic_back_light->light_sensor_num, ot_u32);

    scene_load_int(module, "dynamic_back_light:info_print", dynamic_back_light->info_print, ot_bool);

    scene_load_int(module, "dynamic_back_light:img_bright_enable", dynamic_back_light->img_bright_enable, ot_bool);
    scene_load_int(module, "dynamic_back_light:img_array_cnt", dynamic_back_light->img_array_cnt, ot_u32);
    scene_load_array(module, "dynamic_back_light:img_light_array", dynamic_back_light->img_light_array,
                     dynamic_back_light->img_array_cnt, ot_u32);
    scene_load_array(module, "dynamic_back_light:img_bright_array", dynamic_back_light->img_bright_array,
                     dynamic_back_light->img_array_cnt, ot_u32);

    scene_load_array(module, "dynamic_back_light:img_bright_base",   dynamic_back_light->img_bright_base,  OT_EIS_VI_DRC_TM_NODE_NUM, ot_u16);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_ca(const ot_char *module, ot_scene_dynamic_ca *dynamic_ca)
{
    ot_scenecomm_check_pointer_return(dynamic_ca, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;
    ot_u32 i;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

    scene_load_int(module, "dynamic_ca:iso_count", dynamic_ca->iso_count, ot_u32);
    scene_load_array(module, "dynamic_ca:iso_level", dynamic_ca->iso_level, dynamic_ca->iso_count, ot_u32);

    for (i = 0; i < dynamic_ca->iso_count; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_ca:ca_y_ratio_lut_iso_%u", i);
        scene_load_array(module, node, dynamic_ca->ca_y_ratio_iso_lut[i], OT_EIS_VI_CA_YRATIO_LUT_LENGTH, ot_u32);
    }

    scene_load_int(module, "dynamic_ca:ratio_count", dynamic_ca->ratio_count, ot_u32);
    scene_load_array(module, "dynamic_ca:ratio_level", dynamic_ca->ratio_level, dynamic_ca->ratio_count, ot_u32);
    scene_load_array(module, "dynamic_ca:blend_weight", dynamic_ca->blend_weight, dynamic_ca->ratio_count, ot_u32);

    for (i = 0; i < dynamic_ca->ratio_count; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_ca:ca_y_ratio_lut_ratio_%u", i);
        scene_load_array(module, node, dynamic_ca->ca_y_ratio_ratio_lut[i], OT_EIS_VI_CA_YRATIO_LUT_LENGTH, ot_u32);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_blc(const ot_char *module, ot_scene_dynamic_blc *dynamic_blc)
{
    ot_scenecomm_check_pointer_return(dynamic_blc, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_blc:black_level_mode", dynamic_blc->black_level_mode, ot_u8);
    scene_load_int(module, "dynamic_blc:blc_count", dynamic_blc->blc_count, ot_u32);
    scene_load_array(module, "dynamic_blc:iso_thresh", dynamic_blc->iso_thresh, dynamic_blc->blc_count, ot_u32);

    /* 4 channels value R Gr Gb B */
    scene_load_array(module, "dynamic_blc:blc_r", dynamic_blc->blc_r, dynamic_blc->blc_count, ot_u32);
    scene_load_array(module, "dynamic_blc:blc_gr", dynamic_blc->blc_gr, dynamic_blc->blc_count, ot_u32);
    scene_load_array(module, "dynamic_blc:blc_gb", dynamic_blc->blc_gb, dynamic_blc->blc_count, ot_u32);
    scene_load_array(module, "dynamic_blc:blc_b", dynamic_blc->blc_b, dynamic_blc->blc_count, ot_u32);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_awb(const ot_char *module, ot_scene_dynamic_awb *dynamic_awb)
{
    ot_scenecomm_check_pointer_return(dynamic_awb, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_awb:iso_count", dynamic_awb->iso_count, ot_u8);
    scene_load_array(module, "dynamic_awb:iso_level", dynamic_awb->iso_level, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:in_out_mode", dynamic_awb->in_out_op_mode, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:in_out_status", dynamic_awb->manual_in_out_status, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:auto_low_color_temp", dynamic_awb->auto_low_color_temp, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:auto_high_color_temp", dynamic_awb->auto_high_color_temp, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:low_start", dynamic_awb->low_start, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:low_stop", dynamic_awb->low_stop, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:high_start", dynamic_awb->high_start, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:high_stop", dynamic_awb->high_stop, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:focus_en", dynamic_awb->focus_en, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:zone_radius", dynamic_awb->zone_radius, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:shift_limit", dynamic_awb->shift_limit, dynamic_awb->iso_count, ot_u32);
    scene_load_array(module, "dynamic_awb:out_shift_limit", dynamic_awb->out_shift_limit, dynamic_awb->iso_count, ot_u32);

    scene_load_array(module, "dynamic_awb:bd_ct_th_indoor", dynamic_awb->bd_ct_th_indoor, dynamic_awb->iso_count, ot_u16);
    scene_load_array(module, "dynamic_awb:bd_ct_th_outdoor", dynamic_awb->bd_ct_th_outdoor, dynamic_awb->iso_count, ot_u16);
    scene_load_array(module, "dynamic_awb:bm_ctn_ct_th", dynamic_awb->bm_ctn_ct_th, dynamic_awb->iso_count, ot_u16);
    scene_load_array(module, "dynamic_awb:bm_au_out_num", dynamic_awb->bm_au_out_num, dynamic_awb->iso_count, ot_u16);
    scene_load_array(module, "dynamic_awb:bm_sft", dynamic_awb->bm_sft, dynamic_awb->iso_count, ot_u8);

    return OT_SUCCESS;
}
static ot_s32 scene_load_dynamic_dpc(const ot_char *module, ot_scene_dynamic_dpc *dynamic_dpc)
{
    ot_scenecomm_check_pointer_return(dynamic_dpc, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;
    ot_u32 i;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

    scene_load_int(module, "dynamic_dpc:iso_count", dynamic_dpc->iso_count, ot_u8);
    scene_load_array(module, "dynamic_dpc:iso_level", dynamic_dpc->iso_level, dynamic_dpc->iso_count, ot_u32);

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_dpc:sup_twinkle_en_%u", i);
        scene_load_array(module, node, dynamic_dpc->sup_twinkle_en[i], dynamic_dpc->iso_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_dpc:soft_thr_%u", i);
        scene_load_array(module, node, dynamic_dpc->soft_thr[i], dynamic_dpc->iso_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_dpc:soft_slope_%u", i);
        scene_load_array(module, node, dynamic_dpc->soft_slope[i], dynamic_dpc->iso_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_dpc:bright_strength_%u", i);
        scene_load_array(module, node, dynamic_dpc->bright_strength[i], dynamic_dpc->iso_count, ot_u8);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "dynamic_dpc:dark_strength_%u", i);
        scene_load_array(module, node, dynamic_dpc->dark_strength[i], dynamic_dpc->iso_count, ot_u8);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_fswdr(const ot_char *module, ot_scene_dynamic_fswdr *dynamic_fswdr)
{
    ot_scenecomm_check_pointer_return(dynamic_fswdr, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_wdr:ratio_count", dynamic_fswdr->ratio_count, ot_u8);
    scene_load_array(module, "dynamic_wdr:ratio_level",
        dynamic_fswdr->ratio_level, dynamic_fswdr->ratio_count, ot_u32);
    scene_load_array(module, "dynamic_wdr:wdr_merge_mode",
        dynamic_fswdr->wdr_merge_mode, dynamic_fswdr->ratio_count, ot_u8);
    scene_load_array(module, "dynamic_wdr:motion_comp",
        dynamic_fswdr->motion_comp, dynamic_fswdr->ratio_count, ot_u8);

    scene_load_int(module, "dynamic_wdr:iso_count", dynamic_fswdr->iso_count, ot_u8);
    scene_load_int(module, "dynamic_wdr:ratio_thr", dynamic_fswdr->ratio_thr, ot_u32);
    scene_load_array(module, "dynamic_wdr:iso_level",
        dynamic_fswdr->iso_ltoh_thresh, dynamic_fswdr->iso_count, ot_u32);
    scene_load_array(module, "dynamic_wdr:fusion_blend_en",
        dynamic_fswdr->fusion_blend_en, dynamic_fswdr->iso_count, ot_bool);
    scene_load_array(module, "dynamic_wdr:fusion_blend_wgt",
        dynamic_fswdr->fusion_blend_wgt, dynamic_fswdr->iso_count, ot_u8);
    scene_load_array(module, "dynamic_wdr:fusion_blend_en_h",
        dynamic_fswdr->fusion_blend_en_h, dynamic_fswdr->iso_count, ot_bool);
    scene_load_array(module, "dynamic_wdr:fusion_blend_wgt_h",
        dynamic_fswdr->fusion_blend_wgt_h, dynamic_fswdr->iso_count, ot_u8);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_shading(const ot_char *module, ot_scene_dynamic_shading *dynamic_shading)
{
    ot_scenecomm_check_pointer_return(dynamic_shading, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_shading:exp_thresh_cnt", dynamic_shading->exp_thresh_cnt, ot_u32);

    scene_load_array(module, "dynamic_shading:exp_thresh_ltoh", dynamic_shading->exp_thresh_ltoh,
        dynamic_shading->exp_thresh_cnt, ot_u64);

    scene_load_array(module, "dynamic_shading:mesh_strength", dynamic_shading->mesh_strength,
        dynamic_shading->exp_thresh_cnt, ot_u16);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_clut(const ot_char *module, ot_scene_dynamic_clut *dynamic_clut)
{
    ot_scenecomm_check_pointer_return(dynamic_clut, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;
    char table_path[SCENETOOL_MAX_FILESIZE] = {0};
    char filename[SCENETOOL_MAX_FILESIZE] = {0};
    ot_bool dynamic_clut_en = OT_FALSE;

    scene_load_int(module, "dynamic_clut:iso_count", dynamic_clut->iso_count, ot_u32);

    scene_load_array(module, "dynamic_clut:iso_level", dynamic_clut->iso_level, dynamic_clut->iso_count, ot_u32);
    scene_load_array(module, "dynamic_clut:gain_r", dynamic_clut->gain_r, dynamic_clut->iso_count, ot_u32);
    scene_load_array(module, "dynamic_clut:gain_g", dynamic_clut->gain_g, dynamic_clut->iso_count, ot_u32);
    scene_load_array(module, "dynamic_clut:gain_b", dynamic_clut->gain_b, dynamic_clut->iso_count, ot_u32);

    scene_load_int(module, "module_state:dynamic_clut", dynamic_clut_en, ot_bool);
    if (dynamic_clut_en) {
        scene_load_str(module, "dynamic_clut:clut_table_1", filename, SCENETOOL_MAX_FILESIZE);
        snprintf_truncated_s(table_path, SCENETOOL_MAX_FILESIZE, "%s/%s/%s", g_ini_dir, g_relative_path, filename);
        ret = scene_read_clut_table(&dynamic_clut->clut_table[0], table_path);
        ot_scenecomm_check_return(ret, OT_FAILURE);

        dynamic_clut->gain_r[0] = dynamic_clut->clut_table[0].gain[0];
        dynamic_clut->gain_g[0] = dynamic_clut->clut_table[0].gain[1];
        dynamic_clut->gain_b[0] = dynamic_clut->clut_table[0].gain[2];

        scene_load_str(module, "dynamic_clut:clut_table_2", filename, SCENETOOL_MAX_FILESIZE);
        snprintf_truncated_s(table_path, SCENETOOL_MAX_FILESIZE, "%s/%s/%s", g_ini_dir, g_relative_path, filename);
        ret = scene_read_clut_table(&dynamic_clut->clut_table[1], table_path);
        ot_scenecomm_check_return(ret, OT_FAILURE);

        dynamic_clut->gain_r[1] = dynamic_clut->clut_table[1].gain[0];
        dynamic_clut->gain_g[1] = dynamic_clut->clut_table[1].gain[1];
        dynamic_clut->gain_b[1] = dynamic_clut->clut_table[1].gain[2];
    }

    return OT_SUCCESS;
}


static ot_s32 scene_load_dynamic_ldci(const ot_char *module, ot_scene_dynamic_ldci *dynamic_ldci)
{
    ot_scenecomm_check_pointer_return(dynamic_ldci, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_ldci:enable_cnt", dynamic_ldci->enable_cnt, ot_u32);

    scene_load_array(module, "dynamic_ldci:enable_exp_thresh_ltoh", dynamic_ldci->enable_exp_thresh_ltoh,
        dynamic_ldci->enable_cnt, ot_u64);
    scene_load_array(module, "dynamic_ldci:enable", dynamic_ldci->enable, dynamic_ldci->enable_cnt, ot_u8);

    scene_load_int(module, "dynamic_ldci:exp_thresh_cnt", dynamic_ldci->exp_thresh_cnt, ot_u32);

    scene_load_array(module, "dynamic_ldci:exp_thresh_ltoh", dynamic_ldci->exp_thresh_ltoh,
        dynamic_ldci->exp_thresh_cnt, ot_u64);
    scene_load_array(module, "dynamic_ldci:manual_ldci_he_pos_wgt", dynamic_ldci->manual_ldci_he_pos_wgt,
        dynamic_ldci->exp_thresh_cnt, ot_u8);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_ldcihlc(const ot_char *module, ot_scene_dynamic_ldcihlc *dynamic_ldcihlc)
{
    ot_scenecomm_check_pointer_return(dynamic_ldcihlc, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_ldcihlc:threshold", dynamic_ldcihlc->threshold, ot_u8);
    scene_load_int(module, "dynamic_ldcihlc:op_type", dynamic_ldcihlc->op_type, ot_u8);
    scene_load_int(module, "dynamic_ldcihlc:strength_manual", dynamic_ldcihlc->strength_manual, ot_u8);
    scene_load_array(module, "dynamic_ldcihlc:strength_auto", dynamic_ldcihlc->strength_auto,
        OT_EIS_VI_AUTO_ISO_NUM, ot_u8);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_abnr(const ot_char *module, ot_scene_dynamic_abnr *dynamic_abnr)
{
    ot_scenecomm_check_pointer_return(dynamic_abnr, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_abnr:abnr_thresh_cnt", dynamic_abnr->abnr_thresh_cnt, ot_u32);
    scene_load_array(module, "dynamic_abnr:abnr_thresh", dynamic_abnr->abnr_thresh,
        dynamic_abnr->abnr_thresh_cnt, ot_u32);
    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_iqt(const ot_char *module, ot_scene_dynamic_iqt *dynamic_iqt)
{
    ot_scenecomm_check_pointer_return(dynamic_iqt, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_iqt:mode", dynamic_iqt->iqt_attr.mode, ot_u8);
    scene_load_int(module, "dynamic_iqt:manual_enable", dynamic_iqt->iqt_attr.manual_enable, ot_bool);
    scene_load_int(module, "dynamic_iqt:ynr_manual_strength", dynamic_iqt->iqt_attr.ynr_manual_strength, ot_u8);
    scene_load_int(module, "dynamic_iqt:cnr_manual_strength", dynamic_iqt->iqt_attr.cnr_manual_strength, ot_u8);
    scene_load_int(module, "dynamic_iqt:dfl_manual_strength", dynamic_iqt->iqt_attr.dfl_manual_strength, ot_u8);
    scene_load_array(module, "dynamic_iqt:eco_thresh", dynamic_iqt->iqt_attr.eco_thresholds, OT_EIS_SCENE_IQT_ECO_THRES_SIZE, ot_u32);
    scene_load_array(module, "dynamic_iqt:nr_thresh", dynamic_iqt->iqt_attr.nr_attr.thresholds, OT_EIS_SCENE_IQT_NR_THR_SIZE, ot_u32);
    scene_load_int(module, "dynamic_iqt:dfl_strength", dynamic_iqt->iqt_attr.dfl_attr.strength, ot_u8);
    scene_load_int(module, "dynamic_iqt:dfl_start_th", dynamic_iqt->iqt_attr.dfl_attr.start_th, ot_u16);
    scene_load_array(module, "dynamic_iqt:dfl_exposure_th", dynamic_iqt->iqt_attr.dfl_attr.exposure_th, OT_EIS_SCENE_IQT_DFL_EXP_THRES_SIZE, ot_u32);
    scene_load_array(module, "dynamic_iqt:dfl_avglight_th", dynamic_iqt->iqt_attr.dfl_attr.avglight_th, OT_EIS_SCENE_IQT_DFL_AVG_THRES_SIZE, ot_u32);
    scene_load_array(module, "dynamic_iqt:dfl_blending_weight", dynamic_iqt->iqt_attr.dfl_attr.blending_weight, OT_EIS_SCENE_IQT_DFL_BLD_WEIGHT_SIZE, ot_u16);
    scene_load_array(module, "dynamic_iqt:dfl_hist_refer", dynamic_iqt->iqt_attr.dfl_attr.hist_refer, OT_EIS_SCENE_IQT_DFL_HIST_THR_SIZE, ot_u32);
    scene_load_array(module, "dynamic_iqt:dfl_hist_delta", dynamic_iqt->iqt_attr.dfl_attr.hist_delta, OT_EIS_SCENE_IQT_DFL_HIST_THR_SIZE, ot_u32);
    scene_load_array(module, "dynamic_iqt:dfl_hist_weight", dynamic_iqt->iqt_attr.dfl_attr.hist_weight, OT_EIS_SCENE_IQT_DFL_HIST_THR_SIZE, ot_u16);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_mce(const ot_char *module, ot_scene_dynamic_mce *dynamic_mce)
{
    ot_scenecomm_check_pointer_return(dynamic_mce, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_mce:mce_iso_thresh", dynamic_mce->mce_iso_thresh, ot_u32);

    return OT_SUCCESS;
}

static ot_s32 scene_load_dynamic_false_color(const ot_char *module, ot_scene_dynamic_false_color *dynamic_false_color)
{
    ot_scenecomm_check_pointer_return(dynamic_false_color, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_falsecolor:total_num", dynamic_false_color->total_num, ot_u32);

    scene_load_array(module, "dynamic_falsecolor:false_color_exp_thresh",
        dynamic_false_color->false_color_exp_thresh, dynamic_false_color->total_num, ot_u32);
    scene_load_array(module, "dynamic_falsecolor:manual_strength", dynamic_false_color->manual_strength,
        dynamic_false_color->total_num, ot_u8);

    return OT_SUCCESS;
}


static ot_s32 scene_load_dynamic_fpn(const ot_char *module, ot_scene_dynamic_fpn *dynamic_fpn)
{
    ot_scenecomm_check_pointer_return(dynamic_fpn, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;

    scene_load_int(module, "dynamic_fpn:iso_count", dynamic_fpn->iso_count, ot_u32);
    scene_load_int(module, "dynamic_fpn:aibnr_mode", dynamic_fpn->aibnr_mode, ot_bool);
    scene_load_int(module, "dynamic_fpn:fpn_iso_thresh", dynamic_fpn->fpn_iso_thresh, ot_u32);
    scene_load_array(module, "dynamic_fpn:iso_thresh", dynamic_fpn->iso_thresh, dynamic_fpn->iso_count, ot_u32);
    scene_load_array(module, "dynamic_fpn:fpn_offset", dynamic_fpn->fpn_offset, dynamic_fpn->iso_count, ot_u32);
    return OT_SUCCESS;
}

#define SCENE_3DNR_ARG_LIST \
    &(p_x->nry0_en), &(p_x->nry1_en), &(p_x->nry2_en), &(p_x->nry3_en), \
    &pas[0].sfs1, &ps[1].sfs1, &ps[1].sbr1, &ps[2].sfs1, &ps[2].sbr1, &ps[3].sfs1, &ps[3].sbr1, \
    &pai[0].aux_ies, _tmprt3x(&ps, sfs2, sft2, sbr2), \
    _tmprt3x(&ps, sfs4, sft4, sbr4), \
    &pas[0].sfs5, &pas[0].sft5, &pas[0].sbr5, &ps[4].sfs2, &ps[4].sft2, &ps[4].sbr2, \
    &pai[0].ies0, &pai[0].ies1, &pai[0].ies2, &pai[0].ies3, _tmprt4_4(&pi, ies0, ies1, ies2, ies3), \
    &pai[0].o_sht, &pai[0].u_sht, &pi[3].o_sht, &pi[3].u_sht, &pi[4].o_sht, &pi[4].u_sht, \
    &ps[2].k_mode, &ps[3].k_mode, \
    &pas[0].spn3, &pas[0].sbn3, &pas[0].pbr3, \
    &pas[0].spn4, &pas[0].sbn4, &pas[0].pbr4, \
    &pas[0].spn6, &pas[0].sbn6, &pas[0].pbr6, &pas[0].j_mode6, _tmprt4_4(&ps, spn, sbn, pbr, j_mode), \
    &pas[0].sfr6[0], &pas[0].sfr6[1], &pas[0].sfr6[2], &pas[0].sfr6[3], _tmprt4_4(&ps, sfr6[0], sfr6[1], sfr6[2], sfr6[3]), \
    &pas[0].sbr6[0], &pas[0].sbr6[1], _tmprt2_4x(&ps, sbr6[0], sbr6[1]), \
    &pas[0].sfn0, &pas[0].sfn1, &pas[0].sfn2, &pas[0].sfn3, _tmprt4_4(&ps, sfn0_0, sfn1_0, sfn2_0, sfn3_0), \
    &pas[0].sth1, &pas[0].sth2, &pas[0].sth3, _tmprt3_4(&ps, sth1_0, sth2_0, sth3_0), \
    _tmprt4_2(&ps, sfn0_1, sfn1_1, sfn2_1, sfn3_1), \
    &pas[1].sfs1, _tmprt3z(&ps, 1, 2, sth1_1, sth2_1, sth3_1), \
    &pai[1].aux_ies, &ps[3].bri_idx0, &ps[3].bri_idx1, \
    &pt[1].ref_en, \
    &pai[1].ies0, &pai[1].ies1, &pai[1].ies2, &pai[1].ies3, &pt[0].auto_math, &(p_x->nrc0_en), \
    &pai[1].o_sht, &pai[1].u_sht, &pt[0].tfs0, &p_x->nrc0.pre_sfc, \
    &pm[0].dci_w, &p_x->nrc0.sfc, \
    &pas[1].spn3, &pas[1].sbn3, &pas[1].pbr3, &pt[1].tss0, &pt[1].tss1, &pt[2].tss0, &pt[2].tss1, &p_x->nrc0.sfc_enhance, &p_x->nrc0.sfc_ext, \
    &pas[1].spn4, &pas[1].sbn4, &pas[1].pbr4, &pt[1].tfs0, &pt[1].tfs1, &pt[2].tfs0, &pt[2].tfs1, &p_x->nrc0.tpc, \
    &pas[1].spn6, &pas[1].sbn6, &pas[1].pbr6, &pas[1].j_mode6, _tmprt3z(&pt, 1, 2, tfr0[0], tfr0[1], tfr0[2]), &p_x->nrc0.tfc, \
    &pas[1].sfr6[0], &pas[1].sfr6[1], &pas[1].sfr6[2], &pas[1].sfr6[3], _t3_(&pt, 1, tfr0[3], tfr0[4], tfr0[5]), _t3_(&pt, 2, tfr0[3], tfr0[4], tfr0[5]), &p_x->nrc0.trc, \
    &pas[1].sbr6[0], &pas[1].sbr6[1], _tmprt3z(&pt, 1, 2, tfr1[0], tfr1[1], tfr1[2]), \
    _tmprt3z(&pt, 1, 2, tfr1[3], tfr1[4], tfr1[5]), \
    &pas[1].sfn0, &pas[1].sfn1, &pas[1].sfn2, &pas[1].sfn3, &pt[0].math_mode, &pt[1].math_mode, &(p_x->nrc1_en), \
    &pas[1].sth1, &pas[1].sth2, &pas[1].sth3, &pc->sfs2, &pc->sft2, &pc->sbr2, \
    &pm[0].adv_math, &pm[1].adv_math, &pm[0].mcth, \
    &pas[0].bld1,  &pm[0].math0, &pm[0].math1, &pm[1].math0, &pm[1].math1, &pc->pbr0, &pc->pbr1, \
    &p_x->adv_mode, &pm[0].mate0, &pm[0].mate1, &pm[1].mate0, &pm[1].mate1, \
    &pas[0].bri_idx, &pas[1].bri_idx, &pm[0].mabw0, &pm[0].mabw1, &pm[1].mabw0, &pm[1].mabw1, \
    _t4a_0_(&ps, 3, bri_str), _t4a_4_(&ps, 3, bri_str), \
    _t4a_8_(&ps, 3, bri_str), _t4a_12_(&ps, 3, bri_str), &ps[3].bri_str[0x10], \
    _t4a_0_(&ps, 2, sbs_k), _t4a_4_(&ps, 2, sbs_k), _t4a_8_(&ps, 2, sbs_k), _t4a_12_(&ps, 2, sbs_k), \
    _t4a_10_(&ps, 2, sbs_k), _t4a_14_(&ps, 2, sbs_k), _t4a_18_(&ps, 2, sbs_k), _t4a_1c_(&ps, 2, sbs_k), &ps[2].sbs_k[0x20], \
    _t4a_0_(&ps, 2, sds_k), _t4a_4_(&ps, 2, sds_k), _t4a_8_(&ps, 2, sds_k), _t4a_12_(&ps, 2, sds_k), \
    _t4a_10_(&ps, 2, sds_k), _t4a_14_(&ps, 2, sds_k), _t4a_18_(&ps, 2, sds_k), _t4a_1c_(&ps, 2, sds_k), &ps[2].sds_k[0x20], \
    _t4a_0_(&ps, 3, sbs_k), _t4a_4_(&ps, 3, sbs_k), _t4a_8_(&ps, 3, sbs_k), _t4a_12_(&ps, 3, sbs_k), \
    _t4a_10_(&ps, 3, sbs_k), _t4a_14_(&ps, 3, sbs_k), _t4a_18_(&ps, 3, sbs_k), _t4a_1c_(&ps, 3, sbs_k), &ps[3].sbs_k[0x20], \
    _t4a_0_(&ps, 3, sds_k), _t4a_4_(&ps, 3, sds_k), _t4a_8_(&ps, 3, sds_k), _t4a_12_(&ps, 3, sds_k), \
    _t4a_10_(&ps, 3, sds_k), _t4a_14_(&ps, 3, sds_k), _t4a_18_(&ps, 3, sds_k), _t4a_1c_(&ps, 3, sds_k), &ps[3].sds_k[0x20]


static const char *g_3dnr_fmt =
    " -en          %3d:        |              %3d:      |       %3d:       |               %3d:                    |"
    " -nXsf1       %3d:        |          %3d:    %3d:  |    %3d:    %3d:  |            %3d:    %3d:               |"
    " -nXsf2       %3d:        |          %3d:%3d:%3d:  |    %3d:%3d:%3d:  |            %3d:%3d:%3d:               |"
    " -nXsf4                   |          %3d:%3d:%3d:  |    %3d:%3d:%3d:  |            %3d:%3d:%3d:               |"
    " -nXsfs5     %3d:%3d:%3d: |                        |                  |            %3d:%3d:%3d:               |"
    " -nXsf5  %3d:%3d:%3d:%3d: |        %3d:%3d:%3d:%3d:|  %3d:%3d:%3d:%3d:|  %3d:%3d:%3d:%3d: |  %3d:%3d:%3d:%3d: |"
    " -nXsht5         %3d:%3d: |                        |                  | -nXsht5  %3d:%3d: |          %3d:%3d: |"
    "                          |                        |-kmode      %3d:  |              %3d: |                   |"
    " -nXsf3      %3d:%3d:%3d: |                        |                  |                   |                   |"
    " -nXsf4      %3d:%3d:%3d: |                        |                  |                   |                   |"
    " -nXsf6  %3d:%3d:%3d:%3d: |        %3d:%3d:%3d:%3d:|  %3d:%3d:%3d:%3d:|  %3d:%3d:%3d:%3d: |  %3d:%3d:%3d:%3d: |"
    " -nXsfr6 %3d:%3d:%3d:%3d: |        %3d:%3d:%3d:%3d:|  %3d:%3d:%3d:%3d:|  %3d:%3d:%3d:%3d: |  %3d:%3d:%3d:%3d: |"
    " -nXsbr6         %3d:%3d: |             %3d:%3d:   |          %3d:%3d:|          %3d:%3d: |          %3d:%3d: |"
    "                          |                        |                  |                   |                   |"
    " -nXsfn  %3d:%3d:%3d:%3d: |        %3d:%3d:%3d:%3d:|  %3d:%3d:%3d:%3d:|  %3d:%3d:%3d:%3d: |  %3d:%3d:%3d:%3d: |"
    " -nXsth      %3d:%3d:%3d: |            %3d:%3d:%3d:|      %3d:%3d:%3d:|      %3d:%3d:%3d: |      %3d:%3d:%3d: |"
    " *************************|-nX2sfn %3d:%3d:%3d:%3d:|  %3d:%3d:%3d:%3d:|                                       |"
    " -nX2sf1      %3d:        |-nX2sth     %3d:%3d:%3d:|      %3d:%3d:%3d:|                                       |"
    " -nX2sf2      %3d:        |                        |                  |-nXbIdx           %3d:%3d:             |"
    "                          |-ref          %3d:      |                  |********************nrc0***************|"
    " -nX2sf5 %3d:%3d:%3d:%3d: |-preMath      %3d:      |                  |  -nC0en             %3d:              |"
    " -nX2sht5        %3d:%3d: |-preTFS       %3d:      |                  |  -presfc            %3d:              |"
    "                          |-dciW         %3d:      |                  |  -sfc               %3d:              |"
    " -nX2sf3     %3d:%3d:%3d: |-nXtss      %3d:%3d:    |     %3d:%3d:     |  -sfcExt          %3d:%3d:            |"
    " -nX2sf4     %3d:%3d:%3d: |-nXtfs      %3d:%3d:    |     %3d:%3d:     |  -tpc               %3d:              |"
    " -nX2sf6 %3d:%3d:%3d:%3d: |-nXtfr0   %3d:%3d:%3d:  |   %3d:%3d:%3d:   |  -tfc               %3d:              |"
    " -nX2sfr6%3d:%3d:%3d:%3d: |          %3d:%3d:%3d:  |   %3d:%3d:%3d:   |  -trc               %3d:              |"
    " -nX2sbr6        %3d:%3d: |-nXtfr1   %3d:%3d:%3d:  |   %3d:%3d:%3d:   |********************nrc1***************|"
    "                          |          %3d:%3d:%3d:  |   %3d:%3d:%3d:   |                                       |"
    " -nX2sfn %3d:%3d:%3d:%3d: |-maMode     %3d:%3d:    |                  | -nC1en              %3d:              |"
    " -nX2sth     %3d:%3d:%3d: |                        |                  | -nCsf2          %3d:%3d:%3d:          |"
    "                          |-advMath    %3d:%3d:    |-mcth    %3d:     |                                       |"
    " -mXmath             %3d: |            %3d:%3d:    |     %3d:%3d:     | -nCstr           %3d:| %3d:           |"
    " -advMode            %3d: |-mXmate     %3d:%3d:    |     %3d:%3d:     |                                       |"
    " -nXbIdx         %3d:%3d: |-mXmabw     %3d:%3d:    |     %3d:%3d:     |                                       |"
    "***************************************************************************************************************"
    " -n3bri0    %3d %3d %3d %3d   -n3bri4  %3d %3d %3d %3d                                                         "
    " -n3bri8    %3d %3d %3d %3d   -n3bri12 %3d %3d %3d %3d %3d                                                     "
    " -n2sbs0    %3d %3d %3d %3d %3d %3d %3d %3d     -n2sbs8   %3d %3d %3d %3d %3d %3d %3d %3d                      "
    " -n2sbs16   %3d %3d %3d %3d %3d %3d %3d %3d     -n2sbs24  %3d %3d %3d %3d %3d %3d %3d %3d %3d                  "
    " -n2sds0    %3d %3d %3d %3d %3d %3d %3d %3d     -n2sds8   %3d %3d %3d %3d %3d %3d %3d %3d                      "
    " -n2sds16   %3d %3d %3d %3d %3d %3d %3d %3d     -n2sds24  %3d %3d %3d %3d %3d %3d %3d %3d %3d                  "
    " -n3sbs0    %3d %3d %3d %3d %3d %3d %3d %3d     -n3sbs8   %3d %3d %3d %3d %3d %3d %3d %3d                      "
    " -n3sbs16   %3d %3d %3d %3d %3d %3d %3d %3d     -n3sbs24  %3d %3d %3d %3d %3d %3d %3d %3d %3d                  "
    " -n3sds0    %3d %3d %3d %3d %3d %3d %3d %3d     -n3sds8   %3d %3d %3d %3d %3d %3d %3d %3d                      "
    " -n3sds16   %3d %3d %3d %3d %3d %3d %3d %3d     -n3sds24  %3d %3d %3d %3d %3d %3d %3d %3d %3d                  ";


static ot_s32 scene_load_static_3dnr(const ot_char *module, const ot_scene_module_state *module_state,
    ot_scene_static_3dnr *static_3dnr)
{
    ot_scenecomm_check_pointer_return(static_3dnr, OT_FAILURE);
    ot_s32 ret;
    ot_char *get_string = NULL;
    ot_u32 i;
    ot_s32 get_value;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };

    scene_load_int(module, "static_3dnr:enable", static_3dnr->enable, ot_bool);
    scene_load_int(module, "static_3dnr:threed_nr_count", static_3dnr->threed_nr_count, ot_u32);

    scene_load_array(module, "static_3dnr:threed_nr_iso", static_3dnr->threed_nr_iso,
        static_3dnr->threed_nr_count, ot_u32);

    for (i = 0; i < static_3dnr->threed_nr_count; i++) {
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "static_3dnr:3DnrParam_%u", i);
        ret = ot_confaccess_get_string(SCENE_INIPARAM, module, (const char *)node, NULL, &get_string);
        scene_iniparam_check_load_return(ret, module);

        if (get_string != NULL) {
            ot_scene_3dnr *p_x = &(static_3dnr->threednr_value[i]);
            t_v200_vproc_iey *pi = p_x->iey;
            t_v200_vproc_sfy *ps = p_x->sfy;
            t_v200_vproc_mdy *pm = p_x->mdy;
            t_v200_vproc_tfy *pt = p_x->tfy;
            t_v200_vproc_nrc1 *pc = &p_x->nrc1;

            t_v200_vproc_adv_iey *pai = p_x->adv_ie_y;
            t_v200_vproc_adv_sfy *pas = p_x->adv_sf_y;

            ret = sscanf_s(get_string, g_3dnr_fmt, SCENE_3DNR_ARG_LIST);
            free(get_string);
            get_string = OT_NULL;
            if (ret == -1) {
                scene_loge("sscanf_s error\n");
                return OT_FAILURE;
            }
        }
    }
    return OT_SUCCESS;
}

#define scene_load_module(func, module, addr) do {     \
        ret = func(module, addr);                      \
        scene_iniparam_check_load_return(ret, module); \
    } while (0)

static ot_s32 scene_load_scene_param_part1(const ot_char *module, ot_scene_pipe_param *scene_param)
{
    ot_scenecomm_check_pointer_return(scene_param, OT_FAILURE);
    ot_s32 ret;

    ret = scene_load_dynamic_drc(module, &scene_param->module_state, &scene_param->dynamic_drc);
    scene_iniparam_check_load_return(ret, module);

    scene_load_module(scene_load_dynamic_gamma, module, &scene_param->dynamic_gamma);
    scene_load_module(scene_load_dynamic_nr, module, &scene_param->dynamic_nr);
    scene_load_module(scene_load_dynamic_shading, module, &scene_param->dynamic_shading);
    scene_load_module(scene_load_dynamic_clut, module, &scene_param->dynamic_clut);
    scene_load_module(scene_load_dynamic_ca, module, &scene_param->dynamic_ca);
    scene_load_module(scene_load_dynamic_blc, module, &scene_param->dynamic_blc);
    scene_load_module(scene_load_dynamic_dpc, module, &scene_param->dynamic_dpc);
    scene_load_module(scene_load_dynamic_awb, module, &scene_param->dynamic_awb);
    scene_load_module(scene_load_dynamic_fswdr, module, &scene_param->dynamic_fswdr);
    scene_load_module(scene_load_dynamic_ldci, module, &scene_param->dynamic_ldci);
    scene_load_module(scene_load_dynamic_ldcihlc, module, &scene_param->dynamic_ldcihlc);
    scene_load_module(scene_load_dynamic_false_color, module, &scene_param->dynamic_false_color);
    scene_load_module(scene_load_dynamic_fpn, module, &scene_param->dynamic_fpn);
    scene_load_module(scene_load_dynamic_back_light, module, &scene_param->dynamic_back_light);

    ret = scene_load_static_3dnr(module, &scene_param->module_state, &scene_param->static_threednr);
    scene_iniparam_check_load_return(ret, module);
    return OT_SUCCESS;
}


static ot_s32 scene_load_scene_param(const ot_char *module, ot_scene_pipe_param *scene_param)
{
    ot_scenecomm_check_pointer_return(scene_param, OT_FAILURE);
    ot_s32 ret;
    scene_load_module(scene_load_module_state, module, &scene_param->module_state);
    scene_load_module(scene_load_static_ae, module, &scene_param->static_ae);
    scene_load_module(scene_load_ae_weight_tab, module, &scene_param->static_statistics);
    scene_load_module(scene_load_static_ae_route_ex, module, &scene_param->static_ae_route_ex);
    scene_load_module(scene_load_static_wdr_exposure, module, &scene_param->static_wdr_exposure);
    scene_load_module(scene_load_static_fswdr, module, &scene_param->static_fswdr);
    scene_load_module(scene_load_static_awb, module, &scene_param->static_awb);
    scene_load_module(scene_load_static_awb_ex, module, &scene_param->static_awb_ex);
    scene_load_module(scene_load_static_clut, module, &scene_param->static_clut);
    scene_load_module(scene_load_static_cmm, module, &scene_param->static_ccm);
    scene_load_module(scene_load_static_saturation, module, &scene_param->static_saturation);
    scene_load_module(scene_load_static_ldci, module, &scene_param->static_ldci);
    scene_load_module(scene_load_static_ldcihlc, module, &scene_param->static_ldcihlc);
    scene_load_module(scene_load_static_blc, module, &scene_param->static_blc);
    scene_load_module(scene_load_static_drc, module, &scene_param->static_drc);
    scene_load_module(scene_load_static_nr, module, &scene_param->static_nr);
    scene_load_module(scene_load_static_ca, module, &scene_param->static_ca);
    scene_load_module(scene_load_static_cac, module, &scene_param->static_cac);
    scene_load_module(scene_load_static_dpc, module, &scene_param->static_dpc);
    scene_load_module(scene_load_static_dehaze, module, &scene_param->static_dehaze);
    scene_load_module(scene_load_static_shading, module, &scene_param->static_shading);
    scene_load_module(scene_load_static_csc, module, &scene_param->staic_csc);
    scene_load_module(scene_load_static_demosaic, module, &scene_param->static_dm);
    scene_load_module(scene_load_static_cross_talk, module, &scene_param->static_crosstalk);
    scene_load_module(scene_load_static_bayershp, module, &scene_param->static_bayershp);
    scene_load_module(scene_load_static_sharpen, module, &scene_param->static_sharpen);
    scene_load_module(scene_load_static_back_light, module, &scene_param->static_back_light);
    scene_load_module(scene_load_dynamic_ae, module, &scene_param->dynamic_ae);
    scene_load_module(scene_load_dynamic_fps, module, &scene_param->dynamic_fps);
    scene_load_module(scene_load_dynamic_dehaze, module, &scene_param->dynamic_dehaze);
    scene_load_module(scene_load_dynamic_vproc, module, &scene_param->dynamic_vproc);
    scene_load_module(scene_load_static_mce, module, &scene_param->static_mce);
    scene_load_module(scene_load_dynamic_abnr, module, &scene_param->dynamic_abnr);
    scene_load_module(scene_load_dynamic_iqt, module, &scene_param->dynamic_iqt);
    scene_load_module(scene_load_dynamic_mce, module, &scene_param->dynamic_mce);

    ret = scene_load_scene_param_part1(module, scene_param);
    scene_loge("load scene[%d] \n", __LINE__);
    scene_iniparam_check_load_return(ret, module);
    return OT_SUCCESS;
}

static ot_s32 scene_load_scene_conf(ot_scene_pipe_param *scene_pipe_param, ot_s32 count)
{
    ot_scenecomm_check_pointer_return(scene_pipe_param, OT_FAILURE);
    ot_s32 ret;
    ot_s32 mode_index;
    ot_char module[SCENE_INIPARAM_MODULE_NAME_LEN] = { 0 };

    for (mode_index = 0; mode_index < count && mode_index < OT_SCENE_PIPETYPE_NUM; mode_index++) {
        snprintf_truncated_s(module, SCENE_INIPARAM_MODULE_NAME_LEN, "%s%d", SCENE_INI_SCENEMODE, mode_index);

        ret = scene_load_scene_param((const char *)module, scene_pipe_param + mode_index);
        if (ret != OT_SUCCESS) {
            scene_loge("load scene[%d] config failed\n", mode_index);
            return OT_FAILURE;
        }
    }

    return OT_SUCCESS;
}


static ot_s32 scene_load_mediapipe_param(const ot_char *module, ot_scene_mediapipe *scene_mediapipe)
{
    ot_scenecomm_check_pointer_return(scene_mediapipe, OT_FAILURE);
    ot_s32 ret;
    ot_s32 get_value;
    ot_u32 i;
    ot_char node[SCENE_INIPARAM_NODE_NAME_LEN] = { 0 };
    scene_mediapipe_comm_param *mediapipe_comm = &scene_mediapipe->mediapipe_comm;

    scene_load_int(module, "mediapipe_comm:mediapipe_num", mediapipe_comm->mediapipe_num, ot_u32);
    if(mediapipe_comm->mediapipe_num >= OT_SCENE_PIPE_MAX_NUM) {
        scene_loge(RED " mediapipe_num(%d) must be less than %d" NONE "\n", mediapipe_comm->mediapipe_num, OT_SCENE_PIPE_MAX_NUM);
        return OT_FAILURE;
    }

    snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_comm:vi_vproc_mode_type");
    scene_load_int(module, node, mediapipe_comm->vi_vproc_mode_type, ot_u8);

    snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_comm:vo_display_dev");
    scene_load_enum(module, node, mediapipe_comm->vo_display_dev);

    snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_comm:vo_fps");
    scene_load_float(module, node, mediapipe_comm->vo_fps, ot_float);

    snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_comm:vrb_buf_blks");
    scene_load_array(module, node, mediapipe_comm->vrb_buf_blks, SCENE_VRB_BUF_BLK_NUM, ot_u32);

    for (i = 0; i < mediapipe_comm->mediapipe_num; i++) {
        scene_mediapipe_attr_param *attr_param = &scene_mediapipe->mediapipe_attr[i];
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_enable_%u:thr_enable_normal", i);
        scene_load_int(module, node, attr_param->thr_en_param.thr_enable_normal, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_enable_%u:thr_enable_mce", i);
        scene_load_int(module, node, attr_param->thr_en_param.thr_enable_mce, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_enable_%u:thr_enable_deflicker", i);
        scene_load_int(module, node, attr_param->thr_en_param.thr_enable_deflicker, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_enable_%u:thr_enable_clut", i);
        scene_load_int(module, node, attr_param->thr_en_param.thr_enable_clut, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_enable_%u:thr_enable_dehaze", i);
        scene_load_int(module, node, attr_param->thr_en_param.thr_enable_dehaze, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_enable_%u:thr_enable_abnr", i);
        scene_load_int(module, node, attr_param->thr_en_param.thr_enable_abnr, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_enable_%u:thr_enable_iqt", i);
        scene_load_int(module, node, attr_param->thr_en_param.thr_enable_iqt, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_enable_%u:thr_enable_vp2vo", i);
        scene_load_int(module, node, attr_param->thr_en_param.thr_enable_vp2vo, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_usleep_%u:thr_usleep_normal", i);
        scene_load_int(module, node, attr_param->thr_sl_param.thr_usleep_normal, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_usleep_%u:thr_usleep_clut", i);
        scene_load_int(module, node, attr_param->thr_sl_param.thr_usleep_clut, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_usleep_%u:thr_usleep_dehaze", i);
        scene_load_int(module, node, attr_param->thr_sl_param.thr_usleep_dehaze, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_usleep_%u:thr_usleep_abnr", i);
        scene_load_int(module, node, attr_param->thr_sl_param.thr_usleep_abnr, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_usleep_%u:thr_usleep_fp_pipe", i);
        scene_load_int(module, node, attr_param->thr_sl_param.thr_usleep_fp_pipe, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "thread_usleep_%u:thr_usleep_fp_chn", i);
        scene_load_int(module, node, attr_param->thr_sl_param.thr_usleep_fp_chn, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vi_with_sensor", i);
        scene_load_int(module, node, attr_param->mediapipe_param.vi_with_sensor, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vi_sns_type", i);
        scene_load_enum(module, node, attr_param->mediapipe_param.vi_sns_type);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vi_dev_id", i);
        scene_load_int(module, node, attr_param->mediapipe_param.vi_dev_id, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_pipe_frame_pool_enable", i);
        scene_load_int(module, node, attr_param->mediapipe_param.vproc_pipe_frame_pool_enable, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_frame_pool_enable", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_frame_pool_enable, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_enable", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_enable, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_frc_src", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_frc_src, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_frc_dst", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_frc_dst, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_img_w", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_img_w, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_img_h", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_img_h, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_crop_enable", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_crop_enable, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_crop_mode", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_crop_mode, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_crop_x", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_crop_x, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u16);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_crop_y", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_crop_y, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u16);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_crop_w", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_crop_w, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_crop_h", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_crop_h, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_mirror_enable", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_mirror_enable, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_rotation_enable", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_rotation_enable, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:vproc_chn_rotation", i);
        scene_load_array(module, node, attr_param->mediapipe_param.vproc_chn_rotation, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:save_video_enable", i);
        scene_load_int(module, node, attr_param->mediapipe_param.save_video_enable, ot_bool);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_chn_id", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_chn_id, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_codec_format", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_codec_format, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_stream_buf_size", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_stream_buf_size, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_gop", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_gop, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_rc_mode", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_rc_mode, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_statis_time", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_statis_time, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_max_bit_rate", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_max_bit_rate, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_short_statis_time", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_short_statis_time, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_long_statis_time", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_long_statis_time, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_long_max_bit_rate", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_long_max_bit_rate, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_long_min_bit_rate", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_long_min_bit_rate, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_gop_mode", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_gop_mode, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_qp_delta_ip", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_qp_delta_ip, ot_u32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:venc_max_file_size_m", i);
        scene_load_int(module, node, attr_param->mediapipe_param.venc_max_file_size_m, ot_u32);

        // compress
        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:comp_mode_vi_pipe", i);
        scene_load_int(module, node, attr_param->mediapipe_param.comp_mode_vi_pipe, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:comp_mode_vi_chn", i);
        scene_load_int(module, node, attr_param->mediapipe_param.comp_mode_vi_chn, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:comp_mode_vproc_pipe", i);
        scene_load_array(module, node, attr_param->mediapipe_param.comp_mode_vproc_pipe, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:comp_mode_vproc_chn", i);
        scene_load_array(module, node, attr_param->mediapipe_param.comp_mode_vproc_chn, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mediapipe_%u:comp_mode_3dnr", i);
        scene_load_int(module, node, attr_param->mediapipe_param.comp_mode_3dnr, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "mce_%u:mce_chn_id", i);
        scene_load_int(module, node, attr_param->mce_param.mce_chn_id, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "debug_%u:debug_deflicker_enable", i);
        scene_load_int(module, node, attr_param->debug_param.debug_deflicker_enable, ot_u8);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "vp2vo_%u:vp2vo_chn_id", i);
        scene_load_array(module, node, attr_param->vp2vo_param.vp2vo_chn_id, OT_EIS_VPROC_GRP_CHN_MAX_NUM, ot_s32);

        snprintf_truncated_s(node, SCENE_INIPARAM_NODE_NAME_LEN, "vp2vo_%u:vp2vo_on_one_thread", i);
        scene_load_int(module, node, attr_param->vp2vo_param.vp2vo_on_one_thread, ot_bool);
    }

    return OT_SUCCESS;
}

static ot_s32 scene_load_mediapipe_conf(ot_scene_mediapipe *scene_mediapipe)
{
    ot_scenecomm_check_pointer_return(scene_mediapipe, OT_FAILURE);
    ot_s32 ret;
    ot_char module[SCENE_INIPARAM_MODULE_NAME_LEN] = { 0 };

    snprintf_truncated_s(module, SCENE_INIPARAM_MODULE_NAME_LEN, "%s", SCENE_INI_MEDIAPIPE);

    ret = scene_load_mediapipe_param((const char *)module, scene_mediapipe);
    if (ret != OT_SUCCESS) {
        scene_loge("load scene_mediapipe config failed\n");
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

ot_s32 ot_scene_create_param(const ot_char *dir_name, ot_scene_param *scene_param, ot_scene_mediapipe *scene_mediapipe)
{
    ot_s32 ret;
    ot_u32 module_num = 0;
    ot_char ini_path[SCENETOOL_MAX_FILESIZE] = {0};
    /* Load Product Ini Configure */
    if (dir_name == OT_NULL || scene_param == OT_NULL) {
        scene_loge("Null Pointer.");
        return OT_SCENE_ENONPTR;
    }

    snprintf_truncated_s(g_ini_dir, SCENETOOL_MAX_FILESIZE, "%s", dir_name);
    snprintf_truncated_s(ini_path, SCENETOOL_MAX_FILESIZE, "%s%s", dir_name, "/config_cfgaccess_hd.ini");
    ret = ot_confaccess_init(SCENE_INIPARAM, (const char *)ini_path, &module_num);
    if (ret != OT_SUCCESS) {
        scene_loge("load ini [%s] failed [%08x]\n", ini_path, ret);
        return OT_FAILURE;
    }

    ini_dictionary *ini_info_dict = ini_process_file(ini_path);
    if (ini_info_dict) {
        const ot_char *value = ini_get_string(ini_info_dict, (const char *)"relative_path:relative_path", NULL);
        if (value != OT_NULL) {
            snprintf_truncated_s(g_relative_path, SCENETOOL_MAX_FILESIZE, "%s", value);
        }
        free_ini_info_dict(ini_info_dict);
    }

    ot_scene_param scene_param_tmp = {0};
    (ot_void)memset_s(&scene_param_tmp, sizeof(ot_scene_param), 0, sizeof(ot_scene_param));
    ret = scene_load_scene_conf(scene_param_tmp.pipe_param, module_num - 1);
    if (ret != OT_SUCCESS) {
        scene_loge("SCENE_LoadConf failed!\n");
        return OT_FAILURE;
    }
    (ot_void)memcpy_s(scene_param, sizeof(ot_scene_param), &scene_param_tmp, sizeof(ot_scene_param));

    if (scene_mediapipe != OT_NULL) {
        ot_scene_mediapipe mediapipe_tmp = {0};
        (ot_void)memset_s(&mediapipe_tmp, sizeof(ot_scene_mediapipe), 0, sizeof(ot_scene_mediapipe));
        ret = scene_load_mediapipe_conf(&mediapipe_tmp);
        if (ret != OT_SUCCESS) {
            scene_loge("SCENE_LoadConf failed!\n");
            return OT_FAILURE;
        }
        (ot_void)memcpy_s(scene_mediapipe, sizeof(ot_scene_mediapipe), &mediapipe_tmp, sizeof(ot_scene_mediapipe));
    }

    ret = ot_confaccess_deinit(SCENE_INIPARAM);
    if (ret != OT_SUCCESS) {
        scene_loge("load ini [%s] failed [%08x]\n", ini_path, ret);
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
