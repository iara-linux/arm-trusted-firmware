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

struct mt_atf_args {
	uint32_t atf_magic;
	uint32_t tee_support;
	uint32_t tee_entry;
	uint32_t tee_boot_arg_addr;
	uint32_t hwuid[4];
	uint32_t hrid[2];
	uint32_t atf_log_port;
	uint32_t atf_log_baudrate;
	uint32_t atf_log_buf_start;
	uint32_t atf_log_buf_size;
	uint32_t atf_irq_num;
	uint32_t devinfo[DEVINFO_SIZE];
	uint32_t atf_aee_debug_buf_start;
	uint32_t atf_aee_debug_buf_size;
};

#endif
