/******************************************************************************
  Copyright (C), 2024-2124, Oritek Tech. Co., Ltd.
 ******************************************************************************
  File Name     : ot_proc_common.h
  Version       : Initial
  Author        : Oritek multimedia software group
  Created       : 2024/07/03
  Description   : Common apis for user proc system.
  History       :
  1.Date        : 2024/07/03
    Author      : 
    Modification: Created file
*******************************************************************************/
#ifndef __OT_USER_PROC_COMMON_H__
#define __OT_USER_PROC_COMMON_H__

#include "ot_eis_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct
{
    ot_u8* buffer;
    ot_u32 size;
    ot_u32 offset;
} ot_proc_show_buffer_attr;

typedef ot_s32 (* ot_proc_show_func)(ot_proc_show_buffer_attr* buffer, ot_void *priv_data);
typedef ot_s32 (* ot_proc_cmd_func)(ot_proc_show_buffer_attr* buffer, ot_u32 args, ot_u8 *argv[], ot_void *priv_data);

typedef struct
{
    ot_s8 *name;
    ot_s8 *dir_name;
    ot_proc_show_func proc_show;
    ot_proc_cmd_func proc_cmd;
    ot_void *priv_data;
} ot_proc_entry_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
