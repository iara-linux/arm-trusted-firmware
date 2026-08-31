/*
 * Copyright (c) 2014-2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOOTARGS_H
#define _BOOTARGS_H

#define DEVINFO_SIZE		4
#define ATF_ARGS_MAGIC		0x4d415446

struct mtk_bl_param {
	uint64_t bootarg_loc;
	uint64_t bootarg_size;
	uint64_t bl33_start_addr;
	uint64_t tee_info_addr;
};

struct mt_atf_args {
	uint32_t atf_magic;
	uint32_t tee_support;
	uint32_t tee_entry;
	uint64_t tee_boot_arg_addr;
	uint32_t hwuid[4];     /* HW Unique id for t-base used */
	uint32_t atf_hrid_size; /* Check this atf_hrid_size to read from HRID array */
	uint32_t HRID[8];      /* HW random id for t-base used */
	uint32_t atf_log_port;
	uint32_t atf_log_baudrate;
	uint64_t atf_log_buf_start;
	uint32_t atf_log_buf_size;
	uint32_t atf_irq_num;
	uint32_t devinfo[DEVINFO_SIZE];
	uint64_t atf_aee_debug_buf_start;
	uint32_t atf_aee_debug_buf_size;
	uint32_t msg_fde_key[4]; /* size of message auth key is 16bytes(128 bits) */
//#if CFG_TEE_SUPPORT
	uint32_t tee_rpmb_size;
//#endif
};

#endif
