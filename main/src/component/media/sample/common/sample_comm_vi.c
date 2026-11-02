/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>

#include "sample_comm.h"
#include "ot_eis_common.h"
#include "ot_eis_mipi_rx.h"

#include "securec.h"
#include "ot_eis_sns_ctrl.h"
#include "ot_eis_vi.h"
#include "ot_eis_vi_api.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vi_awb_api.h"
#include "ot_board_info.h"

#define FPN_FILE_NAME_LENGTH 150
#define FPN_CALIB_TIMES 8
#define WIDTH_1920 1920
#define HEIGHT_1080 1080
#define HEIGHT_1280 1280
#define WIDTH_3840 3840
#define HEIGHT_2160 2160
#define WIDTH_2688 2688
#define HEIGHT_1520 1520
#define HEIGHT_1536 1536
#define SLEEP_TIME 1000
#define MIPI_EXT_TYPE_NUM 3

#define SNS_NUM_2           2

#define OX08B40_I2C_ADDR    0x6c
#define OX05B1S_I2C_ADDR    0x6c
#define OX03F10_I2C_ADDR    0x6c
#define OX03C_I2C_ADDR      0x6c
#define IMX290_I2C_ADDR     0x34
#define IMX623_I2C_ADDR     0x34
#define GC2093_I2C_ADDR     0xfc
#define SC220AT_I2C_ADDR    0x60
#define IMX582_I2C_ADDR     0x34

#define MAX9295_I2C_ADDR    0x80
#define MAX9296_I2C_ADDR    0x90

#define JSON_FILE_LENGTH    100

#define I2C_DST_ADDR(src_addr, idx)     \
    ((src_addr) < 0xF0) ? (src_addr + 2 * (idx + 1)) : (src_addr - 2 * (idx + 1))

extern ot_eis_sns_obj g_sns_ox08b40_obj;
extern ot_eis_sns_obj g_sns_ox03f10_obj;
extern ot_eis_sns_obj g_sns_ox03c_obj;
extern ot_eis_sns_obj g_sns_ox05b1s_obj;
extern ot_eis_sns_obj g_sns_imx623_obj;
extern ot_eis_sns_obj g_sns_imx290_obj;
extern ot_eis_sns_obj g_sns_gc2093_obj;
extern ot_eis_sns_obj g_sns_sc220at_obj;
extern ot_eis_sns_obj g_sns_imx582_obj;

static ot_eis_vi_if_attr g_mipi_4lane_sensor_imx290_10bit_2m_wdr_dev0_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 1920,
    .img_height = 1080,
    .mipi_attr = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_DOL,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_sensor_imx290_10bit_2m_wdr_dev2_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 1920,
    .img_height = 1080,
    .mipi_attr = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_DOL,
        .sns_num = 1,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_sensor_gc2093_10bit_2m_wdr_dev0_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 1920,
    .img_height = 1080,
    .mipi_attr = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_VC,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_sensor_gc2093_10bit_2m_wdr_dev2_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 1920,
    .img_height = 1080,
    .mipi_attr = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_VC,
        .sns_num = 1,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_sensor_gc2093_10bit_2m_nowdr_dev0_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 1920,
    .img_height = 1080,
    .mipi_attr = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_sensor_gc2093_10bit_2m_nowdr_dev2_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 1920,
    .img_height = 1080,
    .mipi_attr = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_sensor_ox08b40_12bit_8m_nowdr_dev0_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 3840,
    .img_height = 2160,
    .mipi_attr = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_12,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_sensor_ox08b40_12bit_8m_nowdr_dev2_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 3840,
    .img_height = 2160,
    .mipi_attr = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_RAW_12,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_sensor_imx582_10bit_8m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 3840,
    .img_height = 2160,
    .mipi_attr = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_sensor_imx582_10bit_12m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width  = 4000,
    .img_height = 3000,
    .mipi_attr = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};
 

static ot_eis_vi_if_attr g_mipi_4lane_sensor_imx582_10bit_48m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 8000,
    .img_height = 6000,
    .mipi_attr = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};


static ot_eis_vi_if_attr g_mipi_4lane_sensor_ox05b1s_10bit_5m_nowdr_dev0_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 2592,
    .img_height = 1944,
    .mipi_attr = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_10,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn0_sensor_imx623_16bit_3m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1936,
    .img_height = HEIGHT_1552,
    .mipi_attr  = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_16,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn2_sensor_imx623_16bit_3m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1936,
    .img_height = HEIGHT_1552,
    .mipi_attr  = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_RAW_16,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn0_sensor_ox03c_16bit_3m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1920,
    .img_height = HEIGHT_1280,
    .mipi_attr  = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_16,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn2_sensor_ox03c_16bit_3m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1920,
    .img_height = HEIGHT_1280,
    .mipi_attr  = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_RAW_16,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn0_sensor_ox03f10_16bit_3m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1920,
    .img_height = HEIGHT_1536,
    .mipi_attr  = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_16,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn2_sensor_ox03f10_16bit_3m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1920,
    .img_height = HEIGHT_1536,
    .mipi_attr  = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_RAW_16,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn0_sensor_sc220at_yuv422_2m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1920,
    .img_height = HEIGHT_1080,
    .mipi_attr  = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_YUV422_8BIT,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn2_sensor_sc220at_yuv422_2m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1920,
    .img_height = HEIGHT_1080,
    .mipi_attr  = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_YUV422_8BIT,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn0_sensor_sc220at_raw12_2m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1920,
    .img_height = HEIGHT_1080,
    .mipi_attr  = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_12,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {0, 1, 2, 3},
    }
};

static ot_eis_vi_if_attr g_mipi_4lane_chn2_sensor_sc220at_raw12_2m_nowdr_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = WIDTH_1920,
    .img_height = HEIGHT_1080,
    .mipi_attr  = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_RAW_12,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .sns_num = 1,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_intf_ext_attr g_mipi_ext_attr_default = {
    .data_attr = {
        .type_num = MIPI_EXT_TYPE_NUM,
        .data_bit_width = {12, 12, 12},
        .data_type = {0x2c, 0x2c, 0x2c}
    }
};

static ot_eis_vi_dev_attr g_imx290_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFC00000, 0x00000000},
    .in_size       = { 1920, 1080 },
};

static ot_eis_vi_dev_attr g_gc2093_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFC00000, 0x00000000},
    .in_size       = { 1920, 1080 },
};

static ot_eis_vi_dev_attr g_ox08b40_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFF00000, 0x00000000},
    .in_size       = { 3840, 2160 },
};

static ot_eis_vi_dev_attr g_imx582_8m_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFC00000, 0x00000000},
    .in_size       = { 3840, 2160 },
};

static ot_eis_vi_dev_attr g_imx582_12m_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFC00000, 0x00000000},
	.in_size       = { 4000, 3000 },
};


static ot_eis_vi_dev_attr g_imx582_48m_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFC00000, 0x00000000},
	.in_size       = { 8000, 6000 },
};


static ot_eis_vi_dev_attr g_ox05b1s_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFC00000, 0x00000000},
    .in_size       = { 2592, 1944 },
};

static ot_eis_vi_dev_attr g_imx623_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFFF0000, 0x00000000},
    .in_size       = { WIDTH_1936, HEIGHT_1552 },
};

static ot_eis_vi_dev_attr g_ox03c_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFFF0000, 0x00000000},
    .in_size       = { WIDTH_1920, HEIGHT_1280 },
};

static ot_eis_vi_dev_attr g_ox03f10_eis_vi_dev_attr = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFFF0000, 0x00000000},
    .in_size       = { WIDTH_1920, HEIGHT_1536 },
};

static ot_eis_vi_dev_attr g_sc220at_eis_vi_dev_attr_yuv422 = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_YUV422,
    .data_type = OT_EIS_VI_DATA_TYPE_YUV,
    .data_seq  = OT_EIS_VI_DATA_SEQ_VYUY,

    .component_mask = {0xFF000000, 0x00FF0000},
    .in_size       = { WIDTH_1920, HEIGHT_1080 },
};

static ot_eis_vi_dev_attr g_sc220at_eis_vi_dev_attr_raw12 = {
    .mipirx_dev_no = 0,
    .intf_mode = OT_EIS_VI_INTF_MODE_MIPI_RAW,
    .data_type = OT_EIS_VI_DATA_TYPE_RAW,
    .data_seq  = OT_EIS_VI_DATA_SEQ_YVYU,

    .component_mask = {0xFFF00000, 0x00000000},
    .in_size       = { WIDTH_1920, HEIGHT_1080 },
};

static ot_eis_vi_pipe_attr g_ox05b1s_eis_vi_pipe_attr_linear = {
    .image_mode   = { 2592, 1944, 60, 0 },
    .wdr_mode     = OT_EIS_WDR_MODE_NONE,
    .bayer_format = OT_EIS_VI_BAYER_BGGR,
    .bus_info = { OT_EIS_VI_SNS_TYPE_I2C, 2, OX05B1S_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_10BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_ox08b40_eis_vi_pipe_attr_linear = {
    .image_mode   = { 3840, 2160, 30, 0 },
    .wdr_mode     = OT_EIS_WDR_MODE_BUILT_IN,
    .bayer_format = OT_EIS_VI_BAYER_BGGR,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 2, OX08B40_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_12BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_imx582_8m_eis_vi_pipe_attr_linear = {
    .image_mode   = { 3840, 2160, 30, 0 },
    .wdr_mode     = OT_EIS_WDR_MODE_NONE,
    .bayer_format = OT_EIS_VI_BAYER_RGGB,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 5, IMX582_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_10BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_imx582_12m_eis_vi_pipe_attr_linear = {
    .image_mode   = { 4000, 3000, 20, 1 },
    .wdr_mode     = OT_EIS_WDR_MODE_NONE,
    .bayer_format = OT_EIS_VI_BAYER_RGGB,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 5, IMX582_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_10BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_imx582_48m_eis_vi_pipe_attr_linear = {
    .image_mode   = { 8000, 6000, 5, 2 },
    .wdr_mode     = OT_EIS_WDR_MODE_NONE,
    .bayer_format = OT_EIS_VI_BAYER_RGGB,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 5, IMX582_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_10BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_imx290_eis_vi_pipe_attr_wdr = {
    .image_mode   = { 1920, 1080, 60, 0 },
    .wdr_mode     = OT_EIS_WDR_MODE_2To1_LINE,
    .bayer_format = OT_EIS_VI_BAYER_RGGB,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 2, IMX290_I2C_ADDR },
    .vc_id        = {0, 1},
    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_10BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_gc2093_eis_vi_pipe_attr_wdr = {
    .image_mode   = { 1920, 1080, 30, 0 },
    .wdr_mode     = OT_EIS_WDR_MODE_2To1_LINE,
    .bayer_format = OT_EIS_VI_BAYER_RGGB,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 2, GC2093_I2C_ADDR },
    .vc_id        = {0, 1},
    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_10BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_gc2093_eis_vi_pipe_attr_linear = {
    .image_mode   = { 1920, 1080, 30, 0 },
    .wdr_mode     = OT_EIS_WDR_MODE_NONE,
    .bayer_format = OT_EIS_VI_BAYER_RGGB,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 2, GC2093_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_10BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_imx623_eis_vi_pipe_attr_built_in = {
    .image_mode   = { WIDTH_1936, HEIGHT_1552, 60, 0 },
    .wdr_mode     = OT_EIS_WDR_MODE_BUILT_IN,
    .bayer_format = OT_EIS_VI_BAYER_RGGB,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 5, IMX623_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_16BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_ox03c_eis_vi_pipe_attr_built_in = {
    .image_mode   = { WIDTH_1920, HEIGHT_1280, 60, 0 },
   .wdr_mode     = OT_EIS_WDR_MODE_BUILT_IN,
    .bayer_format = OT_EIS_VI_BAYER_BGGR,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 5, OX03C_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_16BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_ox03f10_eis_vi_pipe_attr_built_in = {
    .image_mode   = { WIDTH_1920, HEIGHT_1536, 60, 0 },
    .wdr_mode     = OT_EIS_WDR_MODE_BUILT_IN,
    .bayer_format = OT_EIS_VI_BAYER_BGGR,
    .bus_info     = { OT_EIS_VI_SNS_TYPE_I2C, 5, OX03F10_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_16BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_sc220at_eis_vi_pipe_attr_yuv422 = {
    // sns_mode = 0, yuv422 master mode; sns_mode = 1, raw12 master mode
    .image_mode   = { WIDTH_1920, HEIGHT_1080, 30, 0 },
    .wdr_mode     = OT_EIS_WDR_MODE_NONE,
    .bayer_format = OT_EIS_VI_BAYER_BGGR,
    .bus_info = { OT_EIS_VI_SNS_TYPE_I2C, 5, SC220AT_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_TRUE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_pipe_attr g_sc220at_eis_vi_pipe_attr_raw12_wdr = {
    // sns_mode = 0, yuv422 master mode; sns_mode = 1, raw12 master mode
    .image_mode   = { WIDTH_1920, HEIGHT_1080, 30, 1 },
    .wdr_mode     = OT_EIS_WDR_MODE_BUILT_IN,
    .bayer_format = OT_EIS_VI_BAYER_BGGR,
    .bus_info = { OT_EIS_VI_SNS_TYPE_I2C, 5, SC220AT_I2C_ADDR },

    .frame_source    = OT_EIS_VI_PIPE_FRAME_SOURCE_SENSOR,
    .bypass_mode     = OT_EIS_VI_PIPE_BYPASS_NONE,
    .isp_bypass      = OT_FALSE,
    .pixel_format    = OT_EIS_IMAGE_FORMAT_BAYER_12BPP,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .frame_rate_ctrl = { -1, -1 },
};

static ot_eis_vi_chn_attr g_eis_vi_chn_attr = {
    .size            = { 3840, 2160 },
    .pixel_format    = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
    .compress_mode   = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    .crop_info       = { 0, { 0, 0, 3840, 2160 }},
    .frame_rate_ctrl = { -1, -1 },

    /* for user_mode */
    .work_mode         = OT_EIS_VI_CHN_WORK_MODE_PIPE,
    .frame_queue_depth = 0,
    .pool_handle       = NULL,
};

ot_eis_sns_obj *sample_comm_isp_get_sns_obj(sample_sns_type sns_type)
{
    switch (sns_type) {
        case OV_OX08B40_MIPI_8M_30FPS_12BIT:
        case OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT:
            return &g_sns_ox08b40_obj;
        case OV_OX03C_MIPI_3M_60FPS_16BIT:
            return &g_sns_ox03c_obj;
        case OV_OX03F10_MIPI_3M_60FPS_12BIT:
        case OV_OX03F10_MIPI_3M_60FPS_16BIT:
            return &g_sns_ox03f10_obj;
        case OV_OX05B1S_MIPI_5M_60FPS_10BIT:
            return &g_sns_ox05b1s_obj;
        case SONY_IMX623_MIPI_3M_60FPS_16BIT:
            return &g_sns_imx623_obj;
        case SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1:
        case SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1:
            return &g_sns_imx290_obj;
        case SONY_IMX582_MIPI_8M_30FPS_10BIT_DIRECT:
        case SONY_IMX582_MIPI_12M_20FPS_10BIT_DIRECT:
        case SONY_IMX582_MIPI_48M_5FPS_10BIT_DIRECT:
            return &g_sns_imx582_obj;
        case GC_GC2093_MIPI_2M_30FPS_10BIT:
        case GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1:
            return &g_sns_gc2093_obj;
        case SMS_SC220AT_MIPI_2M_30FPS_YUV422:
        case SMS_SC220AT_MIPI_2M_30FPS_RAW12:
            return &g_sns_sc220at_obj;
        default:
            return OT_NULL;
    }
}


static ot_void sample_comm_vi_get_mipi_attr_by_dev_id(sample_sns_type sns_type, ot_vi_dev_id vi_dev,
                                                      ot_eis_vi_if_attr *mipi_attr)
{
    switch(sns_type) {
        case OV_OX08B40_MIPI_8M_30FPS_12BIT:
        case OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT:
            if (vi_dev == 0) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_sensor_ox08b40_12bit_8m_nowdr_dev0_attr, sizeof(ot_eis_vi_if_attr));
            } else if (vi_dev == 2) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_sensor_ox08b40_12bit_8m_nowdr_dev2_attr, sizeof(ot_eis_vi_if_attr));
            }
            break;
        case SONY_IMX582_MIPI_8M_30FPS_10BIT_DIRECT:
            (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                &g_mipi_4lane_sensor_imx582_10bit_8m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            break;
 		case SONY_IMX582_MIPI_12M_20FPS_10BIT_DIRECT:
             (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                 &g_mipi_4lane_sensor_imx582_10bit_12m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
             break;
        case SONY_IMX582_MIPI_48M_5FPS_10BIT_DIRECT:
            (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                &g_mipi_4lane_sensor_imx582_10bit_48m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            break;
        case OV_OX03C_MIPI_3M_60FPS_16BIT:
            if (vi_dev == 0) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn0_sensor_ox03c_16bit_3m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            } else if (vi_dev == 2) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn2_sensor_ox03c_16bit_3m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            }
            break;
        case OV_OX05B1S_MIPI_5M_60FPS_10BIT:
            (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                &g_mipi_4lane_sensor_ox05b1s_10bit_5m_nowdr_dev0_attr, sizeof(ot_eis_vi_if_attr));
            break;
        case OV_OX03F10_MIPI_3M_60FPS_12BIT:
        case OV_OX03F10_MIPI_3M_60FPS_16BIT:
            if (vi_dev == 0) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn0_sensor_ox03f10_16bit_3m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            } else if (vi_dev == 2) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn2_sensor_ox03f10_16bit_3m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            }
            if (sns_type == OV_OX03F10_MIPI_3M_60FPS_12BIT) {
                mipi_attr->mipi_attr.data_fmt = OT_MIPIRX_DT_RAW_12;
            }
            break;
        case SONY_IMX623_MIPI_3M_60FPS_16BIT:
            if (vi_dev == 0) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn0_sensor_imx623_16bit_3m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            } else if (vi_dev == 2) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn2_sensor_imx623_16bit_3m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            }
            break;
        case SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1:
        case SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1:
            if (vi_dev == 0) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_sensor_imx290_10bit_2m_wdr_dev0_attr, sizeof(ot_eis_vi_if_attr));
            } else if (vi_dev == 2) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_sensor_imx290_10bit_2m_wdr_dev2_attr, sizeof(ot_eis_vi_if_attr));
            }
            if (sns_type == SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1) {
                mipi_attr->mipi_attr.data_fmt = OT_MIPIRX_DT_RAW_12;
            }
            break;
        case GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1:
            if (vi_dev == 0) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_sensor_gc2093_10bit_2m_wdr_dev0_attr, sizeof(ot_eis_vi_if_attr));
            } else if (vi_dev == 2) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_sensor_gc2093_10bit_2m_wdr_dev2_attr, sizeof(ot_eis_vi_if_attr));
            }
            break;
        case GC_GC2093_MIPI_2M_30FPS_10BIT:
            if (vi_dev == 0) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_sensor_gc2093_10bit_2m_nowdr_dev0_attr, sizeof(ot_eis_vi_if_attr));
            } else if (vi_dev == 2) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_sensor_gc2093_10bit_2m_nowdr_dev2_attr, sizeof(ot_eis_vi_if_attr));
            }
            break;
        case SMS_SC220AT_MIPI_2M_30FPS_YUV422:
            if (vi_dev == 0) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn0_sensor_sc220at_yuv422_2m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            } else if (vi_dev == 2) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn2_sensor_sc220at_yuv422_2m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            }
            break;
        case SMS_SC220AT_MIPI_2M_30FPS_RAW12:
            if (vi_dev == 0) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn0_sensor_sc220at_raw12_2m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            } else if (vi_dev == 2) {
                (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                    &g_mipi_4lane_chn2_sensor_sc220at_raw12_2m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            }
            break;
        default:
            (ot_void)memcpy_s(mipi_attr, sizeof(ot_eis_vi_if_attr),
                &g_mipi_4lane_chn0_sensor_imx623_16bit_3m_nowdr_attr, sizeof(ot_eis_vi_if_attr));
            break;
    }
}

ot_void sample_comm_vi_get_size_by_sns_type(sample_sns_type sns_type, ot_eis_img_size *size)
{
    switch (sns_type) {
        case OV_OX08B40_MIPI_8M_30FPS_12BIT:
        case OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT:
        case SONY_IMX582_MIPI_8M_30FPS_10BIT_DIRECT:
            size->width  = WIDTH_3840;
            size->height = HEIGHT_2160;
            break;
        case OV_OX03C_MIPI_3M_60FPS_16BIT:
            size->width = WIDTH_1920;
            size->height = HEIGHT_1280;
            break;
        case OV_OX05B1S_MIPI_5M_60FPS_10BIT:
            size->width = 2592;
            size->height = 1944;
            break;
        case OV_OX03F10_MIPI_3M_60FPS_12BIT:
        case OV_OX03F10_MIPI_3M_60FPS_16BIT:
            size->width = WIDTH_1920;
            size->height = HEIGHT_1536;
            break;
        case SONY_IMX623_MIPI_3M_60FPS_16BIT:
            size->width = WIDTH_1936;
            size->height = HEIGHT_1552;
            break;
        case SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1:
        case SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1:
        case GC_GC2093_MIPI_2M_30FPS_10BIT:
        case GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1:
            size->width = WIDTH_1920;
            size->height = HEIGHT_1080;
            break;
        case SMS_SC220AT_MIPI_2M_30FPS_YUV422:
        case SMS_SC220AT_MIPI_2M_30FPS_RAW12:
            size->width  = WIDTH_1920;
            size->height = HEIGHT_1080;
            break;
		case SONY_IMX582_MIPI_12M_20FPS_10BIT_DIRECT:
            size->width  = 4000;
            size->height = 3000;
            break;
        case SONY_IMX582_MIPI_48M_5FPS_10BIT_DIRECT:
            size->width  = 8000;
            size->height = 6000;
            break;
        default:
            size->width  = WIDTH_1920;
            size->height = HEIGHT_1080;
            break;
    }
}

ot_void sample_comm_vi_get_default_sns_info(sample_sns_type sns_type, ot_s32 dev_id, sample_sns_info *sns_info)
{
    sns_info->sns_type    = sns_type;
    if (dev_id == 2) {
        sns_info->sns_clk_src = 1;
        sns_info->sns_rst_src = 1;
        sns_info->bus_id      = MIPI_DEV2_I2C_BUS;
    } else {
        sns_info->sns_clk_src = 1;
        sns_info->sns_rst_src = 1;
        sns_info->bus_id      = 5;  
    }
    sns_info->sns_clk_rst_en = OT_TRUE;
}

static ot_u8 sample_comm_vi_get_sensor_addr(sample_sns_type sns_type)
{
    switch (sns_type) {
        case OV_OX08B40_MIPI_8M_30FPS_12BIT:
        case OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT:
            return OX08B40_I2C_ADDR;
        case OV_OX03C_MIPI_3M_60FPS_16BIT:
            return OX03C_I2C_ADDR;
        case OV_OX05B1S_MIPI_5M_60FPS_10BIT:
            return OX05B1S_I2C_ADDR;
        case OV_OX03F10_MIPI_3M_60FPS_12BIT:
        case OV_OX03F10_MIPI_3M_60FPS_16BIT:
            return OX03F10_I2C_ADDR;
        case SONY_IMX623_MIPI_3M_60FPS_16BIT:
            return IMX623_I2C_ADDR;
        case SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1:
        case SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1:
            return IMX290_I2C_ADDR;
        case GC_GC2093_MIPI_2M_30FPS_10BIT:
        case GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1:
            return GC2093_I2C_ADDR;
        default: return IMX623_I2C_ADDR;
    }
}

static ot_void sample_comm_vi_set_dev_two_sensor_serdes_vc_id(
    sample_sns_type sns_type, ot_eis_vi_serdes_attr *serdes_attr)
{
    switch (sns_type) {
        // WDR VC 2TO1
        case GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1:
            serdes_attr->multi_port_attr[0].vc_id[0] = 0;
            serdes_attr->multi_port_attr[0].vc_id[1] = 1;
            serdes_attr->multi_port_attr[1].vc_id[0] = 2;
            serdes_attr->multi_port_attr[1].vc_id[1] = 3;
            break;
        // WDR DOL 2TO1
        case SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1:
        case SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1:
            serdes_attr->multi_port_attr[0].vc_id[0] = 0;
            serdes_attr->multi_port_attr[1].vc_id[0] = 2;
            break;
        // linear sensor
        default:
            serdes_attr->multi_port_attr[0].vc_id[0] = 0;
            serdes_attr->multi_port_attr[1].vc_id[0] = 1;
            break;
    }
}

static ot_void sample_comm_vi_set_dev_two_sensor_pipe_vc_id(
    sample_sns_type sns_type, sample_vi_cfg *vi_cfg)
{
    switch (sns_type) {
        // WDR VC 2TO1, dol format in serdes only one vc, in pipe two vc
        case SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1:
        case SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1:
            vi_cfg->pipe_info[0].pipe_attr.vc_id[0] = 0;
            vi_cfg->pipe_info[0].pipe_attr.vc_id[1] = 1;
            vi_cfg->pipe_info[1].pipe_attr.vc_id[0] = 2;
            vi_cfg->pipe_info[1].pipe_attr.vc_id[1] = 3;
            break;
        default:
            memcpy(vi_cfg->pipe_info[0].pipe_attr.vc_id,
                vi_cfg->serdes_info.serdes_attr.multi_port_attr[0].vc_id, OT_EIS_VI_SERDES_VC_ID_MAX);
            memcpy(vi_cfg->pipe_info[1].pipe_attr.vc_id,
                vi_cfg->serdes_info.serdes_attr.multi_port_attr[1].vc_id, OT_EIS_VI_SERDES_VC_ID_MAX);
            break;
    }
}

ot_void sample_comm_vi_get_default_serdes_info(sample_sns_type sns_type, ot_s32 dev_id, ot_u8 sns_nums, sample_vi_serdes_info *info)
{
    ot_eis_vi_serdes_attr *serdes_attr = &info->serdes_attr;
    static ot_u8 val = 0;

    serdes_attr->sns_lanes          = 4; // 4lane
    serdes_attr->bus_info.bus_type  = OT_VI_SERDES_BUS_TYPE_I2C;
    serdes_attr->deserial_addr      = MAX9296_I2C_ADDR;
    serdes_attr->sns_nums           = sns_nums;
    if (serdes_attr->sns_nums == 1) {
        serdes_attr->one_port_attr.serial_addr = MAX9295_I2C_ADDR;
        serdes_attr->freq_mhz       = 1200; // 1200Mhz
    } else if (serdes_attr->sns_nums == SNS_NUM_2) {
        ot_s32 i;

        serdes_attr->freq_mhz       = 2000; // 2000Mhz
        sample_comm_vi_set_dev_two_sensor_serdes_vc_id(sns_type, serdes_attr);
        for (i = 0; i < serdes_attr->sns_nums; i++) {
            ot_eis_vi_serdes_multi_port_attr *port_attr = &(serdes_attr->multi_port_attr[i]);
            port_attr->serdes_port     = (i == 0) ? OT_VI_SERDES_PORT_0 : OT_VI_SERDES_PORT_1;
            port_attr->serial_src_addr = MAX9295_I2C_ADDR;
            port_attr->serial_dst_addr = I2C_DST_ADDR(MAX9295_I2C_ADDR, i);
            port_attr->sns_src_addr    = sample_comm_vi_get_sensor_addr(sns_type);
            port_attr->sns_dst_addr    = I2C_DST_ADDR(port_attr->sns_src_addr, i);
        }
    }

    // bus id
    if (dev_id == 2) {
        serdes_attr->bus_info.bus_id = MIPI_DEV2_I2C_BUS;
    } else {
        serdes_attr->bus_info.bus_id = MIPI_DEV0_I2C_BUS;
    }
    if (sns_type == OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT   || 
        sns_type == SONY_IMX582_MIPI_8M_30FPS_10BIT_DIRECT  || 
        sns_type == SONY_IMX582_MIPI_12M_20FPS_10BIT_DIRECT ||
        sns_type == SONY_IMX582_MIPI_48M_5FPS_10BIT_DIRECT) 
    {
        serdes_attr->bus_info.bus_id = -1;
    }

    // data format
    if ((sns_type == SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1) || (sns_type == GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1) ||
        (sns_type == GC_GC2093_MIPI_2M_30FPS_10BIT) || (sns_type == OV_OX05B1S_MIPI_5M_60FPS_10BIT)) {
        serdes_attr->data_fmt = OT_VI_SERDES_DATA_FMT_RAW_10BIT;
    } else if ((sns_type == OV_OX08B40_MIPI_8M_30FPS_12BIT) || (sns_type == OV_OX03F10_MIPI_3M_60FPS_12BIT) ||
               (sns_type == SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1) || (sns_type == SMS_SC220AT_MIPI_2M_30FPS_RAW12)) {
        serdes_attr->data_fmt = OT_VI_SERDES_DATA_FMT_RAW_12BIT;
    } else if (sns_type == SMS_SC220AT_MIPI_2M_30FPS_YUV422) {
        serdes_attr->data_fmt = OT_VI_SERDES_DATA_FMT_YUV422_8BIT;
    } else {
        serdes_attr->data_fmt = OT_VI_SERDES_DATA_FMT_RAW_16BIT;
    }

    // freq
    if ((sns_type == GC_GC2093_MIPI_2M_30FPS_10BIT) || (sns_type == GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1) ||
        (sns_type == SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1)) {
        if (sns_nums == 1) {
            serdes_attr->freq_mhz = 600; // 600MHz be enough
        } else {
            serdes_attr->freq_mhz = 1200; // 1200MHz be enough
        }
    }

    // sns lanes
    if ((sns_type == GC_GC2093_MIPI_2M_30FPS_10BIT) || (sns_type == GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1)) {
        serdes_attr->sns_lanes = 2;
    }

    // wdr type
    if ((sns_type == GC_GC2093_MIPI_2M_30FPS_10BIT) || (sns_type == OV_OX03F10_MIPI_3M_60FPS_12BIT) ||
        (sns_type == OV_OX03C_MIPI_3M_60FPS_16BIT) || (sns_type == OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT) ||
        (sns_type == OV_OX08B40_MIPI_8M_30FPS_12BIT) || (sns_type == OV_OX03F10_MIPI_3M_60FPS_16BIT) ||
        (sns_type == SONY_IMX623_MIPI_3M_60FPS_16BIT) || (sns_type == SMS_SC220AT_MIPI_2M_30FPS_YUV422) ||
        (sns_type == OV_OX05B1S_MIPI_5M_60FPS_10BIT) || (sns_type == SMS_SC220AT_MIPI_2M_30FPS_RAW12)) {
        serdes_attr->wdr_type = OT_VI_SERDES_WDR_TYPE_NONE;
    } else if ((sns_type == SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1) || (sns_type == SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1)) {
        serdes_attr->wdr_type = OT_VI_SERDES_WDR_TYPE_DOL;
    } else if (sns_type == GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1) {
        serdes_attr->wdr_type = OT_VI_SERDES_WDR_TYPE_VC;
        serdes_attr->wdr_mode = OT_VI_SERDES_WDR_MODE_2TO1;
    }

    if ((sns_type == OV_OX03F10_MIPI_3M_60FPS_12BIT) || (sns_type == OV_OX03F10_MIPI_3M_60FPS_16BIT)) {
        static ot_bool is_x3f = OT_TRUE;
        serdes_attr->priv_data = &is_x3f;
    }

    if((sns_type == SMS_SC220AT_MIPI_2M_30FPS_YUV422) || (sns_type == SMS_SC220AT_MIPI_2M_30FPS_RAW12)) {
        ot_u8 is_sc220 = OT_TRUE;
        val = (is_sc220 << 1);
        serdes_attr->priv_data = &val;
    }
}

static ot_void sample_comm_vi_get_mipi_ext_data_attr(sample_sns_type sns_type, ot_eis_vi_intf_ext_attr *mipi_ext_attr)
{
    (ot_void)memcpy_s(mipi_ext_attr, sizeof(ot_eis_vi_intf_ext_attr),
        &g_mipi_ext_attr_default, sizeof(ot_eis_vi_intf_ext_attr));
}
/* used for two sensor: mipi lane 4 + 4 */
ot_void sample_comm_vi_get_mipi_info_by_dev_id(sample_sns_type sns_type, ot_vi_dev_id vi_dev, sample_mipi_info *mipi_info)
{
    ot_chip_type_t chip_type = OT_CHIP_TYPE_PLUSV100;
    ot_bdi_get_chip_type(&chip_type);
    if (chip_type == OT_CHIP_TYPE_TINYV100) {
        mipi_info->init_attr.divide_mode = OT_MIPIRX_LANE_LAYOUT_MODE_0;
    } else {
        mipi_info->init_attr.divide_mode = OT_MIPIRX_LANE_LAYOUT_MODE_1;
    }

    sample_comm_vi_get_mipi_attr_by_dev_id(sns_type, vi_dev, &mipi_info->mipi_attr);
    sample_comm_vi_get_mipi_ext_data_attr(sns_type, &mipi_info->mipi_ext_attr);
}

ot_void sample_comm_vi_get_default_dev_info(sample_sns_type sns_type, ot_eis_vi_dev_attr *dev_info)
{
    switch (sns_type) {
        case OV_OX08B40_MIPI_8M_30FPS_12BIT:
        case OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_ox08b40_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            break;
        case SONY_IMX582_MIPI_8M_30FPS_10BIT_DIRECT:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_imx582_8m_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            break;
        case SONY_IMX582_MIPI_12M_20FPS_10BIT_DIRECT:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_imx582_12m_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            break;
        case SONY_IMX582_MIPI_48M_5FPS_10BIT_DIRECT:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_imx582_48m_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            break;
        case OV_OX03C_MIPI_3M_60FPS_16BIT:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_ox03c_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            break;
        case OV_OX05B1S_MIPI_5M_60FPS_10BIT:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_ox05b1s_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            break;
        case OV_OX03F10_MIPI_3M_60FPS_12BIT:
        case OV_OX03F10_MIPI_3M_60FPS_16BIT:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_ox03f10_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            if (sns_type == OV_OX03F10_MIPI_3M_60FPS_12BIT) {
                dev_info->component_mask[0] = 0xFFF00000;
            }
            break;
        case SONY_IMX623_MIPI_3M_60FPS_16BIT:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_imx623_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            break;
        case SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1:
        case SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_imx290_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            if (sns_type == SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1) {
                dev_info->component_mask[0] = 0xFFF00000;
            }
            break;
        case GC_GC2093_MIPI_2M_30FPS_10BIT:
        case GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_gc2093_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            break;
        case SMS_SC220AT_MIPI_2M_30FPS_YUV422:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_sc220at_eis_vi_dev_attr_yuv422, sizeof(ot_eis_vi_dev_attr));
            break;
        case SMS_SC220AT_MIPI_2M_30FPS_RAW12:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_sc220at_eis_vi_dev_attr_raw12, sizeof(ot_eis_vi_dev_attr));
            break;
        default:
            memcpy_s(dev_info, sizeof(ot_eis_vi_dev_attr), &g_imx623_eis_vi_dev_attr, sizeof(ot_eis_vi_dev_attr));
            break;
    }
}

static ot_void sample_comm_vi_get_default_pipe_attr(sample_sns_type sns_type, ot_eis_vi_pipe_attr *pipe_attr,
                                                    ot_s32 bus_id)
{
    switch (sns_type) {
        case OV_OX08B40_MIPI_8M_30FPS_12BIT:
        case OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_ox08b40_eis_vi_pipe_attr_linear, sizeof(ot_eis_vi_pipe_attr));
            break;
        case SONY_IMX582_MIPI_8M_30FPS_10BIT_DIRECT:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_imx582_8m_eis_vi_pipe_attr_linear, sizeof(ot_eis_vi_pipe_attr));
            break;
 		case SONY_IMX582_MIPI_12M_20FPS_10BIT_DIRECT:
             memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                      &g_imx582_12m_eis_vi_pipe_attr_linear, sizeof(ot_eis_vi_pipe_attr));
             break;
        case SONY_IMX582_MIPI_48M_5FPS_10BIT_DIRECT:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_imx582_48m_eis_vi_pipe_attr_linear, sizeof(ot_eis_vi_pipe_attr));
            break;
        case OV_OX05B1S_MIPI_5M_60FPS_10BIT:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_ox05b1s_eis_vi_pipe_attr_linear, sizeof(ot_eis_vi_pipe_attr));
            break;
        case OV_OX03F10_MIPI_3M_60FPS_12BIT:
        case OV_OX03F10_MIPI_3M_60FPS_16BIT:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_ox03f10_eis_vi_pipe_attr_built_in, sizeof(ot_eis_vi_pipe_attr));
            if (sns_type == OV_OX03F10_MIPI_3M_60FPS_12BIT) {
                pipe_attr->pixel_format = OT_EIS_IMAGE_FORMAT_BAYER_12BPP;
            }
            break;
        case SONY_IMX623_MIPI_3M_60FPS_16BIT:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_imx623_eis_vi_pipe_attr_built_in, sizeof(ot_eis_vi_pipe_attr));
            break;
        case OV_OX03C_MIPI_3M_60FPS_16BIT:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_ox03c_eis_vi_pipe_attr_built_in, sizeof(ot_eis_vi_pipe_attr));
            break;
        case SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1:
        case SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_imx290_eis_vi_pipe_attr_wdr, sizeof(ot_eis_vi_pipe_attr));
            if (sns_type == SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1) {
                pipe_attr->image_mode.fps = 30;
                pipe_attr->pixel_format = OT_EIS_IMAGE_FORMAT_BAYER_12BPP;
            }
            break;
        case GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_gc2093_eis_vi_pipe_attr_wdr, sizeof(ot_eis_vi_pipe_attr));
            break;
        case GC_GC2093_MIPI_2M_30FPS_10BIT:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_gc2093_eis_vi_pipe_attr_linear, sizeof(ot_eis_vi_pipe_attr));
            break;
        case SMS_SC220AT_MIPI_2M_30FPS_YUV422:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_sc220at_eis_vi_pipe_attr_yuv422, sizeof(ot_eis_vi_pipe_attr));
            break;
        case SMS_SC220AT_MIPI_2M_30FPS_RAW12:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_sc220at_eis_vi_pipe_attr_raw12_wdr, sizeof(ot_eis_vi_pipe_attr));
            break;
        default:
            memcpy_s(pipe_attr, sizeof(ot_eis_vi_pipe_attr),
                     &g_imx623_eis_vi_pipe_attr_built_in, sizeof(ot_eis_vi_pipe_attr));
            break;
    }
    pipe_attr->bus_info.bus_id = bus_id;
}

ot_void sample_comm_vi_get_default_pipe_info(sample_sns_type sns_type, sample_vi_pipe_info *pipe_info, ot_s32 bus_id)
{
    ot_s32 i, pipe_num = 0;
    sample_comm_vi_get_default_pipe_attr(sns_type, &pipe_info->pipe_attr, bus_id);

    if (sns_type == SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1 ||
        sns_type == SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1 ||
        sns_type == GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1) {
            pipe_num = 2;
    } else {
        pipe_num = 1;
    }

    for (i = 0; i < pipe_num; i++) {
        pipe_info->pipe_id[i] = i;
    }

    pipe_info->nr_attr.enable = OT_TRUE;
    pipe_info->nr_attr.nr_type = OT_EIS_NR_TYPE_VIDEO_NORM;
}

ot_void sample_comm_vi_get_default_chn_info(sample_sns_type sns_type, ot_eis_vi_chn_attr *chn_attr)
{
    ot_eis_img_size  size = { 0 };
    sample_comm_vi_get_size_by_sns_type(sns_type, &size);
    memcpy_s(chn_attr, sizeof(ot_eis_vi_chn_attr), &g_eis_vi_chn_attr, sizeof(ot_eis_vi_chn_attr));

    if (sns_type == SMS_SC220AT_MIPI_2M_30FPS_YUV422) {
        chn_attr->pixel_format = OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR;
    }
    chn_attr->size.width = size.width;
    chn_attr->size.height = size.height;

    chn_attr->crop_info.rect.x = 0;
    chn_attr->crop_info.rect.y = 0;
    chn_attr->crop_info.rect.width = size.width;
    chn_attr->crop_info.rect.height = size.height;
}

ot_void sample_comm_vi_get_default_vi_cfg(sample_sns_type sns_type, sample_vi_cfg *vi_cfg)
{
    ot_s32 dev_id = 0;

    (ot_void)memset_s(vi_cfg, sizeof(sample_vi_cfg), 0, sizeof(sample_vi_cfg));

    if (sns_type == OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT) {
        dev_id = 2;
    }
    /* sensor info */
    sample_comm_vi_get_default_sns_info(sns_type, dev_id, &vi_cfg->sns_info);
    /* mipi info */
    sample_comm_vi_get_mipi_info_by_dev_id(sns_type, dev_id, &vi_cfg->mipi_info);
    /* serdes info */
    sample_comm_vi_get_default_serdes_info(sns_type, dev_id, 1, &vi_cfg->serdes_info);

    /* dev info */
    sample_comm_vi_get_default_dev_info(sns_type, &vi_cfg->dev_info.dev_attr);
    vi_cfg->dev_info.dev_attr.mipirx_dev_no = vi_cfg->mipi_info.mipi_attr.mipi_attr.devno;

    /* pipe info */
    sample_comm_vi_get_default_pipe_info(sns_type, &vi_cfg->pipe_info[0], vi_cfg->sns_info.bus_id);

    vi_cfg->pipe_num = 1;
    vi_cfg->pipe_info[0].chn_info.chn_id = 0;
    sample_comm_vi_get_default_chn_info(sns_type, &vi_cfg->pipe_info[0].chn_info.chn_attr);
}

ot_void sample_comm_vi_get_default_vi_cfg_by_dev_id(sample_sns_type sns_type, sample_vi_cfg *vi_cfg, ot_s32 dev_id)
{
    (ot_void)memset_s(vi_cfg, sizeof(sample_vi_cfg), 0, sizeof(sample_vi_cfg));
    /* sensor info */
    sample_comm_vi_get_default_sns_info(sns_type, dev_id, &vi_cfg->sns_info);
    /* mipi info */
    sample_comm_vi_get_mipi_info_by_dev_id(sns_type, dev_id, &vi_cfg->mipi_info);
    /* serdes info */
    sample_comm_vi_get_default_serdes_info(sns_type, dev_id, 1, &vi_cfg->serdes_info);

    /* dev info */
    sample_comm_vi_get_default_dev_info(sns_type, &vi_cfg->dev_info.dev_attr);
    vi_cfg->dev_info.dev_attr.mipirx_dev_no = vi_cfg->mipi_info.mipi_attr.mipi_attr.devno;

    /* pipe info */
    sample_comm_vi_get_default_pipe_info(sns_type, &vi_cfg->pipe_info[0], vi_cfg->sns_info.bus_id);

    vi_cfg->pipe_num = 1;
    vi_cfg->pipe_info[0].chn_info.chn_id = 0;
    sample_comm_vi_get_default_chn_info(sns_type, &vi_cfg->pipe_info[0].chn_info.chn_attr);
}

ot_void sample_comm_vi_get_default_vi_cfg_by_dev_id_and_sns_num(
    sample_sns_type sns_type, sample_vi_cfg *vi_cfg, ot_s32 dev_id, ot_u8 sns_nums)
{
    ot_u32 i;
    (ot_void)memset_s(vi_cfg, sizeof(sample_vi_cfg), 0, sizeof(sample_vi_cfg));
    /* sensor info */
    sample_comm_vi_get_default_sns_info(sns_type, dev_id, &vi_cfg->sns_info);
    /* mipi info */
    sample_comm_vi_get_mipi_info_by_dev_id(sns_type, dev_id, &vi_cfg->mipi_info);
    vi_cfg->mipi_info.mipi_attr.mipi_attr.sns_num = sns_nums;

    /* serdes info */
    sample_comm_vi_get_default_serdes_info(sns_type, dev_id, sns_nums, &vi_cfg->serdes_info);

    /* dev info */
    sample_comm_vi_get_default_dev_info(sns_type, &vi_cfg->dev_info.dev_attr);
    vi_cfg->dev_info.dev_attr.mipirx_dev_no = vi_cfg->mipi_info.mipi_attr.mipi_attr.devno;

    /* pipe info */
    vi_cfg->pipe_num = sns_nums;
    for (i = 0; i < vi_cfg->pipe_num; i++) {
        sample_comm_vi_get_default_pipe_info(sns_type, &(vi_cfg->pipe_info[i]), vi_cfg->sns_info.bus_id);
        sample_comm_vi_get_default_chn_info(sns_type, &(vi_cfg->pipe_info[i]).chn_info.chn_attr);
        vi_cfg->pipe_info[i].pipe_attr.bus_info.bus_addr =
            vi_cfg->serdes_info.serdes_attr.multi_port_attr[i].sns_dst_addr;

        vi_cfg->pipe_info[i].chn_info.chn_id = 0;
    }
    if (sns_nums == SNS_NUM_2) {
        sample_comm_vi_set_dev_two_sensor_pipe_vc_id(sns_type, vi_cfg);
    }
}

static ot_u32 g_mipi_init = 0;
static ot_s32 sample_comm_vi_start_mipi_rx(const sample_mipi_info *mipi_info, ot_eis_handle *mipi_hdl)
{
    ot_s32 ret;
    ot_eis_handle vi_intf_hdl;
    if (g_mipi_init == 0) {
        ret = ot_eis_vi_if_init(&mipi_info->init_attr);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vi_if_init ret err: %08x\n", ret);
            goto IF_FAILED;
        }
    }
    g_mipi_init++;

    ret = ot_eis_vi_if_create(&vi_intf_hdl, &mipi_info->mipi_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_if_create ret err: %08x\n", ret);
        goto IF_DEINIT;
    }

    ret = ot_eis_vi_if_set_ext_attr(vi_intf_hdl, &mipi_info->mipi_ext_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_if_set_ext_attr ret err: %08x\n", ret);
        goto IF_DESTROY;
    }

    ret = ot_eis_vi_if_start(vi_intf_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_if_start ret err: %08x\n", ret);
        goto IF_DESTROY;
    }

    *mipi_hdl = vi_intf_hdl;
    return OT_SUCCESS;

IF_DESTROY:
    ot_eis_vi_if_destroy(vi_intf_hdl);

IF_DEINIT:
    g_mipi_init--;
    if (g_mipi_init == 0) {
        ot_eis_vi_if_deinit();
    }

IF_FAILED:
    return OT_FAILURE;
}

ot_s32 sample_comm_vi_stop_mipi_rx(ot_eis_handle mipi_hdl)
{
    ot_s32 ret = ot_eis_vi_if_stop(mipi_hdl);
    if(ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_if_stop ret err: %08x\n", ret);
        return OT_FAILURE;
    }

    ret = ot_eis_vi_if_destroy(mipi_hdl);
    if(ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_if_destroy ret err: %08x\n", ret);
    }
    g_mipi_init--;
    if (g_mipi_init == 0) {
        ret = ot_eis_vi_if_deinit();
        if(ret != OT_SUCCESS) {
            sample_print("ot_eis_vi_if_deinit ret err: %08x\n", ret);
            return OT_FAILURE;
        }
    }
    return OT_SUCCESS;
}
static ot_s32 sample_comm_vi_start_sensor(sample_sns_info *sns_info, ot_bool quickboot_en)
{
    ot_s32 ret;
    if (quickboot_en == OT_TRUE) {
        return OT_SUCCESS;
    }
    ret = ot_eis_vi_if_sensor_clk_enable(sns_info->sns_clk_src, OT_TRUE);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_sensor_clk_enable ret err: %08x\n", ret);
        return OT_FAILURE;
    }

    ret = ot_eis_vi_if_sensor_enable(sns_info->sns_rst_src, OT_TRUE);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_sensor_enable ret err: %08x\n", ret);
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}
static ot_s32 sample_comm_vi_stop_sensor(const sample_sns_info *sns_info, ot_bool quickboot_en)
{
    ot_s32 ret;
    if (quickboot_en == OT_TRUE) {
        return OT_SUCCESS;
    }
    ret = ot_eis_vi_if_sensor_enable(sns_info->sns_rst_src, OT_FALSE);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_sensor_enable ret err: %08x\n", ret);
        return OT_FAILURE;
    }

    ret = ot_eis_vi_if_sensor_clk_enable(sns_info->sns_clk_src, OT_FALSE);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_sensor_clk_enable ret err: %08x\n", ret);
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_vi_set_quickboot_param(ot_eis_handle pipe_hdl, ot_bool quickboot_en)
{
    ot_s32 ret;
    ot_eis_vi_quickboot_attr quickboot = { 0 };
    quickboot.enable = quickboot_en;
    ret = ot_eis_vi_pipe_set_quickboot_attr(pipe_hdl, &quickboot);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_pipe_set_quickboot_attr failed with %#x!\n", ret);
        return ret;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_comm_vi_set_header_sns_param(ot_eis_handle pipe_hdl, sample_sns_info *sns_info)
{
    ot_s32 ret;
    ot_eis_sns_obj *sns_obj;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] =  { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    ot_eis_vi_sns_alg_default  sns_alg_param;
    ot_eis_vi_awb_sensor_default   sns_awb_param;
    ot_eis_sns_reg_table  sns_table_param[5];
    ot_sns_set_param *sns_param_obj;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return OT_FAILURE;
    }
    master_pipe_id = vi_pipe[0];

    switch (sns_info->sns_type) {
        case SONY_IMX623_MIPI_3M_60FPS_16BIT:
            sns_param_obj = &g_imx623_tuning_obj;
            break;
        case OV_OX03C_MIPI_3M_60FPS_16BIT:
            sns_param_obj = &g_ox03c_tuning_obj;
            break;
        case OV_OX03F10_MIPI_3M_60FPS_16BIT:
            sns_param_obj = &g_ox03f_tuning_obj;
            break;
        case OV_OX08B40_MIPI_8M_30FPS_12BIT:
            sns_param_obj = &g_ox08b40_tuning_obj;
            break;
        case SMS_SC220AT_MIPI_2M_30FPS_RAW12:
        case SMS_SC220AT_MIPI_2M_30FPS_YUV422:
            sns_param_obj = &g_sc220at_tuning_obj;
            break;
        default :
            printf("no sns param file return\n");
            return OT_FAILURE;
            break;
    }

    sns_param_obj->set_alg(master_pipe_id, &sns_alg_param);
    sns_param_obj->set_awb(master_pipe_id, &sns_awb_param);
    sns_param_obj->set_sensor_table(master_pipe_id, sns_table_param);

    sns_obj = sample_comm_isp_get_sns_obj(sns_info->sns_type);
    if (sns_obj == OT_NULL) {
        printf("sensor %d not exist!\n", sns_info->sns_type);
        return OT_FAILURE;
    }

    if ( (sns_obj->pfn_sns_set_alg_default == OT_NULL) || (sns_obj->pfn_sns_set_awb_default == OT_NULL) ||
        (sns_obj->pfn_sns_set_reg_table == OT_NULL) ) {
        printf("sensor param  set func not exist!\n");
        return OT_FAILURE;
    }

    ret = sns_obj->pfn_sns_set_alg_default(pipe_hdl, &sns_alg_param);
    if (ret != OT_SUCCESS) {
        printf("pfn_sns_set_alg_default failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    ret = sns_obj->pfn_sns_set_awb_default(pipe_hdl, &sns_awb_param);
    if (ret != OT_SUCCESS) {
        printf("pfn_sns_set_alg_default failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    ret = sns_obj->pfn_sns_set_reg_table(pipe_hdl,sns_table_param,5);
    if (ret != OT_SUCCESS) {
        printf("pfn_sns_set_reg_table failed with %#x!\n", ret);
       return OT_FAILURE;
    }
    printf("sensor param use header\n");
    return OT_SUCCESS;
}

#ifdef USE_JSON_FILE
static ot_s32 sample_comm_vi_set_json_sns_param(ot_eis_handle pipe_hdl, sample_sns_info *sns_info)
{
    ot_s32 ret;
    ot_eis_sns_obj *sns_obj;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] =  { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;
    char sns_alg_def_file[JSON_FILE_LENGTH];
    char sns_awb_def_file[JSON_FILE_LENGTH];
    char sns_table_def_file[JSON_FILE_LENGTH];
    ot_eis_vi_sns_alg_default  sns_alg_def;
    ot_eis_vi_awb_sensor_default   sns_ext_awb;
    ot_eis_sns_reg_table  ext_sns_table[5];

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    master_pipe_id = vi_pipe[0];

    ret = sample_comm_get_tuning_file(master_pipe_id, sns_alg_def_file, sns_awb_def_file, sns_table_def_file);
    if (ret != OT_SUCCESS) {
        printf("sensor tuning file not exist!\n");
        return OT_FAILURE;
    }

    sns_obj = sample_comm_isp_get_sns_obj(sns_info->sns_type);
    if (sns_obj == OT_NULL) {
        printf("sensor %d not exist!\n", sns_info->sns_type);
        return OT_FAILURE;
    }

    if ( (sns_obj->pfn_sns_set_alg_default == OT_NULL) || (sns_obj->pfn_sns_set_awb_default == OT_NULL) ||
        (sns_obj->pfn_sns_set_reg_table == OT_NULL) ) {
        printf("sensor param  set func not exist!\n");
        return OT_FAILURE;
    }

    ret = sample_comm_parse_alg_param(sns_alg_def_file, &sns_alg_def,master_pipe_id);
    if(ret != OT_SUCCESS) {
        printf("sample_comm_parse_alg_param failed \n");
        return OT_FAILURE;
    }
    ret = sns_obj->pfn_sns_set_alg_default(pipe_hdl, &sns_alg_def);
    if (ret != OT_SUCCESS) {
        printf("pfn_sns_set_alg_default failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    ret = sample_comm_parse_awb_param(sns_awb_def_file, &sns_ext_awb);
    if(ret != OT_SUCCESS) {
        printf("sample_comm_parse_awb_param failed \n");
        return OT_FAILURE;
    }
    ret = sns_obj->pfn_sns_set_awb_default(pipe_hdl, &sns_ext_awb);
    if (ret != OT_SUCCESS) {
        printf("pfn_sns_set_alg_default failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    ret = sample_comm_parse_sensor_table_param(sns_table_def_file ,ext_sns_table, master_pipe_id);
    if(ret != OT_SUCCESS) {
        printf("sample_comm_parse_sensor_table_param failed \n");
        return OT_FAILURE;
    }
    ret = sns_obj->pfn_sns_set_reg_table(pipe_hdl,ext_sns_table,5);
    if (ret != OT_SUCCESS) {
        sample_comm_vi_json_sensor_table_free(master_pipe_id);
        printf("pfn_sns_set_reg_table failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    printf("pipe[%d] param use json: %s %s %s\n", master_pipe_id, sns_alg_def_file,
        sns_awb_def_file, sns_awb_def_file);
    return OT_SUCCESS;
}
#endif

static ot_s32 sample_comm_vi_start_isp(ot_eis_handle pipe_hdl, sample_vi_cfg *vi_cfg)
{
    ot_s32 ret;
    ot_eis_sns_obj *sns_obj;
    ot_vi_pipe_id vi_pipe[OT_EIS_VI_PIPE_ID_NUM_MAX] =  { 0 };
    ot_s32 pipe_num = 0;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, vi_pipe, &pipe_num);
    if (ret != OT_SUCCESS) {
        return ret;
    }
#ifdef USE_JSON_FILE
    ot_vi_pipe_id master_pipe_id = 0;
    master_pipe_id = vi_pipe[0];
#endif

    sns_obj = sample_comm_isp_get_sns_obj(vi_cfg->sns_info.sns_type);
    if (sns_obj == OT_NULL) {
        printf("sensor %d not exist!\n", vi_cfg->sns_info.sns_type);
        return OT_FAILURE;
    }
#ifdef USE_JSON_FILE
    ret = sample_comm_vi_set_json_sns_param(pipe_hdl,&vi_cfg->sns_info);
    if (ret != OT_SUCCESS) {
        printf("ot_sns_ext_json_parser_failed 0x%x!\n", ret);
    }
#else
    ret = sample_comm_vi_set_header_sns_param(pipe_hdl,&vi_cfg->sns_info);
    if (ret != OT_SUCCESS) {
        printf("ot_sns_ext_header_parser_failed 0x%x!\n", ret);
    }
#endif

    ret = ot_eis_vi_sensor_register(pipe_hdl, sns_obj);
    if (ret != OT_SUCCESS) {
#ifdef USE_JSON_FILE
        sample_comm_vi_json_sensor_table_free(master_pipe_id);
#endif
        printf("register sensor to vi failed\n");
        return OT_FAILURE;
    }

    ret = ot_eis_vi_ae_register(pipe_hdl);
    if (ret != OT_SUCCESS) {
        printf("ot_eis_vi_ae_register failed with %#x!\n", ret);
        goto sns_unregister;
    }

    ret = ot_eis_vi_awb_register(pipe_hdl);
    if (ret != OT_SUCCESS) {
        printf("ot_media_vi_awb_register failed with %#x!\n", ret);
        goto ae_unregister;
    }

    ret = sample_comm_vi_set_quickboot_param(pipe_hdl, vi_cfg->pipe_info[0].quickboot_en);
    if (ret != OT_SUCCESS) {
        goto awb_unregister;
    }

    ret = ot_eis_vi_pipe_init(pipe_hdl);
    if (ret != OT_SUCCESS) {
        printf("ot_media_vi_pipe_init failed with 0x%x!\n", ret);
        goto awb_unregister;
    }

    ret = ot_eis_vi_pipe_start(pipe_hdl);
    if (ret != OT_SUCCESS) {
        printf("ot_eis_vi_pipe_start failed with 0x%x!\n", ret);
        goto pipe_deinit;
    }
#ifdef USE_JSON_FILE
    sample_comm_vi_json_sensor_table_free(master_pipe_id);
#endif
    return OT_SUCCESS;

pipe_deinit:
    ot_eis_vi_pipe_deinit(pipe_hdl);
awb_unregister:
    ot_eis_vi_awb_unregister(pipe_hdl);
ae_unregister:
    ot_eis_vi_ae_unregister(pipe_hdl);
sns_unregister:
#ifdef USE_JSON_FILE
    sample_comm_vi_json_sensor_table_free(master_pipe_id);
#endif
    ot_eis_vi_sensor_unregister(pipe_hdl);
    return ret;
}

static ot_void sample_comm_vi_stop_isp(ot_vi_pipe_handle   pipe_hdl)
{
    ot_eis_vi_pipe_stop(pipe_hdl);
    ot_eis_vi_pipe_deinit(pipe_hdl);
    ot_eis_vi_awb_unregister(pipe_hdl);
    ot_eis_vi_ae_unregister(pipe_hdl);
    ot_eis_vi_sensor_unregister(pipe_hdl);
}

static ot_s32 sample_comm_vi_set_3dnr_attr(ot_vi_pipe_handle pipe_hdl, sample_vi_cfg *vi_cfg)
{
    ot_s32 ret;
    ot_eis_3dnr_attr nr_attr = { 0 };
    ot_eis_3dnr_pos pos = { 0 };
    if (vi_cfg->pipe_info[0].nr_attr.enable == OT_FALSE) {
        return OT_SUCCESS;
    }

    ret = ot_eis_media_get_3dnr_pos(&pos);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_media_get_3dnr_pos failed!\n");
        return ret;
    }

    if (pos != OT_EIS_3DNR_POS_VI) {
        return OT_SUCCESS;
    }

    ret = ot_eis_vi_pipe_get_3dnr_attr(pipe_hdl, &nr_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_pipe_get_3dnr_attr failed!\n");
        return ret;
    }

    nr_attr.enable  = vi_cfg->pipe_info[0].nr_attr.enable;
    nr_attr.nr_type = vi_cfg->pipe_info[0].nr_attr.nr_type;

    ret = ot_eis_vi_pipe_set_3dnr_attr(pipe_hdl, &nr_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_pipe_set_3dnr_attr failed!\n");
        return ret;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_comm_start_vi_serdes(sample_vi_serdes_info *serdes_info)
{
    ot_s32 ret = OT_SUCCESS;

    ret = ot_eis_vi_serdes_init();
    if (ret != OT_SUCCESS) {
        sample_print("vi serdes init failed.\n");
        return ret;
    }

    ret = ot_eis_vi_serdes_create(&(serdes_info->serdes_hdl), "MAX9295,MAX9296", &(serdes_info->serdes_attr));
    if (ret != OT_SUCCESS) {
        ot_eis_vi_serdes_deinit();
        sample_print("vi serdes create failed.\n");
        return ret;
    }

    ret = ot_eis_vi_serdes_start(serdes_info->serdes_hdl);
    if (ret != OT_SUCCESS) {
        ot_eis_vi_serdes_destroy(serdes_info->serdes_hdl);
        ot_eis_vi_serdes_deinit();
        sample_print("vi serdes start failed.\n");
        return ret;
    }

    return ret;
}

static ot_s32 sample_comm_stop_vi_serdes(const sample_vi_serdes_info *serdes_info)
{
    ot_s32 ret = OT_SUCCESS;
    ret |= ot_eis_vi_serdes_stop(serdes_info->serdes_hdl);
    ret |= ot_eis_vi_serdes_destroy(serdes_info->serdes_hdl);
    ret |= ot_eis_vi_serdes_deinit();
    return ret;
}

ot_s32 sample_comm_vi_start_sensor_mipirx(sample_vi_cfg *vi_cfg)
{
    ot_s32 ret;
    ot_eis_handle mipi_hdl;
    ret = sample_comm_vi_start_mipi_rx(&vi_cfg->mipi_info, &mipi_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("start mipi rx failed!\n");
        goto start_mipi_rx_failed;
    }
    vi_cfg->mipi_hdl = mipi_hdl;

    ret = sample_comm_vi_start_sensor(&vi_cfg->sns_info, vi_cfg->pipe_info[0].quickboot_en);
    if (ret != OT_SUCCESS) {
        sample_print("start mipi rx failed!\n");
        goto stop_mipi_rx;
    }

    ot_vi_dev_handle dev_hdl;
    ret = ot_eis_vi_dev_create(&dev_hdl, &vi_cfg->dev_info.dev_attr);
    if (ret != OT_SUCCESS) {
        sample_print("start dev failed!\n");
        goto stop_sensor;
    }
    vi_cfg->dev_info.dev_hdl = dev_hdl;

    if (vi_cfg->pipe_info[0].quickboot_en == OT_FALSE) {
        ret = sample_comm_start_vi_serdes(&vi_cfg->serdes_info);
        if (ret != OT_SUCCESS) {
            sample_print("start serdes failed!\n");
            goto stop_sensor;
        }
    }

    return OT_SUCCESS;

stop_sensor:
    sample_comm_vi_stop_sensor(&vi_cfg->sns_info, vi_cfg->pipe_info[0].quickboot_en);
stop_mipi_rx:
    sample_comm_vi_stop_mipi_rx(mipi_hdl);
start_mipi_rx_failed:
    return OT_FAILURE;
}

ot_s32 sample_comm_vi_start_chn_dev(sample_vi_cfg *vi_cfg)
{
    ot_s32 ret;
    ot_vi_dev_handle dev_hdl;

    ot_vi_pipe_handle pipe_hdl = NULL;
    ret = ot_eis_vi_pipe_create(vi_cfg->pipe_info->pipe_id, &pipe_hdl, &vi_cfg->pipe_info[0].pipe_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_pipe_create failed!\n");
        goto dev_destroy;
    }
    vi_cfg->pipe_info[0].pipe_hdl = pipe_hdl;

    ot_vi_chn_handle chn_hdl = NULL;
    ret = ot_eis_vi_chn_create(vi_cfg->pipe_info[0].chn_info.chn_id, &chn_hdl, &vi_cfg->pipe_info[0].chn_info.chn_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_chn_create failed!\n");
        goto pipe_destroy;
    }
    vi_cfg->pipe_info[0].chn_info.chn_hdl = chn_hdl;

    dev_hdl = vi_cfg->dev_info.dev_hdl;
    ret = ot_eis_vi_pipe_attach_dev(pipe_hdl, dev_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_pipe_attach_dev failed!\n");
        goto chn_destroy;
    }

    ret = ot_eis_vi_pipe_attach_chn(pipe_hdl, vi_cfg->pipe_info[0].chn_info.chn_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_pipe_attach_chn failed!\n");
        goto pipe_detach_dev;
    }

    ret = sample_comm_vi_set_3dnr_attr(pipe_hdl, vi_cfg);
    if (ret != OT_SUCCESS) {
        goto pipe_detach_chn;
    }

    ret = sample_comm_vi_start_isp(pipe_hdl, vi_cfg);
    if (ret != OT_SUCCESS) {
        sample_print("start isp failed!\n");
        goto pipe_detach_chn;
    }

    return OT_SUCCESS;

pipe_detach_chn:
    ot_eis_vi_pipe_detach_chn(pipe_hdl, vi_cfg->pipe_info[0].chn_info.chn_hdl);
pipe_detach_dev:
    ot_eis_vi_pipe_detach_dev(pipe_hdl, dev_hdl);
chn_destroy:
    ot_eis_vi_chn_destroy(vi_cfg->pipe_info[0].chn_info.chn_hdl);
pipe_destroy:
    ot_eis_vi_pipe_destroy(pipe_hdl);
dev_destroy:
    ot_eis_vi_dev_destroy(dev_hdl);
    return OT_FAILURE;
}

ot_void sample_comm_vi_stop_sensor_mipirx(const sample_vi_cfg *vi_cfg)
{
    ot_eis_vi_dev_destroy(vi_cfg->dev_info.dev_hdl);

    sample_comm_vi_stop_sensor(&vi_cfg->sns_info, vi_cfg->pipe_info[0].quickboot_en);
    sample_comm_vi_stop_mipi_rx(vi_cfg->mipi_hdl);
}

ot_void sample_comm_vi_stop_chn_dev(const sample_vi_cfg *vi_cfg)
{
    sample_comm_vi_stop_isp(vi_cfg->pipe_info[0].pipe_hdl);
    sample_comm_stop_vi_serdes(&(vi_cfg->serdes_info));
    ot_eis_vi_pipe_detach_chn(vi_cfg->pipe_info[0].pipe_hdl, vi_cfg->pipe_info[0].chn_info.chn_hdl);
    ot_eis_vi_pipe_detach_dev(vi_cfg->pipe_info[0].pipe_hdl, vi_cfg->dev_info.dev_hdl);
    ot_eis_vi_chn_destroy(vi_cfg->pipe_info[0].chn_info.chn_hdl);

    ot_eis_vi_pipe_destroy(vi_cfg->pipe_info[0].pipe_hdl);
}
ot_s32 sample_comm_create_attach_pipe_chn_by_pipe_id(sample_vi_cfg *vi_cfg, sample_vi_pipe_info *pipe_info)
{
    ot_s32 ret;
    ot_vi_pipe_handle pipe_hdl;
    ot_vi_chn_handle chn_hdl;
    ot_vi_dev_handle dev_hdl = vi_cfg->dev_info.dev_hdl;

    ret = ot_eis_vi_pipe_create(pipe_info->pipe_id, &pipe_hdl, &pipe_info->pipe_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_pipe_create failed!\n");
        return OT_FAILURE;
    }

    pipe_info->pipe_hdl = pipe_hdl;

    ret = ot_eis_vi_chn_create(pipe_info->chn_info.chn_id,
        &chn_hdl, &pipe_info->chn_info.chn_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_chn_create failed!\n");
        goto pipe_destroy;
    }
    pipe_info->chn_info.chn_hdl = chn_hdl;

    ret = ot_eis_vi_pipe_attach_dev(pipe_hdl, dev_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_pipe_attach_dev failed!\n");
        goto chn_destroy;
    }

    ret = ot_eis_vi_pipe_attach_chn(pipe_hdl, pipe_info->chn_info.chn_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vi_pipe_attach_chn failed!\n");
        goto pipe_detach_dev;
    }

    ret = sample_comm_vi_set_3dnr_attr(pipe_hdl, vi_cfg);
    if (ret != OT_SUCCESS) {
        sample_print("sample_comm_vi_set_3dnr_attr failed!\n");
        goto pipe_detach_chn;
    }

    ret = sample_comm_vi_start_isp(pipe_hdl, vi_cfg);
    if (ret != OT_SUCCESS) {
        sample_print("start isp failed!\n");
        goto pipe_detach_chn;
    }

    return OT_SUCCESS;

pipe_detach_chn:
    ot_eis_vi_pipe_detach_chn(pipe_hdl, pipe_info->chn_info.chn_hdl);
pipe_detach_dev:
    ot_eis_vi_pipe_detach_dev(pipe_hdl, dev_hdl);
chn_destroy:
    ot_eis_vi_chn_destroy(pipe_info->chn_info.chn_hdl);
pipe_destroy:
    ot_eis_vi_pipe_destroy(pipe_hdl);

    return OT_FAILURE;
}

ot_void sample_comm_detach_destroy_pipe_chn_by_pipe_id(const sample_vi_cfg *vi_cfg, ot_s32 pipe_id)
{
    sample_comm_vi_stop_isp(vi_cfg->pipe_info[pipe_id].pipe_hdl);
    ot_eis_vi_pipe_detach_chn(vi_cfg->pipe_info[pipe_id].pipe_hdl, vi_cfg->pipe_info[pipe_id].chn_info.chn_hdl);
    ot_eis_vi_pipe_detach_dev(vi_cfg->pipe_info[pipe_id].pipe_hdl, vi_cfg->dev_info.dev_hdl);
    ot_eis_vi_chn_destroy(vi_cfg->pipe_info[pipe_id].chn_info.chn_hdl);
    ot_eis_vi_pipe_destroy(vi_cfg->pipe_info[pipe_id].pipe_hdl);
}


ot_s32 sample_comm_vi_start_vi(sample_vi_cfg *vi_cfg)
{
    ot_s32 ret, i, j;
    ot_eis_handle mipi_hdl;

    ret = sample_comm_vi_start_mipi_rx(&vi_cfg->mipi_info, &mipi_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("start mipi rx failed!\n");
        goto start_mipi_rx_failed;
    }
    vi_cfg->mipi_hdl = mipi_hdl;

    ret = sample_comm_vi_start_sensor(&vi_cfg->sns_info, vi_cfg->pipe_info[0].quickboot_en);
    if (ret != OT_SUCCESS) {
        sample_print("start mipi rx failed!\n");
        goto stop_mipi_rx;
    }

    if (vi_cfg->pipe_info[0].quickboot_en == OT_FALSE) {
        ret = sample_comm_start_vi_serdes(&vi_cfg->serdes_info);
        if (ret != OT_SUCCESS) {
            sample_print("start serdes failed!\n");
            goto stop_mipi_rx;
        }
    }

    ot_vi_dev_handle dev_hdl;
    ret = ot_eis_vi_dev_create(&dev_hdl, &vi_cfg->dev_info.dev_attr);
    if (ret != OT_SUCCESS) {
        sample_print("start dev failed!\n");
        goto stop_sensor;
    }
    vi_cfg->dev_info.dev_hdl = dev_hdl;

    for (i = 0; i < vi_cfg->pipe_num; i++) {
        ret = sample_comm_create_attach_pipe_chn_by_pipe_id(vi_cfg, &vi_cfg->pipe_info[i]);
        if (ret != OT_SUCCESS) {
            sample_print("sample_comm_create_attach_pipe_chn_by_pipe_id failed!\n");
            goto pipe_chn_detach_destory;
        }
    }

    return OT_SUCCESS;

pipe_chn_detach_destory:
    for (j = i - 1; j >= 0; j--) {
        sample_comm_detach_destroy_pipe_chn_by_pipe_id(vi_cfg, j);
    }
    ot_eis_vi_dev_destroy(dev_hdl);
stop_sensor:
    sample_comm_vi_stop_sensor(&vi_cfg->sns_info, vi_cfg->pipe_info[0].quickboot_en);
stop_mipi_rx:
    sample_comm_vi_stop_mipi_rx(mipi_hdl);
start_mipi_rx_failed:
    return OT_FAILURE;
}

ot_void sample_comm_vi_stop_vi(const sample_vi_cfg *vi_cfg)
{
    ot_s32 i;
    for (i = 0; i < vi_cfg[0].pipe_num; i++) {
        sample_comm_detach_destroy_pipe_chn_by_pipe_id(vi_cfg, i);
    }

    ot_eis_vi_dev_destroy(vi_cfg->dev_info.dev_hdl);

    if (vi_cfg->pipe_info[0].quickboot_en == OT_FALSE) {
        sample_comm_stop_vi_serdes(&(vi_cfg->serdes_info));
    }
    sample_comm_vi_stop_sensor(&vi_cfg->sns_info, vi_cfg->pipe_info[0].quickboot_en);
    sample_comm_vi_stop_mipi_rx(vi_cfg->mipi_hdl);
}

ot_s32 sample_comm_vi_restart(sample_vi_cfg *vi_cfg)
{
    ot_s32 i;
    ot_vi_pipe_handle pipe_hdl;
    ot_vi_chn_handle chn_hdl;

    for (i = 0; i < vi_cfg->pipe_num; i++) {
        pipe_hdl = vi_cfg->pipe_info[i].pipe_hdl;
        chn_hdl = vi_cfg->pipe_info[i].chn_info.chn_hdl;

        ot_eis_vi_pipe_attach_chn(pipe_hdl, chn_hdl);
        ot_eis_vi_pipe_init(pipe_hdl);
        ot_eis_vi_pipe_start(pipe_hdl);
    }

    return OT_SUCCESS;
}

ot_void sample_comm_vi_part_stop(sample_vi_cfg *vi_cfg)
{
    ot_s32 i;
    ot_vi_pipe_handle pipe_hdl;
    ot_vi_chn_handle chn_hdl;

    for (i = 0; i < vi_cfg[0].pipe_num; i++) {
        pipe_hdl = vi_cfg->pipe_info[i].pipe_hdl;
        chn_hdl = vi_cfg->pipe_info[i].chn_info.chn_hdl;

        ot_eis_vi_pipe_stop(pipe_hdl);
        ot_eis_vi_pipe_deinit(pipe_hdl);
        ot_eis_vi_pipe_detach_chn(pipe_hdl, chn_hdl);
    }
}

ot_s32 sample_vi_sys_init(ot_eis_img_size *img_size, ot_eis_vi_vproc_mode_type mode_type)
{
    ot_s32 ret;
    ot_video_buffer_attr vrb_cfg;

    sample_common_get_vrb_cfg(3, img_size->width, img_size->height, &vrb_cfg);
    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    if (sample_comm_sys_set_vi_vproc_init_cfg(mode_type) != OT_SUCCESS) {
        goto sys_exit;
    }

    return OT_SUCCESS;
sys_exit:
    sample_comm_sys_exit();
    return OT_FAILURE;
}

ot_s32 sample_comm_vi_start_route(sample_vi_cfg vi_cfg[], sample_vproc_attr vproc_attr[], ot_u32 route_num,
                                  ot_eis_vi_vproc_mode_type mode_type, ot_u32 display_num)
{
    ot_s32 ret;
    ot_s32 j, i0, i1, i2, i3;
    sample_sns_type sns_type = vi_cfg[0].sns_info.sns_type;
    ot_eis_handle media_pipe_hdl;
    ot_bool pipe_sw[OT_EIS_VPROC_GRP_PIPE_MAX_NUM] = {OT_TRUE, OT_FALSE, OT_FALSE, OT_FALSE};
    ot_bool chn_sw[OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {OT_TRUE, OT_FALSE, OT_FALSE, OT_FALSE};
    ot_eis_img_size img_size = { 0 };
    sample_vo_attr *vo_attr = OT_NULL;

    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    vo_attr = sample_comm_args_get_vo_attr();
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return OT_FAILURE;
    }

    if (sample_vi_sys_init(&img_size, mode_type) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    if (vi_cfg[0].pipe_num == 1) {
        for (i0 = 0; i0 < route_num; i0++) {
            ret = sample_comm_vi_start_vi(&vi_cfg[i0]);
            if (ret != OT_SUCCESS) {
                goto start_vi_failed;
            }
        }
    } else {
        ret = sample_comm_vi_start_vi(&vi_cfg[0]);
        if (ret != OT_SUCCESS) {
            goto start_vi_failed;
        }
    }

    // vproc
    for (i1 = 0; i1 < route_num; i1++) {
        ret = sample_comm_start_vproc(&vproc_attr[i1], pipe_sw, chn_sw);
        if (ret != OT_SUCCESS) {
            goto start_vproc_failed;
        }
    }

    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_comm_media_pipe_init(&media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto pipe_start_failed;
    }

    if (vi_cfg[0].pipe_num == 1) {
        for (i2 = 0; i2 < route_num; i2++) {
            ret = sample_comm_vi_bind_vproc(vi_cfg[i2].pipe_info[0].chn_info.chn_hdl, vproc_attr[i2].pipe_hdl[0], media_pipe_hdl);
            if (ret != OT_SUCCESS) {
                goto vi_vproc_bind_failed;
            }
        }
    } else {
        for (i2 = 0; i2 < vi_cfg[0].pipe_num; i2++) {
            ret = sample_comm_vi_bind_vproc(vi_cfg[0].pipe_info[i2].chn_info.chn_hdl, vproc_attr[i2].pipe_hdl[0], media_pipe_hdl);
            if (ret != OT_SUCCESS) {
                goto vi_vproc_bind_failed;
            }
        }
    }

    for (i3 = 0; i3 < route_num; i3++) {
        if (vo_attr->in_port_enable[i3] == OT_TRUE) {
            ret = sample_comm_vproc_bind_vo(vproc_attr[i3].chn_hdl[0], vo_attr->in_port_handle[i3], media_pipe_hdl);
            if (ret != OT_SUCCESS) {
                goto vproc_bind_vo_failed;
            }
        }
    }

    vi_cfg[0].media_pipe_hdl = media_pipe_hdl;

    return OT_SUCCESS;

vproc_bind_vo_failed:
    for (j = i3 - 1; j >= 0; j--) {
        sample_comm_vproc_un_bind_vo(vproc_attr[j].chn_hdl[0], vo_attr->in_port_handle[j], media_pipe_hdl);
    }
vi_vproc_bind_failed:
    if (vi_cfg[0].pipe_num == 1) {
        for (j = i2 - 1; j >= 0; j--) {
            sample_comm_vi_un_bind_vproc(vi_cfg[j].pipe_info[0].chn_info.chn_hdl, vproc_attr[j].pipe_hdl[0], media_pipe_hdl);
        }
    } else {
        for (j = i2 - 1; j >= 0; j--) {
            sample_comm_vi_un_bind_vproc(vi_cfg[0].pipe_info[j].chn_info.chn_hdl, vproc_attr[j].pipe_hdl[0], media_pipe_hdl);
        }
    }
    sample_comm_media_pipe_stop(media_pipe_hdl);
pipe_start_failed:
    sample_comm_stop_vo(vo_attr);
start_vo_failed:

start_vproc_failed:
    for (j = i1 - 1; j >= 0; j--) {
        sample_comm_stop_vproc(&vproc_attr[j]);
    }
start_vi_failed:
    if (vi_cfg[0].pipe_num == 1) {
        for (j = i0 - 1; j >= 0; j--) {
            sample_comm_vi_stop_vi(&vi_cfg[j]);
        }
    } else {
        sample_comm_vi_stop_vi(&vi_cfg[0]);
    }
    sample_comm_sys_exit();

    return OT_FAILURE;
}

ot_void sample_comm_vi_stop_route(sample_vi_cfg vi_cfg[], sample_vproc_attr vproc_attr[], ot_s32 route_num, ot_u32 display_num)
{
    ot_s32 i;
    ot_eis_handle media_pipe_hdl;
    sample_vo_attr *vo_attr = OT_NULL;
    vo_attr = sample_comm_args_get_vo_attr();
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return;
    }

    media_pipe_hdl = vi_cfg[0].media_pipe_hdl;
    for (i = 0; i < route_num; i++) {
        sample_comm_vproc_un_bind_vo(vproc_attr[i].chn_hdl[0], vo_attr->in_port_handle[i], media_pipe_hdl);
    }

    if (vi_cfg[0].pipe_num == 1) {
        for (i = 0; i < route_num; i++) {
            sample_comm_vi_un_bind_vproc(vi_cfg[i].pipe_info[0].chn_info.chn_hdl, vproc_attr[i].pipe_hdl[0], media_pipe_hdl);
        }
    } else {
        for (i = 0; i < vi_cfg[0].pipe_num; i++) {
            sample_comm_vi_un_bind_vproc(vi_cfg[0].pipe_info[i].chn_info.chn_hdl, vproc_attr[i].pipe_hdl[0], media_pipe_hdl);
        }
    }

    sample_comm_media_pipe_stop(media_pipe_hdl);
    sample_comm_stop_vo(vo_attr);
    for (i = 0; i < route_num; i++) {
        sample_comm_stop_vproc(&vproc_attr[i]);
    }

    if (vi_cfg[0].pipe_num == 1) {
        for (i = 0; i < route_num; i++) {
            sample_comm_vi_stop_vi(&vi_cfg[i]);
        }
    } else {
        sample_comm_vi_stop_vi(&vi_cfg[0]);
    }
    sample_comm_sys_exit();
}

static ot_s32 sample_comm_vi_malloc_frame_blk(ot_eis_handle pool_hdl, ot_eis_img_attr *img_attr,
                                              ot_eis_buf_size_calc_cfg *calc_cfg, sample_vi_user_frame_info *user_frame_info)
{
    ot_s32 ret;
    ot_phys_addr phys_addr;
    ot_buffer_pool_blk blk;
    ot_void *virt_addr = OT_NULL;
    ot_eis_video_frame *frame_info = OT_NULL;

    ret = ot_buffer_pool_blk_acquire(pool_hdl, calc_cfg->blk_size, &blk);
    if (ret != OT_SUCCESS) {
        sample_print("ot_buffer_pool_blk_acquire err, size:%u\n", calc_cfg->blk_size);
        return OT_FAILURE;
    }

    phys_addr = blk.phys_addr;
    ret = ot_smr_mmap(phys_addr, calc_cfg->blk_size, OT_FALSE, &virt_addr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_smr_mmap err!\n");
        ot_buffer_pool_blk_release(pool_hdl, &blk);
        return OT_FAILURE;
    }

    memcpy_s(&user_frame_info->pool_blk, sizeof(ot_buffer_pool_blk), &blk, sizeof(ot_buffer_pool_blk));
    user_frame_info->blk_size = calc_cfg->blk_size;

    frame_info = &user_frame_info->frame_info;

    frame_info->pool_hdl           = pool_hdl;
    frame_info->mod_id             = OT_EIS_MODULE_VI;
    frame_info->buff.phys_addr[0]  = phys_addr;
    frame_info->buff.phys_addr[1]  = frame_info->buff.phys_addr[0] + calc_cfg->main_y_size;
    frame_info->buff.virt_addr[0]  = virt_addr;
    frame_info->buff.virt_addr[1]  = frame_info->buff.virt_addr[0] + calc_cfg->main_y_size;
    frame_info->buff.stride[0]     = calc_cfg->main_stride;
    frame_info->buff.stride[1]     = calc_cfg->main_stride;
    frame_info->attr.width         = img_attr->width;
    frame_info->attr.height        = img_attr->height;
    frame_info->attr.pixel_fmt     = img_attr->pixel_fmt;
    frame_info->attr.layout        = img_attr->layout;
    frame_info->attr.compress_mode = img_attr->compress_mode;
    frame_info->color_space        = OT_EIS_IMAGE_COLOR_SPACE_BT601;

    return OT_SUCCESS;
}

ot_void sample_comm_vi_free_frame_blk(ot_eis_handle pool_hdl, sample_vi_user_frame_info *user_frame_info)
{
    ot_s32 ret;
    ot_u32 blk_size = user_frame_info->blk_size;
    ot_void *virt_addr = user_frame_info->frame_info.buff.virt_addr[0];

    ret = ot_smr_munmap(virt_addr, blk_size);
    if (ret != OT_SUCCESS) {
        sample_print("ot_smr_munmap failure!\n");
    }

    ret = ot_buffer_pool_blk_release(pool_hdl, &user_frame_info->pool_blk);
    if (ret != OT_SUCCESS) {
        sample_print("ot_buffer_pool_blk_release block failure\n");
    }

    user_frame_info->pool_blk.phys_addr = 0;
    user_frame_info->pool_blk.block_size = 0;
}


ot_s32 sample_comm_vi_get_frame_blk(ot_eis_img_attr *img_attr,
                                    sample_vi_user_frame_info user_frame_info[], ot_s32 frame_cnt)
{
    ot_s32 ret;
    ot_s32 i;
    ot_eis_handle pool_hdl;
    ot_eis_buf_size_calc_cfg calc_cfg = { 0 };
    ot_video_buffer_attr pool_attr = { 0 };

    sample_common_get_pic_buf_cfg(img_attr, &calc_cfg);

    pool_attr.cnt = 1;
    pool_attr.buf_blks[0].cnt = frame_cnt;
    pool_attr.buf_blks[0].size = calc_cfg.blk_size;

    ret = ot_buffer_pool_init();
    if(ret != 0){
       printf("buffer_pool init fail! ret[%d]\n", ret);
       return OT_FAILURE;
    }

    ret =  ot_buffer_pool_create(&pool_hdl, &pool_attr);
    if(ret != 0){
       printf("buffer_pool create fail! ret[%d]\n", ret);
       goto pool_create_failed;
    }

    for (i = 0; i < frame_cnt; i++) {
        ret = sample_comm_vi_malloc_frame_blk(pool_hdl, img_attr, &calc_cfg, &user_frame_info[i]);
        if (ret != OT_SUCCESS) {
            goto exit;
        }
    }

    return OT_SUCCESS;

exit:
    for (i = i - 1; i >= 0; i--) {
        sample_comm_vi_free_frame_blk(pool_hdl, &user_frame_info[i]);
    }

    ot_buffer_pool_destroy(pool_hdl);

pool_create_failed:
    ot_buffer_pool_deinit();
    return ret;
}

ot_void sample_comm_vi_release_frame_blk(sample_vi_user_frame_info user_frame_info[], ot_s32 frame_cnt)
{
    ot_s32 i;
    ot_eis_handle pool_hdl = user_frame_info[0].frame_info.pool_hdl;

    for (i = 0; i < frame_cnt; i++) {
        sample_comm_vi_free_frame_blk(pool_hdl, &user_frame_info[i]);
    }

    ot_buffer_pool_destroy(pool_hdl);
    ot_buffer_pool_deinit();
}

static ot_s32 sample_comm_vi_get_fpn_frame_info(ot_vi_pipe_handle pipe_hdl, ot_eis_img_pixel_format pixel_format,
                                                sample_vi_user_frame_info *user_frame_info, ot_s32 blk_cnt)
{
    ot_s32 ret;
    ot_eis_img_attr img_attr = { 0 };
    ot_eis_vi_public_attr public_attr = { 0 };

    ret = ot_eis_vi_public_get_attr(pipe_hdl, &public_attr);
    if (ret != OT_SUCCESS) {
        sample_print("vi get public attr failed!\n");
        return ret;
    }

    img_attr.bit_depth     = OT_EIS_PIXEL_BIT_DEPTH_8;
    img_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE;
    img_attr.height        = public_attr.sns_size.height;
    img_attr.width         = public_attr.sns_size.width;
    img_attr.layout        = OT_EIS_IMAGE_LAYOUT_LINEAR;
    img_attr.pixel_fmt     = pixel_format;

    ret = sample_comm_vi_get_frame_blk(&img_attr, user_frame_info, blk_cnt);
    if (ret != OT_SUCCESS) {
        sample_print("get fpn frame vb failed!\n");
        return ret;
    }

    return OT_SUCCESS;
}

ot_u32 sample_comm_vi_get_raw_bit_width(ot_eis_img_pixel_format pixel_format)
{
    ot_u32 bit_width;

    switch (pixel_format) {
        case OT_EIS_IMAGE_FORMAT_BAYER_8BPP:
            bit_width = 8;
            break;
        case OT_EIS_IMAGE_FORMAT_BAYER_10BPP:
            bit_width = 10;
            break;
        case OT_EIS_IMAGE_FORMAT_BAYER_12BPP:
            bit_width = 12;
            break;
        case OT_EIS_IMAGE_FORMAT_BAYER_14BPP:
            bit_width = 14;
            break;
        case OT_EIS_IMAGE_FORMAT_BAYER_16BPP:
            bit_width = 16;
            break;
        default:
            bit_width = 8;
            break;
    }

    return bit_width;
}


static ot_s32 sample_comm_vi_get_fpn_file_name_iso(ot_eis_video_frame *video_frame, const ot_char *dir_name,
                                                   ot_char *file_name, ot_u32 length, ot_u32 iso)
{
    ot_s32 err;
    err = snprintf_s(file_name, length, length - 1, "./%s/FPN_dark_frame_%ux%u_%dbit_iso%u.raw",
                     dir_name, video_frame->attr.width, video_frame->attr.height,
                     sample_comm_vi_get_raw_bit_width(video_frame->attr.pixel_fmt), iso);
    if (err < 0) {
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_void sample_comm_vi_read_fpn_file(ot_eis_vi_fpn_frame_info *fpn_frame_info, FILE *pfd)
{
    ot_eis_video_frame *frame_info;
    ot_s32 i;

    frame_info = &fpn_frame_info->fpn_frame;
    (ot_void)fread((ot_u8 *)frame_info->buff.virt_addr[0], fpn_frame_info->frm_size, 1, pfd);

    for (i = 0; i < OT_EIS_VI_STRIPING_MAX_NUM; i++) {
        (ot_void)fread((ot_u8 *)&fpn_frame_info->offset[i], 4, 1, pfd); /* 4: 4byte */
    }

    (ot_void)fread((ot_u8 *)&fpn_frame_info->frm_size, 4, 1, pfd); /* 4: 4byte */
    (ot_void)fread((ot_u8 *)&fpn_frame_info->iso, 4, 1, pfd); /* 4: 4byte */
}

ot_s32 sample_comm_vi_enable_fpn_correction_for_scene(ot_vi_pipe_handle pipe_hdl, sample_vi_fpn_correction_cfg *correction_cfg,
    ot_u32 iso, sample_scene_fpn_offset_cfg *scene_fpn_offset_cfg, const ot_char *dir_name)
{
    ot_s32 ret = OT_SUCCESS;
    ot_u32 i;
    FILE *pfd = OT_NULL;
    ot_eis_vi_fpn_cor_attr correction_attr;
    sample_vi_user_frame_info *user_frame_info = &correction_cfg->user_frame_info;
    ot_char fpn_file_name[FPN_FILE_NAME_LENGTH];
    check_return(sample_comm_vi_get_fpn_frame_info(pipe_hdl, correction_cfg->pixel_format, user_frame_info, 1),
                 "sample_comm_vi_get_fpn_frame_info");
    (ot_void)memcpy_s(&correction_attr.fpn_frm_info.fpn_frame, sizeof(ot_eis_video_frame),
                      &user_frame_info->frame_info, sizeof(ot_eis_video_frame));

    check_return(sample_comm_vi_get_fpn_file_name_iso(&correction_attr.fpn_frm_info.fpn_frame, dir_name,
                                                      fpn_file_name, FPN_FILE_NAME_LENGTH, iso),
                 "sample_comm_vi_get_fpn_file_name_iso");
    pfd = fopen(fpn_file_name, "rb");
    if (pfd == OT_NULL) {
        printf("open file %s err!\n", fpn_file_name);
        goto exit;
    }
    printf("open file %s success!\n", fpn_file_name);
    correction_attr.fpn_frm_info.frm_size = user_frame_info->blk_size;
    sample_comm_vi_read_fpn_file(&correction_attr.fpn_frm_info, pfd);
    ret = fclose(pfd);
    if (ret != OT_SUCCESS) {
        goto exit;
    }
    correction_attr.fpn_frm_info.iso = iso;
    for (i = 0; i < OT_EIS_VI_STRIPING_MAX_NUM; i++) {
        correction_attr.fpn_frm_info.offset[i] = scene_fpn_offset_cfg->offset;
        printf("offset[%u] = %#x; ", i, scene_fpn_offset_cfg->offset);
    }
    printf("\n frame_size = %u. iso = %u.\n", correction_attr.fpn_frm_info.frm_size, correction_attr.fpn_frm_info.iso);
    correction_attr.enable = OT_TRUE;
    correction_attr.aibnr_mode = correction_cfg->aibnr_mode;
    correction_attr.op_type = correction_cfg->op_mode;
    correction_attr.fpn_type = correction_cfg->fpn_type;
    correction_attr.manual_attr.strength = correction_cfg->strength;
    ret = ot_eis_vi_fpn_set_cor_attr(pipe_hdl, &correction_attr);
    if (ret != OT_SUCCESS) {
        sample_print("set fpn attr failed!\n");
        goto exit;
    }
    return OT_SUCCESS;

exit:
    sample_comm_vi_release_frame_blk(user_frame_info, 1);
    return ret;
}
ot_s32 sample_comm_vi_disable_fpn_correction(ot_vi_pipe_handle pipe_hdl,
                                             sample_vi_fpn_correction_cfg *correction_cfg)
{
    ot_s32 ret;
    ot_eis_vi_fpn_cor_attr correction_attr;

    ret = ot_eis_vi_fpn_get_cor_attr(pipe_hdl, &correction_attr);
    if (ret != OT_SUCCESS) {
        sample_print("get fpn attr failed!\n");
        return OT_FAILURE;
    }

    correction_attr.enable = OT_FALSE;
    ret = ot_eis_vi_fpn_set_cor_attr(pipe_hdl, &correction_attr);
    if (ret != OT_SUCCESS) {
        sample_print("set fpn attr failed!\n");
        return OT_FAILURE;
    }

    sample_comm_vi_release_frame_blk(&correction_cfg->user_frame_info, 1);

    return OT_SUCCESS;
}
