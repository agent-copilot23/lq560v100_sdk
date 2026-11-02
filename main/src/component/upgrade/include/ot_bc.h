/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */
#ifndef OT_BC_H
#define OT_BC_H
#include "ot_eis_type.h"
#include "ot_bc_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MISC_PATH               "/dev/by-name/misc"

typedef enum {
    OT_BC_SLOT_A=0,
    OT_BC_SLOT_B,
} ot_bc_slot ;

typedef enum {
    OT_BC_SB_ALOG_NM = 0,
    OT_BC_SB_ALOG_GM,
} ot_bc_algo;
/**
 * @brief init for bc module
 *
 * @return ot_s32
 */
ot_s32 ot_bc_init(ot_void);
/**
 * @brief deinit for bc module
 *
 * @return ot_s32
 */
ot_s32 ot_bc_deinit(ot_void);
/**
 * @brief get current slot
 *
 * @param slot
 * @return ot_s32
 */
ot_s32 ot_bc_get_cur_slot(ot_bc_slot *slot);
/**
 * @brief mark current slot boot ok
 *
 * @return ot_s32
 */
ot_s32 ot_bc_markup_cur_slot(ot_void);
/**
 * @brief switch boot slot
 *
 * @param slot
 * @return ot_s32
 */
ot_s32 ot_bc_switch_slot(ot_bc_slot slot);
/**
 * @brief get opt boot version number:0-64
 *
 * @param system_ver
 * @return ot_s32
 */
ot_s32 ot_bc_get_system_version(ot_u32* system_ver);
/**
 * @brief set opt boot version number:0-64
 *
 * @param system_ver
 * @return ot_s32
 */
ot_s32 ot_bc_set_system_version(ot_u32 system_ver);
/**
 * @brief get opt secure verify enable status
 *
 * @return ot_bool
 */
ot_bool ot_bc_is_secure_enable(ot_void);
/**
 * @brief get opt secure boot algo type
 *
 * @return ot_s32
 */
ot_bc_algo ot_bc_get_secure_algo(ot_void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif