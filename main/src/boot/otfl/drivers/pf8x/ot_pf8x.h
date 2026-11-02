/*
 * Copyright (c) Oritek Technologies CO., Ltd. 2020-2023. All rights reserved.
 */
#ifndef OT_PF8X_H_
#define OT_PF8X_H_

#define OT_PWR_VDD_CORE (0x1U)
#define OT_PWR_VDD_NPU  (0x2U)
#define OT_PWR_VDD_CPU  (0x3U)

/**
 * @brief  pf8x调节电压
 * @param  vid  电压id
 *         voltage  电压
 * @return 0: 成功, -1: 失败
 */
ot_s32 ot_pf8x_set_voltage(ot_u32 vid, ot_u32 voltage);

/**
 * @brief  pf8x初始化
 * @param  ot_void
 * @return 0: 成功, -1: 失败
 */
ot_s32 ot_pf8x_init(ot_void);

#endif /* OT_PF8X_H_ */
