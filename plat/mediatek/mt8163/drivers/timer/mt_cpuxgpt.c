/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <lib/mmio.h>

#include <mt8163_def.h>
#include <mt_cpuxgpt.h>

/* REG */
#define CPUXGPT_CTL		(MCUCFG_BASE + 0x670)
#define CPUXGPT_IDX		(MCUCFG_BASE + 0x674)

#define INDEX_CNT_L_INIT	0x008
#define INDEX_CNT_H_INIT	0x00C

static void write_cpuxgpt(unsigned int reg_index, unsigned int value)
{
	mmio_write_32(CPUXGPT_IDX, reg_index);
	mmio_write_32(CPUXGPT_CTL, value);
}

static void cpuxgpt_set_init_cnt(unsigned int countH, unsigned int countL)
{
	write_cpuxgpt(INDEX_CNT_H_INIT, countH);
	/* update count when countL programmed */
	write_cpuxgpt(INDEX_CNT_L_INIT, countL);
}

void generic_timer_backup(void)
{
	uint64_t cval;

	cval = read_cntpct_el0();
	cpuxgpt_set_init_cnt((uint32_t)(cval >> 32),
			     (uint32_t)(cval & 0xffffffff));
}
