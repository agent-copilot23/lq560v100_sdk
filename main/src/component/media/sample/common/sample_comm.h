/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef SAMPLE_COMM_H
#define SAMPLE_COMM_H

#include <pthread.h>

#include "ot_eis_common.h"
#include "securec.h"
#include "ot_eis_errno.h"

//#include "ot_common_sys.h"

#include "ot_eis_mipi_rx.h"
#include "ot_eis_type.h"
#include "ot_eis_vi.h"
#include "ot_eis_vi_alg.h"
#include "ot_buffer_pool_common.h"
#include "ot_smr.h"
#include "ot_eis_media_sys.h"
#include "ot_vrb.h"
#include "ot_eis_math.h"

#include "ot_buffer_pool.h"
#include "ot_media_pipe.h"

#include "ot_eis_vo_in_port.h"
#include "ot_eis_vo_surface.h"
#include "ot_eis_vo_display.h"
#include "ot_eis_vo_sys.h"
#include "ot_eis_vo_type.h"
#include "ot_eis_vdec.h"
#include "ot_eis_venc.h"

#include "ot_eis_vproc_common.h"
#include "ot_eis_vproc_err.h"
#include "ot_eis_vproc_grp.h"
#include "ot_eis_vproc_pipe.h"
#include "ot_eis_vproc_chn.h"
#include "ot_eis_vproc.h"
#include "ot_eis_vi_serdes.h"
#include "ot_eis_vo_serdes.h"
#include "ot_eis_vi_sns.h"
#include "ot_eis_vi_awb.h"
#include "ot_eis_sns_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */


/* macro define */
#define FILE_NAME_LEN 128
#define FILE_PATH_LEN 128

//#define USE_JSON_FILE

#define SAMPLE_PIXEL_FORMAT OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420

#define COLOR_RGB_RED      0xFF0000
#define COLOR_RGB_GREEN    0x00FF00
#define COLOR_RGB_BLUE     0x0000FF
#define COLOR_RGB_BLACK    0x000000
#define COLOR_RGB_YELLOW   0xFFFF00
#define COLOR_RGB_CYN      0x00ffff
#define COLOR_RGB_WHITE    0xffffff

#define SAMPLE_VO_DEV_DHD0 0                  /* VO's device HD0 */
#define SAMPLE_VO_DEV_UHD  SAMPLE_VO_DEV_DHD0 /* VO's ultra HD device:HD0 */
#define SAMPLE_VO_LAYER_VHD0 0
#define SAMPLE_RGN_HANDLE_NUM_MAX 16
#define SAMPLE_RGN_HANDLE_NUM_MIN 1

#define SAMPLE_AUDIO_EXTERN_AI_DEV 0
#define SAMPLE_AUDIO_EXTERN_AO_DEV 0
#define SAMPLE_AUDIO_INNER_AI_DEV 0
#define SAMPLE_AUDIO_INNER_AO_DEV 0

#define SAMPLE_AUDIO_POINT_NUM_PER_FRAME 480
#define SAMPLE_AUDIO_AI_USER_FRAME_DEPTH 5

#define WDR_MAX_PIPE_NUM 4
#define VI_MAX_PIPE_NUM 4

#define CHN_NUM_PRE_DEV            4
#define SECOND_CHN_OFFSET_2MUX     2

#define D1_WIDTH            720
#define D1_HEIGHT_PAL       576
#define D1_HEIGHT_NTSC      480

#define _960H_WIDTH         960
#define _960H_HEIGHT_PAL    576
#define _960H_HEIGHT_NTSC   480

#define HD_WIDTH            1280
#define HD_HEIGHT           720

#define FHD_WIDTH           1920
#define FHD_HEIGHT          1080

#define _4K_WIDTH           3840
#define _4K_HEIGHT          2160

#define WIDTH_2688          2688
#define WIDTH_2592          2592
#define HEIGHT_1520         1520

#define WIDTH_1936          1936
#define HEIGHT_1552         1552

#define WIDTH_1280          1280
#define HEIGHT_768          768

#define AD_NVP6158 0
#define AD_TP2856 1

#define SAMPLE_VIO_MAX_ROUTE_NUM 4
#define SAMPLE_VIO_POOL_NUM 2

#define MIPI_DEV0_I2C_BUS   6
#define MIPI_DEV2_I2C_BUS   5

#define SAMPLE_AD_TYPE AD_TP2856

#define NVP6158_FILE "/dev/nc_vdec"
#define TP2856_FILE "/dev/tp2802dev"
#define TP2828_FILE "/dev/tp2823dev"

#define ACODEC_FILE "/dev/acodec"

#define ES8388_FILE "/dev/es8388"
#define ES8388_CHIP_ID 0

#define VO_LT8618SX 1
#define LT8618SX_DEV_NAME "/dev/lt8618sx"

#define VO_MIPI_SUPPORT 1
#define MIPI_TX_DEV_NAME "/dev/ot_mipi_tx"

#define SAMPLE_FRAME_BUF_RATIO_MAX 100
#define SAMPLE_FRAME_BUF_RATIO_MIN 70

#define VPROC_FRAME_QUEUE_DEPTH 4
#define VPROC_FRAME_RATE_DEFAULT -1

#define minor_chn(vi_chn) ((vi_chn) + 1)

#define OT_VO_OUTPUT_FRAME_RATE_60         (60)
#define OT_VO_OUTPUT_FRAME_RATE_50         (50)
#define OT_VO_OUTPUT_FRAME_RATE_30         (30)

#define SCREEN_VDD_GPIO_GRP     10
#define SCREEN_VDD_GPIO_PIN     7

#define LIGHT_SENSOR_MAX_NUM    2

#define sample_pause() \
    do { \
        printf("---------------press enter key to exit!---------------\n"); \
        getchar(); \
    } while (0)

#define sample_print(fmt...) \
    do { \
        printf("[%s]-%d: ", __FUNCTION__, __LINE__); \
        printf(fmt); \
    } while (0)

#define check_null_ptr_return(ptr) \
    do { \
        if ((ptr) == OT_NULL) { \
            printf("func:%s,line:%d, NULL pointer\n", __FUNCTION__, __LINE__); \
            return OT_FAILURE; \
        } \
    } while (0)

#define check_chn_return(express, chn, name) \
    do { \
        ot_s32 ret_ = (express); \
        if (ret_ != OT_SUCCESS) { \
            printf("\033[0;31m%s chn %d failed at %s: LINE: %d with %#x!\033[0;39m\n", \
                   (name), (chn), __FUNCTION__, __LINE__, ret_); \
            fflush(stdout); \
            return ret_; \
        } \
    } while (0)

#define check_return(express, name) \
    do { \
        ot_s32 ret_ = (express); \
        if (ret_ != OT_SUCCESS) { \
            printf("\033[0;31m%s failed at %s: LINE: %d with %#x!\033[0;39m\n", \
                   (name), __FUNCTION__, __LINE__, ret_); \
            return ret_; \
        } \
    } while (0)

#define sample_check_eok_return(ret, err_code) \
    do { \
        if ((ret) != EOK) { \
            printf("%s:%d:strncpy_s failed.\n", __FUNCTION__, __LINE__); \
            return (err_code); \
        } \
    } while (0)

#define rgn_check_handle_num_return(handle_num) \
    do { \
        if (((handle_num) < SAMPLE_RGN_HANDLE_NUM_MIN) || ((handle_num) > SAMPLE_RGN_HANDLE_NUM_MAX)) { \
            sample_print("handle_num(%d) should be in [%d, %d].\n", \
                (handle_num), SAMPLE_RGN_HANDLE_NUM_MIN, SAMPLE_RGN_HANDLE_NUM_MAX); \
            return OT_FAILURE; \
        } \
    } while (0)

#define check_digit(x) ((x) >= '0' && (x) <= '9')

/* structure define */
typedef enum {
    PIC_CIF,
    PIC_360P,    /* 640 * 360 */
    PIC_D1_PAL,  /* 720 * 576 */
    PIC_D1_NTSC, /* 720 * 480 */
    PIC_960H,      /* 960 * 576 */
    PIC_720P,    /* 1280 * 720 */
    PIC_1080P,   /* 1920 * 1080 */
    PIC_480P,
    PIC_576P,
    PIC_800X600,
    PIC_1024X768,
    PIC_1280X960,
    PIC_1280X1024,
    PIC_1366X768,
    PIC_1440X900,
    PIC_1280X800,
    PIC_1600X1200,
    PIC_1680X1050,
    PIC_1920X1200,
    PIC_640X480,
    PIC_1920X2160,
    PIC_2560X1440,
    PIC_2560X1600,
    PIC_2592X1520,
    PIC_2688X1520,
    PIC_2592X1944,
    PIC_3840X2160,
    PIC_4096X2160,
    PIC_3000X3000,
    PIC_4000X3000,
    PIC_6080X2800,
    PIC_7680X4320,
    PIC_3840X8640,
    PIC_BUTT
} ot_pic_size;

typedef enum {
    OT_VO_SAMPLE_OUT_PAL = 0, /* PAL standard */
    OT_VO_SAMPLE_OUT_NTSC = 1, /* NTSC standard */
    OT_VO_SAMPLE_OUT_960H_PAL = 2, /* ITU-R BT.1302 960 x 576 at 50 Hz (interlaced) */
    OT_VO_SAMPLE_OUT_960H_NTSC = 3, /* ITU-R BT.1302 960 x 480 at 60 Hz (interlaced) */

    OT_VO_SAMPLE_OUT_640x480_60 = 4, /* VESA 640 x 480 at 60 Hz (non-interlaced) CVT */
    OT_VO_SAMPLE_OUT_480P60 = 5, /* 720 x 480 at 60 Hz. */
    OT_VO_SAMPLE_OUT_576P50 = 6, /* 720 x 576 at 50 Hz. */
    OT_VO_SAMPLE_OUT_800x600_60 = 7, /* VESA 800 x 600 at 60 Hz (non-interlaced) */
    OT_VO_SAMPLE_OUT_1024x768_60 = 8, /* VESA 1024 x 768 at 60 Hz (non-interlaced) */
    OT_VO_SAMPLE_OUT_720P50 = 9, /* 1280 x 720 at 50 Hz. */
    OT_VO_SAMPLE_OUT_720P60 = 10, /* 1280 x 720 at 60 Hz. */
    OT_VO_SAMPLE_OUT_1280x800_60 = 11, /* 1280*800@60Hz VGA@60Hz */
    OT_VO_SAMPLE_OUT_1280x1024_60 = 12, /* VESA 1280 x 1024 at 60 Hz (non-interlaced) */
    OT_VO_SAMPLE_OUT_1366x768_60 = 13, /* VESA 1366 x 768 at 60 Hz (non-interlaced) */
    OT_VO_SAMPLE_OUT_1400x1050_60 = 14, /* VESA 1400 x 1050 at 60 Hz (non-interlaced) CVT */
    OT_VO_SAMPLE_OUT_1440x900_60 = 15, /* VESA 1440 x 900 at 60 Hz (non-interlaced) CVT Compliant */
    OT_VO_SAMPLE_OUT_1680x1050_60 = 16, /* VESA 1680 x 1050 at 60 Hz (non-interlaced) */

    OT_VO_SAMPLE_OUT_1080P24 = 17, /* 1920 x 1080 at 24 Hz. */
    OT_VO_SAMPLE_OUT_1080P25 = 18, /* 1920 x 1080 at 25 Hz. */
    OT_VO_SAMPLE_OUT_1080P30 = 19, /* 1920 x 1080 at 30 Hz. */
    OT_VO_SAMPLE_OUT_1080I50 = 20, /* 1920 x 1080 at 50 Hz, interlaced. */
    OT_VO_SAMPLE_OUT_1080I60 = 21, /* 1920 x 1080 at 60 Hz, interlaced. */
    OT_VO_SAMPLE_OUT_1080P50 = 22, /* 1920 x 1080 at 50 Hz. */
    OT_VO_SAMPLE_OUT_1080P60 = 23, /* 1920 x 1080 at 60 Hz. */

    OT_VO_SAMPLE_OUT_1600x1200_60 = 24, /* VESA 1600 x 1200 at 60 Hz (non-interlaced) */
    OT_VO_SAMPLE_OUT_1920x1200_60 = 25, /* VESA 1920 x 1200 at 60 Hz (non-interlaced) CVT (Reduced Blanking) */
    OT_VO_SAMPLE_OUT_1920x2160_30 = 26, /* 1920x2160_30 */
    OT_VO_SAMPLE_OUT_2560x1440_30 = 27, /* 2560x1440_30 */
    OT_VO_SAMPLE_OUT_2560x1440_60 = 28, /* 2560x1440_60 */
    OT_VO_SAMPLE_OUT_2560x1600_60 = 29, /* 2560x1600_60 */

    OT_VO_SAMPLE_OUT_3840x2160_24 = 30, /* 3840x2160_24 */
    OT_VO_SAMPLE_OUT_3840x2160_25 = 31, /* 3840x2160_25 */
    OT_VO_SAMPLE_OUT_3840x2160_30 = 32, /* 3840x2160_30 */
    OT_VO_SAMPLE_OUT_3840x2160_50 = 33, /* 3840x2160_50 */
    OT_VO_SAMPLE_OUT_3840x2160_60 = 34, /* 3840x2160_60 */
    OT_VO_SAMPLE_OUT_4096x2160_24 = 35, /* 4096x2160_24 */
    OT_VO_SAMPLE_OUT_4096x2160_25 = 36, /* 4096x2160_25 */
    OT_VO_SAMPLE_OUT_4096x2160_30 = 37, /* 4096x2160_30 */
    OT_VO_SAMPLE_OUT_4096x2160_50 = 38, /* 4096x2160_50 */
    OT_VO_SAMPLE_OUT_4096x2160_60 = 39, /* 4096x2160_60 */
    OT_VO_SAMPLE_OUT_7680x4320_30 = 40, /* 7680x4320_30 */

    OT_VO_SAMPLE_OUT_240x320_50 = 41, /* 240x320_50 */
    OT_VO_SAMPLE_OUT_320x240_50 = 42, /* 320x240_50 */
    OT_VO_SAMPLE_OUT_240x320_60 = 43, /* 240x320_60 */
    OT_VO_SAMPLE_OUT_320x240_60 = 44, /* 320x240_60 */
    OT_VO_SAMPLE_OUT_800x600_50 = 45, /* 800x600_50 */

    OT_VO_SAMPLE_OUT_720x1280_60 = 46, /* For MIPI DSI Tx 720 x1280 at 60 Hz */
    OT_VO_SAMPLE_OUT_1080x1920_60 = 47, /* For MIPI DSI Tx 1080x1920 at 60 Hz */

    OT_VO_SAMPLE_OUT_USER = 48, /* User timing. */

    OT_VO_SAMPLE_OUT_BUTT,
} ot_vo_sample_intf_sync;

typedef enum {
    OV_OX08B40_MIPI_8M_30FPS_12BIT,
    OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT,
    OV_OX03C_MIPI_3M_60FPS_16BIT,
    OV_OX03F10_MIPI_3M_60FPS_12BIT,
    OV_OX03F10_MIPI_3M_60FPS_16BIT,
    OV_OX05B1S_MIPI_5M_60FPS_10BIT,
    SONY_IMX623_MIPI_3M_60FPS_16BIT,
    SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1,
    SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1,
    GC_GC2093_MIPI_2M_30FPS_10BIT,
    GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1,
    SMS_SC220AT_MIPI_2M_30FPS_YUV422,
    SMS_SC220AT_MIPI_2M_30FPS_RAW12,
    SONY_IMX582_MIPI_8M_30FPS_10BIT_DIRECT,
    SONY_IMX582_MIPI_12M_20FPS_10BIT_DIRECT,
    SONY_IMX582_MIPI_48M_5FPS_10BIT_DIRECT,
    SAMPLE_SNS_DUMMY,

    SNS_TYPE_BUTT,
} sample_sns_type;

typedef enum {
    SAMPLE_SNS_ZERO,
    SAMPLE_SNS_ONE,
    SAMPLE_SNS_TWO,
    SAMPLE_SNS_THREE,
    SAMPLE_SNS_FOUR,
    SAMPLE_SNS_MAX
} sample_sns_num;

typedef enum {
    PWM_ID_0,
    PWM_ID_1,
    PWM_ID_BUTT,
} sample_pwm_id;

typedef enum {
    ONE_MIPT_ONE_SENSOR, // 仅一个端口输入, MAX96717 和 MAX9295 配置可复用
    ONE_MIPI_TWO_SENSOR, // 两个端口均输入, MAX96717 和 MAX9295 配置可复用
    VI_SERDES_MODE_BUTT,
} sample_vi_serdes_mode;

typedef struct {
    ot_char vi_serdes_name[OT_EIS_VI_SERDES_NAME_MAX];
    sample_vi_serdes_mode vi_serdes_mode;
} sample_vi_serdes;

typedef struct {
    ot_char vo_serdes_name[OT_EIS_VO_SERDES_NAME_MAX];
    ot_eis_vo_serdes_fpdlink_mode vo_serdes_mode;
} sample_vo_serdes;

typedef enum {
    LIGHT_SENSOR_ID_0,
    LIGHT_SENSOR_ID_1,
    LIGHT_SENSOR_ID_BUTT,
} sample_light_sensor_id;

typedef struct {
    ot_u8 src_addr;
    ot_u8 dst_addr;
} sample_light_sensor_port_attr;

typedef struct {
    ot_bool is_used;
    ot_u8 light_sensor_num;
    sample_light_sensor_port_attr port_attr[LIGHT_SENSOR_MAX_NUM];
} sample_light_sensor;

typedef enum {
    TM070JDHP08_1280X768,
    AV069HDT_1280X720,
    VO_BT1120_1280X720,
    JC07027001_800X1280,
    DISPLAY_DEV_TYPE_BUTT,
} sample_vo_display_dev_type;

typedef enum {
    ONE_IN_PORT_MIPI_AV069HDT_1280X720,
    ONE_IN_PORT_MIPI_AV069HDT_960X540,
    ONE_IN_PORT_MIPI_768,
    TWO_IN_PORT_MIPI_768,
    ONE_IN_PORT_MIPI_800X1280,
    ONE_IN_PORT_BT1120_1280X720,
    TWO_IN_PORT_MIPI_AV069HDT_2560X720,
    TWO_IN_PORT_MIPI_2560X768,
    VO_DISPLAY_MODE_BUTT,
} sample_vo_display_mode;

typedef enum {
    ONE_MIPI_MAX9295_MAX9296_IMX623 = 0, // one mipi one sensor
    ONE_MIIP_MAX9295_MAX9296_GC2093,
    ONE_MIPI_MAX9295_MAX9296_IMX290,
    ONE_MIPI_MAX9295_MAX9296_OX03C,
    ONE_MIPI_MAX9295_MAX9296_OX08B40,
    ONE_MIPI_MAX9295_MAX9296_OX03F10,
    ONE_MIPI_MAX9295_MAX9296_SC220AT,
    ONE_MIPI_MAX9295_MAX9296_OX05B1S,
    ONE_MIPI_DIRECT_IMX582,
    ONE_MIPI_ONE_SNS_MAX,

    TWO_MIPI_MAX9295_MAX9296_IMX623_IMX623 = 20, // two mipi two sensor
    TWO_MIPI_MAX9295_MAX9296_GC2093_IMX290,
    TWO_MIPI_MAX9295_MAX9296_IMX290_GC2093,
    SAMPLE_INPUT_PARAM_MAX,
} sample_input_param;

typedef enum {
    TI941_TI948_TM070JDHP08_1280X768,
    TI941_TI948_AV069HDT_1280X720,
    TI941_TI948_JC07027001_800X1280,
    TI941_TI948_TM070JDHP08_1280X768_VO_2_PORT, // vo 2 port - one display panel
    SCREEN_BT1120_INTERFACE_1280X720,
    SAMPLE_OUTPUT_PARAM_ONE_DISPLAY_MAX,

    TI941_TI948_TM070JDHP08_2560X768_SEPARATE = 20, // separate mode ,vo 2 port - two display panel
    TI941_TI948_AV069HDT_2560X720_SEPARATE,
    SAMPLE_OUTPUT_PARAM_MAX,
} sample_output_param;

typedef struct {
    ot_s32 sample_index;
    sample_input_param input_param;
    sample_output_param output_param;
} sample_param_comb;

typedef struct {
    sample_sns_type sns_type;
    ot_u32          sns_clk_src;
    ot_u32          sns_rst_src;
    ot_s32          bus_id;
    ot_bool         sns_clk_rst_en;
} sample_sns_info;

typedef struct {
    ot_eis_vi_serdes_handle serdes_hdl;
    ot_eis_vi_serdes_attr   serdes_attr;
} sample_vi_serdes_info;

typedef struct {
    ot_eis_vo_serdes_handle serdes_hdl;
    ot_eis_vo_serdes_attr   serdes_attr;
} sample_vo_serdes_info;

typedef struct {
    ot_eis_vi_if_init_attr init_attr;
    ot_eis_vi_if_attr  mipi_attr;
    ot_eis_vi_intf_ext_attr mipi_ext_attr;
} sample_mipi_info;

typedef struct {
    ot_eis_vi_dev_attr  dev_attr; //input
    ot_vi_dev_handle    dev_hdl; //out
} sample_vi_dev_info;

typedef struct {
    ot_vi_chn_id        chn_id; //input
    ot_eis_vi_chn_attr  chn_attr; //input
    ot_vi_chn_handle    chn_hdl; //out
} sample_vi_chn_info;

typedef struct {
    ot_eis_vi_pipe_attr pipe_attr; //input
    ot_vi_pipe_handle   pipe_hdl; //out
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX];

    sample_vi_chn_info chn_info;

    ot_eis_3dnr_attr   nr_attr;

    ot_bool quickboot_en;
} sample_vi_pipe_info;

typedef struct {
    sample_sns_info     sns_info;
    sample_mipi_info    mipi_info;
    ot_eis_handle           mipi_hdl;
    sample_vi_serdes_info serdes_info;

    sample_vi_dev_info dev_info;
    ot_u32 pipe_num;
    sample_vi_pipe_info pipe_info[VI_MAX_PIPE_NUM];

    ot_eis_handle           media_pipe_hdl;
} sample_vi_cfg;

typedef struct {
    ot_buffer_pool_blk  pool_blk;
    ot_u32              blk_size;
    ot_eis_video_frame frame_info;
} sample_vi_user_frame_info;

typedef struct {
    ot_eis_op_mode     op_mode;
    ot_bool                   aibnr_mode;
    ot_eis_vi_fpn_type        fpn_type;
    ot_u32                    strength;
    ot_eis_img_pixel_format   pixel_format;
    sample_vi_user_frame_info user_frame_info;
} sample_vi_fpn_correction_cfg;

typedef struct {
    ot_u32      offset;
} sample_scene_fpn_offset_cfg;

typedef struct {
    ot_eis_module_id    mod_id;
    ot_s32      dev_num;
    ot_s32      chn_num;
} ot_eis_mod_attr;

typedef struct {
    ot_bool in_port_enable[OT_VO_MAX_BIND_PORT_NUM];
    ot_eis_handle in_port_handle[OT_VO_MAX_BIND_PORT_NUM];
    ot_eis_vo_in_port_attr in_port_attr[OT_VO_MAX_BIND_PORT_NUM];
    ot_eis_vo_surface_bind_attr surface_bind_attr[OT_VO_MAX_BIND_PORT_NUM];
    ot_eis_vo_mirror_attr mirror_attr[OT_VO_MAX_BIND_PORT_NUM];
    ot_eis_vo_rot_attr rot_attr[OT_VO_MAX_BIND_PORT_NUM];

    ot_bool surface_enable[OT_VO_MAX_BIND_SURF_NUM];
    ot_eis_handle surface_handle[OT_VO_MAX_BIND_SURF_NUM];
    ot_eis_vo_surface surface[OT_VO_MAX_BIND_SURF_NUM];
    ot_eis_vo_surface_attr surface_attr[OT_VO_MAX_BIND_SURF_NUM];
    ot_eis_vo_crop_attr crop_attr[OT_VO_MAX_BIND_SURF_NUM];
    ot_eis_vo_mirror_attr surface_mirror_attr[OT_VO_MAX_BIND_SURF_NUM];
    ot_eis_vo_csc_attr csc_attr[OT_VO_MAX_BIND_SURF_NUM];
    ot_eis_vo_display_bind_attr display_bind_attr[OT_VO_MAX_BIND_SURF_NUM];

    ot_bool display_enable[OT_VO_MAX_DISP_NUM];
    ot_eis_handle display_handle[OT_VO_MAX_DISP_NUM];
    ot_eis_vo_display display[OT_VO_MAX_DISP_NUM];
    ot_eis_vo_display_attr display_attr[OT_VO_MAX_DISP_NUM];
    ot_eis_handle mipi_tx_hdl;

    sample_vo_serdes_info serdes_info;
    sample_vo_display_mode display_mode;
    sample_light_sensor light_sensor_attr;
} sample_vo_attr;

typedef struct {
    sample_sns_type sns_type[SAMPLE_SNS_MAX];
} sample_vi_attr;

typedef struct {
    ot_eis_vproc_mirror_param mirror_param;
    ot_eis_vproc_flip_param flip_param;
    ot_eis_vproc_crop_param crop_param;
    ot_eis_vproc_mosaic_param mosaic_param;
    ot_eis_vproc_line_param line_param;
    ot_eis_vproc_cover_param cover_param;
    ot_eis_vproc_rect_param rect_param;
    ot_eis_vproc_osd_param osd_param;
    ot_eis_vproc_gdc_param gdc_param;
    ot_eis_vproc_fisheye_param fisheye_param;
    ot_eis_vproc_ldc_param ldc_param;
    ot_eis_vproc_pmf_param pmf_param;
    ot_eis_vproc_xylut_param xylut_param;
    ot_eis_3dnr_attr nr_param;
    ot_eis_vproc_scale_coef_param scale_coef_param;
    ot_eis_vproc_chn_rotation rotation_param;
    ot_eis_vproc_luma_param luma_param;
} sample_vproc_set_attr;

typedef struct {
    ot_eis_handle pipe_hdl[OT_EIS_VPROC_GRP_PIPE_MAX_NUM];
    ot_eis_vproc_pipe_attr pipe_attr[OT_EIS_VPROC_GRP_PIPE_MAX_NUM];

    ot_eis_handle grp_hdl;
    ot_eis_vproc_grp_attr grp_attr;

    ot_eis_handle chn_hdl[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_eis_vproc_chn_attr chn_attr[OT_EIS_VPROC_GRP_CHN_MAX_NUM];

    ot_bool is_set_attr;
    sample_vproc_set_attr set_attr;
} sample_vproc_attr;

#define REGION_OP_CHN               (0x01L << 0)
#define REGION_OP_DEV               (0x01L << 1)
#define REGION_DESTROY                  (0x01L << 2)
typedef ot_u32 region_op_flag;

typedef enum {
    THREAD_CTRL_START,
    THREAD_CTRL_PAUSE,
    THREAD_CTRL_STOP,
} thread_contrl;

typedef struct {
    ot_eis_vdec_chn_handle chn_hdl;
    ot_vdec_codec_format type;
    ot_char c_file_path[FILE_PATH_LEN];
    ot_char c_file_name[FILE_NAME_LEN];
    ot_s32 milli_sec;
    ot_s32 min_buf_size;
    ot_s32 interval_time;
    thread_contrl e_thread_ctrl;
    ot_u64 pts_init;
    ot_u64 pts_increase;
    ot_bool circle_send;
    ot_u64 last_time;
    ot_u64 time_gap;
    ot_u64 fps;
} vdec_thread_param;

typedef struct {
    ot_u32 pic_buf_size;
    ot_u32 tmv_buf_size;
    ot_bool pic_buf_alloc;
    ot_bool tmv_buf_alloc;
} sample_vdec_buf;

typedef struct {
    ot_eis_vdec_chn_attr chn_attr;
    ot_eis_vdec_chn_handle vdec_hdl;
} sample_vdec_attr;


typedef struct {
    ot_eis_venc_chn_attr venc_attr;
    ot_eis_venc_chn_handle venc_hdl;
}sample_venc_attr;

typedef struct {
    ot_void (*set_alg)(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_alg_default *alg);
    ot_void (*set_awb)(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_sensor_default *sns_awb_param);
    ot_void (*set_sensor_table)(ot_vi_pipe_id vi_pipe, ot_eis_sns_reg_table *sns_table_param);
} ot_sns_set_param;

extern ot_sns_set_param g_imx623_tuning_obj;
extern ot_sns_set_param g_ox03c_tuning_obj;
extern ot_sns_set_param g_ox03f_tuning_obj;
extern ot_sns_set_param g_ox08b40_tuning_obj;
extern ot_sns_set_param g_sc220at_tuning_obj;

/* function announce */
ot_void sample_sys_signal(void (*func)(int));
ot_void *sample_sys_io_mmap(ot_u64 phy_addr, ot_u32 size);
ot_s32 sample_sys_munmap(ot_void *vir_addr, ot_u32 size);
ot_s32 sample_sys_set_reg(ot_u64 addr, ot_u32 value);
ot_s32 sample_sys_get_reg(ot_u64 addr, ot_u32 *value);

ot_s32 sample_comm_media_pipe_init(ot_eis_handle *pip_hdl);
ot_void sample_comm_media_pipe_stop(ot_eis_handle pip_hdl);

ot_s32 sample_common_get_buffer_pool_cfg(const ot_eis_img_attr *img_attr, ot_u32 *blk_size);
ot_s32 sample_common_get_vrb_cfg(ot_u32 pipe_num, ot_u32 width, ot_u32 height, ot_video_buffer_attr *buffer_attr);
ot_void sample_common_get_pic_buf_cfg(const ot_eis_img_attr *img_attr, ot_eis_buf_size_calc_cfg *calc_cfg);


ot_s32 sample_comm_sys_get_pic_size(ot_pic_size pic_size, ot_eis_img_size *size);
ot_pic_size sample_comm_sys_get_pic_enum(const ot_eis_img_size *size);
ot_s32 sample_comm_sys_init(const ot_video_buffer_attr *buf_cfg);
ot_void sample_comm_sys_exit(ot_void);
ot_s32 sample_comm_sys_set_vi_vproc_init_cfg(ot_eis_vi_vproc_mode_type mode_type);

ot_s32 sample_comm_vi_bind_vo(ot_eis_handle vi_chn_hdl, ot_eis_handle vo_inport_hdl, ot_eis_handle pip_hdl);
ot_s32 sample_comm_vi_un_bind_vo(ot_eis_handle vi_chn_hdl, ot_eis_handle vo_inport_hdl, ot_eis_handle pip_hdl);
ot_s32 sample_comm_vi_bind_vproc(ot_eis_handle vi_chn_hdl, ot_eis_handle vproc_pipe_hdl, ot_eis_handle pip_hdl);
ot_s32 sample_comm_vi_un_bind_vproc(ot_eis_handle vi_chn_hdl, ot_eis_handle vproc_pipe_hdl, ot_eis_handle pip_hdl);

ot_s32 sample_comm_vi_bind_venc(ot_eis_handle vi_chn_hdl, ot_eis_handle venc_chn_hdl, ot_eis_handle pip_hdl);
ot_s32 sample_comm_vi_un_bind_venc(ot_eis_handle vi_chn_hdl, ot_eis_handle venc_chn_hdl, ot_eis_handle pip_hdl);

ot_s32 sample_comm_vproc_bind_vproc(ot_eis_handle vproc_chn_hdl, ot_eis_handle vproc_pipe_hdl, ot_eis_handle pip_hdl);
ot_s32 sample_comm_vproc_un_bind_vproc(ot_eis_handle vproc_chn_hdl, ot_eis_handle vproc_pipe_hdl, ot_eis_handle pip_hdl);

ot_s32 sample_comm_vproc_bind_vo(ot_eis_handle vproc_chn_hdl, ot_eis_handle vo_in_port_hdl, ot_eis_handle pip_hdl);
ot_s32 sample_comm_vproc_un_bind_vo(ot_eis_handle vproc_chn_hdl, ot_eis_handle vo_in_port_hdl, ot_eis_handle pip_hdl);

ot_s32 sample_comm_vproc_bind_venc(ot_eis_handle vproc_chn_hdl, ot_eis_handle venc_chn_hdl, ot_eis_handle pip_hdl);
ot_s32 sample_comm_vproc_un_bind_venc(ot_eis_handle vproc_chn_hdl, ot_eis_handle venc_chn_hdl, ot_eis_handle pip_hdl);

ot_void sample_comm_vproc_get_default_grp_info(sample_sns_type sns_type, ot_eis_vproc_grp_attr *grp_info);
ot_void sample_comm_vproc_get_default_pipe_info(sample_sns_type sns_type, ot_eis_vproc_pipe_attr *pipe_attr,
    ot_eis_vproc_work_mode mode);
ot_void sample_comm_vproc_get_default_chn_info(sample_sns_type sns_type, ot_eis_vproc_chn_attr *chn_attr,
    ot_eis_vproc_work_mode mode);
ot_s32 sample_media_vproc_get_default_attr_by_snsor(sample_sns_type sns_type, sample_vproc_attr* vproc_attr);
ot_void sample_comm_vi_get_default_vi_cfg_by_dev_id_and_sns_num(
    sample_sns_type sns_type, sample_vi_cfg *vi_cfg, ot_s32 dev_id, ot_u8 sns_nums);
ot_s32 sample_comm_vproc_load_gdc_2dlut_file(ot_s8 *name, ot_eis_vproc_gdc_param *gdc_param,
    ot_eis_vproc_xylut_param *xylut);


ot_void sample_comm_vi_get_size_by_sns_type(sample_sns_type sns_type, ot_eis_img_size *size);
ot_void sample_comm_vi_get_default_vi_cfg(sample_sns_type sns_type, sample_vi_cfg *vi_cfg);
ot_void sample_comm_vi_get_default_vi_cfg_by_dev_id(sample_sns_type sns_type, sample_vi_cfg *vi_cfg, ot_s32 dev_id);

ot_s32 sample_vi_sys_init(ot_eis_img_size *img_size, ot_eis_vi_vproc_mode_type mode_type);

ot_s32 sample_comm_vi_start_sensor_mipirx(sample_vi_cfg *vi_cfg);
ot_void sample_comm_vi_stop_sensor_mipirx(const sample_vi_cfg *vi_cfg);

ot_s32 sample_comm_vi_start_chn_dev(sample_vi_cfg *vi_cfg);
ot_void sample_comm_vi_stop_chn_dev(const sample_vi_cfg *vi_cfg);

ot_s32 sample_comm_vi_start_vi(sample_vi_cfg *vi_cfg);
ot_void sample_comm_vi_stop_vi(const sample_vi_cfg *vi_cfg);

ot_s32 sample_comm_vi_restart(sample_vi_cfg *vi_cfg);
ot_void sample_comm_vi_part_stop(sample_vi_cfg *vi_cfg);

ot_s32 sample_comm_vi_start_route(sample_vi_cfg vi_cfg[], sample_vproc_attr vproc_attr[], ot_u32 route_num,
                                  ot_eis_vi_vproc_mode_type mode_type, ot_u32 display_num);
ot_void sample_comm_vi_stop_route(sample_vi_cfg vi_cfg[], sample_vproc_attr vproc_attr[], ot_s32 route_num, ot_u32 display_num);

ot_s32 sample_comm_vi_enable_fpn_correction_for_scene(ot_vi_pipe_handle pipe_hdl, sample_vi_fpn_correction_cfg *correction_cfg,
    ot_u32 iso, sample_scene_fpn_offset_cfg *scene_fpn_offset_cfg, const ot_char *dir_name);
ot_s32 sample_comm_vi_disable_fpn_correction(ot_vi_pipe_handle pipe_hdl,
                                             sample_vi_fpn_correction_cfg *correction_cfg);

ot_s32 sample_comm_venc_start(sample_venc_attr *venc_attr);
ot_s32 sample_comm_venc_stop(sample_venc_attr *attr);

ot_void sample_comm_vdec_start_send_stream(vdec_thread_param *vdec_send, pthread_t *vdec_thread);

ot_s32 sample_comm_vdec_start(sample_vdec_attr *vdec_attr);
ot_s32 sample_comm_vdec_stop(ot_eis_vdec_chn_handle vdec_hdl);

ot_s32 sample_comm_start_vo(sample_vo_attr *vo_attr);
ot_s32 sample_comm_stop_vo(sample_vo_attr *vo_attr);
ot_s32 sample_comm_vio_start_route(sample_vi_cfg *vi_cfg, sample_vo_attr *vo_attr);
ot_void sample_comm_vio_stop_route(sample_vi_cfg *vi_cfg, sample_vo_attr *vo_attr);

sample_vo_attr *sample_comm_get_vo_attr(sample_vo_display_mode display_mode);
ot_void sample_common_save_frame(sample_vo_attr *vo_attr);
ot_void sample_common_save_in_port_frame(sample_vo_attr *vo_attr, ot_u32 in_port_index);

ot_s32 sample_comm_start_vproc(sample_vproc_attr* vproc_attr, ot_bool pipe_sw[], ot_bool chn_sw[]);
ot_s32 sample_comm_stop_vproc(sample_vproc_attr* vproc_attr);
ot_s32 sample_comm_vi_get_vproc_send_frame(sample_vproc_attr* vproc_attr,
                                          ot_bool pipe_sw[], ot_bool chn_sw[],
                                          ot_eis_handle vi_chn_hdl, ot_eis_handle vo_in_port_hdl);

ot_s32 sample_comm_media_create_buf_pool(ot_eis_handle* pool_hdl, ot_eis_img_attr* img_attr, ot_u32 cnt);

ot_s32 sample_comm_start_mipi_tx(sample_vo_attr *vo_attr);
ot_void sample_comm_stop_mipi_tx(sample_vo_attr *vo_attr);

ot_s32 sample_comm_start_pwm(sample_pwm_id pwm_id, ot_u32 duty_cycle);
ot_s32 sample_comm_stop_pwm(sample_pwm_id pwm_id);

ot_s32 sample_comm_gpio_output_ctrl(ot_u8 gpio_grp, ot_u8 gpio_pin, ot_bool pull_high);

ot_void sample_common_draw_cornerbox(sample_vo_attr *vo_attr);

int sample_comm_oge_draw_line();

ot_s32 sample_comm_vo_set_fps(ot_float fps);
ot_float sample_comm_vo_get_fps(ot_void);

sample_sns_type sample_comm_args_get_sns_type(ot_s32 idx);
sample_sns_num sample_comm_args_get_sns_num(ot_void);
sample_vo_display_dev_type sample_comm_args_get_vo_display_type(ot_void);
ot_void sample_comm_args_set_sample_index(ot_s32 sample_index);
sample_param_comb *sample_comm_args_get_param_comb(ot_void);
ot_s32 sample_comm_args_register(ot_s32 argc, ot_char *argv[]);
ot_void sample_comm_args_help(ot_void);
sample_vo_attr *sample_comm_args_get_vo_attr(ot_void);
ot_void sample_comm_vi_args_help(ot_void);
ot_void sample_comm_vo_args_help(ot_void);

ot_void sample_comm_lightsensor_start(sample_light_sensor *light_sensor_attr);
ot_void sample_comm_lightsensor_stop(sample_light_sensor *light_sensor_attr);
ot_u16 sample_comm_lightsensor_get_lux(sample_light_sensor_id port_id);

ot_s32 sample_comm_parse_alg_param(const char * const filename, ot_eis_vi_sns_alg_default *alg, ot_vi_pipe_id vi_pipe);
ot_s32 sample_comm_parse_awb_param(const char * const filename,ot_eis_vi_awb_sensor_default *sns_awb);
ot_s32 sample_comm_parse_sensor_table_param(const char * const filename, ot_eis_sns_reg_table  *sns_table,
    ot_vi_pipe_id vi_pipe);
ot_void sample_comm_vi_json_sensor_table_free(ot_vi_pipe_id vi_pipe);
ot_s32 sample_comm_get_tuning_file(ot_vi_pipe_id vi_pipe, char *alg_file, char *awb_file, char *table_file);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef SAMPLE_COMMON_H */
