#include "serdes_obj_mgt.h"
#include "vi_serdes_drv.h"

extern const ot_serdes_obj *max9295_max9296_get_vi_serdes_obj(ot_void);

ot_s32 vi_serdes_drv_register(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ret |= serdes_obj_register(OT_SERDES_TYPE_VI, max9295_max9296_get_vi_serdes_obj());
    return ret;
}

ot_s32 vi_serdes_drv_unregister(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ret |= serdes_obj_unregister(OT_SERDES_TYPE_VI, max9295_max9296_get_vi_serdes_obj());
    return ret;
}