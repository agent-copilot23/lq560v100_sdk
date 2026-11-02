#ifndef IMX582_CMOS_H
#define IMX582_CMOS_H

#include "ot_eis_sns_ctrl.h"
#include "securec.h"
#include "ot_eis_video.h"
#include "ot_eis_vi_3a.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX582_I2C_ADDR          0x34
#define IMX582_ADDR_BYTE         2
#define IMX582_DATA_BYTE         1
#define I2C_DATA_BYTE            1
#define I2C_REG_ADDR_2BYTE       2
#define I2C_REG_ADDR_1BYTE       1
#define imx582_sensor_get_ctx(pipe, ctx)   ((ctx) = imx582_get_ctx(pipe))

#define IMX582_FULL_LINES_MAX 0xFFFFF

#define IMX582_INCREASE_LINES 1 /* make real fps less than stand fps because NVR require */

#define IMX582_VMAX_2160P30_LINEAR                    (2550) //0x09F6
#define IMX582_VMAX_4K3K20_LINEAR                     (3408) //0x17a6
#define IMX582_VMAX_8K6K5_LINEAR                      (7810) //0x1e82

typedef enum {
    IMX582_SENSOR_2160P_30FPS_LINEAR_MODE = 0,
	IMX582_SENSOR_4K3K_30FPS_LINEAR_MODE, 
    IMX582_SENSOR_8K6K_5FPS_LINEAR_MODE,
    IMX582_MODE_BUTT
} imx582_res_mode;

typedef struct {
    ot_u32      ver_lines;
    ot_u32      max_ver_lines;
    ot_float    max_fps;
    ot_float    min_fps;
    ot_u32      width;
    ot_u32      height;
    ot_u8       sns_mode;
    ot_eis_wdr_mode  wdr_mode;
    imx582_res_mode  img_mode;
} imx582_video_mode_tbl;

typedef struct {
    ot_u32 dec[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32 inc[OT_EIS_VI_WDR_MAX_FRAME_NUM];
} time_step;
ot_eis_sns_state *imx582_get_ctx(ot_vi_pipe_id vi_pipe);
ot_eis_sns_reg_table *imx582_get_ext_sensor_table(ot_vi_pipe_id vi_pipe);
ot_u8 imx582_get_ext_sensor_flag(ot_vi_pipe_id vi_pipe);
ot_eis_sns_bus_info *imx582_get_bus_info(ot_vi_pipe_id vi_pipe);

ot_void imx582_init(ot_vi_pipe_id vi_pipe);
ot_void imx582_exit(ot_vi_pipe_id vi_pipe);
ot_void imx582_standby(ot_vi_pipe_handle pipe_hdl);
ot_void imx582_restart(ot_vi_pipe_handle pipe_hdl);
ot_void imx582_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp);
ot_void imx582_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en);

ot_s32 imx582_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data);
ot_s32 imx582_read_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 *data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* IMX582_CMOS_H */
