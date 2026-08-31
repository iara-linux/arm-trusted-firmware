/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MTSPMC_PRIVATE_H
#define MTSPMC_PRIVATE_H

#include <arch_helpers.h>
#include <cortex_a35.h>

#include <mt8516_def.h>

/* SPM related registers */
#define SPM_POWERON_CONFIG_EN		(SPM_BASE + 0x000)
/* bit-fields of SPM_POWERON_CONFIG_EN */
#define BCLK_CG_EN			(1U << 0)
#define PROJECT_CODE			(0xb16 << 16)

#define SPM_PWR_STATUS			(SPM_BASE + 0x60c)
#define SPM_PWR_STATUS_2ND		(SPM_BASE + 0x610)

/* SPMC related registers */
#define SPMC_CPU_PWR_CON(cpu)		(SPM_BASE + 0xc40 + ((cpu) * 4))
#define SPMC_CPUTOP_PWR_CON		(SPM_BASE + 0xc50)
#define SPMC_CPUTOP_CLK_DIS		(SPM_BASE + 0xc58)
#define SPMC_BYPASS			(SPM_BASE + 0xc5c)

/* bit-fields of SPMC_*_PWR_CON */
#define PWRCTRL_PWR_ON_2ND		(1U << 0)
#define PWRCTRL_PWR_RST_B		(1U << 1)
#define PWRCTRL_PWR_ON			(1U << 2)
#define PWRCTRL_PWR_ACK			(1U << 16)

/* bit-fields of SPMC_CPUTOP_CLK_DIS */
#define PWRCTRL_PWR_CLK_DIS		(1U << 0)

/* bit-fields of SPM_PWR_STATUS */
#define PWR_STATUS_MP0_CPU(cpu)		(1U << (9 + (cpu)))

/* SPARK control */
#define MCUCFG_CPU_SPMC_CTL(cpu)	(MCUCFG_BASE + 0x1c30 + ((cpu) * 4))
/* bit-fields of MCUCFG_CPU_SPMC_CTL */
#define SW_SPARK_EN			(1U << 0)
#define SW_NO_WAIT_Q			(1U << 1)

/* reset vector, MCUCFG_BASE + 0x38 + cpu * 8 */
#define MCUCFG_BOOTADDR(cpu)		MP0_MISC_CONFIG_BOOT_ADDR(cpu)

/* aarch64 reset state, one bit per core */
#define MCUCFG_INITARCH			MP0_MISC_CONFIG3
#define MCUCFG_INITARCH_SHIFT		MP0_CPUCFG_64BIT_SHIFT

DEFINE_RENAME_SYSREG_RW_FUNCS(a35_cpuectlr_el1, CORTEX_A35_CPUECTLR_EL1)

#endif /* MTSPMC_PRIVATE_H */
