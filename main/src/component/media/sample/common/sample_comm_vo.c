/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>


#include "sample_comm.h"
#include "ot_oge.h"
#include "ot_gfb.h"
#include "ot_smr.h"
#include "load_bmp.h"


#define LT_IOC_MAGIC   's'
#define LT_CMD_SETMODE   _IOW(LT_IOC_MAGIC, 0x01, unsigned int)
#define LT_CMD_SETCLK    _IOW(LT_IOC_MAGIC, 0x02, unsigned int)  /* set clk firstly */
#define OT_INVALID_VALUE       (-1)

#define ARGB8888_WHITE  0xFFFFFFFF
#define ARGB8888_RED    0xFFFF0000
#define ARGB8888_BLACK  0x00000000
#define ARGB8888_GREEN  0xFF00FF00
#define ARGB8888_BLUE   0xFF0000FF
#define ARGB8888_YELLOW 0xFF00FFFF
#define SAMPLE_BMP_PATH        "./source_file/humen_red.bmp"

#define TI941_I2C_ADDR    0x18
#define TI948_I2C_ADDR    0x58
#define OPT3001_I2C_ADDR  0x88

#define I2C_DST_ADDR(src_addr, idx)     \
    ((src_addr) < 0xF0) ? (src_addr + 2 * (idx + 1)) : (src_addr - 2 * (idx + 1))


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VO_MAX_FRM_WIDTH 8192
static FILE *g_vo_pfd = OT_NULL;
static char *g_v_buf_virt_y = OT_NULL;
static char *g_v_buf_virt_c = OT_NULL;
static ot_u32 g_ysize, g_csize;

static struct fb_bitfield g_a32 = {24, 8, 0};
static struct fb_bitfield g_r32 = {16, 8, 0};
static struct fb_bitfield g_g32 = {8,  8, 0};
static struct fb_bitfield g_b32 = {0,  8, 0};

#define VO_FPS_DEF   50
#define VO_FPS_MAX   60
#define VO_FPS_MIN   30
static ot_float g_vo_fps = VO_FPS_DEF;

static sample_vo_attr g_vo_tm070jdhp08_1280x768_attr  = {
    // in port param
    .in_port_enable[0] = OT_TRUE,
    .in_port_enable[1] = OT_FALSE,
    .in_port_attr[0] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 1280,
            .height = 768,
        },
    },
    .surface_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .show = OT_TRUE,
    },
    // surface param
    .surface_enable[0] = OT_TRUE,
    .surface[0] = OT_EIS_VO_VIDEO_SURFACE_0,
    .surface_attr[0] = {
        .width = 1280,
        .height = 768,
        .buf_len = 3,
    },
    .display_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .order = 0,
    },
    // display param
    .display[0] = OT_EIS_VO_DISPLAY_0,
    .display_attr[0] = {
        .vtth = 0,
        .interface = OT_EIS_VO_INTF_MIPI_0,
        .size = {
            .type = OT_EIS_VO_DISPLAY_CUSTOMIZED,
            .frame_rate = OT_VO_OUTPUT_FRAME_RATE_60,
        },
        .sync_info = {
            .syncm          = 0,
            .iop            = 1,
            .intfb          = 1,

            .vact           = 768,
            .vbb            = 27,
            .vfb            = 3,

            .hact           = 1280,
            .hbb            = 320,
            .hfb            = 64,
            .hmid           = 1,

            .bvact          = 0,
            .bvbb           = 0,
            .bvfb           = 0,

            .hpw            = 128,
            .vpw            = 7,

            .idv            = 0,
            .ihs            = 0,
            .ivs            = 0,
        },
    },
};

static sample_vo_attr g_vo_jc07027001_800x1280_attr  = {
    // in port param
    .in_port_enable[0] = OT_TRUE,
    .in_port_enable[1] = OT_FALSE,
    .in_port_attr[0] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 800,
            .height = 1280,
        },
    },
    .surface_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .show = OT_TRUE,
    },
    // surface param
    .surface_enable[0] = OT_TRUE,
    .surface[0] = OT_EIS_VO_VIDEO_SURFACE_0,
    .surface_attr[0] = {
        .width = 800,
        .height = 1280,
        .buf_len = 3,
    },
    .display_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .order = 0,
    },
    // display param
    .display[0] = OT_EIS_VO_DISPLAY_0,
    .display_attr[0] = {
        .vtth = 0,
        .interface = OT_EIS_VO_INTF_MIPI_0,
        .size = {
            .type = OT_EIS_VO_DISPLAY_CUSTOMIZED,
            .frame_rate = OT_VO_OUTPUT_FRAME_RATE_60,
        },
        .sync_info = {
            .syncm          = 0,
            .iop            = 1,
            .intfb          = 1,

            .vact           = 1280,
            .vbb            = 14,
            .vfb            = 15,

            .hact           = 800,
            .hbb            = 40,
            .hfb            = 80,
            .hmid           = 0,

            .bvact          = 0,
            .bvbb           = 0,
            .bvfb           = 0,

            .hpw            = 20,
            .vpw            = 6,

            .idv            = 0,
            .ihs            = 0,
            .ivs            = 0,
        },
    },
};

static sample_vo_attr g_vo_2_port_tm070jdhp08_1280x768_attr = {
    // in port param
    .in_port_enable[0] = OT_TRUE,
    .in_port_enable[1] = OT_TRUE,
    .in_port_attr[0] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 960,
            .height = 768,
        },
    },
    .surface_bind_attr[0] = {
        .priority = 0,
        .x = 0,
        .y = 0,
        .show = OT_TRUE,
    },
    .in_port_attr[1] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 320,
            .height = 768,
        },
    },
    .surface_bind_attr[1] = {
        .x = 960,
        .y = 0,
        .show = OT_TRUE,
    },
    // surface param
    .surface_enable[0] = OT_TRUE,
    .surface[0] = OT_EIS_VO_VIDEO_SURFACE_0,
    .surface_attr[0] = {
        .width = 1280,
        .height = 768,
        .buf_len = 3,
    },
    .display_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .order = 0,
    },
    // display param
    .display[0] = OT_EIS_VO_DISPLAY_0,
    .display_attr[0] = {
        .vtth = 0,
        .interface = OT_EIS_VO_INTF_MIPI_0,
        .size = {
            .type = OT_EIS_VO_DISPLAY_CUSTOMIZED,
            .frame_rate = OT_VO_OUTPUT_FRAME_RATE_60,
        },
        .sync_info = {
            .syncm          = 0,
            .iop            = 1,
            .intfb          = 1,

            .vact           = 768,
            .vbb            = 27,
            .vfb            = 3,

            .hact           = 1280,
            .hbb            = 320,
            .hfb            = 64,
            .hmid           = 1,

            .bvact          = 0,
            .bvbb           = 0,
            .bvfb           = 0,

            .hpw            = 128,
            .vpw            = 7,

            .idv            = 0,
            .ihs            = 0,
            .ivs            = 0,
        },
    },
};

static sample_vo_attr g_sample_vo_2_port_mipi_tx_2560p_attr = {
    // in port param
    .in_port_enable[0] = OT_TRUE,
    .in_port_enable[1] = OT_TRUE,
    .in_port_attr[0] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 1280,
            .height = 768,
        },
    },
    .surface_bind_attr[0] = {
        .priority = 0,
        .x = 0,
        .y = 0,
        .show = OT_TRUE,
    },
    .in_port_attr[1] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 1280,
            .height = 768,
        },
    },
    .surface_bind_attr[1] = {
        .x = 1280,
        .y = 0,
        .show = OT_TRUE,
    },
    // surface param
    .surface_enable[0] = OT_TRUE,
    .surface[0] = OT_EIS_VO_VIDEO_SURFACE_0,
    .surface_attr[0] = {
        .width = 2560,
        .height = 768,
        .buf_len = 3,
        .early_disp = OT_TRUE,
    },
    .display_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .order = 0,
    },
    // display param
    .display[0] = OT_EIS_VO_DISPLAY_0,
    .display_attr[0] = {
        .vtth = 0,
        .interface = OT_EIS_VO_INTF_MIPI_0,
        .size = {
            .type = OT_EIS_VO_DISPLAY_CUSTOMIZED,
            .frame_rate = OT_VO_OUTPUT_FRAME_RATE_60,
        },
        .sync_info = {
            .syncm          = 0,
            .iop            = 1,
            .intfb          = 1,

            .vact           = 768,
            .vbb            = 27,
            .vfb            = 3,

            .hact           = 2560,
            .hbb            = 640,
            .hfb            = 128,
            .hmid           = 1,

            .bvact          = 0,
            .bvbb           = 0,
            .bvfb           = 0,

            .hpw            = 256,
            .vpw            = 7,

            .idv            = 0,
            .ihs            = 0,
            .ivs            = 0,
        },
    },
};

static sample_vo_attr g_vo_av069hdt_1280x720p60_attr  = {
    // in port param
    .in_port_enable[0] = OT_TRUE,
    .in_port_enable[1] = OT_FALSE,
    .in_port_attr[0] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 1280,
            .height = 720,
        },
    },
    .surface_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .show = OT_TRUE,
    },
    // surface param
    .surface_enable[0] = OT_TRUE,
    .surface[0] = OT_EIS_VO_VIDEO_SURFACE_0,
    .surface_attr[0] = {
        .width = 1280,
        .height = 720,
        .buf_len = 3,
    },
    .display_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .order = 0,
    },
    // display param
    .display[0] = OT_EIS_VO_DISPLAY_0,
    .display_attr[0] = {
        .vtth = 0,
        .interface = OT_EIS_VO_INTF_MIPI_0,
        .size = {
            .type = OT_EIS_VO_DISPLAY_CUSTOMIZED,
            .frame_rate = OT_VO_OUTPUT_FRAME_RATE_60,
        },
        .sync_info = {
            .syncm          = 0,
            .iop            = 1,
            .intfb          = 1,

            .vact           = 720,
            .vbb            = 6,
            .vfb            = 12,

            .hact           = 1280,
            .hbb            = 10,
            .hfb            = 134,
            .hmid           = 0,

            .bvact          = 0,
            .bvbb           = 0,
            .bvfb           = 0,

            .hpw            = 2,
            .vpw            = 2,

            .idv            = 0,
            .ihs            = 0,
            .ivs            = 0,
        },
    },
};

static sample_vo_attr g_vo_av069hdt_960x540p60_attr  = {
    // in port param
    .in_port_enable[0] = OT_TRUE,
    .in_port_attr[0] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 1280,
            .height = 720,
        },
    },
    .surface_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .show = OT_TRUE,
    },
    // surface param
    .surface_enable[0] = OT_TRUE,
    .surface[0] = OT_EIS_VO_VIDEO_SURFACE_0,
    .surface_attr[0] = {
        .width = 1280,
        .height = 720,
        .buf_len = 3,
    },
    .display_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .order = 0,
    },
    // display param
    .display[0] = OT_EIS_VO_DISPLAY_0,
    .display_attr[0] = {
        .bg_color = 16711680, //red:0xff0000
        .vtth = 0,
        .interface = OT_EIS_VO_INTF_MIPI_0,
        .size = {
            .type = OT_EIS_VO_DISPLAY_CUSTOMIZED,
            .frame_rate = OT_VO_OUTPUT_FRAME_RATE_60,
        },
        .sync_info = {
            .syncm          = 0,
            .iop            = 1,
            .intfb          = 1,

            .vact           = 720,
            .vbb            = 6,
            .vfb            = 12,

            .hact           = 1280,
            .hbb            = 10,
            .hfb            = 134,
            .hmid           = 0,

            .bvact          = 0,
            .bvbb           = 0,
            .bvfb           = 0,

            .hpw            = 2,
            .vpw            = 2,

            .idv            = 0,
            .ihs            = 0,
            .ivs            = 0,
        },
    },
};

static sample_vo_attr g_vo_av069hdt_2560x720p60_attr  = {
    // in port param
    .in_port_enable[0] = OT_TRUE,
    .in_port_enable[1] = OT_TRUE,
    .in_port_attr[0] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 1280,
            .height = 720,
        },
    },
    .surface_bind_attr[0] = {
        .priority = 0,
        .x = 0,
        .y = 0,
        .show = OT_TRUE,
    },
    .in_port_attr[1] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 1280,
            .height = 720,
        },
    },
    .surface_bind_attr[1] = {
        .x = 1280,
        .y = 0,
        .show = OT_TRUE,
    },
    // surface param
    .surface_enable[0] = OT_TRUE,
    .surface[0] = OT_EIS_VO_VIDEO_SURFACE_0,
    .surface_attr[0] = {
        .width = 2560,
        .height = 720,
        .buf_len = 3,
        .early_disp = OT_TRUE,
    },
    .display_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .order = 0,
    },
    // display param
    .display[0] = OT_EIS_VO_DISPLAY_0,
    .display_attr[0] = {
        .vtth = 0,
        .interface = OT_EIS_VO_INTF_MIPI_0,
        .size = {
            .type = OT_EIS_VO_DISPLAY_CUSTOMIZED,
            .frame_rate = OT_VO_OUTPUT_FRAME_RATE_60,
        },
        .sync_info = {
            .syncm          = 0,
            .iop            = 1,
            .intfb          = 1,

            .vact           = 720,
            .vbb            = 6,
            .vfb            = 12,

            .hact           = 2560,
            .hbb            = 10,
            .hfb            = 134,
            .hmid           = 0,

            .bvact          = 0,
            .bvbb           = 0,
            .bvfb           = 0,

            .hpw            = 2,
            .vpw            = 2,

            .idv            = 0,
            .ihs            = 0,
            .ivs            = 0,
        },
    },
};

static sample_vo_attr g_vo_bt1120_1280x720p30_attr = {
    // in port param
    .in_port_enable[0] = OT_TRUE,
    .in_port_enable[1] = OT_FALSE,
    .in_port_attr[0] = {
        .pixel_attr=  {
            .format = OT_EIS_IMAGE_FORMAT_YUV_422_SEMIPLANAR,
            .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
            .width = 1280,
            .height = 720,
        },
    },
    .surface_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .show = OT_TRUE,
    },
    // surface param
    .surface_enable[0] = OT_TRUE,
    .surface[0] = OT_EIS_VO_VIDEO_SURFACE_0,
    .surface_attr[0] = {
        .width = 1280,
        .height = 720,
        .buf_len = 3,
    },
    .display_bind_attr[0] = {
        .x = 0,
        .y = 0,
        .order = 0,
    },
    // display param
    .display[0] = OT_EIS_VO_DISPLAY_0,
    .display_attr[0] = {
        .vtth = 0,
        .interface = OT_EIS_VO_INTF_BT1120,
        .size = {
            .type = OT_EIS_VO_DISPLAY_CUSTOMIZED,
            .frame_rate = OT_VO_OUTPUT_FRAME_RATE_30,
        },
        .sync_info = {
            .syncm          = 0,
            .iop            = 1,
            .intfb          = 1,
            .vact           = 720,
            .vbb            = 20,
            .vfb            = 10,
            .hact           = 1280,
            .hbb            = 220,
            .hfb            = 100,
            .hmid           = 1,
            .bvact          = 1,
            .bvbb           = 1,
            .bvfb           = 1,
            .hpw            = 40,
            .vpw            = 5,
            .idv            = 0,
            .ihs            = 0,
            .ivs            = 0,
        },
    },
};

static ot_void sample_comm_vo_default_serdes_attr(ot_eis_vo_serdes_attr *serdes_attr)
{
    serdes_attr->bus_info.bus_id = -1;
    serdes_attr->bus_info.bus_type = OT_VO_SERDES_BUS_TYPE_I2C;
    serdes_attr->clk_mode = OT_VO_SERDES_CLK_MODE_CONTINUE;
    serdes_attr->fpdlink_mode = OT_VO_SERDES_FPDLINK_MODE_SINGLE;
    serdes_attr->video_mode = OT_VO_SERDES_BURST;
    serdes_attr->data_fmt = OT_VO_SERDES_DATA_FMT_RGB_24BIT;
    serdes_attr->priv_data = OT_NULL;
    serdes_attr->serial_addr = TI941_I2C_ADDR;

}

sample_vo_attr *sample_comm_get_vo_attr(sample_vo_display_mode display_mode)
{
    ot_eis_vo_serdes_attr serdes_attr = {0};
    sample_vo_attr *vo_attr = OT_NULL;
    sample_comm_vo_default_serdes_attr(&serdes_attr);
    switch (display_mode) {
        case ONE_IN_PORT_MIPI_768:
            vo_attr = &g_vo_tm070jdhp08_1280x768_attr;
            break;
        case ONE_IN_PORT_MIPI_AV069HDT_1280X720:
            vo_attr = &g_vo_av069hdt_1280x720p60_attr;
            serdes_attr.video_mode = OT_VO_SERDES_NON_BURST_SYNC_PULSES;
            break;
        case ONE_IN_PORT_MIPI_AV069HDT_960X540:
            vo_attr = &g_vo_av069hdt_960x540p60_attr;
            serdes_attr.video_mode = OT_VO_SERDES_NON_BURST_SYNC_PULSES;
            break;
        case ONE_IN_PORT_BT1120_1280X720:
            vo_attr = &g_vo_bt1120_1280x720p30_attr;
            break;
        case TWO_IN_PORT_MIPI_768:
            vo_attr = &g_vo_2_port_tm070jdhp08_1280x768_attr;
            break;
        case ONE_IN_PORT_MIPI_800X1280:
            vo_attr = &g_vo_jc07027001_800x1280_attr;
            serdes_attr.bus_info.bus_id = -1;
            break;
        case TWO_IN_PORT_MIPI_AV069HDT_2560X720:
            vo_attr = &g_vo_av069hdt_2560x720p60_attr;
            serdes_attr.fpdlink_mode = OT_VO_SERDES_FPDLINK_MODE_SEPARATE;
            serdes_attr.video_mode = OT_VO_SERDES_NON_BURST_SYNC_PULSES;
            break;
        case TWO_IN_PORT_MIPI_2560X768:
            vo_attr = &g_sample_vo_2_port_mipi_tx_2560p_attr;
            serdes_attr.fpdlink_mode = OT_VO_SERDES_FPDLINK_MODE_SEPARATE;
            break;
        default:
            vo_attr = &g_vo_tm070jdhp08_1280x768_attr ;
            break;
    }
    if (serdes_attr.fpdlink_mode == OT_VO_SERDES_FPDLINK_MODE_SINGLE) {
        serdes_attr.one_port_attr.deserial_addr = TI948_I2C_ADDR;

        vo_attr->light_sensor_attr.light_sensor_num = 1;
        vo_attr->light_sensor_attr.port_attr[0].src_addr = OPT3001_I2C_ADDR;
        vo_attr->light_sensor_attr.port_attr[0].dst_addr = OPT3001_I2C_ADDR;
    } else {
        serdes_attr.multi_port_attr[0].serdes_port = OT_VO_SERDES_PORT_0;
        serdes_attr.multi_port_attr[0].deserial_src_addr = TI948_I2C_ADDR;
        serdes_attr.multi_port_attr[0].deserial_dst_addr = I2C_DST_ADDR(TI948_I2C_ADDR, 0);

        serdes_attr.multi_port_attr[1].serdes_port = OT_VO_SERDES_PORT_1;
        serdes_attr.multi_port_attr[1].deserial_src_addr = TI948_I2C_ADDR;
        serdes_attr.multi_port_attr[1].deserial_dst_addr = I2C_DST_ADDR(TI948_I2C_ADDR, 1);

        vo_attr->light_sensor_attr.light_sensor_num = 2;

        vo_attr->light_sensor_attr.port_attr[0].src_addr = OPT3001_I2C_ADDR;
        vo_attr->light_sensor_attr.port_attr[0].dst_addr = I2C_DST_ADDR(OPT3001_I2C_ADDR, 0);

        vo_attr->light_sensor_attr.port_attr[1].src_addr = OPT3001_I2C_ADDR;
        vo_attr->light_sensor_attr.port_attr[1].dst_addr = I2C_DST_ADDR(OPT3001_I2C_ADDR, 1);
    }
    vo_attr->display_mode = display_mode;
    vo_attr->light_sensor_attr.is_used = OT_FALSE;
    serdes_attr.priv_data = &vo_attr->light_sensor_attr;
    memcpy(&(vo_attr->serdes_info.serdes_attr), &serdes_attr, sizeof(ot_eis_vo_serdes_attr));
    return vo_attr;
}

static ot_s32 sample_comm_vo_start_serdes(sample_vo_serdes_info *serdes_info)
{
    ot_s32 ret = OT_SUCCESS;

    ret = ot_eis_vo_serdes_init();
    if (ret != OT_SUCCESS) {
        sample_print("vo serdes init failed.\n");
        return ret;
    }

    ret = ot_eis_vo_serdes_create(&(serdes_info->serdes_hdl), "TI941,TI948", &(serdes_info->serdes_attr));
    if (ret != OT_SUCCESS) {
        ot_eis_vo_serdes_deinit();
        sample_print("vo serdes create failed.\n");
        return ret;
    }

    ret = ot_eis_vo_serdes_start(serdes_info->serdes_hdl);
    if (ret != OT_SUCCESS) {
        ot_eis_vo_serdes_destroy(serdes_info->serdes_hdl);
        ot_eis_vo_serdes_deinit();
        sample_print("vo serdes start failed.\n");
        return ret;
    }

    return ret;
}

static ot_s32 sample_comm_vo_stop_serdes(const sample_vo_serdes_info *serdes_info)
{
    ot_s32 ret = OT_SUCCESS;
    ret |= ot_eis_vo_serdes_stop(serdes_info->serdes_hdl);
    ret |= ot_eis_vo_serdes_destroy(serdes_info->serdes_hdl);
    ret |= ot_eis_vo_serdes_deinit();
    return ret;
}

ot_s32 sample_comm_vo_set_fps(ot_float fps)
{
    if (fps < VO_FPS_MIN || fps > VO_FPS_MAX) {
        sample_print("Invalid fps %f, min %u, max %u\n", fps, VO_FPS_MIN, VO_FPS_MAX);
        return OT_FAILURE;
    }
    g_vo_fps = fps;
    return OT_SUCCESS;
}

ot_float sample_comm_vo_get_fps(ot_void)
{
    return g_vo_fps;
}

static ot_void sample_comm_vo_calculate_clock_info(ot_eis_vo_display_attr *display_attr)
{
    ot_u32 pixel_clk, h_total, v_total;
    ot_float fps;
    if (display_attr->size.type != OT_EIS_VO_DISPLAY_CUSTOMIZED) {
        return;
    }

    if (display_attr->interface == OT_EIS_VO_INTF_BT1120) {
        sample_comm_vo_set_fps(VO_FPS_MIN);
    }

    h_total = display_attr->sync_info.hact + display_attr->sync_info.hbb + display_attr->sync_info.hfb;
    v_total = display_attr->sync_info.vact + display_attr->sync_info.vbb + display_attr->sync_info.vfb;
    fps = sample_comm_vo_get_fps();

    pixel_clk = (h_total * v_total * fps + 300000) / 1000 * 1000;

    printf("h_total[%d], v_total[%d], pixel clock[%d] fps[%.1f]\n", h_total, v_total, pixel_clk, fps);

    display_attr->clk_info.op_mode = OT_EIS_OP_MODE_AUTO;
    display_attr->clk_info.auto_user_sync_info.pixel_clk = pixel_clk;
}

ot_s32 sample_comm_start_vo(sample_vo_attr *vo_attr)
{
    ot_u32 i;
    // init vo
    ot_eis_vo_init();
    // create in_port 1
    // in_port_attr.port_type = vo_attr->in_port_attr.port_type;
    for (i = 0; i < OT_VO_MAX_BIND_PORT_NUM; i++)
    {
        if (vo_attr->in_port_enable[i] == OT_TRUE)
        {
            ot_eis_vo_in_port_create(&vo_attr->in_port_handle[i], &vo_attr->in_port_attr[i]);
        }
    }

    // create surface
    for (i = 0; i < OT_VO_MAX_BIND_SURF_NUM; i++)
    {
        if (vo_attr->surface_enable[i] == OT_TRUE)
        {
            ot_eis_vo_surface_create(vo_attr->surface[i], &vo_attr->surface_handle[i], &vo_attr->surface_attr[i]);
        }
    }

    // bind in_port
    for (i = 0; i < OT_VO_MAX_BIND_PORT_NUM; i++)
    {
        if (vo_attr->in_port_enable[i] == OT_TRUE) {
            ot_eis_vo_surface_bind(vo_attr->surface_handle[0],
            vo_attr->in_port_handle[i], &vo_attr->surface_bind_attr[i]);
        }
    }

    /* surface crop */
    if (vo_attr->crop_attr[0].enable == OT_TRUE)
    {
        printf("crop: enable[%d], x[%d], y[%d], w[%d], h[%d]\n", vo_attr->crop_attr[0].enable, vo_attr->crop_attr[0].rect.x,
        vo_attr->crop_attr[0].rect.y,
        vo_attr->crop_attr[0].rect.w,
        vo_attr->crop_attr[0].rect.h);
        ot_eis_vo_surface_set_crop(vo_attr->surface_handle[0], &vo_attr->crop_attr[0]);
    }

    // create display
    sample_comm_vo_calculate_clock_info(&vo_attr->display_attr[0]);
    // vo_attr->display_attr[0].clk_info.auto_user_sync_info.pixel_clk = 72250 * 1000;
    ot_eis_vo_display_create(vo_attr->display[0], &vo_attr->display_handle[0], &vo_attr->display_attr[0]);

    // bind surface
    ot_eis_vo_display_bind(vo_attr->display_handle[0], vo_attr->surface_handle[0], &vo_attr->display_bind_attr[0]);

    if (vo_attr->display_attr[0].interface == OT_EIS_VO_INTF_MIPI_0) {
        /* set defalut csc to BT709 FULL */
        ot_eis_vo_mipi_param param;
        param.csc.contrast = 50;
        param.csc.ex_csc_en = 0;
        param.csc.hue = 50;
        param.csc.luma = 50;
        param.csc.saturation = 50;
        param.csc.csc_matrix = OT_EIS_VO_CSC_MATRIX_BT709FULL_TO_RGBFULL;
        param.gamma_info.gamma_enable = OT_FALSE;
        ot_eis_vo_display_set_mipi_param(vo_attr->display_handle[0], &param);
    } else if(vo_attr->display_attr[0].interface == OT_EIS_VO_INTF_BT1120) {
        ot_eis_vo_bt_param bt_param;
        bt_param.yc_inverted_en = 0;
        bt_param.bit_inverted_en = 0;
        bt_param.clk_edge = OT_EIS_VO_CLK_EDGE_SINGLE;
        ot_eis_vo_display_set_bt_param(vo_attr->display_handle[0], &bt_param);
    }
    // display start
    ot_eis_vo_display_start(vo_attr->display_handle[0]);

    if (vo_attr->display_attr[0].interface == OT_EIS_VO_INTF_MIPI_0) {
        sample_comm_start_mipi_tx(vo_attr);
    }

    if ((vo_attr->display_mode != ONE_IN_PORT_MIPI_800X1280) && (vo_attr->display_attr[0].interface != OT_EIS_VO_INTF_BT1120)) {
        sample_comm_vo_start_serdes(&vo_attr->serdes_info);
        usleep(10 * 1000); // for serdes stable
        sample_comm_lightsensor_start(&vo_attr->light_sensor_attr);
    }
    usleep(10000); // Turn on the backlight after the serdes stabilizes
    if (vo_attr->serdes_info.serdes_attr.fpdlink_mode == OT_VO_SERDES_FPDLINK_MODE_SINGLE) {
        sample_comm_start_pwm(PWM_ID_0, 8000); // duty cycle 8000
    } else {
        sample_comm_start_pwm(PWM_ID_0, 8000); // duty cycle 8000
        sample_comm_start_pwm(PWM_ID_1, 8000); // duty cycle 8000
    }
    return OT_SUCCESS;
}

ot_s32 sample_comm_stop_vo(sample_vo_attr *vo_attr)
{
    ot_u32 i;

    if (vo_attr->serdes_info.serdes_attr.fpdlink_mode == OT_VO_SERDES_FPDLINK_MODE_SINGLE) {
        sample_comm_stop_pwm(PWM_ID_0);
    } else {
        sample_comm_stop_pwm(PWM_ID_0);
        sample_comm_stop_pwm(PWM_ID_1);
    }

    //reset serdes
    if ((vo_attr->display_mode != ONE_IN_PORT_MIPI_800X1280) && (vo_attr->display_attr[0].interface != OT_EIS_VO_INTF_BT1120)) {
        sample_comm_lightsensor_stop(&vo_attr->light_sensor_attr);
        sample_comm_vo_stop_serdes(&vo_attr->serdes_info);
    }
    // display stop
    ot_eis_vo_display_stop(vo_attr->display_handle[0]);
    // unbind surface and in port
    for (i = 0; i < OT_VO_MAX_BIND_PORT_NUM; i++)
    {
        if (vo_attr->in_port_enable[i] == OT_TRUE)
        {
            ot_eis_vo_surface_unbind(vo_attr->surface_handle[0], vo_attr->in_port_handle[i]);
            // in port destroy
            ot_eis_vo_in_port_destroy(vo_attr->in_port_handle[i]);
        }
    }
    // surface destroy
    for (i = 0; i < OT_VO_MAX_BIND_SURF_NUM; i++)
    {
        if (vo_attr->surface_enable[i] == OT_TRUE)
        {
            // unbind display and surface
            ot_eis_vo_display_unbind(vo_attr->display_handle[0], vo_attr->surface_handle[i]);
            ot_eis_vo_surface_destroy(vo_attr->surface_handle[i]);
        }
    }
    // display destroy
    ot_eis_vo_display_destroy(vo_attr->display_handle[0]);
    if (vo_attr->display_attr[0].interface == OT_EIS_VO_INTF_MIPI_0) {
        sample_comm_stop_mipi_tx(vo_attr);
    }
    // deinit vo
    ot_eis_vo_deinit();

    return 0;
}

static ot_void sample_get_uv_height_image(const ot_eis_video_frame *v_buf, ot_u32 *uv_height)
{
    ot_eis_img_pixel_format pixel_format = v_buf->attr.pixel_fmt;

    if ((pixel_format == OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR) ||
        (pixel_format == OT_EIS_IMAGE_FORMAT_YUV_420_SEMIPLANAR)) {
        *uv_height = v_buf->attr.height / 2; /* 1 / 2 */
    } else if ((pixel_format == OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR) ||
        (pixel_format == OT_EIS_IMAGE_FORMAT_YUV_422_SEMIPLANAR)) {
        *uv_height = v_buf->attr.height;
    } else {
        *uv_height = 0;
    }
}

static ot_void sample_cal_yc_size_image(const ot_eis_video_frame *v_buf)
{
    ot_eis_img_pixel_format pixel_format = v_buf->attr.pixel_fmt;

    g_ysize = (v_buf->buff.stride[0]) * (v_buf->attr.height);
    if ((pixel_format == OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR) ||
        (pixel_format == OT_EIS_IMAGE_FORMAT_YUV_420_SEMIPLANAR)) {
        g_csize = (v_buf->buff.stride[1]) * (v_buf->attr.height) / 2; /* 1 / 2 */
    } else if ((pixel_format == OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR) ||
        (pixel_format == OT_EIS_IMAGE_FORMAT_YUV_422_SEMIPLANAR)) {
        g_csize = (v_buf->buff.stride[1]) * (v_buf->attr.height);
    } else {
        g_csize = 0;
    }
}

static ot_void sample_save_y_image(const ot_eis_video_frame *v_buf)
{
    unsigned int h;
    char *mem_content = OT_NULL;

    /* save Y */
    (void)fprintf(stderr, "saving......Y......");
    (void)fflush(stderr);
    for (h = 0; h < v_buf->attr.height; h++) {
        mem_content = g_v_buf_virt_y + h * v_buf->buff.stride[0];
        (void)fwrite(mem_content, v_buf->attr.width, 1, g_vo_pfd);
    }
    (void)fflush(g_vo_pfd);
}

static ot_void sample_save_chroma_image(const ot_eis_video_frame *v_buf, unsigned int data_offset)
{
    unsigned int w, h;
    char *mem_content = OT_NULL;
    static unsigned char tmp_buff[VO_MAX_FRM_WIDTH];
    ot_u32 uv_height = 0;

    sample_get_uv_height_image(v_buf, &uv_height);

    (void)fflush(stderr);
    for (h = 0; h < uv_height; h++) {
        mem_content = g_v_buf_virt_c + h * v_buf->buff.stride[1];
        mem_content += data_offset;
        for (w = 0; w < v_buf->attr.width / 2; w++) { /* 1 / 2 */
            tmp_buff[w] = *mem_content;
            mem_content += 2; /* 2 bytes */
        }
        (void)fwrite(tmp_buff, v_buf->attr.width / 2, 1, g_vo_pfd); /* 1 / 2 */
    }
    (void)fflush(g_vo_pfd);
}

ot_void sample_common_save_frame(sample_vo_attr *vo_attr)
{
    ot_eis_video_frame frame = { 0 };
    ot_eis_vo_surface_get_frame(vo_attr->surface_handle, &frame);

    printf("success get vo frame\n");
    printf("w:%d, h:%d, formaet:%d\n",frame.attr.height, frame.attr.width, frame.attr.pixel_fmt);

    ot_phys_addr phys_addr;
    ot_eis_img_pixel_format pixel_format = frame.attr.pixel_fmt;
    char file_name[256] = "";
    sprintf(file_name, "%lld%s", frame.pts, "_get_frame_saved.yuv");
    g_vo_pfd = fopen(file_name, "wb");
    if (g_vo_pfd == OT_NULL) {
        printf("open file failed, errno %d!\n", errno);
    }
    sample_cal_yc_size_image(&frame);

    phys_addr = frame.buff.phys_addr[0];
    ot_smr_mmap(phys_addr, g_ysize, OT_FALSE, (ot_void**)&g_v_buf_virt_y);
    if (g_v_buf_virt_y == OT_NULL) {
        return;
    }

    sample_save_y_image(&frame);

    ot_smr_munmap(g_v_buf_virt_y, g_ysize);
    g_v_buf_virt_y = OT_NULL;
    if (pixel_format == OT_EIS_IMAGE_FORMAT_YUV_400) {
        (void)fprintf(stderr, "done %d!\n", frame.index);
        (void)fflush(stderr);
        return;
    }

    ot_smr_mmap(frame.buff.phys_addr[1], g_csize, OT_FALSE, (ot_void**)&g_v_buf_virt_c);
    if (g_v_buf_virt_c == OT_NULL) {
        return;
    }
    if ((pixel_format == OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR) ||
        (pixel_format == OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR)) {
        /* save U */
        (void)fprintf(stderr, "U......");
        sample_save_chroma_image(&frame, 1);
        /* save V */
        (void)fprintf(stderr, "V......");
        sample_save_chroma_image(&frame, 0);
    } else if ((pixel_format == OT_EIS_IMAGE_FORMAT_YUV_420_SEMIPLANAR) ||
        (pixel_format == OT_EIS_IMAGE_FORMAT_YUV_422_SEMIPLANAR)) {
        /* save U */
        (void)fprintf(stderr, "U......");
        sample_save_chroma_image(&frame, 0);
        /* save V */
        (void)fprintf(stderr, "V......");
        sample_save_chroma_image(&frame, 1);
    }
    ot_smr_munmap(g_v_buf_virt_c, g_csize);
    g_v_buf_virt_c = OT_NULL;
    fclose(g_vo_pfd);
    (void)fprintf(stderr, "done %d!\n", frame.index);
    (void)fflush(stderr);
    ot_eis_vo_surface_release_frame(vo_attr->surface_handle, &frame);
}


ot_void sample_common_save_in_port_frame(sample_vo_attr *vo_attr, ot_u32 in_port_index)
{
    ot_eis_video_frame frame = { 0 };
    ot_eis_vo_in_port_get_frame(vo_attr->in_port_handle[in_port_index], &frame);

    printf("success get vo in port frame\n");
    printf("w:%d, h:%d, formaet:%d\n",frame.attr.height, frame.attr.width, frame.attr.pixel_fmt);

    ot_phys_addr phys_addr;
    ot_eis_img_pixel_format pixel_format = frame.attr.pixel_fmt;
    char file_name[256] = "";
    sprintf(file_name, "%lld %s", frame.pts, "_get_in_port_frame_saved.yuv");
    g_vo_pfd = fopen(file_name, "wb");
    if (g_vo_pfd == OT_NULL) {
        printf("open file failed, errno %d!\n", errno);
    }
    sample_cal_yc_size_image(&frame);

    phys_addr = frame.buff.phys_addr[0];
    ot_smr_mmap(phys_addr, g_ysize, OT_FALSE, (ot_void**)&g_v_buf_virt_y);
    if (g_v_buf_virt_y == OT_NULL) {
        return;
    }

    sample_save_y_image(&frame);

    ot_smr_munmap(g_v_buf_virt_y, g_ysize);
    g_v_buf_virt_y = OT_NULL;
    if (pixel_format == OT_EIS_IMAGE_FORMAT_YUV_400) {
        (void)fprintf(stderr, "done %d!\n", frame.index);
        (void)fflush(stderr);
        return;
    }

    ot_smr_mmap(frame.buff.phys_addr[1], g_csize, OT_FALSE, (ot_void**)&g_v_buf_virt_c);
    if (g_v_buf_virt_c == OT_NULL) {
        return;
    }
    if ((pixel_format == OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR) ||
        (pixel_format == OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR)) {
        /* save U */
        (void)fprintf(stderr, "U......");
        sample_save_chroma_image(&frame, 1);
        /* save V */
        (void)fprintf(stderr, "V......");
        sample_save_chroma_image(&frame, 0);
    } else if ((pixel_format == OT_EIS_IMAGE_FORMAT_YUV_420_SEMIPLANAR) ||
        (pixel_format == OT_EIS_IMAGE_FORMAT_YUV_422_SEMIPLANAR)) {
        /* save U */
        (void)fprintf(stderr, "U......");
        sample_save_chroma_image(&frame, 0);
        /* save V */
        (void)fprintf(stderr, "V......");
        sample_save_chroma_image(&frame, 1);
    }
    ot_smr_munmap(g_v_buf_virt_c, g_csize);
    g_v_buf_virt_c = OT_NULL;
    fclose(g_vo_pfd);
    (void)fprintf(stderr, "done %d!\n", frame.index);
    (void)fflush(stderr);
    ot_eis_vo_in_port_release_frame(vo_attr->in_port_handle[in_port_index], &frame);
}

ot_void sample_common_draw_cornerbox(sample_vo_attr *vo_attr) {

    ot_oge_handle handle;
    ot_u32 i, j, x, y;
    ot_u32 width, height;
    ot_s32 ret;
    ot_s32 fd;
    ot_void *viraddr = NULL;
    ot_void *buf;
    ot_u32 fill_data;

    ot_phys_addr g_phyaddr, buf_phyaddr = 0;
    ot_eis_fb_capability cap = {0};
    ot_eis_fb_colorkey color_key;
    ot_eis_fb_point point;
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    ot_eis_fb_buf canvas_buf;
    ot_smr_alloc_attr smr_alloc_attr = {0};
    ot_eis_fb_layer_info layer_info = {0};

    ot_oge_surface dst_surface = {0};
    ot_oge_surface src_surface = {0};
    ot_oge_rect dst_rect = {0};
    ot_oge_rect src_rect = {0};
    ot_oge_single_src single_src = {0};
    ot_oge_none_src none_src = {0};

    width = vo_attr->surface_attr[1].width;
    height = vo_attr->surface_attr[1].height;

    ret = ot_gfb_open(0, &fd);
    if (ret != OT_SUCCESS) {
        sample_print("ot_gfb_open failed.\n");
        return;
    }
    ret = ot_oge_open();
    if (ret < 0) {
        sample_print("ot_oge_open failed.\n");
        return;
    }

    ret = ot_gfb_get_capability(fd, &cap);
    if (ret != OT_SUCCESS) {
        sample_print("ot_gfb_get_capability failed.\n");
        return;
    }

    printf("print gfb capability message: \n");
    printf("cap.is_key_rgb: %d\n", cap.is_key_rgb);
    printf("cap.is_key_alpha: %d\n", cap.is_key_alpha);
    printf("cap.is_global_alpha: %d\n", cap.is_global_alpha);
    printf("cap.is_cmap: %d\n", cap.is_cmap);
    printf("cap.has_cmap_reg: %d\n", cap.has_cmap_reg);
    printf("cap.is_vo_scale: %d\n", cap.is_vo_scale);
    printf("cap.is_layer_support: %d\n", cap.is_layer_support);
    printf("cap.max_width: %d\n", cap.max_width);
    printf("cap.max_height: %d\n", cap.max_height);
    printf("cap.min_width: %d\n", cap.min_width);
    printf("cap.min_height: %d\n", cap.min_height);
    printf("cap.ver_deflicker_level: %d\n", cap.ver_deflicker_level);
    printf("cap.hor_deflicker_level: %d\n", cap.hor_deflicker_level);
    printf("cap.is_ghdr: %d\n", cap.is_ghdr);
    printf("cap.is_osb: %d\n", cap.is_osb);
    ret = ot_gfb_get_var_screen_attr(fd, &var);
    var.transp = g_a32;
    var.red    = g_r32;
    var.green  = g_g32;
    var.blue   = g_b32;
    var.bits_per_pixel = 32; /* 4 bits per pixel */
    var.xres_virtual = width;
    var.yres_virtual = height; /* double mode res_virtual = xres */
    var.xres = width;
    var.yres = height;
    var.activate = 0;
    var.xoffset = 0;
    var.yoffset = 0;
    ret = ot_gfb_set_var_screen_attr(fd, &var);
    ret = ot_gfb_get_var_screen_attr(fd, &var);
    printf("print gfb var message: \n");
    printf("gfb.xres: %d\n", var.xres);
    printf("gfb.yres: %d\n", var.yres);
    printf("gfb.xres_virtual: %d\n", var.xres_virtual);
    printf("gfb.yres_virtual: %d\n", var.yres_virtual);
    printf("gfb.xoffset: %d\n", var.xoffset);
    printf("gfb.yoffset: %d\n", var.yoffset);
    printf("gfb.bits_per_pixel: %d\n", var.bits_per_pixel);
    printf("gfb.activate: %d\n", var.activate);
    printf("gfb.height: %d\n", var.height);
    printf("gfb.width: %d\n", var.width);
    printf("gfb.accel_flags: %d\n", var.accel_flags);
    printf("gfb.pixclock: %d\n", var.pixclock);
    printf("gfb.hsync_len: %d\n", var.hsync_len);
    printf("gfb.vsync_len: %d\n", var.vsync_len);
    ret = ot_gfb_get_fix_screenf_info(fd, &fix);
    printf("print gfb fix message: \n");
    printf("fix.id: %s\n", fix.id);
    printf("fix.smem_len: %d\n", fix.smem_len);
    printf("fix.type: %d\n", fix.type);
    printf("fix.type_aux: %d\n", fix.type_aux);
    printf("fix.visual: %d\n", fix.visual);
    printf("fix.xpanstep: %d\n", fix.xpanstep);
    printf("fix.ypanstep: %d\n", fix.ypanstep);
    printf("fix.ywrapstep: %d\n", fix.ywrapstep);
    printf("fix.line_length: %d\n", fix.line_length);
    printf("fix.mmio_start: %ld\n", fix.mmio_start);
    printf("fix.mmio_len: %d\n", fix.mmio_len);
    color_key.enable = OT_TRUE;
    color_key.value = 0x01010101; //black
    ret = ot_gfb_set_colorkey_vaule(fd, &color_key);
    point.x_pos = 0;
    point.y_pos = 0;
    ret = ot_gfb_set_screen_origin(fd, &point);
    layer_info.buf_mode = OT_EIS_FB_LAYER_BUF_DOUBLE;
    layer_info.mask = OT_EIS_FB_LAYER_MASK_BUF_MODE;
    ret = ot_gfb_set_layer_info(fd, &layer_info);
    ot_gfb_set_display_state(fd, OT_TRUE);
    smr_alloc_attr.len = width * height * 4;
    smr_alloc_attr.cached = OT_FALSE;
    strcpy(smr_alloc_attr.region_name , "");
    ret = ot_smr_alloc(&smr_alloc_attr, &g_phyaddr, &viraddr);
    canvas_buf.canvas.phys_addr = g_phyaddr;
    canvas_buf.canvas.width = width;
    canvas_buf.canvas.height = height;
    canvas_buf.canvas.pitch = width * 4;
    canvas_buf.canvas.format = OT_EIS_FB_FORMAT_ARGB8888;
    memset_s(viraddr, width * height * (4), 0x00, canvas_buf.canvas.pitch * canvas_buf.canvas.height);
    for (y = (height / 2 - 2); y < (height / 2 + 2); y++) { /* 2 alg data */
        for (x = 0; x < width; x++) {
                *((ot_u32*)viraddr + y * width + x) = ARGB8888_GREEN;
        }
    }
    for (y = 0; y < height; y++) {
        for (x = (width / 2 - 2); x < (width / 2 + 2); x++) { /* 2 alg data */
                *((ot_u32*)viraddr + y * width + x) = ARGB8888_GREEN;
        }
    }
    printf("begin to draw line by cpu\n");
    canvas_buf.update_rect.x = 0;
    canvas_buf.update_rect.y = 0;
    canvas_buf.update_rect.width = width;
    canvas_buf.update_rect.height = height;
    ret = ot_gfb_refresh_display_info(fd, &canvas_buf);
    sleep(5);
    printf("begin to disappear line by cpu\n");
    memset_s(viraddr, width * height * (4), 0x00, canvas_buf.canvas.pitch * canvas_buf.canvas.height);
    ret = ot_gfb_refresh_display_info(fd, &canvas_buf);
    printf("begin to draw by oge\n");
    none_src.dst_rect = &dst_rect;
    none_src.dst_surface = &dst_surface;
    dst_surface.color_format = OT_OGE_COLOR_FORMAT_ARGB8888;
    dst_surface.width = width;
    dst_surface.height = height;
    dst_surface.stride = width * 4;
    dst_surface.phys_addr = g_phyaddr;
    dst_rect.pos_x = 600;
    dst_rect.pos_y = 200;
    dst_rect.width = 2;
    dst_rect.height = 200;
    fill_data = ARGB8888_RED;
    handle = ot_oge_begin_job();
    if (handle == OT_ERR_OGE_INVALID_HANDLE || handle == OT_ERR_OGE_DEV_NOT_OPEN) {
        printf("begin job failed \n");
        return;
    }
    ot_oge_quick_fill(handle, &none_src, fill_data);
    dst_rect.pos_x = 603;
    dst_rect.pos_y = 200;
    dst_rect.width = 298;
    dst_rect.height = 2;
    fill_data = ARGB8888_BLUE;
    ot_oge_quick_fill(handle, &none_src, fill_data);
    dst_rect.pos_x = 900;
    dst_rect.pos_y = 200;
    dst_rect.width = 2;
    dst_rect.height = 198;
    fill_data = ARGB8888_GREEN;
    ot_oge_quick_fill(handle, &none_src, fill_data);
    dst_rect.pos_x = 603;
    dst_rect.pos_y = 400;
    dst_rect.width = 297;
    dst_rect.height = 2;
    fill_data = ARGB8888_YELLOW;
    ot_oge_quick_fill(handle, &none_src, fill_data);
    ot_oge_end_job(handle, OT_FALSE, OT_TRUE, 60);
    ret = ot_gfb_refresh_display_info(fd, &canvas_buf);
    sleep(5);
    printf("begin to disappear rect by oge\n");
    // memset_s(viraddr, width * height * (4), 0x00, canvas_buf.canvas.pitch * canvas_buf.canvas.height);
    // ret = ot_gfb_refresh_display_info(fd, &canvas_buf);
    printf("begin to load bmp by oge\n");
    smr_alloc_attr.len = 160 * 160 * 4;
    smr_alloc_attr.cached = OT_FALSE;
    strcpy(smr_alloc_attr.region_name , "");
    ret = ot_smr_alloc(&smr_alloc_attr, &buf_phyaddr, &buf);
    ot_load_bmp_area(SAMPLE_BMP_PATH, buf, 160, 160, 160 * 4);

    /* 32bpp bmp图片没有a，需要给a配置FF，以免被colorkey忽略掉 */
    for (i = 0; i < 160; i++)
    {
        for (j = 0; j < 160; j++)
        {
            *((ot_u32 *)buf + i * 160 + j) |= 0xFF000000;
        }
    }

    printf("read bmp file success\n");
    dst_rect.pos_x = 100;
    dst_rect.pos_y = 100;
    dst_rect.width = 160;
    dst_rect.height = 160;
    src_rect.pos_x   = 0;
    src_rect.pos_y   = 0;
    src_rect.height = 160;
    src_rect.width  = 160;
    dst_surface.color_format = OT_OGE_COLOR_FORMAT_ARGB8888;

    src_surface.color_format = OT_OGE_COLOR_FORMAT_ARGB8888;
    src_surface.width = 160;
    src_surface.height = 160;
    src_surface.stride = 4 * 160;
    src_surface.phys_addr = buf_phyaddr;
    src_surface.support_alpha_ex_1555 = OT_TRUE;
    src_surface.alpha_max_is_255 = OT_TRUE;
    src_surface.alpha0 = 0X00;
    src_surface.alpha1 = 0XFF;
    single_src.src_surface = &src_surface;
    single_src.dst_surface = &dst_surface;
    single_src.src_rect = &src_rect;
    single_src.dst_rect = &dst_rect;
    handle = ot_oge_begin_job();
    if (handle == OT_ERR_OGE_INVALID_HANDLE || handle == OT_ERR_OGE_DEV_NOT_OPEN) {
        printf("begin job failed \n");
        return;
    }
    ret = ot_oge_quick_copy(handle, &single_src);
    ot_oge_end_job(handle, OT_FALSE, OT_TRUE, 10);
    ot_oge_wait_the_task_done(handle);
    ret = ot_gfb_refresh_display_info(fd, &canvas_buf);
    ot_smr_free(g_phyaddr);
    ot_smr_free(buf_phyaddr);
    printf("end to show oge\n");
}

ot_s32 sample_comm_vio_start_route(sample_vi_cfg *vi_cfg, sample_vo_attr *vo_attr)
{
    ot_s32 i, ret;
    ot_eis_img_size img_size;
    ot_eis_handle media_pipe_hdl;
    ot_video_buffer_attr vrb_cfg;
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    sample_common_get_vrb_cfg(3, img_size.width, img_size.height, &vrb_cfg);

    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    ret = sample_comm_vi_start_vi(vi_cfg);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }

    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_comm_media_pipe_init(&media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto pipe_start_failed;
    }

    for (i = 0; i < OT_VO_MAX_BIND_PORT_NUM; i++)
    {
        if (vo_attr->in_port_enable[i] == OT_TRUE) {
            ret = sample_comm_vi_bind_vo(vi_cfg->pipe_info[0].chn_info.chn_hdl, vo_attr->in_port_handle[i], media_pipe_hdl);
            if (ret != OT_SUCCESS) {
                goto vi_vo_bind_failed;
            }
        }
    }
    vi_cfg->media_pipe_hdl = media_pipe_hdl;
    return OT_SUCCESS;

vi_vo_bind_failed:
    sample_comm_media_pipe_stop(media_pipe_hdl);
pipe_start_failed:
    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_vi_stop_vi(vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();

    return OT_FAILURE;
}

ot_void sample_comm_vio_stop_route(sample_vi_cfg *vi_cfg, sample_vo_attr *vo_attr)
{
    ot_u32 i;
    for (i = 0; i < OT_VO_MAX_BIND_PORT_NUM; i++)
    {
        if (vo_attr->in_port_enable[i] == OT_TRUE) {
            sample_comm_vi_un_bind_vo(vi_cfg->pipe_info[0].chn_info.chn_hdl, vo_attr->in_port_handle[i], vi_cfg->media_pipe_hdl);
        }
    }
    sample_comm_media_pipe_stop(vi_cfg->media_pipe_hdl);
    sample_comm_stop_vo(vo_attr);
    sample_comm_vi_stop_vi(vi_cfg);
    sample_comm_sys_exit();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
