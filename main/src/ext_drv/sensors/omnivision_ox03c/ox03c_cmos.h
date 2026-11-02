#ifndef OX03C_CMOS_H
#define OX03C_CMOS_H


#include "ot_eis_sns_ctrl.h"
#include "securec.h"
#include "ot_eis_video.h"
#include "ot_eis_vi_3a.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OX03C_DATA_TYPE_RAW16 // RAW16 or RAW12

#define OX03C_RATE_60FPS // 60fps or 30 fps

#define OX03C_I2C_ADDR          0x6c
#define MAX9296_I2C_ADDR        0x90
#define MAX9295_I2C_ADDR        0x80
#define MAX96717_I2C_ADDR       0x80
#define OX03C_ADDR_BYTE         2
#define OX03C_DATA_BYTE         1
#define I2C_DATA_BYTE 1
#define I2C_REG_ADDR_2BYTE 2
#define I2C_REG_ADDR_1BYTE 1
#define ox03c_sensor_get_ctx(pipe, ctx)   ((ctx) = ox03c_get_ctx(pipe))

#define OX03C_FULL_LINES_MAX 0xFFFFF
#define OX03C_FULL_LINES_MAX_BUILT_IN_WDR 0x19A28             //1750*60 (0.5fps)

#define OX03C_INCREASE_LINES 1 /* make real fps less than stand fps because NVR require */

#define OX03C_VMAX_1280P30_LINEAR                      (690)
#define OX03C_VMAX_1280P30_BUILT_IN_WDR                (690)
#define OX03C_VMAX_1280P90TO30_WDR                     (690)   /* 10bit */

typedef enum {
    OX03C_SENSOR_1280P_30FPS_LINEAR_MODE = 0,
    OX03C_SENSOR_1280P_30FPS_WDR_built_in_MODE,
    OX03C_SENSOR_1280P_30FPS_WDR_4TO1_MODE,
    OX03C_MODE_BUTT
} ox03c_res_mode;

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
} ox03c_video_mode_tbl;

typedef struct {
    ot_u32 dec[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32 inc[OT_EIS_VI_WDR_MAX_FRAME_NUM];
} time_step;
ot_eis_sns_state *ox03c_get_ctx(ot_vi_pipe_id vi_pipe);
ot_eis_sns_reg_table *ox03c_get_ext_sensor_table(ot_vi_pipe_id vi_pipe);
ot_u8 ox03c_get_ext_sensor_flag(ot_vi_pipe_id vi_pipe);
ot_eis_sns_bus_info *ox03c_get_bus_info(ot_vi_pipe_id vi_pipe);

ot_void ox03c_init(ot_vi_pipe_id vi_pipe);
ot_void ox03c_exit(ot_vi_pipe_id vi_pipe);
ot_void ox03c_standby(ot_vi_pipe_handle pipe_hdl);
ot_void ox03c_restart(ot_vi_pipe_handle pipe_hdl);
ot_void ox03c_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp);
ot_void ox03c_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en);

ot_s32 ox03c_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data);
ot_s32 ox03c_read_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 *data);
static ot_u32 cmos_again_calc_by_again(ot_u32 again_ref,float ratio);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* OX03C_CMOS_H */
