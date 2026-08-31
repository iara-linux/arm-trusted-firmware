/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MT8516_DEF_H
#define MT8516_DEF_H

#define MT8516_PRIMARY_CPU 0x0

/* Register base address */
#define IO_PHYS			(0x10000000)
#define INFRACFG_AO_BASE	(IO_PHYS + 0x1000)
#define GPIO_BASE		(IO_PHYS + 0x5000)
#define SPM_BASE		(IO_PHYS + 0x6000)
#define RGU_BASE		(IO_PHYS + 0x7000)
#define PMIC_WRAP_BASE		(IO_PHYS + 0xF000)
#define MCUCFG_BASE		(IO_PHYS + 0x200000)
#define TRNG_base		(IO_PHYS + 0x20F000)
#define MT_GIC_BASE		(IO_PHYS + 0x310000)
#define PLAT_MT_CCI_BASE	(IO_PHYS + 0x390000)
#define DEVAPC0_BASE		(IO_PHYS + 0x10000)

/* Aggregate of all devices in the first GB */
#define MTK_DEV_RNG0_BASE	IO_PHYS
#define MTK_DEV_RNG0_SIZE	0x400000
#define MTK_DEV_RNG1_BASE	(IO_PHYS + 0x1000000)
#define MTK_DEV_RNG1_SIZE	0x4000000

/* DRAM base of MT8516 */
#define DRAM_BASE		(0x40000000)

/*******************************************************************************
 * UART related constants
 ******************************************************************************/
#define MT8516_UART0_BASE	(IO_PHYS + 0x01005000)

#ifndef MT8516_BAUDRATE
#define MT8516_BAUDRATE		(921600)
#endif

#ifndef MT8516_UART_CLOCK
#define MT8516_UART_CLOCK	(26000000)
#endif

/*******************************************************************************
 * L2C related constants
 ******************************************************************************/
#define CONFIGED_256K		0x1
#define CONFIGED_512K		0x3
#define L2C_SIZE_CFG_OFF	8
#define L2C_SHARE_ENABLE	0

/*******************************************************************************
 * MCUSYS Related constants
 ******************************************************************************/
#define MP0_MISC_CONFIG_BOOT_ADDR(cpu)	(MCUCFG_BASE + 0x38 + ((cpu) * 8))
#define MP1_MISC_CONFIG_BOOT_ADDR(cpu)	(MCUCFG_BASE + 0x238 + ((cpu) * 8))
#define MP0_MISC_CONFIG3		(MCUCFG_BASE + 0x3c)

#define MP0_CPUCFG_64BIT_SHIFT		12
#define MP0_CPUCFG_64BIT		(0xfU << MP0_CPUCFG_64BIT_SHIFT)

#define BOOTROM_BOOT_ADDR		(INFRACFG_AO_BASE + 0x800)
#define BOOTROM_SEC_CTRL		(INFRACFG_AO_BASE + 0x804)
#define SW_ROM_PD			(1U << 31)

#define MP0_MISC_CONFIG0		(MCUCFG_BASE + 0x0030)
#define MP0_MISC_CONFIG9		(MCUCFG_BASE + 0x0054)
#define MP1_MISC_CONFIG0		(MCUCFG_BASE + 0x0230)
#define MP1_MISC_CONFIG9		(MCUCFG_BASE + 0x0254)

#define MP0_CA7L_CACHE_CONFIG		(MCUCFG_BASE + 0)
#define MP1_CA7L_CACHE_CONFIG		(MCUCFG_BASE + 0x200)

/*******************************************************************************
 * SCU Related constants
 ******************************************************************************/
#define MP0_AXI_CONFIG		(MCUCFG_BASE + 0x002C)
#define MP0_ACINACTM		(1U << 4)

/*******************************************************************************
 * System counter frequency related constants
 ******************************************************************************/
#define SYS_COUNTER_FREQ_IN_TICKS	13000000

/*******************************************************************************
 * GIC-400 & interrupt handling related constants
 ******************************************************************************/

/* Base MTK_platform compatible GIC memory map */
#define BASE_GICD_BASE		(MT_GIC_BASE + 0x0)
#define BASE_GICC_BASE		(MT_GIC_BASE + 0x10000)
#define BASE_GICR_BASE		0	/* no GICR in GIC-400 */
#define BASE_GICH_BASE		(MT_GIC_BASE + 0x30000)
#define BASE_GICV_BASE		(MT_GIC_BASE + 0x50000)
#define INT_POL_CTL0		0x10200620
#define INT_POL_SECCTL0         0x10200A00
#define INT_POL_SECCTL_NUM      8

#define GIC_PRIVATE_SIGNALS	(32)

/*******************************************************************************
 * WDT related constants
 ******************************************************************************/
#define MTK_WDT_BASE		(RGU_BASE + 0)
#define MTK_WDT_SWRST		(MTK_WDT_BASE + 0x0014)

#define MTK_WDT_MODE_DUAL_MODE	0x0040
#define MTK_WDT_MODE_IRQ	0x0008
#define MTK_WDT_MODE_KEY	0x22000000
#define MTK_WDT_MODE_EXTEN	0x0004
#define MTK_WDT_SWRST_KEY	0x1209

/* FIQ platform related define */
#define MT_IRQ_SEC_SGI_0	8
#define MT_IRQ_SEC_SGI_1	9
#define MT_IRQ_SEC_SGI_2	10
#define MT_IRQ_SEC_SGI_3	11
#define MT_IRQ_SEC_SGI_4	12
#define MT_IRQ_SEC_SGI_5	13
#define MT_IRQ_SEC_SGI_6	14
#define MT_IRQ_SEC_SGI_7	15

#endif /* MT8516_DEF_H */
