#include "ot_eis_vo_serdes.h"
#include "serdes_ctx_mgt.h"
#include "vo_serdes_drv.h"

#define vo_serdes_get_mode_id(errno)     ((errno >> 16) & 0xff)
#define vo_serdes_errno_convert(errno)                                  \
    ((vo_serdes_get_mode_id(ret) == OT_EIS_MODULE_VO_SERDES) ?          \
        (errno) : OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VO_SERDES, errno))

#define vo_serdes_chk_ptr_null_return(ptr)                              \
    serdes_chk_ptr_null_return(ptr, OT_VO_SERDES_ERR_NULL_PTR)

#define vo_serdes_chk_data_le_return(data, min)                          \
    serdes_chk_data_le_return(data, min, OT_VO_SERDES_ERR_INVALID_PARAM)

#define vo_serdes_chk_data_ge_return(data, max)                          \
    serdes_chk_data_ge_return(data, max, OT_VO_SERDES_ERR_OVER_SIZE)

static ot_u32 g_vo_serdes_cnt = 0;
static pthread_mutex_t g_vo_serdes_lock = PTHREAD_MUTEX_INITIALIZER;

#define vo_serdes_chk_is_init()                     \
    do {                                            \
        serdes_lock(g_vo_serdes_lock);              \
        if (g_vo_serdes_cnt == 0) {                 \
            serdes_unlock(g_vo_serdes_lock);        \
            serdes_err("haven't init\n");           \
            return OT_VO_SERDES_ERR_NOT_INIT;       \
        }                                           \
        serdes_unlock(g_vo_serdes_lock);            \
    } while (0)

ot_s32 ot_eis_vo_serdes_init(ot_void)
{
    ot_s32 ret;

    serdes_lock(g_vo_serdes_lock);
    if (++g_vo_serdes_cnt > 1) {
        serdes_unlock(g_vo_serdes_lock);
        return OT_SUCCESS;
    }

    serdes_ctx_init(OT_SERDES_TYPE_VO);

    ret = vo_serdes_drv_register();
    if (ret != OT_SUCCESS) {
        serdes_unlock(g_vo_serdes_lock);
        serdes_err("serdes drv register failed.\n");
        return vo_serdes_errno_convert(ret);
    }
    serdes_unlock(g_vo_serdes_lock);
    return ret;
}

ot_s32 ot_eis_vo_serdes_deinit(ot_void)
{
    ot_s32 ret;

    vo_serdes_chk_is_init();

    serdes_lock(g_vo_serdes_lock);
    if (g_vo_serdes_cnt > 0) {
        if (--g_vo_serdes_cnt != 0) {
            serdes_unlock(g_vo_serdes_lock);
            return OT_SUCCESS;
        }
    }
    serdes_unlock(g_vo_serdes_lock);

    ret = vo_serdes_drv_unregister();
    if (ret != OT_SUCCESS) {
        serdes_err("serdes drv unregister failed.\n");
        return vo_serdes_errno_convert(ret);
    }
    serdes_ctx_deinit(OT_SERDES_TYPE_VO);
    return ret;
}

ot_s32 ot_eis_vo_serdes_create(ot_eis_vo_serdes_handle *serdes_hdl,
    const ot_s8 *serdes_name, const ot_eis_vo_serdes_attr *serdes_attr)
{
    ot_s32 ret;

    vo_serdes_chk_is_init();
    vo_serdes_chk_ptr_null_return(serdes_hdl);

    vo_serdes_chk_ptr_null_return(serdes_name);
    vo_serdes_chk_data_ge_return(strlen(serdes_name), OT_EIS_VO_SERDES_NAME_MAX);

    vo_serdes_chk_ptr_null_return(serdes_attr);
    vo_serdes_chk_data_ge_return(serdes_attr->bus_info.bus_type, OT_VO_SERDES_BUS_TYPE_INVALID);
    vo_serdes_chk_data_ge_return(serdes_attr->data_fmt, OT_VO_SERDES_DATA_FMT_INVALID);

    ret = serdes_ctx_create(OT_SERDES_TYPE_VO, serdes_hdl,
        serdes_name, serdes_attr, OT_EIS_VO_SERDES_HDL_MAX);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes ctx stop failed.\n");
        return vo_serdes_errno_convert(ret);
    }
    return ret;
}

ot_s32 ot_eis_vo_serdes_destroy(ot_eis_vo_serdes_handle serdes_hdl)
{
    ot_s32 ret;

    vo_serdes_chk_is_init();
    vo_serdes_chk_ptr_null_return(serdes_hdl);

    ret = serdes_ctx_destroy(OT_SERDES_TYPE_VO, serdes_hdl);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes ctx destroy failed.\n");
        return vo_serdes_errno_convert(ret);
    }
    return ret;
}

ot_s32 ot_eis_vo_serdes_start(ot_eis_vo_serdes_handle serdes_hdl)
{
    ot_s32 ret;

    vo_serdes_chk_is_init();
    vo_serdes_chk_ptr_null_return(serdes_hdl);

    ret = serdes_ctx_start(OT_SERDES_TYPE_VO, serdes_hdl);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes ctx start failed.\n");
        return vo_serdes_errno_convert(ret);
    }
    return ret;
}

ot_s32 ot_eis_vo_serdes_stop(ot_eis_vo_serdes_handle serdes_hdl)
{
    ot_s32 ret;

    vo_serdes_chk_is_init();
    vo_serdes_chk_ptr_null_return(serdes_hdl);

    ret = serdes_ctx_stop(OT_SERDES_TYPE_VO, serdes_hdl);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes ctx stop failed.\n");
        return vo_serdes_errno_convert(ret);
    }
    return ret;
}

ot_s32 ot_eis_vo_serdes_restart(ot_eis_vo_serdes_handle serdes_hdl, ot_eis_vo_serdes_port port_id)
{
    ot_s32 ret;

    vo_serdes_chk_is_init();
    vo_serdes_chk_ptr_null_return(serdes_hdl);

    ret = serdes_ctx_restart(OT_SERDES_TYPE_VO, serdes_hdl, (ot_u32)port_id);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes ctx restart failed.\n");
        return vo_serdes_errno_convert(ret);
    }
    return ret;
}
