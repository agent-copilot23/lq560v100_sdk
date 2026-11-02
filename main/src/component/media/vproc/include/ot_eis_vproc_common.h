#ifndef __OT_EIS_VPROC_COMMON_H__
#define __OT_EIS_VPROC_COMMON_H__

#include "ot_eis_type.h"
#include "ot_eis_video.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OT_EIS_VPROC_LIB_VER "1.0.1.0"

#define OT_EIS_VPROC_PIPE_MIN_WIDTH (120)
#define OT_EIS_VPROC_PIPE_MAX_WIDTH (4096)
#define OT_EIS_VPROC_PIPE_MIN_HEIGHT (120)
#define OT_EIS_VPROC_PIPE_MAX_HEIGHT (4096)

#define OT_EIS_VPROC_GRP_MIN_WIDTH (120)
#define OT_EIS_VPROC_GRP_MAX_WIDTH (4096)
#define OT_EIS_VPROC_GRP_MIN_HEIGHT (120)
#define OT_EIS_VPROC_GRP_MAX_HEIGHT (4096)

#define OT_EIS_VPROC_CHN_MIN_WIDTH (120)
#define OT_EIS_VPROC_CHN_MAX_WIDTH (4096)
#define OT_EIS_VPROC_CHN_MIN_HEIGHT (120)
#define OT_EIS_VPROC_CHN_MAX_HEIGHT (4096)

#define OT_EIS_VPROC_WIDTH_ALIGN (2)
#define OT_EIS_VPROC_HEIGHT_ALIGN (2)

#define OT_EIS_VPROC_GRP_MAX_NUM (8)
#define OT_EIS_VPROC_GRP_PIPE_MAX_NUM (4)
#define OT_EIS_VPROC_GRP_CHN_MAX_NUM (4)

#define OT_EIS_VPROC_PIPE_MAX_NUM (OT_EIS_VPROC_GRP_MAX_NUM * OT_EIS_VPROC_GRP_PIPE_MAX_NUM)
#define OT_EIS_VPROC_CHN_MAX_NUM (OT_EIS_VPROC_GRP_MAX_NUM * OT_EIS_VPROC_GRP_CHN_MAX_NUM)

#define OT_EIS_VPROC_MOSAIC_MAX_NUM (12)
#define OT_EIS_VPROC_LINE_MAX_NUM (12)
#define OT_EIS_VPROC_COVER_MAX_NUM (8)
#define OT_EIS_VPROC_RECT_MAX_NUM (12)
#define OT_EIS_VPROC_OSD_MAX_NUM (8)

//#define OT_EIS_VPROC_RGN_MAX_NUM (256)
#define OT_EIS_VPROC_RGN_MAX_NUM (OT_EIS_VPROC_MOSAIC_MAX_NUM + OT_EIS_VPROC_LINE_MAX_NUM + OT_EIS_VPROC_COVER_MAX_NUM + OT_EIS_VPROC_RECT_MAX_NUM + OT_EIS_VPROC_OSD_MAX_NUM)

#define OT_EIS_VPROC_LINE_POINT_NUM (2)
#define OT_EIS_VPROC_COVER_POINT_NUM (4)
#define OT_EIS_VPROC_OSD_LUT_NUM (2)
#define OT_EIS_VPROC_LUMA_MAX_NUM (64)
#define OT_EIS_VPROC_LDC_SRC_LENS_COEF_NUM (9)
#define OT_EIS_VPROC_PMF_MATRIX_MAX_NUM (9)

typedef ot_s32 ot_vproc_grp_id;
typedef ot_s32 ot_vproc_chn_id;

typedef enum {
    OT_EIS_VPROC_WORK_MODE_PIPE,
    OT_EIS_VPROC_WORK_MODE_USER,
    OT_EIS_VPROC_WORK_MODE_BUTT
} ot_eis_vproc_work_mode;

typedef struct {
    ot_u32              max_width;
    ot_u32              max_height;
    ot_eis_img_pixel_format pixel_fmt;
} ot_eis_vproc_grp_attr;

typedef struct {
    ot_eis_img_attr image_attr;
    ot_eis_vproc_work_mode mode;
    ot_eis_frame_rate_ctrl frc;
    ot_u32 frame_queue_depth;
    ot_eis_handle pool_handle;
} ot_eis_vproc_chn_attr;

typedef struct {
    ot_eis_vproc_work_mode mode;
    ot_eis_frame_rate_ctrl frc;
    ot_eis_img_attr image_attr;
} ot_eis_vproc_pipe_attr;

typedef struct {
    ot_bool enable;
    ot_eis_coord_type crop_type;
    ot_eis_img_rect crop_rect;
} ot_eis_vproc_crop_param;

typedef struct {
    ot_bool enable;
} ot_eis_vproc_mirror_param;

typedef struct {
    ot_bool enable;
} ot_eis_vproc_flip_param;


typedef enum {
    OT_EIS_SCALE_COEF_NORMAL = 0,
    OT_EIS_SCALE_COEF_BILINEAR = 1,
    OT_EIS_SCALE_COEF_BUTT
} ot_eis_scale_coef_type;

typedef struct {
    ot_eis_scale_coef_type scale_coef_type;
} ot_eis_vproc_scale_coef_param;


typedef enum {
    OT_EIS_MOSAIC_BLK_SIZE_16, /* MOSAIC size 16*16  */
    OT_EIS_MOSAIC_BLK_SIZE_32, /* MOSAIC size 32*32  */
    OT_EIS_MOSAIC_BLK_SIZE_64, /* MOSAIC size 64*64  */
    OT_EIS_MOSAIC_BLK_SIZE_BUTT
} ot_eis_mosaic_size;

typedef struct {
    ot_eis_img_rect rect;
    ot_eis_mosaic_size blk_size;
} ot_eis_mosaic;

typedef struct {
    ot_bool enable;
    ot_u8 num;
    ot_eis_mosaic *mosaic;
} ot_eis_vproc_mosaic_param;

typedef struct {
    ot_u8 thickness;
    ot_u32 color;
    ot_eis_img_point point[OT_EIS_VPROC_LINE_POINT_NUM];
} ot_eis_line;


typedef struct {
    ot_bool enable;
    ot_u32 num;
    ot_eis_line *line;
} ot_eis_vproc_line_param;


typedef struct {
    ot_bool is_hollow;
    ot_u16 opacity;
    ot_u32 thick;
    ot_u32 color;
    ot_eis_img_point point[OT_EIS_VPROC_COVER_POINT_NUM];
} ot_eis_cover;

typedef struct {
    ot_bool enable;
    ot_u8 num;
    ot_eis_cover *cover;
} ot_eis_vproc_cover_param;

typedef struct {
    ot_u8 thick;
    ot_bool is_corner;
    ot_u16 hor_len;
    ot_u16 ver_len;
    ot_u32 color;
    ot_eis_img_rect rect;
} ot_eis_vproc_rect;

typedef struct {
    ot_bool enable;
    ot_u8 num;
    ot_eis_vproc_rect *rect;
} ot_eis_vproc_rect_param;

typedef enum{
    OT_FULL_RANGE,
    OT_LIMITED_RANGE,
    OT_DATA_RANGE_BUTT
} ot_eis_color_range;

typedef struct {
    ot_eis_img_rect osd_rgn;
    ot_u32 color;
    ot_u32 alpha_lut[OT_EIS_VPROC_OSD_LUT_NUM];
    ot_eis_img_frame *osd_frame;
} ot_eis_osd_attr;

typedef struct {
    ot_bool enable;
    ot_u8 num;
    ot_eis_osd_attr *osd;
} ot_eis_vproc_osd_param;

typedef struct {
    ot_bool enable;
    ot_u32 num;
    ot_eis_img_rect *luma;
} ot_eis_vproc_luma_param;

typedef struct {
    ot_u32 num;
    ot_u64 luma_stats[OT_EIS_VPROC_LUMA_MAX_NUM];
} ot_eis_vproc_luma_results;


typedef enum  {
    OT_EIS_RTT_0   = 0,
    OT_EIS_RTT_90  = 1,
    OT_EIS_RTT_180 = 2,
    OT_EIS_RTT_270 = 3,
    OT_EIS_RTT_BUTT
} ot_eis_rotation;

typedef struct {
    ot_bool enable;
    ot_eis_rotation angle;
} ot_eis_vproc_chn_rotation;

typedef enum{
    OT_EIS_WORK_2D_LUT_POLAT = 0,
    OT_EIS_WORK_FORMULA_DERIVAT = 1,
    OT_EIS_WORK_BUTT
} ot_eis_gdc_work_mode;

typedef enum {
    OT_EIS_GDC_LDC_SEL,
    OT_EIS_GDC_FISHEYE_SEL,
} ot_eis_modul_sel;


typedef enum {
    OT_EIS_LUT_CELL_SIZE_16,
    OT_EIS_LUT_CELL_SIZE_32,
    OT_EIS_LUT_CELL_SIZE_64,
    OT_EIS_LUT_CELL_SIZE_128,
    OT_EIS_LUT_CELL_SIZE_256,
    OT_EIS_LUT_CELL_SIZE_BUTT
} ot_eis_vproc_lut_cell_size;

typedef struct {
    ot_eis_modul_sel ldc_or_fisheye; // 0:ldc使能，1:fisheye使能
    ot_eis_gdc_work_mode gdc_mode;
    ot_eis_img_size src;
    ot_eis_img_size dst;
    ot_eis_img_size mid;
    ot_eis_vproc_lut_cell_size cell_size;
} ot_eis_vproc_gdc_cfg;

typedef struct {
    ot_bool enable;
    ot_eis_vproc_gdc_cfg *cfg;
} ot_eis_vproc_gdc_param;

typedef enum {
    OT_EIS_FISHEYE_MODE_360,
    OT_EIS_FISHEYE_MODE_180,
    OT_EIS_FISHEYE_MODE_NORMAL,
    OT_EIS_FISHEYE_MODE_BUTT
} ot_eis_vproc_fisheye_mode;

typedef enum {
    OT_LMF_DATA_TYPE_CALIB,
    OT_LMF_DATA_TYPE_CONVERT,
    OT_LMF_DATA_TYPE_BUTT
} ot_eis_vproc_lmf_type;

typedef struct {
    ot_eis_vproc_lmf_type type;
    ot_u8 *lut_data;
    ot_u32 len;
} ot_eis_lmf_data;

typedef struct {
    ot_bool use_roixy_en;
    ot_u8 trap_direct;
    ot_u16 fov_in;
    ot_u16 trap_gain;
    ot_u16 pan;
    ot_u16 tilt;
    ot_u16 roll;
    ot_u32 radius_out;
    ot_eis_lmf_data fish_lut;
    ot_eis_img_point offset;
    ot_eis_img_point roi_center;
    ot_eis_img_size zoom;
} ot_eis_vproc_fisheye_normal;

typedef struct {
    ot_u8 trap_direct;
    ot_u16 fov_in;
    ot_u16 trap_gain;
    ot_u16 pan;
    ot_u16 tilt;
    ot_u16 roll;
    ot_s16 fan;
    ot_u32 radius_out;
    ot_eis_lmf_data fish_lut;
    ot_eis_img_point offset;
    ot_eis_img_size zoom;
} ot_eis_vproc_fisheye_180;

typedef struct {
    ot_u8 flip_enable;
    ot_u16 pan;
    ot_u16 tilt;
    ot_u32 radius_in;
    ot_u32 radius_out;
    ot_eis_lmf_data fish_lut;
    ot_eis_img_point offset;
    ot_eis_img_size zoom;
} ot_eis_vproc_fisheye_360;

typedef union {
    ot_eis_vproc_fisheye_normal fisheye_noraml;
    ot_eis_vproc_fisheye_180 fisheye_180;
    ot_eis_vproc_fisheye_360 fisheye_360;
} ot_eis_vproc_fisheye_attr;

typedef struct {
    ot_eis_vproc_fisheye_mode fisheye_mode;
    ot_eis_vproc_fisheye_attr fisheye_attr;
} ot_eis_vproc_fisheye_cfg;

typedef struct {
    ot_bool enable;
    ot_eis_vproc_fisheye_cfg *fisheye;
} ot_eis_vproc_fisheye_param;

typedef enum
{
    OT_EIS_VPROC_LDC_CALIB_FREE = 0,
    OT_EIS_VPROC_LDC_CALIB_BASE,
    OT_EIS_VPROC_LDC_MODE_BUTT
} ot_eis_vproc_ldc_mode;

typedef struct {
    ot_bool keep_aspect;
    ot_s32 x_ratio;
    ot_s32 y_ratio;
    ot_s32 xy_ratio;
} ot_eis_ldc_ratio_type;

typedef struct {
    ot_s32 distortion_ratio;
    ot_eis_ldc_ratio_type ratio;
    ot_eis_img_point center_offset;
} ot_eis_ldc_calib_free;

typedef struct {
    ot_s32 distortion_ratio;
    ot_eis_ldc_ratio_type ratio;
    ot_eis_lmf_data ldc_lut;
    ot_eis_img_point center_offset;
} ot_eis_ldc_lmf;

typedef struct {
    ot_eis_ldc_ratio_type ratio;
    ot_eis_lmf_data ldc_lut;
} ot_eis_ldc_calib_base;

typedef struct {
    ot_eis_ldc_ratio_type ratio;
    ot_s32 focal_len_x;
    ot_s32 focal_len_y;
    ot_eis_img_point coord_shift;
    ot_s32 src_calibration_ratio[OT_EIS_VPROC_LDC_SRC_LENS_COEF_NUM];
    ot_s32 src_calibration_ratio_next[OT_EIS_VPROC_LDC_SRC_LENS_COEF_NUM];
    ot_u32 coef_intp_ratio;
} ot_eis_ldc_v2_attr;


typedef union {
    ot_eis_ldc_calib_free free;
    ot_eis_ldc_v2_attr base;
} ot_eis_ldc_attr;

typedef struct {
    ot_eis_vproc_ldc_mode ldc_mode;
    ot_eis_ldc_attr ldc_attr;
} ot_eis_ldc_cfg;

typedef struct {
    ot_bool enable;
    ot_eis_ldc_cfg *ldc;
} ot_eis_vproc_ldc_param;

typedef enum {
    OT_PMF_WORK_MODE_SPREAD,
    OT_PMF_WORK_MODE_PMF,
    OT_PMF_WORK_MODE_ROTATION,
    OT_PMF_WORK_MODE_BUTT
} ot_eis_pmf_work_type;

typedef struct {
    ot_bool pmf_overstep_en;
    ot_s32 spread_coef;
} ot_eis_pmf_work_spread;

typedef struct {
    ot_bool pmf_overstep_en;
    ot_s64 matrix[OT_EIS_VPROC_PMF_MATRIX_MAX_NUM];
} ot_eis_pmf_work_pmf;

typedef enum {
    OT_RTT_VIEW_MODE_FULL,
    OT_RTT_VIEW_MODE_TYPICAL,
    OT_RTT_VIEW_MODE_INSIDE,
    OT_RTT_VIEW_MODE_BUTT
} ot_eis_rtt_view_type;

typedef struct {
    ot_bool enable;
    ot_eis_rtt_view_type view_type;
    ot_s32 angle;
    ot_eis_img_point center_offset;
} ot_eis_pmf_rotation;

typedef union {
    ot_eis_pmf_work_spread spread;
    ot_eis_pmf_work_pmf pmf_task;
    ot_eis_pmf_rotation rotaion;
} ot_eis_pmf_attr;

typedef struct {
    ot_eis_pmf_work_type mode;
    ot_eis_pmf_attr attr;
} ot_eis_pmf_cfg;

typedef struct {
    ot_bool enable;
    ot_eis_pmf_cfg *pmf;
} ot_eis_vproc_pmf_param;

typedef struct {
    ot_u32 xylut_len;
    ot_phys_addr gdc_2dlut_data;
} ot_eis_vproc_gdc_xylut;

typedef struct {
    ot_bool enable;
    ot_eis_vproc_gdc_xylut *gdc_lut;
} ot_eis_vproc_xylut_param;

typedef struct {
    ot_u32 try_acquire_cnt;
    ot_u32 acquire_succ_cnt;
    ot_u32 try_release_cnt;
    ot_u32 release_succ_cnt;
} ot_eis_vproc_chn_status;

#ifdef __cplusplus
}
#endif

#endif