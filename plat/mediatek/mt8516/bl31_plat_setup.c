/*
 * Copyright (c) 2013-2019, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <inttypes.h>

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
#include <l2c.h>
#include <mtk_plat_common.h>
#include <mtspmc.h>
#include <plat_private.h>
#include <scu.h>

void plat_mt_gic_init(void);

static entry_point_info_t bl32_ep_info;
static entry_point_info_t bl33_ep_info;

static void platform_setup_cpu(void)
{
	unsigned int i;

	/* set LITTLE cores arm64 boot mode */
	for (i = 0; i < PLATFORM_CLUSTER0_CORE_COUNT; i++)
		mcucfg_init_archstate(i, 1);

	spmc_init();
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
	const struct mtk_bl_param *pl_args;
	const struct mt_atf_args *args;

	l2c_configure_caches();

	console_16550_register(MT8516_UART0_BASE, MT8516_UART_CLOCK, MT8516_BAUDRATE, &console);

#if MT8516_HAVE_PMIC != 1
	INFO("PMIC: This BL31 is not built with PMIC support.\n");
#endif

	pl_args = (const struct mtk_bl_param *)((uint64_t)arg0 & 0xffffffff);
	args = (const struct mt_atf_args *)pl_args->tee_info_addr;

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

	if (pl_args->bl33_start_addr >= DRAM_BASE) {
		INFO("PL: BL33 entry address from Preloader: 0x%" PRIx64 "\n", pl_args->bl33_start_addr);
		bl33_ep_info.pc = pl_args->bl33_start_addr;
	} else {
		WARN("PL: Bad address for BL33 entry (0x%" PRIx64 "), use 0x%08x\n",
		     pl_args->bl33_start_addr, (uint32_t)PRELOADED_BL33_BASE);
		bl33_ep_info.pc = PRELOADED_BL33_BASE;
	}

	/* Hand the Preloader's boot arguments down to BL33. */
	if ((pl_args->bootarg_loc >= DRAM_BASE) && (pl_args->bootarg_size != 0U)) {
		bl33_ep_info.args.arg4 = (u_register_t)pl_args->bootarg_loc;
		bl33_ep_info.args.arg5 = (u_register_t)pl_args->bootarg_size;
		INFO("PL: Preloader boot arguments: 0x%" PRIx64 " sz 0x%" PRIx64 "\n",
		     pl_args->bootarg_loc, pl_args->bootarg_size);
	} else {
		WARN("PL: Bad size or address for PL bootargs (0x%" PRIx64 " size 0x%" PRIx64 ")\n",
		     pl_args->bootarg_loc, pl_args->bootarg_size);
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
	plat_mt_gic_init();
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
