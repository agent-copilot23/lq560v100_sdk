#ifndef __OT_EIS_VPROC_CHN_H__
#define __OT_EIS_VPROC_CHN_H__

#include "ot_eis_video.h"
#include "ot_eis_vproc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建vproc chn，每条通路使用vproc 需要调用一次
 * 
 * @param [in]  chn_id  : chn 在grp中id
 * @param [out] chn_hdl :chn的handle
 * @param [in] chn_attr :chn的属性 
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_create(const ot_vproc_chn_id chn_id, ot_eis_handle* chn_hdl, const ot_eis_vproc_chn_attr* chn_attr);
/**
 * @brief vproc chn销毁，销毁前必须调用stop和detach
 * 
 * @param [in] chn_hdl :
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_destroy(const ot_eis_handle chn_hdl);

/**
 * @brief 使用vproc chn handle 获取chn id
 * 
 * @param [in]  chn_id  : chn 在grp中id
 * @param [out] chn_hdl :chn的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_get_chn_id_by_chn_hdl(ot_eis_handle chn_hdl, ot_vproc_chn_id *chn_id);

/**
 * @brief 创建vproc chn，每条通路使用vproc 需要调用一次
 * 
 * @param [in] grp_id   : vproc grp 的handle
 * @param [in]  chn_id  : vproc chn 在grp中id
 * @param [out] chn_hdl : chn的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_get_chn_hdl_by_ids(ot_vproc_grp_id grp_id, ot_vproc_chn_id chn_id, ot_eis_handle *chn_hdl);


/**
 * @brief vproc chn属性设置，start前设置
 * 
 * @param [in] chn_hdl :chn的handle
 * @param [in] chn_attr :chn的属性
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_attr(const ot_eis_handle chn_hdl, const ot_eis_vproc_chn_attr* chn_attr);
/**
 * @brief 获取vproc chn 属性
 * 
 * @param [in] chn_hdl :chn的handle，
 * @param [in] chn_attr :chn的属性
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_attr(const ot_eis_handle chn_hdl, ot_eis_vproc_chn_attr* chn_attr);

/**
 * @brief 使能vproc输出out port的输出
 *
 * @param [in] out_hdl :vproc chn的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_enable(const ot_eis_handle chn_hdl);

/**
 * @brief 禁止vproc输出out port的输出
 *
 * @param [in] out_hdl :vproc chn的handle
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_disable(const ot_eis_handle chn_hdl);

/**
 * @brief 使能vproc输出out port的dump功能
 *
 * @param [in] out_hdl :vproc chn的handle
 * @param [in] en :dump使能标志, OT_TREE表示使能dump功能，OT_FALSE表示禁止dump功能
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_dump_enable(const ot_eis_handle chn_hdl, ot_bool en, ot_u32 depth);

/**
 * @brief 从vproc chn获取一帧
 * 
 * @param [in] chn_hdl :vproc chn的handle
 * @param [out] frame :帧信息，vproc填充
 * @param [in] timeout :阻塞时间 ms
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_acquire_frame(const ot_eis_handle chn_hdl, ot_eis_img_frame* frame, ot_s32 timeout);
/**
 * @brief 向vproc chn释放一帧，这帧必须是从vproc chn acquire的
 * 
 * @param [in] chn_hdl :vproc chn的handle
 * @param [in] frame :frame 帧信息
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_release_frame(const ot_eis_handle chn_hdl, const ot_eis_img_frame* frame);

/**
 * @brief 设置 chn上的mirror参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :mirror 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_mirror(const ot_eis_handle chn_hdl, const ot_eis_vproc_mirror_param *param);

/**
 * @brief 获取chn上的mirror参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :mirror 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_mirror(const ot_eis_handle chn_hdl, ot_eis_vproc_mirror_param *param);

/**
 * @brief 设置 chn上的flip参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :flip 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_flip(const ot_eis_handle chn_hdl, const ot_eis_vproc_flip_param *param);

/**
 * @brief 获取chn上的flip参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :flip 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_flip(const ot_eis_handle chn_hdl, ot_eis_vproc_flip_param *param);

/**
 * @brief 设置 chn上的crop参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :crop 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_crop(const ot_eis_handle chn_hdl, const ot_eis_vproc_crop_param *param);

/**
 * @brief 获取chn上的crop参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :crop 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_crop(const ot_eis_handle chn_hdl, ot_eis_vproc_crop_param *param);

/**
 * @brief 设置 chn上的scale参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :scale 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_scale_coef(const ot_eis_handle chn_hdl, const ot_eis_vproc_scale_coef_param *param);

/**
 * @brief 获取chn上的scale参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :scale 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_scale_coef(const ot_eis_handle chn_hdl, ot_eis_vproc_scale_coef_param *param);


/**
 * @brief 设置 chn上的mosaic参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :mosaic 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_mosaic(const ot_eis_handle chn_hdl, const ot_eis_vproc_mosaic_param *param);

/**
 * @brief 获取chn上的mosaic参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :mosaic 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_mosaic(const ot_eis_handle chn_hdl, ot_eis_vproc_mosaic_param *param);

/**
 * @brief 设置 chn上的line参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :line 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_line(const ot_eis_handle chn_hdl, const ot_eis_vproc_line_param *param);

/**
 * @brief 获取chn上的line参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :line 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_line(const ot_eis_handle chn_hdl, ot_eis_vproc_line_param *param);


/**
 * @brief 设置 chn上的cover参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :cover 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_cover(const ot_eis_handle chn_hdl, const ot_eis_vproc_cover_param *param);

/**
 * @brief 获取chn上的cover参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :cover 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_cover(const ot_eis_handle chn_hdl, ot_eis_vproc_cover_param *param);

/**
 * @brief 设置 chn上的rect参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :rect 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_rect(const ot_eis_handle chn_hdl, const ot_eis_vproc_rect_param *param);

/**
 * @brief 获取chn上的rect参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :rect 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_rect(const ot_eis_handle chn_hdl, ot_eis_vproc_rect_param *param);

/**
 * @brief 设置 chn上的osd参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :osd 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_osd(const ot_eis_handle chn_hdl, const ot_eis_vproc_osd_param *param);

/**
 * @brief 获取chn上的osd参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [out] param :osd参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_osd(const ot_eis_handle chn_hdl, ot_eis_vproc_osd_param *param);

/**
 * @brief 设置 chn上的luma参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :luma 参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_luma(const ot_eis_handle chn_hdl, const ot_eis_vproc_luma_param *param);

/**
 * @brief 获取chn上的luma参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [out] param :luma参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_luma(const ot_eis_handle chn_hdl, ot_eis_vproc_luma_param *param);

/**
 * @brief 获取chn上的luma 统计结果
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :luma 统计结果
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_luma_result(const ot_eis_handle chn_hdl, ot_eis_vproc_luma_results *param);

/**
 * @brief 设置 chn上的低延迟参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :低延迟参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_low_delay(const ot_eis_handle chn_hdl, const ot_eis_low_delay_info *param);

/**
 * @brief 获取chn上的低延迟参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [out] param :低延迟参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_low_delay(const ot_eis_handle chn_hdl, ot_eis_low_delay_info *param);

/**
 * @brief 设置 chn上的旋转
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [in] param :旋转参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_set_rotation(const ot_eis_handle chn_hdl, const ot_eis_vproc_chn_rotation *param);

/**
 * @brief 获取chn上的低延迟参数
 *
 * @param [in] chn_hdl :chn 的handle
 * @param [out] param :旋转参数
 * @return ot_s32  0: 成功，非0：错误码
 */
ot_s32 ot_eis_vproc_chn_get_rotation(const ot_eis_handle chn_hdl, ot_eis_vproc_chn_rotation *param);

#ifdef __cplusplus
}
#endif

#endif