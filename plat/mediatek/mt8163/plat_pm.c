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
#include <lib/mmio.h>
#include <lib/psci/psci.h>
#include <plat/arm/common/plat_arm.h>

#include <mt8163_def.h>
#include <mt_cpuxgpt.h>
#include <mtcmos.h>
#include <rtc.h>
#include <scu.h>
#include <spm.h>
#include <spm_hotplug.h>
#include <spm_mcdi.h>
#include <spm_suspend.h>
#include <wdt.h>

#define MTK_PWR_LVL0			0
#define MTK_PWR_LVL1			1
#define MTK_PWR_LVL2			2

#define MTK_CORE_PWR_STATE(state)	(state)->pwr_domain_state[MTK_PWR_LVL0]
#define MTK_CLUSTER_PWR_STATE(state)	(state)->pwr_domain_state[MTK_PWR_LVL1]
#define MTK_SYSTEM_PWR_STATE(state)	(state)->pwr_domain_state[MTK_PWR_LVL2]

#define MTK_LOCAL_STATE_RUN		0
#define MTK_LOCAL_STATE_RET		PLAT_MAX_RET_STATE
#define MTK_LOCAL_STATE_OFF		PLAT_MAX_OFF_STATE

static uintptr_t plat_sec_entry;

struct core_context {
	unsigned long timer_data[8];
	unsigned int count;
	unsigned int rst;
	unsigned int abt;
	unsigned int brk;
};

static struct core_context cpu_context[PLATFORM_CLUSTER0_CORE_COUNT];

static struct core_context *get_core_data(unsigned long mpidr)
{
	return &cpu_context[mpidr & MPIDR_CPU_MASK];
}

static void mt_save_generic_timer(unsigned long *container)
{
	uint64_t ctl;
	uint64_t val;

	__asm__ volatile("mrs	%x0, cntkctl_el1\n\t"
			 "mrs	%x1, cntp_cval_el0\n\t"
			 "stp	%x0, %x1, [%2, #0]"
			 : "=&r" (ctl), "=&r" (val)
			 : "r" (container)
			 : "memory");

	__asm__ volatile("mrs	%x0, cntp_tval_el0\n\t"
			 "mrs	%x1, cntp_ctl_el0\n\t"
			 "stp	%x0, %x1, [%2, #16]"
			 : "=&r" (val), "=&r" (ctl)
			 : "r" (container)
			 : "memory");

	__asm__ volatile("mrs	%x0, cntv_tval_el0\n\t"
			 "mrs	%x1, cntv_ctl_el0\n\t"
			 "stp	%x0, %x1, [%2, #32]"
			 : "=&r" (val), "=&r" (ctl)
			 : "r" (container)
			 : "memory");
}

static void mt_restore_generic_timer(unsigned long *container)
{
	uint64_t ctl;
	uint64_t val;

	__asm__ volatile("ldp	%x0, %x1, [%2, #0]\n\t"
			 "msr	cntkctl_el1, %x0\n\t"
			 "msr	cntp_cval_el0, %x1"
			 : "=&r" (ctl), "=&r" (val)
			 : "r" (container)
			 : "memory");

	__asm__ volatile("ldp	%x0, %x1, [%2, #16]\n\t"
			 "msr	cntp_tval_el0, %x0\n\t"
			 "msr	cntp_ctl_el0, %x1"
			 : "=&r" (val), "=&r" (ctl)
			 : "r" (container)
			 : "memory");

	__asm__ volatile("ldp	%x0, %x1, [%2, #32]\n\t"
			 "msr	cntv_tval_el0, %x0\n\t"
			 "msr	cntv_ctl_el0, %x1"
			 : "=&r" (val), "=&r" (ctl)
			 : "r" (container)
			 : "memory");
}

static void stop_generic_timer(void)
{
	/*
	 * Disable the timer and mask the irq to prevent
	 * suprious interrupts on this cpu interface. It
	 * will bite us when we come back if we don't. It
	 * will be replayed on the inbound cluster.
	 */
	uint64_t cntpctl = read_cntp_ctl_el0();

	write_cntp_ctl_el0(clr_cntp_ctl_enable(cntpctl));
}

static void mt_cpu_save(unsigned long mpidr)
{
	struct core_context *core = get_core_data(mpidr);

	mt_save_generic_timer(core->timer_data);

	/* disable timer irq, and upper layer should enable it again. */
	stop_generic_timer();
}

static void mt_cpu_restore(unsigned long mpidr)
{
	struct core_context *core = get_core_data(mpidr);

	mt_restore_generic_timer(core->timer_data);
}

static void mt_platform_save_context(unsigned long mpidr)
{
	/* mcusys_save_context: */
	mt_cpu_save(mpidr);
}

static void mt_platform_restore_context(unsigned long mpidr)
{
	/* mcusys_restore_context: */
	mt_cpu_restore(mpidr);
}

/*******************************************************************************
 * MTK_platform handler called when an affinity instance is about to be turned
 * on. The level and mpidr determine the affinity instance.
 ******************************************************************************/
static int plat_power_domain_on(u_register_t mpidr)
{
	unsigned long cpu_id = mpidr & MPIDR_CPU_MASK;
	uintptr_t rv;

	if (mpidr & MPIDR_CLUSTER_MASK)
		return PSCI_E_INVALID_PARAMS;

	rv = (uintptr_t)MP0_MISC_CONFIG_BOOT_ADDR(cpu_id);
	mmio_write_32(rv, plat_sec_entry);

	INFO("%s[%ld], entry %x\n", __func__, cpu_id, mmio_read_32(rv));

	spm_hotplug_on(mpidr);
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
	assert(state->pwr_domain_state[MPIDR_AFFLVL0] == MTK_LOCAL_STATE_OFF);

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

	if (mpidr & MPIDR_CLUSTER_MASK)
		return;

	/* Prevent interrupts from spuriously waking up this cpu */
	gicv2_cpuif_disable();

	spm_hotplug_off(mpidr);
}

static void plat_cpu_standby(plat_local_state_t cpu_state)
{
	u_register_t scr;

	scr = read_scr_el3();
	write_scr_el3(scr | SCR_IRQ_BIT);
	isb();
	dsb();
	wfi();
	write_scr_el3(scr);
}

/*******************************************************************************
 * MTK_platform handler called when an affinity instance is about to be
 * suspended. The level and mpidr determine the affinity instance. The 'state'
 * arg. allows the platform to decide whether the cluster is being turned off
 * and take apt actions.
 *
 * CAUTION: This function is called with coherent stacks so that caches can be
 * turned off, flushed and coherency disabled. There is no guarantee that caches
 * will remain turned on across calls to this function as each affinity level is
 * dealt with. So do not write & read global variables across calls. It will be
 * wise to do flush a write to the global to prevent unpredictable results.
 ******************************************************************************/
static void plat_power_domain_suspend(const psci_power_state_t *state)
{
	unsigned long mpidr = read_mpidr_el1();
	unsigned long cpu_id = mpidr & MPIDR_CPU_MASK;
	uintptr_t rv;

	/* Nothing to do for a plain retention request. */
	if (MTK_CORE_PWR_STATE(state) != MTK_LOCAL_STATE_OFF)
		return;

	rv = (uintptr_t)MP0_MISC_CONFIG_BOOT_ADDR(cpu_id);
	mmio_write_32(rv, plat_sec_entry);

	if (MTK_SYSTEM_PWR_STATE(state) != MTK_LOCAL_STATE_OFF)
		spm_mcdi_prepare(mpidr);

	mt_platform_save_context(mpidr);

	if (MTK_SYSTEM_PWR_STATE(state) == MTK_LOCAL_STATE_OFF) {
		wdt_suspend();
		disable_scu(mpidr);
		generic_timer_backup();
		spm_system_suspend();
		/* Prevent interrupts from spuriously waking up this cpu */
		gicv2_cpuif_disable();
	}
}

/*******************************************************************************
 * MTK_platform handler called when an affinity instance has just been powered
 * on after having been suspended earlier. The level and mpidr determine the
 * affinity instance.
 ******************************************************************************/
static void plat_power_domain_suspend_finish(const psci_power_state_t *state)
{
	unsigned long mpidr = read_mpidr_el1();

	if (state->pwr_domain_state[MTK_PWR_LVL0] == MTK_LOCAL_STATE_RET)
		return;

	if (MTK_SYSTEM_PWR_STATE(state) == MTK_LOCAL_STATE_OFF) {
		/* Enable the gic cpu interface */
		plat_arm_gic_init();
		spm_system_suspend_finish();
		enable_scu(mpidr);
		wdt_resume();
	}

	mt_platform_restore_context(mpidr);

	if (MTK_SYSTEM_PWR_STATE(state) != MTK_LOCAL_STATE_OFF)
		spm_mcdi_finish(mpidr);

	gicv2_pcpu_distif_init();
}

static void plat_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	unsigned int i;

	for (i = MTK_PWR_LVL0; i <= PLAT_MAX_PWR_LVL; i++)
		req_state->pwr_domain_state[i] = MTK_LOCAL_STATE_OFF;
}

static int plat_validate_power_state(unsigned int power_state,
					psci_power_state_t *req_state)
{
	int pstate = psci_get_pstate_type(power_state);
	int pwr_lvl = psci_get_pstate_pwrlvl(power_state);
	int i;

	assert(req_state);

	if (pwr_lvl > PLAT_MAX_PWR_LVL)
		return PSCI_E_INVALID_PARAMS;

	/* Sanity check the requested state */
	if (pstate == PSTATE_TYPE_STANDBY) {
		/*
		 * It's possible to enter standby only on power level 0
		 * Ignore any other power level.
		 */
		if (pwr_lvl != 0)
			return PSCI_E_INVALID_PARAMS;

		req_state->pwr_domain_state[MTK_PWR_LVL0] =
					MTK_LOCAL_STATE_RET;
	} else {
		for (i = 0; i <= pwr_lvl; i++)
			req_state->pwr_domain_state[i] =
					MTK_LOCAL_STATE_OFF;
	}

	/*
	 * We expect the 'state id' to be zero.
	 */
	if (psci_get_pstate_id(power_state))
		return PSCI_E_INVALID_PARAMS;

	return PSCI_E_SUCCESS;
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

	rtc_bbpu_power_down();

	wfi();
	ERROR("MTK System Off: operation not handled.\n");
	panic();
}

/*******************************************************************************
 * Export the platform handlers to enable psci to invoke them
 ******************************************************************************/
static const plat_psci_ops_t plat_plat_psci_ops = {
	.cpu_standby = plat_cpu_standby,
	.pwr_domain_on = plat_power_domain_on,
	.pwr_domain_on_finish = plat_power_domain_on_finish,
	.pwr_domain_off = plat_power_domain_off,
	.pwr_domain_suspend = plat_power_domain_suspend,
	.pwr_domain_suspend_finish = plat_power_domain_suspend_finish,
	.get_sys_suspend_power_state = plat_get_sys_suspend_power_state,
	.validate_power_state = plat_validate_power_state,
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
