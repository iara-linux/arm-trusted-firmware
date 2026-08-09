/*
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PMIC_H
#define PMIC_H

#include <stdint.h>

/* Chip ID Register and Masks */
#define MT6323_CID		0x0100
#define MT6323_CID_MASK		0xFFFF
#define MT6323_CID_E1		0x1023
#define MT6323_CID_E2		0x2023

int pmic_probe(void);

#endif /* PMIC_H */
