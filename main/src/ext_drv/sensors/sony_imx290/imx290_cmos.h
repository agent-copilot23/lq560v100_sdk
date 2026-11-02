#ifndef IMX290_CMOS_H
#define IMX290_CMOS_H

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

#define IMX290_I2C_ADDR    0x34
#define IMX290_ADDR_BYTE   2
#define IMX290_DATA_BYTE   1
#define imx290_sensor_get_ctx(pipe, ctx)   ((ctx) = imx290_get_ctx(pipe))

#define IMX290_FULL_LINES_MAX 0xFFFF
#define IMX290_FULL_LINES_MAX_2TO1_WDR 0x34BC   /*5fps*/
#define IMX290_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
#define IMX290_VMAX_2M_60FPS_12BIT_LINEAR     (1125 + IMX290_INCREASE_LINES)
#define IMX290_VMAX_2M_60FPS_10BIT_2TO1_WDR   (1125 + IMX290_INCREASE_LINES)   /* 10bit */
#define IMX290_VMAX_2M_30FPS_12BIT_2TO1_WDR   (1125 + IMX290_INCREASE_LINES)   /* 12bit */

typedef enum {
    IMX290_2M_60FPS_12BIT_LINEAR_MODE = 0,
    IMX290_2M_30FPS_12BIT_2TO1_DOL_MODE,
    IMX290_2M_60FPS_10BIT_2TO1_DOL_MODE,
    IMX290_MODE_BUTT
} imx290_res_mode;

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
} imx290_video_mode_tbl;

typedef enum {
    EXPO_L_IDX = 0,
    EXPO_H_IDX,
    AGAIN_L_IDX,
    AGAIN_H_IDX,
    DGAIN_L_IDX,
    DGAIN_H_IDX,
    VMAX_L_IDX,
    VMAX_H_IDX,
    REG_MAX_IDX
}imx290_linear_reg_index;

typedef enum {
    SHS1_L_IDX = 0,
    SHS1_M_IDX,
    SHS1_H_IDX,
    SHS2_L_IDX,
    SHS2_M_IDX,
    SHS2_H_IDX,
    RHS1_L_IDX,
    RHS1_M_IDX,
    RHS1_H_IDX,
    LEF_GAIN_IDX,
    SEF1_GAIN_IDX,
    WDR_VMAX_L_IDX,
    WDR_VMAX_M_IDX,
    WDR_VMAX_H_IDX,
    Y_OUT_SIZE_L_IDX,
    Y_OUT_SIZE_H_IDX,
    WDR_REG_MAX_IDX
}imx290_wdr_reg_index;

typedef struct {
    ot_u32 dec[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32 inc[OT_EIS_VI_WDR_MAX_FRAME_NUM];
} time_step;
ot_eis_sns_state *imx290_get_ctx(ot_vi_pipe_id vi_pipe);
ot_eis_sns_bus_info *imx290_get_bus_info(ot_vi_pipe_id vi_pipe);

ot_void imx290_init(ot_vi_pipe_id vi_pipe);
ot_void imx290_exit(ot_vi_pipe_id vi_pipe);
ot_void imx290_standby(ot_vi_pipe_handle pipe_hdl);
ot_void imx290_restart(ot_vi_pipe_handle pipe_hdl);
ot_void imx290_blc_clamp(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp blc_clamp);
ot_void imx290_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en);

ot_s32  imx290_write_register(ot_vi_pipe_id vi_pipe, ot_u32 addr, ot_u32 data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* IMX290_CMOS_H */
