/*
 * Copyright (c) Oritek Technologies CO., Ltd. 2020-2023. All rights reserved.
 */
#include "ot_type.h"
#include "hal_i2c.h"
#include "ot_pf8x.h"
#include "ot_pf8x_config.h"
#include "pmic_pf8x.h"

#define I2C_ID      2U
#define VDD_VOLTAGE_IS_VALID(x)		(((x) >= VDD_CORE_MIM_VOLTAGE) && ((x) <= VDD_CORE_MAX_VOLTAGE))
#define VDD_NPU_VOLTAGE_IS_VALID(x)	(((x) >= VDD_NPU_MIM_VOLTAGE) && ((x) <= VDD_NPU_MAX_VOLTAGE))
#define VDD_CPU_VOLTAGE_IS_VALID(x)	(((x) >= VDD_CPU_MIM_VOLTAGE) && ((x) <= VDD_CPU_MAX_VOLTAGE))

static pf8x_drv_data_t gs_pf8x_drv_data = {
		.i2cAddressOtp = 0U,
		.secureEnabled = OT_FALSE,
		.crcEnabled = OT_TRUE
	};

static ot_s32 ot_pf8x_set_vdd_core_voltage(ot_u32 voltage)
{
	ot_s32 ret;
	ot_u8 vdd_core = 0U;

	vdd_core = (ot_u8)(VDD_CORE_SW1_VOLTAGE_SEL(voltage));
	ret = (ot_s32)PF8X_SetBuckVoltage(&gs_pf8x_drv_data, pf8xRegBuckSW1, \
									   pf8xRegStateRun, vdd_core);
	if (ret != 0)
		return -1;
	ret = (ot_s32)PF8X_SetBuckModeConfig(&gs_pf8x_drv_data, pf8xRegBuckSW1, \
										  pf8xRegPdGroup1, pf8xRegModePwm, pf8xRegModeOff);
	if (ret != 0)
		return -1;

	vdd_core = (ot_u8)(VDD_CORE_SW2_VOLTAGE_SEL(voltage));
	ret = (ot_s32)PF8X_SetBuckVoltage(&gs_pf8x_drv_data, pf8xRegBuckSW2, \
									   pf8xRegStateRun, vdd_core);
	if (ret != 0)
		return -1;

	ret = (ot_s32)PF8X_SetBuckModeConfig(&gs_pf8x_drv_data, pf8xRegBuckSW2, \
										  pf8xRegPdGroup1, pf8xRegModePwm, pf8xRegModeOff);
	if (ret != 0)
		return -1;

	return 0;
}

static ot_s32 ot_pf8x_set_vdd_npu_voltage(ot_u32 voltage)
{
	ot_s32 ret;
	ot_u8 vdd_npu = 0U;

	vdd_npu = (ot_u8)(VDD_NPU_SW3_VOLTAGE_SEL(voltage));
	ret = (ot_s32)PF8X_SetBuckVoltage(&gs_pf8x_drv_data, pf8xRegBuckSW3, \
									   pf8xRegStateRun, vdd_npu);
	if (ret != 0)
		return -1;
	ret = (ot_s32)PF8X_SetBuckModeConfig(&gs_pf8x_drv_data, pf8xRegBuckSW3, \
										  pf8xRegPdGroup1, pf8xRegModePwm, pf8xRegModeOff);
	if (ret != 0)
		return -1;

	vdd_npu = (ot_u8)(VDD_NPU_SW4_VOLTAGE_SEL(voltage));
	ret = (ot_s32)PF8X_SetBuckVoltage(&gs_pf8x_drv_data, pf8xRegBuckSW4, \
									   pf8xRegStateRun, vdd_npu);
	if (ret != 0)
		return -1;

	ret = (ot_s32)PF8X_SetBuckModeConfig(&gs_pf8x_drv_data, pf8xRegBuckSW4, \
										  pf8xRegPdGroup1, pf8xRegModePwm, pf8xRegModeOff);
	if (ret != 0)
		return -1;

	return 0;
}

static ot_s32 ot_pf8x_set_vdd_cpu_voltage(ot_u32 voltage)
{
	ot_s32 ret;
	ot_u8 vdd_cpu = 0U;

	vdd_cpu = (ot_u8)(VDD_CPU_SW6_VOLTAGE_SEL(voltage));
	ret = (ot_s32)PF8X_SetBuckVoltage(&gs_pf8x_drv_data, pf8xRegBuckSW6, \
									   pf8xRegStateRun, vdd_cpu);
	if (ret != 0)
		return -1;
	ret = (ot_s32)PF8X_SetBuckModeConfig(&gs_pf8x_drv_data, pf8xRegBuckSW6, \
										  pf8xRegPdGroup1, pf8xRegModePwm, pf8xRegModeOff);
	if (ret != 0)
		return -1;

	return 0;
}

ot_s32 ot_pf8x_set_voltage(ot_u32 vid, ot_u32 voltage)
{
	ot_s32 ret;

	switch (vid) {
	case OT_PWR_VDD_CORE:
		if (!VDD_VOLTAGE_IS_VALID(voltage))
			return -1;
		ret = ot_pf8x_set_vdd_core_voltage(voltage);
		if (ret != 0)
			return -1;
		break;
	case OT_PWR_VDD_NPU:
		if (!VDD_NPU_VOLTAGE_IS_VALID(voltage))
			return -1;
		ret = ot_pf8x_set_vdd_npu_voltage(voltage);
		if (ret != 0)
			return -1;
		break;
	case OT_PWR_VDD_CPU:
		if (!VDD_CPU_VOLTAGE_IS_VALID(voltage))
			return -1;
		ret = ot_pf8x_set_vdd_cpu_voltage(voltage);
		if (ret != 0)
			return -1;
		break;
	default:
		return -1;
	}

	return 0;
}

ot_s32 ot_pf8x_init(ot_void)
{
	ot_s32 ret;

	ret = hal_i2c_init(I2C_ID);
	if (ret < 0)
		return ret;

	return 0;
}
