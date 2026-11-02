#ifndef __OT_EIS_VPROC_GRP_H__
#define __OT_EIS_VPROC_GRP_H__

#include "ot_eis_type.h"
#include "ot_eis_vproc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建vproc grp，每条通路使用vproc 需要调用一次
 *
 * @param [in] grp_id :grp的id
 * @param [out] grp_hdl :grp的handle
 * @param [in] grp_attr :grp的属性
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_create(const ot_vproc_grp_id grp_id, ot_eis_handle* grp_hdl, const ot_eis_vproc_grp_attr* grp_attr);
/**
 * @brief vproc grp销毁，销毁前必须调用stop和detach
 *
 * @param [in] grp_hdl : vproc grp的 handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_destroy(ot_eis_handle grp_hdl);
/**
 * @brief 使用vproc grp handle 获取 grp id
 *
 * @param [in] grp_hdl : vproc grp的 handle
 * @param [out] ot_vproc_grp_id : vproc grp的 id
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_get_grp_id_by_grp_hdl(ot_eis_handle grp_hdl, ot_vproc_grp_id *ot_vproc_grp_id);

/**
 * @brief 使用vproc grp id 获取 grp handle
 *
 * @param [in] ot_vproc_grp_id : vproc grp的 id
 * @param [out] grp_hdl : vproc grp的 handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_get_grp_hdl_by_grp_id(ot_vproc_grp_id ot_vproc_grp_id, ot_eis_handle *grp_hdl);

/**
 * @brief vproc grp属性设置，start前设置
 *
 * @param [in] grp_hdl :grp的handle
 * @param [in] grp_attr :grp的属性
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_set_attr(ot_eis_handle grp_hdl, ot_eis_vproc_grp_attr* grp_attr);
/**
 * @brief 获取vproc grp 属性
 *
 * @param [in] grp_hdl :grp的handle，
 * @param [out] grp_attr :grp的属性
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_get_attr(ot_eis_handle grp_hdl, ot_eis_vproc_grp_attr* grp_attr);
/**
 * @brief 启动vproc grp开始处理图像
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_start(ot_eis_handle grp_hdl);

/**
 * @brief 停止vproc grp，停止处理图像
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_stop(ot_eis_handle grp_hdl);
/**
 * @brief 绑定vproc grp 和chn
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [in] pipe_hdl :vproc pipe的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_attach_pipe(ot_eis_handle grp_hdl, ot_eis_handle pipe_hdl);

/**
 * @brief 绑定vproc out port 和chn
 *
 * @param [in] grp_hdl :grp的handle
 * @param [in] chn_hdl :vproc chn的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_attach_chn(ot_eis_handle grp_hdl, ot_eis_handle chn_hdl);

/**
 * @brief 将与chn绑定的 vproc grp与chn解除绑定
 *
 * @param [in] grp_hdl :grp的handle
 * @param [in] pipe_hdl :vproc pipe的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_detach_pipe(ot_eis_handle grp_hdl, ot_eis_handle pipe_hdl);

/**
 * @brief 将与chn绑定的 特定out port 与chn解除绑定
 *
 * @param [in] grp :chn的handle
 * @param [in] chn_hdl :vproc chn的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_detach_chn(ot_eis_handle grp_hdl, ot_eis_handle chn_hdl);

/**
 * @brief 将多个 pipe 与grp绑定
 *
 * @param [in] grp_hdl:grp的handle
 * @param [in] pipe_hdl_arr:pipe的handle数组
 * @param [in] pipe_num：pipe数量
 * @return ot_s32 0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_get_attched_pipes(ot_eis_handle grp_hdl, ot_eis_handle pipe_hdl_arr[], ot_s32* pipe_num);
/**
 * @brief 将多个 chn 与grp绑定
 *
 * @param grp_hdl :grp的handle
 * @param chn_hdl_arr :chn的handle数组
 * @param chn_num ：chn数量
 * @return ot_s32 0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_get_attched_chns(ot_eis_handle grp_hdl, ot_eis_handle chn_hdl_arr[], ot_s32* chn_num);


/**
 * @brief 设置grp 输入上的crop参数
 *
 * @param [in] grp_hdl :grp的handle
 * @param [in] param :crop 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_set_crop(const ot_eis_handle grp_hdl, const ot_eis_vproc_crop_param *param);

/**
 * @brief 获取grp 输入上的crop参数
 *
 * @param [in] grp_hdl :grp的handle
 * @param [out] param :crop 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_get_crop(const ot_eis_handle grp_hdl, ot_eis_vproc_crop_param *param);

/**
 * @brief 设置grp 输入上的3dnr参数
 *
 * @param [in] grp_hdl :grp的handle
 * @param [in] param :3dnr 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_set_3dnr(const ot_eis_handle grp_hdl, const ot_eis_3dnr_attr *attr);

/**
 * @brief 获取grp 输入上的3dnr参数
 *
 * @param [in] grp_hdl :grp的handle
 * @param [out] param :3dnr 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_get_3dnr(const ot_eis_handle grp_hdl, ot_eis_3dnr_attr *attr);

/**
 * @brief 设置grp输入上的gdc 公共参数
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [in] param :gdc 公共参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_set_gdc(const ot_eis_handle grp_hdl, const ot_eis_vproc_gdc_param *param);

/**
 * @brief 获取vproc grp输入上的gdc公共参数
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [out] param :gdc 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_get_gdc(const ot_eis_handle grp_hdl, ot_eis_vproc_gdc_param *param);

/**
 * @brief 获取vproc grp 输入上的fisheye参数
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [in] param :fisheye 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_set_fisheye(const ot_eis_handle grp_hdl, const ot_eis_vproc_fisheye_param *param);

/**
 * @brief 获取vproc grp 输入上的fisheye参数
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [out] param :输出fisheye 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_get_fisheye(const ot_eis_handle grp_hdl, ot_eis_vproc_fisheye_param *param);

/**
 * @brief 设置vproc grp 输入上的ldc参数
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [in] param :ldc 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_set_ldc(const ot_eis_handle grp_hdl, const ot_eis_vproc_ldc_param *param);

/**
 * @brief 获取vproc grp 输入上的ldc参数
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [in] param :ldc 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_get_ldc(const ot_eis_handle grp_hdl, ot_eis_vproc_ldc_param *param);

/**
 * @brief 设置vproc grp 输入上的pmf参数
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [in] param :pmf 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_set_pmf(const ot_eis_handle grp_hdl, const ot_eis_vproc_pmf_param *param);

/**
 * @brief 获取vproc grp 输入上的pmf参数
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [in] param :pmf 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_get_pmf(const ot_eis_handle grp_hdl, ot_eis_vproc_pmf_param *param);

/**
 * @brief 设置vproc grp 输入上的xylut参数
 *
 * @param [in] grp_hdl :vproc grp的handle
 * @param [in] param :xylut 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_grp_set_xylut(const ot_eis_handle grp_hdl, const ot_eis_vproc_xylut_param *param);


#ifdef __cplusplus
}
#endif

#endif