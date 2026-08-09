/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021, MediaTek Inc. All rights reserved.
 */

#ifndef RNG_PLAT_H
#define RNG_PLAT_H

#include <platform_def.h>

#define MTK_RETRY_CNT        10
#define MTK_TIMEOUT_POLL     1000

/*******************************************************************************
 * TRNG related constants
 ******************************************************************************/
#define RNG_CTRL            (TRNG_BASE + 0x0000)
#define RNG_DATA            (TRNG_BASE + 0x0008)

#define RNG_EN              BIT(0)
#define RNG_READY           BIT(31)

#endif
