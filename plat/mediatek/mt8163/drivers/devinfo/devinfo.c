// SPDX-License-Identifier: BSD-3-Clause

#include <common/debug.h>
#include <lib/mmio.h>
#include <mt8163_def.h>
#include <devinfo.h>

static const uint32_t devinfo_addrs[] = {
	0x10206020, 0x10206030, 0x10206038, 0x10206040,
	0x10206044, 0x10206048, 0x10206060, 0x10206100,
	0x10206104, 0x10206108, 0x10206120, 0x10206130,
	0x10206140, 0x10206144, 0x10206170, 0x10206174,
	0x10206178, 0x1020617C, 0x10206180, 0x10206184,
	0x08000000, 0x10206188, 0x102061B0, 0x102061B4,
	0x102061B8, 0x102061BC, 0x102061C0, 0x102061C4,
	0x102061C8, 0x102061CC, 0x1020604C, 0x10206050,
	0x10206054, 0x10206090, 0x10206094, 0x10206098,
	0x1020609C, 0x102060A0, 0x102060A4, 0x102060A8,
	0x102060AC,
};

uint32_t get_devinfo_with_index(uint32_t index)
{
	if (index >= ARRAY_SIZE(devinfo_addrs)) {
		ERROR("Invalid devinfo index %u\n", index);
		return 0xFFFFFFFF;
	}

	return mmio_read_32(devinfo_addrs[index]);
}
