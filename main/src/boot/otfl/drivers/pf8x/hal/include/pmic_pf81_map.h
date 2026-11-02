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
 * @file pmic_pf81_map.h
 * @brief PF81 register map.
 *
 * This header file contains addresses, masks, shifts and shifted discreet values
 * for runtime registers of the PF81.
 * See datasheet for details.
 *
 * @author nxa22158
 * @version 1.0
 * @date 21-August-2019
 * @copyright Copyright (c) 2019 NXP
 */

#ifndef PMIC_PF81_MAP_H__
#define PMIC_PF81_MAP_H__

/******************************************************************************/
/* DEVICE_ID - Type: R                                                        */
/******************************************************************************/

#define PF8X_DEVICE_ID_ADDR              0x00U
#define PF8X_DEVICE_ID_DEFAULT           0x40U

/**
 * Device ID
 */
#define PF8X_F_DEVICE_ID_MASK            0x0FU
/**
 * Device Family
 */
#define PF8X_F_DEVICE_FAM_MASK           0xF0U

/**
 * Device ID
 */
#define PF8X_F_DEVICE_ID_SHIFT           0x00U
/**
 * Device Family
 */
#define PF8X_F_DEVICE_FAM_SHIFT          0x04U

/**
 * PF8100
 */
#define PF8X_F_DEVICE_ID_PF8100          (0x00U << PF8X_F_DEVICE_ID_SHIFT)
/**
 * PF8200
 */
#define PF8X_F_DEVICE_ID_PF8200          (0x01U << PF8X_F_DEVICE_ID_SHIFT)

/**
 * PF8xxx Family
 */
#define PF8X_F_DEVICE_FAM_PF8XXX         (0x04U << PF8X_F_DEVICE_FAM_SHIFT)

/******************************************************************************/
/* REV_ID - Type: R                                                           */
/******************************************************************************/

#define PF8X_REV_ID_ADDR                 0x01U
#define PF8X_REV_ID_DEFAULT              0x00U

/**
 * N/A
 */
#define PF8X_F_METAL_LAYER_REV_MASK      0x0FU
/**
 * N/A
 */
#define PF8X_F_FULL_LAYER_REV_MASK       0xF0U

/**
 * N/A
 */
#define PF8X_F_METAL_LAYER_REV_SHIFT     0x00U
/**
 * N/A
 */
#define PF8X_F_FULL_LAYER_REV_SHIFT      0x04U

/******************************************************************************/
/* EMREV - Type: R                                                            */
/******************************************************************************/

#define PF8X_EMREV_ADDR                  0x02U
#define PF8X_EMREV_DEFAULT               0xF0U

/**
 * N/A
 */
#define PF8X_F_EMREV_MASK                0x07U
/**
 * N/A
 */
#define PF8X_F_PROG_IDH_MASK             0xF0U

/**
 * N/A
 */
#define PF8X_F_EMREV_SHIFT               0x00U
/**
 * N/A
 */
#define PF8X_F_PROG_IDH_SHIFT            0x04U

/******************************************************************************/
/* PROG_ID - Type: R                                                          */
/******************************************************************************/

#define PF8X_PROG_ID_ADDR                0x03U
#define PF8X_PROG_ID_DEFAULT             0xFFU

/**
 * N/A
 */
#define PF8X_F_PROG_IDL_MASK             0xFFU

/**
 * N/A
 */
#define PF8X_F_PROG_IDL_SHIFT            0x00U

/******************************************************************************/
/* INT_STATUS1 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_INT_STATUS1_ADDR            0x04U
#define PF8X_INT_STATUS1_DEFAULT         0x00U

/**
 * Bit Latches when a VIN _OVLO occurs while VIN_OVLO is enabled. Bit can be cleared even if OVLO condition is still 
 * present.
 */
#define PF8X_F_VIN_OVLO_I_MASK           0x01U
/**
 * Bit is latched when FSOB pin is asserted by any of the programmable events
 */
#define PF8X_F_FSOB_I_MASK               0x02U
/**
 * Bit is latched on the falling edge of XINTB pin
 */
#define PF8X_F_XINTB_I_MASK              0x04U
/**
 * Bit is latched when a Power-down Sequence is finalized Run to Standby mode
 */
#define PF8X_F_PWRDN_I_MASK              0x08U
/**
 * Bit is latched when a Power-up Sequence is finalized Off to Run mode, Standby to Run mode or WD_Recovery to Run Mode
 */
#define PF8X_F_PWRUP_I_MASK              0x10U
/**
 * Bit latches when a bad CRC is detected during an I2C transaction
 */
#define PF8X_F_CRC_I_MASK                0x20U
/**
 * Interrupts when the device has reached the new frequency programmed in manual tuning
 */
#define PF8X_F_FREQ_RDY_I_MASK           0x40U
/**
 * Bit latches when PMIC_OFF is set high, Clearing this flag shall stop the shutdown request.
 */
#define PF8X_F_SDWN_I_MASK               0x80U

/**
 * Bit Latches when a VIN _OVLO occurs while VIN_OVLO is enabled. Bit can be cleared even if OVLO condition is still 
 * present.
 */
#define PF8X_F_VIN_OVLO_I_SHIFT          0x00U
/**
 * Bit is latched when FSOB pin is asserted by any of the programmable events
 */
#define PF8X_F_FSOB_I_SHIFT              0x01U
/**
 * Bit is latched on the falling edge of XINTB pin
 */
#define PF8X_F_XINTB_I_SHIFT             0x02U
/**
 * Bit is latched when a Power-down Sequence is finalized Run to Standby mode
 */
#define PF8X_F_PWRDN_I_SHIFT             0x03U
/**
 * Bit is latched when a Power-up Sequence is finalized Off to Run mode, Standby to Run mode or WD_Recovery to Run Mode
 */
#define PF8X_F_PWRUP_I_SHIFT             0x04U
/**
 * Bit latches when a bad CRC is detected during an I2C transaction
 */
#define PF8X_F_CRC_I_SHIFT               0x05U
/**
 * Interrupts when the device has reached the new frequency programmed in manual tuning
 */
#define PF8X_F_FREQ_RDY_I_SHIFT          0x06U
/**
 * Bit latches when PMIC_OFF is set high, Clearing this flag shall stop the shutdown request.
 */
#define PF8X_F_SDWN_I_SHIFT              0x07U

/**
 * 0
 */
#define PF8X_F_VIN_OVLO_I_0              (0x00U << PF8X_F_VIN_OVLO_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_VIN_OVLO_I_1              (0x01U << PF8X_F_VIN_OVLO_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_FSOB_I_0                  (0x00U << PF8X_F_FSOB_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_FSOB_I_1                  (0x01U << PF8X_F_FSOB_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_XINTB_I_0                 (0x00U << PF8X_F_XINTB_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_XINTB_I_1                 (0x01U << PF8X_F_XINTB_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_PWRDN_I_0                 (0x00U << PF8X_F_PWRDN_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRDN_I_1                 (0x01U << PF8X_F_PWRDN_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_PWRUP_I_0                 (0x00U << PF8X_F_PWRUP_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRUP_I_1                 (0x01U << PF8X_F_PWRUP_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_CRC_I_0                   (0x00U << PF8X_F_CRC_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_CRC_I_1                   (0x01U << PF8X_F_CRC_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_FREQ_RDY_I_0              (0x00U << PF8X_F_FREQ_RDY_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_FREQ_RDY_I_1              (0x01U << PF8X_F_FREQ_RDY_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SDWN_I_0                  (0x00U << PF8X_F_SDWN_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SDWN_I_1                  (0x01U << PF8X_F_SDWN_I_SHIFT)

/******************************************************************************/
/* INT_MASK1 - Type: RW                                                       */
/******************************************************************************/

#define PF8X_INT_MASK1_ADDR              0x05U
#define PF8X_INT_MASK1_DEFAULT           0xFFU

/**
 * Mask the VIN_OVLO_I interrupt
 */
#define PF8X_F_VIN_OVLO_M_MASK           0x01U
/**
 * Mask the FSO_I interrupt
 */
#define PF8X_F_FSOB_M_MASK               0x02U
/**
 * Mask the XINTB_I interrupt
 */
#define PF8X_F_XINTB_M_MASK              0x04U
/**
 * Mask the PWRDN_I interrupt
 */
#define PF8X_F_PWRDN_M_MASK              0x08U
/**
 * Mask the PWRUP_I interrupt
 */
#define PF8X_F_PWRUP_M_MASK              0x10U
/**
 * Mask the CRC_I interrupt
 */
#define PF8X_F_CRC_M_MASK                0x20U
/**
 * Mask the FREQ_RDY_I interrupt
 */
#define PF8X_F_FREQ_RDY_M_MASK           0x40U
/**
 * Mask the SDWN_I interrupt
 */
#define PF8X_F_SDWN_M_MASK               0x80U

/**
 * Mask the VIN_OVLO_I interrupt
 */
#define PF8X_F_VIN_OVLO_M_SHIFT          0x00U
/**
 * Mask the FSO_I interrupt
 */
#define PF8X_F_FSOB_M_SHIFT              0x01U
/**
 * Mask the XINTB_I interrupt
 */
#define PF8X_F_XINTB_M_SHIFT             0x02U
/**
 * Mask the PWRDN_I interrupt
 */
#define PF8X_F_PWRDN_M_SHIFT             0x03U
/**
 * Mask the PWRUP_I interrupt
 */
#define PF8X_F_PWRUP_M_SHIFT             0x04U
/**
 * Mask the CRC_I interrupt
 */
#define PF8X_F_CRC_M_SHIFT               0x05U
/**
 * Mask the FREQ_RDY_I interrupt
 */
#define PF8X_F_FREQ_RDY_M_SHIFT          0x06U
/**
 * Mask the SDWN_I interrupt
 */
#define PF8X_F_SDWN_M_SHIFT              0x07U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_VIN_OVLO_M_UNMASKED       (0x00U << PF8X_F_VIN_OVLO_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_VIN_OVLO_M_MASKED         (0x01U << PF8X_F_VIN_OVLO_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_FSOB_M_UNMASKED           (0x00U << PF8X_F_FSOB_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_FSOB_M_MASKED             (0x01U << PF8X_F_FSOB_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_XINTB_M_UNMASKED          (0x00U << PF8X_F_XINTB_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_XINTB_M_MASKED            (0x01U << PF8X_F_XINTB_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_PWRDN_M_UNMASKED          (0x00U << PF8X_F_PWRDN_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_PWRDN_M_MASKED            (0x01U << PF8X_F_PWRDN_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_PWRUP_M_UNMASKED          (0x00U << PF8X_F_PWRUP_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_PWRUP_M_MASKED            (0x01U << PF8X_F_PWRUP_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_CRC_M_UNMASKED            (0x00U << PF8X_F_CRC_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_CRC_M_MASKED              (0x01U << PF8X_F_CRC_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_FREQ_RDY_M_UNMASKED       (0x00U << PF8X_F_FREQ_RDY_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_FREQ_RDY_M_MASKED         (0x01U << PF8X_F_FREQ_RDY_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SDWN_M_UNMASKED           (0x00U << PF8X_F_SDWN_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SDWN_M_MASKED             (0x01U << PF8X_F_SDWN_M_SHIFT)

/******************************************************************************/
/* INT_SENSE1 - Type: R                                                       */
/******************************************************************************/

#define PF8X_INT_SENSE1_ADDR             0x06U
#define PF8X_INT_SENSE1_DEFAULT          0x00U

/**
 * Sense pin is 1 as long as the VIN_OVLO condition is still present
 */
#define PF8X_F_VIN_OVLO_S_MASK           0x01U
/**
 * Follow the state of the FSOB pin
 */
#define PF8X_F_FSOB_S_MASK               0x02U
/**
 * Follow the state of the XINTB pin
 */
#define PF8X_F_XINTB_S_MASK              0x04U

/**
 * Sense pin is 1 as long as the VIN_OVLO condition is still present
 */
#define PF8X_F_VIN_OVLO_S_SHIFT          0x00U
/**
 * Follow the state of the FSOB pin
 */
#define PF8X_F_FSOB_S_SHIFT              0x01U
/**
 * Follow the state of the XINTB pin
 */
#define PF8X_F_XINTB_S_SHIFT             0x02U

/**
 * 0
 */
#define PF8X_F_VIN_OVLO_S_0              (0x00U << PF8X_F_VIN_OVLO_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_VIN_OVLO_S_1              (0x01U << PF8X_F_VIN_OVLO_S_SHIFT)

/**
 * Low
 */
#define PF8X_F_FSOB_S_LOW                (0x00U << PF8X_F_FSOB_S_SHIFT)
/**
 * High
 */
#define PF8X_F_FSOB_S_HIGH               (0x01U << PF8X_F_FSOB_S_SHIFT)

/**
 * Low
 */
#define PF8X_F_XINTB_S_LOW               (0x00U << PF8X_F_XINTB_S_SHIFT)
/**
 * High
 */
#define PF8X_F_XINTB_S_HIGH              (0x01U << PF8X_F_XINTB_S_SHIFT)

/******************************************************************************/
/* INT_STATUS2 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_INT_STATUS2_ADDR            0x07U
#define PF8X_INT_STATUS2_DEFAULT         0x00U

/**
 * Bit is latch when Central Temp Sensor crosses the 80_C Threshold rising
 */
#define PF8X_F_THERM_80_I_MASK           0x01U
/**
 * Bit is latch when Central Temp Sensor crosses the 95_C Threshold rising
 */
#define PF8X_F_THERM_95_I_MASK           0x02U
/**
 * Bit is latch when Central Temp Sensor crosses the 110_C Threshold rising
 */
#define PF8X_F_THERM_110_I_MASK          0x04U
/**
 * Bit is latch when Central Temp Sensor crosses the 125_C Threshold rising
 */
#define PF8X_F_THERM_125_I_MASK          0x08U
/**
 * Bit is latch when Central Temp Sensor crosses the 140_C Threshold rising
 */
#define PF8X_F_THERM_140_I_MASK          0x10U
/**
 * Bit is latch when Central Temp Sensor crosses the 155_C Threshold rising
 */
#define PF8X_F_THERM_155_I_MASK          0x20U
/**
 * Bit is latched when the internal Frequency Watchdog detects an input frequency out of range or missing on the 
 * SYNCIN pin. If the faulty clock condition persist and the FSYN _FLT_I bit is cleared, the bit will be re-set back.
 */
#define PF8X_F_FSYNC_FLT_I_MASK          0x40U
/**
 * Bit is latched when the soft WD reset is finished after a WDI event.
 */
#define PF8X_F_WDI_I_MASK                0x80U

/**
 * Bit is latch when Central Temp Sensor crosses the 80_C Threshold rising
 */
#define PF8X_F_THERM_80_I_SHIFT          0x00U
/**
 * Bit is latch when Central Temp Sensor crosses the 95_C Threshold rising
 */
#define PF8X_F_THERM_95_I_SHIFT          0x01U
/**
 * Bit is latch when Central Temp Sensor crosses the 110_C Threshold rising
 */
#define PF8X_F_THERM_110_I_SHIFT         0x02U
/**
 * Bit is latch when Central Temp Sensor crosses the 125_C Threshold rising
 */
#define PF8X_F_THERM_125_I_SHIFT         0x03U
/**
 * Bit is latch when Central Temp Sensor crosses the 140_C Threshold rising
 */
#define PF8X_F_THERM_140_I_SHIFT         0x04U
/**
 * Bit is latch when Central Temp Sensor crosses the 155_C Threshold rising
 */
#define PF8X_F_THERM_155_I_SHIFT         0x05U
/**
 * Bit is latched when the internal Frequency Watchdog detects an input frequency out of range or missing on the 
 * SYNCIN pin. If the faulty clock condition persist and the FSYN _FLT_I bit is cleared, the bit will be re-set back.
 */
#define PF8X_F_FSYNC_FLT_I_SHIFT         0x06U
/**
 * Bit is latched when the soft WD reset is finished after a WDI event.
 */
#define PF8X_F_WDI_I_SHIFT               0x07U

/**
 * 0
 */
#define PF8X_F_THERM_80_I_0              (0x00U << PF8X_F_THERM_80_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_80_I_1              (0x01U << PF8X_F_THERM_80_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_95_I_0              (0x00U << PF8X_F_THERM_95_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_95_I_1              (0x01U << PF8X_F_THERM_95_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_110_I_0             (0x00U << PF8X_F_THERM_110_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_110_I_1             (0x01U << PF8X_F_THERM_110_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_125_I_0             (0x00U << PF8X_F_THERM_125_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_125_I_1             (0x01U << PF8X_F_THERM_125_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_140_I_0             (0x00U << PF8X_F_THERM_140_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_140_I_1             (0x01U << PF8X_F_THERM_140_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_155_I_0             (0x00U << PF8X_F_THERM_155_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_155_I_1             (0x01U << PF8X_F_THERM_155_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_FSYNC_FLT_I_0             (0x00U << PF8X_F_FSYNC_FLT_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_FSYNC_FLT_I_1             (0x01U << PF8X_F_FSYNC_FLT_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_WDI_I_0                   (0x00U << PF8X_F_WDI_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_WDI_I_1                   (0x01U << PF8X_F_WDI_I_SHIFT)

/******************************************************************************/
/* INT_MASK2 - Type: RW                                                       */
/******************************************************************************/

#define PF8X_INT_MASK2_ADDR              0x08U
#define PF8X_INT_MASK2_DEFAULT           0x3FU

/**
 * Mask the THERM_80_I interrupt
 */
#define PF8X_F_THERM_80_M_MASK           0x01U
/**
 * Mask the THERM_95_I interrupt
 */
#define PF8X_F_THERM_95_M_MASK           0x02U
/**
 * Mask the THERM_110_I interrupt
 */
#define PF8X_F_THERM_110_M_MASK          0x04U
/**
 * Mask the THERM_125_I interrupt
 */
#define PF8X_F_THERM_125_M_MASK          0x08U
/**
 * Mask the THERM_140_I interrupt
 */
#define PF8X_F_THERM_140_M_MASK          0x10U
/**
 * Mask the THERM_155_I interrupt
 */
#define PF8X_F_THERM_155_M_MASK          0x20U
/**
 * Mask the FSYNC_FLT_I interrupt
 */
#define PF8X_F_FSYNC_FLT_M_MASK          0x40U
/**
 * Mask the WDI_I interrupt
 */
#define PF8X_F_WDI_M_MASK                0x80U

/**
 * Mask the THERM_80_I interrupt
 */
#define PF8X_F_THERM_80_M_SHIFT          0x00U
/**
 * Mask the THERM_95_I interrupt
 */
#define PF8X_F_THERM_95_M_SHIFT          0x01U
/**
 * Mask the THERM_110_I interrupt
 */
#define PF8X_F_THERM_110_M_SHIFT         0x02U
/**
 * Mask the THERM_125_I interrupt
 */
#define PF8X_F_THERM_125_M_SHIFT         0x03U
/**
 * Mask the THERM_140_I interrupt
 */
#define PF8X_F_THERM_140_M_SHIFT         0x04U
/**
 * Mask the THERM_155_I interrupt
 */
#define PF8X_F_THERM_155_M_SHIFT         0x05U
/**
 * Mask the FSYNC_FLT_I interrupt
 */
#define PF8X_F_FSYNC_FLT_M_SHIFT         0x06U
/**
 * Mask the WDI_I interrupt
 */
#define PF8X_F_WDI_M_SHIFT               0x07U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_THERM_80_M_UNMASKED       (0x00U << PF8X_F_THERM_80_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_THERM_80_M_MASKED         (0x01U << PF8X_F_THERM_80_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_THERM_95_M_UNMASKED       (0x00U << PF8X_F_THERM_95_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_THERM_95_M_MASKED         (0x01U << PF8X_F_THERM_95_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_THERM_110_M_UNMASKED      (0x00U << PF8X_F_THERM_110_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_THERM_110_M_MASKED        (0x01U << PF8X_F_THERM_110_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_THERM_125_M_UNMASKED      (0x00U << PF8X_F_THERM_125_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_THERM_125_M_MASKED        (0x01U << PF8X_F_THERM_125_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_THERM_140_M_UNMASKED      (0x00U << PF8X_F_THERM_140_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_THERM_140_M_MASKED        (0x01U << PF8X_F_THERM_140_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_THERM_155_M_UNMASKED      (0x00U << PF8X_F_THERM_155_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_THERM_155_M_MASKED        (0x01U << PF8X_F_THERM_155_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_FSYNC_FLT_M_UNMASKED      (0x00U << PF8X_F_FSYNC_FLT_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_FSYNC_FLT_M_MASKED        (0x01U << PF8X_F_FSYNC_FLT_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_WDI_M_UNMASKED            (0x00U << PF8X_F_WDI_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_WDI_M_MASKED              (0x01U << PF8X_F_WDI_M_SHIFT)

/******************************************************************************/
/* INT_SENSE2 - Type: R                                                       */
/******************************************************************************/

#define PF8X_INT_SENSE2_ADDR             0x09U
#define PF8X_INT_SENSE2_DEFAULT          0x00U

/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_80_S_MASK           0x01U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_95_S_MASK           0x02U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_110_S_MASK          0x04U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_125_S_MASK          0x08U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_140_S_MASK          0x10U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_155_S_MASK          0x20U
/**
 * Sense pin is 1 as long as the internal watchdog is detecting a frequency out of range
 */
#define PF8X_F_FSYNC_FLT_S_MASK          0x40U
/**
 * Sense pin is 1 as long as the WDI pin is high.
 */
#define PF8X_F_WDI_S_MASK                0x80U

/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_80_S_SHIFT          0x00U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_95_S_SHIFT          0x01U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_110_S_SHIFT         0x02U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_125_S_SHIFT         0x03U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_140_S_SHIFT         0x04U
/**
 * Sense pin is 1 as long as the temperature is above the  corresponding temperature Threshold minus a 5__C hysteresis
 */
#define PF8X_F_THERM_155_S_SHIFT         0x05U
/**
 * Sense pin is 1 as long as the internal watchdog is detecting a frequency out of range
 */
#define PF8X_F_FSYNC_FLT_S_SHIFT         0x06U
/**
 * Sense pin is 1 as long as the WDI pin is high.
 */
#define PF8X_F_WDI_S_SHIFT               0x07U

/**
 * 0
 */
#define PF8X_F_THERM_80_S_0              (0x00U << PF8X_F_THERM_80_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_80_S_1              (0x01U << PF8X_F_THERM_80_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_95_S_0              (0x00U << PF8X_F_THERM_95_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_95_S_1              (0x01U << PF8X_F_THERM_95_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_110_S_0             (0x00U << PF8X_F_THERM_110_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_110_S_1             (0x01U << PF8X_F_THERM_110_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_125_S_0             (0x00U << PF8X_F_THERM_125_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_125_S_1             (0x01U << PF8X_F_THERM_125_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_140_S_0             (0x00U << PF8X_F_THERM_140_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_140_S_1             (0x01U << PF8X_F_THERM_140_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_THERM_155_S_0             (0x00U << PF8X_F_THERM_155_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_THERM_155_S_1             (0x01U << PF8X_F_THERM_155_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_FSYNC_FLT_S_0             (0x00U << PF8X_F_FSYNC_FLT_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_FSYNC_FLT_S_1             (0x01U << PF8X_F_FSYNC_FLT_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_WDI_S_0                   (0x00U << PF8X_F_WDI_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_WDI_S_1                   (0x01U << PF8X_F_WDI_S_SHIFT)

/******************************************************************************/
/* SW_MODE_INT - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW_MODE_INT_ADDR            0x0AU
#define PF8X_SW_MODE_INT_DEFAULT         0x00U

/**
 * Interrupt Bit is set when the SW1 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW1_MODE_I_MASK           0x01U
/**
 * Interrupt Bit is set when the SW2 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW2_MODE_I_MASK           0x02U
/**
 * Interrupt Bit is set when the SW3 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW3_MODE_I_MASK           0x04U
/**
 * Interrupt Bit is set when the SW4 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW4_MODE_I_MASK           0x08U
/**
 * Interrupt Bit is set when the SW5 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW5_MODE_I_MASK           0x10U
/**
 * Interrupt Bit is set when the SW6 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW6_MODE_I_MASK           0x20U
/**
 * Interrupt Bit is set when the SW7 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW7_MODE_I_MASK           0x40U

/**
 * Interrupt Bit is set when the SW1 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW1_MODE_I_SHIFT          0x00U
/**
 * Interrupt Bit is set when the SW2 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW2_MODE_I_SHIFT          0x01U
/**
 * Interrupt Bit is set when the SW3 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW3_MODE_I_SHIFT          0x02U
/**
 * Interrupt Bit is set when the SW4 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW4_MODE_I_SHIFT          0x03U
/**
 * Interrupt Bit is set when the SW5 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW5_MODE_I_SHIFT          0x04U
/**
 * Interrupt Bit is set when the SW6 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW6_MODE_I_SHIFT          0x05U
/**
 * Interrupt Bit is set when the SW7 regulator has successfully changed operating mode (PWM, PFM,  Auto Skip)
 */
#define PF8X_F_SW7_MODE_I_SHIFT          0x06U

/**
 * 0
 */
#define PF8X_F_SW1_MODE_I_0              (0x00U << PF8X_F_SW1_MODE_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW1_MODE_I_1              (0x01U << PF8X_F_SW1_MODE_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW2_MODE_I_0              (0x00U << PF8X_F_SW2_MODE_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW2_MODE_I_1              (0x01U << PF8X_F_SW2_MODE_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW3_MODE_I_0              (0x00U << PF8X_F_SW3_MODE_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW3_MODE_I_1              (0x01U << PF8X_F_SW3_MODE_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW4_MODE_I_0              (0x00U << PF8X_F_SW4_MODE_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW4_MODE_I_1              (0x01U << PF8X_F_SW4_MODE_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW5_MODE_I_0              (0x00U << PF8X_F_SW5_MODE_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW5_MODE_I_1              (0x01U << PF8X_F_SW5_MODE_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW6_MODE_I_0              (0x00U << PF8X_F_SW6_MODE_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW6_MODE_I_1              (0x01U << PF8X_F_SW6_MODE_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW7_MODE_I_0              (0x00U << PF8X_F_SW7_MODE_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW7_MODE_I_1              (0x01U << PF8X_F_SW7_MODE_I_SHIFT)

/******************************************************************************/
/* SW_MODE_MASK - Type: RW                                                    */
/******************************************************************************/

#define PF8X_SW_MODE_MASK_ADDR           0x0BU
#define PF8X_SW_MODE_MASK_DEFAULT        0x7FU

/**
 * Mask the SW1_MODE_I interrupt 
 */
#define PF8X_F_SW1_MODE_M_MASK           0x01U
/**
 * Mask the SW2_MODE_I interrupt 
 */
#define PF8X_F_SW2_MODE_M_MASK           0x02U
/**
 * Mask the SW3_MODE_I interrupt 
 */
#define PF8X_F_SW3_MODE_M_MASK           0x04U
/**
 * Mask the SW4_MODE_I interrupt 
 */
#define PF8X_F_SW4_MODE_M_MASK           0x08U
/**
 * Mask the SW5_MODE_I interrupt 
 */
#define PF8X_F_SW5_MODE_M_MASK           0x10U
/**
 * Mask the SW6_MODE_I interrupt 
 */
#define PF8X_F_SW6_MODE_M_MASK           0x20U
/**
 * Mask the SW7_MODE_I interrupt 
 */
#define PF8X_F_SW7_MODE_M_MASK           0x40U

/**
 * Mask the SW1_MODE_I interrupt 
 */
#define PF8X_F_SW1_MODE_M_SHIFT          0x00U
/**
 * Mask the SW2_MODE_I interrupt 
 */
#define PF8X_F_SW2_MODE_M_SHIFT          0x01U
/**
 * Mask the SW3_MODE_I interrupt 
 */
#define PF8X_F_SW3_MODE_M_SHIFT          0x02U
/**
 * Mask the SW4_MODE_I interrupt 
 */
#define PF8X_F_SW4_MODE_M_SHIFT          0x03U
/**
 * Mask the SW5_MODE_I interrupt 
 */
#define PF8X_F_SW5_MODE_M_SHIFT          0x04U
/**
 * Mask the SW6_MODE_I interrupt 
 */
#define PF8X_F_SW6_MODE_M_SHIFT          0x05U
/**
 * Mask the SW7_MODE_I interrupt 
 */
#define PF8X_F_SW7_MODE_M_SHIFT          0x06U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW1_MODE_M_UNMASKED       (0x00U << PF8X_F_SW1_MODE_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW1_MODE_M_MASKED         (0x01U << PF8X_F_SW1_MODE_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW2_MODE_M_UNMASKED       (0x00U << PF8X_F_SW2_MODE_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW2_MODE_M_MASKED         (0x01U << PF8X_F_SW2_MODE_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW3_MODE_M_UNMASKED       (0x00U << PF8X_F_SW3_MODE_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW3_MODE_M_MASKED         (0x01U << PF8X_F_SW3_MODE_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW4_MODE_M_UNMASKED       (0x00U << PF8X_F_SW4_MODE_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW4_MODE_M_MASKED         (0x01U << PF8X_F_SW4_MODE_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW5_MODE_M_UNMASKED       (0x00U << PF8X_F_SW5_MODE_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW5_MODE_M_MASKED         (0x01U << PF8X_F_SW5_MODE_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW6_MODE_M_UNMASKED       (0x00U << PF8X_F_SW6_MODE_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW6_MODE_M_MASKED         (0x01U << PF8X_F_SW6_MODE_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW7_MODE_M_UNMASKED       (0x00U << PF8X_F_SW7_MODE_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW7_MODE_M_MASKED         (0x01U << PF8X_F_SW7_MODE_M_SHIFT)

/******************************************************************************/
/* SW_ILIM_INT - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW_ILIM_INT_ADDR            0x12U
#define PF8X_SW_ILIM_INT_DEFAULT         0x00U

/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW1_ILIM_I_MASK           0x01U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW2_ILIM_I_MASK           0x02U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW3_ILIM_I_MASK           0x04U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW4_ILIM_I_MASK           0x08U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW5_ILIM_I_MASK           0x10U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW6_ILIM_I_MASK           0x20U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW7_ILIM_I_MASK           0x40U

/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW1_ILIM_I_SHIFT          0x00U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW2_ILIM_I_SHIFT          0x01U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW3_ILIM_I_SHIFT          0x02U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW4_ILIM_I_SHIFT          0x03U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW5_ILIM_I_SHIFT          0x04U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW6_ILIM_I_SHIFT          0x05U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW7_ILIM_I_SHIFT          0x06U

/**
 * 0
 */
#define PF8X_F_SW1_ILIM_I_0              (0x00U << PF8X_F_SW1_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW1_ILIM_I_1              (0x01U << PF8X_F_SW1_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW2_ILIM_I_0              (0x00U << PF8X_F_SW2_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW2_ILIM_I_1              (0x01U << PF8X_F_SW2_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW3_ILIM_I_0              (0x00U << PF8X_F_SW3_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW3_ILIM_I_1              (0x01U << PF8X_F_SW3_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW4_ILIM_I_0              (0x00U << PF8X_F_SW4_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW4_ILIM_I_1              (0x01U << PF8X_F_SW4_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW5_ILIM_I_0              (0x00U << PF8X_F_SW5_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW5_ILIM_I_1              (0x01U << PF8X_F_SW5_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW6_ILIM_I_0              (0x00U << PF8X_F_SW6_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW6_ILIM_I_1              (0x01U << PF8X_F_SW6_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW7_ILIM_I_0              (0x00U << PF8X_F_SW7_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW7_ILIM_I_1              (0x01U << PF8X_F_SW7_ILIM_I_SHIFT)

/******************************************************************************/
/* SW_ILIM_MASK - Type: RW                                                    */
/******************************************************************************/

#define PF8X_SW_ILIM_MASK_ADDR           0x13U
#define PF8X_SW_ILIM_MASK_DEFAULT        0x7FU

/**
 * Mask the SW1_ILIM_I interrupt
 */
#define PF8X_F_SW1_ILIM_M_MASK           0x01U
/**
 * Mask the SW2_ILIM_I interrupt
 */
#define PF8X_F_SW2_ILIM_M_MASK           0x02U
/**
 * Mask the SW3_ILIM_I interrupt
 */
#define PF8X_F_SW3_ILIM_M_MASK           0x04U
/**
 * Mask the SW4_ILIM_I interrupt
 */
#define PF8X_F_SW4_ILIM_M_MASK           0x08U
/**
 * Mask the SW5_ILIM_I interrupt
 */
#define PF8X_F_SW5_ILIM_M_MASK           0x10U
/**
 * Mask the SW6_ILIM_I interrupt
 */
#define PF8X_F_SW6_ILIM_M_MASK           0x20U
/**
 * Mask the SW7_ILIM_I interrupt
 */
#define PF8X_F_SW7_ILIM_M_MASK           0x40U

/**
 * Mask the SW1_ILIM_I interrupt
 */
#define PF8X_F_SW1_ILIM_M_SHIFT          0x00U
/**
 * Mask the SW2_ILIM_I interrupt
 */
#define PF8X_F_SW2_ILIM_M_SHIFT          0x01U
/**
 * Mask the SW3_ILIM_I interrupt
 */
#define PF8X_F_SW3_ILIM_M_SHIFT          0x02U
/**
 * Mask the SW4_ILIM_I interrupt
 */
#define PF8X_F_SW4_ILIM_M_SHIFT          0x03U
/**
 * Mask the SW5_ILIM_I interrupt
 */
#define PF8X_F_SW5_ILIM_M_SHIFT          0x04U
/**
 * Mask the SW6_ILIM_I interrupt
 */
#define PF8X_F_SW6_ILIM_M_SHIFT          0x05U
/**
 * Mask the SW7_ILIM_I interrupt
 */
#define PF8X_F_SW7_ILIM_M_SHIFT          0x06U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW1_ILIM_M_UNMASKED       (0x00U << PF8X_F_SW1_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW1_ILIM_M_MASKED         (0x01U << PF8X_F_SW1_ILIM_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW2_ILIM_M_UNMASKED       (0x00U << PF8X_F_SW2_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW2_ILIM_M_MASKED         (0x01U << PF8X_F_SW2_ILIM_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW3_ILIM_M_UNMASKED       (0x00U << PF8X_F_SW3_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW3_ILIM_M_MASKED         (0x01U << PF8X_F_SW3_ILIM_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW4_ILIM_M_UNMASKED       (0x00U << PF8X_F_SW4_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW4_ILIM_M_MASKED         (0x01U << PF8X_F_SW4_ILIM_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW5_ILIM_M_UNMASKED       (0x00U << PF8X_F_SW5_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW5_ILIM_M_MASKED         (0x01U << PF8X_F_SW5_ILIM_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW6_ILIM_M_UNMASKED       (0x00U << PF8X_F_SW6_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW6_ILIM_M_MASKED         (0x01U << PF8X_F_SW6_ILIM_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW7_ILIM_M_UNMASKED       (0x00U << PF8X_F_SW7_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW7_ILIM_M_MASKED         (0x01U << PF8X_F_SW7_ILIM_M_SHIFT)

/******************************************************************************/
/* SW_ILIM_SENSE - Type: R                                                    */
/******************************************************************************/

#define PF8X_SW_ILIM_SENSE_ADDR          0x14U
#define PF8X_SW_ILIM_SENSE_DEFAULT       0x00U

/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW1_ILIM_S_MASK           0x01U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW2_ILIM_S_MASK           0x02U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW3_ILIM_S_MASK           0x04U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW4_ILIM_S_MASK           0x08U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW5_ILIM_S_MASK           0x10U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW6_ILIM_S_MASK           0x20U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW7_ILIM_S_MASK           0x40U

/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW1_ILIM_S_SHIFT          0x00U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW2_ILIM_S_SHIFT          0x01U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW3_ILIM_S_SHIFT          0x02U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW4_ILIM_S_SHIFT          0x03U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW5_ILIM_S_SHIFT          0x04U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW6_ILIM_S_SHIFT          0x05U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW7_ILIM_S_SHIFT          0x06U

/**
 * 0
 */
#define PF8X_F_SW1_ILIM_S_0              (0x00U << PF8X_F_SW1_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW1_ILIM_S_1              (0x01U << PF8X_F_SW1_ILIM_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW2_ILIM_S_0              (0x00U << PF8X_F_SW2_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW2_ILIM_S_1              (0x01U << PF8X_F_SW2_ILIM_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW3_ILIM_S_0              (0x00U << PF8X_F_SW3_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW3_ILIM_S_1              (0x01U << PF8X_F_SW3_ILIM_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW4_ILIM_S_0              (0x00U << PF8X_F_SW4_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW4_ILIM_S_1              (0x01U << PF8X_F_SW4_ILIM_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW5_ILIM_S_0              (0x00U << PF8X_F_SW5_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW5_ILIM_S_1              (0x01U << PF8X_F_SW5_ILIM_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW6_ILIM_S_0              (0x00U << PF8X_F_SW6_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW6_ILIM_S_1              (0x01U << PF8X_F_SW6_ILIM_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW7_ILIM_S_0              (0x00U << PF8X_F_SW7_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW7_ILIM_S_1              (0x01U << PF8X_F_SW7_ILIM_S_SHIFT)

/******************************************************************************/
/* LDO_ILIM_INT - Type: RW                                                    */
/******************************************************************************/

#define PF8X_LDO_ILIM_INT_ADDR           0x15U
#define PF8X_LDO_ILIM_INT_DEFAULT        0x00U

/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO1_ILIM_I_MASK          0x01U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO2_ILIM_I_MASK          0x02U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO3_ILIM_I_MASK          0x04U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO4_ILIM_I_MASK          0x08U

/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO1_ILIM_I_SHIFT         0x00U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO2_ILIM_I_SHIFT         0x01U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO3_ILIM_I_SHIFT         0x02U
/**
 * Interrupt Bit is set if the regulator experience a ILIM failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO4_ILIM_I_SHIFT         0x03U

/**
 * 0
 */
#define PF8X_F_LDO1_ILIM_I_0             (0x00U << PF8X_F_LDO1_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO1_ILIM_I_1             (0x01U << PF8X_F_LDO1_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO2_ILIM_I_0             (0x00U << PF8X_F_LDO2_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO2_ILIM_I_1             (0x01U << PF8X_F_LDO2_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO3_ILIM_I_0             (0x00U << PF8X_F_LDO3_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO3_ILIM_I_1             (0x01U << PF8X_F_LDO3_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO4_ILIM_I_0             (0x00U << PF8X_F_LDO4_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO4_ILIM_I_1             (0x01U << PF8X_F_LDO4_ILIM_I_SHIFT)

/******************************************************************************/
/* LDO_ILIM_MASK - Type: RW                                                   */
/******************************************************************************/

#define PF8X_LDO_ILIM_MASK_ADDR          0x16U
#define PF8X_LDO_ILIM_MASK_DEFAULT       0x0FU

/**
 * Mask the LDO1_ILIM_I interrupt
 */
#define PF8X_F_LDO1_ILIM_M_MASK          0x01U
/**
 * Mask the LDO2_ILIM_I interrupt
 */
#define PF8X_F_LDO2_ILIM_M_MASK          0x02U
/**
 * Mask the LDO3_ILIM_I interrupt
 */
#define PF8X_F_LDO3_ILIM_M_MASK          0x04U
/**
 * Mask the LDO4_ILIM_I interrupt
 */
#define PF8X_F_LDO4_ILIM_M_MASK          0x08U

/**
 * Mask the LDO1_ILIM_I interrupt
 */
#define PF8X_F_LDO1_ILIM_M_SHIFT         0x00U
/**
 * Mask the LDO2_ILIM_I interrupt
 */
#define PF8X_F_LDO2_ILIM_M_SHIFT         0x01U
/**
 * Mask the LDO3_ILIM_I interrupt
 */
#define PF8X_F_LDO3_ILIM_M_SHIFT         0x02U
/**
 * Mask the LDO4_ILIM_I interrupt
 */
#define PF8X_F_LDO4_ILIM_M_SHIFT         0x03U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO1_ILIM_M_UNMASKED      (0x00U << PF8X_F_LDO1_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO1_ILIM_M_MASKED        (0x01U << PF8X_F_LDO1_ILIM_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO2_ILIM_M_UNMASKED      (0x00U << PF8X_F_LDO2_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO2_ILIM_M_MASKED        (0x01U << PF8X_F_LDO2_ILIM_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO3_ILIM_M_UNMASKED      (0x00U << PF8X_F_LDO3_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO3_ILIM_M_MASKED        (0x01U << PF8X_F_LDO3_ILIM_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO4_ILIM_M_UNMASKED      (0x00U << PF8X_F_LDO4_ILIM_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO4_ILIM_M_MASKED        (0x01U << PF8X_F_LDO4_ILIM_M_SHIFT)

/******************************************************************************/
/* LDO_ILIM_SENSE - Type: R                                                   */
/******************************************************************************/

#define PF8X_LDO_ILIM_SENSE_ADDR         0x17U
#define PF8X_LDO_ILIM_SENSE_DEFAULT      0x00U

/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO1_ILIM_S_MASK          0x01U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO2_ILIM_S_MASK          0x02U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO3_ILIM_S_MASK          0x04U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO4_ILIM_S_MASK          0x08U

/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO1_ILIM_S_SHIFT         0x00U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO2_ILIM_S_SHIFT         0x01U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO3_ILIM_S_SHIFT         0x02U
/**
 * Sense pin is 1 as long as the ILIM condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO4_ILIM_S_SHIFT         0x03U

/**
 * 0
 */
#define PF8X_F_LDO1_ILIM_S_0             (0x00U << PF8X_F_LDO1_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO1_ILIM_S_1             (0x01U << PF8X_F_LDO1_ILIM_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO2_ILIM_S_0             (0x00U << PF8X_F_LDO2_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO2_ILIM_S_1             (0x01U << PF8X_F_LDO2_ILIM_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO3_ILIM_S_0             (0x00U << PF8X_F_LDO3_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO3_ILIM_S_1             (0x01U << PF8X_F_LDO3_ILIM_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO4_ILIM_S_0             (0x00U << PF8X_F_LDO4_ILIM_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO4_ILIM_S_1             (0x01U << PF8X_F_LDO4_ILIM_S_SHIFT)

/******************************************************************************/
/* SW_UV_INT - Type: RW                                                       */
/******************************************************************************/

#define PF8X_SW_UV_INT_ADDR              0x18U
#define PF8X_SW_UV_INT_DEFAULT           0x00U

/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW1_UV_I_MASK             0x01U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW2_UV_I_MASK             0x02U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW3_UV_I_MASK             0x04U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW4_UV_I_MASK             0x08U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW5_UV_I_MASK             0x10U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW6_UV_I_MASK             0x20U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW7_UV_I_MASK             0x40U

/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW1_UV_I_SHIFT            0x00U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW2_UV_I_SHIFT            0x01U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW3_UV_I_SHIFT            0x02U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW4_UV_I_SHIFT            0x03U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW5_UV_I_SHIFT            0x04U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW6_UV_I_SHIFT            0x05U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW7_UV_I_SHIFT            0x06U

/**
 * 0
 */
#define PF8X_F_SW1_UV_I_0                (0x00U << PF8X_F_SW1_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW1_UV_I_1                (0x01U << PF8X_F_SW1_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW2_UV_I_0                (0x00U << PF8X_F_SW2_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW2_UV_I_1                (0x01U << PF8X_F_SW2_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW3_UV_I_0                (0x00U << PF8X_F_SW3_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW3_UV_I_1                (0x01U << PF8X_F_SW3_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW4_UV_I_0                (0x00U << PF8X_F_SW4_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW4_UV_I_1                (0x01U << PF8X_F_SW4_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW5_UV_I_0                (0x00U << PF8X_F_SW5_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW5_UV_I_1                (0x01U << PF8X_F_SW5_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW6_UV_I_0                (0x00U << PF8X_F_SW6_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW6_UV_I_1                (0x01U << PF8X_F_SW6_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW7_UV_I_0                (0x00U << PF8X_F_SW7_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW7_UV_I_1                (0x01U << PF8X_F_SW7_UV_I_SHIFT)

/******************************************************************************/
/* SW_UV_MASK - Type: RW                                                      */
/******************************************************************************/

#define PF8X_SW_UV_MASK_ADDR             0x19U
#define PF8X_SW_UV_MASK_DEFAULT          0x7FU

/**
 * Mask the SW1_UV_I interrupt 
 */
#define PF8X_F_SW1_UV_M_MASK             0x01U
/**
 * Mask the SW2_UV_I interrupt 
 */
#define PF8X_F_SW2_UV_M_MASK             0x02U
/**
 * Mask the SW3_UV_I interrupt 
 */
#define PF8X_F_SW3_UV_M_MASK             0x04U
/**
 * Mask the SW4_UV_I interrupt 
 */
#define PF8X_F_SW4_UV_M_MASK             0x08U
/**
 * Mask the SW5_UV_I interrupt 
 */
#define PF8X_F_SW5_UV_M_MASK             0x10U
/**
 * Mask the SW6_UV_I interrupt 
 */
#define PF8X_F_SW6_UV_M_MASK             0x20U
/**
 * Mask the SW7_UV_I interrupt 
 */
#define PF8X_F_SW7_UV_M_MASK             0x40U

/**
 * Mask the SW1_UV_I interrupt 
 */
#define PF8X_F_SW1_UV_M_SHIFT            0x00U
/**
 * Mask the SW2_UV_I interrupt 
 */
#define PF8X_F_SW2_UV_M_SHIFT            0x01U
/**
 * Mask the SW3_UV_I interrupt 
 */
#define PF8X_F_SW3_UV_M_SHIFT            0x02U
/**
 * Mask the SW4_UV_I interrupt 
 */
#define PF8X_F_SW4_UV_M_SHIFT            0x03U
/**
 * Mask the SW5_UV_I interrupt 
 */
#define PF8X_F_SW5_UV_M_SHIFT            0x04U
/**
 * Mask the SW6_UV_I interrupt 
 */
#define PF8X_F_SW6_UV_M_SHIFT            0x05U
/**
 * Mask the SW7_UV_I interrupt 
 */
#define PF8X_F_SW7_UV_M_SHIFT            0x06U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW1_UV_M_UNMASKED         (0x00U << PF8X_F_SW1_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW1_UV_M_MASKED           (0x01U << PF8X_F_SW1_UV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW2_UV_M_UNMASKED         (0x00U << PF8X_F_SW2_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW2_UV_M_MASKED           (0x01U << PF8X_F_SW2_UV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW3_UV_M_UNMASKED         (0x00U << PF8X_F_SW3_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW3_UV_M_MASKED           (0x01U << PF8X_F_SW3_UV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW4_UV_M_UNMASKED         (0x00U << PF8X_F_SW4_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW4_UV_M_MASKED           (0x01U << PF8X_F_SW4_UV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW5_UV_M_UNMASKED         (0x00U << PF8X_F_SW5_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW5_UV_M_MASKED           (0x01U << PF8X_F_SW5_UV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW6_UV_M_UNMASKED         (0x00U << PF8X_F_SW6_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW6_UV_M_MASKED           (0x01U << PF8X_F_SW6_UV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW7_UV_M_UNMASKED         (0x00U << PF8X_F_SW7_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW7_UV_M_MASKED           (0x01U << PF8X_F_SW7_UV_M_SHIFT)

/******************************************************************************/
/* SW_UV_SENSE - Type: R                                                      */
/******************************************************************************/

#define PF8X_SW_UV_SENSE_ADDR            0x1AU
#define PF8X_SW_UV_SENSE_DEFAULT         0x00U

/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW1_UV_S_MASK             0x01U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW2_UV_S_MASK             0x02U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW3_UV_S_MASK             0x04U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW4_UV_S_MASK             0x08U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW5_UV_S_MASK             0x10U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW6_UV_S_MASK             0x20U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW7_UV_S_MASK             0x40U

/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW1_UV_S_SHIFT            0x00U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW2_UV_S_SHIFT            0x01U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW3_UV_S_SHIFT            0x02U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW4_UV_S_SHIFT            0x03U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW5_UV_S_SHIFT            0x04U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW6_UV_S_SHIFT            0x05U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW7_UV_S_SHIFT            0x06U

/**
 * 0
 */
#define PF8X_F_SW1_UV_S_0                (0x00U << PF8X_F_SW1_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW1_UV_S_1                (0x01U << PF8X_F_SW1_UV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW2_UV_S_0                (0x00U << PF8X_F_SW2_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW2_UV_S_1                (0x01U << PF8X_F_SW2_UV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW3_UV_S_0                (0x00U << PF8X_F_SW3_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW3_UV_S_1                (0x01U << PF8X_F_SW3_UV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW4_UV_S_0                (0x00U << PF8X_F_SW4_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW4_UV_S_1                (0x01U << PF8X_F_SW4_UV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW5_UV_S_0                (0x00U << PF8X_F_SW5_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW5_UV_S_1                (0x01U << PF8X_F_SW5_UV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW6_UV_S_0                (0x00U << PF8X_F_SW6_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW6_UV_S_1                (0x01U << PF8X_F_SW6_UV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW7_UV_S_0                (0x00U << PF8X_F_SW7_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW7_UV_S_1                (0x01U << PF8X_F_SW7_UV_S_SHIFT)

/******************************************************************************/
/* SW_OV_INT - Type: RW                                                       */
/******************************************************************************/

#define PF8X_SW_OV_INT_ADDR              0x1BU
#define PF8X_SW_OV_INT_DEFAULT           0x00U

/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW1_OV_I_MASK             0x01U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW2_OV_I_MASK             0x02U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW3_OV_I_MASK             0x04U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW4_OV_I_MASK             0x08U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW5_OV_I_MASK             0x10U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW6_OV_I_MASK             0x20U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW7_OV_I_MASK             0x40U

/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW1_OV_I_SHIFT            0x00U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW2_OV_I_SHIFT            0x01U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW3_OV_I_SHIFT            0x02U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW4_OV_I_SHIFT            0x03U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW5_OV_I_SHIFT            0x04U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW6_OV_I_SHIFT            0x05U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_SW7_OV_I_SHIFT            0x06U

/**
 * 0
 */
#define PF8X_F_SW1_OV_I_0                (0x00U << PF8X_F_SW1_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW1_OV_I_1                (0x01U << PF8X_F_SW1_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW2_OV_I_0                (0x00U << PF8X_F_SW2_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW2_OV_I_1                (0x01U << PF8X_F_SW2_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW3_OV_I_0                (0x00U << PF8X_F_SW3_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW3_OV_I_1                (0x01U << PF8X_F_SW3_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW4_OV_I_0                (0x00U << PF8X_F_SW4_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW4_OV_I_1                (0x01U << PF8X_F_SW4_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW5_OV_I_0                (0x00U << PF8X_F_SW5_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW5_OV_I_1                (0x01U << PF8X_F_SW5_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW6_OV_I_0                (0x00U << PF8X_F_SW6_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW6_OV_I_1                (0x01U << PF8X_F_SW6_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW7_OV_I_0                (0x00U << PF8X_F_SW7_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW7_OV_I_1                (0x01U << PF8X_F_SW7_OV_I_SHIFT)

/******************************************************************************/
/* SW_OV_MASK - Type: RW                                                      */
/******************************************************************************/

#define PF8X_SW_OV_MASK_ADDR             0x1CU
#define PF8X_SW_OV_MASK_DEFAULT          0x7FU

/**
 * Mask the SW1_OV_I interrupt
 */
#define PF8X_F_SW1_OV_M_MASK             0x01U
/**
 * Mask the SW2_OV_I interrupt
 */
#define PF8X_F_SW2_OV_M_MASK             0x02U
/**
 * Mask the SW3_OV_I interrupt
 */
#define PF8X_F_SW3_OV_M_MASK             0x04U
/**
 * Mask the SW4_OV_I interrupt
 */
#define PF8X_F_SW4_OV_M_MASK             0x08U
/**
 * Mask the SW5_OV_I interrupt
 */
#define PF8X_F_SW5_OV_M_MASK             0x10U
/**
 * Mask the SW6_OV_I interrupt
 */
#define PF8X_F_SW6_OV_M_MASK             0x20U
/**
 * Mask the SW7_OV_I interrupt
 */
#define PF8X_F_SW7_OV_M_MASK             0x40U

/**
 * Mask the SW1_OV_I interrupt
 */
#define PF8X_F_SW1_OV_M_SHIFT            0x00U
/**
 * Mask the SW2_OV_I interrupt
 */
#define PF8X_F_SW2_OV_M_SHIFT            0x01U
/**
 * Mask the SW3_OV_I interrupt
 */
#define PF8X_F_SW3_OV_M_SHIFT            0x02U
/**
 * Mask the SW4_OV_I interrupt
 */
#define PF8X_F_SW4_OV_M_SHIFT            0x03U
/**
 * Mask the SW5_OV_I interrupt
 */
#define PF8X_F_SW5_OV_M_SHIFT            0x04U
/**
 * Mask the SW6_OV_I interrupt
 */
#define PF8X_F_SW6_OV_M_SHIFT            0x05U
/**
 * Mask the SW7_OV_I interrupt
 */
#define PF8X_F_SW7_OV_M_SHIFT            0x06U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW1_OV_M_UNMASKED         (0x00U << PF8X_F_SW1_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW1_OV_M_MASKED           (0x01U << PF8X_F_SW1_OV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW2_OV_M_UNMASKED         (0x00U << PF8X_F_SW2_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW2_OV_M_MASKED           (0x01U << PF8X_F_SW2_OV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW3_OV_M_UNMASKED         (0x00U << PF8X_F_SW3_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW3_OV_M_MASKED           (0x01U << PF8X_F_SW3_OV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW4_OV_M_UNMASKED         (0x00U << PF8X_F_SW4_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW4_OV_M_MASKED           (0x01U << PF8X_F_SW4_OV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW5_OV_M_UNMASKED         (0x00U << PF8X_F_SW5_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW5_OV_M_MASKED           (0x01U << PF8X_F_SW5_OV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW6_OV_M_UNMASKED         (0x00U << PF8X_F_SW6_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW6_OV_M_MASKED           (0x01U << PF8X_F_SW6_OV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_SW7_OV_M_UNMASKED         (0x00U << PF8X_F_SW7_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_SW7_OV_M_MASKED           (0x01U << PF8X_F_SW7_OV_M_SHIFT)

/******************************************************************************/
/* SW_OV_SENSE - Type: R                                                      */
/******************************************************************************/

#define PF8X_SW_OV_SENSE_ADDR            0x1DU
#define PF8X_SW_OV_SENSE_DEFAULT         0x00U

/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW1_OV_S_MASK             0x01U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW2_OV_S_MASK             0x02U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW3_OV_S_MASK             0x04U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW4_OV_S_MASK             0x08U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW5_OV_S_MASK             0x10U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW6_OV_S_MASK             0x20U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW7_OV_S_MASK             0x40U

/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW1_OV_S_SHIFT            0x00U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW2_OV_S_SHIFT            0x01U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW3_OV_S_SHIFT            0x02U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW4_OV_S_SHIFT            0x03U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW5_OV_S_SHIFT            0x04U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW6_OV_S_SHIFT            0x05U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_SW7_OV_S_SHIFT            0x06U

/**
 * 0
 */
#define PF8X_F_SW1_OV_S_0                (0x00U << PF8X_F_SW1_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW1_OV_S_1                (0x01U << PF8X_F_SW1_OV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW2_OV_S_0                (0x00U << PF8X_F_SW2_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW2_OV_S_1                (0x01U << PF8X_F_SW2_OV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW3_OV_S_0                (0x00U << PF8X_F_SW3_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW3_OV_S_1                (0x01U << PF8X_F_SW3_OV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW4_OV_S_0                (0x00U << PF8X_F_SW4_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW4_OV_S_1                (0x01U << PF8X_F_SW4_OV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW5_OV_S_0                (0x00U << PF8X_F_SW5_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW5_OV_S_1                (0x01U << PF8X_F_SW5_OV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW6_OV_S_0                (0x00U << PF8X_F_SW6_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW6_OV_S_1                (0x01U << PF8X_F_SW6_OV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_SW7_OV_S_0                (0x00U << PF8X_F_SW7_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_SW7_OV_S_1                (0x01U << PF8X_F_SW7_OV_S_SHIFT)

/******************************************************************************/
/* LDO_UV_INT - Type: RW                                                      */
/******************************************************************************/

#define PF8X_LDO_UV_INT_ADDR             0x1EU
#define PF8X_LDO_UV_INT_DEFAULT          0x00U

/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO1_UV_I_MASK            0x01U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO2_UV_I_MASK            0x02U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO3_UV_I_MASK            0x04U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO4_UV_I_MASK            0x08U

/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO1_UV_I_SHIFT           0x00U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO2_UV_I_SHIFT           0x01U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO3_UV_I_SHIFT           0x02U
/**
 * Interrupt Bit is set if the regulator experience a UV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO4_UV_I_SHIFT           0x03U

/**
 * 0
 */
#define PF8X_F_LDO1_UV_I_0               (0x00U << PF8X_F_LDO1_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO1_UV_I_1               (0x01U << PF8X_F_LDO1_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO2_UV_I_0               (0x00U << PF8X_F_LDO2_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO2_UV_I_1               (0x01U << PF8X_F_LDO2_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO3_UV_I_0               (0x00U << PF8X_F_LDO3_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO3_UV_I_1               (0x01U << PF8X_F_LDO3_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO4_UV_I_0               (0x00U << PF8X_F_LDO4_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO4_UV_I_1               (0x01U << PF8X_F_LDO4_UV_I_SHIFT)

/******************************************************************************/
/* LDO_UV_MASK - Type: RW                                                     */
/******************************************************************************/

#define PF8X_LDO_UV_MASK_ADDR            0x1FU
#define PF8X_LDO_UV_MASK_DEFAULT         0x0FU

/**
 * Mask the LDO1_UV_I interrupt
 */
#define PF8X_F_LDO1_UV_M_MASK            0x01U
/**
 * Mask the LDO2_UV_I interrupt
 */
#define PF8X_F_LDO2_UV_M_MASK            0x02U
/**
 * Mask the LDO3_UV_I interrupt
 */
#define PF8X_F_LDO3_UV_M_MASK            0x04U
/**
 * Mask the LDO4_UV_I interrupt
 */
#define PF8X_F_LDO4_UV_M_MASK            0x08U

/**
 * Mask the LDO1_UV_I interrupt
 */
#define PF8X_F_LDO1_UV_M_SHIFT           0x00U
/**
 * Mask the LDO2_UV_I interrupt
 */
#define PF8X_F_LDO2_UV_M_SHIFT           0x01U
/**
 * Mask the LDO3_UV_I interrupt
 */
#define PF8X_F_LDO3_UV_M_SHIFT           0x02U
/**
 * Mask the LDO4_UV_I interrupt
 */
#define PF8X_F_LDO4_UV_M_SHIFT           0x03U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO1_UV_M_UNMASKED        (0x00U << PF8X_F_LDO1_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO1_UV_M_MASKED          (0x01U << PF8X_F_LDO1_UV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO2_UV_M_UNMASKED        (0x00U << PF8X_F_LDO2_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO2_UV_M_MASKED          (0x01U << PF8X_F_LDO2_UV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO3_UV_M_UNMASKED        (0x00U << PF8X_F_LDO3_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO3_UV_M_MASKED          (0x01U << PF8X_F_LDO3_UV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO4_UV_M_UNMASKED        (0x00U << PF8X_F_LDO4_UV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO4_UV_M_MASKED          (0x01U << PF8X_F_LDO4_UV_M_SHIFT)

/******************************************************************************/
/* LDO_UV_SENSE - Type: R                                                     */
/******************************************************************************/

#define PF8X_LDO_UV_SENSE_ADDR           0x20U
#define PF8X_LDO_UV_SENSE_DEFAULT        0x00U

/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO1_UV_S_MASK            0x01U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO2_UV_S_MASK            0x02U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO3_UV_S_MASK            0x04U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO4_UV_S_MASK            0x08U

/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO1_UV_S_SHIFT           0x00U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO2_UV_S_SHIFT           0x01U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO3_UV_S_SHIFT           0x02U
/**
 * Sense pin is 1 as long as the UV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO4_UV_S_SHIFT           0x03U

/**
 * 0
 */
#define PF8X_F_LDO1_UV_S_0               (0x00U << PF8X_F_LDO1_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO1_UV_S_1               (0x01U << PF8X_F_LDO1_UV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO2_UV_S_0               (0x00U << PF8X_F_LDO2_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO2_UV_S_1               (0x01U << PF8X_F_LDO2_UV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO3_UV_S_0               (0x00U << PF8X_F_LDO3_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO3_UV_S_1               (0x01U << PF8X_F_LDO3_UV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO4_UV_S_0               (0x00U << PF8X_F_LDO4_UV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO4_UV_S_1               (0x01U << PF8X_F_LDO4_UV_S_SHIFT)

/******************************************************************************/
/* LDO_OV_INT - Type: RW                                                      */
/******************************************************************************/

#define PF8X_LDO_OV_INT_ADDR             0x21U
#define PF8X_LDO_OV_INT_DEFAULT          0x00U

/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO1_OV_I_MASK            0x01U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO2_OV_I_MASK            0x02U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO3_OV_I_MASK            0x04U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO4_OV_I_MASK            0x08U

/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO1_OV_I_SHIFT           0x00U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO2_OV_I_SHIFT           0x01U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO3_OV_I_SHIFT           0x02U
/**
 * Interrupt Bit is set if the regulator experience a OV failure. Interrupt bit can be cleared even if the fault 
 * condition is still present.
 */
#define PF8X_F_LDO4_OV_I_SHIFT           0x03U

/**
 * 0
 */
#define PF8X_F_LDO1_OV_I_0               (0x00U << PF8X_F_LDO1_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO1_OV_I_1               (0x01U << PF8X_F_LDO1_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO2_OV_I_0               (0x00U << PF8X_F_LDO2_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO2_OV_I_1               (0x01U << PF8X_F_LDO2_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO3_OV_I_0               (0x00U << PF8X_F_LDO3_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO3_OV_I_1               (0x01U << PF8X_F_LDO3_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO4_OV_I_0               (0x00U << PF8X_F_LDO4_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO4_OV_I_1               (0x01U << PF8X_F_LDO4_OV_I_SHIFT)

/******************************************************************************/
/* LDO_OV_MASK - Type: RW                                                     */
/******************************************************************************/

#define PF8X_LDO_OV_MASK_ADDR            0x22U
#define PF8X_LDO_OV_MASK_DEFAULT         0x0FU

/**
 * Mask the LDO1_OV_I interrupt
 */
#define PF8X_F_LDO1_OV_M_MASK            0x01U
/**
 * Mask the LDO2_OV_I interrupt
 */
#define PF8X_F_LDO2_OV_M_MASK            0x02U
/**
 * Mask the LDO3_OV_I interrupt
 */
#define PF8X_F_LDO3_OV_M_MASK            0x04U
/**
 * Mask the LDO4_OV_I interrupt
 */
#define PF8X_F_LDO4_OV_M_MASK            0x08U

/**
 * Mask the LDO1_OV_I interrupt
 */
#define PF8X_F_LDO1_OV_M_SHIFT           0x00U
/**
 * Mask the LDO2_OV_I interrupt
 */
#define PF8X_F_LDO2_OV_M_SHIFT           0x01U
/**
 * Mask the LDO3_OV_I interrupt
 */
#define PF8X_F_LDO3_OV_M_SHIFT           0x02U
/**
 * Mask the LDO4_OV_I interrupt
 */
#define PF8X_F_LDO4_OV_M_SHIFT           0x03U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO1_OV_M_UNMASKED        (0x00U << PF8X_F_LDO1_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO1_OV_M_MASKED          (0x01U << PF8X_F_LDO1_OV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO2_OV_M_UNMASKED        (0x00U << PF8X_F_LDO2_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO2_OV_M_MASKED          (0x01U << PF8X_F_LDO2_OV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO3_OV_M_UNMASKED        (0x00U << PF8X_F_LDO3_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO3_OV_M_MASKED          (0x01U << PF8X_F_LDO3_OV_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_LDO4_OV_M_UNMASKED        (0x00U << PF8X_F_LDO4_OV_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_LDO4_OV_M_MASKED          (0x01U << PF8X_F_LDO4_OV_M_SHIFT)

/******************************************************************************/
/* LDO_OV_SENSE - Type: R                                                     */
/******************************************************************************/

#define PF8X_LDO_OV_SENSE_ADDR           0x23U
#define PF8X_LDO_OV_SENSE_DEFAULT        0x00U

/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO1_OV_S_MASK            0x01U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO2_OV_S_MASK            0x02U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO3_OV_S_MASK            0x04U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO4_OV_S_MASK            0x08U

/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO1_OV_S_SHIFT           0x00U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO2_OV_S_SHIFT           0x01U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO3_OV_S_SHIFT           0x02U
/**
 * Sense pin is 1 as long as the OV condition is still present in the corresponding Regulator.
 */
#define PF8X_F_LDO4_OV_S_SHIFT           0x03U

/**
 * 0
 */
#define PF8X_F_LDO1_OV_S_0               (0x00U << PF8X_F_LDO1_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO1_OV_S_1               (0x01U << PF8X_F_LDO1_OV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO2_OV_S_0               (0x00U << PF8X_F_LDO2_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO2_OV_S_1               (0x01U << PF8X_F_LDO2_OV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO3_OV_S_0               (0x00U << PF8X_F_LDO3_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO3_OV_S_1               (0x01U << PF8X_F_LDO3_OV_S_SHIFT)

/**
 * 0
 */
#define PF8X_F_LDO4_OV_S_0               (0x00U << PF8X_F_LDO4_OV_S_SHIFT)
/**
 * 1
 */
#define PF8X_F_LDO4_OV_S_1               (0x01U << PF8X_F_LDO4_OV_S_SHIFT)

/******************************************************************************/
/* PWRON_INT - Type: RW                                                       */
/******************************************************************************/

#define PF8X_PWRON_INT_ADDR              0x24U
#define PF8X_PWRON_INT_DEFAULT           0x00U

/**
 * Interrupt bit when PWRON is pushed in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_PUSH_I_MASK         0x01U
/**
 * Interrupt bit when PWRON is Released (After a push transition ) in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_REL_I_MASK          0x02U
/**
 * Interrupt bit when PWRON is pushed for 1S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_1S_I_MASK           0x04U
/**
 * Interrupt bit when PWRON is pushed for 2S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_2S_I_MASK           0x08U
/**
 * Interrupt bit when PWRON is pushed for 3S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_3S_I_MASK           0x10U
/**
 * Interrupt bit when PWRON is pushed for 4S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_4S_I_MASK           0x20U
/**
 * Interrupt bit when PWRON is pushed for 8S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_8S_I_MASK           0x40U
/**
 * Interrupt bit when Bandgap monitor is out of range
 */
#define PF8X_F_BGMON_I_MASK              0x80U

/**
 * Interrupt bit when PWRON is pushed in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_PUSH_I_SHIFT        0x00U
/**
 * Interrupt bit when PWRON is Released (After a push transition ) in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_REL_I_SHIFT         0x01U
/**
 * Interrupt bit when PWRON is pushed for 1S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_1S_I_SHIFT          0x02U
/**
 * Interrupt bit when PWRON is pushed for 2S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_2S_I_SHIFT          0x03U
/**
 * Interrupt bit when PWRON is pushed for 3S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_3S_I_SHIFT          0x04U
/**
 * Interrupt bit when PWRON is pushed for 4S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_4S_I_SHIFT          0x05U
/**
 * Interrupt bit when PWRON is pushed for 8S in Edge sensitive Mode only
 */
#define PF8X_F_PWRON_8S_I_SHIFT          0x06U
/**
 * Interrupt bit when Bandgap monitor is out of range
 */
#define PF8X_F_BGMON_I_SHIFT             0x07U

/**
 * 0
 */
#define PF8X_F_PWRON_PUSH_I_0            (0x00U << PF8X_F_PWRON_PUSH_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRON_PUSH_I_1            (0x01U << PF8X_F_PWRON_PUSH_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_PWRON_REL_I_0             (0x00U << PF8X_F_PWRON_REL_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRON_REL_I_1             (0x01U << PF8X_F_PWRON_REL_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_PWRON_1S_I_0              (0x00U << PF8X_F_PWRON_1S_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRON_1S_I_1              (0x01U << PF8X_F_PWRON_1S_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_PWRON_2S_I_0              (0x00U << PF8X_F_PWRON_2S_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRON_2S_I_1              (0x01U << PF8X_F_PWRON_2S_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_PWRON_3S_I_0              (0x00U << PF8X_F_PWRON_3S_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRON_3S_I_1              (0x01U << PF8X_F_PWRON_3S_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_PWRON_4S_I_0              (0x00U << PF8X_F_PWRON_4S_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRON_4S_I_1              (0x01U << PF8X_F_PWRON_4S_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_PWRON_8S_I_0              (0x00U << PF8X_F_PWRON_8S_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRON_8S_I_1              (0x01U << PF8X_F_PWRON_8S_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_BGMON_I_0                 (0x00U << PF8X_F_BGMON_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_BGMON_I_1                 (0x01U << PF8X_F_BGMON_I_SHIFT)

/******************************************************************************/
/* PWRON_MASK - Type: RW                                                      */
/******************************************************************************/

#define PF8X_PWRON_MASK_ADDR             0x25U
#define PF8X_PWRON_MASK_DEFAULT          0xFFU

/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_PUSH_M_MASK         0x01U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_REL_M_MASK          0x02U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_1S_M_MASK           0x04U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_2S_M_MASK           0x08U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_3S_M_MASK           0x10U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_4S_M_MASK           0x20U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_8S_M_MASK           0x40U
/**
 * Mask the corresponding BGMON I interrupt
 */
#define PF8X_F_BGMON_M_MASK              0x80U

/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_PUSH_M_SHIFT        0x00U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_REL_M_SHIFT         0x01U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_1S_M_SHIFT          0x02U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_2S_M_SHIFT          0x03U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_3S_M_SHIFT          0x04U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_4S_M_SHIFT          0x05U
/**
 * Mask the corresponding Push button I interrupt
 */
#define PF8X_F_PWRON_8S_M_SHIFT          0x06U
/**
 * Mask the corresponding BGMON I interrupt
 */
#define PF8X_F_BGMON_M_SHIFT             0x07U

/**
 * Interrupt Unmasked
 */
#define PF8X_F_PWRON_PUSH_M_UNMASKED     (0x00U << PF8X_F_PWRON_PUSH_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_PWRON_PUSH_M_MASKED       (0x01U << PF8X_F_PWRON_PUSH_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_PWRON_REL_M_UNMASKED      (0x00U << PF8X_F_PWRON_REL_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_PWRON_REL_M_MASKED        (0x01U << PF8X_F_PWRON_REL_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_PWRON_1S_M_UNMASKED       (0x00U << PF8X_F_PWRON_1S_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_PWRON_1S_M_MASKED         (0x01U << PF8X_F_PWRON_1S_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_PWRON_2S_M_UNMASKED       (0x00U << PF8X_F_PWRON_2S_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_PWRON_2S_M_MASKED         (0x01U << PF8X_F_PWRON_2S_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_PWRON_3S_M_UNMASKED       (0x00U << PF8X_F_PWRON_3S_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_PWRON_3S_M_MASKED         (0x01U << PF8X_F_PWRON_3S_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_PWRON_4S_M_UNMASKED       (0x00U << PF8X_F_PWRON_4S_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_PWRON_4S_M_MASKED         (0x01U << PF8X_F_PWRON_4S_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_PWRON_8S_M_UNMASKED       (0x00U << PF8X_F_PWRON_8S_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_PWRON_8S_M_MASKED         (0x01U << PF8X_F_PWRON_8S_M_SHIFT)

/**
 * Interrupt Unmasked
 */
#define PF8X_F_BGMON_M_UNMASKED          (0x00U << PF8X_F_BGMON_M_SHIFT)
/**
 * Interrupt Masked
 */
#define PF8X_F_BGMON_M_MASKED            (0x01U << PF8X_F_BGMON_M_SHIFT)

/******************************************************************************/
/* PWRON_SENSE - Type: R                                                      */
/******************************************************************************/

#define PF8X_PWRON_SENSE_ADDR            0x26U
#define PF8X_PWRON_SENSE_DEFAULT         0x00U

/**
 * Current Status of PWRON pin
 */
#define PF8X_F_PWRON_S_MASK              0x01U
/**
 * Current Status of Bandgap comparator
 */
#define PF8X_F_BGMON_S_MASK              0x80U

/**
 * Current Status of PWRON pin
 */
#define PF8X_F_PWRON_S_SHIFT             0x00U
/**
 * Current Status of Bandgap comparator
 */
#define PF8X_F_BGMON_S_SHIFT             0x07U

/**
 * PWRON pin low (push button pressed)
 */
#define PF8X_F_PWRON_S_LOW               (0x00U << PF8X_F_PWRON_S_SHIFT)
/**
 * PWRON pin high (push button released)
 */
#define PF8X_F_PWRON_S_HIGH              (0x01U << PF8X_F_PWRON_S_SHIFT)

/**
 * PWRON pin low (push button pressed)
 */
#define PF8X_F_BGMON_S_LOW               (0x00U << PF8X_F_BGMON_S_SHIFT)
/**
 * PWRON pin high (push button released)
 */
#define PF8X_F_BGMON_S_HIGH              (0x01U << PF8X_F_BGMON_S_SHIFT)

/******************************************************************************/
/* SYS_INT - Type: RW                                                         */
/******************************************************************************/

#define PF8X_SYS_INT_ADDR                0x27U
#define PF8X_SYS_INT_DEFAULT             0x00U

/**
 * System interrupt coming from STATUS1 interrupt register
 */
#define PF8X_F_STATUS1_I_MASK            0x01U
/**
 * System interrupt coming from STATUS2 interrupt register
 */
#define PF8X_F_STATUS2_I_MASK            0x02U
/**
 * System interrupt coming from SWMODE interrupt register
 */
#define PF8X_F_MODE_I_MASK               0x04U
/**
 * System interrupt coming from one of the ILIM interrupt registers
 */
#define PF8X_F_ILIM_I_MASK               0x08U
/**
 * System interrupt coming from one of the UV interrupt registers
 */
#define PF8X_F_UV_I_MASK                 0x10U
/**
 * System interrupt coming from one of the OV interrupt registers
 */
#define PF8X_F_OV_I_MASK                 0x20U
/**
 * System interrupt coming from PWRON interrupt register
 */
#define PF8X_F_PWRON_I_MASK              0x40U
/**
 * System Interrupt coming from EWARN pin
 */
#define PF8X_F_EWARN_I_MASK              0x80U

/**
 * System interrupt coming from STATUS1 interrupt register
 */
#define PF8X_F_STATUS1_I_SHIFT           0x00U
/**
 * System interrupt coming from STATUS2 interrupt register
 */
#define PF8X_F_STATUS2_I_SHIFT           0x01U
/**
 * System interrupt coming from SWMODE interrupt register
 */
#define PF8X_F_MODE_I_SHIFT              0x02U
/**
 * System interrupt coming from one of the ILIM interrupt registers
 */
#define PF8X_F_ILIM_I_SHIFT              0x03U
/**
 * System interrupt coming from one of the UV interrupt registers
 */
#define PF8X_F_UV_I_SHIFT                0x04U
/**
 * System interrupt coming from one of the OV interrupt registers
 */
#define PF8X_F_OV_I_SHIFT                0x05U
/**
 * System interrupt coming from PWRON interrupt register
 */
#define PF8X_F_PWRON_I_SHIFT             0x06U
/**
 * System Interrupt coming from EWARN pin
 */
#define PF8X_F_EWARN_I_SHIFT             0x07U

/**
 * 0
 */
#define PF8X_F_STATUS1_I_0               (0x00U << PF8X_F_STATUS1_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_STATUS1_I_1               (0x01U << PF8X_F_STATUS1_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_STATUS2_I_0               (0x00U << PF8X_F_STATUS2_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_STATUS2_I_1               (0x01U << PF8X_F_STATUS2_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_MODE_I_0                  (0x00U << PF8X_F_MODE_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_MODE_I_1                  (0x01U << PF8X_F_MODE_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_ILIM_I_0                  (0x00U << PF8X_F_ILIM_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_ILIM_I_1                  (0x01U << PF8X_F_ILIM_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_UV_I_0                    (0x00U << PF8X_F_UV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_UV_I_1                    (0x01U << PF8X_F_UV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_OV_I_0                    (0x00U << PF8X_F_OV_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_OV_I_1                    (0x01U << PF8X_F_OV_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_PWRON_I_0                 (0x00U << PF8X_F_PWRON_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_PWRON_I_1                 (0x01U << PF8X_F_PWRON_I_SHIFT)

/**
 * 0
 */
#define PF8X_F_EWARN_I_0                 (0x00U << PF8X_F_EWARN_I_SHIFT)
/**
 * 1
 */
#define PF8X_F_EWARN_I_1                 (0x01U << PF8X_F_EWARN_I_SHIFT)

/******************************************************************************/
/* HARDFAULT_FLAGS - Type: RW                                                 */
/******************************************************************************/

#define PF8X_HARDFAULT_FLAGS_ADDR        0x29U
#define PF8X_HARDFAULT_FLAGS_DEFAULT     0x00U

/**
 * Flag the specifc Fault that cause the turn off event.
 */
#define PF8X_F_TSD_FAIL_MASK             0x01U
/**
 * Flag the specifc Fault that cause the turn off event.
 */
#define PF8X_F_REG_FAIL_MASK             0x02U
/**
 * Flag the specifc Fault that cause the turn off event.
 */
#define PF8X_F_WD_FAIL_MASK              0x04U
/**
 * Flag the specifc Fault that cause the turn off event.
 */
#define PF8X_F_PU_FAIL_MASK              0x08U

/**
 * Flag the specifc Fault that cause the turn off event.
 */
#define PF8X_F_TSD_FAIL_SHIFT            0x00U
/**
 * Flag the specifc Fault that cause the turn off event.
 */
#define PF8X_F_REG_FAIL_SHIFT            0x01U
/**
 * Flag the specifc Fault that cause the turn off event.
 */
#define PF8X_F_WD_FAIL_SHIFT             0x02U
/**
 * Flag the specifc Fault that cause the turn off event.
 */
#define PF8X_F_PU_FAIL_SHIFT             0x03U

/**
 * Event not detected
 */
#define PF8X_F_TSD_FAIL_NO_EVENT         (0x00U << PF8X_F_TSD_FAIL_SHIFT)
/**
 * Event detected
 */
#define PF8X_F_TSD_FAIL_EVENT            (0x01U << PF8X_F_TSD_FAIL_SHIFT)

/**
 * Event not detected
 */
#define PF8X_F_REG_FAIL_NO_EVENT         (0x00U << PF8X_F_REG_FAIL_SHIFT)
/**
 * Event detected
 */
#define PF8X_F_REG_FAIL_EVENT            (0x01U << PF8X_F_REG_FAIL_SHIFT)

/**
 * Event not detected
 */
#define PF8X_F_WD_FAIL_NO_EVENT          (0x00U << PF8X_F_WD_FAIL_SHIFT)
/**
 * Event detected
 */
#define PF8X_F_WD_FAIL_EVENT             (0x01U << PF8X_F_WD_FAIL_SHIFT)

/**
 * Event not detected
 */
#define PF8X_F_PU_FAIL_NO_EVENT          (0x00U << PF8X_F_PU_FAIL_SHIFT)
/**
 * Event detected
 */
#define PF8X_F_PU_FAIL_EVENT             (0x01U << PF8X_F_PU_FAIL_SHIFT)

/******************************************************************************/
/* FSOB_FLAGS - Type: RW                                                      */
/******************************************************************************/

#define PF8X_FSOB_FLAGS_ADDR             0x2AU
#define PF8X_FSOB_FLAGS_DEFAULT          0x00U

/**
 * Flag the event that cause the FSOB pin to assert. This is a Secure Bits and required the I2C secure Write mechanism 
 * to be cleared.
 */
#define PF8X_F_FSOB_HFAULT_NOK_MASK      0x01U
/**
 * Flag the event that cause the FSOB pin to assert. This is a Secure Bits and required the I2C secure Write mechanism 
 * to be cleared.
 */
#define PF8X_F_FSOB_WDC_NOK_MASK         0x02U
/**
 * Flag the event that cause the FSOB pin to assert. This is a Secure Bits and required the I2C secure Write mechanism 
 * to be cleared.
 */
#define PF8X_F_FSOB_WDI_NOK_MASK         0x04U
/**
 * Flag the event that cause the FSOB pin to assert. This is a Secure Bits and required the I2C secure Write mechanism 
 * to be cleared.
 */
#define PF8X_F_FSOB_SFAULT_NOK_MASK      0x08U

/**
 * Flag the event that cause the FSOB pin to assert. This is a Secure Bits and required the I2C secure Write mechanism 
 * to be cleared.
 */
#define PF8X_F_FSOB_HFAULT_NOK_SHIFT     0x00U
/**
 * Flag the event that cause the FSOB pin to assert. This is a Secure Bits and required the I2C secure Write mechanism 
 * to be cleared.
 */
#define PF8X_F_FSOB_WDC_NOK_SHIFT        0x01U
/**
 * Flag the event that cause the FSOB pin to assert. This is a Secure Bits and required the I2C secure Write mechanism 
 * to be cleared.
 */
#define PF8X_F_FSOB_WDI_NOK_SHIFT        0x02U
/**
 * Flag the event that cause the FSOB pin to assert. This is a Secure Bits and required the I2C secure Write mechanism 
 * to be cleared.
 */
#define PF8X_F_FSOB_SFAULT_NOK_SHIFT     0x03U

/**
 * Event not detected
 */
#define PF8X_F_FSOB_HFAULT_NOK_NO_EVENT  (0x00U << PF8X_F_FSOB_HFAULT_NOK_SHIFT)
/**
 * Event detected
 */
#define PF8X_F_FSOB_HFAULT_NOK_EVENT     (0x01U << PF8X_F_FSOB_HFAULT_NOK_SHIFT)

/**
 * Event not detected
 */
#define PF8X_F_FSOB_WDC_NOK_NO_EVENT     (0x00U << PF8X_F_FSOB_WDC_NOK_SHIFT)
/**
 * Event detected
 */
#define PF8X_F_FSOB_WDC_NOK_EVENT        (0x01U << PF8X_F_FSOB_WDC_NOK_SHIFT)

/**
 * Event not detected
 */
#define PF8X_F_FSOB_WDI_NOK_NO_EVENT     (0x00U << PF8X_F_FSOB_WDI_NOK_SHIFT)
/**
 * Event detected
 */
#define PF8X_F_FSOB_WDI_NOK_EVENT        (0x01U << PF8X_F_FSOB_WDI_NOK_SHIFT)

/**
 * Event not detected
 */
#define PF8X_F_FSOB_SFAULT_NOK_NO_EVENT  (0x00U << PF8X_F_FSOB_SFAULT_NOK_SHIFT)
/**
 * Event detected
 */
#define PF8X_F_FSOB_SFAULT_NOK_EVENT     (0x01U << PF8X_F_FSOB_SFAULT_NOK_SHIFT)

/******************************************************************************/
/* FSOB_SELECT - Type: RW                                                     */
/******************************************************************************/

#define PF8X_FSOB_SELECT_ADDR            0x2BU
#define PF8X_FSOB_SELECT_DEFAULT         0x00U

/**
 * Select the events that may assert the FSOB pin.
 */
#define PF8X_F_FSOB_HARDFAULT_MASK       0x01U
/**
 * Select the events that may assert the FSOB pin.
 */
#define PF8X_F_FSOB_WDC_MASK             0x02U
/**
 * Select the events that may assert the FSOB pin.
 */
#define PF8X_F_FSOB_WDI_MASK             0x04U
/**
 * Select the events that may assert the FSOB pin.
 */
#define PF8X_F_FSOB_SOFTFAULT_MASK       0x08U

/**
 * Select the events that may assert the FSOB pin.
 */
#define PF8X_F_FSOB_HARDFAULT_SHIFT      0x00U
/**
 * Select the events that may assert the FSOB pin.
 */
#define PF8X_F_FSOB_WDC_SHIFT            0x01U
/**
 * Select the events that may assert the FSOB pin.
 */
#define PF8X_F_FSOB_WDI_SHIFT            0x02U
/**
 * Select the events that may assert the FSOB pin.
 */
#define PF8X_F_FSOB_SOFTFAULT_SHIFT      0x03U

/**
 * Corresponding event does not assert FSOB pin
 */
#define PF8X_F_FSOB_HARDFAULT_NO_ASSERT  (0x00U << PF8X_F_FSOB_HARDFAULT_SHIFT)
/**
 * Corresponding event can assert the FSOB pin
 */
#define PF8X_F_FSOB_HARDFAULT_ASSERT     (0x01U << PF8X_F_FSOB_HARDFAULT_SHIFT)

/**
 * Corresponding event does not assert FSOB pin
 */
#define PF8X_F_FSOB_WDC_NO_ASSERT        (0x00U << PF8X_F_FSOB_WDC_SHIFT)
/**
 * Corresponding event can assert the FSOB pin
 */
#define PF8X_F_FSOB_WDC_ASSERT           (0x01U << PF8X_F_FSOB_WDC_SHIFT)

/**
 * Corresponding event does not assert FSOB pin
 */
#define PF8X_F_FSOB_WDI_NO_ASSERT        (0x00U << PF8X_F_FSOB_WDI_SHIFT)
/**
 * Corresponding event can assert the FSOB pin
 */
#define PF8X_F_FSOB_WDI_ASSERT           (0x01U << PF8X_F_FSOB_WDI_SHIFT)

/**
 * Corresponding event does not assert FSOB pin
 */
#define PF8X_F_FSOB_SOFTFAULT_NO_ASSERT  (0x00U << PF8X_F_FSOB_SOFTFAULT_SHIFT)
/**
 * Corresponding event can assert the FSOB pin
 */
#define PF8X_F_FSOB_SOFTFAULT_ASSERT     (0x01U << PF8X_F_FSOB_SOFTFAULT_SHIFT)

/******************************************************************************/
/* TEST_FLAGS - Type: RW                                                      */
/******************************************************************************/

#define PF8X_TEST_FLAGS_ADDR             0x30U
#define PF8X_TEST_FLAGS_DEFAULT          0x00U

/**
 * OTP_NOK
 */
#define PF8X_F_OTP_NOK_MASK              0x01U
/**
 * TRIM_NOK
 */
#define PF8X_F_TRIM_NOK_MASK             0x02U
/**
 * VSELECT sense bit
 */
#define PF8X_F_VSELECT_S_MASK            0x08U
/**
 * LDO2EN sense bit
 */
#define PF8X_F_LDO2EN_S_MASK             0x10U

/**
 * OTP_NOK
 */
#define PF8X_F_OTP_NOK_SHIFT             0x00U
/**
 * TRIM_NOK
 */
#define PF8X_F_TRIM_NOK_SHIFT            0x01U
/**
 * VSELECT sense bit
 */
#define PF8X_F_VSELECT_S_SHIFT           0x03U
/**
 * LDO2EN sense bit
 */
#define PF8X_F_LDO2EN_S_SHIFT            0x04U

/**
 * 0
 */
#define PF8X_F_OTP_NOK_0                 (0x00U << PF8X_F_OTP_NOK_SHIFT)
/**
 * 1
 */
#define PF8X_F_OTP_NOK_1                 (0x01U << PF8X_F_OTP_NOK_SHIFT)

/**
 * 0
 */
#define PF8X_F_TRIM_NOK_0                (0x00U << PF8X_F_TRIM_NOK_SHIFT)
/**
 * 1
 */
#define PF8X_F_TRIM_NOK_1                (0x01U << PF8X_F_TRIM_NOK_SHIFT)

/**
 * Bit is externally grounded
 */
#define PF8X_F_VSELECT_S_BIT_GROUNDED    (0x00U << PF8X_F_VSELECT_S_SHIFT)
/**
 * Bit is externally set high.
 */
#define PF8X_F_VSELECT_S_BIT_HIGH        (0x01U << PF8X_F_VSELECT_S_SHIFT)

/**
 * Bit is externally grounded
 */
#define PF8X_F_LDO2EN_S_BIT_GROUNDED     (0x00U << PF8X_F_LDO2EN_S_SHIFT)
/**
 * Bit is externally set high.
 */
#define PF8X_F_LDO2EN_S_BIT_HIGH         (0x01U << PF8X_F_LDO2EN_S_SHIFT)

/******************************************************************************/
/* VMONEN1 - Type: RW                                                         */
/******************************************************************************/

#define PF8X_VMONEN1_ADDR                0x35U
#define PF8X_VMONEN1_DEFAULT             0x7FU

/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW1VMON_EN_MASK           0x01U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW2VMON_EN_MASK           0x02U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW3VMON_EN_MASK           0x04U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW4VMON_EN_MASK           0x08U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW5VMON_EN_MASK           0x10U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW6VMON_EN_MASK           0x20U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW7VMON_EN_MASK           0x40U

/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW1VMON_EN_SHIFT          0x00U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW2VMON_EN_SHIFT          0x01U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW3VMON_EN_SHIFT          0x02U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW4VMON_EN_SHIFT          0x03U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW5VMON_EN_SHIFT          0x04U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW6VMON_EN_SHIFT          0x05U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_SW7VMON_EN_SHIFT          0x06U

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_SW1VMON_EN_VMON_DISABLE   (0x00U << PF8X_F_SW1VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with SW1 voltage output. (Default)
 */
#define PF8X_F_SW1VMON_EN_VMON_ENABLE    (0x01U << PF8X_F_SW1VMON_EN_SHIFT)

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_SW2VMON_EN_VMON_DISABLE   (0x00U << PF8X_F_SW2VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with SW2 voltage output. (Default)
 */
#define PF8X_F_SW2VMON_EN_VMON_ENABLE    (0x01U << PF8X_F_SW2VMON_EN_SHIFT)

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_SW3VMON_EN_VMON_DISABLE   (0x00U << PF8X_F_SW3VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with SW3 voltage output. (Default)
 */
#define PF8X_F_SW3VMON_EN_VMON_ENABLE    (0x01U << PF8X_F_SW3VMON_EN_SHIFT)

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_SW4VMON_EN_VMON_DISABLE   (0x00U << PF8X_F_SW4VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with SW4 voltage output. (Default)
 */
#define PF8X_F_SW4VMON_EN_VMON_ENABLE    (0x01U << PF8X_F_SW4VMON_EN_SHIFT)

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_SW5VMON_EN_VMON_DISABLE   (0x00U << PF8X_F_SW5VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with SW5 voltage output. (Default)
 */
#define PF8X_F_SW5VMON_EN_VMON_ENABLE    (0x01U << PF8X_F_SW5VMON_EN_SHIFT)

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_SW6VMON_EN_VMON_DISABLE   (0x00U << PF8X_F_SW6VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with SW6 voltage output. (Default)
 */
#define PF8X_F_SW6VMON_EN_VMON_ENABLE    (0x01U << PF8X_F_SW6VMON_EN_SHIFT)

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_SW7VMON_EN_VMON_DISABLE   (0x00U << PF8X_F_SW7VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with SW7 voltage output. (Default)
 */
#define PF8X_F_SW7VMON_EN_VMON_ENABLE    (0x01U << PF8X_F_SW7VMON_EN_SHIFT)

/******************************************************************************/
/* VMONEN2 - Type: RW                                                         */
/******************************************************************************/

#define PF8X_VMONEN2_ADDR                0x36U
#define PF8X_VMONEN2_DEFAULT             0x0FU

/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_LDO1VMON_EN_MASK          0x01U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_LDO2VMON_EN_MASK          0x02U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_LDO3VMON_EN_MASK          0x04U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_LDO4VMON_EN_MASK          0x08U

/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_LDO1VMON_EN_SHIFT         0x00U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_LDO2VMON_EN_SHIFT         0x01U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_LDO3VMON_EN_SHIFT         0x02U
/**
 * Enable Regulator Voltage Monitor. These are Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_LDO4VMON_EN_SHIFT         0x03U

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_LDO1VMON_EN_VMON_DISABLE  (0x00U << PF8X_F_LDO1VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with LDO1 voltage output. (Default)
 */
#define PF8X_F_LDO1VMON_EN_VMON_ENABLE   (0x01U << PF8X_F_LDO1VMON_EN_SHIFT)

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_LDO2VMON_EN_VMON_DISABLE  (0x00U << PF8X_F_LDO2VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with LDO2 voltage output. (Default)
 */
#define PF8X_F_LDO2VMON_EN_VMON_ENABLE   (0x01U << PF8X_F_LDO2VMON_EN_SHIFT)

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_LDO3VMON_EN_VMON_DISABLE  (0x00U << PF8X_F_LDO3VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with LDO3 voltage output. (Default)
 */
#define PF8X_F_LDO3VMON_EN_VMON_ENABLE   (0x01U << PF8X_F_LDO3VMON_EN_SHIFT)

/**
 * Voltage monitor is Disabled
 */
#define PF8X_F_LDO4VMON_EN_VMON_DISABLE  (0x00U << PF8X_F_LDO4VMON_EN_SHIFT)
/**
 * Voltage monitor enables along with LDO4 voltage output. (Default)
 */
#define PF8X_F_LDO4VMON_EN_VMON_ENABLE   (0x01U << PF8X_F_LDO4VMON_EN_SHIFT)

/******************************************************************************/
/* CTRL1 - Type: RW                                                           */
/******************************************************************************/

#define PF8X_CTRL1_ADDR                  0x37U
#define PF8X_CTRL1_DEFAULT               0x10U

/**
 * Selects whether PMIC reacts to WDI in Standby mode. This is a Secure Bits and required the I2C secure Write 
 * mechanism to be disabled.
 */
#define PF8X_F_WDI_STBY_ACTIVE_MASK      0x02U
/**
 * Enables the watchdog timer during STBY mode. This is a Secure Bits and required the I2C secure Write mechanism to 
 * be disabled.
 */
#define PF8X_F_WD_STBY_EN_MASK           0x04U
/**
 * Enables watchdog timer. This is a Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_WD_EN_MASK                0x08U
/**
 * Enable the Temperature Monitoring circiut. (if TEMP MONITOR WORKS OK can be removed from thi spec). This is a 
 * Secure Bit and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_TMP_MON_EN_MASK           0x10U
/**
 * Select the type of reset to be perform upon a WDI event.
 */
#define PF8X_F_WDI_MODE_MASK             0x20U
/**
 * Allow the VIN OVLO Monitor to Shutdown the device upon a VIN_OVLO event. This is a Secure Bits and required the I2C 
 * secure Write mechanism to be disabled.
 */
#define PF8X_F_VIN_OVLO_SDWN_MASK        0x40U
/**
 * Enables or disable the OVLO monitor and protection. This is a Secure Bits and required the I2C secure Write 
 * mechanism to be disabled.
 */
#define PF8X_F_VIN_OVLO_EN_MASK          0x80U

/**
 * Selects whether PMIC reacts to WDI in Standby mode. This is a Secure Bits and required the I2C secure Write 
 * mechanism to be disabled.
 */
#define PF8X_F_WDI_STBY_ACTIVE_SHIFT     0x01U
/**
 * Enables the watchdog timer during STBY mode. This is a Secure Bits and required the I2C secure Write mechanism to 
 * be disabled.
 */
#define PF8X_F_WD_STBY_EN_SHIFT          0x02U
/**
 * Enables watchdog timer. This is a Secure Bits and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_WD_EN_SHIFT               0x03U
/**
 * Enable the Temperature Monitoring circiut. (if TEMP MONITOR WORKS OK can be removed from thi spec). This is a 
 * Secure Bit and required the I2C secure Write mechanism to be disabled.
 */
#define PF8X_F_TMP_MON_EN_SHIFT          0x04U
/**
 * Select the type of reset to be perform upon a WDI event.
 */
#define PF8X_F_WDI_MODE_SHIFT            0x05U
/**
 * Allow the VIN OVLO Monitor to Shutdown the device upon a VIN_OVLO event. This is a Secure Bits and required the I2C 
 * secure Write mechanism to be disabled.
 */
#define PF8X_F_VIN_OVLO_SDWN_SHIFT       0x06U
/**
 * Enables or disable the OVLO monitor and protection. This is a Secure Bits and required the I2C secure Write 
 * mechanism to be disabled.
 */
#define PF8X_F_VIN_OVLO_EN_SHIFT         0x07U

/**
 * WDI event is disabled in Stadby
 */
#define PF8X_F_WDI_STBY_ACTIVE_DISABLED  (0x00U << PF8X_F_WDI_STBY_ACTIVE_SHIFT)
/**
 * WDI event enabled in Standby
 */
#define PF8X_F_WDI_STBY_ACTIVE_ENABLED   (0x01U << PF8X_F_WDI_STBY_ACTIVE_SHIFT)

/**
 * Disabled
 */
#define PF8X_F_WD_STBY_EN_DISABLED       (0x00U << PF8X_F_WD_STBY_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_WD_STBY_EN_ENABLED        (0x01U << PF8X_F_WD_STBY_EN_SHIFT)

/**
 * WD timer Disabled
 */
#define PF8X_F_WD_EN_DISABLED            (0x00U << PF8X_F_WD_EN_SHIFT)
/**
 * WD timer Enabled
 */
#define PF8X_F_WD_EN_ENABLED             (0x01U << PF8X_F_WD_EN_SHIFT)

/**
 * Disabled
 */
#define PF8X_F_TMP_MON_EN_DISABLED       (0x00U << PF8X_F_TMP_MON_EN_SHIFT)
/**
 * Enabled (default)
 */
#define PF8X_F_TMP_MON_EN_ENABLED        (0x01U << PF8X_F_TMP_MON_EN_SHIFT)

/**
 * Soft WD reset
 */
#define PF8X_F_WDI_MODE_SOFT_WD_RESET    (0x00U << PF8X_F_WDI_MODE_SHIFT)
/**
 * Hard WD reset
 */
#define PF8X_F_WDI_MODE_HARD_WD_RESET    (0x01U << PF8X_F_WDI_MODE_SHIFT)

/**
 * No shutdown (only Interrupt sent)
 */
#define PF8X_F_VIN_OVLO_SDWN_NO_SHUTDOWN (0x00U << PF8X_F_VIN_OVLO_SDWN_SHIFT)
/**
 * Device shutsdown upon a VIN_OVLO
 */
#define PF8X_F_VIN_OVLO_SDWN_SHUTDOWN    (0x01U << PF8X_F_VIN_OVLO_SDWN_SHIFT)

/**
 * The VIN_OVLO monitor is Disabled
 */
#define PF8X_F_VIN_OVLO_EN_DISABLED      (0x00U << PF8X_F_VIN_OVLO_EN_SHIFT)
/**
 * The VIN_OVLO monitor is Enabled
 */
#define PF8X_F_VIN_OVLO_EN_ENABLED       (0x01U << PF8X_F_VIN_OVLO_EN_SHIFT)

/******************************************************************************/
/* CTRL2 - Type: RW                                                           */
/******************************************************************************/

#define PF8X_CTRL2_ADDR                  0x38U
#define PF8X_CTRL2_DEFAULT               0x10U

/**
 * Control the Status of PGOOD by I2C in GPO mode during the Standby state (this bit is  set high upon OTP loading if 
 * selected to turn on during Power up sequence)
 */
#define PF8X_F_STBY_PG_GPO_MASK          0x01U
/**
 * Control the Status of PGOOD by I2C in GPO mode during the RUN state (this bit is  set high upon OTP loading if 
 * selected to turn on during Power up sequence)
 */
#define PF8X_F_RUN_PG_GPO_MASK           0x02U
/**
 * Invert the polarity of the STANDBY pin 
 */
#define PF8X_F_STANDBYINV_MASK           0x04U
/**
 * When device is in the OFF mode enable LPM or QPU
 */
#define PF8X_F_LPM_OFF_MASK              0x08U
/**
 * Enable the Thermal monitor to be Alwasys on or Pulsed during the Run Mode. In Stand by it always use the 1ms on 
 * cycle to save current.
 */
#define PF8X_F_TMP_MON_AON_MASK          0x10U
/**
 * Sets the Vin overvoltage lockout debounce timer to 10, 100 or 1000 us.
 */
#define PF8X_F_VIN_OVLO_DBNC_MASK        0xC0U

/**
 * Control the Status of PGOOD by I2C in GPO mode during the Standby state (this bit is  set high upon OTP loading if 
 * selected to turn on during Power up sequence)
 */
#define PF8X_F_STBY_PG_GPO_SHIFT         0x00U
/**
 * Control the Status of PGOOD by I2C in GPO mode during the RUN state (this bit is  set high upon OTP loading if 
 * selected to turn on during Power up sequence)
 */
#define PF8X_F_RUN_PG_GPO_SHIFT          0x01U
/**
 * Invert the polarity of the STANDBY pin 
 */
#define PF8X_F_STANDBYINV_SHIFT          0x02U
/**
 * When device is in the OFF mode enable LPM or QPU
 */
#define PF8X_F_LPM_OFF_SHIFT             0x03U
/**
 * Enable the Thermal monitor to be Alwasys on or Pulsed during the Run Mode. In Stand by it always use the 1ms on 
 * cycle to save current.
 */
#define PF8X_F_TMP_MON_AON_SHIFT         0x04U
/**
 * Sets the Vin overvoltage lockout debounce timer to 10, 100 or 1000 us.
 */
#define PF8X_F_VIN_OVLO_DBNC_SHIFT       0x06U

/**
 * PGOOD Low
 */
#define PF8X_F_STBY_PG_GPO_PGOOD_LOW     (0x00U << PF8X_F_STBY_PG_GPO_SHIFT)
/**
 * PGOOD High
 */
#define PF8X_F_STBY_PG_GPO_PGOOD_HIGH    (0x01U << PF8X_F_STBY_PG_GPO_SHIFT)

/**
 * PGOOD Low
 */
#define PF8X_F_RUN_PG_GPO_PGOOD_LOW      (0x00U << PF8X_F_RUN_PG_GPO_SHIFT)
/**
 * PGOOD High
 */
#define PF8X_F_RUN_PG_GPO_PGOOD_HIGH     (0x01U << PF8X_F_RUN_PG_GPO_SHIFT)

/**
 * STANDBY pin Active High
 */
#define PF8X_F_STANDBYINV_ACTIVE_HIGH    (0x00U << PF8X_F_STANDBYINV_SHIFT)
/**
 * STANDBY pin Active Low
 */
#define PF8X_F_STANDBYINV_ACTIVE_LOW     (0x01U << PF8X_F_STANDBYINV_SHIFT)

/**
 * LPM (Low power mode) (default)
 */
#define PF8X_F_LPM_OFF_LPM               (0x00U << PF8X_F_LPM_OFF_SHIFT)
/**
 * QPU (Quick power up)
 */
#define PF8X_F_LPM_OFF_QPU               (0x01U << PF8X_F_LPM_OFF_SHIFT)

/**
 * Thermal monitor is turend on every 1ms
 */
#define PF8X_F_TMP_MON_AON_1_MS          (0x00U << PF8X_F_TMP_MON_AON_SHIFT)
/**
 * Thermal monitor is Always on 
 */
#define PF8X_F_TMP_MON_AON_ALWAYS_ON     (0x01U << PF8X_F_TMP_MON_AON_SHIFT)

/**
 * 10 us
 */
#define PF8X_F_VIN_OVLO_DBNC_10_US       (0x00U << PF8X_F_VIN_OVLO_DBNC_SHIFT)
/**
 * 100 us
 */
#define PF8X_F_VIN_OVLO_DBNC_100_US      (0x01U << PF8X_F_VIN_OVLO_DBNC_SHIFT)
/**
 * 1000 us
 */
#define PF8X_F_VIN_OVLO_DBNC_1000_US     (0x02U << PF8X_F_VIN_OVLO_DBNC_SHIFT)

/******************************************************************************/
/* CTRL3 - Type: RW                                                           */
/******************************************************************************/

#define PF8X_CTRL3_ADDR                  0x39U
#define PF8X_CTRL3_DEFAULT               0x20U

/**
 * When INTB_TEST is set high, the interrupt pin shall assert for 100us and then de-assert to its normal state. Bit 
 * self-clears to 0 after test pulse is performed.
 */
#define PF8X_F_INTB_TEST_MASK            0x01U
/**
 * When this pin is set high, device start a 500us shutdown timer. If Pin is nor cleared within that time, device will 
 * initiate a shut down sequence.
 */
#define PF8X_F_PMIC_OFF_MASK             0x02U
/**
 * Sets the UV detection debounce
 */
#define PF8X_F_UV_DB_MASK                0x30U
/**
 * Sets the OV detection debounce
 */
#define PF8X_F_OV_DB_MASK                0xC0U

/**
 * When INTB_TEST is set high, the interrupt pin shall assert for 100us and then de-assert to its normal state. Bit 
 * self-clears to 0 after test pulse is performed.
 */
#define PF8X_F_INTB_TEST_SHIFT           0x00U
/**
 * When this pin is set high, device start a 500us shutdown timer. If Pin is nor cleared within that time, device will 
 * initiate a shut down sequence.
 */
#define PF8X_F_PMIC_OFF_SHIFT            0x01U
/**
 * Sets the UV detection debounce
 */
#define PF8X_F_UV_DB_SHIFT               0x04U
/**
 * Sets the OV detection debounce
 */
#define PF8X_F_OV_DB_SHIFT               0x06U

/**
 * 0
 */
#define PF8X_F_INTB_TEST_0               (0x00U << PF8X_F_INTB_TEST_SHIFT)
/**
 * 1
 */
#define PF8X_F_INTB_TEST_1               (0x01U << PF8X_F_INTB_TEST_SHIFT)

/**
 * 0
 */
#define PF8X_F_PMIC_OFF_0                (0x00U << PF8X_F_PMIC_OFF_SHIFT)
/**
 * 1
 */
#define PF8X_F_PMIC_OFF_1                (0x01U << PF8X_F_PMIC_OFF_SHIFT)

/**
 * 5 us
 */
#define PF8X_F_UV_DB_5_US                (0x00U << PF8X_F_UV_DB_SHIFT)
/**
 * 15 us
 */
#define PF8X_F_UV_DB_15_US               (0x01U << PF8X_F_UV_DB_SHIFT)
/**
 * 30 us
 */
#define PF8X_F_UV_DB_30_US               (0x02U << PF8X_F_UV_DB_SHIFT)
/**
 * 40 us
 */
#define PF8X_F_UV_DB_40_US               (0x03U << PF8X_F_UV_DB_SHIFT)

/**
 * 30 us
 */
#define PF8X_F_OV_DB_30_US               (0x00U << PF8X_F_OV_DB_SHIFT)
/**
 * 50 us
 */
#define PF8X_F_OV_DB_50_US               (0x01U << PF8X_F_OV_DB_SHIFT)
/**
 * 80 us
 */
#define PF8X_F_OV_DB_80_US               (0x02U << PF8X_F_OV_DB_SHIFT)
/**
 * 125 us
 */
#define PF8X_F_OV_DB_125_US              (0x03U << PF8X_F_OV_DB_SHIFT)

/******************************************************************************/
/* PWRUP_CTRL - Type: RW                                                      */
/******************************************************************************/

#define PF8X_PWRUP_CTRL_ADDR             0x3AU
#define PF8X_PWRUP_CTRL_DEFAULT          0x00U

/**
 * OTP bit sets the default power up time base for the power up sequencer, Functional bits can be changed to set the 
 * time base for the power down sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SEQ_TBASE_MASK            0x03U
/**
 * Assign a Power Down group to the corresponding register.
 */
#define PF8X_F_RESETBMCU_PDGRP_MASK      0x0CU
/**
 * Assign a Power Down group to the corresponding register.
 */
#define PF8X_F_PGOOD_PDGRP_MASK          0x30U
/**
 * Enable the Power down sequence by groups
 */
#define PF8X_F_PWRDWN_MODE_MASK          0x40U

/**
 * OTP bit sets the default power up time base for the power up sequencer, Functional bits can be changed to set the 
 * time base for the power down sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SEQ_TBASE_SHIFT           0x00U
/**
 * Assign a Power Down group to the corresponding register.
 */
#define PF8X_F_RESETBMCU_PDGRP_SHIFT     0x02U
/**
 * Assign a Power Down group to the corresponding register.
 */
#define PF8X_F_PGOOD_PDGRP_SHIFT         0x04U
/**
 * Enable the Power down sequence by groups
 */
#define PF8X_F_PWRDWN_MODE_SHIFT         0x06U

/**
 * 30 us
 */
#define PF8X_F_SEQ_TBASE_30_US           (0x00U << PF8X_F_SEQ_TBASE_SHIFT)
/**
 * 120 us
 */
#define PF8X_F_SEQ_TBASE_120_US          (0x01U << PF8X_F_SEQ_TBASE_SHIFT)
/**
 * 250 us
 */
#define PF8X_F_SEQ_TBASE_250_US          (0x02U << PF8X_F_SEQ_TBASE_SHIFT)
/**
 * 500 us
 */
#define PF8X_F_SEQ_TBASE_500_US          (0x03U << PF8X_F_SEQ_TBASE_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_RESETBMCU_PDGRP_GROUP_4   (0x00U << PF8X_F_RESETBMCU_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_RESETBMCU_PDGRP_GROUP_3   (0x01U << PF8X_F_RESETBMCU_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_RESETBMCU_PDGRP_GROUP_2   (0x02U << PF8X_F_RESETBMCU_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_RESETBMCU_PDGRP_GROUP_1   (0x03U << PF8X_F_RESETBMCU_PDGRP_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_PGOOD_PDGRP_GROUP_4       (0x00U << PF8X_F_PGOOD_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_PGOOD_PDGRP_GROUP_3       (0x01U << PF8X_F_PGOOD_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_PGOOD_PDGRP_GROUP_2       (0x02U << PF8X_F_PGOOD_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_PGOOD_PDGRP_GROUP_1       (0x03U << PF8X_F_PGOOD_PDGRP_SHIFT)

/**
 * Power down sequence is a mirror of the power up sequence (one by one)
 */
#define PF8X_F_PWRDWN_MODE_MIRROR_SEQ    (0x00U << PF8X_F_PWRDWN_MODE_SHIFT)
/**
 * Power down sequence use the group sequencing mode
 */
#define PF8X_F_PWRDWN_MODE_GROUP_SEQ     (0x01U << PF8X_F_PWRDWN_MODE_SHIFT)

/******************************************************************************/
/* RESETBMCU_PWRUP - Type: RW                                                 */
/******************************************************************************/

#define PF8X_RESETBMCU_PWRUP_ADDR        0x3CU
#define PF8X_RESETBMCU_PWRUP_DEFAULT     0x00U

/**
 * OTP bit set the default sequence for the Output signals,  Functional bits can be changed to set the power down 
 * sequence.
 */
#define PF8X_F_RESETBMCU_SEQ_MASK        0xFFU

/**
 * OTP bit set the default sequence for the Output signals,  Functional bits can be changed to set the power down 
 * sequence.
 */
#define PF8X_F_RESETBMCU_SEQ_SHIFT       0x00U

/******************************************************************************/
/* PGOOD_PWRUP - Type: RW                                                     */
/******************************************************************************/

#define PF8X_PGOOD_PWRUP_ADDR            0x3DU
#define PF8X_PGOOD_PWRUP_DEFAULT         0x00U

/**
 * OTP bit set the default sequence for the Output signals,  Functional bits can be changed to set the power down 
 * sequence.
 */
#define PF8X_F_PGOOD_SEQ_MASK            0xFFU

/**
 * OTP bit set the default sequence for the Output signals,  Functional bits can be changed to set the power down 
 * sequence.
 */
#define PF8X_F_PGOOD_SEQ_SHIFT           0x00U

/******************************************************************************/
/* PWRDN_DLY1 - Type: RW                                                      */
/******************************************************************************/

#define PF8X_PWRDN_DLY1_ADDR             0x3EU
#define PF8X_PWRDN_DLY1_DEFAULT          0x00U

/**
 * Sets the time base for Group power down sequencing.
 */
#define PF8X_F_GRP1_DLY_MASK             0x03U
/**
 * Sets the time base for Group power down sequencing.
 */
#define PF8X_F_GRP2_DLY_MASK             0x0CU
/**
 * Sets the time base for Group power down sequencing.
 */
#define PF8X_F_GRP3_DLY_MASK             0x30U
/**
 * Sets the time base for Group power down sequencing.
 */
#define PF8X_F_GRP4_DLY_MASK             0xC0U

/**
 * Sets the time base for Group power down sequencing.
 */
#define PF8X_F_GRP1_DLY_SHIFT            0x00U
/**
 * Sets the time base for Group power down sequencing.
 */
#define PF8X_F_GRP2_DLY_SHIFT            0x02U
/**
 * Sets the time base for Group power down sequencing.
 */
#define PF8X_F_GRP3_DLY_SHIFT            0x04U
/**
 * Sets the time base for Group power down sequencing.
 */
#define PF8X_F_GRP4_DLY_SHIFT            0x06U

/**
 * 125 us
 */
#define PF8X_F_GRP1_DLY_125_US           (0x00U << PF8X_F_GRP1_DLY_SHIFT)
/**
 * 250 us
 */
#define PF8X_F_GRP1_DLY_250_US           (0x01U << PF8X_F_GRP1_DLY_SHIFT)
/**
 * 500 us
 */
#define PF8X_F_GRP1_DLY_500_US           (0x02U << PF8X_F_GRP1_DLY_SHIFT)
/**
 * 1000 us
 */
#define PF8X_F_GRP1_DLY_1000_US          (0x03U << PF8X_F_GRP1_DLY_SHIFT)

/**
 * 125 us
 */
#define PF8X_F_GRP2_DLY_125_US           (0x00U << PF8X_F_GRP2_DLY_SHIFT)
/**
 * 250 us
 */
#define PF8X_F_GRP2_DLY_250_US           (0x01U << PF8X_F_GRP2_DLY_SHIFT)
/**
 * 500 us
 */
#define PF8X_F_GRP2_DLY_500_US           (0x02U << PF8X_F_GRP2_DLY_SHIFT)
/**
 * 1000 us
 */
#define PF8X_F_GRP2_DLY_1000_US          (0x03U << PF8X_F_GRP2_DLY_SHIFT)

/**
 * 125 us
 */
#define PF8X_F_GRP3_DLY_125_US           (0x00U << PF8X_F_GRP3_DLY_SHIFT)
/**
 * 250 us
 */
#define PF8X_F_GRP3_DLY_250_US           (0x01U << PF8X_F_GRP3_DLY_SHIFT)
/**
 * 500 us
 */
#define PF8X_F_GRP3_DLY_500_US           (0x02U << PF8X_F_GRP3_DLY_SHIFT)
/**
 * 1000 us
 */
#define PF8X_F_GRP3_DLY_1000_US          (0x03U << PF8X_F_GRP3_DLY_SHIFT)

/**
 * 125 us
 */
#define PF8X_F_GRP4_DLY_125_US           (0x00U << PF8X_F_GRP4_DLY_SHIFT)
/**
 * 250 us
 */
#define PF8X_F_GRP4_DLY_250_US           (0x01U << PF8X_F_GRP4_DLY_SHIFT)
/**
 * 500 us
 */
#define PF8X_F_GRP4_DLY_500_US           (0x02U << PF8X_F_GRP4_DLY_SHIFT)
/**
 * 1000 us
 */
#define PF8X_F_GRP4_DLY_1000_US          (0x03U << PF8X_F_GRP4_DLY_SHIFT)

/******************************************************************************/
/* PWRDN_DLY2 - Type: RW                                                      */
/******************************************************************************/

#define PF8X_PWRDN_DLY2_ADDR             0x3FU
#define PF8X_PWRDN_DLY2_DEFAULT          0x00U

/**
 * Set delay between the RESETBMCU asserting and the first regulator in the power down sequence to start powering off.
 */
#define PF8X_F_RESETBMCU_DLY_MASK        0x03U

/**
 * Set delay between the RESETBMCU asserting and the first regulator in the power down sequence to start powering off.
 */
#define PF8X_F_RESETBMCU_DLY_SHIFT       0x00U

/**
 * No delay
 */
#define PF8X_F_RESETBMCU_DLY_NO_DELAY    (0x00U << PF8X_F_RESETBMCU_DLY_SHIFT)
/**
 * 10 us
 */
#define PF8X_F_RESETBMCU_DLY_10_US       (0x01U << PF8X_F_RESETBMCU_DLY_SHIFT)
/**
 * 100 us
 */
#define PF8X_F_RESETBMCU_DLY_100_US      (0x02U << PF8X_F_RESETBMCU_DLY_SHIFT)
/**
 * 500 us
 */
#define PF8X_F_RESETBMCU_DLY_500_US      (0x03U << PF8X_F_RESETBMCU_DLY_SHIFT)

/******************************************************************************/
/* FREQ_CTRL - Type: RW                                                       */
/******************************************************************************/

#define PF8X_FREQ_CTRL_ADDR              0x40U
#define PF8X_FREQ_CTRL_DEFAULT           0x00U

/**
 * Manual frequency tuning control (allow frequency range from 2MHz to 3MHz)
 */
#define PF8X_F_CLK_FREQ_MASK             0x0FU
/**
 * Set the Frequency Spread Spectrum Range
 */
#define PF8X_F_FSS_RANGE_MASK            0x10U
/**
 * Enable Frequency Spread Spectrum feature
 */
#define PF8X_F_FSS_EN_MASK               0x20U
/**
 * Selects the Input Clock frequency range
 */
#define PF8X_F_FSYNC_RANGE_MASK          0x40U
/**
 * Enables/disables CLKOUT
 */
#define PF8X_F_SYNCOUT_EN_MASK           0x80U

/**
 * Manual frequency tuning control (allow frequency range from 2MHz to 3MHz)
 */
#define PF8X_F_CLK_FREQ_SHIFT            0x00U
/**
 * Set the Frequency Spread Spectrum Range
 */
#define PF8X_F_FSS_RANGE_SHIFT           0x04U
/**
 * Enable Frequency Spread Spectrum feature
 */
#define PF8X_F_FSS_EN_SHIFT              0x05U
/**
 * Selects the Input Clock frequency range
 */
#define PF8X_F_FSYNC_RANGE_SHIFT         0x06U
/**
 * Enables/disables CLKOUT
 */
#define PF8X_F_SYNCOUT_EN_SHIFT          0x07U

/**
 * High Speed Oscillator Frequency 20 MHz, Switching Regulators Frequency 2.500 MHz
 */
#define PF8X_F_CLK_FREQ_2_500_MHZ        (0x00U << PF8X_F_CLK_FREQ_SHIFT)
/**
 * High Speed Oscillator Frequency 21 MHz, Switching Regulators Frequency 2.625 MHz
 */
#define PF8X_F_CLK_FREQ_2_625_MHZ        (0x01U << PF8X_F_CLK_FREQ_SHIFT)
/**
 * High Speed Oscillator Frequency 22 MHz, Switching Regulators Frequency 2.750 MHz
 */
#define PF8X_F_CLK_FREQ_2_750_MHZ        (0x02U << PF8X_F_CLK_FREQ_SHIFT)
/**
 * High Speed Oscillator Frequency 23 MHz, Switching Regulators Frequency 2.875 MHz
 */
#define PF8X_F_CLK_FREQ_2_875_MHZ        (0x03U << PF8X_F_CLK_FREQ_SHIFT)
/**
 * High Speed Oscillator Frequency 24 MHz, Switching Regulators Frequency 3.000 MHz
 */
#define PF8X_F_CLK_FREQ_3_000_MHZ        (0x04U << PF8X_F_CLK_FREQ_SHIFT)
/**
 * High Speed Oscillator Frequency 16 MHz, Switching Regulators Frequency 2.000 MHz
 */
#define PF8X_F_CLK_FREQ_2_000_MHZ        (0x09U << PF8X_F_CLK_FREQ_SHIFT)
/**
 * High Speed Oscillator Frequency 17 MHz, Switching Regulators Frequency 2.125 MHz
 */
#define PF8X_F_CLK_FREQ_2_125_MHZ        (0x0AU << PF8X_F_CLK_FREQ_SHIFT)
/**
 * High Speed Oscillator Frequency 18 MHz, Switching Regulators Frequency 2.250 MHz
 */
#define PF8X_F_CLK_FREQ_2_250_MHZ        (0x0BU << PF8X_F_CLK_FREQ_SHIFT)
/**
 * High Speed Oscillator Frequency 19 MHz, Switching Regulators Frequency 2.375 MHz
 */
#define PF8X_F_CLK_FREQ_2_375_MHZ        (0x0CU << PF8X_F_CLK_FREQ_SHIFT)

/**
 * +/- 5%
 */
#define PF8X_F_FSS_RANGE_5               (0x00U << PF8X_F_FSS_RANGE_SHIFT)
/**
 *  +/- 10%
 */
#define PF8X_F_FSS_RANGE_10              (0x01U << PF8X_F_FSS_RANGE_SHIFT)

/**
 * Disabled
 */
#define PF8X_F_FSS_EN_DISABLED           (0x00U << PF8X_F_FSS_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_FSS_EN_ENABLED            (0x01U << PF8X_F_FSS_EN_SHIFT)

/**
 * 2000 kHz to 3000 kHz
 */
#define PF8X_F_FSYNC_RANGE_2000_KHZ_3000_KHZ (0x00U << PF8X_F_FSYNC_RANGE_SHIFT)
/**
 * 333 kHz to 500 kHz
 */
#define PF8X_F_FSYNC_RANGE_333_KHZ_500_KHZ (0x01U << PF8X_F_FSYNC_RANGE_SHIFT)

/**
 * Disabled
 */
#define PF8X_F_SYNCOUT_EN_DISABLED       (0x00U << PF8X_F_SYNCOUT_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_SYNCOUT_EN_ENABLED        (0x01U << PF8X_F_SYNCOUT_EN_SHIFT)

/******************************************************************************/
/* COINCELL_CTRL - Type: RW                                                   */
/******************************************************************************/

#define PF8X_COINCELL_CTRL_ADDR          0x41U
#define PF8X_COINCELL_CTRL_DEFAULT       0x00U

/**
 * Coin cell charging voltage selection
 */
#define PF8X_F_VCOIN_MASK                0x0FU
/**
 * Enable or disable the coin-cell charger during the QPU_Off state
 */
#define PF8X_F_COINCHG_OFF_MASK          0x10U
/**
 * Coincell charger enable in the System ON modes
 */
#define PF8X_F_COINCHG_EN_MASK           0x20U

/**
 * Coin cell charging voltage selection
 */
#define PF8X_F_VCOIN_SHIFT               0x00U
/**
 * Enable or disable the coin-cell charger during the QPU_Off state
 */
#define PF8X_F_COINCHG_OFF_SHIFT         0x04U
/**
 * Coincell charger enable in the System ON modes
 */
#define PF8X_F_COINCHG_EN_SHIFT          0x05U

/**
 * 1.8 V
 */
#define PF8X_F_VCOIN_1_8_V               (0x00U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.0 V
 */
#define PF8X_F_VCOIN_2_0_V               (0x01U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.1 V
 */
#define PF8X_F_VCOIN_2_1_V               (0x02U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.2 V
 */
#define PF8X_F_VCOIN_2_2_V               (0x03U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.3 V
 */
#define PF8X_F_VCOIN_2_3_V               (0x04U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.4 V
 */
#define PF8X_F_VCOIN_2_4_V               (0x05U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.5 V
 */
#define PF8X_F_VCOIN_2_5_V               (0x06U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.6 V
 */
#define PF8X_F_VCOIN_2_6_V               (0x07U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.7 V
 */
#define PF8X_F_VCOIN_2_7_V               (0x08U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.8 V
 */
#define PF8X_F_VCOIN_2_8_V               (0x09U << PF8X_F_VCOIN_SHIFT)
/**
 * 2.9 V
 */
#define PF8X_F_VCOIN_2_9_V               (0x0AU << PF8X_F_VCOIN_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VCOIN_3_0_V               (0x0BU << PF8X_F_VCOIN_SHIFT)
/**
 * 3.1 V
 */
#define PF8X_F_VCOIN_3_1_V               (0x0CU << PF8X_F_VCOIN_SHIFT)
/**
 * 3.2 V
 */
#define PF8X_F_VCOIN_3_2_V               (0x0DU << PF8X_F_VCOIN_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VCOIN_3_3_V               (0x0EU << PF8X_F_VCOIN_SHIFT)
/**
 * 3.6 V
 */
#define PF8X_F_VCOIN_3_6_V               (0x0FU << PF8X_F_VCOIN_SHIFT)

/**
 * Coincell disabled in the QPU_OFF state
 */
#define PF8X_F_COINCHG_OFF_DISABLED      (0x00U << PF8X_F_COINCHG_OFF_SHIFT)
/**
 * Coincell Enabled in the QPU_OFF state
 */
#define PF8X_F_COINCHG_OFF_ENABLED       (0x01U << PF8X_F_COINCHG_OFF_SHIFT)

/**
 * Disabled (default)
 */
#define PF8X_F_COINCHG_EN_DISABLED       (0x00U << PF8X_F_COINCHG_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_COINCHG_EN_ENABLED        (0x01U << PF8X_F_COINCHG_EN_SHIFT)

/******************************************************************************/
/* PWRON - Type: RW                                                           */
/******************************************************************************/

#define PF8X_PWRON_ADDR                  0x42U
#define PF8X_PWRON_DEFAULT               0x00U

/**
 * Sets PWRON push duration for hard reset (4, 8, 12, 16 second) in Edge sensitive mode
 */
#define PF8X_F_TRESET_MASK               0x03U
/**
 * Pressing the PWRON button for longer that TRESET generate an PMIC Reset
 */
#define PF8X_F_PWRON_RST_EN_MASK         0x04U
/**
 * Sets the PWRON  debounce timer (32, 32, 125, 750 ms)
 */
#define PF8X_F_PWRON_DBNC_MASK           0x18U

/**
 * Sets PWRON push duration for hard reset (4, 8, 12, 16 second) in Edge sensitive mode
 */
#define PF8X_F_TRESET_SHIFT              0x00U
/**
 * Pressing the PWRON button for longer that TRESET generate an PMIC Reset
 */
#define PF8X_F_PWRON_RST_EN_SHIFT        0x02U
/**
 * Sets the PWRON  debounce timer (32, 32, 125, 750 ms)
 */
#define PF8X_F_PWRON_DBNC_SHIFT          0x03U

/**
 * 2 s
 */
#define PF8X_F_TRESET_2_S                (0x00U << PF8X_F_TRESET_SHIFT)
/**
 * 4 s
 */
#define PF8X_F_TRESET_4_S                (0x01U << PF8X_F_TRESET_SHIFT)
/**
 * 8 s
 */
#define PF8X_F_TRESET_8_S                (0x02U << PF8X_F_TRESET_SHIFT)
/**
 * 16 s
 */
#define PF8X_F_TRESET_16_S               (0x03U << PF8X_F_TRESET_SHIFT)

/**
 * PMIC shuts down after push button pressed for longer than TRESET
 */
#define PF8X_F_PWRON_RST_EN_SHUTDOWN     (0x00U << PF8X_F_PWRON_RST_EN_SHIFT)
/**
 * PMIC reset after push button pressed for longer than TRESET.
 */
#define PF8X_F_PWRON_RST_EN_RESET        (0x01U << PF8X_F_PWRON_RST_EN_SHIFT)

/**
 * 32 ms
 */
#define PF8X_F_PWRON_DBNC_32_MS          (0x00U << PF8X_F_PWRON_DBNC_SHIFT)
/**
 * 125 ms
 */
#define PF8X_F_PWRON_DBNC_125_MS         (0x02U << PF8X_F_PWRON_DBNC_SHIFT)
/**
 * 750 ms
 */
#define PF8X_F_PWRON_DBNC_750_MS         (0x03U << PF8X_F_PWRON_DBNC_SHIFT)

/******************************************************************************/
/* WD_CONFIG - Type: RW                                                       */
/******************************************************************************/

#define PF8X_WD_CONFIG_ADDR              0x43U
#define PF8X_WD_CONFIG_DEFAULT           0x00U

/**
 * Selects window duration between 1ms and 32.768 s
 */
#define PF8X_F_WD_DURATION_MASK          0x0FU

/**
 * Selects window duration between 1ms and 32.768 s
 */
#define PF8X_F_WD_DURATION_SHIFT         0x00U

/**
 * 1 ms
 */
#define PF8X_F_WD_DURATION_1_MS          (0x00U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 2 ms
 */
#define PF8X_F_WD_DURATION_2_MS          (0x01U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 4 ms
 */
#define PF8X_F_WD_DURATION_4_MS          (0x02U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 8 ms
 */
#define PF8X_F_WD_DURATION_8_MS          (0x03U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 16 ms
 */
#define PF8X_F_WD_DURATION_16_MS         (0x04U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 32 ms
 */
#define PF8X_F_WD_DURATION_32_MS         (0x05U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 64 ms
 */
#define PF8X_F_WD_DURATION_64_MS         (0x06U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 128 ms
 */
#define PF8X_F_WD_DURATION_128_MS        (0x07U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 256 ms
 */
#define PF8X_F_WD_DURATION_256_MS        (0x08U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 512 ms
 */
#define PF8X_F_WD_DURATION_512_MS        (0x09U << PF8X_F_WD_DURATION_SHIFT)
/**
 * 1024 ms
 */
#define PF8X_F_WD_DURATION_1024_MS       (0x0AU << PF8X_F_WD_DURATION_SHIFT)
/**
 * 2048 ms
 */
#define PF8X_F_WD_DURATION_2048_MS       (0x0BU << PF8X_F_WD_DURATION_SHIFT)
/**
 * 4096 ms
 */
#define PF8X_F_WD_DURATION_4096_MS       (0x0CU << PF8X_F_WD_DURATION_SHIFT)
/**
 * 8192 ms
 */
#define PF8X_F_WD_DURATION_8192_MS       (0x0DU << PF8X_F_WD_DURATION_SHIFT)
/**
 * 16384 ms
 */
#define PF8X_F_WD_DURATION_16384_MS      (0x0EU << PF8X_F_WD_DURATION_SHIFT)
/**
 * 32768 ms
 */
#define PF8X_F_WD_DURATION_32768_MS      (0x0FU << PF8X_F_WD_DURATION_SHIFT)

/******************************************************************************/
/* WD_CLEAR - Type: RW                                                        */
/******************************************************************************/

#define PF8X_WD_CLEAR_ADDR               0x44U
#define PF8X_WD_CLEAR_DEFAULT            0x00U

/**
 * When the WD_TIMER is counting, the WD_CLEAR flag is set to 1. Clearing the WD_CLEAR flag within the valid window 
 * shall be interpreted as a successful watchdog refresh and the WD_TIMER shall be reset.The WD_CLEAR flag shall be clear 
 * by writing a 1 to this bit.
 */
#define PF8X_F_WD_CLEAR_MASK             0x01U

/**
 * When the WD_TIMER is counting, the WD_CLEAR flag is set to 1. Clearing the WD_CLEAR flag within the valid window 
 * shall be interpreted as a successful watchdog refresh and the WD_TIMER shall be reset.The WD_CLEAR flag shall be clear 
 * by writing a 1 to this bit.
 */
#define PF8X_F_WD_CLEAR_SHIFT            0x00U

/**
 * 0
 */
#define PF8X_F_WD_CLEAR_0                (0x00U << PF8X_F_WD_CLEAR_SHIFT)
/**
 * 1
 */
#define PF8X_F_WD_CLEAR_1                (0x01U << PF8X_F_WD_CLEAR_SHIFT)

/******************************************************************************/
/* WD_EXPIRE - Type: RW                                                       */
/******************************************************************************/

#define PF8X_WD_EXPIRE_ADDR              0x45U
#define PF8X_WD_EXPIRE_DEFAULT           0x00U

/**
 * Counts the number of time the Timer has expired. When timer expires it is incremented by 2. When Timer is cleared 
 * successfully, it is decreased by 1.
 */
#define PF8X_F_WD_EXPIRE_CNT_MASK        0x07U
/**
 * Sets the Maximum number of times the WD timer is allowed to expire before starting a WD reset.
 */
#define PF8X_F_WD_MAX_EXPIRE_MASK        0x70U

/**
 * Counts the number of time the Timer has expired. When timer expires it is incremented by 2. When Timer is cleared 
 * successfully, it is decreased by 1.
 */
#define PF8X_F_WD_EXPIRE_CNT_SHIFT       0x00U
/**
 * Sets the Maximum number of times the WD timer is allowed to expire before starting a WD reset.
 */
#define PF8X_F_WD_MAX_EXPIRE_SHIFT       0x04U

/**
 * 0
 */
#define PF8X_F_WD_EXPIRE_CNT_0           (0x00U << PF8X_F_WD_EXPIRE_CNT_SHIFT)
/**
 * 1
 */
#define PF8X_F_WD_EXPIRE_CNT_1           (0x01U << PF8X_F_WD_EXPIRE_CNT_SHIFT)
/**
 * 2
 */
#define PF8X_F_WD_EXPIRE_CNT_2           (0x02U << PF8X_F_WD_EXPIRE_CNT_SHIFT)
/**
 * 3
 */
#define PF8X_F_WD_EXPIRE_CNT_3           (0x03U << PF8X_F_WD_EXPIRE_CNT_SHIFT)
/**
 * 4
 */
#define PF8X_F_WD_EXPIRE_CNT_4           (0x04U << PF8X_F_WD_EXPIRE_CNT_SHIFT)
/**
 * 5
 */
#define PF8X_F_WD_EXPIRE_CNT_5           (0x05U << PF8X_F_WD_EXPIRE_CNT_SHIFT)
/**
 * 6
 */
#define PF8X_F_WD_EXPIRE_CNT_6           (0x06U << PF8X_F_WD_EXPIRE_CNT_SHIFT)
/**
 * 7
 */
#define PF8X_F_WD_EXPIRE_CNT_7           (0x07U << PF8X_F_WD_EXPIRE_CNT_SHIFT)

/**
 * 0
 */
#define PF8X_F_WD_MAX_EXPIRE_0           (0x00U << PF8X_F_WD_MAX_EXPIRE_SHIFT)
/**
 * 1
 */
#define PF8X_F_WD_MAX_EXPIRE_1           (0x01U << PF8X_F_WD_MAX_EXPIRE_SHIFT)
/**
 * 2
 */
#define PF8X_F_WD_MAX_EXPIRE_2           (0x02U << PF8X_F_WD_MAX_EXPIRE_SHIFT)
/**
 * 3
 */
#define PF8X_F_WD_MAX_EXPIRE_3           (0x03U << PF8X_F_WD_MAX_EXPIRE_SHIFT)
/**
 * 4
 */
#define PF8X_F_WD_MAX_EXPIRE_4           (0x04U << PF8X_F_WD_MAX_EXPIRE_SHIFT)
/**
 * 5
 */
#define PF8X_F_WD_MAX_EXPIRE_5           (0x05U << PF8X_F_WD_MAX_EXPIRE_SHIFT)
/**
 * 6
 */
#define PF8X_F_WD_MAX_EXPIRE_6           (0x06U << PF8X_F_WD_MAX_EXPIRE_SHIFT)
/**
 * 7
 */
#define PF8X_F_WD_MAX_EXPIRE_7           (0x07U << PF8X_F_WD_MAX_EXPIRE_SHIFT)

/******************************************************************************/
/* WD_COUNTER - Type: RW                                                      */
/******************************************************************************/

#define PF8X_WD_COUNTER_ADDR             0x46U
#define PF8X_WD_COUNTER_DEFAULT          0x00U

/**
 * Watchdog failure event counter. After a succesful recovery from a WD fault, MCU is expected to reset these bits.
 */
#define PF8X_F_WD_EVENT_CNT_MASK         0x0FU
/**
 * Sets the Max number of WATCHDOG failures before proceeding to Fail safe transition.
 */
#define PF8X_F_WD_MAX_CNT_MASK           0xF0U

/**
 * Watchdog failure event counter. After a succesful recovery from a WD fault, MCU is expected to reset these bits.
 */
#define PF8X_F_WD_EVENT_CNT_SHIFT        0x00U
/**
 * Sets the Max number of WATCHDOG failures before proceeding to Fail safe transition.
 */
#define PF8X_F_WD_MAX_CNT_SHIFT          0x04U

/**
 * 0
 */
#define PF8X_F_WD_EVENT_CNT_0            (0x00U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 1
 */
#define PF8X_F_WD_EVENT_CNT_1            (0x01U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 2
 */
#define PF8X_F_WD_EVENT_CNT_2            (0x02U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 3
 */
#define PF8X_F_WD_EVENT_CNT_3            (0x03U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 4
 */
#define PF8X_F_WD_EVENT_CNT_4            (0x04U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 5
 */
#define PF8X_F_WD_EVENT_CNT_5            (0x05U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 6
 */
#define PF8X_F_WD_EVENT_CNT_6            (0x06U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 7
 */
#define PF8X_F_WD_EVENT_CNT_7            (0x07U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 8
 */
#define PF8X_F_WD_EVENT_CNT_8            (0x08U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 9
 */
#define PF8X_F_WD_EVENT_CNT_9            (0x09U << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 10
 */
#define PF8X_F_WD_EVENT_CNT_10           (0x0AU << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 11
 */
#define PF8X_F_WD_EVENT_CNT_11           (0x0BU << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 12
 */
#define PF8X_F_WD_EVENT_CNT_12           (0x0CU << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 13
 */
#define PF8X_F_WD_EVENT_CNT_13           (0x0DU << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 14
 */
#define PF8X_F_WD_EVENT_CNT_14           (0x0EU << PF8X_F_WD_EVENT_CNT_SHIFT)
/**
 * 15
 */
#define PF8X_F_WD_EVENT_CNT_15           (0x0FU << PF8X_F_WD_EVENT_CNT_SHIFT)

/**
 * 0
 */
#define PF8X_F_WD_MAX_CNT_0              (0x00U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 1
 */
#define PF8X_F_WD_MAX_CNT_1              (0x01U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 2
 */
#define PF8X_F_WD_MAX_CNT_2              (0x02U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 3
 */
#define PF8X_F_WD_MAX_CNT_3              (0x03U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 4
 */
#define PF8X_F_WD_MAX_CNT_4              (0x04U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 5
 */
#define PF8X_F_WD_MAX_CNT_5              (0x05U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 6
 */
#define PF8X_F_WD_MAX_CNT_6              (0x06U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 7
 */
#define PF8X_F_WD_MAX_CNT_7              (0x07U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 8
 */
#define PF8X_F_WD_MAX_CNT_8              (0x08U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 9
 */
#define PF8X_F_WD_MAX_CNT_9              (0x09U << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 10
 */
#define PF8X_F_WD_MAX_CNT_10             (0x0AU << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 11
 */
#define PF8X_F_WD_MAX_CNT_11             (0x0BU << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 12
 */
#define PF8X_F_WD_MAX_CNT_12             (0x0CU << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 13
 */
#define PF8X_F_WD_MAX_CNT_13             (0x0DU << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 14
 */
#define PF8X_F_WD_MAX_CNT_14             (0x0EU << PF8X_F_WD_MAX_CNT_SHIFT)
/**
 * 15
 */
#define PF8X_F_WD_MAX_CNT_15             (0x0FU << PF8X_F_WD_MAX_CNT_SHIFT)

/******************************************************************************/
/* FAULT_COUNTER - Type: RW                                                   */
/******************************************************************************/

#define PF8X_FAULT_COUNTER_ADDR          0x47U
#define PF8X_FAULT_COUNTER_DEFAULT       0x00U

/**
 * Counts the number of times the device goes into a regulator Fault condition. After a Fault condition is cleared the 
 * MCU is expected to reset this counter
 */
#define PF8X_F_FAULT_CNT_MASK            0x0FU
/**
 * Set the maximum number of times the device can go into a regulator Fault condition prior to move to start a power 
 * down sequence and move to the fail-safe transition.
 */
#define PF8X_F_FAULT_MAX_CNT_MASK        0xF0U

/**
 * Counts the number of times the device goes into a regulator Fault condition. After a Fault condition is cleared the 
 * MCU is expected to reset this counter
 */
#define PF8X_F_FAULT_CNT_SHIFT           0x00U
/**
 * Set the maximum number of times the device can go into a regulator Fault condition prior to move to start a power 
 * down sequence and move to the fail-safe transition.
 */
#define PF8X_F_FAULT_MAX_CNT_SHIFT       0x04U

/**
 * 0
 */
#define PF8X_F_FAULT_CNT_0               (0x00U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 1
 */
#define PF8X_F_FAULT_CNT_1               (0x01U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 2
 */
#define PF8X_F_FAULT_CNT_2               (0x02U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 3
 */
#define PF8X_F_FAULT_CNT_3               (0x03U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 4
 */
#define PF8X_F_FAULT_CNT_4               (0x04U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 5
 */
#define PF8X_F_FAULT_CNT_5               (0x05U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 6
 */
#define PF8X_F_FAULT_CNT_6               (0x06U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 7
 */
#define PF8X_F_FAULT_CNT_7               (0x07U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 8
 */
#define PF8X_F_FAULT_CNT_8               (0x08U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 9
 */
#define PF8X_F_FAULT_CNT_9               (0x09U << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 10
 */
#define PF8X_F_FAULT_CNT_10              (0x0AU << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 11
 */
#define PF8X_F_FAULT_CNT_11              (0x0BU << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 12
 */
#define PF8X_F_FAULT_CNT_12              (0x0CU << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 13
 */
#define PF8X_F_FAULT_CNT_13              (0x0DU << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 14
 */
#define PF8X_F_FAULT_CNT_14              (0x0EU << PF8X_F_FAULT_CNT_SHIFT)
/**
 * 15
 */
#define PF8X_F_FAULT_CNT_15              (0x0FU << PF8X_F_FAULT_CNT_SHIFT)

/**
 * Disable
 */
#define PF8X_F_FAULT_MAX_CNT_DISABLE     (0x00U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 1
 */
#define PF8X_F_FAULT_MAX_CNT_1           (0x01U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 2
 */
#define PF8X_F_FAULT_MAX_CNT_2           (0x02U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 3
 */
#define PF8X_F_FAULT_MAX_CNT_3           (0x03U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 4
 */
#define PF8X_F_FAULT_MAX_CNT_4           (0x04U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 5
 */
#define PF8X_F_FAULT_MAX_CNT_5           (0x05U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 6
 */
#define PF8X_F_FAULT_MAX_CNT_6           (0x06U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 7
 */
#define PF8X_F_FAULT_MAX_CNT_7           (0x07U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 8
 */
#define PF8X_F_FAULT_MAX_CNT_8           (0x08U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 9
 */
#define PF8X_F_FAULT_MAX_CNT_9           (0x09U << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 10
 */
#define PF8X_F_FAULT_MAX_CNT_10          (0x0AU << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 11
 */
#define PF8X_F_FAULT_MAX_CNT_11          (0x0BU << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 12
 */
#define PF8X_F_FAULT_MAX_CNT_12          (0x0CU << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 13
 */
#define PF8X_F_FAULT_MAX_CNT_13          (0x0DU << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 14
 */
#define PF8X_F_FAULT_MAX_CNT_14          (0x0EU << PF8X_F_FAULT_MAX_CNT_SHIFT)
/**
 * 15
 */
#define PF8X_F_FAULT_MAX_CNT_15          (0x0FU << PF8X_F_FAULT_MAX_CNT_SHIFT)

/******************************************************************************/
/* FAULT_TIMERS - Type: RW                                                    */
/******************************************************************************/

#define PF8X_FAULT_TIMERS_ADDR           0x49U
#define PF8X_FAULT_TIMERS_DEFAULT        0x00U

/**
 * Set the duration of the Fault Timer. From 1 to 2056 ms
 */
#define PF8X_F_TIMER_FAULT_MASK          0x0FU

/**
 * Set the duration of the Fault Timer. From 1 to 2056 ms
 */
#define PF8X_F_TIMER_FAULT_SHIFT         0x00U

/**
 * Timer Value 1 ms
 */
#define PF8X_F_TIMER_FAULT_1_MS          (0x00U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 2 ms
 */
#define PF8X_F_TIMER_FAULT_2_MS          (0x01U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 4 ms
 */
#define PF8X_F_TIMER_FAULT_4_MS          (0x02U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 8 ms
 */
#define PF8X_F_TIMER_FAULT_8_MS          (0x03U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 16 ms
 */
#define PF8X_F_TIMER_FAULT_16_MS         (0x04U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 32 ms
 */
#define PF8X_F_TIMER_FAULT_32_MS         (0x05U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 64 ms
 */
#define PF8X_F_TIMER_FAULT_64_MS         (0x06U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 128 ms
 */
#define PF8X_F_TIMER_FAULT_128_MS        (0x07U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 256 ms
 */
#define PF8X_F_TIMER_FAULT_256_MS        (0x08U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 512 ms
 */
#define PF8X_F_TIMER_FAULT_512_MS        (0x09U << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 1024 ms
 */
#define PF8X_F_TIMER_FAULT_1024_MS       (0x0AU << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value 2056 ms
 */
#define PF8X_F_TIMER_FAULT_2056_MS       (0x0BU << PF8X_F_TIMER_FAULT_SHIFT)
/**
 * Timer Value Disabled
 */
#define PF8X_F_TIMER_FAULT_DISABLED      (0x0FU << PF8X_F_TIMER_FAULT_SHIFT)

/******************************************************************************/
/* AMUX - Type: RW                                                            */
/******************************************************************************/

#define PF8X_AMUX_ADDR                   0x4AU
#define PF8X_AMUX_DEFAULT                0x00U

/**
 * Selec the Analog Input to acces by the AMUX pin. Default value upon power off Is 0x0000
 */
#define PF8X_F_AMUX_SEL_MASK             0x1FU
/**
 * AMUX Enable Bit
 */
#define PF8X_F_AMUX_EN_MASK              0x20U

/**
 * Selec the Analog Input to acces by the AMUX pin. Default value upon power off Is 0x0000
 */
#define PF8X_F_AMUX_SEL_SHIFT            0x00U
/**
 * AMUX Enable Bit
 */
#define PF8X_F_AMUX_EN_SHIFT             0x05U

/**
 * AMUX Selection Disabled, Internal signal Dividing Ratio N/A
 */
#define PF8X_F_AMUX_SEL_DISABLED         (0x00U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection VIN, Internal signal Dividing Ratio 4
 */
#define PF8X_F_AMUX_SEL_VIN              (0x01U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection VSNVS, Internal signal Dividing Ratio 3.5
 */
#define PF8X_F_AMUX_SEL_VSNVS            (0x02U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection LICELL, Internal signal Dividing Ratio 3
 */
#define PF8X_F_AMUX_SEL_LICELL           (0x03U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection SW1_FB, Internal signal Dividing Ratio 1.25 (1.8V setting) 1 (All other settings)
 */
#define PF8X_F_AMUX_SEL_SW1_FB           (0x04U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection SW2_FB, Internal signal Dividing Ratio 1.25 (1.8V setting) 1 (All other settings)
 */
#define PF8X_F_AMUX_SEL_SW2_FB           (0x05U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection SW3_FB, Internal signal Dividing Ratio 1.25 (1.8V setting) 1 (All other settings)
 */
#define PF8X_F_AMUX_SEL_SW3_FB           (0x06U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection SW4_FB, Internal signal Dividing Ratio 1.25 (1.8V setting) 1 (All other settings)
 */
#define PF8X_F_AMUX_SEL_SW4_FB           (0x07U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection SW5_FB, Internal signal Dividing Ratio 1.25 (1.8V setting) 1 (All other settings)
 */
#define PF8X_F_AMUX_SEL_SW5_FB           (0x08U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection SW6_FB, Internal signal Dividing Ratio 1.25 (1.8V setting) 1 (All other settings)
 */
#define PF8X_F_AMUX_SEL_SW6_FB           (0x09U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection SW7_FB, Internal signal Dividing Ratio 10/3.5 = 2.86
 */
#define PF8X_F_AMUX_SEL_SW7_FB           (0x0AU << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection LDO1, Internal signal Dividing Ratio 10/3 = 2.33
 */
#define PF8X_F_AMUX_SEL_LDO1             (0x0BU << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection LDO2, Internal signal Dividing Ratio 10/3 = 2.33
 */
#define PF8X_F_AMUX_SEL_LDO2             (0x0CU << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection LDO3, Internal signal Dividing Ratio 10/3 = 2.33
 */
#define PF8X_F_AMUX_SEL_LDO3             (0x0DU << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection LDO4, Internal signal Dividing Ratio 10/3 = 2.33
 */
#define PF8X_F_AMUX_SEL_LDO4             (0x0EU << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_IC, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_IC          (0x0FU << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_SW1, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_SW1         (0x10U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_SW2, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_SW2         (0x11U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_SW3, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_SW3         (0x12U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_SW4, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_SW4         (0x13U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_SW5, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_SW5         (0x14U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_SW6, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_SW6         (0x15U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_SW7, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_SW7         (0x16U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_LDO1_2, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_LDO1_2      (0x17U << PF8X_F_AMUX_SEL_SHIFT)
/**
 * AMUX Selection TEMP_LDO3_4, Internal signal Dividing Ratio 1
 */
#define PF8X_F_AMUX_SEL_TEMP_LDO3_4      (0x18U << PF8X_F_AMUX_SEL_SHIFT)

/**
 * Disabled (default)
 */
#define PF8X_F_AMUX_EN_DISABLED          (0x00U << PF8X_F_AMUX_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_AMUX_EN_ENABLED           (0x01U << PF8X_F_AMUX_EN_SHIFT)

/******************************************************************************/
/* SW1_CONFIG1 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW1_CONFIG1_ADDR            0x4DU
#define PF8X_SW1_CONFIG1_DEFAULT         0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW1_PG_EN_MASK            0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW1_WDBYPASS_MASK         0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW1_ILIM_STATE_MASK       0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW1_OV_STATE_MASK         0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW1_UV_STATE_MASK         0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW1_ILIM_BYPASS_MASK      0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW1_OV_BYPASS_MASK        0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW1_UV_BYPASS_MASK        0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW1_PG_EN_SHIFT           0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW1_WDBYPASS_SHIFT        0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW1_ILIM_STATE_SHIFT      0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW1_OV_STATE_SHIFT        0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW1_UV_STATE_SHIFT        0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW1_ILIM_BYPASS_SHIFT     0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW1_OV_BYPASS_SHIFT       0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW1_UV_BYPASS_SHIFT       0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_SW1_PG_EN_DISABLED        (0x00U << PF8X_F_SW1_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_SW1_PG_EN_ENABLED         (0x01U << PF8X_F_SW1_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_SW1_WDBYPASS_RESET        (0x00U << PF8X_F_SW1_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_SW1_WDBYPASS_BYPASS       (0x01U << PF8X_F_SW1_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW1_ILIM_STATE_DISABLED   (0x00U << PF8X_F_SW1_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW1_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW1_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW1_OV_STATE_DISABLED     (0x00U << PF8X_F_SW1_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW1_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW1_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW1_UV_STATE_DISABLED     (0x00U << PF8X_F_SW1_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW1_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW1_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW1_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW1_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW1_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW1_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW1_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW1_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW1_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW1_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW1_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW1_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW1_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW1_UV_BYPASS_SHIFT)

/******************************************************************************/
/* SW1_CONFIG2 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW1_CONFIG2_ADDR            0x4EU
#define PF8X_SW1_CONFIG2_DEFAULT         0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW1PHASE_MASK             0x07U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW1ILIM_MASK              0x18U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW1DVS_RAMP_MASK          0x20U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW1_FLT_REN_MASK          0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW1PHASE_SHIFT            0x00U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW1ILIM_SHIFT             0x03U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW1DVS_RAMP_SHIFT         0x05U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW1_FLT_REN_SHIFT         0x07U

/**
 * 45_
 */
#define PF8X_F_SW1PHASE_45               (0x00U << PF8X_F_SW1PHASE_SHIFT)
/**
 * 90_
 */
#define PF8X_F_SW1PHASE_90               (0x01U << PF8X_F_SW1PHASE_SHIFT)
/**
 * 135_
 */
#define PF8X_F_SW1PHASE_135              (0x02U << PF8X_F_SW1PHASE_SHIFT)
/**
 * 180_
 */
#define PF8X_F_SW1PHASE_180              (0x03U << PF8X_F_SW1PHASE_SHIFT)
/**
 * 225_
 */
#define PF8X_F_SW1PHASE_225              (0x04U << PF8X_F_SW1PHASE_SHIFT)
/**
 * 270_
 */
#define PF8X_F_SW1PHASE_270              (0x05U << PF8X_F_SW1PHASE_SHIFT)
/**
 * 315_
 */
#define PF8X_F_SW1PHASE_315              (0x06U << PF8X_F_SW1PHASE_SHIFT)
/**
 * 0_
 */
#define PF8X_F_SW1PHASE_0                (0x07U << PF8X_F_SW1PHASE_SHIFT)

/**
 * 2.1 A
 */
#define PF8X_F_SW1ILIM_2_1_A             (0x00U << PF8X_F_SW1ILIM_SHIFT)
/**
 * 2.6 A
 */
#define PF8X_F_SW1ILIM_2_6_A             (0x01U << PF8X_F_SW1ILIM_SHIFT)
/**
 * 3.0 A
 */
#define PF8X_F_SW1ILIM_3_0_A             (0x02U << PF8X_F_SW1ILIM_SHIFT)
/**
 * 4.5 A
 */
#define PF8X_F_SW1ILIM_4_5_A             (0x03U << PF8X_F_SW1ILIM_SHIFT)

/**
 * 6.25 mV/us
 */
#define PF8X_F_SW1DVS_RAMP_6_25          (0x00U << PF8X_F_SW1DVS_RAMP_SHIFT)
/**
 * 12.5 mV/us
 */
#define PF8X_F_SW1DVS_RAMP_12_5          (0x01U << PF8X_F_SW1DVS_RAMP_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_SW1_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_SW1_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_SW1_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_SW1_FLT_REN_SHIFT)

/******************************************************************************/
/* SW1_PWRUP - Type: RW                                                       */
/******************************************************************************/

#define PF8X_SW1_PWRUP_ADDR              0x4FU
#define PF8X_SW1_PWRUP_DEFAULT           0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW1_SEQ_MASK              0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW1_SEQ_SHIFT             0x00U

/******************************************************************************/
/* SW1_MODE - Type: RW                                                        */
/******************************************************************************/

#define PF8X_SW1_MODE_ADDR               0x50U
#define PF8X_SW1_MODE_DEFAULT            0x00U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW1_RUN_MODE_MASK         0x03U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW1_STBY_MODE_MASK        0x0CU
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW1_PDGRP_MASK            0x30U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW1_RUN_MODE_SHIFT        0x00U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW1_STBY_MODE_SHIFT       0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW1_PDGRP_SHIFT           0x04U

/**
 * OFF
 */
#define PF8X_F_SW1_RUN_MODE_OFF          (0x00U << PF8X_F_SW1_RUN_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW1_RUN_MODE_PWM          (0x01U << PF8X_F_SW1_RUN_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW1_RUN_MODE_PFM          (0x02U << PF8X_F_SW1_RUN_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW1_RUN_MODE_AUTO         (0x03U << PF8X_F_SW1_RUN_MODE_SHIFT)

/**
 * OFF
 */
#define PF8X_F_SW1_STBY_MODE_OFF         (0x00U << PF8X_F_SW1_STBY_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW1_STBY_MODE_PWM         (0x01U << PF8X_F_SW1_STBY_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW1_STBY_MODE_PFM         (0x02U << PF8X_F_SW1_STBY_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW1_STBY_MODE_AUTO        (0x03U << PF8X_F_SW1_STBY_MODE_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_SW1_PDGRP_GROUP_4         (0x00U << PF8X_F_SW1_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_SW1_PDGRP_GROUP_3         (0x01U << PF8X_F_SW1_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_SW1_PDGRP_GROUP_2         (0x02U << PF8X_F_SW1_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_SW1_PDGRP_GROUP_1         (0x03U << PF8X_F_SW1_PDGRP_SHIFT)

/******************************************************************************/
/* SW1_RUN_VOLT - Type: RW                                                    */
/******************************************************************************/

#define PF8X_SW1_RUN_VOLT_ADDR           0x51U
#define PF8X_SW1_RUN_VOLT_DEFAULT        0x00U

/**
 * SW1 Output voltage configuration in given Mode. It uses the OTP_VSW1 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW1_RUN_MASK             0xFFU

/**
 * SW1 Output voltage configuration in given Mode. It uses the OTP_VSW1 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW1_RUN_SHIFT            0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW1_RUN_0_4_V            (0x00U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW1_RUN_0_40625_V        (0x01U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW1_RUN_0_4125_V         (0x02U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW1_RUN_0_41875_V        (0x03U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW1_RUN_0_425_V          (0x04U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW1_RUN_0_43125_V        (0x05U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW1_RUN_0_4375_V         (0x06U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW1_RUN_0_44375V         (0x07U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW1_RUN_0_45_V           (0x08U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW1_RUN_0_45625_V        (0x09U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW1_RUN_0_4625_V         (0x0AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW1_RUN_0_46875_V        (0x0BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW1_RUN_0_475_V          (0x0CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW1_RUN_0_48125_V        (0x0DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW1_RUN_0_4875_V         (0x0EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW1_RUN_0_49375_V        (0x0FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW1_RUN_0_5_V            (0x10U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW1_RUN_0_50625_V        (0x11U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW1_RUN_0_5125_V         (0x12U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW1_RUN_0_51875_V        (0x13U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW1_RUN_0_525_V          (0x14U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW1_RUN_0_53125_V        (0x15U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW1_RUN_0_5375_V         (0x16U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW1_RUN_0_54375_V        (0x17U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW1_RUN_0_55_V           (0x18U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW1_RUN_0_55625_V        (0x19U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW1_RUN_0_5625_V         (0x1AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW1_RUN_0_56875_V        (0x1BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW1_RUN_0_575_V          (0x1CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW1_RUN_0_58125_V        (0x1DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW1_RUN_0_5875_V         (0x1EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW1_RUN_0_59375_V        (0x1FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW1_RUN_0_6_V            (0x20U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW1_RUN_0_60625_V        (0x21U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW1_RUN_0_6125_V         (0x22U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW1_RUN_0_61875_V        (0x23U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW1_RUN_0_625_V          (0x24U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW1_RUN_0_63125_V        (0x25U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW1_RUN_0_6375_V         (0x26U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW1_RUN_0_64375_V        (0x27U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW1_RUN_0_65_V           (0x28U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW1_RUN_0_65625_V        (0x29U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW1_RUN_0_6625_V         (0x2AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW1_RUN_0_66875_V        (0x2BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW1_RUN_0_675_V          (0x2CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW1_RUN_0_68125_V        (0x2DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW1_RUN_0_6875_V         (0x2EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW1_RUN_0_69375_V        (0x2FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW1_RUN_0_7_V            (0x30U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW1_RUN_0_70625_V        (0x31U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW1_RUN_0_7125_V         (0x32U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW1_RUN_0_71875_V        (0x33U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW1_RUN_0_725_V          (0x34U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW1_RUN_0_73125_V        (0x35U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW1_RUN_0_7375_V         (0x36U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW1_RUN_0_74375_V        (0x37U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW1_RUN_0_75_V           (0x38U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW1_RUN_0_75625_V        (0x39U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW1_RUN_0_7625_V         (0x3AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW1_RUN_0_76875_V        (0x3BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW1_RUN_0_775_V          (0x3CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW1_RUN_0_78125_V        (0x3DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW1_RUN_0_7875_V         (0x3EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW1_RUN_0_79375_V        (0x3FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW1_RUN_0_8_V            (0x40U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW1_RUN_0_80625_V        (0x41U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW1_RUN_0_8125_V         (0x42U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW1_RUN_0_81875_V        (0x43U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW1_RUN_0_825_V          (0x44U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW1_RUN_0_83125_V        (0x45U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW1_RUN_0_8375_V         (0x46U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW1_RUN_0_84375_V        (0x47U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW1_RUN_0_85_V           (0x48U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW1_RUN_0_85625_V        (0x49U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW1_RUN_0_8625_V         (0x4AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW1_RUN_0_86875_V        (0x4BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW1_RUN_0_875_V          (0x4CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW1_RUN_0_88125_V        (0x4DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW1_RUN_0_8875_V         (0x4EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW1_RUN_0_89375_V        (0x4FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW1_RUN_0_9_V            (0x50U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW1_RUN_0_90625_V        (0x51U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW1_RUN_0_9125_V         (0x52U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW1_RUN_0_91875_V        (0x53U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW1_RUN_0_925_V          (0x54U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW1_RUN_0_93125_V        (0x55U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW1_RUN_0_9375_V         (0x56U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW1_RUN_0_94375_V        (0x57U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW1_RUN_0_95_V           (0x58U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW1_RUN_0_95625_V        (0x59U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW1_RUN_0_9625_V         (0x5AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW1_RUN_0_96875_V        (0x5BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW1_RUN_0_975_V          (0x5CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW1_RUN_0_98125_V        (0x5DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW1_RUN_0_9875_V         (0x5EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW1_RUN_0_99375_V        (0x5FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW1_RUN_1_V              (0x60U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW1_RUN_1_00625_V        (0x61U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW1_RUN_1_0125_V         (0x62U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW1_RUN_1_01875_V        (0x63U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW1_RUN_1_025_V          (0x64U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW1_RUN_1_03125_V        (0x65U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW1_RUN_1_0375_V         (0x66U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW1_RUN_1_04375_V        (0x67U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW1_RUN_1_05_V           (0x68U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW1_RUN_1_05625_V        (0x69U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW1_RUN_1_0625_V         (0x6AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW1_RUN_1_06875_V        (0x6BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW1_RUN_1_075_V          (0x6CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW1_RUN_1_08125_V        (0x6DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW1_RUN_1_0875_V         (0x6EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW1_RUN_1_09375_V        (0x6FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW1_RUN_1_1_V            (0x70U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW1_RUN_1_10625_V        (0x71U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW1_RUN_1_1125_V         (0x72U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW1_RUN_1_11875_V        (0x73U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW1_RUN_1_125_V          (0x74U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW1_RUN_1_13125_V        (0x75U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW1_RUN_1_1375_V         (0x76U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW1_RUN_1_14375_V        (0x77U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW1_RUN_1_15_V           (0x78U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW1_RUN_1_15625_V        (0x79U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW1_RUN_1_1625_V         (0x7AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW1_RUN_1_16875_V        (0x7BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW1_RUN_1_175_V          (0x7CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW1_RUN_1_18125_V        (0x7DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW1_RUN_1_1875_V         (0x7EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW1_RUN_1_19375_V        (0x7FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW1_RUN_1_2_V            (0x80U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW1_RUN_1_20625_V        (0x81U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW1_RUN_1_2125_V         (0x82U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW1_RUN_1_21875_V        (0x83U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW1_RUN_1_225_V          (0x84U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW1_RUN_1_23125_V        (0x85U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW1_RUN_1_2375_V         (0x86U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW1_RUN_1_24375_V        (0x87U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW1_RUN_1_25_V           (0x88U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW1_RUN_1_25625_V        (0x89U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW1_RUN_1_2625_V         (0x8AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW1_RUN_1_26875_V        (0x8BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW1_RUN_1_275_V          (0x8CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW1_RUN_1_28125_V        (0x8DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW1_RUN_1_2875_V         (0x8EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW1_RUN_1_29375_V        (0x8FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW1_RUN_1_3_V            (0x90U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW1_RUN_1_30625_V        (0x91U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW1_RUN_1_3125_V         (0x92U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW1_RUN_1_31875_V        (0x93U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW1_RUN_1_325_V          (0x94U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW1_RUN_1_33125_V        (0x95U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW1_RUN_1_3375_V         (0x96U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW1_RUN_1_34375_V        (0x97U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW1_RUN_1_35_V           (0x98U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW1_RUN_1_35625_V        (0x99U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW1_RUN_1_3625_V         (0x9AU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW1_RUN_1_36875_V        (0x9BU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW1_RUN_1_375_V          (0x9CU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW1_RUN_1_38125_V        (0x9DU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW1_RUN_1_3875_V         (0x9EU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW1_RUN_1_39375_V        (0x9FU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW1_RUN_1_4_V            (0xA0U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW1_RUN_1_40625_V        (0xA1U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW1_RUN_1_4125_V         (0xA2U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW1_RUN_1_41875_V        (0xA3U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW1_RUN_1_425_V          (0xA4U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW1_RUN_1_43125_V        (0xA5U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW1_RUN_1_4375_V         (0xA6U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW1_RUN_1_44375_V        (0xA7U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW1_RUN_1_45_V           (0xA8U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW1_RUN_1_45625_V        (0xA9U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW1_RUN_1_4625_V         (0xAAU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW1_RUN_1_46875_V        (0xABU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW1_RUN_1_475_V          (0xACU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW1_RUN_1_48125_V        (0xADU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW1_RUN_1_4875_V         (0xAEU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW1_RUN_1_49375_V        (0xAFU << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW1_RUN_1_5_V            (0xB0U << PF8X_F_VSW1_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW1_RUN_1_8_V            (0xB1U << PF8X_F_VSW1_RUN_SHIFT)

/******************************************************************************/
/* SW1_STBY_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_SW1_STBY_VOLT_ADDR          0x52U
#define PF8X_SW1_STBY_VOLT_DEFAULT       0x00U

/**
 * SW1 Output voltage configuration in given Mode. It uses the OTP_VSW1 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW1_STBY_MASK            0xFFU

/**
 * SW1 Output voltage configuration in given Mode. It uses the OTP_VSW1 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW1_STBY_SHIFT           0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW1_STBY_0_4_V           (0x00U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW1_STBY_0_40625_V       (0x01U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW1_STBY_0_4125_V        (0x02U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW1_STBY_0_41875_V       (0x03U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW1_STBY_0_425_V         (0x04U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW1_STBY_0_43125_V       (0x05U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW1_STBY_0_4375_V        (0x06U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW1_STBY_0_44375V        (0x07U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW1_STBY_0_45_V          (0x08U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW1_STBY_0_45625_V       (0x09U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW1_STBY_0_4625_V        (0x0AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW1_STBY_0_46875_V       (0x0BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW1_STBY_0_475_V         (0x0CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW1_STBY_0_48125_V       (0x0DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW1_STBY_0_4875_V        (0x0EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW1_STBY_0_49375_V       (0x0FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW1_STBY_0_5_V           (0x10U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW1_STBY_0_50625_V       (0x11U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW1_STBY_0_5125_V        (0x12U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW1_STBY_0_51875_V       (0x13U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW1_STBY_0_525_V         (0x14U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW1_STBY_0_53125_V       (0x15U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW1_STBY_0_5375_V        (0x16U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW1_STBY_0_54375_V       (0x17U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW1_STBY_0_55_V          (0x18U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW1_STBY_0_55625_V       (0x19U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW1_STBY_0_5625_V        (0x1AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW1_STBY_0_56875_V       (0x1BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW1_STBY_0_575_V         (0x1CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW1_STBY_0_58125_V       (0x1DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW1_STBY_0_5875_V        (0x1EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW1_STBY_0_59375_V       (0x1FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW1_STBY_0_6_V           (0x20U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW1_STBY_0_60625_V       (0x21U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW1_STBY_0_6125_V        (0x22U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW1_STBY_0_61875_V       (0x23U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW1_STBY_0_625_V         (0x24U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW1_STBY_0_63125_V       (0x25U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW1_STBY_0_6375_V        (0x26U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW1_STBY_0_64375_V       (0x27U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW1_STBY_0_65_V          (0x28U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW1_STBY_0_65625_V       (0x29U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW1_STBY_0_6625_V        (0x2AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW1_STBY_0_66875_V       (0x2BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW1_STBY_0_675_V         (0x2CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW1_STBY_0_68125_V       (0x2DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW1_STBY_0_6875_V        (0x2EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW1_STBY_0_69375_V       (0x2FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW1_STBY_0_7_V           (0x30U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW1_STBY_0_70625_V       (0x31U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW1_STBY_0_7125_V        (0x32U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW1_STBY_0_71875_V       (0x33U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW1_STBY_0_725_V         (0x34U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW1_STBY_0_73125_V       (0x35U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW1_STBY_0_7375_V        (0x36U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW1_STBY_0_74375_V       (0x37U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW1_STBY_0_75_V          (0x38U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW1_STBY_0_75625_V       (0x39U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW1_STBY_0_7625_V        (0x3AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW1_STBY_0_76875_V       (0x3BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW1_STBY_0_775_V         (0x3CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW1_STBY_0_78125_V       (0x3DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW1_STBY_0_7875_V        (0x3EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW1_STBY_0_79375_V       (0x3FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW1_STBY_0_8_V           (0x40U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW1_STBY_0_80625_V       (0x41U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW1_STBY_0_8125_V        (0x42U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW1_STBY_0_81875_V       (0x43U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW1_STBY_0_825_V         (0x44U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW1_STBY_0_83125_V       (0x45U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW1_STBY_0_8375_V        (0x46U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW1_STBY_0_84375_V       (0x47U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW1_STBY_0_85_V          (0x48U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW1_STBY_0_85625_V       (0x49U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW1_STBY_0_8625_V        (0x4AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW1_STBY_0_86875_V       (0x4BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW1_STBY_0_875_V         (0x4CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW1_STBY_0_88125_V       (0x4DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW1_STBY_0_8875_V        (0x4EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW1_STBY_0_89375_V       (0x4FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW1_STBY_0_9_V           (0x50U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW1_STBY_0_90625_V       (0x51U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW1_STBY_0_9125_V        (0x52U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW1_STBY_0_91875_V       (0x53U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW1_STBY_0_925_V         (0x54U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW1_STBY_0_93125_V       (0x55U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW1_STBY_0_9375_V        (0x56U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW1_STBY_0_94375_V       (0x57U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW1_STBY_0_95_V          (0x58U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW1_STBY_0_95625_V       (0x59U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW1_STBY_0_9625_V        (0x5AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW1_STBY_0_96875_V       (0x5BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW1_STBY_0_975_V         (0x5CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW1_STBY_0_98125_V       (0x5DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW1_STBY_0_9875_V        (0x5EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW1_STBY_0_99375_V       (0x5FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW1_STBY_1_V             (0x60U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW1_STBY_1_00625_V       (0x61U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW1_STBY_1_0125_V        (0x62U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW1_STBY_1_01875_V       (0x63U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW1_STBY_1_025_V         (0x64U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW1_STBY_1_03125_V       (0x65U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW1_STBY_1_0375_V        (0x66U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW1_STBY_1_04375_V       (0x67U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW1_STBY_1_05_V          (0x68U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW1_STBY_1_05625_V       (0x69U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW1_STBY_1_0625_V        (0x6AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW1_STBY_1_06875_V       (0x6BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW1_STBY_1_075_V         (0x6CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW1_STBY_1_08125_V       (0x6DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW1_STBY_1_0875_V        (0x6EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW1_STBY_1_09375_V       (0x6FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW1_STBY_1_1_V           (0x70U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW1_STBY_1_10625_V       (0x71U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW1_STBY_1_1125_V        (0x72U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW1_STBY_1_11875_V       (0x73U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW1_STBY_1_125_V         (0x74U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW1_STBY_1_13125_V       (0x75U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW1_STBY_1_1375_V        (0x76U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW1_STBY_1_14375_V       (0x77U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW1_STBY_1_15_V          (0x78U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW1_STBY_1_15625_V       (0x79U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW1_STBY_1_1625_V        (0x7AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW1_STBY_1_16875_V       (0x7BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW1_STBY_1_175_V         (0x7CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW1_STBY_1_18125_V       (0x7DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW1_STBY_1_1875_V        (0x7EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW1_STBY_1_19375_V       (0x7FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW1_STBY_1_2_V           (0x80U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW1_STBY_1_20625_V       (0x81U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW1_STBY_1_2125_V        (0x82U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW1_STBY_1_21875_V       (0x83U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW1_STBY_1_225_V         (0x84U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW1_STBY_1_23125_V       (0x85U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW1_STBY_1_2375_V        (0x86U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW1_STBY_1_24375_V       (0x87U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW1_STBY_1_25_V          (0x88U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW1_STBY_1_25625_V       (0x89U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW1_STBY_1_2625_V        (0x8AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW1_STBY_1_26875_V       (0x8BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW1_STBY_1_275_V         (0x8CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW1_STBY_1_28125_V       (0x8DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW1_STBY_1_2875_V        (0x8EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW1_STBY_1_29375_V       (0x8FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW1_STBY_1_3_V           (0x90U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW1_STBY_1_30625_V       (0x91U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW1_STBY_1_3125_V        (0x92U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW1_STBY_1_31875_V       (0x93U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW1_STBY_1_325_V         (0x94U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW1_STBY_1_33125_V       (0x95U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW1_STBY_1_3375_V        (0x96U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW1_STBY_1_34375_V       (0x97U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW1_STBY_1_35_V          (0x98U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW1_STBY_1_35625_V       (0x99U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW1_STBY_1_3625_V        (0x9AU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW1_STBY_1_36875_V       (0x9BU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW1_STBY_1_375_V         (0x9CU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW1_STBY_1_38125_V       (0x9DU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW1_STBY_1_3875_V        (0x9EU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW1_STBY_1_39375_V       (0x9FU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW1_STBY_1_4_V           (0xA0U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW1_STBY_1_40625_V       (0xA1U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW1_STBY_1_4125_V        (0xA2U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW1_STBY_1_41875_V       (0xA3U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW1_STBY_1_425_V         (0xA4U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW1_STBY_1_43125_V       (0xA5U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW1_STBY_1_4375_V        (0xA6U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW1_STBY_1_44375_V       (0xA7U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW1_STBY_1_45_V          (0xA8U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW1_STBY_1_45625_V       (0xA9U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW1_STBY_1_4625_V        (0xAAU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW1_STBY_1_46875_V       (0xABU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW1_STBY_1_475_V         (0xACU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW1_STBY_1_48125_V       (0xADU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW1_STBY_1_4875_V        (0xAEU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW1_STBY_1_49375_V       (0xAFU << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW1_STBY_1_5_V           (0xB0U << PF8X_F_VSW1_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW1_STBY_1_8_V           (0xB1U << PF8X_F_VSW1_STBY_SHIFT)

/******************************************************************************/
/* SW2_CONFIG1 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW2_CONFIG1_ADDR            0x55U
#define PF8X_SW2_CONFIG1_DEFAULT         0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW2_PG_EN_MASK            0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW2_WDBYPASS_MASK         0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW2_ILIM_STATE_MASK       0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW2_OV_STATE_MASK         0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW2_UV_STATE_MASK         0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW2_ILIM_BYPASS_MASK      0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW2_OV_BYPASS_MASK        0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW2_UV_BYPASS_MASK        0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW2_PG_EN_SHIFT           0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW2_WDBYPASS_SHIFT        0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW2_ILIM_STATE_SHIFT      0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW2_OV_STATE_SHIFT        0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW2_UV_STATE_SHIFT        0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW2_ILIM_BYPASS_SHIFT     0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW2_OV_BYPASS_SHIFT       0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW2_UV_BYPASS_SHIFT       0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_SW2_PG_EN_DISABLED        (0x00U << PF8X_F_SW2_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_SW2_PG_EN_ENABLED         (0x01U << PF8X_F_SW2_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_SW2_WDBYPASS_RESET        (0x00U << PF8X_F_SW2_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_SW2_WDBYPASS_BYPASS       (0x01U << PF8X_F_SW2_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW2_ILIM_STATE_DISABLED   (0x00U << PF8X_F_SW2_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW2_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW2_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW2_OV_STATE_DISABLED     (0x00U << PF8X_F_SW2_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW2_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW2_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW2_UV_STATE_DISABLED     (0x00U << PF8X_F_SW2_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW2_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW2_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW2_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW2_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW2_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW2_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW2_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW2_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW2_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW2_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW2_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW2_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW2_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW2_UV_BYPASS_SHIFT)

/******************************************************************************/
/* SW2_CONFIG2 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW2_CONFIG2_ADDR            0x56U
#define PF8X_SW2_CONFIG2_DEFAULT         0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW2PHASE_MASK             0x07U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW2ILIM_MASK              0x18U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW2DVS_RAMP_MASK          0x20U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW2_FLT_REN_MASK          0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW2PHASE_SHIFT            0x00U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW2ILIM_SHIFT             0x03U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW2DVS_RAMP_SHIFT         0x05U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW2_FLT_REN_SHIFT         0x07U

/**
 * 45_
 */
#define PF8X_F_SW2PHASE_45               (0x00U << PF8X_F_SW2PHASE_SHIFT)
/**
 * 90_
 */
#define PF8X_F_SW2PHASE_90               (0x01U << PF8X_F_SW2PHASE_SHIFT)
/**
 * 135_
 */
#define PF8X_F_SW2PHASE_135              (0x02U << PF8X_F_SW2PHASE_SHIFT)
/**
 * 180_
 */
#define PF8X_F_SW2PHASE_180              (0x03U << PF8X_F_SW2PHASE_SHIFT)
/**
 * 225_
 */
#define PF8X_F_SW2PHASE_225              (0x04U << PF8X_F_SW2PHASE_SHIFT)
/**
 * 270_
 */
#define PF8X_F_SW2PHASE_270              (0x05U << PF8X_F_SW2PHASE_SHIFT)
/**
 * 315_
 */
#define PF8X_F_SW2PHASE_315              (0x06U << PF8X_F_SW2PHASE_SHIFT)
/**
 * 0_
 */
#define PF8X_F_SW2PHASE_0                (0x07U << PF8X_F_SW2PHASE_SHIFT)

/**
 * 2.1 A
 */
#define PF8X_F_SW2ILIM_2_1_A             (0x00U << PF8X_F_SW2ILIM_SHIFT)
/**
 * 2.6 A
 */
#define PF8X_F_SW2ILIM_2_6_A             (0x01U << PF8X_F_SW2ILIM_SHIFT)
/**
 * 3.0 A
 */
#define PF8X_F_SW2ILIM_3_0_A             (0x02U << PF8X_F_SW2ILIM_SHIFT)
/**
 * 4.5 A
 */
#define PF8X_F_SW2ILIM_4_5_A             (0x03U << PF8X_F_SW2ILIM_SHIFT)

/**
 * 6.25 mV/us
 */
#define PF8X_F_SW2DVS_RAMP_6_25          (0x00U << PF8X_F_SW2DVS_RAMP_SHIFT)
/**
 * 12.5 mV/us
 */
#define PF8X_F_SW2DVS_RAMP_12_5          (0x01U << PF8X_F_SW2DVS_RAMP_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_SW2_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_SW2_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_SW2_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_SW2_FLT_REN_SHIFT)

/******************************************************************************/
/* SW2_PWRUP - Type: RW                                                       */
/******************************************************************************/

#define PF8X_SW2_PWRUP_ADDR              0x57U
#define PF8X_SW2_PWRUP_DEFAULT           0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW2_SEQ_MASK              0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW2_SEQ_SHIFT             0x00U

/******************************************************************************/
/* SW2_MODE - Type: RW                                                        */
/******************************************************************************/

#define PF8X_SW2_MODE_ADDR               0x58U
#define PF8X_SW2_MODE_DEFAULT            0x00U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW2_RUN_MODE_MASK         0x03U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW2_STBY_MODE_MASK        0x0CU
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW2_PDGRP_MASK            0x30U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW2_RUN_MODE_SHIFT        0x00U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW2_STBY_MODE_SHIFT       0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW2_PDGRP_SHIFT           0x04U

/**
 * OFF
 */
#define PF8X_F_SW2_RUN_MODE_OFF          (0x00U << PF8X_F_SW2_RUN_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW2_RUN_MODE_PWM          (0x01U << PF8X_F_SW2_RUN_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW2_RUN_MODE_PFM          (0x02U << PF8X_F_SW2_RUN_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW2_RUN_MODE_AUTO         (0x03U << PF8X_F_SW2_RUN_MODE_SHIFT)

/**
 * OFF
 */
#define PF8X_F_SW2_STBY_MODE_OFF         (0x00U << PF8X_F_SW2_STBY_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW2_STBY_MODE_PWM         (0x01U << PF8X_F_SW2_STBY_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW2_STBY_MODE_PFM         (0x02U << PF8X_F_SW2_STBY_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW2_STBY_MODE_AUTO        (0x03U << PF8X_F_SW2_STBY_MODE_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_SW2_PDGRP_GROUP_4         (0x00U << PF8X_F_SW2_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_SW2_PDGRP_GROUP_3         (0x01U << PF8X_F_SW2_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_SW2_PDGRP_GROUP_2         (0x02U << PF8X_F_SW2_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_SW2_PDGRP_GROUP_1         (0x03U << PF8X_F_SW2_PDGRP_SHIFT)

/******************************************************************************/
/* SW2_RUN_VOLT - Type: RW                                                    */
/******************************************************************************/

#define PF8X_SW2_RUN_VOLT_ADDR           0x59U
#define PF8X_SW2_RUN_VOLT_DEFAULT        0x00U

/**
 * SW2 Output voltage configuration in given Mode. It uses the OTP_VSW2 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW2_RUN_MASK             0xFFU

/**
 * SW2 Output voltage configuration in given Mode. It uses the OTP_VSW2 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW2_RUN_SHIFT            0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW2_RUN_0_4_V            (0x00U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW2_RUN_0_40625_V        (0x01U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW2_RUN_0_4125_V         (0x02U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW2_RUN_0_41875_V        (0x03U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW2_RUN_0_425_V          (0x04U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW2_RUN_0_43125_V        (0x05U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW2_RUN_0_4375_V         (0x06U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW2_RUN_0_44375V         (0x07U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW2_RUN_0_45_V           (0x08U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW2_RUN_0_45625_V        (0x09U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW2_RUN_0_4625_V         (0x0AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW2_RUN_0_46875_V        (0x0BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW2_RUN_0_475_V          (0x0CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW2_RUN_0_48125_V        (0x0DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW2_RUN_0_4875_V         (0x0EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW2_RUN_0_49375_V        (0x0FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW2_RUN_0_5_V            (0x10U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW2_RUN_0_50625_V        (0x11U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW2_RUN_0_5125_V         (0x12U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW2_RUN_0_51875_V        (0x13U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW2_RUN_0_525_V          (0x14U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW2_RUN_0_53125_V        (0x15U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW2_RUN_0_5375_V         (0x16U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW2_RUN_0_54375_V        (0x17U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW2_RUN_0_55_V           (0x18U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW2_RUN_0_55625_V        (0x19U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW2_RUN_0_5625_V         (0x1AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW2_RUN_0_56875_V        (0x1BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW2_RUN_0_575_V          (0x1CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW2_RUN_0_58125_V        (0x1DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW2_RUN_0_5875_V         (0x1EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW2_RUN_0_59375_V        (0x1FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW2_RUN_0_6_V            (0x20U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW2_RUN_0_60625_V        (0x21U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW2_RUN_0_6125_V         (0x22U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW2_RUN_0_61875_V        (0x23U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW2_RUN_0_625_V          (0x24U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW2_RUN_0_63125_V        (0x25U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW2_RUN_0_6375_V         (0x26U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW2_RUN_0_64375_V        (0x27U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW2_RUN_0_65_V           (0x28U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW2_RUN_0_65625_V        (0x29U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW2_RUN_0_6625_V         (0x2AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW2_RUN_0_66875_V        (0x2BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW2_RUN_0_675_V          (0x2CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW2_RUN_0_68125_V        (0x2DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW2_RUN_0_6875_V         (0x2EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW2_RUN_0_69375_V        (0x2FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW2_RUN_0_7_V            (0x30U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW2_RUN_0_70625_V        (0x31U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW2_RUN_0_7125_V         (0x32U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW2_RUN_0_71875_V        (0x33U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW2_RUN_0_725_V          (0x34U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW2_RUN_0_73125_V        (0x35U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW2_RUN_0_7375_V         (0x36U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW2_RUN_0_74375_V        (0x37U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW2_RUN_0_75_V           (0x38U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW2_RUN_0_75625_V        (0x39U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW2_RUN_0_7625_V         (0x3AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW2_RUN_0_76875_V        (0x3BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW2_RUN_0_775_V          (0x3CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW2_RUN_0_78125_V        (0x3DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW2_RUN_0_7875_V         (0x3EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW2_RUN_0_79375_V        (0x3FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW2_RUN_0_8_V            (0x40U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW2_RUN_0_80625_V        (0x41U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW2_RUN_0_8125_V         (0x42U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW2_RUN_0_81875_V        (0x43U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW2_RUN_0_825_V          (0x44U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW2_RUN_0_83125_V        (0x45U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW2_RUN_0_8375_V         (0x46U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW2_RUN_0_84375_V        (0x47U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW2_RUN_0_85_V           (0x48U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW2_RUN_0_85625_V        (0x49U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW2_RUN_0_8625_V         (0x4AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW2_RUN_0_86875_V        (0x4BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW2_RUN_0_875_V          (0x4CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW2_RUN_0_88125_V        (0x4DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW2_RUN_0_8875_V         (0x4EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW2_RUN_0_89375_V        (0x4FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW2_RUN_0_9_V            (0x50U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW2_RUN_0_90625_V        (0x51U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW2_RUN_0_9125_V         (0x52U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW2_RUN_0_91875_V        (0x53U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW2_RUN_0_925_V          (0x54U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW2_RUN_0_93125_V        (0x55U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW2_RUN_0_9375_V         (0x56U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW2_RUN_0_94375_V        (0x57U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW2_RUN_0_95_V           (0x58U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW2_RUN_0_95625_V        (0x59U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW2_RUN_0_9625_V         (0x5AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW2_RUN_0_96875_V        (0x5BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW2_RUN_0_975_V          (0x5CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW2_RUN_0_98125_V        (0x5DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW2_RUN_0_9875_V         (0x5EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW2_RUN_0_99375_V        (0x5FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW2_RUN_1_V              (0x60U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW2_RUN_1_00625_V        (0x61U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW2_RUN_1_0125_V         (0x62U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW2_RUN_1_01875_V        (0x63U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW2_RUN_1_025_V          (0x64U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW2_RUN_1_03125_V        (0x65U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW2_RUN_1_0375_V         (0x66U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW2_RUN_1_04375_V        (0x67U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW2_RUN_1_05_V           (0x68U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW2_RUN_1_05625_V        (0x69U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW2_RUN_1_0625_V         (0x6AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW2_RUN_1_06875_V        (0x6BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW2_RUN_1_075_V          (0x6CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW2_RUN_1_08125_V        (0x6DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW2_RUN_1_0875_V         (0x6EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW2_RUN_1_09375_V        (0x6FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW2_RUN_1_1_V            (0x70U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW2_RUN_1_10625_V        (0x71U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW2_RUN_1_1125_V         (0x72U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW2_RUN_1_11875_V        (0x73U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW2_RUN_1_125_V          (0x74U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW2_RUN_1_13125_V        (0x75U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW2_RUN_1_1375_V         (0x76U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW2_RUN_1_14375_V        (0x77U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW2_RUN_1_15_V           (0x78U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW2_RUN_1_15625_V        (0x79U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW2_RUN_1_1625_V         (0x7AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW2_RUN_1_16875_V        (0x7BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW2_RUN_1_175_V          (0x7CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW2_RUN_1_18125_V        (0x7DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW2_RUN_1_1875_V         (0x7EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW2_RUN_1_19375_V        (0x7FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW2_RUN_1_2_V            (0x80U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW2_RUN_1_20625_V        (0x81U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW2_RUN_1_2125_V         (0x82U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW2_RUN_1_21875_V        (0x83U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW2_RUN_1_225_V          (0x84U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW2_RUN_1_23125_V        (0x85U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW2_RUN_1_2375_V         (0x86U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW2_RUN_1_24375_V        (0x87U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW2_RUN_1_25_V           (0x88U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW2_RUN_1_25625_V        (0x89U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW2_RUN_1_2625_V         (0x8AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW2_RUN_1_26875_V        (0x8BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW2_RUN_1_275_V          (0x8CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW2_RUN_1_28125_V        (0x8DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW2_RUN_1_2875_V         (0x8EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW2_RUN_1_29375_V        (0x8FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW2_RUN_1_3_V            (0x90U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW2_RUN_1_30625_V        (0x91U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW2_RUN_1_3125_V         (0x92U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW2_RUN_1_31875_V        (0x93U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW2_RUN_1_325_V          (0x94U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW2_RUN_1_33125_V        (0x95U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW2_RUN_1_3375_V         (0x96U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW2_RUN_1_34375_V        (0x97U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW2_RUN_1_35_V           (0x98U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW2_RUN_1_35625_V        (0x99U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW2_RUN_1_3625_V         (0x9AU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW2_RUN_1_36875_V        (0x9BU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW2_RUN_1_375_V          (0x9CU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW2_RUN_1_38125_V        (0x9DU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW2_RUN_1_3875_V         (0x9EU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW2_RUN_1_39375_V        (0x9FU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW2_RUN_1_4_V            (0xA0U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW2_RUN_1_40625_V        (0xA1U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW2_RUN_1_4125_V         (0xA2U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW2_RUN_1_41875_V        (0xA3U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW2_RUN_1_425_V          (0xA4U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW2_RUN_1_43125_V        (0xA5U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW2_RUN_1_4375_V         (0xA6U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW2_RUN_1_44375_V        (0xA7U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW2_RUN_1_45_V           (0xA8U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW2_RUN_1_45625_V        (0xA9U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW2_RUN_1_4625_V         (0xAAU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW2_RUN_1_46875_V        (0xABU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW2_RUN_1_475_V          (0xACU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW2_RUN_1_48125_V        (0xADU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW2_RUN_1_4875_V         (0xAEU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW2_RUN_1_49375_V        (0xAFU << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW2_RUN_1_5_V            (0xB0U << PF8X_F_VSW2_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW2_RUN_1_8_V            (0xB1U << PF8X_F_VSW2_RUN_SHIFT)

/******************************************************************************/
/* SW2_STBY_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_SW2_STBY_VOLT_ADDR          0x5AU
#define PF8X_SW2_STBY_VOLT_DEFAULT       0x00U

/**
 * SW2 Output voltage configuration in given Mode. It uses the OTP_VSW2 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW2_STBY_MASK            0xFFU

/**
 * SW2 Output voltage configuration in given Mode. It uses the OTP_VSW2 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW2_STBY_SHIFT           0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW2_STBY_0_4_V           (0x00U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW2_STBY_0_40625_V       (0x01U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW2_STBY_0_4125_V        (0x02U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW2_STBY_0_41875_V       (0x03U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW2_STBY_0_425_V         (0x04U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW2_STBY_0_43125_V       (0x05U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW2_STBY_0_4375_V        (0x06U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW2_STBY_0_44375V        (0x07U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW2_STBY_0_45_V          (0x08U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW2_STBY_0_45625_V       (0x09U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW2_STBY_0_4625_V        (0x0AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW2_STBY_0_46875_V       (0x0BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW2_STBY_0_475_V         (0x0CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW2_STBY_0_48125_V       (0x0DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW2_STBY_0_4875_V        (0x0EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW2_STBY_0_49375_V       (0x0FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW2_STBY_0_5_V           (0x10U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW2_STBY_0_50625_V       (0x11U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW2_STBY_0_5125_V        (0x12U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW2_STBY_0_51875_V       (0x13U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW2_STBY_0_525_V         (0x14U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW2_STBY_0_53125_V       (0x15U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW2_STBY_0_5375_V        (0x16U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW2_STBY_0_54375_V       (0x17U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW2_STBY_0_55_V          (0x18U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW2_STBY_0_55625_V       (0x19U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW2_STBY_0_5625_V        (0x1AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW2_STBY_0_56875_V       (0x1BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW2_STBY_0_575_V         (0x1CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW2_STBY_0_58125_V       (0x1DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW2_STBY_0_5875_V        (0x1EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW2_STBY_0_59375_V       (0x1FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW2_STBY_0_6_V           (0x20U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW2_STBY_0_60625_V       (0x21U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW2_STBY_0_6125_V        (0x22U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW2_STBY_0_61875_V       (0x23U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW2_STBY_0_625_V         (0x24U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW2_STBY_0_63125_V       (0x25U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW2_STBY_0_6375_V        (0x26U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW2_STBY_0_64375_V       (0x27U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW2_STBY_0_65_V          (0x28U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW2_STBY_0_65625_V       (0x29U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW2_STBY_0_6625_V        (0x2AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW2_STBY_0_66875_V       (0x2BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW2_STBY_0_675_V         (0x2CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW2_STBY_0_68125_V       (0x2DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW2_STBY_0_6875_V        (0x2EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW2_STBY_0_69375_V       (0x2FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW2_STBY_0_7_V           (0x30U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW2_STBY_0_70625_V       (0x31U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW2_STBY_0_7125_V        (0x32U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW2_STBY_0_71875_V       (0x33U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW2_STBY_0_725_V         (0x34U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW2_STBY_0_73125_V       (0x35U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW2_STBY_0_7375_V        (0x36U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW2_STBY_0_74375_V       (0x37U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW2_STBY_0_75_V          (0x38U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW2_STBY_0_75625_V       (0x39U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW2_STBY_0_7625_V        (0x3AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW2_STBY_0_76875_V       (0x3BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW2_STBY_0_775_V         (0x3CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW2_STBY_0_78125_V       (0x3DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW2_STBY_0_7875_V        (0x3EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW2_STBY_0_79375_V       (0x3FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW2_STBY_0_8_V           (0x40U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW2_STBY_0_80625_V       (0x41U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW2_STBY_0_8125_V        (0x42U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW2_STBY_0_81875_V       (0x43U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW2_STBY_0_825_V         (0x44U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW2_STBY_0_83125_V       (0x45U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW2_STBY_0_8375_V        (0x46U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW2_STBY_0_84375_V       (0x47U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW2_STBY_0_85_V          (0x48U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW2_STBY_0_85625_V       (0x49U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW2_STBY_0_8625_V        (0x4AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW2_STBY_0_86875_V       (0x4BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW2_STBY_0_875_V         (0x4CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW2_STBY_0_88125_V       (0x4DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW2_STBY_0_8875_V        (0x4EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW2_STBY_0_89375_V       (0x4FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW2_STBY_0_9_V           (0x50U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW2_STBY_0_90625_V       (0x51U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW2_STBY_0_9125_V        (0x52U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW2_STBY_0_91875_V       (0x53U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW2_STBY_0_925_V         (0x54U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW2_STBY_0_93125_V       (0x55U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW2_STBY_0_9375_V        (0x56U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW2_STBY_0_94375_V       (0x57U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW2_STBY_0_95_V          (0x58U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW2_STBY_0_95625_V       (0x59U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW2_STBY_0_9625_V        (0x5AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW2_STBY_0_96875_V       (0x5BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW2_STBY_0_975_V         (0x5CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW2_STBY_0_98125_V       (0x5DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW2_STBY_0_9875_V        (0x5EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW2_STBY_0_99375_V       (0x5FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW2_STBY_1_V             (0x60U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW2_STBY_1_00625_V       (0x61U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW2_STBY_1_0125_V        (0x62U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW2_STBY_1_01875_V       (0x63U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW2_STBY_1_025_V         (0x64U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW2_STBY_1_03125_V       (0x65U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW2_STBY_1_0375_V        (0x66U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW2_STBY_1_04375_V       (0x67U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW2_STBY_1_05_V          (0x68U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW2_STBY_1_05625_V       (0x69U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW2_STBY_1_0625_V        (0x6AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW2_STBY_1_06875_V       (0x6BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW2_STBY_1_075_V         (0x6CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW2_STBY_1_08125_V       (0x6DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW2_STBY_1_0875_V        (0x6EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW2_STBY_1_09375_V       (0x6FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW2_STBY_1_1_V           (0x70U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW2_STBY_1_10625_V       (0x71U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW2_STBY_1_1125_V        (0x72U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW2_STBY_1_11875_V       (0x73U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW2_STBY_1_125_V         (0x74U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW2_STBY_1_13125_V       (0x75U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW2_STBY_1_1375_V        (0x76U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW2_STBY_1_14375_V       (0x77U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW2_STBY_1_15_V          (0x78U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW2_STBY_1_15625_V       (0x79U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW2_STBY_1_1625_V        (0x7AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW2_STBY_1_16875_V       (0x7BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW2_STBY_1_175_V         (0x7CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW2_STBY_1_18125_V       (0x7DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW2_STBY_1_1875_V        (0x7EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW2_STBY_1_19375_V       (0x7FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW2_STBY_1_2_V           (0x80U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW2_STBY_1_20625_V       (0x81U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW2_STBY_1_2125_V        (0x82U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW2_STBY_1_21875_V       (0x83U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW2_STBY_1_225_V         (0x84U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW2_STBY_1_23125_V       (0x85U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW2_STBY_1_2375_V        (0x86U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW2_STBY_1_24375_V       (0x87U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW2_STBY_1_25_V          (0x88U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW2_STBY_1_25625_V       (0x89U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW2_STBY_1_2625_V        (0x8AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW2_STBY_1_26875_V       (0x8BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW2_STBY_1_275_V         (0x8CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW2_STBY_1_28125_V       (0x8DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW2_STBY_1_2875_V        (0x8EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW2_STBY_1_29375_V       (0x8FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW2_STBY_1_3_V           (0x90U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW2_STBY_1_30625_V       (0x91U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW2_STBY_1_3125_V        (0x92U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW2_STBY_1_31875_V       (0x93U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW2_STBY_1_325_V         (0x94U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW2_STBY_1_33125_V       (0x95U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW2_STBY_1_3375_V        (0x96U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW2_STBY_1_34375_V       (0x97U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW2_STBY_1_35_V          (0x98U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW2_STBY_1_35625_V       (0x99U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW2_STBY_1_3625_V        (0x9AU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW2_STBY_1_36875_V       (0x9BU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW2_STBY_1_375_V         (0x9CU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW2_STBY_1_38125_V       (0x9DU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW2_STBY_1_3875_V        (0x9EU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW2_STBY_1_39375_V       (0x9FU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW2_STBY_1_4_V           (0xA0U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW2_STBY_1_40625_V       (0xA1U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW2_STBY_1_4125_V        (0xA2U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW2_STBY_1_41875_V       (0xA3U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW2_STBY_1_425_V         (0xA4U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW2_STBY_1_43125_V       (0xA5U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW2_STBY_1_4375_V        (0xA6U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW2_STBY_1_44375_V       (0xA7U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW2_STBY_1_45_V          (0xA8U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW2_STBY_1_45625_V       (0xA9U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW2_STBY_1_4625_V        (0xAAU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW2_STBY_1_46875_V       (0xABU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW2_STBY_1_475_V         (0xACU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW2_STBY_1_48125_V       (0xADU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW2_STBY_1_4875_V        (0xAEU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW2_STBY_1_49375_V       (0xAFU << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW2_STBY_1_5_V           (0xB0U << PF8X_F_VSW2_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW2_STBY_1_8_V           (0xB1U << PF8X_F_VSW2_STBY_SHIFT)

/******************************************************************************/
/* SW3_CONFIG1 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW3_CONFIG1_ADDR            0x5DU
#define PF8X_SW3_CONFIG1_DEFAULT         0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW3_PG_EN_MASK            0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW3_WDBYPASS_MASK         0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW3_ILIM_STATE_MASK       0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW3_OV_STATE_MASK         0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW3_UV_STATE_MASK         0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW3_ILIM_BYPASS_MASK      0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW3_OV_BYPASS_MASK        0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW3_UV_BYPASS_MASK        0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW3_PG_EN_SHIFT           0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW3_WDBYPASS_SHIFT        0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW3_ILIM_STATE_SHIFT      0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW3_OV_STATE_SHIFT        0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW3_UV_STATE_SHIFT        0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW3_ILIM_BYPASS_SHIFT     0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW3_OV_BYPASS_SHIFT       0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW3_UV_BYPASS_SHIFT       0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_SW3_PG_EN_DISABLED        (0x00U << PF8X_F_SW3_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_SW3_PG_EN_ENABLED         (0x01U << PF8X_F_SW3_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_SW3_WDBYPASS_RESET        (0x00U << PF8X_F_SW3_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_SW3_WDBYPASS_BYPASS       (0x01U << PF8X_F_SW3_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW3_ILIM_STATE_DISABLED   (0x00U << PF8X_F_SW3_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW3_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW3_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW3_OV_STATE_DISABLED     (0x00U << PF8X_F_SW3_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW3_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW3_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW3_UV_STATE_DISABLED     (0x00U << PF8X_F_SW3_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW3_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW3_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW3_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW3_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW3_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW3_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW3_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW3_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW3_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW3_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW3_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW3_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW3_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW3_UV_BYPASS_SHIFT)

/******************************************************************************/
/* SW3_CONFIG2 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW3_CONFIG2_ADDR            0x5EU
#define PF8X_SW3_CONFIG2_DEFAULT         0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW3PHASE_MASK             0x07U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW3ILIM_MASK              0x18U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW3DVS_RAMP_MASK          0x20U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW3_FLT_REN_MASK          0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW3PHASE_SHIFT            0x00U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW3ILIM_SHIFT             0x03U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW3DVS_RAMP_SHIFT         0x05U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW3_FLT_REN_SHIFT         0x07U

/**
 * 45_
 */
#define PF8X_F_SW3PHASE_45               (0x00U << PF8X_F_SW3PHASE_SHIFT)
/**
 * 90_
 */
#define PF8X_F_SW3PHASE_90               (0x01U << PF8X_F_SW3PHASE_SHIFT)
/**
 * 135_
 */
#define PF8X_F_SW3PHASE_135              (0x02U << PF8X_F_SW3PHASE_SHIFT)
/**
 * 180_
 */
#define PF8X_F_SW3PHASE_180              (0x03U << PF8X_F_SW3PHASE_SHIFT)
/**
 * 225_
 */
#define PF8X_F_SW3PHASE_225              (0x04U << PF8X_F_SW3PHASE_SHIFT)
/**
 * 270_
 */
#define PF8X_F_SW3PHASE_270              (0x05U << PF8X_F_SW3PHASE_SHIFT)
/**
 * 315_
 */
#define PF8X_F_SW3PHASE_315              (0x06U << PF8X_F_SW3PHASE_SHIFT)
/**
 * 0_
 */
#define PF8X_F_SW3PHASE_0                (0x07U << PF8X_F_SW3PHASE_SHIFT)

/**
 * 2.1 A
 */
#define PF8X_F_SW3ILIM_2_1_A             (0x00U << PF8X_F_SW3ILIM_SHIFT)
/**
 * 2.6 A
 */
#define PF8X_F_SW3ILIM_2_6_A             (0x01U << PF8X_F_SW3ILIM_SHIFT)
/**
 * 3.0 A
 */
#define PF8X_F_SW3ILIM_3_0_A             (0x02U << PF8X_F_SW3ILIM_SHIFT)
/**
 * 4.5 A
 */
#define PF8X_F_SW3ILIM_4_5_A             (0x03U << PF8X_F_SW3ILIM_SHIFT)

/**
 * 6.25 mV/us
 */
#define PF8X_F_SW3DVS_RAMP_6_25          (0x00U << PF8X_F_SW3DVS_RAMP_SHIFT)
/**
 * 12.5 mV/us
 */
#define PF8X_F_SW3DVS_RAMP_12_5          (0x01U << PF8X_F_SW3DVS_RAMP_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_SW3_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_SW3_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_SW3_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_SW3_FLT_REN_SHIFT)

/******************************************************************************/
/* SW3_PWRUP - Type: RW                                                       */
/******************************************************************************/

#define PF8X_SW3_PWRUP_ADDR              0x5FU
#define PF8X_SW3_PWRUP_DEFAULT           0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW3_SEQ_MASK              0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW3_SEQ_SHIFT             0x00U

/******************************************************************************/
/* SW3_MODE - Type: RW                                                        */
/******************************************************************************/

#define PF8X_SW3_MODE_ADDR               0x60U
#define PF8X_SW3_MODE_DEFAULT            0x00U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW3_RUN_MODE_MASK         0x03U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW3_STBY_MODE_MASK        0x0CU
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW3_PDGRP_MASK            0x30U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW3_RUN_MODE_SHIFT        0x00U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW3_STBY_MODE_SHIFT       0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW3_PDGRP_SHIFT           0x04U

/**
 * OFF
 */
#define PF8X_F_SW3_RUN_MODE_OFF          (0x00U << PF8X_F_SW3_RUN_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW3_RUN_MODE_PWM          (0x01U << PF8X_F_SW3_RUN_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW3_RUN_MODE_PFM          (0x02U << PF8X_F_SW3_RUN_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW3_RUN_MODE_AUTO         (0x03U << PF8X_F_SW3_RUN_MODE_SHIFT)

/**
 * OFF
 */
#define PF8X_F_SW3_STBY_MODE_OFF         (0x00U << PF8X_F_SW3_STBY_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW3_STBY_MODE_PWM         (0x01U << PF8X_F_SW3_STBY_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW3_STBY_MODE_PFM         (0x02U << PF8X_F_SW3_STBY_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW3_STBY_MODE_AUTO        (0x03U << PF8X_F_SW3_STBY_MODE_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_SW3_PDGRP_GROUP_4         (0x00U << PF8X_F_SW3_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_SW3_PDGRP_GROUP_3         (0x01U << PF8X_F_SW3_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_SW3_PDGRP_GROUP_2         (0x02U << PF8X_F_SW3_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_SW3_PDGRP_GROUP_1         (0x03U << PF8X_F_SW3_PDGRP_SHIFT)

/******************************************************************************/
/* SW3_RUN_VOLT - Type: RW                                                    */
/******************************************************************************/

#define PF8X_SW3_RUN_VOLT_ADDR           0x61U
#define PF8X_SW3_RUN_VOLT_DEFAULT        0x00U

/**
 * SW3 Output voltage configuration in given Mode. It uses the OTP_VSW3 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW3_RUN_MASK             0xFFU

/**
 * SW3 Output voltage configuration in given Mode. It uses the OTP_VSW3 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW3_RUN_SHIFT            0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW3_RUN_0_4_V            (0x00U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW3_RUN_0_40625_V        (0x01U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW3_RUN_0_4125_V         (0x02U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW3_RUN_0_41875_V        (0x03U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW3_RUN_0_425_V          (0x04U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW3_RUN_0_43125_V        (0x05U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW3_RUN_0_4375_V         (0x06U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW3_RUN_0_44375V         (0x07U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW3_RUN_0_45_V           (0x08U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW3_RUN_0_45625_V        (0x09U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW3_RUN_0_4625_V         (0x0AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW3_RUN_0_46875_V        (0x0BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW3_RUN_0_475_V          (0x0CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW3_RUN_0_48125_V        (0x0DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW3_RUN_0_4875_V         (0x0EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW3_RUN_0_49375_V        (0x0FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW3_RUN_0_5_V            (0x10U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW3_RUN_0_50625_V        (0x11U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW3_RUN_0_5125_V         (0x12U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW3_RUN_0_51875_V        (0x13U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW3_RUN_0_525_V          (0x14U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW3_RUN_0_53125_V        (0x15U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW3_RUN_0_5375_V         (0x16U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW3_RUN_0_54375_V        (0x17U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW3_RUN_0_55_V           (0x18U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW3_RUN_0_55625_V        (0x19U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW3_RUN_0_5625_V         (0x1AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW3_RUN_0_56875_V        (0x1BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW3_RUN_0_575_V          (0x1CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW3_RUN_0_58125_V        (0x1DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW3_RUN_0_5875_V         (0x1EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW3_RUN_0_59375_V        (0x1FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW3_RUN_0_6_V            (0x20U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW3_RUN_0_60625_V        (0x21U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW3_RUN_0_6125_V         (0x22U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW3_RUN_0_61875_V        (0x23U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW3_RUN_0_625_V          (0x24U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW3_RUN_0_63125_V        (0x25U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW3_RUN_0_6375_V         (0x26U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW3_RUN_0_64375_V        (0x27U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW3_RUN_0_65_V           (0x28U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW3_RUN_0_65625_V        (0x29U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW3_RUN_0_6625_V         (0x2AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW3_RUN_0_66875_V        (0x2BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW3_RUN_0_675_V          (0x2CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW3_RUN_0_68125_V        (0x2DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW3_RUN_0_6875_V         (0x2EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW3_RUN_0_69375_V        (0x2FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW3_RUN_0_7_V            (0x30U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW3_RUN_0_70625_V        (0x31U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW3_RUN_0_7125_V         (0x32U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW3_RUN_0_71875_V        (0x33U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW3_RUN_0_725_V          (0x34U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW3_RUN_0_73125_V        (0x35U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW3_RUN_0_7375_V         (0x36U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW3_RUN_0_74375_V        (0x37U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW3_RUN_0_75_V           (0x38U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW3_RUN_0_75625_V        (0x39U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW3_RUN_0_7625_V         (0x3AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW3_RUN_0_76875_V        (0x3BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW3_RUN_0_775_V          (0x3CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW3_RUN_0_78125_V        (0x3DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW3_RUN_0_7875_V         (0x3EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW3_RUN_0_79375_V        (0x3FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW3_RUN_0_8_V            (0x40U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW3_RUN_0_80625_V        (0x41U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW3_RUN_0_8125_V         (0x42U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW3_RUN_0_81875_V        (0x43U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW3_RUN_0_825_V          (0x44U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW3_RUN_0_83125_V        (0x45U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW3_RUN_0_8375_V         (0x46U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW3_RUN_0_84375_V        (0x47U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW3_RUN_0_85_V           (0x48U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW3_RUN_0_85625_V        (0x49U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW3_RUN_0_8625_V         (0x4AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW3_RUN_0_86875_V        (0x4BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW3_RUN_0_875_V          (0x4CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW3_RUN_0_88125_V        (0x4DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW3_RUN_0_8875_V         (0x4EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW3_RUN_0_89375_V        (0x4FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW3_RUN_0_9_V            (0x50U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW3_RUN_0_90625_V        (0x51U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW3_RUN_0_9125_V         (0x52U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW3_RUN_0_91875_V        (0x53U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW3_RUN_0_925_V          (0x54U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW3_RUN_0_93125_V        (0x55U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW3_RUN_0_9375_V         (0x56U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW3_RUN_0_94375_V        (0x57U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW3_RUN_0_95_V           (0x58U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW3_RUN_0_95625_V        (0x59U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW3_RUN_0_9625_V         (0x5AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW3_RUN_0_96875_V        (0x5BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW3_RUN_0_975_V          (0x5CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW3_RUN_0_98125_V        (0x5DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW3_RUN_0_9875_V         (0x5EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW3_RUN_0_99375_V        (0x5FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW3_RUN_1_V              (0x60U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW3_RUN_1_00625_V        (0x61U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW3_RUN_1_0125_V         (0x62U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW3_RUN_1_01875_V        (0x63U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW3_RUN_1_025_V          (0x64U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW3_RUN_1_03125_V        (0x65U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW3_RUN_1_0375_V         (0x66U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW3_RUN_1_04375_V        (0x67U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW3_RUN_1_05_V           (0x68U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW3_RUN_1_05625_V        (0x69U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW3_RUN_1_0625_V         (0x6AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW3_RUN_1_06875_V        (0x6BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW3_RUN_1_075_V          (0x6CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW3_RUN_1_08125_V        (0x6DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW3_RUN_1_0875_V         (0x6EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW3_RUN_1_09375_V        (0x6FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW3_RUN_1_1_V            (0x70U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW3_RUN_1_10625_V        (0x71U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW3_RUN_1_1125_V         (0x72U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW3_RUN_1_11875_V        (0x73U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW3_RUN_1_125_V          (0x74U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW3_RUN_1_13125_V        (0x75U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW3_RUN_1_1375_V         (0x76U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW3_RUN_1_14375_V        (0x77U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW3_RUN_1_15_V           (0x78U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW3_RUN_1_15625_V        (0x79U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW3_RUN_1_1625_V         (0x7AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW3_RUN_1_16875_V        (0x7BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW3_RUN_1_175_V          (0x7CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW3_RUN_1_18125_V        (0x7DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW3_RUN_1_1875_V         (0x7EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW3_RUN_1_19375_V        (0x7FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW3_RUN_1_2_V            (0x80U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW3_RUN_1_20625_V        (0x81U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW3_RUN_1_2125_V         (0x82U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW3_RUN_1_21875_V        (0x83U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW3_RUN_1_225_V          (0x84U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW3_RUN_1_23125_V        (0x85U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW3_RUN_1_2375_V         (0x86U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW3_RUN_1_24375_V        (0x87U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW3_RUN_1_25_V           (0x88U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW3_RUN_1_25625_V        (0x89U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW3_RUN_1_2625_V         (0x8AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW3_RUN_1_26875_V        (0x8BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW3_RUN_1_275_V          (0x8CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW3_RUN_1_28125_V        (0x8DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW3_RUN_1_2875_V         (0x8EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW3_RUN_1_29375_V        (0x8FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW3_RUN_1_3_V            (0x90U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW3_RUN_1_30625_V        (0x91U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW3_RUN_1_3125_V         (0x92U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW3_RUN_1_31875_V        (0x93U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW3_RUN_1_325_V          (0x94U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW3_RUN_1_33125_V        (0x95U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW3_RUN_1_3375_V         (0x96U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW3_RUN_1_34375_V        (0x97U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW3_RUN_1_35_V           (0x98U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW3_RUN_1_35625_V        (0x99U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW3_RUN_1_3625_V         (0x9AU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW3_RUN_1_36875_V        (0x9BU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW3_RUN_1_375_V          (0x9CU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW3_RUN_1_38125_V        (0x9DU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW3_RUN_1_3875_V         (0x9EU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW3_RUN_1_39375_V        (0x9FU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW3_RUN_1_4_V            (0xA0U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW3_RUN_1_40625_V        (0xA1U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW3_RUN_1_4125_V         (0xA2U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW3_RUN_1_41875_V        (0xA3U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW3_RUN_1_425_V          (0xA4U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW3_RUN_1_43125_V        (0xA5U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW3_RUN_1_4375_V         (0xA6U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW3_RUN_1_44375_V        (0xA7U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW3_RUN_1_45_V           (0xA8U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW3_RUN_1_45625_V        (0xA9U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW3_RUN_1_4625_V         (0xAAU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW3_RUN_1_46875_V        (0xABU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW3_RUN_1_475_V          (0xACU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW3_RUN_1_48125_V        (0xADU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW3_RUN_1_4875_V         (0xAEU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW3_RUN_1_49375_V        (0xAFU << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW3_RUN_1_5_V            (0xB0U << PF8X_F_VSW3_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW3_RUN_1_8_V            (0xB1U << PF8X_F_VSW3_RUN_SHIFT)

/******************************************************************************/
/* SW3_STBY_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_SW3_STBY_VOLT_ADDR          0x62U
#define PF8X_SW3_STBY_VOLT_DEFAULT       0x00U

/**
 * SW3 Output voltage configuration in given Mode. It uses the OTP_VSW3 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW3_STBY_MASK            0xFFU

/**
 * SW3 Output voltage configuration in given Mode. It uses the OTP_VSW3 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW3_STBY_SHIFT           0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW3_STBY_0_4_V           (0x00U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW3_STBY_0_40625_V       (0x01U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW3_STBY_0_4125_V        (0x02U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW3_STBY_0_41875_V       (0x03U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW3_STBY_0_425_V         (0x04U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW3_STBY_0_43125_V       (0x05U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW3_STBY_0_4375_V        (0x06U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW3_STBY_0_44375V        (0x07U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW3_STBY_0_45_V          (0x08U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW3_STBY_0_45625_V       (0x09U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW3_STBY_0_4625_V        (0x0AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW3_STBY_0_46875_V       (0x0BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW3_STBY_0_475_V         (0x0CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW3_STBY_0_48125_V       (0x0DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW3_STBY_0_4875_V        (0x0EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW3_STBY_0_49375_V       (0x0FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW3_STBY_0_5_V           (0x10U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW3_STBY_0_50625_V       (0x11U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW3_STBY_0_5125_V        (0x12U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW3_STBY_0_51875_V       (0x13U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW3_STBY_0_525_V         (0x14U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW3_STBY_0_53125_V       (0x15U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW3_STBY_0_5375_V        (0x16U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW3_STBY_0_54375_V       (0x17U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW3_STBY_0_55_V          (0x18U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW3_STBY_0_55625_V       (0x19U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW3_STBY_0_5625_V        (0x1AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW3_STBY_0_56875_V       (0x1BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW3_STBY_0_575_V         (0x1CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW3_STBY_0_58125_V       (0x1DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW3_STBY_0_5875_V        (0x1EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW3_STBY_0_59375_V       (0x1FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW3_STBY_0_6_V           (0x20U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW3_STBY_0_60625_V       (0x21U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW3_STBY_0_6125_V        (0x22U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW3_STBY_0_61875_V       (0x23U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW3_STBY_0_625_V         (0x24U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW3_STBY_0_63125_V       (0x25U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW3_STBY_0_6375_V        (0x26U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW3_STBY_0_64375_V       (0x27U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW3_STBY_0_65_V          (0x28U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW3_STBY_0_65625_V       (0x29U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW3_STBY_0_6625_V        (0x2AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW3_STBY_0_66875_V       (0x2BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW3_STBY_0_675_V         (0x2CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW3_STBY_0_68125_V       (0x2DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW3_STBY_0_6875_V        (0x2EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW3_STBY_0_69375_V       (0x2FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW3_STBY_0_7_V           (0x30U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW3_STBY_0_70625_V       (0x31U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW3_STBY_0_7125_V        (0x32U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW3_STBY_0_71875_V       (0x33U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW3_STBY_0_725_V         (0x34U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW3_STBY_0_73125_V       (0x35U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW3_STBY_0_7375_V        (0x36U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW3_STBY_0_74375_V       (0x37U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW3_STBY_0_75_V          (0x38U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW3_STBY_0_75625_V       (0x39U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW3_STBY_0_7625_V        (0x3AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW3_STBY_0_76875_V       (0x3BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW3_STBY_0_775_V         (0x3CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW3_STBY_0_78125_V       (0x3DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW3_STBY_0_7875_V        (0x3EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW3_STBY_0_79375_V       (0x3FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW3_STBY_0_8_V           (0x40U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW3_STBY_0_80625_V       (0x41U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW3_STBY_0_8125_V        (0x42U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW3_STBY_0_81875_V       (0x43U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW3_STBY_0_825_V         (0x44U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW3_STBY_0_83125_V       (0x45U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW3_STBY_0_8375_V        (0x46U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW3_STBY_0_84375_V       (0x47U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW3_STBY_0_85_V          (0x48U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW3_STBY_0_85625_V       (0x49U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW3_STBY_0_8625_V        (0x4AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW3_STBY_0_86875_V       (0x4BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW3_STBY_0_875_V         (0x4CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW3_STBY_0_88125_V       (0x4DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW3_STBY_0_8875_V        (0x4EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW3_STBY_0_89375_V       (0x4FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW3_STBY_0_9_V           (0x50U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW3_STBY_0_90625_V       (0x51U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW3_STBY_0_9125_V        (0x52U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW3_STBY_0_91875_V       (0x53U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW3_STBY_0_925_V         (0x54U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW3_STBY_0_93125_V       (0x55U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW3_STBY_0_9375_V        (0x56U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW3_STBY_0_94375_V       (0x57U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW3_STBY_0_95_V          (0x58U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW3_STBY_0_95625_V       (0x59U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW3_STBY_0_9625_V        (0x5AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW3_STBY_0_96875_V       (0x5BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW3_STBY_0_975_V         (0x5CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW3_STBY_0_98125_V       (0x5DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW3_STBY_0_9875_V        (0x5EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW3_STBY_0_99375_V       (0x5FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW3_STBY_1_V             (0x60U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW3_STBY_1_00625_V       (0x61U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW3_STBY_1_0125_V        (0x62U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW3_STBY_1_01875_V       (0x63U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW3_STBY_1_025_V         (0x64U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW3_STBY_1_03125_V       (0x65U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW3_STBY_1_0375_V        (0x66U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW3_STBY_1_04375_V       (0x67U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW3_STBY_1_05_V          (0x68U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW3_STBY_1_05625_V       (0x69U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW3_STBY_1_0625_V        (0x6AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW3_STBY_1_06875_V       (0x6BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW3_STBY_1_075_V         (0x6CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW3_STBY_1_08125_V       (0x6DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW3_STBY_1_0875_V        (0x6EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW3_STBY_1_09375_V       (0x6FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW3_STBY_1_1_V           (0x70U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW3_STBY_1_10625_V       (0x71U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW3_STBY_1_1125_V        (0x72U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW3_STBY_1_11875_V       (0x73U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW3_STBY_1_125_V         (0x74U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW3_STBY_1_13125_V       (0x75U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW3_STBY_1_1375_V        (0x76U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW3_STBY_1_14375_V       (0x77U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW3_STBY_1_15_V          (0x78U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW3_STBY_1_15625_V       (0x79U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW3_STBY_1_1625_V        (0x7AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW3_STBY_1_16875_V       (0x7BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW3_STBY_1_175_V         (0x7CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW3_STBY_1_18125_V       (0x7DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW3_STBY_1_1875_V        (0x7EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW3_STBY_1_19375_V       (0x7FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW3_STBY_1_2_V           (0x80U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW3_STBY_1_20625_V       (0x81U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW3_STBY_1_2125_V        (0x82U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW3_STBY_1_21875_V       (0x83U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW3_STBY_1_225_V         (0x84U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW3_STBY_1_23125_V       (0x85U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW3_STBY_1_2375_V        (0x86U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW3_STBY_1_24375_V       (0x87U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW3_STBY_1_25_V          (0x88U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW3_STBY_1_25625_V       (0x89U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW3_STBY_1_2625_V        (0x8AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW3_STBY_1_26875_V       (0x8BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW3_STBY_1_275_V         (0x8CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW3_STBY_1_28125_V       (0x8DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW3_STBY_1_2875_V        (0x8EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW3_STBY_1_29375_V       (0x8FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW3_STBY_1_3_V           (0x90U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW3_STBY_1_30625_V       (0x91U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW3_STBY_1_3125_V        (0x92U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW3_STBY_1_31875_V       (0x93U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW3_STBY_1_325_V         (0x94U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW3_STBY_1_33125_V       (0x95U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW3_STBY_1_3375_V        (0x96U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW3_STBY_1_34375_V       (0x97U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW3_STBY_1_35_V          (0x98U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW3_STBY_1_35625_V       (0x99U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW3_STBY_1_3625_V        (0x9AU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW3_STBY_1_36875_V       (0x9BU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW3_STBY_1_375_V         (0x9CU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW3_STBY_1_38125_V       (0x9DU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW3_STBY_1_3875_V        (0x9EU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW3_STBY_1_39375_V       (0x9FU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW3_STBY_1_4_V           (0xA0U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW3_STBY_1_40625_V       (0xA1U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW3_STBY_1_4125_V        (0xA2U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW3_STBY_1_41875_V       (0xA3U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW3_STBY_1_425_V         (0xA4U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW3_STBY_1_43125_V       (0xA5U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW3_STBY_1_4375_V        (0xA6U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW3_STBY_1_44375_V       (0xA7U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW3_STBY_1_45_V          (0xA8U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW3_STBY_1_45625_V       (0xA9U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW3_STBY_1_4625_V        (0xAAU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW3_STBY_1_46875_V       (0xABU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW3_STBY_1_475_V         (0xACU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW3_STBY_1_48125_V       (0xADU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW3_STBY_1_4875_V        (0xAEU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW3_STBY_1_49375_V       (0xAFU << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW3_STBY_1_5_V           (0xB0U << PF8X_F_VSW3_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW3_STBY_1_8_V           (0xB1U << PF8X_F_VSW3_STBY_SHIFT)

/******************************************************************************/
/* SW4_CONFIG1 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW4_CONFIG1_ADDR            0x65U
#define PF8X_SW4_CONFIG1_DEFAULT         0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW4_PG_EN_MASK            0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW4_WDBYPASS_MASK         0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW4_ILIM_STATE_MASK       0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW4_OV_STATE_MASK         0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW4_UV_STATE_MASK         0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW4_ILIM_BYPASS_MASK      0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW4_OV_BYPASS_MASK        0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW4_UV_BYPASS_MASK        0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW4_PG_EN_SHIFT           0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW4_WDBYPASS_SHIFT        0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW4_ILIM_STATE_SHIFT      0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW4_OV_STATE_SHIFT        0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW4_UV_STATE_SHIFT        0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW4_ILIM_BYPASS_SHIFT     0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW4_OV_BYPASS_SHIFT       0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW4_UV_BYPASS_SHIFT       0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_SW4_PG_EN_DISABLED        (0x00U << PF8X_F_SW4_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_SW4_PG_EN_ENABLED         (0x01U << PF8X_F_SW4_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_SW4_WDBYPASS_RESET        (0x00U << PF8X_F_SW4_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_SW4_WDBYPASS_BYPASS       (0x01U << PF8X_F_SW4_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW4_ILIM_STATE_DISABLED   (0x00U << PF8X_F_SW4_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW4_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW4_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW4_OV_STATE_DISABLED     (0x00U << PF8X_F_SW4_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW4_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW4_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW4_UV_STATE_DISABLED     (0x00U << PF8X_F_SW4_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW4_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW4_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW4_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW4_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW4_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW4_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW4_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW4_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW4_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW4_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW4_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW4_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW4_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW4_UV_BYPASS_SHIFT)

/******************************************************************************/
/* SW4_CONFIG2 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW4_CONFIG2_ADDR            0x66U
#define PF8X_SW4_CONFIG2_DEFAULT         0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW4PHASE_MASK             0x07U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW4ILIM_MASK              0x18U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW4DVS_RAMP_MASK          0x20U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW4_FLT_REN_MASK          0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW4PHASE_SHIFT            0x00U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW4ILIM_SHIFT             0x03U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW4DVS_RAMP_SHIFT         0x05U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW4_FLT_REN_SHIFT         0x07U

/**
 * 45_
 */
#define PF8X_F_SW4PHASE_45               (0x00U << PF8X_F_SW4PHASE_SHIFT)
/**
 * 90_
 */
#define PF8X_F_SW4PHASE_90               (0x01U << PF8X_F_SW4PHASE_SHIFT)
/**
 * 135_
 */
#define PF8X_F_SW4PHASE_135              (0x02U << PF8X_F_SW4PHASE_SHIFT)
/**
 * 180_
 */
#define PF8X_F_SW4PHASE_180              (0x03U << PF8X_F_SW4PHASE_SHIFT)
/**
 * 225_
 */
#define PF8X_F_SW4PHASE_225              (0x04U << PF8X_F_SW4PHASE_SHIFT)
/**
 * 270_
 */
#define PF8X_F_SW4PHASE_270              (0x05U << PF8X_F_SW4PHASE_SHIFT)
/**
 * 315_
 */
#define PF8X_F_SW4PHASE_315              (0x06U << PF8X_F_SW4PHASE_SHIFT)
/**
 * 0_
 */
#define PF8X_F_SW4PHASE_0                (0x07U << PF8X_F_SW4PHASE_SHIFT)

/**
 * 2.1 A
 */
#define PF8X_F_SW4ILIM_2_1_A             (0x00U << PF8X_F_SW4ILIM_SHIFT)
/**
 * 2.6 A
 */
#define PF8X_F_SW4ILIM_2_6_A             (0x01U << PF8X_F_SW4ILIM_SHIFT)
/**
 * 3.0 A
 */
#define PF8X_F_SW4ILIM_3_0_A             (0x02U << PF8X_F_SW4ILIM_SHIFT)
/**
 * 4.5 A
 */
#define PF8X_F_SW4ILIM_4_5_A             (0x03U << PF8X_F_SW4ILIM_SHIFT)

/**
 * 6.25 mV/us
 */
#define PF8X_F_SW4DVS_RAMP_6_25          (0x00U << PF8X_F_SW4DVS_RAMP_SHIFT)
/**
 * 12.5 mV/us
 */
#define PF8X_F_SW4DVS_RAMP_12_5          (0x01U << PF8X_F_SW4DVS_RAMP_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_SW4_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_SW4_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_SW4_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_SW4_FLT_REN_SHIFT)

/******************************************************************************/
/* SW4_PWRUP - Type: RW                                                       */
/******************************************************************************/

#define PF8X_SW4_PWRUP_ADDR              0x67U
#define PF8X_SW4_PWRUP_DEFAULT           0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW4_SEQ_MASK              0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW4_SEQ_SHIFT             0x00U

/******************************************************************************/
/* SW4_MODE - Type: RW                                                        */
/******************************************************************************/

#define PF8X_SW4_MODE_ADDR               0x68U
#define PF8X_SW4_MODE_DEFAULT            0x00U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW4_RUN_MODE_MASK         0x03U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW4_STBY_MODE_MASK        0x0CU
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW4_PDGRP_MASK            0x30U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW4_RUN_MODE_SHIFT        0x00U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW4_STBY_MODE_SHIFT       0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW4_PDGRP_SHIFT           0x04U

/**
 * OFF
 */
#define PF8X_F_SW4_RUN_MODE_OFF          (0x00U << PF8X_F_SW4_RUN_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW4_RUN_MODE_PWM          (0x01U << PF8X_F_SW4_RUN_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW4_RUN_MODE_PFM          (0x02U << PF8X_F_SW4_RUN_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW4_RUN_MODE_AUTO         (0x03U << PF8X_F_SW4_RUN_MODE_SHIFT)

/**
 * OFF
 */
#define PF8X_F_SW4_STBY_MODE_OFF         (0x00U << PF8X_F_SW4_STBY_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW4_STBY_MODE_PWM         (0x01U << PF8X_F_SW4_STBY_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW4_STBY_MODE_PFM         (0x02U << PF8X_F_SW4_STBY_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW4_STBY_MODE_AUTO        (0x03U << PF8X_F_SW4_STBY_MODE_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_SW4_PDGRP_GROUP_4         (0x00U << PF8X_F_SW4_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_SW4_PDGRP_GROUP_3         (0x01U << PF8X_F_SW4_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_SW4_PDGRP_GROUP_2         (0x02U << PF8X_F_SW4_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_SW4_PDGRP_GROUP_1         (0x03U << PF8X_F_SW4_PDGRP_SHIFT)

/******************************************************************************/
/* SW4_RUN_VOLT - Type: RW                                                    */
/******************************************************************************/

#define PF8X_SW4_RUN_VOLT_ADDR           0x69U
#define PF8X_SW4_RUN_VOLT_DEFAULT        0x00U

/**
 * SW4 Output voltage configuration in given Mode. It uses the OTP_VSW4 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW4_RUN_MASK             0xFFU

/**
 * SW4 Output voltage configuration in given Mode. It uses the OTP_VSW4 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW4_RUN_SHIFT            0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW4_RUN_0_4_V            (0x00U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW4_RUN_0_40625_V        (0x01U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW4_RUN_0_4125_V         (0x02U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW4_RUN_0_41875_V        (0x03U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW4_RUN_0_425_V          (0x04U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW4_RUN_0_43125_V        (0x05U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW4_RUN_0_4375_V         (0x06U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW4_RUN_0_44375V         (0x07U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW4_RUN_0_45_V           (0x08U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW4_RUN_0_45625_V        (0x09U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW4_RUN_0_4625_V         (0x0AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW4_RUN_0_46875_V        (0x0BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW4_RUN_0_475_V          (0x0CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW4_RUN_0_48125_V        (0x0DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW4_RUN_0_4875_V         (0x0EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW4_RUN_0_49375_V        (0x0FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW4_RUN_0_5_V            (0x10U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW4_RUN_0_50625_V        (0x11U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW4_RUN_0_5125_V         (0x12U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW4_RUN_0_51875_V        (0x13U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW4_RUN_0_525_V          (0x14U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW4_RUN_0_53125_V        (0x15U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW4_RUN_0_5375_V         (0x16U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW4_RUN_0_54375_V        (0x17U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW4_RUN_0_55_V           (0x18U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW4_RUN_0_55625_V        (0x19U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW4_RUN_0_5625_V         (0x1AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW4_RUN_0_56875_V        (0x1BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW4_RUN_0_575_V          (0x1CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW4_RUN_0_58125_V        (0x1DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW4_RUN_0_5875_V         (0x1EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW4_RUN_0_59375_V        (0x1FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW4_RUN_0_6_V            (0x20U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW4_RUN_0_60625_V        (0x21U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW4_RUN_0_6125_V         (0x22U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW4_RUN_0_61875_V        (0x23U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW4_RUN_0_625_V          (0x24U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW4_RUN_0_63125_V        (0x25U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW4_RUN_0_6375_V         (0x26U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW4_RUN_0_64375_V        (0x27U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW4_RUN_0_65_V           (0x28U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW4_RUN_0_65625_V        (0x29U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW4_RUN_0_6625_V         (0x2AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW4_RUN_0_66875_V        (0x2BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW4_RUN_0_675_V          (0x2CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW4_RUN_0_68125_V        (0x2DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW4_RUN_0_6875_V         (0x2EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW4_RUN_0_69375_V        (0x2FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW4_RUN_0_7_V            (0x30U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW4_RUN_0_70625_V        (0x31U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW4_RUN_0_7125_V         (0x32U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW4_RUN_0_71875_V        (0x33U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW4_RUN_0_725_V          (0x34U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW4_RUN_0_73125_V        (0x35U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW4_RUN_0_7375_V         (0x36U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW4_RUN_0_74375_V        (0x37U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW4_RUN_0_75_V           (0x38U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW4_RUN_0_75625_V        (0x39U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW4_RUN_0_7625_V         (0x3AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW4_RUN_0_76875_V        (0x3BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW4_RUN_0_775_V          (0x3CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW4_RUN_0_78125_V        (0x3DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW4_RUN_0_7875_V         (0x3EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW4_RUN_0_79375_V        (0x3FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW4_RUN_0_8_V            (0x40U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW4_RUN_0_80625_V        (0x41U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW4_RUN_0_8125_V         (0x42U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW4_RUN_0_81875_V        (0x43U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW4_RUN_0_825_V          (0x44U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW4_RUN_0_83125_V        (0x45U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW4_RUN_0_8375_V         (0x46U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW4_RUN_0_84375_V        (0x47U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW4_RUN_0_85_V           (0x48U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW4_RUN_0_85625_V        (0x49U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW4_RUN_0_8625_V         (0x4AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW4_RUN_0_86875_V        (0x4BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW4_RUN_0_875_V          (0x4CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW4_RUN_0_88125_V        (0x4DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW4_RUN_0_8875_V         (0x4EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW4_RUN_0_89375_V        (0x4FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW4_RUN_0_9_V            (0x50U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW4_RUN_0_90625_V        (0x51U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW4_RUN_0_9125_V         (0x52U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW4_RUN_0_91875_V        (0x53U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW4_RUN_0_925_V          (0x54U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW4_RUN_0_93125_V        (0x55U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW4_RUN_0_9375_V         (0x56U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW4_RUN_0_94375_V        (0x57U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW4_RUN_0_95_V           (0x58U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW4_RUN_0_95625_V        (0x59U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW4_RUN_0_9625_V         (0x5AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW4_RUN_0_96875_V        (0x5BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW4_RUN_0_975_V          (0x5CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW4_RUN_0_98125_V        (0x5DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW4_RUN_0_9875_V         (0x5EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW4_RUN_0_99375_V        (0x5FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW4_RUN_1_V              (0x60U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW4_RUN_1_00625_V        (0x61U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW4_RUN_1_0125_V         (0x62U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW4_RUN_1_01875_V        (0x63U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW4_RUN_1_025_V          (0x64U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW4_RUN_1_03125_V        (0x65U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW4_RUN_1_0375_V         (0x66U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW4_RUN_1_04375_V        (0x67U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW4_RUN_1_05_V           (0x68U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW4_RUN_1_05625_V        (0x69U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW4_RUN_1_0625_V         (0x6AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW4_RUN_1_06875_V        (0x6BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW4_RUN_1_075_V          (0x6CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW4_RUN_1_08125_V        (0x6DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW4_RUN_1_0875_V         (0x6EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW4_RUN_1_09375_V        (0x6FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW4_RUN_1_1_V            (0x70U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW4_RUN_1_10625_V        (0x71U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW4_RUN_1_1125_V         (0x72U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW4_RUN_1_11875_V        (0x73U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW4_RUN_1_125_V          (0x74U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW4_RUN_1_13125_V        (0x75U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW4_RUN_1_1375_V         (0x76U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW4_RUN_1_14375_V        (0x77U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW4_RUN_1_15_V           (0x78U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW4_RUN_1_15625_V        (0x79U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW4_RUN_1_1625_V         (0x7AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW4_RUN_1_16875_V        (0x7BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW4_RUN_1_175_V          (0x7CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW4_RUN_1_18125_V        (0x7DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW4_RUN_1_1875_V         (0x7EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW4_RUN_1_19375_V        (0x7FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW4_RUN_1_2_V            (0x80U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW4_RUN_1_20625_V        (0x81U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW4_RUN_1_2125_V         (0x82U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW4_RUN_1_21875_V        (0x83U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW4_RUN_1_225_V          (0x84U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW4_RUN_1_23125_V        (0x85U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW4_RUN_1_2375_V         (0x86U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW4_RUN_1_24375_V        (0x87U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW4_RUN_1_25_V           (0x88U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW4_RUN_1_25625_V        (0x89U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW4_RUN_1_2625_V         (0x8AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW4_RUN_1_26875_V        (0x8BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW4_RUN_1_275_V          (0x8CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW4_RUN_1_28125_V        (0x8DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW4_RUN_1_2875_V         (0x8EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW4_RUN_1_29375_V        (0x8FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW4_RUN_1_3_V            (0x90U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW4_RUN_1_30625_V        (0x91U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW4_RUN_1_3125_V         (0x92U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW4_RUN_1_31875_V        (0x93U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW4_RUN_1_325_V          (0x94U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW4_RUN_1_33125_V        (0x95U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW4_RUN_1_3375_V         (0x96U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW4_RUN_1_34375_V        (0x97U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW4_RUN_1_35_V           (0x98U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW4_RUN_1_35625_V        (0x99U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW4_RUN_1_3625_V         (0x9AU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW4_RUN_1_36875_V        (0x9BU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW4_RUN_1_375_V          (0x9CU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW4_RUN_1_38125_V        (0x9DU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW4_RUN_1_3875_V         (0x9EU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW4_RUN_1_39375_V        (0x9FU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW4_RUN_1_4_V            (0xA0U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW4_RUN_1_40625_V        (0xA1U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW4_RUN_1_4125_V         (0xA2U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW4_RUN_1_41875_V        (0xA3U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW4_RUN_1_425_V          (0xA4U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW4_RUN_1_43125_V        (0xA5U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW4_RUN_1_4375_V         (0xA6U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW4_RUN_1_44375_V        (0xA7U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW4_RUN_1_45_V           (0xA8U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW4_RUN_1_45625_V        (0xA9U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW4_RUN_1_4625_V         (0xAAU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW4_RUN_1_46875_V        (0xABU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW4_RUN_1_475_V          (0xACU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW4_RUN_1_48125_V        (0xADU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW4_RUN_1_4875_V         (0xAEU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW4_RUN_1_49375_V        (0xAFU << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW4_RUN_1_5_V            (0xB0U << PF8X_F_VSW4_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW4_RUN_1_8_V            (0xB1U << PF8X_F_VSW4_RUN_SHIFT)

/******************************************************************************/
/* SW4_STBY_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_SW4_STBY_VOLT_ADDR          0x6AU
#define PF8X_SW4_STBY_VOLT_DEFAULT       0x00U

/**
 * SW4 Output voltage configuration in given Mode. It uses the OTP_VSW4 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW4_STBY_MASK            0xFFU

/**
 * SW4 Output voltage configuration in given Mode. It uses the OTP_VSW4 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW4_STBY_SHIFT           0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW4_STBY_0_4_V           (0x00U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW4_STBY_0_40625_V       (0x01U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW4_STBY_0_4125_V        (0x02U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW4_STBY_0_41875_V       (0x03U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW4_STBY_0_425_V         (0x04U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW4_STBY_0_43125_V       (0x05U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW4_STBY_0_4375_V        (0x06U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW4_STBY_0_44375V        (0x07U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW4_STBY_0_45_V          (0x08U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW4_STBY_0_45625_V       (0x09U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW4_STBY_0_4625_V        (0x0AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW4_STBY_0_46875_V       (0x0BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW4_STBY_0_475_V         (0x0CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW4_STBY_0_48125_V       (0x0DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW4_STBY_0_4875_V        (0x0EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW4_STBY_0_49375_V       (0x0FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW4_STBY_0_5_V           (0x10U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW4_STBY_0_50625_V       (0x11U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW4_STBY_0_5125_V        (0x12U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW4_STBY_0_51875_V       (0x13U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW4_STBY_0_525_V         (0x14U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW4_STBY_0_53125_V       (0x15U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW4_STBY_0_5375_V        (0x16U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW4_STBY_0_54375_V       (0x17U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW4_STBY_0_55_V          (0x18U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW4_STBY_0_55625_V       (0x19U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW4_STBY_0_5625_V        (0x1AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW4_STBY_0_56875_V       (0x1BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW4_STBY_0_575_V         (0x1CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW4_STBY_0_58125_V       (0x1DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW4_STBY_0_5875_V        (0x1EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW4_STBY_0_59375_V       (0x1FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW4_STBY_0_6_V           (0x20U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW4_STBY_0_60625_V       (0x21U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW4_STBY_0_6125_V        (0x22U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW4_STBY_0_61875_V       (0x23U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW4_STBY_0_625_V         (0x24U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW4_STBY_0_63125_V       (0x25U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW4_STBY_0_6375_V        (0x26U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW4_STBY_0_64375_V       (0x27U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW4_STBY_0_65_V          (0x28U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW4_STBY_0_65625_V       (0x29U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW4_STBY_0_6625_V        (0x2AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW4_STBY_0_66875_V       (0x2BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW4_STBY_0_675_V         (0x2CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW4_STBY_0_68125_V       (0x2DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW4_STBY_0_6875_V        (0x2EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW4_STBY_0_69375_V       (0x2FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW4_STBY_0_7_V           (0x30U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW4_STBY_0_70625_V       (0x31U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW4_STBY_0_7125_V        (0x32U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW4_STBY_0_71875_V       (0x33U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW4_STBY_0_725_V         (0x34U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW4_STBY_0_73125_V       (0x35U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW4_STBY_0_7375_V        (0x36U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW4_STBY_0_74375_V       (0x37U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW4_STBY_0_75_V          (0x38U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW4_STBY_0_75625_V       (0x39U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW4_STBY_0_7625_V        (0x3AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW4_STBY_0_76875_V       (0x3BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW4_STBY_0_775_V         (0x3CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW4_STBY_0_78125_V       (0x3DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW4_STBY_0_7875_V        (0x3EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW4_STBY_0_79375_V       (0x3FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW4_STBY_0_8_V           (0x40U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW4_STBY_0_80625_V       (0x41U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW4_STBY_0_8125_V        (0x42U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW4_STBY_0_81875_V       (0x43U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW4_STBY_0_825_V         (0x44U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW4_STBY_0_83125_V       (0x45U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW4_STBY_0_8375_V        (0x46U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW4_STBY_0_84375_V       (0x47U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW4_STBY_0_85_V          (0x48U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW4_STBY_0_85625_V       (0x49U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW4_STBY_0_8625_V        (0x4AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW4_STBY_0_86875_V       (0x4BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW4_STBY_0_875_V         (0x4CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW4_STBY_0_88125_V       (0x4DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW4_STBY_0_8875_V        (0x4EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW4_STBY_0_89375_V       (0x4FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW4_STBY_0_9_V           (0x50U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW4_STBY_0_90625_V       (0x51U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW4_STBY_0_9125_V        (0x52U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW4_STBY_0_91875_V       (0x53U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW4_STBY_0_925_V         (0x54U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW4_STBY_0_93125_V       (0x55U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW4_STBY_0_9375_V        (0x56U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW4_STBY_0_94375_V       (0x57U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW4_STBY_0_95_V          (0x58U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW4_STBY_0_95625_V       (0x59U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW4_STBY_0_9625_V        (0x5AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW4_STBY_0_96875_V       (0x5BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW4_STBY_0_975_V         (0x5CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW4_STBY_0_98125_V       (0x5DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW4_STBY_0_9875_V        (0x5EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW4_STBY_0_99375_V       (0x5FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW4_STBY_1_V             (0x60U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW4_STBY_1_00625_V       (0x61U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW4_STBY_1_0125_V        (0x62U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW4_STBY_1_01875_V       (0x63U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW4_STBY_1_025_V         (0x64U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW4_STBY_1_03125_V       (0x65U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW4_STBY_1_0375_V        (0x66U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW4_STBY_1_04375_V       (0x67U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW4_STBY_1_05_V          (0x68U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW4_STBY_1_05625_V       (0x69U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW4_STBY_1_0625_V        (0x6AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW4_STBY_1_06875_V       (0x6BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW4_STBY_1_075_V         (0x6CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW4_STBY_1_08125_V       (0x6DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW4_STBY_1_0875_V        (0x6EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW4_STBY_1_09375_V       (0x6FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW4_STBY_1_1_V           (0x70U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW4_STBY_1_10625_V       (0x71U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW4_STBY_1_1125_V        (0x72U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW4_STBY_1_11875_V       (0x73U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW4_STBY_1_125_V         (0x74U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW4_STBY_1_13125_V       (0x75U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW4_STBY_1_1375_V        (0x76U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW4_STBY_1_14375_V       (0x77U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW4_STBY_1_15_V          (0x78U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW4_STBY_1_15625_V       (0x79U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW4_STBY_1_1625_V        (0x7AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW4_STBY_1_16875_V       (0x7BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW4_STBY_1_175_V         (0x7CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW4_STBY_1_18125_V       (0x7DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW4_STBY_1_1875_V        (0x7EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW4_STBY_1_19375_V       (0x7FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW4_STBY_1_2_V           (0x80U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW4_STBY_1_20625_V       (0x81U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW4_STBY_1_2125_V        (0x82U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW4_STBY_1_21875_V       (0x83U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW4_STBY_1_225_V         (0x84U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW4_STBY_1_23125_V       (0x85U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW4_STBY_1_2375_V        (0x86U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW4_STBY_1_24375_V       (0x87U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW4_STBY_1_25_V          (0x88U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW4_STBY_1_25625_V       (0x89U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW4_STBY_1_2625_V        (0x8AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW4_STBY_1_26875_V       (0x8BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW4_STBY_1_275_V         (0x8CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW4_STBY_1_28125_V       (0x8DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW4_STBY_1_2875_V        (0x8EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW4_STBY_1_29375_V       (0x8FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW4_STBY_1_3_V           (0x90U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW4_STBY_1_30625_V       (0x91U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW4_STBY_1_3125_V        (0x92U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW4_STBY_1_31875_V       (0x93U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW4_STBY_1_325_V         (0x94U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW4_STBY_1_33125_V       (0x95U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW4_STBY_1_3375_V        (0x96U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW4_STBY_1_34375_V       (0x97U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW4_STBY_1_35_V          (0x98U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW4_STBY_1_35625_V       (0x99U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW4_STBY_1_3625_V        (0x9AU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW4_STBY_1_36875_V       (0x9BU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW4_STBY_1_375_V         (0x9CU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW4_STBY_1_38125_V       (0x9DU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW4_STBY_1_3875_V        (0x9EU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW4_STBY_1_39375_V       (0x9FU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW4_STBY_1_4_V           (0xA0U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW4_STBY_1_40625_V       (0xA1U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW4_STBY_1_4125_V        (0xA2U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW4_STBY_1_41875_V       (0xA3U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW4_STBY_1_425_V         (0xA4U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW4_STBY_1_43125_V       (0xA5U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW4_STBY_1_4375_V        (0xA6U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW4_STBY_1_44375_V       (0xA7U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW4_STBY_1_45_V          (0xA8U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW4_STBY_1_45625_V       (0xA9U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW4_STBY_1_4625_V        (0xAAU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW4_STBY_1_46875_V       (0xABU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW4_STBY_1_475_V         (0xACU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW4_STBY_1_48125_V       (0xADU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW4_STBY_1_4875_V        (0xAEU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW4_STBY_1_49375_V       (0xAFU << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW4_STBY_1_5_V           (0xB0U << PF8X_F_VSW4_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW4_STBY_1_8_V           (0xB1U << PF8X_F_VSW4_STBY_SHIFT)

/******************************************************************************/
/* SW5_CONFIG1 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW5_CONFIG1_ADDR            0x6DU
#define PF8X_SW5_CONFIG1_DEFAULT         0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW5_PG_EN_MASK            0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW5_WDBYPASS_MASK         0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW5_ILIM_STATE_MASK       0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW5_OV_STATE_MASK         0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW5_UV_STATE_MASK         0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW5_ILIM_BYPASS_MASK      0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW5_OV_BYPASS_MASK        0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW5_UV_BYPASS_MASK        0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW5_PG_EN_SHIFT           0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW5_WDBYPASS_SHIFT        0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW5_ILIM_STATE_SHIFT      0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW5_OV_STATE_SHIFT        0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW5_UV_STATE_SHIFT        0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW5_ILIM_BYPASS_SHIFT     0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW5_OV_BYPASS_SHIFT       0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW5_UV_BYPASS_SHIFT       0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_SW5_PG_EN_DISABLED        (0x00U << PF8X_F_SW5_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_SW5_PG_EN_ENABLED         (0x01U << PF8X_F_SW5_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_SW5_WDBYPASS_RESET        (0x00U << PF8X_F_SW5_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_SW5_WDBYPASS_BYPASS       (0x01U << PF8X_F_SW5_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW5_ILIM_STATE_DISABLED   (0x00U << PF8X_F_SW5_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW5_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW5_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW5_OV_STATE_DISABLED     (0x00U << PF8X_F_SW5_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW5_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW5_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW5_UV_STATE_DISABLED     (0x00U << PF8X_F_SW5_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW5_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW5_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW5_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW5_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW5_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW5_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW5_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW5_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW5_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW5_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW5_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW5_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW5_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW5_UV_BYPASS_SHIFT)

/******************************************************************************/
/* SW5_CONFIG2 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW5_CONFIG2_ADDR            0x6EU
#define PF8X_SW5_CONFIG2_DEFAULT         0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW5PHASE_MASK             0x07U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW5ILIM_MASK              0x18U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW5DVS_RAMP_MASK          0x20U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW5_FLT_REN_MASK          0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW5PHASE_SHIFT            0x00U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW5ILIM_SHIFT             0x03U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW5DVS_RAMP_SHIFT         0x05U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW5_FLT_REN_SHIFT         0x07U

/**
 * 45_
 */
#define PF8X_F_SW5PHASE_45               (0x00U << PF8X_F_SW5PHASE_SHIFT)
/**
 * 90_
 */
#define PF8X_F_SW5PHASE_90               (0x01U << PF8X_F_SW5PHASE_SHIFT)
/**
 * 135_
 */
#define PF8X_F_SW5PHASE_135              (0x02U << PF8X_F_SW5PHASE_SHIFT)
/**
 * 180_
 */
#define PF8X_F_SW5PHASE_180              (0x03U << PF8X_F_SW5PHASE_SHIFT)
/**
 * 225_
 */
#define PF8X_F_SW5PHASE_225              (0x04U << PF8X_F_SW5PHASE_SHIFT)
/**
 * 270_
 */
#define PF8X_F_SW5PHASE_270              (0x05U << PF8X_F_SW5PHASE_SHIFT)
/**
 * 315_
 */
#define PF8X_F_SW5PHASE_315              (0x06U << PF8X_F_SW5PHASE_SHIFT)
/**
 * 0_
 */
#define PF8X_F_SW5PHASE_0                (0x07U << PF8X_F_SW5PHASE_SHIFT)

/**
 * 2.1 A
 */
#define PF8X_F_SW5ILIM_2_1_A             (0x00U << PF8X_F_SW5ILIM_SHIFT)
/**
 * 2.6 A
 */
#define PF8X_F_SW5ILIM_2_6_A             (0x01U << PF8X_F_SW5ILIM_SHIFT)
/**
 * 3.0 A
 */
#define PF8X_F_SW5ILIM_3_0_A             (0x02U << PF8X_F_SW5ILIM_SHIFT)
/**
 * 4.5 A
 */
#define PF8X_F_SW5ILIM_4_5_A             (0x03U << PF8X_F_SW5ILIM_SHIFT)

/**
 * 6.25 mV/us
 */
#define PF8X_F_SW5DVS_RAMP_6_25          (0x00U << PF8X_F_SW5DVS_RAMP_SHIFT)
/**
 * 12.5 mV/us
 */
#define PF8X_F_SW5DVS_RAMP_12_5          (0x01U << PF8X_F_SW5DVS_RAMP_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_SW5_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_SW5_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_SW5_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_SW5_FLT_REN_SHIFT)

/******************************************************************************/
/* SW5_PWRUP - Type: RW                                                       */
/******************************************************************************/

#define PF8X_SW5_PWRUP_ADDR              0x6FU
#define PF8X_SW5_PWRUP_DEFAULT           0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW5_SEQ_MASK              0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW5_SEQ_SHIFT             0x00U

/******************************************************************************/
/* SW5_MODE - Type: RW                                                        */
/******************************************************************************/

#define PF8X_SW5_MODE_ADDR               0x70U
#define PF8X_SW5_MODE_DEFAULT            0x00U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW5_RUN_MODE_MASK         0x03U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW5_STBY_MODE_MASK        0x0CU
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW5_PDGRP_MASK            0x30U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW5_RUN_MODE_SHIFT        0x00U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW5_STBY_MODE_SHIFT       0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW5_PDGRP_SHIFT           0x04U

/**
 * OFF
 */
#define PF8X_F_SW5_RUN_MODE_OFF          (0x00U << PF8X_F_SW5_RUN_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW5_RUN_MODE_PWM          (0x01U << PF8X_F_SW5_RUN_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW5_RUN_MODE_PFM          (0x02U << PF8X_F_SW5_RUN_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW5_RUN_MODE_AUTO         (0x03U << PF8X_F_SW5_RUN_MODE_SHIFT)

/**
 * OFF
 */
#define PF8X_F_SW5_STBY_MODE_OFF         (0x00U << PF8X_F_SW5_STBY_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW5_STBY_MODE_PWM         (0x01U << PF8X_F_SW5_STBY_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW5_STBY_MODE_PFM         (0x02U << PF8X_F_SW5_STBY_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW5_STBY_MODE_AUTO        (0x03U << PF8X_F_SW5_STBY_MODE_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_SW5_PDGRP_GROUP_4         (0x00U << PF8X_F_SW5_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_SW5_PDGRP_GROUP_3         (0x01U << PF8X_F_SW5_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_SW5_PDGRP_GROUP_2         (0x02U << PF8X_F_SW5_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_SW5_PDGRP_GROUP_1         (0x03U << PF8X_F_SW5_PDGRP_SHIFT)

/******************************************************************************/
/* SW5_RUN_VOLT - Type: RW                                                    */
/******************************************************************************/

#define PF8X_SW5_RUN_VOLT_ADDR           0x71U
#define PF8X_SW5_RUN_VOLT_DEFAULT        0x00U

/**
 * SW5 Output voltage configuration in given Mode. It uses the OTP_VSW5 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW5_RUN_MASK             0xFFU

/**
 * SW5 Output voltage configuration in given Mode. It uses the OTP_VSW5 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW5_RUN_SHIFT            0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW5_RUN_0_4_V            (0x00U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW5_RUN_0_40625_V        (0x01U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW5_RUN_0_4125_V         (0x02U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW5_RUN_0_41875_V        (0x03U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW5_RUN_0_425_V          (0x04U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW5_RUN_0_43125_V        (0x05U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW5_RUN_0_4375_V         (0x06U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW5_RUN_0_44375V         (0x07U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW5_RUN_0_45_V           (0x08U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW5_RUN_0_45625_V        (0x09U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW5_RUN_0_4625_V         (0x0AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW5_RUN_0_46875_V        (0x0BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW5_RUN_0_475_V          (0x0CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW5_RUN_0_48125_V        (0x0DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW5_RUN_0_4875_V         (0x0EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW5_RUN_0_49375_V        (0x0FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW5_RUN_0_5_V            (0x10U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW5_RUN_0_50625_V        (0x11U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW5_RUN_0_5125_V         (0x12U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW5_RUN_0_51875_V        (0x13U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW5_RUN_0_525_V          (0x14U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW5_RUN_0_53125_V        (0x15U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW5_RUN_0_5375_V         (0x16U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW5_RUN_0_54375_V        (0x17U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW5_RUN_0_55_V           (0x18U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW5_RUN_0_55625_V        (0x19U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW5_RUN_0_5625_V         (0x1AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW5_RUN_0_56875_V        (0x1BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW5_RUN_0_575_V          (0x1CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW5_RUN_0_58125_V        (0x1DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW5_RUN_0_5875_V         (0x1EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW5_RUN_0_59375_V        (0x1FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW5_RUN_0_6_V            (0x20U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW5_RUN_0_60625_V        (0x21U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW5_RUN_0_6125_V         (0x22U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW5_RUN_0_61875_V        (0x23U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW5_RUN_0_625_V          (0x24U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW5_RUN_0_63125_V        (0x25U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW5_RUN_0_6375_V         (0x26U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW5_RUN_0_64375_V        (0x27U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW5_RUN_0_65_V           (0x28U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW5_RUN_0_65625_V        (0x29U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW5_RUN_0_6625_V         (0x2AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW5_RUN_0_66875_V        (0x2BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW5_RUN_0_675_V          (0x2CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW5_RUN_0_68125_V        (0x2DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW5_RUN_0_6875_V         (0x2EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW5_RUN_0_69375_V        (0x2FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW5_RUN_0_7_V            (0x30U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW5_RUN_0_70625_V        (0x31U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW5_RUN_0_7125_V         (0x32U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW5_RUN_0_71875_V        (0x33U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW5_RUN_0_725_V          (0x34U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW5_RUN_0_73125_V        (0x35U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW5_RUN_0_7375_V         (0x36U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW5_RUN_0_74375_V        (0x37U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW5_RUN_0_75_V           (0x38U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW5_RUN_0_75625_V        (0x39U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW5_RUN_0_7625_V         (0x3AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW5_RUN_0_76875_V        (0x3BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW5_RUN_0_775_V          (0x3CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW5_RUN_0_78125_V        (0x3DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW5_RUN_0_7875_V         (0x3EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW5_RUN_0_79375_V        (0x3FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW5_RUN_0_8_V            (0x40U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW5_RUN_0_80625_V        (0x41U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW5_RUN_0_8125_V         (0x42U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW5_RUN_0_81875_V        (0x43U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW5_RUN_0_825_V          (0x44U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW5_RUN_0_83125_V        (0x45U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW5_RUN_0_8375_V         (0x46U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW5_RUN_0_84375_V        (0x47U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW5_RUN_0_85_V           (0x48U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW5_RUN_0_85625_V        (0x49U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW5_RUN_0_8625_V         (0x4AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW5_RUN_0_86875_V        (0x4BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW5_RUN_0_875_V          (0x4CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW5_RUN_0_88125_V        (0x4DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW5_RUN_0_8875_V         (0x4EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW5_RUN_0_89375_V        (0x4FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW5_RUN_0_9_V            (0x50U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW5_RUN_0_90625_V        (0x51U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW5_RUN_0_9125_V         (0x52U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW5_RUN_0_91875_V        (0x53U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW5_RUN_0_925_V          (0x54U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW5_RUN_0_93125_V        (0x55U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW5_RUN_0_9375_V         (0x56U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW5_RUN_0_94375_V        (0x57U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW5_RUN_0_95_V           (0x58U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW5_RUN_0_95625_V        (0x59U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW5_RUN_0_9625_V         (0x5AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW5_RUN_0_96875_V        (0x5BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW5_RUN_0_975_V          (0x5CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW5_RUN_0_98125_V        (0x5DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW5_RUN_0_9875_V         (0x5EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW5_RUN_0_99375_V        (0x5FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW5_RUN_1_V              (0x60U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW5_RUN_1_00625_V        (0x61U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW5_RUN_1_0125_V         (0x62U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW5_RUN_1_01875_V        (0x63U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW5_RUN_1_025_V          (0x64U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW5_RUN_1_03125_V        (0x65U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW5_RUN_1_0375_V         (0x66U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW5_RUN_1_04375_V        (0x67U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW5_RUN_1_05_V           (0x68U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW5_RUN_1_05625_V        (0x69U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW5_RUN_1_0625_V         (0x6AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW5_RUN_1_06875_V        (0x6BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW5_RUN_1_075_V          (0x6CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW5_RUN_1_08125_V        (0x6DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW5_RUN_1_0875_V         (0x6EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW5_RUN_1_09375_V        (0x6FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW5_RUN_1_1_V            (0x70U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW5_RUN_1_10625_V        (0x71U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW5_RUN_1_1125_V         (0x72U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW5_RUN_1_11875_V        (0x73U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW5_RUN_1_125_V          (0x74U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW5_RUN_1_13125_V        (0x75U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW5_RUN_1_1375_V         (0x76U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW5_RUN_1_14375_V        (0x77U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW5_RUN_1_15_V           (0x78U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW5_RUN_1_15625_V        (0x79U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW5_RUN_1_1625_V         (0x7AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW5_RUN_1_16875_V        (0x7BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW5_RUN_1_175_V          (0x7CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW5_RUN_1_18125_V        (0x7DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW5_RUN_1_1875_V         (0x7EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW5_RUN_1_19375_V        (0x7FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW5_RUN_1_2_V            (0x80U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW5_RUN_1_20625_V        (0x81U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW5_RUN_1_2125_V         (0x82U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW5_RUN_1_21875_V        (0x83U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW5_RUN_1_225_V          (0x84U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW5_RUN_1_23125_V        (0x85U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW5_RUN_1_2375_V         (0x86U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW5_RUN_1_24375_V        (0x87U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW5_RUN_1_25_V           (0x88U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW5_RUN_1_25625_V        (0x89U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW5_RUN_1_2625_V         (0x8AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW5_RUN_1_26875_V        (0x8BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW5_RUN_1_275_V          (0x8CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW5_RUN_1_28125_V        (0x8DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW5_RUN_1_2875_V         (0x8EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW5_RUN_1_29375_V        (0x8FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW5_RUN_1_3_V            (0x90U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW5_RUN_1_30625_V        (0x91U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW5_RUN_1_3125_V         (0x92U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW5_RUN_1_31875_V        (0x93U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW5_RUN_1_325_V          (0x94U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW5_RUN_1_33125_V        (0x95U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW5_RUN_1_3375_V         (0x96U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW5_RUN_1_34375_V        (0x97U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW5_RUN_1_35_V           (0x98U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW5_RUN_1_35625_V        (0x99U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW5_RUN_1_3625_V         (0x9AU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW5_RUN_1_36875_V        (0x9BU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW5_RUN_1_375_V          (0x9CU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW5_RUN_1_38125_V        (0x9DU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW5_RUN_1_3875_V         (0x9EU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW5_RUN_1_39375_V        (0x9FU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW5_RUN_1_4_V            (0xA0U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW5_RUN_1_40625_V        (0xA1U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW5_RUN_1_4125_V         (0xA2U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW5_RUN_1_41875_V        (0xA3U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW5_RUN_1_425_V          (0xA4U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW5_RUN_1_43125_V        (0xA5U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW5_RUN_1_4375_V         (0xA6U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW5_RUN_1_44375_V        (0xA7U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW5_RUN_1_45_V           (0xA8U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW5_RUN_1_45625_V        (0xA9U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW5_RUN_1_4625_V         (0xAAU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW5_RUN_1_46875_V        (0xABU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW5_RUN_1_475_V          (0xACU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW5_RUN_1_48125_V        (0xADU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW5_RUN_1_4875_V         (0xAEU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW5_RUN_1_49375_V        (0xAFU << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW5_RUN_1_5_V            (0xB0U << PF8X_F_VSW5_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW5_RUN_1_8_V            (0xB1U << PF8X_F_VSW5_RUN_SHIFT)

/******************************************************************************/
/* SW5_STBY_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_SW5_STBY_VOLT_ADDR          0x72U
#define PF8X_SW5_STBY_VOLT_DEFAULT       0x00U

/**
 * SW5 Output voltage configuration in given Mode. It uses the OTP_VSW5 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW5_STBY_MASK            0xFFU

/**
 * SW5 Output voltage configuration in given Mode. It uses the OTP_VSW5 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW5_STBY_SHIFT           0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW5_STBY_0_4_V           (0x00U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW5_STBY_0_40625_V       (0x01U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW5_STBY_0_4125_V        (0x02U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW5_STBY_0_41875_V       (0x03U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW5_STBY_0_425_V         (0x04U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW5_STBY_0_43125_V       (0x05U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW5_STBY_0_4375_V        (0x06U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW5_STBY_0_44375V        (0x07U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW5_STBY_0_45_V          (0x08U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW5_STBY_0_45625_V       (0x09U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW5_STBY_0_4625_V        (0x0AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW5_STBY_0_46875_V       (0x0BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW5_STBY_0_475_V         (0x0CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW5_STBY_0_48125_V       (0x0DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW5_STBY_0_4875_V        (0x0EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW5_STBY_0_49375_V       (0x0FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW5_STBY_0_5_V           (0x10U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW5_STBY_0_50625_V       (0x11U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW5_STBY_0_5125_V        (0x12U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW5_STBY_0_51875_V       (0x13U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW5_STBY_0_525_V         (0x14U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW5_STBY_0_53125_V       (0x15U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW5_STBY_0_5375_V        (0x16U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW5_STBY_0_54375_V       (0x17U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW5_STBY_0_55_V          (0x18U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW5_STBY_0_55625_V       (0x19U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW5_STBY_0_5625_V        (0x1AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW5_STBY_0_56875_V       (0x1BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW5_STBY_0_575_V         (0x1CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW5_STBY_0_58125_V       (0x1DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW5_STBY_0_5875_V        (0x1EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW5_STBY_0_59375_V       (0x1FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW5_STBY_0_6_V           (0x20U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW5_STBY_0_60625_V       (0x21U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW5_STBY_0_6125_V        (0x22U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW5_STBY_0_61875_V       (0x23U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW5_STBY_0_625_V         (0x24U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW5_STBY_0_63125_V       (0x25U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW5_STBY_0_6375_V        (0x26U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW5_STBY_0_64375_V       (0x27U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW5_STBY_0_65_V          (0x28U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW5_STBY_0_65625_V       (0x29U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW5_STBY_0_6625_V        (0x2AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW5_STBY_0_66875_V       (0x2BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW5_STBY_0_675_V         (0x2CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW5_STBY_0_68125_V       (0x2DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW5_STBY_0_6875_V        (0x2EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW5_STBY_0_69375_V       (0x2FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW5_STBY_0_7_V           (0x30U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW5_STBY_0_70625_V       (0x31U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW5_STBY_0_7125_V        (0x32U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW5_STBY_0_71875_V       (0x33U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW5_STBY_0_725_V         (0x34U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW5_STBY_0_73125_V       (0x35U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW5_STBY_0_7375_V        (0x36U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW5_STBY_0_74375_V       (0x37U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW5_STBY_0_75_V          (0x38U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW5_STBY_0_75625_V       (0x39U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW5_STBY_0_7625_V        (0x3AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW5_STBY_0_76875_V       (0x3BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW5_STBY_0_775_V         (0x3CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW5_STBY_0_78125_V       (0x3DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW5_STBY_0_7875_V        (0x3EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW5_STBY_0_79375_V       (0x3FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW5_STBY_0_8_V           (0x40U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW5_STBY_0_80625_V       (0x41U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW5_STBY_0_8125_V        (0x42U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW5_STBY_0_81875_V       (0x43U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW5_STBY_0_825_V         (0x44U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW5_STBY_0_83125_V       (0x45U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW5_STBY_0_8375_V        (0x46U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW5_STBY_0_84375_V       (0x47U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW5_STBY_0_85_V          (0x48U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW5_STBY_0_85625_V       (0x49U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW5_STBY_0_8625_V        (0x4AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW5_STBY_0_86875_V       (0x4BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW5_STBY_0_875_V         (0x4CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW5_STBY_0_88125_V       (0x4DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW5_STBY_0_8875_V        (0x4EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW5_STBY_0_89375_V       (0x4FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW5_STBY_0_9_V           (0x50U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW5_STBY_0_90625_V       (0x51U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW5_STBY_0_9125_V        (0x52U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW5_STBY_0_91875_V       (0x53U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW5_STBY_0_925_V         (0x54U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW5_STBY_0_93125_V       (0x55U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW5_STBY_0_9375_V        (0x56U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW5_STBY_0_94375_V       (0x57U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW5_STBY_0_95_V          (0x58U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW5_STBY_0_95625_V       (0x59U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW5_STBY_0_9625_V        (0x5AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW5_STBY_0_96875_V       (0x5BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW5_STBY_0_975_V         (0x5CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW5_STBY_0_98125_V       (0x5DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW5_STBY_0_9875_V        (0x5EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW5_STBY_0_99375_V       (0x5FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW5_STBY_1_V             (0x60U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW5_STBY_1_00625_V       (0x61U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW5_STBY_1_0125_V        (0x62U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW5_STBY_1_01875_V       (0x63U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW5_STBY_1_025_V         (0x64U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW5_STBY_1_03125_V       (0x65U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW5_STBY_1_0375_V        (0x66U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW5_STBY_1_04375_V       (0x67U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW5_STBY_1_05_V          (0x68U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW5_STBY_1_05625_V       (0x69U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW5_STBY_1_0625_V        (0x6AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW5_STBY_1_06875_V       (0x6BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW5_STBY_1_075_V         (0x6CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW5_STBY_1_08125_V       (0x6DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW5_STBY_1_0875_V        (0x6EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW5_STBY_1_09375_V       (0x6FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW5_STBY_1_1_V           (0x70U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW5_STBY_1_10625_V       (0x71U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW5_STBY_1_1125_V        (0x72U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW5_STBY_1_11875_V       (0x73U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW5_STBY_1_125_V         (0x74U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW5_STBY_1_13125_V       (0x75U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW5_STBY_1_1375_V        (0x76U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW5_STBY_1_14375_V       (0x77U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW5_STBY_1_15_V          (0x78U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW5_STBY_1_15625_V       (0x79U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW5_STBY_1_1625_V        (0x7AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW5_STBY_1_16875_V       (0x7BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW5_STBY_1_175_V         (0x7CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW5_STBY_1_18125_V       (0x7DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW5_STBY_1_1875_V        (0x7EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW5_STBY_1_19375_V       (0x7FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW5_STBY_1_2_V           (0x80U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW5_STBY_1_20625_V       (0x81U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW5_STBY_1_2125_V        (0x82U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW5_STBY_1_21875_V       (0x83U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW5_STBY_1_225_V         (0x84U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW5_STBY_1_23125_V       (0x85U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW5_STBY_1_2375_V        (0x86U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW5_STBY_1_24375_V       (0x87U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW5_STBY_1_25_V          (0x88U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW5_STBY_1_25625_V       (0x89U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW5_STBY_1_2625_V        (0x8AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW5_STBY_1_26875_V       (0x8BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW5_STBY_1_275_V         (0x8CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW5_STBY_1_28125_V       (0x8DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW5_STBY_1_2875_V        (0x8EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW5_STBY_1_29375_V       (0x8FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW5_STBY_1_3_V           (0x90U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW5_STBY_1_30625_V       (0x91U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW5_STBY_1_3125_V        (0x92U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW5_STBY_1_31875_V       (0x93U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW5_STBY_1_325_V         (0x94U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW5_STBY_1_33125_V       (0x95U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW5_STBY_1_3375_V        (0x96U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW5_STBY_1_34375_V       (0x97U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW5_STBY_1_35_V          (0x98U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW5_STBY_1_35625_V       (0x99U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW5_STBY_1_3625_V        (0x9AU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW5_STBY_1_36875_V       (0x9BU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW5_STBY_1_375_V         (0x9CU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW5_STBY_1_38125_V       (0x9DU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW5_STBY_1_3875_V        (0x9EU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW5_STBY_1_39375_V       (0x9FU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW5_STBY_1_4_V           (0xA0U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW5_STBY_1_40625_V       (0xA1U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW5_STBY_1_4125_V        (0xA2U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW5_STBY_1_41875_V       (0xA3U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW5_STBY_1_425_V         (0xA4U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW5_STBY_1_43125_V       (0xA5U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW5_STBY_1_4375_V        (0xA6U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW5_STBY_1_44375_V       (0xA7U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW5_STBY_1_45_V          (0xA8U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW5_STBY_1_45625_V       (0xA9U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW5_STBY_1_4625_V        (0xAAU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW5_STBY_1_46875_V       (0xABU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW5_STBY_1_475_V         (0xACU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW5_STBY_1_48125_V       (0xADU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW5_STBY_1_4875_V        (0xAEU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW5_STBY_1_49375_V       (0xAFU << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW5_STBY_1_5_V           (0xB0U << PF8X_F_VSW5_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW5_STBY_1_8_V           (0xB1U << PF8X_F_VSW5_STBY_SHIFT)

/******************************************************************************/
/* SW6_CONFIG1 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW6_CONFIG1_ADDR            0x75U
#define PF8X_SW6_CONFIG1_DEFAULT         0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW6_PG_EN_MASK            0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW6_WDBYPASS_MASK         0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW6_ILIM_STATE_MASK       0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW6_OV_STATE_MASK         0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW6_UV_STATE_MASK         0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW6_ILIM_BYPASS_MASK      0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW6_OV_BYPASS_MASK        0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW6_UV_BYPASS_MASK        0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW6_PG_EN_SHIFT           0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW6_WDBYPASS_SHIFT        0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW6_ILIM_STATE_SHIFT      0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW6_OV_STATE_SHIFT        0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW6_UV_STATE_SHIFT        0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW6_ILIM_BYPASS_SHIFT     0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW6_OV_BYPASS_SHIFT       0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW6_UV_BYPASS_SHIFT       0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_SW6_PG_EN_DISABLED        (0x00U << PF8X_F_SW6_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_SW6_PG_EN_ENABLED         (0x01U << PF8X_F_SW6_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_SW6_WDBYPASS_RESET        (0x00U << PF8X_F_SW6_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_SW6_WDBYPASS_BYPASS       (0x01U << PF8X_F_SW6_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW6_ILIM_STATE_DISABLED   (0x00U << PF8X_F_SW6_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW6_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW6_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW6_OV_STATE_DISABLED     (0x00U << PF8X_F_SW6_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW6_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW6_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW6_UV_STATE_DISABLED     (0x00U << PF8X_F_SW6_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW6_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW6_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW6_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW6_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW6_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW6_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW6_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW6_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW6_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW6_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW6_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW6_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW6_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW6_UV_BYPASS_SHIFT)

/******************************************************************************/
/* SW6_CONFIG2 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW6_CONFIG2_ADDR            0x76U
#define PF8X_SW6_CONFIG2_DEFAULT         0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW6PHASE_MASK             0x07U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW6ILIM_MASK              0x18U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW6DVS_RAMP_MASK          0x20U
/**
 * Enable VTT mode for SW6. This bit is ignored if OTP_SW5CONFIG[1:0] = 01 (dual phase)
 */
#define PF8X_F_SW6_VTTEN_MASK            0x40U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW6_FLT_REN_MASK          0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW6PHASE_SHIFT            0x00U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW6ILIM_SHIFT             0x03U
/**
 * Set th DVS ramp speed for switching regulator
 */
#define PF8X_F_SW6DVS_RAMP_SHIFT         0x05U
/**
 * Enable VTT mode for SW6. This bit is ignored if OTP_SW5CONFIG[1:0] = 01 (dual phase)
 */
#define PF8X_F_SW6_VTTEN_SHIFT           0x06U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW6_FLT_REN_SHIFT         0x07U

/**
 * 45_
 */
#define PF8X_F_SW6PHASE_45               (0x00U << PF8X_F_SW6PHASE_SHIFT)
/**
 * 90_
 */
#define PF8X_F_SW6PHASE_90               (0x01U << PF8X_F_SW6PHASE_SHIFT)
/**
 * 135_
 */
#define PF8X_F_SW6PHASE_135              (0x02U << PF8X_F_SW6PHASE_SHIFT)
/**
 * 180_
 */
#define PF8X_F_SW6PHASE_180              (0x03U << PF8X_F_SW6PHASE_SHIFT)
/**
 * 225_
 */
#define PF8X_F_SW6PHASE_225              (0x04U << PF8X_F_SW6PHASE_SHIFT)
/**
 * 270_
 */
#define PF8X_F_SW6PHASE_270              (0x05U << PF8X_F_SW6PHASE_SHIFT)
/**
 * 315_
 */
#define PF8X_F_SW6PHASE_315              (0x06U << PF8X_F_SW6PHASE_SHIFT)
/**
 * 0_
 */
#define PF8X_F_SW6PHASE_0                (0x07U << PF8X_F_SW6PHASE_SHIFT)

/**
 * 2.1 A
 */
#define PF8X_F_SW6ILIM_2_1_A             (0x00U << PF8X_F_SW6ILIM_SHIFT)
/**
 * 2.6 A
 */
#define PF8X_F_SW6ILIM_2_6_A             (0x01U << PF8X_F_SW6ILIM_SHIFT)
/**
 * 3.0 A
 */
#define PF8X_F_SW6ILIM_3_0_A             (0x02U << PF8X_F_SW6ILIM_SHIFT)
/**
 * 4.5 A
 */
#define PF8X_F_SW6ILIM_4_5_A             (0x03U << PF8X_F_SW6ILIM_SHIFT)

/**
 * 6.25 mV/us
 */
#define PF8X_F_SW6DVS_RAMP_6_25          (0x00U << PF8X_F_SW6DVS_RAMP_SHIFT)
/**
 * 12.5 mV/us
 */
#define PF8X_F_SW6DVS_RAMP_12_5          (0x01U << PF8X_F_SW6DVS_RAMP_SHIFT)

/**
 * VTT mode disabled
 */
#define PF8X_F_SW6_VTTEN_VTT_DISABLED    (0x00U << PF8X_F_SW6_VTTEN_SHIFT)
/**
 * VTT mode enabled (follows SW5 as reference)
 */
#define PF8X_F_SW6_VTTEN_VTT_ENABLED     (0x01U << PF8X_F_SW6_VTTEN_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_SW6_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_SW6_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_SW6_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_SW6_FLT_REN_SHIFT)

/******************************************************************************/
/* SW6_PWRUP - Type: RW                                                       */
/******************************************************************************/

#define PF8X_SW6_PWRUP_ADDR              0x77U
#define PF8X_SW6_PWRUP_DEFAULT           0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW6_SEQ_MASK              0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW6_SEQ_SHIFT             0x00U

/******************************************************************************/
/* SW6_MODE - Type: RW                                                        */
/******************************************************************************/

#define PF8X_SW6_MODE_ADDR               0x78U
#define PF8X_SW6_MODE_DEFAULT            0x00U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW6_RUN_MODE_MASK         0x03U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW6_STBY_MODE_MASK        0x0CU
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW6_PDGRP_MASK            0x30U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW6_RUN_MODE_SHIFT        0x00U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW6_STBY_MODE_SHIFT       0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW6_PDGRP_SHIFT           0x04U

/**
 * OFF
 */
#define PF8X_F_SW6_RUN_MODE_OFF          (0x00U << PF8X_F_SW6_RUN_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW6_RUN_MODE_PWM          (0x01U << PF8X_F_SW6_RUN_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW6_RUN_MODE_PFM          (0x02U << PF8X_F_SW6_RUN_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW6_RUN_MODE_AUTO         (0x03U << PF8X_F_SW6_RUN_MODE_SHIFT)

/**
 * OFF
 */
#define PF8X_F_SW6_STBY_MODE_OFF         (0x00U << PF8X_F_SW6_STBY_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW6_STBY_MODE_PWM         (0x01U << PF8X_F_SW6_STBY_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW6_STBY_MODE_PFM         (0x02U << PF8X_F_SW6_STBY_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW6_STBY_MODE_AUTO        (0x03U << PF8X_F_SW6_STBY_MODE_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_SW6_PDGRP_GROUP_4         (0x00U << PF8X_F_SW6_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_SW6_PDGRP_GROUP_3         (0x01U << PF8X_F_SW6_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_SW6_PDGRP_GROUP_2         (0x02U << PF8X_F_SW6_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_SW6_PDGRP_GROUP_1         (0x03U << PF8X_F_SW6_PDGRP_SHIFT)

/******************************************************************************/
/* SW6_RUN_VOLT - Type: RW                                                    */
/******************************************************************************/

#define PF8X_SW6_RUN_VOLT_ADDR           0x79U
#define PF8X_SW6_RUN_VOLT_DEFAULT        0x00U

/**
 * SW6 Output voltage configuration in given Mode. It uses the OTP_VSW6 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW6_RUN_MASK             0xFFU

/**
 * SW6 Output voltage configuration in given Mode. It uses the OTP_VSW6 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW6_RUN_SHIFT            0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW6_RUN_0_4_V            (0x00U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW6_RUN_0_40625_V        (0x01U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW6_RUN_0_4125_V         (0x02U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW6_RUN_0_41875_V        (0x03U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW6_RUN_0_425_V          (0x04U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW6_RUN_0_43125_V        (0x05U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW6_RUN_0_4375_V         (0x06U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW6_RUN_0_44375V         (0x07U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW6_RUN_0_45_V           (0x08U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW6_RUN_0_45625_V        (0x09U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW6_RUN_0_4625_V         (0x0AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW6_RUN_0_46875_V        (0x0BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW6_RUN_0_475_V          (0x0CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW6_RUN_0_48125_V        (0x0DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW6_RUN_0_4875_V         (0x0EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW6_RUN_0_49375_V        (0x0FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW6_RUN_0_5_V            (0x10U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW6_RUN_0_50625_V        (0x11U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW6_RUN_0_5125_V         (0x12U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW6_RUN_0_51875_V        (0x13U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW6_RUN_0_525_V          (0x14U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW6_RUN_0_53125_V        (0x15U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW6_RUN_0_5375_V         (0x16U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW6_RUN_0_54375_V        (0x17U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW6_RUN_0_55_V           (0x18U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW6_RUN_0_55625_V        (0x19U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW6_RUN_0_5625_V         (0x1AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW6_RUN_0_56875_V        (0x1BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW6_RUN_0_575_V          (0x1CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW6_RUN_0_58125_V        (0x1DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW6_RUN_0_5875_V         (0x1EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW6_RUN_0_59375_V        (0x1FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW6_RUN_0_6_V            (0x20U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW6_RUN_0_60625_V        (0x21U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW6_RUN_0_6125_V         (0x22U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW6_RUN_0_61875_V        (0x23U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW6_RUN_0_625_V          (0x24U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW6_RUN_0_63125_V        (0x25U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW6_RUN_0_6375_V         (0x26U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW6_RUN_0_64375_V        (0x27U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW6_RUN_0_65_V           (0x28U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW6_RUN_0_65625_V        (0x29U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW6_RUN_0_6625_V         (0x2AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW6_RUN_0_66875_V        (0x2BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW6_RUN_0_675_V          (0x2CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW6_RUN_0_68125_V        (0x2DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW6_RUN_0_6875_V         (0x2EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW6_RUN_0_69375_V        (0x2FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW6_RUN_0_7_V            (0x30U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW6_RUN_0_70625_V        (0x31U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW6_RUN_0_7125_V         (0x32U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW6_RUN_0_71875_V        (0x33U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW6_RUN_0_725_V          (0x34U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW6_RUN_0_73125_V        (0x35U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW6_RUN_0_7375_V         (0x36U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW6_RUN_0_74375_V        (0x37U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW6_RUN_0_75_V           (0x38U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW6_RUN_0_75625_V        (0x39U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW6_RUN_0_7625_V         (0x3AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW6_RUN_0_76875_V        (0x3BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW6_RUN_0_775_V          (0x3CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW6_RUN_0_78125_V        (0x3DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW6_RUN_0_7875_V         (0x3EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW6_RUN_0_79375_V        (0x3FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW6_RUN_0_8_V            (0x40U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW6_RUN_0_80625_V        (0x41U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW6_RUN_0_8125_V         (0x42U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW6_RUN_0_81875_V        (0x43U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW6_RUN_0_825_V          (0x44U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW6_RUN_0_83125_V        (0x45U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW6_RUN_0_8375_V         (0x46U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW6_RUN_0_84375_V        (0x47U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW6_RUN_0_85_V           (0x48U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW6_RUN_0_85625_V        (0x49U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW6_RUN_0_8625_V         (0x4AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW6_RUN_0_86875_V        (0x4BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW6_RUN_0_875_V          (0x4CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW6_RUN_0_88125_V        (0x4DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW6_RUN_0_8875_V         (0x4EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW6_RUN_0_89375_V        (0x4FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW6_RUN_0_9_V            (0x50U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW6_RUN_0_90625_V        (0x51U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW6_RUN_0_9125_V         (0x52U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW6_RUN_0_91875_V        (0x53U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW6_RUN_0_925_V          (0x54U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW6_RUN_0_93125_V        (0x55U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW6_RUN_0_9375_V         (0x56U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW6_RUN_0_94375_V        (0x57U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW6_RUN_0_95_V           (0x58U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW6_RUN_0_95625_V        (0x59U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW6_RUN_0_9625_V         (0x5AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW6_RUN_0_96875_V        (0x5BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW6_RUN_0_975_V          (0x5CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW6_RUN_0_98125_V        (0x5DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW6_RUN_0_9875_V         (0x5EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW6_RUN_0_99375_V        (0x5FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW6_RUN_1_V              (0x60U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW6_RUN_1_00625_V        (0x61U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW6_RUN_1_0125_V         (0x62U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW6_RUN_1_01875_V        (0x63U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW6_RUN_1_025_V          (0x64U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW6_RUN_1_03125_V        (0x65U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW6_RUN_1_0375_V         (0x66U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW6_RUN_1_04375_V        (0x67U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW6_RUN_1_05_V           (0x68U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW6_RUN_1_05625_V        (0x69U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW6_RUN_1_0625_V         (0x6AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW6_RUN_1_06875_V        (0x6BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW6_RUN_1_075_V          (0x6CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW6_RUN_1_08125_V        (0x6DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW6_RUN_1_0875_V         (0x6EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW6_RUN_1_09375_V        (0x6FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW6_RUN_1_1_V            (0x70U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW6_RUN_1_10625_V        (0x71U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW6_RUN_1_1125_V         (0x72U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW6_RUN_1_11875_V        (0x73U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW6_RUN_1_125_V          (0x74U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW6_RUN_1_13125_V        (0x75U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW6_RUN_1_1375_V         (0x76U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW6_RUN_1_14375_V        (0x77U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW6_RUN_1_15_V           (0x78U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW6_RUN_1_15625_V        (0x79U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW6_RUN_1_1625_V         (0x7AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW6_RUN_1_16875_V        (0x7BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW6_RUN_1_175_V          (0x7CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW6_RUN_1_18125_V        (0x7DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW6_RUN_1_1875_V         (0x7EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW6_RUN_1_19375_V        (0x7FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW6_RUN_1_2_V            (0x80U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW6_RUN_1_20625_V        (0x81U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW6_RUN_1_2125_V         (0x82U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW6_RUN_1_21875_V        (0x83U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW6_RUN_1_225_V          (0x84U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW6_RUN_1_23125_V        (0x85U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW6_RUN_1_2375_V         (0x86U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW6_RUN_1_24375_V        (0x87U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW6_RUN_1_25_V           (0x88U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW6_RUN_1_25625_V        (0x89U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW6_RUN_1_2625_V         (0x8AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW6_RUN_1_26875_V        (0x8BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW6_RUN_1_275_V          (0x8CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW6_RUN_1_28125_V        (0x8DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW6_RUN_1_2875_V         (0x8EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW6_RUN_1_29375_V        (0x8FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW6_RUN_1_3_V            (0x90U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW6_RUN_1_30625_V        (0x91U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW6_RUN_1_3125_V         (0x92U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW6_RUN_1_31875_V        (0x93U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW6_RUN_1_325_V          (0x94U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW6_RUN_1_33125_V        (0x95U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW6_RUN_1_3375_V         (0x96U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW6_RUN_1_34375_V        (0x97U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW6_RUN_1_35_V           (0x98U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW6_RUN_1_35625_V        (0x99U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW6_RUN_1_3625_V         (0x9AU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW6_RUN_1_36875_V        (0x9BU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW6_RUN_1_375_V          (0x9CU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW6_RUN_1_38125_V        (0x9DU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW6_RUN_1_3875_V         (0x9EU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW6_RUN_1_39375_V        (0x9FU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW6_RUN_1_4_V            (0xA0U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW6_RUN_1_40625_V        (0xA1U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW6_RUN_1_4125_V         (0xA2U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW6_RUN_1_41875_V        (0xA3U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW6_RUN_1_425_V          (0xA4U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW6_RUN_1_43125_V        (0xA5U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW6_RUN_1_4375_V         (0xA6U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW6_RUN_1_44375_V        (0xA7U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW6_RUN_1_45_V           (0xA8U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW6_RUN_1_45625_V        (0xA9U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW6_RUN_1_4625_V         (0xAAU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW6_RUN_1_46875_V        (0xABU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW6_RUN_1_475_V          (0xACU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW6_RUN_1_48125_V        (0xADU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW6_RUN_1_4875_V         (0xAEU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW6_RUN_1_49375_V        (0xAFU << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW6_RUN_1_5_V            (0xB0U << PF8X_F_VSW6_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW6_RUN_1_8_V            (0xB1U << PF8X_F_VSW6_RUN_SHIFT)

/******************************************************************************/
/* SW6_STBY_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_SW6_STBY_VOLT_ADDR          0x7AU
#define PF8X_SW6_STBY_VOLT_DEFAULT       0x00U

/**
 * SW6 Output voltage configuration in given Mode. It uses the OTP_VSW6 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW6_STBY_MASK            0xFFU

/**
 * SW6 Output voltage configuration in given Mode. It uses the OTP_VSW6 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW6_STBY_SHIFT           0x00U

/**
 * 0.4 V
 */
#define PF8X_F_VSW6_STBY_0_4_V           (0x00U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.40625 V
 */
#define PF8X_F_VSW6_STBY_0_40625_V       (0x01U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.4125 V
 */
#define PF8X_F_VSW6_STBY_0_4125_V        (0x02U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.41875 V
 */
#define PF8X_F_VSW6_STBY_0_41875_V       (0x03U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.425 V
 */
#define PF8X_F_VSW6_STBY_0_425_V         (0x04U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.43125 V
 */
#define PF8X_F_VSW6_STBY_0_43125_V       (0x05U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.4375 V
 */
#define PF8X_F_VSW6_STBY_0_4375_V        (0x06U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.44375 V
 */
#define PF8X_F_VSW6_STBY_0_44375V        (0x07U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.45 V
 */
#define PF8X_F_VSW6_STBY_0_45_V          (0x08U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.45625 V
 */
#define PF8X_F_VSW6_STBY_0_45625_V       (0x09U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.4625 V
 */
#define PF8X_F_VSW6_STBY_0_4625_V        (0x0AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.46875 V
 */
#define PF8X_F_VSW6_STBY_0_46875_V       (0x0BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.475 V
 */
#define PF8X_F_VSW6_STBY_0_475_V         (0x0CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.48125 V
 */
#define PF8X_F_VSW6_STBY_0_48125_V       (0x0DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.4875 V
 */
#define PF8X_F_VSW6_STBY_0_4875_V        (0x0EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.49375 V
 */
#define PF8X_F_VSW6_STBY_0_49375_V       (0x0FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.5 V
 */
#define PF8X_F_VSW6_STBY_0_5_V           (0x10U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.50625 V
 */
#define PF8X_F_VSW6_STBY_0_50625_V       (0x11U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.5125 V
 */
#define PF8X_F_VSW6_STBY_0_5125_V        (0x12U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 51875 V
 */
#define PF8X_F_VSW6_STBY_0_51875_V       (0x13U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.525 V
 */
#define PF8X_F_VSW6_STBY_0_525_V         (0x14U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.53125 V
 */
#define PF8X_F_VSW6_STBY_0_53125_V       (0x15U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.5375 V
 */
#define PF8X_F_VSW6_STBY_0_5375_V        (0x16U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.54375 V
 */
#define PF8X_F_VSW6_STBY_0_54375_V       (0x17U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.55 V
 */
#define PF8X_F_VSW6_STBY_0_55_V          (0x18U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.55625 V
 */
#define PF8X_F_VSW6_STBY_0_55625_V       (0x19U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.5625 V
 */
#define PF8X_F_VSW6_STBY_0_5625_V        (0x1AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.56875 V
 */
#define PF8X_F_VSW6_STBY_0_56875_V       (0x1BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.575 V
 */
#define PF8X_F_VSW6_STBY_0_575_V         (0x1CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.58125 V
 */
#define PF8X_F_VSW6_STBY_0_58125_V       (0x1DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.5875 V
 */
#define PF8X_F_VSW6_STBY_0_5875_V        (0x1EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.59375 V
 */
#define PF8X_F_VSW6_STBY_0_59375_V       (0x1FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.6 V
 */
#define PF8X_F_VSW6_STBY_0_6_V           (0x20U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.60625 V
 */
#define PF8X_F_VSW6_STBY_0_60625_V       (0x21U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.6125 V
 */
#define PF8X_F_VSW6_STBY_0_6125_V        (0x22U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.61875 V
 */
#define PF8X_F_VSW6_STBY_0_61875_V       (0x23U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.625 V
 */
#define PF8X_F_VSW6_STBY_0_625_V         (0x24U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.63125 V
 */
#define PF8X_F_VSW6_STBY_0_63125_V       (0x25U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.6375 V
 */
#define PF8X_F_VSW6_STBY_0_6375_V        (0x26U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.64375 V
 */
#define PF8X_F_VSW6_STBY_0_64375_V       (0x27U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.65 V
 */
#define PF8X_F_VSW6_STBY_0_65_V          (0x28U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.65625 V
 */
#define PF8X_F_VSW6_STBY_0_65625_V       (0x29U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.6625 V
 */
#define PF8X_F_VSW6_STBY_0_6625_V        (0x2AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.66875 V
 */
#define PF8X_F_VSW6_STBY_0_66875_V       (0x2BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.675 V
 */
#define PF8X_F_VSW6_STBY_0_675_V         (0x2CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.68125 V
 */
#define PF8X_F_VSW6_STBY_0_68125_V       (0x2DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.6875 V
 */
#define PF8X_F_VSW6_STBY_0_6875_V        (0x2EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.69375 V
 */
#define PF8X_F_VSW6_STBY_0_69375_V       (0x2FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.7 V
 */
#define PF8X_F_VSW6_STBY_0_7_V           (0x30U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.70625 V
 */
#define PF8X_F_VSW6_STBY_0_70625_V       (0x31U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.7125 V
 */
#define PF8X_F_VSW6_STBY_0_7125_V        (0x32U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.71875 V
 */
#define PF8X_F_VSW6_STBY_0_71875_V       (0x33U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.725 V
 */
#define PF8X_F_VSW6_STBY_0_725_V         (0x34U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.73125 V
 */
#define PF8X_F_VSW6_STBY_0_73125_V       (0x35U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.7375 V
 */
#define PF8X_F_VSW6_STBY_0_7375_V        (0x36U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.74375 V
 */
#define PF8X_F_VSW6_STBY_0_74375_V       (0x37U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.75 V
 */
#define PF8X_F_VSW6_STBY_0_75_V          (0x38U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.75625 V
 */
#define PF8X_F_VSW6_STBY_0_75625_V       (0x39U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.7625 V
 */
#define PF8X_F_VSW6_STBY_0_7625_V        (0x3AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.76875 V
 */
#define PF8X_F_VSW6_STBY_0_76875_V       (0x3BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.775 V
 */
#define PF8X_F_VSW6_STBY_0_775_V         (0x3CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.78125 V
 */
#define PF8X_F_VSW6_STBY_0_78125_V       (0x3DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.7875 V
 */
#define PF8X_F_VSW6_STBY_0_7875_V        (0x3EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.79375 V
 */
#define PF8X_F_VSW6_STBY_0_79375_V       (0x3FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.8 V
 */
#define PF8X_F_VSW6_STBY_0_8_V           (0x40U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.80625 V
 */
#define PF8X_F_VSW6_STBY_0_80625_V       (0x41U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.8125 V
 */
#define PF8X_F_VSW6_STBY_0_8125_V        (0x42U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.81875 V
 */
#define PF8X_F_VSW6_STBY_0_81875_V       (0x43U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.825 V
 */
#define PF8X_F_VSW6_STBY_0_825_V         (0x44U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.83125 V
 */
#define PF8X_F_VSW6_STBY_0_83125_V       (0x45U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.8375 V
 */
#define PF8X_F_VSW6_STBY_0_8375_V        (0x46U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.84375 V
 */
#define PF8X_F_VSW6_STBY_0_84375_V       (0x47U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.85 V
 */
#define PF8X_F_VSW6_STBY_0_85_V          (0x48U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.85625 V
 */
#define PF8X_F_VSW6_STBY_0_85625_V       (0x49U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.8625 V
 */
#define PF8X_F_VSW6_STBY_0_8625_V        (0x4AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.86875 V
 */
#define PF8X_F_VSW6_STBY_0_86875_V       (0x4BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.875 V
 */
#define PF8X_F_VSW6_STBY_0_875_V         (0x4CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.88125 V
 */
#define PF8X_F_VSW6_STBY_0_88125_V       (0x4DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.8875 V
 */
#define PF8X_F_VSW6_STBY_0_8875_V        (0x4EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.89375 V
 */
#define PF8X_F_VSW6_STBY_0_89375_V       (0x4FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.9 V
 */
#define PF8X_F_VSW6_STBY_0_9_V           (0x50U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.90625 V
 */
#define PF8X_F_VSW6_STBY_0_90625_V       (0x51U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.9125 V
 */
#define PF8X_F_VSW6_STBY_0_9125_V        (0x52U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.91875 V
 */
#define PF8X_F_VSW6_STBY_0_91875_V       (0x53U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.925 V
 */
#define PF8X_F_VSW6_STBY_0_925_V         (0x54U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.93125 V
 */
#define PF8X_F_VSW6_STBY_0_93125_V       (0x55U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.9375 V
 */
#define PF8X_F_VSW6_STBY_0_9375_V        (0x56U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.94375 V
 */
#define PF8X_F_VSW6_STBY_0_94375_V       (0x57U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.95 V
 */
#define PF8X_F_VSW6_STBY_0_95_V          (0x58U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.95625 V
 */
#define PF8X_F_VSW6_STBY_0_95625_V       (0x59U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.9625 V
 */
#define PF8X_F_VSW6_STBY_0_9625_V        (0x5AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.96875 V
 */
#define PF8X_F_VSW6_STBY_0_96875_V       (0x5BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.975 V
 */
#define PF8X_F_VSW6_STBY_0_975_V         (0x5CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.98125 V
 */
#define PF8X_F_VSW6_STBY_0_98125_V       (0x5DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.9875 V
 */
#define PF8X_F_VSW6_STBY_0_9875_V        (0x5EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 0.99375 V
 */
#define PF8X_F_VSW6_STBY_0_99375_V       (0x5FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1 V
 */
#define PF8X_F_VSW6_STBY_1_V             (0x60U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.00625 V
 */
#define PF8X_F_VSW6_STBY_1_00625_V       (0x61U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.0125 V
 */
#define PF8X_F_VSW6_STBY_1_0125_V        (0x62U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.01875 V
 */
#define PF8X_F_VSW6_STBY_1_01875_V       (0x63U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.025 V
 */
#define PF8X_F_VSW6_STBY_1_025_V         (0x64U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.03125 V
 */
#define PF8X_F_VSW6_STBY_1_03125_V       (0x65U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.0375 V
 */
#define PF8X_F_VSW6_STBY_1_0375_V        (0x66U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.04375 V
 */
#define PF8X_F_VSW6_STBY_1_04375_V       (0x67U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.05 V
 */
#define PF8X_F_VSW6_STBY_1_05_V          (0x68U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.05625 V
 */
#define PF8X_F_VSW6_STBY_1_05625_V       (0x69U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.0625 V
 */
#define PF8X_F_VSW6_STBY_1_0625_V        (0x6AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.06875 V
 */
#define PF8X_F_VSW6_STBY_1_06875_V       (0x6BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.075 V
 */
#define PF8X_F_VSW6_STBY_1_075_V         (0x6CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.08125 V
 */
#define PF8X_F_VSW6_STBY_1_08125_V       (0x6DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.0875 V
 */
#define PF8X_F_VSW6_STBY_1_0875_V        (0x6EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.09375 V
 */
#define PF8X_F_VSW6_STBY_1_09375_V       (0x6FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.1 V
 */
#define PF8X_F_VSW6_STBY_1_1_V           (0x70U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.10625 V
 */
#define PF8X_F_VSW6_STBY_1_10625_V       (0x71U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.1125 V
 */
#define PF8X_F_VSW6_STBY_1_1125_V        (0x72U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.11875 V
 */
#define PF8X_F_VSW6_STBY_1_11875_V       (0x73U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.125 V
 */
#define PF8X_F_VSW6_STBY_1_125_V         (0x74U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.13125 V
 */
#define PF8X_F_VSW6_STBY_1_13125_V       (0x75U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.1375 V
 */
#define PF8X_F_VSW6_STBY_1_1375_V        (0x76U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.14375 V
 */
#define PF8X_F_VSW6_STBY_1_14375_V       (0x77U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.15 V
 */
#define PF8X_F_VSW6_STBY_1_15_V          (0x78U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.15625 V
 */
#define PF8X_F_VSW6_STBY_1_15625_V       (0x79U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.1625 V
 */
#define PF8X_F_VSW6_STBY_1_1625_V        (0x7AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.16875 V
 */
#define PF8X_F_VSW6_STBY_1_16875_V       (0x7BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.175 V
 */
#define PF8X_F_VSW6_STBY_1_175_V         (0x7CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.18125 V
 */
#define PF8X_F_VSW6_STBY_1_18125_V       (0x7DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.1875 V
 */
#define PF8X_F_VSW6_STBY_1_1875_V        (0x7EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.19375 V
 */
#define PF8X_F_VSW6_STBY_1_19375_V       (0x7FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.2 V
 */
#define PF8X_F_VSW6_STBY_1_2_V           (0x80U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.20625 V
 */
#define PF8X_F_VSW6_STBY_1_20625_V       (0x81U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.2125 V
 */
#define PF8X_F_VSW6_STBY_1_2125_V        (0x82U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.21875 V
 */
#define PF8X_F_VSW6_STBY_1_21875_V       (0x83U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.225 V
 */
#define PF8X_F_VSW6_STBY_1_225_V         (0x84U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.23125 V
 */
#define PF8X_F_VSW6_STBY_1_23125_V       (0x85U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.2375 V
 */
#define PF8X_F_VSW6_STBY_1_2375_V        (0x86U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.24375 V
 */
#define PF8X_F_VSW6_STBY_1_24375_V       (0x87U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW6_STBY_1_25_V          (0x88U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.25625 V
 */
#define PF8X_F_VSW6_STBY_1_25625_V       (0x89U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.2625 V
 */
#define PF8X_F_VSW6_STBY_1_2625_V        (0x8AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.26875 V
 */
#define PF8X_F_VSW6_STBY_1_26875_V       (0x8BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.275 V
 */
#define PF8X_F_VSW6_STBY_1_275_V         (0x8CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.28125 V
 */
#define PF8X_F_VSW6_STBY_1_28125_V       (0x8DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.2875 V
 */
#define PF8X_F_VSW6_STBY_1_2875_V        (0x8EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.29375 V
 */
#define PF8X_F_VSW6_STBY_1_29375_V       (0x8FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.3 V
 */
#define PF8X_F_VSW6_STBY_1_3_V           (0x90U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.30625 V
 */
#define PF8X_F_VSW6_STBY_1_30625_V       (0x91U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.3125 V
 */
#define PF8X_F_VSW6_STBY_1_3125_V        (0x92U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.31875 V
 */
#define PF8X_F_VSW6_STBY_1_31875_V       (0x93U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.325 V
 */
#define PF8X_F_VSW6_STBY_1_325_V         (0x94U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.33125 V
 */
#define PF8X_F_VSW6_STBY_1_33125_V       (0x95U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.3375 V
 */
#define PF8X_F_VSW6_STBY_1_3375_V        (0x96U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.34375 V
 */
#define PF8X_F_VSW6_STBY_1_34375_V       (0x97U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW6_STBY_1_35_V          (0x98U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.35625 V
 */
#define PF8X_F_VSW6_STBY_1_35625_V       (0x99U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.3625 V
 */
#define PF8X_F_VSW6_STBY_1_3625_V        (0x9AU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.36875 V
 */
#define PF8X_F_VSW6_STBY_1_36875_V       (0x9BU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.375 V
 */
#define PF8X_F_VSW6_STBY_1_375_V         (0x9CU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.38125 V
 */
#define PF8X_F_VSW6_STBY_1_38125_V       (0x9DU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.3875 V
 */
#define PF8X_F_VSW6_STBY_1_3875_V        (0x9EU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.39375 V
 */
#define PF8X_F_VSW6_STBY_1_39375_V       (0x9FU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.4 V
 */
#define PF8X_F_VSW6_STBY_1_4_V           (0xA0U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.40625 V
 */
#define PF8X_F_VSW6_STBY_1_40625_V       (0xA1U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.4125 V
 */
#define PF8X_F_VSW6_STBY_1_4125_V        (0xA2U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.41875 V
 */
#define PF8X_F_VSW6_STBY_1_41875_V       (0xA3U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.425 V
 */
#define PF8X_F_VSW6_STBY_1_425_V         (0xA4U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.43125 V
 */
#define PF8X_F_VSW6_STBY_1_43125_V       (0xA5U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.4375 V
 */
#define PF8X_F_VSW6_STBY_1_4375_V        (0xA6U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.44375 V
 */
#define PF8X_F_VSW6_STBY_1_44375_V       (0xA7U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.45 V
 */
#define PF8X_F_VSW6_STBY_1_45_V          (0xA8U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.45625 V
 */
#define PF8X_F_VSW6_STBY_1_45625_V       (0xA9U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.4625 V
 */
#define PF8X_F_VSW6_STBY_1_4625_V        (0xAAU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.46875 V
 */
#define PF8X_F_VSW6_STBY_1_46875_V       (0xABU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.475 V
 */
#define PF8X_F_VSW6_STBY_1_475_V         (0xACU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.48125 V
 */
#define PF8X_F_VSW6_STBY_1_48125_V       (0xADU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.4875 V
 */
#define PF8X_F_VSW6_STBY_1_4875_V        (0xAEU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.49375 V
 */
#define PF8X_F_VSW6_STBY_1_49375_V       (0xAFU << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.5 V
 */
#define PF8X_F_VSW6_STBY_1_5_V           (0xB0U << PF8X_F_VSW6_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSW6_STBY_1_8_V           (0xB1U << PF8X_F_VSW6_STBY_SHIFT)

/******************************************************************************/
/* SW7_CONFIG1 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW7_CONFIG1_ADDR            0x7DU
#define PF8X_SW7_CONFIG1_DEFAULT         0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW7_PG_EN_MASK            0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW7_WDBYPASS_MASK         0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW7_ILIM_STATE_MASK       0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW7_OV_STATE_MASK         0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW7_UV_STATE_MASK         0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW7_ILIM_BYPASS_MASK      0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW7_OV_BYPASS_MASK        0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW7_UV_BYPASS_MASK        0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_SW7_PG_EN_SHIFT           0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_SW7_WDBYPASS_SHIFT        0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_SW7_ILIM_STATE_SHIFT      0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_SW7_OV_STATE_SHIFT        0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_SW7_UV_STATE_SHIFT        0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_SW7_ILIM_BYPASS_SHIFT     0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_SW7_OV_BYPASS_SHIFT       0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_SW7_UV_BYPASS_SHIFT       0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_SW7_PG_EN_DISABLED        (0x00U << PF8X_F_SW7_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_SW7_PG_EN_ENABLED         (0x01U << PF8X_F_SW7_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_SW7_WDBYPASS_RESET        (0x00U << PF8X_F_SW7_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_SW7_WDBYPASS_BYPASS       (0x01U << PF8X_F_SW7_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW7_ILIM_STATE_DISABLED   (0x00U << PF8X_F_SW7_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW7_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW7_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW7_OV_STATE_DISABLED     (0x00U << PF8X_F_SW7_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW7_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW7_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_SW7_UV_STATE_DISABLED     (0x00U << PF8X_F_SW7_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_SW7_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_SW7_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW7_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW7_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW7_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW7_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW7_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW7_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW7_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW7_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_SW7_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_SW7_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_SW7_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_SW7_UV_BYPASS_SHIFT)

/******************************************************************************/
/* SW7_CONFIG2 - Type: RW                                                     */
/******************************************************************************/

#define PF8X_SW7_CONFIG2_ADDR            0x7EU
#define PF8X_SW7_CONFIG2_DEFAULT         0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW7PHASE_MASK             0x07U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW7ILIM_MASK              0x18U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW7_FLT_REN_MASK          0x80U

/**
 * Set the Phase shift for Switching regulator
 */
#define PF8X_F_SW7PHASE_SHIFT            0x00U
/**
 * Set the current limit for Switching Regulator
 */
#define PF8X_F_SW7ILIM_SHIFT             0x03U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_SW7_FLT_REN_SHIFT         0x07U

/**
 * 45_
 */
#define PF8X_F_SW7PHASE_45               (0x00U << PF8X_F_SW7PHASE_SHIFT)
/**
 * 90_
 */
#define PF8X_F_SW7PHASE_90               (0x01U << PF8X_F_SW7PHASE_SHIFT)
/**
 * 135_
 */
#define PF8X_F_SW7PHASE_135              (0x02U << PF8X_F_SW7PHASE_SHIFT)
/**
 * 180_
 */
#define PF8X_F_SW7PHASE_180              (0x03U << PF8X_F_SW7PHASE_SHIFT)
/**
 * 225_
 */
#define PF8X_F_SW7PHASE_225              (0x04U << PF8X_F_SW7PHASE_SHIFT)
/**
 * 270_
 */
#define PF8X_F_SW7PHASE_270              (0x05U << PF8X_F_SW7PHASE_SHIFT)
/**
 * 315_
 */
#define PF8X_F_SW7PHASE_315              (0x06U << PF8X_F_SW7PHASE_SHIFT)
/**
 * 0_
 */
#define PF8X_F_SW7PHASE_0                (0x07U << PF8X_F_SW7PHASE_SHIFT)

/**
 * 2.1 A
 */
#define PF8X_F_SW7ILIM_2_1_A             (0x00U << PF8X_F_SW7ILIM_SHIFT)
/**
 * 2.6 A
 */
#define PF8X_F_SW7ILIM_2_6_A             (0x01U << PF8X_F_SW7ILIM_SHIFT)
/**
 * 3.0 A
 */
#define PF8X_F_SW7ILIM_3_0_A             (0x02U << PF8X_F_SW7ILIM_SHIFT)
/**
 * 4.5 A
 */
#define PF8X_F_SW7ILIM_4_5_A             (0x03U << PF8X_F_SW7ILIM_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_SW7_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_SW7_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_SW7_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_SW7_FLT_REN_SHIFT)

/******************************************************************************/
/* SW7_PWRUP - Type: RW                                                       */
/******************************************************************************/

#define PF8X_SW7_PWRUP_ADDR              0x7FU
#define PF8X_SW7_PWRUP_DEFAULT           0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW7_SEQ_MASK              0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_SW7_SEQ_SHIFT             0x00U

/******************************************************************************/
/* SW7_MODE - Type: RW                                                        */
/******************************************************************************/

#define PF8X_SW7_MODE_ADDR               0x80U
#define PF8X_SW7_MODE_DEFAULT            0x00U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW7_RUN_MODE_MASK         0x03U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW7_STBY_MODE_MASK        0x0CU
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW7_PDGRP_MASK            0x30U

/**
 * Mode configuration for VSWx in Run Mode when regulator is part of the Power up sequence, default value is Auto 
 * Skip, otherwise, default value is OFF.
 */
#define PF8X_F_SW7_RUN_MODE_SHIFT        0x00U
/**
 * Mode configuration for VSWx in Standby Mode when regulator is part of the Power up sequence, default value is Auto 
 * skip Mode, otherwise, default value is OFF.
 */
#define PF8X_F_SW7_STBY_MODE_SHIFT       0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_SW7_PDGRP_SHIFT           0x04U

/**
 * OFF
 */
#define PF8X_F_SW7_RUN_MODE_OFF          (0x00U << PF8X_F_SW7_RUN_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW7_RUN_MODE_PWM          (0x01U << PF8X_F_SW7_RUN_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW7_RUN_MODE_PFM          (0x02U << PF8X_F_SW7_RUN_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW7_RUN_MODE_AUTO         (0x03U << PF8X_F_SW7_RUN_MODE_SHIFT)

/**
 * OFF
 */
#define PF8X_F_SW7_STBY_MODE_OFF         (0x00U << PF8X_F_SW7_STBY_MODE_SHIFT)
/**
 * PWM
 */
#define PF8X_F_SW7_STBY_MODE_PWM         (0x01U << PF8X_F_SW7_STBY_MODE_SHIFT)
/**
 * PFM
 */
#define PF8X_F_SW7_STBY_MODE_PFM         (0x02U << PF8X_F_SW7_STBY_MODE_SHIFT)
/**
 * Auto Skip mode
 */
#define PF8X_F_SW7_STBY_MODE_AUTO        (0x03U << PF8X_F_SW7_STBY_MODE_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_SW7_PDGRP_GROUP_4         (0x00U << PF8X_F_SW7_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_SW7_PDGRP_GROUP_3         (0x01U << PF8X_F_SW7_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_SW7_PDGRP_GROUP_2         (0x02U << PF8X_F_SW7_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_SW7_PDGRP_GROUP_1         (0x03U << PF8X_F_SW7_PDGRP_SHIFT)

/******************************************************************************/
/* SW7_VOLT - Type: RW                                                        */
/******************************************************************************/

#define PF8X_SW7_VOLT_ADDR               0x81U
#define PF8X_SW7_VOLT_DEFAULT            0x00U

/**
 * SW7 Output voltage configuration in given Mode. It uses the OTP_VSW7 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW7_MASK                 0x1FU

/**
 * SW7 Output voltage configuration in given Mode. It uses the OTP_VSW7 to load the Default Voltage upon Power up
 */
#define PF8X_F_VSW7_SHIFT                0x00U

/**
 * 1.00 V
 */
#define PF8X_F_VSW7_1_00_V               (0x00U << PF8X_F_VSW7_SHIFT)
/**
 * 1.10 V
 */
#define PF8X_F_VSW7_1_10_V               (0x01U << PF8X_F_VSW7_SHIFT)
/**
 * 1.20 V
 */
#define PF8X_F_VSW7_1_20_V               (0x02U << PF8X_F_VSW7_SHIFT)
/**
 * 1.25 V
 */
#define PF8X_F_VSW7_1_25_V               (0x03U << PF8X_F_VSW7_SHIFT)
/**
 * 1.30 V
 */
#define PF8X_F_VSW7_1_30_V               (0x04U << PF8X_F_VSW7_SHIFT)
/**
 * 1.35 V
 */
#define PF8X_F_VSW7_1_35_V               (0x05U << PF8X_F_VSW7_SHIFT)
/**
 * 1.50 V
 */
#define PF8X_F_VSW7_1_50_V               (0x06U << PF8X_F_VSW7_SHIFT)
/**
 * 1.60 V
 */
#define PF8X_F_VSW7_1_60_V               (0x07U << PF8X_F_VSW7_SHIFT)
/**
 * 1.80 V
 */
#define PF8X_F_VSW7_1_80_V               (0x08U << PF8X_F_VSW7_SHIFT)
/**
 * 1.85 V
 */
#define PF8X_F_VSW7_1_85_V               (0x09U << PF8X_F_VSW7_SHIFT)
/**
 * 2.00 V
 */
#define PF8X_F_VSW7_2_00_V               (0x0AU << PF8X_F_VSW7_SHIFT)
/**
 * 2.10 V
 */
#define PF8X_F_VSW7_2_10_V               (0x0BU << PF8X_F_VSW7_SHIFT)
/**
 * 2.15 V
 */
#define PF8X_F_VSW7_2_15_V               (0x0CU << PF8X_F_VSW7_SHIFT)
/**
 * 2.25 V
 */
#define PF8X_F_VSW7_2_25_V               (0x0DU << PF8X_F_VSW7_SHIFT)
/**
 * 2.30 V
 */
#define PF8X_F_VSW7_2_30_V               (0x0EU << PF8X_F_VSW7_SHIFT)
/**
 * 2.40 V
 */
#define PF8X_F_VSW7_2_40_V               (0x0FU << PF8X_F_VSW7_SHIFT)
/**
 * 2.50 V
 */
#define PF8X_F_VSW7_2_50_V               (0x10U << PF8X_F_VSW7_SHIFT)
/**
 * 2.80 V
 */
#define PF8X_F_VSW7_2_80_V               (0x11U << PF8X_F_VSW7_SHIFT)
/**
 * 3.15 V
 */
#define PF8X_F_VSW7_3_15_V               (0x12U << PF8X_F_VSW7_SHIFT)
/**
 * 3.20 V
 */
#define PF8X_F_VSW7_3_20_V               (0x13U << PF8X_F_VSW7_SHIFT)
/**
 * 3.25 V
 */
#define PF8X_F_VSW7_3_25_V               (0x14U << PF8X_F_VSW7_SHIFT)
/**
 * 3.30 V
 */
#define PF8X_F_VSW7_3_30_V               (0x15U << PF8X_F_VSW7_SHIFT)
/**
 * 3.35 V
 */
#define PF8X_F_VSW7_3_35_V               (0x16U << PF8X_F_VSW7_SHIFT)
/**
 * 3.40 V
 */
#define PF8X_F_VSW7_3_40_V               (0x17U << PF8X_F_VSW7_SHIFT)
/**
 * 3.50 V
 */
#define PF8X_F_VSW7_3_50_V               (0x18U << PF8X_F_VSW7_SHIFT)
/**
 * 3.80 V
 */
#define PF8X_F_VSW7_3_80_V               (0x19U << PF8X_F_VSW7_SHIFT)
/**
 * 4.00 V
 */
#define PF8X_F_VSW7_4_00_V               (0x1AU << PF8X_F_VSW7_SHIFT)
/**
 * 4.10 V
 */
#define PF8X_F_VSW7_4_10_V               (0x1BU << PF8X_F_VSW7_SHIFT)

/******************************************************************************/
/* LDO1_CONFIG1 - Type: RW                                                    */
/******************************************************************************/

#define PF8X_LDO1_CONFIG1_ADDR           0x85U
#define PF8X_LDO1_CONFIG1_DEFAULT        0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_LDO1_PG_EN_MASK           0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_LDO1_WDBYPASS_MASK        0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_LDO1_ILIM_STATE_MASK      0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_LDO1_OV_STATE_MASK        0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_LDO1_UV_STATE_MASK        0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_LDO1_ILIM_BYPASS_MASK     0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_LDO1_OV_BYPASS_MASK       0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_LDO1_UV_BYPASS_MASK       0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_LDO1_PG_EN_SHIFT          0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_LDO1_WDBYPASS_SHIFT       0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_LDO1_ILIM_STATE_SHIFT     0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_LDO1_OV_STATE_SHIFT       0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_LDO1_UV_STATE_SHIFT       0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_LDO1_ILIM_BYPASS_SHIFT    0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_LDO1_OV_BYPASS_SHIFT      0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_LDO1_UV_BYPASS_SHIFT      0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_LDO1_PG_EN_DISABLED       (0x00U << PF8X_F_LDO1_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_LDO1_PG_EN_ENABLED        (0x01U << PF8X_F_LDO1_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_LDO1_WDBYPASS_RESET       (0x00U << PF8X_F_LDO1_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_LDO1_WDBYPASS_BYPASS      (0x01U << PF8X_F_LDO1_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO1_ILIM_STATE_DISABLED  (0x00U << PF8X_F_LDO1_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO1_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO1_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO1_OV_STATE_DISABLED    (0x00U << PF8X_F_LDO1_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO1_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO1_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO1_UV_STATE_DISABLED    (0x00U << PF8X_F_LDO1_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO1_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO1_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO1_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO1_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO1_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO1_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO1_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO1_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO1_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO1_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO1_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO1_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO1_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO1_UV_BYPASS_SHIFT)

/******************************************************************************/
/* LDO1_CONFIG2 - Type: RW                                                    */
/******************************************************************************/

#define PF8X_LDO1_CONFIG2_ADDR           0x86U
#define PF8X_LDO1_CONFIG2_DEFAULT        0x80U

/**
 * Enable bit for LDO during Standby Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled. 
 */
#define PF8X_F_LDO1_STBY_EN_MASK         0x01U
/**
 * Enable bit for LDO during Run Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled
 */
#define PF8X_F_LDO1_RUN_EN_MASK          0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_LDO1_PDGRP_MASK           0x60U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_LDO1_FLT_REN_MASK         0x80U

/**
 * Enable bit for LDO during Standby Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled. 
 */
#define PF8X_F_LDO1_STBY_EN_SHIFT        0x00U
/**
 * Enable bit for LDO during Run Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled
 */
#define PF8X_F_LDO1_RUN_EN_SHIFT         0x01U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_LDO1_PDGRP_SHIFT          0x05U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_LDO1_FLT_REN_SHIFT        0x07U

/**
 * Disabled
 */
#define PF8X_F_LDO1_STBY_EN_DISABLED     (0x00U << PF8X_F_LDO1_STBY_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_LDO1_STBY_EN_ENABLED      (0x01U << PF8X_F_LDO1_STBY_EN_SHIFT)

/**
 * Disabled
 */
#define PF8X_F_LDO1_RUN_EN_DISABLED      (0x00U << PF8X_F_LDO1_RUN_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_LDO1_RUN_EN_ENABLED       (0x01U << PF8X_F_LDO1_RUN_EN_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_LDO1_PDGRP_GROUP_4        (0x00U << PF8X_F_LDO1_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_LDO1_PDGRP_GROUP_3        (0x01U << PF8X_F_LDO1_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_LDO1_PDGRP_GROUP_2        (0x02U << PF8X_F_LDO1_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_LDO1_PDGRP_GROUP_1        (0x03U << PF8X_F_LDO1_PDGRP_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_LDO1_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_LDO1_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_LDO1_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_LDO1_FLT_REN_SHIFT)

/******************************************************************************/
/* LDO1_PWRUP - Type: RW                                                      */
/******************************************************************************/

#define PF8X_LDO1_PWRUP_ADDR             0x87U
#define PF8X_LDO1_PWRUP_DEFAULT          0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_LDO1_SEQ_MASK             0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_LDO1_SEQ_SHIFT            0x00U

/******************************************************************************/
/* LDO1_RUN_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_LDO1_RUN_VOLT_ADDR          0x88U
#define PF8X_LDO1_RUN_VOLT_DEFAULT       0x00U

/**
 * LDO1 Output voltage configuration in Run Mode. It uses the OTP_VLDO1 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO1_RUN_MASK            0x0FU

/**
 * LDO1 Output voltage configuration in Run Mode. It uses the OTP_VLDO1 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO1_RUN_SHIFT           0x00U

/**
 * 1.5 V
 */
#define PF8X_F_VLDO1_RUN_1_5_V           (0x00U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 1.6 V
 */
#define PF8X_F_VLDO1_RUN_1_6_V           (0x01U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VLDO1_RUN_1_8_V           (0x02U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 1.85 V
 */
#define PF8X_F_VLDO1_RUN_1_85_V          (0x03U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 2.15 V
 */
#define PF8X_F_VLDO1_RUN_2_15_V          (0x04U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 2.5 V
 */
#define PF8X_F_VLDO1_RUN_2_5_V           (0x05U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 2.8 V
 */
#define PF8X_F_VLDO1_RUN_2_8_V           (0x06U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VLDO1_RUN_3_0_V           (0x07U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 3.1 V
 */
#define PF8X_F_VLDO1_RUN_3_1_V           (0x08U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 3.15 V
 */
#define PF8X_F_VLDO1_RUN_3_15_V          (0x09U << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 3.2 V
 */
#define PF8X_F_VLDO1_RUN_3_2_V           (0x0AU << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VLDO1_RUN_3_3_V           (0x0BU << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 3.35 V
 */
#define PF8X_F_VLDO1_RUN_3_35_V          (0x0CU << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 1.65 V
 */
#define PF8X_F_VLDO1_RUN_1_65_V          (0x0DU << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 1.7 V
 */
#define PF8X_F_VLDO1_RUN_1_7_V           (0x0EU << PF8X_F_VLDO1_RUN_SHIFT)
/**
 * 5.0 V
 */
#define PF8X_F_VLDO1_RUN_5_0_V           (0x0FU << PF8X_F_VLDO1_RUN_SHIFT)

/******************************************************************************/
/* LDO1_STBY_VOLT - Type: RW                                                  */
/******************************************************************************/

#define PF8X_LDO1_STBY_VOLT_ADDR         0x89U
#define PF8X_LDO1_STBY_VOLT_DEFAULT      0x00U

/**
 * LDO1 Output voltage configuration in Standby Mode. It uses the OTP_VLDO1 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO1_STBY_MASK           0x0FU

/**
 * LDO1 Output voltage configuration in Standby Mode. It uses the OTP_VLDO1 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO1_STBY_SHIFT          0x00U

/**
 * 1.5 V
 */
#define PF8X_F_VLDO1_STBY_1_5_V          (0x00U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 1.6 V
 */
#define PF8X_F_VLDO1_STBY_1_6_V          (0x01U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VLDO1_STBY_1_8_V          (0x02U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 1.85 V
 */
#define PF8X_F_VLDO1_STBY_1_85_V         (0x03U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 2.15 V
 */
#define PF8X_F_VLDO1_STBY_2_15_V         (0x04U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 2.5 V
 */
#define PF8X_F_VLDO1_STBY_2_5_V          (0x05U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 2.8 V
 */
#define PF8X_F_VLDO1_STBY_2_8_V          (0x06U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VLDO1_STBY_3_0_V          (0x07U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 3.1 V
 */
#define PF8X_F_VLDO1_STBY_3_1_V          (0x08U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 3.15 V
 */
#define PF8X_F_VLDO1_STBY_3_15_V         (0x09U << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 3.2 V
 */
#define PF8X_F_VLDO1_STBY_3_2_V          (0x0AU << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VLDO1_STBY_3_3_V          (0x0BU << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 3.35 V
 */
#define PF8X_F_VLDO1_STBY_3_35_V         (0x0CU << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 1.65 V
 */
#define PF8X_F_VLDO1_STBY_1_65_V         (0x0DU << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 1.7 V
 */
#define PF8X_F_VLDO1_STBY_1_7_V          (0x0EU << PF8X_F_VLDO1_STBY_SHIFT)
/**
 * 5.0 V
 */
#define PF8X_F_VLDO1_STBY_5_0_V          (0x0FU << PF8X_F_VLDO1_STBY_SHIFT)

/******************************************************************************/
/* LDO2_CONFIG1 - Type: RW                                                    */
/******************************************************************************/

#define PF8X_LDO2_CONFIG1_ADDR           0x8BU
#define PF8X_LDO2_CONFIG1_DEFAULT        0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_LDO2_PG_EN_MASK           0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_LDO2_WDBYPASS_MASK        0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_LDO2_ILIM_STATE_MASK      0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_LDO2_OV_STATE_MASK        0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_LDO2_UV_STATE_MASK        0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_LDO2_ILIM_BYPASS_MASK     0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_LDO2_OV_BYPASS_MASK       0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_LDO2_UV_BYPASS_MASK       0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_LDO2_PG_EN_SHIFT          0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_LDO2_WDBYPASS_SHIFT       0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_LDO2_ILIM_STATE_SHIFT     0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_LDO2_OV_STATE_SHIFT       0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_LDO2_UV_STATE_SHIFT       0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_LDO2_ILIM_BYPASS_SHIFT    0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_LDO2_OV_BYPASS_SHIFT      0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_LDO2_UV_BYPASS_SHIFT      0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_LDO2_PG_EN_DISABLED       (0x00U << PF8X_F_LDO2_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_LDO2_PG_EN_ENABLED        (0x01U << PF8X_F_LDO2_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_LDO2_WDBYPASS_RESET       (0x00U << PF8X_F_LDO2_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_LDO2_WDBYPASS_BYPASS      (0x01U << PF8X_F_LDO2_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO2_ILIM_STATE_DISABLED  (0x00U << PF8X_F_LDO2_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO2_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO2_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO2_OV_STATE_DISABLED    (0x00U << PF8X_F_LDO2_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO2_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO2_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO2_UV_STATE_DISABLED    (0x00U << PF8X_F_LDO2_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO2_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO2_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO2_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO2_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO2_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO2_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO2_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO2_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO2_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO2_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO2_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO2_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO2_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO2_UV_BYPASS_SHIFT)

/******************************************************************************/
/* LDO2_CONFIG2 - Type: RW                                                    */
/******************************************************************************/

#define PF8X_LDO2_CONFIG2_ADDR           0x8CU
#define PF8X_LDO2_CONFIG2_DEFAULT        0x80U

/**
 * Enable bit for LDO during Standby Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled. 
 */
#define PF8X_F_LDO2_STBY_EN_MASK         0x01U
/**
 * Enable bit for LDO during Run Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled
 */
#define PF8X_F_LDO2_RUN_EN_MASK          0x02U
/**
 * Programs the voltage setting for LDO2
 */
#define PF8X_F_VSELECT_EN_MASK           0x08U
/**
 * Enables hardware control for LDO2 output
 */
#define PF8X_F_LDO2HW_EN_MASK            0x10U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_LDO2_PDGRP_MASK           0x60U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_LDO2_FLT_REN_MASK         0x80U

/**
 * Enable bit for LDO during Standby Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled. 
 */
#define PF8X_F_LDO2_STBY_EN_SHIFT        0x00U
/**
 * Enable bit for LDO during Run Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled
 */
#define PF8X_F_LDO2_RUN_EN_SHIFT         0x01U
/**
 * Programs the voltage setting for LDO2
 */
#define PF8X_F_VSELECT_EN_SHIFT          0x03U
/**
 * Enables hardware control for LDO2 output
 */
#define PF8X_F_LDO2HW_EN_SHIFT           0x04U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_LDO2_PDGRP_SHIFT          0x05U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_LDO2_FLT_REN_SHIFT        0x07U

/**
 * Disabled
 */
#define PF8X_F_LDO2_STBY_EN_DISABLED     (0x00U << PF8X_F_LDO2_STBY_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_LDO2_STBY_EN_ENABLED      (0x01U << PF8X_F_LDO2_STBY_EN_SHIFT)

/**
 * Disabled
 */
#define PF8X_F_LDO2_RUN_EN_DISABLED      (0x00U << PF8X_F_LDO2_RUN_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_LDO2_RUN_EN_ENABLED       (0x01U << PF8X_F_LDO2_RUN_EN_SHIFT)

/**
 * LDO2 voltage is set with the VLDO2_RUN[4:0] bits
 */
#define PF8X_F_VSELECT_EN_VLDO2_RUN      (0x00U << PF8X_F_VSELECT_EN_SHIFT)
/**
 * LDO2 voltage is set with the VSELECT pin (Low = 3.3V, high = 1.8V)
 */
#define PF8X_F_VSELECT_EN_VSELECT        (0x01U << PF8X_F_VSELECT_EN_SHIFT)

/**
 * LDO2 is enabled or disabled only by software
 */
#define PF8X_F_LDO2HW_EN_SW              (0x00U << PF8X_F_LDO2HW_EN_SHIFT)
/**
 * LDO2 is enabled or disable by a combiantion of LDO2EN pin and software bit
 */
#define PF8X_F_LDO2HW_EN_SW_LDO2EN       (0x01U << PF8X_F_LDO2HW_EN_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_LDO2_PDGRP_GROUP_4        (0x00U << PF8X_F_LDO2_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_LDO2_PDGRP_GROUP_3        (0x01U << PF8X_F_LDO2_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_LDO2_PDGRP_GROUP_2        (0x02U << PF8X_F_LDO2_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_LDO2_PDGRP_GROUP_1        (0x03U << PF8X_F_LDO2_PDGRP_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_LDO2_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_LDO2_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_LDO2_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_LDO2_FLT_REN_SHIFT)

/******************************************************************************/
/* LDO2_PWRUP - Type: RW                                                      */
/******************************************************************************/

#define PF8X_LDO2_PWRUP_ADDR             0x8DU
#define PF8X_LDO2_PWRUP_DEFAULT          0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_LDO2_SEQ_MASK             0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_LDO2_SEQ_SHIFT            0x00U

/******************************************************************************/
/* LDO2_RUN_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_LDO2_RUN_VOLT_ADDR          0x8EU
#define PF8X_LDO2_RUN_VOLT_DEFAULT       0x00U

/**
 * LDO2 Output voltage configuration in Run Mode. It uses the OTP_VLDO2 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO2_RUN_MASK            0x0FU

/**
 * LDO2 Output voltage configuration in Run Mode. It uses the OTP_VLDO2 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO2_RUN_SHIFT           0x00U

/**
 * 1.5 V
 */
#define PF8X_F_VLDO2_RUN_1_5_V           (0x00U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 1.6 V
 */
#define PF8X_F_VLDO2_RUN_1_6_V           (0x01U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VLDO2_RUN_1_8_V           (0x02U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 1.85 V
 */
#define PF8X_F_VLDO2_RUN_1_85_V          (0x03U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 2.15 V
 */
#define PF8X_F_VLDO2_RUN_2_15_V          (0x04U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 2.5 V
 */
#define PF8X_F_VLDO2_RUN_2_5_V           (0x05U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 2.8 V
 */
#define PF8X_F_VLDO2_RUN_2_8_V           (0x06U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VLDO2_RUN_3_0_V           (0x07U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 3.1 V
 */
#define PF8X_F_VLDO2_RUN_3_1_V           (0x08U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 3.15 V
 */
#define PF8X_F_VLDO2_RUN_3_15_V          (0x09U << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 3.2 V
 */
#define PF8X_F_VLDO2_RUN_3_2_V           (0x0AU << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VLDO2_RUN_3_3_V           (0x0BU << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 3.35 V
 */
#define PF8X_F_VLDO2_RUN_3_35_V          (0x0CU << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 1.65 V
 */
#define PF8X_F_VLDO2_RUN_1_65_V          (0x0DU << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 1.7 V
 */
#define PF8X_F_VLDO2_RUN_1_7_V           (0x0EU << PF8X_F_VLDO2_RUN_SHIFT)
/**
 * 5.0 V
 */
#define PF8X_F_VLDO2_RUN_5_0_V           (0x0FU << PF8X_F_VLDO2_RUN_SHIFT)

/******************************************************************************/
/* LDO2_STBY_VOLT - Type: RW                                                  */
/******************************************************************************/

#define PF8X_LDO2_STBY_VOLT_ADDR         0x8FU
#define PF8X_LDO2_STBY_VOLT_DEFAULT      0x00U

/**
 * LDO2 Output voltage configuration in Standby Mode. It uses the OTP_VLDO2 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO2_STBY_MASK           0x0FU

/**
 * LDO2 Output voltage configuration in Standby Mode. It uses the OTP_VLDO2 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO2_STBY_SHIFT          0x00U

/**
 * 1.5 V
 */
#define PF8X_F_VLDO2_STBY_1_5_V          (0x00U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 1.6 V
 */
#define PF8X_F_VLDO2_STBY_1_6_V          (0x01U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VLDO2_STBY_1_8_V          (0x02U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 1.85 V
 */
#define PF8X_F_VLDO2_STBY_1_85_V         (0x03U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 2.15 V
 */
#define PF8X_F_VLDO2_STBY_2_15_V         (0x04U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 2.5 V
 */
#define PF8X_F_VLDO2_STBY_2_5_V          (0x05U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 2.8 V
 */
#define PF8X_F_VLDO2_STBY_2_8_V          (0x06U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VLDO2_STBY_3_0_V          (0x07U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 3.1 V
 */
#define PF8X_F_VLDO2_STBY_3_1_V          (0x08U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 3.15 V
 */
#define PF8X_F_VLDO2_STBY_3_15_V         (0x09U << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 3.2 V
 */
#define PF8X_F_VLDO2_STBY_3_2_V          (0x0AU << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VLDO2_STBY_3_3_V          (0x0BU << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 3.35 V
 */
#define PF8X_F_VLDO2_STBY_3_35_V         (0x0CU << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 1.65 V
 */
#define PF8X_F_VLDO2_STBY_1_65_V         (0x0DU << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 1.7 V
 */
#define PF8X_F_VLDO2_STBY_1_7_V          (0x0EU << PF8X_F_VLDO2_STBY_SHIFT)
/**
 * 5.0 V
 */
#define PF8X_F_VLDO2_STBY_5_0_V          (0x0FU << PF8X_F_VLDO2_STBY_SHIFT)

/******************************************************************************/
/* LDO3_CONFIG1 - Type: RW                                                    */
/******************************************************************************/

#define PF8X_LDO3_CONFIG1_ADDR           0x91U
#define PF8X_LDO3_CONFIG1_DEFAULT        0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_LDO3_PG_EN_MASK           0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_LDO3_WDBYPASS_MASK        0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_LDO3_ILIM_STATE_MASK      0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_LDO3_OV_STATE_MASK        0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_LDO3_UV_STATE_MASK        0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_LDO3_ILIM_BYPASS_MASK     0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_LDO3_OV_BYPASS_MASK       0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_LDO3_UV_BYPASS_MASK       0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_LDO3_PG_EN_SHIFT          0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_LDO3_WDBYPASS_SHIFT       0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_LDO3_ILIM_STATE_SHIFT     0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_LDO3_OV_STATE_SHIFT       0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_LDO3_UV_STATE_SHIFT       0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_LDO3_ILIM_BYPASS_SHIFT    0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_LDO3_OV_BYPASS_SHIFT      0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_LDO3_UV_BYPASS_SHIFT      0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_LDO3_PG_EN_DISABLED       (0x00U << PF8X_F_LDO3_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_LDO3_PG_EN_ENABLED        (0x01U << PF8X_F_LDO3_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_LDO3_WDBYPASS_RESET       (0x00U << PF8X_F_LDO3_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_LDO3_WDBYPASS_BYPASS      (0x01U << PF8X_F_LDO3_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO3_ILIM_STATE_DISABLED  (0x00U << PF8X_F_LDO3_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO3_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO3_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO3_OV_STATE_DISABLED    (0x00U << PF8X_F_LDO3_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO3_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO3_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO3_UV_STATE_DISABLED    (0x00U << PF8X_F_LDO3_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO3_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO3_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO3_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO3_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO3_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO3_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO3_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO3_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO3_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO3_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO3_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO3_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO3_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO3_UV_BYPASS_SHIFT)

/******************************************************************************/
/* LDO3_CONFIG2 - Type: RW                                                    */
/******************************************************************************/

#define PF8X_LDO3_CONFIG2_ADDR           0x92U
#define PF8X_LDO3_CONFIG2_DEFAULT        0x80U

/**
 * Enable bit for LDO during Standby Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled. 
 */
#define PF8X_F_LDO3_STBY_EN_MASK         0x01U
/**
 * Enable bit for LDO during Run Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled
 */
#define PF8X_F_LDO3_RUN_EN_MASK          0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_LDO3_PDGRP_MASK           0x60U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_LDO3_FLT_REN_MASK         0x80U

/**
 * Enable bit for LDO during Standby Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled. 
 */
#define PF8X_F_LDO3_STBY_EN_SHIFT        0x00U
/**
 * Enable bit for LDO during Run Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled
 */
#define PF8X_F_LDO3_RUN_EN_SHIFT         0x01U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_LDO3_PDGRP_SHIFT          0x05U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_LDO3_FLT_REN_SHIFT        0x07U

/**
 * Disabled
 */
#define PF8X_F_LDO3_STBY_EN_DISABLED     (0x00U << PF8X_F_LDO3_STBY_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_LDO3_STBY_EN_ENABLED      (0x01U << PF8X_F_LDO3_STBY_EN_SHIFT)

/**
 * Disabled
 */
#define PF8X_F_LDO3_RUN_EN_DISABLED      (0x00U << PF8X_F_LDO3_RUN_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_LDO3_RUN_EN_ENABLED       (0x01U << PF8X_F_LDO3_RUN_EN_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_LDO3_PDGRP_GROUP_4        (0x00U << PF8X_F_LDO3_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_LDO3_PDGRP_GROUP_3        (0x01U << PF8X_F_LDO3_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_LDO3_PDGRP_GROUP_2        (0x02U << PF8X_F_LDO3_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_LDO3_PDGRP_GROUP_1        (0x03U << PF8X_F_LDO3_PDGRP_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_LDO3_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_LDO3_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_LDO3_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_LDO3_FLT_REN_SHIFT)

/******************************************************************************/
/* LDO3_PWRUP - Type: RW                                                      */
/******************************************************************************/

#define PF8X_LDO3_PWRUP_ADDR             0x93U
#define PF8X_LDO3_PWRUP_DEFAULT          0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_LDO3_SEQ_MASK             0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_LDO3_SEQ_SHIFT            0x00U

/******************************************************************************/
/* LDO3_RUN_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_LDO3_RUN_VOLT_ADDR          0x94U
#define PF8X_LDO3_RUN_VOLT_DEFAULT       0x00U

/**
 * LDO3 Output voltage configuration in Run Mode. It uses the OTP_VLDO3 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO3_RUN_MASK            0x0FU

/**
 * LDO3 Output voltage configuration in Run Mode. It uses the OTP_VLDO3 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO3_RUN_SHIFT           0x00U

/**
 * 1.5 V
 */
#define PF8X_F_VLDO3_RUN_1_5_V           (0x00U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 1.6 V
 */
#define PF8X_F_VLDO3_RUN_1_6_V           (0x01U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VLDO3_RUN_1_8_V           (0x02U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 1.85 V
 */
#define PF8X_F_VLDO3_RUN_1_85_V          (0x03U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 2.15 V
 */
#define PF8X_F_VLDO3_RUN_2_15_V          (0x04U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 2.5 V
 */
#define PF8X_F_VLDO3_RUN_2_5_V           (0x05U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 2.8 V
 */
#define PF8X_F_VLDO3_RUN_2_8_V           (0x06U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VLDO3_RUN_3_0_V           (0x07U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 3.1 V
 */
#define PF8X_F_VLDO3_RUN_3_1_V           (0x08U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 3.15 V
 */
#define PF8X_F_VLDO3_RUN_3_15_V          (0x09U << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 3.2 V
 */
#define PF8X_F_VLDO3_RUN_3_2_V           (0x0AU << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VLDO3_RUN_3_3_V           (0x0BU << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 3.35 V
 */
#define PF8X_F_VLDO3_RUN_3_35_V          (0x0CU << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 1.65 V
 */
#define PF8X_F_VLDO3_RUN_1_65_V          (0x0DU << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 1.7 V
 */
#define PF8X_F_VLDO3_RUN_1_7_V           (0x0EU << PF8X_F_VLDO3_RUN_SHIFT)
/**
 * 5.0 V
 */
#define PF8X_F_VLDO3_RUN_5_0_V           (0x0FU << PF8X_F_VLDO3_RUN_SHIFT)

/******************************************************************************/
/* LDO3_STBY_VOLT - Type: RW                                                  */
/******************************************************************************/

#define PF8X_LDO3_STBY_VOLT_ADDR         0x95U
#define PF8X_LDO3_STBY_VOLT_DEFAULT      0x00U

/**
 * LDO3 Output voltage configuration in Standby Mode. It uses the OTP_VLDO3 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO3_STBY_MASK           0x0FU

/**
 * LDO3 Output voltage configuration in Standby Mode. It uses the OTP_VLDO3 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO3_STBY_SHIFT          0x00U

/**
 * 1.5 V
 */
#define PF8X_F_VLDO3_STBY_1_5_V          (0x00U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 1.6 V
 */
#define PF8X_F_VLDO3_STBY_1_6_V          (0x01U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VLDO3_STBY_1_8_V          (0x02U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 1.85 V
 */
#define PF8X_F_VLDO3_STBY_1_85_V         (0x03U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 2.15 V
 */
#define PF8X_F_VLDO3_STBY_2_15_V         (0x04U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 2.5 V
 */
#define PF8X_F_VLDO3_STBY_2_5_V          (0x05U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 2.8 V
 */
#define PF8X_F_VLDO3_STBY_2_8_V          (0x06U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VLDO3_STBY_3_0_V          (0x07U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 3.1 V
 */
#define PF8X_F_VLDO3_STBY_3_1_V          (0x08U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 3.15 V
 */
#define PF8X_F_VLDO3_STBY_3_15_V         (0x09U << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 3.2 V
 */
#define PF8X_F_VLDO3_STBY_3_2_V          (0x0AU << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VLDO3_STBY_3_3_V          (0x0BU << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 3.35 V
 */
#define PF8X_F_VLDO3_STBY_3_35_V         (0x0CU << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 1.65 V
 */
#define PF8X_F_VLDO3_STBY_1_65_V         (0x0DU << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 1.7 V
 */
#define PF8X_F_VLDO3_STBY_1_7_V          (0x0EU << PF8X_F_VLDO3_STBY_SHIFT)
/**
 * 5.0 V
 */
#define PF8X_F_VLDO3_STBY_5_0_V          (0x0FU << PF8X_F_VLDO3_STBY_SHIFT)

/******************************************************************************/
/* LDO4_CONFIG1 - Type: RW                                                    */
/******************************************************************************/

#define PF8X_LDO4_CONFIG1_ADDR           0x97U
#define PF8X_LDO4_CONFIG1_DEFAULT        0x1CU

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_LDO4_PG_EN_MASK           0x01U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_LDO4_WDBYPASS_MASK        0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_LDO4_ILIM_STATE_MASK      0x04U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_LDO4_OV_STATE_MASK        0x08U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_LDO4_UV_STATE_MASK        0x10U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_LDO4_ILIM_BYPASS_MASK     0x20U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_LDO4_OV_BYPASS_MASK       0x40U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_LDO4_UV_BYPASS_MASK       0x80U

/**
 * Allows the regulator to be part of the PGOOD pin behavior.
 */
#define PF8X_F_LDO4_PG_EN_SHIFT          0x00U
/**
 * Enable Watchdog bypass for regulator
 */
#define PF8X_F_LDO4_WDBYPASS_SHIFT       0x01U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an ILIM Fault event. For LDOS 
 * this bit correspond to the LDOxOCPEN in IP Specification.
 */
#define PF8X_F_LDO4_ILIM_STATE_SHIFT     0x02U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an OV Fault event
 */
#define PF8X_F_LDO4_OV_STATE_SHIFT       0x03U
/**
 * Decides wherther the regulators remains enable or is disabled when it experiences an UV Fault event
 */
#define PF8X_F_LDO4_UV_STATE_SHIFT       0x04U
/**
 * Bypass the ILIM Fault event
 */
#define PF8X_F_LDO4_ILIM_BYPASS_SHIFT    0x05U
/**
 * Bypass the OV Fault event
 */
#define PF8X_F_LDO4_OV_BYPASS_SHIFT      0x06U
/**
 * Bypass the UV Fault event
 */
#define PF8X_F_LDO4_UV_BYPASS_SHIFT      0x07U

/**
 * Regulator is not able to set or clear the PGOOD pin
 */
#define PF8X_F_LDO4_PG_EN_DISABLED       (0x00U << PF8X_F_LDO4_PG_EN_SHIFT)
/**
 * Regulator's PG bit is part of the AND function to control PGOOD pin
 */
#define PF8X_F_LDO4_PG_EN_ENABLED        (0x01U << PF8X_F_LDO4_PG_EN_SHIFT)

/**
 * Regulator reacts upon a Watchdog
 */
#define PF8X_F_LDO4_WDBYPASS_RESET       (0x00U << PF8X_F_LDO4_WDBYPASS_SHIFT)
/**
 * Regulator bypass Watchdog event and keeps current configuration
 */
#define PF8X_F_LDO4_WDBYPASS_BYPASS      (0x01U << PF8X_F_LDO4_WDBYPASS_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO4_ILIM_STATE_DISABLED  (0x00U << PF8X_F_LDO4_ILIM_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO4_ILIM_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO4_ILIM_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO4_OV_STATE_DISABLED    (0x00U << PF8X_F_LDO4_OV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO4_OV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO4_OV_STATE_SHIFT)

/**
 * Regulator is disabled
 */
#define PF8X_F_LDO4_UV_STATE_DISABLED    (0x00U << PF8X_F_LDO4_UV_STATE_SHIFT)
/**
 * Regulator remains in previous state
 */
#define PF8X_F_LDO4_UV_STATE_PREVIOUS_STATE (0x01U << PF8X_F_LDO4_UV_STATE_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO4_ILIM_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO4_ILIM_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO4_ILIM_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO4_ILIM_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO4_OV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO4_OV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO4_OV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO4_OV_BYPASS_SHIFT)

/**
 * A fault event will trigger a protective behavior ( start the Fault timer, Fault counter and disable the regulator 
 * if programmed to do so)
 */
#define PF8X_F_LDO4_UV_BYPASS_FAULT_EVENT (0x00U << PF8X_F_LDO4_UV_BYPASS_SHIFT)
/**
 * The fault is bypassed, The regulator does not follow any corrective behavior (interrupt is still set if not masked)
 */
#define PF8X_F_LDO4_UV_BYPASS_FAULT_BYPASSED (0x01U << PF8X_F_LDO4_UV_BYPASS_SHIFT)

/******************************************************************************/
/* LDO4_CONFIG2 - Type: RW                                                    */
/******************************************************************************/

#define PF8X_LDO4_CONFIG2_ADDR           0x98U
#define PF8X_LDO4_CONFIG2_DEFAULT        0x80U

/**
 * Enable bit for LDO during Standby Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled. 
 */
#define PF8X_F_LDO4_STBY_EN_MASK         0x01U
/**
 * Enable bit for LDO during Run Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled
 */
#define PF8X_F_LDO4_RUN_EN_MASK          0x02U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_LDO4_PDGRP_MASK           0x60U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_LDO4_FLT_REN_MASK         0x80U

/**
 * Enable bit for LDO during Standby Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled. 
 */
#define PF8X_F_LDO4_STBY_EN_SHIFT        0x00U
/**
 * Enable bit for LDO during Run Mode when regulator is part of the power up sequence, default value is Enabled, 
 * otherwise, default value is disabled
 */
#define PF8X_F_LDO4_RUN_EN_SHIFT         0x01U
/**
 * Assign a Power Down group to the corresponding register
 */
#define PF8X_F_LDO4_PDGRP_SHIFT          0x05U
/**
 * If a regulator is disabled upon a fault, this bit allow the regulator to return to its previous Enabled 
 * configuration when the fault condition is cleared
 */
#define PF8X_F_LDO4_FLT_REN_SHIFT        0x07U

/**
 * Disabled
 */
#define PF8X_F_LDO4_STBY_EN_DISABLED     (0x00U << PF8X_F_LDO4_STBY_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_LDO4_STBY_EN_ENABLED      (0x01U << PF8X_F_LDO4_STBY_EN_SHIFT)

/**
 * Disabled
 */
#define PF8X_F_LDO4_RUN_EN_DISABLED      (0x00U << PF8X_F_LDO4_RUN_EN_SHIFT)
/**
 * Enabled
 */
#define PF8X_F_LDO4_RUN_EN_ENABLED       (0x01U << PF8X_F_LDO4_RUN_EN_SHIFT)

/**
 * Regulator is in Group 4 (turn of first)
 */
#define PF8X_F_LDO4_PDGRP_GROUP_4        (0x00U << PF8X_F_LDO4_PDGRP_SHIFT)
/**
 * Regulator is in Group 3
 */
#define PF8X_F_LDO4_PDGRP_GROUP_3        (0x01U << PF8X_F_LDO4_PDGRP_SHIFT)
/**
 * Regulator is in Group 2
 */
#define PF8X_F_LDO4_PDGRP_GROUP_2        (0x02U << PF8X_F_LDO4_PDGRP_SHIFT)
/**
 * Regulator is in Group 1 (turn off Last)
 */
#define PF8X_F_LDO4_PDGRP_GROUP_1        (0x03U << PF8X_F_LDO4_PDGRP_SHIFT)

/**
 * Regulator remains disabled upon clrearing the fault
 */
#define PF8X_F_LDO4_FLT_REN_REMAINS_DISABLED (0x00U << PF8X_F_LDO4_FLT_REN_SHIFT)
/**
 * Regulator returns to its previous configuration
 */
#define PF8X_F_LDO4_FLT_REN_PREVIOUS_CONFIGURATION (0x01U << PF8X_F_LDO4_FLT_REN_SHIFT)

/******************************************************************************/
/* LDO4_PWRUP - Type: RW                                                      */
/******************************************************************************/

#define PF8X_LDO4_PWRUP_ADDR             0x99U
#define PF8X_LDO4_PWRUP_DEFAULT          0x00U

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_LDO4_SEQ_MASK             0xFFU

/**
 * OTP bit sets the default sequence of the voltage regulators, Functional bits can be changed to set the power down 
 * sequence. Power Off Reset values to default OTP value.
 */
#define PF8X_F_LDO4_SEQ_SHIFT            0x00U

/******************************************************************************/
/* LDO4_RUN_VOLT - Type: RW                                                   */
/******************************************************************************/

#define PF8X_LDO4_RUN_VOLT_ADDR          0x9AU
#define PF8X_LDO4_RUN_VOLT_DEFAULT       0x00U

/**
 * LDO4 Output voltage configuration in Run Mode. It uses the OTP_VLDO4 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO4_RUN_MASK            0x0FU

/**
 * LDO4 Output voltage configuration in Run Mode. It uses the OTP_VLDO4 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO4_RUN_SHIFT           0x00U

/**
 * 1.5 V
 */
#define PF8X_F_VLDO4_RUN_1_5_V           (0x00U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 1.6 V
 */
#define PF8X_F_VLDO4_RUN_1_6_V           (0x01U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VLDO4_RUN_1_8_V           (0x02U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 1.85 V
 */
#define PF8X_F_VLDO4_RUN_1_85_V          (0x03U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 2.15 V
 */
#define PF8X_F_VLDO4_RUN_2_15_V          (0x04U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 2.5 V
 */
#define PF8X_F_VLDO4_RUN_2_5_V           (0x05U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 2.8 V
 */
#define PF8X_F_VLDO4_RUN_2_8_V           (0x06U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VLDO4_RUN_3_0_V           (0x07U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 3.1 V
 */
#define PF8X_F_VLDO4_RUN_3_1_V           (0x08U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 3.15 V
 */
#define PF8X_F_VLDO4_RUN_3_15_V          (0x09U << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 3.2 V
 */
#define PF8X_F_VLDO4_RUN_3_2_V           (0x0AU << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VLDO4_RUN_3_3_V           (0x0BU << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 3.35 V
 */
#define PF8X_F_VLDO4_RUN_3_35_V          (0x0CU << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 1.65 V
 */
#define PF8X_F_VLDO4_RUN_1_65_V          (0x0DU << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 1.7 V
 */
#define PF8X_F_VLDO4_RUN_1_7_V           (0x0EU << PF8X_F_VLDO4_RUN_SHIFT)
/**
 * 5.0 V
 */
#define PF8X_F_VLDO4_RUN_5_0_V           (0x0FU << PF8X_F_VLDO4_RUN_SHIFT)

/******************************************************************************/
/* LDO4_STBY_VOLT - Type: RW                                                  */
/******************************************************************************/

#define PF8X_LDO4_STBY_VOLT_ADDR         0x9BU
#define PF8X_LDO4_STBY_VOLT_DEFAULT      0x00U

/**
 * LDO4 Output voltage configuration in Standby Mode. It uses the OTP_VLDO4 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO4_STBY_MASK           0x0FU

/**
 * LDO4 Output voltage configuration in Standby Mode. It uses the OTP_VLDO4 to load the Default Voltage upon Power up
 */
#define PF8X_F_VLDO4_STBY_SHIFT          0x00U

/**
 * 1.5 V
 */
#define PF8X_F_VLDO4_STBY_1_5_V          (0x00U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 1.6 V
 */
#define PF8X_F_VLDO4_STBY_1_6_V          (0x01U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VLDO4_STBY_1_8_V          (0x02U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 1.85 V
 */
#define PF8X_F_VLDO4_STBY_1_85_V         (0x03U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 2.15 V
 */
#define PF8X_F_VLDO4_STBY_2_15_V         (0x04U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 2.5 V
 */
#define PF8X_F_VLDO4_STBY_2_5_V          (0x05U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 2.8 V
 */
#define PF8X_F_VLDO4_STBY_2_8_V          (0x06U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VLDO4_STBY_3_0_V          (0x07U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 3.1 V
 */
#define PF8X_F_VLDO4_STBY_3_1_V          (0x08U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 3.15 V
 */
#define PF8X_F_VLDO4_STBY_3_15_V         (0x09U << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 3.2 V
 */
#define PF8X_F_VLDO4_STBY_3_2_V          (0x0AU << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VLDO4_STBY_3_3_V          (0x0BU << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 3.35 V
 */
#define PF8X_F_VLDO4_STBY_3_35_V         (0x0CU << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 1.65 V
 */
#define PF8X_F_VLDO4_STBY_1_65_V         (0x0DU << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 1.7 V
 */
#define PF8X_F_VLDO4_STBY_1_7_V          (0x0EU << PF8X_F_VLDO4_STBY_SHIFT)
/**
 * 5.0 V
 */
#define PF8X_F_VLDO4_STBY_5_0_V          (0x0FU << PF8X_F_VLDO4_STBY_SHIFT)

/******************************************************************************/
/* VSNVS_CONFIG1 - Type: RW                                                   */
/******************************************************************************/

#define PF8X_VSNVS_CONFIG1_ADDR          0x9DU
#define PF8X_VSNVS_CONFIG1_DEFAULT       0x00U

/**
 * Selects VSNVS output voltage
 */
#define PF8X_F_VSNVSVOLT_MASK            0x03U

/**
 * Selects VSNVS output voltage
 */
#define PF8X_F_VSNVSVOLT_SHIFT           0x00U

/**
 * OFF
 */
#define PF8X_F_VSNVSVOLT_OFF             (0x00U << PF8X_F_VSNVSVOLT_SHIFT)
/**
 * 1.8 V
 */
#define PF8X_F_VSNVSVOLT_1_8V            (0x01U << PF8X_F_VSNVSVOLT_SHIFT)
/**
 * 3.0 V
 */
#define PF8X_F_VSNVSVOLT_3_0V            (0x02U << PF8X_F_VSNVSVOLT_SHIFT)
/**
 * 3.3 V
 */
#define PF8X_F_VSNVSVOLT_3_3V            (0x03U << PF8X_F_VSNVSVOLT_SHIFT)

/******************************************************************************/
/* PAGE_SELECT - Type: RW                                                     */
/******************************************************************************/

#define PF8X_PAGE_SELECT_ADDR            0x9FU
#define PF8X_PAGE_SELECT_DEFAULT         0x00U

/**
 * Select the I2C register page for TBB/OTP configuration and Test features.
 */
#define PF8X_F_PAGE_SELECT_MASK          0x07U

/**
 * Select the I2C register page for TBB/OTP configuration and Test features.
 */
#define PF8X_F_PAGE_SELECT_SHIFT         0x00U

/**
 * Functional Page selected (no extended page active)
 */
#define PF8X_F_PAGE_SELECT_FUNCT_PAGE    (0x00U << PF8X_F_PAGE_SELECT_SHIFT)
/**
 * OTP Mirror Registers
 */
#define PF8X_F_PAGE_SELECT_OTP_MIRROR    (0x01U << PF8X_F_PAGE_SELECT_SHIFT)
/**
 * OTP Misc
 */
#define PF8X_F_PAGE_SELECT_OTP_MISC      (0x02U << PF8X_F_PAGE_SELECT_SHIFT)



#endif /* PMIC_PF81_MAP_H__ */
