/*
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <lib/mmio.h>

#include <platform_def.h>
#include <devapc.h>

#if DEBUG
static const char *devapc_perm_str[] = {
	[NO_PROTECTION] = "NO_PROTECTION",
	[SEC_RW_ONLY] = "SEC_RW_ONLY",
	[SEC_RW_NS_R] = "SEC_RW_NS_R",
	[FORBIDDEN] = "FORBIDDEN",
};
#endif

static const struct devapc_slave_cfg devapc_slaves[] = {
	PROT_MODULE(INFRASYS_BOOTROM_SRAM,       FORBIDDEN),
	PROT_MODULE(INFRASYS_EFUSEC,             SEC_RW_NS_R),
	PROT_MODULE(INFRA_AO_SEJ,                SEC_RW_ONLY),
	PROT_MODULE(INFRASYS_TRNG,               SEC_RW_ONLY),
	PROT_MODULE(INFRASYS_GCPU,               SEC_RW_ONLY),
	PROT_MODULE(INFRA_AO_DEVICE_APC_AO,      SEC_RW_ONLY),
	PROT_MODULE(INFRASYS_DEVICE_APC_MONITOR, SEC_RW_ONLY),
	PROT_MODULE(DEBUGSYS,                    FORBIDDEN),
	PROT_MODULE(INFRASYS_MBIST_CONTROL_REG,  FORBIDDEN),
	PROT_MODULE(INFRA_AO_RSVD_1,             FORBIDDEN),
	PROT_MODULE(INFRA_AO_RSVD_2,             FORBIDDEN),
	PROT_MODULE(INFRASYS_RSVD_3,             FORBIDDEN),
	PROT_MODULE(INFRASYS_RSVD_4,             FORBIDDEN),
	PROT_MODULE(INFRA_AO_RSVD_5,             FORBIDDEN),
	PROT_MODULE(INFRA_AO_RSVD_6,             FORBIDDEN),
};

static void set_module_apc(enum devapc_slave module, enum devapc_domain domain,
			   enum devapc_perm perm)
{
	uint32_t apc_index = module / MOD_NO_IN_1_DEVAPC;
	uint32_t apc_shift = (module % MOD_NO_IN_1_DEVAPC) * 2;
	uintptr_t base;

	base = DEVAPC_D0_APC_0 + (domain * DEVAPC_DOMAIN_STRIDE) +
	       (apc_index * 4);

	mmio_clrsetbits_32(base, 0x3U << apc_shift,
			   ((uint32_t)perm) << apc_shift);
}

static void devapc_slaves_init(void)
{
	unsigned int i;

	for (i = 0U; i < ARRAY_SIZE(devapc_slaves); i++) {
		set_module_apc(devapc_slaves[i].module, DEVAPC_DOMAIN_AP,
			       devapc_slaves[i].ap_perm);

#if DEBUG
		INFO("DEVAPC: set protection on %s (%s)\n",
		     devapc_slaves[i].name,
		     devapc_perm_str[devapc_slaves[i].ap_perm]);
#endif
	}
}

void devapc_init(void)
{
	/* Disable devapc before permission update */
	mmio_write_32(DEVAPC_APC_CON, 0x0U);

	/* Set-up permissions for slaves */
	devapc_slaves_init();

	/* Enable devapc */
	mmio_setbits_32(DEVAPC_APC_CON, DEVAPC_APC_CON_EN);
}
