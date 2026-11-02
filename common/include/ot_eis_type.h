/******************************************************************************
 Copyright (C), 2022 Shenzhen Oritek Semiconductors Co., Ltd. All right reserved.
******************************************************************************
Description   : the common data type definition
Created       : 2022/4/20
Last Modified : 2022/11/10
******************************************************************************/
#ifndef OT_EIS_TYPE_H
#define OT_EIS_TYPE_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef char            ot_s8;
typedef unsigned char 	ot_u8;
typedef char 	        ot_char;

typedef signed short 	ot_s16;
typedef unsigned short 	ot_u16;

typedef signed int		ot_s32;
typedef unsigned int 	ot_u32;

typedef float			ot_float;
typedef double			ot_double;

typedef signed long 	ot_slong;
typedef unsigned long 	ot_ulong;

typedef signed long long 	ot_s64;
typedef unsigned long long 	ot_u64;

typedef void ot_void;
#ifndef OT_NULL
#define OT_NULL ((void*)0)
#endif

typedef enum {
    OT_FALSE = 0,
    OT_TRUE = 1
} ot_bool;

typedef void* ot_eis_handle;

typedef ot_u64 ot_phys_addr;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __OT_TYPE_H__ */
