/******************************************************************************
 Copyright (C), 2022 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : the cve api
Created       : 2022/11/16
Last Modified :
******************************************************************************/

#ifndef OT_AVP_CVE_API_H
#define OT_AVP_CVE_API_H

#include "ot_avp_cve_comm.h"
#include "ot_avp_cve_errno.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/**
 * @brief Init CVE.
 *
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_init();

/**
 * @brief Deinit CVE.
 *
 * @return ot_avp_cve_status_e.
 */
ot_avp_cve_status_e ot_avp_cve_deinit();


/**
 * @brief Wait for a particular task hardware execute done.
 *
 * @param [in] handle 算子任务句柄, 不能为空
 * @param [in] timeout 超时时间, 单位ms, 取值: [-1, 65535]
 * @return ot_avp_cve_status_e 任务完成状态
 */
ot_avp_cve_status_e ot_avp_cve_wait_task(ot_avp_handle handle, ot_s32 timeout);

/**
 * @brief 创建二维数据直接内存拷贝任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源数据指针，不能为空
 * @param [out] dst 目的数据指针，copy模式下不能为空
 * @param [in] ctrl dma_2d算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_dma_2d(ot_avp_handle *handle, const ot_avp_cve_src_mem_2d *src, const ot_avp_cve_dst_mem_2d *dst,
    const ot_avp_cve_dma_2d_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建3x3/5x5模板滤波任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] ctrl filter算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_filter(ot_avp_handle *handle, const ot_avp_cve_src_img *src, const ot_avp_cve_dst_img *dst,
    const ot_avp_cve_filter_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建3x3/5x5模板sobel-like梯度计算任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 使用输入模板直接计算的结果输出指针
 * @param [out] dst_T 使用输入模板的转置计算的结果输出指针
 * @param [in] ctrl sobel算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_sobel(ot_avp_handle *handle, const ot_avp_cve_src_img *src,
    const ot_avp_cve_dst_img *dst, const ot_avp_cve_dst_img *dst_T,
    const ot_avp_cve_sobel_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建3x3/5x5模板梯度幅值与幅角任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst_mag 幅值输出
 * @param [out] dst_ang 相角输出
 * @param [in] ctrl mag_and_ang算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_mag_and_ang(ot_avp_handle *handle, const ot_avp_cve_src_img *src,
    const ot_avp_cve_dst_img *dst_mag, const ot_avp_cve_dst_img *dst_ang,
    const ot_avp_cve_mag_and_ang_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建针对U8C1/U16C1/S16C1灰度图的阈值化任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] ctrl thresh算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_thresh(ot_avp_handle *handle, const ot_avp_cve_src_img *src, const ot_avp_cve_dst_img *dst,
    const ot_avp_cve_thresh_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建针对图像形态学操作的任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] ctrl morph算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_morph(ot_avp_handle *handle, const ot_avp_cve_src_img *src, const ot_avp_cve_dst_img *dst,
    const ot_avp_cve_morph_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建针对图像(bitmap和U8C1)像素点按位与任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src1 源图像1指针，不能为空
 * @param [in] src2 源图像2指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_bitwise_and(ot_avp_handle *handle, const ot_avp_cve_src_img *src1, const ot_avp_cve_src_img *src2,
    const ot_avp_cve_dst_img *dst, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建针对图像(bitmap和U8C1)像素点按位或任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src1 源图像1指针，不能为空
 * @param [in] src2 源图像2指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_bitwise_or(ot_avp_handle *handle, const ot_avp_cve_src_img *src1, const ot_avp_cve_src_img *src2,
    const ot_avp_cve_dst_img *dst, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建针对图像(bitmap和U8C1)像素点按位异或任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src1 源图像1指针，不能为空
 * @param [in] src2 源图像2指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_bitwise_xor(ot_avp_handle *handle, const ot_avp_cve_src_img *src1, const ot_avp_cve_src_img *src2,
    const ot_avp_cve_dst_img *dst, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建针对图像的加权加法任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src1 源1图像指针，不能为空
 * @param [in] src2 源2图像指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] ctrl add算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_add(ot_avp_handle *handle, const ot_avp_cve_src_img *src1, const ot_avp_cve_src_img *src2,
    const ot_avp_cve_dst_img *dst, const ot_avp_cve_add_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建针对图像的减法任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src1 源图像指针
 * @param [in] src2 源图像指针
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] ctrl sub算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_sub(ot_avp_handle *handle, const ot_avp_cve_src_img *src1, const ot_avp_cve_src_img *src2,
    const ot_avp_cve_dst_img *dst, const ot_avp_cve_sub_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建灰度图像的直方图统计任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 输出直方图统计的256bin信息指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */

ot_avp_cve_status_e ot_avp_cve_hist(ot_avp_handle *handle, const ot_avp_cve_src_img *src, ot_avp_cve_dst_hist_data* dst,
    ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建map（映射赋值）任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [in] lut 查找表信息指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_map(ot_avp_handle *handle, const ot_avp_cve_src_img *src, const ot_avp_cve_src_lut_data* lut,
    ot_avp_cve_dst_img *dst, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建灰度图像的直方图均衡化计算任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_equalize_hist(ot_avp_handle *handle, const ot_avp_cve_src_img *src, ot_avp_cve_dst_img *dst,
    const ot_avp_cve_equalize_hist_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建图像的积分图(sum/square)计算任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] ctrl integ控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_integ(ot_avp_handle *handle, const ot_avp_cve_src_img *src, ot_avp_cve_dst_img *dst,
    const ot_avp_cve_integ_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建以3x3/5x5的图像中值滤波计算任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] ctrl 算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_median_filter(ot_avp_handle *handle, const ot_avp_cve_src_img *src, const ot_avp_cve_dst_img *dst,
    const ot_avp_cve_median_filter_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建以3x3/5x5的图像最大值最小值滤波计算任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] ctrl 算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_min_max_filter(ot_avp_handle *handle, const ot_avp_cve_src_img *src, const ot_avp_cve_dst_img *dst,
    const ot_avp_cve_min_max_filter_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建计算图像src1和图像src2对应分块之间的SAD(差的绝对值之和)，并输出SAD的阈值化结果计算任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src1 源图像1指针，不能为空
 * @param [in] src2 源图像2指针，不能为空
 * @param [out] dst_sad 目的sad图像指针
 * @param [out] dst_thr 目的thr图像指针
 * @param [in] ctrl 算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_sad(ot_avp_handle *handle, const ot_avp_cve_img* src1, const ot_avp_cve_img* src2,
    const ot_avp_cve_img* dst_sad, const ot_avp_cve_img* dst_thr,
    const ot_avp_cve_sad_ctrl *ctrl, ot_avp_cve_invoke_mode mode);



/**
 * @brief 创建CSC(Color Space Convert)RGB与YCbCr色彩空间转换计算任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst 目的图像指针，不能为空
 * @param [in] ctrl 算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_csc(ot_avp_handle *handle, const ot_avp_cve_src_img *src, ot_avp_cve_dst_img *dst,
    const ot_avp_cve_csc_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建图像缩放任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像数组，不能为空
 * @param [out] dst 目的图像数组，不能为空
 * @param [in] ctrl 控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_resize(ot_avp_handle *handle, const ot_avp_cve_src_img src[], ot_avp_cve_dst_img dst[],
    const ot_avp_cve_resize_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建Shi-Tomasi角点计算任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 源图像指针，不能为空
 * @param [out] dst_corner_info 输出角点指针，不能为空
 * @param [in] ctrl 算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_st_corner(ot_avp_handle *handle, const ot_avp_cve_src_img *src,
    ot_dst_mem_info *dst_corner_info, const ot_avp_cve_st_corner_ctrl *ctrl, ot_avp_cve_invoke_mode mode);

/**
 * @brief 创建金字塔LK光流任务
 *
 * @param [out] handle 算子任务句柄，不能为空
 * @param [in] src 输入信息指针，不能为空
 * @param [out] out_info 输出信息指针，不能为空
 * @param [in] ctrl 算子控制参数指针，不能为空
 * @param [in] mode 任务工作模式设置参数指针，不能为空
 * @return ot_avp_cve_status_e
 */
ot_avp_cve_status_e ot_avp_cve_lk_optical_flow_pyr(ot_avp_handle *handle, const ot_avp_cve_lk_optical_flow_pyr_src_info *src,
    const ot_avp_cve_lk_optical_flow_pyr_dst_info *dst,
    const ot_avp_cve_lk_optical_flow_pyr_ctrl *ctrl, ot_avp_cve_invoke_mode mode);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif