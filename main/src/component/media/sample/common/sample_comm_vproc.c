/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "sample_comm.h"

#include "ot_eis_vi_api.h"
#include "ot_eis_vo_in_port.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */
#define WIDTH_1920 1920
#define HEIGHT_1080 1080
#define WIDTH_3840 3840
#define HEIGHT_2160 2160
#define WIDTH_2688 2688
#define HEIGHT_1520 1520

#define WIDTH_1280 1280
#define HEIGHT_720 720

#define WIDTH_1280 1280
#define HEIGHT_768 768

#define WIDTH_1936 1936
#define HEIGHT_1552 1552


#define VO_IN_WIDTH WIDTH_1280
#define VO_IN_HEIGHT HEIGHT_768

#define VI_OUT_WIDTH WIDTH_1936
#define VI_OUT_HEIGHT HEIGHT_1552


static const ot_eis_vproc_pipe_attr g_vproc_pipe_attr = {
    .mode = OT_EIS_VPROC_WORK_MODE_PIPE,
    .frc = {
        .src_frame_rate = VPROC_FRAME_RATE_DEFAULT,
        .dst_frame_rate = VPROC_FRAME_RATE_DEFAULT,
    },
    .image_attr = {
        .width = VI_OUT_WIDTH,
        .height = VI_OUT_HEIGHT,
        .bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8,
        .pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
        .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
        .compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    },
};

static const ot_eis_vproc_chn_attr g_vproc_chn_attr = {
    .image_attr = {
        .width = WIDTH_3840,
        .height = HEIGHT_2160,
        .bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8,
        .pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
        .layout = OT_EIS_IMAGE_LAYOUT_LINEAR,
        .compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE,
    },
    .mode = OT_EIS_VPROC_WORK_MODE_PIPE,
    .frc = {
        .src_frame_rate = VPROC_FRAME_RATE_DEFAULT,
        .dst_frame_rate = VPROC_FRAME_RATE_DEFAULT,
    },
    .frame_queue_depth = VPROC_FRAME_QUEUE_DEPTH,
    .pool_handle = 0,
};

static sample_vproc_attr g_vproc_attr = {
    // pipe param
    .pipe_attr[0] = g_vproc_pipe_attr,
    .pipe_attr[1] = g_vproc_pipe_attr,
    .pipe_attr[2] = g_vproc_pipe_attr,
    .pipe_attr[3] = g_vproc_pipe_attr,

    // group param
    .grp_attr = {
        .max_width = WIDTH_3840,
        .max_height = HEIGHT_2160,
        .pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR,
    },

    // channel param
    .chn_attr[0] = g_vproc_chn_attr,
    .chn_attr[1] = g_vproc_chn_attr,
    .chn_attr[2] = g_vproc_chn_attr,
    .chn_attr[3] = g_vproc_chn_attr,
};


static ot_s32 sample_media_vproc_start_pipe(ot_eis_handle grp_handle, const ot_bool *pipe_sw,
                                           ot_eis_handle pipe_hdl[],
                                           ot_eis_vproc_pipe_attr pipe_attr[])
{
    ot_s32 ret = OT_SUCCESS;
    ot_s32 i = 0;

    for(i = 0; i < OT_EIS_VPROC_GRP_PIPE_MAX_NUM; ++i) {
        if (pipe_sw[i] == OT_TRUE) {

            ret = ot_eis_vproc_pipe_create(&pipe_hdl[i], &pipe_attr[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_pipe_create failed with %#x\n", ret);
                goto destroy_pipe;
            }

            ret = ot_eis_vproc_grp_attach_pipe(grp_handle, pipe_hdl[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_attach_pipe failed with %#x\n", ret);
                goto destroy_pipe;
            }
        }
    }

    return OT_SUCCESS;

destroy_pipe:

    for (i = i - 1; i >= 0; i--) {
        if (pipe_sw[i] == OT_TRUE) {
            ret = ot_eis_vproc_grp_detach_pipe(grp_handle, pipe_hdl[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_detach_pipe failed with %#x!\n", ret);
            }

            ret = ot_eis_vproc_pipe_destroy(pipe_hdl[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_pipe_destroy failed with %#x!\n", ret);
            }
        }
    }
    return OT_FAILURE;
}


static ot_s32 sample_vproc_set_chn_param(ot_u32 id, sample_vproc_attr* vproc_attr)
{
    ot_s32 ret = OT_FAILURE;
    ot_u32 i = id;
    if (vproc_attr->is_set_attr != OT_TRUE) {
        return OT_SUCCESS;
    }
    if (vproc_attr->set_attr.mirror_param.enable) {
        ret = ot_eis_vproc_chn_set_mirror(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.mirror_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_mirror failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }
    if (vproc_attr->set_attr.flip_param.enable) {
        ret = ot_eis_vproc_chn_set_flip(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.flip_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_flip failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }
    if (vproc_attr->set_attr.crop_param.enable) {
        ret = ot_eis_vproc_chn_set_crop(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.crop_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_crop failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }

    if (vproc_attr->set_attr.mosaic_param.enable) {
        ret = ot_eis_vproc_chn_set_mosaic(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.mosaic_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_mosaic failed with %#x\n", ret);
            return OT_FAILURE;;
        }
    }

    if (vproc_attr->set_attr.line_param.enable) {
        ret = ot_eis_vproc_chn_set_line(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.line_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_line failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }
    
    if (vproc_attr->set_attr.cover_param.enable) {
        ret = ot_eis_vproc_chn_set_cover(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.cover_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_cover failed with %#x\n", ret);
            return OT_FAILURE;;
        }
    }
    if (vproc_attr->set_attr.rect_param.enable) {
        ret = ot_eis_vproc_chn_set_rect(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.rect_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_rect failed with %#x\n", ret);
            return OT_FAILURE;;
        }
    }

    if (vproc_attr->set_attr.osd_param.enable) {
        ret = ot_eis_vproc_chn_set_osd(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.osd_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_osd failed with %#x\n", ret);
            return OT_FAILURE;;
        }
    }

    if (vproc_attr->set_attr.rotation_param.enable) {
        ret = ot_eis_vproc_chn_set_rotation(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.rotation_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_rotation failed with %#x\n", ret);
            return OT_FAILURE;;
        }
    }

    if (vproc_attr->set_attr.luma_param.enable) {
        ret = ot_eis_vproc_chn_set_luma(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.luma_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_set_luma failed with %#x\n", ret);
            return OT_FAILURE;;
        }
    }

    ret = ot_eis_vproc_chn_set_scale_coef(vproc_attr->chn_hdl[i], &(vproc_attr->set_attr.scale_coef_param));
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_chn_set_scale_coef failed with %#x\n", ret);
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_media_vproc_start_chn(ot_eis_handle grp_handle, const ot_bool *chn_sw, sample_vproc_attr* vproc_attr)
{
    ot_s32 ret = OT_SUCCESS;
    ot_s32 i = 0;

    for (i = 0; i < OT_EIS_VPROC_GRP_CHN_MAX_NUM; ++i) {
        if (chn_sw[i] == OT_TRUE) {
            ret = ot_eis_vproc_chn_create(i, &vproc_attr->chn_hdl[i], &vproc_attr->chn_attr[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_chn_create failed with %#x\n", ret);
                goto destroy_chn;
            }

            ret = sample_vproc_set_chn_param(i, vproc_attr);
            if (ret != OT_SUCCESS) {
                sample_print("sample_vproc_set_chn_param error chn handle 0x%llx return 0x%x \n",
                    (ot_u64)vproc_attr->chn_hdl[i], ret);
                goto destroy_chn;
            }

            ret = ot_eis_vproc_grp_attach_chn(grp_handle, vproc_attr->chn_hdl[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_attach_chn failed with %#x\n", ret);
                goto destroy_chn;
            }

        }
    }
    return OT_SUCCESS;

destroy_chn:

    for (i = i - 1; i >= 0; i--) {
        if (chn_sw[i] == OT_TRUE) {
            ret = ot_eis_vproc_grp_detach_chn(grp_handle, vproc_attr->chn_hdl[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_grp_detach_chn failed with %#x!\n", ret);
            }

            ret = ot_eis_vproc_chn_destroy(vproc_attr->chn_hdl[i]);
            if (ret != OT_SUCCESS) {
                sample_print("ot_eis_vproc_chn_destroy failed with %#x!\n", ret);
            }
        }
    }
    return OT_FAILURE;
}

static ot_s32 sample_media_vproc_destroy_pipe(ot_eis_handle grp_hdl)
{
    ot_s32 ret = OT_SUCCESS;
    ot_eis_handle pipe_hdls[OT_EIS_VPROC_GRP_PIPE_MAX_NUM];

    ot_s32 pipe_num = 0;
    ret = ot_eis_vproc_grp_get_attched_pipes(grp_hdl, pipe_hdls, &pipe_num);

    for (int i = 0; i < pipe_num; i++) {
        ret = ot_eis_vproc_grp_detach_pipe(grp_hdl, pipe_hdls[i]);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_detach_pipe failed with %#x!\n", ret);
        }

        ret = ot_eis_vproc_pipe_destroy(pipe_hdls[i]);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_pipe_destroy failed with %#x!\n", ret);
        }
    }

    return ret;
}

static ot_s32 sample_media_set_vproc_grp_param(ot_eis_handle vproc_grp_hdl, sample_vproc_attr* vproc_attr)
{
    ot_s32 ret = OT_FAILURE;
    if(vproc_attr->is_set_attr != OT_TRUE) {
        return OT_SUCCESS;
    }

    if (vproc_attr->set_attr.gdc_param.enable) {
        ret = ot_eis_vproc_grp_set_gdc(vproc_grp_hdl, &(vproc_attr->set_attr.gdc_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_set_gdc failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }
    if (vproc_attr->set_attr.fisheye_param.enable) {
        ret = ot_eis_vproc_grp_set_fisheye(vproc_grp_hdl, &(vproc_attr->set_attr.fisheye_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_set_fisheye failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }

    if (vproc_attr->set_attr.ldc_param.enable) {
        ret = ot_eis_vproc_grp_set_ldc(vproc_grp_hdl, &(vproc_attr->set_attr.ldc_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_set_ldc failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }

    if (vproc_attr->set_attr.pmf_param.enable) {
        ret = ot_eis_vproc_grp_set_pmf(vproc_grp_hdl, &(vproc_attr->set_attr.pmf_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_set_pmf failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }
    if (vproc_attr->set_attr.xylut_param.enable) {
        ret = ot_eis_vproc_grp_set_xylut(vproc_grp_hdl, &(vproc_attr->set_attr.xylut_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_set_xylut failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }
    if (vproc_attr->set_attr.nr_param.enable) {
        vproc_attr->set_attr.nr_param.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_FRAME;
        ret = ot_eis_vproc_grp_set_3dnr(vproc_grp_hdl, &(vproc_attr->set_attr.nr_param));
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_set_3dnr failed with %#x\n", ret);
            return OT_FAILURE;
        }
    }

    return OT_SUCCESS;
}

static ot_vproc_grp_id g_grp_id = 0;

static ot_s32 sample_media_vproc_start(ot_bool pipe_sw[], ot_bool chn_sw[], sample_vproc_attr* vproc_attr)
{
    ot_s32 ret = OT_SUCCESS;

    ret = ot_eis_vproc_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_init failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    ot_eis_handle vproc_grp_hdl;
    ret = ot_eis_vproc_grp_create(g_grp_id, &vproc_grp_hdl, &vproc_attr->grp_attr);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_create failed with %#x\n", ret);
        return OT_FAILURE;
    }
    vproc_attr->grp_hdl = vproc_grp_hdl;
    g_grp_id++;
    ret = sample_media_set_vproc_grp_param(vproc_grp_hdl, vproc_attr);
    if (ret != OT_SUCCESS) {
        sample_print("sample_media_set_vproc_grp_param error grp handle 0x%llx\n", (ot_u64)vproc_grp_hdl);
        goto destroy_grp;
    }

    ret = ot_eis_vproc_grp_start(vproc_grp_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_start failed with %#x\n", ret);
        goto destroy_grp;
    }

    ret = sample_media_vproc_start_pipe(vproc_grp_hdl, pipe_sw, vproc_attr->pipe_hdl, vproc_attr->pipe_attr);
    if (ret != OT_SUCCESS) {
        goto stop_grp;
    }

    ret = sample_media_vproc_start_chn(vproc_grp_hdl, chn_sw, vproc_attr);
    if (ret != OT_SUCCESS) {
        goto destroy_pipe;
    }

    return OT_SUCCESS;

destroy_pipe:
    ret = sample_media_vproc_destroy_pipe(vproc_grp_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_pipe_destroy failed with %#x!\n", ret);
    }

stop_grp:
    ret = ot_eis_vproc_grp_stop(vproc_grp_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_stop failed with %#x!\n", ret);
    }

destroy_grp:
    ret = ot_eis_vproc_grp_destroy(vproc_grp_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_destroy failed with %#x!\n", ret);
    }
    if (g_grp_id > 0) {
        g_grp_id--;
    }
    return OT_FAILURE;
}

ot_s32 sample_media_vproc_grp_detach_chns(ot_eis_handle grp_hdl, ot_eis_handle chn_hdls[], ot_s32 chn_num)
{
    ot_s32 ret = OT_SUCCESS;

    for (int i = 0; i < chn_num; i++) {
        ret = ot_eis_vproc_grp_detach_chn(grp_hdl, chn_hdls[i]);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_detach_chn failed with %#x!\n", ret);
        }
    }

    return ret;
}

ot_s32 sample_media_vproc_grp_detach_pipes(ot_eis_handle grp_hdl, ot_eis_handle pipe_hdls[], ot_s32 pipe_num)
{
    ot_s32 ret = OT_SUCCESS;

    for (int i = 0; i < pipe_num; i++) {
        ret = ot_eis_vproc_grp_detach_pipe(grp_hdl, pipe_hdls[i]);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_grp_detach_pipe failed with %#x!\n", ret);
        }
    }
    return ret;
}

ot_s32 sample_media_vproc_destroy_chns(ot_eis_handle hdls[], ot_s32 num)
{
    ot_s32 ret = OT_SUCCESS;

    for (int i = 0; i < num; i++) {
        ret = ot_eis_vproc_chn_destroy(hdls[i]);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_destroy failed with %#x!\n", ret);
        }
    }
    return ret;
}

ot_s32 sample_media_vproc_destroy_pipes(ot_eis_handle hdls[], ot_s32 num)
{
    ot_s32 ret = OT_SUCCESS;

    for (int i = 0; i < num; i++) {
        ret = ot_eis_vproc_pipe_destroy(hdls[i]);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_pipe_destroy failed with %#x!\n", ret);
        }
    }
    return ret;
}

ot_s32 sample_media_vproc_stop(sample_vproc_attr* vproc_attr)
{
    ot_eis_handle pipe_hdls[OT_EIS_VPROC_GRP_PIPE_MAX_NUM];
    ot_eis_handle chn_hdls[OT_EIS_VPROC_GRP_CHN_MAX_NUM];
    ot_s32 pipe_num = 0;
    ot_s32 chn_num = 0;

    ot_s32 ret = OT_SUCCESS;

    ret = ot_eis_vproc_grp_stop(vproc_attr->grp_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_stop failed with %#x!\n", ret);
    }

    // detach chns
    ret = ot_eis_vproc_grp_get_attched_chns(vproc_attr->grp_hdl, chn_hdls, &chn_num);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_get_attched_chns failed with %#x!\n", ret);
    }

    ret = sample_media_vproc_grp_detach_chns(vproc_attr->grp_hdl, chn_hdls, chn_num);
    if (ret != OT_SUCCESS) {
        sample_print("sample_media_vproc_grp_detach_chns failed with %#x!\n", ret);
    }

    // detach pipes
    ret = ot_eis_vproc_grp_get_attched_pipes(vproc_attr->grp_hdl, pipe_hdls, &pipe_num);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_get_attched_pipes failed with %#x!\n", ret);
    }

    ret = sample_media_vproc_grp_detach_pipes(vproc_attr->grp_hdl, pipe_hdls, pipe_num);
    if (ret != OT_SUCCESS) {
        sample_print("sample_media_vproc_grp_detach_pipes failed with %#x!\n", ret);
    }

    // destroy chns
    ret = sample_media_vproc_destroy_chns(chn_hdls, chn_num);
    if (ret != OT_SUCCESS) {
        sample_print("smaple_media_vproc_destroy_chns failed with %#x!\n", ret);
    }

    // destroy pipes
    ret = sample_media_vproc_destroy_pipes(pipe_hdls, pipe_num);
    if (ret != OT_SUCCESS) {
        sample_print("smaple_media_vproc_destroy_pipes failed with %#x!\n", ret);
    }

    // destroy group
    ret = ot_eis_vproc_grp_destroy(vproc_attr->grp_hdl);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_vproc_grp_destroy failed with %#x!\n", ret);
    }
    if (g_grp_id > 0) {
        g_grp_id--;
    }

    return ret;
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

static ot_s32 sample_media_vi_get_vproc_send_frame(sample_vproc_attr * vproc_attr,
                                                  ot_bool pipe_sw[], ot_bool chn_sw[],
                                                  ot_eis_handle vi_chn_hdl,
                                                  ot_eis_handle vo_in_port_hdl)
{
    ot_s32 ret = OT_SUCCESS;
    ot_eis_handle pipe_h;
    ot_eis_handle chn_h;
    ret = sample_media_vproc_search_user(vproc_attr, pipe_sw, chn_sw, &pipe_h, &chn_h);
    if (ret != OT_SUCCESS) {
        sample_print("sample_media_vproc_search_user failed with %#x\n", ret);
        return ret;
    }

    do {
        printf("--------------- start send frame !---------------\n");

        ot_eis_video_frame vi_frame;
        ret = ot_eis_vi_chn_get_frame(vi_chn_hdl, &vi_frame, -1);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vi_chn_get_frame failed with %#x\n", ret);
            return ret;
        }
        printf(">>>>>>>>>>>>>>>> get vi frame >>>>>>>>>>>>>>>>\n");

        ret = ot_eis_vproc_pipe_send_frame(pipe_h, &vi_frame, 1);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_pipe_send_frame failed with %#x\n", ret);
            ot_eis_vi_chn_release_frame(vi_chn_hdl, &vi_frame);
            return ret;
        }
        ot_eis_vi_chn_release_frame(vi_chn_hdl, &vi_frame);

        printf(">>>>>>>>>>>>>>>> send vproc frame >>>>>>>>>>>>>>>>\n");

        ot_eis_video_frame vproc_frame;
        ret = ot_eis_vproc_chn_acquire_frame(chn_h, &vproc_frame, -1);
        if (ret != OT_SUCCESS) {
            sample_print("ot_eis_vproc_chn_acquire_frame failed with %#x\n", ret);
            return ret;
        }

        printf(">>>>>>>>>>>>>>>> get vproc frame >>>>>>>>>>>>>>>>\n");

        ret = ot_eis_vo_in_port_send_frame(vo_in_port_hdl, &vproc_frame);
        if (ret != OT_SUCCESS) {
            sample_print("ot_media_vo_in_port_send_frame failed with %#x\n", ret);
            ot_eis_vproc_chn_release_frame(chn_h, &vproc_frame);
            return ret;
        }
        ot_eis_vproc_chn_release_frame(chn_h, &vproc_frame);

        printf(">>>>>>>>>>>>>>>> send vo frame >>>>>>>>>>>>>>>>\n");

        usleep(10000);
        if (getchar() == 'b') {
            break;
        }
    } while (1);

    return ret;
}

ot_s32 sample_comm_start_vproc(sample_vproc_attr* vproc_attr, ot_bool pipe_sw[], ot_bool chn_sw[])
{
    ot_s32 ret;

    ret = sample_media_vproc_start(pipe_sw, chn_sw, vproc_attr);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    return OT_SUCCESS;
}

ot_s32 sample_comm_stop_vproc(sample_vproc_attr* vproc_attr)
{
    ot_s32 ret;

    ret = sample_media_vproc_stop(vproc_attr);
    if (ret != OT_SUCCESS) {
        return ret;
    }
    return ret;
}

ot_s32 sample_comm_vi_get_vproc_send_frame(sample_vproc_attr * vproc_attr, ot_bool pipe_sw[], ot_bool chn_sw[],
                                          ot_eis_handle vi_chn_hdl, ot_eis_handle vo_in_port_hdl)
{
    return sample_media_vi_get_vproc_send_frame(vproc_attr, pipe_sw, chn_sw, vi_chn_hdl, vo_in_port_hdl);
}

ot_void sample_comm_vproc_get_default_grp_info(sample_sns_type sns_type, ot_eis_vproc_grp_attr *grp_info)
{
    ot_eis_img_size size = {0};

    if(grp_info == OT_NULL){
        sample_print("sample_media_vproc_get_default_attr_by_snsor failed with param invalid!\n");
        return;
    }

    sample_comm_vi_get_size_by_sns_type(sns_type, &size);
    grp_info->max_width = size.width;
    grp_info->max_height = size.height;
    grp_info->pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR;
}

ot_void sample_comm_vproc_get_default_pipe_info(sample_sns_type sns_type, ot_eis_vproc_pipe_attr *pipe_attr,
    ot_eis_vproc_work_mode mode)
{
    ot_eis_img_size size = {0};

    if(pipe_attr == OT_NULL){
        sample_print("sample_media_vproc_get_default_attr_by_snsor failed with param invalid!\n");
        return;
    }

    sample_comm_vi_get_size_by_sns_type(sns_type, &size);
    pipe_attr->image_attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR;
    pipe_attr->image_attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR;
    pipe_attr->image_attr.bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8;
    pipe_attr->image_attr.width = size.width;
    pipe_attr->image_attr.height = size.height;
    pipe_attr->frc.src_frame_rate = VPROC_FRAME_RATE_DEFAULT;
    pipe_attr->frc.dst_frame_rate = VPROC_FRAME_RATE_DEFAULT;
    pipe_attr->mode = mode;

}

ot_void sample_comm_vproc_get_default_chn_info(sample_sns_type sns_type, ot_eis_vproc_chn_attr *chn_attr,
    ot_eis_vproc_work_mode mode)
{
    ot_eis_img_size size = {0};

    if(chn_attr == OT_NULL){
        sample_print("sample_media_vproc_get_default_attr_by_snsor failed with param invalid!\n");
        return;
    }
    sample_comm_vi_get_size_by_sns_type(sns_type, &size);

    chn_attr->image_attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR;
    chn_attr->image_attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR;
    chn_attr->image_attr.width = size.width;
    chn_attr->image_attr.height = size.height;
    chn_attr->image_attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR;
    chn_attr->image_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE;
    chn_attr->image_attr.bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8;
    chn_attr->frc.src_frame_rate = VPROC_FRAME_RATE_DEFAULT;
    chn_attr->frc.dst_frame_rate = VPROC_FRAME_RATE_DEFAULT;
    if (mode == OT_EIS_VPROC_WORK_MODE_PIPE) {
        chn_attr->frame_queue_depth = 0;
    } else {
        chn_attr->frame_queue_depth = VPROC_FRAME_QUEUE_DEPTH;
    }
    chn_attr->mode = mode;

}


ot_s32 sample_media_vproc_get_default_attr_by_snsor(sample_sns_type sns_type, sample_vproc_attr* vproc_attr)
{
    ot_eis_img_size size = {0};

    if(vproc_attr == OT_NULL){
        sample_print("sample_media_vproc_get_default_attr_by_snsor failed with param invalid!\n");
        return -1;
    }
    (ot_void)memcpy_s(vproc_attr, sizeof(sample_vproc_attr), &g_vproc_attr, sizeof(sample_vproc_attr));
    sample_comm_vi_get_size_by_sns_type(sns_type, &size);
    vproc_attr->grp_attr.max_width = size.width;
    vproc_attr->grp_attr.max_height = size.height;
    vproc_attr->pipe_attr[0].image_attr.width  = size.width;
    vproc_attr->pipe_attr[0].image_attr.height = size.height;
    vproc_attr->pipe_attr[1].image_attr.width  = size.width;
    vproc_attr->pipe_attr[1].image_attr.height = size.height;
    vproc_attr->pipe_attr[1].mode = OT_EIS_VPROC_WORK_MODE_USER;
    vproc_attr->pipe_attr[2].image_attr.width  = size.width;
    vproc_attr->pipe_attr[2].image_attr.height = size.height;
    vproc_attr->pipe_attr[2].mode = OT_EIS_VPROC_WORK_MODE_USER;
    vproc_attr->pipe_attr[3].image_attr.width  = size.width;
    vproc_attr->pipe_attr[3].image_attr.height = size.height;
    vproc_attr->pipe_attr[3].mode = OT_EIS_VPROC_WORK_MODE_USER;

    vproc_attr->chn_attr[0].image_attr.width  = VO_IN_WIDTH;
    vproc_attr->chn_attr[0].image_attr.height = VO_IN_HEIGHT;
    vproc_attr->chn_attr[0].frame_queue_depth = 0;
    vproc_attr->chn_attr[1].image_attr.width  = size.width;
    vproc_attr->chn_attr[1].image_attr.height = size.height;
    vproc_attr->chn_attr[1].mode = OT_EIS_VPROC_WORK_MODE_USER;
    vproc_attr->chn_attr[2].image_attr.width  = size.width;
    vproc_attr->chn_attr[2].image_attr.height = size.height;
    vproc_attr->chn_attr[2].mode = OT_EIS_VPROC_WORK_MODE_USER;
    vproc_attr->chn_attr[3].image_attr.width  = size.width;
    vproc_attr->chn_attr[3].image_attr.height = size.height;
    vproc_attr->chn_attr[3].mode = OT_EIS_VPROC_WORK_MODE_USER;

    return 0;
}



#pragma pack(1)
typedef struct {
    ot_s32 mesh_width;
    ot_s32 mesh_height;
    ot_s32 cell_width;
    ot_s32 cell_height;
    ot_s32 dst_width;
    ot_s32 dst_height;
    ot_s8 reserved[76];
} ot_vproc_gdc_2dlut_head;
#pragma pack()

ot_eis_vproc_lut_cell_size sample_get_lut_cell_size(ot_u32 cell_size)
{
    ot_eis_vproc_lut_cell_size ret = OT_EIS_LUT_CELL_SIZE_BUTT;

    switch (cell_size) {
    case 16:
        ret = OT_EIS_LUT_CELL_SIZE_16;
        break;
    case 32:
        ret = OT_EIS_LUT_CELL_SIZE_32;
        break;
    case 64:
        ret = OT_EIS_LUT_CELL_SIZE_64;
        break;

    case 128:
        ret = OT_EIS_LUT_CELL_SIZE_128;
        break;

    case 256:
        ret = OT_EIS_LUT_CELL_SIZE_256;
        break;

    default:
        break;
    }
    return ret;
}


ot_s32 sample_get_file_size(FILE *file)
{
    ot_s32 size;

    // 将文件指针移动到文件末尾
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek");
        return -1; // 错误处理
    }

    // 获取当前文件指针的位置，即文件大小
    size = ftell(file);
    if (size == -1) {
        perror("ftell");
        return -1; // 错误处理
    }

    // 将文件指针移回文件开头
    if (fseek(file, 0, SEEK_SET) != 0) {
        perror("fseek");
        return -1; // 错误处理
    }

    return size;
}


ot_s32 sample_comm_vproc_load_gdc_2dlut_file(ot_s8 *name, ot_eis_vproc_gdc_param *gdc_param,
    ot_eis_vproc_xylut_param *xylut)
{
    ot_s32 ret = OT_FAILURE;
    ot_s32 bytes_read;
    ot_vproc_gdc_2dlut_head head = {0};
    ot_phys_addr lut_addr = 0;
    ot_void *lut_data = OT_NULL;
    ot_u32 lut_len = 0;
    ot_u32 file_size = 0;
    ot_eis_vproc_lut_cell_size cell_size = OT_EIS_LUT_CELL_SIZE_BUTT;
    ot_smr_alloc_attr smr_alloc_attr = {0};

    FILE* fp = fopen(name, "rb");
    if (fp == NULL) {
        sample_print("fp == NULL\n");
        return OT_FAILURE;
    }
    file_size = sample_get_file_size(fp);
    bytes_read = fread(&head, 1, sizeof(ot_vproc_gdc_2dlut_head), fp);
    if (bytes_read == 0) {
        sample_print("Failed to read from file");
        goto close_file;
    }
    cell_size = sample_get_lut_cell_size(head.cell_width);

    lut_len = file_size - sizeof(ot_vproc_gdc_2dlut_head);

    if (head.cell_width != 0 && head.cell_width != head.cell_height && cell_size == OT_EIS_LUT_CELL_SIZE_BUTT) {
        sample_print("err head mesh_width %d mesh_height %d cell_width %d cell_height %d dst_width %d dst_height %d lut len %d\n",
            head.mesh_width, head.mesh_height, head.cell_width, head.cell_height, head.dst_width, head.dst_height, lut_len);
        goto close_file;
    }
    smr_alloc_attr.len = lut_len;
    smr_alloc_attr.cached = OT_FALSE;
    strcpy(smr_alloc_attr.region_name , "anony");
    ret = ot_smr_alloc(&smr_alloc_attr, &lut_addr, &lut_data);
    if (ret != OT_SUCCESS) {
        sample_print("ot_smr_alloc failed");
        goto close_file;
    }
    bytes_read = fread(lut_data, sizeof(ot_u8), lut_len, fp);
    if (bytes_read == 0) {
        sample_print("Failed to read from file");
        goto free_lut;
    }
    gdc_param->cfg->cell_size = cell_size;
    gdc_param->cfg->gdc_mode = OT_EIS_WORK_2D_LUT_POLAT;
    gdc_param->cfg->mid.width = head.dst_width;
    gdc_param->cfg->mid.height = head.dst_height;
    gdc_param->cfg->dst.width = head.dst_width;
    gdc_param->cfg->dst.height = head.dst_height;
    xylut->enable = OT_TRUE;
    xylut->gdc_lut->gdc_2dlut_data = lut_addr;
    xylut->gdc_lut->xylut_len = lut_len;
    fclose(fp);
    return OT_SUCCESS;

free_lut:
    ot_smr_free(lut_addr);
close_file:
    fclose(fp);
    return OT_FAILURE;
}


ot_void sample_comm_vproc_free_2dlut(ot_eis_vproc_xylut_param *xylut)
{
    ot_smr_free(xylut->gdc_lut->gdc_2dlut_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
