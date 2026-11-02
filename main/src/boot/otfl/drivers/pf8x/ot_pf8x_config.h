/*
 * Copyright (c) Oritek Technologies CO., Ltd. 2020-2023. All rights reserved.
 */
#ifndef OT_PF8X_CONFIG_H_
#define OT_PF8X_CONFIG_H_

#include "ot_type.h"

/* dvdd_core config begin */
#define VDD_CORE_MAX_VOLTAGE            (1050U) /* mV */
#define VDD_CORE_MIM_VOLTAGE            (650U)  /* mV */

#define VDD_CORE_SW1_MIN_VOLTAGE        (400)  /* mV */
#define VDD_CORE_SW1_MAX_VOLTAGE        (1800) /* mV */
#define VDD_CORE_SW1_VOLTAGE_SEL(x)     (((((x - VDD_CORE_SW1_MIN_VOLTAGE) * 100) % 625) == 0) ? \
                                          (((x - VDD_CORE_SW1_MIN_VOLTAGE) * 100) / 625) : \
                                          ((((x - VDD_CORE_SW1_MIN_VOLTAGE) * 100) / 625) + 1))

#define VDD_CORE_SW2_MIN_VOLTAGE        (400)  /* mV */
#define VDD_CORE_SW2_MAX_VOLTAGE        (1800) /* mV */
#define VDD_CORE_SW2_VOLTAGE_SEL(x)     (((((x - VDD_CORE_SW2_MIN_VOLTAGE) * 100) % 625) == 0) ? \
                                          (((x - VDD_CORE_SW2_MIN_VOLTAGE) * 100) / 625) : \
                                          ((((x - VDD_CORE_SW2_MIN_VOLTAGE) * 100) / 625) + 1))
/* dvdd_core config end */

/* dvdd_npu config begin */
#define VDD_NPU_MAX_VOLTAGE             (1050U) /* mV */
#define VDD_NPU_MIM_VOLTAGE             (650U) /* mV */

#define VDD_NPU_SW3_MIN_VOLTAGE         (400)  /* mV */
#define VDD_NPU_SW3_MAX_VOLTAGE         (1800) /* mV */
#define VDD_NPU_SW3_VOLTAGE_SEL(x)      (((((x - VDD_NPU_SW3_MIN_VOLTAGE) * 100) % 625) == 0) ? \
                                          (((x - VDD_NPU_SW3_MIN_VOLTAGE) * 100) / 625) : \
                                          ((((x - VDD_NPU_SW3_MIN_VOLTAGE) * 100) / 625) + 1))

#define VDD_NPU_SW4_MIN_VOLTAGE         (400)  /* mV */
#define VDD_NPU_SW4_MAX_VOLTAGE         (1800) /* mV */
#define VDD_NPU_SW4_VOLTAGE_SEL(x)      (((((x - VDD_NPU_SW4_MIN_VOLTAGE) * 100) % 625) == 0) ? \
                                          (((x - VDD_NPU_SW4_MIN_VOLTAGE) * 100) / 625) : \
                                          ((((x - VDD_NPU_SW4_MIN_VOLTAGE) * 100) / 625) + 1))
/* dvdd_npu config end */

/* dvdd_cpu config begin */
#define VDD_CPU_MAX_VOLTAGE             (1050U) /* mV */
#define VDD_CPU_MIM_VOLTAGE             (650U) /* mV */

#define VDD_CPU_SW6_MIN_VOLTAGE         (400)  /* mV */
#define VDD_CPU_SW6_MAX_VOLTAGE         (1800) /* mV */
#define VDD_CPU_SW6_VOLTAGE_SEL(x)      (((((x - VDD_CPU_SW6_MIN_VOLTAGE) * 100) % 625) == 0) ? \
                                          (((x - VDD_CPU_SW6_MIN_VOLTAGE) * 100) / 625) : \
                                          ((((x - VDD_CPU_SW6_MIN_VOLTAGE) * 100) / 625) + 1))

/* dvdd_cpu config end */

#endif /* OT_PF8X_CONFIG_H_ */
