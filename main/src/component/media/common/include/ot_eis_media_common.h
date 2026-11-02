#ifndef OT_EIS_MEDIA_COMMON_H
#define OT_EIS_MEDIA_COMMON_H

#include "ot_eis_type.h"
#include "ot_eis_video.h"
#include "ot_eis_mod.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OT_EIS_MEDIA_AUDIO,
    OT_EIS_MEDIA_VIDEO,
    OT_EIS_MEDIA_BUTT
} ot_eis_media_type;

typedef struct {
    ot_eis_module_id node_type;
    ot_eis_handle handle;
} ot_eis_media_node_port;

typedef struct {
    ot_eis_media_type media_type;
    union {
        ot_eis_video_frame video_frame;
    } frame_info;
} ot_eis_media_frame;

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
