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
 * @file pmic_pf8x.c
 * @brief Implementation of I2C communication logic for NXP PMIC PF8x.
 *
 * @author nxa22158
 * @version 1.0
 * @date 21-August-2019
 * @copyright Copyright (c) 2019 NXP
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "pmic_pf8x.h"
#include "pmic_pf8x_assert.h"
#include "pmic_pf8x_common.h"

#if PF8X_DEVICE == PF8X_DEVICE_PF81
#include "pmic_pf81_map.h"
#else
#include "pmic_pf82_map.h"
#endif
#include "share_drivers.h"
#include "hal_i2c.h"
#include "ot_type.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define I2C_CONTROLLER_ID                      2U
#define READ_REG_TX_BUFF_SIZE                  1U
#define READ_MSG_SIZE                          2U
#define WRITE_MSG_SIZE                         1U

/* I2C frame. */
#define PF8X_ADDRESS_BASE 0x08U      /* I2C device base address */
#define PF8X_COMM_FRAME_SIZE 0x03U   /* Length of the communication frame */
#define PF8X_FRAME_SIZE 0x04U        /* Length of the complete I2C frame */
#define PF8X_READ_FRAME_LENGTH 0x01U /* Length of the data frame for I2C read command. */
#define PF8X_RX_SIZE 0x02U           /* Length of the received I2C data frame */

/* CRC polynomial. */
#define PF8X_CRC_TBL_SIZE 256U       /* Size of CRC table. */
#define PF8X_CRC_POLYNOM 0x1DU       /* CRC polynom. */
#define PF8X_CRC_INIT 0xFFU          /* CRC initial value. */

/*******************************************************************************
 * Constants
 ******************************************************************************/

/** @brief CRC lookup table. */
static const ot_u8 PF8X_CRC_TABLE[PF8X_CRC_TBL_SIZE] = {
    0x00U, 0x1DU, 0x3AU, 0x27U, 0x74U, 0x69U, 0x4EU, 0x53U, 0xE8U, 0xF5U, 0xD2U, 0xCFU, 0x9CU,
    0x81U, 0xA6U, 0xBBU, 0xCDU, 0xD0U, 0xF7U, 0xEAU, 0xB9U, 0xA4U, 0x83U, 0x9EU, 0x25U, 0x38U,
    0x1FU, 0x02U, 0x51U, 0x4CU, 0x6BU, 0x76U, 0x87U, 0x9AU, 0xBDU, 0xA0U, 0xF3U, 0xEEU, 0xC9U,
    0xD4U, 0x6FU, 0x72U, 0x55U, 0x48U, 0x1BU, 0x06U, 0x21U, 0x3CU, 0x4AU, 0x57U, 0x70U, 0x6DU,
    0x3EU, 0x23U, 0x04U, 0x19U, 0xA2U, 0xBFU, 0x98U, 0x85U, 0xD6U, 0xCBU, 0xECU, 0xF1U, 0x13U,
    0x0EU, 0x29U, 0x34U, 0x67U, 0x7AU, 0x5DU, 0x40U, 0xFBU, 0xE6U, 0xC1U, 0xDCU, 0x8FU, 0x92U,
    0xB5U, 0xA8U, 0xDEU, 0xC3U, 0xE4U, 0xF9U, 0xAAU, 0xB7U, 0x90U, 0x8DU, 0x36U, 0x2BU, 0x0CU,
    0x11U, 0x42U, 0x5FU, 0x78U, 0x65U, 0x94U, 0x89U, 0xAEU, 0xB3U, 0xE0U, 0xFDU, 0xDAU, 0xC7U,
    0x7CU, 0x61U, 0x46U, 0x5BU, 0x08U, 0x15U, 0x32U, 0x2FU, 0x59U, 0x44U, 0x63U, 0x7EU, 0x2DU,
    0x30U, 0x17U, 0x0AU, 0xB1U, 0xACU, 0x8BU, 0x96U, 0xC5U, 0xD8U, 0xFFU, 0xE2U, 0x26U, 0x3BU,
    0x1CU, 0x01U, 0x52U, 0x4FU, 0x68U, 0x75U, 0xCEU, 0xD3U, 0xF4U, 0xE9U, 0xBAU, 0xA7U, 0x80U,
    0x9DU, 0xEBU, 0xF6U, 0xD1U, 0xCCU, 0x9FU, 0x82U, 0xA5U, 0xB8U, 0x03U, 0x1EU, 0x39U, 0x24U,
    0x77U, 0x6AU, 0x4DU, 0x50U, 0xA1U, 0xBCU, 0x9BU, 0x86U, 0xD5U, 0xC8U, 0xEFU, 0xF2U, 0x49U,
    0x54U, 0x73U, 0x6EU, 0x3DU, 0x20U, 0x07U, 0x1AU, 0x6CU, 0x71U, 0x56U, 0x4BU, 0x18U, 0x05U,
    0x22U, 0x3FU, 0x84U, 0x99U, 0xBEU, 0xA3U, 0xF0U, 0xEDU, 0xCAU, 0xD7U, 0x35U, 0x28U, 0x0FU,
    0x12U, 0x41U, 0x5CU, 0x7BU, 0x66U, 0xDDU, 0xC0U, 0xE7U, 0xFAU, 0xA9U, 0xB4U, 0x93U, 0x8EU,
    0xF8U, 0xE5U, 0xC2U, 0xDFU, 0x8CU, 0x91U, 0xB6U, 0xABU, 0x10U, 0x0DU, 0x2AU, 0x37U, 0x64U,
    0x79U, 0x5EU, 0x43U, 0xB2U, 0xAFU, 0x88U, 0x95U, 0xC6U, 0xDBU, 0xFCU, 0xE1U, 0x5AU, 0x47U,
    0x60U, 0x7DU, 0x2EU, 0x33U, 0x14U, 0x09U, 0x7FU, 0x62U, 0x45U, 0x58U, 0x0BU, 0x16U, 0x31U,
    0x2CU, 0x97U, 0x8AU, 0xADU, 0xB0U, 0xE3U, 0xFEU, 0xD9U, 0xC4U
};

/*******************************************************************************
 * Local Functions Prototypes
 ******************************************************************************/

/** @brief This function calculates CRC value of passed data array.
 *         Takes bytes in inverted order due to frame format.
 *
 * @param [in] data     Data array.
 * @param [in] dataLen  Length of the data array.
 *
 * @return CRC8
 */
static ot_u8 PF8X_CalcCRC(const ot_u8* data, ot_u8 dataLen);

/** @brief Performs CRC check of the data array.
 *
 * CRC is expected in the first array item (index 0).
 *
 * @param [in] data     Data array.
 * @param [in] dataLen  Length of the data array.
 *
 * @return Returns pf8xStatusOk if CRC is correct.
 */
static pf8x_status_t PF8X_CheckCRC(const ot_u8* data, ot_u8 dataLen);

/** @brief Returns 7-bit I2C address.
 *
 * @param [in] drvData  Driver run-time data.
 *
 * @return PF8x I2C address.
 */
static ot_u8 PF8X_GetI2CAddress(pf8x_drv_data_t* drvData);

/*******************************************************************************
 * Local Variables
 ******************************************************************************/

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

/*******************************************************************************
 * Local Functions - Implementation
 ******************************************************************************/

/* I2C驱动程序需要的数据字节顺序与PF82驱动程序相反 */
static ot_void reverseByteArray(ot_u8* start, ot_u32 size)
{
    ot_u8* lo = start;
    ot_u8* hi = start + size - 1;
    ot_u8 swap;
    while (lo < hi)
    {
        swap = *lo;
        *lo++ = *hi;
        *hi-- = swap;
    }
}

static pf8x_status_t mcu_i2c_read_register(ot_u8 *rx_frame, ot_u8 i2c_addr, ot_u8 reg_addr, ot_u8 frame_len)
{
    pf8x_status_t ret = pf8xStatusError;
	i2c_client client = {0};

	client.i2c_num = I2C_CONTROLLER_ID;
	client.dev_addr = (i2c_addr >> 1);
	client.reg_addr = reg_addr;
	client.reg_width = 1U;

	ret = hal_i2c_recv(&client, rx_frame, frame_len);
    if (pf8xStatusOk != ret) {
        log_serial_puts((const signed char  *)"[error] hal_i2c_recv error\n");
        return ret;
    }
    reverseByteArray(rx_frame, frame_len);

    return pf8xStatusOk;
}

static pf8x_status_t mcu_i2c_write_register(ot_u8 *tx_frame, ot_u8 i2c_addr, ot_u8 frame_len)
{
    pf8x_status_t ret = pf8xStatusError;

    reverseByteArray(tx_frame, frame_len);
    ret = hal_i2c_send(I2C_CONTROLLER_ID, (i2c_addr), tx_frame, frame_len);
    if (pf8xStatusOk != ret) {
        log_serial_puts((const signed char  *)"[error] hal_i2c_send error\n");
        return ret;
    }

    return pf8xStatusOk;
}


/* This function calculates CRC value of passed data array.
 * Takes bytes in inverted order due to frame format. */
static ot_u8 PF8X_CalcCRC(const ot_u8* data, ot_u8 dataLen)
{
    ot_u8 crc;      /* Result. */
    ot_u8 tableIdx; /* Index to the CRC table. */
    ot_u8 dataIdx;  /* Index to the data array (memory). */

    /* Set CRC seed value. */
    crc = PF8X_CRC_INIT;

    for (dataIdx = (ot_u8)(dataLen - 1); dataIdx > 0; dataIdx--)
    {
        tableIdx = crc ^ data[dataIdx];
        crc = PF8X_CRC_TABLE[tableIdx];
    }
    return crc;
}

/* Performs CRC check of the data array. */
static pf8x_status_t PF8X_CheckCRC(const ot_u8* data, ot_u8 dataLen)
{
    ot_u8 frameCrc;  /* CRC value from resp. */
    ot_u8 compCrc;   /* Computed CRC value. */

    /* Check CRC. */
    frameCrc = data[0];
    compCrc = PF8X_CalcCRC(data, dataLen);
    return (compCrc != frameCrc) ? pf8xStatusError : pf8xStatusOk;
}

/* Returns 7-bit I2C address. */
static ot_u8 PF8X_GetI2CAddress(pf8x_drv_data_t* drvData)
{
    (ot_void)drvData;
    ot_u8 i2cAddress = PF8X_ADDRESS_BASE;

    return i2cAddress;
}

/*******************************************************************************
 * API - Implementation
 ******************************************************************************/

/* Performs a single read register based on provided address. */
pf8x_status_t PF8X_ReadRegister(pf8x_drv_data_t* drvData, ot_u8 address,
        ot_u8* rxData)
{
    pf8x_status_t status;
    ot_u8 i2cAddress;
    ot_u8 txFrame[PF8X_READ_FRAME_LENGTH] = {0};
    ot_u8 rxFrameCrc[PF8X_FRAME_SIZE] = {0};
    ot_u8 rxFrame[PF8X_FRAME_SIZE - 3U] = {0};

    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(rxData != OT_NULL);

    i2cAddress = PF8X_GetI2CAddress(drvData);
    txFrame[0] = address;

    if(drvData->crcEnabled)
    {
        /* Read 2 bytes - 1 data byte, 1 CRC byte. */
        status = mcu_i2c_read_register(rxFrameCrc, i2cAddress, txFrame[0], PF8X_RX_SIZE);
        if (status != pf8xStatusOk)
        {
            return status;
        }

        /* I2C response frame contains just data and CRC.
         * The Device address and Register address is added
         * for proper CRC check. */
        rxFrameCrc[2] = address;
        rxFrameCrc[3] = (ot_u8)((ot_u8)(i2cAddress << 1U) | 0x01U); /* R/W bit is 1 for I2C read */

        /* Check CRC. */
        status = PF8X_CheckCRC(rxFrameCrc, PF8X_FRAME_SIZE);
        if (status != pf8xStatusOk)
        {
            return status;
        }

        *rxData = rxFrameCrc[1];
    }
    else
    {
        /* Read 1 byte - data byte only, no CRC. */
        status = mcu_i2c_read_register(rxFrame, i2cAddress, txFrame[0], (PF8X_RX_SIZE - 1U));
        if (status != pf8xStatusOk)
        {
            return status;
        }

        *rxData = rxFrame[0];
    }

    return pf8xStatusOk;
}

/* Sends write command to the PF8x. */
pf8x_status_t PF8X_WriteRegister(pf8x_drv_data_t* drvData, ot_u8 address,
        ot_u8 writeData)
{
    ot_u8 i2cAddress;
    ot_u8 txFrameCrc[PF8X_FRAME_SIZE] = {0};
    ot_u8 txFrame[PF8X_FRAME_SIZE - 1U] = {0};

    FS_ASSERT(drvData != OT_NULL);

    i2cAddress = PF8X_GetI2CAddress(drvData);

    if(drvData->crcEnabled)
    {
        txFrameCrc[3] = (ot_u8)(i2cAddress << 1U); /* R/W bit is 0 for write */

        /* Sets address of the register (first two bits are ignored). */
        txFrameCrc[2] = address;

        /* Sets data. */
        txFrameCrc[1] = writeData;

        /* Add CRC. */
        txFrameCrc[0] = PF8X_CalcCRC(txFrameCrc, PF8X_FRAME_SIZE);

        /* Write 3 bytes - register address byte, data byte, CRC byte. */
        return mcu_i2c_write_register(txFrameCrc, i2cAddress, PF8X_COMM_FRAME_SIZE);
    }
    else
    {
        txFrame[2] = (ot_u8)(i2cAddress << 1U); /* R/W bit is 0 for write */

        /* Sets address of the register (first two bits are ignored). */
        txFrame[1] = address;

        /* Sets data. */
        txFrame[0] = writeData;

        /* Write 2 bytes - register address byte, data byte, no CRC. */
        return mcu_i2c_write_register(txFrame, i2cAddress, (PF8X_COMM_FRAME_SIZE - 1U));
    }
}

/* Performs a write to a single PF82 secured register. */
pf8x_status_t PF8X_WriteRegisterSecured(pf8x_drv_data_t* drvData,
		ot_u8 address, ot_u8 writeData)
{
#if PF8X_DEVICE == PF8X_DEVICE_PF82
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);

    /* Write data to secured register. */
    status = PF8X_WriteRegister(drvData, address, writeData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Read generated random code from RANDOM_GEN register. */
    status = PF8X_ReadRegister(drvData, PF8X_RANDOM_GEN_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Write generated random code to RANDOM_CHK register. */
    return PF8X_WriteRegister(drvData, PF8X_RANDOM_CHK_ADDR, rxData);
#else
    return pf8xStatusError; /* Supported only on PF82. */
#endif
}


/* Reads device identification information. */
pf8x_status_t PF8X_GetId(pf8x_drv_data_t* drvData, ot_u8* deviceFamily, ot_u8* deviceId,
        ot_u8* fullRev, ot_u8* metalRev, ot_u8* emRev, ot_u16* progId)
{
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);

    /* Skip reading DEVICE_ID register, if deviceFamily or deviceID is OT_NULL. */
    if((deviceFamily != OT_NULL) && (deviceId != OT_NULL))
    {
        /* Read DEVICE_ID register. */
        status = PF8X_ReadRegister(drvData, PF8X_DEVICE_ID_ADDR, &rxData);
        if (status != pf8xStatusOk)
        {
            return status;
        }

        /* Store device family and device ID. */
        *deviceFamily = (rxData & PF8X_F_DEVICE_FAM_MASK) >> PF8X_F_DEVICE_FAM_SHIFT;
        *deviceId = (rxData & PF8X_F_DEVICE_ID_MASK) >> PF8X_F_DEVICE_ID_SHIFT;
    }

    /* Skip reading REV_ID register, if fullRev or metalRev is OT_NULL. */
    if((fullRev != OT_NULL) && (metalRev != OT_NULL))
    {
        /* Read REV_ID register. */
        status = PF8X_ReadRegister(drvData, PF8X_REV_ID_ADDR, &rxData);
        if (status != pf8xStatusOk)
        {
            return status;
        }

        /* Store full layer revision and metal layer revision. */
        *fullRev = (ot_u8)((rxData & PF8X_F_FULL_LAYER_REV_MASK) >> PF8X_F_FULL_LAYER_REV_SHIFT);
        *metalRev = (ot_u8)((rxData & PF8X_F_METAL_LAYER_REV_MASK) >> PF8X_F_METAL_LAYER_REV_SHIFT);
    }

    /* Skip reading EMREV and PROG_ID register, if emRev or progId is OT_NULL. */
    if((emRev != OT_NULL) && (progId != OT_NULL))
    {
        /* Read EMREV register. */
        status = PF8X_ReadRegister(drvData, PF8X_EMREV_ADDR, &rxData);
        if (status != pf8xStatusOk)
        {
            return status;
        }

        /* Store program ID high byte and EMREV. */
        *progId = (ot_u16)((ot_u16)((rxData & PF8X_F_PROG_IDH_MASK) >> PF8X_F_PROG_IDH_SHIFT) << 8U);
        *emRev = (ot_u8)(rxData & PF8X_F_EMREV_MASK) >> PF8X_F_EMREV_SHIFT;

        /* Read PROG_ID register. */
        status = PF8X_ReadRegister(drvData, PF8X_PROG_ID_ADDR, &rxData);
        if (status != pf8xStatusOk)
        {
            return status;
        }

        /* Store program ID low byte. */
        *progId |= (ot_u16)((rxData & PF8X_F_PROG_IDL_MASK) >> PF8X_F_PROG_IDL_SHIFT);
    }

    return pf8xStatusOk;
}

/* Sets power up/down configuration. */
pf8x_status_t PF8X_SetPowerSequence(pf8x_drv_data_t* drvData,
        px8x_pin_reg_t pinReg, ot_u8 seqSlot)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Write settings to PWRUP register. */
    return PF8X_WriteRegister(drvData, pinReg, seqSlot);
}

/* Sets buck regulator configuration. */
pf8x_status_t PF8X_SetBuckConfig(pf8x_drv_data_t* drvData, pf8x_reg_buck_t buckSwNum,
        ot_bool uvBypass, ot_bool ovBypass, ot_bool ilimBypass, ot_bool uvState, ot_bool ovState,
        ot_bool ilimState, ot_bool wdBypass, ot_bool pgoodEnable, ot_bool fltRen, ot_bool vttEnable,
        ot_bool dvsRamp, ot_u8 ilim, ot_u8 phase)
{
    pf8x_status_t status;

    FS_ASSERT(drvData != OT_NULL);

    /* Write mode settings to selected regulator CONFIG1 register. */
    status = PF8X_WriteRegister(drvData, (ot_u8)(PF8X_SW1_CONFIG1_ADDR + (ot_u8)(8 * (ot_u8)buckSwNum)),
            (ot_u8)((ot_u8)((ot_u8)uvBypass << PF8X_F_SW1_UV_BYPASS_SHIFT) |
            (ot_u8)((ot_u8)ovBypass << PF8X_F_SW1_OV_BYPASS_SHIFT) |
            (ot_u8)((ot_u8)ilimBypass << PF8X_F_SW1_ILIM_BYPASS_SHIFT) |
            (ot_u8)((ot_u8)uvState << PF8X_F_SW1_UV_STATE_SHIFT) |
            (ot_u8)((ot_u8)ovState << PF8X_F_SW1_OV_STATE_SHIFT) |
            (ot_u8)((ot_u8)ilimState << PF8X_F_SW1_ILIM_STATE_SHIFT) |
            (ot_u8)((ot_u8)wdBypass << PF8X_F_SW1_WDBYPASS_SHIFT) |
            (ot_u8)((ot_u8)pgoodEnable << PF8X_F_SW1_PG_EN_SHIFT)));
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Write settings to selected regulator CONFIG2 register. */
    return PF8X_WriteRegister(drvData, (ot_u8)(PF8X_SW1_CONFIG2_ADDR + (ot_u8)(8 * (ot_u8)buckSwNum)),
            (ot_u8)(((ot_u8)fltRen << PF8X_F_SW6_FLT_REN_SHIFT) |
            ((ot_u8)vttEnable << PF8X_F_SW6_VTTEN_SHIFT) |
            ((ot_u8)dvsRamp << PF8X_F_SW6DVS_RAMP_SHIFT) |
            ilim |
            phase));
}

/* Sets buck regulator mode and power down group. */
pf8x_status_t PF8X_SetBuckModeConfig(pf8x_drv_data_t* drvData, pf8x_reg_buck_t buckSwNum,
        pf8x_reg_powerdown_t pdGroup, pf8x_reg_mode_t runMode, pf8x_reg_mode_t stbyMode)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Write mode settings to selected regulator MODE register. */
    return PF8X_WriteRegister(drvData, (ot_u8)(PF8X_SW1_MODE_ADDR + (ot_u8)(8 * (ot_u8)buckSwNum)),
            (ot_u8)((ot_u8)(pdGroup << PF8X_F_SW1_PDGRP_SHIFT) |
            (ot_u8)(stbyMode << PF8X_F_SW1_STBY_MODE_SHIFT) |
            (ot_u8)(runMode << PF8X_F_SW1_RUN_MODE_SHIFT)));
}

/* Sets buck regulator voltage. */
pf8x_status_t PF8X_SetBuckVoltage(pf8x_drv_data_t* drvData, pf8x_reg_buck_t buckSwNum,
        pf8x_reg_state_t regState, ot_u8 voltage)
{
    pf8x_status_t status;

    FS_ASSERT(drvData != OT_NULL);

    /* Write voltage to selected regulator VOLT register. */
    if(buckSwNum < pf8xRegBuckSW7)
    {
        status = PF8X_WriteRegister(drvData, (ot_u8)(PF8X_SW1_RUN_VOLT_ADDR +
                regState + (ot_u8)(8 * (ot_u8)buckSwNum)), voltage);
    }
    else
    {
        status = PF8X_WriteRegister(drvData, PF8X_SW7_VOLT_ADDR, voltage);
    }

    return status;
}

/* Sets LDO regulator configuration. */
pf8x_status_t PF8X_SetLdoConfig(pf8x_drv_data_t* drvData, pf8x_reg_ldo_t ldoNum,
        ot_bool uvBypass, ot_bool ovBypass, ot_bool ilimBypass, ot_bool uvState, ot_bool ovState,
        ot_bool ilimState, ot_bool wdBypass, ot_bool pgoodEnable)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Write settings to selected regulator CONFIG1 register. */
    return PF8X_WriteRegister(drvData, (ot_u8)(PF8X_LDO1_CONFIG1_ADDR + (ot_u8)(6 * (ot_u8)ldoNum)),
            (ot_u8)(((ot_u8)uvBypass << PF8X_F_LDO1_UV_BYPASS_SHIFT) |
            ((ot_u8)ovBypass << PF8X_F_LDO1_OV_BYPASS_SHIFT) |
            ((ot_u8)ilimBypass << PF8X_F_LDO1_ILIM_BYPASS_SHIFT) |
            ((ot_u8)uvState << PF8X_F_LDO1_UV_STATE_SHIFT) |
            ((ot_u8)ovState << PF8X_F_LDO1_OV_STATE_SHIFT) |
            ((ot_u8)ilimState << PF8X_F_LDO1_ILIM_STATE_SHIFT) |
            ((ot_u8)wdBypass << PF8X_F_LDO1_WDBYPASS_SHIFT) |
            ((ot_u8)pgoodEnable << PF8X_F_LDO1_PG_EN_SHIFT)));
}

/* Sets LDO regulator mode and power down group. */
pf8x_status_t PF8X_SetLdoModeConfig(pf8x_drv_data_t* drvData, pf8x_reg_ldo_t ldoNum,
        ot_bool fltRen, pf8x_reg_powerdown_t pdGroup, ot_bool runEnable, ot_bool stbyEnable)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Write mode settings to selected regulator CONFIG2 register. */
    return PF8X_WriteRegister(drvData, (ot_u8)(PF8X_LDO1_CONFIG2_ADDR + (ot_u8)(6 * (ot_u8)ldoNum)),
            (ot_u8)(((ot_u8)fltRen << PF8X_F_LDO1_FLT_REN_SHIFT) |
            (pdGroup << PF8X_F_LDO1_PDGRP_SHIFT) |
            ((ot_u8)runEnable << PF8X_F_LDO1_RUN_EN_SHIFT) |
            ((ot_u8)stbyEnable << PF8X_F_LDO1_STBY_EN_SHIFT)));
}

/* Sets LDO regulator voltage. */
pf8x_status_t PF8X_SetLdoVoltage(pf8x_drv_data_t* drvData, pf8x_reg_ldo_t ldoNum,
        pf8x_reg_state_t regState, ot_u8 voltage)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Write voltage to selected regulator VOLT register. */
    return PF8X_WriteRegister(drvData, (ot_u8)(PF8X_LDO1_RUN_VOLT_ADDR +
            regState + (ot_u8)(6 * (ot_u8)ldoNum)), voltage);
}

/* Set frequency configuration of the PMIC. */
pf8x_status_t PF8X_SetFreqConfig(pf8x_drv_data_t* drvData, ot_bool syncoutEnable,
        pf8x_fsync_range_t fsyncRange, ot_bool fssEnable, pf8x_fss_range_t fssRange, ot_u8 clkFreq)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Write settings to FREQ_CTRL register. */
    return PF8X_WriteRegister(drvData, PF8X_FREQ_CTRL_ADDR,
            (ot_u8)((syncoutEnable << PF8X_F_SYNCOUT_EN_SHIFT) |
            fsyncRange |
            (fssEnable << PF8X_F_FSS_EN_SHIFT) |
            fssRange |
            clkFreq));
}

/* Sets input voltage monitor configuration. */
pf8x_status_t PF8X_SetVinOvloConfig(pf8x_drv_data_t* drvData, ot_bool ovloEnable,
        ot_bool ovloShutdown, pf8x_ovlo_dbnc_t ovloDebounce)
{
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);

    /* Read CTRL1 register. */
    status = PF8X_ReadRegister(drvData, PF8X_CTRL1_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Clear bits before changing them. */
    rxData &= (ot_u8)(~(PF8X_F_VIN_OVLO_EN_MASK | PF8X_F_VIN_OVLO_SDWN_MASK));
    /* Modify register content. */
    rxData |= (ot_u8)(((ot_u8)ovloEnable << PF8X_F_VIN_OVLO_EN_SHIFT) |
            ((ot_u8)ovloShutdown << PF8X_F_VIN_OVLO_SDWN_SHIFT));

    /* Write settings to CTRL1 register. */
    if(drvData->secureEnabled)
    {
        status = PF8X_WriteRegisterSecured(drvData, PF8X_CTRL1_ADDR, rxData);
    }
    else
    {
        status = PF8X_WriteRegister(drvData, PF8X_CTRL1_ADDR, rxData);
    }
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Read CTRL2 register. */
    status = PF8X_ReadRegister(drvData, PF8X_CTRL2_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Clear bits before changing them. */
    rxData &= (ot_u8)(~(PF8X_F_VIN_OVLO_DBNC_MASK));
    /* Modify register content. */
    rxData |= ovloDebounce;

    /* Write settings to CTRL2 register. */
    return PF8X_WriteRegister(drvData, PF8X_CTRL2_ADDR, rxData);
}

/* Sets voltage monitor configuration. */
pf8x_status_t PF8X_SetVoltMonitorConfig(pf8x_drv_data_t* drvData,
        ot_u8 swMonMask, ot_u8 ldoMonMask)
{
    pf8x_status_t status;

    FS_ASSERT(drvData != OT_NULL);

    /* Write settings to VMONEN1 register. */
    if(drvData->secureEnabled)
    {
        status = PF8X_WriteRegisterSecured(drvData, PF8X_VMONEN1_ADDR, swMonMask);
    }
    else
    {
        status = PF8X_WriteRegister(drvData, PF8X_VMONEN1_ADDR, swMonMask);
    }
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Write settings to VMONEN2 register. */
    if(drvData->secureEnabled)
    {
        return PF8X_WriteRegisterSecured(drvData, PF8X_VMONEN2_ADDR, ldoMonMask);
    }
    else
    {
        return PF8X_WriteRegister(drvData, PF8X_VMONEN2_ADDR, ldoMonMask);
    }
}

/* Sets temperature monitor configuration. */
pf8x_status_t PF8X_SetTempMonitorConfig(pf8x_drv_data_t* drvData,
        ot_bool tempMonEnable, ot_bool tempMonAon)
{
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);

    /* Read CTRL1 register. */
    status = PF8X_ReadRegister(drvData, PF8X_CTRL1_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Clear bits before changing them. */
    rxData &= (ot_u8)(~(PF8X_F_TMP_MON_EN_MASK));
    /* Modify register content. */
    rxData |= (ot_u8)((ot_u8)tempMonEnable << PF8X_F_TMP_MON_EN_SHIFT);

    /* Write settings to CTRL1 register. */
    if(drvData->secureEnabled)
    {
        status = PF8X_WriteRegisterSecured(drvData, PF8X_CTRL1_ADDR, rxData);
    }
    else
    {
        status = PF8X_WriteRegister(drvData, PF8X_CTRL1_ADDR, rxData);
    }
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Read CTRL2 register. */
    status = PF8X_ReadRegister(drvData, PF8X_CTRL2_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Clear bits before changing them. */
    rxData &= (ot_u8)(~(PF8X_F_TMP_MON_AON_MASK));
    /* Modify register content. */
    rxData |= (ot_u8)((ot_u8)tempMonAon << PF8X_F_TMP_MON_AON_SHIFT);

    /* Write settings to CTRL2 register. */
    return PF8X_WriteRegister(drvData, PF8X_CTRL2_ADDR, rxData);
}

/* Reads interrupt flags from selected interrupt group. */
pf8x_status_t PF8X_GetInterrupts(pf8x_drv_data_t* drvData, pf8x_int_group_t intGroup, ot_u8* intStatus)
{
    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(intStatus != OT_NULL);

    /* Read interrupt register. */
    return PF8X_ReadRegister(drvData, intGroup, intStatus);
}

/* Clears selected interrupt flags from selected interrupt group. */
pf8x_status_t PF8X_ClearInterrupts(pf8x_drv_data_t* drvData, pf8x_int_group_t intGroup, ot_u8 intMask)
{
    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(intGroup < pf8xIntGroupSysInt); /* SYS_INT register is cleared automatically. */

    /* Clear flags in interrupt register. */
    return PF8X_WriteRegister(drvData, intGroup, intMask);
}

/* Sets which interrupts pull the INTB pin. */
pf8x_status_t PF8X_MaskInterrupts(pf8x_drv_data_t* drvData, pf8x_int_group_t intGroup, ot_u8 intMask)
{
    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(intGroup < pf8xIntGroupSysInt); /* SYS_INT register cannot be masked. */

    /* Write to interrupt MASK register. */
    return PF8X_WriteRegister(drvData, (ot_u8)(intGroup + 1U), intMask);
}

/* Reads the actual status of the signal triggering interrupt. */
pf8x_status_t PF8X_SenseInterrupts(pf8x_drv_data_t* drvData, pf8x_int_group_t intGroup, ot_u8* intSense)
{
    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(intSense != OT_NULL);
    /* There is no SENSE register for SW_MODE and SYS_INT. */
    FS_ASSERT((intGroup < pf8xIntGroupSysInt) && (intGroup != pf8xIntGroupSwMode));

    /* Read interrupt SENSE register. */
    return PF8X_ReadRegister(drvData, (ot_u8)(intGroup + 2U), intSense);
}

/* Reads hardfault status. */
pf8x_status_t PF8X_GetHardfaultFlags(pf8x_drv_data_t* drvData, ot_u8* hfFlags)
{
    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(hfFlags != OT_NULL);

    /* Read HARDFAULT_FLAGS register. */
    return PF8X_ReadRegister(drvData, PF8X_HARDFAULT_FLAGS_ADDR, hfFlags);
}

/* Clears hardfault status flags. */
pf8x_status_t PF8X_ClearHardfaultFlags(pf8x_drv_data_t* drvData, ot_u8 flagsMask)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Clear hardfault flags. */
    return PF8X_WriteRegister(drvData, PF8X_HARDFAULT_FLAGS_ADDR, flagsMask);
}

/* Sets FSOB configuration. */
pf8x_status_t PF8X_SetFSOBConfig(pf8x_drv_data_t* drvData,
        ot_bool softfault, ot_bool wdi, ot_bool wdc, ot_bool hardfault)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Write settings to FSOB_SELECT register. */
    return PF8X_WriteRegister(drvData, PF8X_FSOB_SELECT_ADDR,
            (ot_u8)(((ot_u8)softfault << PF8X_F_FSOB_SOFTFAULT_SHIFT) |
            ((ot_u8)wdi << PF8X_F_FSOB_WDI_SHIFT) |
            ((ot_u8)wdc << PF8X_F_FSOB_WDC_SHIFT) |
            ((ot_u8)hardfault << PF8X_F_FSOB_HARDFAULT_SHIFT)));
}

/* Reads FSOB status. */
pf8x_status_t PF8X_GetFSOBFlags(pf8x_drv_data_t* drvData, ot_u8* fsFlags)
{
    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(fsFlags != OT_NULL);

    /* Read FSOB_FLAGS register. */
    return PF8X_ReadRegister(drvData, PF8X_FSOB_FLAGS_ADDR, fsFlags);
}

/* Clears FSOB status flags. */
pf8x_status_t PF8X_ClearFSOBFlags(pf8x_drv_data_t* drvData, ot_u8 flagsMask)
{
    pf8x_status_t status;

    FS_ASSERT(drvData != OT_NULL);

    /* Clear FSOB flags. */
    if(drvData->secureEnabled)
    {
        status = PF8X_WriteRegisterSecured(drvData, PF8X_FSOB_FLAGS_ADDR, flagsMask);
    }
    else
    {
        status = PF8X_WriteRegister(drvData, PF8X_FSOB_FLAGS_ADDR, flagsMask);
    }

    return status;
}

/* Sets watchdog configuration. */
pf8x_status_t PF8X_SetWatchdogConfig(pf8x_drv_data_t* drvData, ot_bool wdEnable,
        ot_bool wdStbyEnable, ot_u8 wdDuration, ot_u8 wdMaxExpire, ot_u8 wdMaxCnt)
{
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);

    /* Read CTRL1 register. */
    status = PF8X_ReadRegister(drvData, PF8X_CTRL1_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Clear bits before changing them. */
    rxData &= (ot_u8)(~(PF8X_F_WD_EN_MASK | PF8X_F_WD_STBY_EN_MASK));
    /* Modify register content. */
    rxData |= (ot_u8)(((ot_u8)wdEnable << PF8X_F_WD_EN_SHIFT) |
            ((ot_u8)wdStbyEnable << PF8X_F_WD_STBY_EN_SHIFT));

    /* Write settings to CTRL1 register. */
    if(drvData->secureEnabled)
    {
        status = PF8X_WriteRegisterSecured(drvData, PF8X_CTRL1_ADDR, rxData);
    }
    else
    {
        status = PF8X_WriteRegister(drvData, PF8X_CTRL1_ADDR, rxData);
    }
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Write settings to WD_CONFIG register. */
    status = PF8X_WriteRegister(drvData, PF8X_WD_CONFIG_ADDR, wdDuration);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Read WD_EXPIRE register. */
    status = PF8X_ReadRegister(drvData, PF8X_WD_EXPIRE_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Clear bits before changing them. */
    rxData &= (ot_u8)(~(PF8X_F_WD_MAX_EXPIRE_MASK));
    /* Modify register content. */
    rxData |= wdMaxExpire;

    /* Write settings to WD_EXPIRE register. */
    status = PF8X_WriteRegister(drvData, PF8X_WD_EXPIRE_ADDR, rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Read WD_COUNTER register. */
    status = PF8X_ReadRegister(drvData, PF8X_WD_COUNTER_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Clear bits before changing them. */
    rxData &= (ot_u8)(~(PF8X_F_WD_MAX_CNT_MASK));
    /* Modify register content. */
    rxData |= wdMaxCnt;

    /* Write settings to WD_COUNTER register. */
    return PF8X_WriteRegister(drvData, PF8X_WD_COUNTER_ADDR, rxData);
}

/* Reads watchdog status. */
pf8x_status_t PF8X_GetWatchdogStatus(pf8x_drv_data_t* drvData, ot_u8* wdExpireCnt, ot_u8* wdEventCnt)
{
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(wdExpireCnt != OT_NULL);
    FS_ASSERT(wdEventCnt != OT_NULL);

    /* Read WD_EXPIRE register. */
    status = PF8X_ReadRegister(drvData, PF8X_WD_EXPIRE_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    *wdExpireCnt = (rxData & PF8X_F_WD_EXPIRE_CNT_MASK) >> PF8X_F_WD_EXPIRE_CNT_SHIFT;

    /* Read WD_COUNTER register. */
    status = PF8X_ReadRegister(drvData, PF8X_WD_COUNTER_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    *wdEventCnt = (rxData & PF8X_F_WD_EVENT_CNT_MASK) >> PF8X_F_WD_EVENT_CNT_SHIFT;

    return status;
}

/* Clears watchdog. */
pf8x_status_t PF8X_ClearWatchdog(pf8x_drv_data_t* drvData)
{
    pf8x_status_t status;

    FS_ASSERT(drvData != OT_NULL);

    /* Write to WD_CLEAR register. */
    if(drvData->secureEnabled)
    {
        status = PF8X_WriteRegisterSecured(drvData, PF8X_WD_CLEAR_ADDR, PF8X_F_WD_CLEAR_1);
    }
    else
    {
        status = PF8X_WriteRegister(drvData, PF8X_WD_CLEAR_ADDR, PF8X_F_WD_CLEAR_1);
    }

    return status;
}

/* Sets fault configuration. */
pf8x_status_t PF8X_SetFaultConfig(pf8x_drv_data_t* drvData, ot_u8 faultMaxCnt, ot_u8 faultTimer)
{
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);

    /* Read FAULT_COUNTER register. */
    status = PF8X_ReadRegister(drvData, PF8X_FAULT_COUNTER_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Clear bits before changing them. */
    rxData &= (ot_u8)(~(PF8X_F_FAULT_MAX_CNT_MASK));
    /* Modify register content. */
    rxData |= faultMaxCnt;

    /* Write settings to FAULT_COUNTER register. */
    status = PF8X_WriteRegister(drvData, PF8X_FAULT_COUNTER_ADDR, rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Write settings to FAULT_TIMERS register. */
    return PF8X_WriteRegister(drvData, PF8X_FAULT_TIMERS_ADDR, faultTimer);
}

/* Reads fault counter value. */
pf8x_status_t PF8X_GetFaultCounter(pf8x_drv_data_t* drvData, ot_u8* faultCnt)
{
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(faultCnt != OT_NULL);

    /* Read FAULT_COUNTER register. */
    status = PF8X_ReadRegister(drvData, PF8X_FAULT_COUNTER_ADDR, &rxData);

    *faultCnt = (rxData & PF8X_F_FAULT_CNT_MASK) >> PF8X_F_FAULT_CNT_SHIFT;

    return status;
}

#if PF8X_DEVICE == PF8X_DEVICE_PF82
/* Runs ABIST on demand. */
pf8x_status_t PF8X_RunAbist(pf8x_drv_data_t* drvData)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Write to ABIST_RUN register. */
    if(drvData->secureEnabled)
    {
        return PF8X_WriteRegisterSecured(drvData, PF8X_ABIST_RUN_ADDR, PF8X_F_AB_RUN_1);
    }
    else
    {
        return PF8X_WriteRegister(drvData, PF8X_ABIST_RUN_ADDR, PF8X_F_AB_RUN_1);
    }
}

/* Reads ABIST OV and UV flags. */
pf8x_status_t PF8X_GetAbistFlags(pf8x_drv_data_t* drvData, ot_bool ovUvFlags, ot_u16* abistFlags)
{
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(abistFlags != OT_NULL);

    /* Read ABIST_OV1/UV1 register (SW flags). */
    status = PF8X_ReadRegister(drvData, (ot_u8)(PF8X_ABIST_OV1_ADDR + (ot_u8)(2 * (ot_u8)ovUvFlags)), &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    *abistFlags = rxData;

    /* Read ABIST_OV2/UV2 register (LDO flags). */
    status = PF8X_ReadRegister(drvData, (ot_u8)(PF8X_ABIST_OV2_ADDR + (ot_u8)(2 * (ot_u8)ovUvFlags)), &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    *abistFlags |= (ot_u8)((ot_u8)(rxData & 0x0FU) << 8U);

    return status;
}

/* Clears ABIST status flags. */
pf8x_status_t PF8X_ClearAbistFlags(pf8x_drv_data_t* drvData, ot_bool ovUvFlags, ot_u16 flagsMask)
{
    pf8x_status_t status;

    FS_ASSERT(drvData != OT_NULL);

    /* Clear ABIST_OV1/UV1 register (SW flags). */
    if(drvData->secureEnabled)
    {
        status = PF8X_WriteRegisterSecured(drvData, (ot_u8)(PF8X_ABIST_OV1_ADDR + (ot_u8)(2 * (ot_u8)ovUvFlags)), (ot_u8)(flagsMask & 0x00FFU));
    }
    else
    {
        status = PF8X_WriteRegister(drvData, (ot_u8)(PF8X_ABIST_OV1_ADDR + (ot_u8)(2 * (ot_u8)ovUvFlags)), (ot_u8)(flagsMask & 0x00FFU));
    }
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Clear ABIST_OV2/UV2 register (LDO flags). */
    if(drvData->secureEnabled)
    {
        return PF8X_WriteRegisterSecured(drvData, (ot_u8)(PF8X_ABIST_OV2_ADDR + (ot_u8)(2 * (ot_u8)ovUvFlags)), (ot_u8)((flagsMask & 0xFF00U) >> 8U));
    }
    else
    {
        return PF8X_WriteRegister(drvData, (ot_u8)(PF8X_ABIST_OV2_ADDR + (ot_u8)(2 * (ot_u8)ovUvFlags)), (ot_u8)((flagsMask & 0xFF00U) >> 8U));
    }
}
#endif

/* Reads test flags. */
pf8x_status_t PF8X_GetTestFlags(pf8x_drv_data_t* drvData, ot_u8* testFlags)
{
    FS_ASSERT(drvData != OT_NULL);
    FS_ASSERT(testFlags != OT_NULL);

    /* Read TEST_FLAGS register. */
    return PF8X_ReadRegister(drvData, PF8X_TEST_FLAGS_ADDR, testFlags);
}

/* Clears test flags. */
pf8x_status_t PF8X_ClearTestFlags(pf8x_drv_data_t* drvData, ot_u8 flagsMask)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Clear TEST_FLAGS register. */
    return PF8X_WriteRegister(drvData, PF8X_TEST_FLAGS_ADDR, flagsMask);
}

/* Sets analog multiplexer (AMUX). */
pf8x_status_t PF8X_SetAmux(pf8x_drv_data_t* drvData, ot_bool amuxEnable, ot_u8 amuxSel)
{
    FS_ASSERT(drvData != OT_NULL);

    /* Write settings to AMUX register. */
    return PF8X_WriteRegister(drvData, PF8X_AMUX_ADDR,
            (ot_u8)(((ot_u8)amuxEnable << PF8X_F_AMUX_EN_SHIFT) |
            amuxSel));
}

/* Turns PMIC off (after 500us shutdown timer expires). */
pf8x_status_t PF8X_TurnOff(pf8x_drv_data_t* drvData)
{
    pf8x_status_t status;
    ot_u8 rxData = 0;

    FS_ASSERT(drvData != OT_NULL);

    /* Read CTRL3 register. */
    status = PF8X_ReadRegister(drvData, PF8X_CTRL3_ADDR, &rxData);
    if (status != pf8xStatusOk)
    {
        return status;
    }

    /* Set PMIC_OFF bit. */
    rxData |= PF8X_F_PMIC_OFF_1;

    /* Write to CTRL3 register. */
    return PF8X_WriteRegister(drvData, PF8X_CTRL3_ADDR, rxData);
}
