/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include "ot_scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
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

#include "scene_media.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define I2C_DST_ADDR(src_addr, idx)     \
    ((src_addr) < 0xF0) ? (src_addr + 2 * (idx + 1)) : (src_addr - 2 * (idx + 1))

static ot_s32 sample_comm_set_file_name(ot_eis_venc_chn_handle venc_chn, ot_char* file_postfix, ot_char* real_file_name)
{
    time_t t = time(NULL);
    struct tm tNow;

    ot_char file_name[FILE_NAME_LEN];
    if (snprintf_s(file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1, "./") < 0) {
        return OT_FAILURE;
    }

    if (realpath(file_name, real_file_name) == OT_NULL) {
        sample_print("chn[%d] stream file path error\n", (ot_s32)((intptr_t)venc_chn));
        return OT_FAILURE;
    }

    localtime_r(&t, &tNow);

    if (snprintf_s(real_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "ch%02d_%04d%02d%02d_%02d%02d%02d%s",
        (ot_s32)((intptr_t)venc_chn),
        tNow.tm_year + 1900, tNow.tm_mon + 1, tNow.tm_mday,
        tNow.tm_hour, tNow.tm_min, tNow.tm_sec,
        file_postfix) < 0)
    {
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

static ot_bool venc_is_idr_frame(ot_eis_venc_stream *ot_stream)
{
    ot_bool rc = OT_FALSE;
    if (ot_stream == NULL || ot_stream->stream_lens < 6) {
        return rc;
    }

    ot_u8 *startCode = ot_stream->stream_virt_addr;
    ot_u8 *naluType  = startCode + 4;
    rc = (startCode[0] == 0x00 && startCode[1] == 0x00 && startCode[2] == 0x00 && startCode[3] == 0x01) && (naluType[0] == 0x40 && naluType[1] == 0x01);
    return rc;
}

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

ot_s32 sample_scene_media_set_filename(sample_venc_attr* attr, FILE *save_fp[], ot_u32 pipe_id)
{
    ot_scenecomm_check_pointer_return(attr, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_SCENE_EOUTOFRANGE);
    ot_scenecomm_check_pointer_return(save_fp, OT_SCENE_ENONPTR);

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

    save_fp[pipe_id] = fopen(real_file_name, "wb");
    if (!save_fp[pipe_id]) {
        sample_print("open file[%s] failed!\n", real_file_name);
        return OT_FAILURE;
    }
    fd = fileno(save_fp[pipe_id]);
    fchmod(fd, S_IRUSR | S_IWUSR);

    return OT_SUCCESS;
}


static ot_s32 get_file_size(FILE *fptr)
{
    int fd = fileno(fptr);

    struct stat fileStat;
    if (fstat(fd, &fileStat) == 0) {
        return fileStat.st_size;
    }
    return 0;
}

ot_s32 sample_scene_media_save_stream(sample_venc_attr* attr, FILE *save_fp[], ot_u32 pipe_id, 
                ot_u32 *jpeg_acquire_cnt, ot_u32 max_file_size)
{
    ot_scenecomm_check_pointer_return(attr, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(pipe_id >= OT_SCENE_PIPE_MAX_NUM, OT_SCENE_EOUTOFRANGE);
    ot_scenecomm_check_pointer_return(jpeg_acquire_cnt, OT_SCENE_ENONPTR);
    ot_scenecomm_check_pointer_return(save_fp, OT_SCENE_ENONPTR);

    ot_s32 ret = OT_FALSE;
    ot_eis_venc_chn_status status = { 0 };
    
    ret = ot_eis_venc_chn_query_status(attr->venc_hdl, &status);
    if (ret != OT_SUCCESS) {
        // sample_print("ot_eis_venc_chn_query_status chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return ret;
    }

    if (status.unTaken_streams == 0) {
        usleep(1000);
        return ret;
    }

    ot_eis_venc_stream ot_stream = { 0 };
    
    ret = ot_eis_venc_chn_acquire_stream(attr->venc_hdl, &ot_stream, 300);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_acquire_stream chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return ret;
    }

    *jpeg_acquire_cnt = *jpeg_acquire_cnt + 1;
    if (attr->venc_attr.codecFormat != OT_EIS_VENC_CODEC_JPEG ||
        (attr->venc_attr.codecFormat == OT_EIS_VENC_CODEC_JPEG && *jpeg_acquire_cnt == 10))
    {
        if ((get_file_size(save_fp[pipe_id]) > max_file_size) && venc_is_idr_frame(&ot_stream)) {
            fclose(save_fp[pipe_id]);
            sample_scene_media_set_filename(attr, save_fp, pipe_id);
        }

        fwrite(ot_stream.stream_virt_addr, ot_stream.stream_lens, 1, save_fp[pipe_id]);
        fflush(save_fp[pipe_id]);
    }

    ret = ot_eis_venc_chn_release_stream(attr->venc_hdl, &ot_stream);
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_venc_chn_release_stream chn[%d] failed with %#x!\n", (ot_s32)((intptr_t)attr->venc_hdl), ret);
        return ret;
    }

    if (attr->venc_attr.codecFormat == OT_EIS_VENC_CODEC_JPEG  && *jpeg_acquire_cnt == 10) {
        return 1;//SAMPLE_RETURN_JPEG;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_vproc_user_mode_buf_pool_creat(ot_eis_img_attr *img_attr, ot_eis_handle *pool_hdl, ot_u32 block_size)
{
    ot_s32 ret = OT_FALSE;
    ot_video_buffer_attr pool_attr = { 0 };
    ot_eis_handle tmp;

    img_attr->bit_depth = OT_EIS_PIXEL_BIT_DEPTH_8;    
    pool_attr.cnt = 1;
    pool_attr.buf_blks[0].cnt = 10;
    pool_attr.buf_blks[0].size = block_size;

   ret = ot_buffer_pool_init();
   if (ret != OT_SUCCESS && ret != OT_ERR_BUFPOOL_ALREADY_INIT) {
       scene_loge("buffer_pool init fail! ret[%d]\n", ret);
       return OT_FAILURE;
   }

    ret = ot_buffer_pool_create(&tmp, &pool_attr);
    if (ret != 0) {
        scene_loge("buffer_pool create fail! ret[%d]\n", ret);
        goto pool_create_failed;
    }
    *pool_hdl = tmp;
    // scene_logi("create pool hdl 0x%llx \n", (ot_u64)tmp);
    return OT_SUCCESS;

pool_create_failed:
    ot_buffer_pool_deinit();
    return ret;
}

static ot_s32 sample_vio_sys_init(ot_u32 vrb_buf_blks[], ot_eis_img_size *img_size, ot_eis_vi_vproc_mode_type mode_type)
{
    ot_video_buffer_attr vrb_cfg;
    ot_u32 pipe_num = 3;

    sample_common_get_vrb_cfg(pipe_num, img_size->width, img_size->height, &vrb_cfg);
    for (int i = 0; i < pipe_num; i++)
    {
        vrb_cfg.buf_blks[i].cnt = vrb_buf_blks[i];
    }

    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    if (sample_comm_sys_set_vi_vproc_init_cfg(mode_type) != OT_SUCCESS) {
        goto sys_exit;
    }

    if (ot_eis_media_set_3dnr_pos(OT_EIS_3DNR_POS_VPROC) != OT_SUCCESS) {
        goto sys_exit;
    }

    return OT_SUCCESS;
sys_exit:
    sample_comm_sys_exit();
    return OT_FAILURE;
}


static ot_void sample_vproc_user_mode_buf_pool_destroy(ot_eis_handle pool_hdl)
{
    ot_buffer_pool_destroy(pool_hdl);
    ot_buffer_pool_deinit();
}


static ot_void sample_vi_get_default_sns_info(sample_sns_type sns_type, ot_s32 dev_id, sample_sns_info *sns_info, ot_bool vi_with_sensor)
{
    sns_info->sns_type    = sns_type;
    if (dev_id == 2) {
        sns_info->sns_clk_src = 1;
        sns_info->sns_rst_src = 1;
        sns_info->bus_id      = vi_with_sensor ? MIPI_DEV2_I2C_BUS : -1; 
    } else {
        sns_info->sns_clk_src = 0;
        sns_info->sns_rst_src = 0;
        sns_info->bus_id      = vi_with_sensor ? MIPI_DEV0_I2C_BUS : -1;
    }
    sns_info->sns_clk_rst_en = OT_TRUE;
}

extern ot_void sample_comm_vi_get_mipi_info_by_dev_id(sample_sns_type sns_type, ot_vi_dev_id vi_dev, sample_mipi_info *mipi_info);
extern ot_void sample_comm_vi_get_default_dev_info(sample_sns_type sns_type, ot_eis_vi_dev_attr *dev_info);
extern ot_void sample_comm_vi_get_default_pipe_info(sample_sns_type sns_type, sample_vi_pipe_info *pipe_info, ot_s32 bus_id);
extern ot_void sample_comm_vi_get_default_chn_info(sample_sns_type sns_type, ot_eis_vi_chn_attr *chn_attr);
extern ot_void sample_comm_vi_get_default_serdes_info(sample_sns_type sns_type, ot_s32 dev_id, ot_u8 sns_nums, sample_vi_serdes_info *info);

static ot_void sample_vi_get_default_vi_cfg(sample_sns_type sns_type, sample_vi_cfg *vi_cfg, ot_bool vi_with_sensor, ot_s32 dev_id)
{

    (ot_void)memset_s(vi_cfg, sizeof(sample_vi_cfg), 0, sizeof(sample_vi_cfg));

    /* sensor info */
    sample_vi_get_default_sns_info(sns_type, dev_id, &vi_cfg->sns_info, vi_with_sensor);
    /* mipi info */
    sample_comm_vi_get_mipi_info_by_dev_id(sns_type, dev_id, &vi_cfg->mipi_info);
    /* serdes info */
    sample_comm_vi_get_default_serdes_info(sns_type, dev_id, 1, &vi_cfg->serdes_info);
    vi_cfg->serdes_info.serdes_attr.bus_info.bus_id = vi_cfg->sns_info.bus_id;
    
    /* dev info */
    sample_comm_vi_get_default_dev_info(sns_type, &vi_cfg->dev_info.dev_attr);
    vi_cfg->dev_info.dev_attr.mipirx_dev_no = vi_cfg->mipi_info.mipi_attr.mipi_attr.devno;

    /* pipe info */
    sample_comm_vi_get_default_pipe_info(sns_type, &vi_cfg->pipe_info[0], vi_cfg->sns_info.bus_id);

    vi_cfg->pipe_num = 1;
    vi_cfg->pipe_info[0].chn_info.chn_id = 0;
    sample_comm_vi_get_default_chn_info(sns_type, &vi_cfg->pipe_info[0].chn_info.chn_attr);
}

ot_s32 sample_scene_get_vproc_frame_attr(ot_eis_handle vproc_chn_hdl, ot_eis_img_attr *attr)
{
    ot_scenecomm_check_pointer_return(attr, OT_SCENE_ENONPTR);

    ot_eis_img_frame frame = {0};
    ot_s32 ret = ot_eis_vproc_chn_acquire_frame(vproc_chn_hdl, &frame, 3000);    
    if (ret != OT_SUCCESS) {
        sample_print("acquire frame failed\n");
        return OT_FAILURE;
    }

    memcpy_s(attr, sizeof(ot_eis_img_attr), &frame.attr, sizeof(ot_eis_img_attr));
    
    ot_eis_vproc_chn_release_frame(vproc_chn_hdl, &frame);
    return OT_SUCCESS;
}

static ot_s32 scene_set_venc_attr(scene_mediapipe_param *param, sample_venc_attr *attr, ot_eis_handle chn_hdl)
{
    ot_eis_venc_chn_attr *venc_attr = &attr->venc_attr;
    // codecFormat
    venc_attr->codecFormat = param->venc_codec_format;
    // common_attr
    ot_eis_img_attr frame_attr = {0};
    ot_s32 ret = sample_scene_get_vproc_frame_attr(chn_hdl, &frame_attr);    
    if (ret != OT_SUCCESS) {
        return OT_FAILURE;
    }

    venc_attr->common_attr.out_width = frame_attr.width;
    venc_attr->common_attr.out_height = frame_attr.height;
    venc_attr->common_attr.max_pic_width = venc_attr->common_attr.out_width;
    venc_attr->common_attr.max_pic_height = venc_attr->common_attr.out_height;
    venc_attr->common_attr.stream_buf_size = param->venc_stream_buf_size;
    // video_attr
    venc_attr->video_attr.rc_attr.gop = param->venc_gop;
    venc_attr->video_attr.rc_attr.input_fps = param->vproc_chn_frc_src[param->venc_chn_id];
    venc_attr->video_attr.rc_attr.output_fps = param->vproc_chn_frc_dst[param->venc_chn_id];
    venc_attr->video_attr.rc_attr.rc_mode = param->venc_rc_mode;
    venc_attr->video_attr.rc_attr.cvbr_attr.statis_time = param->venc_statis_time;
    venc_attr->video_attr.rc_attr.cvbr_attr.max_bit_rate = param->venc_max_bit_rate;
    venc_attr->video_attr.rc_attr.cvbr_attr.short_statis_time = param->venc_short_statis_time;
    venc_attr->video_attr.rc_attr.cvbr_attr.long_statis_time = param->venc_long_statis_time;
    venc_attr->video_attr.rc_attr.cvbr_attr.long_max_bit_rate = param->venc_long_max_bit_rate;
    venc_attr->video_attr.rc_attr.cvbr_attr.long_min_bit_rate = param->venc_long_min_bit_rate;
    venc_attr->video_attr.gop_attr.gop_mode = param->venc_gop_mode;
    venc_attr->video_attr.gop_attr.gop_normal.qp_delta_ip = param->venc_qp_delta_ip;
    // venc_hdl
    attr->venc_hdl = (ot_eis_venc_chn_handle)-1;
    return OT_SUCCESS;
}

static ot_s32 scene_sys_init(scene_mediapipe_attr *mediapipe_attr)
{
    ot_s32 ret = OT_FALSE;
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    ot_eis_img_size size = {0};
    ot_eis_img_size max_size = {0};
    ot_scene_mediapipe *scene_mediapipe = &mediapipe_attr->mediapipe_param;   
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;

    for (int i = 0; i < pipe_num; i++) {         
        sample_comm_vi_get_size_by_sns_type(scene_mediapipe->mediapipe_attr[i].mediapipe_param.vi_sns_type, &size);
        max_size.width = size.width > max_size.width ? size.width : max_size.width;
        max_size.height = size.height > max_size.height ? size.height : max_size.height;       
    }

    ret = sample_vio_sys_init(scene_mediapipe->mediapipe_comm.vrb_buf_blks, &max_size, 
                scene_mediapipe->mediapipe_comm.vi_vproc_mode_type);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER); 

    return OT_SUCCESS;
}

static ot_s32 scene_start_vi(scene_mediapipe_attr *mediapipe_attr)
{
    ot_s32 ret = OT_FALSE;
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    ot_bool started[OT_SCENE_PIPE_MAX_NUM] = {0};
    ot_scene_mediapipe *scene_mediapipe = &mediapipe_attr->mediapipe_param;    
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;

    for (int i = 0; i < pipe_num; i++) {        
        sample_vi_get_default_vi_cfg(scene_mediapipe->mediapipe_attr[i].mediapipe_param.vi_sns_type, &mediapipe_attr->vi_cfg[i], 
                    scene_mediapipe->mediapipe_attr[i].mediapipe_param.vi_with_sensor, 
                    scene_mediapipe->mediapipe_attr[i].mediapipe_param.vi_dev_id);

        //specify pipe_id, sample_vi,for 2093+290
            for (int j = 0; j < 2; j++) {
                mediapipe_attr->vi_cfg[i].pipe_info[0].pipe_id[j] = 2 * i + j;
            }

        // set compre mode of vi pipe, chn and 3dnr 
        mediapipe_attr->vi_cfg[i].pipe_info[0].pipe_attr.compress_mode = scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_pipe;
        mediapipe_attr->vi_cfg[i].pipe_info[0].chn_info.chn_attr.compress_mode = scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_chn;
        mediapipe_attr->vi_cfg[i].pipe_info[0].nr_attr.compress_mode = scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_3dnr;

        ret = sample_comm_vi_start_vi(&mediapipe_attr->vi_cfg[i]);
        ot_scenecomm_check_goto(ret, err_exit);
        started[i] = OT_TRUE;
    }

    return OT_SUCCESS;
err_exit:
    for (int i = 0; i < pipe_num; i++) {  
        if (started[i]) {
            sample_comm_vi_stop_vi(&mediapipe_attr->vi_cfg[i]);
        }
    }
    return OT_SCENE_EINTER;
}

static ot_s32 scene_get_max_block_size(ot_eis_img_attr *img_attr, ot_u32 width, ot_u32 height, ot_u32* max_size)
{
    ot_scenecomm_check_pointer_return(img_attr, OT_SCENE_ENONPTR);
    ot_eis_img_attr attr = {0};
    ot_u32 block_size1, block_size2;    

    memcpy_s(&attr, sizeof(ot_eis_img_attr), img_attr, sizeof(ot_eis_img_attr));
    attr.width = width;
    attr.height = height;
    sample_common_get_buffer_pool_cfg(&attr, &block_size1);

    attr.width = height;
    attr.height = width;
    sample_common_get_buffer_pool_cfg(&attr, &block_size2);
    *max_size = block_size1 > block_size2 ? block_size1 : block_size2;
    
    return OT_SUCCESS;
}

static ot_s32 scene_start_vproc(scene_mediapipe_attr *mediapipe_attr)
{
    ot_s32 ret = OT_FALSE;
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    ot_scene_mediapipe *scene_mediapipe = &mediapipe_attr->mediapipe_param;    
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;
    ot_bool started[OT_SCENE_PIPE_MAX_NUM] = {0};
    ot_bool pool_created[OT_SCENE_PIPE_MAX_NUM] = {0};
    ot_bool pipe_sw[OT_EIS_VPROC_GRP_PIPE_MAX_NUM] = {OT_TRUE, OT_FALSE, OT_FALSE, OT_FALSE};
    ot_bool chn_sw[OT_EIS_VPROC_GRP_CHN_MAX_NUM] = {OT_FALSE, OT_FALSE, OT_FALSE, OT_FALSE};

    for (int i = 0; i < pipe_num; i++) {
        memset_s(chn_sw, sizeof(ot_bool) * OT_EIS_VPROC_GRP_CHN_MAX_NUM, 0, sizeof(ot_bool) * OT_EIS_VPROC_GRP_CHN_MAX_NUM);

        sample_media_vproc_get_default_attr_by_snsor(scene_mediapipe->mediapipe_attr[i].mediapipe_param.vi_sns_type, &mediapipe_attr->vproc_attr[i]);
        memset(&mediapipe_attr->vproc_attr[i].set_attr, 0, sizeof(mediapipe_attr->vproc_attr[i].set_attr));
        
        for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {                       
            chn_sw[j] = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_enable[j];
            if (chn_sw[j] == OT_FALSE) {
                continue;
            }
            // set compre mode of vproc pipe, chn and 3dnr 
            mediapipe_attr->vproc_attr[i].pipe_attr[j].image_attr.compress_mode = scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_pipe[j];
            mediapipe_attr->vproc_attr[i].chn_attr[j].image_attr.compress_mode = scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_chn[j];
            mediapipe_attr->vproc_attr[i].set_attr.nr_param.compress_mode = scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_3dnr;            

            mediapipe_attr->vproc_attr[i].chn_attr[j].image_attr.width   = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_img_w[j];
            mediapipe_attr->vproc_attr[i].chn_attr[j].image_attr.height  = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_img_h[j];
            mediapipe_attr->vproc_attr[i].chn_attr[j].frc.src_frame_rate = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_frc_src[j];
            mediapipe_attr->vproc_attr[i].chn_attr[j].frc.dst_frame_rate = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_frc_dst[j];
            
            //vproc chn default type: pipe
            mediapipe_attr->vproc_attr[i].chn_attr[j].mode = OT_EIS_VPROC_WORK_MODE_PIPE;
            if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_frame_pool_enable[j] == OT_TRUE) {
                        
                mediapipe_attr->vproc_attr[i].chn_attr[j].mode = OT_EIS_VPROC_WORK_MODE_USER;
                mediapipe_attr->vproc_attr[i].chn_attr[j].frame_queue_depth = 4;

                // get max block size
                ot_u32 block_size = 0;
                scene_get_max_block_size(&mediapipe_attr->vproc_attr[i].chn_attr[j].image_attr, 
                    scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_img_w[j],
                    scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_img_h[j],
                    &block_size);

                ret = sample_vproc_user_mode_buf_pool_creat(&mediapipe_attr->vproc_attr[i].chn_attr[j].image_attr, 
                            &mediapipe_attr->pool_hdl[i], block_size);
                ot_scenecomm_check_goto(ret, err_exit);
                mediapipe_attr->vproc_attr[i].chn_attr[j].pool_handle = mediapipe_attr->pool_hdl[i];
                pool_created[i] = OT_TRUE;
            }            
        } 

        ret = sample_comm_start_vproc(&mediapipe_attr->vproc_attr[i], pipe_sw, chn_sw);
        ot_scenecomm_check_goto(ret, err_exit);
        started[i] = OT_TRUE;

        // reset vproc    
        for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
            if (chn_sw[j] == OT_FALSE) {
                continue;
            }

            // mirror
            ot_eis_vproc_mirror_param mirror_param = {0};
            mirror_param.enable = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_mirror_enable[j];
            ret = ot_eis_vproc_chn_set_mirror(mediapipe_attr->vproc_attr[i].chn_hdl[j], &mirror_param);
            ot_scenecomm_check_goto(ret, err_exit);

            // crop
            ot_eis_vproc_crop_param crop_param = {0};
            crop_param.enable = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_crop_enable[j];
            crop_param.crop_type = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_crop_mode[j];
            crop_param.crop_rect.x = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_crop_x[j];
            crop_param.crop_rect.y = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_crop_y[j];
            crop_param.crop_rect.width = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_crop_w[j];
            crop_param.crop_rect.height = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_crop_h[j];
            ret = ot_eis_vproc_chn_set_crop(mediapipe_attr->vproc_attr[i].chn_hdl[j], &crop_param);
            ot_scenecomm_check_goto(ret, err_exit);

            // rotation
            ot_eis_vproc_chn_rotation rot_param = {0};
            rot_param.enable = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_rotation_enable[j];
            rot_param.angle = scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_rotation[j];
            ret = ot_eis_vproc_chn_set_rotation(mediapipe_attr->vproc_attr[i].chn_hdl[j], &rot_param);
            ot_scenecomm_check_goto(ret, err_exit);
        }
    }   

    return OT_SUCCESS;
err_exit:
    for (int i = 0; i < pipe_num; i++) {
        if (pool_created[i]) {
            sample_vproc_user_mode_buf_pool_destroy(mediapipe_attr->pool_hdl[i]);
        }

        if (started[i]) {
            sample_comm_stop_vproc(&mediapipe_attr->vproc_attr[i]);
        }        
    }    
    return OT_SCENE_EINTER;
}

static ot_s32 scene_start_vo(scene_mediapipe_attr *mediapipe_attr)
{
    ot_s32 ret = OT_FALSE;
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    ot_scene_mediapipe *scene_mediapipe = &mediapipe_attr->mediapipe_param;    
    mediapipe_attr->vo_attr = sample_comm_get_vo_attr(scene_mediapipe->mediapipe_comm.vo_display_dev);
    ot_scenecomm_expr_true_return(mediapipe_attr->vo_attr == OT_NULL, OT_SCENE_EINTER);

    sample_comm_vo_set_fps(scene_mediapipe->mediapipe_comm.vo_fps);

    //enable light_sensor,for test
    mediapipe_attr->vo_attr->light_sensor_attr.is_used = OT_TRUE;
    ret = sample_comm_start_vo(mediapipe_attr->vo_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER); 
    
    return OT_SUCCESS;
}

static ot_s32 scene_bind_media_pipe(scene_mediapipe_attr *mediapipe_attr)
{
    ot_s32 ret = OT_FALSE;
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR); 
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;
    ot_bool vi_vproc_bind[OT_SCENE_PIPE_MAX_NUM] = {0};
    ot_bool vproc_vo_bind[OT_SCENE_PIPE_MAX_NUM] = {0};
    ret = sample_comm_media_pipe_init(&mediapipe_attr->media_pipe_hdl);
    ot_scenecomm_check_goto(ret, err_exit_media_pipe);

    for (int i = 0; i < pipe_num; i++) {
        if (mediapipe_attr->vi_cfg[i].pipe_info[0].chn_info.chn_attr.work_mode == OT_EIS_VI_CHN_WORK_MODE_PIPE) {
            ret = sample_comm_vi_bind_vproc(mediapipe_attr->vi_cfg[i].pipe_info[0].chn_info.chn_hdl, 
                                mediapipe_attr->vproc_attr[i].pipe_hdl[0], mediapipe_attr->media_pipe_hdl);
            ot_scenecomm_check_goto(ret, err_exit);
            vi_vproc_bind[i] = OT_TRUE;
        }

        if (mediapipe_attr->vproc_attr[i].chn_attr[0].mode == OT_EIS_VPROC_WORK_MODE_PIPE) {
            mediapipe_attr->vi_cfg[i].media_pipe_hdl = mediapipe_attr->media_pipe_hdl;
            ret = sample_comm_vproc_bind_vo(mediapipe_attr->vproc_attr[i].chn_hdl[0], 
                        mediapipe_attr->vo_attr->in_port_handle[i], mediapipe_attr->media_pipe_hdl);
            if (ret != OT_SUCCESS) {
                continue;
            }
            vproc_vo_bind[i] = OT_TRUE;            
        }
    }

    return OT_SUCCESS;

err_exit:

    for (int i = 0; i < pipe_num; i++) {
        if (vi_vproc_bind[i]) {
            sample_comm_vi_un_bind_vproc(mediapipe_attr->vi_cfg[i].pipe_info[0].chn_info.chn_hdl, 
                            mediapipe_attr->vproc_attr[i].pipe_hdl[0], mediapipe_attr->media_pipe_hdl);
        }

        if (vproc_vo_bind[i]) {
            sample_comm_vproc_un_bind_vo(mediapipe_attr->vproc_attr[i].chn_hdl[0], 
                        mediapipe_attr->vo_attr->in_port_handle[i], mediapipe_attr->media_pipe_hdl);
        }        
    }    
    
    sample_comm_media_pipe_stop(mediapipe_attr->media_pipe_hdl);    
err_exit_media_pipe:    
    return OT_SCENE_EINTER;    
}

static ot_s32 scene_set_lowdelay(scene_mediapipe_attr *mediapipe_attr)
{
    ot_s32 ret = OT_FALSE;
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;
    ot_u8 mode_type = mediapipe_attr->mediapipe_param.mediapipe_comm.vi_vproc_mode_type;
    ot_eis_low_delay_info lowdelay = {0};
    lowdelay.enable = OT_TRUE;
    lowdelay.line_cnt= 200;
    lowdelay.one_buf_en = OT_FALSE;

    for (int i = 0; i < pipe_num; i++) {
        ot_eis_vi_public_attr pub_attr = {0};
        ret = ot_eis_vi_public_get_attr(mediapipe_attr->vi_cfg[i].pipe_info[0].pipe_hdl, &pub_attr);
        if (ret != OT_SUCCESS) {
            continue;
        }

        if (!(pub_attr.wdr_mode > OT_EIS_WDR_MODE_RESERVED && pub_attr.wdr_mode < OT_EIS_WDR_MODE_BUTT) && 
            (mode_type == OT_EIS_VI_OFFLINE_VPROC_OFFLINE || mode_type == OT_EIS_VI_OFFLINE_VPROC_ONLINE)) {
            // not wdr mode
            ret = ot_eis_vi_pipe_set_low_delay_attr(mediapipe_attr->vi_cfg[i].pipe_info[0].pipe_hdl, &lowdelay);
            if (ret != OT_SUCCESS) {
                sample_print("enable vi pipe low delay failed!\n");
            }
        }

        if ((mode_type == OT_EIS_VI_OFFLINE_VPROC_OFFLINE || mode_type == OT_EIS_VI_OFFLINE_VPROC_ONLINE)) {
            ret = ot_eis_vi_chn_set_low_delay_attr(mediapipe_attr->vi_cfg[i].pipe_info[0].chn_info.chn_hdl, &lowdelay);
            if (ret != OT_SUCCESS) {
                sample_print("enable vi chn low delay failed!\n");
            }
        }
        
        for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
            if (mediapipe_attr->mediapipe_param.mediapipe_attr[i].mediapipe_param.vproc_chn_enable[j] && 
                (mediapipe_attr->mediapipe_param.mediapipe_attr[i].vp2vo_param.vp2vo_chn_id[j] > -1 || 
                (mediapipe_attr->vproc_attr[i].chn_attr[j].mode == OT_EIS_VPROC_WORK_MODE_PIPE && 
                (mode_type == OT_EIS_VI_OFFLINE_VPROC_OFFLINE || mode_type == OT_EIS_VI_ONLINE_VPROC_OFFLINE)))) {
                ret = ot_eis_vproc_chn_set_low_delay(mediapipe_attr->vproc_attr[i].chn_hdl[j], &lowdelay);
                if (ret != OT_SUCCESS) {
                    sample_print("enable vproc chn low delay failed!\n");
                }
            }
        }        
    }
    return OT_SUCCESS;
}

static ot_s32 scene_start_venc(scene_mediapipe_attr *mediapipe_attr)
{
    ot_s32 ret = OT_FALSE;
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    ot_scene_mediapipe *scene_mediapipe = &mediapipe_attr->mediapipe_param;   
    ot_bool started[OT_SCENE_PIPE_MAX_NUM] = {0};
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;
    for (int i = 0; i < pipe_num; i++) {       
        if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.save_video_enable) {
            // start venc
            ret = scene_set_venc_attr(&scene_mediapipe->mediapipe_attr[i].mediapipe_param, &mediapipe_attr->venc_attr[i], 
                    mediapipe_attr->vproc_attr[i].chn_hdl[scene_mediapipe->mediapipe_attr[i].mediapipe_param.venc_chn_id]);
            ot_scenecomm_check_goto(ret, err_exit);

            ret = sample_comm_venc_start(&mediapipe_attr->venc_attr[i]);
            ot_scenecomm_check_goto(ret, err_exit);
            started[i] = OT_TRUE;    
        }
    }
    return OT_SUCCESS;
err_exit:
    for (int i = 0; i < pipe_num; i++) {
        if (started[i]) {
            sample_comm_venc_stop(&mediapipe_attr->venc_attr[i]);
        }    
    }    
    
    sample_comm_media_pipe_stop(mediapipe_attr->media_pipe_hdl);    
    return OT_SCENE_EINTER;      
}

static void scene_sys_deinit()
{
    sample_comm_sys_exit();
}

static ot_s32 scene_stop_vi(scene_mediapipe_attr *mediapipe_attr)
{
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;
    for (int i = 0; i < pipe_num; i++) {       
        sample_comm_vi_stop_vi(&mediapipe_attr->vi_cfg[i]);
    }
    return OT_SUCCESS;
}

static ot_s32 scene_stop_vproc(scene_mediapipe_attr *mediapipe_attr)
{
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    ot_scene_mediapipe *scene_mediapipe = &mediapipe_attr->mediapipe_param;   
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;
    ot_bool pool_created = OT_FALSE;
    for (int i = 0; i < pipe_num; i++) {
        pool_created = OT_FALSE;
        for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
            if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_enable[j] == OT_TRUE &&
                scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_frame_pool_enable[j] == OT_TRUE) {
                pool_created = OT_TRUE;
                break;
            }
        }
        if (pool_created) {
            sample_vproc_user_mode_buf_pool_destroy(mediapipe_attr->pool_hdl[i]);
        }        
        sample_comm_stop_vproc(&mediapipe_attr->vproc_attr[i]);
    }
    return OT_SUCCESS;
}

static ot_s32 scene_stop_vo(scene_mediapipe_attr *mediapipe_attr)
{
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);    
    sample_comm_stop_vo(mediapipe_attr->vo_attr);
    return OT_SUCCESS;
}

static ot_s32 scene_unbind_media_pipe(scene_mediapipe_attr *mediapipe_attr)
{
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;
    
    for (int i = 0; i < pipe_num; i++) {
        if (mediapipe_attr->mediapipe_param.mediapipe_attr[i].mediapipe_param.vproc_chn_enable[0] == 1 &&
            mediapipe_attr->mediapipe_param.mediapipe_attr[i].mediapipe_param.vproc_chn_frame_pool_enable[0] == 0) {
            sample_comm_vproc_un_bind_vo(mediapipe_attr->vproc_attr[i].chn_hdl[0], 
                        mediapipe_attr->vo_attr->in_port_handle[i], mediapipe_attr->media_pipe_hdl);
        }
        
        sample_comm_vi_un_bind_vproc(mediapipe_attr->vi_cfg[i].pipe_info[0].chn_info.chn_hdl, 
                            mediapipe_attr->vproc_attr[i].pipe_hdl[0], mediapipe_attr->media_pipe_hdl);
    }
    sample_comm_media_pipe_stop(mediapipe_attr->media_pipe_hdl);
    return OT_SUCCESS;
}

static ot_s32 scene_stop_venc(scene_mediapipe_attr *mediapipe_attr)
{
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    ot_scene_mediapipe *scene_mediapipe = &mediapipe_attr->mediapipe_param;   
    ot_u32 pipe_num = mediapipe_attr->mediapipe_param.mediapipe_comm.mediapipe_num;    
    for (int i = 0; i < pipe_num; i++) {
        if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.save_video_enable) {
            sample_comm_venc_stop(&mediapipe_attr->venc_attr[i]);
        }
    }
    return OT_SUCCESS;
}

static ot_s32 scene_check_mediapipe(ot_scene_mediapipe *scene_mediapipe)
{
    ot_scenecomm_check_pointer_return(scene_mediapipe, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(scene_mediapipe->mediapipe_comm.mediapipe_num > OT_SCENE_PIPE_MAX_NUM ||
        scene_mediapipe->mediapipe_comm.mediapipe_num < 1, OT_SCENE_EOUTOFRANGE);
    ot_scenecomm_expr_true_return(scene_mediapipe->mediapipe_comm.vi_vproc_mode_type >= OT_EIS_VI_VPROC_MODE_BUTT, OT_SCENE_EOUTOFRANGE);

    for (int i = 0; i < scene_mediapipe->mediapipe_comm.mediapipe_num; i++) {        
        for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
            ot_scenecomm_expr_true_return(scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_crop_mode[j] >= OT_EIS_COORD_BUTT, OT_SCENE_EOUTOFRANGE);
            ot_scenecomm_expr_true_return(scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_rotation[j] >= OT_EIS_RTT_BUTT, OT_SCENE_EOUTOFRANGE);            
        }

        ot_scenecomm_expr_true_return(scene_mediapipe->mediapipe_attr[i].mediapipe_param.save_video_enable == OT_TRUE &&
                                (scene_mediapipe->mediapipe_attr[i].mediapipe_param.venc_chn_id >= OT_EIS_VPROC_GRP_CHN_MAX_NUM ||
                                scene_mediapipe->mediapipe_attr[i].mediapipe_param.venc_chn_id <= 0) , OT_SCENE_EOUTOFRANGE);  

        if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.vproc_chn_enable[0] == OT_FALSE) {
            scene_loge("vproc chn0 must be enable\n");
            return OT_SCENE_EINVAL;
        }

        if (scene_mediapipe->mediapipe_attr[i].thr_en_param.thr_enable_iqt == OT_TRUE &&
            scene_mediapipe->mediapipe_attr[i].thr_en_param.thr_enable_vp2vo == OT_TRUE
        ) {
            scene_loge("Thread iqt and vp2vo of mediapipe %d cannot be enabled simultaneously\n", i);
            return OT_SCENE_EINVAL;
        } 

        if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_pipe != OT_EIS_IMAGE_COMPRESS_MODE_NONE &&
            scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_pipe != OT_EIS_IMAGE_COMPRESS_MODE_FRAME &&
            scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_pipe != OT_EIS_IMAGE_COMPRESS_MODE_LINE) {
            scene_loge("comp_mode_vi_pipe(%d) should be %d, %d or %d\n", 
                scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_pipe, 
                OT_EIS_IMAGE_COMPRESS_MODE_NONE, OT_EIS_IMAGE_COMPRESS_MODE_FRAME, OT_EIS_IMAGE_COMPRESS_MODE_LINE);
            return OT_SCENE_EINVAL;
        }
        
        if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_chn != OT_EIS_IMAGE_COMPRESS_MODE_NONE &&
            scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_chn != OT_EIS_IMAGE_COMPRESS_MODE_SEG &&
            scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_chn != OT_EIS_IMAGE_COMPRESS_MODE_SEG) {
            scene_loge("comp_mode_vi_chn(%d) should be %d, %d or %d\n", 
                scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vi_chn,
                OT_EIS_IMAGE_COMPRESS_MODE_NONE, OT_EIS_IMAGE_COMPRESS_MODE_SEG, OT_EIS_IMAGE_COMPRESS_MODE_SEG);
            return OT_SCENE_EINVAL;
        }

        if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_3dnr != OT_EIS_IMAGE_COMPRESS_MODE_NONE &&
            scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_3dnr != OT_EIS_IMAGE_COMPRESS_MODE_FRAME) {
            scene_loge("comp_mode_3dnr(%d) should be %d or %d\n", 
                scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_3dnr, 
                OT_EIS_IMAGE_COMPRESS_MODE_NONE, OT_EIS_IMAGE_COMPRESS_MODE_FRAME);
            return OT_SCENE_EINVAL;
        }
        
        for (int j = 0; j < OT_EIS_VPROC_GRP_CHN_MAX_NUM; j++) {
            if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_pipe[j] != OT_EIS_IMAGE_COMPRESS_MODE_NONE &&
                scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_pipe[j] != OT_EIS_IMAGE_COMPRESS_MODE_SEG &&
                scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_pipe[j] != OT_EIS_IMAGE_COMPRESS_MODE_SEG) {
                scene_loge("comp_mode_vproc_pipe[%d](%d) be %d, %d or %d\n", 
                    j, scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_pipe[j], 
                    OT_EIS_IMAGE_COMPRESS_MODE_NONE, OT_EIS_IMAGE_COMPRESS_MODE_SEG, OT_EIS_IMAGE_COMPRESS_MODE_SEG);
                return OT_SCENE_EINVAL;
            }

            if (scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_chn[j] != OT_EIS_IMAGE_COMPRESS_MODE_NONE &&
                scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_chn[j] != OT_EIS_IMAGE_COMPRESS_MODE_SEG &&
                scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_chn[j] != OT_EIS_IMAGE_COMPRESS_MODE_SEG) {
                scene_loge("comp_mode_vproc_chn[%d](%d) be %d, %d or %d\n", 
                    j, scene_mediapipe->mediapipe_attr[i].mediapipe_param.comp_mode_vproc_chn[j], 
                    OT_EIS_IMAGE_COMPRESS_MODE_NONE, OT_EIS_IMAGE_COMPRESS_MODE_SEG, OT_EIS_IMAGE_COMPRESS_MODE_SEG);
                return OT_SCENE_EINVAL;
            }
        }
    }

    return OT_SUCCESS;
}


ot_s32 sample_scene_media_start(scene_mediapipe_attr *mediapipe_attr)
{
    ot_s32 ret = OT_FAILURE;
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    ot_scenecomm_expr_true_return(mediapipe_attr->is_started == OT_TRUE, OT_SCENE_EINITIALIZED);
    
    ret = scene_check_mediapipe(&mediapipe_attr->mediapipe_param);
    ot_scenecomm_check_return(ret, OT_SCENE_EINVAL);

    // sys init
    ret = scene_sys_init(mediapipe_attr);
    ot_scenecomm_check_return(ret, OT_SCENE_EINTER);

    // vi    
    ret = scene_start_vi(mediapipe_attr);
    ot_scenecomm_check_goto(ret, err_exit_vi);
    
    // vproc 
    ret = scene_start_vproc(mediapipe_attr);
    ot_scenecomm_check_goto(ret, err_exit_vproc);
    
    // vo
    ret = scene_start_vo(mediapipe_attr);
    ot_scenecomm_check_goto(ret, err_exit_vo);     

    // bind media pipe
    ret = scene_bind_media_pipe(mediapipe_attr);
    ot_scenecomm_check_goto(ret, err_exit_bind);    

    // set lowdelay
    ret = scene_set_lowdelay(mediapipe_attr);
    ot_scenecomm_check_goto(ret, err_exit_lowdelay);

    // venc
    ret = scene_start_venc(mediapipe_attr);
    ot_scenecomm_check_goto(ret, err_exit_lowdelay);

    mediapipe_attr->is_started = OT_TRUE;
    return OT_SUCCESS;

err_exit_lowdelay:
    scene_unbind_media_pipe(mediapipe_attr);
err_exit_bind:
    scene_stop_vo(mediapipe_attr);
err_exit_vo:
    scene_stop_vproc(mediapipe_attr);
err_exit_vproc:
    scene_stop_vi(mediapipe_attr);
err_exit_vi:
    scene_sys_deinit();
    return OT_FAILURE;
}


ot_s32 sample_scene_media_release(scene_mediapipe_attr *mediapipe_attr)
{
    ot_scenecomm_check_pointer_return(mediapipe_attr, OT_SCENE_ENONPTR);
    if (mediapipe_attr->is_started) {
        scene_stop_venc(mediapipe_attr);
        scene_unbind_media_pipe(mediapipe_attr);
        scene_stop_vo(mediapipe_attr);
        scene_stop_vproc(mediapipe_attr);
        scene_stop_vi(mediapipe_attr);
        scene_sys_deinit();
        scene_logi("sample_comm_sys_exit success\n");
        mediapipe_attr->is_started = OT_FALSE;
    }
    
    return OT_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
