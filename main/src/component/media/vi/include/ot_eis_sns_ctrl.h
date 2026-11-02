/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#ifndef OT_EIS_SNS_CTRL_H
#define OT_EIS_SNS_CTRL_H

#include "ot_eis_type.h"
#include "ot_eis_vi_alg.h"
#include "ot_eis_vi_sns.h"
#include "ot_eis_vi_awb.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define OT_EIS_VI_SNS_SAVE_INFO_MAX 2
typedef struct {
    ot_bool      init;                   /* OT_TRUE: Sensor init */
    ot_bool      sync_init;              /* OT_TRUE: Sync Reg init */
    ot_u8        img_mode;
    ot_u8        hdr;                 /* OT_TRUE: HDR enable */
    ot_eis_wdr_mode  wdr_mode;

    ot_eis_vi_sns_regs_info regs_info[OT_EIS_VI_SNS_SAVE_INFO_MAX]; /* [0]: Sensor reg info of cur-frame;
                                                                       [1]: Sensor reg info of pre-frame ; */

    ot_u32      fl[OT_EIS_VI_SNS_SAVE_INFO_MAX];          /* [0]: FullLines of cur-frame;
                                                             [1]: Pre FullLines of pre-frame */
    ot_u32      fl_std;                    /* FullLines std */
    ot_u32      wdr_int_time[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32      sns_wb_gain[OT_EIS_VI_BAYER_CHN_NUM];
} ot_eis_sns_state;

typedef enum {
     OT_EIS_SNS_NORMAL      = 0,
     OT_EIS_SNS_MIRROR      = 1,
     OT_EIS_SNS_FLIP        = 2,
     OT_EIS_SNS_MIRROR_FLIP = 3,
     OT_EIS_SNS_BUTT
} ot_eis_sns_mirrorflip_type;

typedef struct {
    ot_eis_vi_sns_commbus com_bus;
    unsigned char bus_addr;
} ot_eis_sns_bus_info;

typedef struct {
    ot_bool blc_clamp_en; /* OT_TRUE: sensor black level correction enable */
} ot_eis_sns_blc_clamp;

typedef struct {
    ot_bool is_ir_mode;
    ot_u32 ae_comp;
    ot_u32 exp_time;
    ot_float int_time_accu;
    ot_u32 a_gain;
    ot_float again_accu;
    ot_u32 d_gain;
    ot_float dgain_accu;
    ot_u32 ispd_gain;
    ot_u32 exposure;
    ot_u32 init_iso;
    ot_u32 lines_per500ms;
    ot_u32 piris_fno;
    ot_u16 wb_r_gain;
    ot_u16 wb_g_gain;
    ot_u16 wb_b_gain;
    ot_u16 sample_r_gain;
    ot_u16 sample_b_gain;
    ot_u16 init_ccm[OT_EIS_VI_CCM_MATRIX_SIZE];
    ot_bool ae_route_ex_valid;
    ot_bool quick_start_en;
    ot_eis_vi_ae_route ae_route;
    ot_eis_vi_ae_route_ex ae_route_ex;
    ot_eis_vi_ae_route ae_route_sf;
    ot_eis_vi_ae_route_ex ae_route_sf_ex;
    ot_u32 ae_stat_pos;
} ot_eis_vi_init_attr;

typedef struct {
    ot_u16 addr;
    ot_u16 data;
} ot_eis_sns_reg_data;

typedef struct {
    ot_eis_sns_reg_data *reg_data;
    ot_s32 data_len;
    ot_bool write_flag;
} ot_eis_sns_reg_table;

typedef struct {
    ot_s32  (*pfn_sns_register)(ot_vi_pipe_handle pipe_hdl);
    ot_s32  (*pfn_sns_unregister)(ot_vi_pipe_handle pipe_hdl);
    ot_s32  (*pfn_sns_set_bus_info)(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_bus_info *sns_bus_info);
    ot_void (*pfn_sns_standby)(ot_vi_pipe_handle pipe_hdl);
    ot_void (*pfn_sns_restart)(ot_vi_pipe_handle pipe_hdl);
    ot_void (*pfn_sns_set_mirror_flip)(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_mirrorflip_type sns_mirror_flip);
    ot_void (*pfn_sns_set_blc_clamp)(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_blc_clamp sns_blc_clamp);
    ot_s32  (*pfn_sns_set_init)(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_init_attr *init_attr);
    ot_s32  (*pfn_sns_set_alg_default)(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_sns_alg_default *sns_alg_def);
    ot_s32  (*pfn_sns_set_awb_default)(ot_vi_pipe_handle pipe_hdl, ot_eis_vi_awb_sensor_default *sns_awb_def);
    ot_s32  (*pfn_sns_set_reg_table)(ot_vi_pipe_handle pipe_hdl, ot_eis_sns_reg_table *reg_table, ot_s32 table_cnt);
} ot_eis_sns_obj;

#define sns_err_trace(fmt...) \
    do { \
        printf("[Func]:%s [Line]:%d [Info]: ", __FUNCTION__, __LINE__); \
        printf(fmt); \
    } while (0)


#define sns_check_pointer_return(ptr) \
    do { \
        if ((ptr) == OT_NULL) { \
            sns_err_trace("Null Pointer!\n"); \
            return OT_ERR_EIS_VI_NULL_PTR; \
        } \
    } while (0)

#define sns_check_pointer_void_return(ptr) \
    do { \
        if ((ptr) == OT_NULL) { \
            sns_err_trace("Null Pointer!\n"); \
            return; \
        } \
    } while (0)

#define sns_free(ptr) \
    do { \
        if ((ptr) != OT_NULL) { \
            free(ptr); \
            (ptr) = OT_NULL; \
        } \
    } while (0)

#define sns_div_0_to_1(a)   (((a) == 0) ? 1 : (a))
#define sns_div_0_to_1_float(a) ((((a) < 1E-10) && ((a) > (-1E-10))) ? 1 : (a))

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_SNS_CTRL_H */
