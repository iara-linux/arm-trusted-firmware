/*
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DEVAPC_H
#define DEVAPC_H

#include <stdint.h>

enum devapc_domain {
	DEVAPC_DOMAIN_AP = 0,
	DEVAPC_DOMAIN_MD,
	DEVAPC_DOMAIN_CONN,
	DEVAPC_DOMAIN_MD32,
};

enum devapc_perm {
	NO_PROTECTION = 0,
	SEC_RW_ONLY,
	SEC_RW_NS_R,
	FORBIDDEN,
};

enum devapc_slave {
	DEVAPC_SLAVE_INFRA_AO_TOP_LEVEL_CLOCK_GENERATOR = 0,
	DEVAPC_SLAVE_INFRA_AO_INFRASYS_CONFIG_REGS      = 1,
	DEVAPC_SLAVE_IO_CONFIGURATION                   = 2,
	DEVAPC_SLAVE_INFRA_AO_PERISYS_CONFIG_REGS       = 3,
	DEVAPC_SLAVE_INFRA_AO_DRAMC_CONTROLLER          = 4,
	DEVAPC_SLAVE_INFRA_AO_GPIO_CONTROLLER           = 5,
	DEVAPC_SLAVE_INFRA_AO_TOP_LEVEL_SLP_MANAGER     = 6,
	DEVAPC_SLAVE_INFRA_AO_TOP_LEVEL_RESET_GENERATOR = 7,
	DEVAPC_SLAVE_INFRA_AO_GPT                       = 8,
	DEVAPC_SLAVE_INFRA_AO_RSVD_1                    = 9,
	DEVAPC_SLAVE_INFRA_AO_SEJ                       = 10,
	DEVAPC_SLAVE_INFRA_AO_APMCU_EINT_CONTROLLER     = 11,
	DEVAPC_SLAVE_INFRASYS_AP_MIXED_CONTROL_REG      = 12,
	DEVAPC_SLAVE_INFRA_AO_PMIC_WRAP_CONTROL_REG     = 13,
	DEVAPC_SLAVE_INFRA_AO_DEVICE_APC_AO             = 14,
	DEVAPC_SLAVE_INFRA_AO_DDRPHY_CONTROL_REG        = 15,
	DEVAPC_SLAVE_INFRA_AO_KPAD_CONTROL_REG          = 16,
	DEVAPC_SLAVE_TOP_MISC_REG                       = 17,
	DEVAPC_SLAVE_INFRA_AO_RSVD_2                    = 18,
	DEVAPC_SLAVE_INFRA_AO_CEC_CONTROL_REG           = 19,
	DEVAPC_SLAVE_INFRA_AO_IRRX_CONTROL_REG          = 20,
	DEVAPC_SLAVE_INFRA_AO_SPM_MD32                  = 21,
	DEVAPC_SLAVE_INFRASYS_MCUSYS_CONFIG_REG         = 22,
	DEVAPC_SLAVE_INFRASYS_CONTROL_REG               = 23,
	DEVAPC_SLAVE_INFRASYS_BOOTROM_SRAM              = 24,
	DEVAPC_SLAVE_INFRASYS_EMI_BUS_INTERFACE         = 25,
	DEVAPC_SLAVE_INFRASYS_SYSTEM_CIRQ               = 26,
	DEVAPC_SLAVE_INFRASYS_MM_IOMMU_CONFIGURATION    = 27,
	DEVAPC_SLAVE_INFRASYS_EFUSEC                    = 28,
	DEVAPC_SLAVE_INFRASYS_DEVICE_APC_MONITOR        = 29,
	DEVAPC_SLAVE_INFRASYS_MCU_BIU_CONFIGURATION     = 30,
	DEVAPC_SLAVE_INFRASYS_RSVD_3                    = 31,
	DEVAPC_SLAVE_INFRASYS_CA7_AP_CCIF               = 32,
	DEVAPC_SLAVE_INFRASYS_CA7_MD_CCIF               = 33,
	DEVAPC_SLAVE_INFRASYS_RSVD_4                    = 34,
	DEVAPC_SLAVE_INFRASYS_MBIST_CONTROL_REG         = 35,
	DEVAPC_SLAVE_INFRASYS_DRAMC_NAO_REGION_REG      = 36,
	DEVAPC_SLAVE_INFRASYS_TRNG                      = 37,
	DEVAPC_SLAVE_INFRASYS_GCPU                      = 38,
	DEVAPC_SLAVE_INFRA_AO_RSVD_5                    = 39,
	DEVAPC_SLAVE_INFRASYS_GCE                       = 40,
	DEVAPC_SLAVE_INFRA_AO_RSVD_6                    = 41,
	DEVAPC_SLAVE_INFRASYS_PERISYS_IOMMU             = 42,
	DEVAPC_SLAVE_INFRA_AO_ANA_MIPI_DSI0             = 43,
	DEVAPC_SLAVE_INFRA_AO_ANA_MIPI_DSI1             = 44,
	DEVAPC_SLAVE_INFRA_AO_ANA_MIPI_CSI0             = 45,
	DEVAPC_SLAVE_INFRA_AO_ANA_MIPI_CSI1             = 46,
	DEVAPC_SLAVE_INFRA_AO_ANA_MIPI_CSI2             = 47,
	DEVAPC_SLAVE_DEBUGSYS                           = 48,
	DEVAPC_SLAVE_DMA                                = 49,
	DEVAPC_SLAVE_AUXADC                             = 50,
	DEVAPC_SLAVE_UART0                              = 51,
	DEVAPC_SLAVE_UART1                              = 52,
	DEVAPC_SLAVE_UART2                              = 53,
	DEVAPC_SLAVE_UART3                              = 54,
	DEVAPC_SLAVE_PWM                                = 55,
	DEVAPC_SLAVE_I2C0                               = 56,
	DEVAPC_SLAVE_I2C1                               = 57,
	DEVAPC_SLAVE_I2C2                               = 58,
	DEVAPC_SLAVE_SPI0                               = 59,
	DEVAPC_SLAVE_PTP                                = 60,
	DEVAPC_SLAVE_BTIF                               = 61,
	DEVAPC_SLAVE_NFI                                = 62,
	DEVAPC_SLAVE_NFI_ECC                            = 63,
	DEVAPC_SLAVE_I2C3                               = 64,
	DEVAPC_SLAVE_UFOZIP                             = 65,
	DEVAPC_SLAVE_USB2_0                             = 66,
	DEVAPC_SLAVE_USB2_0_SIF                         = 67,
	DEVAPC_SLAVE_AUDIO                              = 68,
	DEVAPC_SLAVE_MSDC0                              = 69,
	DEVAPC_SLAVE_MSDC1                              = 70,
	DEVAPC_SLAVE_MSDC2                              = 71,
	DEVAPC_SLAVE_MSDC3                              = 72,
	DEVAPC_SLAVE_USB20_1                            = 73,
	DEVAPC_SLAVE_CONN_PERIPHERALS                   = 74,
	DEVAPC_SLAVE_NIC_WRAP                           = 75,
	DEVAPC_SLAVE_WCN_AHB_SLAVE                      = 76,
	DEVAPC_SLAVE_MJC_CONFIG                         = 77,
	DEVAPC_SLAVE_MJC_TOP                            = 78,
	DEVAPC_SLAVE_SMI_LARB5                          = 79,
	DEVAPC_SLAVE_MFG_CONFIG                         = 80,
	DEVAPC_SLAVE_G3D_CONFIG                         = 81,
	DEVAPC_SLAVE_G3D_UX_CMN                         = 82,
	DEVAPC_SLAVE_G3D_UX_DW0                         = 83,
	DEVAPC_SLAVE_G3D_MX                             = 84,
	DEVAPC_SLAVE_G3D_IOMMU                          = 85,
	DEVAPC_SLAVE_MALI                               = 86,
	DEVAPC_SLAVE_MMSYS_CONFIG                       = 87,
	DEVAPC_SLAVE_MDP_RDMA                           = 88,
	DEVAPC_SLAVE_MDP_RSZ0                           = 89,
	DEVAPC_SLAVE_MDP_RSZ1                           = 90,
	DEVAPC_SLAVE_MDP_WDMA                           = 91,
	DEVAPC_SLAVE_MDP_WROT                           = 92,
	DEVAPC_SLAVE_MDP_TDSHP                          = 93,
	DEVAPC_SLAVE_DISP_OVL0                          = 94,
	DEVAPC_SLAVE_DISP_OVL1                          = 95,
	DEVAPC_SLAVE_DISP_RDMA0                         = 96,
	DEVAPC_SLAVE_DISP_RDMA1                         = 97,
	DEVAPC_SLAVE_DISP_WDMA                          = 98,
	DEVAPC_SLAVE_DISP_COLOR                         = 99,
	DEVAPC_SLAVE_DISP_CCORR                         = 100,
	DEVAPC_SLAVE_DISP_AAL                           = 101,
	DEVAPC_SLAVE_DISP_GAMMA                         = 102,
	DEVAPC_SLAVE_DISP_DITHER                        = 103,
	DEVAPC_SLAVE_DSI_UFOE                           = 104,
	DEVAPC_SLAVE_DSI0                               = 105,
	DEVAPC_SLAVE_DPI0                               = 106,
	DEVAPC_SLAVE_DISP_PWM0                          = 107,
	DEVAPC_SLAVE_MM_MUTEX                           = 108,
	DEVAPC_SLAVE_SMI_LARB0                          = 109,
	DEVAPC_SLAVE_SMI_COMMON                         = 110,
	DEVAPC_SLAVE_DISP_WDMA1                         = 111,
	DEVAPC_SLAVE_UFOD_RDMA0                         = 112,
	DEVAPC_SLAVE_UFOD_RDMA1                         = 113,
	DEVAPC_SLAVE_LVDS                               = 114,
	DEVAPC_SLAVE_DPI1                               = 115,
	DEVAPC_SLAVE_HDMI                               = 116,
	DEVAPC_SLAVE_DISP_DSC                           = 117,
	DEVAPC_SLAVE_IMGSYS_CONFIG                      = 118,
	DEVAPC_SLAVE_IMGSYS_SMI_LARB2                   = 119,
	DEVAPC_SLAVE_IMGSYS_SMI_LARB2_2                 = 120,
	DEVAPC_SLAVE_IMGSYS_SMI_LARB2_3                 = 121,
	DEVAPC_SLAVE_IMGSYS_CAM0                        = 122,
	DEVAPC_SLAVE_IMGSYS_CAM1                        = 123,
	DEVAPC_SLAVE_IMGSYS_CAM2                        = 124,
	DEVAPC_SLAVE_IMGSYS_CAM3                        = 125,
	DEVAPC_SLAVE_IMGSYS_SENINF                      = 126,
	DEVAPC_SLAVE_IMGSYS_CAMSV                       = 127,
	DEVAPC_SLAVE_IMGSYS_CAMSV_2                     = 128,
	DEVAPC_SLAVE_IMGSYS_FD                          = 129,
	DEVAPC_SLAVE_IMGSYS_MIPI_RX                     = 130,
	DEVAPC_SLAVE_CAM0                               = 131,
	DEVAPC_SLAVE_CAM2                               = 132,
	DEVAPC_SLAVE_CAM3                               = 133,
	DEVAPC_SLAVE_VDECSYS_GLOBAL_CONFIGURATION       = 134,
	DEVAPC_SLAVE_VDECSYS_SMI_LARB1                  = 135,
	DEVAPC_SLAVE_VDEC_FULL_TOP                      = 136,
	DEVAPC_SLAVE_VENC_GLOBAL_CON                    = 137,
	DEVAPC_SLAVE_SMI_LARB3                          = 138,
	DEVAPC_SLAVE_VENC                               = 139,
	DEVAPC_SLAVE_JPGENC                             = 140,
	DEVAPC_SLAVE_JPGDEC                             = 141,
	DEVAPC_SLAVE_NUM,
};

struct devapc_slave_cfg {
#if DEBUG
	const char *name;
#endif
	enum devapc_slave module;
	enum devapc_perm ap_perm;
};

#if DEBUG
#define PROT_MODULE(mod, perm)				\
	(struct devapc_slave_cfg)	{		\
		.name = #mod,				\
		.module = DEVAPC_SLAVE_##mod,		\
		.ap_perm = perm,			\
	}
#else
#define PROT_MODULE(mod, perm)				\
	(struct devapc_slave_cfg)	{		\
		.module = DEVAPC_SLAVE_##mod,		\
		.ap_perm = perm,			\
	}
#endif

void devapc_init(void);

#endif /* DEVAPC_H */
