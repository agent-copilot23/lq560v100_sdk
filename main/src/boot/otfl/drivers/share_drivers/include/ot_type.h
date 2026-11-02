/*
 * Copyright (c) Oritek Technologies Co., Ltd. 2020-2023. All rights reserved.
 */
#ifndef __OT_TYPE_H__
#define __OT_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif
#endif

/* defines */
#define OT_NULL             NULL
#define OT_NULL_PTR         OT_NULL
#define OT_FALSE 0
#define OT_TRUE  1

typedef unsigned char           ot_uchar;
typedef unsigned char           ot_u8;
typedef unsigned short          ot_u16;
typedef unsigned int            ot_u32;
typedef unsigned long long      ot_u64;
typedef unsigned long           ot_ulong;
typedef char                    ot_char;
typedef signed char             ot_s8;
typedef short                   ot_s16;
typedef int                     ot_s32;
typedef long long               ot_s64;
typedef long                    ot_slong;
typedef float                   ot_float;
typedef double                  ot_double;
typedef void                    ot_void;
typedef ot_u8                   ot_bool;
typedef ot_u64                  ot_handle;
typedef ot_u8                   ot_byte;
typedef ot_byte                *ot_pbyte;
typedef ot_void                *ot_pvoid;
typedef volatile ot_u32         ot_u32_reg;
typedef ot_ulong           	ot_size_t;
typedef signed long           	ot_ssize_t;
typedef ot_handle      		ot_mem_size_t;
typedef ot_s64               	ot_mem_handle_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __OT_TYPES_H__ */

