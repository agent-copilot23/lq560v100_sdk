/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <sys/time.h>

#include "sample_comm.h"
#include "securec.h"
#include "ot_buffer_pool.h"
#include "ot_eis_vi_api.h"


typedef struct {
    ot_eis_handle vproc_chn_hdl;
    ot_eis_handle venc_hdl;
} vproc_send_attr;


static volatile sig_atomic_t g_sig_flag = 0;

ot_eis_vi_vproc_mode_type g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
ot_eis_3dnr_pos g_nr_pos = OT_EIS_3DNR_POS_VPROC;

static ot_bool g_save_file_pthread = OT_FALSE;

static sample_vproc_attr g_sample_vproc_attr = {0};


static sample_venc_attr g_sample_venc_attr = {
    .venc_attr = {
        .codecFormat = OT_EIS_VENC_CODEC_HEVC,
        .common_attr = {
            .max_pic_width = 2688,//3840
            .max_pic_height = 1520,//2160
            .out_width = 2688,//3840
            .out_height = 1520,//2160
            .stream_buf_size = 6220800,
        },
        .video_attr = {
            .rc_attr = {
                .gop = 30,
                .output_fps = 30,
                .input_fps = 60,
                .rc_mode = OT_EIS_VENC_RC_MODE_CVBR,
                .cvbr_attr = {
                    .statis_time = 1,
                    .max_bit_rate = 13312,
                    .short_statis_time = 1,
                    .long_statis_time = 1,
                    .long_max_bit_rate = 13312,
                    .long_min_bit_rate = 3072,
                },
            },
            .gop_attr = {
                .gop_mode = OT_EIS_VENC_GOP_MODE_NORMAL,
                .gop_normal = {
                    .qp_delta_ip = 2,
                },
            },
        },
    },
    .venc_hdl = (ot_eis_venc_chn_handle)-1,
};


ot_eis_nr_cfg g_sample_nr_param = {
    .iey[0] = {.ies0 = 0, .ies1 = 0, .ies2 = 0, .ies3 = 0, .iedz = 0, .o_sht = 0, .u_sht = 0},
    .iey[1] = {.ies0 = 0, .ies1 = 0, .ies2 = 0, .ies3 = 0, .iedz = 0, .o_sht = 0, .u_sht = 0},
    .iey[2] = {.ies0 = 0, .ies1 = 0, .ies2 = 0, .ies3 = 0, .iedz = 0, .o_sht = 0, .u_sht = 0},
    .iey[3] = {.ies0 = 0, .ies1 = 0, .ies2 = 0, .ies3 = 0, .iedz = 0, .o_sht = 0, .u_sht = 0},
    .iey[4] = {.ies0 = 0, .ies1 = 0, .ies2 = 0, .ies3 = 0, .iedz = 0, .o_sht = 0, .u_sht = 0},
    .sfy[0] = {.spn = 0, .sbn = 0, .pbr = 0, .j_mode = 0, .reserved0 = 0,
        .sfr6 = {0, 0, 0, 0},
        .sbr6 = {0, 0},
        .sfs1 = 0, .sbr1 = 0, .sfs2 = 0, .sft2 = 0, .sbr2 = 0, .sfs4 = 0, .sft4 = 0, .sbr4 = 0,
        .sth1_0 = 0, .sth2_0 = 0, .sth3_0 = 0,
        .sth1_1 = 0, .sth2_1 = 0, .sth3_1 = 0,
        .sfn0_0 = 0, .sfn1_0 = 0, .sfn2_0 = 0, .sfn3_0 = 0,
        .sfn0_1 = 0, .sfn1_1 = 0, .sfn2_1 = 0, .sfn3_1 = 0,
        .bri_str = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .bri_idx0 = 0, .bri_idx1 = 0, .k_mode = 0,
        .sbs_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        .sds_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
    .sfy[1] = {.spn = 0, .sbn = 0, .pbr = 0, .j_mode = 0, .reserved0 = 0,
        .sfr6 = {0, 0, 0, 0},
        .sbr6 = {0, 0},
        .sfs1 = 0, .sbr1 = 0, .sfs2 = 0, .sft2 = 0, .sbr2 = 0, .sfs4 = 0, .sft4 = 0, .sbr4 = 0,
        .sth1_0 = 0, .sth2_0 = 0, .sth3_0 = 0,
        .sth1_1 = 0, .sth2_1 = 0, .sth3_1 = 0,
        .sfn0_0 = 0, .sfn1_0 = 0, .sfn2_0 = 0, .sfn3_0 = 0,
        .sfn0_1 = 0, .sfn1_1 = 0, .sfn2_1 = 0, .sfn3_1 = 0,
        .bri_str = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .bri_idx0 = 0, .bri_idx1 = 0, .k_mode = 0,
        .sbs_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        .sds_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
    .sfy[2] = {.spn = 0, .sbn = 0, .pbr = 0, .j_mode = 0, .reserved0 = 0,
        .sfr6 = {0, 0, 0, 0},
        .sbr6 = {0, 0},
        .sfs1 = 0, .sbr1 = 0, .sfs2 = 0, .sft2 = 0, .sbr2 = 0, .sfs4 = 0, .sft4 = 0, .sbr4 = 0,
        .sth1_0 = 0, .sth2_0 = 0, .sth3_0 = 0,
        .sth1_1 = 0, .sth2_1 = 0, .sth3_1 = 0,
        .sfn0_0 = 0, .sfn1_0 = 0, .sfn2_0 = 0, .sfn3_0 = 0,
        .sfn0_1 = 0, .sfn1_1 = 0, .sfn2_1 = 0, .sfn3_1 = 0,
        .bri_str = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .bri_idx0 = 0, .bri_idx1 = 0, .k_mode = 0,
        .sbs_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        .sds_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
    .sfy[3] = {.spn = 0, .sbn = 0, .pbr = 0, .j_mode = 0, .reserved0 = 0,
        .sfr6 = {0, 0, 0, 0},
        .sbr6 = {0, 0},
        .sfs1 = 0, .sbr1 = 0, .sfs2 = 0, .sft2 = 0, .sbr2 = 0, .sfs4 = 0, .sft4 = 0, .sbr4 = 0,
        .sth1_0 = 0, .sth2_0 = 0, .sth3_0 = 0,
        .sth1_1 = 0, .sth2_1 = 0, .sth3_1 = 0,
        .sfn0_0 = 0, .sfn1_0 = 0, .sfn2_0 = 0, .sfn3_0 = 0,
        .sfn0_1 = 0, .sfn1_1 = 0, .sfn2_1 = 0, .sfn3_1 = 0,
        .bri_str = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .bri_idx0 = 0, .bri_idx1 = 0, .k_mode = 0,
        .sbs_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        .sds_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
    .sfy[4] = {.spn = 0, .sbn = 0, .pbr = 0, .j_mode = 0, .reserved0 = 0,
        .sfr6 = {0, 0, 0, 0},
        .sbr6 = {0, 0},
        .sfs1 = 0, .sbr1 = 0, .sfs2 = 0, .sft2 = 0, .sbr2 = 0, .sfs4 = 0, .sft4 = 0, .sbr4 = 0,
        .sth1_0 = 0, .sth2_0 = 0, .sth3_0 = 0,
        .sth1_1 = 0, .sth2_1 = 0, .sth3_1 = 0,
        .sfn0_0 = 0, .sfn1_0 = 0, .sfn2_0 = 0, .sfn3_0 = 0,
        .sfn0_1 = 0, .sfn1_1 = 0, .sfn2_1 = 0, .sfn3_1 = 0,
        .bri_str = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        .bri_idx0 = 0, .bri_idx1 = 0, .k_mode = 0,
        .sbs_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        .sds_k = {  0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
    .mdy[0] = {.math0 = 1, .mate0 = 0, .adv_math = 0, .math1 = 1, .mate1 = 0, .reserved0 = 0,
            .mabw0 = 5, .mabw1 = 5, .artc = 0, .mcth = 0, .dci_w = 0},
    .mdy[1] = {.math0 = 1, .mate0 = 0, .adv_math = 0, .math1 = 1, .mate1 = 0, .reserved0 = 0,
            .mabw0 = 5, .mabw1 = 5, .artc = 0, .mcth = 0, .dci_w = 0},
    .tfy[0] = {.tfs0 = 0, .tdz0 = 0, .ref_en = 0, .reserved0 = 0, .tfs1 = 0, .tdz1 = 0,
            .math_mode = 1, .reserved1 = 0, .tss0= 0, .tss1 = 0, .auto_math = 0,
        .tfr0 = {0, 0, 0, 0, 0, 0},
        .tfr1 = {0, 0, 0, 0, 0, 0}},
    .tfy[1] = {.tfs0 = 0, .tdz0 = 0, .ref_en = 0, .reserved0 = 0, .tfs1 = 0, .tdz1 = 0,
            .math_mode = 1, .reserved1 = 0, .tss0 = 0, .tss1 = 0, .auto_math = 0,
        .tfr0 = {0, 0, 0, 0, 0, 0},
        .tfr1 = {0, 0, 0, 0, 0, 0}},
    .tfy[2] = {.tfs0 = 0, .tdz0 = 0, .ref_en = 0, .reserved0 = 0, .tfs1 = 0, .tdz1 = 0,
            .math_mode = 1, .reserved1 = 0, .tss0 = 0, .tss1 = 0, .auto_math = 0,
        .tfr0 = {0, 0, 0, 0, 0, 0},
        .tfr1 = {0, 0, 0, 0, 0, 0}},
    .nrc0 = {.sfc_enhance = 0, .sfc_ext = 0, .trc = 0, .sfc = 0, .tfc = 0, .reserved1 = 0,
        .tpc = 1, .pre_sfc = 0},
    .nrc1 = {.sfs2 = 0, .sft2 = 0, .sbr2 = 0, .pbr0 = 0, .pbr1 = 0, .reserved0 = 0},
    .limit_range_en = 0, .nry0_en = 0, .nry1_en = 0, .nry2_en = 0, .nry3_en = 0,
    .nrc0_en = 0, .nrc1_en = 0, .reserved0 = 0
};


static ot_void sample_print_vproc_attr(ot_void)
{
    ot_u32 i = 0;
    ot_eis_vproc_pipe_attr *pipe;
    ot_eis_vproc_chn_attr *chn;
    printf("grp (%d %d) \n", g_sample_vproc_attr.grp_attr.max_width, g_sample_vproc_attr.grp_attr.max_height);
    for (i = 0; i < OT_EIS_VPROC_GRP_PIPE_MAX_NUM; i++) {
        pipe = &g_sample_vproc_attr.pipe_attr[i];
        printf("pipe %d frc (%d %d) mode %d (%d %d) fmt %d \n", i,
            pipe->frc.src_frame_rate,
            pipe->frc.dst_frame_rate,
            pipe->mode,
            pipe->image_attr.width,
            pipe->image_attr.height,
            pipe->image_attr.pixel_fmt);
    }

    for (i = 0; i < OT_EIS_VPROC_GRP_CHN_MAX_NUM; i++) {
        chn = &g_sample_vproc_attr.chn_attr[i];
        printf("chn %d frc (%d %d) mode %d depth %d (%d %d) fmt %d \n", i,
            chn->frc.src_frame_rate,
            chn->frc.dst_frame_rate,
            chn->mode,
            chn->frame_queue_depth,
            chn->image_attr.width,
            chn->image_attr.height,
            chn->image_attr.pixel_fmt);
    }
    printf("set attr %d \n", g_sample_vproc_attr.is_set_attr);
    printf("crop attr %d (%d %d %d %d)\n", g_sample_vproc_attr.set_attr.crop_param.enable,
        g_sample_vproc_attr.set_attr.crop_param.crop_rect.x,
        g_sample_vproc_attr.set_attr.crop_param.crop_rect.y,
        g_sample_vproc_attr.set_attr.crop_param.crop_rect.width,
        g_sample_vproc_attr.set_attr.crop_param.crop_rect.height);
    printf("mosaic attr %d num %d \n", g_sample_vproc_attr.set_attr.mosaic_param.enable,
        g_sample_vproc_attr.set_attr.mosaic_param.num);
    printf("cover attr %d num %d \n", g_sample_vproc_attr.set_attr.cover_param.enable,
        g_sample_vproc_attr.set_attr.cover_param.num);
    printf("line attr %d num %d \n", g_sample_vproc_attr.set_attr.line_param.enable,
        g_sample_vproc_attr.set_attr.line_param.num);

}

static ot_void sample_get_char(ot_void)
{
    if (g_sig_flag == 1) {
        return;
    }

    sample_pause();
}


static ot_s32 sample_alloc_osd_frame(ot_eis_img_frame *osd_frm, ot_u32 w, ot_u32 h, ot_eis_img_pixel_format fmt)
{
    ot_u32 pixel_size = 0;
    ot_u32 stride = 0;
    ot_void *tmp;
    if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_1555 || fmt == OT_EIS_IMAGE_FORMAT_ARGB_4444) {
        pixel_size = 2;
    } else if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_8888) {
        pixel_size = 4;
    }
    osd_frm->attr.pixel_fmt = fmt;
    osd_frm->attr.width = w;
    osd_frm->attr.height = h;
    osd_frm->buff.stride[0] = w * pixel_size;
    stride = osd_frm->buff.stride[0];
    tmp = malloc(h * stride);
    if (tmp == OT_NULL) {
        printf("alloc frame buffer err\n");
        return OT_FAILURE;
    }
    osd_frm->buff.virt_addr[0] = tmp;
    return OT_SUCCESS;
}

static ot_s32 sample_free_osd_frame(ot_eis_img_frame *osd_frm)
{
    free(osd_frm->buff.virt_addr[0]);
    return OT_SUCCESS;
}

static ot_s32 sample_fill_osd_frame(ot_eis_img_frame *osd_frm)
{
    ot_u32 pixel_size = 0;
    ot_void *tmp;
    ot_u32 rgb_data;
    ot_eis_img_pixel_format fmt = osd_frm->attr.pixel_fmt;
    ot_u32 stride = osd_frm->buff.stride[0];
    ot_u32 r = 0;
    ot_u32 g = 0;
    ot_u32 b = 0;
    ot_u32 i, j;
    ot_float rate = 256.0 / (osd_frm->attr.width * 1.0);

    if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_1555) {
        pixel_size = 2;
        rate = 32.0 / (osd_frm->attr.width * 1.0);
    }
    if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_4444) {
        pixel_size = 2;
        rate = 16.0 / (osd_frm->attr.width * 1.0);
    }
    if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_8888) {
        pixel_size = 4;
        rate = 256.0 / (osd_frm->attr.width * 1.0);
    }

    for (i = 0; i < osd_frm->attr.height; i++) {
        for (j = 0; j < osd_frm->attr.width; j++) {
            r = rate * j;
            g = rate * j;
            b = rate * j;
            if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_1555) {
                rgb_data = (1 << 15 | (r & 0x1F) << 10 | (g & 0x1F) << 5 | (b & 0x1F)) & 0xFFFF;
            }
            if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_4444) {
                rgb_data = (0xF << 12 | (r & 0xF) << 8 | (g & 0xF) << 4 | (b & 0xF)) & 0xFFFF;
            }
            if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_8888) {
                rgb_data = (0xFF << 24 | (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF)) & 0xFFFFFFFF;
            }
            tmp = osd_frm->buff.virt_addr[0] + i * stride + j * pixel_size;
            memcpy(tmp, &rgb_data, pixel_size);
        }
    }

    return OT_SUCCESS;
}

ot_u64 sample_comm_get_time_us(ot_void)
{
    struct timeval tv;
    struct timezone tz;
    ot_u64 ret;
    gettimeofday (&tv , &tz);
    ret = tv.tv_sec * 1000000 + tv.tv_usec;
    return ret;
}

static ot_slong file_size(FILE *stream)
{
    ot_slong curpos, length;
    curpos = ftell(stream);
    fseek(stream, 0L, SEEK_END);
    length = ftell(stream);
    fseek(stream, curpos, SEEK_SET);
    return length;
}

static ot_s32 sample_read_osd_frame(ot_eis_img_frame *osd_frm, ot_s8 *file_name, ot_u32 idx)
{
    ot_u32 pixel_size = 0;
    ot_u8 *data;
    ot_eis_img_pixel_format fmt = osd_frm->attr.pixel_fmt;
    ot_u32 i, cnt;
    ot_u32 len = 0;

    if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_1555) {
        pixel_size = 2;
    }
    if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_4444) {
        pixel_size = 2;
    }
    if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_8888) {
        pixel_size = 4;
    }
    data = osd_frm->buff.virt_addr[0];
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        sample_print("file open failed, check now name:%s\n", file_name);
        return OT_FAILURE;
    }
    len = file_size(fp);
    sample_print("file:%s size %d \n", file_name, len);

    for (i = 0; i < osd_frm->attr.height; i++) {
        cnt = fread(data, pixel_size, osd_frm->attr.width, fp);
        if (cnt != osd_frm->attr.width) {
            fclose(fp);
            sample_print("read argb failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += osd_frm->buff.stride[0];
        //data += osd_frm->attr.width * pixel_size;
    }
    osd_frm->index = idx;
    osd_frm->pts = sample_comm_get_time_us();

    fclose(fp);

    return OT_SUCCESS;
}



ot_s32 sample_vproc_comm_get_block_for_frame(ot_eis_handle pool, ot_eis_video_frame *frame)
{
    ot_s32 ret = 0;
    ot_eis_buf_size_calc_cfg calc_cfg = { 0 };
    ot_phys_addr phys_addr;
    ot_void *virt_addr = OT_NULL;
    ot_buffer_pool_blk blk = { 0 };

    sample_common_get_pic_buf_cfg(&frame->attr, &calc_cfg);
    ret = ot_buffer_pool_blk_acquire(pool, calc_cfg.blk_size, &blk);
    if (ret != OT_SUCCESS) {
        sample_print("ot_buffer_pool_blk_acquire err, size:%u\n", calc_cfg.blk_size);
        return OT_FAILURE;
    }

    phys_addr = blk.phys_addr;
    ret = ot_smr_mmap(phys_addr, calc_cfg.blk_size, OT_FALSE, &virt_addr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_smr_mmap err!\n");
        ot_buffer_pool_blk_release(pool, &blk);
        return OT_FAILURE;
    }
    frame->pool_hdl           = pool;
    frame->mod_id             = OT_EIS_MODULE_VPROC;
    frame->color_space        = OT_EIS_IMAGE_COLOR_SPACE_BT601;
    frame->buff.phys_addr[0]  = phys_addr;
    frame->buff.phys_addr[1]  = frame->buff.phys_addr[0] + calc_cfg.main_y_size;
    frame->buff.virt_addr[0]  = virt_addr;
    frame->buff.virt_addr[1]  = frame->buff.virt_addr[0] + calc_cfg.main_y_size;
    frame->buff.stride[0]     = calc_cfg.main_stride;
    frame->buff.stride[1]     = calc_cfg.main_stride;

    return ret;
}

ot_s32 sample_vproc_release_frame(ot_eis_handle pool, ot_eis_video_frame *frame)
{
    ot_buffer_pool_blk blk;
    ot_s32 ret = 0;
    ot_eis_buf_size_calc_cfg calc_cfg = { 0 };

    sample_common_get_pic_buf_cfg(&frame->attr, &calc_cfg);
    blk.phys_addr = frame->buff.phys_addr[0];
    blk.block_size = calc_cfg.blk_size;
    ret = ot_buffer_pool_blk_release(pool, &blk);
    return ret;
}

ot_void *sample_vproc_map(ot_phys_addr buff, ot_u32 size)
{
    ot_void *addr = OT_NULL;
    ot_s32 ret = OT_SUCCESS;
    ret = ot_smr_mmap(buff, size, OT_FALSE, &addr);
    if (ret != OT_SUCCESS) {
        sample_print("buff addr 0x%llx size %d virt 0x%llx return 0x%x",
            buff, size, (ot_u64)addr, ret);
        addr = OT_NULL;
    }

    return addr;
}

ot_u32 sample_vproc_unmap(ot_void* virt_addr, ot_u32 size)
{
    if (virt_addr != OT_NULL) {
         ot_smr_munmap(virt_addr, size);
    }
    return OT_SUCCESS;
}

ot_s32 ot_img_write_to_yuv420_semiplaner(ot_eis_img_frame *img, char *file_name, ot_bool append, ot_bool remap)
{
    ot_u32 cnt;
    ot_u8 *data;
    ot_u8 *virt;
    ot_u8 *tmp;
    ot_u32 buff_size;
    ot_u32 c_offset = 0;
    FILE* fp;

    if (append) {
        // 追加写
        fp = fopen(file_name, "a+");
    } else {
        // 覆盖写
        fp = fopen(file_name, "wb");
    }
    sample_print("%s frame%d y_addr 0x%llx c_addr 0x%llx \n", file_name,
        img->index, img->buff.phys_addr[0], img->buff.phys_addr[1]);
    if (!fp) {
        sample_print("file open failed, check now name:%s\n", file_name);
        return OT_FAILURE;
    }

    // Y
    if (remap) {
        buff_size = img->buff.stride[0] * img->attr.height + img->buff.stride[1] * img->attr.height / 2;
        virt = sample_vproc_map(img->buff.phys_addr[0], buff_size);
        if (virt == OT_NULL) {
            sample_print("mmap phys 0x%llx size %d error \n", img->buff.phys_addr[0], buff_size);
            return OT_FAILURE;
        }
        c_offset = img->buff.phys_addr[1] - img->buff.phys_addr[0];
        sample_print("mmap part y virt_addr 0x%llx c offset %d \n", (ot_u64)virt, c_offset);

    }else {
        virt = img->buff.virt_addr[0];
    }
    sample_print("write y data 0x%llx to file %s \n", (ot_u64)virt, file_name);
    data = virt;
    for (int i = 0; i < img->attr.height; i++) {
        cnt = fwrite(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fflush(fp);
            fclose(fp);
            sample_print("write y failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[0];
    }

    // UV
    if (remap) {
        tmp = virt + c_offset;
    } else {
        tmp = img->buff.virt_addr[1];
    }
    sample_print("write c data 0x%llx to file %s \n", (ot_u64)tmp, file_name);
    data = tmp;
    for (int i = 0; i < img->attr.height / 2; i++) {
        cnt = fwrite(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fflush(fp);
            fclose(fp);
            sample_print(" write uv failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[1];
    }
    if (remap) {
        sample_print("ot_smr_munmap part y virt_addr 0x%llx \n", (ot_u64)virt);
        sample_vproc_unmap(virt, buff_size);
    }
    fflush(fp);
    fclose(fp);
    return OT_SUCCESS;
}

ot_s32 ot_img_read_to_yuv420_semiplaner(ot_eis_img_frame *img, char *file_name, ot_u32 idx)
{
    ot_u32 cnt;
    ot_u8 *data;
    ot_slong len;
    ot_u32 offset_size;
    ot_u32 frm_size;
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        sample_print("file open failed, check now name:%s\n", file_name);
        return OT_FAILURE;
    }
    len = file_size(fp);
    sample_print("file:%s size %ld \n", file_name, len);
    frm_size = (img->attr.width * img->attr.height) +
        (img->attr.width * img->attr.height / 2);
    offset_size = frm_size * idx;
    sample_print("offset_size size %d \n", offset_size);
    if (offset_size + frm_size > len) {
        offset_size = offset_size % ((len / frm_size) * frm_size);
        sample_print("file :%s not have %d frame use frame %d\n", file_name, idx, offset_size / frm_size);
    }
    fseek(fp, offset_size, SEEK_SET);
    data = (void *)img->buff.virt_addr[0];
    for (int i = 0; i < img->attr.height; i++) {
        cnt = fread(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fclose(fp);
            sample_print("read y failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[0];
    }

    // UV
    data = (void *)img->buff.virt_addr[1];
    for (int i = 0; i < img->attr.height / 2; i++) {
        cnt = fread(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fclose(fp);
            sample_print(" read u failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[1];
    }
    img->index = idx;
    img->pts = sample_comm_get_time_us();
    fclose(fp);
    return OT_SUCCESS;
}

ot_s32 ot_img_read_to_yuv422_semiplaner(ot_eis_img_frame *img, char *file_name, ot_u32 idx)
{
    ot_u32 cnt;
    ot_u8 *data;
    ot_slong len;
    ot_u32 offset_size;
    ot_u32 frm_size;
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        sample_print("file open failed, check now name:%s\n", file_name);
        return OT_FAILURE;
    }
    len = file_size(fp);
    sample_print("file:%s size %ld \n", file_name, len);
    frm_size = (img->attr.width * img->attr.height) +
        (img->attr.width * img->attr.height);
    offset_size = frm_size * idx;
    sample_print("offset_size size %d \n", offset_size);
    if (offset_size + frm_size > len) {
        offset_size = offset_size % ((len / frm_size) * frm_size);
        sample_print("file :%s not have %d frame use frame %d\n", file_name, idx, offset_size / frm_size);
    }
    fseek(fp, offset_size, SEEK_SET);
    data = (void *)img->buff.virt_addr[0];
    for (int i = 0; i < img->attr.height; i++) {
        cnt = fread(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fclose(fp);
            sample_print("read y failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[0];
    }

    // UV
    data = (void *)img->buff.virt_addr[1];
    for (int i = 0; i < img->attr.height; i++) {
        cnt = fread(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fclose(fp);
            sample_print(" read u failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[1];
    }
    img->index = idx;
    img->pts = sample_comm_get_time_us();
    fclose(fp);
    return OT_SUCCESS;
}

ot_s32 ot_img_write_to_yuv422_semiplaner(ot_eis_img_frame *img, char *file_name, ot_bool append, ot_bool remap)
{
    ot_u32 cnt;
    ot_u8 *data;
    ot_u8 *virt;
    ot_u8 *tmp;
    ot_u32 buff_size;
    ot_u32 c_offset = 0;
    FILE* fp;

    if (append) {
        // 追加写
        fp = fopen(file_name, "a+");
        sample_print("append open file %s", file_name);
    } else {
        // 覆盖写
        fp = fopen(file_name, "wb");
        sample_print("overwire open file %s", file_name);
    }
    sample_print("%s frame%d y_addr 0x%llx stride %d c_addr 0x%llx stride %d", file_name,
        img->index, img->buff.phys_addr[0], img->buff.stride[0], img->buff.phys_addr[1], img->buff.stride[1]);
    if (!fp) {
        sample_print("file open failed, check now name:%s\n", file_name);
        return OT_FAILURE;
    }

    // Y
    if (remap) {
        buff_size = img->buff.stride[0] * img->attr.height +
            img->buff.stride[1] * img->attr.height;
        virt = sample_vproc_map(img->buff.phys_addr[0], buff_size);
        if (virt == OT_NULL) {
            sample_print("mmap phys 0x%llx size %d error \n", img->buff.phys_addr[0], buff_size);
            return OT_FAILURE;
        }
        c_offset = img->buff.phys_addr[1] - img->buff.phys_addr[0];
        sample_print("ot_smr_mmap part y virt_addr 0x%llx c offset %d", (ot_u64)virt, c_offset);
    } else {
        virt = img->buff.virt_addr[0];
    }
    sample_print("write y data 0x%llx to file %s", (ot_u64)virt, file_name);

    data = virt;
    for (int i = 0; i < img->attr.height; i++) {
        cnt = fwrite(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fflush(fp);
            fclose(fp);
            sample_print("write y failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[0];
    }
    // UV
    if (remap) {
        tmp = virt + c_offset;
    } else {
        tmp = img->buff.virt_addr[1];
    }
    sample_print("write c data 0x%llx to file %s", (ot_u64)tmp, file_name);
    data = tmp;

    for (int i = 0; i < img->attr.height; i++) {
        cnt = fwrite(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fflush(fp);
            fclose(fp);
            sample_print(" write uv failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[1];
    }
    if (remap) {
        sample_print("ot_smr_munmap part y virt_addr 0x%llx", (ot_u64)virt);
        sample_vproc_unmap(virt, buff_size);
    }
    fflush(fp);
    fclose(fp);
    return OT_SUCCESS;
}

ot_s32 ot_img_write_to_yuv400_semiplaner(ot_eis_img_frame *img, char *file_name, ot_bool append, ot_bool remap)
{
    ot_u32 cnt;
    ot_u8 *data;
    ot_u8 *virt;
    ot_u32 buff_size;
    FILE* fp;

    if (append) {
        // 追加写
        fp = fopen(file_name, "a+");
        sample_print("append open file %s", file_name);
    } else {
        // 覆盖写
        fp = fopen(file_name, "wb");
        sample_print("overwire open file %s", file_name);
    }
    sample_print("%s frame%d y_addr 0x%llx stride %d c_addr 0x%llx stride %d", file_name,
        img->index, img->buff.phys_addr[0], img->buff.stride[0], img->buff.phys_addr[1], img->buff.stride[1]);
    if (!fp) {
        sample_print("file open failed, check now name:%s\n", file_name);
        return OT_FAILURE;
    }

    // Y
    if (remap) {
        buff_size = img->buff.stride[0] * img->attr.height;
        virt = sample_vproc_map(img->buff.phys_addr[0], buff_size);
        if (virt == OT_NULL) {
            sample_print("mmap phys 0x%llx size %d error \n", img->buff.phys_addr[0], buff_size);
            return OT_FAILURE;
        }
        sample_print("ot_smr_mmap part y virt_addr 0x%llx", (ot_u64)virt);
    } else {
        virt = img->buff.virt_addr[0];
    }
    sample_print("write y data 0x%llx to file %s", (ot_u64)virt, file_name);

    data = virt;
    for (int i = 0; i < img->attr.height; i++) {
        cnt = fwrite(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fflush(fp);
            fclose(fp);
            sample_print("write y failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[0];
    }
    if (remap) {
        sample_print("ot_smr_munmap part y virt_addr 0x%llx", (ot_u64)virt);
        sample_vproc_unmap(virt, buff_size);
    }
    fflush(fp);
    fclose(fp);
    return OT_SUCCESS;
}

ot_s32 ot_img_read_to_yuv400_semiplaner(ot_eis_img_frame *img, char *file_name, ot_u32 idx)
{
    ot_u32 cnt;
    ot_u8 *data;
    ot_slong len;
    ot_u32 offset_size;
    ot_u32 frm_size;
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        sample_print("file open failed, check now name:%s\n", file_name);
        return OT_FAILURE;
    }
    len = file_size(fp);
    sample_print("file:%s size %ld \n", file_name, len);
    frm_size = (img->attr.width * img->attr.height);
    offset_size = frm_size * idx;
    sample_print("offset_size size %d \n", offset_size);
    if (offset_size + frm_size > len) {
        offset_size = offset_size % ((len / frm_size) * frm_size);
        sample_print("file :%s not have %d frame use frame %d\n", file_name, idx, offset_size / frm_size);

    }
    fseek(fp, offset_size, SEEK_SET);
    data = (void *)img->buff.virt_addr[0];
    for (int i = 0; i < img->attr.height; i++) {
        cnt = fread(data, 1, img->attr.width, fp);
        if (cnt != img->attr.width) {
            fclose(fp);
            sample_print("read y failed i:%d cnt:%d name:%s\n", i, cnt, file_name);
            return OT_FAILURE;
        }
        data += img->buff.stride[0];
    }

    img->index = idx;
    img->pts = sample_comm_get_time_us();

    fclose(fp);
    return OT_SUCCESS;
}


ot_s32 ot_img_read_frame_from_file(ot_eis_img_frame *img, char *file_name, ot_u32 idx)
{
    ot_s32 ret = OT_SUCCESS;
    switch(img->attr.pixel_fmt) {

    case OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR:
    case OT_EIS_IMAGE_FORMAT_YUV_420_SEMIPLANAR:
        img->buff.stride[0] = (img->attr.width + 15) & 0xFFF0;
        img->buff.stride[1] = (img->attr.width + 15) & 0xFFF0;
        ret = ot_img_read_to_yuv420_semiplaner(img, file_name, idx);
        break;

    case OT_EIS_IMAGE_FORMAT_YUV_422_SEMIPLANAR:
    case OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR:
        img->buff.stride[0] = (img->attr.width + 15) & 0xFFF0;
        img->buff.stride[1] = (img->attr.width + 15) & 0xFFF0;
        ret = ot_img_read_to_yuv422_semiplaner(img, file_name, idx);
        break;
    case OT_EIS_IMAGE_FORMAT_YUV_400:
        img->buff.stride[0] = (img->attr.width + 15) & 0xFFF0;
        ret = ot_img_read_to_yuv400_semiplaner(img, file_name, idx);
        break;
    default :
        sample_print("not support fmt %d ", img->attr.pixel_fmt);
        break;
    }
    return ret;
}

ot_s32 ot_img_write_frame_to_file(ot_eis_img_frame *img, char *file_name, ot_bool append, ot_bool remap)
{
    ot_s32 ret = OT_SUCCESS;
    switch(img->attr.pixel_fmt) {
    case OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR:
    case OT_EIS_IMAGE_FORMAT_YUV_420_SEMIPLANAR:
        ret = ot_img_write_to_yuv420_semiplaner(img, file_name, append, remap);
        break;

    case OT_EIS_IMAGE_FORMAT_YUV_422_SEMIPLANAR:
    case OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR:
        ret = ot_img_write_to_yuv422_semiplaner(img, file_name, append, remap);
        break;
    default :
        sample_print("not support fmt %d ", img->attr.pixel_fmt);
        break;
    }
    return ret;
}


ot_s32 sample_vproc_yuv_frame_alloc(ot_eis_handle pool, ot_eis_img_frame *frm)
{
    ot_s32 ret = OT_SUCCESS;
    ot_eis_img_attr *in_attr;
    in_attr = &frm->attr;
    if (in_attr->bit_depth == OT_EIS_PIXEL_BIT_DEPTH_8) {
        if (in_attr->pixel_fmt == OT_EIS_IMAGE_FORMAT_YUV_420_SEMIPLANAR ||
            in_attr->pixel_fmt == OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR ||
            in_attr->pixel_fmt == OT_EIS_IMAGE_FORMAT_YUV_422_SEMIPLANAR ||
            in_attr->pixel_fmt == OT_EIS_IMAGE_FORMAT_YVU_422_SEMIPLANAR ||
            in_attr->pixel_fmt == OT_EIS_IMAGE_FORMAT_YUV_400) {
            sample_vproc_comm_get_block_for_frame(pool, frm);
        } else {
            sample_print("not support fmt %d ", in_attr->pixel_fmt);
            ret = OT_FAILURE;
        }
    } else {
        sample_print("not support bit %d ", in_attr->bit_depth);
        ret = OT_FAILURE;
    }
    return ret;
}


static ot_s32 sample_vio_sys_init(ot_eis_img_size *img_size)
{
    ot_video_buffer_attr vrb_cfg;

    sample_common_get_vrb_cfg(3, img_size->width, img_size->height, &vrb_cfg);
    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    if (sample_comm_sys_set_vi_vproc_init_cfg(g_mode_type) != OT_SUCCESS) {
        goto sys_exit;
    }

    if (ot_eis_media_set_3dnr_pos(g_nr_pos) != OT_SUCCESS) {
        goto sys_exit;
    }

    return OT_SUCCESS;
sys_exit:
    sample_comm_sys_exit();
    return OT_FAILURE;
}

static ot_s32 sample_vio_start_route(sample_vi_cfg *vi_cfg, ot_u32 route_num)
{
    ot_s32 ret;
    ot_eis_handle media_pipe_hdl;
    ot_bool pipe_sw[OT_EIS_VPROC_GRP_PIPE_MAX_NUM] = {OT_TRUE, OT_FALSE, OT_FALSE, OT_FALSE};
    ot_bool chn_sw[OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {OT_TRUE, OT_FALSE, OT_FALSE, OT_FALSE};
    ot_eis_img_size img_size = {0};
    sample_vo_attr *vo_attr = OT_NULL;

    sample_comm_vi_get_size_by_sns_type(sample_comm_args_get_sns_type(0), &img_size);
    vo_attr = sample_comm_args_get_vo_attr();
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return OT_FAILURE;
    }

    if (sample_vio_sys_init(&img_size) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    ret = sample_comm_vi_start_vi(vi_cfg);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }

    // vproc
    ret = sample_comm_start_vproc(&g_sample_vproc_attr, pipe_sw, chn_sw);
    if (ret != OT_SUCCESS) {
        goto start_vproc_failed;
    }

    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_comm_media_pipe_init(&media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto pipe_start_failed;
    }

    ret = sample_comm_vi_bind_vproc(vi_cfg->pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto vi_vproc_bind_failed;
    }

    ret = sample_comm_vproc_bind_vo(g_sample_vproc_attr.chn_hdl[0], vo_attr->in_port_handle[0], media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto vproc_bind_vo_failed;
    }

    vi_cfg->media_pipe_hdl = media_pipe_hdl;

    return OT_SUCCESS;


vproc_bind_vo_failed:
    sample_comm_vi_un_bind_vproc(vi_cfg->pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
vi_vproc_bind_failed:
    sample_comm_media_pipe_stop(media_pipe_hdl);
pipe_start_failed:
    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_stop_vproc(&g_sample_vproc_attr);
start_vproc_failed:
    sample_comm_vi_stop_vi(vi_cfg);
start_vi_failed:
    sample_comm_sys_exit();

    return OT_FAILURE;
}

static ot_void sample_vio_stop_route(sample_vi_cfg *vi_cfg, ot_s32 route_num)
{
    sample_vo_attr *vo_attr = OT_NULL;

    vo_attr = sample_comm_args_get_vo_attr();
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return;
    }
    // sample_comm_vi_un_bind_vo(vi_cfg->chn_hdl, vo_attr->in_port_handle, vi_cfg->media_pipe_hdl);
    sample_comm_vproc_un_bind_vo(g_sample_vproc_attr.chn_hdl[0], vo_attr->in_port_handle[0], vi_cfg->media_pipe_hdl);
    sample_comm_vi_un_bind_vproc(vi_cfg->pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], vi_cfg->media_pipe_hdl);

    sample_comm_media_pipe_stop(vi_cfg->media_pipe_hdl);
    sample_comm_stop_vo(vo_attr);
    sample_comm_stop_vproc(&g_sample_vproc_attr);
    sample_comm_vi_stop_vi(vi_cfg);
    sample_comm_sys_exit();
}

static ot_s32 sample_vi_user_mode_buf_pool_creat(sample_sns_type sns_type, ot_eis_handle pool_hdl)
{
    ot_s32 ret;
    ot_eis_img_size size;
    ot_u32 block_size;
    ot_video_buffer_attr pool_attr = { 0 };
    ot_eis_img_attr img_attr = { 0 };
    sample_comm_vi_get_size_by_sns_type(sns_type, &size);

    /* default YUV pool: SP420 + compress_none */
    img_attr.bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8;
    img_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE;
    img_attr.height = size.height;
    img_attr.width = size.width;
    img_attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR;
    img_attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR;

    sample_common_get_buffer_pool_cfg(&img_attr, &block_size);
    pool_attr.cnt = 1;
    pool_attr.buf_blks[0].cnt = 10;
    pool_attr.buf_blks[0].size = block_size;

    ret = ot_buffer_pool_init();
    if (ret != OT_SUCCESS && ret != OT_ERR_BUFPOOL_ALREADY_INIT) {
       printf("buffer_pool init fail! ret[%d]\n", ret);
       return OT_FAILURE;
    }

    ret =  ot_buffer_pool_create(pool_hdl, &pool_attr);
    if(ret != 0){
       printf("buffer_pool create fail! ret[%d]\n", ret);
       goto pool_create_failed;
    }

    return OT_SUCCESS;

pool_create_failed:
    ot_buffer_pool_deinit();
    return ret;
}

static ot_void sample_vi_user_mode_buf_pool_destroy(ot_eis_handle pool_hdl)
{
    ot_buffer_pool_destroy(pool_hdl);
    ot_buffer_pool_deinit();
}

typedef struct {
    ot_eis_handle vi_chn_hdl;
    ot_eis_handle vproc_pipe_hdl;
    ot_eis_handle vproc_chn_hdl;
    ot_eis_handle vo_inport_hdl;
} vi_chn_send_attr;
static ot_bool g_send_chn_pthread = OT_FALSE;
static ot_bool g_vproc_set_pthread = OT_FALSE;


static ot_s32 sample_vproc_user_mode_buf_pool_creat(ot_eis_img_attr *img_attr, ot_eis_handle *pool_hdl)
{
    ot_s32 ret;
    ot_u32 block_size;
    ot_video_buffer_attr pool_attr = { 0 };
    ot_eis_handle tmp;

    img_attr->bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8;
    sample_common_get_buffer_pool_cfg(img_attr, &block_size);
    pool_attr.cnt = 1;
    pool_attr.buf_blks[0].cnt = 10;
    pool_attr.buf_blks[0].size = block_size;

    ret = ot_buffer_pool_init();
    if (ret != OT_SUCCESS && ret != OT_ERR_BUFPOOL_ALREADY_INIT) {
        printf("buffer_pool init fail! ret[%d]\n", ret);
        return OT_FAILURE;
    }

    ret = ot_buffer_pool_create(&tmp, &pool_attr);
    if (ret != 0) {
        printf("buffer_pool create fail! ret[%d]\n", ret);
        goto pool_create_failed;
    }
    *pool_hdl = tmp;
    sample_print("create pool hdl 0x%llx \n", (ot_u64)tmp);
    return OT_SUCCESS;

pool_create_failed:
    ot_buffer_pool_deinit();
    return ret;
}

static ot_void sample_vproc_user_mode_buf_pool_destroy(ot_eis_handle pool_hdl)
{
    sample_print("destroy pool hdl 0x%llx \n", (ot_u64)pool_hdl);
    ot_buffer_pool_destroy(pool_hdl);
    ot_buffer_pool_deinit();
}


static ot_void *sample_vi_get_frame_send_to_vo_thread(ot_void *param)
{
    ot_s32 ret;
    ot_eis_handle vi_chn_hdl;
    ot_eis_handle vproc_pipe_hdl;
    ot_eis_handle vproc_chn_hdl;
    ot_eis_handle vo_inport_hdl;
    vi_chn_send_attr *send_attr = (vi_chn_send_attr *)param;
    ot_eis_video_frame vi_video_frame = { 0 };
    ot_eis_video_frame vproc_video_frame = { 0 };
    ot_s32 milli_sec = 500;

    vi_chn_hdl = send_attr->vi_chn_hdl;
    vproc_pipe_hdl = send_attr->vproc_pipe_hdl;
    vproc_chn_hdl = send_attr->vproc_chn_hdl;
    vo_inport_hdl = send_attr->vo_inport_hdl;
    while (g_send_chn_pthread) {
        ret = ot_eis_vi_chn_get_frame(vi_chn_hdl, &vi_video_frame, milli_sec);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vi_chn_get_frame failed with %#x\n", ret);
            goto exit;
        }

        ret = ot_eis_vproc_pipe_send_frame(vproc_pipe_hdl, &vi_video_frame, 1);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vproc_pipe_send_frame failed with %#x\n", ret);
            ot_eis_vi_chn_release_frame(vi_chn_hdl, &vi_video_frame);
            goto exit;
        }

        ret = ot_eis_vproc_chn_acquire_frame(vproc_chn_hdl, &vproc_video_frame, milli_sec);
        if (ret != OT_SUCCESS) {
            if (ret != OT_EIS_ERR_VPROC_BUF_EMPTY) {
                printf("ot_eis_vproc_chn_acquire_frame failed with %#x\n", ret);
                ot_eis_vi_chn_release_frame(vi_chn_hdl, &vi_video_frame);
                goto exit;
            } else {
                ot_eis_vi_chn_release_frame(vi_chn_hdl, &vi_video_frame);
                continue;
            }
        }

        ret = ot_eis_vo_in_port_send_frame(vo_inport_hdl, &vproc_video_frame);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vo_in_port_send_frame failed with %#x\n", ret);
        }

        (void)ot_eis_vi_chn_release_frame(vi_chn_hdl, &vi_video_frame);
        (void)ot_eis_vproc_chn_release_frame(vproc_chn_hdl, &vproc_video_frame);
    }

exit:
    return OT_NULL;
}

static ot_s32 sample_media_vproc_search_user(sample_vproc_attr * vproc_attr, ot_bool pipe_sw[], ot_bool chn_sw[],
                                            ot_eis_handle* pipe_hdl, ot_eis_handle* user_hdl)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < OT_EIS_VPROC_GRP_PIPE_MAX_NUM; i++) {
        if ((pipe_sw[i] == OT_TRUE) && (vproc_attr->pipe_attr[i].mode == OT_EIS_VPROC_WORK_MODE_USER)) {
            break;
        }
    }
    for (j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
        if ((chn_sw[j] == OT_TRUE) && (vproc_attr->chn_attr[j].mode == OT_EIS_VPROC_WORK_MODE_USER)) {
            break;
        }
    }
    if ((i >= OT_EIS_VPROC_GRP_PIPE_MAX_NUM) || (j >= OT_EIS_VPROC_GRP_CHN_MAX_NUM)) {
        sample_print("no match pipe_sw or chn_sw for send frame\n");
        return OT_FAILURE;
    }

    *pipe_hdl = vproc_attr->pipe_hdl[i];
    *user_hdl = vproc_attr->chn_hdl[j];

    return OT_SUCCESS;
}

// static ot_s32 sample_vi_get_frame_send_to_vo(sample_vi_cfg *vi_cfg, sample_vo_attr *vo_attr)
static ot_s32 sample_vi_get_frame_send_to_vo(sample_vi_cfg *vi_cfg, sample_vo_attr *vo_attr, sample_vproc_attr *vproc_attr,
                                             ot_bool pipe_sw[], ot_bool chn_sw[])
{
    ot_s32 ret;
    pthread_t thread_id = 0;
    vi_chn_send_attr send_attr = { 0 };
    send_attr.vi_chn_hdl = vi_cfg->pipe_info[0].chn_info.chn_hdl;
    send_attr.vo_inport_hdl = vo_attr->in_port_handle[0];

    ret = sample_media_vproc_search_user(vproc_attr, pipe_sw, chn_sw, &send_attr.vproc_pipe_hdl, &send_attr.vproc_chn_hdl);
    if (ret != OT_SUCCESS) {
        printf("sample_media_vproc_search_user failed!\n");
        g_send_chn_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    g_send_chn_pthread = OT_TRUE;
    ret = pthread_create(&thread_id, OT_NULL, sample_vi_get_frame_send_to_vo_thread, (ot_void *)&send_attr);
    if (ret != OT_SUCCESS) {
        printf("vi create send frame thread failed!\n");
        g_send_chn_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    printf("vi send frame thread running, print any key to exit!\n");
    getchar();

    g_send_chn_pthread = OT_FALSE;
    pthread_join(thread_id, OT_NULL);
    return ret;
}

static ot_void sample_vio_print_vi_mode_list()
{
    printf("vi vproc mode list: \n");
    printf("    (0) VI_ONLINE_VPROC_ONLINE\n");
    printf("    (1) VI_ONLINE_VPROC_OFFLINE\n");
    printf("    (2) VI_OFFLINE_VPROC_OFFLINE\n");
    printf("please select mode:\n");
}

static ot_void sample_vio_get_vi_vproc_mode_by_char(ot_char ch, ot_bool is_wdr)
{
    switch (ch) {
        case '0':
            g_mode_type = OT_EIS_VI_ONLINE_VPROC_ONLINE;
            break;
        case '1':
            g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;
            break;
        case '2':
            g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
            break;
        default:
            g_mode_type = OT_EIS_VI_ONLINE_VPROC_ONLINE;
            break;
    }
}

static ot_void sample_vio_get_vi_vproc_mode(ot_bool is_wdr_mode)
{
    ot_char ch = '0';
    ot_char end_ch;
    ot_char input[3] = {0}; /* max_len: 3 */
    ot_s32 max_len = 3; /* max_len: 3 */

    end_ch = '2';

    sample_vio_print_vi_mode_list();

    while (g_sig_flag == 0) {
        if (gets_s(input, max_len) != OT_NULL && strlen(input) == 1 && input[0] >= ch && input[0] <= end_ch) {
            break;
        } else {
            printf("\nInvalid param, please enter again!\n\n");
            sample_vio_print_vi_mode_list();
        }
        (ot_void)fflush(stdin);
    }

    sample_vio_get_vi_vproc_mode_by_char(input[0], is_wdr_mode);
}

static ot_s32 sample_vio_all_mode(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    sample_vio_get_vi_vproc_mode(OT_FALSE);

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    if (sample_vio_start_route(&vi_cfg, 1) != OT_SUCCESS) {
        return ret;
    }

    sample_get_char();
    ret = ot_eis_vproc_pipe_get_attr(g_sample_vproc_attr.pipe_hdl[0], &g_sample_vproc_attr.pipe_attr[0]);
    sample_print("get pipe ret 0x%x \n", ret);

    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_s32 sample_vio_vproc_for_user_mode(sample_vproc_attr* vproc_attr, ot_bool chn_sw[])
{
    ot_s32 ret = OT_SUCCESS;

    int j;

    for (j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
        if ((chn_sw[j] == OT_TRUE) && (vproc_attr->chn_attr[j].mode == OT_EIS_VPROC_WORK_MODE_USER)) {
            break;
        }
    }
    if (j >= OT_EIS_VPROC_GRP_CHN_MAX_NUM) {
        sample_print("no match pipe_sw or chn_sw for send frame\n");
        return OT_FAILURE;
    }

    return ret;
}

static ot_s32 sample_vio_chn_usr_mode(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);
    ot_eis_handle pool_hdl = OT_NULL;
    ot_eis_handle vproc_pool_hdl = OT_NULL;
    ot_eis_img_size img_size = {0};
    sample_vo_attr *vo_attr = OT_NULL;

    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    vo_attr = sample_comm_args_get_vo_attr();
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return OT_FAILURE;
    }

    // ot_eis_handle media_pipe_hdl;
    ot_bool pipe_sw[OT_EIS_VPROC_GRP_PIPE_MAX_NUM] = {OT_FALSE, OT_TRUE, OT_FALSE, OT_FALSE};
    ot_bool chn_sw[OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {OT_FALSE, OT_TRUE, OT_FALSE, OT_FALSE};

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));
    if (sample_vio_sys_init(&img_size) != OT_SUCCESS) {
        return OT_FAILURE;
    }
    sample_print_vproc_attr();

    ret = sample_vi_user_mode_buf_pool_creat(sns_type, &pool_hdl);
    if (ret != OT_SUCCESS) {
        goto buf_pool_creat_failed;
    }

    vi_cfg.pipe_info[0].chn_info.chn_attr.work_mode = OT_EIS_VI_CHN_WORK_MODE_USER;
    vi_cfg.pipe_info[0].chn_info.chn_attr.frame_queue_depth = 8;
    vi_cfg.pipe_info[0].chn_info.chn_attr.pool_handle = pool_hdl;

    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }

    // vproc
    ret = sample_vio_vproc_for_user_mode(&g_sample_vproc_attr, chn_sw);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }
    ret = sample_vproc_user_mode_buf_pool_creat(&g_sample_vproc_attr.chn_attr[1].image_attr, &vproc_pool_hdl);
    if (ret != OT_SUCCESS) {
        goto buf_pool_creat_failed;
    }
    sample_print("sample_vproc_user_mode_buf_pool_creat pool hdl 0x%llx \n", (ot_u64)vproc_pool_hdl);
    g_sample_vproc_attr.chn_attr[1].pool_handle = vproc_pool_hdl;
    ret = sample_comm_start_vproc(&g_sample_vproc_attr, pipe_sw, chn_sw);
    if (ret != OT_SUCCESS) {
        goto start_vproc_failed;
    }

    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_vi_get_frame_send_to_vo(&vi_cfg, vo_attr, &g_sample_vproc_attr, pipe_sw, chn_sw);
    // send frame
    // ret = sample_comm_vi_get_vproc_send_frame(&g_sample_vproc_attr, pipe_sw, chn_sw, vi_cfg.chn_hdl, vo_attr->in_port_handle);
    if (ret != OT_SUCCESS) {
        goto vproc_send_failed;
    }

vproc_send_failed:
    // sample_comm_media_pipe_stop(media_pipe_hdl);
// pipe_start_failed:
    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_stop_vproc(&g_sample_vproc_attr);
start_vproc_failed:
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_vi_user_mode_buf_pool_destroy(pool_hdl);
buf_pool_creat_failed:
    sample_comm_sys_exit();

    return ret;
}


static ot_u32 g_jpeg_acquire_cnt = 0;
static FILE* g_file = OT_NULL;

static ot_s32 sample_comm_venc_get_file_postfix(ot_eis_venc_codec_format payload, ot_char* file_postfix, ot_u8 len)
{
    if (payload == OT_EIS_VENC_CODEC_H264) {
        if (strcpy_s(file_postfix, len, ".h264") != EOK) {
            return OT_FAILURE;
        }
    }
    else if (payload == OT_EIS_VENC_CODEC_HEVC) {
        if (strcpy_s(file_postfix, len, ".h265") != EOK) {
            return OT_FAILURE;
        }
    }
    else if (payload == OT_EIS_VENC_CODEC_JPEG) {
        if (strcpy_s(file_postfix, len, ".jpg") != EOK) {
            return OT_FAILURE;
        }
    }
    else {
        sample_print("payload type err!\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_comm_set_file_name(ot_eis_venc_chn_handle venc_chn, ot_char* file_postfix, ot_char* real_file_name)
{
    ot_char file_name[FILE_NAME_LEN];
    if (snprintf_s(file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1, "./") < 0) {
        return OT_FAILURE;
    }

    if (realpath(file_name, real_file_name) == OT_NULL) {
        sample_print("chn[%d] stream file path error\n", (ot_s32)((intptr_t)venc_chn));
        return OT_FAILURE;
    }

    if (snprintf_s(real_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "stream_chn%d%s", (ot_s32)((intptr_t)venc_chn), file_postfix) < 0) {
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_comm_set_name_save_stream(sample_venc_attr* attr)
{
    ot_s32 ret, fd;
    ot_char real_file_name[PATH_MAX] = { 0 };

    ot_eis_venc_codec_format payload_type = attr->venc_attr.codecFormat;
    ot_char file_postfix[10];

    ret = sample_comm_venc_get_file_postfix(payload_type, file_postfix, sizeof(file_postfix));
    if (ret != OT_SUCCESS) {
        sample_print("sample_comm_venc_get_file_postfix [%d] failed with %#x!\n", payload_type, ret);
        return ret;
    }

    ret = sample_comm_set_file_name(attr->venc_hdl, file_postfix, real_file_name);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    g_file = fopen(real_file_name, "wb");
    if (!g_file) {
        sample_print("open file[%s] failed!\n", real_file_name);
        return OT_FAILURE;
    }
    fd = fileno(g_file);
    fchmod(fd, S_IRUSR | S_IWUSR);

    return OT_SUCCESS;
}

static ot_s32 test_sample_comm_venc_save_stream(FILE* fd, ot_eis_venc_stream* stream)
{
    fwrite(stream->stream_virt_addr, stream->stream_lens, 1, fd);
    fflush(fd);

    return OT_SUCCESS;
}

static ot_s32 test_sample_comm_save_frame_to_file(FILE* fd, ot_eis_venc_stream* stream)
{
    ot_s32 ret;

    ret = test_sample_comm_venc_save_stream(g_file, stream);
    if (ret != OT_SUCCESS) {
        sample_print("save stream failed!\n");
        return ret;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_comm_get_stream_from_one_channl(sample_venc_attr* attr)
{
    ot_s32 ret;
    ot_eis_venc_chn_status status = { 0 };

    ret = ot_eis_venc_chn_query_status(attr->venc_hdl, &status);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_query_status chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return ret;
    }

    if (status.unTaken_streams == 0) {
        usleep(20000);
        return ret;
    }

    ot_eis_venc_stream ot_stream = { 0 };
    ret = ot_eis_venc_chn_acquire_stream(g_sample_venc_attr.venc_hdl, &ot_stream, -1);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_acquire_stream chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), ret);
        return ret;
    }

    g_jpeg_acquire_cnt = g_jpeg_acquire_cnt + 1;
    if (attr->venc_attr.codecFormat != OT_EIS_VENC_CODEC_JPEG ||
        (attr->venc_attr.codecFormat == OT_EIS_VENC_CODEC_JPEG && g_jpeg_acquire_cnt == 10)) {
        ret = test_sample_comm_save_frame_to_file(g_file, &ot_stream);
        if (ret != OT_SUCCESS) {
            sample_print("test_sample_comm_save_frame_to_file chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), ret);
        }
    }

    ret = ot_eis_venc_chn_release_stream(g_sample_venc_attr.venc_hdl, &ot_stream);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_release_stream chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), ret);
        return ret;
    }

    if (attr->venc_attr.codecFormat == OT_EIS_VENC_CODEC_JPEG  && g_jpeg_acquire_cnt == 10) {
        return 1;//SAMPLE_RETURN_JPEG;
    }

    return OT_SUCCESS;
}

/* get stream from each channels and save them */
ot_void* sample_vproc_venc_get_venc_stream_proc(ot_void* p)
{
    ot_s32 ret;

    prctl(PR_SET_NAME, "get_venc_stream", 0, 0, 0);

    ret = sample_comm_set_name_save_stream(&g_sample_venc_attr);
    if (ret != OT_SUCCESS) {
        sample_print("set name save stream failed\n");
        return OT_NULL;
    }

    while (g_save_file_pthread == OT_TRUE) {
        ret = sample_comm_get_stream_from_one_channl(&g_sample_venc_attr);
        if (ret == 1) {
            sample_print("sample_comm_get_stream_from_one_channl chn[%d] JPEG done!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl));
            break;
        }

        if (ret != OT_SUCCESS) {
            sample_print("sample_comm_get_stream_from_one_channl chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)g_sample_venc_attr.venc_hdl), ret);
        }
    }

    g_send_chn_pthread = OT_FALSE;
    fclose(g_file);
    return OT_NULL;
}

typedef struct {
    ot_u32 try_acquire_cnt;
    ot_u32 acquire_succ_cnt;
    ot_u32 try_release_cnt;
    ot_u32 release_succ_cnt;
} media_vproc_stats;

extern ot_s32 ot_eis_vproc_chn_get_stats(const ot_eis_handle chn_hdl, media_vproc_stats* status);

ot_u64 get_time_ms(ot_void)
{
    ot_u64 ret = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ret = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return ret;
}

static ot_void* sample_vproc_get_frame_send_to_venc_thread(ot_void* param)
{
    ot_s32 ret;
    ot_eis_handle vproc_chn_hdl;
    ot_eis_handle venc_hdl;
    vproc_send_attr* send_attr = (vproc_send_attr*)param;
    ot_eis_video_frame video_frame = { 0 };
    ot_eis_video_frame video_frame_tmp = { 0 };
    ot_eis_venc_image_frame venc_frame = { 0 };
    ot_u64 curr_pts = 0;
    ot_s32 milli_sec = -1;
    ot_u32 cnt = 0;
    media_vproc_stats stt;
    ot_u64 prv, curr, tt, max_tt = 0;
    vproc_chn_hdl = send_attr->vproc_chn_hdl;
    venc_hdl = send_attr->venc_hdl;
    while (g_send_chn_pthread) {
        ot_eis_media_get_cur_pts(&curr_pts);
        ret = ot_eis_vproc_chn_acquire_frame(vproc_chn_hdl, &video_frame_tmp, milli_sec);
        //printf("frame index %d pts %lld use %lld us phy_addr 0x%llx \n", video_frame_tmp.index,
        //    video_frame_tmp.pts, curr_pts - video_frame_tmp.pts, video_frame_tmp.buff.phys_addr[0]);
        venc_frame.frame = &video_frame_tmp;
        ret = ot_eis_venc_send_frame(venc_hdl, &venc_frame, -1);

        ret = ot_eis_vproc_chn_acquire_frame(vproc_chn_hdl, &video_frame, milli_sec);
        if (ret != OT_SUCCESS) {
            ot_eis_vproc_chn_get_stats(vproc_chn_hdl, &stt);
            sample_print("chn 1 acq try/succ %d %d rls try/succ %d %d \n", stt.try_acquire_cnt, stt.acquire_succ_cnt,
                    stt.try_release_cnt, stt.release_succ_cnt);
            printf("ot_eis_vi_chn_get_frame failed with %#x\n", ret);
            continue;
        }
        //printf("frame index %d pts %lld use %lld us phy_addr 0x%llx \n", video_frame.index,
        //    video_frame.pts, curr_pts - video_frame.pts, video_frame.buff.phys_addr[0]);

        venc_frame.frame = &video_frame;
        ret = ot_eis_venc_send_frame(venc_hdl, &venc_frame, -1);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_venc_send_frame failed with %#x\n", ret);
            ot_eis_vproc_chn_release_frame(vproc_chn_hdl, &video_frame);
        }
        prv = get_time_ms();
        ret = ot_eis_vproc_chn_release_frame(vproc_chn_hdl, &video_frame);
        curr = get_time_ms();
        tt = curr - prv;
        if (tt > max_tt) {
            max_tt = tt;
            printf("chn 1 ot_eis_vproc_chn_release_frame use %lld ms \n", tt);
        }
        if (ret != OT_SUCCESS) {
            sample_print("vproc_chn_release_frame error return %d \n", ret);
        }
        ret = ot_eis_vproc_chn_release_frame(vproc_chn_hdl, &video_frame_tmp);
        if (ret != OT_SUCCESS) {
            sample_print("vproc_chn_release_frame error return %d \n", ret);
        }
        cnt++;
        if (cnt % 1024 == 0) {
            ot_eis_vproc_chn_get_stats(vproc_chn_hdl, &stt);
            printf("chn 1 acq try/succ %d %d rls try/succ %d %d curr use %lld ms max %lld ms \n",
                stt.try_acquire_cnt, stt.acquire_succ_cnt, stt.try_release_cnt, stt.release_succ_cnt, tt, max_tt);
        }
    }

    return OT_NULL;
}

static ot_s32 sample_vproc_frame_to_venc_to_file(sample_vproc_attr* vproc_attr, sample_venc_attr* venc_attr)
{
    ot_s32 ret;
    pthread_t thread_id = 0;
    pthread_t save_pid = 0;
    vproc_send_attr send_attr = { 0 };
    send_attr.vproc_chn_hdl = vproc_attr->chn_hdl[1];
    send_attr.venc_hdl = venc_attr->venc_hdl;

    g_send_chn_pthread = OT_TRUE;
    ret = pthread_create(&thread_id, OT_NULL, sample_vproc_get_frame_send_to_venc_thread, (ot_void*)&send_attr);
    if (ret != OT_SUCCESS) {
        printf("create send frame thread failed!\n");
        g_send_chn_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    // 从venc取帧存入文件
    g_save_file_pthread = OT_TRUE;
    ret = pthread_create(&save_pid, OT_NULL, sample_vproc_venc_get_venc_stream_proc, OT_NULL);
    if (ret != OT_SUCCESS) {
        printf("create save frame thread failed!\n");
        g_save_file_pthread = OT_FALSE;
        return OT_FAILURE;
    }

    printf("threads running, print any key to exit!\n");
    getchar();

    g_save_file_pthread = OT_FALSE;
    pthread_join(save_pid, OT_NULL);
    g_send_chn_pthread = OT_FALSE;
    pthread_join(thread_id, OT_NULL);

    return ret;
}

ot_u64 g_max_tt = 0;

static ot_void *sample_vproc_frame_test_thread(ot_void *param)
{
    ot_s32 ret;
    ot_s32 milli_sec = -1;
    sample_vproc_attr *sample_attr = (sample_vproc_attr *)param;
    ot_eis_handle vproc_chn_hdl = sample_attr->chn_hdl[2];
    ot_eis_video_frame video_frame = { 0 };
    media_vproc_stats stt;
    ot_u64 prv, curr, tt;
    ot_u32 cnt = 0;

    sample_print(" frame test thread run \n");

    while (g_vproc_set_pthread) {
        ret = ot_eis_vproc_chn_acquire_frame(vproc_chn_hdl, &video_frame, milli_sec);
        if (ret != OT_SUCCESS) {
            ot_eis_vproc_chn_get_stats(vproc_chn_hdl, &stt);
            sample_print("chn 2 acq try/succ %d %d rls try/succ %d %d \n", stt.try_acquire_cnt, stt.acquire_succ_cnt,
                    stt.try_release_cnt, stt.release_succ_cnt);
            printf("ot_eis_vi_chn_get_frame failed with %#x\n", ret);
            continue;
        }
        prv = get_time_ms();
        ret = ot_eis_vproc_chn_release_frame(vproc_chn_hdl, &video_frame);
        curr = get_time_ms();
        tt = curr - prv;
        if (tt > g_max_tt) {
            g_max_tt = tt;
            printf("chn 2 ot_eis_vproc_chn_release_frame use %lld ms \n", tt);
        }
        if (ret != OT_SUCCESS) {
            sample_print("vproc_chn_release_frame error return %d \n", ret);
        }
        cnt++;
        if (cnt % 1024 == 0) {
            ot_eis_vproc_chn_get_stats(vproc_chn_hdl, &stt);
            printf("chn 2 acq try/succ %d %d rls try/succ %d %d curr use %lld ms max %lld ms \n",
                stt.try_acquire_cnt, stt.acquire_succ_cnt, stt.try_release_cnt, stt.release_succ_cnt, tt, g_max_tt);
        }
    }
    return OT_NULL;
}

pthread_t g_set_pid_test[10];

static ot_s32 sample_vproc_acq_rls_frame_start(ot_void)
{
    ot_s32 ret;
    ot_u32 i;

    g_vproc_set_pthread = OT_TRUE;
    for (i = 0; i < 10; i++) {
        ret = pthread_create(&g_set_pid_test[i], OT_NULL, sample_vproc_frame_test_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        usleep(50000);
    }

    return ret;
}

static ot_s32 sample_vproc_acq_rls_frame_stop(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ot_u32 i;

    g_vproc_set_pthread = OT_FALSE;
    for (i = 0; i < 10; i++) {
        pthread_join(g_set_pid_test[i], OT_NULL);
    }

    return ret;
}


static ot_s32 sample_vproc_to_vo_and_vproc_to_venc(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);
    ot_eis_handle pool_hdl = OT_NULL;
    ot_eis_img_size img_size = {0};
    sample_vo_attr *vo_attr = OT_NULL;
    ot_eis_handle media_pipe_hdl;
    ot_eis_low_delay_info lowdelay = {0};

    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    vo_attr = sample_comm_args_get_vo_attr();
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return OT_FAILURE;
    }

    // ot_eis_handle media_pipe_hdl;
    ot_bool pipe_sw[OT_EIS_VPROC_GRP_PIPE_MAX_NUM] = {OT_TRUE, OT_FALSE, OT_FALSE, OT_FALSE};
    ot_bool chn_sw[OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {OT_TRUE, OT_TRUE, OT_TRUE, OT_FALSE};

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));
    if (sample_vio_sys_init(&img_size) != OT_SUCCESS) {
        return OT_FAILURE;
    }
    sample_print_vproc_attr();

    ret = sample_vproc_user_mode_buf_pool_creat(&g_sample_vproc_attr.chn_attr[1].image_attr, &pool_hdl);
    if (ret != OT_SUCCESS) {
        goto buf_pool_creat_failed;
    }
    sample_print("sample_vproc_user_mode_buf_pool_creat pool hdl 0x%llx \n", (ot_u64)pool_hdl);

    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }

    // vproc
    chn_sw[0] = OT_FALSE;
    ret = sample_vio_vproc_for_user_mode(&g_sample_vproc_attr, chn_sw);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }
    g_sample_vproc_attr.chn_attr[1].pool_handle = pool_hdl;
    //g_sample_vproc_attr.chn_attr[1].frc.src_frame_rate = 60;
    //g_sample_vproc_attr.chn_attr[1].frc.dst_frame_rate = 17;
    g_sample_vproc_attr.chn_attr[1].image_attr.width = 1280;
    g_sample_vproc_attr.chn_attr[1].image_attr.height = 800;
    g_sample_vproc_attr.chn_attr[2].frc.src_frame_rate = 60;
    g_sample_vproc_attr.chn_attr[2].frc.dst_frame_rate = 17;

    sample_print_vproc_attr();
    chn_sw[0] = OT_TRUE;
    ret = sample_comm_start_vproc(&g_sample_vproc_attr, pipe_sw, chn_sw);
    if (ret != OT_SUCCESS) {
        goto start_vproc_failed;
    }

    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_comm_media_pipe_init(&media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto pipe_start_failed;
    }

    ret = sample_comm_vi_bind_vproc(vi_cfg.pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto vi_vproc_bind_failed;
    }
    lowdelay.enable = OT_TRUE;
    lowdelay.line_cnt= 32;
    lowdelay.one_buf_en = OT_FALSE;

    ret = ot_eis_vproc_chn_set_low_delay(g_sample_vproc_attr.chn_hdl[0], &lowdelay);
    if (ret != OT_SUCCESS) {
        goto vproc_bind_vo_failed;
    }

    ret = sample_comm_vproc_bind_vo(g_sample_vproc_attr.chn_hdl[0], vo_attr->in_port_handle[0], media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto vproc_bind_vo_failed;
    }
    vi_cfg.media_pipe_hdl = media_pipe_hdl;
    g_sample_venc_attr.venc_attr.common_attr.max_pic_width = g_sample_vproc_attr.chn_attr[1].image_attr.width;
    g_sample_venc_attr.venc_attr.common_attr.max_pic_height = g_sample_vproc_attr.chn_attr[1].image_attr.height;
    g_sample_venc_attr.venc_attr.common_attr.out_width = g_sample_vproc_attr.chn_attr[1].image_attr.width;
    g_sample_venc_attr.venc_attr.common_attr.out_height = g_sample_vproc_attr.chn_attr[1].image_attr.height;

    ret = sample_comm_venc_start(&g_sample_venc_attr);
    if (ret != OT_SUCCESS) {
        goto start_venc_failed;
    }

    sample_vproc_acq_rls_frame_start();

    ret = sample_vproc_frame_to_venc_to_file(&g_sample_vproc_attr, &g_sample_venc_attr);

    // send frame
    // ret = sample_comm_vi_get_vproc_send_frame(&g_sample_vproc_attr, pipe_sw, chn_sw, vi_cfg.chn_hdl, vo_attr->in_port_handle);
    if (ret != OT_SUCCESS) {
        goto start_venc_failed;
    }
    sample_get_char();
    sample_vproc_acq_rls_frame_stop();

start_venc_failed:
    sample_comm_venc_stop(&g_sample_venc_attr);

vproc_bind_vo_failed:
    sample_comm_vi_un_bind_vproc(vi_cfg.pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
vi_vproc_bind_failed:
    sample_comm_media_pipe_stop(media_pipe_hdl);
pipe_start_failed:
    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_stop_vproc(&g_sample_vproc_attr);
start_vproc_failed:
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:
    sample_vproc_user_mode_buf_pool_destroy(pool_hdl);
buf_pool_creat_failed:
    sample_comm_sys_exit();

    return ret;
}


static ot_void sample_vio_usage(const char *prg_name)
{
    printf("usage : %s <index> \n", prg_name);
    printf("index:\n");
    printf("    (0) all mode route          :vi linear(Online/Offline) -> vproc(Online/Offline) -> venc && vo.\n");
    printf("    (1) vproc get & send frame   :vi chn get frame -> vproc pipe send frame -> vproc chn send frame -> vo.\n");
    printf("    (2) vproc crop               :vi -> vproc -> vo.\n");
    printf("    (3) vproc flip/mirror        :vi -> vproc -> vo.\n");
    printf("    (4) vproc mosaic             :vi -> vproc -> vo.\n");
    printf("    (5) vproc line               :vi -> vproc -> vo.\n");
    printf("    (6) vproc cover              :vi -> vproc -> vo.\n");
    printf("    (7) vproc rect               :vi -> vproc -> vo.\n");
    printf("    (8) vproc osd                :vi -> vproc -> vo.\n");
    printf("    (9) vproc fisheye            :vi -> vproc -> vo.\n");
    printf("    (10) vproc ldc               :vi -> vproc -> vo.\n");
    printf("    (11) vproc pmf               :vi -> vproc -> vo.\n");
    printf("    (12) vproc 3dnr              :vi -> vproc -> vo.\n");
    printf("    (13) vproc attach            :vi -> vproc -> vo.\n");
    printf("    (14) vproc vo & venc         :vi -> vproc -> vo.\n");
    printf("                                          |--> venc.\n");
    printf("    (15) vproc vproc             :vi -> vproc -> vproc -> vo.\n");
    printf("    (16) vproc luma              :vi -> vproc -> vo.\n");
    printf("    (17) frame dma copy          :vi -> vproc -> vo.\n");
    printf("    (18) gdc 2dlut 16x16         :file -> vproc -> file.\n");
    sample_comm_args_help();
    printf("\neg: %s 0 -i 0 -o 0\n\n", prg_name);
}

static ot_void sample_vio_handle_sig(ot_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_sig_flag = 1;
    }
}

static ot_void sample_register_sig_handler(ot_void (*sig_handle)(ot_s32))
{
    struct sigaction sa;

    (ot_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handle;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, OT_NULL);
    sigaction(SIGTERM, &sa, OT_NULL);
}


static ot_s32 sample_vio_vproc_crop(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);
    ot_eis_vproc_chn_attr chn_attr;
    ot_eis_vproc_crop_param crop_param;
    ot_u64 start = 0;
    ot_u64 end = 0;
    ot_u32 i, j;
    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);

    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));
    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.crop_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.crop_param.crop_type = OT_EIS_COORD_ABS;
    g_sample_vproc_attr.set_attr.crop_param.crop_rect.x = 0;
    g_sample_vproc_attr.set_attr.crop_param.crop_rect.y = 0;
    g_sample_vproc_attr.set_attr.crop_param.crop_rect.width = 1280;
    g_sample_vproc_attr.set_attr.crop_param.crop_rect.height = 720;
    sample_print_vproc_attr();

    if (sample_vio_start_route(&vi_cfg, 1) != OT_SUCCESS) {
        return ret;
    }
    sample_print("ot_eis_vproc_chn_set_scale_coef type %d\n", g_sample_vproc_attr.set_attr.scale_coef_param.scale_coef_type);
    ret = ot_eis_vproc_chn_set_scale_coef(g_sample_vproc_attr.chn_hdl[0], &(g_sample_vproc_attr.set_attr.scale_coef_param));
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_chn_set_scale_coef failed with %#x\n", ret);
    }
    ret = ot_eis_vproc_chn_get_scale_coef(g_sample_vproc_attr.chn_hdl[0], &(g_sample_vproc_attr.set_attr.scale_coef_param));
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_chn_set_scale_coef failed with %#x\n", ret);
    }
    sample_print("ot_eis_vproc_chn_get_scale_coef type %d\n", g_sample_vproc_attr.set_attr.scale_coef_param.scale_coef_type);
    sample_get_char();
    sample_print("grp_set_crop disable\n");
    g_sample_vproc_attr.set_attr.crop_param.enable = OT_FALSE;
    ret = ot_eis_vproc_grp_set_crop(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.crop_param);
    sample_get_char();
    sample_print("grp_set_crop enable \n");
    g_sample_vproc_attr.set_attr.crop_param.enable = OT_TRUE;
    ret = ot_eis_vproc_grp_set_crop(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.crop_param);
    sample_get_char();
    sample_print("grp_set_crop disable\n");
    g_sample_vproc_attr.set_attr.crop_param.enable = OT_FALSE;
    ret = ot_eis_vproc_grp_set_crop(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.crop_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_crop error ret 0x%x \n", ret);
    }
    sample_get_char();
    sample_print("chn_set_crop enable\n");
    g_sample_vproc_attr.set_attr.crop_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_crop(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.crop_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_crop error ret 0x%x \n", ret);
    }
    sample_get_char();

    crop_param = g_sample_vproc_attr.set_attr.crop_param;
    g_sample_vproc_attr.set_attr.crop_param.enable = OT_TRUE;
    chn_attr = g_sample_vproc_attr.chn_attr[0];
    chn_attr.image_attr.width = 1920;
    chn_attr.image_attr.height = 1080;
    crop_param.crop_rect.width = 1280;
    crop_param.crop_rect.height = 720;
    sample_print("scaler crop test 1 begin \n");
    for (j = 0; j < 10; j++) {
         for (i = 0; i < 20; i++) {
            chn_attr.image_attr.width -= 32;
            chn_attr.image_attr.height -= 18;
            crop_param.crop_rect.x = chn_attr.image_attr.width - crop_param.crop_rect.width;
            crop_param.crop_rect.y = chn_attr.image_attr.height - crop_param.crop_rect.height;
            ret = ot_eis_vproc_chn_set_crop(g_sample_vproc_attr.chn_hdl[0], &crop_param);
            if (ret != OT_SUCCESS) {
                sample_print("vproc_chn_set_crop error ret 0x%x \n", ret);
            }
            ret = ot_eis_vproc_chn_set_attr(g_sample_vproc_attr.chn_hdl[0], &chn_attr);
            if (ret != OT_SUCCESS) {
                sample_print("vproc_chn_set_attr error ret 0x%x \n", ret);
            }
            usleep(200000);
        }
        for (i = 0; i < 20; i++) {
            chn_attr.image_attr.width += 32;
            chn_attr.image_attr.height += 18;
            crop_param.crop_rect.x = chn_attr.image_attr.width - crop_param.crop_rect.width;
            crop_param.crop_rect.y = chn_attr.image_attr.height - crop_param.crop_rect.height;
            ret = ot_eis_vproc_chn_set_crop(g_sample_vproc_attr.chn_hdl[0], &crop_param);
            if (ret != OT_SUCCESS) {
                sample_print("vproc_chn_set_crop error ret 0x%x \n", ret);
            }
            ret = ot_eis_vproc_chn_set_attr(g_sample_vproc_attr.chn_hdl[0], &chn_attr);
            if (ret != OT_SUCCESS) {
                sample_print("vproc_chn_set_attr error ret 0x%x \n", ret);
            }
            usleep(200000);
        }
    }
    sample_print("scaler crop test 1 end \n");
    sample_get_char();
    chn_attr.image_attr.width = 1920;
    chn_attr.image_attr.height = 1080;
    crop_param.crop_rect.width = 1280;
    crop_param.crop_rect.height = 720;
    sample_print("scaler crop test 2 begin \n");
    for (j = 0; j < 10; j++) {
         for (i = 0; i < 20; i++) {
            chn_attr.image_attr.width -= 32;
            chn_attr.image_attr.height -= 18;
            crop_param.crop_rect.x = chn_attr.image_attr.width - crop_param.crop_rect.width;
            crop_param.crop_rect.y = chn_attr.image_attr.height - crop_param.crop_rect.height;
            ret = ot_eis_vproc_chn_disable(g_sample_vproc_attr.chn_hdl[0]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_chn_disable error ret 0x%x \n", ret);
            }
            ret = ot_eis_vproc_chn_set_crop(g_sample_vproc_attr.chn_hdl[0], &crop_param);
            if (ret != OT_SUCCESS) {
                sample_print("vproc_chn_set_crop error ret 0x%x \n", ret);
            }
            ret = ot_eis_vproc_chn_set_attr(g_sample_vproc_attr.chn_hdl[0], &chn_attr);
            if (ret != OT_SUCCESS) {
                sample_print("vproc_chn_set_attr error ret 0x%x \n", ret);
            }
            ret = ot_eis_vproc_chn_enable(g_sample_vproc_attr.chn_hdl[0]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_chn_enable error ret 0x%x \n", ret);
            }
            usleep(200000);
        }
        for (i = 0; i < 20; i++) {
            chn_attr.image_attr.width += 32;
            chn_attr.image_attr.height += 18;
            crop_param.crop_rect.x = chn_attr.image_attr.width - crop_param.crop_rect.width;
            crop_param.crop_rect.y = chn_attr.image_attr.height - crop_param.crop_rect.height;
            ret = ot_eis_vproc_chn_disable(g_sample_vproc_attr.chn_hdl[0]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_chn_disable error ret 0x%x \n", ret);
            }
            ret = ot_eis_vproc_chn_set_crop(g_sample_vproc_attr.chn_hdl[0], &crop_param);
            if (ret != OT_SUCCESS) {
                sample_print("vproc_chn_set_crop error ret 0x%x \n", ret);
            }
            ret = ot_eis_vproc_chn_set_attr(g_sample_vproc_attr.chn_hdl[0], &chn_attr);
            if (ret != OT_SUCCESS) {
                sample_print("vproc_chn_set_attr error ret 0x%x \n", ret);
            }
            ret = ot_eis_vproc_chn_enable(g_sample_vproc_attr.chn_hdl[0]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_chn_enable error ret 0x%x \n", ret);
            }
            usleep(200000);
        }
    }
    sample_print("scaler crop test 2 end \n");

    sample_get_char();
    crop_param = g_sample_vproc_attr.set_attr.crop_param;
    g_sample_vproc_attr.set_attr.crop_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_crop(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.crop_param);
    chn_attr.image_attr.width = 1920;
    chn_attr.image_attr.height = 1080;
    crop_param.crop_rect.width = 1280;
    crop_param.crop_rect.height = 720;
    sample_print("ot_eis_vproc_chn_set_attr (1920 1080) crop (1280 720)\n");
    ret = ot_eis_vproc_chn_set_crop(g_sample_vproc_attr.chn_hdl[0], &crop_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_crop error ret 0x%x \n", ret);
    }
    usleep(20000);
    ret = ot_eis_vproc_chn_set_attr(g_sample_vproc_attr.chn_hdl[0], &chn_attr);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_attr error ret 0x%x \n", ret);
    }

    sample_get_char();
    chn_attr.image_attr.width = 1280;
    chn_attr.image_attr.height = 720;
    crop_param.crop_rect.width = 1920;
    crop_param.crop_rect.height = 1080;

    sample_print("ot_eis_vproc_chn_set_attr (1280 720) crop(1920 1080)\n");
    ot_eis_media_get_cur_pts(&start);

    ot_eis_vproc_grp_stop(g_sample_vproc_attr.grp_hdl);
    ret = ot_eis_vproc_chn_set_attr(g_sample_vproc_attr.chn_hdl[0], &chn_attr);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_attr error ret 0x%x \n", ret);
    }
    //usleep(20000);
    ret = ot_eis_vproc_chn_set_crop(g_sample_vproc_attr.chn_hdl[0], &crop_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_crop error ret 0x%x \n", ret);
    }
    ot_eis_vproc_grp_start(g_sample_vproc_attr.grp_hdl);
    ot_eis_media_get_cur_pts(&end);
    sample_print("vproc stop start use %lld us \n", end - start);

    sample_get_char();

    chn_attr = g_sample_vproc_attr.chn_attr[0];

    sample_print("ot_eis_vproc_chn_set_attr (1280 720)\n");
    ret = ot_eis_vproc_chn_set_attr(g_sample_vproc_attr.chn_hdl[0], &chn_attr);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_attr error ret 0x%x \n", ret);
    }
    sample_get_char();
    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_s32 sample_vio_vproc_mirror(ot_void)
{
    ot_s32 ret;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.mirror_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.flip_param.enable = OT_TRUE;
    sample_print_vproc_attr();
    if (sample_vio_start_route(&vi_cfg, 1) != OT_SUCCESS) {
        return ret;
    }

    sample_get_char();
    g_sample_vproc_attr.set_attr.flip_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_flip(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.flip_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_flip error ret 0x%x \n", ret);
    }
    g_sample_vproc_attr.set_attr.mirror_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_mirror(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.mirror_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_mirror error ret 0x%x \n", ret);
    }
    sample_get_char();
    g_sample_vproc_attr.set_attr.flip_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_flip(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.flip_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_flip error ret 0x%x \n", ret);
    }
    g_sample_vproc_attr.set_attr.mirror_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_mirror(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.mirror_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_mirror error ret 0x%x \n", ret);
    }

    sample_get_char();

    printf("set chn attr \n");
    ret = ot_eis_vproc_chn_set_attr(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.chn_attr[0]);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_attr error ret 0x%x \n", ret);
    }
    sample_get_char();
    g_sample_vproc_attr.chn_attr[0].image_attr.width /= 2;
    g_sample_vproc_attr.chn_attr[0].image_attr.height /= 2;
    ret = ot_eis_vproc_chn_set_attr(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.chn_attr[0]);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_attr error ret 0x%x \n", ret);
    }
    sample_get_char();
    g_sample_vproc_attr.set_attr.flip_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_flip(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.flip_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_flip error ret 0x%x \n", ret);
    }
    g_sample_vproc_attr.set_attr.mirror_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_mirror(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.mirror_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_mirror error ret 0x%x \n", ret);
    }
    ot_eis_vproc_chn_rotation rotation_param = { 0 };
    rotation_param.enable = OT_TRUE;
    rotation_param.angle = OT_EIS_RTT_90;
    ret = ot_eis_vproc_chn_set_rotation(g_sample_vproc_attr.chn_hdl[0], &rotation_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rotation error ret 0x%x \n", ret);
    }
    sample_get_char();
    rotation_param.angle = OT_EIS_RTT_180;
    ret = ot_eis_vproc_chn_set_rotation(g_sample_vproc_attr.chn_hdl[0], &rotation_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rotation error ret 0x%x \n", ret);
    }
    sample_get_char();
    rotation_param.angle = OT_EIS_RTT_270;
    ret = ot_eis_vproc_chn_set_rotation(g_sample_vproc_attr.chn_hdl[0], &rotation_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rotation error ret 0x%x \n", ret);
    }
    sample_get_char();
    rotation_param.angle = OT_EIS_RTT_0;
    ret = ot_eis_vproc_chn_set_rotation(g_sample_vproc_attr.chn_hdl[0], &rotation_param);
    if (ret != OT_SUCCESS) {
    	sample_print("vproc_chn_set_rotation error ret 0x%x \n", ret);
    }

    ret = ot_eis_vproc_chn_get_rotation(g_sample_vproc_attr.chn_hdl[0], &rotation_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rotation error ret 0x%x \n", ret);
    }
    sample_get_char();

    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_void *sample_vproc_set_mosaic_thread(ot_void *param)
{
    ot_s32 ret;
    ot_u32 i;
    sample_vproc_attr *sample_attr = (sample_vproc_attr *)param;
    ot_u32 w = sample_attr->chn_attr[0].image_attr.width;
    ot_u32 h = sample_attr->chn_attr[0].image_attr.height;
    ot_eis_vproc_mosaic_param mosaic_param;
    ot_eis_mosaic mosaic[12];

    mosaic[0].blk_size = OT_EIS_MOSAIC_BLK_SIZE_16;
    mosaic[0].rect.x = 100;
    mosaic[0].rect.y = 100;
    mosaic[0].rect.width = 80;
    mosaic[0].rect.height = 60;
    mosaic[1].blk_size = OT_EIS_MOSAIC_BLK_SIZE_32;
    mosaic[1].rect.x = 500;
    mosaic[1].rect.y = 100;
    mosaic[1].rect.width = 160;
    mosaic[1].rect.height = 120;

    for (i = 1; i < 6; i++) {
        mosaic[2 * i + 0] = mosaic[0];
        mosaic[2 * i + 1] = mosaic[1];
    }

    while (g_vproc_set_pthread) {
        for (i = 0; i < 12; i++) {
            mosaic[i].rect.x += (rand() % 10) * 4;
            if (mosaic[i].rect.x > w) {
                mosaic[i].rect.x = 0;
            }
            mosaic[i].rect.y += (rand() % 8) * 2;
            if (mosaic[i].rect.y > h) {
                mosaic[i].rect.y = 0;
            }
        }
        mosaic_param.enable = OT_TRUE;
        mosaic_param.num = 12;
        mosaic_param.mosaic = &mosaic[0];
        ret = ot_eis_vproc_chn_set_mosaic(sample_attr->chn_hdl[0], &mosaic_param);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vproc_chn_set_rect error 0x%x \n", ret);
        }
        sleep(1);
    }

    return OT_NULL;
}


static ot_s32 sample_vio_vproc_mosaic(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ot_u32 i = 0;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.mosaic_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.mosaic_param.num = 2;
    ot_eis_mosaic mosaic[2];
    mosaic[0].blk_size = OT_EIS_MOSAIC_BLK_SIZE_16;
    mosaic[0].rect.x = 100;
    mosaic[0].rect.y = 100;
    mosaic[0].rect.width = 300;
    mosaic[0].rect.height = 300;
    mosaic[1].blk_size = OT_EIS_MOSAIC_BLK_SIZE_32;
    mosaic[1].rect.x = 500;
    mosaic[1].rect.y = 100;
    mosaic[1].rect.width = 200;
    mosaic[1].rect.height = 200;
    g_sample_vproc_attr.set_attr.mosaic_param.mosaic = &mosaic[0];
    sample_print_vproc_attr();
    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }
    sample_get_char();
    pthread_t set_pid[20];
    g_vproc_set_pthread = OT_TRUE;
    for (i = 0; i < 20; i++) {
        ret = pthread_create(&set_pid[i], OT_NULL, sample_vproc_set_mosaic_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        usleep(50000);
    }
    sample_get_char();
    g_vproc_set_pthread = OT_FALSE;
    for (i = 0; i < 20; i++) {
        pthread_join(set_pid[i], OT_NULL);
    }
    sample_get_char();
    g_sample_vproc_attr.set_attr.mosaic_param.mosaic = &mosaic[0];
    g_sample_vproc_attr.set_attr.mosaic_param.num = 2;
    g_sample_vproc_attr.set_attr.mosaic_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_mosaic(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.mosaic_param);
    sample_print("set mosaic ret 0x%x \n", ret);
    sample_get_char();
    g_sample_vproc_attr.set_attr.mosaic_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_mosaic(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.mosaic_param);
    sample_print("set mosaic ret 0x%x \n", ret);
    sample_get_char();
    ret = ot_eis_vproc_chn_get_mosaic(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.mosaic_param);
    sample_print("get mosaic ret 0x%x \n", ret);
    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_void *sample_vproc_set_line_thread(ot_void *param)
{
    ot_s32 ret;
    ot_u32 i;
    sample_vproc_attr *sample_attr = (sample_vproc_attr *)param;
    ot_u32 w = sample_attr->chn_attr[0].image_attr.width;
    ot_u32 h = sample_attr->chn_attr[0].image_attr.height;
    ot_eis_vproc_line_param line_param;
    ot_eis_line line[12];

    line[0].color = 0xff0000;
    line[0].thickness = 4;
    line[0].point[0].x = 100;
    line[0].point[0].y = 300;
    line[0].point[1].x = 300;
    line[0].point[1].y = 600;
    line[1].color = 0x00ff00;
    line[1].thickness = 4;
    line[1].point[0].x = 500;
    line[1].point[0].y = 400;
    line[1].point[1].x = 800;
    line[1].point[1].y = 300;

    for (i = 1; i < 6; i++) {
        line[2 * i + 0] = line[0];
        line[2 * i + 1] = line[1];
    }

    while (g_vproc_set_pthread) {
        for (i = 0; i < 12; i++) {
            for (ot_u32 j = 0; j < 2; j++) {
                line[i].point[j].x += (rand() % 10) * 2;
                if (line[i].point[j].x > w) {
                    line[i].point[j].x = 0;
                }
                line[i].point[j].y += (rand() % 8) * 2;
                if (line[i].point[j].y > h) {
                    line[i].point[j].y = 0;
                }
            }
            if (line[i].point[0].x == line[i].point[1].x && line[i].point[0].y == line[i].point[1].y) {
                line[i].point[0].x += (rand() % 10) * 2;
                if (line[i].point[0].x > w) {
                    line[i].point[0].x = 0;
                }
                line[i].point[0].y += (rand() % 8) * 2;
                if (line[i].point[0].y > h) {
                    line[i].point[0].y = 0;
                }
            }
        }
        line_param.enable = OT_TRUE;
        line_param.num = 12;
        line_param.line = &line[0];
        ret = ot_eis_vproc_chn_set_line(sample_attr->chn_hdl[0], &line_param);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vproc_chn_set_line error 0x%x \n", ret);
        }
        sleep(1);
    }

    return OT_NULL;
}


static ot_s32 sample_vio_vproc_line(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ot_u32 i;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.line_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.line_param.num = 2;
    ot_eis_line line[2];
    line[0].color = 0xff0000;
    line[0].thickness = 6;
    line[0].point[0].x = 100;
    line[0].point[0].y = 300;
    line[0].point[1].x = 300;
    line[0].point[1].y = 600;
    line[1].color = 0x00ff00;
    line[1].thickness = 10;
    line[1].point[0].x = 500;
    line[1].point[0].y = 400;
    line[1].point[1].x = 800;
    line[1].point[1].y = 300;
    g_sample_vproc_attr.set_attr.line_param.line = &line[0];
    sample_print_vproc_attr();
    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }
    sample_get_char();
    pthread_t set_pid[20];
    g_vproc_set_pthread = OT_TRUE;
    for (i = 0; i < 20; i++) {
        ret = pthread_create(&set_pid[i], OT_NULL, sample_vproc_set_line_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        usleep(50000);
    }
    sample_get_char();
    g_vproc_set_pthread = OT_FALSE;
    for (i = 0; i < 20; i++) {
        pthread_join(set_pid[i], OT_NULL);
    }

    sample_get_char();
    g_sample_vproc_attr.set_attr.line_param.num = 2;
    g_sample_vproc_attr.set_attr.line_param.line = &line[0];
    g_sample_vproc_attr.set_attr.line_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_line(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.line_param);
    sample_print("set line ret 0x%x \n", ret);
    sample_get_char();
    g_sample_vproc_attr.set_attr.line_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_line(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.line_param);
    sample_print("set line ret 0x%x \n", ret);
    sample_get_char();
    ret = ot_eis_vproc_chn_get_line(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.line_param);
    sample_print("get line ret 0x%x \n", ret);

    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_void *sample_vproc_set_cover_thread(ot_void *param)
{
    ot_s32 ret;
    ot_u32 i;
    sample_vproc_attr *sample_attr = (sample_vproc_attr *)param;
    ot_u32 w = sample_attr->chn_attr[0].image_attr.width;
    ot_u32 h = sample_attr->chn_attr[0].image_attr.height;
    ot_eis_vproc_cover_param cover_param;
    ot_eis_cover cover[8];

    cover[0].color = 0xff00ff;
    cover[0].thick = 4;
    cover[0].is_hollow = OT_TRUE;
    cover[0].point[0].x = 100;
    cover[0].point[0].y = 80;
    cover[0].point[1].x = 200;
    cover[0].point[1].y = 80;
    cover[0].point[2].x = 100;
    cover[0].point[2].y = 300;
    cover[0].point[3].x = 240;
    cover[0].point[3].y = 300;

    cover[1].color = 0x00ffff;
    cover[1].thick = 4;
    cover[1].is_hollow = OT_TRUE;
    cover[1].point[0].x = 500;
    cover[1].point[0].y = 400;
    cover[1].point[1].x = 700;
    cover[1].point[1].y = 400;
    cover[1].point[2].x = 500;
    cover[1].point[2].y = 560;
    cover[1].point[3].x = 700;
    cover[1].point[3].y = 560;

    for (i = 1; i < 4; i++) {
        cover[2 * i + 0] = cover[0];
        cover[2 * i + 1] = cover[1];
    }

    while (g_vproc_set_pthread) {
        for (i = 0; i < 8; i++) {
            for (ot_u32 j = 0; j < 4; j++) {
                cover[i].point[j].x += (rand() % 10) * 2;
                if (cover[i].point[j].x > w) {
                    cover[i].point[j].x = 0;
                }
                cover[i].point[j].y += (rand() % 8) * 2;
                if (cover[i].point[j].y > h) {
                    cover[i].point[j].y = 0;
                }
            }
        }
        cover_param.enable = OT_TRUE;
        cover_param.num = 8;
        cover_param.cover = &cover[0];
        ret = ot_eis_vproc_chn_set_cover(sample_attr->chn_hdl[0], &cover_param);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vproc_chn_set_cover error 0x%x \n", ret);
        }
        sleep(1);
    }

    return OT_NULL;
}


static ot_s32 sample_vio_vproc_cover(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ot_u32 i;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.cover_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.cover_param.num = 2;
    ot_eis_cover cover[2];
    cover[0].color = 0xff0000;
    cover[0].thick = 6;
    cover[0].is_hollow = OT_FALSE;
    cover[0].point[0].x = 100;
    cover[0].point[0].y = 300;
    cover[0].point[1].x = 100;
    cover[0].point[1].y = 600;
    cover[0].point[2].x = 500;
    cover[0].point[2].y = 300;
    cover[0].point[3].x = 500;
    cover[0].point[3].y = 600;

    cover[1].color = 0x00ff00;
    cover[1].thick = 10;
    cover[1].is_hollow = OT_TRUE;
    cover[1].point[0].x = 500;
    cover[1].point[0].y = 400;
    cover[1].point[1].x = 1000;
    cover[1].point[1].y = 400;
    cover[1].point[2].x = 500;
    cover[1].point[2].y = 700;
    cover[1].point[3].x = 800;
    cover[1].point[3].y = 700;
    g_sample_vproc_attr.set_attr.cover_param.cover = &cover[0];
    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }

    sample_get_char();
    pthread_t set_pid[20];
    g_vproc_set_pthread = OT_TRUE;
    for (i = 0; i < 20; i++) {
        ret = pthread_create(&set_pid[i], OT_NULL, sample_vproc_set_cover_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        usleep(50000);
    }
    sample_get_char();
    g_vproc_set_pthread = OT_FALSE;
    for (i = 0; i < 20; i++) {
        pthread_join(set_pid[i], OT_NULL);
    }

    sample_get_char();
    g_sample_vproc_attr.set_attr.cover_param.num = 2;
    g_sample_vproc_attr.set_attr.cover_param.cover = &cover[0];
    g_sample_vproc_attr.set_attr.cover_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_cover(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.cover_param);
    sample_print("set cover ret 0x%x \n", ret);
    sample_get_char();
    g_sample_vproc_attr.set_attr.cover_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_cover(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.cover_param);
    sample_print("set cover ret 0x%x \n", ret);
    sample_get_char();
    ret = ot_eis_vproc_chn_get_cover(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.cover_param);
    sample_print("get cover ret 0x%x \n", ret);

    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_void *sample_vproc_set_rect_thread(ot_void *param)
{
    ot_s32 ret;
    ot_u32 i;
    sample_vproc_attr *sample_attr = (sample_vproc_attr *)param;
    ot_u32 w = sample_attr->chn_attr[0].image_attr.width;
    ot_u32 h = sample_attr->chn_attr[0].image_attr.height;
    ot_eis_vproc_rect_param rect_param;
    ot_eis_vproc_rect rect[16];

    rect[0].color = 0xff0000;
    rect[0].thick = 4;
    rect[0].is_corner = OT_FALSE;
    rect[0].rect.x = 100;
    rect[0].rect.y = 300;
    rect[0].rect.width = 200;
    rect[0].rect.height = 120;
    rect[1].color = 0x00ff00;
    rect[1].thick = 4;
    rect[1].is_corner = OT_FALSE;
    rect[1].rect.x = 120;
    rect[1].rect.y = 400;
    rect[1].rect.width = 200;
    rect[1].rect.height = 120;
    rect[2].color = 0x0000ff;
    rect[2].thick = 4;
    rect[2].is_corner = OT_FALSE;
    rect[2].rect.x = 140;
    rect[2].rect.y = 420;
    rect[2].rect.width = 200;
    rect[2].rect.height = 120;
    rect[3].color = 0xffff00;
    rect[3].thick = 4;
    rect[3].is_corner = OT_FALSE;
    rect[3].rect.x = 160;
    rect[3].rect.y = 440;
    rect[3].rect.width = 200;
    rect[3].rect.height = 120;
    rect[4].color = 0xff00ff;
    rect[4].thick = 4;
    rect[4].is_corner = OT_FALSE;
    rect[4].rect.x = 180;
    rect[4].rect.y = 460;
    rect[4].rect.width = 200;
    rect[4].rect.height = 120;
    rect[5].color = 0x00ffff;
    rect[5].thick = 4;
    rect[5].is_corner = OT_FALSE;
    rect[5].rect.x = 200;
    rect[5].rect.y = 480;
    rect[5].rect.width = 200;
    rect[5].rect.height = 120;
    rect[6].color = 0xffffff;
    rect[6].thick = 4;
    rect[6].is_corner = OT_FALSE;
    rect[6].rect.x = 220;
    rect[6].rect.y = 500;
    rect[6].rect.width = 200;
    rect[6].rect.height = 120;

    rect[7].color = 0x000000;
    rect[7].thick = 4;
    rect[7].is_corner = OT_FALSE;
    rect[7].rect.x = 240;
    rect[7].rect.y = 520;
    rect[7].rect.width = 200;
    rect[7].rect.height = 120;

    rect[8].color = 0x8f0000;
    rect[8].thick = 4;
    rect[8].is_corner = OT_FALSE;
    rect[8].rect.x = 260;
    rect[8].rect.y = 540;
    rect[8].rect.width = 200;
    rect[8].rect.height = 120;

    rect[9].color = 0x008f00;
    rect[9].thick = 4;
    rect[9].is_corner = OT_FALSE;
    rect[9].rect.x = 280;
    rect[9].rect.y = 560;
    rect[9].rect.width = 200;
    rect[9].rect.height = 120;

    rect[10].color = 0x008f00;
    rect[10].thick = 4;
    rect[10].is_corner = OT_FALSE;
    rect[10].rect.x = 280;
    rect[10].rect.y = 560;
    rect[10].rect.width = 200;
    rect[10].rect.height = 120;

    rect[11].color = 0x000f8f;
    rect[11].thick = 4;
    rect[11].is_corner = OT_FALSE;
    rect[11].rect.x = 280;
    rect[11].rect.y = 560;
    rect[11].rect.width = 200;
    rect[11].rect.height = 120;

    rect[12].color = 0x008f0f;
    rect[12].thick = 4;
    rect[12].is_corner = OT_FALSE;
    rect[12].rect.x = 280;
    rect[12].rect.y = 560;
    rect[12].rect.width = 200;
    rect[12].rect.height = 120;

    rect[13].color = 0x00f08f;
    rect[13].thick = 4;
    rect[13].is_corner = OT_FALSE;
    rect[13].rect.x = 560;
    rect[13].rect.y = 300;
    rect[13].rect.width = 200;
    rect[13].rect.height = 120;

    rect[14].color = 0x008f8f;
    rect[14].thick = 4;
    rect[14].is_corner = OT_FALSE;
    rect[14].rect.x = 580;
    rect[14].rect.y = 320;
    rect[14].rect.width = 200;
    rect[14].rect.height = 120;

    rect[15].color = 0x8f8f00;
    rect[15].thick = 4;
    rect[15].is_corner = OT_FALSE;
    rect[15].rect.x = 600;
    rect[15].rect.y = 340;
    rect[15].rect.width = 200;
    rect[15].rect.height = 120;

    while (g_vproc_set_pthread) {
        for (i = 0; i < 12; i++) {
            rect[i].rect.x += (rand() % 10) * 2;
            if (rect[i].rect.x > w) {
                rect[i].rect.x = 0;
            }
            rect[i].rect.y += (rand() % 8) * 2;
            if (rect[i].rect.y > h) {
                rect[i].rect.y = 0;
            }
        }
        rect_param.enable = OT_TRUE;
        rect_param.num = 12;
        rect_param.rect = &rect[0];
        ret = ot_eis_vproc_chn_set_rect(sample_attr->chn_hdl[0], &rect_param);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vproc_chn_set_rect error 0x%x \n", ret);
        }
        sleep(1);
    }

    return OT_NULL;
}


static ot_s32 sample_vio_vproc_rect(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ot_u32 i = 0;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.rect_param.num = 2;
    ot_eis_vproc_rect rect[2];
    rect[0].color = 0xff0000;
    rect[0].thick = 6;
    rect[0].is_corner = OT_FALSE;
    rect[0].rect.x = 100;
    rect[0].rect.y = 300;
    rect[0].rect.width = 300;
    rect[0].rect.height = 600;
    rect[1].color = 0x00ff00;
    rect[1].thick = 10;
    rect[1].is_corner = OT_TRUE;
    rect[1].ver_len = 60;
    rect[1].hor_len = 60;
    rect[1].rect.x = 500;
    rect[1].rect.y = 400;
    rect[1].rect.width = 800;
    rect[1].rect.height = 300;
    g_sample_vproc_attr.set_attr.rect_param.rect = &rect[0];
    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }
    sample_get_char();
    pthread_t set_pid[20];
    g_vproc_set_pthread = OT_TRUE;
    for (i = 0; i < 20; i++) {
        ret = pthread_create(&set_pid[i], OT_NULL, sample_vproc_set_rect_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        usleep(50000);
    }
    sample_get_char();
    g_vproc_set_pthread = OT_FALSE;
    for (i = 0; i < 20; i++) {
        pthread_join(set_pid[i], OT_NULL);
    }
    sample_get_char();
    g_sample_vproc_attr.set_attr.rect_param.num = 2;
    g_sample_vproc_attr.set_attr.rect_param.rect = &rect[0];
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_rect(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.rect_param);
    sample_print("set rect ret 0x%x \n", ret);
    sample_get_char();
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_rect(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.rect_param);
    sample_print("set rect ret 0x%x \n", ret);
    sample_get_char();
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_rect(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.rect_param);
    sample_print("set rect ret 0x%x \n", ret);
    sample_get_char();
    ret = ot_eis_vproc_chn_get_rect(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.rect_param);
    sample_print("get rect ret 0x%x \n", ret);

    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_void *sample_vproc_set_osd_thread(ot_void *param)
{
    ot_s32 ret;
    ot_u32 i;
    ot_u32 cnt = 0;
    ot_u32 idx = 0;
    sample_vproc_attr *sample_attr = (sample_vproc_attr *)param;
    ot_u32 w = sample_attr->chn_attr[0].image_attr.width;
    ot_u32 h = sample_attr->chn_attr[0].image_attr.height;
    ot_eis_vproc_osd_param osd_param;
    ot_eis_osd_attr osd[8];
    ot_eis_img_frame osd_frame[4];

    osd[0].osd_rgn.x = 10;
    osd[0].osd_rgn.y = 10;
    osd[0].osd_rgn.width = 162;
    osd[0].osd_rgn.height = 14;
    osd[0].color = 0xff;
    for (ot_u32 i = 0; i < OT_EIS_VPROC_OSD_LUT_NUM; i++) {
        osd[0].alpha_lut[i] = i * (256 / OT_EIS_VPROC_OSD_LUT_NUM);
    }
    sample_alloc_osd_frame(&osd_frame[0], 162, 14, OT_EIS_IMAGE_FORMAT_ARGB_8888);
    sample_read_osd_frame(&osd_frame[0], "./res/162_14_argb8888_01.argb", 0);
    sample_alloc_osd_frame(&osd_frame[1], 162, 14, OT_EIS_IMAGE_FORMAT_ARGB_8888);
    sample_read_osd_frame(&osd_frame[1], "./res/162_14_argb8888_02.argb", 0);

    osd[0].osd_frame = &osd_frame[0];

    osd[1].osd_rgn.x = 200;
    osd[1].osd_rgn.y = 100;
    osd[1].osd_rgn.width = 400;
    osd[1].osd_rgn.height = 300;
    osd[1].color = 0xff;
    for (ot_u32 i = 0; i < OT_EIS_VPROC_OSD_LUT_NUM; i++) {
        osd[1].alpha_lut[i] = i * (256 / OT_EIS_VPROC_OSD_LUT_NUM);
    }
    sample_alloc_osd_frame(&osd_frame[2], 400, 300, OT_EIS_IMAGE_FORMAT_ARGB_4444);
    sample_fill_osd_frame(&osd_frame[2]);
    osd[1].osd_frame = &osd_frame[2];

    osd[2].osd_rgn.x = 500;
    osd[2].osd_rgn.y = 400;
    osd[2].osd_rgn.width = 200;
    osd[2].osd_rgn.height = 160;
    osd[2].color = 0xff;
    for (ot_u32 i = 0; i < OT_EIS_VPROC_OSD_LUT_NUM; i++) {
        osd[2].alpha_lut[i] = i * (256 / OT_EIS_VPROC_OSD_LUT_NUM);
    }
    sample_alloc_osd_frame(&osd_frame[3], 200, 160, OT_EIS_IMAGE_FORMAT_ARGB_1555);
    sample_fill_osd_frame(&osd_frame[3]);
    osd[2].osd_frame = &osd_frame[3];

    osd[3] = osd[1];
    osd[4] = osd[2];
    osd[5] = osd[1];
    osd[6] = osd[2];
    osd[7] = osd[1];

    while (g_vproc_set_pthread) {
        cnt++;
        if (cnt % 25 == 0) {
            for (i = 1; i < 8; i++) {
                osd[i].osd_rgn.x += (rand() % 10) * 2;
                if (osd[i].osd_rgn.x > w) {
                    osd[i].osd_rgn.x = 0;
                }
                osd[i].osd_rgn.y += (rand() % 8) * 2;
                if (osd[i].osd_rgn.y > h) {
                    osd[i].osd_rgn.y = 0;
                }
            }
            idx++;
        }
        osd_param.enable = OT_TRUE;
        osd_param.num = 8;
        osd_param.osd = osd;
        osd_param.osd[0].osd_frame = &osd_frame[idx % 2];
        ret = ot_eis_vproc_chn_set_osd(sample_attr->chn_hdl[0], &osd_param);
        if (ret != OT_SUCCESS) {
            printf("ot_eis_vproc_chn_set_osd error 0x%x \n", ret);
        }
        usleep(40000);
    }
    sample_free_osd_frame(&osd_frame[0]);
    sample_free_osd_frame(&osd_frame[1]);

    return OT_NULL;
}


static ot_s32 sample_vio_vproc_osd(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ot_u32 i;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.osd_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.osd_param.num = 2;
    ot_eis_osd_attr osd[2];
    ot_eis_img_frame osd_frame[2];
    osd[0].osd_rgn.x = 100;
    osd[0].osd_rgn.y = 300;
    osd[0].osd_rgn.width = 300;
    osd[0].osd_rgn.height = 200;
    osd[0].color = 0xff0000; //RGB R
    for (ot_u32 i = 0; i < OT_EIS_VPROC_OSD_LUT_NUM; i++) {
        osd[0].alpha_lut[i] = (i + 1) * (256 / OT_EIS_VPROC_OSD_LUT_NUM) - 1;
    }
    sample_alloc_osd_frame(&osd_frame[0], 300, 128, OT_EIS_IMAGE_FORMAT_ARGB_1555);
    sample_fill_osd_frame(&osd_frame[0]);

    osd[0].osd_frame = &osd_frame[0];

    osd[1].osd_rgn.x = 500;
    osd[1].osd_rgn.y = 400;
    osd[1].osd_rgn.width = 800;
    osd[1].osd_rgn.height = 300;
    osd[1].color = 0xff0000;
    for (ot_u32 i = 0; i < OT_EIS_VPROC_OSD_LUT_NUM; i++) {
        osd[1].alpha_lut[i] = (i + 1) * (256 / OT_EIS_VPROC_OSD_LUT_NUM) - 1;
    }
    sample_alloc_osd_frame(&osd_frame[1], 800, 400, OT_EIS_IMAGE_FORMAT_ARGB_4444);
    sample_fill_osd_frame(&osd_frame[1]);
    osd[1].osd_frame = &osd_frame[1];

    g_sample_vproc_attr.set_attr.osd_param.osd = &osd[0];
    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }

    sample_get_char();
    pthread_t set_pid[20];
    g_vproc_set_pthread = OT_TRUE;
    for (i = 0; i < 20; i++) {
        ret = pthread_create(&set_pid[i], OT_NULL, sample_vproc_set_osd_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        usleep(50000);
    }
    sample_get_char();
    g_vproc_set_pthread = OT_FALSE;
    for (i = 0; i < 20; i++) {
        pthread_join(set_pid[i], OT_NULL);
    }
    sample_get_char();
    g_sample_vproc_attr.set_attr.osd_param.num = 2;
    g_sample_vproc_attr.set_attr.osd_param.osd = &osd[0];
    g_sample_vproc_attr.set_attr.osd_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_osd(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.osd_param);
    sample_print("set osd ret 0x%x \n", ret);
    sample_get_char();
    g_sample_vproc_attr.set_attr.osd_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_osd(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.osd_param);
    sample_print("set osd ret 0x%x \n", ret);
    sample_get_char();
    ret = ot_eis_vproc_chn_set_osd(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.osd_param);
    sample_print("get osd ret 0x%x \n", ret);

    sample_free_osd_frame(&osd_frame[0]);
    sample_free_osd_frame(&osd_frame[1]);
    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

ot_u16 g_lmf_coef[128] = {
    0, 15, 31, 47, 63, 79, 95, 111, 127, 143, 159, 175,
    191, 207, 223, 239, 255, 271, 286, 302, 318, 334, 350, 365, 381, 397, 412,
    428, 443, 459, 474, 490, 505, 520, 536, 551, 566, 581, 596, 611, 626, 641,
    656, 670, 685, 699, 713, 728, 742, 756, 769, 783, 797, 810, 823, 836, 848,
    861, 873, 885, 896, 908, 919, 929, 940, 950, 959, 969, 984, 998, 1013, 1027,
    1042, 1056, 1071, 1085, 1100, 1114, 1129, 1143, 1158, 1172, 1187, 1201, 1215,
    1230, 1244, 1259, 1273, 1288, 1302, 1317, 1331, 1346, 1360, 1375, 1389, 1404,
    1418, 1433, 1447, 1462, 1476, 1491, 1505, 1519, 1534, 1548, 1563, 1577, 1592,
    1606, 1621, 1635, 1650, 1664, 1679, 1693, 1708, 1722, 1737, 1751, 1766, 1780, 1795, 1809, 1823, 1838
};


static ot_s32 sample_vio_vproc_xylut(ot_void)
{
    static ot_u32 iso_arr[16];
    static ot_eis_nr_cfg nr_param[16];
    ot_eis_3dnr_attr nr_attr = { 0 };

    ot_s32 ret = OT_SUCCESS;
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);
    ot_eis_vproc_gdc_cfg gdc = {0};
    ot_eis_vproc_gdc_xylut xylut;
    ot_eis_handle pool_hdl;
    ot_u32 block_size;
    ot_video_buffer_attr pool_attr = { 0 };
    ot_eis_img_attr img_attr = {1936, 1552, OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
        OT_EIS_IMAGE_LAYOUT_LINEAR, OT_EIS_PIXEL_BIT_DEPTH_8, OT_EIS_IMAGE_COMPRESS_MODE_NONE};
    ot_eis_img_frame src, dst;
    ot_video_buffer_attr vrb_cfg;

    ret = ot_eis_media_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_media_init failed!\n");
        return OT_FAILURE;
    }
    if (sample_comm_sys_set_vi_vproc_init_cfg(g_mode_type) != OT_SUCCESS) {
        sample_print("set_vi_vproc_mode failed with %#x!\n", ret);
        return OT_FAILURE;
    }
    sample_common_get_vrb_cfg(3, 2880, 1344, &vrb_cfg);
    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    if (sample_comm_sys_set_vi_vproc_init_cfg(g_mode_type) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    if (ot_eis_media_set_3dnr_pos(g_nr_pos) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    sample_common_get_buffer_pool_cfg(&img_attr, &block_size);
    pool_attr.cnt = 1;
    pool_attr.buf_blks[0].cnt = 4;
    pool_attr.buf_blks[0].size = block_size;

    ret = ot_buffer_pool_init();
    if (ret != OT_SUCCESS && ret != OT_ERR_BUFPOOL_ALREADY_INIT) {
       sample_print("buffer_pool init fail! ret[%d]\n", ret);
       return OT_FAILURE;
    }

    ret =  ot_buffer_pool_create(&pool_hdl, &pool_attr);
    if(ret != 0){
       sample_print("buffer_pool create fail! ret[%d]\n", ret);
    }
    src.attr = img_attr;
    src.attr.width = 1936;
    src.attr.height = 1552;
    dst.attr = img_attr;
    ret = sample_vproc_comm_get_block_for_frame(pool_hdl, &src);
    if(ret != 0){
        sample_print("sample_vproc_comm_get_block_for_frame fail! ret[%d]\n", ret);
        goto pool_create_failed;
    }
    ret = ot_img_read_frame_from_file(&src, "./res/yuv420sp_1936_1552_fisheye.yuv", 0);
    if(ret != 0){
        sample_print("ot_img_read_frame_from_file fail! ret[%d]\n", ret);
        goto release_frame;
    }
    ret = sample_vproc_comm_get_block_for_frame(pool_hdl, &dst);

    ret = ot_eis_vproc_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_init failed with %#x!\n", ret);
        goto release_frame;
    }

    gdc.src.width = 1936;
    gdc.src.height = 1552;
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    g_sample_vproc_attr.grp_attr.max_width = 2880;//img_attr.width;
    g_sample_vproc_attr.grp_attr.max_height = 1552;//img_attr.height;
    g_sample_vproc_attr.set_attr.gdc_param.cfg = &gdc;
    g_sample_vproc_attr.set_attr.gdc_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.fisheye_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.xylut_param.gdc_lut = &xylut;
    g_sample_vproc_attr.set_attr.xylut_param.enable = OT_TRUE;

    ret = sample_comm_vproc_load_gdc_2dlut_file("./res/lut_2880X1344_map.bin", &g_sample_vproc_attr.set_attr.gdc_param, 
        &g_sample_vproc_attr.set_attr.xylut_param);
    if (ret != OT_SUCCESS) {
        sample_print("sample_comm_vproc_load_gdc_2dlut_file failed with %#x!\n", ret);
        goto release_frame;
    }
    g_sample_vproc_attr.pipe_attr[1].image_attr = img_attr;
    g_sample_vproc_attr.pipe_attr[1].mode = OT_EIS_VPROC_WORK_MODE_USER;
    g_sample_vproc_attr.chn_attr[1].image_attr = img_attr;
    g_sample_vproc_attr.chn_attr[1].image_attr.width = gdc.dst.width;
    g_sample_vproc_attr.chn_attr[1].image_attr.height = gdc.dst.height;
    g_sample_vproc_attr.chn_attr[1].mode = OT_EIS_VPROC_WORK_MODE_USER;
    g_sample_vproc_attr.chn_attr[1].frame_queue_depth = 2;
    sample_print_vproc_attr();
    ret = ot_eis_vproc_grp_create(0, &g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.grp_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_create failed with %#x\n", ret);
        goto release_frame;
    }
    sample_print("ot_eis_vproc_grp_create handle 0x%llx\n", (ot_u64)g_sample_vproc_attr.grp_hdl);
    ret = ot_eis_vproc_pipe_create(&g_sample_vproc_attr.pipe_hdl[1], &g_sample_vproc_attr.pipe_attr[1]);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_pipe_create failed with %#x\n", ret);
        goto vproc_stop;
    }
    sample_print("ot_eis_vproc_pipe_create handle 0x%llx\n", (ot_u64)g_sample_vproc_attr.pipe_hdl[1]);
    ret = ot_eis_vproc_grp_attach_pipe(g_sample_vproc_attr.grp_hdl, g_sample_vproc_attr.pipe_hdl[1]);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_attach_pipe failed with %#x\n", ret);
        goto vproc_stop;
    }
    ret = ot_eis_vproc_chn_create(1, &g_sample_vproc_attr.chn_hdl[1], &g_sample_vproc_attr.chn_attr[1]);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_chn_create failed with 0x%#x\n", ret);
        goto vproc_stop;
    }
    sample_print("ot_eis_vproc_chn_create handle 0x%llx\n", (ot_u64)g_sample_vproc_attr.chn_hdl[1]);
    ret = ot_eis_vproc_grp_attach_chn(g_sample_vproc_attr.grp_hdl, g_sample_vproc_attr.chn_hdl[1]);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_attach_chn failed with %#x\n", ret);
        goto vproc_stop;
    }
    ret = ot_eis_vproc_grp_start(g_sample_vproc_attr.grp_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_start failed with %#x\n", ret);
        goto vproc_stop;
    }

    for (ot_u32 i = 0; i < 16; i++) {
        iso_arr[i] = 100 + i * ((3276800 - 100) / 16);
        nr_param[i] = g_sample_nr_param;
    }

    nr_attr.enable = OT_FALSE;
    nr_attr.nr_type = OT_EIS_NR_TYPE_VIDEO_NORM;
    nr_attr.param.nr_cfg.nr_mode = OT_EIS_OP_MODE_AUTO;
    nr_attr.param.nr_cfg.nr_auto.param_num = 16;
    nr_attr.param.nr_cfg.nr_auto.iso = iso_arr;
    nr_attr.param.nr_cfg.nr_auto.nr_param = nr_param;
    ot_eis_vproc_grp_set_3dnr(g_sample_vproc_attr.grp_hdl, &nr_attr);
    ret = ot_eis_vproc_grp_set_gdc(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.gdc_param);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_set_gdc failed with %#x\n", ret);
        goto vproc_stop;
    }
    ret = ot_eis_vproc_grp_set_xylut(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.xylut_param);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_set_xylut failed with %#x\n", ret);
        goto vproc_stop;
    }

    nr_attr.enable = OT_TRUE;
    ot_eis_vproc_grp_set_3dnr(g_sample_vproc_attr.grp_hdl, &nr_attr);
    for (ot_u32 i = 0; i < 3; i++) {
        ret = ot_eis_vproc_pipe_send_frame(g_sample_vproc_attr.pipe_hdl[1], &src, 1);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_pipe_send_frame failed with %#x\n", ret);
        }
        ret = ot_eis_vproc_chn_acquire_frame(g_sample_vproc_attr.chn_hdl[1], &dst, 500);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_acquire_frame failed with %#x\n", ret);
            continue;
        }
        if (i == 0) {
            ot_img_write_frame_to_file(&dst, "./res/yvu420sp_2880_1344.yuv", OT_FALSE, OT_TRUE);
        } else {
            ot_img_write_frame_to_file(&dst, "./res/yvu420sp_2880_1344.yuv", OT_TRUE, OT_TRUE);
        }

        ret = ot_eis_vproc_chn_release_frame(g_sample_vproc_attr.chn_hdl[1], &dst);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_acquire_frame failed with %#x\n", ret);
        }
    }

vproc_stop:
    ret = ot_eis_vproc_grp_stop(g_sample_vproc_attr.grp_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_stop failed with %#x\n", ret);
    }
    ret = ot_eis_vproc_grp_detach_chn(g_sample_vproc_attr.grp_hdl, g_sample_vproc_attr.chn_hdl[1]);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_detach_chn failed with %#x\n", ret);
    }
    ret = ot_eis_vproc_chn_destroy(g_sample_vproc_attr.chn_hdl[1]);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_chn_destroy failed with 0x%#x\n", ret);
    }
    ret = ot_eis_vproc_grp_detach_pipe(g_sample_vproc_attr.grp_hdl, g_sample_vproc_attr.pipe_hdl[1]);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_detach_pipe failed with %#x\n", ret);
    }
    ret = ot_eis_vproc_pipe_destroy(g_sample_vproc_attr.pipe_hdl[1]);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_pipe_destroy failed with 0x%#x\n", ret);
    }
    ret = ot_eis_vproc_grp_destroy(g_sample_vproc_attr.grp_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_destroy failed with %#x\n", ret);
    }
    ret = ot_eis_vproc_deinit();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_deinit failed with %#x!\n", ret);
    }

release_frame:
    sample_vproc_release_frame(pool_hdl, &src);
    sample_vproc_release_frame(pool_hdl, &dst);
    ot_buffer_pool_destroy(pool_hdl);
pool_create_failed:
    ot_buffer_pool_deinit();
    sample_comm_sys_exit();

    return OT_SUCCESS;
}


static ot_s32 sample_vio_vproc_fisheye(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;
    g_nr_pos = OT_EIS_3DNR_POS_VPROC;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.gdc_param.enable = OT_TRUE;
    ot_eis_vproc_gdc_cfg gdc = { 0};
    ot_eis_vproc_fisheye_cfg fisheye;

    gdc.src.width = g_sample_vproc_attr.grp_attr.max_width;
    gdc.src.height = g_sample_vproc_attr.grp_attr.max_height;
    gdc.mid.width = g_sample_vproc_attr.grp_attr.max_width;
    gdc.mid.height = g_sample_vproc_attr.grp_attr.max_height;
    gdc.dst.width = g_sample_vproc_attr.grp_attr.max_width;
    gdc.dst.height = g_sample_vproc_attr.grp_attr.max_height;
    gdc.gdc_mode = OT_EIS_WORK_FORMULA_DERIVAT;
    gdc.ldc_or_fisheye = OT_EIS_GDC_FISHEYE_SEL;
    gdc.cell_size = OT_EIS_LUT_CELL_SIZE_16;
    g_sample_vproc_attr.set_attr.gdc_param.cfg = &gdc;
    g_sample_vproc_attr.set_attr.gdc_param.enable = OT_TRUE;
    fisheye.fisheye_mode = OT_EIS_FISHEYE_MODE_NORMAL;
    fisheye.fisheye_attr.fisheye_noraml.fov_in = 180;
    fisheye.fisheye_attr.fisheye_noraml.zoom.width = 4095;
    fisheye.fisheye_attr.fisheye_noraml.zoom.height = 4095;
    fisheye.fisheye_attr.fisheye_noraml.pan = 180;
    fisheye.fisheye_attr.fisheye_noraml.tilt = 180;
    fisheye.fisheye_attr.fisheye_noraml.roll = 0;
    fisheye.fisheye_attr.fisheye_noraml.trap_gain = 10;
    fisheye.fisheye_attr.fisheye_noraml.offset.x = 0;
    fisheye.fisheye_attr.fisheye_noraml.offset.y = 0;
    fisheye.fisheye_attr.fisheye_noraml.radius_out = g_sample_vproc_attr.grp_attr.max_width / 2;
    fisheye.fisheye_attr.fisheye_noraml.fish_lut.len = sizeof(g_lmf_coef);
    fisheye.fisheye_attr.fisheye_noraml.fish_lut.lut_data = (ot_u8 *)g_lmf_coef;
    g_sample_vproc_attr.set_attr.fisheye_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.fisheye_param.fisheye = &fisheye;

    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }

    sample_get_char();
    ret = ot_eis_vproc_grp_get_gdc(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.gdc_param);
    sample_print("get gdc ret 0x%x \n", ret);
    ret = ot_eis_vproc_grp_get_fisheye(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.fisheye_param);
    sample_print("get fisheye ret 0x%x \n", ret);
    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_s32 sample_vio_vproc_ldc(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;
    g_nr_pos    = OT_EIS_3DNR_POS_VPROC;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.gdc_param.enable = OT_TRUE;
    ot_eis_vproc_gdc_cfg gdc = {0};
    ot_eis_ldc_cfg ldc;

    gdc.src.width = g_sample_vproc_attr.grp_attr.max_width;
    gdc.src.height = g_sample_vproc_attr.grp_attr.max_height;
    gdc.mid.width = g_sample_vproc_attr.grp_attr.max_width;
    gdc.mid.height = g_sample_vproc_attr.grp_attr.max_height;
    gdc.dst.width = g_sample_vproc_attr.grp_attr.max_width;
    gdc.dst.height = g_sample_vproc_attr.grp_attr.max_height;
    gdc.gdc_mode = OT_EIS_WORK_FORMULA_DERIVAT;
    gdc.ldc_or_fisheye = OT_EIS_GDC_LDC_SEL;
    gdc.cell_size = OT_EIS_LUT_CELL_SIZE_16;
    g_sample_vproc_attr.set_attr.gdc_param.cfg = &gdc;
    g_sample_vproc_attr.set_attr.gdc_param.enable = OT_TRUE;
    ldc.ldc_mode = OT_EIS_VPROC_LDC_CALIB_FREE;
    ldc.ldc_attr.free.distortion_ratio = 100;
    ldc.ldc_attr.free.center_offset.x = 0;
    ldc.ldc_attr.free.center_offset.y = 0;
    ldc.ldc_attr.free.ratio.keep_aspect = 0;
    ldc.ldc_attr.free.ratio.xy_ratio = 100;
    ldc.ldc_attr.free.ratio.x_ratio = 100;
    ldc.ldc_attr.free.ratio.y_ratio = 100;
    g_sample_vproc_attr.set_attr.ldc_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.ldc_param.ldc = &ldc;

    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }

    sample_get_char();
    if (sns_type == SONY_IMX623_MIPI_3M_60FPS_16BIT) {
        /* 623 calibration param */
        ldc.ldc_mode = OT_EIS_VPROC_LDC_CALIB_BASE;
        ldc.ldc_attr.base.ratio.keep_aspect = 1;
        ldc.ldc_attr.base.ratio.x_ratio = 100;
        ldc.ldc_attr.base.ratio.y_ratio = 100;
        ldc.ldc_attr.base.ratio.xy_ratio = 100;
        ldc.ldc_attr.base.focal_len_x = 200381;
        ldc.ldc_attr.base.focal_len_y = 200381;
        ldc.ldc_attr.base.coord_shift.x = 96800;
        ldc.ldc_attr.base.coord_shift.y = 77600;
        ldc.ldc_attr.base.src_calibration_ratio[0] = 100000;
        ldc.ldc_attr.base.src_calibration_ratio[1] = -40418;
        ldc.ldc_attr.base.src_calibration_ratio[2] = 21474;
        ldc.ldc_attr.base.src_calibration_ratio[3] = -7504;
        ldc.ldc_attr.base.src_calibration_ratio[4] = 0;
        ldc.ldc_attr.base.src_calibration_ratio[5] = 0;
        ldc.ldc_attr.base.src_calibration_ratio[6] = 0;
        ldc.ldc_attr.base.src_calibration_ratio[7] = 0;
        ldc.ldc_attr.base.src_calibration_ratio[8] = 3200000;
        ldc.ldc_attr.base.src_calibration_ratio_next[0] = 100000;
        ldc.ldc_attr.base.src_calibration_ratio_next[1] = 0;
        ldc.ldc_attr.base.src_calibration_ratio_next[2] = 0;
        ldc.ldc_attr.base.src_calibration_ratio_next[3] = 0;
        ldc.ldc_attr.base.src_calibration_ratio_next[4] = 0;
        ldc.ldc_attr.base.src_calibration_ratio_next[5] = 0;
        ldc.ldc_attr.base.src_calibration_ratio_next[6] = 0;
        ldc.ldc_attr.base.src_calibration_ratio_next[7] = 0;
        ldc.ldc_attr.base.src_calibration_ratio_next[8] = 3200000;
        ldc.ldc_attr.base.coef_intp_ratio = 16384;

        g_sample_vproc_attr.set_attr.ldc_param.enable = OT_TRUE;
        g_sample_vproc_attr.set_attr.ldc_param.ldc = &ldc;
        ret = ot_eis_vproc_grp_set_ldc(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.ldc_param);
        sample_print("set ldc ret 0x%x \n", ret);
        sample_get_char();
    }

    ret = ot_eis_vproc_grp_get_gdc(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.gdc_param);
    sample_print("get gdc ret 0x%x \n", ret);
    ret = ot_eis_vproc_grp_get_ldc(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.ldc_param);
    sample_print("get ldc ret 0x%x \n", ret);

    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_s32 sample_vio_vproc_pmf(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;
    g_nr_pos = OT_EIS_3DNR_POS_VPROC;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.gdc_param.enable = OT_TRUE;
    ot_eis_vproc_gdc_cfg gdc = {0};
    ot_eis_pmf_cfg pmf;

    gdc.src.width = g_sample_vproc_attr.grp_attr.max_width;
    gdc.src.height = g_sample_vproc_attr.grp_attr.max_height;
    gdc.mid.width = g_sample_vproc_attr.grp_attr.max_width;
    gdc.mid.height = g_sample_vproc_attr.grp_attr.max_height;
    gdc.dst.width = g_sample_vproc_attr.grp_attr.max_width;
    gdc.dst.height = g_sample_vproc_attr.grp_attr.max_height;
    gdc.gdc_mode = OT_EIS_WORK_FORMULA_DERIVAT;
    gdc.ldc_or_fisheye = OT_EIS_GDC_LDC_SEL;
    gdc.cell_size = OT_EIS_LUT_CELL_SIZE_16;
    g_sample_vproc_attr.set_attr.gdc_param.cfg = &gdc;
    g_sample_vproc_attr.set_attr.gdc_param.enable = OT_TRUE;
    pmf.mode = OT_PMF_WORK_MODE_ROTATION;
    pmf.attr.rotaion.view_type = OT_RTT_VIEW_MODE_INSIDE;
    pmf.attr.rotaion.angle = 45;
    pmf.attr.rotaion.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.pmf_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.pmf_param.pmf = &pmf;

    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }

    sample_get_char();
    pmf.attr.rotaion.view_type = OT_RTT_VIEW_MODE_FULL;
    pmf.attr.rotaion.angle = 135;
    pmf.attr.rotaion.enable = OT_TRUE;
    ret = ot_eis_vproc_grp_set_pmf(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.pmf_param);
    sample_print("set pmf ret 0x%x \n", ret);
    sample_get_char();
    pmf.attr.rotaion.view_type = OT_RTT_VIEW_MODE_TYPICAL;
    pmf.attr.rotaion.angle = 315;
    pmf.attr.rotaion.enable = OT_TRUE;
    pmf.attr.rotaion.center_offset.x = 0;
    pmf.attr.rotaion.center_offset.y = 0;
    ret = ot_eis_vproc_grp_set_pmf(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.pmf_param);
    sample_print("set pmf ret 0x%x \n", ret);
    sample_get_char();
    ret = ot_eis_vproc_grp_get_gdc(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.gdc_param);
    sample_print("get gdc ret 0x%x \n", ret);
    ret = ot_eis_vproc_grp_get_pmf(g_sample_vproc_attr.grp_hdl, &g_sample_vproc_attr.set_attr.pmf_param);
    sample_print("get ldc ret 0x%x \n", ret);
    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}

static ot_s32 sample_vio_vproc_3dnr(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);
    static ot_u32 iso_arr[16];
    static ot_eis_nr_cfg nr_param[16];
    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;
    g_nr_pos    = OT_EIS_3DNR_POS_VPROC;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));
    for (ot_u32 i = 0; i < 16; i++) {
        iso_arr[i] = 100 + i * ((3276800 - 100) / 16);
        nr_param[i] = g_sample_nr_param;
    }

    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.nr_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.nr_param.nr_type = OT_EIS_NR_TYPE_VIDEO_NORM;
    g_sample_vproc_attr.set_attr.nr_param.param.nr_cfg.nr_mode = OT_EIS_OP_MODE_AUTO;
    g_sample_vproc_attr.set_attr.nr_param.param.nr_cfg.nr_auto.param_num = 16;
    g_sample_vproc_attr.set_attr.nr_param.param.nr_cfg.nr_auto.iso = iso_arr;
    g_sample_vproc_attr.set_attr.nr_param.param.nr_cfg.nr_auto.nr_param = nr_param;

    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }

    sample_get_char();

    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}


static ot_s32 sample_vio_vproc_luma(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    sample_vi_cfg vi_cfg = { 0 };
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);
    ot_eis_img_rect ls_rect[5];
    ot_eis_vproc_luma_results results = {0};
    /* 仅支持 vproc offline */
    g_mode_type = OT_EIS_VI_ONLINE_VPROC_OFFLINE;

    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));
    ls_rect[0].x = 0;
    ls_rect[0].y = 0;
    ls_rect[0].width = 32;
    ls_rect[0].height = 32;
    ls_rect[1].x = 32;
    ls_rect[1].y = 0;
    ls_rect[1].width = 32;
    ls_rect[1].height = 32;
    ls_rect[2].x = 0;
    ls_rect[2].y = 32;
    ls_rect[2].width = 32;
    ls_rect[2].height = 32;
    ls_rect[3].x = 32;
    ls_rect[3].y = 32;
    ls_rect[3].width = 32;
    ls_rect[3].height = 32;
    ls_rect[4].x = 0;
    ls_rect[4].y = 0;
    ls_rect[4].width = 64;
    ls_rect[4].height = 64;
    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.luma_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.luma_param.num = 5;
    g_sample_vproc_attr.set_attr.luma_param.luma = ls_rect;
    ret = sample_vio_start_route(&vi_cfg, 1);
    if (ret != OT_SUCCESS) {
        printf("sample_vio_start_route error 0x%x \n", ret);
        return ret;
    }
    ret = ot_eis_vproc_chn_get_luma_result(g_sample_vproc_attr.chn_hdl[0], &results);
    if (ret != OT_SUCCESS) {
        printf("vproc_get_luma_result error 0x%x \n", ret);
        return ret;
    }
    printf("luma results num %d : %lld  %lld  %lld  %lld  %lld \n", results.num, results.luma_stats[0], results.luma_stats[1],
        results.luma_stats[2], results.luma_stats[3], results.luma_stats[4]);
    sample_get_char();
    ret = ot_eis_vproc_chn_get_luma_result(g_sample_vproc_attr.chn_hdl[0], &results);
    if (ret != OT_SUCCESS) {
        printf("vproc_get_luma_result error 0x%x \n", ret);
        return ret;
    }
    printf("luma results num %d : %lld  %lld  %lld  %lld  %lld \n", results.num, results.luma_stats[0], results.luma_stats[1],
        results.luma_stats[2], results.luma_stats[3], results.luma_stats[4]);

    sample_get_char();
    ret = ot_eis_vproc_chn_get_luma_result(g_sample_vproc_attr.chn_hdl[0], &results);
    if (ret != OT_SUCCESS) {
        printf("vproc_get_luma_result error 0x%x \n", ret);
        return ret;
    }
    printf("luma results num %d : %lld  %lld  %lld  %lld  %lld \n", results.num, results.luma_stats[0], results.luma_stats[1],
        results.luma_stats[2], results.luma_stats[3], results.luma_stats[4]);

    sample_get_char();
    ret = ot_eis_vproc_chn_get_luma_result(g_sample_vproc_attr.chn_hdl[0], &results);
    if (ret != OT_SUCCESS) {
        printf("vproc_get_luma_result error 0x%x \n", ret);
        return ret;
    }
    printf("luma results num %d : %lld  %lld  %lld  %lld  %lld \n", results.num, results.luma_stats[0], results.luma_stats[1],
        results.luma_stats[2], results.luma_stats[3], results.luma_stats[4]);

    sample_vio_stop_route(&vi_cfg, 1);
    return OT_SUCCESS;
}


static ot_s32 sample_vproc_attach(ot_void)
{
    ot_u32 i = 0;
    ot_u32 n = 0;
    ot_s32 ret = OT_SUCCESS;
    ot_eis_vproc_grp_attr grp_attr;
    ot_eis_vproc_chn_attr chn_attr;
    ot_eis_vproc_pipe_attr pipe_attr;
    ot_eis_handle grp_handle[OT_EIS_VPROC_GRP_MAX_NUM];
    ot_eis_handle chn_handle[OT_EIS_VPROC_GRP_MAX_NUM][OT_EIS_VPROC_GRP_PIPE_MAX_NUM];
    ot_eis_handle pipe_handle[OT_EIS_VPROC_GRP_MAX_NUM][OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);

    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    grp_attr = g_sample_vproc_attr.grp_attr;
    chn_attr = g_sample_vproc_attr.chn_attr[0];
    pipe_attr = g_sample_vproc_attr.pipe_attr[0];

    ret = ot_eis_media_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_media_init failed!\n");
        return OT_FAILURE;
    }
    if (sample_comm_sys_set_vi_vproc_init_cfg(g_mode_type) != OT_SUCCESS) {
        sample_print("set_vi_vproc_mode failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    ret = ot_eis_vproc_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_init failed with %#x!\n", ret);
        return OT_FAILURE;
    }
    for (n = 0; n < OT_EIS_VPROC_GRP_MAX_NUM; n++) {
        ret = ot_eis_vproc_grp_create(n, &grp_handle[n], &grp_attr);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_create failed with %#x\n", ret);
        }
        sample_print("ot_eis_vproc_grp_create handle 0x%llx\n", (ot_u64)grp_handle[n]);

        for(i = 0; i < OT_EIS_VPROC_GRP_PIPE_MAX_NUM; ++i) {
            ret = ot_eis_vproc_pipe_create(&pipe_handle[n][i], &pipe_attr);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_pipe_create failed with %#x\n", ret);
            }
            sample_print("ot_eis_vproc_pipe_create handle 0x%llx\n", (ot_u64)pipe_handle[n][i]);
            ret = ot_eis_vproc_grp_attach_pipe(grp_handle[n], pipe_handle[n][i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_attach_pipe failed with %#x\n", ret);
            }
        }

        for (i = 0; i < OT_EIS_VPROC_GRP_CHN_MAX_NUM; ++i) {
            ret = ot_eis_vproc_chn_create(i, &chn_handle[n][i], &chn_attr);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_chn_create failed with 0x%#x\n", ret);
            }
            sample_print("ot_eis_vproc_chn_create handle 0x%llx\n", (ot_u64)chn_handle[n][i]);
            ret = ot_eis_vproc_grp_attach_chn(grp_handle[n], chn_handle[n][i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_attach_chn failed with %#x\n", ret);
            }
        }
    }

    sample_get_char();

    for (n = 0; n < OT_EIS_VPROC_GRP_MAX_NUM; n++) {
        ot_eis_handle tmp;
        ret = ot_eis_vproc_get_grp_hdl_by_grp_id(n, &tmp);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_get_grp_hdl_by_grp_id failed with %#x\n", ret);
        }
        sample_print("ot_eis_vproc_get_grp_hdl_by_grp_id handle 0x%llx create handle 0x%llx \n", (ot_u64)tmp, (ot_u64)grp_handle[n]);

        for (i = 0; i < OT_EIS_VPROC_GRP_CHN_MAX_NUM; ++i) {
            ot_eis_handle chn_tmp;
            ret = ot_eis_vproc_get_chn_hdl_by_ids(n, i, &chn_tmp);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_get_chn_hdl_by_ids %d %d failed with 0x%#x\n", n, i, ret);
            }
            sample_print("ot_eis_vproc_get_chn_hdl_by_ids handle 0x%llx create chn handle %#llx \n", (ot_u64)chn_tmp, (ot_u64)chn_handle[n][i]);
        }
    }

    sample_get_char();

    for (n = 0; n < OT_EIS_VPROC_GRP_MAX_NUM; n++) {
        ot_s32 tmp;
        ret = ot_eis_vproc_get_grp_id_by_grp_hdl(grp_handle[n], &tmp);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_get_grp_id_by_grp_hdl failed with %#x\n", ret);
        }
        sample_print("ot_eis_vproc_get_grp_id_by_grp_hdl grp id %d create id %d \n", tmp, n);

        for (i = 0; i < OT_EIS_VPROC_GRP_CHN_MAX_NUM; ++i) {
            ot_s32 chn_tmp;
            ret = ot_eis_vproc_get_chn_id_by_chn_hdl(chn_handle[n][i], &chn_tmp);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_get_chn_id_by_chn_hdl %d %d failed with 0x%#x\n", n, i, ret);
            }
            sample_print("ot_eis_vproc_get_chn_id_by_chn_hdl ch id %d create chn id %d \n", chn_tmp, i);
        }
    }

    sample_get_char();

    for (n = 0; n < OT_EIS_VPROC_GRP_MAX_NUM; n++) {
        for(i = 0; i < OT_EIS_VPROC_GRP_PIPE_MAX_NUM; ++i) {
            ot_eis_vproc_grp_detach_pipe(grp_handle[n], pipe_handle[n][i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_detach_pipe hdl %#llx failed with %#x\n", (ot_u64)pipe_handle[n][i], ret);
            }
            sample_print("ot_eis_vproc_pipe_destroy handle 0x%llx\n", (ot_u64)pipe_handle[n][i]);
            ret = ot_eis_vproc_pipe_destroy(pipe_handle[n][i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_pipe_destroy hdl %#llx failed with %#x\n", (ot_u64)pipe_handle[n][i], ret);
            }
        }

        for (i = 0; i < OT_EIS_VPROC_GRP_CHN_MAX_NUM; ++i) {
            ret = ot_eis_vproc_grp_detach_chn(grp_handle[n], chn_handle[n][i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_detach_chn failed with %#x\n", ret);
            }
            sample_print("ot_eis_vproc_chn_destroy handle 0x%llx\n", (ot_u64)chn_handle[n][i]);
            ret = ot_eis_vproc_chn_destroy(chn_handle[n][i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_chn_destroy failed with %#x\n", ret);
            }
        }
        sample_print("ot_eis_vproc_grp_destroy handle 0x%llx\n", (ot_u64)grp_handle[n]);
        ret = ot_eis_vproc_grp_destroy(grp_handle[n]);
    }
    ret = ot_eis_vproc_deinit();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_deinit failed with %#x!\n", ret);
        return OT_FAILURE;
    }
    ret = ot_eis_media_deinit();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_media_deinit failed!\n");
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}


static ot_s32 sample_vproc_to_vproc_to_vo(ot_void)
{
    ot_s32 ret;
    const ot_u32 vproc_num = 5;
    sample_vi_cfg vi_cfg;
    sample_sns_type sns_type = sample_comm_args_get_sns_type(0);
    ot_eis_img_size img_size = {0};
    sample_vo_attr *vo_attr = OT_NULL;
    ot_eis_vproc_pipe_attr pipe_attr;
    ot_eis_handle vproc_pipe[OT_EIS_VPROC_GRP_MAX_NUM];
    ot_eis_vproc_grp_attr grp_attr;
    ot_eis_handle vproc_grp[OT_EIS_VPROC_GRP_MAX_NUM];
    ot_eis_vproc_chn_attr chn_attr;
    ot_eis_handle vproc_chn[OT_EIS_VPROC_GRP_MAX_NUM];
    ot_eis_handle media_pipe_hdl;
    ot_eis_low_delay_info lowdelay = {0};
    ot_eis_handle hdls[4];
    ot_s32 num;
    ot_u32 i = 0;
    ot_u32 j = 0;

    sample_comm_vi_get_size_by_sns_type(sns_type, &img_size);
    vo_attr = sample_comm_args_get_vo_attr();
    if (vo_attr == OT_NULL) {
        sample_print("Invalid vo attr.\n");
        return OT_FAILURE;
    }

    // ot_eis_handle media_pipe_hdl;
    ot_bool pipe_sw[OT_EIS_VPROC_GRP_PIPE_MAX_NUM] = {OT_TRUE, OT_FALSE, OT_FALSE, OT_FALSE};
    ot_bool chn_sw[OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {OT_TRUE, OT_FALSE, OT_FALSE, OT_FALSE};

    g_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    sample_media_vproc_get_default_attr_by_snsor(sns_type, &g_sample_vproc_attr);
    memset(&g_sample_vproc_attr.set_attr, 0, sizeof(g_sample_vproc_attr.set_attr));
    if (sample_vio_sys_init(&img_size) != OT_SUCCESS) {
        return OT_FAILURE;
    }
    sample_print_vproc_attr();

    ret = sample_comm_vi_start_vi(&vi_cfg);
    if (ret != OT_SUCCESS) {
        goto start_vi_failed;
    }


    ret = sample_comm_start_vproc(&g_sample_vproc_attr, pipe_sw, chn_sw);
    if (ret != OT_SUCCESS) {
        goto start_vproc_failed;
    }

    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        goto start_vo_failed;
    }

    ret = sample_comm_media_pipe_init(&media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto pipe_start_failed;
    }
    chn_attr = g_sample_vproc_attr.chn_attr[0];
    pipe_attr = g_sample_vproc_attr.pipe_attr[0];
    pipe_attr.image_attr = chn_attr.image_attr;
    grp_attr = g_sample_vproc_attr.grp_attr;
    grp_attr.max_width = chn_attr.image_attr.width;
    grp_attr.max_height = chn_attr.image_attr.height;

    ret = sample_comm_vi_bind_vproc(vi_cfg.pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto vi_vproc_bind_failed;
    }

    for (i = 0; i < vproc_num; i++) {
        ret = ot_eis_vproc_grp_create(i + 1, &vproc_grp[i], &grp_attr);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_create failed with %#x\n", ret);
            return OT_FAILURE;
        }

        ret = ot_eis_vproc_grp_start(vproc_grp[i]);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_start failed with %#x\n", ret);
            goto vproc_create_failed;
        }

        ret = ot_eis_vproc_pipe_create(&vproc_pipe[i], &pipe_attr);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_pipe_create failed with %#x\n", ret);
            goto vproc_create_failed;
        }

        ret = ot_eis_vproc_grp_attach_pipe(vproc_grp[i], vproc_pipe[i]);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_attach_pipe failed with %#x\n", ret);
            goto vproc_create_failed;
        }

        ret = ot_eis_vproc_chn_create(0, &vproc_chn[i], &chn_attr);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_create failed with %#x\n", ret);
            goto vproc_create_failed;
        }

        ret = ot_eis_vproc_grp_attach_chn(vproc_grp[i], vproc_chn[i]);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_attach_chn failed with %#x\n", ret);
            goto vproc_create_failed;
        }
        if (i == 0) {
            ret = sample_comm_vproc_bind_vproc(g_sample_vproc_attr.chn_hdl[0], vproc_pipe[0], media_pipe_hdl);
            if (ret != OT_SUCCESS) {
                sample_print("sample_comm_vproc_bind_vproc failed with %#x\n", ret);
                goto pipe_start_failed;
            }
        } else {
            ret = sample_comm_vproc_bind_vproc(vproc_chn[i - 1], vproc_pipe[i], media_pipe_hdl);
            if (ret != OT_SUCCESS) {
                sample_print("sample_comm_vproc_bind_vproc failed with %#x\n", ret);
                goto pipe_start_failed;
            }
        }
    }

    ret = sample_comm_vproc_bind_vo(vproc_chn[vproc_num - 1], vo_attr->in_port_handle[0], media_pipe_hdl);
    if (ret != OT_SUCCESS) {
        goto vproc_bind_vo_failed;
    }
    sample_get_char();
    sample_print("all chn lowdelay enable \n");
    lowdelay.enable = OT_TRUE;
    lowdelay.line_cnt= 32;
    lowdelay.one_buf_en = OT_FALSE;

    ret = ot_eis_vproc_chn_set_low_delay(g_sample_vproc_attr.chn_hdl[0], &lowdelay);
    if (ret != OT_SUCCESS) {
        goto vproc_bind_vo_failed;
    }
    for (i = 0; i < vproc_num; i++) {
        ret = ot_eis_vproc_chn_set_low_delay(vproc_chn[i], &lowdelay);
        if (ret != OT_SUCCESS) {
            goto vproc_bind_vo_failed;
        }
    }
    sample_get_char();
    sample_print("all chn lowdelay disable \n");
    lowdelay.enable = OT_FALSE;
    lowdelay.line_cnt= 0;
    lowdelay.one_buf_en = OT_FALSE;

    ret = ot_eis_vproc_chn_set_low_delay(g_sample_vproc_attr.chn_hdl[0], &lowdelay);
    if (ret != OT_SUCCESS) {
        goto vproc_bind_vo_failed;
    }
    for (i = 0; i < vproc_num; i++) {
        ret = ot_eis_vproc_chn_set_low_delay(vproc_chn[i], &lowdelay);
        if (ret != OT_SUCCESS) {
            goto vproc_bind_vo_failed;
        }
    }

    sample_get_char();
    pthread_t set_pid[5][20];
    g_vproc_set_pthread = OT_TRUE;

    for (i = 0; i < 1; i++) {
        ret = pthread_create(&set_pid[0][i], OT_NULL, sample_vproc_set_mosaic_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        sample_print("vproc chn mosaic thread \n");
        usleep(2000000);
        ret = pthread_create(&set_pid[1][i], OT_NULL, sample_vproc_set_line_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        sample_print("vproc chn line thread \n");

        usleep(2000000);
        ret = pthread_create(&set_pid[2][i], OT_NULL, sample_vproc_set_cover_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        sample_print("vproc chn cover thread \n");

        usleep(2000000);
        ret = pthread_create(&set_pid[3][i], OT_NULL, sample_vproc_set_rect_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        sample_print("vproc chn rect thread \n");

        usleep(2000000);
        ret = pthread_create(&set_pid[4][i], OT_NULL, sample_vproc_set_osd_thread, &g_sample_vproc_attr);
        if (ret != OT_SUCCESS) {
            printf("create vproc set thread failed!\n");
            g_vproc_set_pthread = OT_FALSE;
            return OT_FAILURE;
        }
        sample_print("vproc chn osd thread \n");
    }
    sample_get_char();
    g_vproc_set_pthread = OT_FALSE;
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 1; i++) {
            pthread_join(set_pid[j][i], OT_NULL);
        }
    }
    g_sample_vproc_attr.set_attr.mosaic_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_mosaic(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.mosaic_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_mosaic failed with %#x!\n", ret);
    }
    g_sample_vproc_attr.set_attr.line_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_line(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.line_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_line failed with %#x!\n", ret);
    }
    g_sample_vproc_attr.set_attr.osd_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_osd(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.osd_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_osd failed with %#x!\n", ret);
    }

    g_sample_vproc_attr.set_attr.rect_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_rect(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.rect_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rect failed with %#x!\n", ret);
    }

    g_sample_vproc_attr.set_attr.cover_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_cover(g_sample_vproc_attr.chn_hdl[0], &g_sample_vproc_attr.set_attr.cover_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_cover failed with %#x!\n", ret);
    }

    sample_get_char();
    g_sample_vproc_attr.is_set_attr = OT_TRUE;
    g_sample_vproc_attr.set_attr.osd_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.osd_param.num = 2;
    ot_eis_osd_attr osd[2];
    ot_eis_img_frame osd_fram[2];
    osd[0].osd_rgn.x = 100;
    osd[0].osd_rgn.y = 50;
    osd[0].osd_rgn.width = 300;
    osd[0].osd_rgn.height = 200;
    osd[0].color = 0xff;
    for (ot_u32 i = 0; i < OT_EIS_VPROC_OSD_LUT_NUM; i++) {
        osd[0].alpha_lut[i] = i * (256 / OT_EIS_VPROC_OSD_LUT_NUM);
    }
    sample_alloc_osd_frame(&osd_fram[0], 300, 128, OT_EIS_IMAGE_FORMAT_ARGB_1555);
    sample_fill_osd_frame(&osd_fram[0]);

    osd[0].osd_frame = &osd_fram[0];

    osd[1].osd_rgn.x = 500;
    osd[1].osd_rgn.y = 400;
    osd[1].osd_rgn.width = 150;
    osd[1].osd_rgn.height = 80;
    osd[1].color = 0xff;
    for (ot_u32 i = 0; i < OT_EIS_VPROC_OSD_LUT_NUM; i++) {
        osd[1].alpha_lut[i] = i * (256 / OT_EIS_VPROC_OSD_LUT_NUM);
    }
    sample_alloc_osd_frame(&osd_fram[1], 150, 80, OT_EIS_IMAGE_FORMAT_ARGB_4444);
    sample_fill_osd_frame(&osd_fram[1]);
    osd[1].osd_frame = &osd_fram[1];
    g_sample_vproc_attr.set_attr.osd_param.osd = &osd[0];

    g_sample_vproc_attr.set_attr.rect_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.rect_param.num = 10;
    ot_eis_vproc_rect rect[10];
    rect[0].color = 0xff0000;
    rect[0].thick = 6;
    rect[0].is_corner = OT_FALSE;
    rect[0].rect.x = 100;
    rect[0].rect.y = 300;
    rect[0].rect.width = 200;
    rect[0].rect.height = 120;
    rect[1].color = 0x00ff00;
    rect[1].thick = 4;
    rect[1].is_corner = OT_FALSE;
    rect[1].rect.x = 120;
    rect[1].rect.y = 400;
    rect[1].rect.width = 100;
    rect[1].rect.height = 60;
    rect[2].color = 0x0000ff;
    rect[2].thick = 4;
    rect[2].is_corner = OT_FALSE;
    rect[2].rect.x = 140;
    rect[2].rect.y = 420;
    rect[2].rect.width = 100;
    rect[2].rect.height = 60;
    rect[3].color = 0xffff00;
    rect[3].thick = 4;
    rect[3].is_corner = OT_FALSE;
    rect[3].rect.x = 160;
    rect[3].rect.y = 440;
    rect[3].rect.width = 100;
    rect[3].rect.height = 60;
    rect[4].color = 0xff00ff;
    rect[4].thick = 4;
    rect[4].is_corner = OT_FALSE;
    rect[4].rect.x = 180;
    rect[4].rect.y = 460;
    rect[4].rect.width = 100;
    rect[4].rect.height = 60;
    rect[5].color = 0x00ffff;
    rect[5].thick = 4;
    rect[5].is_corner = OT_FALSE;
    rect[5].rect.x = 200;
    rect[5].rect.y = 480;
    rect[5].rect.width = 100;
    rect[5].rect.height = 60;
    rect[6].color = 0xffffff;
    rect[6].thick = 4;
    rect[6].is_corner = OT_FALSE;
    rect[6].rect.x = 220;
    rect[6].rect.y = 500;
    rect[6].rect.width = 100;
    rect[6].rect.height = 60;

    rect[7].color = 0x000000;
    rect[7].thick = 4;
    rect[7].is_corner = OT_FALSE;
    rect[7].rect.x = 240;
    rect[7].rect.y = 520;
    rect[7].rect.width = 100;
    rect[7].rect.height = 60;

    rect[8].color = 0x8f0000;
    rect[8].thick = 4;
    rect[8].is_corner = OT_FALSE;
    rect[8].rect.x = 260;
    rect[8].rect.y = 540;
    rect[8].rect.width = 100;
    rect[8].rect.height = 60;

    rect[9].color = 0x008f00;
    rect[9].thick = 4;
    rect[9].is_corner = OT_FALSE;
    rect[9].rect.x = 280;
    rect[9].rect.y = 560;
    rect[9].rect.width = 100;
    rect[9].rect.height = 60;

    g_sample_vproc_attr.set_attr.rect_param.rect = &rect[0];

    g_sample_vproc_attr.set_attr.line_param.num = 10;
    ot_eis_line line[10];
    line[0].color = 0xff0000;
    line[0].thickness = 6;
    line[0].point[0].x = 100;
    line[0].point[0].y = 300;
    line[0].point[1].x = 800;
    line[0].point[1].y = 300;
    line[1].color = 0x00ff00;
    line[1].thickness = 10;
    line[1].point[0].x = 500;
    line[1].point[0].y = 400;
    line[1].point[1].x = 800;
    line[1].point[1].y = 300;
    for (ot_u32 i = 0; i < 5; i++) {
        line[2 * i + 0] = line[0];
        line[0].point[0].x = 100 + i * 20;
        line[0].point[0].y = 300 + i * 20;
        line[2 * i + 1] = line[1];
        line[1].point[0].x = 500 + i * 20;
        line[1].point[0].y = 400 + i * 20;
    }
    g_sample_vproc_attr.set_attr.line_param.line = &line[0];

    //g_sample_vproc_attr.set_attr.mirror_param.enable = OT_TRUE;
    //ot_eis_vproc_chn_set_mirror(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.mirror_param);


    g_sample_vproc_attr.set_attr.osd_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.line_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_osd(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.osd_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_osd failed with %#x!\n", ret);
    }
    ret = ot_eis_vproc_chn_set_rect(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.rect_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rect failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_line(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.line_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_line failed with %#x!\n", ret);
    }

    sample_get_char();

    g_sample_vproc_attr.set_attr.osd_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.line_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_osd(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.osd_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_osd failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_rect(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.rect_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rect failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_line(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.line_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_line failed with %#x!\n", ret);
    }

    sample_get_char();

    g_sample_vproc_attr.set_attr.osd_param.enable = OT_FALSE;
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_FALSE;
    g_sample_vproc_attr.set_attr.line_param.enable = OT_FALSE;
    //g_sample_vproc_attr.set_attr.mirror_param.enable = OT_FALSE;
    //ot_eis_vproc_chn_set_mirror(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.mirror_param);
    ret = ot_eis_vproc_chn_set_osd(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.osd_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_osd failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_rect(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.rect_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rect failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_line(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.line_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_line failed with %#x!\n", ret);
    }

    sample_get_char();
    g_sample_vproc_attr.set_attr.osd_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.line_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.mirror_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_mirror(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.mirror_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_mirror failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_rect(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.rect_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rect failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_osd(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.osd_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_osd failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_line(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.line_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_line failed with %#x!\n", ret);
    }

    sample_get_char();

    g_sample_vproc_attr.set_attr.osd_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.line_param.enable = OT_TRUE;
    g_sample_vproc_attr.set_attr.mirror_param.enable = OT_TRUE;
    ret = ot_eis_vproc_chn_set_mirror(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.mirror_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_mirror failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_rect(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.rect_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rect failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_osd(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.osd_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_osd failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_line(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.line_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_line failed with %#x!\n", ret);
    }

    sample_get_char();


    g_sample_vproc_attr.set_attr.osd_param.enable = OT_FALSE;
    g_sample_vproc_attr.set_attr.rect_param.enable = OT_FALSE;
    g_sample_vproc_attr.set_attr.line_param.enable = OT_FALSE;
    g_sample_vproc_attr.set_attr.mirror_param.enable = OT_FALSE;
    ret = ot_eis_vproc_chn_set_mirror(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.mirror_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_mirror failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_rect(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.rect_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_rect failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_osd(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.osd_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_osd failed with %#x!\n", ret);
    }

    ret = ot_eis_vproc_chn_set_line(vproc_chn[vproc_num - 1], &g_sample_vproc_attr.set_attr.line_param);
    if (ret != OT_SUCCESS) {
        sample_print("vproc_chn_set_line failed with %#x!\n", ret);
    }

    sample_get_char();

    sample_free_osd_frame(&osd_fram[0]);
    sample_free_osd_frame(&osd_fram[1]);

vproc_create_failed:
    for (j = 0; j < vproc_num; j++) {
        ot_eis_vproc_grp_stop(vproc_grp[j]);
        ret = ot_eis_vproc_grp_get_attched_pipes(vproc_grp[j], hdls, &num);

        for (int i = 0; i < num; i++) {
            ret = ot_eis_vproc_grp_detach_pipe(vproc_grp[j], hdls[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_detach_pipe failed with %#x!\n", ret);
            }

            ret = ot_eis_vproc_pipe_destroy(hdls[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_pipe_destroy failed with %#x!\n", ret);
            }
        }
        ot_eis_vproc_grp_get_attched_chns(vproc_grp[j], hdls, &num);
        for (int i = 0; i < num; i++) {
            ret = ot_eis_vproc_grp_detach_chn(vproc_grp[j], hdls[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_detach_pipe failed with %#x!\n", ret);
            }

            ret = ot_eis_vproc_chn_destroy(hdls[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_pipe_destroy failed with %#x!\n", ret);
            }
        }
        ot_eis_vproc_grp_destroy(vproc_grp[j]);
    }

vproc_bind_vo_failed:

    sample_comm_vi_un_bind_vproc(vi_cfg.pipe_info[0].chn_info.chn_hdl, g_sample_vproc_attr.pipe_hdl[0], media_pipe_hdl);
vi_vproc_bind_failed:
    sample_comm_media_pipe_stop(media_pipe_hdl);
pipe_start_failed:
    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_stop_vproc(&g_sample_vproc_attr);
start_vproc_failed:
    sample_comm_vi_stop_vi(&vi_cfg);
start_vi_failed:

    sample_comm_sys_exit();

    return ret;
}


ot_s32 sample_vio_vproc_frame_copy(ot_void)
{
    ot_s32 ret;
    ot_eis_handle pool_hdl;
    ot_u32 block_size;
    ot_video_buffer_attr pool_attr = { 0 };
    ot_eis_img_attr img_attr = { 0 };
    ot_eis_img_frame src, dst;
    ot_video_buffer_attr vrb_cfg;
    ot_u64 start_us, end_us;
    ot_u64 curr_us = 0, max_us = 0, avg_us = 0, cnt_us = 0;
    ot_u32 i = 0;
    img_attr.bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8;
    img_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE;
    img_attr.height = HEIGHT_1552;
    img_attr.width = WIDTH_1936;
    img_attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR;
    img_attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YUV_420_SEMIPLANAR;

    sample_common_get_vrb_cfg(3, img_attr.width, img_attr.height, &vrb_cfg);
    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    if (sample_comm_sys_set_vi_vproc_init_cfg(g_mode_type) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    if (ot_eis_media_set_3dnr_pos(g_nr_pos) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    sample_common_get_buffer_pool_cfg(&img_attr, &block_size);
    pool_attr.cnt = 1;
    pool_attr.buf_blks[0].cnt = 4;
    pool_attr.buf_blks[0].size = block_size;

    ret = ot_buffer_pool_init();
    if (ret != OT_SUCCESS && ret != OT_ERR_BUFPOOL_ALREADY_INIT) {
       printf("buffer_pool init fail! ret[%d]\n", ret);
       return OT_FAILURE;
    }

    ret =  ot_buffer_pool_create(&pool_hdl, &pool_attr);
    if(ret != 0){
       printf("buffer_pool create fail! ret[%d]\n", ret);
       goto pool_create_failed;
    }
    src.attr = img_attr;
    dst.attr = img_attr;
    ret = sample_vproc_comm_get_block_for_frame(pool_hdl, &src);
    if(ret != 0){
        printf("sample_vproc_comm_get_block_for_frame fail! ret[%d]\n", ret);
        goto pool_create_failed;
    }
    ret = ot_img_read_frame_from_file(&src, "./res/yuv420sp_1936_1552.yuv", 0);
    if(ret != 0){
        printf("ot_img_read_frame_from_file fail! ret[%d]\n", ret);
        goto release_frame;
    }
    ret = sample_vproc_comm_get_block_for_frame(pool_hdl, &dst);
    if(ret != 0){
        printf("sample_vproc_comm_get_block_for_frame fail! ret[%d]\n", ret);
        goto release_frame;
    }
    for (i = 0; i < 10000; i++) {
        start_us = sample_comm_get_time_us();
        ret = ot_eis_frame_dma_copy(&src, &dst);
        end_us = sample_comm_get_time_us();
        if(ret != 0){
            printf("ot_eis_frame_dma_copy fail! ret[%d]\n", ret);
            goto release_frame;
        }
        curr_us = end_us - start_us;
        cnt_us += curr_us;
        avg_us = cnt_us / (i + 1);
        if (curr_us > max_us) {
            max_us = curr_us;
        }
    }

    sample_print("ot_eis_frame_dma_copy cnt %d  %lld us max %lld us avg %lld  us \n", i, cnt_us, max_us, avg_us);
    ret = ot_img_write_frame_to_file(&dst, "./res/yuv420sp_1936_1552_dma_copy.yuv", OT_FALSE, OT_FALSE);
    if(ret != 0){
        printf("ot_img_read_frame_from_file fail! ret[%d]\n", ret);
        goto release_frame;
    }
    ret = OT_SUCCESS;
release_frame:
    sample_vproc_release_frame(pool_hdl, &src);
    sample_vproc_release_frame(pool_hdl, &dst);
pool_create_failed:
    ot_buffer_pool_deinit();
    sample_comm_sys_exit();
    return ret;
}

static ot_s32 sample_vproc_save_chn(ot_void)
{
    ot_s32 ret = OT_SUCCESS;
    ot_eis_handle chn;
    ot_eis_img_frame frm = {0};
    ot_s32 cnt = 100;
    ret = ot_smr_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_smr_init failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    ret = ot_eis_vproc_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_init failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    chn = 0;
    do {
        ret = ot_eis_vproc_chn_acquire_frame(chn, &frm, 500);
        cnt--;
    } while (ret != OT_SUCCESS && cnt <= 0);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_chn_acquire_frame failed with %#x!\n", ret);
        ot_eis_vproc_deinit();
        ret = ot_smr_deinit();
        return ret;
    }

    ret = ot_img_write_frame_to_file(&frm, "./res/yuv420sp_1936_1552_chn0.yuv", OT_FALSE, OT_TRUE);
    if(ret != 0){
        sample_print("ot_img_read_frame_from_file fail! ret[%d]\n", ret);
    }
    ot_eis_vproc_chn_release_frame(chn, &frm);
    ot_eis_vproc_deinit();
    ot_smr_deinit();
    return OT_SUCCESS;
}


static ot_s32 sample_vio_execute_case(ot_u32 case_index)
{
    ot_s32 ret;

    switch (case_index) {
        case 0: /* 0 all mode route */
            ret = sample_vio_all_mode();
            break;
        case 1: /* send frame */
            ret = sample_vio_chn_usr_mode();
            break;
        case 2: /* crop  */
            ret = sample_vio_vproc_crop();
            break;
        case 3: /* mirror  */
            ret = sample_vio_vproc_mirror();
            break;
        case 4: /* mosaic  */
            printf("index 4 sample_vio_vproc_mosaic \n");
            ret = sample_vio_vproc_mosaic();
            break;

        case 5: /* line  */
            printf("index 5 sample_vio_vproc_line \n");
            ret = sample_vio_vproc_line();
            break;

        case 6: /* cover  */
            printf("index 6 sample_vio_vproc_cover \n");
            ret = sample_vio_vproc_cover();
            break;

        case 7: /* rect  */
            printf("index 7 sample_vio_vproc_rect \n");
            ret = sample_vio_vproc_rect();
            break;

        case 8: /* osd  */
            printf("index 8 sample_vio_vproc_osd \n");
            ret = sample_vio_vproc_osd();
            break;

        case 9: /* fisheye  */
            printf("index 9 sample_vio_vproc_fisheye \n");
            ret = sample_vio_vproc_fisheye();
            break;

        case 10: /* ldc */
            printf("index 10 sample_vio_vproc_ldc \n");
            ret = sample_vio_vproc_ldc();
            break;

        case 11: /* pmf */
            printf("index 11 sample_vio_vproc_pmf \n");
            ret = sample_vio_vproc_pmf();
            break;

        case 12: /* 3dnr */
            printf("index 12 sample_vio_vproc_3dnr \n");
            ret = sample_vio_vproc_3dnr();
            break;

        case 13: /* attach */
            printf("index 13 sample_vproc_attach \n");
            ret = sample_vproc_attach();
            break;

        case 14: /* venc */
            printf("index 14 vproc_to_vo_and_vproc_to_venc \n");
            ret = sample_vproc_to_vo_and_vproc_to_venc();
            break;

        case 15: /* vproc->vproc */
            printf("index 15 vproc_to_vproc_to_vo \n");
            ret = sample_vproc_to_vproc_to_vo();
            break;

        case 16: /* luma */
            printf("index 16 vproc_luma \n");
            ret = sample_vio_vproc_luma();
            break;
        case 17: /* copy */
            printf("index 17 frame_copy \n");
            ret = sample_vio_vproc_frame_copy();
            break;
        case 18: /* xylut */
            printf("index 18 gdc 2dlut \n");
            ret = sample_vio_vproc_xylut();
            break;

        default:
            ret = OT_FAILURE;
            break;
    }

    return ret;
}

static ot_s32 sample_vproc_check_params(sample_param_comb* param_comb)
{
    if (param_comb->input_param >= ONE_MIPI_ONE_SNS_MAX) {
        sample_print("Invalid params\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

ot_s32 main(ot_s32 argc, ot_char *argv[])
{
    ot_s32 ret;
    ot_u32 index;
    sample_param_comb *param_comb = OT_NULL;

    if ((argc < 2) || (argc > 6)) { /* 2,6:arg num */
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    if (strlen(argv[1]) > 2 || strlen(argv[1]) <= 0 || !check_digit(argv[1][0]) || /* 2:arg len */
        (strlen(argv[1]) == 2 && (!check_digit(argv[1][1])))) { /* 2:arg len */
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    sample_register_sig_handler(sample_vio_handle_sig);

    index = strtol(argv[1], NULL, 10); /* base 10 */

    ret = sample_comm_args_register(argc, argv);
    if (ret != OT_SUCCESS) {
        sample_print("register params failed.\n");
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    param_comb = sample_comm_args_get_param_comb();
    ret = sample_vproc_check_params(param_comb);
    if (ret != OT_SUCCESS) {
        sample_print("check params failed.\n");
        sample_vio_usage(argv[0]);
        return OT_FAILURE;
    }

    ret = sample_vio_execute_case(index);
    if ((ret == OT_SUCCESS) && (g_sig_flag == 0)) {
        printf("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    return ret;
}
