#ifndef OX08B40_CMOS_H
#define OX08B40_CMOS_H

#include "ot_eis_sns_ctrl.h"
#include "securec.h"
#include "ot_eis_video.h"
#include "ot_eis_vi_3a.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// #define OX08B40_DATA_TYPE_RAW16  // RAW16 or RAW12

// #define OX08B40_RATE_60FPS       // 60fps or 30 fps

#define OX08B40_I2C_ADDR            0x6c
#define OX08B40_ADDR_BYTE         2
#define OX08B40_DATA_BYTE         1
#define I2C_DATA_BYTE 1
#define I2C_REG_ADDR_2BYTE 2
#define I2C_REG_ADDR_1BYTE 1
#define ox08b40_sensor_get_ctx(pipe, ctx)   ((ctx) = ox08b40_get_ctx(pipe))

#define OX08B40_FULL_LINES_MAX 0xFFFFF
#define OX08B40_FULL_LINES_MAX_BUILT_IN_WDR 0x19A28             //1750*60 (0.5fps)

#define OX08B40_INCREASE_LINES 1 /* make real fps less than stand fps because NVR require */

#define OX08B40_VMAX_2160P30_LINEAR                      (1136)
#define OX08B40_VMAX_2160P30_BUILT_IN_WDR                (1136)

typedef enum {
    OX08B40_SENSOR_2160P_30FPS_LINEAR_MODE = 0,
    OX08B40_SENSOR_2160P_30FPS_BUILT_IN_MODE,
    OX08B40_MODE_BUTT
} ox08b40_res_mode;

typedef struct {
    ot_u32      ver_lines;
    ot_u32      max_ver_lines;
    ot_float    max_fps;
    ot_float    min_fps;
    ot_u32      width;
    ot_u32      height;
    ot_u8       sns_mode;
    ot_eis_wdr_mode wdr_mode;
    const char *mode_name;
} ox08b40_video_mode_tbl;

typedef struct {
    ot_u32 dec[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32 inc[OT_EIS_VI_WDR_MAX_FRAME_NUM];
} time_step;
ot_eis_sns_state *ox08b40_get_ctx(ot_vi_pipe_id vi_pipe);
ot_eis_sns_reg_table *ox08b40_get_ext_sensor_table(ot_vi_pipe_id vi_pipe);
ot_u8 ox08b40_get_ext_sensor_flag(ot_vi_pipe_id vi_pipe);
ot_eis_sns_bus_info *ox08b40_get_bus_info(ot_vi_pipe_id vi_pipe);

ot_void ox08b40_init(ot_vi_pipe_id vi_pipe);
ot_void ox08b40_exit(ot_vi_pipe_id vi_pipe);
ot_void ox08b40_standby(ot_vi_pipe_handle pipe_hdl);
ot_void ox08b40_restart(ot_vi_pipe_handle pipe_hdl);
ot_void ox08b40_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp);
ot_void ox08b40_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en);

ot_s32 ox08b40_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data);
ot_s32 ox08b40_read_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 *data);

static ot_u32 cmos_again_calc_by_again(ot_u32 again_ref,float ratio);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* OX08B40_CMOS_H */
