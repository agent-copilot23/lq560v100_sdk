#ifndef OT_EIS_ABNR_API_H
#define OT_EIS_ABNR_API_H

#include "ot_eis_type.h"
#include "ot_eis_abnr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/**
* \cond Chinese
*   \brief ABNR任务初始化
*   \param[in] pipe_hdl 数据流Handle
*   \param[in] abnr_params abnr外部控制参数结构体指针
*   \return OT_SUCCESS 加载成功
*   \note  在开启ABNR前调用
* \endcond
*/
ot_s32 ot_eis_abnr_init(ot_vi_pipe_handle pipe_hdl, ot_eis_abnr_params *abnr_params);


/**
* \cond Chinese
*   \brief ABNR任务初始化
*   \param[in] pipe_hdl 数据流Handle
*   \return OT_SUCCESS 加载成功
*   \note  在开启ABNR AUTO/PROC 线程前调用
* \endcond
*/
ot_s32 ot_eis_abnr_deinit(ot_vi_pipe_handle pipe_hdl);


/**
* \cond Chinese
*   \brief 打开ABNR
*   \param[in] pipe_hdl 数据流Handle
*   \return OT_SUCCESS 加载成功
*   \note
* \endcond
*/
ot_s32 ot_eis_abnr_enable(ot_vi_pipe_handle pipe_hdl);

/**
* \cond Chinese
*   \brief 关闭ABNR
*   \param[in] pipe_hdl 数据流Handle
*   \return OT_SUCCESS 加载成功
*   \note
* \endcond
*/
ot_s32 ot_eis_abnr_disable(ot_vi_pipe_handle pipe_hdl);


/**
* \cond Chinese
*   \brief 外部设置ABNR强度
*   \param[in] strength abnr强度[0, 1024]
*   \return OT_SUCCESS 加载成功
*   \note
* \endcond
*/
ot_s32 ot_eis_abnr_set_strength(ot_u16 strength);

/**
* \cond Chinese
*   \brief 外部设置场景iso, 用于内部自适应控制ABNR动态强度
*   \param[in] pipe_hdl 数据流Handle
*   \param[in] iso 当前iso值
*   \return OT_SUCCESS 加载成功
*   \note
* \endcond
*/
ot_s32 ot_eis_abnr_set_iso(ot_vi_pipe_handle pipe_hdl, ot_u64 iso);

/**
* \cond Chinese
*   \brief 外部设置场景iso, 用于内部自适应控制ABNR动态强度
*   \param[in] pipe_hdl 数据流Handle
*   \param[in] abnr_roi_point roi区域起始点结构体指针
*   \return OT_SUCCESS 加载成功
*   \note
* \endcond
*/
ot_s32 ot_eis_abnr_set_roi_rect(ot_vi_pipe_handle pipe_hdl, ot_eis_img_point *abnr_roi_point);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_ABNR_API_H */