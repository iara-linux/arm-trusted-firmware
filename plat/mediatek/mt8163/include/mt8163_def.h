/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MT8163_DEF_H
#define MT8163_DEF_H

#define MT8163_PRIMARY_CPU	0x0

/* Register base address */
#define IO_PHYS			(0x10000000)
#define INFRACFG_AO_BASE	(IO_PHYS + 0x1000)
#define SRAMROM_SEC_BASE	(IO_PHYS + 0x1800)
#define PERI_CON_BASE		(IO_PHYS + 0x3000)
#define GPIO_BASE		(IO_PHYS + 0x5000)
#define SPM_BASE		(IO_PHYS + 0x6000)
#define RGU_BASE		(IO_PHYS + 0x7000)
#define PMIC_WRAP_BASE		(IO_PHYS + 0xD000)
#define DEVAPC0_BASE		(IO_PHYS + 0xE000)
#define MCUCFG_BASE		(IO_PHYS + 0x200000)
#define APMIXED_BASE		(IO_PHYS + 0x209000)
#define TRNG_BASE		(IO_PHYS + 0x20F000)
#define CRYPT_BASE		(IO_PHYS + 0x210000)
#define MT_GIC_BASE		(IO_PHYS + 0x220000)

/* DRAM base of MT8163 */
#define DRAM_BASE		(0x40000000)

/* Aggregate of all devices in the first GB */
#define MTK_DEV_RNG0_BASE	IO_PHYS
#define MTK_DEV_RNG0_SIZE	0x400000
#define MTK_DEV_RNG1_BASE	(IO_PHYS + 0x1000000)
#define MTK_DEV_RNG1_SIZE	0x4000000

/*******************************************************************************
 * System counter frequency related constants
 ******************************************************************************/
#define SYS_COUNTER_FREQ_IN_TICKS	13000000

/*******************************************************************************
 * SPM_PCM related constants
 ******************************************************************************/
#define SPM_PCM_STAGE_BASE	(0x43030000)
#define SPM_PCM_STAGE_SLOTS	4
#define SPM_PCM_STAGE_SLOT_SIZE	0x1000
#define SPM_PCM_STAGE_SIZE	(SPM_PCM_STAGE_SLOTS * SPM_PCM_STAGE_SLOT_SIZE)
#define SPM_PCM_STAGE_SLOT(n)	(SPM_PCM_STAGE_BASE +			\
				 ((n) * SPM_PCM_STAGE_SLOT_SIZE))

/*******************************************************************************
 * DEVAPC related constants
 ******************************************************************************/
#define DEVAPC0_MAS_SEC_0	(DEVAPC0_BASE + 0x500)
#define DEVAPC0_APC_CON		(DEVAPC0_BASE + 0xF00)

/*******************************************************************************
 * MD32 related constants
 ******************************************************************************/
#define MD32_SRAM_BASE		(IO_PHYS + 0x20000)
#define MD32_PTCM_BASE		MD32_SRAM_BASE
#define MD32_PTCM_SIZE		0x18000
#define MD32_DTCM_BASE		(MD32_SRAM_BASE + MD32_PTCM_SIZE)
#define MD32_DTCM_SIZE		0x20000
#define MD32_BASE		(IO_PHYS + 0x58000)
#define MD32_CLK_CTRL_BASE	(IO_PHYS + 0x59000)

/*******************************************************************************
 * UART related constants
 ******************************************************************************/
#define MT8163_UART0_BASE	(IO_PHYS + 0x01002000)
#define MT8163_UART1_BASE	(IO_PHYS + 0x01003000)
#define MT8163_UART2_BASE	(IO_PHYS + 0x01004000)
#define MT8163_UART3_BASE	(IO_PHYS + 0x01005000)

#ifndef MT8163_BAUDRATE
#define MT8163_BAUDRATE		(115200)
#endif

#ifndef MT8163_UART_CLOCK
#define MT8163_UART_CLOCK	(26000000)
#endif

/*******************************************************************************
 * L2C related constants
 ******************************************************************************/
#define CONFIGED_256K		0x1
#define CONFIGED_512K		0x3
#define L2C_SIZE_CFG_OFF	8
#define L2C_SHARE_ENABLE	12

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
 * SRAMROM related constants
 ******************************************************************************/
#define SRAMROM_SEC_CTRL	(SRAMROM_SEC_BASE + 0x4)
#define SRAMROM_SEC_ADDR	(SRAMROM_SEC_BASE + 0x8)

/*******************************************************************************
 * GIC-400 & interrupt handling related constants
 ******************************************************************************/

/* Base MTK_platform compatible GIC memory map */
#define BASE_GICD_BASE		(MT_GIC_BASE + 0x1000)
#define BASE_GICC_BASE		(MT_GIC_BASE + 0x2000)
#define BASE_GICR_BASE		0	/* no GICR in GIC-400 */
#define BASE_GICH_BASE		(MT_GIC_BASE + 0x4000)
#define BASE_GICV_BASE		(MT_GIC_BASE + 0x6000)
#define INT_POL_CTL0		0x10200620

/* FIQ platform related define */
#define MT_IRQ_SEC_SGI_0	8
#define MT_IRQ_SEC_SGI_1	9
#define MT_IRQ_SEC_SGI_2	10
#define MT_IRQ_SEC_SGI_3	11
#define MT_IRQ_SEC_SGI_4	12
#define MT_IRQ_SEC_SGI_5	13
#define MT_IRQ_SEC_SGI_6	14
#define MT_IRQ_SEC_SGI_7	15

#endif /* MT8163_DEF_H */
