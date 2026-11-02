#ifndef __OT_AVP_NPU_COMMON_H__
#define __OT_AVP_NPU_COMMON_H__

#include <inttypes.h>
#include "ot_avp_tensor_common.h"
#include <string.h>
#include <stdbool.h>

#ifndef ot_avp_handle
typedef void* ot_avp_handle;
#endif

#define OT_AVP_NPU_EXTRA_CONFIG_LEN 128
typedef struct {
    ot_u8 key[OT_AVP_NPU_EXTRA_CONFIG_LEN];
    ot_u8 value[OT_AVP_NPU_EXTRA_CONFIG_LEN];
} extra_config;
/**
 * \cond Chinese
 *   \brief NPU的初始化配置
 * \endcond
 * \cond English
 *   \brief NPU initialize config
 * \endcond
*/
typedef struct {
    /**
    * \cond Chinese
    *   需要使用的ID号，如果要同时启动多个核，则ID号按位或运算，例如启动核0和核1，则core_ids=(1<<0)+(1<<1)=3
    * \endcond
    * \cond English
    *   the core id that need to be started. if want use multi core, core_ids should construct by bits add. such as core_ids=(1<<0)+(1<<1)
    * \endcond
    */
    ot_u32 core_ids;
    ot_bool dump_layer;
    ot_bool perf_mode;
    ot_u8 log_level;
    ot_u32 thread_num;
    ot_u32 extra_config_num;
    extra_config* extra_configs;
} ot_avp_npu_config;

typedef enum {
    OT_AVP_MDL_PRI_LOW    = 0,
    OT_AVP_MDL_PRI_MEDIUM = 1,
    OT_AVP_MDL_PRI_HIGH   = 2,
} ot_avp_npu_mdl_priority;

/**
 * \cond Chinese
 *   \brief NPU的模型配置
 * \endcond
 * \cond English
 *   \brief NPU model config
 * \endcond
*/
typedef struct {
    ot_avp_npu_mdl_priority priority_level;  // 模型执行的优先级，高优先级任务优先调度
    ot_bool priority_preemp;                 // 是否使能可抢占低优先级任务，默认开启。
} ot_avp_npu_mdl_config;

typedef struct ot_avp_npu_dataset ot_avp_npu_dataset;

#endif
