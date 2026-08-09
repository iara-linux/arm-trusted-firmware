/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RTC_H
#define RTC_H

#define RTC_BASE		0x8000

/* RTC registers */
enum {
	RTC_BBPU	= RTC_BASE + 0x0000,
	RTC_IRQ_STA	= RTC_BASE + 0x0002,
	RTC_IRQ_EN	= RTC_BASE + 0x0004,
	RTC_CII_EN	= RTC_BASE + 0x0006
};

enum {
	RTC_OSC32CON	= RTC_BASE + 0x0026,
	RTC_WRTGR	= RTC_BASE + 0x003C,
	RTC_CON		= RTC_BASE + 0x003E
};

enum {
	RTC_PDN1	= RTC_BASE + 0x002C,
	RTC_PDN2	= RTC_BASE + 0x002E,
	RTC_SPAR0	= RTC_BASE + 0x0030,
	RTC_SPAR1	= RTC_BASE + 0x0032,
	RTC_PROT	= RTC_BASE + 0x0036,
	RTC_DIFF	= RTC_BASE + 0x0038,
	RTC_CALI	= RTC_BASE + 0x003A
};

enum {
	RTC_PROT_UNLOCK1 = 0x586A,
	RTC_PROT_UNLOCK2 = 0x9136
};

enum {
	RTC_BBPU_PWREN	= 1U << 0,	/* BBPU = 1 when alarm occurs */
	RTC_BBPU_BBPU	= 1U << 2,	/* 1: power on, 0: power down */
	RTC_BBPU_AUTO	= 1U << 3,	/* BBPU = 0 when xreset_rstb low */
	RTC_BBPU_CLRPKY	= 1U << 4,
	RTC_BBPU_RELOAD	= 1U << 5,
	RTC_BBPU_CBUSY	= 1U << 6
};

enum {
	RTC_BBPU_KEY	= 0x43 << 8
};

/* external API */
uint16_t RTC_Read(uint32_t addr);
void RTC_Write(uint32_t addr, uint16_t data);
int32_t rtc_busy_wait(void);
int32_t RTC_Write_Trigger(void);
int32_t Writeif_unlock(void);
void rtc_bbpu_power_down(void);

#endif /* RTC_H */
