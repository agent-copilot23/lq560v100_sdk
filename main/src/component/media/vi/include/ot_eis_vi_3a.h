#ifndef OT_EIS_VI_3A_H
#define OT_EIS_VI_3A_H

#include "ot_eis_vi_alg.h"
#include "ot_eis_vi_sns.h"
#include "ot_eis_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define OT_EIS_VI_3A_MAX_PIPE_NUM 15

typedef enum {
    OT_EIS_VI_WDR_MODE_SET = 8000,
    OT_EIS_VI_PROC_WRITE,

    OT_EIS_VI_AE_FPS_BASE_SET,
    OT_EIS_VI_AE_BLC_SET,
    OT_EIS_VI_AE_RC_SET,
    OT_EIS_VI_AE_BAYER_FORMAT_SET,
    OT_EIS_VI_AE_INIT_INFO_GET,

    OT_EIS_VI_AWB_ISO_SET,  /* set iso, change saturation when iso change */
    OT_EIS_VI_CHANGE_IMAGE_MODE_SET,
    OT_EIS_VI_UPDATE_INFO_GET,
    OT_EIS_VI_FRAMEINFO_GET,
    OT_EIS_VI_ATTACHINFO_GET,
    OT_EIS_VI_COLORGAMUTINFO_GET,
    OT_EIS_VI_AWB_INTTIME_SET,
    OT_EIS_VI_BAS_MODE_SET,
    OT_EIS_VI_PROTRIGGER_SET,
    OT_EIS_VI_AWB_PIRIS_SET,
    OT_EIS_VI_AWB_SNAP_MODE_SET,
    OT_EIS_VI_AWB_ZONE_ROW_SET,
    OT_EIS_VI_AWB_ZONE_COL_SET,
    OT_EIS_VI_AWB_ZONE_BIN_SET,
    OT_EIS_VI_AWB_ERR_GET,
    OT_EIS_VI_AWB_PREWB_REVERSE_GET,

    OT_EIS_VI_DEHAZE_DEBUG_ATTR_SET,
    OT_EIS_VI_AE_DEBUG_ATTR_SET,
    OT_EIS_VI_CTRL_CMD_BUTT,
} ot_eis_vi_alg_ctrl_cmd;

typedef struct {
    ot_char *proc_buff;
    ot_u32   buff_len;
    size_t   write_len;   /* the len count should contain '\0'. */
} ot_eis_vi_alg_ctrl_proc_write;

typedef struct {
    ot_bool stitch_enable;
    ot_bool main_pipe;
    ot_u8   stitch_pipe_num;
    ot_s8   stitch_bind_id[OT_EIS_VI_MAX_STITCH_NUM];
} ot_eis_vi_stitch_attr;

/* AE */
/* the init param of ae alg */
typedef struct {
    ot_s32 sns_id;
    ot_u8  wdr_mode;
    ot_u8  hdr_mode;
    ot_u16 black_level;
    ot_float fps;
    ot_eis_vi_bayer_format bayer;
    ot_eis_vi_stitch_attr stitch_attr;

    ot_s32 reserved;
} ot_eis_vi_ae_param;

/* the statistics of ae alg */
typedef struct {
    ot_u32  pixel_count[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32  pixel_weight[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32  histogram_mem_array[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_HIST_NUM];
    ot_u32  histogram_mem_array_ir[OT_EIS_VI_HIST_NUM]; /* not support */
} ot_eis_vi_ae_fe_hist_stat;

typedef struct {
    ot_u16  global_avg_r[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u16  global_avg_gr[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u16  global_avg_gb[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u16  global_avg_b[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u16  global_avg_ir; /* not support */
} ot_eis_vi_ae_fe_global_stat;

typedef struct {
    ot_u16  zone_avg[OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN][OT_EIS_VI_BAYER_CHN_NUM];
    ot_u16  zone_avg_ir[OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN]; /* not support  */
} ot_eis_vi_ae_fe_zone_stat;

typedef struct {
    ot_u16  zone_avg[OT_EIS_VI_MAX_STITCH_NUM][OT_EIS_VI_WDR_MAX_FRAME_NUM][OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN]\
        [OT_EIS_VI_BAYER_CHN_NUM];
} ot_eis_vi_ae_fe_stitch_stat;

typedef struct {
    ot_u32  pixel_count;
    ot_u32  pixel_weight;
    ot_u32  histogram_mem_array[OT_EIS_VI_HIST_NUM];
    ot_u32  estimate_histogram_mem_array[OT_EIS_VI_HIST_NUM];
} ot_eis_vi_ae_be_hist_stat;

typedef struct {
    ot_u16  global_avg_r;
    ot_u16  global_avg_gr;
    ot_u16  global_avg_gb;
    ot_u16  global_avg_b;
} ot_eis_vi_ae_be_global_stat;

typedef struct {
    ot_u16  zone_avg[OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN][OT_EIS_VI_BAYER_CHN_NUM];
} ot_eis_vi_ae_be_zone_stat;

typedef struct {
    ot_u16  zone_avg[OT_EIS_VI_MAX_STITCH_NUM][OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN][OT_EIS_VI_BAYER_CHN_NUM];
} ot_eis_vi_ae_be_stitch_stat;

typedef struct {
    ot_u32  frame_cnt;    /* the counting of frame */
    ot_u64  frame_pts;
    ot_u16  frame_width;
    ot_u16  frame_height;
    ot_eis_vi_smart_info smart_info;

    ot_eis_vi_ae_fe_hist_stat     *fe_ae_stat1;
    ot_eis_vi_ae_fe_global_stat   *fe_ae_stat2;
    ot_eis_vi_ae_fe_zone_stat     *fe_ae_stat3;
    ot_eis_vi_ae_fe_stitch_stat   *fe_ae_sti_stat;
    ot_eis_vi_ae_be_hist_stat     *be_ae_stat1;
    ot_eis_vi_ae_be_global_stat   *be_ae_stat2;
    ot_eis_vi_ae_be_zone_stat     *be_ae_stat3;
    ot_eis_vi_ae_be_stitch_stat   *be_ae_sti_stat;
} ot_eis_vi_ae_input_info;

typedef struct {
    ot_bool change;

    ot_bool hist_adjust;
    ot_u8 ae_be_sel;
    ot_u8 four_plane_mode;
    ot_u8 hist_offset_x;
    ot_u8 hist_offset_y;
    ot_u8 hist_skip_x;
    ot_u8 hist_skip_y;

    ot_bool mode_update;
    ot_u8 hist_mode;
    ot_u8 aver_mode;
    ot_u8 max_gain_mode;

    ot_bool wight_table_update;
    ot_u8 weight_table[OT_EIS_VI_3A_MAX_PIPE_NUM][OT_EIS_VI_AE_ZONE_ROW][OT_EIS_VI_AE_ZONE_COLUMN];
} ot_eis_vi_ae_stat_attr;

/* the final calculate of ae alg */
#define AE_INT_TIME_NUM          4
typedef struct {
    ot_u32  int_time[AE_INT_TIME_NUM];
    ot_u32  isp_dgain;
    ot_u32  again;
    ot_u32  dgain;
    ot_u32  iso;
    ot_u32  isp_dgain_sf;
    ot_u32  again_sf;
    ot_u32  dgain_sf;
    ot_u32  iso_sf;
    ot_u8   ae_run_interval;

    ot_bool piris_valid;
    ot_s32  piris_pos;
    ot_u32  piris_gain;
    ot_u32  sns_lhcg_exp_ratio;

    ot_eis_vi_fswdr_mode fswdr_mode;
    ot_u32  wdr_gain[OT_EIS_VI_WDR_MAX_FRAME_NUM];
    ot_u32  hmax_times; /* unit is ns */
    ot_u32  vmax; /* unit is line */

    ot_eis_vi_ae_stat_attr stat_attr;
    ot_eis_vi_dcf_update_info update_info;
} ot_eis_vi_ae_result;

typedef struct {
    ot_u32 isp_dgain;
    ot_u32 iso;
} ot_eis_vi_ae_init_info;

typedef struct {
    ot_s32 (*pfn_init)(ot_vi_pipe_id vi_pipe, const ot_eis_vi_ae_param *ae_param);
    ot_s32 (*pfn_process)(ot_vi_pipe_id vi_pipe, const ot_eis_vi_ae_input_info *ae_info,
                         ot_eis_vi_ae_result *ae_result, ot_s32 reserved);
    ot_s32 (*pfn_cmd)(ot_vi_pipe_id vi_pipe, ot_u32 cmd, ot_void *value);
    ot_s32 (*pfn_deinit)(ot_vi_pipe_id vi_pipe);
} ot_eis_vi_ae_register_func;

/* AWB */
typedef enum {
    OT_EIS_VI_AWB_CCM_CONFIG_SET,
    OT_EIS_VI_AWB_CCM_CONFIG_GET,

    OT_EIS_VI_AWB_DEBUG_ATTR_SET,
    OT_EIS_VI_AWB_CTRL_BUTT,
} ot_eis_vi_awb_ctrl_cmd;

typedef struct {
    ot_bool awb_bypass_en;
    ot_bool manual_sat_en;
    ot_bool manual_temp_en;

    ot_u32  manual_sat_value;
    ot_u32  manual_temp_value;
    ot_u16  ccm_speed;

    ot_u16  high_ccm[OT_EIS_VI_CCM_MATRIX_SIZE];
    ot_u16  low_ccm[OT_EIS_VI_CCM_MATRIX_SIZE];
    ot_u16  high_color_temp;
    ot_u16  low_color_temp;
} ot_eis_vi_awb_ccm_config;

/* the init param of awb alg */
typedef struct {
    ot_s32 sns_id;
    ot_u8 wdr_mode;
    ot_u8 awb_zone_row;
    ot_u8 awb_zone_col;
    ot_u8 awb_zone_bin;
    ot_eis_vi_stitch_attr stitch_attr;
    ot_u16 awb_width;
    ot_u16 awb_height;
    ot_u32 init_iso;
    ot_s8 reserved;
} ot_eis_vi_awb_param;

/* the statistics of awb alg */
typedef struct {
    ot_u16  metering_awb_avg_r;
    ot_u16  metering_awb_avg_g;
    ot_u16  metering_awb_avg_b;
    ot_u16  metering_awb_count_all;
} ot_eis_vi_awb_stat_1;

typedef struct {
    ot_u16  metering_mem_array_avg_r[OT_EIS_VI_AWB_ZONE_NUM];
    ot_u16  metering_mem_array_avg_g[OT_EIS_VI_AWB_ZONE_NUM];
    ot_u16  metering_mem_array_avg_b[OT_EIS_VI_AWB_ZONE_NUM];
    ot_u16  metering_mem_array_count_all[OT_EIS_VI_AWB_ZONE_NUM];
} ot_eis_vi_awb_stat_2;

typedef struct {
    ot_u16  zone_col;
    ot_u16  zone_row;
    ot_u16  zone_bin;
    ot_u16  metering_mem_array_avg_r[OT_EIS_VI_AWB_ZONE_STITCH_MAX];
    ot_u16  metering_mem_array_avg_g[OT_EIS_VI_AWB_ZONE_STITCH_MAX];
    ot_u16  metering_mem_array_avg_b[OT_EIS_VI_AWB_ZONE_STITCH_MAX];
    ot_u16  metering_mem_array_count_all[OT_EIS_VI_AWB_ZONE_STITCH_MAX];
} ot_eis_vi_awb_stitch_stat;

typedef struct {
    ot_u16 *zone_avg_r;
    ot_u16 *zone_avg_g;
    ot_u16 *zone_avg_b;
    ot_u16 *zone_count;
} ot_eis_vi_awb_stat_result;

typedef struct {
    ot_u8 col_start;
    ot_u8 col_stop;
    ot_u8 row_start;
    ot_u8 row_stop;
} ot_eis_vi_awb_overlap_zone;

typedef struct {
    ot_u8  stitch_pipe_num;
    ot_eis_vi_awb_overlap_zone left_zone_range[OT_EIS_VI_MAX_STITCH_NUM];
    ot_eis_vi_awb_overlap_zone right_zone_range[OT_EIS_VI_MAX_STITCH_NUM];
} ot_eis_vi_awb_overlap_all_zone;

typedef struct {
    ot_u32  frame_cnt;

    ot_eis_vi_awb_stat_1 *awb_stat1;
    ot_eis_vi_awb_stat_result awb_stat2;
    ot_u8  awb_gain_switch;
    ot_u8  awb_stat_switch;
    ot_bool wb_gain_in_sns;
    ot_u32 wdr_wb_gain[OT_EIS_VI_BAYER_CHN_NUM];
    ot_eis_vi_awb_overlap_all_zone overlap_all_zone;
} ot_eis_vi_awb_input_info;

/* the statistics's attr of awb alg */
typedef struct {
    ot_bool stat_cfg_update;

    ot_u16  metering_white_level_awb;
    ot_u16  metering_black_level_awb;
    ot_u16  metering_cr_ref_max_awb;
    ot_u16  metering_cb_ref_max_awb;
    ot_u16  metering_cr_ref_min_awb;
    ot_u16  metering_cb_ref_min_awb;
} ot_eis_vi_awb_raw_stat_attr;

typedef struct {
    ot_u32 left_overlap_zone_awb[OT_EIS_VI_MAX_STITCH_NUM][OT_EIS_VI_BAYER_CHN_NUM];
    ot_u32 right_overlap_zone_awb[OT_EIS_VI_MAX_STITCH_NUM][OT_EIS_VI_BAYER_CHN_NUM];
} ot_eis_vi_awb_overlap_result;

/* the final calculate of awb alg */
typedef struct {
    ot_u32  white_balance_gain[OT_EIS_VI_BAYER_CHN_NUM];
    ot_u16  color_matrix[OT_EIS_VI_CCM_MATRIX_SIZE];
    ot_u32  color_temp;
    ot_u8   saturation;
    ot_eis_vi_awb_raw_stat_attr raw_stat_attr;
    ot_eis_vi_awb_overlap_result overlap_result;
} ot_eis_vi_awb_result;

typedef struct {
    ot_s32 (*pfn_init)(ot_vi_pipe_id vi_pipe, const ot_eis_vi_awb_param *awb_param, ot_eis_vi_awb_result *awb_result);
    ot_s32 (*pfn_process)(ot_vi_pipe_id vi_pipe, const ot_eis_vi_awb_input_info *awb_info, ot_eis_vi_awb_result *awb_result,
                          ot_s32 reserved);
    ot_s32 (*pfn_cmd)(ot_vi_pipe_id vi_pipe, ot_u32 cmd, ot_void *value);
    ot_s32 (*pfn_deinit)(ot_vi_pipe_id vi_pipe);
} ot_eis_vi_awb_register_func;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_COMM_3A_H */
