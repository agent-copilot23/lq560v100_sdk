#ifndef IMX623_CMOS_H
#define IMX623_CMOS_H

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

#define I2C_DATA_BYTE      1
#define I2C_REG_ADDR_2BYTE 2
#define I2C_REG_ADDR_1BYTE 1
#define imx623_sensor_get_ctx(pipe, ctx)   ((ctx) = imx623_get_ctx(pipe))

#define IMX623_FULL_LINES_MAX 0xFFFFF
#define IMX623_FULL_LINES_MAX_BUILT_IN_WDR 0x19A28             //1750*60 (0.5fps)

#define IMX623_INCREASE_LINES 1 /* make real fps less than stand fps because NVR require */

#define IMX623_VMAX_1552P30_LINEAR                            (1626)                       //for 60fps
#define IMX623_VMAX_1552P30_BUILT_IN_WDR                      (1626)
#define IMX623_VMAX_1552P_120TO30_WDR                         (1626)   /* 10bit */

typedef enum {
    IMX623_SENSOR_1552P_30FPS_LINEAR_MODE = 0,
    IMX623_SENSOR_1552P_30FPS_WDR_built_in_MODE,
    IMX623_SENSOR_1552P_30FPS_WDR_4TO1_MODE,
    IMX623_MODE_BUTT
} imx623_res_mode;

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
} imx623_video_mode_tbl;

typedef struct {
    ot_u32 dec[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32 inc[OT_EIS_VI_WDR_MAX_FRAME_NUM];
} time_step;
ot_eis_sns_state *imx623_get_ctx(ot_vi_pipe_id vi_pipe);
ot_eis_sns_reg_table *imx623_get_ext_sensor_table(ot_vi_pipe_id vi_pipe);
ot_u8 imx623_get_ext_sensor_flag(ot_vi_pipe_id vi_pipe);
ot_eis_sns_bus_info *imx623_get_bus_info(ot_vi_pipe_id vi_pipe);

ot_void imx623_init(ot_vi_pipe_id vi_pipe);
ot_void imx623_exit(ot_vi_pipe_id vi_pipe);
ot_void imx623_standby(ot_vi_pipe_handle pipe_hdl);
ot_void imx623_restart(ot_vi_pipe_handle pipe_hdl);
ot_void imx623_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp);
ot_void imx623_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en);

ot_s32  imx623_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* IMX623_CMOS_H */
