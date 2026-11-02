#include "ot_avp_cve.h"
#include "ot_smr.h"
#include "ot_eis_media_sys.h"
#include "sample_ai.h"
#include "ot_avp_npu_rts.h"
#include "sample_comm.h"
#include "securec.h"
#include "ot_buffer_pool.h"
#include "ot_eis_vi_api.h"
#include "ot_avp_npu_rts.h"
#include <math.h>
#include "ot_avp_cve_alg_lib.h"
#include <sys/time.h>

static sample_npu_attr g_npu_attr = {0};
static sample_cve_attr g_cve_attr = {0};

#if 0
static char *g_class_name[] = {
    "person", "bicycle", "car", "motorbike", "aeroplane",
    "bus", "train", "truck", "boat", "traffic light",
    "fire hydrant", "stop sign", "parking meter", "bench",
    "bird", "cat", "dog", "horse", "sheep", "cow", "elephant",
    "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag",
    "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball",
    "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon",
    "bowl", "banana", "apple", "sandwich", "orange", "broccoli", "carrot", "hot dog",
    "pizza", "donut", "cake", "chair", "sofa", "potted plant", "bed", "dining table",
    "toilet", "TV monitor", "laptop", "mouse", "remote", "keyboard", "cell phone",
    "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase",
    "scissors", "teddy bear", "hair drier", "toothbrush"
};
#endif

sample_npu_attr* get_sample_npu_attr()
{
    return &g_npu_attr;
}

sample_cve_attr* get_sample_cve_attr()
{
    return &g_cve_attr;
}

ot_s32 sample_ai_init()
{
    ot_avp_npu_config config = {
        .dump_layer = false,   // dump layer
        .perf_mode = false,  // perf_mode
        .log_level = 1,   // log level
    };
    ot_s32 ret = OT_FAILURE;

    ret = ot_eis_media_init();
    CHECK_RETURN_WITH_LOG(ret != 0, OT_FAILURE, "MEDIA init error.\n");
    ret = ot_avp_npu_init(&config);
    CHECK_GOTO_WITH_LOG(ret != 0, npu_exit, "NPU init error.\n");
    ret = ot_avp_cve_init();
    CHECK_GOTO_WITH_LOG(ret != 0, cve_exit, "CVE init error.\n");
    ret = ot_smr_init();
    CHECK_GOTO_WITH_LOG(ret != 0, smr_exit, "SMR init error.\n");

    printf("sample_ai_init Success.\n");
    return OT_SUCCESS;

smr_exit:
    ot_avp_cve_deinit();
cve_exit:
    ot_avp_npu_deinit();
npu_exit:
    ot_eis_media_deinit();
    return OT_FAILURE;
}

ot_void sample_ai_deinit()
{
    if (ot_smr_deinit() != 0) {
        printf("SMR deinit fail.\n");
    }
    if (ot_avp_cve_deinit() != 0) {
        printf("CVE deinit fail.\n");
    }
    if (ot_avp_npu_deinit() != 0) {
        printf("NPU deinit fail.\n");
    }
    if (ot_eis_media_deinit() != 0) {
        printf("NPU deinit fail.\n");
    }
    printf("sample_ai_deinit Success.\n");
}

ot_u64 sample_ai_get_time_ms()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    ot_u64 time = (tv.tv_sec * (ot_u64)1000) + tv.tv_usec / 1000;

    return time;
}

#define MOBILENETV2_OUT_NUM 1000
ot_u32 sample_npu_mobilenetv2_process_out(ot_u8* buf, ot_ulong buf_size, ot_avp_npu_shape dims, ot_ulong stride)
{
    ot_float* float_data = (ot_float*)buf;
    ot_u32 label = 0;
    ot_float val = 0.0;
    for (ot_s32 idx = 0; idx < MOBILENETV2_OUT_NUM; idx++) {
        if (val < float_data[idx]) {
            val = float_data[idx];
            label = idx;
        }
    }
    return label;
}

static ot_void sample_cve_trans_rect(sample_rect *src, ot_avp_img_rect* dst, ot_eis_vproc_rect* vproc_rect)
{
    dst->x = src->x1;
    dst->y = src->y1;
    dst->width = src->x2 - src->x1;
    dst->width = (dst->width / 2) * 2; // 能被2整除但不超过给定值的数

    dst->height = src->y2 - src->y1;
    dst->height = (dst->height / 2) * 2;

    dst->classId = src->class_id;

    vproc_rect->thick = 4;
    vproc_rect->color = 0xff0000;
    vproc_rect->rect.x = dst->x;
    vproc_rect->rect.y = dst->y;
    vproc_rect->rect.width = dst->width;
    vproc_rect->rect.height = dst->height;
}

// out0: bbox_num. shape[1, 1, 1, 1]
// out1: bbox. shape[1, 1, 6, 1000]  (x_min, y_min, x_max, y_max, score, classid)
ot_void sample_npu_yolov5_data_trans()
{
    sample_npu_model_info *model_info = &g_npu_attr.yolov5_model_info;
    ot_u32 bbox_num = *(float *)model_info->output_data[0];
    ot_u32 topk = SAMPLE_NPU_MAX_BBOX_NUM;
    ot_u8* buf = (ot_u8 *)model_info->output_data[1];

    if (bbox_num > topk) {
        printf("bbox_num[%u] is invalid.\n", bbox_num);
        return;
    }

    for (ot_u32 i = 0; i < bbox_num; i++) {
        ot_float rect[6];
        sample_rect src = {0};
        for (ot_u32 j = 0; j < 6; j++) {
            rect[j] = *(ot_float *)(buf + j * topk * sizeof(ot_float) + i * sizeof(ot_float));
        }

        src.x1 = rect[0];
        src.y1 = rect[1];
        src.x2 = rect[2];
        src.y2 = rect[3];

        src.x1 = MAX(src.x1, 0);
        src.y1 = MAX(src.y1, 0);
        src.x2 = MAX(src.x2, 0);
        src.y2 = MAX(src.y2, 0);
        src.x1 = MIN(src.x1, 640);
        src.y1 = MIN(src.y1, 640);
        src.x2 = MIN(src.x2, 640);
        src.y2 = MIN(src.y2, 640);

        src.conf = rect[4];
        src.class_id = rect[5];
        // printf("total_num[%d/%d], class_id is: %d, conf is: %f, rect[x1,y1,x2,y2] is: [%f, %f, %f, %f]\n",
        //     i, bbox_num, src.class_id, src.conf, src.x1, src.y1, src.x2, src.y2);

        sample_cve_trans_rect(&src, &g_npu_attr.rect[i],  &g_npu_attr.vproc_rect[i]);
    }
    g_npu_attr.bbox_num = bbox_num;
}

static ot_void sample_cve_crop_for_yuv420sp(ot_avp_cve_img* src_img, ot_avp_cve_img* dst_img, ot_avp_img_rect* rect)
{
    // 只针对img yuv420sp
    ot_data_info y_src_data = {0};
    ot_data_info uv_src_data = {0};
    y_src_data.virt_addr = src_img->virt_addr[0] + rect->y * src_img->stride[0] + rect->x;
    y_src_data.phys_addr = src_img->phys_addr[0] + rect->y * src_img->stride[0] + rect->x;
    y_src_data.width = dst_img->width;
    y_src_data.height = dst_img->height;
    y_src_data.stride = src_img->stride[0];
    uv_src_data.virt_addr = src_img->virt_addr[1] + (rect->y / 2) * src_img->stride[1] + rect->x;
    uv_src_data.phys_addr = src_img->phys_addr[1] + (rect->y / 2) * src_img->stride[1] + rect->x;
    uv_src_data.width = dst_img->width;
    uv_src_data.height = dst_img->height / 2;
    uv_src_data.stride = src_img->stride[1];

    ot_data_info y_dst_data = {0};
    ot_data_info uv_dst_data = {0};
    y_dst_data.virt_addr = dst_img->virt_addr[0];
    y_dst_data.phys_addr = dst_img->phys_addr[0];
    y_dst_data.width = dst_img->width;
    y_dst_data.height = dst_img->height;
    y_dst_data.stride = dst_img->stride[0];
    uv_dst_data.virt_addr = dst_img->virt_addr[1];
    uv_dst_data.phys_addr = dst_img->phys_addr[1];
    uv_dst_data.width = dst_img->width;
    uv_dst_data.height = dst_img->height / 2;
    uv_dst_data.stride = dst_img->stride[1];

    ot_eis_handle handle;
    ot_avp_cve_dma_2d_ctrl ctrl = {0};
    ctrl.mode = OT_AVP_CVE_DMA_2D_MODE_DIRECT_CP;

    if (ot_avp_cve_dma_2d(&handle, &y_src_data, &y_dst_data, &ctrl, OT_AVP_CVE_INVOKE_SYNC) != 0) {
        printf("ot_avp_cve_dma_2d y img fail.\n");
    }
    if (ot_avp_cve_dma_2d(&handle, &uv_src_data, &uv_dst_data, &ctrl, OT_AVP_CVE_INVOKE_SYNC) != 0) {
        printf("ot_avp_cve_dma_2d uv img fail.\n");
    }
}

static ot_u32 sample_cve_resize_get_aux_mem_size(ot_image_pixel_format fmt, ot_u32 batch_num)
{
    ot_u32 size =  1;
    switch (fmt) {
        case OT_IMAGE_FORMAT_U8C1:
        case OT_IMAGE_FORMAT_S8C1:
            size = 1;
            break;
        case OT_IMAGE_FORMAT_U16C1:
        case OT_IMAGE_FORMAT_S16C1:
            size = 2 * sizeof(ot_u16);
            break;
        case OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR:
        case OT_IMAGE_FORMAT_YUV_422_SEMIPLANAR:
            size = 2;
            break;
        case OT_IMAGE_FORMAT_U8C3_PLANAR:
            size = 3;
            break;
        default:
            break;
    }
    return size * batch_num * 48;
}

static ot_void sample_cve_batch_resize_for_yuv420sp(ot_avp_cve_img src_img[], ot_avp_cve_img dst_img[], ot_u32 batch_num)
{
    // 只针对img yuv420sp
    ot_eis_handle handle;
    ot_avp_cve_resize_ctrl ctrl = {0};
    ctrl.alg_mode = OT_AVP_CVE_RESIZE_MODE_BILINEAR;
    ctrl.batch_num = batch_num;
    if (ot_avp_cve_create_mem_info(&ctrl.mem, sample_cve_resize_get_aux_mem_size(OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR, batch_num)) != 0) {
        printf("ot_avp_cve_create_mem_info  fail.\n");
    }

    if (ot_avp_cve_resize(&handle, src_img, dst_img, &ctrl, OT_AVP_CVE_INVOKE_SYNC) != 0) {
        printf("ot_avp_cve_resize fail.\n");
    }
    ot_avp_cve_destory_mem_info(&ctrl.mem);
}

static ot_s32 sample_batch_img_create(ot_avp_cve_img img[], ot_image_pixel_format type, ot_u32 width, ot_u32 height, ot_u8 batch_num)
{
    ot_s32 i, j, ret;
    for (i = 0; i < batch_num; i++) {
        ret = ot_avp_cve_create_img(&img[i], type, width, height);
        if (ret != OT_AVP_CVE_SUCCESS) {
            goto exit;
        }
    }
    return OT_AVP_CVE_SUCCESS;

exit:
    for (j = i; j >= 0; j--) {
        ot_avp_cve_destory_img(&img[j]);
    }
    return ret;
}

static ot_void sample_batch_img_destory(ot_avp_cve_img img[], ot_u8 batch_num)
{
    ot_u8 i;
    for (i = 0; i < batch_num; i++) {
        ot_avp_cve_destory_img(&img[i]);
    }
}

static ot_void sample_trans_video_frame_to_img(ot_avp_cve_img* dst, ot_eis_video_frame* frame)
{
    dst->virt_addr[0] = (ot_u64)frame->buff.virt_addr[0];
    dst->virt_addr[1] = (ot_u64)frame->buff.virt_addr[1];
    dst->phys_addr[0] = (ot_u64)frame->buff.phys_addr[0];
    dst->phys_addr[1] = (ot_u64)frame->buff.phys_addr[1];
    dst->stride[0] = frame->buff.stride[0];
    dst->stride[1] = frame->buff.stride[1];
    dst->width = frame->attr.width;
    dst->height = frame->attr.height;
    dst->type = OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR;
}

ot_void sample_cve_crop_and_resize(ot_eis_video_frame* in_frame)
{
    g_cve_attr.img_num = g_npu_attr.bbox_num;
    ot_u32 final_num = MIN(g_cve_attr.img_num, SAMPLE_NPU_MAX_NMS_OUT_NUM);
    ot_avp_cve_img src = {0};
    sample_trans_video_frame_to_img(&src, in_frame);

    // 只处理前16个框。
    for (int i = 0 ; i < final_num; i++) {
        ot_avp_img_rect *rect = &g_npu_attr.rect[i];
        ot_avp_cve_create_img(&g_cve_attr.resize_in[i], OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR, rect->width, rect->height);
        sample_cve_crop_for_yuv420sp(&src, &g_cve_attr.resize_in[i], rect);
    }

    sample_cve_batch_resize_for_yuv420sp(g_cve_attr.resize_in, g_cve_attr.resize_out, final_num);
#if 0
    for (int i = 0 ; i <  g_cve_attr.img_num; i++) {
        static int j = 1;
        if (j > 1) {
            break;
        }
        j++;
        char dst_file[128] = "cve_out_";
        sprintf_s(dst_file, sizeof(dst_file), "cve_out_%dx%d_%d.yuv",
            g_cve_attr.resize_out[i].width, g_cve_attr.resize_out[i].height, i);
        ot_avp_cve_img_write_file(&g_cve_attr.resize_out[i], dst_file);
    }
#endif

    sample_batch_img_destory(g_cve_attr.resize_in, final_num);
}

static ot_s32 sample_npu_load_model()
{
    ot_s32 ret = OT_FAILURE;
    ot_s8 *yolov5_model_path = "./model/yolov5s_silu_no_detect_yuv.ortm";
    ot_s8 *mobilenetv2_model_path = "./model/mobilenetv2_board.ortm";

    ret = ot_avp_npu_load_model(yolov5_model_path, &g_npu_attr.yolov5_model_hdl);
    CHECK_RETURN_WITH_LOG(ret != OT_SUCCESS, ret, "load model[%s] fail.\n", yolov5_model_path);
    ret = ot_avp_npu_load_model(mobilenetv2_model_path, &g_npu_attr.mobilenetv2_model_hdl);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, load_mobilenetv2_fail, "load model[%s] fail.\n", mobilenetv2_model_path);

    return OT_SUCCESS;

load_mobilenetv2_fail:
    ot_avp_npu_unload_model(g_npu_attr.yolov5_model_hdl);
    return OT_FAILURE;
}

static ot_void sample_npu_unload_model()
{
    ot_avp_npu_unload_model(g_npu_attr.mobilenetv2_model_hdl);
    ot_avp_npu_unload_model(g_npu_attr.yolov5_model_hdl);
}

ot_void sample_npu_destory_data(ot_eis_handle model_hdl, sample_npu_model_info* model_info)
{
    ot_avp_npu_destroy_dataset(model_info->input_dataset);
    ot_avp_npu_destroy_dataset(model_info->output_dataset);

    for (ot_s32 i = 0; i < model_info->output_num; i++) {
        ot_avp_npu_free(model_info->output_data[i]);
    }

    for (ot_s32 i = 0; i < model_info->input_num; i++) {
        ot_avp_npu_free(model_info->input_data[i]);
    }
}

ot_void sample_npu_prepare_data(ot_eis_handle model_hdl, sample_npu_model_info* model_info)
{
    ot_s32 ret = OT_FAILURE;

    ot_avp_npu_get_input_num(model_hdl, &model_info->input_num);
    ot_avp_npu_get_output_num(model_hdl, &model_info->output_num);

    ot_avp_npu_get_input_shape_by_index(model_hdl, 0, &model_info->input_shape);
    ot_avp_npu_get_output_shape_by_index(model_hdl, 0, &model_info->output_shape);

    model_info->input_dataset = ot_avp_npu_create_dataset();
    model_info->output_dataset = ot_avp_npu_create_dataset();

    printf("\n%s input_num[%d], output_num[%d].\n", __FUNCTION__, model_info->input_num, model_info->output_num);

    for (ot_s32 i = 0; i < model_info->output_num; i++) {
        ot_avp_npu_shape dims;
        ret = ot_avp_npu_get_output_shape_by_index(model_hdl, i, &dims);
        CHECK_RETURN_WITH_LOG(ret != 0, RETURN_NONE, "get shape failed");
        memcpy(&model_info->output_dim[i], &dims, sizeof(ot_avp_npu_shape));

        ot_ulong stride = ot_avp_npu_get_output_default_stride(model_hdl, i);
        CHECK_RETURN_WITH_LOG(stride == 0, RETURN_NONE, "stride can't be 0");
        model_info->output_stride[i] = stride;

        ot_ulong buffer_size = ot_avp_npu_get_output_size_by_index(model_hdl, i);
        CHECK_RETURN_WITH_LOG(buffer_size == 0, RETURN_NONE, "buffer size can't be 0");
        model_info->output_buffsize[i] = buffer_size;

        printf("output[%d]:\n   buffer_size[%ld], stride[%ld]\n", i, buffer_size, stride);

        ret = ot_avp_npu_malloc(&model_info->output_data[i], buffer_size);
        CHECK_RETURN_WITH_LOG(ret != 0, RETURN_NONE, "malloc failed");

        ot_avp_data_type data_type;
        ot_avp_npu_get_output_dtype_by_index(model_hdl, i, &data_type);
        ot_ulong data_size = ot_avp_npu_data_type_size(data_type) / 8;
        CHECK_RETURN_WITH_LOG(data_size == 0, RETURN_NONE, "buffer size can't be 0");
        model_info->output_data_size[i] = data_size;

        ret = ot_avp_npu_add_buffer(model_info->output_dataset, model_info->output_data[i], 0, buffer_size, stride);
        CHECK_RETURN_WITH_LOG(ret != 0, RETURN_NONE, "add output buffer failed");
    }

    for (ot_s32 i = 0; i < model_info->input_num; i++) {
        ot_avp_npu_shape dims;
        ret = ot_avp_npu_get_input_shape_by_index(model_hdl, i, &dims);
        CHECK_RETURN_WITH_LOG(ret != 0, RETURN_NONE, "get shape failed");
        memcpy(&model_info->input_dim[i], &dims, sizeof(ot_avp_npu_shape));

        ot_ulong stride = ot_avp_npu_get_input_default_stride(model_hdl, i);
        CHECK_RETURN_WITH_LOG(stride == 0, RETURN_NONE, "stride can't be 0");
        model_info->input_stride[i] = stride;

        ot_ulong buffer_size = ot_avp_npu_get_input_size_by_index(model_hdl, i);
        CHECK_RETURN_WITH_LOG(buffer_size == 0, RETURN_NONE, "buffer size can't be 0");
        model_info->input_buffsize[i] = buffer_size;

        ret = ot_avp_npu_malloc(&model_info->input_data[i], buffer_size);
        CHECK_RETURN_WITH_LOG(ret != 0, RETURN_NONE, "malloc failed");

        ot_avp_data_type data_type;
        ot_avp_npu_get_input_dtype_by_index(model_hdl, i, &data_type);
        ot_ulong data_size = ot_avp_npu_data_type_size(data_type) / 8;
        CHECK_RETURN_WITH_LOG(data_size == 0, RETURN_NONE, "buffer size can't be 0");
        model_info->input_data_size[i] = data_size;

        ret = ot_avp_npu_add_buffer(model_info->input_dataset, model_info->input_data[i], 0, buffer_size, stride);
        CHECK_RETURN_WITH_LOG(ret != 0, RETURN_NONE, "add input buffer failed");
    }
}

static ot_s32 sample_fill_osd_frame(ot_eis_img_frame *osd_frm, ot_s8 * file_path)
{
    // 只针对BGRA 8bit的输入 64x32
    ot_s32 ret = OT_FAILURE;
    FILE* fd = fopen(file_path, "rb");
    if (fd == NULL) {
        printf("fopen fail. file_path:%s\n", file_path);
        return ret;
    }

    ot_u8 *tmp = (ot_u8 *)osd_frm->buff.virt_addr[0];
    ot_u32 size = osd_frm->attr.height * osd_frm->buff.stride[0];
    if (fread(tmp, 1, size, fd) != size) {
        printf("Read file fail\n");
        goto EXIT;
    }

EXIT:
    fclose(fd);
    return -1;
}

static ot_s32 sample_ai_alloc_osd_frame(ot_eis_img_frame *osd_frm, ot_u32 w, ot_u32 h, ot_eis_img_pixel_format fmt)
{
    ot_u32 pixel_size = 0;
    ot_u32 stride = 0;
    ot_void *tmp;
    if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_1555 || fmt == OT_EIS_IMAGE_FORMAT_ARGB_4444) {
        pixel_size = 2;
    } else if (fmt == OT_EIS_IMAGE_FORMAT_ARGB_8888) {
        pixel_size = 4;
    } else if (fmt == OT_EIS_IMAGE_FORMAT_RGB_888) {
        pixel_size = 3;
    }
    osd_frm->attr.pixel_fmt = fmt;
    osd_frm->attr.width = w;
    osd_frm->attr.height = h;
    osd_frm->buff.stride[0] = (w * pixel_size + 15) & 0xFFF0;
    stride = osd_frm->buff.stride[0];
    tmp = malloc(h * stride);
    if (tmp == OT_NULL) {
        printf("alloc frame buffer err\n");
        return OT_FAILURE;
    }
    osd_frm->buff.virt_addr[0] = tmp;
    return OT_SUCCESS;
}

static ot_s32 sample_ai_free_osd_frame(ot_eis_img_frame *osd_frm)
{
    free(osd_frm->buff.virt_addr[0]);
    return OT_SUCCESS;
}

ot_s32 sample_ai_create_data()
{
    ot_s32 ret = OT_FAILURE;

    ret = sample_npu_load_model();
    CHECK_RETURN_WITH_LOG(ret != OT_SUCCESS, ret, "load model fail.\n");

    sample_npu_prepare_data(g_npu_attr.yolov5_model_hdl, &g_npu_attr.yolov5_model_info);
    sample_npu_prepare_data(g_npu_attr.mobilenetv2_model_hdl, &g_npu_attr.mobilenetv2_model_info);

    g_npu_attr.rect = (ot_avp_img_rect *)malloc(SAMPLE_NPU_MAX_BBOX_NUM * sizeof(ot_avp_img_rect));
    memset(g_npu_attr.rect, 0, SAMPLE_NPU_MAX_BBOX_NUM * sizeof(ot_avp_img_rect));

    g_npu_attr.vproc_rect = (ot_eis_vproc_rect *)malloc(SAMPLE_NPU_MAX_BBOX_NUM * sizeof(ot_eis_vproc_rect));
    memset(g_npu_attr.vproc_rect, 0, SAMPLE_NPU_MAX_BBOX_NUM * sizeof(ot_eis_vproc_rect));

    g_npu_attr.osd_attr = (ot_eis_osd_attr *)malloc(SAMPLE_NPU_MAX_BBOX_NUM * sizeof(ot_eis_osd_attr));
    memset(g_npu_attr.osd_attr, 0, SAMPLE_NPU_MAX_BBOX_NUM * sizeof(ot_eis_osd_attr));

    g_npu_attr.bbox_num = 0;

    // 创建 resize in img
    g_cve_attr.resize_in = (ot_avp_cve_img *)malloc(sizeof(ot_avp_cve_img) * SAMPLE_NPU_MAX_BBOX_NUM);
    memset(g_cve_attr.resize_in, 0, sizeof(ot_avp_cve_img) * SAMPLE_NPU_MAX_BBOX_NUM);
    // 创建 resize out img
    g_cve_attr.resize_out = (ot_avp_cve_img *)malloc(sizeof(ot_avp_cve_img) * SAMPLE_NPU_MAX_BBOX_NUM);
    memset(g_cve_attr.resize_out, 0, sizeof(ot_avp_cve_img) * SAMPLE_NPU_MAX_BBOX_NUM);

    sample_batch_img_create(g_cve_attr.resize_out, OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR, 224, 224, SAMPLE_NPU_MAX_NMS_OUT_NUM);

    sample_ai_alloc_osd_frame(&g_npu_attr.osd_fram[0], 64, 32, OT_EIS_IMAGE_FORMAT_ARGB_8888); // person lable
    sample_ai_alloc_osd_frame(&g_npu_attr.osd_fram[1], 64, 32, OT_EIS_IMAGE_FORMAT_ARGB_8888); // car label
    sample_ai_alloc_osd_frame(&g_npu_attr.osd_fram[2], 64, 32, OT_EIS_IMAGE_FORMAT_ARGB_8888); // unknown label
    sample_fill_osd_frame(&g_npu_attr.osd_fram[0], "./data/person.argb8888");
    sample_fill_osd_frame(&g_npu_attr.osd_fram[1], "./data/car.argb8888");
    sample_fill_osd_frame(&g_npu_attr.osd_fram[2], "./data/unknown.argb8888");

    return ret;
}

ot_void sample_ai_destory_data()
{
    sample_npu_unload_model();
    sample_npu_destory_data(g_npu_attr.yolov5_model_hdl, &g_npu_attr.yolov5_model_info);
    sample_npu_destory_data(g_npu_attr.mobilenetv2_model_hdl, &g_npu_attr.mobilenetv2_model_info);
    sample_batch_img_destory(g_cve_attr.resize_out, SAMPLE_NPU_MAX_NMS_OUT_NUM);
    for (ot_u32 i = 0; i < 3; i++) {
        sample_ai_free_osd_frame(&g_npu_attr.osd_fram[i]);
    }

    if (g_npu_attr.rect != OT_NULL) {
        free(g_npu_attr.rect);
        g_npu_attr.rect = OT_NULL;
    }
    if (g_npu_attr.vproc_rect != OT_NULL) {
        free(g_npu_attr.vproc_rect);
        g_npu_attr.vproc_rect = OT_NULL;
    }
    if (g_cve_attr.resize_in != OT_NULL) {
        free(g_cve_attr.resize_in);
        g_cve_attr.resize_in = OT_NULL;
    }
    if (g_cve_attr.resize_out != OT_NULL) {
        free(g_cve_attr.resize_out);
        g_cve_attr.resize_out = OT_NULL;
    }
   if (g_npu_attr.osd_attr != OT_NULL) {
        free(g_npu_attr.osd_attr);
        g_npu_attr.osd_attr = OT_NULL;
    }
}

ot_void sample_ai_reset_data()
{
    if (g_npu_attr.rect != OT_NULL) {
        memset(g_npu_attr.rect, 0, SAMPLE_NPU_MAX_BBOX_NUM * sizeof(ot_avp_img_rect));
    }
    if (g_npu_attr.vproc_rect != OT_NULL) {
        memset(g_npu_attr.vproc_rect, 0, SAMPLE_NPU_MAX_BBOX_NUM * sizeof(ot_eis_vproc_rect));
    }
    if (g_cve_attr.resize_in != OT_NULL) {
        memset(g_cve_attr.resize_in, 0, sizeof(ot_avp_cve_img) * SAMPLE_NPU_MAX_BBOX_NUM);
    }

    for (int i = 0; i < g_npu_attr.yolov5_model_info.output_num; i++) {
        if (g_npu_attr.yolov5_model_info.output_data[i] != OT_NULL) {
            memset(g_npu_attr.yolov5_model_info.output_data[i], 0, g_npu_attr.yolov5_model_info.output_buffsize[i]);
        }
    }
    g_npu_attr.bbox_num = 0;
}

ot_void sample_npu_mobilenetv2_execute()
{
    for (ot_u32 i = 0; i < g_cve_attr.img_num; i++) {
        ot_avp_npu_set_buffer(g_npu_attr.mobilenetv2_model_info.input_dataset, 0,
            (ot_u8 *)g_cve_attr.resize_out[i].virt_addr[0], 0,
            g_npu_attr.mobilenetv2_model_info.input_buffsize[0],
            g_npu_attr.mobilenetv2_model_info.input_stride[0]);

        ot_avp_npu_execute(g_npu_attr.mobilenetv2_model_hdl,
                           g_npu_attr.mobilenetv2_model_info.input_dataset,
                           g_npu_attr.mobilenetv2_model_info.output_dataset);

        sample_npu_mobilenetv2_process_out(
            g_npu_attr.mobilenetv2_model_info.output_data[0],
            g_npu_attr.mobilenetv2_model_info.output_buffsize[0],
            g_npu_attr.mobilenetv2_model_info.output_dim[0],
            g_npu_attr.mobilenetv2_model_info.output_stride[0]
            );
    }
}
