/*
 * Copyright (c) 2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @file pmic_pf8x.h
 * @brief Definitions of I2C communication logic for NXP PMIC PF8x.
 *
 * @author nxa22158
 * @version 1.0
 * @date 21-August-2019
 * @copyright Copyright (c) 2019 NXP
 */

#ifndef PMIC_PF8X_H_
#define PMIC_PF8X_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "pmic_pf8x_common.h"
#include "ot_type.h"

#if PF8X_DEVICE == PF8X_DEVICE_PF81
#include "pmic_pf81_map.h"
#else
#include "pmic_pf82_map.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/** @defgroup EnumsDefs Enums definition
 * @{ */
/** @brief Buck regulator. */
typedef enum
{
    pf8xRegBuckSW1 = 0U,     /**< SW1 buck. */
    pf8xRegBuckSW2 = 1U,     /**< SW2 buck. */
    pf8xRegBuckSW3 = 2U,     /**< SW3 buck. */
    pf8xRegBuckSW4 = 3U,     /**< SW4 buck. */
    pf8xRegBuckSW5 = 4U,     /**< SW5 buck. */
    pf8xRegBuckSW6 = 5U,     /**< SW6 buck. */
    pf8xRegBuckSW7 = 6U      /**< SW7 buck. */
} pf8x_reg_buck_t;

/** @brief LDO regulator. */
typedef enum
{
    pf8xRegLdo1 = 0U,        /**< LDO1. */
    pf8xRegLdo2 = 1U,        /**< LDO2. */
    pf8xRegLdo3 = 2U,        /**< LDO3. */
    pf8xRegLdo4 = 3U         /**< LDO4. */
} pf8x_reg_ldo_t;

/** @brief Regulator state. */
typedef enum
{
    pf8xRegStateRun     = 0U, /**< Run state. */
    pf8xRegStateStandby = 1U  /**< Standby state. */
} pf8x_reg_state_t;

/** @brief Regulator mode. */
typedef enum
{
    pf8xRegModeOff = 0U,     /**< OFF mode. */
    pf8xRegModePwm = 1U,     /**< PWM mode. */
    pf8xRegModePfm = 2U,     /**< PFM mode. */
    pf8xRegModeAutoskip = 3U /**< Autoskip mode. */
} pf8x_reg_mode_t;

/** @brief Regulator power down group. */
typedef enum
{
    pf8xRegPdGroup4 = 0U,    /**< Power down group 4. */
    pf8xRegPdGroup3 = 1U,    /**< Power down group 3. */
    pf8xRegPdGroup2 = 2U,    /**< Power down group 2. */
    pf8xRegPdGroup1 = 3U     /**< Power down group 1. */
} pf8x_reg_powerdown_t;

/** @brief Pin/regulator power sequence. */
typedef enum
{
    pf8xPinRegResetbMcu = PF8X_RESETBMCU_PWRUP_ADDR, /**< INT_STATUS1 interrupts. */
    pf8xPinRegPgood = PF8X_PGOOD_PWRUP_ADDR,         /**< INT_STATUS2 interrupts. */
    pf8xPinRegSw1 = PF8X_SW1_PWRUP_ADDR,             /**< SW_MODE interrupts. */
    pf8xPinRegSw2 = PF8X_SW2_PWRUP_ADDR,             /**< SW_ILIM interrupts. */
    pf8xPinRegSw3 = PF8X_SW3_PWRUP_ADDR,             /**< LDO_ILIM interrupts. */
    pf8xPinRegSw4 = PF8X_SW4_PWRUP_ADDR,             /**< SW_UV interrupts. */
    pf8xPinRegSw5 = PF8X_SW5_PWRUP_ADDR,             /**< SW_OV interrupts. */
    pf8xPinRegSw6 = PF8X_SW6_PWRUP_ADDR,             /**< LDO_UV interrupts. */
    pf8xPinRegSw7 = PF8X_SW7_PWRUP_ADDR,             /**< LDO_OV interrupts. */
    pf8xPinRegLdo1 = PF8X_LDO1_PWRUP_ADDR,           /**< LDO_UV interrupts. */
    pf8xPinRegLdo2 = PF8X_LDO2_PWRUP_ADDR,           /**< LDO_OV interrupts. */
    pf8xPinRegLdo3 = PF8X_LDO3_PWRUP_ADDR,           /**< PWRON interrupts. */
    pf8xPinRegLdo4 = PF8X_LDO4_PWRUP_ADDR            /**< SYS_INT interrupts. */
} px8x_pin_reg_t;

/** @brief Sync frequency range. */
typedef enum
{
    /**< 2000 kHz to 3000 kHz. */
    pf8xFsyncRangeHigh = PF8X_F_FSYNC_RANGE_2000_KHZ_3000_KHZ,
    /**< 333 kHz to 500 kHz. */
    pf8xFsyncRangeLow = PF8X_F_FSYNC_RANGE_333_KHZ_500_KHZ
} pf8x_fsync_range_t;

/** @brief FSS range. */
typedef enum
{
    pf8xFssRangeNarrow = PF8X_F_FSS_RANGE_5, /**< +/- 5% range. */
    pf8xFssRangeWide = PF8X_F_FSS_RANGE_10   /**< +/- 10% range. */
} pf8x_fss_range_t;

/** @brief VIN_OVLO debounce. */
typedef enum
{
    pf8xOvloDbnc10us = PF8X_F_VIN_OVLO_DBNC_10_US,    /**< Debounce 10 us. */
    pf8xOvloDbnc100us = PF8X_F_VIN_OVLO_DBNC_100_US,  /**< Debounce 100 us. */
    pf8xOvloDbnc1000us = PF8X_F_VIN_OVLO_DBNC_1000_US /**< Debounce 1000 us. */
} pf8x_ovlo_dbnc_t;

/** @brief Interrupt group. */
typedef enum
{
    pf8xIntGroupStatus1 = PF8X_INT_STATUS1_ADDR,  /**< INT_STATUS1 interrupts. */
    pf8xIntGroupStatus2 = PF8X_INT_STATUS2_ADDR,  /**< INT_STATUS2 interrupts. */
    pf8xIntGroupSwMode = PF8X_SW_MODE_INT_ADDR,   /**< SW_MODE interrupts. */
    pf8xIntGroupSwIlim = PF8X_SW_ILIM_INT_ADDR,   /**< SW_ILIM interrupts. */
    pf8xIntGroupLdoIlim = PF8X_LDO_ILIM_INT_ADDR, /**< LDO_ILIM interrupts. */
    pf8xIntGroupSwUv = PF8X_SW_UV_INT_ADDR,       /**< SW_UV interrupts. */
    pf8xIntGroupSwOv = PF8X_SW_OV_INT_ADDR,       /**< SW_OV interrupts. */
    pf8xIntGroupLdoUv = PF8X_LDO_UV_INT_ADDR,     /**< LDO_UV interrupts. */
    pf8xIntGroupLdoOv = PF8X_LDO_OV_INT_ADDR,     /**< LDO_OV interrupts. */
    pf8xIntGroupPwron = PF8X_PWRON_INT_ADDR,      /**< PWRON interrupts. */
    pf8xIntGroupSysInt = PF8X_SYS_INT_ADDR        /**< SYS_INT interrupts. */
} pf8x_int_group_t;
/** @} */

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes of extern functions
 ******************************************************************************/
/** @} */
/*******************************************************************************
 * API
 ******************************************************************************/
/** @addtogroup API
 * @{ */
/** @brief Performs a read from a single PF8x register.
 *
 * Reads a single register based on provided address.
 *
 * @param [in]  drvData Driver run-time data.
 * @param [in]  address Register address.
 * @param [out] rxData  Response from PMIC.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_ReadRegister(pf8x_drv_data_t* drvData, ot_u8 address,
        ot_u8* rxData);

/** @brief Sends write command to the PF8x.
 *
 * Writes a single register based on provided address and data to write.
 *
 * @param [in] drvData   Driver run-time data.
 * @param [in] address   Register address.
 * @param [in] writeData Register write value.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_WriteRegister(pf8x_drv_data_t* drvData, ot_u8 address,
        ot_u8 writeData);

/** @brief Performs a write to a single PF82 secured register.
 *
 * Writes a single secured register based on provided address and data to write.
 * Reads generated random code from RANDOM_GEN register and writes it back to
 * RANDOM_CHK register to complete the secure write transfer.
 *
 * Note: Only available on PF82xx devices.
 *
 * @param [in] drvData   Driver run-time data.
 * @param [in] address   Register address.
 * @param [in] writeData Register write value.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_WriteRegisterSecured(pf8x_drv_data_t* drvData,
		    ot_u8 address, ot_u8 writeData);

/** @brief Reads device identification information.
 *
 * The function gets the following IDs:
 * - device family and device IDs from DEVICE_ID register
 * - full layer and metal layer revision IDs from REV_ID register
 * - EMREV and program ID from EMREV and PROG_ID registers
 *
 * If one of the pointers is OT_NULL, none of the IDs within the same register are read
 * (for example if deviceId is OT_NULL, both device ID and device family ID are ignored).
 *
 * @param [in]  drvData      Driver run-time data.
 * @param [out] deviceFamily Device family ID.
 * @param [out] deviceId     Device ID.
 * @param [out] fullRev      Full layer revision.
 * @param [out] metalRev     Metal layer revision.
 * @param [out] emRev        EMREV.
 * @param [out] progId       Program ID.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_GetId(pf8x_drv_data_t* drvData, ot_u8* deviceFamily, ot_u8* deviceId,
        ot_u8* fullRev, ot_u8* metalRev, ot_u8* emRev, ot_u16* progId);

/** @brief Sets power up/down configuration.
 *
 * @param [in] drvData Driver run-time data.
 * @param [in] pinReg  Pin or regulator power sequence.
 * @param [in] seqSlot Sequence slot (0 = off, 1 = SLOT0, 255 = SLOT254).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetPowerSequence(pf8x_drv_data_t* drvData,
        px8x_pin_reg_t pinReg, ot_u8 seqSlot);

/** @brief Sets buck regulator configuration.
 *
 * @param [in] drvData     Driver run-time data.
 * @param [in] buckSwNum   Buck regulator number.
 * @param [in] uvBypass    true = regulator bypasses undervoltage event.
 * @param [in] ovBypass    true = regulator bypasses overvoltage event.
 * @param [in] ilimBypass  true = regulator bypasses ILIM event.
 * @param [in] uvState     true = regulator remains in previous state.
 * @param [in] ovState     true = regulator remains in previous state.
 * @param [in] ilimState   true = regulator remains in previous state.
 * @param [in] wdBypass    true = regulator bypasses watchdog event.
 * @param [in] pgoodEnable true = regulator affects PGOOD pin.
 * @param [in] fltRen      true = return to previous state after fault is cleared.
 * @param [in] vttEnable   true = VTT mode enabled (only applicable for SW6).
 * @param [in] dvsRamp     true = 12.5 mV/us, false = 6.25 mV/us (not applicable for SW7).
 * @param [in] ilim        Current limit (use PF8X_F_SW1ILIM_ macros corresponding
 *                         to actual regulator number).
 * @param [in] phase       Phase shift (use PF8X_F_SW1PHASE_ macros corresponding
 *                         to actual regulator number).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetBuckConfig(pf8x_drv_data_t* drvData, pf8x_reg_buck_t buckSwNum,
        ot_bool uvBypass, ot_bool ovBypass, ot_bool ilimBypass, ot_bool uvState, ot_bool ovState,
        ot_bool ilimState, ot_bool wdBypass, ot_bool pgoodEnable, ot_bool fltRen, ot_bool vttEnable,
        ot_bool dvsRamp, ot_u8 ilim, ot_u8 phase);

/** @brief Sets buck regulator mode and power down group.
 *
 * @param [in] drvData   Driver run-time data.
 * @param [in] buckSwNum Buck regulator number.
 * @param [in] pdGroup   Power down group.
 * @param [in] runMode   Mode in run state.
 * @param [in] stbyMode  Mode in standby state.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetBuckModeConfig(pf8x_drv_data_t* drvData, pf8x_reg_buck_t buckSwNum,
        pf8x_reg_powerdown_t pdGroup, pf8x_reg_mode_t runMode, pf8x_reg_mode_t stbyMode);

/** @brief Sets buck regulator voltage.
 *
 * The function sets buck regulator voltages separately for run and standby state.
 *
 * @param [in] drvData   Driver run-time data.
 * @param [in] buckSwNum Buck regulator number.
 * @param [in] regState  Regulator state.
 * @param [in] voltage   Voltage (use PF8X_F_VSW1_RUN_ or PF8X_F_VSW1_STANDBY_ macros
 *                       corresponding to actual regulator number and regulator state).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetBuckVoltage(pf8x_drv_data_t* drvData, pf8x_reg_buck_t buckSwNum,
        pf8x_reg_state_t regState, ot_u8 voltage);

/** @brief Sets LDO regulator configuration.
 *
 * @param [in] drvData     Driver run-time data.
 * @param [in] ldoNum      LDO regulator number.
 * @param [in] uvBypass    true = regulator bypasses undervoltage event.
 * @param [in] ovBypass    true = regulator bypasses overvoltage event.
 * @param [in] ilimBypass  true = regulator bypasses ILIM event.
 * @param [in] uvState     true = regulator remains in previous state.
 * @param [in] ovState     true = regulator remains in previous state.
 * @param [in] ilimState   true = regulator remains in previous state.
 * @param [in] wdBypass    true = regulator bypasses watchdog event.
 * @param [in] pgoodEnable true = regulator affects PGOOD pin.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetLdoConfig(pf8x_drv_data_t* drvData, pf8x_reg_ldo_t ldoNum,
        ot_bool uvBypass, ot_bool ovBypass, ot_bool ilimBypass, ot_bool uvState, ot_bool ovState,
        ot_bool ilimState, ot_bool wdBypass, ot_bool pgoodEnable);

/** @brief Sets LDO regulator mode and power down group.
 *
 * @param [in] drvData     Driver run-time data.
 * @param [in] ldoNum      LDO regulator number.
 * @param [in] fltRen      true = return to previous state after fault is cleared.
 * @param [in] pdGroup     Power down group.
 * @param [in] runEnabled  true = enabled in run state.
 * @param [in] stbyEnabled true = enabled in standby state.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetLdoModeConfig(pf8x_drv_data_t* drvData, pf8x_reg_ldo_t ldoNum,
        ot_bool fltRen, pf8x_reg_powerdown_t pdGroup, ot_bool runEnabled, ot_bool stbyEnabled);

/** @brief Sets LDO regulator voltage.
 *
 * The function sets LDO regulator voltages separately for run and standby state.
 *
 * @param [in] drvData  Driver run-time data.
 * @param [in] ldoNum   LDO regulator number.
 * @param [in] regState Regulator state.
 * @param [in] voltage  Voltage (use PF8X_F_VLDO1_RUN or PF8X_F_VLDO1_STANDBY_ macros
 *                      corresponding to actual regulator number and regulator state).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetLdoVoltage(pf8x_drv_data_t* drvData, pf8x_reg_ldo_t ldoNum,
        pf8x_reg_state_t regState, ot_u8 voltage);

/** @brief Set frequency configuration of the PMIC.
 *
 * @param [in] drvData       Driver run-time data.
 * @param [in] syncoutEnable true = SYNCOUT enabled.
 * @param [in] fsyncRange    Input frequency range of SYNCIN pin.
 * @param [in] fssEnable     true = frequency spread-spectrum enabled.
 * @param [in] fssRange      Clock frequency range.
 * @param [in] clkFreq       Switching regulators frequency (use PF8X_F_CLK_FREQ_ macros).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetFreqConfig(pf8x_drv_data_t* drvData, ot_bool syncoutEnable,
        pf8x_fsync_range_t fsyncRange, ot_bool fssEnable, pf8x_fss_range_t fssRange, ot_u8 clkFreq);

/** @brief Sets input voltage monitor configuration.
 *
 * @param [in] drvData      Driver run-time data.
 * @param [in] ovloEnable   true = OVLO enabled.
 * @param [in] ovloShutdown true = OVLO initiates power down.
 * @param [in] ovloDebounce OVLO debounce time.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetVinOvloConfig(pf8x_drv_data_t* drvData, ot_bool ovloEnable,
        ot_bool ovloShutdown, pf8x_ovlo_dbnc_t ovloDebounce);

/** @brief Sets voltage monitor configuration.
 *
 * @param [in] drvData    Driver run-time data.
 * @param [in] swMonMask  Voltage monitors for SW regulators.
 * @param [in] ldoMonMask Voltage monitors for LDO regulators.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetVoltMonitorConfig(pf8x_drv_data_t* drvData,
        ot_u8 swMonMask, ot_u8 ldoMonMask);

/** @brief Sets temperature monitor configuration.
 *
 * @param [in] drvData       Driver run-time data.
 * @param [in] tempMonEnable true = enable temperature monitor circuit.
 * @param [in] tempMonAon    false = sampling mode, true = always on mode.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetTempMonitorConfig(pf8x_drv_data_t* drvData,
        ot_bool tempMonEnable, ot_bool tempMonAon);

/** @brief Reads interrupt flags from selected interrupt group.
 *
 * @param [in]  drvData   Driver run-time data.
 * @param [in]  intGroup  Interrupt group.
 * @param [out] intStatus Interrupt flags.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_GetInterrupts(pf8x_drv_data_t* drvData, pf8x_int_group_t intGroup, ot_u8* intStatus);

/** @brief Clears selected interrupt flags from selected interrupt group.
 *
 * @param [in] drvData  Driver run-time data.
 * @param [in] intGroup Interrupt group.
 * @param [in] intMask  Interrupts to clear.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_ClearInterrupts(pf8x_drv_data_t* drvData, pf8x_int_group_t intGroup, ot_u8 intMask);

/** @brief Sets which interrupts pull the INTB pin.
 *
 * @param [in] drvData  Driver run-time data.
 * @param [in] intGroup Interrupt group.
 * @param [in] intMask  Interrupt mask.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_MaskInterrupts(pf8x_drv_data_t* drvData, pf8x_int_group_t intGroup, ot_u8 intMask);

/** @brief Reads the actual status of the signal triggering interrupt.
 *
 * @param [in]  drvData  Driver run-time data.
 * @param [in]  intGroup Interrupt group.
 * @param [out] intSense Status of interrupt signals.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SenseInterrupts(pf8x_drv_data_t* drvData, pf8x_int_group_t intGroup, ot_u8* intSense);

/** @brief Reads hardfault status.
 *
 * @param [in]  drvData Driver run-time data.
 * @param [out] hfFlags Hardfault flags.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_GetHardfaultFlags(pf8x_drv_data_t* drvData, ot_u8* hfFlags);

/** @brief Clears hardfault status flags.
 *
 * @param [in] drvData   Driver run-time data.
 * @param [in] flagsMask Flags to clear.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_ClearHardfaultFlags(pf8x_drv_data_t* drvData, ot_u8 flagsMask);

/** @brief Sets FSOB configuration.
 *
 * @param [in] drvData   Driver run-time data.
 * @param [in] softfault true = FSOB asserted when regulator fault is present.
 * @param [in] wdi       true = FSOB asserted when watchdog reset occurs due to WDI event.
 * @param [in] wdc       true = FSOB asserted when watchdog reset occurs due to WD counter fault.
 * @param [in] hardfault true = FSOB asserted when hardfault shutdown occurs.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetFSOBConfig(pf8x_drv_data_t* drvData,
        ot_bool softfault, ot_bool wdi, ot_bool wdc, ot_bool hardfault);

/** @brief Reads FSOB status.
 *
 * @param [in]  drvData Driver run-time data.
 * @param [out] fsFlags FSOB flags.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_GetFSOBFlags(pf8x_drv_data_t* drvData, ot_u8* fsFlags);

/** @brief Clears FSOB status flags.
 *
 * @param [in] drvData     Driver run-time data.
 * @param [in] flagsMask   Flags to clear.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_ClearFSOBFlags(pf8x_drv_data_t* drvData, ot_u8 flagsMask);

/** @brief Sets watchdog configuration.
 *
 * @param [in] drvData      Driver run-time data.
 * @param [in] wdEnable     true = watchdog timer enabled.
 * @param [in] wdStbyEnable true = watchdog timer enabled during standby.
 * @param [in] wdDuration   Watchdog timer duration (use PF8X_F_WD_DURATION_ macros).
 * @param [in] wdMaxExpire  Number of times WD can expire before WD reset
 *                          (use PF8X_F_WD_MAX_EXPIRE_ macros).
 * @param [in] wdMaxCnt     Number of WD events before the state machine goes to fail-safe
 *                          (use PF8X_F_WD_MAX_CNT_ macros).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetWatchdogConfig(pf8x_drv_data_t* drvData, ot_bool wdEnable,
        ot_bool wdStbyEnable, ot_u8 wdDuration, ot_u8 wdMaxExpire, ot_u8 wdMaxCnt);

/** @brief Reads watchdog status.
 *
 * @param [in]  drvData     Driver run-time data.
 * @param [out] wdExpireCnt Number of times WD expired.
 * @param [out] wdEventCnt  Number of WD events.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_GetWatchdogStatus(pf8x_drv_data_t* drvData, ot_u8* wdExpireCnt, ot_u8* wdEventCnt);

/** @brief Clears watchdog.
 *
 * Watchdog clearing timing needs to be handled in the application.
 *
 * @param [in] drvData     Driver run-time data.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_ClearWatchdog(pf8x_drv_data_t* drvData);

/** @brief Sets fault configuration.
 *
 * @param [in] drvData     Driver run-time data.
 * @param [in] faultMaxCnt Number of faults to initiate power down (use PF8X_F_FAULT_MAX_CNT_ macros).
 * @param [in] faultTimer  Fault timer duration (use PF8X_F_TIMER_FAULT_ macros).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetFaultConfig(pf8x_drv_data_t* drvData, ot_u8 faultMaxCnt, ot_u8 faultTimer);

/** @brief Reads fault counter value.
 *
 * @param [in]  drvData  Driver run-time data.
 * @param [out] faultCnt Fault counter value.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_GetFaultCounter(pf8x_drv_data_t* drvData, ot_u8* faultCnt);

#if PF8X_DEVICE == PF8X_DEVICE_PF82
/** @brief Runs ABIST on demand.
 *
 * @param [in] drvData Driver run-time data.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_RunAbist(pf8x_drv_data_t* drvData);

/** @brief Reads ABIST OV and UV flags.
 *
 * @param [in]  drvData    Driver run-time data.
 * @param [in]  ovUvFlags  false = OV flags, true = UV flags.
 * @param [out] abistFlags ABIST flags (high byte = LDO flags, low byte = SW flags).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_GetAbistFlags(pf8x_drv_data_t* drvData, ot_bool ovUvFlags, ot_u16* abistFlags);

/** @brief Clears ABIST status flags.
 *
 * @param [in] drvData   Driver run-time data.
 * @param [in] ovUvFlags false = OV flags, true = UV flags.
 * @param [in] flagsMask Flags to clear (high byte = LDO flags, low byte = SW flags).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_ClearAbistFlags(pf8x_drv_data_t* drvData, ot_bool ovUvFlags, ot_u16 flagsMask);
#endif

/** @brief Reads test flags.
 *
 * @param [in]  drvData   Driver run-time data.
 * @param [out] testFlags Test flags.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_GetTestFlags(pf8x_drv_data_t* drvData, ot_u8* testFlags);

/** @brief Clears test flags.
 *
 * @param [in] drvData   Driver run-time data.
 * @param [in] flagsMask Flags to clear.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_ClearTestFlags(pf8x_drv_data_t* drvData, ot_u8 flagsMask);

/** @brief Sets analog multiplexer (AMUX).
 *
 * @param [in] drvData    Driver run-time data.
 * @param [in] amuxEnable true = AMUX enabled.
 * @param [in] amuxSel    AMUX selection (use PF8X_F_AMUX_SEL_ macros).
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_SetAmux(pf8x_drv_data_t* drvData, ot_bool amuxEnable, ot_u8 amuxSel);

/** @brief Turns PMIC off (after 500us shutdown timer expires).
 *
 * @param [in] drvData Driver run-time data.
 *
 * @return pf8x_status_t "Status return code."
 */
pf8x_status_t PF8X_TurnOff(pf8x_drv_data_t* drvData);

/** @} */
#endif /* PMIC_PF8X_H_ */
