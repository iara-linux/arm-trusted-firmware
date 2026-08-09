/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __SPM_MCDI_H__
#define __SPM_MCDI_H__

void spm_mcdi_wakeup_all_cores(void);
void spm_mcdi_wfi_sel_enter(unsigned long mpidr);
void spm_mcdi_wfi_sel_leave(unsigned long mpidr);
void spm_mcdi_prepare(unsigned long mpidr);
void spm_mcdi_finish(unsigned long mpidr);

#endif /* __SPM_MCDI_H__ */
