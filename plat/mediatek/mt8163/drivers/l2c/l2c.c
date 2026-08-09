// SPDX-License-Identifier: BSD-3-Clause

#include <common/debug.h>
#include <lib/mmio.h>
#include <mt8163_def.h>
#include <devinfo.h>
#include <l2c.h>

void l2c_configure_caches(void)
{
	unsigned int cache_cfg0, cache_cfg1;

	/* mp0 L2$ 512KB */
	cache_cfg0 = mmio_read_32(MP0_CA7L_CACHE_CONFIG) | (CONFIGED_512K << L2C_SIZE_CFG_OFF);
	mmio_write_32(MP0_CA7L_CACHE_CONFIG, cache_cfg0);
	cache_cfg0 = mmio_read_32(MP0_CA7L_CACHE_CONFIG) & ~(0x1 << L2C_SHARE_ENABLE);
	mmio_write_32(MP0_CA7L_CACHE_CONFIG, cache_cfg0);

	/* set INFRA_L2C_SRAM_CG */
	mmio_write_32(INFRACFG_AO_BASE + 0x0088, 1 << 22);

	if (((get_devinfo_with_index(0) >> 4) & 0xff) == 0xc0) {
		/* mp1 L2$ 256KB */
		cache_cfg1 = mmio_read_32(MP1_CA7L_CACHE_CONFIG);
		cache_cfg1 &= ~(0xf << L2C_SIZE_CFG_OFF);
		cache_cfg1 |= (CONFIGED_256K << L2C_SIZE_CFG_OFF);
		mmio_write_32(MP1_CA7L_CACHE_CONFIG, cache_cfg1);
		cache_cfg1 = mmio_read_32(MP1_CA7L_CACHE_CONFIG) | (0x1 << L2C_SHARE_ENABLE);
		mmio_write_32(MP1_CA7L_CACHE_CONFIG, cache_cfg1);
	} else { 
		/* mp1 L2$ 512KB */
		cache_cfg1 = mmio_read_32(MP1_CA7L_CACHE_CONFIG) | (CONFIGED_512K << L2C_SIZE_CFG_OFF);
		mmio_write_32(MP1_CA7L_CACHE_CONFIG, cache_cfg1);
		cache_cfg1 = mmio_read_32(MP1_CA7L_CACHE_CONFIG) & ~(0x1 << L2C_SHARE_ENABLE);
		mmio_write_32(MP1_CA7L_CACHE_CONFIG, cache_cfg1);
	}
}
