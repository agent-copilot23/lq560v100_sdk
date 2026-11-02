/***********************************************************************************
*  Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
*  ********************************************************************************
*  Description   : vrb api
*  Created       : 2023/03/03
*  Last Modified : 2023/03/03
*  Group         : SDK Team
*  ********************************************************************************/

#ifndef OT_VRB_H
#define OT_VRB_H

#include "ot_eis_type.h"
#include "ot_buffer_pool_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================
 * Name: ot_vrb_config
 * Author：
 * Date：2023-07-04
 * Function：VRB config and initialize the memory managered by vrb
 * Input： ot_video_buffer_attr* attr
 * Output: none
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_vrb_config(const ot_video_buffer_attr* attr);

/*==================================================================
 * Name: ot_vrb_init
 * Author：
 * Date：2023-07-04
 * Function：VRB init
 * Input： none
 * Output: none
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_vrb_init(); 

/*==================================================================
 * Name: ot_vrb_deinit
 * Author：
 * Date：2023-07-04
 * Function：VRB deinit
 * Input： none
 * Output: none
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_vrb_deinit();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OT_VRB_API_H */
