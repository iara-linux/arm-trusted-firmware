/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLATFORM_DEF_H
#define PLATFORM_DEF_H

#include <common/interrupt_props.h>
#include <drivers/arm/gic_common.h>
#include <lib/utils_def.h>

#include "mt8516_def.h"

/*******************************************************************************
 * Platform binary types for linking
 ******************************************************************************/
#define PLATFORM_LINKER_FORMAT		"elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH		aarch64

/*******************************************************************************
 * Generic platform constants
 ******************************************************************************/
#if defined(IMAGE_BL31)
#define PLATFORM_STACK_SIZE		0x800
#else
#define PLATFORM_STACK_SIZE		0x440
#endif

#define FIRMWARE_WELCOME_STR		"Booting Trusted Firmware\n"

#define PLATFORM_MAX_AFFLVL		MPIDR_AFFLVL2
#define PLAT_MAX_PWR_LVL		U(2)
#define PLAT_MAX_RET_STATE		U(1)
#define PLAT_MAX_OFF_STATE		U(2)
#define PLATFORM_SYSTEM_COUNT		U(1)
#define PLATFORM_CLUSTER_COUNT		U(1)
#define PLATFORM_CLUSTER0_CORE_COUNT	U(4)
#define PLATFORM_CORE_COUNT		PLATFORM_CLUSTER0_CORE_COUNT
#define PLATFORM_MAX_CPUS_PER_CLUSTER	U(4)
#define PLATFORM_NUM_AFFS		(PLATFORM_SYSTEM_COUNT +	\
					 PLATFORM_CLUSTER_COUNT +	\
					 PLATFORM_CORE_COUNT)

#define SOC_CHIP_ID			U(0x8516)

/*******************************************************************************
 * Platform memory map related constants
 ******************************************************************************/
/*
 * Unlike MT8173 (which runs BL31 from SRAM at 0x100000), Preloader loads BL31
 * image into the secure DRAM carveout as part of the TEE image, and jumps straight
 * into it. Therefore, TZRAM_BASE must match the hardcoded load address used by
 * Preloader.
 *
 * 0x43000000 +-------------------+
 *            | shared mem (4KB)  |
 * 0x43001000 +-------------------+
 *            |                   |
 *            |   BL31 (124KB)    |
 *            |                   |
 * 0x43020000 +-------------------+
*/
#ifndef TZRAM_BASE
#define TZRAM_BASE			(0x43000000)
#endif
#define TZRAM_SIZE			(0x20000)

/*******************************************************************************
 * BL31 specific defines.
 ******************************************************************************/
#define BL31_BASE			(TZRAM_BASE + 0x1000)
#define BL31_LIMIT			(TZRAM_BASE + TZRAM_SIZE)

#ifndef PLAT_MT8516_BL33_BASE
#define PLAT_MT8516_BL33_BASE		(0x41E00000)
#endif

/*******************************************************************************
 * Platform specific page table and MMU setup constants
 ******************************************************************************/
#define PLAT_PHY_ADDR_SPACE_SIZE	(1ULL << 32)
#define PLAT_VIRT_ADDR_SPACE_SIZE	(1ULL << 32)
#define MAX_XLAT_TABLES			4
#define MAX_MMAP_REGIONS		16

/*******************************************************************************
 * Declarations and constants to access the mailboxes safely. Each mailbox is
 * aligned on the biggest cache line size in the platform. This is known only
 * to the platform as it might have a combination of integrated and external
 * caches. Such alignment ensures that two maiboxes do not sit on the same cache
 * line at any cache level. They could belong to different cpus/clusters &
 * get written while being protected by different locks causing corruption of
 * a valid mailbox address.
 ******************************************************************************/
#define CACHE_WRITEBACK_SHIFT	6
#define CACHE_WRITEBACK_GRANULE	(1 << CACHE_WRITEBACK_SHIFT)

#define PLAT_ARM_GICD_BASE		BASE_GICD_BASE
#define PLAT_ARM_GICC_BASE		BASE_GICC_BASE

#define PLAT_ARM_G1S_IRQ_PROPS(grp) \
	INTR_PROP_DESC(MT_IRQ_SEC_SGI_0, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(MT_IRQ_SEC_SGI_1, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(MT_IRQ_SEC_SGI_2, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(MT_IRQ_SEC_SGI_3, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(MT_IRQ_SEC_SGI_4, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(MT_IRQ_SEC_SGI_5, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(MT_IRQ_SEC_SGI_6, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(MT_IRQ_SEC_SGI_7, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE)

#define PLAT_ARM_G0_IRQ_PROPS(grp)

/*******************************************************************************
 * Platform specific IRQ
 ******************************************************************************/
#define WDT_IRQ_BIT_ID       (230)
#define FIQ_SMP_CALL_SGI     (13)
#define ATF_LOG_IRQ_ID       (253)

#endif /* PLATFORM_DEF_H */
