/*
 * Copyright (c) 2020, Google LLC. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <lib/mmio.h>
#include <mt8516_def.h>
#include <wdt.h>

/*
 * We expect the WDT registers to be correctly initialized by BL2 firmware
 * (which may be board specific), so we do not reinitialize them here.
 */

void wdt_trigger_reset(void)
{
	mmio_clrsetbits_32(MTK_WDT_BASE,
		(MTK_WDT_MODE_DUAL_MODE | MTK_WDT_MODE_IRQ),
		MTK_WDT_MODE_KEY);
	mmio_setbits_32(MTK_WDT_BASE, (MTK_WDT_MODE_KEY | MTK_WDT_MODE_EXTEN));
	mmio_setbits_32(MTK_WDT_SWRST, MTK_WDT_SWRST_KEY);
}
