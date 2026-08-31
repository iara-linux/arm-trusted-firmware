/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/mmio.h>
#include <platform_def.h>

#include <mtspmc.h>

#include "mtspmc_private.h"

static void set_retention(int tick)
{
	uint64_t cpuectlr;

	cpuectlr = read_a35_cpuectlr_el1();
	cpuectlr &= ~0x7ULL;
	cpuectlr |= tick & 0x7;
	write_a35_cpuectlr_el1(cpuectlr);
}

void spm_enable_cpu_auto_off(int cpu)
{
	set_retention(1);
	mmio_clrbits_32(MCUCFG_CPU_SPMC_CTL(cpu), SW_NO_WAIT_Q);
}

void spm_disable_cpu_auto_off(int cpu)
{
	mmio_setbits_32(MCUCFG_CPU_SPMC_CTL(cpu), SW_NO_WAIT_Q);
	set_retention(0);
}

void spm_set_cpu_power_off(int cpu)
{
	mmio_clrbits_32(SPMC_CPU_PWR_CON(cpu), PWRCTRL_PWR_ON);
}

void mcucfg_set_bootaddr(int cpu, uintptr_t bootaddr)
{
	mmio_write_32(MCUCFG_BOOTADDR(cpu), bootaddr);
}

uintptr_t mcucfg_get_bootaddr(int cpu)
{
	return mmio_read_32(MCUCFG_BOOTADDR(cpu));
}

void mcucfg_init_archstate(int cpu, int arm64)
{
	uint32_t bit = 1U << (MCUCFG_INITARCH_SHIFT + cpu);

	mmio_clrsetbits_32(MCUCFG_INITARCH, bit, (arm64 & 1) ? bit : 0);
}

int spm_get_cpu_powerstate(int cpu)
{
	return (mmio_read_32(SPMC_CPU_PWR_CON(cpu)) & PWRCTRL_PWR_ACK) != 0;
}

int spmc_init(void)
{
	unsigned int cpu;

	/* enable SPM register control */
	mmio_write_32(SPM_POWERON_CONFIG_EN, PROJECT_CODE | BCLK_CG_EN);

	/* 0: SPMC mode  1: Legacy mode */
	mmio_write_32(SPMC_BYPASS, 0);

	for (cpu = 1; cpu < PLATFORM_CLUSTER0_CORE_COUNT; cpu++) {
		mmio_clrbits_32(SPMC_CPU_PWR_CON(cpu), PWRCTRL_PWR_ON_2ND);
		mmio_clrbits_32(SPMC_CPU_PWR_CON(cpu), PWRCTRL_PWR_RST_B);

		mmio_clrbits_32(SPMC_CPU_PWR_CON(cpu), PWRCTRL_PWR_ON);
	}

	return 0;
}

/**
 * Power on a core with specified core index
 *
 * @cpu: the CPU ID of the CPU which to be powered on
 */
void spm_poweron_cpu(int cpu)
{
	INFO("spmc: power on core %d\n", cpu);

	/* Set PWR_ON = 1 */
	mmio_setbits_32(SPMC_CPU_PWR_CON(cpu), PWRCTRL_PWR_ON);

	/* Wait until the core acknowledges */
	while (!spm_get_cpu_powerstate(cpu))
		;

	INFO("spmc: power on core %d successfully\n", cpu);
}

/**
 * Power off a core with the specified core index
 *
 * @cpu: the CPU ID of the CPU which to be powered off
 */
void spm_poweroff_cpu(int cpu)
{
	INFO("spmc: power off core %d\n", cpu);

	/* Set PWR_ON = 0 */
	mmio_clrbits_32(SPMC_CPU_PWR_CON(cpu), PWRCTRL_PWR_ON);

	/* Wait until the core acknowledges */
	while (spm_get_cpu_powerstate(cpu))
		;

	INFO("spmc: power off core %d successfully\n", cpu);
}
