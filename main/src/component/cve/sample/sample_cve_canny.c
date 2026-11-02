#include "sample_cve_main.h"
#include <stdlib.h>
#define OT_SAMPLE_CVE_CANNY_HIGH_THR     150
#define OT_SAMPLE_CVE_CANNY_LOW_THR      50
#define OT_SAMPLE_CVE_THR_U16_HIGH_THR   100
#define OT_SAMPLE_CVE_THR_U16_LOW_THR    100
#define OT_SAMPLE_CVE_THR_U16_MAX_VAL    255
#define OT_SAMPLE_CVE_CANNY_EDGE_NUM     3
#define OT_SAMPLE_CVE_D1_WIDTH           720
#define OT_SAMPLE_CVE_D1_HEIGHT          576
#define OT_AVP_CVE_CANNY_OUT_FILENAME  "./data/test_canny_out.yuv"
#define OT_AVP_CVE_CANNY_IN_FILENAME   "./data/input/source/canny.yuv"
#define OT_SAMPLE_CVE_CANNY_QUERY_TIMEOUT 10000

typedef struct {
    ot_avp_cve_src_img src;
    ot_avp_cve_dst_img mag;
    ot_avp_cve_dst_img ang;
    ot_avp_cve_dst_img edge;
    ot_avp_cve_mag_and_ang_ctrl mag_and_ang_ctrl;
    ot_avp_cve_thresh_ctrl thr_u16_ctrl;
    ot_s8 srcFile[CVE_MAX_FILE_PATH];
    ot_s8 dstFile[CVE_MAX_FILE_PATH];
} ot_sample_cve_canny_info;

static ot_sample_cve_canny_info g_canny_info;
/*
 * function : Canny uninit
 */
static ot_void sample_cve_canny_uninit(ot_sample_cve_canny_info *canny_info)
{
    ot_avp_cve_destory_img(&canny_info->src);
    ot_avp_cve_destory_img(&canny_info->edge);
    ot_avp_cve_destory_img(&canny_info->mag);
    ot_avp_cve_destory_img(&canny_info->ang);
}

static ot_void sample_cve_canny_ctrl_init(ot_sample_cve_canny_info *canny_info)
{
    static ot_s16 mask[OT_AVP_CVE_MASK_NUM] = {
        0,  0, 0, 0, 0,
        0, -1, 0, 1, 0,
        0, -2, 0, 2, 0,
        0, -1, 0, 1, 0,
        0,  0, 0, 0, 0
    };

    canny_info->mag_and_ang_ctrl.kernel_attr.coef = mask;
    canny_info->mag_and_ang_ctrl.kernel_attr.ksize_mode = OT_KERNEL_SIZE_MODE_5x5;
    canny_info->mag_and_ang_ctrl.out_mode = OT_AVP_CVE_OUT_MAG_ONLY;
    canny_info->mag_and_ang_ctrl.thresh = 0;

    canny_info->thr_u16_ctrl.mode = OT_AVP_CVE_THRESH_U16_TERNARY;
    canny_info->thr_u16_ctrl.high_thr = OT_SAMPLE_CVE_THR_U16_HIGH_THR;
    canny_info->thr_u16_ctrl.low_thr = OT_SAMPLE_CVE_THR_U16_LOW_THR;
    canny_info->thr_u16_ctrl.max_val = OT_SAMPLE_CVE_THR_U16_MAX_VAL;
    canny_info->thr_u16_ctrl.mid_val = 0;
    canny_info->thr_u16_ctrl.min_val = 0;
}

/*
 * function : Canny init
 */
static ot_s32 sample_cve_canny_init(ot_sample_cve_canny_info *canny_info, ot_u32 width,
    ot_u32 height)
{
    ot_s32 ret;
    (ot_void)memset(canny_info, 0x00, sizeof(ot_sample_cve_canny_info));
    strcpy(canny_info->dstFile, OT_AVP_CVE_CANNY_OUT_FILENAME);
    strcpy(canny_info->srcFile, OT_AVP_CVE_CANNY_IN_FILENAME);

    sample_cve_canny_ctrl_init(canny_info);

    ret = ot_avp_cve_create_img(&canny_info->src, OT_IMAGE_FORMAT_U8C1, width, height);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, fail,  "Error,create src img failed!\n");

    ret = ot_avp_cve_create_img(&canny_info->mag, OT_IMAGE_FORMAT_U16C1, width, height);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, fail,  "Error,create mag img failed!\n");

    ret = ot_avp_cve_create_img(&canny_info->ang, OT_IMAGE_FORMAT_U8C1, width, height);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, fail,  "Error,create ang img failed!\n");
    canny_info->ang.stride[0] = canny_info->mag.stride[0];
    ret = ot_avp_cve_create_img(&canny_info->edge, OT_IMAGE_FORMAT_U8C1, width, height);
    CHECK_GOTO_WITH_LOG(ret != OT_SUCCESS, fail,  "Error,create edge img failed!\n");
    printf("sample_cve_canny_init  Success!\n");
    return OT_SUCCESS;
fail:
    sample_cve_canny_uninit(canny_info);
    return ret;
}


/*
 * function : show part canny sample
 */
static ot_s32 sample_cve_part_canny(ot_sample_cve_canny_info *canny_info)
{
    ot_s32 ret;
    ot_avp_handle handle;
    ot_avp_cve_invoke_mode mode = OT_AVP_CVE_INVOKE_SYNC;

    ret = ot_avp_cve_img_read_file(&canny_info->src, canny_info->srcFile);
    CHECK_RETURN_WITH_LOG(ret != OT_SUCCESS,OT_AVP_CVE_ERR_FAIL,
        "Error(%#x),Read src file failed!\n", ret);

    ret = ot_avp_cve_mag_and_ang(&handle, &canny_info->src, &canny_info->mag, OT_NULL,
        &canny_info->mag_and_ang_ctrl, mode);
    CHECK_RETURN_WITH_LOG(ret != OT_SUCCESS,  OT_AVP_CVE_ERR_FAIL,
        "Error(%#x),ot_avp_cve_mag_and_ang failed!\n", ret);

    ret = ot_avp_cve_thresh(&handle, &canny_info->mag, &canny_info->edge, &canny_info->thr_u16_ctrl, mode);
    CHECK_RETURN_WITH_LOG(ret != OT_SUCCESS,  OT_AVP_CVE_ERR_FAIL,
        "Error(%#x),ot_avp_cve_threshold_u16 failed!\n", ret);

    ret = ot_avp_cve_wait_task(handle,OT_SAMPLE_CVE_CANNY_QUERY_TIMEOUT);
    CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS,  OT_AVP_CVE_ERR_FAIL, "ot_avp_cve_wait_task run fail.\n");

    ret = ot_avp_cve_img_write_file(&canny_info->edge, canny_info->dstFile);
    CHECK_RETURN_WITH_LOG(ret != OT_SUCCESS,  OT_AVP_CVE_ERR_FAIL,
        "Error(%#x),Write edge file failed!\n", ret);

    return OT_SUCCESS;
}



ot_void sample_cve_canny(int argc, char *argv[])
{
    // 1. 初始化 CVE
    ot_s32 status = sample_cve_init();
    CHECK_RETURN_WITH_LOG(status != 0, RETURN_NONE, "failed to init sample cve.\n");
    const ot_u16 width = OT_SAMPLE_CVE_D1_WIDTH;
    const ot_u16 height = OT_SAMPLE_CVE_D1_HEIGHT;

    // 2. 构造入参
    status = sample_cve_canny_init(&g_canny_info, width, height);
    CHECK_GOTO(status != 0, EXIT);

    // 3. 执行任务
    status = sample_cve_part_canny(&g_canny_info);
    if (status == 0) {
        printf("Task[canny] Process Success!\n");
    } else {
        printf("Task[canny] Process Fail!\n");
    }

    // 4. 析构入参
    sample_cve_canny_uninit(&g_canny_info);

    memset(&g_canny_info, 0, sizeof(g_canny_info));

EXIT:
    // 5. 去初始化 CVE
    sample_cve_deinit();
}