/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#ifndef LOAD_BMP_H
#define LOAD_BMP_H

#include "ot_eis_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OT_MAX(a, b) ((a) > (b) ? (a) : (b))
#define OT_MIN(a, b) ((a) < (b) ? (a) : (b))
#define OT_ABS     abs

ot_ulong ot_file_read(
    ot_void * ptr,
    ot_ulong size,
    ot_void *p
    );

ot_s32 ot_file_seek(
    ot_void *p,
    ot_ulong offset,
    ot_bool end
    );

ot_u8 ot_file_read_byte(
    ot_void *p
    );

ot_u16 ot_file_read_word(
    ot_void *p
    );

ot_u32 ot_file_read_dword(
    ot_void *p
    );

ot_s32 ot_file_read_long(
    ot_void *p
    );

ot_u32 ot_file_tell(
    ot_void *p
    );
ot_s32 ot_file_write(
    ot_void *ptr,
    ot_ulong size,
    ot_void *p
    );

ot_s32 ot_file_write_word(ot_void *p, ot_u16 w);

ot_s32 ot_file_write_dword(ot_void *p, ot_u32 dw);

ot_s32 ot_file_write_long(ot_void *p, ot_s32  l);

ot_s32 ot_file_close(ot_void *p);

ot_s32 ot_load_bmp_area(
    ot_char* file_name,
    ot_u8* buf,
    ot_u32 buf_width,
    ot_u32 buf_height,
    ot_u32 buf_stride
    );

#ifdef __cplusplus
}
#endif

#endif /* End of #ifndef __LOAD_BMP_H__ */
