#include <stdio.h>
#include <sys/time.h>
#include "sample_comm.h"
#include "cJSON.h"
#include <stdlib.h>
#include "ot_eis_vi_sns.h"
#include "ot_eis_vi_awb.h"
#include "ot_eis_sns_ctrl.h"

#define json_err(fmt, ...)                                                      \
    do {                                                                        \
        printf("[ERR] %s-%d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);      \
    } while (0)

#define check_data_neq_return(data0, data1)                         \
    do {                                                            \
        if ((data0) != (data1)) {                                   \
            sample_print("%d and %d isn't equal\n", data0, data1);  \
            return OT_FAILURE;                                      \
        }                                                           \
    } while (0)



#define check_null_type_return(param,cJSON_type)                        \
    do {                                                                 \
        if(param == OT_NULL) {                                           \
            json_err("param NULL\n");                                   \
            return OT_FAILURE;                                          \
        }                                                               \
        if(param->type != cJSON_type) {                                 \
            json_err("param type\n");                                   \
            return OT_FAILURE;                                          \
        }                                                               \
    } while(0)

#define check_null_return(item)                                     \
    do {                                                             \
        if(item == OT_NULL) {                                        \
            json_err("item NULL\n");                                \
            return OT_FAILURE;                                      \
        }                                                           \
    }while(0)

#define check_ret_return(ret)                                       \
    do {                                                             \
        if(ret != OT_SUCCESS) {                                      \
            json_err("return err\n");                               \
            return OT_FAILURE;                                      \
        }                                                           \
    }while(0)

typedef struct {
    ot_eis_vi_sns_alg_key           key;
    ot_eis_vi_drc_attr              drc;
    ot_eis_vi_demosaic_attr         demosaic;
    ot_eis_vi_gamma_attr            gamma;
    ot_eis_vi_sharpen_attr          sharpen;
    ot_eis_vi_ldci_attr             ldci;
    ot_eis_vi_dpc_dynamic_cor_attr  dpc;
    ot_eis_vi_sns_lsc               lsc;
    ot_eis_vi_cr_attr               ge;
    ot_eis_vi_anti_false_color_attr anti_false_color;
    ot_eis_vi_nr_attr               bayer_nr;
    ot_eis_vi_ca_attr               ca;
    ot_eis_vi_expander_attr         expander;
    ot_eis_vi_sns_clut              clut;
    ot_eis_vi_wdr_attr              wdr;
    ot_eis_vi_dehaze_attr           dehaze;
    ot_eis_vi_cac_attr              cac;
    ot_eis_vi_bayer_sharpen_attr    bshp;
    ot_eis_vi_cmos_acs              acs;
    ot_eis_vi_cmos_lblc             lblc;
    ot_eis_vi_sns_black_level       sns_blc;
    ot_eis_vi_noise_calibration     noise_calibration;
    ot_eis_vi_sns_max_resolution sns_max_resolution;
    ot_eis_vi_sns_mode           sns_mode;
    ot_eis_vi_sns_wdr_switch_attr       wdr_switch_attr;
} sample_sns_alg;

typedef enum{
    SNS_DKSHD_PARAM_TABLE   = 0,
    SNS_ADJ_PARAM_TABLE     = 1,
    SNS_PXSHD_PARAM_TABLE   = 2,
    SNS_AWB_PARAM_TABLE     = 3,
    SNS_FME_PARAM_TABLE     = 4,
    SNS_PARAM_TABLE_NUM     = 5
}sample_sensor_table_type;

static sample_sns_alg  g_ext_sns_alg[OT_EIS_VI_MAX_PIPE_NUM];
static ot_eis_sns_reg_table  g_ext_sns_table[OT_EIS_VI_MAX_PIPE_NUM][SNS_PARAM_TABLE_NUM];

static char* read_file(const char *filename)
{
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;

    /* open in read binary mode */
    file = fopen(filename, "rb");
    if (file == NULL) {
        json_err("fopen %s failed\n", filename);
        return OT_NULL;
    }

    /* get the length */
    if (fseek(file, 0, SEEK_END) != 0) {
        json_err("fseek failed\n");
        goto fclose_file;
    }

    length = ftell(file);
    if (length < 0) {
        json_err("ftell failed\n");
        goto fclose_file;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        json_err("fseek failed\n");
        goto fclose_file;
    }
    /* allocate content buffer */
    content = (char*)malloc((size_t)length + sizeof(""));
    if (content == NULL) {
        json_err("malloc failed\n");
        goto fclose_file;
    }

    /* read the file into memory */
    read_chars = fread(content, sizeof(char), (size_t)length, file);
    if ((long)read_chars != length) {
        json_err("fread failed\n");
        free(content);
        content = NULL;
        goto fclose_file;
    }
    content[read_chars] = '\0';

fclose_file:
    fclose(file);
    return content;
}

static cJSON *parse_file(const char * const filename)
{
    char *file = NULL;
    cJSON *json = NULL;

    file = read_file(filename);
    if (file == NULL) {
        json_err("read_file failed\n");
        return NULL;
    }

    json = cJSON_Parse(file);
    if (json == NULL) {
        json_err("cJSON_Parse failed\n");
    }

    free(file);
    return json;
}



static cJSON *json_get_object(const cJSON *json,const char *str_key)
{
    cJSON *param = cJSON_GetObjectItemCaseSensitive(json,str_key);

    if(param == OT_NULL) {
        json_err("param NULL\n");
        return OT_NULL;
    }
    if(param->type != cJSON_Object) {
        json_err("param type\n");
        return OT_NULL;
    }
    return param;
}

static ot_s32  sample_comm_parse_1dimen_float_array_by_key(cJSON *json, ot_float *data, ot_s32 size, const char *str_key)
{
    cJSON *array_1d,*array_2d;
    array_1d = cJSON_GetObjectItemCaseSensitive(json, str_key);
    check_null_type_return(array_1d, cJSON_Array);

    ot_u32 array_cut  = cJSON_GetArraySize(array_1d);
    if(array_cut > size)
        array_cut = size;

    array_2d = array_1d->child;
    for(int i = 0; i < array_cut; i++) {
        check_null_type_return(array_2d,cJSON_Number);
        data[i] = array_2d->valuedouble;
        array_2d = array_2d->next;
    }
    return OT_SUCCESS;
}

static ot_s32  sample_comm_parse_1dimen_u32_array_by_key(cJSON *json, ot_u32 *data, ot_s32 size, const char *str_key)
{
    cJSON *array_1d, *array_2d;
    array_1d = cJSON_GetObjectItemCaseSensitive(json, str_key);
    check_null_type_return(array_1d, cJSON_Array);
    ot_s32 cnt = cJSON_GetArraySize(array_1d);
    if(cnt > size)
        cnt = size;

    array_2d = array_1d->child;
    for(int i = 0; i < cnt; i++) {
        check_null_type_return(array_2d,cJSON_Number);
        data[i] = array_2d->valueint;
        array_2d = array_2d->next;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_2dimen_u32_array_by_key(cJSON *json, ot_u32 *data, ot_s32 size_x, ot_s32 size_y, const char *str_key)
{
    ot_u32  cnt_x,cnt_y;
    cJSON *array_1d, *array_2d, *array_3d;

    array_1d = cJSON_GetObjectItemCaseSensitive(json, str_key);
    check_null_type_return(array_1d, cJSON_Array);
    cnt_x  = cJSON_GetArraySize(array_1d);

    array_2d = array_1d->child;
    for (int i = 0; i < cnt_x; i++) {
        check_null_type_return(array_2d, cJSON_Array);
        cnt_y  = cJSON_GetArraySize(array_2d);

        array_3d = array_2d->child;
        for (int j = 0; j < cnt_y; j++) {
            check_null_type_return(array_3d, cJSON_Number);
            *(data + i*size_y + j) =  array_3d->valueint;
            array_3d = array_3d->next;
        }
        array_2d = array_2d->next;
    }

    return OT_SUCCESS;
}

static ot_s32  sample_comm_parse_1dimen_u16_array_by_key(cJSON *json, ot_u16 *data, ot_s32 size, const char *str_key)
{
    cJSON *array_1d, *array_2d;

    array_1d = cJSON_GetObjectItemCaseSensitive(json, str_key);
    check_null_type_return(array_1d, cJSON_Array);
    ot_s32 cnt = cJSON_GetArraySize(array_1d);
    if(cnt > size)
        cnt = size;

    array_2d = array_1d->child;
    for(int i = 0; i < cnt; i++) {
        check_null_type_return(array_2d,cJSON_Number);
        data[i] = array_2d->valueint;
        array_2d = array_2d->next;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_2dimen_u16_array_by_key(cJSON *json, ot_u16 *data, ot_s32 size_x, ot_s32 size_y, const char *str_key)
{
    ot_u32  cnt_x,cnt_y;
    cJSON *array_1d, *array_2d,*array_3d;

    array_1d = cJSON_GetObjectItemCaseSensitive(json, str_key);
    check_null_type_return(array_1d,cJSON_Array);
    cnt_x  = cJSON_GetArraySize(array_1d);

    array_2d = array_1d->child;
    for (int i = 0; i < cnt_x; i++) {
        check_null_type_return(array_2d,cJSON_Array);
        cnt_y  = cJSON_GetArraySize(array_2d);

        array_3d = array_2d->child;
        for (int j = 0; j < cnt_y; j++) {
            check_null_type_return(array_3d, cJSON_Number);
            *(data + i*size_y + j) =  array_3d->valueint;
            array_3d = array_3d->next;
        }
        array_2d = array_2d->next;
    }
    return OT_SUCCESS;
}

static ot_s32  sample_comm_parse_1dimen_u8_array_by_key(cJSON *json, ot_u8 *data, ot_s32 size, const char *str_key)
{
    cJSON *array_1d,*array_2d;

    array_1d = cJSON_GetObjectItemCaseSensitive(json, str_key);
    check_null_type_return(array_1d, cJSON_Array);
    ot_s32 cnt = cJSON_GetArraySize(array_1d);
    if(cnt > size)
        cnt = size;

    array_2d = array_1d->child;
    for(int i = 0; i < cnt; i++) {
        check_null_type_return(array_2d,cJSON_Number);
        data[i] = array_2d->valueint;
        array_2d = array_2d->next;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_2dimen_u8_array_by_key(cJSON *json, ot_u8 *data, ot_s32 size_x, ot_s32 size_y, const char *str_key)
{
    ot_u32  cnt_x,cnt_y;
    cJSON *array_1d, *array_2d, *array_3d;

    array_1d = cJSON_GetObjectItemCaseSensitive(json, str_key);
    check_null_type_return(array_1d,cJSON_Array);
    cnt_x  = cJSON_GetArraySize(array_1d);

    array_2d = array_1d->child;
    for (int i = 0; i < cnt_x; i++) {
        check_null_type_return(array_2d, cJSON_Array);
        cnt_y  = cJSON_GetArraySize(array_2d);

        array_3d = array_2d->child;
        for (int j = 0; j < cnt_y; j++) {
            check_null_type_return(array_3d, cJSON_Number);
            *(data + i*size_y + j) =  array_3d->valueint;
            array_3d = array_3d->next;
        }
        array_2d = array_2d->next;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_u8_by_key(const cJSON *item, ot_u8 *data, const char *str_key)
{
    cJSON *param = cJSON_GetObjectItemCaseSensitive(item, str_key);
    check_null_type_return(param, cJSON_Number);
    *data = param->valueint;
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_u16_by_key(const cJSON *item, ot_u16 *data, const char *str_key)
{
    cJSON *param = cJSON_GetObjectItemCaseSensitive(item, str_key);
    check_null_type_return(param,cJSON_Number);
    *data = param->valueint;
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_u32_by_key(const cJSON *item, ot_u32 *data,const char *str_key)
{
    cJSON *param = cJSON_GetObjectItemCaseSensitive(item, str_key);
    check_null_type_return(param, cJSON_Number);
    *data = param->valueint;
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_float_by_key(const cJSON *item, ot_float *data, const char *str_key)
{
    cJSON *param = cJSON_GetObjectItemCaseSensitive(item, str_key);
    check_null_type_return(param, cJSON_Number);
    *data = param->valuedouble;
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_string_by_key(const cJSON *item, ot_u32 *data, const char *str_key)
{
    cJSON *param = cJSON_GetObjectItemCaseSensitive(item, str_key);
    check_null_type_return(param, cJSON_String);
    *data = strtol(param->valuestring, NULL, 16);
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_str_by_key(const cJSON *item, char *data, const char *str_key)
{
    cJSON *param = cJSON_GetObjectItemCaseSensitive(item, str_key);
    check_null_type_return(param, cJSON_String);
    strcpy(data, param->valuestring);
    return OT_SUCCESS;
}

static ot_s32 sample_comm_vi_parse_1dimen_u16_string_by_key(cJSON *json, const char *str_key, ot_u32 size_x,
    ot_u16 *data)
{
    ot_u32  cnt_x;
    cJSON *array_1d, *array_2d;

    array_1d = cJSON_GetObjectItemCaseSensitive(json, str_key);
    check_null_type_return(array_1d, cJSON_Array);
    cnt_x  = cJSON_GetArraySize(array_1d);
    if(cnt_x > size_x) {
        cnt_x = size_x;
    }

    array_2d = array_1d->child;
    for (int i = 0; i < cnt_x; i++) {
        check_null_type_return(array_2d, cJSON_String);
        *(data + i) = strtol(array_2d->valuestring, NULL, 16);
        array_2d = array_2d->next;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_key(const cJSON *json, const char *str_key, ot_eis_vi_sns_alg_key *alg_key)
{
    ot_s32 ret;
    cJSON *item = OT_NULL;
    ot_u32  tmp_key;

    item =  json_get_object(json, str_key);
    check_null_return(item);

    ret = sample_comm_parse_string_by_key(item,(ot_u32*)&tmp_key, "g_key");
    check_ret_return(ret);
    alg_key->key = tmp_key;

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_drc(const cJSON *json, const char *str_key, ot_eis_vi_drc_attr *drc)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json, str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item, "g_cmos_drc");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->enable, "enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&drc->curve_select, "curve_select");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->bright_gain_limit, "bright_gain_limit");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->bright_gain_limit_step, "bright_gain_limit_step");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->dark_gain_limit_luma, "dark_gain_limit_luma");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->dark_gain_limit_chroma, "dark_gain_limit_chroma");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->contrast_ctrl, "contrast_ctrl");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->rim_reduction_strength, "rim_reduction_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->rim_reduction_threshold, "rim_reduction_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&drc->color_correction_lut,
            OT_EIS_VI_DRC_CC_NODE_NUM,"color_correction_lut");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&drc->tone_mapping_value,
            OT_EIS_VI_DRC_TM_NODE_NUM,"tone_mapping_value");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->spatial_filter_coef,"spatial_filter_coef");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->range_filter_coef,"range_filter_coef");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->detail_adjust_coef,"detail_adjust_coef");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_1_level,(ot_u8*)&drc->local_mixing_bright,
            OT_EIS_VI_DRC_LMIX_NODE_NUM,"local_mixing_bright");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_1_level,(ot_u8*)&drc->local_mixing_dark,
            OT_EIS_VI_DRC_LMIX_NODE_NUM,"local_mixing_dark");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->high_saturation_color_ctrl,"high_saturation_color_ctrl");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->global_color_ctrl,"global_color_ctrl");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&drc->shoot_reduction_en,"shoot_reduction_en");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&drc->op_type,"op_type");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level, "manual_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&drc->manual_attr.strength,"manual_attr_strength");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level, "auto_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&drc->auto_attr.strength,"auto_attr_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&drc->auto_attr.strength_max,"auto_attr_strength_max");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&drc->auto_attr.strength_min,"auto_attr_strength_min");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level, "asymmetry_curve");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&drc->asymmetry_curve.asymmetry,"asymmetry_curve_asymmetry");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&drc->asymmetry_curve.second_pole,"asymmetry_curve_second_pole");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&drc->asymmetry_curve.stretch,"asymmetry_curve_stretch");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&drc->asymmetry_curve.compress,"asymmetry_curve_compress");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_demosaic(const cJSON *json,const char *str_key, ot_eis_vi_demosaic_attr *demosaic)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_demosaic");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&demosaic->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&demosaic->op_type,"op_type");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&demosaic->ai_detail_strength,"ai_detail_strength");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"manual_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&demosaic->manual_attr.nddm_strength,"nddm_strength");
    check_ret_return(ret);
    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&demosaic->manual_attr.nddm_mf_detail_strength,"nddm_mf_detail_strength");
    check_ret_return(ret);
    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&demosaic->manual_attr.nddm_hf_detail_strength,"nddm_hf_detail_strength");
    check_ret_return(ret);
    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&demosaic->manual_attr.detail_smooth_range,"detail_smooth_range");
    check_ret_return(ret);
    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&demosaic->manual_attr.color_noise_f_threshold,"color_noise_f_threshold");
    check_ret_return(ret);
    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&demosaic->manual_attr.color_noise_f_strength,"color_noise_f_strength");
    check_ret_return(ret);
    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&demosaic->manual_attr.color_noise_y_threshold,"color_noise_y_threshold");
    check_ret_return(ret);
    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&demosaic->manual_attr.color_noise_y_strength,"color_noise_y_strength");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"auto_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&demosaic->auto_attr.nddm_strength[0],
        OT_EIS_VI_AUTO_ISO_NUM,"nddm_strength");
    check_ret_return(ret);
    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&demosaic->auto_attr.nddm_mf_detail_strength[0],
        OT_EIS_VI_AUTO_ISO_NUM,"nddm_mf_detail_strength");
    check_ret_return(ret);
    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&demosaic->auto_attr.nddm_hf_detail_strength[0],
        OT_EIS_VI_AUTO_ISO_NUM,"nddm_hf_detail_strength");
    check_ret_return(ret);
    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&demosaic->auto_attr.detail_smooth_range[0],
        OT_EIS_VI_AUTO_ISO_NUM,"detail_smooth_range");
    check_ret_return(ret);
    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&demosaic->auto_attr.color_noise_f_threshold[0],
        OT_EIS_VI_AUTO_ISO_NUM,"color_noise_f_threshold");
    check_ret_return(ret);
    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&demosaic->auto_attr.color_noise_f_strength[0],
        OT_EIS_VI_AUTO_ISO_NUM,"color_noise_f_strength");
    check_ret_return(ret);
    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&demosaic->auto_attr.color_noise_y_threshold[0],
        OT_EIS_VI_AUTO_ISO_NUM,"color_noise_y_threshold");
    check_ret_return(ret);
    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&demosaic->auto_attr.color_noise_y_strength[0],
        OT_EIS_VI_AUTO_ISO_NUM,"color_noise_y_strength");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_gamma(const cJSON *json,const char *str_key,ot_eis_vi_gamma_attr *gamma)
{
    ot_s32 ret;
    cJSON *item, *item_1_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_gamma");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&gamma->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&gamma->table[0],
        OT_EIS_VI_GAMMA_NODE_NUM,"table");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&gamma->curve_type,"curve_type");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_yuv_sharpen(const cJSON *json,const char *str_key,ot_eis_vi_sharpen_attr *yuv_sharpen)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_yuv_sharpen");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level, (ot_u8*)&yuv_sharpen->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level, (ot_u8*)&yuv_sharpen->motion_en,"motion_en");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&yuv_sharpen->motion_threshold0,"motion_threshold0");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&yuv_sharpen->motion_threshold1,"motion_threshold1");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&yuv_sharpen->motion_gain0,"motion_gain0");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&yuv_sharpen->motion_gain1,"motion_gain1");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&yuv_sharpen->skin_umin,"skin_umin");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&yuv_sharpen->skin_vmin,"skin_vmin");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&yuv_sharpen->skin_umax,"skin_umax");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&yuv_sharpen->skin_vmax,"skin_vmax");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&yuv_sharpen->op_type,"op_type");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&yuv_sharpen->detail_map,"detail_map");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"manual_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.luma_wgt,
        OT_EIS_VI_SHARPEN_LUMA_NUM,"manual_attr_luma_wgt");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->manual_attr.texture_strength,
        OT_EIS_VI_SHARPEN_GAIN_NUM,"manual_attr_texture_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->manual_attr.edge_strength,
        OT_EIS_VI_SHARPEN_GAIN_NUM,"manual_attr_edge_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&yuv_sharpen->manual_attr.texture_freq,"manual_attr_texture_freq");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&yuv_sharpen->manual_attr.edge_freq,"manual_attr_edge_freq");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.over_shoot,"manual_attr_over_shoot");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.under_shoot,"manual_attr_under_shoot");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->manual_attr.motion_texture_strength,
        OT_EIS_VI_SHARPEN_GAIN_NUM,"manual_attr_motion_texture_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->manual_attr.motion_edge_strength,
        OT_EIS_VI_SHARPEN_GAIN_NUM,"manual_attr_motion_edge_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&yuv_sharpen->manual_attr.motion_texture_freq,"manual_attr_motion_texture_freq");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&yuv_sharpen->manual_attr.motion_edge_freq,"manual_attr_motion_edge_freq");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.motion_over_shoot,"manual_attr_motion_over_shoot");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.motion_under_shoot,"manual_attr_motion_under_shoot");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.shoot_sup_strength,"manual_attr_u8shoot_sup_str");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.shoot_sup_adj,"manual_attr_u8shoot_sup_adj");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.detail_ctrl,"manual_attr_detail_ctrl");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.detail_ctrl_threshold,"manual_attr_detail_ctrl_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.edge_filt_strength,"manual_attr_edge_filt_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.edge_filt_max_cap,"manual_attr_edge_filt_max_cap");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.r_gain,"manual_attr_r_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.g_gain,"manual_attr_g_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.b_gain,"manual_attr_b_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&yuv_sharpen->manual_attr.skin_gain,"manual_attr_skin_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&yuv_sharpen->manual_attr.max_sharp_gain,"manual_attr_max_sharp_gain");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"auto_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_2dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.luma_wgt,OT_EIS_VI_SHARPEN_LUMA_NUM,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_luma_wgt");
    check_ret_return(ret);

    ret = sample_comm_parse_2dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->auto_attr.texture_strength,OT_EIS_VI_SHARPEN_GAIN_NUM,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_texture_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_2dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->auto_attr.edge_strength,OT_EIS_VI_SHARPEN_GAIN_NUM,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_edge_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->auto_attr.texture_freq,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_texture_freq");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->auto_attr.edge_freq,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_edge_freq");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.over_shoot,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_over_shoot");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.under_shoot,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_under_shoot");
    check_ret_return(ret);

    ret = sample_comm_parse_2dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->auto_attr.motion_texture_strength,OT_EIS_VI_SHARPEN_GAIN_NUM,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_motion_texture_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_2dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->auto_attr.motion_edge_strength,OT_EIS_VI_SHARPEN_GAIN_NUM,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_motion_edge_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->auto_attr.motion_texture_freq,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_motion_texture_freq");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->auto_attr.motion_edge_freq,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_motion_edge_freq");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.motion_over_shoot,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_motion_over_shoot");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.motion_under_shoot,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_motion_under_shoot");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.shoot_sup_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_shoot_sup_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.shoot_sup_adj,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_shoot_sup_adj");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.detail_ctrl,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_detail_ctrl");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.detail_ctrl_threshold,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_detail_ctrl_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.edge_filt_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_edge_filt_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.edge_filt_max_cap,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_edge_filt_max_cap");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.r_gain,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_r_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.g_gain,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_g_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.b_gain,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_b_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&yuv_sharpen->auto_attr.skin_gain,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_skin_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&yuv_sharpen->auto_attr.max_sharp_gain,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_max_sharp_gain");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_ldci(const cJSON *json,const char *str_key, ot_eis_vi_ldci_attr *ldci)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level, *item_3_level, *item_4_level, *item_5_level, *item_6_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_ldci");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&ldci->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&ldci->gauss_lpf_sigma,"gauss_lpf_sigma");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&ldci->op_type,"op_type");
    check_ret_return(ret);

    ot_u8 coef_attr[3] = {0};
    item_2_level =  json_get_object(item_1_level,"manual_attr");
    check_null_return(item_2_level);
    item_3_level =  json_get_object(item_2_level,"he_wgt");
    check_null_return(item_3_level);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level,(ot_u8*)&coef_attr[0],
        3,"manual_attr_he_wgt_he_pos_wgt");
    check_ret_return(ret);
    ldci->manual_attr.he_wgt.he_pos_wgt.wgt = coef_attr[0];
    ldci->manual_attr.he_wgt.he_pos_wgt.sigma = coef_attr[1];
    ldci->manual_attr.he_wgt.he_pos_wgt.mean = coef_attr[2];

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level,(ot_u8*)&coef_attr,
        3,"manual_attr_he_wgt_he_neg_wgt");
    check_ret_return(ret);
    ldci->manual_attr.he_wgt.he_neg_wgt.wgt = coef_attr[0];
    ldci->manual_attr.he_wgt.he_neg_wgt.sigma = coef_attr[1];
    ldci->manual_attr.he_wgt.he_neg_wgt.mean = coef_attr[2];

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&ldci->manual_attr.blc_ctrl,"manual_attr_blc_ctrl");
    check_ret_return(ret);

    ot_u8 he_wgt[OT_EIS_VI_AUTO_ISO_NUM*2][3] = {0};
    item_2_level =  json_get_object(item_1_level,"auto_attr");
    check_null_return(item_2_level);

    item_3_level = cJSON_GetObjectItemCaseSensitive(item_2_level, "he_wgt");
    check_null_type_return(item_3_level,cJSON_Array);
    int cnt_x  = cJSON_GetArraySize(item_3_level);
    if(cnt_x  > OT_EIS_VI_AUTO_ISO_NUM) {
        cnt_x  = OT_EIS_VI_AUTO_ISO_NUM;
    }
    item_4_level = item_3_level->child;
    for( int i = 0; i < cnt_x; i++) {
        item_5_level = item_4_level->child;
        for(int j = 0; j < 2; j++) {
            item_6_level = item_5_level->child;
            for(int k = 0; k < 3; k++){
                he_wgt[i*2+j][k] = item_6_level->valueint;
                item_6_level = item_6_level->next;
            }
            item_5_level = item_5_level->next;
        }
        item_4_level = item_4_level->next;
    }

    for(int i = 0; i <OT_EIS_VI_AUTO_ISO_NUM; i++ ) {
        ldci->auto_attr.he_wgt[i].he_pos_wgt.wgt = he_wgt[i*2][0];
        ldci->auto_attr.he_wgt[i].he_pos_wgt.sigma = he_wgt[i*2][1];
        ldci->auto_attr.he_wgt[i].he_pos_wgt.mean = he_wgt[i*2][2];

        ldci->auto_attr.he_wgt[i].he_neg_wgt.wgt = he_wgt[i*2+1][0];
        ldci->auto_attr.he_wgt[i].he_neg_wgt.sigma = he_wgt[i*2+1][1];
        ldci->auto_attr.he_wgt[i].he_neg_wgt.mean = he_wgt[i*2+1][2];
    }

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&ldci->auto_attr.blc_ctrl,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_blc_ctrl");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&ldci->tpr_incr_coef,"tpr_incr_coef");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&ldci->tpr_decr_coef,"tpr_decr_coef");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_dpc(const cJSON *json,const char *str_key, ot_eis_vi_dpc_dynamic_cor_attr *dpc)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level, *item_3_level, *item_4_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_dpc");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level, (ot_u8*)&dpc->enable, "enable");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"frame_dynamic");
    check_null_return(item_2_level);

    char frame_str[32] = "\0";
    for(int i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        sprintf(frame_str, "frame_dynamic%d", i);
        item_3_level =  json_get_object(item_2_level, frame_str);
        check_null_return(item_3_level);

        ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&dpc->frame_dynamic[i].sup_twinkle_en,
            "sup_twinkle_en");
        check_ret_return(ret);

        ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&dpc->frame_dynamic[i].soft_thr,
        "soft_thr");
        check_ret_return(ret);

        ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&dpc->frame_dynamic[i].soft_slope,
            "soft_slope");
        check_ret_return(ret);

        ret = sample_comm_parse_u32_by_key(item_3_level, (ot_u32*)&dpc->frame_dynamic[i].op_type,
            "op_type");
        check_ret_return(ret);

        item_4_level =  json_get_object(item_3_level,"manual_attr");
        check_null_return(item_4_level);

        ret = sample_comm_parse_u8_by_key(item_4_level, (ot_u8*)&dpc->frame_dynamic[i].manual_attr.strength,
            "manual_attr_strength");
        check_ret_return(ret);

        ret = sample_comm_parse_u8_by_key(item_4_level, (ot_u8*)&dpc->frame_dynamic[i].manual_attr.blend_ratio,
            "manual_attr_blend_ratio");
        check_ret_return(ret);

        item_4_level =  json_get_object(item_3_level,"auto_attr");
        check_null_return(item_4_level);

        ret = sample_comm_parse_1dimen_u8_array_by_key(item_4_level, (ot_u8*)&dpc->frame_dynamic[i].auto_attr.strength[0],
                OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_strength");
        check_ret_return(ret);

        ret = sample_comm_parse_1dimen_u8_array_by_key(item_4_level, (ot_u8*)&dpc->frame_dynamic[i].auto_attr.blend_ratio[0],
            OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_blend_ratio");
        check_ret_return(ret);

        ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&dpc->frame_dynamic[i].bright_strength,
            "bright_strength");
        check_ret_return(ret);

        ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&dpc->frame_dynamic[i].dark_strength,
            "dark_strength");
        check_ret_return(ret);
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_lsc(const cJSON *json,const char *str_key,ot_eis_vi_sns_lsc *lsc)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level, *item_3_level, *item_4_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_lsc");
    check_null_return(item_1_level);

    item_2_level =  json_get_object(item_1_level,"lsc_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&lsc->lsc_attr.enable,"shading_enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&lsc->lsc_attr.mesh_strength,"shading_mesh_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&lsc->lsc_attr.blend_ratio,"shading_blend_ratio");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"lsc_lut");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&lsc->lsc_lut.mesh_scale,"shading_lut_mesh_scale");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&lsc->lsc_lut.x_grid_width,
            OT_EIS_VI_MLSC_X_HALF_GRID_NUM,"shading_lut_x_grid_width");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&lsc->lsc_lut.y_grid_width,
            OT_EIS_VI_MLSC_Y_HALF_GRID_NUM,"shading_lut_y_grid_width");
    check_ret_return(ret);

    item_3_level =  json_get_object(item_2_level,"lsc_gain_lut");
    check_null_return(item_3_level);
    item_4_level =  json_get_object(item_3_level,"lsc_gain_lut0");
    check_null_return(item_4_level);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_4_level,(ot_u16*)&lsc->lsc_lut.lsc_gain_lut[0].r_gain,
            OT_EIS_VI_LSC_GRID_POINTS,"shading_lut_r_gain_idx0");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_4_level,(ot_u16*)&lsc->lsc_lut.lsc_gain_lut[0].gr_gain,
            OT_EIS_VI_LSC_GRID_POINTS,"shading_lut_gr_gain_idx0");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_4_level,(ot_u16*)&lsc->lsc_lut.lsc_gain_lut[0].gb_gain,
            OT_EIS_VI_LSC_GRID_POINTS,"shading_lut_gb_gain_idx0");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_4_level,(ot_u16*)&lsc->lsc_lut.lsc_gain_lut[0].b_gain,
            OT_EIS_VI_LSC_GRID_POINTS,"shading_lut_b_gain_idx0");
    check_ret_return(ret);

    item_4_level =  json_get_object(item_3_level,"lsc_gain_lut1");
    check_null_return(item_4_level);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_4_level,(ot_u16*)&lsc->lsc_lut.lsc_gain_lut[1].r_gain,
            OT_EIS_VI_LSC_GRID_POINTS,"shading_lut_r_gain_idx1");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_4_level,(ot_u16*)&lsc->lsc_lut.lsc_gain_lut[1].gr_gain,
            OT_EIS_VI_LSC_GRID_POINTS,"shading_lut_gr_gain_idx1");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_4_level,(ot_u16*)&lsc->lsc_lut.lsc_gain_lut[1].gb_gain,
            OT_EIS_VI_LSC_GRID_POINTS,"shading_lut_gb_gain_idx1");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_4_level,(ot_u16*)&lsc->lsc_lut.lsc_gain_lut[1].b_gain,
            OT_EIS_VI_LSC_GRID_POINTS,"shading_lut_b_gain_idx1");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_ge(const cJSON *json,const char *str_key,ot_eis_vi_cr_attr *ge)
{
    ot_s32 ret;
    cJSON *item, *item_1_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_ge");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&ge->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&ge->slope,"slope");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&ge->sensi_slope,"sensi_slope");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&ge->sensi_threshold,"sensi_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&ge->strength,
        OT_EIS_VI_AUTO_ISO_NUM,"strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&ge->threshold,
        OT_EIS_VI_AUTO_ISO_NUM,"threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_1_level,(ot_u8*)&ge->filter_mode,
        OT_EIS_VI_AUTO_ISO_NUM,"filter_mode");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&ge->np_offset,
        OT_EIS_VI_AUTO_ISO_NUM,"np_offset");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_anti_false_color(const cJSON *json,const char *str_key,ot_eis_vi_anti_false_color_attr *color)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_anti_false_color");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&color->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&color->op_type,"op_type");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"manual_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&color->manual_attr.threshold,"threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&color->manual_attr.strength,"strength");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"auto_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&color->auto_attr.threshold,
        OT_EIS_VI_AUTO_ISO_NUM,"threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&color->auto_attr.strength,
        OT_EIS_VI_AUTO_ISO_NUM,"strength");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_cac(const cJSON *json,const char *str_key,ot_eis_vi_cac_attr  *cac)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level, *item_3_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_cac");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&cac->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&cac->op_type,"op_type");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&cac->detect_mode,"detect_mode");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&cac->purple_upper_limit,"purple_upper_limit");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&cac->purple_lower_limit,"purple_lower_limit");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"acac_cfg");
    check_null_return(item_2_level);
    item_3_level =  json_get_object(item_2_level,"acac_manual");
    check_null_return(item_3_level);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_manual.edge_threshold,
        OT_EIS_VI_CAC_THR_NUM,"acac_manual_edge_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_manual.edge_gain,"acac_manual_edge_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_manual.cac_rb_strength,"acac_manual_cac_rb_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_manual.purple_alpha,"acac_manual_purple_alpha");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_manual.edge_alpha,"acac_manual_edge_alpha");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_manual.satu_low_threshold,"acac_manual_satu_low_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_manual.satu_high_threshold,"acac_manual_satu_high_threshold");
    check_ret_return(ret);

    item_3_level =  json_get_object(item_2_level,"acac_auto");
    check_null_return(item_3_level);

    ret = sample_comm_parse_2dimen_u16_array_by_key(item_3_level,(ot_u16 *)&cac->acac_cfg.acac_auto.edge_threshold[0][0],OT_EIS_VI_CAC_THR_NUM,
        OT_EIS_VI_AUTO_ISO_NUM,"acac_auto_edge_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_auto.edge_gain[0],
        OT_EIS_VI_AUTO_ISO_NUM,"acac_auto_edge_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_auto.cac_rb_strength[0],
        OT_EIS_VI_AUTO_ISO_NUM,"acac_auto_cac_rb_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_auto.purple_alpha[0],
        OT_EIS_VI_AUTO_ISO_NUM,"acac_auto_purple_alpha");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_auto.edge_alpha[0],
        OT_EIS_VI_AUTO_ISO_NUM,"acac_auto_edge_alpha");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_auto.satu_low_threshold[0],
        OT_EIS_VI_AUTO_ISO_NUM,"acac_auto_satu_low_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level,(ot_u16*)&cac->acac_cfg.acac_auto.satu_high_threshold[0],
        OT_EIS_VI_AUTO_ISO_NUM,"acac_auto_satu_high_threshold");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"lcac_cfg");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&cac->lcac_cfg.purple_detect_range,"purple_detect_range");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&cac->lcac_cfg.var_threshold,"var_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&cac->lcac_cfg.r_detect_threshold[0],
        OT_EIS_VI_CAC_CURVE_NUM,"r_detect_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&cac->lcac_cfg.g_detect_threshold[0],
        OT_EIS_VI_CAC_CURVE_NUM,"g_detect_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&cac->lcac_cfg.b_detect_threshold[0],
        OT_EIS_VI_CAC_CURVE_NUM,"b_detect_threshold");
    check_ret_return(ret);

    item_3_level =  json_get_object(item_2_level,"lcac_manual");
    check_null_return(item_3_level);

    ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&cac->lcac_cfg.lcac_manual.de_purple_cr_strength,
        "lcac_manual_de_purple_cr_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&cac->lcac_cfg.lcac_manual.de_purple_cb_strength,
        "lcac_manual_de_purple_cb_strength");
    check_ret_return(ret);

    item_3_level =  json_get_object(item_2_level,"lcac_auto");
    check_null_return(item_3_level);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level,(ot_u8*)&cac->lcac_cfg.lcac_auto.de_purple_cr_strength,
        OT_EIS_VI_CAC_EXP_RATIO_NUM,"lcac_auto_de_purple_cr_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level,(ot_u8*)&cac->lcac_cfg.lcac_auto.de_purple_cb_strength,
        OT_EIS_VI_CAC_EXP_RATIO_NUM,"lcac_auto_de_purple_cb_strength");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_bayershp(const cJSON *json,const char *str_key,ot_eis_vi_bayer_sharpen_attr *bayershp)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_bayershp");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&bayershp->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&bayershp->dark_threshold[0],
        OT_EIS_VI_BSHP_THD_NUM,"dark_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&bayershp->texture_threshold[0],
        OT_EIS_VI_BSHP_THD_NUM,"texture_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&bayershp->op_type,"op_type");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"manual_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayershp->manual_attr.mf_strength[0],
        OT_EIS_VI_BSHP_CURVE_NUM,"manual_attr_mf_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayershp->manual_attr.hf_strength[0],
        OT_EIS_VI_BSHP_CURVE_NUM,"manual_attr_hf_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayershp->manual_attr.dark_strength[0],
        OT_EIS_VI_BSHP_CURVE_NUM,"manual_attr_dark_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&bayershp->manual_attr.mf_gain,"manual_attr_mf_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&bayershp->manual_attr.hf_gain,"manual_attr_hf_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&bayershp->manual_attr.dark_gain,"manual_attr_dark_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&bayershp->manual_attr.overshoot,"manual_attr_overshoot");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&bayershp->manual_attr.undershoot,"manual_attr_undershoot");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"auto_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_2dimen_u8_array_by_key(item_2_level,(ot_u8 *)&bayershp->auto_attr.mf_strength[0][0],OT_EIS_VI_BSHP_CURVE_NUM,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_mf_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_2dimen_u8_array_by_key(item_2_level,(ot_u8 *)&bayershp->auto_attr.hf_strength[0][0],OT_EIS_VI_BSHP_CURVE_NUM,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_hf_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_2dimen_u8_array_by_key(item_2_level,(ot_u8 *)&bayershp->auto_attr.dark_strength[0][0],OT_EIS_VI_BSHP_CURVE_NUM,
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_dark_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayershp->auto_attr.mf_gain[0],
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_mf_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayershp->auto_attr.hf_gain[0],
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_hf_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayershp->auto_attr.dark_gain[0],
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_dark_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&bayershp->auto_attr.overshoot[0],
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_overshoot");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level,(ot_u16*)&bayershp->auto_attr.undershoot[0],
        OT_EIS_VI_AUTO_ISO_NUM,"auto_attr_undershoot");
    check_ret_return(ret);

    return OT_SUCCESS;
}
static ot_s32 sample_comm_parse_bayer_nr(const cJSON *json,const char *str_key,ot_eis_vi_nr_attr *bayer_nr)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level, *item_3_level, *item_4_level, *item_5_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_bayer_nr");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&bayer_nr->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&bayer_nr->op_type,"op_type");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&bayer_nr->tnr_en,"tnr_enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&bayer_nr->lsc_nr_en,"lsc_nr_enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&bayer_nr->lsc_ratio1,"lsc_ratio1");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&bayer_nr->coring_ratio,
        OT_EIS_VI_BAYERNR_LUT_LENGTH,"coring_ratio");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&bayer_nr->mix_gain,
        OT_EIS_VI_BAYERNR_LUT_LENGTH1,"mix_gain");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"snr_cfg");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u32_by_key(item_2_level,(ot_u32*)&bayer_nr->snr_cfg.snr_version,"snr_version");
    check_ret_return(ret);

    item_3_level =  json_get_object(item_2_level,"snr_attr");
    check_null_return(item_3_level);
    item_4_level =  json_get_object(item_3_level,"union");
    check_null_return(item_4_level);
    item_5_level =  json_get_object(item_4_level,"snr_auto");
    check_null_return(item_5_level);

    ret = sample_comm_parse_2dimen_u16_array_by_key(item_5_level,(ot_u16 *)&bayer_nr->snr_cfg.snr_attr.snr_auto.sfm0_coarse_strength,
        OT_EIS_VI_BAYER_CHN_NUM,OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_sfm0_coarse_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_auto.sfm0_detail_prot,
        OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_sfm0_detail_prot");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_5_level,(ot_u16*)&bayer_nr->snr_cfg.snr_attr.snr_auto.sfm1_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_sfm1_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_auto.sfm1_adp_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_sfm1_adp_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_auto.sfm6_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_sfm6_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_auto.sfm7_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_sfm7_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_auto.sth,
        OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_sth");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_auto.fine_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_fine_str");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_5_level,(ot_u16*)&bayer_nr->snr_cfg.snr_attr.snr_auto.coring_wgt,
        OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_coring_wgt");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_auto.coring_mot_ratio,
        OT_EIS_VI_AUTO_ISO_NUM,"snr_attr_snr_auto_coring_mot_ratio");
    check_ret_return(ret);

    item_5_level =  json_get_object(item_4_level,"snr_manual");
    check_null_return(item_5_level);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_5_level,(ot_u16*)&bayer_nr->snr_cfg.snr_attr.snr_manual.sfm0_coarse_strength,
        OT_EIS_VI_BAYER_CHN_NUM,"snr_attr_snr_manual_sfm0_coarse_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_manual.sfm0_detail_prot,
        "snr_attr_snr_manual_sfm0_detail_prot");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_5_level,(ot_u16*)&bayer_nr->snr_cfg.snr_attr.snr_manual.sfm1_strength,
        "snr_attr_snr_manual_sfm1_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_manual.sfm1_adp_strength,
        "snr_attr_snr_manual_sfm1_adp_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_manual.sfm6_strength,
        "snr_attr_snr_manual_sfm6_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_manual.sfm7_strength,
        "snr_attr_snr_manual_sfm7_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_manual.sth,
        "snr_attr_snr_manual_sth");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_manual.fine_strength,
        "snr_attr_snr_manual_fine_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_5_level,(ot_u16*)&bayer_nr->snr_cfg.snr_attr.snr_manual.coring_wgt,
        "snr_attr_snr_manual_coring_wgt");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_5_level,(ot_u8*)&bayer_nr->snr_cfg.snr_attr.snr_manual.coring_mot_ratio,
        "snr_attr_snr_manual_coring_mot_ratio");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"tnr_cfg");
    check_null_return(item_2_level);
    item_3_level =  json_get_object(item_2_level,"tnr_auto");
    check_null_return(item_3_level);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.md_mode,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_md_mode");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.md_size_ratio,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_md_size_ratio");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.md_anti_flicker_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_md_anti_flicker_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.md_static_ratio,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_md_static_ratio");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.md_motion_ratio,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_md_motion_ratio");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.md_static_fine_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_md_static_fine_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.tfs,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_tfs");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.user_define_md,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_user_define_md");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level, (ot_u16*)&bayer_nr->tnr_cfg.tnr_auto.user_define_slope,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_user_define_slope");
    check_ret_return(ret);

     ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level, (ot_u16*)&bayer_nr->tnr_cfg.tnr_auto.user_define_dark_thresh,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_user_define_dark_thresh");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.user_define_color_thresh,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_user_define_color_thresh");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.sfr_r,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_sfr_r");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.sfr_g,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_sfr_g");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_auto.sfr_b,
        OT_EIS_VI_AUTO_ISO_NUM,"tnr_auto_sfr_b");
    check_ret_return(ret);

    item_3_level =  json_get_object(item_2_level,"tnr_manual");
    check_null_return(item_3_level);

    ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.md_mode,
        "tnr_manual_md_mode");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.md_size_ratio,
        "tnr_manual_md_size_ratio");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.md_anti_flicker_strength,
        "tnr_manual_md_anti_flicker_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.md_static_ratio,
        "tnr_manual_md_static_ratio");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.md_motion_ratio,
        "tnr_manual_md_motion_ratio");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.md_static_fine_strength,
        "tnr_manual_md_static_fine_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.tfs,
        "tnr_manual_tfs");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.user_define_md,
        "tnr_manual_user_define_md");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_3_level,(ot_u16*)&bayer_nr->tnr_cfg.tnr_manual.user_define_slope,
        "tnr_manual_user_define_slope");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_3_level,(ot_u16*)&bayer_nr->tnr_cfg.tnr_manual.user_define_dark_thresh,
        "tnr_manual_user_define_dark_thresh");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.user_define_color_thresh,
        "tnr_manual_user_define_color_thresh");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.sfr_r,
        "tnr_manual_sfr_r");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.sfr_g,
        "tnr_manual_sfr_g");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->tnr_cfg.tnr_manual.sfr_b,
        "tnr_manual_sfr_b");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"wdr_cfg");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayer_nr->wdr_cfg.snr_sfm0_wdr_strength,
        OT_EIS_VI_WDR_MAX_FRAME_NUM,"snr_sfm0_wdr_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayer_nr->wdr_cfg.snr_sfm0_fusion_strength,
        OT_EIS_VI_WDR_MAX_FRAME_NUM,"snr_sfm0_fusion_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayer_nr->wdr_cfg.snr_wdr_sfm6_strength,
        OT_EIS_VI_WDR_MAX_FRAME_NUM,"snr_wdr_sfm6_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayer_nr->wdr_cfg.snr_wdr_sfm7_strength,
        OT_EIS_VI_WDR_MAX_FRAME_NUM,"snr_wdr_sfm7_strength");
    check_ret_return(ret);

     ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayer_nr->wdr_cfg.snr_fusion_sfm6_strength,
        OT_EIS_VI_WDR_MAX_FRAME_NUM,"snr_fusion_sfm6_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayer_nr->wdr_cfg.snr_fusion_sfm7_strength,
        OT_EIS_VI_WDR_MAX_FRAME_NUM,"snr_fusion_sfm7_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayer_nr->wdr_cfg.md_wdr_strength,
        OT_EIS_VI_WDR_MAX_FRAME_NUM,"md_wdr_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level,(ot_u8*)&bayer_nr->wdr_cfg.md_fusion_strength,
        OT_EIS_VI_WDR_MAX_FRAME_NUM,"md_fusion_strength");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"dering_cfg");
    check_null_return(item_2_level);
    item_3_level =  json_get_object(item_2_level,"dering_auto");
    check_null_return(item_3_level);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_3_level,(ot_u16*)&bayer_nr->dering_cfg.dering_auto.dering_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"dering_auto_dering_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level,(ot_u8*)&bayer_nr->dering_cfg.dering_auto.dering_thresh,
        OT_EIS_VI_AUTO_ISO_NUM,"dering_auto_dering_thresh");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level,(ot_u8*)&bayer_nr->dering_cfg.dering_auto.dering_static_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"dering_auto_dering_static_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_3_level,(ot_u8*)&bayer_nr->dering_cfg.dering_auto.dering_motion_strength,
        OT_EIS_VI_AUTO_ISO_NUM,"dering_auto_dering_motion_strength");
    check_ret_return(ret);

    item_3_level =  json_get_object(item_2_level,"dering_manual");
    check_null_return(item_3_level);

    ret = sample_comm_parse_u16_by_key(item_3_level,(ot_u16*)&bayer_nr->dering_cfg.dering_manual.dering_strength,
        "dering_manual_dering_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->dering_cfg.dering_manual.dering_thresh,
        "dering_manual_dering_thresh");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->dering_cfg.dering_manual.dering_static_strength,
        "dering_manual_dering_static_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level,(ot_u8*)&bayer_nr->dering_cfg.dering_manual.dering_motion_strength,
        "dering_manual_dering_motion_strength");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_expander(const cJSON *json,const char *str_key,ot_eis_vi_expander_attr *expander)
{
    ot_s32 ret;
    cJSON *item, *item_1_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_expander");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&expander->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&expander->bit_depth_in,"bit_depth_in");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&expander->bit_depth_out,"bit_depth_out");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&expander->knee_point_num,"knee_point_num");
    check_ret_return(ret);

    ot_u32 img_point[OT_EIS_VI_EXPANDER_POINT_NUM_MAX][2];

    ret = sample_comm_parse_2dimen_u32_array_by_key(item_1_level,(ot_u32 *)&img_point[0][0],OT_EIS_VI_EXPANDER_POINT_NUM_MAX,
        2,"knee_point_coord");
    check_ret_return(ret);

    for(int i = 0; i < OT_EIS_VI_EXPANDER_POINT_NUM_MAX;i++){
        expander->knee_point_coord[i].x = img_point[i][0];
        expander->knee_point_coord[i].y = img_point[i][1];
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_clut(const cJSON *json,const char *str_key,ot_eis_vi_sns_clut *clut)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_clut");
    check_null_return(item_1_level);

    item_2_level =  json_get_object(item_1_level,"clut_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&clut->clut_attr.enable,"clut_attr_enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_2_level,(ot_u32*)&clut->clut_attr.gain_r,"clut_attr_gain_r");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_2_level,(ot_u32*)&clut->clut_attr.gain_g,"clut_attr_gain_g");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_2_level,(ot_u32*)&clut->clut_attr.gain_b,"clut_attr_gain_b");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"clut_lut");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u32_array_by_key(item_2_level,(ot_u32*)&clut->clut_lut.lut,
            OT_EIS_VI_CLUT_LUT_LENGTH,"clut_attr_lut");
    check_ret_return(ret);

    return OT_SUCCESS;

}

static ot_s32 sample_comm_parse_wdr(const cJSON *json,const char *str_key,ot_eis_vi_wdr_attr *wdr)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level, *item_3_level, *item_4_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_wdr");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u32_by_key(item_1_level, (ot_u32*)&wdr->wdr_merge_mode,"wdr_merge_mode");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level, "wdr_combine_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level, (ot_u8*)&wdr->wdr_combine_attr.motion_comp,"combine_attr_motion_comp");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level, (ot_u16*)&wdr->wdr_combine_attr.short_threshold,"combine_attr_short_thr");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level, (ot_u16*)&wdr->wdr_combine_attr.long_threshold,"combine_attr_long_thr");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level, (ot_u8*)&wdr->wdr_combine_attr.force_long,"combine_attr_force_long");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&wdr->wdr_combine_attr.force_long_low_threshold,"combine_attr_force_long_low_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level, (ot_u16*)&wdr->wdr_combine_attr.force_long_hig_threshold,"combine_attr_force_long_hig_threshold");
    check_ret_return(ret);

    item_3_level =  json_get_object(item_2_level,"wdr_mdt");
    check_null_return(item_3_level);

    ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&wdr->wdr_combine_attr.wdr_mdt.short_expo_chk,"combine_attr_mdt_short_expo_chk");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_3_level, (ot_u16*)&wdr->wdr_combine_attr.wdr_mdt.short_check_threshold,"combine_attr_mdt_short_check_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&wdr->wdr_combine_attr.wdr_mdt.md_ref_flicker,"combine_attr_mdt_md_ref_flicker");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&wdr->wdr_combine_attr.wdr_mdt.mdt_still_threshold,"combine_attr_mdt_mdt_still_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&wdr->wdr_combine_attr.wdr_mdt.mdt_full_threshold,"combine_attr_mdt_mdt_full_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_3_level, (ot_u8*)&wdr->wdr_combine_attr.wdr_mdt.mdt_long_blend,"combine_attr_mdt_mdt_long_blend");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_3_level, (ot_u32*)&wdr->wdr_combine_attr.wdr_mdt.op_type,"combine_attr_mdt_op_type");
    check_ret_return(ret);

    item_4_level =  json_get_object(item_3_level,"manual_attr");
    check_null_return(item_4_level);

    ret = sample_comm_parse_u8_by_key(item_4_level, (ot_u8*)&wdr->wdr_combine_attr.wdr_mdt.manual_attr.md_thr_low_gain,"combine_attr_mdt_manual_low_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_4_level, (ot_u8*)&wdr->wdr_combine_attr.wdr_mdt.manual_attr.md_thr_hig_gain,"combine_attr_mdt_manual_high_gain");
    check_ret_return(ret);

    item_4_level =  json_get_object(item_3_level,"auto_attr");
    check_null_return(item_4_level);

    ret = sample_comm_parse_2dimen_u8_array_by_key(item_4_level, (ot_u8*)&wdr->wdr_combine_attr.wdr_mdt.auto_attr.md_thr_low_gain[0][0], OT_EIS_VI_WDR_RATIO_NUM,
            OT_EIS_VI_AUTO_ISO_NUM,"combine_attr_wdr_mdt_auto_low_gain");
    check_ret_return(ret);

    ret = sample_comm_parse_2dimen_u8_array_by_key(item_4_level, (ot_u8*)&wdr->wdr_combine_attr.wdr_mdt.auto_attr.md_thr_hig_gain[0][0], OT_EIS_VI_WDR_RATIO_NUM,
            OT_EIS_VI_AUTO_ISO_NUM,"combine_attr_wdr_mdt_auto_high_gain");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"fusion_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level, (ot_u8*)&wdr->fusion_attr.fusion_blend_en,"fusion_attr_blend_en");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level, (ot_u8*)&wdr->fusion_attr.fusion_blend_wgt,"fusion_attr_blend_wgt");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level, (ot_u16*)&wdr->fusion_attr.fusion_threshold,
            OT_EIS_VI_WDR_MAX_FRAME_NUM,"fusion_attr_threshold");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level, (ot_u8*)&wdr->fusion_attr.fusion_force_gray_en,"fusion_attr_force_gray_en");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level, (ot_u8*)&wdr->fusion_attr.fusion_force_blend_threshold,"fusion_attr_force_blend_threshold");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_dehaze(const cJSON *json, const char *str_key, ot_eis_vi_dehaze_attr *dehaze)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_dehaze");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level, (ot_u8*)&dehaze->enable,"enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_1_level, (ot_u8*)&dehaze->user_lut_en,"user_lut_en");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_1_level, (ot_u8*)&dehaze->dehaze_lut[0],
            OT_EIS_VI_DEHAZE_LUT_SIZE,"dehaze_lut");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&dehaze->op_type,"op_type");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"manual_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&dehaze->manual_attr.strength,"manual_attr_strength");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"auto_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level, (ot_u8*)&dehaze->auto_attr.strength, "auto_attr_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level, (ot_u16*)&dehaze->tmprflt_incr_coef, "tmprflt_incr_coef");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_1_level, (ot_u16*)&dehaze->tmprflt_decr_coef, "tmprflt_decr_coef");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_acs(const cJSON *json,const char *str_key,ot_eis_vi_cmos_acs *acs)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_acs");
    check_null_return(item_1_level);

    item_2_level =  json_get_object(item_1_level,"acs_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&acs->acs_attr.enable,"acs_attr_enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&acs->acs_attr.y_strength,"acs_attr_y_strength");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level,(ot_u16*)&acs->acs_attr.run_interval,"acs_attr_run_interval");
    check_ret_return(ret);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&acs->acs_attr.lock_enable,"acs_attr_lock_enable");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"acs_calib_param");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level, (ot_u16*)&acs->acs_calib_param.light_index,
            OT_EIS_VI_ACS_LIGHT_NUM * OT_EIS_VI_ACS_CHN_NUM, "acs_calib_light_index");
    check_ret_return(ret);

    ret = sample_comm_parse_float_by_key(item_2_level, (ot_float*)&acs->acs_calib_param.model_ar_min,"acs_calib_model_ar_min");
    check_ret_return(ret);

    ret = sample_comm_parse_float_by_key(item_2_level, (ot_float*)&acs->acs_calib_param.model_ar_step,"acs_calib_model_ar_step");
    check_ret_return(ret);

    ret = sample_comm_parse_float_by_key(item_2_level, (ot_float*)&acs->acs_calib_param.model_ab_min,"acs_calib_model_ab_min");
    check_ret_return(ret);

    ret = sample_comm_parse_float_by_key(item_2_level, (ot_float*)&acs->acs_calib_param.model_ab_step,"acs_calib_model_ab_step");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level, (ot_u16*)&acs->acs_calib_param.light_type_g_high,"acs_calib_light_type_g_high");
    check_ret_return(ret);

    ret = sample_comm_parse_u16_by_key(item_2_level, (ot_u16*)&acs->acs_calib_param.light_type_g_low,"acs_calib_light_type_g_low");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"acs_y_shading_lut");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level, (ot_u16*)&acs->acs_y_shading_lut.g_param_high_ct[0],
            OT_EIS_VI_LSC_GRID_POINTS, "acs_y_shading_lut_g_param_high_ct");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level, (ot_u16*)&acs->acs_y_shading_lut.g_param_low_ct[0],
            OT_EIS_VI_LSC_GRID_POINTS, "acs_y_shading_lut_g_param_low_ct");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"acs_color_shading_lut");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_float_array_by_key(item_2_level, (ot_float*)&acs->acs_color_shading_lut.avg_rg_map[0],
            OT_EIS_VI_LSC_GRID_POINTS,"acs_color_shading_lut_avg_rg_map");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_float_array_by_key(item_2_level, (ot_float*)&acs->acs_color_shading_lut.avg_bg_map[0],
            OT_EIS_VI_LSC_GRID_POINTS,"acs_color_shading_lut_avg_bg_map");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_float_array_by_key(item_2_level, (ot_float*)&acs->acs_color_shading_lut.prof_rg_map[0],
            OT_EIS_VI_LSC_GRID_POINTS,"acs_color_shading_lut_prof_rg_map");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_float_array_by_key(item_2_level, (ot_float*)&acs->acs_color_shading_lut.prof_bg_map[0],
            OT_EIS_VI_LSC_GRID_POINTS,"acs_color_shading_lut_prof_bg_map");
    check_ret_return(ret);

    return OT_SUCCESS;

}

static ot_s32 sample_comm_parse_blc(const cJSON *json, const char *str_key,ot_eis_vi_sns_black_level *blc)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_blc");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&blc->user_black_level_en,"user_black_level_en");
    check_ret_return(ret);

    ret = sample_comm_parse_2dimen_u16_array_by_key(item_1_level,(ot_u16 *)&blc->user_black_level[0][0],OT_EIS_VI_WDR_MAX_FRAME_NUM,
        OT_EIS_VI_BAYER_CHN_NUM,"user_black_level");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&blc->black_level_mode,"black_level_mode");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level,"manual_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_2dimen_u16_array_by_key(item_2_level,(ot_u16 *)&(blc->manual_attr.black_level),OT_EIS_VI_WDR_MAX_FRAME_NUM,
        OT_EIS_VI_BAYER_CHN_NUM,"black_level");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level, "auto_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level,(ot_u8*)&blc->auto_attr.update,"update");
    check_ret_return(ret);

    ret = sample_comm_parse_2dimen_u16_array_by_key(item_2_level,(ot_u16 *)&blc->auto_attr.black_level[0][0],OT_EIS_VI_WDR_MAX_FRAME_NUM,
        OT_EIS_VI_BAYER_CHN_NUM,"black_level");
    check_ret_return(ret);
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_ca(const cJSON *json, const char *str_key, ot_eis_vi_ca_attr *ca)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_cmos_ca");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&ca->enable, "enable");
    check_ret_return(ret);

    ret = sample_comm_parse_u32_by_key(item_1_level,(ot_u32*)&ca->ca_cp_en, "ca_cp_en");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level, "ca");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u32_array_by_key(item_2_level, (ot_u32*)&ca->ca.y_ratio_lut[0],
            OT_EIS_VI_CA_YRATIO_LUT_LENGTH, "y_ratio_lut");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u32_array_by_key(item_2_level, (ot_u32*)&ca->ca.iso_ratio[0],
            OT_EIS_VI_AUTO_ISO_NUM, "iso_ratio");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u32_array_by_key(item_2_level, (ot_u32*)&ca->ca.y_sat_lut[0],
            OT_EIS_VI_CA_YRATIO_LUT_LENGTH, "y_sat_lut");
    check_ret_return(ret);

    item_2_level =  json_get_object(item_1_level, "cp");
    check_null_return(item_2_level);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level, (ot_u8*)&ca->cp.cp_lut_y[0],
            OT_EIS_VI_CA_YRATIO_LUT_LENGTH, "cp_lut_y");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level, (ot_u8*)&ca->cp.cp_lut_u[0],
            OT_EIS_VI_CA_YRATIO_LUT_LENGTH, "cp_lut_u");
    check_ret_return(ret);

    ret = sample_comm_parse_1dimen_u8_array_by_key(item_2_level, (ot_u8*)&ca->cp.cp_lut_v[0],
            OT_EIS_VI_CA_YRATIO_LUT_LENGTH, "cp_lut_v");
    check_ret_return(ret);

    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_noise_calibration(const cJSON *json, const char *str_key, ot_eis_vi_noise_calibration *noise_calibration)
{
    ot_s32 ret;
    cJSON *item;
    ot_float  noise_tmp[OT_EIS_VI_BAYER_CALIBRATION_PARA_NUM];

    item =  json_get_object(json,str_key);
    check_null_return(item);

    ret = sample_comm_parse_1dimen_float_array_by_key(item,(ot_float*)&noise_tmp[0], OT_EIS_VI_BAYER_CALIBRATION_PARA_NUM, "g_cmos_noise_calibration");
    check_ret_return(ret);
    for(int i = 0; i < OT_EIS_VI_BAYER_CALIBRATION_PARA_NUM; i++) {
        noise_calibration->calibration_coef[i] = noise_tmp[i];
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_awb_agc_table(const cJSON *json, const char *str_key, ot_eis_vi_awb_agc_table *awb_agc_table)
{
    ot_s32 ret;
    cJSON *item = OT_NULL;

    item =  json_get_object(json,str_key);
    check_null_return(item);

    ret = sample_comm_parse_u8_by_key(item,(ot_u8*)&awb_agc_table->valid,"valid");
    check_ret_return(ret);
    ret = sample_comm_parse_1dimen_u8_array_by_key(item,(ot_u8*)&awb_agc_table->saturation,
        OT_EIS_VI_AUTO_ISO_NUM, "saturation");
    check_ret_return(ret);
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_awb_ccm(const cJSON *json, const char *str_key, ot_eis_vi_awb_ccm *ccm)
{
    ot_s32 ret;
    cJSON *item, *item_1_level, *item_2_level;

    item =  json_get_object(json,str_key);
    check_null_return(item);

    ret = sample_comm_parse_u16_by_key(item,(ot_u16*)&ccm->ccm_tab_num,"ccm_tab_num");
    check_ret_return(ret);

    char ccm_tab_str[32] = "\0";
    item_1_level =  json_get_object(item,"ccm_tab");
    check_null_return(item_1_level);

    for(int i = 0; i < OT_EIS_VI_CCM_MATRIX_NUM; i++) {

        sprintf(ccm_tab_str, "ccm_tab%d", i);
        item_2_level =  json_get_object(item_1_level, ccm_tab_str);
        check_null_return(item_2_level);

        ret = sample_comm_parse_u16_by_key(item_2_level, (ot_u16*)&ccm->ccm_tab[i].color_temp, "ccm_tab_color_temp");
        check_ret_return(ret);

        ret = sample_comm_vi_parse_1dimen_u16_string_by_key(item_2_level, "ccm_tab_color_ccm",
            OT_EIS_VI_CCM_MATRIX_SIZE,(ot_u16*)&ccm->ccm_tab[i].ccm[0]);
        check_ret_return(ret);
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_parse_sns_sensor_table(const cJSON *json, const char *str_key,
    ot_eis_sns_reg_table  *sns_table)
{
    cJSON *item,*array_1d, *array_2d;
    ot_eis_sns_reg_data  *table_tmp;

    item =  json_get_object(json,"ot_u16");
    check_null_return(item);

    array_1d =  cJSON_GetObjectItemCaseSensitive(item, str_key);
    check_null_type_return(array_1d, cJSON_Array);
    ot_s32 cnt = cJSON_GetArraySize(array_1d);

    sns_table->reg_data = OT_NULL;
    sns_table->reg_data = malloc(cnt * sizeof(ot_eis_sns_reg_data));
    check_null_return(sns_table->reg_data);
    sns_table->data_len = cnt/2;
    sns_table->write_flag = 1;

    array_2d = array_1d->child;
    for(int i = 0; i < cnt; i++) {
        table_tmp = sns_table->reg_data + i/2;
        check_null_type_return(array_2d,cJSON_String);
        if(i&1) {
            table_tmp->data = strtol(array_2d->valuestring, NULL, 16);
        } else {
            table_tmp->addr = strtol(array_2d->valuestring, NULL, 16);
        }
        array_2d = array_2d->next;
    }
    return OT_SUCCESS;
}


ot_s32 sample_comm_parse_ext_alg_param(const char * const filename,sample_sns_alg *sns_def)
{
    ot_s32 ret;
    cJSON *json = NULL;

    json = parse_file(filename);
    check_null_return(json);
    memset(sns_def, 0, sizeof(sample_sns_alg));

    ret = sample_comm_parse_key(json,"ot_eis_vi_sns_alg_key",&sns_def->key);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse key err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_drc(json,"ot_eis_vi_drc_attr",&sns_def->drc);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse drc err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_demosaic(json,"ot_eis_vi_demosaic_attr",&sns_def->demosaic);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse demosaic err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_gamma(json,"ot_eis_vi_gamma_attr",&sns_def->gamma);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse gamma err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_yuv_sharpen(json,"ot_eis_vi_sharpen_attr",&sns_def->sharpen);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse sharpen err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_ldci(json,"ot_eis_vi_ldci_attr",&sns_def->ldci);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse ldci err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_dpc(json,"ot_eis_vi_dpc_dynamic_cor_attr", &sns_def->dpc);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse dpc err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_lsc(json,"ot_eis_vi_sns_lsc",&sns_def->lsc);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse lsc err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_ge(json,"ot_eis_vi_cr_attr",&sns_def->ge);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse ge err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_anti_false_color(json,"ot_eis_vi_anti_false_color_attr",&sns_def->anti_false_color);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse anti_false_color err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_bayer_nr(json,"ot_eis_vi_nr_attr",&sns_def->bayer_nr);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse bayer_nr err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_expander(json,"ot_eis_vi_expander_attr",&sns_def->expander);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse expander err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_clut(json,"ot_eis_vi_sns_clut",&sns_def->clut);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse clut err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_wdr(json,"ot_eis_vi_wdr_attr",&sns_def->wdr);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse wdr err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_dehaze(json,"ot_eis_vi_dehaze_attr",&sns_def->dehaze);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse dehaze err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_cac(json,"ot_eis_vi_cac_attr",&sns_def->cac);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse cac err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_bayershp(json,"ot_eis_vi_bayer_sharpen_attr",&sns_def->bshp);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse bshp err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_acs(json, "ot_eis_vi_cmos_acs", &sns_def->acs);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse acs err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_blc(json,"ot_eis_vi_sns_black_level",&sns_def->sns_blc);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse sns_blc err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_ca(json, "ot_eis_vi_ca_attr", &sns_def->ca);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse ca err\n");
        goto alg_parse_failure;
    }

    ret = sample_comm_parse_noise_calibration(json,"ot_eis_vi_noise_calibration",&sns_def->noise_calibration);
    if (ret != OT_SUCCESS) {
        sample_print("alg_parse noise_calibration err\n");
        goto alg_parse_failure;
    }

    cJSON_Delete(json);
    return OT_SUCCESS;

alg_parse_failure:
    cJSON_Delete(json);
    return  OT_FAILURE;
}

ot_s32 sample_comm_parse_alg_param(const char * const filename, ot_eis_vi_sns_alg_default *alg, ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;

    ret = sample_comm_parse_ext_alg_param(filename, &g_ext_sns_alg[vi_pipe]);
    if (ret != OT_SUCCESS) {
        sample_print("sample_comm_parse_alg err");
        return OT_FAILURE;
    }
    alg->key =  g_ext_sns_alg[vi_pipe].key;
    alg->drc =  &g_ext_sns_alg[vi_pipe].drc;
    alg->demosaic =  &g_ext_sns_alg[vi_pipe].demosaic;
    alg->gamma =  &g_ext_sns_alg[vi_pipe].gamma;
    alg->sharpen =  &g_ext_sns_alg[vi_pipe].sharpen;
    alg->ldci =  &g_ext_sns_alg[vi_pipe].ldci;
    alg->dpc =  &g_ext_sns_alg[vi_pipe].dpc;
    alg->lsc =  &g_ext_sns_alg[vi_pipe].lsc;
    alg->ge =  &g_ext_sns_alg[vi_pipe].ge;
    alg->anti_false_color =  &g_ext_sns_alg[vi_pipe].anti_false_color;
    alg->bayer_nr =  &g_ext_sns_alg[vi_pipe].bayer_nr;
    alg->ca =  &g_ext_sns_alg[vi_pipe].ca;
    alg->expander =  &g_ext_sns_alg[vi_pipe].expander;
    alg->clut =  &g_ext_sns_alg[vi_pipe].clut;
    alg->wdr =  &g_ext_sns_alg[vi_pipe].wdr;
    alg->dehaze =  &g_ext_sns_alg[vi_pipe].dehaze;
    alg->cac =  &g_ext_sns_alg[vi_pipe].cac;
    alg->bshp =  &g_ext_sns_alg[vi_pipe].bshp;
    alg->acs =  &g_ext_sns_alg[vi_pipe].acs;
    alg->lblc =  &g_ext_sns_alg[vi_pipe].lblc;
    alg->sns_blc =  &g_ext_sns_alg[vi_pipe].sns_blc;
    alg->noise_calibration =  g_ext_sns_alg[vi_pipe].noise_calibration;

    return OT_SUCCESS;
}

ot_s32 sample_comm_parse_awb_param(const char * const filename,ot_eis_vi_awb_sensor_default *sns_awb)
{
    ot_s32 ret;
    cJSON *json = NULL;
    cJSON *item, *item_1_level, *item_2_level;

    memset(sns_awb, 0, sizeof(ot_eis_vi_awb_sensor_default));
    json = parse_file(filename);
    check_null_return(json);

    item =  json_get_object(json,"ot_eis_vi_awb_sensor_default");
    check_null_return(item);
    item_1_level =  json_get_object(item,"g_awb_cmos");
    check_null_return(item_1_level);

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&sns_awb->wb_ref_temp,"wb_ref_temp");
    if (ret != OT_SUCCESS) {
        sample_print("wb_ref_temp parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&sns_awb->gain_offset[0],
        OT_EIS_VI_BAYER_CHN_NUM, "gain_offset");
    if (ret != OT_SUCCESS) {
        sample_print("gain_offset parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_1dimen_u32_array_by_key(item_1_level,(ot_u32*)&sns_awb->wb_para[0],
        OT_EIS_VI_AWB_CURVE_PARA_NUM, "wb_para");
    if (ret != OT_SUCCESS) {
        sample_print("wb_para parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&sns_awb->golden_rgain,"golden_rgain");
    if (ret != OT_SUCCESS) {
        sample_print("golden_rgain parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&sns_awb->golden_bgain,"golden_bgain");
    if (ret != OT_SUCCESS) {
        sample_print("golden_bgain parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&sns_awb->sample_rgain,"sample_rgain");
    if (ret != OT_SUCCESS) {
        sample_print("sample_rgain parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&sns_awb->sample_bgain,"sample_bgain");
    if (ret != OT_SUCCESS) {
        sample_print("sample_bgain parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_awb_agc_table(item_1_level,"agc_tbl",&sns_awb->agc_tbl);
    if (ret != OT_SUCCESS) {
        sample_print("agc_tbl parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_awb_ccm(item_1_level,"ccm",&sns_awb->ccm);
    if (ret != OT_SUCCESS) {
        sample_print("ccm parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&sns_awb->init_rgain,"init_rgain");
    if (ret != OT_SUCCESS) {
        sample_print("init_rgain parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&sns_awb->init_ggain,"init_ggain");
    if (ret != OT_SUCCESS) {
        sample_print("init_ggain parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_u16_by_key(item_1_level,(ot_u16*)&sns_awb->init_bgain,"init_bgain");
    if (ret != OT_SUCCESS) {
        sample_print("init_bgain parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_u8_by_key(item_1_level,(ot_u8*)&sns_awb->awb_run_interval,"awb_run_interval");
    if (ret != OT_SUCCESS) {
        sample_print("awb_run_interval parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_1_level,(ot_u16*)&sns_awb->init_ccm[0],
        OT_EIS_VI_CCM_MATRIX_SIZE, "init_ccm");
    if (ret != OT_SUCCESS) {
        sample_print("init_ccm parse err\n");
        goto parse_failure;
    }

    item_2_level =  json_get_object(item_1_level,"preawb_attr");
    check_null_return(item_2_level);

    ret = sample_comm_parse_u8_by_key(item_2_level, (ot_u8*)&sns_awb->preawb_attr.enable, "preawb_attr_enable");
    if (ret != OT_SUCCESS) {
        sample_print("preawb_attr_enable parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_u8_by_key(item_2_level, (ot_u8*)&sns_awb->preawb_attr.awb_reverse_en, "preawb_attr_awb_reverse_en");
    if (ret != OT_SUCCESS) {
        sample_print("preawb_attr_awb_reverse_en parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_1dimen_u16_array_by_key(item_2_level, (ot_u16*)&sns_awb->preawb_attr.color_temp_array[0],
        OT_EIS_VI_AWB_PREAWB_LUT_SIZE, "preawb_attr_color_temp_array");
    if (ret != OT_SUCCESS) {
        sample_print("color_temp_array parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_1dimen_u32_array_by_key(item_2_level, (ot_u32*)&sns_awb->preawb_attr.ratio_r_array[0],
        OT_EIS_VI_AWB_PREAWB_LUT_SIZE, "preawb_attr_ratio_r_array");
    if (ret != OT_SUCCESS) {
        sample_print("ratio_r_array parse err\n");
        goto parse_failure;
    }

    ret = sample_comm_parse_1dimen_u32_array_by_key(item_2_level, (ot_u32*)&sns_awb->preawb_attr.ratio_b_array[0],
        OT_EIS_VI_AWB_PREAWB_LUT_SIZE, "preawb_attr_ratio_b_array");
    if (ret != OT_SUCCESS) {
        sample_print("ratio_b_array parse err\n");
        goto parse_failure;
    }

    cJSON_Delete(json);
    return OT_SUCCESS;
parse_failure:
    cJSON_Delete(json);
    return  OT_FAILURE;
}

ot_s32 sample_comm_parse_sensor_table_param(const char * const filename, ot_eis_sns_reg_table  *sns_table, ot_vi_pipe_id vi_pipe)
{
    ot_s32 ret;
    cJSON *json = NULL;

    json = parse_file(filename);
    check_null_return(json);

    ot_eis_sns_reg_table  *dkshd_table = &g_ext_sns_table[vi_pipe][0];
    ot_eis_sns_reg_table  *adj_table   = &g_ext_sns_table[vi_pipe][1];
    ot_eis_sns_reg_table  *pxshd_table = &g_ext_sns_table[vi_pipe][2];
    ot_eis_sns_reg_table  *awb_table   = &g_ext_sns_table[vi_pipe][3];
    ot_eis_sns_reg_table  *fme_table   = &g_ext_sns_table[vi_pipe][4];

    ret = sample_comm_parse_sns_sensor_table(json, "sensor_dkshd[][2]", dkshd_table);
    if(ret == OT_FAILURE ) {
       sample_print("g_sensor_seq parse err\n");
       goto seq_failure;
    }

    ret = sample_comm_parse_sns_sensor_table(json, "sensor_adj[][2]", adj_table);
    if(ret == OT_FAILURE ) {
       sample_print("g_sensor_seq parse err\n");
       goto adj_failure;
    }

    ret = sample_comm_parse_sns_sensor_table(json, "sensor_pxshd[][2]", pxshd_table);
    if(ret == OT_FAILURE ) {
       sample_print("g_sensor_seq parse err\n");
       goto pxshd_failure;
    }

    ret = sample_comm_parse_sns_sensor_table(json, "sensor_awb[][2]", awb_table);
    if(ret == OT_FAILURE ) {
       sample_print("g_sensor_seq parse err\n");
       goto awb_failure;
    }

    ret = sample_comm_parse_sns_sensor_table(json, "sensor_fme[][2]",fme_table);
    if(ret == OT_FAILURE ) {
       sample_print("g_sensor_seq parse err\n");
       goto fme_failure;
    }
    (ot_void)memcpy_s(sns_table, 5*sizeof(ot_eis_sns_reg_table),
                      &g_ext_sns_table[vi_pipe][0], 5*sizeof(ot_eis_sns_reg_table));
    return OT_SUCCESS;

fme_failure:
    if (fme_table->reg_data != OT_NULL) {
        free(fme_table->reg_data);
        fme_table->reg_data = OT_NULL;
    }
awb_failure:
    if (awb_table->reg_data != OT_NULL) {
        free(awb_table->reg_data);
        awb_table->reg_data = OT_NULL;
    }
pxshd_failure:
    if(pxshd_table->reg_data != OT_NULL) {
        free(pxshd_table->reg_data);
        pxshd_table->reg_data = OT_NULL;
    }
adj_failure:
    if(adj_table->reg_data != OT_NULL) {
        free(adj_table->reg_data);
        adj_table->reg_data = OT_NULL;
    }
seq_failure:
    if(dkshd_table->reg_data != OT_NULL){
        free(dkshd_table->reg_data);
        dkshd_table->reg_data = OT_NULL;
    }
    cJSON_Delete(json);
    return  OT_FAILURE;

}

ot_void sample_comm_vi_json_sensor_table_free(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_reg_table  *dkshd_table = &g_ext_sns_table[vi_pipe][0];
    ot_eis_sns_reg_table  *adj_table   = &g_ext_sns_table[vi_pipe][1];
    ot_eis_sns_reg_table  *pxshd_table = &g_ext_sns_table[vi_pipe][2];
    ot_eis_sns_reg_table  *awb_table   = &g_ext_sns_table[vi_pipe][3];
    ot_eis_sns_reg_table  *fme_table   = &g_ext_sns_table[vi_pipe][4];

    if (fme_table->reg_data != OT_NULL) {
        free(fme_table->reg_data);
        fme_table->reg_data = OT_NULL;
    }

    if (awb_table->reg_data != OT_NULL) {
        free(awb_table->reg_data);
        awb_table->reg_data = OT_NULL;
    }

    if (pxshd_table->reg_data != OT_NULL) {
        free(pxshd_table->reg_data);
        pxshd_table->reg_data = OT_NULL;
    }

    if (adj_table->reg_data != OT_NULL) {
        free(adj_table->reg_data);
        adj_table->reg_data = OT_NULL;
    }

    if(dkshd_table->reg_data != OT_NULL) {
        free(dkshd_table->reg_data);
        dkshd_table->reg_data = OT_NULL;
    }

}

ot_s32 sample_comm_get_tuning_file(ot_vi_pipe_id vi_pipe, char *alg_file, char *awb_file, char *table_file)
{
    ot_s32 ret;
    cJSON *json, *item;

    json = parse_file("./data/sensor_tuning_cfg.json");
    check_null_return(json);

    switch (vi_pipe) {
        case 0:
            item =  json_get_object(json, "vi_pipe0");
            break;
        case 1:
            item =  json_get_object(json, "vi_pipe1");
            break;
        case 2:
            item =  json_get_object(json, "vi_pipe2");
            break;
        case 3:
            item =  json_get_object(json, "vi_pipe3");
            break;
        case 4:
            item =  json_get_object(json, "vi_pipe4");
            break;
        case 5:
            item =  json_get_object(json, "vi_pipe5");
            break;
        case 6:
            item =  json_get_object(json, "vi_pipe6");
            break;
        case 7:
            item =  json_get_object(json, "vi_pipe7");
            break;
        default :
            printf("no json file return\n");
            cJSON_Delete(json);
            return OT_FAILURE;
            break;
    }
    if(item == OT_NULL ) {
        sample_print("vi_pipe [%d] node  parse failer\n",vi_pipe );
        goto parse_fail;
    }
    ret = sample_comm_parse_str_by_key(item, alg_file, "sns_alg_def_file");
    if(ret != OT_SUCCESS){
        sample_print("sns_alg_def_file parse failer\n");
        goto parse_fail;
    }

    ret = sample_comm_parse_str_by_key(item, awb_file, "sns_awb_def_file");
    if(ret != OT_SUCCESS){
        sample_print("sns_alg_def_file parse failer\n");
        goto parse_fail;
    }

    ret = sample_comm_parse_str_by_key(item, table_file, "sns_table_def_file");
    if(ret != OT_SUCCESS){
        sample_print("sns_alg_def_file parse failer\n");
        goto parse_fail;
    }
    cJSON_Delete(json);
    return  OT_SUCCESS;

parse_fail:
    cJSON_Delete(json);
    return  OT_FAILURE;
}
