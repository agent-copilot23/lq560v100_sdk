/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#include <unistd.h>
#include "ot_bc.h"
#include "sample_utils.h"
#include "sample_upgrade_config.h"
#include "sample_upgrade.h"
#include <libxml/parser.h>

#define PARTTION_INFO_NODE_NAME     "Partition_Info"
#define PART_NODE_NAME              "Part"
#define is_node(node, node_name) (strcmp((node)->name, (const ot_u8*)node_name) == 0)

static const ot_u8 *get_str(xmlNodePtr node, const char *field_name)
{

    xmlChar *res = NULL;
    if (xmlHasProp(node, field_name )) {
        res = xmlGetProp(node, field_name);
    }
    return (const ot_u8 *)res;
}

ot_s32 sample_upgrade_get_partinfo_imageinfo_from_config(ot_void *arg)
{
    ot_s32 ret = OT_FAILURE;

    xmlDocPtr doc = OT_NULL;
    ot_u32 idx = 0;
    ot_bc_slot next = 0;
    const char *config_path = UPGRADE_CONFIG_FILE;
    struct upgrade_control *control = (struct upgrade_control *)arg;
    struct upgrade_part *parts = OT_NULL;
    ot_s8 suffix[3] = {0};
    debug("start\n");

    CHECK_RET(arg == OT_NULL, ret);
    xmlInitParser();
    next = sample_upgrade_get_about_to_upgrade_slot(control);
    sample_upgrade_get_suffix(next, suffix, 3);
    control->upgrade_part_num = 0;
    parts = &control->parts[0];
    memset((ot_void *)parts, 0, sizeof(control->parts));
    doc = xmlParseFile(config_path);
    if (doc != OT_NULL) {
        xmlNodePtr  part_obj, part_info_obj;
        part_info_obj = xmlDocGetRootElement(doc);
        if (part_info_obj == OT_NULL) {
            ret =  OT_FAILURE;
            error("xml got root failed\n");
            goto out;
        }
        while (part_info_obj != OT_NULL) {
            if (is_node(part_info_obj, PARTTION_INFO_NODE_NAME)) {
                part_obj = part_info_obj->children;
                while (part_obj != OT_NULL) {
                    if (is_node(part_obj, PART_NODE_NAME)) {
                        const ot_s8 *part_name, *img_name;
                        part_name = get_str(part_obj, "PartitionName");
                        img_name = get_str(part_obj, "SelectFile");
                        if ((part_name != OT_NULL) && (img_name != OT_NULL)
                        && (strstr(part_name, suffix) != OT_NULL)) {
                            strncpy(parts[idx].part_name, part_name, NAME_LEN);
                            strncpy(parts[idx].image_name, img_name, NAME_LEN);
                            debug("parts[%d] %s %s\n", idx, part_name, img_name);
                            idx++;
                        }
                    }
                    part_obj = part_obj->next;
                }
                break;
            }
            part_info_obj = part_info_obj->next;
        }
        if (idx == 0) {
            error("no valid part in %s\n", config_path);
            ret = OT_FAILURE;
            goto out;
        }
        control->upgrade_part_num = idx;
        ret = OT_SUCCESS;
    } else {
        error("parse %s failed\n", config_path);
    }
    debug("ret:%d\n", ret);
out:
    xmlCleanupParser();
    return ret;
}