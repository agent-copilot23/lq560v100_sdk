#include "sample_cve_main.h"

#define OT_SAMPLE_CVE_TEST_RESIZE_QUERY_SLEEP      100
#define OT_SAMPLE_CVE_RESIZE_QUERY_TIMEOUT 10000
#define OT_SAMPLE_CVE_TEST_RESIZE_QUERY_D1_WIDTH   1920
#define OT_SAMPLE_CVE_TEST_RESIZE_QUERY_D1_HEIGHT  1080
#define OT_SAMPLE_CVE_TEST_RESIZE_QUERY_D1_WIDTH2   720
#define OT_SAMPLE_CVE_TEST_RESIZE_QUERY_D1_HEIGHT2  576
#define OT_AVP_CVE_RESIZE_OUT_FILENAME  "./data/test_resize_out_720_576.y"
#define OT_AVP_CVE_RESIZE_IN_FILENAME   "./data/input/source/src_1920_1080_t001.y"

typedef struct {
    ot_avp_cve_src_img cve_src1;
    ot_avp_cve_dst_img cve_dst;
    ot_s8 srcFile[CVE_MAX_FILE_PATH];
    ot_s8 dstFile[CVE_MAX_FILE_PATH];
} sample_resize_info;

static sample_resize_info g_resize_info = {0};


static ot_void sample_cve_resize_deinit(sample_resize_info* info)
{
    ot_avp_cve_destory_img(&info->cve_src1);
    ot_avp_cve_destory_img(&info->cve_dst);
}

static ot_s32 sample_cve_resize_init(sample_resize_info* info, int argc, char *argv[])
{
    /* 参数固定, 以解决方案的参数为例 */
    ot_s32 status = OT_AVP_CVE_SUCCESS;

    ot_u32 src_width = OT_SAMPLE_CVE_TEST_RESIZE_QUERY_D1_WIDTH;
    ot_u32 src_height = OT_SAMPLE_CVE_TEST_RESIZE_QUERY_D1_HEIGHT;
    ot_u32 dst_width = OT_SAMPLE_CVE_TEST_RESIZE_QUERY_D1_WIDTH2;
    ot_u32 dst_height = OT_SAMPLE_CVE_TEST_RESIZE_QUERY_D1_HEIGHT2;

    strcpy(info->srcFile, OT_AVP_CVE_RESIZE_IN_FILENAME);
    strcpy(info->dstFile, OT_AVP_CVE_RESIZE_OUT_FILENAME);

    status = ot_avp_cve_create_img(&info->cve_src1, OT_IMAGE_FORMAT_U8C1, src_width, src_height);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "malloc cve_src1 fail.\n");

    status = ot_avp_cve_create_img(&info->cve_dst, OT_IMAGE_FORMAT_U8C1, dst_width, dst_height);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "malloc cve_dst fail.\n");

    return 0;
EXIT:
    sample_cve_resize_deinit(info);
    return -1;
}

static ot_void sample_cve_resize_print(sample_resize_info* info)
{
    return;
}

static ot_void get_resize_aux_mem_block_num(ot_u32 src_width, ot_u32 src_height, ot_u32 dst_width, ot_u32 dst_height,
    ot_u32 metric, ot_u32* hor_block_num, ot_u32 *ver_block_num)
{
    if (dst_width >= src_width && dst_height >= src_height)  { // 放大
        printf(" Zoom In\n");
        *hor_block_num = (dst_width / metric) + (dst_width % metric == 0 ? 0 : 1);
        *ver_block_num = (dst_height / metric) + (dst_height % metric == 0 ? 0 : 1);
    } else if (dst_width < src_width && dst_height < src_height) { // 缩小
        printf(" Zoom Out\n");
        ot_u32 src_tile_num_width = (src_width / metric) + (src_width % metric == 0 ? 0 : 1);
        ot_u32 src_tmp_tile_width = (src_width / src_tile_num_width) + (src_width % src_tile_num_width == 0 ? 0 : 1);
        ot_u32 scale_width = (src_width / dst_width) + (src_width % dst_width == 0 ? 0 : 1);

        *hor_block_num = dst_width / ((dst_width/src_tile_num_width) + (((metric - src_tmp_tile_width)/scale_width) & ~0x1));

        ot_u32 src_tile_num_height = (src_height / metric) + (src_height % metric == 0 ? 0 : 1);
        ot_u32 src_tmp_tile_height = (src_height / src_tile_num_height) + (src_height % src_tile_num_height == 0 ? 0 : 1);
        ot_u32 scale_height = (src_height / dst_height) + (src_height % dst_height == 0 ? 0 : 1);

        *ver_block_num = dst_height / ((dst_height / src_tile_num_height) + (((metric - src_tmp_tile_height) / scale_height) & ~0x1));
    }
    *hor_block_num = (*hor_block_num == 0) ? 1 : *hor_block_num;
    *ver_block_num = (*ver_block_num == 0) ? 1 : *ver_block_num;
    printf(" hor_block_num[%u], ver_block_num[%u]\n", *hor_block_num, *ver_block_num);
}

// 此处若批量resize，各个输入输出的宽高不一致，batch_num 传入只能是1, 然后挨个计算。
ot_u32 cve_resize_get_aux_mem_size(ot_avp_cve_img* src, ot_avp_cve_img* dst, ot_u32 batch_num)
{
    ot_u32 assist_unit_size = 48;

    ot_u32 hor_block_num = 1;
    ot_u32 ver_block_num = 1;
    ot_u32 hor_block_num_tmp = 1;
    ot_u32 ver_block_num_tmp = 1;
    ot_u32 comp_num =  1;
    ot_u32 metric = 2032;
    switch (src->type) {
        case OT_IMAGE_FORMAT_U8C1:
        case OT_IMAGE_FORMAT_S8C1:
            comp_num = 1;
            metric = 2032;

            get_resize_aux_mem_block_num(src->width, src->height, dst->width, dst->height, metric, &hor_block_num_tmp, &ver_block_num_tmp);
            hor_block_num = hor_block_num_tmp;
            ver_block_num = ver_block_num_tmp;
            break;
        case OT_IMAGE_FORMAT_U16C1:
        case OT_IMAGE_FORMAT_S16C1:
            comp_num = 2;
            metric = 1016;

            get_resize_aux_mem_block_num(src->width, src->height, dst->width, dst->height, metric, &hor_block_num_tmp, &ver_block_num_tmp);
            hor_block_num = hor_block_num_tmp;
            ver_block_num = ver_block_num_tmp;

            break;
        case OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR:
        case OT_IMAGE_FORMAT_YUV_422_SEMIPLANAR:
            comp_num = 2;
            metric = 2032;

            get_resize_aux_mem_block_num(src->width, src->height, dst->width, dst->height, metric, &hor_block_num_tmp, &ver_block_num_tmp);
            hor_block_num = hor_block_num_tmp;
            ver_block_num = ver_block_num_tmp;

            get_resize_aux_mem_block_num(src->width, src->height / 2, dst->width, dst->height / 2, metric, &hor_block_num_tmp, &ver_block_num_tmp);
            hor_block_num += hor_block_num_tmp;
            ver_block_num += ver_block_num_tmp;
            break;
        case OT_IMAGE_FORMAT_U8C3_PLANAR:
            comp_num = 3;
            metric = 2032;
            get_resize_aux_mem_block_num(src->width, src->height, dst->width, dst->height, metric, &hor_block_num_tmp, &ver_block_num_tmp);
            hor_block_num = hor_block_num_tmp * 3;
            ver_block_num = ver_block_num_tmp * 3;
            break;
        default:
            break;
    }

    printf("resize aux_mem size = %u, hor_block_num[%u], ver_block_num[%u]\n",
        (hor_block_num * ver_block_num) * comp_num * batch_num * assist_unit_size, hor_block_num, ver_block_num);

    return (hor_block_num * ver_block_num) * comp_num * batch_num * assist_unit_size;
}

/* resize + map + memory */
static ot_s32 sample_cve_resize_proc(sample_resize_info* info)
{
    sample_cve_resize_print(info);
    ot_s32 status = OT_AVP_CVE_SUCCESS;
    ot_avp_handle handle;
    ot_avp_cve_invoke_mode invoke_mode = OT_AVP_CVE_INVOKE_SYNC;
    ot_avp_cve_resize_ctrl resize_ctrl;
    (ot_void)memset(&resize_ctrl, 0x00, sizeof(resize_ctrl));
    resize_ctrl.alg_mode = OT_AVP_CVE_RESIZE_MODE_BILINEAR;
    resize_ctrl.batch_num = 1;
    status = ot_avp_cve_create_mem_info(&(resize_ctrl.mem), cve_resize_get_aux_mem_size(&info->cve_src1, &info->cve_dst, resize_ctrl.batch_num));
    CHECK_RETURN_WITH_LOG(status != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
        "ot_avp_cve_filter failed!\n");

    // read from file
    status = ot_avp_cve_img_read_file(&info->cve_src1, (const ot_s8*)info->srcFile);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "read image_t file fail.\n");

    invoke_mode = OT_AVP_CVE_INVOKE_TRIGGER_AND_WAIT;
    status = ot_avp_cve_resize(&handle, &info->cve_src1, &info->cve_dst,
        &resize_ctrl, invoke_mode);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "ot_avp_cve_resize run fail.\n");

    status = ot_avp_cve_wait_task(handle,OT_SAMPLE_CVE_RESIZE_QUERY_TIMEOUT);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "ot_avp_cve_wait_task run fail.\n");

    // write to file
    status = ot_avp_cve_img_write_file(&info->cve_dst, (const ot_s8*)info->dstFile);
    CHECK_GOTO_WITH_LOG(status != OT_AVP_CVE_SUCCESS, EXIT, "write dst_yuv8 file fail.\n");

    ot_avp_cve_destory_mem_info(&resize_ctrl.mem);

    return 0;
EXIT:
    printf("sample_cve_resize_proc fail. status[%d].\n", status);
    return -1;
}

ot_void sample_cve_resize(int argc, char *argv[])
{
    // 1. 初始化 CVE
    ot_s32 status = sample_cve_init();
    CHECK_RETURN_WITH_LOG(status != 0, RETURN_NONE, "failed to init sample cve.\n");

    // 2. 构造入参
    status = sample_cve_resize_init(&g_resize_info, argc, argv);
    CHECK_GOTO(status != 0, EXIT);

    // 3. 执行任务
    status = sample_cve_resize_proc(&g_resize_info);
    if (status == 0) {
        printf("Task[resize] Process Success!\n");
    } else {
        printf("Task[resize] Process Fail!\n");
    }

    // 4. 析构入参
    sample_cve_resize_deinit(&g_resize_info);

    memset(&g_resize_info, 0, sizeof(g_resize_info));

EXIT:
    // 5. 去初始化 CVE
    sample_cve_deinit();
}
