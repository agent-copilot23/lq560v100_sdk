/***********************************************************************************
 *  Copyright (C), 2025 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
 *  ********************************************************************************
 *  Description   : board info common header
 *  Created       : 2025/04/09
 *  Created       : 2025/04/09
 *  Group         : SDK Team
 *  *******************************************************************************/

#ifndef OT_BOARD_INFO_COMMON_H_
#define OT_BOARD_INFO_COMMON_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "ot_eis_mod.h"
#include "ot_eis_type.h"
#include "ot_eis_errno.h"
#include "ot_board_info.h"

typedef enum {
    OT_ERRNO_BOARD_INFO_VALUE_OUT_RANGE = 0x21,
    OT_ERRNO_BOARD_INFO_OPEN_FILE_ERROR = 0x22,
    OT_ERRNO_BOARD_INFO_MMAP_ERROR      = 0x23,
} board_info_err_number;

/* board_info通用错误码 */
#define OT_ERR_BOARD_INFO_ILLEGAL_PARAM \
        OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_BOARD_INFO, OT_EIS_ERRNO_ILLEGAL_PARAM) /* 无效参数 */

/* board_info专用错误码 */
#define OT_ERR_BOARD_INFO_VALUE_OUT_RANGE \
        OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_BOARD_INFO, OT_ERRNO_BOARD_INFO_VALUE_OUT_RANGE) /* 值超出范围 */
#define OT_ERR_BOARD_INFO_OPEN_FILE_ERROR \
        OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_BOARD_INFO, OT_ERRNO_BOARD_INFO_OPEN_FILE_ERROR) /* 打开文件失败 */
#define OT_ERR_BOARD_INFO_MMAP_ERROR \
        OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_BOARD_INFO, OT_ERRNO_BOARD_INFO_MMAP_ERROR)      /* 内存映射错误 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* OT_BOARD_INFO_COMMON_H_ */
