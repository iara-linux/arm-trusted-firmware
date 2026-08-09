/*
 * Copyright (c) 2013-2019, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <arch.h>
#include <asm_macros.S>
#include <mt8163_def.h>

/*
 * These build options introduce funcs that run before
 * plat_reset_handler which clobber x4-x7.
 */
#if !PROGRAMMABLE_RESET_ADDRESS
#error "PROGRAMMABLE_RESET_ADDRESS != 1 is not supported."
#endif
#if !COLD_BOOT_SINGLE_CPU
#error "COLD_BOOT_SINGLE_CPU != 1 is not supported."
#endif

	.globl	platform_mem_init
	.globl	plat_secondary_cold_boot_setup
	.globl	plat_report_exception
	.globl	platform_is_primary_cpu
	.globl  plat_my_core_pos
	.globl	plat_reset_handler
	.globl	mtk_boot_arg_loc
	.globl	mtk_boot_arg_size
	.globl	mtk_bl33_start
	.globl	mtk_atf_arg

	/* --------------------------------------------------------
	 * void plat_reset_handler(void);
	 *
	 * Preloader enters BL31 with eight arguments, but TF-A's
	 * bl31_entrypoint only stashes x0-x3 before running the
	 * common EL3 init, which destroys the rest:
	 *
	 *   x4 = boot argument location  x6 = BL33 start address
	 *   x5 = boot argument size      x7 = atf_arg_t pointer
	 *
	 * We also need a one-shot flag, since call_reset_handler lives
	 * in el3_entrypoint_common, and bl31_warm_entrypoint invokes that
	 * too, so this also runs on every CPU_ON with x4-x7 holding whatever
	 * the waking core happened to have.
	 * --------------------------------------------------------
	 */
func plat_reset_handler
	/*
	 * We are allowed to clobber x0-x18, but x4-x7 are what we
	 * need to preserve, so use x16/x17 as scratch only.
	 */
	adrp	x16, mtk_boot_args_saved
	ldr	w17, [x16, :lo12:mtk_boot_args_saved]
	cbnz	w17, warm_boot_skip

	adrp	x16, mtk_boot_arg_loc
	str	w4, [x16, :lo12:mtk_boot_arg_loc]

	adrp	x16, mtk_boot_arg_size
	str	w5, [x16, :lo12:mtk_boot_arg_size]

	adrp	x16, mtk_bl33_start
	str	w6, [x16, :lo12:mtk_bl33_start]

	adrp	x16, mtk_atf_arg
	str	w7, [x16, :lo12:mtk_atf_arg]

	mov	w17, #1
	adrp	x16, mtk_boot_args_saved
	str	w17, [x16, :lo12:mtk_boot_args_saved]

warm_boot_skip:
	ret
endfunc plat_reset_handler

	/*
	 * These must live in .data, not .bss: el3_entrypoint_common zeroes
	 * BSS after call_reset_handler runs, so a BSS variable would be
	 * wiped.
	 */
	.section .data.mtk_boot_args, "aw", %progbits
	.align	3
mtk_boot_arg_loc:
	.word	0
mtk_boot_arg_size:
	.word	0
mtk_bl33_start:
	.word	0
mtk_atf_arg:
	.word	0
mtk_boot_args_saved:
	.word	0

	.text

	/* --------------------------------------------------------
	 * void platform_mem_init (void);
	 *
	 * Any memory init, relocation is done before by Preloader
	 * before BL31 boots. Called very early in the boot process.
	 * --------------------------------------------------------
	 */
func platform_mem_init
	mov	x0, #0
	ret
endfunc platform_mem_init

	/* -----------------------------------------------------
	 * void plat_secondary_cold_boot_setup (void);
	 *
	 * This function performs any platform specific actions
	 * needed for a secondary cpu after a cold reset e.g
	 * mark the cpu's presence, mechanism to place it in a
	 * holding pen etc.
	 * -----------------------------------------------------
	 */
func plat_secondary_cold_boot_setup
cb_panic:
	b	cb_panic
endfunc plat_secondary_cold_boot_setup

func platform_is_primary_cpu
	and	x0, x0, #(MPIDR_CLUSTER_MASK | MPIDR_CPU_MASK)
	cmp	x0, #MT8163_PRIMARY_CPU
	cset	x0, eq
	ret
endfunc platform_is_primary_cpu

	/* -----------------------------------------------------
	 * unsigned int plat_my_core_pos(void);
	 *
	 * result: CorePos = CoreId + (ClusterId << 2)
	 * -----------------------------------------------------
	 */
func plat_my_core_pos
	mrs     x0, mpidr_el1
	and     x1, x0, #MPIDR_CPU_MASK
	and     x0, x0, #MPIDR_CLUSTER_MASK
	add     x0, x1, x0, LSR #6
	ret
endfunc plat_my_core_pos
