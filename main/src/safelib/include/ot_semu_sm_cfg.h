#ifndef OT_SEMU_SM_CFG_H
#define OT_SEMU_SM_CFG_H

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

// temperature monitor config
typedef struct {
    ot_s16 thres_level_1;
    ot_u16 risk_time; // debounce time for level1
    ot_s16 thres_level_2;
    ot_s16 thres_level_3;
    ot_u16 fatal_time; // debounce time for level3
    ot_s16 thres_level_4;
} ot_semu_rt_temper_level; // temperature

typedef struct {
    ot_u32 detect_interval_ms;
    ot_semu_rt_temper_level range;
} ot_semu_rt_temper_monitor_cfg; // temperature monitor

// video delay config
typedef struct {
    ot_u32 detect_interval_ms;
    ot_u64 max_delay_thres; // video delay threshold
} ot_semu_rt_video_delay_monitor_cfg;

// ddr monitor config
typedef struct {
    ot_u32 detect_interval_ms;
    ot_void* test_addr; // test addr ,should be aligned by 4, must be physical
                        // addr
    ot_u32 test_len;
} ot_semu_rt_ddr_monitor_cfg;

// acore monitor config
typedef struct {
    ot_u32 detect_interval_ms;
} ot_semu_rt_acore_stl_monitor_cfg;

// vproc monitor config
typedef struct {
    ot_u32 detect_interval_ms;
    ot_u32 loss_thres;    // interrupt loss threshold
} ot_semu_rt_vproc_monitor_cfg;

/* registers monitor config */
typedef struct {
    ot_void *phys_address;
    ot_u32 value;
} ot_semu_reg;

typedef struct {
    ot_semu_reg *registers;
    ot_u32 num_registers;
} ot_semu_rt_register_set;

typedef struct {
    ot_u32 detect_interval_ms;
    ot_semu_rt_register_set reg_set;
} ot_semu_rt_registers_monitor_cfg;

// mipitx monitor config
typedef struct {
    ot_u32 fps;
    ot_u32 detect_interval_ms;
} ot_semu_rt_mipitx_fps_monitor_cfg;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif