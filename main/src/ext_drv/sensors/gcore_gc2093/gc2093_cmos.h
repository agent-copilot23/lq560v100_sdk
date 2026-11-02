#ifndef GC2093_CMOS_H
#define GC2093_CMOS_H

#include "ot_eis_sns_ctrl.h"
#include "securec.h"
#include "ot_eis_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif

#define GC2093_I2C_ADDR    0xfc
#define GC2093_ADDR_BYTE   2
#define GC2093_DATA_BYTE   1
#define gc2093_sensor_get_ctx(pipe, ctx)   ((ctx) = gc2093_get_ctx(pipe))

#define GC2093_FULL_LINES_MAX 0x3FFF
#define GC2093_FULL_LINES_MAX_2TO1_WDR 0x3FFF
#define GC2093_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
#define GC2093_VMAX_2M_30FPS_10BIT_LINEAR     (1250 + GC2093_INCREASE_LINES)
#define GC2093_VMAX_2M_30FPS_10BIT_2TO1_WDR   (1250 + GC2093_INCREASE_LINES)   /* 10bit */

typedef enum {
    GC2093_2M_30FPS_10BIT_LINEAR_MODE = 0,
    GC2093_2M_30FPS_10BIT_2TO1_VC_MODE,
    GC2093_MODE_BUTT
} gc2093_res_mode;

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
} gc2093_video_mode_tbl;

typedef enum {
    SHORT_EXPO_H_IDX,  // 01
    SHORT_EXPO_L_IDX,  // 02
    EXPO_H_IDX,       // 03
    EXPO_L_IDX,       // 04
    COL_AGAIN_H_IDX, // b8
    COL_AGAIN_L_IDX, // b9
    AGAIN_MAG1_IDX,  // 0x155
    AGAIN_HOLD_IDX,  // 0x031d
    AGAIN_MAG2_IDX,  // 0xc2
    AGAIN_MAG3_IDX,  // 0xcf
    AGAIN_MAG4_IDX,  // 0xd9
    AGAIN_REL_IDX,   // 0x031d
    AGAIN_L_IDX,     // b3
    DGAIN_H_IDX,     // b1
    DGAIN_L_IDX,     // b2
    VMAX_H_IDX,       // 0x41  (frame length)
    VMAX_L_IDX,       // 0x42
    DePink_A,         // 0x0183
    DePink_B,         // 0x0187
    DePink_C,         // 0x0032
    REG_MAX_IDX
}gc2093_reg_index;    // gc2093_linear_reg_index;

typedef struct {
    ot_u32 dec[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32 inc[OT_EIS_VI_WDR_MAX_FRAME_NUM];
} time_step;
ot_eis_sns_state *gc2093_get_ctx(ot_vi_pipe_id vi_pipe);
ot_eis_sns_bus_info *gc2093_get_bus_info(ot_vi_pipe_id vi_pipe);

ot_void gc2093_init(ot_vi_pipe_id vi_pipe);
ot_void gc2093_exit(ot_vi_pipe_id vi_pipe);
ot_void gc2093_standby(ot_vi_pipe_handle pipe_hdl);
ot_void gc2093_restart(ot_vi_pipe_handle pipe_hdl);
ot_void gc2093_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp);
ot_void gc2093_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en);

ot_s32  gc2093_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* GC2093_CMOS_H */
