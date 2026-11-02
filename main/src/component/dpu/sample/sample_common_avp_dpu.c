/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */
#include "sample_common_avp_dpu.h"

#include <limits.h>
#include "securec.h"
#include "ot_media_pipe.h"
#include "ot_vrb.h"

ot_u32 sample_common_avp_dpu_calc_stride(ot_u32 width, ot_u8 align)
{
    sample_avp_dpu_check_exps_return(align == 0, 0, "align can't be zero!\n");
    return (width + (align - width % align) % align);
}

/* function : create dpu rect memory info */
static ot_s32 sample_common_avp_dpu_rect_create_mem_info(ot_avp_dpu_rect_mem_info *mem_info, ot_u32 size)
{
    ot_s32 ret;
    ot_smr_alloc_attr alloc_attr = {0};

    strcpy(alloc_attr.region_name, "anony");
    strcpy(alloc_attr.chunk_name, "sample_dpu_rect");
    alloc_attr.len = size;
    alloc_attr.cached = OT_FALSE;

    mem_info->size = size;
    ret = ot_smr_alloc(&alloc_attr, (ot_phys_addr *)(&mem_info->phys_addr),
        (ot_void **)&mem_info->virt_addr);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
        "Err(%#x), ot_smr_alloc failed!\n", ret);
    return ret;
}

static ot_s32 sample_avp_dpu_get_lut_from_file(const ot_char *file_name,
    ot_avp_dpu_rect_mem_info *mem_info)
{
    ot_s32 ret;
    ot_u32 size;
    ot_void *virt_addr = OT_NULL;
    FILE *fp = OT_NULL;
    ot_char path[AVP_DPU_PATH_MAX + 1] = {0};

    sample_avp_dpu_check_exps_return(((strlen(file_name) > AVP_DPU_PATH_MAX) || (realpath(file_name, path) == OT_NULL)),
        OT_FAILURE, "Error, file_name is invalid!\n");
    fp = fopen(file_name, "rb");
    sample_avp_dpu_check_exps_return(fp == OT_NULL,
        OT_FAILURE, "Err, open lut file failed!\n");
    ret = fseek(fp, 0L, SEEK_END);
    sample_avp_dpu_check_exps_goto(ret == -1, fail_0, "Error, fseek failed!\n");
    size = ftell(fp);
    sample_avp_dpu_check_exps_goto(size <= 0, fail_0, "Error, ftell failed!\n");
    ret = fseek(fp, 0L, SEEK_SET);
    sample_avp_dpu_check_exps_goto(ret == -1, fail_0, "Error, fseek failed!\n");

    ret = memset_s(mem_info, sizeof(ot_avp_dpu_rect_mem_info), 0, sizeof(ot_avp_dpu_rect_mem_info));
    sample_avp_dpu_check_exps_goto(ret != EOK, fail_0, "Err, memset_s mem_info failed!\n");
    ret = sample_common_avp_dpu_rect_create_mem_info(mem_info, size);
    sample_avp_dpu_check_exps_goto(ret != EOK, fail_0, "Err, dpu_rect_create_mem_info failed!\n");

    virt_addr = sample_avp_dpu_convert_addr_to_ptr(ot_void, mem_info->virt_addr);
    ret = fread(virt_addr, size, 1, fp);
    sample_avp_dpu_check_exps_goto(ret != 1, fail_1, "Error,read lut file failed!\n");
    if (fp != OT_NULL) {
        fclose(fp);
    }

    return OT_SUCCESS;
fail_1:
    sample_avp_dpu_smr_free(mem_info->phys_addr);
fail_0:
    if (fp != OT_NULL) {
        fclose(fp);
    }
    return OT_FAILURE;
}

ot_s32 sample_common_avp_dpu_rect_load_lut(const ot_char *file_name,
    ot_avp_dpu_rect_mem_info *mem_info, ot_s32 *lut_id)
{
    ot_s32 ret;
    ot_char path[AVP_DPU_PATH_MAX + 1] = {0};

    sample_avp_dpu_check_exps_return(((file_name == OT_NULL) || (mem_info == OT_NULL) || (lut_id == OT_NULL)),
        OT_FAILURE, "Error, model_file or nnie_model or lut_id is NULL!\n");
    sample_avp_dpu_check_exps_return(((strlen(file_name) > AVP_DPU_PATH_MAX) || (realpath(file_name, path) == OT_NULL)),
        OT_FAILURE, "Error, file_name is invalid!\n");

    ret = sample_avp_dpu_get_lut_from_file(file_name, mem_info);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS,
        OT_FAILURE, "Err, get_lut_from_file failed!\n");

    ret = ot_avp_dpu_rect_load_lut(mem_info, lut_id);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail,
        "Error(%#x), ot_avp_dpu_rect_load_lut failed!\n", ret);

    return ret;
fail:
    sample_avp_dpu_smr_free(mem_info->phys_addr);
    return ret;
}

ot_void sample_common_avp_dpu_rect_unload_lut(ot_avp_dpu_rect_mem_info *mem_info,
    ot_s32 lut_id)
{
    if (mem_info == OT_NULL) {
        sample_avp_dpu_trace_err("Error, mem_info can't be OT_NULL!\n");
        return;
    }
    (ot_void)ot_avp_dpu_rect_unload_lut(lut_id);
    sample_avp_dpu_smr_free(mem_info->phys_addr);
}

/* function : start dpu rect grp. */
ot_s32 sample_common_avp_dpu_rect_start(ot_eis_handle* rect_grp_hdl,
    ot_avp_dpu_rect_grp_attr *grp_attr, ot_avp_dpu_rect_chn_attr *chn_attr, ot_eis_handle rect_chn_hdl[OT_AVP_DPU_RECT_MAX_CHN_NUM])
{
    ot_eis_handle dpu_rect_chn_hdl;
    ot_s32 ret;
    ot_s32 j;
    ot_s32 dpu_rect_chn_num;

    sample_avp_dpu_check_exps_return(((grp_attr == OT_NULL) || (chn_attr == OT_NULL) || (rect_grp_hdl == OT_NULL)),
        OT_FAILURE, "Error, grp_attr or chn_attr or rect_grp_hdl is OT_NULL!\n");

    ret = ot_avp_dpu_rect_create_grp(rect_grp_hdl, grp_attr);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
        "Err(%#x), ot_avp_dpu_rect_create_grp failed!\n", ret);

    if (grp_attr->rect_mode == OT_AVP_DPU_RECT_MODE_SINGLE) {
        dpu_rect_chn_num = 1;
    } else  {
        dpu_rect_chn_num = OT_AVP_DPU_RECT_MAX_CHN_NUM;
    }

    for (j = 0; j < dpu_rect_chn_num; j++) {

        ret = ot_avp_dpu_rect_create_chn(*rect_grp_hdl, &dpu_rect_chn_hdl);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
            "Err(%#x), ot_avp_dpu_rect_create_chn failed!\n", ret);

        ret = ot_avp_dpu_rect_set_chn_attr(dpu_rect_chn_hdl, &chn_attr[j]);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, rect_start_fail,
            "Err(%#x), ot_avp_dpu_rect_set_chn_attr failed!\n", ret);

        rect_chn_hdl[j] = dpu_rect_chn_hdl;
        ret = ot_avp_dpu_rect_enable_chn(dpu_rect_chn_hdl);
        sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, rect_start_fail,
            "Err(%#x), ot_avp_dpu_rect_enable_chn failed!\n", ret);
    }

    ret = ot_avp_dpu_rect_start_grp(*rect_grp_hdl);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, rect_start_fail,
        "Err(%#x), ot_avp_dpu_rect_start_grp failed!\n", ret);

    return OT_SUCCESS;
rect_start_fail:
    ot_avp_dpu_rect_destroy_chn(dpu_rect_chn_hdl);
    return OT_FAILURE;
}

/* function : stop dpu rect grp */
ot_s32 sample_common_avp_dpu_rect_stop(ot_eis_handle rect_grp_hdl, ot_eis_handle rect_chn_hdl[OT_AVP_DPU_RECT_MAX_CHN_NUM],
                                                ot_avp_dpu_rect_mode rect_mode)
{
    ot_s32 j;
    ot_s32 ret;
    ot_s32 dpu_rect_chn_num;

    ret = ot_avp_dpu_rect_stop_grp(rect_grp_hdl);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
        "Err(%#x), ot_avp_dpu_rect_stop_grp failed!\n", ret);

    if (rect_mode == OT_AVP_DPU_RECT_MODE_SINGLE) {
        dpu_rect_chn_num = 1;
    } else {
        dpu_rect_chn_num = OT_AVP_DPU_RECT_MAX_CHN_NUM;
    }

    for (j = 0; j < dpu_rect_chn_num; j++) {
        ret = ot_avp_dpu_rect_disable_chn(rect_chn_hdl[j]);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
            "Err(%#x), ot_avp_dpu_rect_disable_chn failed!\n", ret);

        ret = ot_avp_dpu_rect_destroy_chn(rect_chn_hdl[j]);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
            "Err(%#x), ot_avp_dpu_rect_destroy_chn failed!\n", ret);
    }

    ret = ot_avp_dpu_rect_destroy_grp(rect_grp_hdl);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
        "Err(%#x), ot_avp_dpu_rect_destroy_grp failed!\n", ret);

    return OT_SUCCESS;
}

static ot_s32 sample_avp_dpu_rect_read_one_frame(FILE *fp, ot_u8 *y,
    ot_u32 width, ot_u32 height, ot_u32 stride)
{
    ot_u8 *dst = OT_NULL;
    ot_u32 row;
    ot_s32 ret;

    dst = y;
    for (row = 0; row < height; row++) {
        ret = fread(dst, width, 1, fp);
        sample_avp_dpu_check_exps_return(ret != 1, OT_FAILURE, "fread failed!\n");
        dst += stride;
    }

    return OT_SUCCESS;
}

/* function : get frame from file */
ot_s32 sample_common_avp_dpu_rect_get_frame_from_file(FILE *fp, const ot_eis_img_size *pic_size,
    ot_u32 stride, ot_eis_img_frame *frame_info, ot_buffer_pool_blk vrb_blk)
{
    ot_u32 size;
    ot_phys_addr phys_addr;
    ot_void *virt_addr = OT_NULL;
    ot_s32 ret;

    sample_avp_dpu_check_exps_return(pic_size == OT_NULL, OT_FAILURE, "pic_size can't be null!\n");
    phys_addr = vrb_blk.phys_addr;
    sample_avp_dpu_check_exps_return(phys_addr == 0,
        OT_FAILURE, "vrb_blk phys_addr null failed!\n");

    size = stride * pic_size->height;
    ret = ot_smr_mmap(phys_addr, size, OT_FALSE, &virt_addr);
    sample_avp_dpu_check_exps_return(virt_addr == OT_NULL,
        OT_FAILURE, "ot_smr_mmap failed!\n");

    ret = ot_buffer_pool_blk_2_pool_hdl(&frame_info->pool_hdl, &vrb_blk);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS,
        fail, "ot_buffer_pool_blk_2_pool_hdl failed!\n");

    frame_info->buff.phys_addr[0] = phys_addr;
    frame_info->buff.virt_addr[0] = virt_addr;
    frame_info->attr.width  = pic_size->width;
    frame_info->attr.height = pic_size->height;
    frame_info->buff.stride[0] = stride;
    frame_info->attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE;
    frame_info->attr.layout = OT_EIS_IMAGE_LAYOUT_LINEAR;
    //frame_info->video_frame.dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    frame_info->attr.pixel_fmt = OT_EIS_IMAGE_FORMAT_YUV_400;
    //frame_info->video_frame.field = OT_VIDEO_FIELD_FRAME;

    ret = sample_avp_dpu_rect_read_one_frame(fp, (ot_u8*)frame_info->buff.virt_addr[0],
        frame_info->attr.width, frame_info->attr.height, frame_info->buff.stride[0]);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail,
        "sample_avp_dpu_rect_read_one_frame failed!\n");

    (ot_void)ot_smr_munmap(virt_addr, size);
    frame_info->buff.virt_addr[0] = OT_NULL;
    virt_addr = OT_NULL;

    return OT_SUCCESS;
fail:
    (ot_void)ot_smr_munmap(virt_addr, size);
    return OT_FAILURE;
}

/* function : write  image to binary file */
static ot_s32 sample_avp_dpu_write_to_bin_file(FILE *fp, ot_u8 *src,
    const ot_eis_img_frame *frame_info, ot_u32 ele_size)
{
    ot_u32 i;
    ot_s32 ret;

    for (i = 0; i < frame_info->attr.height; i++) {
        ret = fwrite(src, frame_info->attr.width * ele_size, 1, fp);
        sample_avp_dpu_check_exps_return(ret != 1, OT_FAILURE, "fwrite file failed!\n");
        src += frame_info->buff.stride[0];
    }

    (void)fflush(fp);
    return  OT_SUCCESS;
}

/* function : write frame to file. */
ot_s32 sample_common_avp_dpu_write_frame_to_file(FILE *fp, ot_u32 ele_size,
    ot_eis_img_frame *frame_info)
{
    ot_u32 stride;
    ot_u32 height;
    ot_void *virt_addr = OT_NULL;
    ot_u32 size;
    ot_s32 ret;

    height = frame_info->attr.height;
    stride = frame_info->buff.stride[0];
    size = stride * height;

    ret = ot_smr_mmap(frame_info->buff.phys_addr[0], size, OT_FALSE, &virt_addr);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS,
        OT_FAILURE, "ot_smr_mmap failed!\n");

    ret = sample_avp_dpu_write_to_bin_file(fp, (ot_u8*)virt_addr, frame_info, ele_size);
    sample_avp_dpu_check_exps_goto(ret != OT_SUCCESS, fail, "write_to_bin_filp failed!\n");
fail:
    (ot_void)ot_smr_munmap(virt_addr, size);
    virt_addr = OT_NULL;

    return ret;
}

/* function : create dpu match memory info */
ot_s32 sample_common_avp_dpu_match_create_mem_info(ot_avp_dpu_match_mem_info *mem_info,
    ot_char *mmb, ot_char *zone, ot_u32 size)
{
    ot_s32 ret;
    ot_smr_alloc_attr alloc_attr = {0};

    sample_avp_dpu_check_exps_return(mem_info == OT_NULL,
        OT_FAILURE, "mem_info can't be NULL\n");
    ret = memset_s(mem_info, sizeof(ot_avp_dpu_match_mem_info), 0, sizeof(ot_avp_dpu_match_mem_info));
    sample_avp_dpu_check_exps_return(ret != EOK,
        OT_FAILURE, "Err, memset_s mem_info failed!\n");

    strcpy(alloc_attr.region_name, "anony");
    strcpy(alloc_attr.chunk_name, "sample_dpu_match");
    alloc_attr.len = size;
    alloc_attr.cached = OT_FALSE;

    mem_info->size = size;
    ret = ot_smr_alloc(&alloc_attr, (ot_phys_addr*)&mem_info->phys_addr,
        (ot_void **)&mem_info->virt_addr);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS,
        OT_FAILURE, "Err(%#x), ot_smr_alloc failed\n", ret);

    return OT_SUCCESS;
}

/* function : start dpu match grp. */
ot_s32 sample_common_avp_dpu_match_start(ot_eis_handle* dpu_match_grp, ot_eis_handle match_chn_hdl[OT_AVP_DPU_MATCH_MAX_CHN_NUM],
    ot_avp_dpu_match_grp_attr *grp_attr, ot_avp_dpu_match_chn_attr *chn_attr)
{
    ot_eis_handle chn_hdl;
    ot_s32 ret;
    ot_s32 j;

    sample_avp_dpu_check_exps_return((grp_attr == OT_NULL) || (chn_attr == OT_NULL),
        OT_FAILURE, "grp_attr or chn_attr can't be NULL\n");

    ret = ot_avp_dpu_match_create_grp(dpu_match_grp, grp_attr);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
        "Err(%#x), ot_avp_dpu_match_create_grp failed\n", ret);

    for (j = 0; j < OT_AVP_DPU_MATCH_MAX_CHN_NUM; j++) {
        ret = ot_avp_dpu_match_create_chn(*dpu_match_grp, &chn_hdl);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
            "Err(%#x), ot_avp_dpu_match_create_chn failed\n", ret);

        match_chn_hdl[j] = chn_hdl;
        
        ret = ot_avp_dpu_match_set_chn_attr(chn_hdl, chn_attr);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
            "Err(%#x), ot_avp_dpu_match_set_chn_attr failed\n", ret);

        ret = ot_avp_dpu_match_enable_chn(chn_hdl);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
            "Err(%#x), ot_avp_dpu_match_enable_chn failed\n", ret);
    }

    ret = ot_avp_dpu_match_start_grp(*dpu_match_grp);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
        "Err(%#x), ot_avp_dpu_match_start_grp failed\n", ret);

    return OT_SUCCESS;
}

/* function : stop dpu match grp */
ot_s32 sample_common_avp_dpu_match_stop(ot_eis_handle dpu_match_grp, ot_eis_handle match_chn_hdl[OT_AVP_DPU_MATCH_MAX_CHN_NUM])
{
    ot_s32 j;
    ot_s32 ret;

    ret = ot_avp_dpu_match_stop_grp(dpu_match_grp);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
        "Err(%#x), ot_avp_dpu_match_stop_grp failed\n", ret);

    for (j = 0; j < OT_AVP_DPU_MATCH_MAX_CHN_NUM; j++) {
        ret = ot_avp_dpu_match_disable_chn(match_chn_hdl[j]);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
            "Err(%#x), ot_avp_dpu_match_disable_chn failed\n", ret);

       ret = ot_avp_dpu_match_destroy_chn(match_chn_hdl[j]);
        sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
            "Err(%#x), ot_avp_dpu_match_destroy_chn failed\n", ret);
    }

    ret = ot_avp_dpu_match_destroy_grp(dpu_match_grp);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, ret,
        "Err(%#x), ot_avp_dpu_match_destroy_grp failed\n", ret);

    return OT_SUCCESS;
}

ot_s32 sample_common_avp_dpu_rect_bind_match(ot_eis_handle pip_hdl, ot_eis_handle rect_chn_handle,
    ot_eis_handle match_chn_handle)
{
    ot_s32 ret;
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_DPU_RECT;
    src_port.handle = rect_chn_handle;

    sink_port.node_type = OT_EIS_MODULE_DPU_MATCH;
    sink_port.handle = match_chn_handle;

    ret = ot_media_pipe_attach(pip_hdl, &src_port, &sink_port);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, OT_FAILURE,
        "Err(%#x), ot_media_pipe_attach(RECT-MATCH) failed!\n", ret);

    return OT_SUCCESS;
}

ot_s32 sample_common_avp_dpu_rect_unbind_match(ot_eis_handle pip_hdl, ot_eis_handle rect_chn_handle,
    ot_eis_handle match_chn_handle)
{
    ot_s32 ret;
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_DPU_RECT;
    src_port.handle = rect_chn_handle;

    sink_port.node_type = OT_EIS_MODULE_DPU_MATCH;
    sink_port.handle = match_chn_handle;

    ret = ot_media_pipe_detach(pip_hdl, &src_port, &sink_port);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, OT_FAILURE,
        "Err(%#x), ot_media_pipe_detach(RECT-MATCH) failed!\n", ret);

    return OT_SUCCESS;
}

ot_s32 sample_common_avp_dpu_vproc_bind_rect(ot_eis_handle pip_hdl, ot_eis_handle vproc_chn_handle,
    ot_eis_handle rect_chn_handle)
{
    ot_s32 ret;
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VPROC;
    src_port.handle = vproc_chn_handle;

    sink_port.node_type = OT_EIS_MODULE_DPU_RECT;
    sink_port.handle = rect_chn_handle;

    ret = ot_media_pipe_attach(pip_hdl, &src_port, &sink_port);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, OT_FAILURE,
        "Err(%#x), ot_media_pipe_attach(VPROC-RECT) failed!\n", ret);

    return OT_SUCCESS;
}

ot_s32 sample_common_avp_dpu_vproc_unbind_rect(ot_eis_handle pip_hdl, ot_eis_handle vproc_chn_handle,
    ot_eis_handle rect_chn_handle)
{
    ot_s32 ret;
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VPROC;
    src_port.handle = vproc_chn_handle;

    sink_port.node_type = OT_EIS_MODULE_DPU_RECT;
    sink_port.handle = rect_chn_handle;

    ret = ot_media_pipe_detach(pip_hdl, &src_port, &sink_port);
    sample_avp_dpu_check_exps_return(ret != OT_SUCCESS, OT_FAILURE,
        "Err(%#x), ot_media_pipe_detach(VPROC-RECT) failed!\n", ret);

    return OT_SUCCESS;
}

ot_s32 sample_common_avp_dpu_get_fg_aggregate_coef_and_unique_ratio(
    sample_avp_dpu_match_density_accuracy_mode density_accuracy_mode, ot_u8 *aggregate_coef, ot_u8 *unique_ratio)
{
    sample_avp_dpu_check_exps_return((aggregate_coef == OT_NULL) || (unique_ratio == OT_NULL),
        OT_FAILURE, "aggregate_coef or unique_ratio can't be NULL\n");

    switch (density_accuracy_mode) {
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D0_A11: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D0_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A0_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D1_A10: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D1_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A0_UNIQ_RATIO;
             break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D2_A9: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D0_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A1_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D3_A8: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D1_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A1_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D4_A7: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D0_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A2_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D5_A6: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D2_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A0_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D6_A5: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D1_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A2_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D7_A4: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D2_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A1_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D8_A3: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D2_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A2_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D9_A2: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D3_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A0_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D10_A1: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D3_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A1_UNIQ_RATIO;
            break;
        }
        case SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D11_A0: {
            *aggregate_coef = SAMPLE_AVP_DPU_MATCH_D3_AGGREGATE_COEFF;
            *unique_ratio = SAMPLE_AVP_DPU_MATCH_A2_UNIQ_RATIO;
            break;
        }
        default: {
            sample_avp_dpu_trace_err("Err, density_accuracy_mode(%d) must be [%d, %d)\n", density_accuracy_mode,
                SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_D0_A11, SAMPLE_AVP_DPU_MATCH_DENSITY_ACCURACY_MODE_BUTT);
            return OT_FAILURE;
        }
    }
    return OT_SUCCESS;
}
