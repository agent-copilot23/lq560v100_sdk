/*
 * Copyright (c) Oritek Technologies Co., Ltd. 2020-2023. All rights reserved.
 */
#ifndef __COMMON_H_
#define __COMMON_H_
#include "ot_type.h"
#include "platform.h"
#include "share_drivers.h"

#define NO_CHECK_WORD	0
#define DELAY_TIME_MS	2

typedef struct {
	ot_u32 length;
	ot_u8 *data;
} ext_data;

typedef struct {
	const ot_u8 *r; /* r/x component of the signature. */
	const ot_u8 *s; /* s/y component of the signature. */
	ot_u32 length;
} ext_pke_sig;

#define ALWAYS_INLINE inline __attribute__((always_inline))

ALWAYS_INLINE static void call_reset()
{
	mdelay(DELAY_TIME_MS);
	timer_deinit();
	reg_set(REG_SYSCTRL_BASE + REG_SC_SYSRES, 0x1);
	reg_set(REG_SYSCTRL_BASE + REG_SC_SYSRES, 0x1);
	reg_set(REG_SYSCTRL_BASE + REG_SC_SYSRES, 0x1);
}
/*-----------------------------------------------------------------
 * timer interface
------------------------------------------------------------------*/
unsigned long timer_get_divider();

#endif /* __COMMON_H_ */
