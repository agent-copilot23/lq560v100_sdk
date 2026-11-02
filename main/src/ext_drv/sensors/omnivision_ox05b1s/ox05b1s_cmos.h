#ifndef OX05B1S_CMOS_H
#define OX05B1S_CMOS_H

#include "ot_eis_sns_ctrl.h"
#include "securec.h"
#include "ot_eis_video.h"
#include "ot_eis_vi_3a.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// #define OX05B1S_DATA_TYPE_RAW16  // RAW16 or RAW12

// #define OX05B1S_RATE_60FPS       // 60fps or 30 fps

#define OX05B1S_I2C_ADDR            0x6c
#define OX05B1S_ADDR_BYTE         2
#define OX05B1S_DATA_BYTE         1
#define I2C_DATA_BYTE 1
#define I2C_REG_ADDR_2BYTE 2
#define I2C_REG_ADDR_1BYTE 1
#define ox05b1s_sensor_get_ctx(pipe, ctx)   ((ctx) = ox05b1s_get_ctx(pipe))

#define OX05B1S_FULL_LINES_MAX 0xFFFFF

#define OX05B1S_INCREASE_LINES 1 /* make real fps less than stand fps because NVR require */

#define OX05B1S_VMAX_1944P30_LINEAR                      (1136)

typedef enum {
    OX05B1S_SENSOR_1944P_30FPS_LINEAR_MODE = 0,
    OX05B1S_MODE_BUTT
} ox05b1s_res_mode;

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
} ox05b1s_video_mode_tbl;

typedef struct {
    ot_u32 dec[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32 inc[OT_EIS_VI_WDR_MAX_FRAME_NUM];
} time_step;
ot_eis_sns_state *ox05b1s_get_ctx(ot_vi_pipe_id vi_pipe);
ot_eis_sns_bus_info *ox05b1s_get_bus_info(ot_vi_pipe_id vi_pipe);

ot_void ox05b1s_init(ot_vi_pipe_id vi_pipe);
ot_void ox05b1s_exit(ot_vi_pipe_id vi_pipe);
ot_void ox05b1s_standby(ot_vi_pipe_handle pipe_hdl);
ot_void ox05b1s_restart(ot_vi_pipe_handle pipe_hdl);
ot_void ox05b1s_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp);
ot_void ox05b1s_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en);

ot_s32 ox05b1s_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data);
ot_s32 ox05b1s_read_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 *data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* OX05B1S_CMOS_H */
