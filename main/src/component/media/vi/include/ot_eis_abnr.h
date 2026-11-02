#ifndef OT_EIS_ABNR_H
#define OT_EIS_ABNR_H

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


typedef enum {
    OT_EIS_ABNR_NOISE_TYPE_IMX623,
    OT_EIS_ABNR_NOISE_TYPE_OX03C10,
    OT_EIS_ABNR_NOISE_TYPE_OX03F10,
} ot_eis_abnr_noise_type;


typedef struct {
    ot_u64                  *thresholds;
    ot_u32                  thresholds_num;
    ot_eis_img_rect         roi_rect;
    ot_eis_abnr_noise_type  noise_type;
} ot_eis_abnr_params;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OT_EIS_ABNR_H */