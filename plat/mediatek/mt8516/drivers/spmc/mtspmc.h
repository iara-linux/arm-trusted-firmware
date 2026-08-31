/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MTSPMC_H
#define MTSPMC_H

#include <stdint.h>

int spmc_init(void);

void spm_poweron_cpu(int cpu);
void spm_poweroff_cpu(int cpu);

int spm_get_cpu_powerstate(int cpu);

void spm_enable_cpu_auto_off(int cpu);
void spm_disable_cpu_auto_off(int cpu);
void spm_set_cpu_power_off(int cpu);

void mcucfg_init_archstate(int cpu, int arm64);
void mcucfg_set_bootaddr(int cpu, uintptr_t bootaddr);
uintptr_t mcucfg_get_bootaddr(int cpu);

#endif /* MTSPMC_H */
