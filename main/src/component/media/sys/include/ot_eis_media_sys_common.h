#ifndef OT_EIS_MEDIA_SYS_COMMON_H
#define OT_EIS_MEDIA_SYS_COMMON_H

#include "ot_eis_type.h"
#include "ot_eis_mod.h"
#include "ot_eis_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

#define OT_COMM_EIS_VI_MAX_PIPE_NUM 15

typedef struct {
    ot_eis_module_id  mod_id;
    ot_s32  level;
} ot_eis_media_log_level;

typedef enum {
    OT_EIS_VI_OFFLINE_VPROC_OFFLINE = 0,
    OT_EIS_VI_OFFLINE_VPROC_ONLINE,
    OT_EIS_VI_ONLINE_VPROC_OFFLINE,
    OT_EIS_VI_ONLINE_VPROC_ONLINE,
    OT_EIS_VI_PARALLEL_VPROC_OFFLINE,
    OT_EIS_VI_PARALLEL_VPROC_PARALLEL,
    OT_EIS_VI_VPROC_MODE_BUTT
} ot_eis_vi_vproc_mode_type;

typedef struct {
    ot_eis_vi_vproc_mode_type mode[OT_COMM_EIS_VI_MAX_PIPE_NUM];
} ot_eis_vi_vproc_mode;

typedef enum {
    OT_EIS_3DNR_POS_VI = 0,
    OT_EIS_3DNR_POS_VPROC,
    OT_EIS_3DNR_POS_BUTT
} ot_eis_3dnr_pos;

#define OT_EIS_MEDIA_LOG_EMERG      0   /* system is unusable                   */
#define OT_EIS_MEDIA_LOG_ALERT      1   /* action must be taken immediately     */
#define OT_EIS_MEDIA_LOG_CRIT       2   /* critical conditions                  */
#define OT_EIS_MEDIA_LOG_ERR        3   /* error conditions                     */
#define OT_EIS_MEDIA_LOG_WARN       4   /* warning conditions                   */
#define OT_EIS_MEDIA_LOG_NOTICE     5   /* normal but significant condition     */
#define OT_EIS_MEDIA_LOG_INFO       6   /* informational                        */
#define OT_EIS_MEDIA_LOG_DEBUG      7   /* debug-level messages                 */

#define OT_ERR_MEDIA_SYS_DEFINE(errno)    OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_MEDIASYS, errno)

#define OT_ERR_MEDIA_SYS_ILLEGAL_PAARAM   OT_ERR_MEDIA_SYS_DEFINE(OT_EIS_ERRNO_ILLEGAL_PARAM)
#define OT_ERR_MEDIA_SYS_NULL_PTR         OT_ERR_MEDIA_SYS_DEFINE(OT_EIS_ERRNO_NULL_PTR)
#define OT_ERR_MEDIA_SYS_NOT_SUPPORT      OT_ERR_MEDIA_SYS_DEFINE(OT_EIS_ERRNO_NO_SUPPORT)
#define OT_ERR_MEDIA_SYS_NO_DISABLE       OT_ERR_MEDIA_SYS_DEFINE(OT_EIS_ERRNO_NO_DISABLE)
#define OT_ERR_MEDIA_SYS_NO_MEM           OT_ERR_MEDIA_SYS_DEFINE(OT_EIS_ERRNO_NO_MEM)
#define OT_ERR_MEDIA_SYS_BUSY             OT_ERR_MEDIA_SYS_DEFINE(OT_EIS_ERRNO_BUSY)

typedef struct {
    ot_u32 blk_size;

    ot_u32 head_stride;
    ot_u32 head_size;
    ot_u32 head_y_size;

    ot_u32 main_stride;
    ot_u32 main_size;
    ot_u32 main_y_size;
} ot_eis_buf_size_calc_cfg;


#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif
