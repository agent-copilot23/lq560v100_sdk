#ifndef SERDES_CTX_MGT_H
#define SERDES_CTX_MGT_H

#include "serdes_comm.h"

ot_s32 serdes_ctx_init(ot_serdes_type serdes_type);
ot_s32 serdes_ctx_deinit(ot_serdes_type serdes_type);

ot_s32 serdes_ctx_create(ot_serdes_type serdes_type, ot_eis_handle *handle,
    const ot_s8 *serdes_name, const ot_serdes_attr *serdes_attr, ot_u32 max_level);
ot_s32 serdes_ctx_destroy(ot_serdes_type serdes_type, ot_eis_handle handle);

ot_s32 serdes_ctx_start(ot_serdes_type serdes_type, ot_eis_handle handle);
ot_s32 serdes_ctx_stop(ot_serdes_type serdes_type, ot_eis_handle handle);
ot_s32 serdes_ctx_restart(ot_serdes_type serdes_type, ot_eis_handle handle, ot_u32 port_id);

#endif /* SERDES_CTX_MGT_H */