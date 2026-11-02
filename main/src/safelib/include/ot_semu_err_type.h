#ifndef OT_SEMU_ERR_TYPE_H
#define OT_SEMU_ERR_TYPE_H

#include "ot_eis_type.h"
#include "ot_semu_sm_cfg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct {
    ot_u32 dev_id;
    ot_u32 vc_id;
    ot_u32 phy_id;
} ot_semu_mipirx_err_info;

typedef struct {
    ot_u32 dev_id;
} ot_semu_i2c_err_info;

typedef struct {
    ot_u32 dev_id;
} ot_semu_spi_err_info;


typedef struct {
    unsigned int chn_id;
    unsigned int dev_id;
} ot_semu_vi_err_info;

typedef struct {
    ot_semu_reg *registers;
    ot_u32 num_registers;
} ot_semu_registers_err_info;

typedef union {
    ot_semu_mipirx_err_info mipirx;
    ot_semu_i2c_err_info i2c;
    ot_semu_spi_err_info spi;
    ot_semu_vi_err_info vi;
    ot_semu_registers_err_info regs;
} ot_semu_err_info;

typedef enum {
    OT_SEMU_SM_TEMPERATURE_MONITOR = 0,
    OT_SEMU_SM_ACORE_STL_MONITOR,
    OT_SEMU_SM_VID_DELAY_MONITOR,
    OT_SEMU_SM_REGISTERS_MONITOR,
    OT_SEMU_SM_DDR_MONITOR,
    OT_SEMU_SM_CLOCK_MONITOR,
    OT_SEMU_SM_MIPIRX_ERR_MONITOR,
    OT_SEMU_SM_MIPITX_ERR_MONITOR,
    OT_SEMU_SM_MIPITX_FPS_ERR_MONITOR,
    OT_SEMU_SM_VI_ERR_MONITOR,
    OT_SEMU_SM_VP_ERR_MONITOR,
    OT_SEMU_SM_VO_ERR_MONITOR,
    OT_SEMU_SM_SPI_ERR_MONITOR,
    OT_SEMU_SM_EMMC_ERR_MONITOR,
    OT_SEMU_SM_I2C_ERR_MONITOR,
    OT_SEMU_SM_FMC_ERR_MONITOR,
    OT_SEMU_SM_FRAME_ERR_MONITOR,
    OT_SEMU_SM_INVALID
} ot_semu_sm_type;

// temperature err type
typedef enum {
    OT_SEMU_TMP_ERR_RECOVER_OK, // level_1 < tmp < level 2, lasting debounce_time
    OT_SEMU_TMP_ERR_RISK,       // level_1 < tmp < level 2, lasting debounce_time
                                // risk_time
    OT_SEMU_TMP_ERR_FATAL,      // level_3 < tmp < level 4, lasting debounce_time
                                // fatal_time
    OT_SEMU_TMP_ERR_RESET,      // > Level_4
    OT_SEMU_TMP_VALUE_ERR,      //get invald tem
    OT_SEMU_TMP_ERR_INVALID
} ot_semu_temper_err_type;

// mipi-rx err type
typedef enum {
    OT_SEMU_MIPIRX_ERR_RECOVER_OK,
    OT_SEMU_MIPIRX_ERR_ECC_1_BIT,     // single bit ecc
    OT_SEMU_MIPIRX_ERR_ECC_MULTI_BIT, // multi bit ecc
    OT_SEMU_MIPIRX_ERR_CRC,
    OT_SEMU_MIPIRX_ERR_DATA_FIFO_OP, // fifo operation read/write err
    OT_SEMU_MIPIRX_ERR_CMD_FIFO_OP,
    OT_SEMU_MIPIRX_ERR_FRAME_SEQ, // frame sequence disorder
    OT_SEMU_MIPIRX_ERR_PHY,
    OT_SEMU_MIPIRX_ERR_ALIGN,
    OT_SEMU_MIPIRX_ERR_INVALID
} ot_semu_mipirx_err_type;

// mipi-tx err type
typedef enum {
    OT_SEMU_MIPITX_ERR_RECOVER_OK,    // only notified with self recovery
    OT_SEMU_MIPITX_ERR_ECC_1_BIT,     // single bit ecc
    OT_SEMU_MIPITX_ERR_ECC_MULTI_BIT, // multi bit ecc
    OT_SEMU_MIPITX_ERR_CRC,
    OT_SEMU_MIPITX_ERR_FRAME_SEQ,
    OT_SEMU_MIPITX_ERR_PROTO_VIOLATION, // protocol violation
    OT_SEMU_MIPITX_ERR_SYNC,
    OT_SEMU_MIPITX_ERR_PERIP_TIMEOUT, // peripheral response timeout
    OT_SEMU_MIPITX_ERR_ESCAPE,        // escape entry err
    OT_SEMU_MIPITX_ERR_PHY_PLLLOCK,
    OT_SEMU_MIPITX_ERR_INVALID
} ot_semu_mipitx_err_type;

// mipi-tx fps err type
typedef enum {
    OT_SEMU_MIPITX_FPS_ERR_RECOVER_OK,    // only notified with self recovery
    OT_SEMU_MIPITX_FPS_ERR_TOO_HIGH,      // fps too high
    OT_SEMU_MIPITX_FPS_ERR_TOO_LOW,       // fps too low
    OT_SEMU_MIPITX_FPS_ERR_INVALID
} ot_semu_mipitx_fps_err_type;

// vi err type
typedef enum {
    OT_SEMU_VI_ERR_RECOVER_OK, // only notified with self recovery
    OT_SEMU_VI_ERR_BUS_ACCESS,
    OT_SEMU_VI_ERR_IN_FRAME_SEQ, // input frame sequence error
    OT_SEMU_VI_ERR_FIFO_OVERFLOW,
    OT_SEMU_VI_ERR_RESOLUTION_CHANGE,
    OT_SEMU_VI_ERR_INVALID
} ot_semu_vi_err_type;

// vo err type
typedef enum {
    OT_SEMU_VO_ERR_RECOVER_OK, // only notified with self recovery
    OT_SEMU_VO_ERR_BUS_ACCESS,
    OT_SEMU_VO_ERR_NODE_HANDLE,
    OT_SEMU_VO_ERR_TASK_PROCESS,
    OT_SEMU_VO_ERR_ONLINE_FRAME,
    OT_SEMU_VO_ERR_LOW_DELAY,
    OT_SEMU_VO_ERR_LOW_BANWDTH,
    OT_SEMU_VO_ERR_INVALID
} ot_semu_vo_err_type;

// vproc err type
typedef enum {
    OT_SEMU_VPROC_ERR_RECOVER_OK, // only notified with self recovery
    OT_SEMU_VPROC_ERR_START,
    OT_SEMU_VPROC_ERR_NODE,
    OT_SEMU_VPROC_ERR_BUS,
    OT_SEMU_VPROC_ERR_LOSS_INT,
    OT_SEMU_VPROC_ERR_INVALID
} ot_semu_vproc_err_type;

// i2c err type
typedef enum {
    OT_SEMU_I2C_ERR_RECOVER_OK,
    OT_SEMU_I2C_ERR_ARBITRATION_LOST,
    OT_SEMU_I2C_ERR_ACK_BIT_UNMATCH,
    OT_SEMU_I2C_ERR_INVALID
} ot_semu_i2c_err_type;

// spi err type
typedef enum {
    OT_SEMU_SPI_ERR_RECOVER_OK,
    OT_SEMU_SPI_ERR_TX_TIMEOUT,
    OT_SEMU_SPI_ERR_RX_FIFO_OVER,
    OT_SEMU_SPI_ERR_INVALID
} ot_semu_spi_err_type;

// emmc err type
typedef enum {
    OT_SEMU_EMMC_ERR_BOOT_ACK,
    OT_SEMU_EMMC_ERR_RESP,
    OT_SEMU_EMMC_ERR_ADMA,
    OT_SEMU_EMMC_ERR_AUTO_CMD,
    OT_SEMU_EMMC_ERR_DATA_END_BIT,
    OT_SEMU_EMMC_ERR_DATA_CRC,
    OT_SEMU_EMMC_ERR_DATA_TOUT,
    OT_SEMU_EMMC_ERR_CMD_IDX,
    OT_SEMU_EMMC_ERR_CMD_END_BIT,
    OT_SEMU_EMMC_ERR_CMD_CRC,
    OT_SEMU_EMMC_ERR_CMD_TOUT,
    OT_SEMU_EMMC_ERR_INVALID
} ot_semu_emmc_err_type;

typedef enum {
    OT_SEMU_FMC_ERR_DMA,
    OT_SEMU_FMC_ERR_ECC_INVAL,
    OT_SEMU_FMC_ERR_OP_FAIL,
    OT_SEMU_FMC_ERR_INVALID
} ot_semu_fmc_err_type;

// acore stl err type
typedef enum {
    OT_SEMU_ACORE_STL_ERR_RECOVER_OK,
    OT_SEMU_ACORE_STL_ERR_STL_TEST,
    OT_SEMU_ACORE_STL_ERR_INVALID
} ot_semu_acore_stl_err_type;

// video delay err type
typedef enum {
    OT_SEMU_VID_DELAY_ERR_RECOVER_OK,
    OT_SEMU_VID_DELAY_ERR_EXPIRE, // exceed the max delay with debounce time
    OT_SEMU_VID_DELAY_ERR_INVALID
} ot_semu_video_delay_err_type;

// ddrc err type
typedef enum {
    OT_SEMU_DDR_ERR_RECOVER_OK,
    OT_SEMU_DDR_ERR_DATALINE_ACCESS, // data line accessing test error
    OT_SEMU_DDR_ERR_INVALID
} ot_semu_ddr_err_type;

/* registers err type */
typedef enum {
    OT_SEMU_REGS_ERR_VALUE,
    OT_SEMU_REGS_ERR_INVALID
} ot_semu_registers_err_type;

typedef enum {
    OT_SEMU_FRAME_ERR_USER_BUFFER_OVERFLOW,
    OT_SEMU_FRAME_ERR_KERNEL_BUFFER_OVERFLOW,
    OT_SEMU_FRAME_ERR_INVALID
} ot_semu_frame_err_type;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif