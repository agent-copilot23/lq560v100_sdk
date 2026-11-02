/***********************************************************************************
 *  Copyright (C), 2023 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
 *  ********************************************************************************
 *  Description   : string标准接口实现
 *  Created       : 2023/08/31
 *  Last Modified : 2023/08/31
 *  Group         : SDK Team
 *  *******************************************************************************/
#include "ot_string.h"

ot_s32 ot_strcmp(const ot_s8 *str1, const ot_s8 *str2)
{
    ot_s32 ret = 0;
    while(!(ret=*(ot_u8*)str1-*(ot_u8*)str2) && *str1) {
        str1++;
        str2++;
    }

    if (ret < 0) {
        return -1;
    } else if (ret > 0) {
        return 1;
    }

    return 0;
}

