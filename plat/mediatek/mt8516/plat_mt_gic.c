/*
 * Copyright (c) 2013-2016, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <drivers/arm/gic_common.h>
#include <drivers/arm/gicv2.h>
#include <bl31/interrupt_mgmt.h>
#include <common/bl_common.h>
#include <mt8516_def.h>
#include <platform_def.h>
#include <lib/mmio.h>
#include <common/debug.h>
#include <plat/common/platform.h>
#include <plat/arm/common/plat_arm.h>

void gicd_write_icfgr(uintptr_t base, unsigned int id, unsigned int val);

void plat_mt_gic_init(void)
{
	int idx;

	/* Initialize the gic cpu and distributor interfaces */
	plat_arm_gic_driver_init();
	plat_arm_gic_init();

	/* set WDT interrupt as falling edge trigger */
	gicd_write_icfgr(BASE_GICD_BASE, WDT_IRQ_BIT_ID,
		0x2 << ((WDT_IRQ_BIT_ID % 16) * 2));

	/* set pol control as non-secure */
	for (idx = 0; idx < INT_POL_SECCTL_NUM; idx++)
		mmio_write_32(INT_POL_SECCTL0 + idx * 4, 0);
}
