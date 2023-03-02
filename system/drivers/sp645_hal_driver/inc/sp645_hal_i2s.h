/**
  ******************************************************************************
  * @file    stm32g0xx_hal_i2s.h
  * @author  MCD Application Team
  * @brief   Header file of I2S HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SP645_HAL_I2S_H
#define SP645_HAL_I2S_H

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_I2S_SUPPORT
#if defined(SPI_I2S_SUPPORT)
/* Includes ------------------------------------------------------------------*/
#include "sp645_cm4.h"
#include "sp645_hal_def.h"
#include "sp64xx.h"

#define DRAM_PCM_BUF_CH_BYTES	(20 * 1024)
#define DMA_TRANS_LEN		(2 * 1024)

#define PLLA_147M	147456000U
#define PLLA_135M	135475200U

#define ROUNDUP(v, size)	((v + size - 1) & (~(size - 1)))

#define BIT(n)		(1U << n)

#define DEBUG_OFF	42

#define I2S0_P		BIT(0)
#define I2S1_P		BIT(1)
#define I2S2_P		BIT(2)

#define I2S0_C		BIT(3)
#define I2S1_C		BIT(4)
#define I2S2_C		BIT(5)

/* I2S TX, Active high to increment/decrement DMA counter */
#define I2S_TX_INC0		(0x1 << 0)
#define I2S_TX_INC1		(0x1 << 6)
#define I2S_TX_INC2		(0x1 << 19)
/* I2S RX */
#define I2S_RX_INC0    		(0x1 << 11)
#define I2S_RX_INC1    		(0x1 << 16)
#define I2S_RX_INC2    		(0x1 << 10)

/* I2S FIFO Enable */
#define I2S_TX_FIFO_EN0		(0x1 << 0)
#define I2S_TX_FIFO_EN1		(0x1 << 6)
#define I2S_TX_FIFO_EN2		(0x1 << 19)

#define I2S_RX_FIFO_EN0    	(0x1 << 11)
#define I2S_RX_FIFO_EN1    	(0x1 << 16)
#define I2S_RX_FIFO_EN2    	(0x1 << 10)

/* I2S Audio Enable */
#define I2S_TX_AUD_EN0		(0x01 | (0x1df << 16))
#define I2S_TX_AUD_EN1		(0x01 << 13)
#define I2S_TX_AUD_EN2		(0x01 << 15)
#define I2S_RX_AUD_EN0    	(0x01 << 3)
#define I2S_RX_AUD_EN1    	(0x01 << 11)
#define I2S_RX_AUD_EN2    	(0x01 << 5)

////////////////////////////////////////
// G61.30 debug function: i2s tx test
#define I2S2_DEBUG_BIT		(14U)
#define I2S1_DEBUG_BIT		(10U)
#define I2S0_DEBUG_BIT		(8U)

////////////////////////////////data_config
#define I2S_MODE_MASK		GENMASK(9, 8)
#define I2S_MODE_BIT		(8U)
#define I2S_LRCYCLE_MASK	GENMASK(6, 5)
#define I2S_LRCYCLE_BIT		(5U)
#define I2S_DWIDTH_MASK		GENMASK(3, 2)
#define I2S_DWIDTH_BIT		(2U)
#define I2S_DELAY_BIT		(0U)

#define LRCK_CYCLE_16		(0U)
#define LRCK_CYCLE_24		(1U)
#define LRCK_CYCLE_32		(2U)

//PCM data to ext DAC delay one BCK cycle after LRCK transition bit[0]
#define i2s_delay_mask		GENMASK(0, 0)
#define i2s_delay_nocyc		(0x0 << 0)
#define i2s_delay_onecyc	(0x1 << 1)
/////////////////////////////////////
#define XCK_GET_EN_BIT		(15U)
#define XCK_EN_MSK		GENMASK(14, 14)
#define XCK_EN_BIT		(14U)
#define XCK_OE_MSK		GENMASK(13, 13)
#define XCK_OE_BIT		(13U)
#define XCK_SRC_MSK		GENMASK(12, 11)
#define XCK_SRC_BIT		(11U)
#define XCK_DIV_MSK		GENMASK(10, 0)
#define XCK_DIV_BIT		(0U)

#define BCK_GET_EN_BIT		(15U)
#define BCK_EN_MSK		GENMASK(14, 14)
#define BCK_EN_BIT		(14U)
#define BCK_SRC_MSK		GENMASK(13, 11)
#define BCK_SRC_BIT		(11U)
#define BCK_DIV_MSK		GENMASK(10, 0)
#define BCK_DIV_BIT		(0U)

#define XBCK_DISABLE		(0U)
#define XBCK_ENABLE		(1U)

#define XBCK_SRC_CDRPLL		(0U)
#define XBCK_SRC_PLLA		(1U)
#define XBCK_SRC_DPLL		(2U)
#define XBCK_SRC_PIXTAL		(3U)
#define BCK_SRC_XCK		(4U)

/** @defgroup I2S_Error I2S Error
  * @{
  */
#define HAL_I2S_ERROR_NONE               (0x00000000U)  /*!< No error                    */
#define HAL_I2S_ERROR_TIMEOUT            (0x00000001U)  /*!< Timeout error               */

/** @defgroup I2S_Mode I2S Mode
  * @{
  */
#define I2S_MODE_SLAVE_TX                (0x0U)
#define I2S_MODE_SLAVE_RX                (0x1U)
#define I2S_MODE_MASTER_TX               (0x2U)
#define I2S_MODE_MASTER_RX               (0x3U)

/** @defgroup I2S_Data_Format I2S Data Format
  * @{
  */
#define I2S_DATAFORMAT_16B		(0x0U)
#define I2S_DATAFORMAT_18B		(0x1U)
#define I2S_DATAFORMAT_20B		(0x2U)
#define I2S_DATAFORMAT_24B		(0x3U)

/** @defgroup I2S_Audio_Frequency I2S Audio Frequency
  * @{
  */
#define I2S_AUDIOFREQ_192K               (192000U)
#define I2S_AUDIOFREQ_96K                (96000U)
#define I2S_AUDIOFREQ_48K                (48000U)
#define I2S_AUDIOFREQ_44K                (44100U)
#define I2S_AUDIOFREQ_32K                (32000U)
#define I2S_AUDIOFREQ_22K                (22050U)
#define I2S_AUDIOFREQ_16K                (16000U)
#define I2S_AUDIOFREQ_11K                (11025U)
#define I2S_AUDIOFREQ_8K                 (8000U)
#define I2S_AUDIOFREQ_DEFAULT            (2U)

typedef struct AUD_t {
	// Group 060 : AUD
	__IOM uint32_t  audif_ctrl                            ; // 00
    	__IOM uint32_t  aud_enable                            ; // 01
    	__IOM uint32_t  pcm_cfg                               ; // 02
    	__IOM uint32_t  i2s_mute_flag_ctrl                    ; // 03
    	__IOM uint32_t  ext_adc_cfg                           ; // 04
    	__IOM uint32_t  int_dac_ctrl0                         ; // 05
    	__IOM uint32_t  int_adc_ctrl                          ; // 06
    	__IOM uint32_t  adc_in_path_switch                    ; // 07
    	__IOM uint32_t  int_adc_dac_cfg                       ; // 08
    	__IOM uint32_t  G060_reserved_9                       ; // 09
    	__IOM uint32_t  iec_cfg                               ; // 10
    	__IOM uint32_t  iec0_valid_out                        ; // 11
    	__IOM uint32_t  iec0_par0_out                         ; // 12
    	__IOM uint32_t  iec0_par1_out                         ; // 13
    	__IOM uint32_t  iec1_valid_out                        ; // 14
    	__IOM uint32_t  iec1_par0_out                         ; // 15
    	__IOM uint32_t  iec1_par1_out                         ; // 16
    	__IOM uint32_t  iec0_rx_debug_info                    ; // 17
    	__IOM uint32_t  iec0_valid_in                         ; // 18
    	__IOM uint32_t  iec0_par0_in                          ; // 19
    	__IOM uint32_t  iec0_par1_in                          ; // 20
   	__IOM uint32_t  iec1_rx_debug_info                    ; // 21
    	__IOM uint32_t  iec1_valid_in                         ; // 22
    	__IOM uint32_t  iec1_par0_in                          ; // 23
    	__IOM uint32_t  iec1_par1_in                          ; // 24
    	__IOM uint32_t  iec2_rx_debug_info                    ; // 25
    	__IOM uint32_t  iec2_valid_in                         ; // 26
    	__IOM uint32_t  iec2_par0_in                          ; // 27
    	__IOM uint32_t  iec2_par1_in                          ; // 28
    	__IOM uint32_t  G060_reserved_29                      ; // 29
    	__IOM uint32_t  iec_tx_user_wdata                     ; // 30
    	__IOM uint32_t  iec_tx_user_ctrl                      ; // 31

    	// Group 061 : AUD
    	__IOM uint32_t  adcp_ch_enable                        ; // 00, ADCPRC Configuration Group 1
    	__IOM uint32_t  adcp_fubypass                         ; // 01, ADCPRC Configuration Group 2
   	__IOM uint32_t  adcp_mode_ctrl                        ; // 02, ADCPRC Mode Control
    	__IOM uint32_t  adcp_init_ctrl                        ; // 03, ADCP Initialization Control
    	__IOM uint32_t  adcp_coeff_din                        ; // 04, Coefficient Data Input
    	__IOM uint32_t  adcp_agc_cfg                          ; // 05, ADCPRC AGC Configuration of Ch0/1
    	__IOM uint32_t  adcp_agc_cfg2                         ; // 06, ADCPRC AGC Configuration of Ch2/3
    	__IOM uint32_t  adcp_gain_0                           ; // 07, ADCP System Gain1
    	__IOM uint32_t  adcp_gain_1                           ; // 08, ADCP System Gain2
    	__IOM uint32_t  adcp_gain_2                           ; // 09, ADCP System Gain3
    	__IOM uint32_t  adcp_gain_3                           ; // 10, ADCP System Gain4
    	__IOM uint32_t  adcp_risc_gain                        ; // 11, ADCP RISC Gain
    	__IOM uint32_t  adcp_mic_l                            ; // 12, ADCPRC Microphone - in Left Channel Data
    	__IOM uint32_t  adcp_mic_r                            ; // 13, ADCPRC Microphone - in Right Channel Data
    	__IOM uint32_t  adcp_agc_gain                         ; // 14, ADCPRC AGC Gain
    	__IOM uint32_t  G061_reserved_15                      ; // 15, Reserved
    	__IOM uint32_t  aud_apt_mode                          ; // 16, Audio Playback Timer Mode
    	__IOM uint32_t  aud_apt_data                          ; // 17, Audio Playback Timer
    	__IOM uint32_t  aud_apt_parameter                     ; // 18, Audio Playback Timer Parameter
    	__IOM uint32_t  G061_reserved_19                      ; // 19, Reserved
    	__IOM uint32_t  aud_audhwya                           ; // 20, DRAM Base Address Offset
    	__IOM uint32_t  aud_inc_0                             ; // 21, DMA Counter Increment/Decrement
    	__IOM uint32_t  aud_delta_0                           ; // 22, Delta Value
    	__IOM uint32_t  aud_fifo_enable                       ; // 23, Audio FIFO Enable
    	__IOM uint32_t  aud_fifo_mode                         ; // 24, FIFO Mode Control
    	__IOM uint32_t  aud_fifo_support                      ; // 25, Supported FIFOs ( Debug Function )
    	__IOM uint32_t  aud_fifo_reset                        ; // 26, Host FIFO Reset
    	__IOM uint32_t  aud_chk_ctrl                          ; // 27, Checksum Control ( Debug Function )
    	__IOM uint32_t  aud_checksum_data                     ; // 28, Checksum Data ( Debug Function )
    	__IOM uint32_t  aud_chk_tcnt                          ; // 29, Target Count of Checksum ( Debug Function )
    	__IOM uint32_t  aud_embedded_input_ctrl               ; // 30, Embedded Input Control ( Debug Function )
    	__IOM uint32_t  aud_misc_ctrl                         ; // 31, Miscellaneous Control

    	// Group 062 : AUD
    	__IOM uint32_t  aud_ext_dac_xck_cfg                   ; // 00
    	__IOM uint32_t  aud_ext_dac_bck_cfg                   ; // 01
    	__IOM uint32_t  aud_iec0_bclk_cfg                     ; // 02
    	__IOM uint32_t  aud_ext_adc_xck_cfg                   ; // 03
    	__IOM uint32_t  aud_ext_adc_bck_cfg                   ; // 04
    	__IOM uint32_t  aud_int_adc_xck_cfg                   ; // 05
    	__IOM uint32_t  G062_reserved_6                       ; // 06
    	__IOM uint32_t  aud_int_dac_xck_cfg                   ; // 07
    	__IOM uint32_t  aud_int_dac_bck_cfg                   ; // 08
    	__IOM uint32_t  aud_iec1_bclk_cfg                     ; // 09
    	__IOM uint32_t  G062_reserved_10                      ; // 10
    	__IOM uint32_t  aud_pcm_iec_bclk_cfg                  ; // 11
    	__IOM uint32_t  aud_xck_osr104_cfg                    ; // 12
    	__IOM uint32_t  aud_hdmi_tx_mclk_cfg                  ; // 13
    	__IOM uint32_t  aud_hdmi_tx_bclk_cfg                  ; // 14
    	__IOM uint32_t  hdmi_tx_i2s_cfg                       ; // 15
    	__IOM uint32_t  hdmi_rx_i2s_cfg                       ; // 16
    	__IOM uint32_t  aud_aadc_agc01_cfg0                   ; // 17
    	__IOM uint32_t  aud_aadc_agc01_cfg1                   ; // 18
    	__IOM uint32_t  aud_aadc_agc01_cfg2                   ; // 19
    	__IOM uint32_t  aud_aadc_agc01_cfg3                   ; // 20
    	__IOM uint32_t  int_adc_ctrl3                         ; // 21
    	__IOM uint32_t  int_adc_ctrl2                         ; // 22
    	__IOM uint32_t  int_dac_ctrl2                         ; // 23
    	__IOM uint32_t  G062_reserved_24                      ; // 24
    	__IOM uint32_t  int_dac_ctrl1                         ; // 25
    	__IOM uint32_t  G062_reserved_26                      ; // 26
    	__IOM uint32_t  G062_reserved_27                      ; // 27
    	__IOM uint32_t  G062_reserved_28                      ; // 28
    	__IOM uint32_t  G062_reserved_29                      ; // 29
    	__IOM uint32_t  G062_reserved_30                      ; // 30
    	__IOM uint32_t  G062_reserved_31                      ; // 31

    	// Group 063 : AUD
    	__IOM uint32_t  aud_bt_ifx_cfg                        ; // 00
    	__IOM uint32_t  aud_bt_i2s_cfg                        ; // 01
    	__IOM uint32_t  aud_bt_xck_cfg                        ; // 02
    	__IOM uint32_t  aud_bt_bck_cfg                        ; // 03
    	__IOM uint32_t  aud_bt_sync_cfg                       ; // 04
    	__IOM uint32_t  G063_reserved_5                       ; // 05
    	__IOM uint32_t  G063_reserved_6                       ; // 06
    	__IOM uint32_t  G063_reserved_7                       ; // 07
    	__IOM uint32_t  aud_pwm_xck_cfg                       ; // 08
    	__IOM uint32_t  aud_pwm_bck_cfg                       ; // 09
    	__IOM uint32_t  G063_reserved_10                      ; // 10
    	__IOM uint32_t  G063_reserved_11                      ; // 11
    	__IOM uint32_t  G063_reserved_12                      ; // 12
   	__IOM uint32_t  G063_reserved_13                      ; // 13
    	__IOM uint32_t  G063_reserved_14                      ; // 14
    	__IOM uint32_t  G063_reserved_15                      ; // 15
    	__IOM uint32_t  G063_reserved_16                      ; // 16
    	__IOM uint32_t  G063_reserved_17                      ; // 17
    	__IOM uint32_t  G063_reserved_18                      ; // 18
    	__IOM uint32_t  G063_reserved_19                      ; // 19
    	__IOM uint32_t  aud_aadc_agc2_cfg0                    ; // 20
    	__IOM uint32_t  aud_aadc_agc2_cfg1                    ; // 21
    	__IOM uint32_t  aud_aadc_agc2_cfg2                    ; // 22
    	__IOM uint32_t  aud_aadc_agc2_cfg3                    ; // 23
    	__IOM uint32_t  aud_opt_test_pat                      ; // 24
    	__IOM uint32_t  aud_sys_status0                       ; // 25
    	__IOM uint32_t  aud_sys_status1                       ; // 26
    	__IOM uint32_t  int_adc_ctrl1                         ; // 27
    	__IOM uint32_t  bt_mute_flag                          ; // 28
    	__IOM uint32_t  cdrpll_losd_ctrl                      ; // 29
    	__IOM uint32_t  G063_reserved_30                      ; // 30
    	__IOM uint32_t  other_config                          ; // 31

    	// Group 064 : AUD
    	__IOM uint32_t  aud_a0_base                           ; //
    	__IOM uint32_t  aud_a0_length                         ; //
    	__IOM uint32_t  aud_a0_ptr                            ; //
    	__IOM uint32_t  aud_a0_cnt                            ; //
    	__IOM uint32_t  aud_a1_base                           ; //
    	__IOM uint32_t  aud_a1_length                         ; //
    	__IOM uint32_t  aud_a1_ptr                            ; //
    	__IOM uint32_t  aud_a1_cnt                            ; //
    	__IOM uint32_t  aud_a2_base                           ; //
    	__IOM uint32_t  aud_a2_length                         ; //
    	__IOM uint32_t  aud_a2_ptr                            ; //
    	__IOM uint32_t  aud_a2_cnt                            ; //
    	__IOM uint32_t  aud_a3_base                           ; //
    	__IOM uint32_t  aud_a3_length                         ; //
    	__IOM uint32_t  aud_a3_ptr                            ; //
    	__IOM uint32_t  aud_a3_cnt                            ; //
    	__IOM uint32_t  aud_a4_base                           ; //
    	__IOM uint32_t  aud_a4_length                         ; //
    	__IOM uint32_t  aud_a4_ptr                            ; //
    	__IOM uint32_t  aud_a4_cnt                            ; //
    	__IOM uint32_t  aud_a5_base                           ; //
    	__IOM uint32_t  aud_a5_length                         ; //
    	__IOM uint32_t  aud_a5_ptr                            ; //
    	__IOM uint32_t  aud_a5_cnt                            ; //
    	__IOM uint32_t  aud_a6_base                           ; //
    	__IOM uint32_t  aud_a6_length                         ; //
    	__IOM uint32_t  aud_a6_ptr                            ; //
    	__IOM uint32_t  aud_a6_cnt                            ; //
    	__IOM uint32_t  aud_a7_base                           ; //
    	__IOM uint32_t  aud_a7_length                         ; //
    	__IOM uint32_t  aud_a7_ptr                            ; //
    	__IOM uint32_t  aud_a7_cnt                            ; //

    	// Group 065 : AUD
    	__IOM uint32_t  aud_a8_base                           ; //
    	__IOM uint32_t  aud_a8_length                         ; //
    	__IOM uint32_t  aud_a8_ptr                            ; //
    	__IOM uint32_t  aud_a8_cnt                            ; //
    	__IOM uint32_t  aud_a9_base                           ; //
    	__IOM uint32_t  aud_a9_length                         ; //
    	__IOM uint32_t  aud_a9_ptr                            ; //
    	__IOM uint32_t  aud_a9_cnt                            ; //
    	__IOM uint32_t  aud_a10_base                          ; //
    	__IOM uint32_t  aud_a10_length                        ; //
    	__IOM uint32_t  aud_a10_ptr                           ; //
    	__IOM uint32_t  aud_a10_cnt                           ; //
    	__IOM uint32_t  aud_a11_base                          ; //
    	__IOM uint32_t  aud_a11_length                        ; //
    	__IOM uint32_t  aud_a11_ptr                           ; //
    	__IOM uint32_t  aud_a11_cnt                           ; //
    	__IOM uint32_t  aud_a12_base                          ; //
    	__IOM uint32_t  aud_a12_length                        ; //
    	__IOM uint32_t  aud_a12_ptr                           ; //
    	__IOM uint32_t  aud_a12_cnt                           ; //
    	__IOM uint32_t  aud_a13_base                          ; //
    	__IOM uint32_t  aud_a13_length                        ; //
    	__IOM uint32_t  aud_a13_ptr                           ; //
    	__IOM uint32_t  aud_a13_cnt                           ; //
    	__IOM uint32_t  aud_a14_base                          ; //
    	__IOM uint32_t  aud_a14_length                        ; //
    	__IOM uint32_t  aud_a14_ptr                           ; //
    	__IOM uint32_t  aud_a14_cnt                           ; //
    	__IOM uint32_t  aud_a15_base                          ; //
    	__IOM uint32_t  aud_a15_length                        ; //
    	__IOM uint32_t  aud_a15_ptr                           ; //
    	__IOM uint32_t  aud_a15_cnt                           ; //

    	// Group 066 : AUD
    	__IOM uint32_t  aud_a16_base                          ; //
    	__IOM uint32_t  aud_a16_length                        ; //
    	__IOM uint32_t  aud_a16_ptr                           ; //
    	__IOM uint32_t  aud_a16_cnt                           ; //
    	__IOM uint32_t  aud_a17_base                          ; //
    	__IOM uint32_t  aud_a17_length                        ; //
    	__IOM uint32_t  aud_a17_ptr                           ; //
    	__IOM uint32_t  aud_a17_cnt                           ; //
    	__IOM uint32_t  aud_a18_base                          ; //
    	__IOM uint32_t  aud_a18_length                        ; //
    	__IOM uint32_t  aud_a18_ptr                           ; //
    	__IOM uint32_t  aud_a18_cnt                           ; //
    	__IOM uint32_t  aud_a19_base                          ; //
    	__IOM uint32_t  aud_a19_length                        ; //
    	__IOM uint32_t  aud_a19_ptr                           ; //
    	__IOM uint32_t  aud_a19_cnt                           ; //
    	__IOM uint32_t  aud_a20_base                          ; //
    	__IOM uint32_t  aud_a20_length                        ; //
    	__IOM uint32_t  aud_a20_ptr                           ; //
    	__IOM uint32_t  aud_a20_cnt                           ; //
    	__IOM uint32_t  aud_a21_base                          ; //
    	__IOM uint32_t  aud_a21_length                        ; //
    	__IOM uint32_t  aud_a21_ptr                           ; //
    	__IOM uint32_t  aud_a21_cnt                           ; //


    	__IOM uint32_t  aud_a22_base                          ; // 24
    	__IOM uint32_t  aud_a22_length                        ; // 25
    	__IOM uint32_t  aud_a22_ptr                           ; // 26
    	__IOM uint32_t  aud_a22_cnt                           ; // 27
    	__IOM uint32_t  aud_a23_base                          ; // 28
    	__IOM uint32_t  aud_a23_length                        ; // 29
    	__IOM uint32_t  aud_a23_ptr                           ; // 30
    	__IOM uint32_t  aud_a23_cnt                           ; // 31

    	// Group 067 : AUD
    	__IOM uint32_t  aud_grm_master_gain                   ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_0                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_1                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_2                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_3                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_4                ; // Gain Control
    	__IOM uint32_t  aud_grm_mix_control_0                 ; // Mixer Setting
    	__IOM uint32_t  aud_grm_mix_control_1                 ; // Mixer Setting
    	__IOM uint32_t  aud_grm_mix_control_2                 ; // Mixer Setting
    	__IOM uint32_t  aud_grm_switch_0                      ; // Channel Switch
    	__IOM uint32_t  aud_grm_switch_1                      ; // Channel Switch
    	__IOM uint32_t  aud_grm_switch_int                    ; // Channel Switch
    	__IOM uint32_t  aud_grm_delta_volume                  ; // Gain Update
    	__IOM uint32_t  aud_grm_delta_ramp_pcm                ; // Gain Update
    	__IOM uint32_t  aud_grm_delta_ramp_risc               ; // Gain Update
    	__IOM uint32_t  aud_grm_delta_ramp_linein             ; // Gain Update
    	__IOM uint32_t  aud_grm_other                         ; // Other Setting
    	__IOM uint32_t  aud_grm_gain_control_5                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_6                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_7                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_8                ; // Gain Control
    	__IOM uint32_t  aud_grm_fifo_eflag                    ; // FIFO Error Flag
    	__IOM uint32_t  G067_reserved_22                      ; //
    	__IOM uint32_t  aud_grm_gain_control_10                      ; //
    	__IOM uint32_t  aud_grm_switch_hdmi_tx                ; // AUD_GRM_SWITCH_HDMI_TX
    	__IOM uint32_t  G067_reserved_25                      ; //
    	__IOM uint32_t  G067_reserved_26                      ; //
    	__IOM uint32_t  G067_reserved_27                      ; //
    	__IOM uint32_t  G067_reserved_28                      ; //
    	__IOM uint32_t  G067_reserved_29                      ; //
    	__IOM uint32_t  aud_grm_path_select                   ; //
    	__IOM uint32_t  G067_reserved_31                      ; //

    	// Group 068 : AUD
    	__IOM uint32_t  G068_AUD[32]                          ;

    	// Group 069 : Reserved
    	__IOM uint32_t  G069_reserved_00                      ;
    	__IOM uint32_t  G069_reserved_01                      ;
    	__IOM uint32_t  G069_reserved_02                      ;
    	__IOM uint32_t  G069_reserved_03                      ;
    	__IOM uint32_t  G069_reserved_04                      ;
    	__IOM uint32_t  G069_reserved_05                      ;
    	__IOM uint32_t  G069_reserved_06                      ;
    	__IOM uint32_t  G069_reserved_07                      ;
    	__IOM uint32_t  G069_reserved_08                      ;
    	__IOM uint32_t  G069_reserved_09                      ;
    	__IOM uint32_t  G069_reserved_10                      ;
    	__IOM uint32_t  G069_reserved_11                      ;
    	__IOM uint32_t  G069_reserved_12                      ;
    	__IOM uint32_t  G069_reserved_13                      ;
    	__IOM uint32_t  G069_reserved_14                      ;
    	__IOM uint32_t  I2S_PWM_CONTROL_1                     ;
    	__IOM uint32_t  I2S_PWM_CONTROL_2                     ;
    	__IOM uint32_t  I2S_PWM_CONTROL_3                     ;
    	__IOM uint32_t  I2S_PWM_CONTROL_4                     ;
    	__IOM uint32_t  CLASSD_MOS_CONTROL                    ;
    	__IOM uint32_t  G069_reserved_20                      ;
    	__IOM uint32_t  G069_reserved_21                      ;
    	__IOM uint32_t  G069_reserved_22                      ;
    	__IOM uint32_t  G069_reserved_23                      ;
    	__IOM uint32_t  G069_reserved_24                      ;
    	__IOM uint32_t  G069_reserved_25                      ;
    	__IOM uint32_t  G069_reserved_26                      ;
    	__IOM uint32_t  G069_reserved_27                      ;
    	__IOM uint32_t  G069_reserved_28                      ;
    	__IOM uint32_t  G069_reserved_29                      ;
    	__IOM uint32_t  G069_reserved_30                      ;
    	__IOM uint32_t  G069_reserved_31                      ;

    	// Group 070 : Reserved
    	__IOM uint32_t  G070_RESERVED[32]                     ;

    	// Group 071 : Reserved
    	__IOM uint32_t  aud_a24_base                          ; // 00
    	__IOM uint32_t  aud_a24_length                        ; // 01
    	__IOM uint32_t  aud_a24_ptr                           ; // 02
    	__IOM uint32_t  aud_a24_cnt                           ; // 03
    	__IOM uint32_t  aud_a25_base                          ; // 04
    	__IOM uint32_t  aud_a25_length                        ; // 05
    	__IOM uint32_t  aud_a25_ptr                           ; // 06
    	__IOM uint32_t  aud_a25_cnt                           ; // 07
    	__IOM uint32_t  aud_a26_base                          ; // 08
    	__IOM uint32_t  aud_a26_length                        ; // 09
    	__IOM uint32_t  aud_a26_ptr                           ; // 10
    	__IOM uint32_t  aud_a26_cnt                           ; // 11
    	__IOM uint32_t  aud_a27_base                          ; // 12
    	__IOM uint32_t  aud_a27_length                        ; // 13
    	__IOM uint32_t  aud_a27_ptr                           ; // 14
    	__IOM uint32_t  aud_a27_cnt                           ; // 15
    	__IOM uint32_t  aud_a28_base                          ; // 16
    	__IOM uint32_t  aud_a28_length                        ; // 17
    	__IOM uint32_t  aud_a28_ptr                           ; // 18
    	__IOM uint32_t  aud_a28_cnt                           ; // 19
    	__IOM uint32_t  aud_a29_base                          ; // 20
    	__IOM uint32_t  aud_a29_length                        ; // 21
    	__IOM uint32_t  aud_a29_ptr                           ; // 22
    	__IOM uint32_t  aud_a29_cnt                           ; // 23
    	__IOM uint32_t  G071_reserved_24                      ; // 24
    	__IOM uint32_t  G071_reserved_25                      ; // 25
    	__IOM uint32_t  G071_reserved_26                      ; // 26
    	__IOM uint32_t  G071_reserved_27                      ; // 27
    	__IOM uint32_t  G071_reserved_28                      ; // 28
    	__IOM uint32_t  G071_reserved_29                      ; // 29
    	__IOM uint32_t  G071_reserved_30                      ; // 30
    	__IOM uint32_t  G071_reserved_31                      ; // 31

    	// Group 072 : Reserved
    	__IOM uint32_t  tdm_rx_cfg0                           ;//0
    	__IOM uint32_t  tdm_rx_cfg1                           ;//1
    	__IOM uint32_t  tdm_rx_cfg2                           ;//2
    	__IOM uint32_t  tdm_rx_cfg3                           ;//3
    	__IOM uint32_t  G72_reserved_4                        ;//4
    	__IOM uint32_t  G72_reserved_5                        ;//5
    	__IOM uint32_t  tdm_tx_cfg0                           ;//6
    	__IOM uint32_t  tdm_tx_cfg1                           ;//7
    	__IOM uint32_t  tdm_tx_cfg2                           ;//8
    	__IOM uint32_t  tdm_tx_cfg3                           ;//9
    	__IOM uint32_t  tdm_tx_cfg4                           ;//10
    	__IOM uint32_t  G72_reserved_11                       ;//11
    	__IOM uint32_t  G72_reserved_12                       ;//12
    	__IOM uint32_t  G72_reserved_13                       ;//13
    	__IOM uint32_t  pdm_rx_cfg0                           ;//14
    	__IOM uint32_t  pdm_rx_cfg1                           ;//15
    	__IOM uint32_t  pdm_rx_cfg2                           ;//16
    	__IOM uint32_t  pdm_rx_cfg3                           ;//17
    	__IOM uint32_t  pdm_rx_cfg4                           ;//18
    	__IOM uint32_t  pdm_rx_cfg5                           ;//19
    	__IOM uint32_t  G72_reserved_20                       ;//20
    	__IOM uint32_t  G72_reserved_21                       ;//21
    	__IOM uint32_t  tdm_tx_xck_cfg                        ;//22
    	__IOM uint32_t  tdm_tx_bck_cfg                        ;//23
    	__IOM uint32_t  tdm_rx_xck_cfg                        ;//24
    	__IOM uint32_t  tdm_rx_bck_cfg                        ;//25 TDM_RX_BCK_CFG
    	__IOM uint32_t  pdm_rx_xck_cfg                        ;//26
    	__IOM uint32_t  pdm_rx_bck_cfg                        ;//27
    	__IOM uint32_t  G72_reserved_28                       ;//28
    	__IOM uint32_t  G72_reserved_29                       ;//29
    	__IOM uint32_t  tdmpdm_tx_sel                         ;//30
    	__IOM uint32_t  G72_reserved_31                       ;//31
} AUD_t;
#define AUD_REG ((volatile AUD_t *)RF_GRP(60, 0))

/* I2S TX */
#define I2S_P_INC0		(0x1 << 0)
#define I2S_P_INC1		(0x1 << 6)
#define I2S_P_INC2		(0x1 << 19)
/* I2S RX */
#define I2S_C_INC0    		(0x1 << 11)
#define I2S_C_INC1    		(0x1 << 16)
#define I2S_C_INC2    		(0x1 << 10)
#define TDMPDM_C_INC0 		((0x1 << 14) | (0x3 << 17) | (0x1 << 21) | (0xf << 22))
#define TDM_P_INC0    		((0x1 << 20) | (0x3 << 26) | 0x1f)

#define aud_enable_i2stdm_p	(0x01 | (0x1df << 16))
#define aud_enable_i2s1_p	(0x01 << 13)
#define aud_enable_i2s2_p	(0x01 << 15)
#define aud_enable_i2s0_c    	(0x01 << 3)
#define aud_enable_i2s1_c    	(0x01 << 11)
#define aud_enable_i2s2_c    	(0x01 << 5)
#define aud_enable_tdmpdm_c 	(0x01 << 12)

typedef enum _SampleRateSetting_e
{
	FS_22 = 22050,
	FS_32 = 32000,
	FS_44 = 44100,
	FS_48 = 48000,
	FS_64 = 64000,
	FS_88 = 88200,
	FS_96 = 96000,
	FS_128 = 128000,
	FS_176 = 176400,
	FS_192 = 192000,
} SampleRateSetting_e;
#define SAMPLE_RATE FS_48

#define	ONEHOT_B10 0x00000400
#define	ONEHOT_B11 0x00000800
#define	ONEHOT_B12 0x00001000

/** @addtogroup STM32G0xx_HAL_Driver
  * @{
  */

/** @addtogroup I2S
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup I2S_Exported_Types I2S Exported Types
  * @{
  */
typedef struct
{
	uint8_t flag_gen_xck_bck;
} I2S_Priv;

/**
  * @brief I2S Init structure definition
  */
typedef struct
{
  uint32_t Mode;                /*!< Specifies the I2S operating mode.
                                     This parameter can be a value of @ref I2S_Mode */

  uint32_t Standard;            /*!< Specifies the standard used for the I2S communication.
                                     This parameter can be a value of @ref I2S_Standard */

  uint32_t DataFormat;          /*!< Specifies the data format for the I2S communication.
                                     This parameter can be a value of @ref I2S_Data_Format */

  uint32_t MCLKOutput;          /*!< Specifies whether the I2S MCLK output is enabled or not.
                                     This parameter can be a value of @ref I2S_MCLK_Output */

  uint32_t AudioFreq;           /*!< Specifies the frequency selected for the I2S communication.
                                     This parameter can be a value of @ref I2S_Audio_Frequency */
} I2S_InitTypeDef;

/**
  * @brief  HAL State structures definition
  */
typedef enum
{
  HAL_I2S_STATE_RESET      = 0x00U,  /*!< I2S not yet initialized or disabled                */
  HAL_I2S_STATE_READY      = 0x01U,  /*!< I2S initialized and ready for use                  */
  HAL_I2S_STATE_BUSY       = 0x02U,  /*!< I2S internal process is ongoing                    */
  HAL_I2S_STATE_BUSY_TX    = 0x03U,  /*!< Data Transmission process is ongoing               */
  HAL_I2S_STATE_BUSY_RX    = 0x04U,  /*!< Data Reception process is ongoing                  */
  HAL_I2S_STATE_TIMEOUT    = 0x06U,  /*!< I2S timeout state                                  */
  HAL_I2S_STATE_ERROR      = 0x07U   /*!< I2S error state                                    */
} HAL_I2S_StateTypeDef;

typedef enum
{
	INDEX_I2S0	= 0x0U,
	INDEX_I2S1	= 0x1U,
	INDEX_I2S2	= 0x2U
} HAL_I2S_IndexTypeDef;

typedef enum
{
	I2S_RX	= 0x0U,
	I2S_TX	= 0x1U
} HAL_I2S_RxTxTypeDef;

typedef enum
{
	I2S_INT_XCK_BCK	= 0x0U,
	I2S_EXT_XCK_BCK	= 0x1U
} HAL_I2S_XckBckTypeDef;

typedef enum
{
	I2S_MASTER	= 0x0U,
	I2S_SLAVE	= 0x2U
} HAL_I2S_ModeTypeDef;

typedef enum
{
	I2S_DEBUG_OFF	= 0x0U,
	I2S_DEBUG_0DB	= 0x1U,
	I2S_DEBUG_60DB	= 0x3U
} HAL_I2S_DebugTypeDef;

typedef struct
{
	volatile uint32_t *data_cfg;	/* Config the master/slave mode, LRCK and PCM data */
	volatile uint32_t *bck;		/* Config the BCK source and division */
	volatile uint32_t *xck;		/* Config the XCK source and division */
	volatile uint32_t *fifo_base;	/* Set the fifo buffer base address */
	volatile uint32_t *fifo_len;	/* Set the fifo buffer length */
	volatile uint32_t *fifo_ptr;	/* Record the position of I2S hardware access the FIFO */
	volatile uint32_t *fifo_cnt;	/* Get the current data count */
} HAL_I2S_RegTypeDef;

/**
  * @brief I2S handle Structure definition
  */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
typedef struct __I2S_HandleTypeDef
#else
typedef struct
#endif
{
	SPI_TypeDef                *Instance;    /*!< I2S registers base address */

	I2S_InitTypeDef            Init;         /*!< I2S communication parameters */

	uint32_t                   *pTxBuffPtr;  /*!< Pointer to I2S Tx transfer buffer */

	__IO uint16_t              TxXferSize;   /*!< I2S Tx transfer size */

	__IO uint16_t              TxXferCount;  /*!< I2S Tx transfer Counter */

	uint32_t                   *pRxBuffPtr;  /*!< Pointer to I2S Rx transfer buffer */

	__IO uint16_t              RxXferSize;   /*!< I2S Rx transfer size */

	__IO uint16_t              RxXferCount;  /*!< I2S Rx transfer counter
	                                      (This field is initialized at the
	                                       same value as transfer size at the
	                                       beginning of the transfer and
	                                       decremented when a sample is received
	                                       NbSamplesReceived = RxBufferSize-RxBufferCount) */

	__IO HAL_LockTypeDef       Lock;         /*!< I2S locking object */

	__IO HAL_I2S_StateTypeDef  State;        /*!< I2S communication state */

	__IO uint32_t              ErrorCode;    /*!< I2S Error code
	                                        This parameter can be a value of @ref I2S_Error */
	HAL_I2S_IndexTypeDef	Index;
	HAL_I2S_XckBckTypeDef 	FlagSrcXckBck;
	HAL_I2S_RxTxTypeDef	FlagTxRx;
	HAL_I2S_ModeTypeDef	FlagMode;
	HAL_I2S_DebugTypeDef	FlagDebug;
	uint32_t		FlagFifoEn;
	uint32_t		FlagAudEn;
	uint32_t		FlagDMAInc;
	HAL_I2S_RegTypeDef	Reg;
	uint32_t		TxOffset;	/* Record the ptr offset of TX fifo buffer */
	uint32_t		RxOffset;	/* Record the ptr offset of RX fifo buffer */

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  void (* TxCpltCallback)(struct __I2S_HandleTypeDef *hi2s);             /*!< I2S Tx Completed callback          */
  void (* RxCpltCallback)(struct __I2S_HandleTypeDef *hi2s);             /*!< I2S Rx Completed callback          */
  void (* TxHalfCpltCallback)(struct __I2S_HandleTypeDef *hi2s);         /*!< I2S Tx Half Completed callback     */
  void (* RxHalfCpltCallback)(struct __I2S_HandleTypeDef *hi2s);         /*!< I2S Rx Half Completed callback     */
  void (* ErrorCallback)(struct __I2S_HandleTypeDef *hi2s);              /*!< I2S Error callback                 */
  void (* MspInitCallback)(struct __I2S_HandleTypeDef *hi2s);            /*!< I2S Msp Init callback              */
  void (* MspDeInitCallback)(struct __I2S_HandleTypeDef *hi2s);          /*!< I2S Msp DeInit callback            */

#endif  /* USE_HAL_I2S_REGISTER_CALLBACKS */
} I2S_HandleTypeDef;

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
/**
  * @brief  HAL I2S Callback ID enumeration definition
  */
typedef enum
{
  HAL_I2S_TX_COMPLETE_CB_ID             = 0x00U,    /*!< I2S Tx Completed callback ID         */
  HAL_I2S_RX_COMPLETE_CB_ID             = 0x01U,    /*!< I2S Rx Completed callback ID         */
  HAL_I2S_TX_HALF_COMPLETE_CB_ID        = 0x03U,    /*!< I2S Tx Half Completed callback ID    */
  HAL_I2S_RX_HALF_COMPLETE_CB_ID        = 0x04U,    /*!< I2S Rx Half Completed callback ID    */
  HAL_I2S_ERROR_CB_ID                   = 0x06U,    /*!< I2S Error callback ID                */
  HAL_I2S_MSPINIT_CB_ID                 = 0x07U,    /*!< I2S Msp Init callback ID             */
  HAL_I2S_MSPDEINIT_CB_ID               = 0x08U     /*!< I2S Msp DeInit callback ID           */

} HAL_I2S_CallbackIDTypeDef;

/**
  * @brief  HAL I2S Callback pointer definition
  */
typedef  void (*pI2S_CallbackTypeDef)(I2S_HandleTypeDef *hi2s); /*!< pointer to an I2S callback function */

#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup I2S_Exported_macros I2S Exported Macros
  * @{
  */

/** @brief  Reset I2S handle state
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
#define __HAL_I2S_RESET_HANDLE_STATE(__HANDLE__)                do{                                                  \
                                                                    (__HANDLE__)->State = HAL_I2S_STATE_RESET;       \
                                                                    (__HANDLE__)->MspInitCallback = NULL;            \
                                                                    (__HANDLE__)->MspDeInitCallback = NULL;          \
                                                                  } while(0)
#else
#define __HAL_I2S_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_I2S_STATE_RESET)
#endif

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup I2S_Exported_Functions
  * @{
  */

/** @addtogroup I2S_Exported_Functions_Group1
  * @{
  */
/* Initialization/de-initialization functions  ********************************/
void HAL_I2S_Default_Init(void);
HAL_StatusTypeDef HAL_I2S_Init(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_DeInit(I2S_HandleTypeDef *hi2s);
void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s);
void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s);

/* Callbacks Register/UnRegister functions  ***********************************/
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
HAL_StatusTypeDef HAL_I2S_RegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID, pI2S_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_I2S_UnRegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup I2S_Exported_Functions_Group2
  * @{
  */
/* I/O operation functions  ***************************************************/
//HAL_StatusTypeDef HAL_I2S_Debug_Tx_Sine(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_Debug_Tx_Sine(HAL_I2S_IndexTypeDef index, HAL_I2S_DebugTypeDef db);

/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_I2S_Transmit(I2S_HandleTypeDef *hi2s, uint32_t *pData, uint32_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2S_Receive(I2S_HandleTypeDef *hi2s, uint32_t *pData, uint32_t Size, uint32_t Timeout);

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_I2S_Transmit_IT(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2S_Receive_IT(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size);
void HAL_I2S_IRQHandler(I2S_HandleTypeDef *hi2s);

/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_I2S_Transmit_DMA(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2S_Receive_DMA(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size);

HAL_StatusTypeDef HAL_I2S_DMAPause(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_DMAResume(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_DMAStop(I2S_HandleTypeDef *hi2s);

/* Callbacks used in non blocking modes (Interrupt and DMA) *******************/
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s);
/**
  * @}
  */

/** @addtogroup I2S_Exported_Functions_Group3
  * @{
  */
/* Peripheral Control and State functions  ************************************/
HAL_I2S_StateTypeDef HAL_I2S_GetState(I2S_HandleTypeDef *hi2s);
uint32_t HAL_I2S_GetError(I2S_HandleTypeDef *hi2s);

/* Private macros ------------------------------------------------------------*/
/** @brief  Checks if I2S Mode parameter is in allowed range.
  * @param  __MODE__ specifies the I2S Mode.
  *         This parameter can be a value of @ref I2S_Mode
  * @retval None
  */
#define IS_I2S_MODE(__MODE__) (((__MODE__) == I2S_MODE_SLAVE_TX)  || \
                               ((__MODE__) == I2S_MODE_SLAVE_RX)  || \
                               ((__MODE__) == I2S_MODE_MASTER_TX) || \
                               ((__MODE__) == I2S_MODE_MASTER_RX))

#define IS_I2S_STANDARD(__STANDARD__) (((__STANDARD__) == I2S_STANDARD_PHILIPS)   || \
                                       ((__STANDARD__) == I2S_STANDARD_MSB)       || \
                                       ((__STANDARD__) == I2S_STANDARD_LSB)       || \
                                       ((__STANDARD__) == I2S_STANDARD_PCM_SHORT) || \
                                       ((__STANDARD__) == I2S_STANDARD_PCM_LONG))

#define IS_I2S_DATA_FORMAT(__FORMAT__) (((__FORMAT__) == I2S_DATAFORMAT_16B)          || \
                                        ((__FORMAT__) == I2S_DATAFORMAT_16B_EXTENDED) || \
                                        ((__FORMAT__) == I2S_DATAFORMAT_24B)          || \
                                        ((__FORMAT__) == I2S_DATAFORMAT_32B))

#define IS_I2S_MCLK_OUTPUT(__OUTPUT__) (((__OUTPUT__) == I2S_MCLKOUTPUT_ENABLE) || \
                                        ((__OUTPUT__) == I2S_MCLKOUTPUT_DISABLE))

#define IS_I2S_AUDIO_FREQ(__FREQ__) ((((__FREQ__) >= I2S_AUDIOFREQ_8K)    && \
                                      ((__FREQ__) <= I2S_AUDIOFREQ_192K)) || \
                                      ((__FREQ__) == I2S_AUDIOFREQ_DEFAULT))

#endif /* SPI_I2S_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif /* SP645_HAL_I2S_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

