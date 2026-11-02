#ifndef OT_SEMU_RT_H
#define OT_SEMU_RT_H

#include "ot_semu_err_type.h"
#include "ot_semu_sm_cfg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef ot_void (*ot_semu_err_callback)(ot_semu_sm_type sm_type,
                                        ot_s32 err_type, ot_semu_err_info *info);

typedef struct {
    ot_semu_err_callback err_callback;
} ot_semu_rt_init_cfg;

ot_s32 ot_semu_rt_init(ot_semu_rt_init_cfg *init_cfg);

ot_s32 ot_semu_rt_deinit(ot_void);

ot_s32
ot_semu_rt_enable_temper_monitor(ot_bool enable,
                                 ot_semu_rt_temper_monitor_cfg *temper_cfg);

ot_s32 ot_semu_rt_enable_mipirx_monitor(ot_bool enable);

ot_s32 ot_semu_rt_enable_vi_monitor(ot_bool enable);

ot_s32 ot_semu_rt_enable_vproc_monitor(ot_bool enable, ot_semu_rt_vproc_monitor_cfg *vproc_cfg);


ot_s32 ot_semu_rt_enable_vo_monitor(ot_bool enable);

ot_s32 ot_semu_rt_enable_mipitx_monitor(ot_bool enable);

ot_s32 ot_semu_rt_enable_mipitx_fps_monitor(ot_bool enable,  ot_semu_rt_mipitx_fps_monitor_cfg *mipitx_cfg);

ot_s32 ot_semu_rt_enable_emmc_monitor(ot_bool enable);

ot_s32 ot_semu_rt_enable_spi_monitor(ot_bool enable);

ot_s32 ot_semu_rt_enable_i2c_monitor(ot_bool enable);

ot_s32 ot_semu_rt_enable_fmc_monitor(ot_bool enable);

ot_s32
ot_semu_rt_enable_video_delay_monitor(ot_bool enable,
                                      const ot_semu_rt_video_delay_monitor_cfg *cfg);

ot_s32 ot_semu_rt_enable_ddr_monitor(ot_bool enable,
                                     const ot_semu_rt_ddr_monitor_cfg *cfg);

ot_s32 ot_semu_rt_enable_clock_monitor(ot_bool enable);

ot_s32 ot_semu_rt_enable_acore_monitor(ot_bool enable,
                                       const ot_semu_rt_acore_stl_monitor_cfg *cfg);

ot_s32 ot_semu_rt_enable_registers_monitor(ot_bool enable,
                                            ot_semu_rt_registers_monitor_cfg *registers_cfg);

ot_s32 ot_semu_rt_start(ot_void);

ot_s32 ot_semu_rt_stop(ot_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
