/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */

#ifndef SAMPLE_UTILS_H
#define SAMPLE_UTILS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "ot_eis_type.h"
#include "ot_eis_errno.h"


#define CHECK_RET(exp, ret) if (exp) {return ret;}
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

enum{
  UG_ERROR=0,
  UG_WARN,
  UG_INFO,
  UG_DEBUG,
};
#define LOGLEVEL  UG_ERROR
#define debug(fmt, args...)     if(LOGLEVEL >= UG_DEBUG) printf(fmt, ##args)
#define info(fmt, args...)      if(LOGLEVEL >= UG_INFO) printf(fmt, ##args)
#define warn(fmt, args...)      if(LOGLEVEL >= UG_WARN) printf(fmt, ##args)
#define error(fmt, args...)     if(LOGLEVEL >= UG_ERROR) printf(fmt, ##args)

ot_s8* sample_upgrade_read_all(const ot_s8 *filename);
ot_bool sample_upgrade_is_path_exist(const ot_s8 *path);
ot_s32 sample_upgrade_read_file(const ot_s8 *path, ot_u32 file_offset, ot_u8 *buff, ot_u32 buff_len);
ot_s32 sample_upgrade_write_file(const ot_s8 *path, ot_u32 file_offset,  ot_u8 *buff, ot_u32 buff_len);
ot_void sample_upgrade_del_file(const ot_s8 *path);
ot_void sample_upgrade_save_bin(const ot_s8 *path, const ot_u8 *content, ot_u32 size);
ot_u32 sample_upgrade_get_loglvl(ot_void);
ot_bool sample_upgrade_get_bool_setting(ot_s8 *n);
ot_s32 sample_format_file(const ot_s8 *file_path);
#endif /* !SAMPLE_UTILS_H*/
