/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include "ot_eis_errno.h"
#include "ot_eis_vi_sns.h"
#include "imx582_cmos_ex.h"
#include "imx582_cmos.h"
#include "ot_eis_vi_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_awb_api.h"
#include "ot_eis_vi_3a.h"
#include "math.h"

#define IMX582_ID                    8
#define IMX582_BLACK_LEVEL           0

#define IMX582_BLACK_LEVEL_0         0             //764 for pedestal 240,191(12bit) to 764(14bit) linear ,193(12bit) to 240(24bit) pwl
#define IMX582_BLACK_LEVEL_1         0             //772 for pedestal 240
#define IMX582_BLACK_LEVEL_2         0             //768 for pedestal 240
#define IMX582_BLACK_LEVEL_3         0             //760 for pedestal 240


#define high_h_h_8bits(x) (((x) & 0xff000000) >> 24)
#define high_h_8bits(x)  (((x) & 0x00ff0000)>>16)
#define higher_2bits(x) (((x) & 0x30000) >> 16)
#define high_8bits(x) (((x) & 0xff00) >> 8)
#define low_8bits(x)  ((x) & 0x00ff)
#define high_8_in_16_bit(x)  (((x) & 0xff00) >> 8)
#define low_8_in_16_bit(x)  ((x) & 0x00ff)
#define high_4_in_8_bit(x)  (((x) & 0xf0)>>4)
#define low_4_in_8_bit(x)  ((x) & 0x0f)
#define high_4_in_14_bit(x)  (((x) & 0x3c00)>>10)
#define mid_8_in_14_bit(x)  (((x) & 0x03fc)>>2)
#define low_2_in_14_bit(x)  ((x) & 0x0003)

#define MAX2(x, y)       ((x) > (y) ? (x) : (y))
#define MIN2(x, y)       ((x) < (y) ? (x) : (y))
#define clip3(x, min, max)         ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define shutter_min_row (375)
#define shutter_max_row_offset (15)      //min offset: 12 + shutter_max_row_vs
#define shutter_max_row_vs (3)      //max 35, usually use:0 - 3

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/
#define imx582_sensor_set_ctx(pipe, ctx)   ((g_imx582_sns_state[pipe]) = (ctx))
#define imx582_sensor_reset_ctx(pipe)      (g_imx582_sns_state[pipe] = OT_NULL)

static ot_eis_vi_fswdr_mode g_fswdr_mode[OT_EIS_VI_MAX_PIPE_NUM] = {
    [0 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = OT_EIS_VI_FSWDR_NORMAL_MODE
};

static ot_u32 g_max_time_get_cnt[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_u32 g_init_exposure[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_init_int_time[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_init_again[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_init_dgain[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_init_isp_dgain[OT_EIS_VI_MAX_PIPE_NUM]  = {0};
static ot_u32 g_lines_per500ms[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_u8 g_ae_stat_pos[OT_EIS_VI_MAX_PIPE_NUM] = {0};

static ot_u16 g_init_wb_gain[OT_EIS_VI_MAX_PIPE_NUM][OT_EIS_VI_RGB_CHN_NUM] = {{0}};
static ot_u16 g_sample_r_gain[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_u16 g_sample_b_gain[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_bool g_quick_start_en[OT_EIS_VI_MAX_PIPE_NUM] = {OT_FALSE};

static ot_bool g_ae_route_ex_valid[OT_EIS_VI_MAX_PIPE_NUM] = {0};
static ot_eis_vi_ae_route g_init_ae_route[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_vi_ae_route_ex g_init_ae_route_ex[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_vi_ae_route g_init_ae_route_sf[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_vi_ae_route_ex g_init_ae_route_sf_ex[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};

static ot_eis_vi_sns_alg_default  g_ext_sns_alg[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_vi_awb_sensor_default g_ext_sns_awb[OT_EIS_VI_MAX_PIPE_NUM] = {{0}};
static ot_eis_sns_reg_table  g_ext_table[OT_EIS_VI_MAX_PIPE_NUM][5] = {{0}};
static ot_u8  g_ext_alg_flag[OT_EIS_VI_MAX_PIPE_NUM] = { 0 };
static ot_u8  g_ext_awb_flag[OT_EIS_VI_MAX_PIPE_NUM] = { 0 };
static ot_u8  g_ext_sensor_table_flag[OT_EIS_VI_MAX_PIPE_NUM] = { 0 };

ot_eis_sns_bus_info g_imx582_bus_info[OT_EIS_VI_MAX_PIPE_NUM] = {
        [0] = {
            .com_bus.i2c_dev = 0,
            .bus_addr = IMX582_I2C_ADDR
        },
        [1 ... OT_EIS_VI_MAX_PIPE_NUM - 1] = {
            .com_bus.i2c_dev = -1,
            .bus_addr = IMX582_I2C_ADDR
        }
};

ot_eis_sns_state *g_imx582_sns_state[OT_EIS_VI_MAX_PIPE_NUM] = {OT_NULL};

static ot_bool blc_clamp_info[OT_EIS_VI_MAX_PIPE_NUM] = {[0 ...(OT_EIS_VI_MAX_PIPE_NUM - 1)] = OT_TRUE};

ot_eis_sns_reg_table *imx582_get_ext_sensor_table(ot_vi_pipe_id vi_pipe)
{
    return &g_ext_table[vi_pipe][0];
}

ot_u8 imx582_get_ext_sensor_flag(ot_vi_pipe_id vi_pipe)
{
    return g_ext_sensor_table_flag[vi_pipe];
}


ot_eis_sns_bus_info *imx582_get_bus_info(ot_vi_pipe_id vi_pipe)
{
    return &g_imx582_bus_info[vi_pipe];
}

ot_eis_sns_state *imx582_get_ctx(ot_vi_pipe_id vi_pipe)
{
    return g_imx582_sns_state[vi_pipe];
}

ot_void imx582_set_blc_clamp_value(ot_vi_pipe_id vi_pipe, ot_bool clamp_en)
{
    blc_clamp_info[vi_pipe] = clamp_en;
}

const imx582_video_mode_tbl g_imx582_mode_tbl[IMX582_MODE_BUTT] = {
    {IMX582_VMAX_2160P30_LINEAR, IMX582_FULL_LINES_MAX, 30, 1.17, 3840, 2160, 0, OT_EIS_WDR_MODE_NONE, IMX582_SENSOR_2160P_30FPS_LINEAR_MODE},
    {IMX582_VMAX_4K3K20_LINEAR,  IMX582_FULL_LINES_MAX, 30,  1.4, 4000, 3000, 1, OT_EIS_WDR_MODE_NONE, IMX582_SENSOR_4K3K_30FPS_LINEAR_MODE},
 //   {IMX582_VMAX_8K6K5_LINEAR,   IMX582_FULL_LINES_MAX,  5,  0.6, 8000, 6000, 2, OT_EIS_WDR_MODE_NONE, IMX582_SENSOR_8K6K_5FPS_LINEAR_MODE},
    {IMX582_VMAX_8K6K5_LINEAR,   IMX582_FULL_LINES_MAX,  5,  0.4, 8000, 6000, 2, OT_EIS_WDR_MODE_NONE, IMX582_SENSOR_8K6K_5FPS_LINEAR_MODE},    //2026.10.16  修改fps的下限，为0.4，以适应0.5的帧率
};

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
/* OX08B Register Address */
#define IMX582_SHT_COARSE_H                     (0x0202) /*  SHT_DCG[15:8]  */
#define IMX582_SHT_COARSE_L                     (0x0203) /*  SHT_DCG[7:0]  */

#define IMX582_AGAIN_H                          (0x0204) /*  AGAIN_HCG_H[7:4]   REG[3:0]*/
#define IMX582_AGAIN_L                          (0x0205) /*  AGAIN_HCG_H[3:0]   REG[7:4]*/

#define IMX582_DGAIN_H                          (0x020e) /*  AGAIN_HCG_H[7:4]   REG[3:0]*/
#define IMX582_DGAIN_L                          (0x020f) /*  AGAIN_HCG_H[3:0]   REG[7:4]*/

#define IMX582_DGAIN_HCG_H                      (0x350A) /*  DGAIN_HCG_H[13:10]  REG[3:0]*/
#define IMX582_DGAIN_HCG_M                      (0x350B) /*  DGAIN_HCG_M[9:2]   REG[7:0]*/

#define IMX582_VMAX_H                           (0x0340) /*  VMAX_H[15:8]   REG[7:0]*/
#define IMX582_VMAX_L                           (0x0341) /*  VMAX_L[7:0]   REG[7:0]*/


#define imx582_err_mode_print(sns_image_mode, sns_state) \
do { \
    sns_err_trace("Not support! Width:%u, Height:%u, Fps:%f, WDRMode:%d\n", \
                  (sns_image_mode)->width, \
                  (sns_image_mode)->height, \
                  (sns_image_mode)->fps, \
                  (sns_state)->wdr_mode); \
} while (0)

static ot_void cmos_get_ae_comm_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft,
    const ot_eis_sns_state *sns_state)
{
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    ae_sns_dft->flicker_freq = 50 * 256; /* light flicker freq: 50Hz, accuracy: 256 */
    ae_sns_dft->full_lines_max = IMX582_FULL_LINES_MAX;
    ae_sns_dft->hmax_times = (1000000000) / (sns_state->fl_std * 30); /* 1000000000ns, 60fps */


    ae_sns_dft->again_accu.accu_type = OT_EIS_VI_AE_ACCURACY_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = OT_EIS_VI_AE_ACCURACY_TABLE;
    ae_sns_dft->dgain_accu.accuracy = 1;

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1 << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 4 << ae_sns_dft->isp_dgain_shift; /* max 2 */
    if (g_lines_per500ms[vi_pipe] == 0) {
        ae_sns_dft->lines_per500ms = sns_state->fl_std * 30 / 2; /* 60fps, div 2 */
    } else {
        ae_sns_dft->lines_per500ms = g_lines_per500ms[vi_pipe];
    }
    ae_sns_dft->max_iris_fno = OT_EIS_VI_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_fno = OT_EIS_VI_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid = OT_FALSE;
    ae_sns_dft->ae_route_attr.total_num = 0;
    ae_sns_dft->ae_route_attr_ex.total_num = 0;
    ae_sns_dft->quick_start.quick_start_enable = g_quick_start_en[vi_pipe];
    ae_sns_dft->quick_start.black_frame_num = 0;
    ae_sns_dft->ae_stat_pos = g_ae_stat_pos[vi_pipe]; /* 1 use be stat to AE */
    return;
}

static ot_void cmos_get_ae_linear_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft,
    const ot_eis_sns_state *sns_state)
{
    ae_sns_dft->int_time_accu.accu_type = OT_EIS_VI_AE_ACCURACY_LINEAR;
    ae_sns_dft->int_time_accu.accuracy = 1; /* accuracy 1 */
    ae_sns_dft->int_time_accu.offset = 0;
    ae_sns_dft->max_again = 64*1024; /* max 32768 */
    ae_sns_dft->min_again = 1150;  /* min 1024 */
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 15.8*1024; /* max 257217 */
    ae_sns_dft->min_dgain = 1024; /* min 1024 */
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->isp_dgain_shift = 8; /* accuracy: 8 */
    ae_sns_dft->min_isp_dgain_target = 1   << ae_sns_dft->isp_dgain_shift;
    ae_sns_dft->max_isp_dgain_target = 255 << ae_sns_dft->isp_dgain_shift; /* max 255 */
    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode = OT_EIS_VI_AE_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_init_exposure[vi_pipe] ? g_init_exposure[vi_pipe] : 76151; /* init 76151 */
    ae_sns_dft->init_int_time = g_init_int_time[vi_pipe];
    ae_sns_dft->init_again = g_init_again[vi_pipe];
    ae_sns_dft->init_dgain = g_init_dgain[vi_pipe];
    ae_sns_dft->init_isp_dgain = g_init_isp_dgain[vi_pipe];

    ae_sns_dft->max_int_time = sns_state->fl_std - 48; /* sub 8 */
    ae_sns_dft->min_int_time = 5; /* min 2 */
    ae_sns_dft->max_int_time_target = 65535; /* max int 65535 */
    ae_sns_dft->min_int_time_target = 1;

    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[vi_pipe];
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr, sizeof(ot_eis_vi_ae_route),
                      &g_init_ae_route[vi_pipe],  sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&ae_sns_dft->ae_route_attr_ex, sizeof(ot_eis_vi_ae_route_ex),
                      &g_init_ae_route_ex[vi_pipe], sizeof(ot_eis_vi_ae_route_ex));

    return;
}

static ot_s32 cmos_get_ae_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(ae_sns_dft);
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (ot_void)memset_s(&ae_sns_dft->ae_route_attr, sizeof(ot_eis_vi_ae_route), 0, sizeof(ot_eis_vi_ae_route));

    cmos_get_ae_comm_default(vi_pipe, ae_sns_dft, sns_state);

    switch (sns_state->wdr_mode) {          //2026.10.15 修改VTS寄存器用来延长最大曝光时间
        case OT_EIS_WDR_MODE_NONE:   /* linear mode */
            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
            break;

        default:
            cmos_get_ae_linear_default(vi_pipe, ae_sns_dft, sns_state);
            break;
    }

    return OT_SUCCESS;
}

static ot_void cmos_config_vmax(ot_eis_sns_state *sns_state, ot_u32 vmax)
{
    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {
        sns_state->regs_info[0].i2c_data[5].data = low_8bits(vmax);
        sns_state->regs_info[0].i2c_data[4].data = high_8bits(vmax);
    } else {
        printf("invalid wdr_mode:%d\n",sns_state->wdr_mode);
    }
    printf("3333333333   vmax hight %lu  vmax low %lu \n",high_8bits(vmax),low_8bits(vmax));
    printf("3333333333   vmax hight %x vmax low %x \n",high_8bits(vmax),low_8bits(vmax));

    a=b;

    return;
}

ot_u32  AE_MANUAL_EXPOSURE = 0;     //2026.10.15  修改手动最大曝光时间
ot_u32  LAST_AE_MANUAL_EXPOSURE = 0; //2026。10.20  记录上一次设置的最大曝光时间

static ot_void cmos_fps_set(ot_vi_pipe_id vi_pipe, ot_float fps, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    
    ot_u32 vmax,vmax_offset;
    ot_bool achieve_fps_flag;
    
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_void_return(ae_sns_dft);
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    imx582_video_mode_tbl *imx582_mode = &g_imx582_mode_tbl[0];
    ot_u32   lines     = imx582_mode->ver_lines;
    ot_u32   lines_max = imx582_mode->max_ver_lines;
    ot_float max_fps   = imx582_mode->max_fps;
    ot_float min_fps   = imx582_mode->min_fps;

    for (int idx = 0; idx < IMX582_MODE_BUTT; idx++) {
        imx582_mode = &g_imx582_mode_tbl[idx];
        if (imx582_mode->img_mode == sns_state->img_mode) {
            lines     = imx582_mode->ver_lines;
            lines_max = imx582_mode->max_ver_lines;
            max_fps   = imx582_mode->max_fps;
            min_fps   = imx582_mode->min_fps;
        }
    }

    AE_MANUAL_EXPOSURE = lines;         //2026.10.15，修改AE值，以设置最大曝光时间
    LAST_AE_MANUAL_EXPOSURE = lines;         //2026.10.15，修改AE值，以设置最大曝光时间
    printf("000: AE_MANUAL_EXPOSURE :%x\n",AE_MANUAL_EXPOSURE);
    printf("001: lines_max :0x%x  lines_max : %lu\n",lines_max,lines_max);
    printf("002: lines :0x%x  lines : %lu\n",lines,lines);
    if ((fps > max_fps) || (fps < min_fps)) {
        sns_err_trace("Not support Fps: %f\n", fps);
        return;
    }

    achieve_fps_flag = OT_TRUE;
    vmax = (ot_u32)(lines * max_fps / sns_div_0_to_1_float(fps));
    vmax = (vmax > lines_max) ? lines_max : vmax;
    vmax = vmax + vmax % 2; /* mod 2 */
    cmos_config_vmax(sns_state, vmax);
    printf("003: vmax :0x%x  vmax : %lu\n",vmax,vmax);
    sns_state->fl_std = vmax;
    ae_sns_dft->lines_per500ms = lines * max_fps / 2; /* div 2 */
    g_lines_per500ms[vi_pipe] = ae_sns_dft->lines_per500ms;

    ae_sns_dft->fps = lines * max_fps * 0x40 / vmax / 0x40;
    ae_sns_dft->fps = (achieve_fps_flag) ? fps : (ae_sns_dft->fps);
    ae_sns_dft->full_lines_std = sns_state->fl_std;
    //2026.10.21  修改寄存器里的值，，让其接受更长时间的曝光时间
    //ae_sns_dft->max_int_time = sns_state->fl_std - 48; /* sub 8 */
    ae_sns_dft->max_int_time = 0xFFFF - 48;
    sns_state->fl[0] = sns_state->fl_std;
    ae_sns_dft->full_lines = sns_state->fl[0];
    ae_sns_dft->hmax_times = (ot_u32)((1000000000) / (sns_state->fl_std * sns_div_0_to_1_float(fps))); /* 1000000000ns */


    
    return;
}

static ot_void cmos_slow_framerate_set(ot_vi_pipe_id vi_pipe, ot_u32 full_lines, ot_eis_vi_ae_sensor_default *ae_sns_dft)
{
    return;
}

static ot_void cmos_inttime_update_linear(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;
    ot_u32 value;
    ot_u32 VTS_tmp;

    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    value = int_time - 1; /* mode 2 */

    value = MIN2(value, 0xFFFFF);
    //2026.10.20，觉得是在这里被限制住了
    //value = MIN2(MAX2(value, 5), sns_state->fl[0] - 48); /* min 5, sub 48 */
    
    sns_state->regs_info[0].i2c_data[1].data = low_8bits(value);
    sns_state->regs_info[0].i2c_data[0].data = high_8bits(value);

     printf("220: value :%u\n",value);

    if(value > (AE_MANUAL_EXPOSURE-8))
        VTS_tmp = value+8;
    else
        VTS_tmp = AE_MANUAL_EXPOSURE;


    if(VTS_tmp != LAST_AE_MANUAL_EXPOSURE)
    {
        sns_state->regs_info[0].i2c_data[5].data = low_8bits(VTS_tmp);   // VTS L
        sns_state->regs_info[0].i2c_data[4].data = high_8bits(VTS_tmp); // VTS H
        printf("111: i2c_data[4].data :%u  i2c_data[4].data :0x%x\n",sns_state->regs_info[0].i2c_data[4].data,sns_state->regs_info[0].i2c_data[4].data);
        printf("112: i2c_data[5].data :%u  i2c_data[5].data :0x%x\n",sns_state->regs_info[0].i2c_data[5].data,sns_state->regs_info[0].i2c_data[5].data);
        printf("113: VTS_tmp :%u\n",VTS_tmp);
        LAST_AE_MANUAL_EXPOSURE = VTS_tmp;
    }

    return;
}


/* while vi notify ae to update sensor regs, ae call these funcs. */
static ot_void cmos_inttime_update(ot_vi_pipe_id vi_pipe, ot_u32 int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
        //cmos_inttime_update_built_in_wdr(vi_pipe, int_time);
    } else {
        cmos_inttime_update_linear(vi_pipe, int_time);
    }

    return;
}
#define AGAIN_NODE_NUM   64
#define DGAIN_NODE_NUM   64
// static ot_u32 g_again_table[AGAIN_NODE_NUM] = {
// 1024,1088,1152,1216,1280,1344,1408,1472,1536,1600,1664,1728,1792,1856,1920,1984,2048,2176,2304,2432,2560,2688,2816,2944,3072,3200,3328,3456,3584,3712,3840,3968,4096,4352,4608,4864,5120,5376,5632,5888,6144,6400,6656,6912,7168,7424,7680,7936,8192,8704,9216,9728,10240,10752,11264,11776,12288,12800,13312,13824,14336,14848,15360,15872};
static ot_u32 g_dgain_table[DGAIN_NODE_NUM] = {
1024,1088,1152,1216,1280,1344,1408,1472,1536,1600,1664,1728,1792,1856,1920,1984,2048,2176,2304,2432,2560,2688,2816,2944,3072,3200,3328,3456,3584,3712,3840,3968,4096,4352,4608,4864,5120,5376,5632,5888,6144,6400,6656,6912,7168,7424,7680,7936,8192,8704,9216,9728,10240,10752,11264,11776,12288,12800,13312,13824,14336,14848,15360,15872};


static ot_void cmos_again_calc_table(ot_vi_pipe_id vi_pipe, ot_u32 *again_lin, ot_u32 *again_db)
{
    int i;

    sns_check_pointer_void_return(again_lin);
    sns_check_pointer_void_return(again_db);

    *again_lin = clip3(*again_lin, 1150, 65536); 
    *again_db = 1024-1048576/(*again_lin);
    *again_db = clip3(*again_db, 112, 1008); 

    *again_lin = 1048576/(1024 - *again_db);
    return;
}

static ot_void cmos_dgain_calc_table(ot_vi_pipe_id vi_pipe, ot_u32 *dgain_lin, ot_u32 *dgain_db)
{
    int i;

    sns_check_pointer_void_return(dgain_lin);
    sns_check_pointer_void_return(dgain_db);

    if (*dgain_lin >= g_dgain_table[DGAIN_NODE_NUM - 1]) {
        *dgain_lin = g_dgain_table[DGAIN_NODE_NUM - 1];
        *dgain_db = g_dgain_table[DGAIN_NODE_NUM - 1]>>2;
        return;
    }

    for (i = 1; i < DGAIN_NODE_NUM; i++) {
        if (*dgain_lin < g_dgain_table[i]) {
            *dgain_lin = g_dgain_table[i - 1];
            *dgain_db = g_dgain_table[i - 1]>>2;
            break;
        }
    }

    return;
}

static ot_void cmos_gains_update(ot_vi_pipe_id vi_pipe, ot_u32 again, ot_u32 dgain)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {  //hum
 		sns_state->regs_info[0].i2c_data[3].data = low_8bits(again); /* index 3 */
        sns_state->regs_info[0].i2c_data[2].data = high_8bits(again); /* index 4 */
 		sns_state->regs_info[0].i2c_data[7].data = low_8bits(dgain); /* index 3 */
        sns_state->regs_info[0].i2c_data[6].data = high_8bits(dgain); /* index 4 */
	} 
    return;
}

static ot_void cmos_get_inttime_max(ot_vi_pipe_id vi_pipe, ot_u16 man_ratio_enable, ot_u32 *ratio,
    ot_eis_vi_ae_int_time_range *int_time, ot_u32 *lf_max_int_time)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_void_return(ratio);
    sns_check_pointer_void_return(int_time);
    sns_check_pointer_void_return(lf_max_int_time);
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    return;
}

/* Only used in LINE_WDR mode */
static ot_void cmos_ae_fswdr_attr_set(ot_vi_pipe_id vi_pipe, ot_eis_vi_ae_fswdr_attr *ae_fswdr_attr)
{
    sns_check_pointer_void_return(ae_fswdr_attr);

    g_fswdr_mode[vi_pipe] = ae_fswdr_attr->fswdr_mode;
    g_max_time_get_cnt[vi_pipe] = 0;

    return;
}
/* awb static param for Fuji Lens New IR_Cut */                  //AWB calibration
#define CALIBRATE_STATIC_TEMP                         4950
#define CALIBRATE_STATIC_WB_R_GAIN                    384
#define CALIBRATE_STATIC_WB_GR_GAIN                   256
#define CALIBRATE_STATIC_WB_GB_GAIN                   256
#define CALIBRATE_STATIC_WB_B_GAIN                    397

/* Calibration results for Auto WB Planck */                       //AWB calibration
#define CALIBRATE_AWB_P1                              (85)
#define CALIBRATE_AWB_P2                              (59)
#define CALIBRATE_AWB_Q1                              (-112)
#define CALIBRATE_AWB_A1                              (360)
#define CALIBRATE_AWB_B1                              -50267
#define CALIBRATE_AWB_C1                              (9735822)

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN                                  0
#define GOLDEN_BGAIN                                  0
static ot_s32 cmos_get_awb_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_sensor_default *awb_sns_dft)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(awb_sns_dft);
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (ot_void)memset_s(awb_sns_dft, sizeof(ot_eis_vi_awb_sensor_default), 0, sizeof(ot_eis_vi_awb_sensor_default));

    if (g_ext_awb_flag[vi_pipe] == 1) {
        //g_ext_sns_awb include wb_cali_param & ccm & preawb param
        (ot_void)memcpy_s(awb_sns_dft, sizeof(ot_eis_vi_awb_sensor_default), &g_ext_sns_awb[vi_pipe],
                          sizeof(ot_eis_vi_awb_sensor_default));
        awb_sns_dft->init_rgain = g_init_wb_gain[vi_pipe][0]; /* 0: Rgain */
        awb_sns_dft->init_ggain = g_init_wb_gain[vi_pipe][1]; /* 1: Ggain */
        awb_sns_dft->init_bgain = g_init_wb_gain[vi_pipe][2]; /* 2: Bgain */
        awb_sns_dft->sample_rgain = g_sample_r_gain[vi_pipe];
        awb_sns_dft->sample_bgain = g_sample_b_gain[vi_pipe];
    } else {
        awb_sns_dft->wb_ref_temp = CALIBRATE_STATIC_TEMP; /* wb_ref_temp 4950 */

        awb_sns_dft->gain_offset[0] = CALIBRATE_STATIC_WB_R_GAIN;
        awb_sns_dft->gain_offset[1] = CALIBRATE_STATIC_WB_GR_GAIN;
        awb_sns_dft->gain_offset[2] = CALIBRATE_STATIC_WB_GB_GAIN; /* index 2 */
        awb_sns_dft->gain_offset[3] = CALIBRATE_STATIC_WB_B_GAIN; /* index 3 */

        awb_sns_dft->wb_para[0] = CALIBRATE_AWB_P1;
        awb_sns_dft->wb_para[1] = CALIBRATE_AWB_P2;
        awb_sns_dft->wb_para[2] = CALIBRATE_AWB_Q1; /* index 2 */
        awb_sns_dft->wb_para[3] = CALIBRATE_AWB_A1; /* index 3 */
        awb_sns_dft->wb_para[4] = CALIBRATE_AWB_B1; /* index 4 */
        awb_sns_dft->wb_para[5] = CALIBRATE_AWB_C1; /* index 5 */

        awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
        awb_sns_dft->golden_bgain = GOLDEN_BGAIN;

        awb_sns_dft->awb_run_interval = 1;

        switch (sns_state->wdr_mode) {
            case OT_EIS_WDR_MODE_NONE:
                (ot_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_eis_vi_awb_ccm), &g_awb_ccm, sizeof(ot_eis_vi_awb_ccm));
                (ot_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_eis_vi_awb_agc_table),
                                &g_awb_agc_table, sizeof(ot_eis_vi_awb_agc_table));
                break;

            case OT_EIS_WDR_MODE_BUILT_IN:
                (ot_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_eis_vi_awb_ccm), &g_awb_ccm_built_in, sizeof(ot_eis_vi_awb_ccm));
                (ot_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_eis_vi_awb_agc_table),
                                &g_awb_agc_table, sizeof(ot_eis_vi_awb_agc_table));

                break;

            default:
                (ot_void)memcpy_s(&awb_sns_dft->ccm, sizeof(ot_eis_vi_awb_ccm), &g_awb_ccm, sizeof(ot_eis_vi_awb_ccm));
                (ot_void)memcpy_s(&awb_sns_dft->agc_tbl, sizeof(ot_eis_vi_awb_agc_table),
                                &g_awb_agc_table, sizeof(ot_eis_vi_awb_agc_table));
                break;
        }
        awb_sns_dft->init_rgain = g_init_wb_gain[vi_pipe][0]; /* 0: Rgain */
        awb_sns_dft->init_ggain = g_init_wb_gain[vi_pipe][1]; /* 1: Ggain */
        awb_sns_dft->init_bgain = g_init_wb_gain[vi_pipe][2]; /* 2: Bgain */
        awb_sns_dft->sample_rgain = g_sample_r_gain[vi_pipe];
        awb_sns_dft->sample_bgain = g_sample_b_gain[vi_pipe];

        awb_sns_dft->preawb_attr.enable = 1;
        ot_u16 color_temp_array[OT_EIS_VI_AWB_PREAWB_LUT_SIZE]     = { 2400, 2800, 4000, 5000, 6500, 7500 };
        ot_u32 preawb_ratio_r_array[OT_EIS_VI_AWB_PREAWB_LUT_SIZE] = { 60702, 60702, 59379, 59116, 58656, 57339 };
        ot_u32 preawb_ratio_b_array[OT_EIS_VI_AWB_PREAWB_LUT_SIZE] = { 66074, 66074, 68168, 72199, 72180, 72491 };
        for (int i = 0; i < OT_EIS_VI_AWB_PREAWB_LUT_SIZE; i++) {
            awb_sns_dft->preawb_attr.color_temp_array[i] = color_temp_array[i];
            awb_sns_dft->preawb_attr.ratio_r_array[i] = preawb_ratio_r_array[i];
            awb_sns_dft->preawb_attr.ratio_b_array[i] = preawb_ratio_b_array[i];
        }
    }
    return OT_SUCCESS;
}

#define PREAWB_GAIN_PREC   10
#define PREAWB_GAIN_MAX 16383
#define PREAWB_GAIN_MIN 0
#define AWB_INFO_GAIN_PREC 8
static ot_s32 cmos_set_awb_preawb(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_preawb_info *preawb_info, ot_eis_vi_awb_info *awb_info)
{
    return OT_SUCCESS;
}


static void cmos_get_isp_linear_default(ot_eis_vi_sns_alg_default *isp_def, ot_vi_pipe_id vi_pipe)
{
	ot_eis_sns_state *sns_state = OT_NULL;
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);
	

    isp_def->key.bit1_demosaic         = 1;
    isp_def->demosaic                  = &g_cmos_demosaic;
    isp_def->key.bit1_sharpen          = 1;
    isp_def->sharpen                   = &g_cmos_yuv_sharpen;
    isp_def->key.bit1_drc              = 1;
    isp_def->drc                       = &g_cmos_drc;
	if (sns_state->img_mode == IMX582_SENSOR_8K6K_5FPS_LINEAR_MODE) {
		printf("bayer_nr use 8k6k\n");
		isp_def->key.bit1_bayer_nr         = 1;
		isp_def->bayer_nr                  = &g_cmos_bayer_nr_8k6k;
	}
    else {
		printf("bayer_nr use 4k3k or 2160p  =============================\n");
		isp_def->key.bit1_bayer_nr         = 1;
		isp_def->bayer_nr                  = &g_cmos_bayer_nr;
	}
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color          = &g_cmos_anti_false_color;
    isp_def->key.bit1_cac              = 1;
    isp_def->cac                       = &g_cmos_cac;
    isp_def->key.bit1_bshp             = 1;
    isp_def->bshp                      = &g_cmos_bayershp;
    isp_def->key.bit1_ldci             = 1;
    isp_def->ldci                      = &g_cmos_ldci;
    isp_def->key.bit1_gamma            = 1;
    isp_def->gamma                     = &g_cmos_gamma;
#ifdef CONFIG_OT_ISP_CR_SUPPORT
    isp_def->key.bit1_ge               = 1;
    isp_def->ge                        = &g_cmos_ge;
#endif
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze;
    isp_def->key.bit1_ca = 0;
    (ot_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_eis_vi_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_eis_vi_noise_calibration));
    return;
}

static void cmos_get_isp_built_in_default(ot_eis_vi_sns_alg_default *isp_def, ot_vi_pipe_id vi_pipe)
{
    isp_def->key.bit1_demosaic         = 1;
    isp_def->demosaic                  = &g_cmos_demosaic_built_in;
    isp_def->key.bit1_sharpen          = 1;
    isp_def->sharpen                   = &g_cmos_yuv_sharpen_built_in;
    isp_def->key.bit1_drc              = 1;
    isp_def->drc                       = &g_cmos_drc_built_in;
    isp_def->key.bit1_bayer_nr         = 1;
    isp_def->bayer_nr                  = &g_cmos_bayer_nr_built_in;
    isp_def->key.bit1_anti_false_color = 1;
    isp_def->anti_false_color          = &g_cmos_anti_false_color;
    isp_def->key.bit1_ldci             = 1;
    isp_def->ldci                      = &g_cmos_ldci_built_in;
    isp_def->key.bit1_gamma            = 1;
    isp_def->gamma                     = &g_cmos_gamma;

    isp_def->key.bit1_expander         = 1;
    isp_def->expander                  = &g_cmos_expander;

#ifdef CONFIG_OT_ISP_CR_SUPPORT
    isp_def->key.bit1_ge               = 1;
    isp_def->ge                        = &g_cmos_ge;
#endif
    isp_def->key.bit1_dehaze = 1;
    isp_def->dehaze = &g_cmos_dehaze;
    (ot_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_eis_vi_noise_calibration),
                      &g_cmos_noise_calibration, sizeof(ot_eis_vi_noise_calibration));
    return;
}
static ot_s32 cmos_get_vi_alg_default(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_alg_default *isp_def)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(isp_def);
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    (ot_void)memset_s(isp_def, sizeof(ot_eis_vi_sns_alg_default), 0, sizeof(ot_eis_vi_sns_alg_default));

    if (g_ext_alg_flag[vi_pipe] == 1) {
        isp_def->key.bit1_clut             = g_ext_sns_alg[vi_pipe].key.bit1_clut;
        isp_def->clut                      = g_ext_sns_alg[vi_pipe].clut;
        isp_def->key.bit1_dpc              = g_ext_sns_alg[vi_pipe].key.bit1_dpc;
        isp_def->dpc                       = g_ext_sns_alg[vi_pipe].dpc;
        isp_def->key.bit1_wdr              = g_ext_sns_alg[vi_pipe].key.bit1_wdr;
        isp_def->wdr                       = g_ext_sns_alg[vi_pipe].wdr;
        isp_def->key.bit1_lsc              = g_ext_sns_alg[vi_pipe].key.bit1_lsc;
        isp_def->lsc                       = g_ext_sns_alg[vi_pipe].lsc;
        isp_def->key.bit1_acs              = g_ext_sns_alg[vi_pipe].key.bit1_acs;
        isp_def->acs                       = g_ext_sns_alg[vi_pipe].acs;
        isp_def->key.bit1_demosaic         = g_ext_sns_alg[vi_pipe].key.bit1_demosaic;
        isp_def->demosaic                  = g_ext_sns_alg[vi_pipe].demosaic;
        isp_def->key.bit1_sharpen          = g_ext_sns_alg[vi_pipe].key.bit1_sharpen;
        isp_def->sharpen                   = g_ext_sns_alg[vi_pipe].sharpen;
        isp_def->key.bit1_drc              = g_ext_sns_alg[vi_pipe].key.bit1_drc;
        isp_def->drc                       = g_ext_sns_alg[vi_pipe].drc;
        isp_def->key.bit1_bayer_nr         = g_ext_sns_alg[vi_pipe].key.bit1_bayer_nr;
        isp_def->bayer_nr                  = g_ext_sns_alg[vi_pipe].bayer_nr;
        isp_def->key.bit1_anti_false_color = g_ext_sns_alg[vi_pipe].key.bit1_anti_false_color;
        isp_def->anti_false_color          = g_ext_sns_alg[vi_pipe].anti_false_color;
        isp_def->key.bit1_ldci             = g_ext_sns_alg[vi_pipe].key.bit1_ldci;
        isp_def->ldci                      = g_ext_sns_alg[vi_pipe].ldci;
        isp_def->key.bit1_gamma            = g_ext_sns_alg[vi_pipe].key.bit1_gamma;
        isp_def->gamma                     = g_ext_sns_alg[vi_pipe].gamma;
        isp_def->key.bit1_expander         = g_ext_sns_alg[vi_pipe].key.bit1_expander;
        isp_def->expander                  = g_ext_sns_alg[vi_pipe].expander;
        isp_def->key.bit1_cac         = g_ext_sns_alg[vi_pipe].key.bit1_cac;
        isp_def->cac                  = g_ext_sns_alg[vi_pipe].cac;
        isp_def->key.bit1_bshp         = g_ext_sns_alg[vi_pipe].key.bit1_bshp;
        isp_def->bshp                  = g_ext_sns_alg[vi_pipe].bshp;
        isp_def->key.bit1_lblc         = g_ext_sns_alg[vi_pipe].key.bit1_lblc;
        isp_def->lblc                  = g_ext_sns_alg[vi_pipe].lblc;
#ifdef CONFIG_OT_ISP_CA_SUPPORT
        isp_def->key.bit1_ca      = g_ext_sns_alg[vi_pipe].key.bit1_ca;
        isp_def->key.ca           = g_ext_sns_alg[vi_pipe].ca;
#endif
#ifdef CONFIG_OT_ISP_CR_SUPPORT
        isp_def->key.bit1_ge               = g_ext_sns_alg[vi_pipe].key.bit1_ge;
        isp_def->ge                        = g_ext_sns_alg[vi_pipe].ge;
#endif
        isp_def->key.bit1_dehaze           = g_ext_sns_alg[vi_pipe].key.bit1_dehaze;
        isp_def->dehaze                    = g_ext_sns_alg[vi_pipe].dehaze;
        (ot_void)memcpy_s(&isp_def->noise_calibration, sizeof(ot_eis_vi_noise_calibration),
                          &g_ext_sns_alg[vi_pipe].noise_calibration, sizeof(ot_eis_vi_noise_calibration));
    }
    else {
        isp_def->key.bit1_clut    = 1;
        isp_def->clut             = &g_cmos_clut;
        isp_def->key.bit1_dpc     = 1;
        isp_def->dpc              = &g_cmos_dpc;
        isp_def->key.bit1_wdr     = 1;
        isp_def->wdr              = &g_cmos_wdr;
		if (sns_state->img_mode == IMX582_SENSOR_8K6K_5FPS_LINEAR_MODE) {
			printf("lsc use 8k6k\n");
			isp_def->key.bit1_lsc      = 1;   
			isp_def->lsc               = &g_cmos_lsc_8k6k;
		}
		else if (sns_state->img_mode == IMX582_SENSOR_4K3K_30FPS_LINEAR_MODE) {
			printf("lsc use 4k3k\n");
			isp_def->key.bit1_lsc      = 1;   
			isp_def->lsc               = &g_cmos_lsc_4k3k;
		}
		else if (sns_state->img_mode == IMX582_SENSOR_2160P_30FPS_LINEAR_MODE) {
			printf("lsc use 2160p\n");
			isp_def->key.bit1_lsc      = 1;   
			isp_def->lsc               = &g_cmos_lsc_2160p;
		}
		else {
			printf("lsc use defualt\n");
			isp_def->key.bit1_lsc      = 1;   
			isp_def->lsc               = &g_cmos_lsc;
		}
        isp_def->key.bit1_acs      = 0;
        isp_def->acs               = &g_cmos_acs;
#ifdef CONFIG_OT_ISP_CA_SUPPORT
        isp_def->key.bit1_ca      = 0;
#endif
        switch (sns_state->wdr_mode) {
            case OT_EIS_WDR_MODE_NONE:
                cmos_get_isp_linear_default(isp_def, vi_pipe);
                break;
            case OT_EIS_WDR_MODE_BUILT_IN:
                cmos_get_isp_built_in_default(isp_def, vi_pipe);
                break;
            default:
                cmos_get_isp_linear_default(isp_def, vi_pipe);
                break;
        }

    }

    isp_def->wdr_switch_attr.exp_ratio[0] = 0x40;

    isp_def->sns_mode.sns_id   = IMX582_ID;
    isp_def->sns_mode.sns_mode = sns_state->img_mode;

    return OT_SUCCESS;
}

static ot_s32 cmos_get_vi_black_level(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_black_level *black_level)
{
    ot_s32  i;
    ot_eis_sns_state *sns_state = OT_NULL;
    const ot_eis_vi_sns_black_level *cmos_blc_def = OT_NULL;

    sns_check_pointer_return(black_level);
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if (g_ext_alg_flag[vi_pipe] == 1) {
        cmos_blc_def = g_ext_sns_alg[vi_pipe].sns_blc;
    } else {
        if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE || sns_state->wdr_mode == OT_EIS_WDR_MODE_BUILT_IN) {
            cmos_blc_def = &g_cmos_blc;
        } else {
            cmos_blc_def = &g_cmos_blc_wdr;
        }
    }

    (ot_void)memcpy_s(black_level, sizeof(ot_eis_vi_sns_black_level), cmos_blc_def, sizeof(ot_eis_vi_sns_black_level));

    return OT_SUCCESS;
}

static ot_s32 cmos_get_blc_clamp_info(ot_vi_pipe_id vi_pipe, ot_bool *blc_clamp_en)
{
    sns_check_pointer_return(blc_clamp_en);

    *blc_clamp_en = blc_clamp_info[vi_pipe];

    return OT_SUCCESS;
}

static ot_void cmos_set_pixel_detect(ot_vi_pipe_id vi_pipe, ot_bool enable)
{
    ot_u32 full_lines_5fps;
    ot_eis_sns_state *sns_state = OT_NULL;

    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);

    if (sns_state->wdr_mode == OT_EIS_WDR_MODE_2To1_LINE) {
        return;
    } 
    else {
        if (sns_state->img_mode == IMX582_SENSOR_2160P_30FPS_LINEAR_MODE) {
            full_lines_5fps = (IMX582_VMAX_2160P30_LINEAR * 30) / 5; /* 30fps, 5fps */
        }
        else if (sns_state->img_mode == IMX582_SENSOR_4K3K_30FPS_LINEAR_MODE) {  
            full_lines_5fps = (IMX582_VMAX_4K3K20_LINEAR * 30) / 5; /* 30fps, 5fps */
        } 
		else if (sns_state->img_mode == IMX582_SENSOR_8K6K_5FPS_LINEAR_MODE) {  
            full_lines_5fps = (IMX582_VMAX_8K6K5_LINEAR * 5) / 5; /* 5fps, 5fps */
        }
    }
    return;
}

static ot_s32 cmos_set_wdr_mode(ot_vi_pipe_id vi_pipe, ot_u8 mode)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    sns_state->sync_init = OT_FALSE;

    switch (mode & 0x3F) {
        case OT_EIS_WDR_MODE_NONE:
            sns_state->wdr_mode = OT_EIS_WDR_MODE_NONE;
            printf("linear mode\n");
            break;
        case OT_EIS_WDR_MODE_BUILT_IN:
            sns_state->wdr_mode = OT_EIS_WDR_MODE_BUILT_IN;
            printf("built-in mode\n");
            break;



        default:
            sns_err_trace("NOT support this mode!\n");
            return OT_FAILURE;
    }

    (ot_void)memset_s(sns_state->wdr_int_time, sizeof(sns_state->wdr_int_time), 0, sizeof(sns_state->wdr_int_time));

    return OT_SUCCESS;
}

static ot_void cmos_comm_sns_reg_info_init(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
    ot_u32 i;
    sns_state->regs_info[0].sns_type = OT_EIS_VI_SNS_TYPE_I2C;
    sns_state->regs_info[0].com_bus.i2c_dev = g_imx582_bus_info[vi_pipe].com_bus.i2c_dev;
    sns_state->regs_info[0].cfg2_valid_delay_max = 2; /* delay_max 2 */
    sns_state->regs_info[0].reg_num = 8; // TODO

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        sns_state->regs_info[0].i2c_data[i].update = OT_TRUE;
        sns_state->regs_info[0].i2c_data[i].dev_addr = g_imx582_bus_info[vi_pipe].bus_addr;
        sns_state->regs_info[0].i2c_data[i].addr_byte_num = IMX582_ADDR_BYTE;
        sns_state->regs_info[0].i2c_data[i].data_byte_num = IMX582_DATA_BYTE;
    }

    return;
}

static ot_void cmos_linear_sns_reg_info_init(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{

    sns_state->regs_info[0].i2c_data[0].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[0].reg_addr = IMX582_SHT_COARSE_H;
    sns_state->regs_info[0].i2c_data[1].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[1].reg_addr = IMX582_SHT_COARSE_L;
    sns_state->regs_info[0].i2c_data[2].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[2].reg_addr = IMX582_AGAIN_H;
    sns_state->regs_info[0].i2c_data[3].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[3].reg_addr = IMX582_AGAIN_L;
    sns_state->regs_info[0].i2c_data[4].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[4].reg_addr = IMX582_VMAX_H;
    sns_state->regs_info[0].i2c_data[5].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[5].reg_addr = IMX582_VMAX_L;
    sns_state->regs_info[0].i2c_data[6].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[6].reg_addr = IMX582_DGAIN_H;
    sns_state->regs_info[0].i2c_data[7].delay_frame_num = 0;
    sns_state->regs_info[0].i2c_data[7].reg_addr = IMX582_DGAIN_L;
    return;
}



static ot_void cmos_sns_reg_info_update(ot_vi_pipe_id vi_pipe, ot_eis_sns_state *sns_state)
{
    ot_u32 i;

    for (i = 0; i < sns_state->regs_info[0].reg_num; i++) {
        if (sns_state->regs_info[0].i2c_data[i].data ==
            sns_state->regs_info[1].i2c_data[i].data) {
            sns_state->regs_info[0].i2c_data[i].update = OT_FALSE;
        } else {
            sns_state->regs_info[0].i2c_data[i].update = OT_TRUE;
        }
    }
    return;
}

static ot_s32 cmos_get_sns_regs_info(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_regs_info *sns_regs_info)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    sns_check_pointer_return(sns_regs_info);
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    if ((sns_state->sync_init == OT_FALSE) || (sns_regs_info->config == OT_FALSE)) {
        cmos_comm_sns_reg_info_init(vi_pipe, sns_state);

        if (sns_state->wdr_mode == OT_EIS_WDR_MODE_NONE) {
            cmos_linear_sns_reg_info_init(vi_pipe, sns_state);
        }
        sns_state->sync_init = OT_TRUE;
    } else {
        cmos_sns_reg_info_update(vi_pipe, sns_state);
    }

    sns_regs_info->config = OT_FALSE;
    (ot_void)memcpy_s(sns_regs_info, sizeof(ot_eis_vi_sns_regs_info),
                      &sns_state->regs_info[0], sizeof(ot_eis_vi_sns_regs_info));
    (ot_void)memcpy_s(&sns_state->regs_info[1], sizeof(ot_eis_vi_sns_regs_info),
                      &sns_state->regs_info[0], sizeof(ot_eis_vi_sns_regs_info));
    sns_state->fl[1] = sns_state->fl[0];

    return OT_SUCCESS;
}

static ot_void cmos_config_image_mode_param(ot_vi_pipe_id vi_pipe, ot_u8 sns_image_mode,
    ot_eis_sns_state *sns_state)
{
    switch (sns_image_mode) {
        case IMX582_SENSOR_2160P_30FPS_LINEAR_MODE:
            sns_state->fl_std = IMX582_VMAX_2160P30_LINEAR;
            break;
        case IMX582_SENSOR_4K3K_30FPS_LINEAR_MODE:
            sns_state->fl_std = IMX582_VMAX_4K3K20_LINEAR;
            break;
		case IMX582_SENSOR_8K6K_5FPS_LINEAR_MODE:
            sns_state->fl_std = IMX582_VMAX_8K6K5_LINEAR;
            break;
        default:
            sns_state->fl_std = IMX582_VMAX_2160P30_LINEAR;
            break;
    }

    return;
}

static ot_s32 cmos_set_image_mode(ot_vi_pipe_id vi_pipe, const ot_eis_vi_sns_image_mode *sns_image_mode)
{
    ot_u8 image_mode = g_imx582_mode_tbl[0].img_mode;
    ot_eis_sns_state *sns_state = OT_NULL;
    sns_check_pointer_return(sns_image_mode);
    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_return(sns_state);

    for (int i = 0; i < IMX582_MODE_BUTT; i++) {
        imx582_video_mode_tbl *imx582_mode = &g_imx582_mode_tbl[i];
        if (imx582_mode->sns_mode == sns_image_mode->sns_mode) {
            image_mode = imx582_mode->img_mode;
        }
    }

    cmos_config_image_mode_param(vi_pipe, image_mode, sns_state);

    if ((sns_state->init == OT_TRUE) && (image_mode == sns_state->img_mode)) {
        return OT_EIS_VI_DO_NOT_NEED_SWITCH_IMAGEMODE; /* Don't need to switch image_mode */
    }
	printf("cmos_set_image_mode =%d\n", image_mode);
    sns_state->sync_init = OT_FALSE;
    sns_state->img_mode = image_mode;
    sns_state->fl[0] = sns_state->fl_std;
    sns_state->fl[1] = sns_state->fl[0];

    return OT_SUCCESS;
}

static ot_void sensor_global_init(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state = OT_NULL;

    imx582_sensor_get_ctx(vi_pipe, sns_state);
    sns_check_pointer_void_return(sns_state);
    sns_state->init      = OT_FALSE;
    sns_state->sync_init = OT_FALSE;
    sns_state->img_mode  = IMX582_SENSOR_2160P_30FPS_LINEAR_MODE;
    sns_state->wdr_mode  = OT_EIS_WDR_MODE_NONE;
    sns_state->fl_std    = IMX582_VMAX_2160P30_LINEAR;
    sns_state->fl[0]     = IMX582_VMAX_2160P30_LINEAR;
    sns_state->fl[1]     = IMX582_VMAX_2160P30_LINEAR;

    (ot_void)memset_s(&sns_state->regs_info[0], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));
    (ot_void)memset_s(&sns_state->regs_info[1], sizeof(ot_eis_vi_sns_regs_info), 0, sizeof(ot_eis_vi_sns_regs_info));

    return;
}

static ot_s32 sensor_ctx_init(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    imx582_sensor_get_ctx(vi_pipe, sns_state_ctx);

    if (sns_state_ctx == OT_NULL) {
        sns_state_ctx = (ot_eis_sns_state *)malloc(sizeof(ot_eis_sns_state));
        if (sns_state_ctx == OT_NULL) {
            sns_err_trace("Isp[%d] SnsCtx malloc memory failed!\n", vi_pipe);
            return OT_FAILURE;
        }
    }

    (ot_void)memset_s(sns_state_ctx, sizeof(ot_eis_sns_state), 0, sizeof(ot_eis_sns_state));

    imx582_sensor_set_ctx(vi_pipe, sns_state_ctx);

    return OT_SUCCESS;
}

static ot_void sensor_ctx_exit(ot_vi_pipe_id vi_pipe)
{
    ot_eis_sns_state *sns_state_ctx = OT_NULL;

    imx582_sensor_get_ctx(vi_pipe, sns_state_ctx);
    sns_free(sns_state_ctx);
    imx582_sensor_reset_ctx(vi_pipe);

    return;
}

static ot_eis_vi_sns_register_func g_imx582_register_to_vi_func = {
    .pfn_sns_init                = imx582_init,
    .pfn_sns_exit                = imx582_exit,
    .pfn_sns_global_init         = sensor_global_init,
    .pfn_sns_set_image_mode      = cmos_set_image_mode,
    .pfn_sns_set_wdr_mode        = cmos_set_wdr_mode,
    .pfn_sns_get_vi_alg_default  = cmos_get_vi_alg_default,
    .pfn_sns_get_vi_black_level  = cmos_get_vi_black_level,
    .pfn_sns_get_blc_clamp_info  = cmos_get_blc_clamp_info,
    .pfn_sns_set_pixel_detect    = cmos_set_pixel_detect,
    .pfn_sns_get_reg_info        = cmos_get_sns_regs_info,
};

static ot_eis_vi_ae_sensor_register_func g_imx582_register_to_ae_func = {
    .pfn_sns_get_ae_default         = cmos_get_ae_default,
    .pfn_sns_cfg_fps                = cmos_fps_set,
    .pfn_sns_cfg_slow_framerate     = cmos_slow_framerate_set,
    .pfn_sns_update_inttime         = cmos_inttime_update,
    .pfn_sns_update_gains           = cmos_gains_update,
    .pfn_sns_calc_table_again       = cmos_again_calc_table,
    .pfn_sns_calc_table_dgain       = cmos_dgain_calc_table,
    .pfn_sns_get_inttime_max        = cmos_get_inttime_max,
    .pfn_sns_set_fswdr_attr         = cmos_ae_fswdr_attr_set,
    .pfn_sns_set_quick_start_status = OT_NULL,
    .pfn_sns_convert_exp_param      = OT_NULL,
    .pfn_sns_get_thermo_default     = OT_NULL,
} ;

static ot_eis_vi_awb_sensor_register_func g_imx582_register_to_awb_func = {
    .pfn_sns_get_awb_default = cmos_get_awb_default,
    .pfn_sns_set_awb_preawb = cmos_set_awb_preawb,
};

static ot_s32 sensor_register_callback(ot_vi_pipe_handle pipe_hdl)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    ret = sensor_ctx_init(master_pipe_id);
    if (ret != OT_SUCCESS) {
        return OT_FAILURE;
    }

    ret = ot_eis_vi_sensor_register_to_vi(pipe_hdl, &g_imx582_register_to_vi_func, IMX582_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to vi failed!\n");
        goto free_sns_ctx;
    }

    ret = ot_eis_vi_sensor_register_to_ae(pipe_hdl, &g_imx582_register_to_ae_func, IMX582_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to ae lib failed!\n");
        goto sns_unreg_to_vi;
    }

    ret = ot_eis_vi_sensor_register_to_awb(pipe_hdl, &g_imx582_register_to_awb_func, IMX582_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor register to awb lib failed!\n");
        goto sns_unreg_to_ae;
    }

    return OT_SUCCESS;

sns_unreg_to_ae:
    ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, IMX582_ID);
sns_unreg_to_vi:
    ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, IMX582_ID);
free_sns_ctx:
    sensor_ctx_exit(master_pipe_id);
    return ret;
}

static ot_s32 sensor_unregister_callback(ot_vi_pipe_handle pipe_hdl)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    ret = ot_eis_vi_sensor_unregister_to_vi(pipe_hdl, IMX582_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }
    ret = ot_eis_vi_sensor_unregister_to_ae(pipe_hdl, IMX582_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_eis_vi_sensor_unregister_to_awb(pipe_hdl, IMX582_ID);
    if (ret != OT_SUCCESS) {
        sns_err_trace("sensor unregister callback function to awb lib failed!\n");
        return ret;
    }

    sensor_ctx_exit(master_pipe_id);

    return OT_SUCCESS;
}

static ot_s32 imx582_set_bus_info(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_bus_info *sns_bus_info)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    memcpy_s(&g_imx582_bus_info[master_pipe_id], sizeof(ot_eis_sns_bus_info), sns_bus_info, sizeof(ot_eis_sns_bus_info));

    return OT_SUCCESS;
}

static ot_s32 sensor_set_init(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_init_attr *init_attr)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];


    sns_check_pointer_return(init_attr);

    g_init_exposure[master_pipe_id]  = init_attr->exposure;
    g_init_int_time[master_pipe_id]  = init_attr->exp_time;
    g_init_again[master_pipe_id]     = init_attr->a_gain;
    g_init_dgain[master_pipe_id]     = init_attr->d_gain;
    g_init_isp_dgain[master_pipe_id] = init_attr->ispd_gain;
    g_lines_per500ms[master_pipe_id] = init_attr->lines_per500ms;
    g_init_wb_gain[master_pipe_id][0] = init_attr->wb_r_gain; /* 0: rgain */
    g_init_wb_gain[master_pipe_id][1] = init_attr->wb_g_gain; /* 1: ggain */
    g_init_wb_gain[master_pipe_id][2] = init_attr->wb_b_gain; /* 2: bgain */
    g_sample_r_gain[master_pipe_id] = init_attr->sample_r_gain;
    g_sample_b_gain[master_pipe_id] = init_attr->sample_b_gain;
    g_quick_start_en[master_pipe_id] = init_attr->quick_start_en;
    g_ae_stat_pos[master_pipe_id]       = init_attr->ae_stat_pos;
    g_ae_route_ex_valid[master_pipe_id] = init_attr->ae_route_ex_valid;
    (ot_void)memcpy_s(&g_init_ae_route[master_pipe_id], sizeof(ot_eis_vi_ae_route),
                      &init_attr->ae_route, sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&g_init_ae_route_ex[master_pipe_id], sizeof(ot_eis_vi_ae_route_ex),
                      &init_attr->ae_route_ex, sizeof(ot_eis_vi_ae_route_ex));
    (ot_void)memcpy_s(&g_init_ae_route_sf[master_pipe_id], sizeof(ot_eis_vi_ae_route),
                      &init_attr->ae_route_sf, sizeof(ot_eis_vi_ae_route));
    (ot_void)memcpy_s(&g_init_ae_route_sf_ex[master_pipe_id], sizeof(ot_eis_vi_ae_route_ex),
                      &init_attr->ae_route_sf_ex, sizeof(ot_eis_vi_ae_route_ex));

    return OT_SUCCESS;
}

static ot_s32 imx582_set_alg_default(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_sns_alg_default *sns_alg_def)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    if (sns_alg_def == OT_NULL) {
        return OT_FAILURE;
    }
    g_ext_sns_alg[master_pipe_id] = *sns_alg_def;
    g_ext_alg_flag[master_pipe_id] = 1;

    return OT_SUCCESS;
}

static ot_s32 imx582_set_awb_default(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_sensor_default *sns_awb_def)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    if(sns_awb_def == OT_NULL) {
        return OT_FAILURE;
    }
    g_ext_sns_awb[master_pipe_id] = *sns_awb_def;
    g_ext_awb_flag[master_pipe_id] = 1;
    return OT_SUCCESS;
}

static ot_s32 imx582_set_reg_table(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_reg_table *reg_table, ot_s32 table_cnt)
{
    ot_s32 ret;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    g_ext_table[master_pipe_id][0] = *reg_table;
    g_ext_table[master_pipe_id][1] = *(reg_table + 1);
    g_ext_table[master_pipe_id][2] = *(reg_table + 2);
    g_ext_table[master_pipe_id][3] = *(reg_table + 3);
    g_ext_table[master_pipe_id][4] = *(reg_table + 4);
    g_ext_sensor_table_flag[master_pipe_id] = 1;
    return OT_SUCCESS;
}

ot_eis_sns_obj g_sns_imx582_obj = {
    .pfn_sns_register              = sensor_register_callback,
    .pfn_sns_unregister            = sensor_unregister_callback,
    .pfn_sns_standby               = imx582_standby,
    .pfn_sns_restart               = imx582_restart,
    .pfn_sns_set_mirror_flip       = OT_NULL,
    .pfn_sns_set_blc_clamp         = imx582_blc_clamp,
    .pfn_sns_set_bus_info          = imx582_set_bus_info,
    .pfn_sns_set_init              = sensor_set_init,
    .pfn_sns_set_alg_default       = imx582_set_alg_default,
    .pfn_sns_set_awb_default       = imx582_set_awb_default,
    .pfn_sns_set_reg_table         = imx582_set_reg_table
};
