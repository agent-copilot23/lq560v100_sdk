#ifndef OT_EIS_IQT_API_H
#define OT_EIS_IQT_API_H

#include "ot_eis_type.h"
#include "ot_eis_iqt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/**
* \cond Chinese
*   \brief IQT初始化
*   \return OT_SUCCESS 初始化成功
*   \note  在开启IQT create前调用
* \endcond
*/
ot_s32 ot_eis_iqt_init();
/**
* \cond Chinese
*   \brief IQT去初始化
*   \return OT_SUCCESS 加载成功
*   \note  iqt destory所有任务后才能调用
* \endcond
*/
ot_s32 ot_eis_iqt_deinit();

/**
* \cond Chinese
*   \brief IQT创建任务
*   \param[in] vi_pipe_hdl vi pipe handle
*   \param[in] vp_chn_hdl  vproc chn handle
*   \param[in] iqt_task_hdl 任务Handle
*   \param[in] iqt_params iqt外部控制参数结构体指针
*   \return OT_SUCCESS 加载成功
*   \note  在IQT init后，process前调用
* \endcond
*/
ot_s32 ot_eis_iqt_create(ot_vi_pipe_handle vi_pipe_hdl, ot_eis_handle vp_chn_hdl, ot_eis_handle *iqt_task_hdl, ot_eis_iqt_params *iqt_params);

/**
* \cond Chinese
*   \brief IQT销毁任务
*   \param[in] iqt_task_hdl 任务Handle
*   \return OT_SUCCESS 加载成功
*   \note  在IQT init后，process前调用
* \endcond
*/
ot_s32 ot_eis_iqt_destory(ot_eis_handle iqt_task_hdl);

/**
* \cond Chinese
*   \brief IQT创建任务
*   \param[in] iqt_task_hdl 任务Handle
*   \param[in] vp_frame frame结构体
*   \return OT_SUCCESS 加载成功
*   \note  处理结果会覆写vp_frame
* \endcond
*/
ot_s32 ot_eis_iqt_process(ot_eis_handle iqt_task_hdl, ot_eis_img_frame *vp_frame);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_IQT_API_H */