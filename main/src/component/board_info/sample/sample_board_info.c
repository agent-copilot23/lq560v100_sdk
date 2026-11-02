/***********************************************************************************
 *  Copyright (C), 2025 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
 *  ********************************************************************************
 *  Description   : test board info impl
 *  Created       : 2025/04/09
 *  Created       : 2025/04/09
 *  Group         : SDK Team
 *  *******************************************************************************/

#include <stdio.h>
#include "ot_board_info.h"
#include "ot_board_info_common.h"

static const ot_s8* chip_type_to_str(ot_chip_type_t type)
{
    switch (type) {
    case OT_CHIP_TYPE_PLUSV100: return "PLUS_V100";
    case OT_CHIP_TYPE_MINIV100: return "MINI_V100";
    case OT_CHIP_TYPE_LITEV100: return "LITE_V100";
    case OT_CHIP_TYPE_TINYV100: return "TINY_V100";
    default:                    return "UNKNOWN";
    }
}

ot_s32 main()
{
    ot_chip_type_t chip_type = OT_CHIP_INVALID;

    if (ot_bdi_get_chip_type(&chip_type) == OT_SUCCESS) {
        printf("Chip type = 0x%x (%s).\n", chip_type, chip_type_to_str(chip_type));
    } else {
        printf("Failed to get chip type, Chip type = 0x%x (%s).\n", chip_type, chip_type_to_str(chip_type));
    }

    return 0;
}
