// SPDX-License-Identifier: BSD-3-Clause

#include <common/debug.h>
#include <lib/mmio.h>
#include <mt8516_def.h>
#include <l2c.h>

void l2c_configure_caches()
{
	unsigned int cache_cfg0;

	/* MT8167: L2$ 512KB;*/
	cache_cfg0 = mmio_read_32(MP0_CA7L_CACHE_CONFIG) & ~(0xF << L2C_SIZE_CFG_OFF);
	cache_cfg0 |= (CONFIGED_512K << L2C_SIZE_CFG_OFF);
	mmio_write_32(MP0_CA7L_CACHE_CONFIG, cache_cfg0);

	cache_cfg0 = mmio_read_32(MP0_CA7L_CACHE_CONFIG) & ~(0x1 << L2C_SHARE_ENABLE);
	mmio_write_32(MP0_CA7L_CACHE_CONFIG, cache_cfg0);
}
