#include "sample_cve_main.h"

#define OT_SAMPLE_CVE_TEST_MEM_QUERY_SLEEP      100
#define OT_SAMPLE_CVE_MEM_QUERY_TIMEOUT 10000
#define OT_SAMPLE_CVE_TEST_MEM_QUERY_D1_WIDTH   720
#define OT_SAMPLE_CVE_TEST_MEM_QUERY_D1_HEIGHT  576
#define OT_AVP_CVE_SOBEL_OUT_T_FILENAME  "./data/test_sobel_out_t.yuv"
#define OT_AVP_CVE_SOBEL_OUT_FILENAME  "./data/test_sobel_out.yuv"
#define OT_AVP_CVE_SOBEL_IN_FILENAME   "./data/input/source/sobel.yuv"
typedef struct {
    ot_avp_cve_src_img cve_src1;
    ot_avp_cve_src_img cve_dstT;
    ot_avp_cve_dst_img cve_dst;

    ot_s8 srcFile[CVE_MAX_FILE_PATH];
    ot_s8 dstFile[CVE_MAX_FILE_PATH];
    ot_s8 dstTFile[CVE_MAX_FILE_PATH];
} sample_sobel_info;

static sample_sobel_info g_sobel_info = {0};

static ot_void sample_cve_sobel_deinit(sample_sobel_info* info)
{
    ot_avp_cve_destory_img(&info->cve_src1);
    ot_avp_cve_destory_img(&info->cve_dstT);
    ot_avp_cve_destory_img(&info->cve_dst);
}

static ot_s32 sample_cve_sobel_init(sample_sobel_info* info, int argc, char *argv[])
{
    /* 参数固定, 以解决方案的参数为例 */
    ot_s32 status = OT_AVP_CVE_SUCCESS;

    ot_u32 src_width = OT_SAMPLE_CVE_TEST_MEM_QUERY_D1_WIDTH;
    ot_u32 src_height = OT_SAMPLE_CVE_TEST_MEM_QUERY_D1_HEIGHT;
    ot_u32 dst_width = OT_SAMPLE_CVE_TEST_MEM_QUERY_D1_WIDTH;
    ot_u32 dst_height = OT_SAMPLE_CVE_TEST_MEM_QUERY_D1_HEIGHT;

    strcpy(info->srcFile, OT_AVP_CVE_SOBEL_IN_FILENAME);
    strcpy(info->dstFile, OT_AVP_CVE_SOBEL_OUT_FILENAME);
    strcpy(info->dstTFile, OT_AVP_CVE_SOBEL_OUT_T_FILENAME);
    status = ot_avp_cve_create_img(&info->cve_src1, OT_IMAGE_FORMAT_U8C1, src_width, src_height);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "malloc cve_src1 fail.\n");
    status = ot_avp_cve_create_img(&info->cve_dst, OT_IMAGE_FORMAT_S16C1, dst_width, dst_height);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "malloc cve_dst fail.\n");
    status = ot_avp_cve_create_img(&info->cve_dstT, OT_IMAGE_FORMAT_S16C1, dst_width, dst_height);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "malloc cve_dstT fail.\n");
    return 0;
EXIT:
    sample_cve_sobel_deinit(info);
    return -1;
}

static ot_void sample_cve_sobel_print(sample_sobel_info* info)
{
    return;
}

/* resize + map + sobel */
static ot_s32 sample_cve_sobel_proc(sample_sobel_info* info)
{
    sample_cve_sobel_print(info);
    ot_s32 status = OT_AVP_CVE_SUCCESS;
    ot_avp_handle handle;
    ot_avp_cve_invoke_mode invoke_mode = OT_AVP_CVE_INVOKE_SYNC;
    ot_avp_cve_sobel_ctrl ctrl;
    ctrl.out_mode = OT_AVP_CVE_SOBEL_OUT_BOTH;
    ctrl.kernel_attr.ksize_mode = OT_KERNEL_SIZE_MODE_5x5;
    ot_s16 mask[OT_AVP_CVE_MASK_NUM] = { 0, 0, 0, 0, 0, 0, -1, 0, 1, 0, 0, -2, 0, 2, 0, 0, -1, 0, 1, 0, 0, 0, 0, 0, 0 };
    ctrl.kernel_attr.coef = mask;

    // read from file
    status = ot_avp_cve_img_read_file(&info->cve_src1, (const ot_s8*)info->srcFile);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "read image_t file fail.\n");

    invoke_mode = OT_AVP_CVE_INVOKE_TRIGGER;
    status = ot_avp_cve_sobel(&handle, &info->cve_src1, &info->cve_dst, &info->cve_dstT, &ctrl, invoke_mode);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "ot_avp_cve_sobel run fail.\n");

    status = ot_avp_cve_wait_task(handle,OT_SAMPLE_CVE_MEM_QUERY_TIMEOUT);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "ot_avp_cve_wait_task run fail.\n");

    // write to file
    status = ot_avp_cve_img_write_file(&info->cve_dst, (const ot_s8*)info->dstFile);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "write cve_dst file fail.\n");

    status = ot_avp_cve_img_write_file(&info->cve_dstT,  (const ot_s8*)info->dstTFile);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "write dst_T file fail.\n");

    return 0;
EXIT:
    printf("sample_cve_sobel_proc fail. status[%d].\n", status);
    return -1;
}

ot_void sample_cve_sobel(int argc, char *argv[])
{
    // 1. 初始化 CVE
    ot_s32 status = sample_cve_init();
    CHECK_RETURN_WITH_LOG(status != 0, RETURN_NONE, "failed to init sample cve.\n");

    // 2. 构造入参
    status = sample_cve_sobel_init(&g_sobel_info, argc, argv);
    CHECK_GOTO(status != 0, EXIT);

    // 3. 执行任务
    status = sample_cve_sobel_proc(&g_sobel_info);
    if (status == 0) {
        printf("Task[sobel] Process Success!\n");
    } else {
        printf("Task[sobel] Process Fail!\n");
    }

    // 4. 析构入参
    sample_cve_sobel_deinit(&g_sobel_info);

    memset(&g_sobel_info, 0, sizeof(g_sobel_info));

EXIT:
    // 5. 去初始化 CVE
    sample_cve_deinit();
}
