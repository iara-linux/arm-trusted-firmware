#
# Copyright (c) 2015-2022, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
#
# SPDX-License-Identifier: BSD-3-Clause
#

MTK_PLAT		:=	plat/mediatek
MTK_PLAT_SOC		:=	${MTK_PLAT}/${PLAT}

# True Random Number Generator firmware Interface
TRNG_SUPPORT := 1

PLAT_INCLUDES		:=	-I${MTK_PLAT}/common/				\
				-I${MTK_PLAT}/include/				\
				-Iinclude/plat/arm/common/aarch64		\
				-I${MTK_PLAT_SOC}/drivers/devapc/		\
				-I${MTK_PLAT_SOC}/drivers/devinfo/		\
				-I${MTK_PLAT_SOC}/drivers/l2c/			\
				-I${MTK_PLAT_SOC}/drivers/mtcmos/		\
				-I${MTK_PLAT_SOC}/drivers/pmic/			\
				-I${MTK_PLAT}/drivers/pmic_wrap/		\
				-I${MTK_PLAT_SOC}/drivers/rtc/			\
				-I${MTK_PLAT_SOC}/drivers/spm/			\
				-I${MTK_PLAT_SOC}/drivers/timer/		\
				-I${MTK_PLAT_SOC}/drivers/wdt/			\
				-I${MTK_PLAT_SOC}/include/

PLAT_BL_COMMON_SOURCES	:=	lib/xlat_tables/xlat_tables_common.c		\
				lib/xlat_tables/aarch64/xlat_tables.c		\
				plat/common/plat_gicv2_base.c			\
				plat/common/plat_gicv2.c			\
				plat/common/aarch64/crash_console_helpers.S

BL31_SOURCES		+=	common/desc_image_load.c			\
				drivers/arm/gic/common/gic_common.c		\
				drivers/arm/gic/v2/gicv2_main.c			\
				drivers/arm/gic/v2/gicv2_helpers.c		\
				drivers/delay_timer/delay_timer.c		\
				drivers/delay_timer/generic_delay_timer.c	\
				drivers/ti/uart/aarch64/16550_console.S		\
				lib/cpus/aarch64/cortex_a53.S			\
				plat/common/plat_psci_common.c			\
				${MTK_PLAT}/common/mtk_plat_common.c		\
				${MTK_PLAT_SOC}/aarch64/plat_helpers.S		\
				${MTK_PLAT_SOC}/aarch64/platform_common.c	\
				${MTK_PLAT_SOC}/drivers/devapc/devapc.c		\
				${MTK_PLAT_SOC}/drivers/devinfo/devinfo.c	\
				${MTK_PLAT_SOC}/drivers/l2c/l2c.c		\
				${MTK_PLAT}/drivers/pmic_wrap/pmic_wrap_init.c	\
				${MTK_PLAT_SOC}/drivers/mtcmos/mtcmos.c		\
				${MTK_PLAT_SOC}/drivers/pmic/pmic.c		\
				${MTK_PLAT}/drivers/rtc/rtc_common.c		\
				${MTK_PLAT_SOC}/drivers/rtc/rtc.c		\
				${MTK_PLAT_SOC}/drivers/spm/spm.c		\
				${MTK_PLAT_SOC}/drivers/spm/spm_hotplug.c	\
				${MTK_PLAT_SOC}/drivers/spm/spm_mcdi.c		\
				${MTK_PLAT_SOC}/drivers/spm/spm_suspend.c	\
				${MTK_PLAT_SOC}/drivers/timer/mt_cpuxgpt.c	\
				${MTK_PLAT_SOC}/drivers/wdt/wdt.c		\
				${MTK_PLAT_SOC}/bl31_plat_setup.c		\
				${MTK_PLAT_SOC}/plat_pm.c			\
				${MTK_PLAT_SOC}/plat_topology.c			\
				${MTK_PLAT_SOC}/scu.c

ifeq (${TRNG_SUPPORT},1)
BL31_SOURCES += ${MTK_PLAT}/drivers/rng/rng.c					\
		${MTK_PLAT}/drivers/rng/${PLAT}/rng_plat.c
endif

# Enable workarounds for selected Cortex-A53 erratas.
ERRATA_A53_836870	:=	1
ERRATA_A53_843419	:=	1
ERRATA_A53_855873	:=	1
ERRATA_A53_1530924	:=	1

# indicate the reset vector address can be programmed
PROGRAMMABLE_RESET_ADDRESS	:=	1

# Preloader does not boot secondary CPUs, we start on a
# single CPU.
COLD_BOOT_SINGLE_CPU	:=	1

# Secondary CPUs will enter into BL31.
RESET_TO_BL31		:=	1

# Do not enable SVE
ENABLE_SVE_FOR_NS	:=	0

# Preloader places LK at this address.
PRELOADED_BL33_BASE	?=	0x4BD00000
$(eval $(call add_define,PRELOADED_BL33_BASE))

# Fallback to this address for BL32 entry
# if it cannot be retrieved from Preloader
# ATF arguments.
ifeq ($(SPD),opteed)
PRELOADED_BL32_BASE	?=	0x43200000
else
PRELOADED_BL32_BASE	?=	0x43200000
endif
$(eval $(call add_define,PRELOADED_BL32_BASE))

# Preloader places ATF at this address.
TZRAM_BASE		?=	0x43000000
$(eval $(call add_define,TZRAM_BASE))

# These defaults will leave the UART baud and clock
# configured to whatever is set up by Preloader.
MT8163_BAUDRATE		?=	0
MT8163_UART_CLOCK	?=	0
$(eval $(call add_define,MT8163_BAUDRATE))
$(eval $(call add_define,MT8163_UART_CLOCK))
