/*
 * Copyright (c) 2026 Ben Grisdale <bengris32@protonmail.ch>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>

#include <pmic.h>
#include <pmic_wrap_init.h>

static int pmic_config_interface(uint32_t reg, uint32_t val,
				 uint32_t mask, uint32_t shift)
{
	uint32_t data;
	int32_t ret;

	ret = pwrap_read(reg, &data);
	if (ret != 0)
		return ret;

	data = data & ~(mask << shift);
	data |= (val << shift);
	ret = pwrap_write(reg, data);

	return ret;
}

static int pmic_init_setting_6323_e2(void)
{
	int ret = 0;

	ret |= pmic_config_interface(0x2, 0xB, 0xF, 4);
	/* [7:4]: RG_VCDT_HV_VTH; 7V OVP */
	/*VBAT_OV_VTH, 4.45V,*/
	ret |= pmic_config_interface(0x000C, 0x3, 0x7, 1);
	ret |= pmic_config_interface(0x1A, 0x3, 0xF, 0);
	/* [3:0]: RG_CHRWDT_TD; align to 6250's */
	ret |= pmic_config_interface(0x24, 0x1, 0x1, 1);
	/* [1:1]: RG_BC11_RST; Reset BC11 detection */
	ret |= pmic_config_interface(0x2A, 0x0, 0x7, 4);
	/* [6:4]: RG_CSDAC_STP; align to 6250's setting */
	ret |= pmic_config_interface(0x2E, 0x1, 0x1, 2);
	/* [2:2]: RG_CSDAC_MODE; */
	ret |= pmic_config_interface(0x2E, 0x1, 0x1, 6);
	/* [6:6]: RG_HWCV_EN; */
	ret |= pmic_config_interface(0x2E, 0x1, 0x1, 7);
	/* [7:7]: RG_ULC_DET_EN; */
	ret |= pmic_config_interface(0x3C, 0x1, 0x1, 5);
	/* [5:5]: THR_HWPDN_EN; */
	ret |= pmic_config_interface(0x40, 0x1, 0x1, 4);
	/* [4:4]: RG_EN_DRVSEL; */
	ret |= pmic_config_interface(0x40, 0x1, 0x1, 5);
	/* [5:5]: RG_RST_DRVSEL; */
	ret |= pmic_config_interface(0x46, 0x1, 0x1, 1);
	/* [1:1]: PWRBB_DEB_EN; */
	ret |= pmic_config_interface(0x48, 0x1, 0x1, 8);
	/* [8:8]: VPROC_PG_H2L_EN; */
	ret |= pmic_config_interface(0x48, 0x1, 0x1, 9);
	/* [9:9]: VSYS_PG_H2L_EN; */
	ret |= pmic_config_interface(0x4E, 0x1, 0x1, 5);
	/* [5:5]: STRUP_AUXADC_RSTB_SW; */
	ret |= pmic_config_interface(0x4E, 0x1, 0x1, 7);
	/* [7:7]: STRUP_AUXADC_RSTB_SEL; */
	ret |= pmic_config_interface(0x50, 0x1, 0x1, 0);
	/* [0:0]: STRUP_PWROFF_SEQ_EN; */
	ret |= pmic_config_interface(0x50, 0x1, 0x1, 1);
	/* [1:1]: STRUP_PWROFF_PREOFF_EN; */
	ret |= pmic_config_interface(0x52, 0x1, 0x1, 9);
	/* [9:9]: SPK_THER_SHDN_L_EN; */
	ret |= pmic_config_interface(0x56, 0x1, 0x1, 0);
	/* [0:0]: RG_SPK_INTG_RST_L; */
	ret |= pmic_config_interface(0x64, 0x1, 0xF, 8);
	/* [11:8]: RG_SPKPGA_GAIN; */
	ret |= pmic_config_interface(0x102, 0x1, 0x1, 6);
	/* [6:6]: RG_RTC_75K_CK_PDN; */
	ret |= pmic_config_interface(0x102, 0x0, 0x1, 11);
	/* [11:11]: RG_DRV_32K_CK_PDN; */
	ret |= pmic_config_interface(0x102, 0x1, 0x1, 15);
	/* [15:15]: RG_BUCK32K_PDN; */
	ret |= pmic_config_interface(0x108, 0x1, 0x1, 12);
	/* [12:12]: RG_EFUSE_CK_PDN; */
	ret |= pmic_config_interface(0x10E, 0x1, 0x1, 5);
	/* [5:5]: RG_AUXADC_CTL_CK_PDN; */
	ret |= pmic_config_interface(0x120, 0x1, 0x1, 4);
	/* [4:4]: RG_SRCLKEN_HW_MODE; */
	ret |= pmic_config_interface(0x120, 0x1, 0x1, 5);
	/* [5:5]: RG_OSC_HW_MODE; */
	ret |= pmic_config_interface(0x148, 0x1, 0x1, 1);
	/* [1:1]: RG_SMT_INT; */
	ret |= pmic_config_interface(0x148, 0x1, 0x1, 3);
	/* [3:3]: RG_SMT_RTC_32K1V8; */
	ret |= pmic_config_interface(0x160, 0x1, 0x1, 2);
	/* [2:2]: MT6323_IRQ_EN_BAT_L; */
	ret |= pmic_config_interface(0x160, 0x1, 0x1, 6);
	/* [6:6]: MT6323_IRQ_EN_THR_L; */
	ret |= pmic_config_interface(0x160, 0x1, 0x1, 7);
	/* [7:7]: MT6323_IRQ_EN_THR_H; */
	ret |= pmic_config_interface(0x166, 0x1, 0x1, 1);
	/* [1:1]: MT6323_IRQ_EN_FCHRKEY; */
	ret |= pmic_config_interface(0x212, 0x2, 0x3, 4);
	/* [5:4]: QI_VPROC_VSLEEP; */
	ret |= pmic_config_interface(0x216, 0x1, 0x1, 1);
	/* [1:1]: VPROC_VOSEL_CTRL; */
	ret |= pmic_config_interface(0x21C, 0x17, 0x7F, 0);
	/* [6:0]: VPROC_SFCHG_FRATE; */
	ret |= pmic_config_interface(0x21C, 0x1, 0x1, 7);
	/* [7:7]: VPROC_SFCHG_FEN; */
	ret |= pmic_config_interface(0x21C, 0x1, 0x1, 15);
	/* [15:15]: VPROC_SFCHG_REN; */
	ret |= pmic_config_interface(0x222, 0x18, 0x7F, 0);
	/* [6:0]: VPROC_VOSEL_SLEEP; */
	ret |= pmic_config_interface(0x230, 0x3, 0x3, 0);
	/* [1:0]: VPROC_TRANSTD; */
	ret |= pmic_config_interface(0x230, 0x1, 0x1, 8);
	/* [8:8]: VPROC_VSLEEP_EN; */
	ret |= pmic_config_interface(0x238, 0x3, 0x3, 0);
	/* [1:0]: RG_VSYS_SLP; */
	ret |= pmic_config_interface(0x238, 0x2, 0x3, 4);
	/* [5:4]: QI_VSYS_VSLEEP; */
	ret |= pmic_config_interface(0x23C, 0x1, 0x1, 1);
	/* [1:1]: VSYS_VOSEL_CTRL; after 0x0256 */
	ret |= pmic_config_interface(0x242, 0x23, 0x7F, 0);
	/* [6:0]: VSYS_SFCHG_FRATE; */
	ret |= pmic_config_interface(0x242, 0x11, 0x7F, 8);
	/* [14:8]: VSYS_SFCHG_RRATE; */
	ret |= pmic_config_interface(0x242, 0x1, 0x1, 15);
	/* [15:15]: VSYS_SFCHG_REN; */
	ret |= pmic_config_interface(0x256, 0x3, 0x3, 0);
	/* [1:0]: VSYS_TRANSTD; */
	ret |= pmic_config_interface(0x256, 0x1, 0x3, 4);
	/* [5:4]: VSYS_VOSEL_TRANS_EN; */
	ret |= pmic_config_interface(0x256, 0x1, 0x1, 8);
	/* [8:8]: VSYS_VSLEEP_EN; */
	ret |= pmic_config_interface(0x302, 0x2, 0x3, 8);
	/* [9:8]: RG_VPA_CSL; OC limit */
	ret |= pmic_config_interface(0x302, 0x1, 0x3, 14);
	/* [15:14]: RG_VPA_ZX_OS; ZX limit */
	ret |= pmic_config_interface(0x310, 0x1, 0x1, 7);
	/* [7:7]: VPA_SFCHG_FEN; */
	ret |= pmic_config_interface(0x310, 0x1, 0x1, 15);
	/* [15:15]: VPA_SFCHG_REN; */
	ret |= pmic_config_interface(0x326, 0x1, 0x1, 0);
	/* [0:0]: VPA_DLC_MAP_EN; */
	ret |= pmic_config_interface(0x402, 0x1, 0x1, 0);
	/* [0:0]: VTCXO_LP_SEL; */
	ret |= pmic_config_interface(0x402, 0x0, 0x1, 11);
	/* [11:11]: VTCXO_ON_CTRL; */
	ret |= pmic_config_interface(0x404, 0x1, 0x1, 0);
	/* [0:0]: VA_LP_SEL; */
	ret |= pmic_config_interface(0x404, 0x2, 0x3, 8);
	/* [9:8]: RG_VA_SENSE_SEL; */
	ret |= pmic_config_interface(0x500, 0x1, 0x1, 0);
	/* [0:0]: VIO28_LP_SEL; */
	ret |= pmic_config_interface(0x502, 0x1, 0x1, 0);
	/* [0:0]: VUSB_LP_SEL; */
	ret |= pmic_config_interface(0x504, 0x1, 0x1, 0);
	/* [0:0]: VMC_LP_SEL; */
	ret |= pmic_config_interface(0x506, 0x1, 0x1, 0);
	/* [0:0]: VMCH_LP_SEL; */
	ret |= pmic_config_interface(0x508, 0x1, 0x1, 0);
	/* [0:0]: VEMC_3V3_LP_SEL; */
	ret |= pmic_config_interface(0x514, 0x1, 0x1, 0);
	/* [0:0]: RG_STB_SIM1_SIO; */
	ret |= pmic_config_interface(0x516, 0x1, 0x1, 0);
	/* [0:0]: VSIM1_LP_SEL; */
	ret |= pmic_config_interface(0x518, 0x1, 0x1, 0);
	/* [0:0]: VSIM2_LP_SEL; */
	ret |= pmic_config_interface(0x524, 0x1, 0x1, 0);
	/* [0:0]: RG_STB_SIM2_SIO; */
	ret |= pmic_config_interface(0x542, 0x1, 0x1, 2);
	/* [2:2]: VIBR_THER_SHEN_EN; */
	ret |= pmic_config_interface(0x54E, 0x1, 0x1, 15);
	/* [15:15]: RG_VRF18_EN; */
	ret |= pmic_config_interface(0x550, 0x0, 0x1, 1);
	/* [1:1]: VRF18_ON_CTRL; */
	ret |= pmic_config_interface(0x552, 0x1, 0x1, 0);
	/* [0:0]: VM_LP_SEL; */
	ret |= pmic_config_interface(0x552, 0x1, 0x1, 14);
	/* [14:14]: RG_VM_EN; */
	ret |= pmic_config_interface(0x556, 0x1, 0x1, 0);
	/* [0:0]: VIO18_LP_SEL; */
	ret |= pmic_config_interface(0x756, 0x1, 0x3, 2);
	/* [3:2]: RG_ADC_TRIM_CH6_SEL; */
	ret |= pmic_config_interface(0x756, 0x1, 0x3, 4);
	/* [5:4]: RG_ADC_TRIM_CH5_SEL; */
	ret |= pmic_config_interface(0x756, 0x1, 0x3, 8);
	/* [9:8]: RG_ADC_TRIM_CH3_SEL; */
	ret |= pmic_config_interface(0x756, 0x1, 0x3, 10);
	/* [11:10]: RG_ADC_TRIM_CH2_SEL; */
	ret |= pmic_config_interface(0x778, 0x1, 0x1, 15);
	/* [15:15]: RG_VREF18_ENB_MD; */

	/* upmu_set_rg_adc_gps_status(1); */
	ret |= pmic_config_interface(0x0768, 0x1, 0x1, 1);
	/* upmu_set_rg_adc_md_status(1); */
	ret |= pmic_config_interface(0x0778, 0x1, 0x1, 0);
	/* upmu_set_rg_deci_gdly_vref18_selb(1); */
	ret |= pmic_config_interface(0x0776, 0x1, 0x1, 1);
	/* upmu_set_rg_deci_gdly_sel_mode(1); */
	ret |= pmic_config_interface(0x0776, 0x1, 0x1, 0);
	/* upmu_set_rg_osr(3); */
	ret |= pmic_config_interface(0x0754, 0x3, 0x7, 10);
	/* upmu_set_rg_chrwdt_en(0); */
	ret |= pmic_config_interface(0x001A, 0x0, 0x1, 4);
	/* upmu_set_rg_chrwdt_int_en(0); */
	ret |= pmic_config_interface(0x001E, 0x0, 0x1, 0);
	/* upmu_set_rg_chrwdt_wr(0); */
	ret |= pmic_config_interface(0x001A, 0x0, 0x1, 8);
	/* upmu_set_rg_chrwdt_flag_wr(1); */
	ret |= pmic_config_interface(0x001E, 0x1, 0x1, 1);

	return ret;
}

static int pmic_init_setting_6323_e1(void)
{
	int ret = 0;

	ret |= pmic_config_interface(0x2, 0xB, 0xF, 4);
	/* [7:4]: RG_VCDT_HV_VTH; 7V OVP */
	/*VBAT_OV_VTH, 4.45V,*/
	ret |= pmic_config_interface(0x000C, 0x3, 0x7, 1);
	ret |= pmic_config_interface(0x1A, 0x3, 0xF, 0);
	/* [3:0]: RG_CHRWDT_TD; align to 6250's */
	ret |= pmic_config_interface(0x24, 0x1, 0x1, 1);
	/* [1:1]: RG_BC11_RST; Reset BC11 detection */
	ret |= pmic_config_interface(0x2A, 0x0, 0x7, 4);
	/* [6:4]: RG_CSDAC_STP; align to 6250's setting */
	ret |= pmic_config_interface(0x2E, 0x1, 0x1, 2);
	/* [2:2]: RG_CSDAC_MODE; */
	ret |= pmic_config_interface(0x2E, 0x1, 0x1, 6);
	/* [6:6]: RG_HWCV_EN; */
	ret |= pmic_config_interface(0x2E, 0x1, 0x1, 7);
	/* [7:7]: RG_ULC_DET_EN; */
	ret |= pmic_config_interface(0x3C, 0x1, 0x1, 5);
	/* [5:5]: THR_HWPDN_EN; */
	ret |= pmic_config_interface(0x40, 0x1, 0x1, 4);
	/* [4:4]: RG_EN_DRVSEL; */
	ret |= pmic_config_interface(0x40, 0x1, 0x1, 5);
	/* [5:5]: RG_RST_DRVSEL; */
	ret |= pmic_config_interface(0x46, 0x1, 0x1, 1);
	/* [1:1]: PWRBB_DEB_EN; */
	ret |= pmic_config_interface(0x48, 0x1, 0x1, 8);
	/* [8:8]: VPROC_PG_H2L_EN; */
	ret |= pmic_config_interface(0x48, 0x1, 0x1, 9);
	/* [9:9]: VSYS_PG_H2L_EN; */
	ret |= pmic_config_interface(0x4E, 0x1, 0x1, 5);
	/* [5:5]: STRUP_AUXADC_RSTB_SW; */
	ret |= pmic_config_interface(0x4E, 0x1, 0x1, 7);
	/* [7:7]: STRUP_AUXADC_RSTB_SEL; */
	ret |= pmic_config_interface(0x50, 0x1, 0x1, 0);
	/* [0:0]: STRUP_PWROFF_SEQ_EN; */
	ret |= pmic_config_interface(0x50, 0x1, 0x1, 1);
	/* [1:1]: STRUP_PWROFF_PREOFF_EN; */
	ret |= pmic_config_interface(0x52, 0x1, 0x1, 9);
	/* [9:9]: SPK_THER_SHDN_L_EN; */
	ret |= pmic_config_interface(0x56, 0x1, 0x1, 0);
	/* [0:0]: RG_SPK_INTG_RST_L; */
	ret |= pmic_config_interface(0x64, 0x1, 0xF, 8);
	/* [11:8]: RG_SPKPGA_GAIN; */
	ret |= pmic_config_interface(0x102, 0x1, 0x1, 6);
	/* [6:6]: RG_RTC_75K_CK_PDN; */
	ret |= pmic_config_interface(0x102, 0x0, 0x1, 11);
	/* [11:11]: RG_DRV_32K_CK_PDN; */
	ret |= pmic_config_interface(0x102, 0x1, 0x1, 15);
	/* [15:15]: RG_BUCK32K_PDN; */
	ret |= pmic_config_interface(0x108, 0x1, 0x1, 12);
	/* [12:12]: RG_EFUSE_CK_PDN; */
	ret |= pmic_config_interface(0x10E, 0x1, 0x1, 5);
	/* [5:5]: RG_AUXADC_CTL_CK_PDN; */
	ret |= pmic_config_interface(0x120, 0x1, 0x1, 4);
	/* [4:4]: RG_SRCLKEN_HW_MODE; */
	ret |= pmic_config_interface(0x120, 0x1, 0x1, 5);
	/* [5:5]: RG_OSC_HW_MODE; */
	ret |= pmic_config_interface(0x148, 0x1, 0x1, 1);
	/* [1:1]: RG_SMT_INT; */
	ret |= pmic_config_interface(0x148, 0x1, 0x1, 3);
	/* [3:3]: RG_SMT_RTC_32K1V8; */
	ret |= pmic_config_interface(0x160, 0x1, 0x1, 2);
	/* [2:2]: MT6323_IRQ_EN_BAT_L; */
	ret |= pmic_config_interface(0x160, 0x1, 0x1, 6);
	/* [6:6]: MT6323_IRQ_EN_THR_L; */
	ret |= pmic_config_interface(0x160, 0x1, 0x1, 7);
	/* [7:7]: MT6323_IRQ_EN_THR_H; */
	ret |= pmic_config_interface(0x166, 0x1, 0x1, 1);
	/* [1:1]: MT6323_IRQ_EN_FCHRKEY; */
	ret |= pmic_config_interface(0x212, 0x2, 0x3, 4);
	/* [5:4]: QI_VPROC_VSLEEP; */
	ret |= pmic_config_interface(0x216, 0x1, 0x1, 1);
	/* [1:1]: VPROC_VOSEL_CTRL; */
	ret |= pmic_config_interface(0x21C, 0x17, 0x7F, 0);
	/* [6:0]: VPROC_SFCHG_FRATE; */
	ret |= pmic_config_interface(0x21C, 0x1, 0x1, 7);
	/* [7:7]: VPROC_SFCHG_FEN; */
	ret |= pmic_config_interface(0x21C, 0x1, 0x1, 15);
	/* [15:15]: VPROC_SFCHG_REN; */
	ret |= pmic_config_interface(0x222, 0x18, 0x7F, 0);
	/* [6:0]: VPROC_VOSEL_SLEEP; */
	ret |= pmic_config_interface(0x230, 0x3, 0x3, 0);
	/* [1:0]: VPROC_TRANSTD; */
	ret |= pmic_config_interface(0x230, 0x1, 0x1, 8);
	/* [8:8]: VPROC_VSLEEP_EN; */
	ret |= pmic_config_interface(0x238, 0x3, 0x3, 0);
	/* [1:0]: RG_VSYS_SLP; */
	ret |= pmic_config_interface(0x238, 0x2, 0x3, 4);
	/* [5:4]: QI_VSYS_VSLEEP; */
	ret |= pmic_config_interface(0x23C, 0x1, 0x1, 1);
	/* [1:1]: VSYS_VOSEL_CTRL; after 0x0256 */
	ret |= pmic_config_interface(0x242, 0x23, 0x7F, 0);
	/* [6:0]: VSYS_SFCHG_FRATE; */
	ret |= pmic_config_interface(0x242, 0x11, 0x7F, 8);
	/* [14:8]: VSYS_SFCHG_RRATE; */
	ret |= pmic_config_interface(0x242, 0x1, 0x1, 15);
	/* [15:15]: VSYS_SFCHG_REN; */
	ret |= pmic_config_interface(0x256, 0x3, 0x3, 0);
	/* [1:0]: VSYS_TRANSTD; */
	ret |= pmic_config_interface(0x256, 0x1, 0x3, 4);
	/* [5:4]: VSYS_VOSEL_TRANS_EN; */
	ret |= pmic_config_interface(0x256, 0x1, 0x1, 8);
	/* [8:8]: VSYS_VSLEEP_EN; */
	ret |= pmic_config_interface(0x302, 0x2, 0x3, 8);
	/* [9:8]: RG_VPA_CSL; OC limit */
	ret |= pmic_config_interface(0x302, 0x1, 0x3, 14);
	/* [15:14]: RG_VPA_ZX_OS; ZX limit */
	ret |= pmic_config_interface(0x310, 0x1, 0x1, 7);
	/* [7:7]: VPA_SFCHG_FEN; */
	ret |= pmic_config_interface(0x310, 0x1, 0x1, 15);
	/* [15:15]: VPA_SFCHG_REN; */
	ret |= pmic_config_interface(0x326, 0x1, 0x1, 0);
	/* [0:0]: VPA_DLC_MAP_EN; */
	ret |= pmic_config_interface(0x402, 0x1, 0x1, 0);
	/* [0:0]: VTCXO_LP_SEL; */
	ret |= pmic_config_interface(0x402, 0x0, 0x1, 11);
	/* [11:11]: VTCXO_ON_CTRL; */
	ret |= pmic_config_interface(0x404, 0x1, 0x1, 0);
	/* [0:0]: VA_LP_SEL; */
	ret |= pmic_config_interface(0x404, 0x2, 0x3, 8);
	/* [9:8]: RG_VA_SENSE_SEL; */
	ret |= pmic_config_interface(0x500, 0x1, 0x1, 0);
	/* [0:0]: VIO28_LP_SEL; */
	ret |= pmic_config_interface(0x502, 0x1, 0x1, 0);
	/* [0:0]: VUSB_LP_SEL; */
	ret |= pmic_config_interface(0x504, 0x1, 0x1, 0);
	/* [0:0]: VMC_LP_SEL; */
	ret |= pmic_config_interface(0x506, 0x1, 0x1, 0);
	/* [0:0]: VMCH_LP_SEL; */
	ret |= pmic_config_interface(0x508, 0x1, 0x1, 0);
	/* [0:0]: VEMC_3V3_LP_SEL; */
	ret |= pmic_config_interface(0x514, 0x1, 0x1, 0);
	/* [0:0]: RG_STB_SIM1_SIO; */
	ret |= pmic_config_interface(0x516, 0x1, 0x1, 0);
	/* [0:0]: VSIM1_LP_SEL; */
	ret |= pmic_config_interface(0x518, 0x1, 0x1, 0);
	/* [0:0]: VSIM2_LP_SEL; */
	ret |= pmic_config_interface(0x524, 0x1, 0x1, 0);
	/* [0:0]: RG_STB_SIM2_SIO; */
	ret |= pmic_config_interface(0x542, 0x1, 0x1, 2);
	/* [2:2]: VIBR_THER_SHEN_EN; */
	ret |= pmic_config_interface(0x54E, 0x1, 0x1, 15);
	/* [15:15]: RG_VRF18_EN; */
	ret |= pmic_config_interface(0x550, 0x0, 0x1, 1);
	/* [1:1]: VRF18_ON_CTRL; */
	ret |= pmic_config_interface(0x552, 0x1, 0x1, 0);
	/* [0:0]: VM_LP_SEL; */
	ret |= pmic_config_interface(0x552, 0x1, 0x1, 14);
	/* [14:14]: RG_VM_EN; */
	ret |= pmic_config_interface(0x556, 0x1, 0x1, 0);
	/* [0:0]: VIO18_LP_SEL; */
	ret |= pmic_config_interface(0x756, 0x1, 0x3, 2);
	/* [3:2]: RG_ADC_TRIM_CH6_SEL; */
	ret |= pmic_config_interface(0x756, 0x1, 0x3, 4);
	/* [5:4]: RG_ADC_TRIM_CH5_SEL; */
	ret |= pmic_config_interface(0x756, 0x1, 0x3, 8);
	/* [9:8]: RG_ADC_TRIM_CH3_SEL; */
	ret |= pmic_config_interface(0x756, 0x1, 0x3, 10);
	/* [11:10]: RG_ADC_TRIM_CH2_SEL; */
	ret |= pmic_config_interface(0x778, 0x1, 0x1, 15);
	/* [15:15]: RG_VREF18_ENB_MD; */

	/* upmu_set_rg_adc_gps_status(1); */
	ret |= pmic_config_interface(0x0768, 0x1, 0x1, 1);
	/* upmu_set_rg_adc_md_status(1); */
	ret |= pmic_config_interface(0x0778, 0x1, 0x1, 0);
	/* upmu_set_rg_deci_gdly_vref18_selb(1); */
	ret |= pmic_config_interface(0x0776, 0x1, 0x1, 1);
	/* upmu_set_rg_deci_gdly_sel_mode(1); */
	ret |= pmic_config_interface(0x0776, 0x1, 0x1, 0);
	/* upmu_set_rg_osr(3); */
	ret |= pmic_config_interface(0x0754, 0x3, 0x7, 10);
	/* upmu_set_rg_chrwdt_en(0); */
	ret |= pmic_config_interface(0x001A, 0x0, 0x1, 4);
	/* upmu_set_rg_chrwdt_int_en(0); */
	ret |= pmic_config_interface(0x001E, 0x0, 0x1, 0);
	/* upmu_set_rg_chrwdt_wr(0); */
	ret |= pmic_config_interface(0x001A, 0x0, 0x1, 8);
	/* upmu_set_rg_chrwdt_flag_wr(1); */
	ret |= pmic_config_interface(0x001E, 0x1, 0x1, 1);

	return ret;
}

int pmic_probe(void)
{
	uint32_t cid = 0;
	int32_t ret;

	ret = pwrap_read(MT6323_CID, &cid);
	if (ret != 0) {
		ERROR("PMIC: pwrap_read failed, ret=%d\n", ret);
		return ret;
	}

	cid &= MT6323_CID_MASK;

	if (cid == MT6323_CID_E1) {
		ret = pmic_init_setting_6323_e1();
	} else if (cid == MT6323_CID_E2) {
		ret = pmic_init_setting_6323_e2();
	} else {
		ERROR("PMIC: unexpected chip ID 0x%04x\n", cid);
		return -1;
	}

	if (ret)
		ERROR("PMIC: Failed to apply init settings: %d\n", ret);

	INFO("PMIC: MT6323 chip ID 0x%04x\n", cid);
	return ret;
}
