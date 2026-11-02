#include "sample_cve_main.h"
#include <limits.h>

#define OT_SAMPLE_CVE_LK_MAX_POINT_NUM      500
#define OT_SAMPLE_CVE_LK_MIN_DIST           5
#define OT_SAMPLE_CVE_LK_PYR_NUM            4
#define OT_SAMPLE_CVE_LK_QUERY_SLEEP        100
#define OT_SAMPLE_CVE_LK_MIN_EIG_VALUE      100
#define OT_SAMPLE_CVE_LK_ITER_CNT           10
#define OT_SAMPLE_CVE_LK_EPS                2
#define OT_SAMPLE_CVE_LEFT_SHIFT_SEVEN      7
#define OT_SAMPLE_CVE_ST_QUALITY_LEVEL      25
#define OT_SAMPLE_CVE_LK_D1_WIDTH           1280
#define OT_SAMPLE_CVE_LK_D1_HEIGHT          720
#define OT_SAMPLE_CVE_LK_MAX_LEVEL          3
#define OT_SAMPLE_CVE_LK_FRAME_NUM          10
#define OT_SAMPLE_CVE_HOR_SEG_SIZE          2
#define OT_SAMPLE_CVE_ELEM_SIZE             1
#define OT_SAMPLE_CVE_VER_SEG_ROWS          2
#define OT_AVP_CVE_ALIGN 16
#define OT_AVP_CVE_LK_OUT_FILENAME  "./data/test_stlk_out.yuv"
#define OT_AVP_CVE_LK_IN_FILENAME   "./data/input/source/src_1280x720_420sp_8bit.yuv"
#define OT_ST_MAX_EIG_VAL 16
#define OT_ST_CORNER_INFO_SIZE 1002
#define OT_SAMPLE_CVE_TEST_MEM_QUERY_SLEEP      100
#define OT_SAMPLE_CVE_MEM_QUERY_TIMEOUT 10000

typedef struct {
    ot_avp_cve_src_img prev_pyr[OT_SAMPLE_CVE_LK_PYR_NUM];
    ot_avp_cve_src_img next_pyr[OT_SAMPLE_CVE_LK_PYR_NUM];
    ot_mem_info prev_points;
    ot_mem_info next_points;
    ot_mem_info status;
    ot_mem_info err;
    ot_avp_cve_lk_optical_flow_pyr_ctrl lk_pyr_ctrl;

    ot_avp_cve_src_img src;
    ot_avp_cve_img dst;
    ot_mem_info corner;
    ot_avp_cve_st_corner_ctrl corner_ctrl;
    ot_avp_cve_img pyr_tmp;
    ot_avp_cve_img src_yuv;
    ot_s8 srcFile[CVE_MAX_FILE_PATH];
} ot_sample_cve_st_lk_inf;

static ot_sample_cve_st_lk_inf g_lk_info;

static ot_u32 sample_cve_calc_stride(ot_u32 width, ot_u8 align)
{
    return (width + (align - width % align) % align);
}

static ot_s32 sample_cve_st_lk_dma(ot_avp_handle *cve_handle, ot_avp_cve_src_img *src,
    ot_avp_cve_dst_img *dst, ot_avp_cve_dma_2d_ctrl *dma_ctrl, ot_bool is_instant)
{
    ot_s32 ret;
    ot_data_info data_src;
    ot_data_info data_dst;
    data_src.virt_addr = src->virt_addr[0];
    data_src.phys_addr = src->phys_addr[0];
    data_src.width = src->width;
    data_src.height = src->height;
    data_src.stride = src->stride[0];

    data_dst.virt_addr = dst->virt_addr[0];
    data_dst.phys_addr = dst->phys_addr[0];
    data_dst.width = dst->width;
    data_dst.height = dst->height;
    data_dst.stride = dst->stride[0];
    printf("sample_cve_st_lk_dma src  width %d height %d phyaddr %lld , dst width %d height %d phyaddr %lld\n", src->width,src->height,
    src->phys_addr[0], dst->width, dst->height,dst->phys_addr[0]);
    ret = ot_avp_cve_dma_2d(cve_handle, &data_src, &data_dst, dma_ctrl, OT_AVP_CVE_INVOKE_SYNC);
    CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
        "Error(%#x),ot_avp_cve_dma failed!\n", ret);

    return ret;
}


/*
 * function : Copy pyr
 */
static ot_void sample_cve_st_lk_copy_pyr(ot_avp_cve_src_img pyr_src[], ot_avp_cve_dst_img pyr_dst[],
    ot_u8 level_num)
{
    ot_u8 i;
    ot_s32 ret;
    ot_avp_handle handle;

    ot_avp_cve_dma_2d_ctrl dma_ctrl;
    (ot_void)memset(&dma_ctrl, 0x00, sizeof(dma_ctrl));
    dma_ctrl.mode = OT_AVP_CVE_DMA_2D_MODE_DIRECT_CP;

    for (i = 0; i <= level_num; i++) {
        ret = sample_cve_st_lk_dma(&handle, &pyr_src[i], &pyr_dst[i], &dma_ctrl, OT_FALSE);
        if (ret != OT_AVP_CVE_SUCCESS) {
            printf("sample_cve_st_lk_dma fail,Error(%d)\n", ret);
            break;
        }
    }
}

/*
 * function : St lk uninit
 */
static ot_void sample_cve_st_lk_uninit(ot_sample_cve_st_lk_inf *lk_info)
{
    ot_u16 i;
    for (i = 0; i <= lk_info->lk_pyr_ctrl.level_num; i++) {
        ot_avp_cve_destory_img(&lk_info->prev_pyr[i]);
        ot_avp_cve_destory_img(&lk_info->next_pyr[i]);
    }

    ot_avp_cve_destory_mem_info(&lk_info->prev_points);
    ot_avp_cve_destory_mem_info(&lk_info->next_points);
    ot_avp_cve_destory_mem_info(&lk_info->status);
    ot_avp_cve_destory_mem_info(&lk_info->err);

    ot_avp_cve_destory_img(&lk_info->src);
    ot_avp_cve_destory_img(&lk_info->dst);
    ot_avp_cve_destory_mem_info(&lk_info->corner);

    ot_avp_cve_destory_mem_info(&lk_info->corner_ctrl.aux_mem);

    ot_avp_cve_destory_img(&lk_info->pyr_tmp);
    ot_avp_cve_destory_img(&lk_info->src_yuv);

}

static ot_s32 sample_cve_lk_param_init(ot_sample_cve_st_lk_inf *lk_info, ot_u32 width, ot_u32 height, ot_u8 level_num)
{
    ot_s32 ret;
    ot_u32 size;
    ot_u32 i;
    lk_info->lk_pyr_ctrl.out_mode = OT_AVP_CVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH;
    lk_info->lk_pyr_ctrl.level_num = level_num;       /* 金字塔层数 */
    lk_info->lk_pyr_ctrl.points_num = OT_SAMPLE_CVE_LK_MAX_POINT_NUM;     /* 特征点数目 */
    lk_info->lk_pyr_ctrl.use_init_flow = OT_TRUE; /* 是否使用初始化光流计算 */
    lk_info->lk_pyr_ctrl.min_eig_thr = OT_SAMPLE_CVE_LK_MIN_EIG_VALUE;    /* 最小特征值阈值 */
    lk_info->lk_pyr_ctrl.iteration_cnt = OT_SAMPLE_CVE_LK_ITER_CNT;   /* 迭代次数 */
    lk_info->lk_pyr_ctrl.epsilon = OT_SAMPLE_CVE_LK_EPS;        /* 迭代收敛条件：dx^2 + dy^2 < epsilon  */
    /* Init Pyr */
    for (i = 0; i <= level_num; i++) {
        printf("ot_avp_cve_create_img lk_info->prev_pyr[%d] width %d height %d\n", i,width >> i, height >> i);
        ret = ot_avp_cve_create_img(&lk_info->prev_pyr[i], OT_IMAGE_FORMAT_U8C1, width >> i, height >> i);
        CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, lk_init_fail,
            "Error(%#x),Create prevPyr[%u] image failed!\n", ret, i);
        printf("ot_avp_cve_create_img lk_info->next_pyr[%d] width %d height %d\n", i,lk_info->prev_pyr[i].width,lk_info->prev_pyr[i].height);
        ret = ot_avp_cve_create_img(&lk_info->next_pyr[i], OT_IMAGE_FORMAT_U8C1, lk_info->prev_pyr[i].width, lk_info->prev_pyr[i].height);
        CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, lk_init_fail,
            "Error(%#x),Create nextPyr[%u] image failed!\n", ret, i);
    }
    /* Init prev pts */
    size = (ot_u32)sizeof(ot_avp_cve_point_s25q7) * OT_SAMPLE_CVE_LK_MAX_POINT_NUM;
    size = sample_cve_calc_stride(size, OT_AVP_CVE_ALIGN);
    ret = ot_avp_cve_create_mem_info(&(lk_info->prev_points), size);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, lk_init_fail,
        "Error(%#x),Create prevPts mem info failed!\n", ret);
    /* Init next pts */
    ret = ot_avp_cve_create_mem_info(&(lk_info->next_points), size);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, lk_init_fail,
        "Error(%#x),Create nextPts mem info failed!\n", ret);

    /* Init status */
    size = (ot_u32)sizeof(ot_u8) * OT_SAMPLE_CVE_LK_MAX_POINT_NUM;
    size = sample_cve_calc_stride(size, OT_AVP_CVE_ALIGN);
    ret = ot_avp_cve_create_mem_info(&(lk_info->status), size);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, lk_init_fail,
        "Error(%#x),Create status mem info failed!\n", ret);

    /* Init err */
    size = (ot_u32)sizeof(ot_u16) * OT_SAMPLE_CVE_LK_MAX_POINT_NUM;
    size = sample_cve_calc_stride(size, OT_AVP_CVE_ALIGN);
    ret = ot_avp_cve_create_mem_info(&(lk_info->err), size);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, lk_init_fail,
        "Error(%#x),Create err mem info failed!\n", ret);

lk_init_fail:
    if (ret != OT_AVP_CVE_SUCCESS) {
        sample_cve_st_lk_uninit(lk_info);
    }
    return ret;
}

static ot_s32 sample_cve_st_param_init(ot_sample_cve_st_lk_inf *lk_info, ot_u32 width, ot_u32 height)
{
    ot_s32 ret;
    ot_u32 size;

    /* Init St */
    ret = ot_avp_cve_create_img(&lk_info->src, OT_IMAGE_FORMAT_U8C1, width, height);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, st_init_fail,
        "Error(%#x),Create src image failed!\n", ret);

    ret = ot_avp_cve_create_img(&lk_info->dst, OT_IMAGE_FORMAT_U8C1, width, height);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, st_init_fail,
        "Error(%#x),Create dst image failed!\n", ret);
    lk_info->corner_ctrl.out_mode = OT_AVP_CVE_ST_OUT_POINT_U16;
    lk_info->corner_ctrl.quality_level = OT_SAMPLE_CVE_ST_QUALITY_LEVEL;
    size = (ot_u32)sizeof(ot_u32) * sample_cve_calc_stride(width, OT_AVP_CVE_ALIGN) * height +
        OT_ST_MAX_EIG_VAL + lk_info->src.stride[0] * height;;

    ret = ot_avp_cve_create_mem_info(&(lk_info->corner_ctrl.aux_mem), size);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, st_init_fail,
        "Error(%#x),Create CandiCornerCtrl.stMem mem info failed!\n", ret);

    size = (ot_u32)sizeof(ot_avp_cve_st_corner_info);
    ret = ot_avp_cve_create_mem_info(&(lk_info->corner), size);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, st_init_fail,
        "Error(%#x),Create corner mem info failed!\n", ret);

    lk_info->corner_ctrl.max_corner_num = OT_SAMPLE_CVE_LK_MAX_POINT_NUM;
    lk_info->corner_ctrl.min_distance = OT_SAMPLE_CVE_LK_MIN_DIST;
    return OT_AVP_CVE_SUCCESS;
st_init_fail:
    if (ret != OT_AVP_CVE_SUCCESS) {
        sample_cve_st_lk_uninit(lk_info);
    }
    return ret;
}

/*
 * function : St lk init
 */
static ot_s32 sample_cve_st_lk_init(ot_sample_cve_st_lk_inf *lk_info, ot_u32 width, ot_u32 height,
    ot_u32 pyr_width, ot_u32 pyr_height, ot_u8 level_num)
{
    ot_s32 ret;

    (ot_void)memset(lk_info, 0x00, sizeof(ot_sample_cve_st_lk_inf));

    strcpy(lk_info->srcFile, OT_AVP_CVE_LK_IN_FILENAME);
    ret = sample_cve_lk_param_init(lk_info, width, height, level_num);
    CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL, "sample_cve_lk_param_init failed\n");

    ret = sample_cve_st_param_init(lk_info, width, height);
    CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL, "sample_cve_st_param_init failed\n");

    ret = ot_avp_cve_create_img(&lk_info->pyr_tmp, OT_IMAGE_FORMAT_U8C1, pyr_width, pyr_height);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, st_lk_init_fail, "Error(%#x),Create pyrTmp image failed!\n", ret);

    ret = ot_avp_cve_create_img(&lk_info->src_yuv, OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR, width, height);
    CHECK_GOTO_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, st_lk_init_fail, "Error(%#x),Create srcYuv image failed!\n", ret);


    ret = OT_AVP_CVE_SUCCESS;

st_lk_init_fail:
    if (ret != OT_AVP_CVE_SUCCESS) {
        sample_cve_st_lk_uninit(lk_info);
    }
    return ret;
}


/*
 * function : Pyr down
 */
static ot_s32 sample_cve_st_lk_pyr_down(ot_sample_cve_st_lk_inf *lk_info, ot_avp_cve_src_img *src,
    ot_avp_cve_dst_img *dst)
{
    ot_s32 ret;
    ot_avp_handle handle;

    ot_avp_cve_filter_ctrl ctrl = {0};
    ctrl.kernel_attr.ksize_mode = OT_KERNEL_SIZE_MODE_5x5;
    ot_s16 mask[25] = {1, 2, 3, 2, 1, 2, 5, 6, 5, 2, 3, 6, 8, 6, 3, 2, 5, 6, 5, 2, 1, 2, 3, 2, 1};
    ctrl.kernel_attr.coef = mask;
    ctrl.border_attr.pad_mode = OT_IMG_BORDER_EDGE;
    ctrl.border_attr.pad_val = 0;
    ctrl.norm_attr.scale = 2;
    ot_avp_cve_invoke_mode mode = OT_AVP_CVE_INVOKE_TRIGGER;

    ot_avp_cve_resize_ctrl resize_ctrl = {0};
    resize_ctrl.alg_mode = OT_AVP_CVE_RESIZE_MODE_BILINEAR;
    resize_ctrl.batch_num = 1;
    ret = ot_avp_cve_create_mem_info(&(resize_ctrl.mem), resize_ctrl.batch_num  * 48);
    CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
        "Error(%#x),ot_avp_cve_filter failed!\n", ret);

    lk_info->pyr_tmp.width = src->width;
    lk_info->pyr_tmp.height = src->height;

    ret = ot_avp_cve_filter(&handle, src, &lk_info->pyr_tmp, &ctrl, mode);
    CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
        "Error(%#x),ot_avp_cve_filter failed!\n", ret);

    ret = ot_avp_cve_resize(&handle, &lk_info->pyr_tmp, dst, &resize_ctrl, mode);
    CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
        "Error(%#x),ot_avp_cve_resize failed!\n", ret);

    ot_s32 status = ot_avp_cve_wait_task(handle, OT_SAMPLE_CVE_MEM_QUERY_TIMEOUT);
    CHECK_RETURN_WITH_LOG(status != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL, "ot_avp_cve_wait_task run fail.\n");

    ot_avp_cve_destory_mem_info(&resize_ctrl.mem);
    return ret;
}


static ot_s32 sample_cve_query_task(ot_avp_handle handle)
{
    ot_s32 status = ot_avp_cve_wait_task(handle,OT_SAMPLE_CVE_MEM_QUERY_TIMEOUT);
    CHECK_RETURN_WITH_LOG(status != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL, "ot_avp_cve_wait_task run fail.\n");
    return OT_AVP_CVE_SUCCESS;
}

static ot_s32 sample_cve_proc_frame(ot_u32 i, ot_avp_handle handle, ot_sample_cve_st_lk_inf *lk)
{
    ot_s32 ret;
    ot_u32 k;
    ot_u32 rect_num;

    ot_avp_cve_st_corner_info *corner_info = cve_convert_addr_to_ptr(ot_avp_cve_st_corner_info, lk->corner.virt_addr);
    ot_avp_cve_point_s25q7 *next_points = cve_convert_addr_to_ptr(ot_avp_cve_point_s25q7, lk->next_points.virt_addr);
    ot_avp_cve_invoke_mode  mode = OT_AVP_CVE_INVOKE_TRIGGER;
    if (i == 0) {
        ret = ot_avp_cve_st_corner(&handle, &lk->next_pyr[0], &lk->corner, &lk->corner_ctrl, mode);
        CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
            "Error(%#x),ot_avp_cve_st_cand_corner failed!\n", ret);

        ret = sample_cve_query_task(handle);
        CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
            "Error(%#x),sample_cve_query_task failed!\n", ret);

        lk->lk_pyr_ctrl.points_num = corner_info->corner_num;
        for (k = 0; k < lk->lk_pyr_ctrl.points_num; k++) {
            next_points[k].x = (ot_s32)(corner_info->point_u16[k].x << OT_SAMPLE_CVE_LEFT_SHIFT_SEVEN);
            next_points[k].y = (ot_s32)(corner_info->point_u16[k].y << OT_SAMPLE_CVE_LEFT_SHIFT_SEVEN);
        }
    } else {
        ot_avp_cve_lk_optical_flow_pyr_src_info src;
        ot_avp_cve_lk_optical_flow_pyr_dst_info dst;
        for(int i = 0; i< OT_SAMPLE_CVE_LK_PYR_NUM; i++){
            src.prev_pyr[i] = lk->prev_pyr[i];
            src.next_pyr[i] = lk->next_pyr[i];
            src.prev_pts = lk->prev_points;
        }

        dst.error = lk->err;
        dst.next_pts = lk->next_points;
        dst.status = lk->status;

        ret = ot_avp_cve_lk_optical_flow_pyr(&handle, &src, &dst, &lk->lk_pyr_ctrl, mode);
        CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
            "Error(%#x),ot_avp_cve_lk_optical_flow_pyr failed!\n", ret);

        ret = sample_cve_query_task(handle);
        CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
            "Error(%#x),sample_cve_query_task failed!\n", ret);

        rect_num = 0;
        for (k = 0; k < lk->lk_pyr_ctrl.points_num; k++) {
            if ((cve_convert_addr_to_ptr(ot_u8, lk->status.virt_addr))[k] == 0) {
                continue;
            }
            next_points[rect_num].x = next_points[k].x;
            next_points[rect_num].y = next_points[k].y;
            rect_num++;
        }

        lk->lk_pyr_ctrl.points_num = rect_num;
    }
    return OT_AVP_CVE_SUCCESS;
}

static ot_s32 sample_cve_st_and_lk_proc(ot_sample_cve_st_lk_inf *lk)
{
    const ot_u32 frame_num = OT_SAMPLE_CVE_LK_FRAME_NUM;
    ot_u32 i, k;
    ot_s32 ret = OT_AVP_CVE_ERR_NULL_PTR;
    ot_avp_handle handle;
    ot_avp_cve_dma_2d_ctrl dma_ctrl = {0};
    CHECK_RETURN_WITH_LOG(lk == OT_NULL, OT_AVP_CVE_ERR_FAIL, "test_mem can't be null\n");

    dma_ctrl.mode = OT_AVP_CVE_DMA_2D_MODE_DIRECT_CP;

    for (i = 0; (i < frame_num); i++) {
        printf("Proc frame %u\n", i);
        ot_s32 status = ot_avp_cve_img_read_file(&lk->src_yuv, (const ot_s8*)lk->srcFile);
        CHECK_RETURN_WITH_LOG(status != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL, "read image_t file fail.\n");
        printf("0============================================================width %d height %d \n",lk->next_pyr[0].width, lk->next_pyr[0].height);
        ret = sample_cve_st_lk_dma(&handle, &lk->src_yuv, &lk->next_pyr[0], &dma_ctrl, OT_TRUE);
        CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
            "Error(%#x),sample_cve_st_lk_dma failed!\n", ret);
        printf("1============================================================\n");
        for (k = 1; k <= lk->lk_pyr_ctrl.level_num; k++) {
            ret = sample_cve_st_lk_pyr_down(lk, &lk->next_pyr[k - 1], &lk->next_pyr[k]);
            CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
                "Error(%#x),sample_cve_st_lk_pyr_down %u failed!\n", ret, k);
        }
        ret = sample_cve_proc_frame(i, handle, lk);
        CHECK_RETURN_WITH_LOG(ret != OT_AVP_CVE_SUCCESS, OT_AVP_CVE_ERR_FAIL,
            "Error(%#x),sample_cve_proc_frame failed!\n", ret);

        memcpy(cve_convert_addr_to_ptr(ot_void, lk->prev_points.virt_addr),
            cve_convert_addr_to_ptr(ot_void, lk->next_points.virt_addr),
            sizeof(ot_avp_cve_point_s25q7) * lk->lk_pyr_ctrl.points_num);

        sample_cve_st_lk_copy_pyr(lk->next_pyr, lk->prev_pyr, lk->lk_pyr_ctrl.level_num);
    }

    return OT_AVP_CVE_SUCCESS;
}



ot_void sample_cve_st_and_lk(int argc, char *argv[])
{

    ot_s32 status = sample_cve_init();
    CHECK_RETURN_WITH_LOG(status != 0, RETURN_NONE, "failed to init sample cve.\n");

    status = sample_cve_st_lk_init(&g_lk_info, OT_SAMPLE_CVE_LK_D1_WIDTH, OT_SAMPLE_CVE_LK_D1_HEIGHT, OT_SAMPLE_CVE_LK_D1_WIDTH, OT_SAMPLE_CVE_LK_D1_HEIGHT,OT_SAMPLE_CVE_LK_MAX_LEVEL);
    CHECK_GOTO(status != 0, EXIT);

    status = sample_cve_st_and_lk_proc(&g_lk_info);
    if (status == 0) {
        printf("Task[st_and_lk] Process Success!\n");
    } else {
        printf("Task[st_and_lk] Process Fail!\n");
    }

    sample_cve_st_lk_uninit(&g_lk_info);

    memset(&g_lk_info, 0x00, sizeof(g_lk_info));

EXIT:

    sample_cve_deinit();
}
