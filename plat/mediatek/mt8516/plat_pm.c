/*
 * Copyright (c) 2013-2020, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <platform_def.h>

#include <arch_helpers.h>
#include <common/debug.h>
#include <drivers/arm/gicv2.h>
#include <lib/psci/psci.h>
#include <plat/arm/common/plat_arm.h>

#include <mt8516_def.h>
#include <mtspmc.h>
#if MT8516_HAVE_PMIC == 1
#include <rtc.h>
#endif
#include <wdt.h>

static uintptr_t plat_sec_entry;

/*******************************************************************************
 * MTK_platform handler called when an affinity instance is about to be turned
 * on. The level and mpidr determine the affinity instance.
 ******************************************************************************/
static int plat_power_domain_on(u_register_t mpidr)
{
	int cpu = MPIDR_AFFLVL0_VAL(mpidr);

	if (mpidr & MPIDR_CLUSTER_MASK)
		return PSCI_E_INVALID_PARAMS;

	/* Write boot address for the CPU thats about to come online . */
	mcucfg_init_archstate(cpu, 1);
	mcucfg_set_bootaddr(cpu, plat_sec_entry);

	INFO("%s[%d], entry %lx\n", __func__, cpu, mcucfg_get_bootaddr(cpu));

	spm_poweron_cpu(cpu);

	return PSCI_E_SUCCESS;
}

/*******************************************************************************
 * MTK_platform handler called when an affinity instance has just been powered
 * on after being turned off earlier. The level and mpidr determine the affinity
 * instance. The 'state' arg. allows the platform to decide whether the cluster
 * was turned off prior to wakeup and do what's necessary to setup it up
 * correctly.
 ******************************************************************************/
static void plat_power_domain_on_finish(const psci_power_state_t *state)
{
	int cpu = MPIDR_AFFLVL0_VAL(read_mpidr_el1());

	assert(state->pwr_domain_state[MPIDR_AFFLVL0] == PLAT_MAX_OFF_STATE);

	spm_disable_cpu_auto_off(cpu);

	/* Enable the gic cpu interface */
	gicv2_cpuif_enable();
	gicv2_pcpu_distif_init();
}

/*******************************************************************************
 * MTK_platform handler called when an affinity instance is about to be turned
 * off. The level and mpidr determine the affinity instance. The 'state' arg.
 * allows the platform to decide whether the cluster is being turned off and
 * take apt actions.
 *
 * CAUTION: This function is called with coherent stacks so that caches can be
 * turned off, flushed and coherency disabled. There is no guarantee that caches
 * will remain turned on across calls to this function as each affinity level is
 * dealt with. So do not write & read global variables across calls. It will be
 * wise to do flush a write to the global to prevent unpredictable results.
 ******************************************************************************/
static void plat_power_domain_off(const psci_power_state_t *state)
{
	unsigned long mpidr = read_mpidr_el1();
	int cpu = MPIDR_AFFLVL0_VAL(mpidr);

	if (mpidr & MPIDR_CLUSTER_MASK)
		return;

	/* Prevent interrupts from spuriously waking up this cpu */
	gicv2_cpuif_disable();

	spm_enable_cpu_auto_off(cpu);
	spm_set_cpu_power_off(cpu);
}

static void __dead2 plat_system_reset(void)
{
	/* Write the System Configuration Control Register */
	INFO("MTK System Reset\n");

	wdt_trigger_reset();

	wfi();
	ERROR("MTK System Reset: operation not handled.\n");
	panic();
}

static void __dead2 plat_system_off(void)
{
	INFO("MTK System Off\n");

#if MT8516_HAVE_PMIC == 1
	rtc_bbpu_power_down();
#else
	wdt_trigger_reset();
#endif

	wfi();
	ERROR("MTK System Off: operation not handled.\n");
	panic();
}

/*******************************************************************************
 * Export the platform handlers to enable psci to invoke them
 ******************************************************************************/
static const plat_psci_ops_t plat_plat_psci_ops = {
	.pwr_domain_on = plat_power_domain_on,
	.pwr_domain_on_finish = plat_power_domain_on_finish,
	.pwr_domain_off = plat_power_domain_off,
	.system_off = plat_system_off,
	.system_reset = plat_system_reset,
};

/*******************************************************************************
 * Export the platform specific power ops.
 ******************************************************************************/
int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	*psci_ops = &plat_plat_psci_ops;
	plat_sec_entry = sec_entrypoint;

	return 0;
}
