/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ot_eis_type.h"
#include "securec.h"
#include "ini.h"
#include "ot_eis_list.h"
#include "ot_scenecomm.h"
#include "ot_confaccess.h"
#include "ot_eis_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* redefine module name */
#ifdef OT_MODULE
#undef OT_MODULE
#endif
#define OT_MODULE "CONFACCESS"

#define CONFACCESS_DEFAULT_MODULE_CFGPATH "./" /* <default module cfg path */
#define CONFACCESS_PATH_SEPARATOR '/'          /* <path separator */

/*  module node information */
typedef struct {
    struct ot_list_head list;
    ot_char name[OT_CONFACCESS_NAME_MAX_LEN];     /* <module name */
    ot_char path[OT_CONFACCESS_PATH_MAX_LEN];     /* <module path */
    ot_char ini_file[OT_CONFACCESS_PATH_MAX_LEN]; /* <module ini filename, not included path */
    ini_dictionary *ini_dir;
} conf_access_module;

/* cfg node information */
typedef struct {
    struct ot_list_head list;
    ot_char cfg_name[OT_CONFACCESS_NAME_MAX_LEN];
    ot_char cfg_path[OT_CONFACCESS_PATH_MAX_LEN];
    conf_access_module module;
    pthread_mutex_t mutex_lock;
} conf_access_node;

/* cfg list */
static struct ot_list_head g_config_access_list = OT_LIST_HEAD_INIT(g_config_access_list);

/* configure count in list */
static ot_s32 g_conf_access_count = 0;

static ot_void confaccess_del_module_list(conf_access_node *cfg_node)
{
    struct ot_list_head *pos = NULL;
    conf_access_module *module = NULL;
    ot_s32 module_cnt = 0;

    ot_list_for_each(pos, &(cfg_node->module.list)) {
        ot_list_del(pos);
        module = ot_list_entry(pos, conf_access_module, list);
        scene_logd("Module: inifile[%s]\n", module->ini_file);
        module_cnt++;
        free_ini_info_dict(module->ini_dir);
        module->ini_dir = NULL;
        ot_scenecomm_safe_free(module);
        pos = &(cfg_node->module.list);
    }
    scene_logd("CfgName[%s] ModuleCnt[%d]\n", cfg_node->cfg_name, module_cnt);
}

static ot_s32 confaccess_get_all_modules_info(ot_s32 module_num, conf_access_node *cfg_pos, const ot_char *cfg_path_buf)
{
    ot_s32 module_cnt = 0;
    for (ot_s32 index = 0; index < module_num; ++index) {
        /* Malloc ModuleNode Memory */
        conf_access_module *module_info = (conf_access_module *)malloc(sizeof(conf_access_module));
        if (module_info == NULL) {
            scene_loge("malloc failed\n");
            return OT_CONFACCESS_EMALLOC;
        }

        /* Module Name */
        ot_char module_node_key[OT_CONFACCESS_KEY_MAX_LEN] = {0};
        snprintf_truncated_s(module_node_key, OT_CONFACCESS_KEY_MAX_LEN, "module:module%d", index + 1);
        const ot_char *value = ini_get_string(cfg_pos->module.ini_dir, (const char *)module_node_key, NULL);
        if (value == NULL) {
            scene_loge("Load %s failed\n", module_node_key);
            ot_scenecomm_safe_free(module_info);
            continue;
        }
        snprintf_truncated_s(module_info->name, OT_CONFACCESS_NAME_MAX_LEN, "%s", value);

        /* Module Path */
        snprintf_truncated_s(module_info->path, OT_CONFACCESS_PATH_MAX_LEN, "%s/", cfg_path_buf);

        if ((strlen(module_info->path) > 1) &&
            ((module_info->path[strlen(module_info->path) - 1]) == CONFACCESS_PATH_SEPARATOR)) {
            module_info->path[strlen(module_info->path) - 1] = '\0';
        }

        /* Module IniFile */
        snprintf_truncated_s(module_node_key, OT_CONFACCESS_KEY_MAX_LEN, "%s:cfg_filename", module_info->name);
        value = ini_get_string(cfg_pos->module.ini_dir, (const char *)module_node_key, NULL);
        if (value == NULL) {
            scene_loge("Load %s failed\n", module_node_key);
            ot_scenecomm_safe_free(module_info);
            continue;
        }

        snprintf_truncated_s(module_info->ini_file, OT_CONFACCESS_PATH_MAX_LEN, "%s/%s", module_info->path, value);

        /* Module IniDir */
        module_info->ini_dir = ini_process_file(module_info->ini_file);
        if (module_info->ini_dir == NULL) {
            scene_loge("Load %s failed, ini errorId %#x\n", module_info->ini_file, ini_get_error_id());
            ot_scenecomm_safe_free(module_info);
            continue;
        }

        /* Add ModuleNode to list */
        ot_list_add(&(module_info->list), &(cfg_pos->module.list));
        module_cnt++;
    }
    scene_logd("New CfgName[%s] ModuleCnt[%d]\n", cfg_pos->cfg_name, module_cnt);

    return OT_SUCCESS;
}

static ot_s32 confaccess_init(const ot_char *cfg_name, const ot_char *cfg_path, ot_u32 *out_module_num)
{
    ot_char cfg_path_buf[OT_CONFACCESS_PATH_MAX_LEN];
    /* Malloc CfgNode Memory */
    conf_access_node *cfg_pos = (conf_access_node *)calloc(1, sizeof(conf_access_node));
    ot_scenecomm_check_pointer_return(cfg_pos, OT_CONFACCESS_EMALLOC);

    /* Record CfgNode Information */
    snprintf_truncated_s(cfg_pos->cfg_name, OT_CONFACCESS_NAME_MAX_LEN, "%s", cfg_name);
    snprintf_truncated_s(cfg_pos->cfg_path, OT_CONFACCESS_PATH_MAX_LEN, "%s", cfg_path);
    ot_mutex_init_lock(cfg_pos->mutex_lock);
    ot_mutex_lock(cfg_pos->mutex_lock);
    cfg_pos->module.list.next = &(cfg_pos->module.list);
    cfg_pos->module.list.prev = &(cfg_pos->module.list);
    cfg_pos->module.ini_dir = ini_process_file(cfg_path);
    if (cfg_pos->module.ini_dir == NULL) {
        scene_loge("load %s failed\n", cfg_path);
        goto exit;
    }

    snprintf_truncated_s(cfg_pos->module.ini_file, OT_CONFACCESS_PATH_MAX_LEN, "%s", cfg_path);
    snprintf_truncated_s(cfg_path_buf, OT_CONFACCESS_PATH_MAX_LEN, "%s", cfg_path);
    dirname(cfg_path_buf);

    /* Get Module Count and Default Path */
    *out_module_num = (ot_u32)ini_get_int(cfg_pos->module.ini_dir, (ot_char *)"module:module_num", 0);
    scene_logd("ModuleNum[%d], DefaultPath[%s]\n", (*out_module_num), cfg_path_buf);

    /* Get All Module Information */
    ot_s32 ret = confaccess_get_all_modules_info((*out_module_num), cfg_pos, (const char *)cfg_path_buf);
    if (ret != OT_SUCCESS) {
        confaccess_del_module_list(cfg_pos);
        free_ini_info_dict(cfg_pos->module.ini_dir);
        goto exit;
    }
    ot_mutex_unlock(cfg_pos->mutex_lock);

    /* Add CfgNode to list */
    ot_list_add(&(cfg_pos->list), &(g_config_access_list));

    g_conf_access_count++;
    scene_logd("CfgName[%s] ModuleCnt[%d] Count[%d]\n", cfg_pos->cfg_name, *out_module_num, g_conf_access_count);
    return OT_SUCCESS;

exit:
    ot_mutex_unlock(cfg_pos->mutex_lock);
    ot_mutex_destroy(cfg_pos->mutex_lock);
    ot_scenecomm_safe_free(cfg_pos);
    return OT_FAILURE;
}

ot_s32 ot_confaccess_init(const ot_char *cfg_name, const ot_char *cfg_path, ot_u32 *module_num)
{
    ot_scenecomm_check_pointer_return(cfg_name, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_pointer_return(cfg_path, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(strlen(cfg_name) < OT_CONFACCESS_NAME_MAX_LEN, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(strlen(cfg_path) < OT_CONFACCESS_PATH_MAX_LEN, OT_CONFACCESS_EINVAL);

    /* Check CfgName Exist or not */
    struct ot_list_head *pos = NULL;
    conf_access_node *cfg_node = NULL;
    ot_list_for_each(pos, &(g_config_access_list)) {
        cfg_node = ot_list_entry(pos, conf_access_node, list);
        if (strncmp(cfg_node->cfg_name, cfg_name, OT_CONFACCESS_NAME_MAX_LEN) == 0) {
            scene_logw("%s already be inited\n", cfg_name);
            return OT_CONFACCESS_EREINIT;
        }
    }

    /* Init Cfg */
    if (g_conf_access_count == 0) {
        g_config_access_list.next = &(g_config_access_list);
        g_config_access_list.prev = &(g_config_access_list);
    }

    ot_s32 ret = confaccess_init(cfg_name, cfg_path, module_num);
    ot_scenecomm_check_return_with_errinfo(ret, ret, "AddCfg");
    return OT_SUCCESS;
}

ot_s32 ot_confaccess_deinit(const ot_char *cfg_name)
{
    ot_scenecomm_check_pointer_return(cfg_name, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(strlen(cfg_name) < OT_CONFACCESS_NAME_MAX_LEN, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(g_conf_access_count > 0, OT_CONFACCESS_ENOTINIT);

    struct ot_list_head *node_pos = NULL;
    struct ot_list_head *module_pos = NULL;
    conf_access_node *cfg_node = NULL;
    conf_access_module *cfg_module = NULL;
    ot_s32 module_cnt = 0;

    ot_list_for_each(node_pos, &(g_config_access_list)) {
        cfg_node = ot_list_entry(node_pos, conf_access_node, list);
        if (strncmp(cfg_node->cfg_name, cfg_name, OT_CONFACCESS_NAME_MAX_LEN) == 0) {
            ot_mutex_lock(cfg_node->mutex_lock);
            ot_list_del(node_pos);
            ot_list_for_each(module_pos, &(cfg_node->module.list)) {
                ot_list_del(module_pos);
                cfg_module = ot_list_entry(module_pos, conf_access_module, list);
                module_cnt++;
                free_ini_info_dict(cfg_module->ini_dir);
                cfg_module->ini_dir = NULL;
                ot_scenecomm_safe_free(cfg_module);
                module_pos = &(cfg_node->module.list);
            }
            scene_logd("CfgName[%s] ModuleCnt[%d]\n", cfg_node->cfg_name, module_cnt);
            free_ini_info_dict(cfg_node->module.ini_dir);
            cfg_node->module.ini_dir = NULL;
            ot_mutex_unlock(cfg_node->mutex_lock);
            ot_mutex_destroy(cfg_node->mutex_lock);
            ot_scenecomm_safe_free(cfg_node);

            node_pos = &(g_config_access_list);
            g_conf_access_count--;
            scene_logd("Now CfgList count[%d]\n", g_conf_access_count);
            return OT_SUCCESS;
        }
    }
    return OT_CONFACCESS_ECFG_NOTEXIST;
}

ot_s32 ot_confaccess_get_string(const ot_char *cfg_name, const ot_char *module, const ot_char *conf_item,
    const ot_char *default_val, ot_char ** const value_vector)
{
    ot_scenecomm_check_pointer_return(cfg_name, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_pointer_return(module, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_pointer_return(conf_item, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_pointer_return(value_vector, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(strlen(cfg_name) < OT_CONFACCESS_NAME_MAX_LEN, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(strlen(module) < OT_CONFACCESS_NAME_MAX_LEN, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(strlen(conf_item) < OT_CONFACCESS_KEY_MAX_LEN, OT_CONFACCESS_EINVAL);

    struct ot_list_head *node_pos = NULL;
    struct ot_list_head *module_pos = NULL;
    conf_access_node *cfg_node = NULL;
    conf_access_module *cfg_module = NULL;

    ot_list_for_each(node_pos, &(g_config_access_list)) {
        cfg_node = ot_list_entry(node_pos, conf_access_node, list);
        if (strncmp(cfg_node->cfg_name, cfg_name, OT_CONFACCESS_NAME_MAX_LEN) != 0) {
            continue;
        }
        ot_mutex_lock(cfg_node->mutex_lock);
        ot_list_for_each(module_pos, &(cfg_node->module.list)) {
            cfg_module = ot_list_entry(module_pos, conf_access_module, list);
            if (strncmp(cfg_module->name, module, OT_CONFACCESS_NAME_MAX_LEN) != 0) {
                continue;
            }
            *value_vector = (ot_char *)ini_get_string(cfg_module->ini_dir, conf_item, default_val);
            if (*value_vector != NULL) {
                *value_vector = strdup(*value_vector);
            }
            ot_mutex_unlock(cfg_node->mutex_lock);
            return OT_SUCCESS;
        }
        ot_mutex_unlock(cfg_node->mutex_lock);
        return OT_CONFACCESS_EMOD_NOTEXIST;
    }
    return OT_CONFACCESS_ECFG_NOTEXIST;
}

ot_s32 ot_confaccess_get_str(const ot_char *cfg_name, const ot_char *module, const ot_char *conf_item,
    ot_char **value_vector)
{
    return ot_confaccess_get_string(cfg_name, module, conf_item, NULL, value_vector);
}

ot_s32 ot_confaccess_get_int(const ot_char *cfg_name, const ot_char *module, const ot_char *conf_item,
    ot_s32 default_val, ot_s32 * const out_value)
{
    ot_scenecomm_check_pointer_return(out_value, OT_CONFACCESS_EINVAL);

    ot_char *value = NULL;
    ot_s32 ret = ot_confaccess_get_string(cfg_name, module, conf_item, NULL, &value);
    ot_scenecomm_check_return(ret, ret);

    if (value == NULL) {
        *out_value = default_val;
    } else {
        *out_value = (ot_s32)strtol(value, NULL, 10); /* base 10 */
        ot_scenecomm_safe_free(value);
    }
    return OT_SUCCESS;
}

ot_s32 ot_confaccess_get_long_long(const ot_char *cfg_name, const ot_char *module, const ot_char *conf_item,
    ot_s32 default_val, ot_s64 * const out_value)
{
    ot_scenecomm_check_pointer_return(out_value, OT_CONFACCESS_EINVAL);

    ot_char *value = NULL;
    ot_s32 ret;
    ret = ot_confaccess_get_string(cfg_name, module, conf_item, NULL, &value);
    ot_scenecomm_check_return(ret, ret);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    if (value == NULL) {
        *out_value = (long long)default_val;
    } else {
        *out_value = strtoll(value, NULL, 10); /* base 10 */
        ot_scenecomm_safe_free(value);
    }

    return ret;
}


ot_s32 ot_confaccess_get_double(const ot_char *cfg_name, const ot_char *module, const ot_char *conf_item,
    ot_double default_val, ot_double * const out_value)
{
    ot_scenecomm_check_pointer_return(out_value, OT_CONFACCESS_EINVAL);

    ot_char *value = NULL;
    ot_s32 ret = ot_confaccess_get_string(cfg_name, module, conf_item, NULL, &value);
    ot_scenecomm_check_return(ret, ret);

    if (value == NULL) {
        *out_value = default_val;
    } else {
        *out_value = strtof(value, NULL);
        ot_scenecomm_safe_free(value);
    }
    return OT_SUCCESS;
}


ot_s32 ot_confaccess_get_bool(const ot_char *cfg_name, const ot_char *module, const ot_char *conf_item,
    ot_bool default_val, ot_bool *out_value)
{
    ot_scenecomm_check_pointer_return(out_value, OT_CONFACCESS_EINVAL);

    ot_char *value = NULL;
    ot_s32 ret = ot_confaccess_get_string(cfg_name, module, conf_item, NULL, &value);
    ot_scenecomm_check_return(ret, ret);

    if (value == NULL) {
        *out_value = default_val;
    } else {
        if ((value[0] == 'y') || (value[0] == 'Y') || (value[0] == '1') || (value[0] == 't') || (value[0] == 'T')) {
            *out_value = OT_TRUE;
        } else if ((value[0] == 'n') || (value[0] == 'N') || (value[0] == '0') || (value[0] == 'f') ||
            value[0] == 'F') {
            *out_value = OT_FALSE;
        } else {
            *out_value = default_val;
        }
        ot_scenecomm_safe_free(value);
    }
    return OT_SUCCESS;
}

ot_s32 ot_confaccess_set_string(const ot_char *cfg_name, const ot_char *module, const ot_char *conf_item,
    const ot_char *value)
{
    ot_scenecomm_check_pointer_return(cfg_name, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_pointer_return(module, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_pointer_return(conf_item, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_pointer_return(value, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(strlen(cfg_name) < OT_CONFACCESS_NAME_MAX_LEN, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(strlen(module) < OT_CONFACCESS_NAME_MAX_LEN, OT_CONFACCESS_EINVAL);
    ot_scenecomm_check_expr_return(strlen(conf_item) < OT_CONFACCESS_KEY_MAX_LEN, OT_CONFACCESS_EINVAL);

    struct ot_list_head *node_pos = NULL;
    struct ot_list_head *module_pos = NULL;
    conf_access_node *cfg_node = NULL;
    conf_access_module *cfg_module = NULL;

    ot_list_for_each(node_pos, &(g_config_access_list)) {
        cfg_node = ot_list_entry(node_pos, conf_access_node, list);
        if (strncmp(cfg_node->cfg_name, cfg_name, OT_CONFACCESS_NAME_MAX_LEN) != 0) {
            continue;
        }
        ot_mutex_lock(cfg_node->mutex_lock);
        ot_list_for_each(module_pos, &(cfg_node->module.list)) {
            cfg_module = ot_list_entry(module_pos, conf_access_module, list);
            if (strncmp(cfg_module->name, module, OT_CONFACCESS_NAME_MAX_LEN) != 0) {
                continue;
            }
            if (set_val_for_dict(cfg_module->ini_dir, conf_item, value) != 0) {
                scene_loge("module[%s] confitem[%s] not exist\n", module, conf_item);
                ot_mutex_unlock(cfg_node->mutex_lock);
                return OT_CONFACCESS_EITEM_NOTEXIST;
            }
            ot_mutex_unlock(cfg_node->mutex_lock);
            return OT_SUCCESS;
        }
        ot_mutex_unlock(cfg_node->mutex_lock);
        return OT_CONFACCESS_EMOD_NOTEXIST;
    }
    return OT_CONFACCESS_ECFG_NOTEXIST;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
