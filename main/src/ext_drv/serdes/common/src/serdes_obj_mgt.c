#include <string.h>
#include <stdlib.h>
#include "serdes_obj_mgt.h"
#include "vi_serdes_drv.h"

typedef struct {
    const ot_serdes_obj *serdes_obj;
    struct ot_list_head objs_list;
} ot_serdes_obj_node;

typedef struct {
    ot_bool is_inited;
    struct ot_list_head objs_head;
    pthread_mutex_t objs_lock;
} ot_serdes_obj_mgt;

static ot_serdes_obj_mgt g_serdes_obj_mgt[OT_SERDES_TYPE_BUTT] = {
    [OT_SERDES_TYPE_VI] = {
        .is_inited      = OT_FALSE,
        .objs_lock      = PTHREAD_MUTEX_INITIALIZER,
    },
    [OT_SERDES_TYPE_VO] = {
        .is_inited      = OT_FALSE,
        .objs_lock      = PTHREAD_MUTEX_INITIALIZER,
    }
};

static ot_serdes_obj_node *serdes_match_serdes_obj_node(ot_serdes_obj_mgt *serdes_obj_mgt, const ot_s8 *serdes_name)
{
    struct ot_list_head *list_node = OT_NULL;
    ot_serdes_obj_node *obj_node = OT_NULL;

    if (ot_list_empty(&(serdes_obj_mgt->objs_head))) {
        return OT_NULL;
    }
    ot_list_for_each(list_node, &(serdes_obj_mgt->objs_head)) {
        obj_node = ot_list_entry(list_node, ot_serdes_obj_node, objs_list);
        if (strcmp(obj_node->serdes_obj->serdes_name, serdes_name) == 0) {
            return obj_node;
        }
    }
    return OT_NULL;
}

ot_s32 serdes_obj_register(ot_serdes_type serdes_type, const ot_serdes_obj *serdes_obj)
{
    ot_serdes_obj_mgt *serdes_obj_mgt = &g_serdes_obj_mgt[serdes_type];
    ot_serdes_obj_node *obj_node = OT_NULL;

    serdes_lock(serdes_obj_mgt->objs_lock);
    if (serdes_obj_mgt->is_inited == OT_FALSE) {
        serdes_obj_mgt->is_inited = OT_TRUE;
        OT_INIT_LIST_HEAD(&(serdes_obj_mgt->objs_head));
    }

    // find serdes obj
    obj_node = serdes_match_serdes_obj_node(serdes_obj_mgt, serdes_obj->serdes_name);
    serdes_unlock(serdes_obj_mgt->objs_lock);
    if (obj_node != OT_NULL) {
        serdes_err("The serdes obj had registered %s\n", obj_node->serdes_obj->serdes_name);
        return OT_EIS_ERRNO_ILLEGAL_PARAM;
    }

    obj_node = (ot_serdes_obj_node *)malloc(sizeof(ot_serdes_obj_node));
    if (obj_node == OT_NULL) {
        serdes_err("malloc error\n");
        return OT_EIS_ERRNO_NO_MEM;
    }
    memset(obj_node, 0, sizeof(ot_serdes_obj_node));
    obj_node->serdes_obj = serdes_obj;

    serdes_lock(serdes_obj_mgt->objs_lock);
    ot_list_add(&(obj_node->objs_list), &(serdes_obj_mgt->objs_head));
    serdes_unlock(serdes_obj_mgt->objs_lock);

    return OT_SUCCESS;
}

ot_s32 serdes_obj_unregister(ot_serdes_type serdes_type, const ot_serdes_obj *serdes_obj)
{
    ot_serdes_obj_mgt *serdes_obj_mgt = &g_serdes_obj_mgt[serdes_type];
    ot_serdes_obj_node *obj_node = OT_NULL;

    // find serdes obj
    serdes_lock(serdes_obj_mgt->objs_lock);
    obj_node = serdes_match_serdes_obj_node(serdes_obj_mgt, serdes_obj->serdes_name);
    if (obj_node == OT_NULL) {
        serdes_unlock(serdes_obj_mgt->objs_lock);
        serdes_err("The serdes obj hasn't registered %s\n", obj_node->serdes_obj->serdes_name);
        return OT_EIS_ERRNO_NULL_PTR;
    }
    ot_list_del(&(obj_node->objs_list));
    free(obj_node);
    serdes_unlock(serdes_obj_mgt->objs_lock);
    return OT_SUCCESS;
}

const ot_serdes_obj *serdes_obj_get_obj(ot_serdes_type serdes_type, const char *serdes_name)
{
    ot_serdes_obj_mgt *serdes_obj_mgt = &g_serdes_obj_mgt[serdes_type];
    ot_serdes_obj_node *obj_node = OT_NULL;

    // find serdes obj
    serdes_lock(serdes_obj_mgt->objs_lock);
    obj_node = serdes_match_serdes_obj_node(serdes_obj_mgt, serdes_name);
    if (obj_node == OT_NULL) {
        serdes_unlock(serdes_obj_mgt->objs_lock);
        serdes_err("The serdes obj hasn't registered %s\n", serdes_name);
        return OT_NULL;
    }
    serdes_unlock(serdes_obj_mgt->objs_lock);
    return obj_node->serdes_obj;
}