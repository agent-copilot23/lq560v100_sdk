/***********************************************************************************
 *  Copyright (C), 2025 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
 *  ********************************************************************************
 *  Description   : board info header
 *  Created       : 2025/04/09
 *  Created       : 2025/04/09
 *  Group         : SDK Team
 *  *******************************************************************************/

#ifndef OT_BOARD_INFO_H_
#define OT_BOARD_INFO_H_

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    OT_CHIP_TYPE_PLUSV100   = 0x1, /* Chip type: PLUSV100 */
    OT_CHIP_TYPE_MINIV100   = 0x2, /* Chip type: MINIV100 */
    OT_CHIP_TYPE_LITEV100   = 0x3, /* Chip type: LITEV100 */
    OT_CHIP_TYPE_TINYV100   = 0x4, /* Chip type: TINYV100 */
    OT_CHIP_INVALID         = 0x5, /* Chip type: INVALID */
} ot_chip_type_t;

/**
* @brief 获取芯片类型接口
* @param chip_type 芯片类型
* @return 0: 结果正确, 其他值: 错误码
*/
ot_s32 ot_bdi_get_chip_type(ot_chip_type_t *chip_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* OT_BOARD_INFO_H_ */
