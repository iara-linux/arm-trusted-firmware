/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLAT_PRIVATE_H
#define PLAT_PRIVATE_H

/*******************************************************************************
 * Function and variable prototypes
 ******************************************************************************/
void plat_configure_mmu_el3(unsigned long total_base,
			    unsigned long total_size,
			    unsigned long,
			    unsigned long,
			    unsigned long,
			    unsigned long);

/* Declarations for plat_topology.c */
int mt_setup_topology(void);

/* Boot arguments Preloader passes in via x4-x7. */
extern uint32_t mtk_boot_arg_loc;
extern uint32_t mtk_boot_arg_size;
extern uint32_t mtk_bl33_start;
extern uint32_t mtk_atf_arg;

#endif /* PLAT_PRIVATE_H */
