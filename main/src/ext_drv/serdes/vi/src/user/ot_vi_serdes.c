#include "ot_eis_vi_serdes.h"
#include "serdes_ctx_mgt.h"
#include "vi_serdes_drv.h"

#define vi_serdes_get_mode_id(errno)     ((errno >> 16) & 0xff)
#define vi_serdes_errno_convert(errno)                                  \
    ((vi_serdes_get_mode_id(ret) == OT_EIS_MODULE_VI_SERDES) ?          \
        (errno) : OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_VI_SERDES, errno))

#define vi_serdes_chk_ptr_null_return(ptr)                              \
    serdes_chk_ptr_null_return(ptr, OT_VI_SERDES_ERR_NULL_PTR)

#define vi_serdes_chk_data_le_return(data, min)                          \
    serdes_chk_data_le_return(data, min, OT_VI_SERDES_ERR_INVALID_PARAM)

#define vi_serdes_chk_data_gt_return(data, max)                          \
    serdes_chk_data_gt_return(data, max, OT_VI_SERDES_ERR_OVER_SIZE)

#define vi_serdes_chk_data_ge_return(data, max)                          \
    serdes_chk_data_ge_return(data, max, OT_VI_SERDES_ERR_OVER_SIZE)

#define vi_serdes_chk_i2c_addr_odd_return(addr)                         \
    serdes_chk_i2c_addr_odd_return(addr, OT_VI_SERDES_ERR_INVALID_PARAM)

#define vi_serdes_chk_i2c_addr_equal_return(src_addr, dst_addr)         \
    serdes_chk_i2c_addr_equal_return(src_addr, dst_addr, OT_VI_SERDES_ERR_INVALID_PARAM)

static ot_u32 g_vi_serdes_cnt = 0;
static pthread_mutex_t g_vi_serdes_lock = PTHREAD_MUTEX_INITIALIZER;

#define vi_serdes_port_check_valid(port)                                 \
    do {                                                              \
        if(port < OT_VI_SERDES_PORT_0                       \
        || port >= OT_VI_SERDES_PORT_INVALID) {               \
            serdes_err("vi serdes serial_port[%d] invalid!\n", port);  \
            return OT_VI_SERDES_ERR_INVALID_PARAM;                         \
        }                                                             \
    } while (0)                                                       \

#define vi_serdes_chk_is_init()                     \
    do {                                            \
        serdes_lock(g_vi_serdes_lock);              \
        if (g_vi_serdes_cnt == 0) {                 \
            serdes_unlock(g_vi_serdes_lock);        \
            serdes_err("haven't init\n");           \
            return OT_VI_SERDES_ERR_NOT_INIT;       \
        }                                           \
        serdes_unlock(g_vi_serdes_lock);            \
    } while (0)

ot_s32 ot_eis_vi_serdes_init(ot_void)
{
    ot_s32 ret;

    serdes_lock(g_vi_serdes_lock);
    if (++g_vi_serdes_cnt > 1) {
        serdes_unlock(g_vi_serdes_lock);
        return OT_SUCCESS;
    }

    serdes_ctx_init(OT_SERDES_TYPE_VI);

    ret = vi_serdes_drv_register();
    if (ret != OT_SUCCESS) {
        serdes_unlock(g_vi_serdes_lock);
        serdes_err("serdes drv register failed.\n");
        return vi_serdes_errno_convert(ret);
    }
    serdes_unlock(g_vi_serdes_lock);
    return ret;
}

ot_s32 ot_eis_vi_serdes_deinit(ot_void)
{
    ot_s32 ret;

    vi_serdes_chk_is_init();

    serdes_lock(g_vi_serdes_lock);
    if (g_vi_serdes_cnt > 0) {
        if (--g_vi_serdes_cnt != 0) {
            serdes_unlock(g_vi_serdes_lock);
            return OT_SUCCESS;
        }
    }
    serdes_unlock(g_vi_serdes_lock);

    // serdes cnt is equal to 0, unregister drivers
    ret = vi_serdes_drv_unregister();
    if (ret != OT_SUCCESS) {
        serdes_err("serdes drv unregister failed.\n");
        return vi_serdes_errno_convert(ret);
    }
    serdes_ctx_deinit(OT_SERDES_TYPE_VI);
    return ret;
}

ot_s32 ot_eis_vi_serdes_create(ot_eis_vi_serdes_handle *serdes_hdl,
    const ot_s8 *serdes_name, const ot_eis_vi_serdes_attr *serdes_attr)
{
    ot_s32 ret;

    vi_serdes_chk_is_init();
    vi_serdes_chk_ptr_null_return(serdes_hdl);

    vi_serdes_chk_ptr_null_return(serdes_name);
    vi_serdes_chk_data_ge_return(strlen(serdes_name), OT_EIS_VI_SERDES_NAME_MAX);

    vi_serdes_chk_ptr_null_return(serdes_attr);
    vi_serdes_chk_data_le_return(serdes_attr->freq_mhz, 0);
    vi_serdes_chk_data_ge_return(serdes_attr->bus_info.bus_type, OT_VI_SERDES_BUS_TYPE_INVALID);
    vi_serdes_chk_data_ge_return(serdes_attr->data_fmt, OT_VI_SERDES_DATA_FMT_INVALID);
    vi_serdes_chk_data_ge_return(serdes_attr->wdr_type, OT_VI_SERDES_WDR_TYPE_INVALID);
    vi_serdes_chk_data_ge_return(serdes_attr->wdr_mode, OT_VI_SERDES_WDR_MODE_INVALID);
    vi_serdes_chk_data_le_return(serdes_attr->sns_lanes, 0);
    vi_serdes_chk_data_gt_return(serdes_attr->sns_lanes, SERDES_LANE_MAX);
    vi_serdes_chk_data_le_return(serdes_attr->sns_nums, 0);
    vi_serdes_chk_i2c_addr_odd_return(serdes_attr->deserial_addr);
    if (serdes_attr->sns_nums == 1) {
        vi_serdes_chk_i2c_addr_odd_return(serdes_attr->one_port_attr.serial_addr);
    } else {
        ot_s32 i, j;
        for (i = 0; i < serdes_attr->sns_nums; i++) {
            vi_serdes_chk_data_ge_return(
                serdes_attr->multi_port_attr[i].serdes_port, OT_VI_SERDES_PORT_INVALID);
            vi_serdes_chk_i2c_addr_odd_return(serdes_attr->multi_port_attr[i].serial_src_addr);
            vi_serdes_chk_i2c_addr_odd_return(serdes_attr->multi_port_attr[i].serial_dst_addr);
            vi_serdes_chk_i2c_addr_odd_return(serdes_attr->multi_port_attr[i].sns_src_addr);
            vi_serdes_chk_i2c_addr_odd_return(serdes_attr->multi_port_attr[i].sns_dst_addr);
            vi_serdes_chk_i2c_addr_equal_return(serdes_attr->multi_port_attr[i].serial_src_addr,
                serdes_attr->multi_port_attr[i].serial_dst_addr);
            vi_serdes_chk_i2c_addr_equal_return(serdes_attr->multi_port_attr[i].sns_src_addr,
                serdes_attr->multi_port_attr[i].sns_dst_addr);
            for (j = 0; j < OT_EIS_VI_SERDES_VC_ID_MAX; j++) {
                vi_serdes_chk_data_ge_return(
                    serdes_attr->multi_port_attr[i].vc_id[j], OT_EIS_VI_SERDES_VC_ID_MAX);
            }
        }
    }

    ret = serdes_ctx_create(OT_SERDES_TYPE_VI, serdes_hdl,
        serdes_name, serdes_attr, OT_EIS_VI_SERDES_HDL_MAX);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes ctx create failed.\n");
        return vi_serdes_errno_convert(ret);
    }
    return ret;
}

ot_s32 ot_eis_vi_serdes_destroy(ot_eis_vi_serdes_handle serdes_hdl)
{
    ot_s32 ret;

    vi_serdes_chk_is_init();
    vi_serdes_chk_ptr_null_return(serdes_hdl);

    ret = serdes_ctx_destroy(OT_SERDES_TYPE_VI, serdes_hdl);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes ctx destroy failed.\n");
        return vi_serdes_errno_convert(ret);
    }
    return ret;
}

ot_s32 ot_eis_vi_serdes_start(ot_eis_vi_serdes_handle serdes_hdl)
{
    ot_s32 ret;

    vi_serdes_chk_is_init();
    vi_serdes_chk_ptr_null_return(serdes_hdl);

    ret = serdes_ctx_start(OT_SERDES_TYPE_VI, serdes_hdl);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes ctx start failed.\n");
        return vi_serdes_errno_convert(ret);
    }
    return ret;
}

ot_s32 ot_eis_vi_serdes_stop(ot_eis_vi_serdes_handle serdes_hdl)
{
    ot_s32 ret;

    vi_serdes_chk_is_init();
    vi_serdes_chk_ptr_null_return(serdes_hdl);

    ret = serdes_ctx_stop(OT_SERDES_TYPE_VI, serdes_hdl);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes ctx stop failed.\n");
        return vi_serdes_errno_convert(ret);
    }
    return ret;
}

ot_s32 ot_eis_vi_serdes_restart(ot_eis_vi_serdes_handle serdes_hdl, ot_eis_vi_serdes_port port_id)
{
    ot_s32 ret = OT_FAILURE;

    vi_serdes_chk_is_init();
    vi_serdes_chk_ptr_null_return(serdes_hdl);
    vi_serdes_port_check_valid(port_id);

    ret = serdes_ctx_restart(OT_SERDES_TYPE_VI, serdes_hdl, (ot_u32)port_id);
    if (ret != OT_SUCCESS) {
        serdes_err("serdes[%px] deserial_port[%u] restart failed.\n", serdes_hdl, port_id);
        return vi_serdes_errno_convert(ret);
    }
    return ret;
}
