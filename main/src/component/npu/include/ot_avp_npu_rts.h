#ifndef __OT_AVP_NPU_RTS_H__
#define __OT_AVP_NPU_RTS_H__
#include "ot_avp_npu_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * \cond Chinese
 *   \name 环境初始化
 * \endcond
 */

//@{
/**
 * \cond Chinese
 *   \brief 初始化NPU资源，需要和ot_avp_npu_deinit配对使用
 *   \section init_example Init Example
 *   \snippet{lineno} samples/api_demo/offline_model_inference.c Init example
 * \endcond
 * \cond English
 *   \brief init npu context
 * \endcond
 */
ot_s32 ot_avp_npu_init(const ot_avp_npu_config *config);

/**
 * \cond Chinese
 *   \brief 释放NPU相关资源
 * \endcond
 * \cond English
 *   \brief Release resource used by npu
 * \endcond
 */
ot_s32 ot_avp_npu_deinit();
//@}

/**
 * \cond Chinese
 *   \name 模型加载
 * \endcond
 */
//@{
/**
* \cond Chinese
*   \brief 从内存加载模型
*   \param[in] path 离线模型路径
*   \param[out] handle 模型Handle，在后续对模型的操作都会用到
*   \return OT_SUCCESS 加载成功
*   \return OT_INVALID_MODEL 加载失败
*   \warning 模型文件必须可读，并且是合法的离线模型
*   \note 必须在ot_avp_npu_init之后调用
* \endcond
* \cond English
*   \brief Load model from file
*   \param[in] path Offline model path
*   \param[out] handle The handle for the model
*   \return OT_SUCCESS on load success.
*   \return OT_INVALID_MODEL on invalid model
*   \warning Model must be offline model and readable
*   \note Before load ot_avp_npu_init must called.
* \endcond
* \see ot_avp_npu_init
*/
ot_s32 ot_avp_npu_load_model(const char* path, ot_avp_handle *handle);

/**
* \cond Chinese
*   \brief 从内存加载模型，并使用用户传入的临时内存，用于模型推理时临时使用，调用接口获取
*   \param[in] model_buf 装载离线模型的内存
*   \param[in] model_len 离线模型内存的大小
*   \param[out] handle 模型Handle
*   \return OT_SUCCESS 加载成功
*   \return OT_INVALID_MODEL 加载失败
*   \warning 模型文件必须可读，并且是合法的离线模型
*   \note 必须在ot_avp_npu_init之后调用
* \endcond
*/
ot_s32 ot_avp_npu_load_model_from_mem(const ot_u8 *model_buf, ot_u32 model_len, ot_avp_handle *handle);

/**
 * \cond Chinese
 *   \brief 模型卸载，模型相关资源回收
 *   \param[in] handle 模型Handle
 * \endcond
 *
 */
ot_s32 ot_avp_npu_unload_model(ot_avp_handle handle);

//@}

/**
 * \cond Chinese
 *   \brief 将模型绑定到某一个或多个核上执行
 *   \param[in] handle 模型Handle
 *   \param[in] core_ids 绑定的核编号列表，按bit组装
 *   \return OT_SUCCESS 绑定成功
 *   \return OT_FAIL 绑定失败
 * \endcond
 */
ot_s32 ot_avp_npu_bind_model_to_core(ot_avp_handle handle, ot_u32 core_ids);

/**
 * \cond Chinese
 *   \name 模型的执行
 * \endcond
 * \cond English
 *   \name Model Execute
 * \endcond
 */
//@{

/**
 * \cond Chinese
 *   \brief 设置离线模型需要的配置参数
 *   \param[in] handle 模型handle
 *   \param[in] config 模型任务优先级执行所需的配置参数
 *   \return OT_SUCCESS 成功配置模型参数
 * \endcond
*/
ot_s32 ot_avp_npu_set_model_config(ot_avp_handle handle, ot_avp_npu_mdl_config* config);

/**
 * \cond Chinese
 *   \brief 设置离线模型需要的临时内存
 *   \param[in] handle 模型handle
 *   \param[in] runtime_buf 模型执行中间过程临时存储，大小使用ot_avp_npu_get_runtime_buffer_size获取
 *   \param[in] runtime_buf_len 上一个参数runtime_buf实际的大小，不能小于所需内存
 *   \return OT_SUCCESS 成功获取到内存大小
 * \endcond
*/
ot_s32 ot_avp_npu_set_runtime_buffer(ot_avp_handle handle, ot_u8 *runtime_buf, ot_u32 runtime_buf_size);

/**
 * \cond Chinese
 *   \brief 模型同步执行
 *   \param[in] handle 模型Handle
 *   \param[in] inputs 模型的输入，datasets中的个数需要和模型的输入个数一致
 *   \param[out] outputs 模型的输出，datasets中的个数需要和模型的输出个数一致
 *   \return OT_SUCCESS 模型执行成功
 * \endcond
 */
ot_s32 ot_avp_npu_execute(ot_avp_handle handle, ot_avp_npu_dataset *inputs, ot_avp_npu_dataset *outputs);

/**
 * \cond Chinese
 *   \brief 模型异步执行
 *   \param[in] handle 模型Handle
 *   \param[in] inputs 模型的输入，datasets中的个数需要和模型的输入个数一致
 *   \param[in] outputs 模型的输出，datasets中的个数需要和模型的输出个数一致
 *   \return OT_SUCCESS 异步任务添加成功
 * \endcond
 */
ot_s32 ot_avp_npu_trigger(ot_avp_handle handle, ot_avp_npu_dataset *inputs, ot_avp_npu_dataset *outputs);


/**
 * \cond Chinese
 *  \brief 等待模型异步执行结束
 *  \param[in] handle 模型Handle
 *  \param[in] timeout 等待的超时时间，当timeout=-1时永久等待，timeout=0时，立即返回，其它值时，为超时时间，单位为毫秒
 *  \return OT_SUCCESS 成功等到模型执行结束
 *
 * \endcond
 */
ot_s32 ot_avp_npu_wait(ot_avp_handle handle, ot_s32 timeout);


/**
 * \cond Chinese
 *   \brief 取消模型执行任务
 *   \param[in] handle 模型Handle
 *   \return OT_SUCCESS 成功取消
 * \endcond
 */
ot_s32 ot_avp_npu_cancel(ot_avp_handle handle);
// @}


/**
 * \cond Chinese
 *   \name 模型信息获取
 * \endcond
 */

//@{

/**
 * \cond Chinese
 *   \brief 获取离线模型需要的临时内存的大小
 *   \param[in] handle 模型Handle
 *   \param[out] runtime_buf_mem_size runtime_buf所需内存大小，单位byte
 *   \return OT_SUCCESS 成功获取到内存大小
 * \endcond
*/
ot_s32 ot_avp_npu_get_runtime_buffer_size(ot_avp_handle handle, ot_u32 *runtime_buf_size);

/**
 * \cond Chinese
 *   \brief 获取模型输入个数
 *   \param[in] handle 模型Handle
 *   \param[out] input_num 模型输入节点个数
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_input_num(ot_avp_handle handle, ot_u32 *input_num);

/**
 * \cond Chinese
 *   \brief 从输入名称获取index
 *   \param[in] handle 模型Handle
 *   \param[in] name 输入名称
 *   \param[in] name_len 输入名称的长度
 *   \param[out] index 获取输入的索引号
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_input_index_by_name(ot_avp_handle handle, const char name[], ot_u32 name_len, ot_u32 *index);

/**
 * \cond Chinese
 *   \brief 获取输入的shape
 *   \param[in] handle 模型Handle
 *   \param[in] index 需要获取输入的索引号
 *   \param[out] shape ot_avp_npu_shape信息
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_input_shape_by_index(ot_avp_handle handle, ot_u32 index, ot_avp_npu_shape *shape);

/**
 * \cond Chinese
 *   \brief 获取输入data format
 *   \param[in] handle 模型Handle
 *   \param[in] index 需要获取输入的索引号
 *   \param[out] format ot_avp_npu_dformat信息
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_input_format_by_index(ot_avp_handle handle, ot_u32 index, ot_avp_npu_dformat *format);

/**
 * \cond Chinese
 *   \brief 获取输入data type
 *   \param[in] handle 模型Handle
 *   \param[in] index 需要获取输入的索引号
 *   \param[out] dtype ot_avp_npu_dtype信息
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_input_dtype_by_index(ot_avp_handle handle, ot_u32 index, ot_avp_data_type *dtype);

/**
 * \cond Chinese
 *   \brief 获取模型输出个数
 *   \param[in] handle 模型Handle
 *   \param[out] output_num 模型输出节点个数
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_output_num(ot_avp_handle handle, ot_u32 *output_num);

/**
 * \cond Chinese
 *   \brief 获取输出的名称
 *   \param[in] handle 模型Handle
 *   \param[in] name 输出名称
 *   \param[in] name_len 输出名称长度
 *   \param[out] index 获取输出的索引号
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_output_index_by_name(ot_avp_handle handle, const char name[], ot_u32 name_len, ot_u32 *index);

/**
 * \cond Chinese
 *   \brief 获取输出的名字
 *   \param[in] handle 模型Handle
 *   \param[in] index 输出的索引号
 *   \param[out] name 输出名称
 *   \param[out] name_len 输出名称长度
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_output_name_by_index(ot_avp_handle handle, ot_u32 index, char name[], ot_u32 name_len);

/**
 * \cond Chinese
 *   \brief 获取输出的shape
 *   \param[in] handle 模型Handle
 *   \param[in] index 需要获取输出的索引号
 *   \param[out] shape ot_avp_npu_shape信息
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_output_shape_by_index(ot_avp_handle handle, ot_u32 index, ot_avp_npu_shape *shape);

/**
 * \cond Chinese
 *   \brief 获取输出data format
 *   \param[in] handle 模型Handle
 *   \param[in] index 需要获取输出的索引号
 *   \param[out] format ot_avp_npu_dformat信息
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_output_format_by_index(ot_avp_handle handle, ot_u32 index, ot_avp_npu_dformat *format);

/**
 * \cond Chinese
 *   \brief 获取输出data type
 *   \param[in] handle 模型Handle
 *   \param[in] index 需要获取输出的索引号
 *   \param[out] dtype ot_avp_npu_dtype信息
 *   \return OT_SUCCESS 信息成功获取到
 * \endcond
 */
ot_s32 ot_avp_npu_get_output_dtype_by_index(ot_avp_handle handle, ot_u32 index, ot_avp_data_type *dtype);
//@}

/**
* \cond Chinese
*   \name 内存相关
* \endcond
*/
//@{

/**
* \cond Chinese
*   \brief 创建输入输出内存
* \endcond
*/
ot_avp_npu_dataset* ot_avp_npu_create_dataset();

/**
* \cond Chinese
*   \brief 将对应的buffer加入到dataset中来
* \endcond
* \section dataset_example Dataset Example
* \snippet{lineno} ./samples/api_demo/offline_model_inference.c Dataset example
*/
ot_s32 ot_avp_npu_add_buffer(ot_avp_npu_dataset *dataset, ot_u8 *virt_addr, ot_u64 phy_addr, ot_u32 buffer_len, ot_u32 buffer_stride);

/**
* \cond Chinese
*   \brief 更新第index个buffer
* \endcond
*/
ot_s32 ot_avp_npu_set_buffer(ot_avp_npu_dataset *dataset, ot_u32 index, ot_u8 *virt_addr, ot_u64 phy_addr, ot_u32 buffer_len, ot_u32 buffer_stride);

/**
* \cond Chinese
*   \brief 销毁dataset
* \endcond
*/
ot_s32 ot_avp_npu_destroy_dataset(ot_avp_npu_dataset *dataset);

/**
* \cond Chinese
*   \brief 获取对应data_type的size
* \endcond
*/
ot_s32 ot_avp_npu_data_type_size(ot_avp_data_type data_type);

/**
* \cond Chinese
*   \brief 获取第index个输入的stride
* \endcond
*/
ot_u32 ot_avp_npu_get_input_default_stride(ot_avp_handle handle, ot_u32 index);

/**
* \cond Chinese
*   \brief 获取第index个输入的size
* \endcond
*/
ot_u32 ot_avp_npu_get_input_size_by_index(ot_avp_handle handle, ot_u32 index);

/**
* \cond Chinese
*   \brief 获取第index个输出的stride
* \endcond
*/
ot_u32 ot_avp_npu_get_output_default_stride(ot_avp_handle handle, ot_u32 index);

/**
* \cond Chinese
*   \brief 获取第index个输出的size
* \endcond
*/
ot_u32 ot_avp_npu_get_output_size_by_index(ot_avp_handle handle, ot_u32 index);

/**
* \cond Chinese
*   \brief 申请size大小的内存
* \endcond
*/
ot_s32 ot_avp_npu_malloc(ot_void **ptr, ot_u64 size);

/**
* \cond Chinese
*   \brief 释放内存
* \endcond
*/
ot_s32 ot_avp_npu_free(ot_void *ptr);

//@}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
