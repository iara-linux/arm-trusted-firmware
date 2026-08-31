/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <lib/mmio.h>

#include <mt8516_def.h>
#include <scu.h>

void disable_scu(unsigned long mpidr)
{
	if (mpidr & MPIDR_CLUSTER_MASK)
		return;

	mmio_setbits_32(MP0_AXI_CONFIG, MP0_ACINACTM);
}

void enable_scu(unsigned long mpidr)
{
	if (mpidr & MPIDR_CLUSTER_MASK)
		return;

	mmio_clrbits_32(MP0_AXI_CONFIG, MP0_ACINACTM);
}
