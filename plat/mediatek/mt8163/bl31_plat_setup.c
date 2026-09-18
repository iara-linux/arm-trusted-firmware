/*
 * Copyright (c) 2013-2019, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <common/bl_common.h>
#include <common/debug.h>
#include <common/desc_image_load.h>
#include <drivers/generic_delay_timer.h>
#include <drivers/ti/uart/uart_16550.h>
#include <lib/mmio.h>
#include <lib/utils.h>
#include <plat/arm/common/plat_arm.h>
#include <plat/common/common_def.h>
#include <plat/common/platform.h>

#include <bootargs.h>
#include <devapc.h>
#include <l2c.h>
#include <pmic.h>
#include <mtcmos.h>
#include <mtk_plat_common.h>
#include <plat_private.h>
#include <scu.h>
#include <spm.h>

static entry_point_info_t bl32_ep_info;
static entry_point_info_t bl33_ep_info;

static void platform_setup_cpu(void)
{
	/* set LITTLE cores arm64 boot mode */
	mmio_setbits_32(MP0_MISC_CONFIG3, MP0_CPUCFG_64BIT);

	/* turn off all the little core's power except cpu 0 */
	mtcmos_little_cpu_off();
}

/*******************************************************************************
 * Return a pointer to the 'entry_point_info' structure of the next image for
 * the security state specified. BL33 corresponds to the non-secure image type
 * while BL32 corresponds to the secure image type. A NULL pointer is returned
 * if the image does not exist.
 ******************************************************************************/
entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info;

	next_image_info = (type == NON_SECURE) ? &bl33_ep_info : &bl32_ep_info;
	assert(next_image_info->h.type == PARAM_EP);

	/* None of the images on this platform can have 0x0 as the entrypoint */
	if (next_image_info->pc)
		return next_image_info;
	else
		return NULL;
}

/******************************************************************************
 * Returns Preloader's ATF arguments, or NULL if it doesn't look valid.
 *****************************************************************************/
static const struct mt_atf_args *mtk_get_atf_args(void)
{
	const struct mt_atf_args *args;

	args = (const struct mt_atf_args *)(uintptr_t)mtk_atf_arg;
	if ((args != NULL) && (args->atf_magic == ATF_ARGS_MAGIC))
		return args;

	/* Try looking for the args at TZRAM_BASE in that case. */
	args = (const struct mt_atf_args *)(uintptr_t)TZRAM_BASE;
	if (args->atf_magic == ATF_ARGS_MAGIC) {
		INFO("Preloader args found at TZRAM_BASE (x7: 0x%08x)\n",
		     mtk_atf_arg);
		return args;
	}

	WARN("No valid Preloader argument block (magic 0x%08x)\n", args->atf_magic);
	return NULL;
}

/******************************************************************************
 * Gets SPSR for BL32 entry
 *****************************************************************************/
static uint32_t mtk_get_spsr_for_bl32_entry(void)
{
	/*
	 * The Secure Payload Dispatcher service is responsible for
	 * setting the SPSR prior to entry into the BL32 image.
	 */
	return 0;
}

/******************************************************************************
 * Gets SPSR for BL33 entry
 *****************************************************************************/
static uint32_t mtk_get_spsr_for_bl33_entry(void)
{
	unsigned long el_status;
	unsigned int mode;
	uint32_t spsr;

	/* Figure out what mode we enter the non-secure world in */
	el_status = read_id_aa64pfr0_el1() >> ID_AA64PFR0_EL2_SHIFT;
	el_status &= ID_AA64PFR0_ELX_MASK;

	mode = (el_status) ? MODE_EL2 : MODE_EL1;

	/*
	 * TODO: Consider the possibility of specifying the SPSR in
	 * the FIP ToC and allowing the platform to have a say as
	 * well.
	 */
	spsr = SPSR_64(mode, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);
	return spsr;
}

/*******************************************************************************
 * Perform any BL31 early platform setup. Here is an opportunity to copy
 * parameters passed by the calling EL (S-EL1 in BL2 & S-EL3 in BL1) before they
 * are lost (potentially). This needs to be done before the MMU is initialized
 * so that the memory layout can be used while creating page tables.
 * BL2 has flushed this information to memory, so we are guaranteed to pick up
 * good data.
 ******************************************************************************/
void bl31_early_platform_setup2(u_register_t arg0, u_register_t arg1,
				u_register_t arg2, u_register_t arg3)
{
	static console_t console;
	const struct mt_atf_args *args;

	l2c_configure_caches();

	console_16550_register(MT8163_UART0_BASE, MT8163_UART_CLOCK, MT8163_BAUDRATE, &console);

	args = mtk_get_atf_args();

	/* Set-up the BL32 entry point if provided. */
	zeromem(&bl32_ep_info, sizeof(bl32_ep_info));
	SET_PARAM_HEAD(&bl32_ep_info, PARAM_EP, VERSION_1, 0);
	SET_SECURITY_STATE(bl32_ep_info.h.attr, SECURE);

	/* If we have valid args passed by PL, use them. */
	if (args && args->tee_support && args->tee_entry) {
		INFO("PL: BL32 entry address from Preloader: 0x%08x\n", args->tee_entry);
		bl32_ep_info.pc = args->tee_entry;
	} else {
		INFO("PL: BL32 entry address not provided, use 0x%08x\n",
		     (uint32_t)PRELOADED_BL32_BASE);
		bl32_ep_info.pc = PRELOADED_BL32_BASE;
	}

	if (bl32_ep_info.pc == 0)
		INFO("BL31: Running without BL32.\n");

	/*
	 * Set-up the BL33 entry point, fall back to our defaults if PL
	 * doesn't provide one.
	 */
	zeromem(&bl33_ep_info, sizeof(bl33_ep_info));
	SET_PARAM_HEAD(&bl33_ep_info, PARAM_EP, VERSION_1, 0);
	SET_SECURITY_STATE(bl33_ep_info.h.attr, NON_SECURE);

	if (mtk_bl33_start >= DRAM_BASE) {
		INFO("PL: BL33 entry address from Preloader: 0x%08x\n", mtk_bl33_start);
		bl33_ep_info.pc = mtk_bl33_start;
	} else {
		WARN("PL: Bad address for BL33 entry (0x%08x), use 0x%08x\n",
		     mtk_bl33_start, (uint32_t)PRELOADED_BL33_BASE);
		bl33_ep_info.pc = PRELOADED_BL33_BASE;
	}

	/* Hand the Preloader's boot arguments down to BL33. */
	if ((mtk_boot_arg_loc >= DRAM_BASE) && (mtk_boot_arg_size != 0U)) {
		bl33_ep_info.args.arg4 = (u_register_t)mtk_boot_arg_loc;
		bl33_ep_info.args.arg5 = (u_register_t)mtk_boot_arg_size;
		INFO("PL: Preloader boot arguments: 0x%08x sz 0x%08x\n",
		     mtk_boot_arg_loc, mtk_boot_arg_size);
	} else {
		WARN("PL: Bad size or address for PL bootargs (0x%08x size 0x%08x)\n",
		     mtk_boot_arg_loc, mtk_boot_arg_size);
	}

	bl32_ep_info.spsr = mtk_get_spsr_for_bl32_entry();
	bl33_ep_info.spsr = mtk_get_spsr_for_bl33_entry();
}

/*******************************************************************************
 * Perform any BL31 platform setup code
 ******************************************************************************/
void bl31_platform_setup(void)
{
	platform_setup_cpu();

	generic_delay_timer_init();

	/* Initialize the gic cpu and distributor interfaces */
	plat_arm_gic_driver_init();
	plat_arm_gic_init();

	/* Configure peripherals access permissions */
	devapc_init();

	/* Initialize hardware peripherals */
	pmic_probe();

	/* Initialize spm at boot time */
	spm_boot_init();
}

/******************************************************************************
 * Perform the very early platform specific architectural setup here. At the
 * moment this is only initializes the mmu in a quick and dirty way.
 ******************************************************************************/
void bl31_plat_arch_setup(void)
{
	enable_scu(read_mpidr());

	plat_configure_mmu_el3(BL_CODE_BASE,
			       BL_COHERENT_RAM_END - BL_CODE_BASE,
			       BL_CODE_BASE,
			       BL_CODE_END,
			       BL_COHERENT_RAM_BASE,
			       BL_COHERENT_RAM_END);
}
