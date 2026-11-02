#ifndef SERDES_OBJ_MGT_H
#define SERDES_OBJ_MGT_H

#include "serdes_comm.h"

typedef struct {
    const ot_s8 *serdes_name;
    ot_s32 (*create)(ot_s32 *serdes_id, const ot_serdes_attr *serdes_attr);
    ot_s32 (*destroy)(ot_s32 serdes_id);
    ot_s32 (*start)(ot_s32 serdes_id);
    ot_s32 (*stop)(ot_s32 serdes_id);
    ot_s32 (*restart)(ot_s32 serdes_id, ot_u32 port_id);
} ot_serdes_obj;

ot_s32 serdes_obj_register(ot_serdes_type serdes_type, const ot_serdes_obj *serdes_obj);

ot_s32 serdes_obj_unregister(ot_serdes_type serdes_type, const ot_serdes_obj *serdes_obj);

const ot_serdes_obj *serdes_obj_get_obj(ot_serdes_type serdes_type, const char *serdes_name);

#endif /* SERDES_OBJ_MGT_H */