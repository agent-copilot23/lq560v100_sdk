#include <string.h>
#include <stdlib.h>
#include "serdes_ctx_mgt.h"
#include "serdes_obj_mgt.h"

typedef struct {
    ot_bool is_started;
    ot_s32 serdes_id;
    const ot_serdes_obj *serdes_obj;
    struct ot_list_head ctx_list;
    pthread_mutex_t node_lock;
} ot_serdes_ctx_node;

typedef struct {
    ot_bool is_inited;

    struct ot_list_head ctx_head;
    pthread_mutex_t ctx_lock;
} ot_serdes_ctx_mgt;

static ot_serdes_ctx_mgt g_serdes_ctx_mgt[OT_SERDES_TYPE_BUTT] = {
    [OT_SERDES_TYPE_VI] = {
        .is_inited      = OT_FALSE,
        .ctx_lock       = PTHREAD_MUTEX_INITIALIZER,
    },
    [OT_SERDES_TYPE_VO] = {
        .is_inited      = OT_FALSE,
        .ctx_lock       = PTHREAD_MUTEX_INITIALIZER,
    }
};

static ot_s32 serdes_match_serdes_ctx(ot_serdes_ctx_mgt *serdes_ctx_mgt, ot_serdes_ctx_node *ctx_node)
{
    struct ot_list_head *list_node = OT_NULL;
    serdes_lock(serdes_ctx_mgt->ctx_lock);
    ot_list_for_each(list_node, &(serdes_ctx_mgt->ctx_head)) {
        if (ot_list_entry(list_node, ot_serdes_ctx_node, ctx_list) == ctx_node) {
            serdes_unlock(serdes_ctx_mgt->ctx_lock);
            return OT_SUCCESS;
        }
    }
    serdes_unlock(serdes_ctx_mgt->ctx_lock);
    serdes_err("Hasn't create handle\n");
    return OT_EIS_ERRNO_NO_EXIST;
}

static ot_s32 serdes_get_serdes_ctx_num(ot_serdes_ctx_mgt *serdes_ctx_mgt)
{
    ot_s32 num = 0;
    struct ot_list_head *list_node = OT_NULL;

    serdes_lock(serdes_ctx_mgt->ctx_lock);
    ot_list_for_each(list_node, &(serdes_ctx_mgt->ctx_head)) {
        num++;
    }
    serdes_unlock(serdes_ctx_mgt->ctx_lock);
    return num;
}

ot_s32 serdes_ctx_init(ot_serdes_type serdes_type)
{
    ot_serdes_ctx_mgt *serdes_ctx_mgt = &g_serdes_ctx_mgt[serdes_type];

    OT_INIT_LIST_HEAD(&(serdes_ctx_mgt->ctx_head));
    return OT_SUCCESS;
}

ot_s32 serdes_ctx_deinit(ot_serdes_type serdes_type)
{
    return OT_SUCCESS;
}

ot_s32 serdes_ctx_create(ot_serdes_type serdes_type, ot_eis_handle *handle,
    const ot_s8 *serdes_name, const ot_serdes_attr *serdes_attr, ot_u32 max_level)
{
    ot_s32 ret, ctx_num;
    ot_serdes_ctx_mgt *serdes_ctx_mgt = &g_serdes_ctx_mgt[serdes_type];
    const ot_serdes_obj *serdes_obj = OT_NULL;
    ot_serdes_ctx_node *ctx_node = OT_NULL;

    ctx_num = serdes_get_serdes_ctx_num(serdes_ctx_mgt);
    if (ctx_num >= max_level) {
        serdes_err("The handle num is full.\n");
        return OT_EIS_ERRNO_OVER_SIZE;
    }

    serdes_obj = serdes_obj_get_obj(serdes_type, serdes_name);
    if (serdes_obj == OT_NULL) {
        serdes_err("get serdes obj failed, %s\n", serdes_name);
        return OT_EIS_ERRNO_NULL_PTR;
    }

    // malloc context
    ctx_node = (ot_serdes_ctx_node *)malloc(sizeof(ot_serdes_ctx_node));
    if (ctx_node == OT_NULL) {
        serdes_err("malloc error\n");
        return OT_EIS_ERRNO_NO_MEM;
    }
    memset(ctx_node, 0, sizeof(ot_serdes_ctx_node));
    ctx_node->serdes_obj = serdes_obj;
    ctx_node->is_started = OT_FALSE;
    serdes_lock_init(ctx_node->node_lock);

    serdes_lock(ctx_node->node_lock);
    if (ctx_node->serdes_obj && ctx_node->serdes_obj->create) {
        ret = ctx_node->serdes_obj->create(&(ctx_node->serdes_id), serdes_attr);
        if (ret != OT_SUCCESS) {
            serdes_unlock(ctx_node->node_lock);
            serdes_lock_deinit(ctx_node->node_lock);
            free(ctx_node);
            serdes_err("The handle create serdes id failed\n");
            return ret;
        }
    }
    serdes_unlock(ctx_node->node_lock);

    serdes_lock(serdes_ctx_mgt->ctx_lock);
    ot_list_add(&(ctx_node->ctx_list), &(serdes_ctx_mgt->ctx_head));
    serdes_unlock(serdes_ctx_mgt->ctx_lock);

    *handle = (ot_eis_handle)ctx_node;

    return OT_SUCCESS;
}

ot_s32 serdes_ctx_destroy(ot_serdes_type serdes_type, ot_eis_handle handle)
{
    ot_s32 ret;
    ot_serdes_ctx_mgt *serdes_ctx_mgt = &g_serdes_ctx_mgt[serdes_type];
    ot_serdes_ctx_node *ctx_node = (ot_serdes_ctx_node *)handle;

    // find context
    ret = serdes_match_serdes_ctx(serdes_ctx_mgt, ctx_node);
    if (ret != OT_SUCCESS) {
        serdes_err("The handle match failed.\n");
        return ret;
    }

    // destroy context
    serdes_lock(ctx_node->node_lock);
    if (ctx_node->is_started == OT_TRUE) {
        serdes_unlock(ctx_node->node_lock);
        serdes_err("The handle don't stop\n");
        return OT_EIS_ERRNO_NO_STOP;
    }
    if (ctx_node->serdes_obj && ctx_node->serdes_obj->destroy) {
        ret = ctx_node->serdes_obj->destroy(ctx_node->serdes_id);
        if (ret != OT_SUCCESS) {
            serdes_unlock(ctx_node->node_lock);
            serdes_err("The handle destroy serdes id failed\n");
            return ret;
        }
    }
    serdes_unlock(ctx_node->node_lock);

    // delete context node
    serdes_lock(serdes_ctx_mgt->ctx_lock);
    serdes_lock_deinit(ctx_node->node_lock);
    ot_list_del(&(ctx_node->ctx_list));
    free(ctx_node);
    serdes_unlock(serdes_ctx_mgt->ctx_lock);

    return OT_SUCCESS;
}

ot_s32 serdes_ctx_start(ot_serdes_type serdes_type, ot_eis_handle handle)
{
    ot_s32 ret;
    ot_serdes_ctx_mgt *serdes_ctx_mgt = &g_serdes_ctx_mgt[serdes_type];
    ot_serdes_ctx_node *ctx_node = (ot_serdes_ctx_node *)handle;

    // find context
    ret = serdes_match_serdes_ctx(serdes_ctx_mgt, ctx_node);
    if (ret != OT_SUCCESS) {
        serdes_err("The handle match failed.\n");
        return ret;
    }

    // start context
    serdes_lock(ctx_node->node_lock);
    if (ctx_node->is_started == OT_TRUE) {
        serdes_unlock(ctx_node->node_lock);
        serdes_err("The handle don't stop\n");
        return OT_EIS_ERRNO_NO_STOP;
    }

    if (ctx_node->serdes_obj && ctx_node->serdes_obj->start) {
        ret = ctx_node->serdes_obj->start(ctx_node->serdes_id);
        if (ret != OT_SUCCESS) {
            serdes_unlock(ctx_node->node_lock);
            serdes_err("The handle start serdes id failed\n");
            return ret;
        }
    }
    ctx_node->is_started = OT_TRUE;
    serdes_unlock(ctx_node->node_lock);
    return OT_SUCCESS;
}

ot_s32 serdes_ctx_restart(ot_serdes_type serdes_type, ot_eis_handle handle, ot_u32 port_id)
{
    ot_s32 ret;
    ot_serdes_ctx_mgt *serdes_ctx_mgt = &g_serdes_ctx_mgt[serdes_type];
    ot_serdes_ctx_node *ctx_node = (ot_serdes_ctx_node *)handle;

    // find context
    ret = serdes_match_serdes_ctx(serdes_ctx_mgt, ctx_node);
    if (ret != OT_SUCCESS) {
        serdes_err("The handle match failed.\n");
        return ret;
    }
    // start context
    serdes_lock(ctx_node->node_lock);
    if (ctx_node->is_started != OT_TRUE) {
        serdes_unlock(ctx_node->node_lock);
        serdes_err("The handle don't start\n");
        return OT_EIS_ERRNO_NO_START;
    }

    if (ctx_node->serdes_obj && ctx_node->serdes_obj->restart) {
        ret = ctx_node->serdes_obj->restart(ctx_node->serdes_id, port_id);
        if (ret != OT_SUCCESS) {
            serdes_unlock(ctx_node->node_lock);
            serdes_err("The handle restart serdes id failed\n");
            return ret;
        }
    }
    serdes_unlock(ctx_node->node_lock);
    return OT_SUCCESS;
}

ot_s32 serdes_ctx_stop(ot_serdes_type serdes_type, ot_eis_handle handle)
{
    ot_s32 ret;
    ot_serdes_ctx_mgt *serdes_ctx_mgt = &g_serdes_ctx_mgt[serdes_type];
    ot_serdes_ctx_node *ctx_node = (ot_serdes_ctx_node *)handle;

    // find context
    ret = serdes_match_serdes_ctx(serdes_ctx_mgt, ctx_node);
    if (ret != OT_SUCCESS) {
        serdes_err("The handle match failed.\n");
        return ret;
    }

    // stop context
    serdes_lock(ctx_node->node_lock);
    if (ctx_node->is_started == OT_FALSE) {
        serdes_unlock(ctx_node->node_lock);
        serdes_err("The handle don't start\n");
        return OT_EIS_ERRNO_NO_START;
    }
    if (ctx_node->serdes_obj && ctx_node->serdes_obj->stop) {
        ret = ctx_node->serdes_obj->stop(ctx_node->serdes_id);
        if (ret != OT_SUCCESS) {
            serdes_unlock(ctx_node->node_lock);
            serdes_err("The handle stop serdes id failed\n");
            return ret;
        }
    }
    ctx_node->is_started = OT_FALSE;
    serdes_unlock(ctx_node->node_lock);
    return OT_SUCCESS;
}