
#include "serdes_obj_mgt.h"
#include "vo_serdes_drv.h"

extern const ot_serdes_obj *ti941_ti948_get_vo_serdes_obj(ot_void);

ot_s32 vo_serdes_drv_register(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ret |= serdes_obj_register(OT_SERDES_TYPE_VO, ti941_ti948_get_vo_serdes_obj());
    return ret;
}

ot_s32 vo_serdes_drv_unregister(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ret |= serdes_obj_unregister(OT_SERDES_TYPE_VO, ti941_ti948_get_vo_serdes_obj());
    return ret;
}