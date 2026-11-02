/******************************************************************************
  Copyright (C), 2024-2124, Oritek Tech. Co., Ltd.
******************************************************************************
  File Name     : ot_proc.h
  Version       : Initial
  Author        : Oritek multimedia software group
  Created       : 2024/07/03
  Description   : Support user proc function.
  History       :
  1.Date        : 2024/07/03
    Author      : xxx
    Modification: Created file
******************************************************************************/

#ifndef __OT_USERPROC_H__
#define __OT_USERPROC_H__

#include "ot_eis_type.h"
#include "ot_proc_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif


/**
@brief User mode proc is initialized.
@attention You need insert module before calling this API.
@param[in] None
@retval ::OT_SUCCESS Success
@retval ::OT_FAILURE Failure
@see None
*/
ot_s32 ot_proc_init(ot_void);

/**
@brief User mode proc is deinitialized.
@attention None.
@param[in] None
@retval ::OT_SUCCESS Success
@retval ::OT_FAILURE Failure
@see None
*/
ot_s32 ot_proc_deinit(ot_void);

/**
@brief User mode proc cretea directory.
@attention You need register module before calling this API. Only support create one level directory.
@param[in] dir_name The directory name.
@param[out] None
@retval ::OT_SUCCESS Success
@retval ::OT_FAILURE Failure
@see None
*/
ot_s32 ot_proc_dir_add(const ot_s8 *dir_name);

/**
@brief User mode proc remove directory.
@attention It will return fail if there are entries in the directory.
@param[in] dir_name The directory name.
@retval ::OT_SUCCESS Success
@retval ::OT_FAILURE Failure
@see None
*/
ot_s32 ot_proc_dir_remove(const ot_s8 *dir_name);

/**
@brief User mode proc add entry.
@attention None
@param[in] entry Parameter of entry.
@retval ::OT_SUCCESS Success
@retval ::OT_FAILURE Failure
@see None
*/
ot_s32 ot_proc_entry_create(const ot_proc_entry_attr* proc_entry);

/**
@brief User mode proc remove entry.
@attention None
@param[in] u32ModuleID Module ID.
@param[in] entry Parameter of entry.
@retval ::OT_SUCCESS Success
@retval ::OT_FAILURE Failure
@see None
*/
ot_s32 ot_proc_entry_destroy(const ot_proc_entry_attr* proc_entry);

/**
@brief User mode proc print function.
@attention None
@param[in] buffer Output buffer parameter.
@param[in] pFmt   Format parameter.
@retval ::OT_SUCCESS Success
@retval ::OT_FAILURE Failure
@see None
*/
ot_s32 ot_proc_entry_show(ot_proc_show_buffer_attr *buffer, const ot_s8 *fmt, ...);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif
