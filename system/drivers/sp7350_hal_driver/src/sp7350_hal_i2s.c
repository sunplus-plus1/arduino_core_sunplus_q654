#include "sp7350_hal_i2s.h"
#include <stdlib.h>
#include "wiring_time.h"

#ifdef __cplusplus
extern "C" {
#endif
#define STAMP(v)                                   \
	do {                                       \
		*(volatile uint32_t *)0xf8000000UL = v; \
	} while (0)

//static HAL_StatusTypeDef  I2S_WaitFlagStateUntilTimeout(I2S_HandleTypeDef *hi2s, uint32_t Flag, FlagStatus State, uint32_t Timeout);

/* As the fifo base, refer to _i2s_config_pcm_buffer() */
extern unsigned long _heap_bottom;
extern unsigned long _heap_top;

static void addr_to_group_print(uint32_t addr)
{
	uint32_t temp, group, reg;

	temp = addr - 0xf8000000;

	group = temp / 128;
	reg = (temp % 128) / 4;

	printf("# G%02d.%02d  0x%08x val 0x%08x\n", group, reg, addr, *(uint32_t *)addr);
}

static void dump_common_reg(I2S_HandleTypeDef *hi2s)
{
	printf("[i2s_test] #I2S%d Reg address:\n", hi2s->Index);
	printf("[i2s_test] #xck		");
	addr_to_group_print((uint32_t)hi2s->Reg.xck);
	printf("[i2s_test] #bck		");
	addr_to_group_print((uint32_t)hi2s->Reg.bck);
	printf("[i2s_test] #data_cfg	");
	addr_to_group_print((uint32_t)hi2s->Reg.data_cfg);
	printf("[i2s_test] #fifo_base	");
	addr_to_group_print((uint32_t)hi2s->Reg.fifo_base);
	printf("[i2s_test] #fifo_len	");
	addr_to_group_print((uint32_t)hi2s->Reg.fifo_len);
}

static void dump_audio_reg(void)
{
	int i, j;
	uint32_t addr, val;

	for(i = 0; i < 13; i++) {
		printf("//Group G%d\n", (60 + i));
		for(j = 0; j < 32; j++) {
			addr = RF_GRP(60 + i, j);
			val = *(uint32_t *)addr;
			printf("0x%08x//%02d\n", val, j);
		}
	}
}

static void dump_i2s_clk_info(I2S_HandleTypeDef *hi2s)
{
	int i;
	uint32_t xck_div, bck_div;
	uint32_t debug_xck, debug_bck, debug_lrck, lrck_cycle;
	int debug_xck_div = 1, debug_bck_div = 1, debug_lrck_div;

	xck_div = READ_BIT(*hi2s->Reg.xck, XCK_DIV_MSK) >> XCK_DIV_BIT;
	bck_div = READ_BIT(*hi2s->Reg.bck, BCK_DIV_MSK) >> BCK_DIV_BIT;
	lrck_cycle = READ_BIT(*hi2s->Reg.data_cfg, I2S_LRCYCLE_MASK) >> I2S_LRCYCLE_BIT;

	for(i = 0; i < 11; i ++) {
		if ((i < 7) && (xck_div & BIT(i)))
			debug_xck_div = debug_xck_div * 2;
		if ((i > 6) && (xck_div & BIT(i)))
			debug_xck_div = debug_xck_div * 3;
	}

	for(i = 0; i < 11; i ++) {
		if ((i < 7) && (bck_div & BIT(i)))
			debug_bck_div = debug_bck_div * 2;
		if ((i > 6) && (bck_div & BIT(i)))
			debug_bck_div = debug_bck_div * 3;
	}
	debug_xck = hi2s->PllSrc / debug_xck_div;
	debug_bck = debug_xck / debug_bck_div;

	if (lrck_cycle == LRCK_CYCLE_16) {
		debug_lrck_div = 16;
	} else if(lrck_cycle == LRCK_CYCLE_32) {
		debug_lrck_div = 32;
	} else {
		printf("[i2s_test] #    Error LRCK cycle setting \n");
	}
	debug_lrck = debug_bck / (debug_lrck_div * 2);

	printf("[i2s_test] #    PLL SRC %d\n", hi2s->PllSrc);
	printf("[i2s_test] #    XCK DIV %d, freq %d\n", debug_xck_div, debug_xck);
	printf("[i2s_test] #    BCK DIV %d, freq %d\n", debug_bck_div, debug_bck);
	printf("[i2s_test] #    LRCK DIV %d freq %d\n", (debug_lrck_div * 2), debug_lrck);
	printf("[i2s_test] #    AudioFreq %d\n", hi2s->Init.AudioFreq);
}

static uint32_t single_swap_4bytes(uint32_t single)
{
	return ((single&0x000000ff)<<24|\
	(single&0x0000ff00)<<8|\
	(single&0x00ff0000)>>8|\
	(single&0xff000000)>>24);
}

//TODO: refine the para uint32_t -> uint8_t
void copy_data(uint32_t *target, uint32_t *src, uint32_t len, uint32_t width)
{
	int i;
	if (width == I2S_DATAFORMAT_16B) {
		for(i = 0; i < len; i++) {
			*(target + i) = *(src + i);
		}
	} else if (width == I2S_DATAFORMAT_24B){//why 24bit need swap
		for(i = 0; i < len; i++) {
			*(target + i) = single_swap_4bytes(*(src + i));
		}
	}
}

/* For RX slave mode work, TX must set slave mode. So default config slave mode. */
static void workaroud_for_config_rx_slave(I2S_HandleTypeDef *hi2s)
{
	switch(hi2s->Index) {
		case INDEX_I2S0:
			AUD_REG->pcm_cfg = 0x24d;//G60.2 I2S0 TX slave mode
			break;
		case INDEX_I2S1:
			AUD_REG->int_adc_dac_cfg = 0x24d;//G60.8 I2S1 TX slave mode
			break;
		case INDEX_I2S2:
			AUD_REG->hdmi_tx_i2s_cfg = 0x24d;//G62.15 I2S2 TX slave mode
			break;
	}
}

static void AUD_Set_PLL(uint32_t freq)
{
	/* reference clk-sp7350.c in kennel */
	if(freq == PLLA_147M) {
		MOON3_REG->sft_cfg[0] = 0xffff5200;
		MOON3_REG->sft_cfg[1] = 0xfffff02c;//bit[2]
		MOON3_REG->sft_cfg[2] = 0xffff1f51;
		MOON3_REG->sft_cfg[3] = 0xffff3fd0;
		MOON3_REG->sft_cfg[4] = 0xffff0168;
	} else if (freq == PLLA_135M) {
		MOON3_REG->sft_cfg[0] = 0xffff5200;
		MOON3_REG->sft_cfg[1] = 0xfffff02c;
		MOON3_REG->sft_cfg[2] = 0xffff0c21;
		MOON3_REG->sft_cfg[3] = 0xffff3fd0;
		MOON3_REG->sft_cfg[4] = 0xffff0154;
	}
	//printf("[%s(%d)] PLLA 0x%x\n", __FUNCTION__, __LINE__, MOON3_REG->sft_cfg[1]);
}

static void F_Mixer_Setting(void)
{
	unsigned int val;

	//67. 0~4
	AUD_REG->aud_grm_master_gain = 0x80000000; //aud_grm_master_gain
	//AUD_REG->aud_grm_master_gain = 0; //aud_grm_master_gain
	AUD_REG->aud_grm_gain_control_0 = 0x80808080; //aud_grm_gain_control_0
	AUD_REG->aud_grm_gain_control_1 = 0x80808080; //aud_grm_gain_control_1
	AUD_REG->aud_grm_gain_control_2 = 0x808000;   //aud_grm_gain_control_2
	//AUD_REG->aud_grm_gain_control_3 = 0x80804040; //aud_grm_gain_control_3
	AUD_REG->aud_grm_gain_control_3 = 0x80808080; //aud_grm_gain_control_3
	AUD_REG->aud_grm_gain_control_4 = 0x0000007f; //aud_grm_gain_control_4
        AUD_REG->aud_grm_gain_control_10 = 0x8000;
	val = 0x2040; //0=raw, mix40, mix39
	val = val | 0x20400000; //0=raw, mix57, mix56

	AUD_REG->aud_grm_mix_control_1 = val;  //aud_grm_mix_control_1

	val = 0;
	AUD_REG->aud_grm_mix_control_2 = val;	   //aud_grm_mix_control_2

	//EXT DAC I2S
	AUD_REG->aud_grm_switch_0 = 0x76543210;    //aud_grm_switch_0
	AUD_REG->aud_grm_switch_1 = 0x98;		   //aud_grm_switch_1

	//INT DAC I2S
	AUD_REG->aud_grm_switch_int = 0x76543210;  //aud_grm_switch_int
	AUD_REG->aud_grm_delta_volume = 0x8000; 	   //aud_grm_delta_volume
	AUD_REG->aud_grm_delta_ramp_pcm = 0x8000;	   //aud_grm_delta_ramp_pcm
	AUD_REG->aud_grm_delta_ramp_risc = 0x8000;	   //aud_grm_delta_ramp_risc
	AUD_REG->aud_grm_delta_ramp_linein = 0x8000;   //aud_grm_delta_ramp_linein
	AUD_REG->aud_grm_other = 0x0;				   //aud_grm_other for A20
	AUD_REG->aud_grm_switch_hdmi_tx = 0x76543210;  //aud_grm_switch_hdmi_tx

}//F_Mixer_Setting

static void F_Adcp_WaitInitOk(void)
{
   	int val;
   	do {
      		//wait(50);
      		//delay_1ms(50);
      		val = AUD_REG->adcp_init_ctrl; //read adcp_init_ctrl
   	} while ((val & ONEHOT_B12) != 0);
   	printf("[aud_test] #F_Adcp_WaitInit is OK\n");
}//F_Adcp_WaitInitOk

static void F_Cfg_AdcIn(void)
{
    	int val, ch_base;
    	///// Config ADCPRC /////
    	AUD_REG->adcp_ch_enable = 0x0;      //adcp_ch_enable
    	AUD_REG->adcp_fubypass = 0x7777;   //adcp_fubypass

    	AUD_REG->adcp_mode_ctrl |= 0x300;       //enable ch2/3

    	AUD_REG->adcp_risc_gain = 0x1111;  //adcp_risc_gain, all gains are 1x

	AUD_REG->G069_reserved_00 = 0x3; // adcprc A16~18
    	// agc setting for both ch0 and ch1 //
    	val = 0x650100;              //steplen0=0, Eth_off=0x65, Eth_on=0x100, steplen0=0
    	AUD_REG->adcp_agc_cfg = val;  //adcp_agc_cfg0

    	//ch0//
    	ch_base = 0 << 4;
    	val = (1 << 6) | ch_base | ONEHOT_B11; //initbuf of ch0
    	AUD_REG->adcp_init_ctrl = val;	  //adcp_init_ctrl
    	F_Adcp_WaitInitOk();
    	val = (1 << 6) | ch_base | 2 | ONEHOT_B10;     //coeff_idx=2, inc=1

    	AUD_REG->adcp_init_ctrl = val;      //adcp_init_ctrl
    	//F_Adcp_WriteIIRcoeff();

    	val = 0x800000;
    	AUD_REG->adcp_gain_0 = val;      //adcp_gain0

    	val = AUD_REG->adcp_risc_gain;  //read adcp_risc_gain
    	val = val & 0xfff0;
    	val = val | 1;
    	AUD_REG->adcp_risc_gain = val;  //adcp_risc_gain

    	//end of ch0//

    	//ch1//
    	ch_base = 1 << 4;
    	val = ( 1 << 6) | ch_base | ONEHOT_B11; //initbuf of ch0
    	AUD_REG->adcp_init_ctrl = val;	  //adcp_init_ctrl
    	F_Adcp_WaitInitOk();
    	val = ( 1 << 6 ) | ch_base | 2 | ONEHOT_B10;     //coeff_idx=2, inc=1

    	AUD_REG->adcp_init_ctrl = val;      //adcp_init_ctrl
    	//F_Adcp_WriteIIRcoeff();

    	val = 0x800000;
    	AUD_REG->adcp_gain_1 = val;      //adcp_gain1

    	val = AUD_REG->adcp_risc_gain;  //read adcp_risc_gain
    	val = val & 0xff0f;
    	val = val | 0x10;
    	AUD_REG->adcp_risc_gain = val;  //adcp_risc_gain

    	//end of ch1//
}//F_Cfg_AdcIn

static void _i2s_reset_all_fifo(void)
{
	AUD_REG->audif_ctrl  = 0x1;    //aud_ctrl = 1 G60.0
	AUD_REG->audif_ctrl  = 0x0;
	while(AUD_REG->aud_fifo_reset);
}

static void _i2s_config_pcm_buffer(I2S_HandleTypeDef *hi2s) //FIXME  RX/TX same buffer
{
	uint32_t base_addr, fifo_addr;

	/* Set FIFO mode */
	if(hi2s->Init.DataFormat == I2S_DATAFORMAT_16B)
		WRITE_REG(AUD_REG->aud_fifo_mode, 0x20001);
	else if(hi2s->Init.DataFormat == I2S_DATAFORMAT_24B)
		WRITE_REG(AUD_REG->aud_fifo_mode, 0x20000);
#if 0
	base_addr = ROUNDUP((uint32_t)&_heap_bottom, 128);
	WRITE_REG(AUD_REG->aud_audhwya, base_addr);

	printf("\n[i2s_test] Start of configure PCM buffer audhwya 0x%x \n", AUD_REG->aud_audhwya);

	fifo_addr = (uint32_t)malloc(DRAM_PCM_BUF_CH_BYTES);
	fifo_addr = ROUNDUP(fifo_addr, 128);

	WRITE_REG(*hi2s->Reg.fifo_base, fifo_addr - base_addr);
	WRITE_REG(*hi2s->Reg.fifo_len, DRAM_PCM_BUF_CH_BYTES);
#else
	uint32_t fifo_offset;
	base_addr = 0x21000000;
	WRITE_REG(AUD_REG->aud_audhwya, base_addr);
	//printf("[i2s_test] Start of configure PCM buffer audhwya 0x%x \n", AUD_REG->aud_audhwya);

	fifo_offset = DRAM_PCM_BUF_CH_BYTES * (2 * hi2s->Index + hi2s->FlagTxRx);
	fifo_offset = ROUNDUP(fifo_offset, 128);

	WRITE_REG(*hi2s->Reg.fifo_base, fifo_offset);
	WRITE_REG(*hi2s->Reg.fifo_len, DRAM_PCM_BUF_CH_BYTES);
#endif
	printf("[i2s_test] address of I2S%d FIFO base 0x%x \n", hi2s->Index, *hi2s->Reg.fifo_base);
}

/* I2S0 RX/TX each use a pin, I2S1/2 RX/TX use a pin in common */
static void _i2s_pin_mux(I2S_HandleTypeDef *handle)
{
	HAL_Module_Clock_enable(AUD, 1);
	HAL_Module_Clock_gate(AUD, 1);
	HAL_Module_Reset(AUD, 0);

	switch(handle->Index) {
		case INDEX_I2S0:
			HAL_PINMUX_Cfg(PINMUX_AUD_DAC_CLK_XCK, 1);// ???1.5[11] I2S0 bck/lrck on {x1}
			//HAL_PINMUX_Cfg(PINMUX_AUD_DAC_CLK_XCK1, 1);// ???1.5[10] I2S0 bck/lrck on {x1}
			HAL_PINMUX_Cfg(PINMUX_AUD_AU_ADC_DATA0, 1);// 1.6[1:0] I2S0 data pin TX/RX mode on {x1}
			break;
		case INDEX_I2S1:
			HAL_PINMUX_Cfg(PINMUX_AUD_AU1_CK, 1);// 1.5[15] I2S1 bck/lrck pin on {x1}
			if(handle->FlagTxRx == I2S_TX)
				HAL_PINMUX_Cfg(PINMUX_AUD_AU1_DATA0, 1);// 1.5[13] I2S1 data pin select TX mode {x1}
			else
				HAL_PINMUX_Cfg(PINMUX_AUD_ADC1_DATA0, 1);// 1.6[3] I2S1 data pin select RX mode {x1}
			break;
		case INDEX_I2S2:
			HAL_PINMUX_Cfg(PINMUX_AUD_AU2_CK, 1);// 1.5[14] I2S2 bck/lrck on {x1}
			if(handle->FlagTxRx == I2S_TX)
				HAL_PINMUX_Cfg(PINMUX_AUD_AU2_DATA0, 1);// 1.5[14] I2S2 data pin select TX mode {x1}
			else
				HAL_PINMUX_Cfg(PINMUX_AUD_ADC2_DATA0, 1);// 1.6[2] I2S2 data pin select RX mode {x1}
			break;
	}
	/* workaround for pinmux bug: Config reg G1.3 and then must clear/set G1.2 bit[18] */
	//MOON1_REG->sft_cfg[2] = 0x40000;
}

static void _i2s_playback_debug(I2S_HandleTypeDef *hi2s)
{
	/* multi write 0 debug reg may make I2S work in trouble */
	if (hi2s->FlagDebug == I2S_DEBUG_OFF)
		return;

	switch(hi2s->Index) {
		case INDEX_I2S0:
			AUD_REG->aud_embedded_input_ctrl |= (hi2s->FlagDebug << I2S0_DEBUG_BIT);
			break;
		case INDEX_I2S1:
			AUD_REG->aud_embedded_input_ctrl |= (hi2s->FlagDebug << I2S1_DEBUG_BIT);
			break;
		case INDEX_I2S2:
			AUD_REG->aud_embedded_input_ctrl |= (hi2s->FlagDebug << I2S2_DEBUG_BIT);
			break;
	}
}

static void _i2s_trigger(I2S_HandleTypeDef *hi2s)
{
	uint32_t val;

	val = READ_REG(AUD_REG->aud_enable);//G60.1
    	val |= hi2s->FlagAudEn;
	WRITE_REG(AUD_REG->aud_enable, val);
}

static void _i2s_turnoff(I2S_HandleTypeDef *hi2s)
{

}

void _i2s_set_pll(I2S_HandleTypeDef *hi2s)
{
	switch(hi2s->Init.AudioFreq) {
		case I2S_AUDIOFREQ_8K:
		case I2S_AUDIOFREQ_16K:
		case I2S_AUDIOFREQ_32K:
		case I2S_AUDIOFREQ_48K:
		case I2S_AUDIOFREQ_96K:
		case I2S_AUDIOFREQ_192K:
			hi2s->PllSrc = PLLA_147M;
			break;
		case I2S_AUDIOFREQ_11K:
		case I2S_AUDIOFREQ_22K:
		case I2S_AUDIOFREQ_44K:
			hi2s->PllSrc = PLLA_135M;
			break;
	}
	AUD_Set_PLL(hi2s->PllSrc);
}

void _i2s_param_analy(I2S_HandleTypeDef *hi2s)
{
	switch(hi2s->Init.Mode) {
		case I2S_MODE_SLAVE_TX:
			hi2s->FlagTxRx = I2S_TX;
			hi2s->FlagSrcXckBck = I2S_EXT_XCK_BCK;
			hi2s->FlagMode = I2S_SLAVE;
			hi2s->TxOffset = 0;
			break;
		case I2S_MODE_SLAVE_RX:
			hi2s->FlagTxRx = I2S_RX;
			hi2s->FlagSrcXckBck = I2S_EXT_XCK_BCK;
			hi2s->FlagMode = I2S_SLAVE;
			hi2s->RxOffset = 0;
			workaroud_for_config_rx_slave(hi2s);
			break;
		case I2S_MODE_MASTER_TX:
			hi2s->FlagTxRx = I2S_TX;
			hi2s->FlagSrcXckBck = I2S_INT_XCK_BCK;
			hi2s->FlagMode = I2S_MASTER;
			hi2s->TxOffset = 0;
			break;
		case I2S_MODE_MASTER_RX:
			hi2s->FlagTxRx = I2S_RX;
			hi2s->FlagSrcXckBck = I2S_INT_XCK_BCK;
			hi2s->FlagMode = I2S_MASTER;
			hi2s->RxOffset = 0;
			break;
	}
}

/* i2s1_tx and i2s2_rx correspond to LSB/MSB in G60.8 */
static void _i2s_format_config(I2S_HandleTypeDef *hi2s)
{
	uint32_t val, store_lsb;

	val = READ_REG(*hi2s->Reg.data_cfg);

	/* Store the I2S1 TX config */
	if(hi2s->Index == INDEX_I2S2 && hi2s->FlagTxRx == I2S_RX) {
		store_lsb = val & GENMASK(3, 0);
		val = (val >> 16);
	}

	CLEAR_BIT(val, I2S_MODE_MASK);
	SET_BIT(val, hi2s->FlagMode << I2S_MODE_BIT);

	CLEAR_BIT(val, I2S_LRCYCLE_MASK);
	if(hi2s->Init.DataFormat == I2S_DATAFORMAT_16B)
		SET_BIT(val, LRCK_CYCLE_16 << I2S_LRCYCLE_BIT);
	else if(hi2s->Init.DataFormat == I2S_DATAFORMAT_24B)
		SET_BIT(val, LRCK_CYCLE_32 << I2S_LRCYCLE_BIT);

	/* Workaround for data on the left and right channels is reversed */
	SET_BIT(val, 1 << I2S_DPARITY_BIT);

	CLEAR_BIT(val, I2S_DWIDTH_MASK);
	SET_BIT(val, hi2s->Init.DataFormat << I2S_DWIDTH_BIT);

	/* I2S must delay one BCK when LRCK level conversion */
	SET_BIT(val, 1 << I2S_DELAY_BIT);

	if(hi2s->Index == INDEX_I2S2 && hi2s->FlagTxRx == I2S_RX)
		val = (val << 16) | store_lsb;

	printf("[i2s_test] #format_config    reg 0x%x    val 0x%x\n", hi2s->Reg.data_cfg, val);
	WRITE_REG(*hi2s->Reg.data_cfg, val);
	//*(volatile unsigned int *)0xf8800488 = 0x5d;
}

void _i2s_init_reg(I2S_HandleTypeDef *hi2s)
{
	switch(hi2s->Index) {
		case INDEX_I2S0:
			hi2s->Reg.bck = &AUD_REG->aud_ext_dac_bck_cfg;
			hi2s->Reg.xck = &AUD_REG->aud_ext_dac_xck_cfg;
			if(hi2s->FlagTxRx == I2S_TX) {
				hi2s->Reg.data_cfg = &AUD_REG->pcm_cfg;
				hi2s->Reg.fifo_base = &AUD_REG->aud_a0_base;
				hi2s->Reg.fifo_len = &AUD_REG->aud_a0_length;
				hi2s->Reg.fifo_ptr = &AUD_REG->aud_a0_ptr;
				hi2s->Reg.fifo_cnt = &AUD_REG->aud_a0_cnt;
				hi2s->FlagFifoEn = I2S_TX_FIFO_EN0;
				hi2s->FlagAudEn = I2S_TX_AUD_EN0;
				hi2s->FlagDMAInc = I2S_TX_INC0;
			} else {
				hi2s->Reg.data_cfg = &AUD_REG->ext_adc_cfg;
				hi2s->Reg.fifo_base = &AUD_REG->aud_a11_base;
				hi2s->Reg.fifo_len = &AUD_REG->aud_a11_length;
				hi2s->Reg.fifo_ptr = &AUD_REG->aud_a11_ptr;
				hi2s->Reg.fifo_cnt = &AUD_REG->aud_a11_cnt;
				hi2s->FlagFifoEn = I2S_RX_FIFO_EN0;
				hi2s->FlagAudEn = I2S_RX_AUD_EN0;
				hi2s->FlagDMAInc = I2S_RX_INC0;
			}
			break;
		case INDEX_I2S1:
			hi2s->Reg.bck = &AUD_REG->aud_int_dac_bck_cfg;
			hi2s->Reg.xck = &AUD_REG->aud_int_dac_xck_cfg;
			if(hi2s->FlagTxRx == I2S_TX) {
				hi2s->Reg.data_cfg = &AUD_REG->int_adc_dac_cfg;//LSB
				hi2s->Reg.fifo_base = &AUD_REG->aud_a6_base;
				hi2s->Reg.fifo_len = &AUD_REG->aud_a6_length;
				hi2s->Reg.fifo_ptr = &AUD_REG->aud_a6_ptr;
				hi2s->Reg.fifo_cnt = &AUD_REG->aud_a6_cnt;
				hi2s->FlagFifoEn = I2S_TX_FIFO_EN1;
				hi2s->FlagAudEn = I2S_TX_AUD_EN1;
				hi2s->FlagDMAInc = I2S_TX_INC1;
			} else {
				hi2s->Reg.data_cfg = &AUD_REG->hdmi_rx_i2s_cfg;
				hi2s->Reg.fifo_base = &AUD_REG->aud_a16_base;
				hi2s->Reg.fifo_len = &AUD_REG->aud_a16_length;
				hi2s->Reg.fifo_ptr = &AUD_REG->aud_a16_ptr;
				hi2s->Reg.fifo_cnt = &AUD_REG->aud_a16_cnt;
				hi2s->FlagFifoEn = I2S_RX_FIFO_EN1;
				hi2s->FlagAudEn = I2S_RX_AUD_EN1;
				hi2s->FlagDMAInc = I2S_RX_INC1;
			}
			break;
		case INDEX_I2S2:
			hi2s->Reg.bck = &AUD_REG->aud_hdmi_tx_bclk_cfg;
			hi2s->Reg.xck = &AUD_REG->aud_hdmi_tx_mclk_cfg;
			if(hi2s->FlagTxRx == I2S_TX) {
				hi2s->Reg.data_cfg = &AUD_REG->hdmi_tx_i2s_cfg;
				hi2s->Reg.fifo_base = &AUD_REG->aud_a19_base;
				hi2s->Reg.fifo_len = &AUD_REG->aud_a19_length;
				hi2s->Reg.fifo_ptr = &AUD_REG->aud_a19_ptr;
				hi2s->Reg.fifo_cnt = &AUD_REG->aud_a19_cnt;
				hi2s->FlagFifoEn = I2S_TX_FIFO_EN2;
				hi2s->FlagAudEn = I2S_TX_AUD_EN2;
				hi2s->FlagDMAInc = I2S_TX_INC2;
			} else {
				hi2s->Reg.data_cfg = &AUD_REG->int_adc_dac_cfg;//MSB
				hi2s->Reg.fifo_base = &AUD_REG->aud_a10_base;
				hi2s->Reg.fifo_len = &AUD_REG->aud_a10_length;
				hi2s->Reg.fifo_ptr = &AUD_REG->aud_a10_ptr;
				hi2s->Reg.fifo_cnt = &AUD_REG->aud_a10_cnt;
				hi2s->FlagFifoEn = I2S_RX_FIFO_EN2;
				hi2s->FlagAudEn = I2S_RX_AUD_EN2;
				hi2s->FlagDMAInc = I2S_RX_INC2;
			}
			break;
	}
}

void _i2s_xck_cfg(I2S_HandleTypeDef *hi2s, uint16_t div)
{
	uint32_t val;

	val = READ_REG(*hi2s->Reg.xck);
	val &= ~(XCK_EN_MSK | XCK_OE_MSK | XCK_SRC_MSK | XCK_DIV_MSK);

	val |= (XBCK_ENABLE << XCK_EN_BIT);
	val |= (XBCK_ENABLE << XCK_OE_BIT);
	val |= (XBCK_SRC_PLLA << XCK_SRC_BIT);
	val |= (div << XCK_DIV_BIT);

	WRITE_REG(*hi2s->Reg.xck, val);
}

void _i2s_bck_cfg(I2S_HandleTypeDef *hi2s, uint16_t div)
{
	uint32_t val;

	val = READ_REG(*hi2s->Reg.bck);
	val &= ~(BCK_EN_MSK | BCK_SRC_MSK | BCK_DIV_MSK);

	val |= (BCK_SRC_XCK << BCK_SRC_BIT);
	val |= (XBCK_ENABLE << BCK_EN_BIT);
	val |= (div << BCK_DIV_BIT);

	WRITE_REG(*hi2s->Reg.bck, val);
}

/* i2s1 tx BCK fixed divided by 2. To avoid add the specif case for i2s1 tx.
 * Temporarily set all the bck division to 2. Only modify xck division.
 */
void _i2s_aud_clk_cfg(I2S_HandleTypeDef *hi2s)
{
	int bck_div, xck_div;

	if(hi2s->FlagMode == I2S_SLAVE)
		return;

	switch(hi2s->Init.AudioFreq) {
		case I2S_AUDIOFREQ_8K:
		case I2S_AUDIOFREQ_16K:
		case I2S_AUDIOFREQ_32K:
		case I2S_AUDIOFREQ_48K:
		case I2S_AUDIOFREQ_96K:
		case I2S_AUDIOFREQ_192K:
			hi2s->PllSrc = PLLA_147M;
			break;
		case I2S_AUDIOFREQ_11K:
		case I2S_AUDIOFREQ_22K:
		case I2S_AUDIOFREQ_44K:
			hi2s->PllSrc = PLLA_135M;
			break;
	}

	AUD_Set_PLL(hi2s->PllSrc);

	if (hi2s->Init.AudioFreq == I2S_AUDIOFREQ_8K)
	{
		/* One of bit[10:7] set 1 and div 3, One of bit[6:0] set 1 and div 2
		 * e.x 0x83 = 0b1000 0111, div = 3 * (2 * 2 * 2)
		 * Specilly, the BCK must set, mean bck_div = 2 at least
		 */
		xck_div = BIT(8) | BIT(7) | BIT(3) | BIT(2) | BIT(1) | BIT(0); // 3*3*2*2*2*2 = 144
		bck_div = BIT(0); // 2
	}
	else if (hi2s->Init.AudioFreq == I2S_AUDIOFREQ_22K)
	{
		xck_div = BIT(7) | BIT(3) | BIT(2) | BIT(1) | BIT(0); // 3*2*2*2*2=48
		bck_div = BIT(0); // 2
	}
	else if((hi2s->Init.AudioFreq == I2S_AUDIOFREQ_44K) || (hi2s->Init.AudioFreq == I2S_AUDIOFREQ_48K))
	{
		xck_div = BIT(7) | BIT(2) | BIT(1) | BIT(0); // 24
		bck_div = BIT(0); // 2
	}
	else if(hi2s->Init.AudioFreq == I2S_AUDIOFREQ_96K)
	{
		xck_div = BIT(7) | BIT(1) | BIT(0); // 12
		bck_div = BIT(0); // 2
	}
	else if(hi2s->Init.AudioFreq == I2S_AUDIOFREQ_192K)
	{
		xck_div = BIT(7) | BIT(0); // 6
		bck_div = BIT(0); //2
	}

	/* S16_LE BCK is divided by 2 more than S24_3LE */
	if (hi2s->Init.DataFormat == I2S_DATAFORMAT_16B)
		xck_div |= BIT(4);

	_i2s_xck_cfg(hi2s, xck_div);
	_i2s_bck_cfg(hi2s, bck_div);

	/* Debug info */
	dump_i2s_clk_info(hi2s);
}

static void _i2s_fifo_enable(I2S_HandleTypeDef *hi2s)
{
	uint32_t val;

	val = READ_REG(AUD_REG->aud_fifo_enable);
    	val |= hi2s->FlagFifoEn;
    	WRITE_REG(AUD_REG->aud_fifo_enable, val);
}

static void _i2s_fifo_reset_block(I2S_HandleTypeDef *hi2s)
{
    	WRITE_REG(AUD_REG->aud_fifo_reset, hi2s->FlagFifoEn);
	while (AUD_REG->aud_fifo_reset);
}

/* adcprc = DC notch + AGC + gain, only for RX,
 * channel 0 <-> i2s0, channel 1 <-> i2s1, refer the block diagram
 * Note: channel 0/1 must be enabled together.
 */
static void _i2s_adcprc_enable(I2S_HandleTypeDef *hi2s)
{
	if(hi2s->FlagTxRx == I2S_TX)
		return;

	WRITE_REG(AUD_REG->adcp_ch_enable, 0xf);//FIXME: why set 0x3 not work
}

/* i2s1 select fifo 6, i2s2 select fifo 19, only for TX,
 * their path all not through the grm, refer the block diagram
 */
static void _i2s_grm_sel(I2S_HandleTypeDef *hi2s)
{
	uint32_t val;

	if((hi2s->FlagTxRx == I2S_RX) || (hi2s->Index == INDEX_I2S0))
		return;

	val = READ_REG(AUD_REG->aud_grm_path_select);
	val |= hi2s->Index;
	WRITE_REG(AUD_REG->aud_grm_path_select, val);
}

static uint32_t _i2s_get_fifo_cnt(I2S_HandleTypeDef *hi2s)
{
	return READ_REG(*hi2s->Reg.fifo_cnt);
}

static uint32_t _i2s_get_fifo_ptr(I2S_HandleTypeDef *hi2s)
{
	return READ_REG(*hi2s->Reg.fifo_ptr);
}

static uint32_t _i2s_dma_check_trans_done(uint32_t ch)
{
	uint32_t val;

	val = READ_REG(AUD_REG->aud_inc_0);
	val &= ch;

	return val;
}

static void _i2s_dma_set_trans_length(uint32_t len)
{
	WRITE_REG(AUD_REG->aud_delta_0, len);
}


static void _i2s_dma_trans_start(uint32_t ch)
{
	WRITE_REG(AUD_REG->aud_inc_0, ch);
}

static uint32_t _i2s_get_fifo_addr(I2S_HandleTypeDef *hi2s)
{
	uint32_t base, offset;

	base = READ_REG(AUD_REG->aud_audhwya);
	offset = READ_REG(*hi2s->Reg.fifo_base);

	return (base + offset);
}

void HAL_I2S_Default_Init(void)
{
	AUD_REG->pcm_cfg		= 0x24d; 	//G60.2  I2S0 TX
	AUD_REG->ext_adc_cfg		= 0x24d;	//G60.4  I2S0 RX slave mode 14d/24d

	AUD_REG->int_adc_dac_cfg	= 0x24d;	//G60.8  I2S1 TX
	AUD_REG->hdmi_rx_i2s_cfg	= 0x24d;	//G62.16 I2S1 RX

	AUD_REG->hdmi_tx_i2s_cfg	= 0x24d;	//G62.15 I2S2 TX
	AUD_REG->int_adc_dac_cfg	= 0x24d024d;	//G60.8  I2S2 RX

	AUD_REG->aud_ext_dac_xck_cfg  	= 0x6887;	//PLLA, 256FS 62.0
	AUD_REG->aud_int_dac_xck_cfg	= 0x6887;	//PLLA, 256FS 62.7
	AUD_REG->aud_hdmi_tx_mclk_cfg	= 0x6887;	//PLLA, 256FS 62.13

	AUD_REG->aud_ext_dac_bck_cfg	= 0x6001;	//64FS 62.1
	AUD_REG->aud_int_dac_bck_cfg	= 0x6001;	//64FS 62.8  Fixed divided by 2
	AUD_REG->aud_hdmi_tx_bclk_cfg	= 0x6001;
}

/*-----------------------------------------------------------*/

/**
  * @brief  Initializes the I2S according to the specified parameters
  *         in the I2S_InitTypeDef and create the associated handle.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Init(I2S_HandleTypeDef *hi2s)
{
	printf("\n[i2s_test] ########### I2S%d Init ##########\n", hi2s->Index);
	/* Check the I2S handle allocation */
	if (hi2s == NULL)
	{
		return HAL_ERROR;
	}

	uint32_t val;

	_i2s_param_analy(hi2s);
	_i2s_init_reg(hi2s);

	/* Config pinmux */
	_i2s_pin_mux(hi2s);

	/* Set XCK and BCK divsion. _i2s_aud_clk_cfg() which contains
	 * the operation of plla active needs to be placed before all
	 * register read and write operations.
	 */
	_i2s_aud_clk_cfg(hi2s);

	/* Config GRM */
	F_Mixer_Setting();///////////////////////////////TODO

	/* Config PLL*/
	//_i2s_set_pll();//put in _i2s_aud_clk_cfg() to calculate the freq easily;

	/* Config master/slave mode, LRCK cycle and pcm data format */
	_i2s_format_config(hi2s);

	/* Debug function */
	//_i2s_playback_debug(hi2s);

	/* Fix */
	if (hi2s->FlagTxRx == I2S_RX)
		F_Cfg_AdcIn();/////////////////////////////////TODO

	_i2s_grm_sel(hi2s);
	_i2s_adcprc_enable(hi2s);

	/* Config the DMA memory */
	_i2s_config_pcm_buffer(hi2s);

	_i2s_fifo_enable(hi2s);
	_i2s_fifo_reset_block(hi2s);
	//_i2s_trigger(hi2s);

	printf("[aud_test] i2s init end\n");
	hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
	hi2s->State     = HAL_I2S_STATE_READY;

	return HAL_OK;
}

/**
  * @brief DeInitializes the I2S peripheral
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DeInit(I2S_HandleTypeDef *hi2s)
{
	/* Check the I2S handle allocation */
	if (hi2s == NULL)
	{
		return HAL_ERROR;
	}

	hi2s->State = HAL_I2S_STATE_BUSY;

	/* Disable the I2S Peripheral Clock */
	//__HAL_I2S_DISABLE(hi2s);


	hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
	hi2s->State     = HAL_I2S_STATE_RESET;

	/* Release Lock */
	__HAL_UNLOCK(hi2s);

	return HAL_OK;
}

/* Must config TX Mode*/
HAL_StatusTypeDef HAL_I2S_Debug_Tx_Sine(HAL_I2S_IndexTypeDef index, HAL_I2S_DebugTypeDef db)
{
	I2S_HandleTypeDef hi2s;

	hi2s.Index = index;
	hi2s.FlagDebug = db;
	hi2s.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s.Init.DataFormat = I2S_DATAFORMAT_24B;
	hi2s.Init.AudioFreq = I2S_AUDIOFREQ_48K;

	_i2s_param_analy(&hi2s);
	_i2s_init_reg(&hi2s);

	/* Config pinmux */
	_i2s_pin_mux(&hi2s);

	/* Set XCK and BCK divsion, _i2s_aud_clk_cfg() which contains
	 * the operation of plla active needs to be placed before all
	 * register read and write operations.
	 */
	_i2s_aud_clk_cfg(&hi2s);

	/* Config master/slave mode, LRCK cycle and pcm data format */
	_i2s_format_config(&hi2s);

	/* Debug function */
	_i2s_playback_debug(&hi2s);

	_i2s_trigger(&hi2s);

	return HAL_OK;
}

/* @brief Transmit the data of a file
 * @param hi2s pointer to a I2S_HandleTypeDef structure that contains
 *        the configuration information for I2S module
 * @param pData a 32-bit pointer to data buffer. This buffer is not
 * 	  corresponding to the FIFO
 * @param Size number of data sample to be sent
 */
HAL_StatusTypeDef HAL_I2S_Transmit(I2S_HandleTypeDef *hi2s, uint32_t *pData, uint32_t Size, uint32_t Timeout)
{
	uint32_t free_cnt;
	uint32_t run_block;
	uint32_t remainder;
	uint32_t copy_len;
	uint32_t front_len, back_len;
	uint32_t *fifo_addr;
	uint32_t pre_offset;

	uint32_t w_lenth = DMA_TRANS_LEN;
	uint32_t loop = 0, done_cnt = 0;
	uint32_t dma_ch = hi2s->FlagDMAInc;

	if ((pData == NULL) || (Size == 0U))
	{
		return  HAL_ERROR;
	}

	/* Process Locked */
	__HAL_LOCK(hi2s);

	if (hi2s->State != HAL_I2S_STATE_READY)
	{
		__HAL_UNLOCK(hi2s);
		return HAL_BUSY;
	}

	/* Set state and reset error code */
	hi2s->State = HAL_I2S_STATE_BUSY_TX;
	hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
	hi2s->pTxBuffPtr = pData;

	fifo_addr = (uint32_t *)_i2s_get_fifo_addr(hi2s);

	run_block = Size / w_lenth;
	remainder = Size % w_lenth;//1996
	if (remainder)
		run_block++;

	_i2s_trigger(hi2s);

	/* dont use for(;;), it is not certain that data is copied in each loop */
	while (done_cnt < Size) {

		do {
			free_cnt = DRAM_PCM_BUF_CH_BYTES - _i2s_get_fifo_cnt(hi2s);
		} while (free_cnt < w_lenth);

		if(loop == (run_block - 1)) { //last part of pcm data
			remainder ? (copy_len = remainder) : (copy_len = w_lenth);
		} else {
			copy_len = w_lenth;
		}

		/* Consider the case that the size of channel (DRAM_PCM_BUF_CH_BYTES)
		 * is not multiple of transfer data (copy_len)
		 */
		pre_offset = hi2s->TxOffset + copy_len;
		if(pre_offset > DRAM_PCM_BUF_CH_BYTES) {
			back_len = pre_offset - DRAM_PCM_BUF_CH_BYTES;
			front_len = copy_len - back_len;
			//copy_data(fifo_addr + hi2s->TxOffset / 4, hi2s->pTxBuffPtr + done_cnt / 4, front_len / 4, hi2s->Init.DataFormat);
			//copy_data(fifo_addr, hi2s->pTxBuffPtr + (done_cnt + front_len) / 4, back_len / 4, hi2s->Init.DataFormat);
			HAL_DMA_Start((uint32_t)(hi2s->pTxBuffPtr + done_cnt / 4), (uint32_t)(fifo_addr + hi2s->TxOffset / 4), front_len, NULL, NULL);
			HAL_DMA_Start((uint32_t)(hi2s->pTxBuffPtr + (done_cnt + front_len) / 4), (uint32_t)(fifo_addr), back_len, NULL, NULL);
		} else {
			HAL_DMA_Start((uint32_t)(hi2s->pTxBuffPtr + done_cnt / 4), (uint32_t)(fifo_addr + hi2s->TxOffset / 4), copy_len, NULL, NULL);
			//copy_data(fifo_addr + hi2s->TxOffset / 4, hi2s->pTxBuffPtr + done_cnt / 4, copy_len / 4, hi2s->Init.DataFormat);
		}

		/* Start the DMA transfer after copy data to buffer immediately */
		while(_i2s_dma_check_trans_done(dma_ch) != 0);

		_i2s_dma_set_trans_length(copy_len);
		_i2s_dma_trans_start(dma_ch);

		while(_i2s_dma_check_trans_done(dma_ch) != 0);

		/* I2S C/P spend longer times than CPU execution code, So put the not-essential code here */
		done_cnt += copy_len;
		loop ++;

		/* Record the fifo buffer offset to avoid data overwriting */
		hi2s->TxOffset += copy_len;
		if (hi2s->TxOffset >= DRAM_PCM_BUF_CH_BYTES)
			hi2s->TxOffset -= DRAM_PCM_BUF_CH_BYTES;

	}

	hi2s->State = HAL_I2S_STATE_READY;
	__HAL_UNLOCK(hi2s);

	return HAL_OK;
}

/**
  * @brief  Receive an amount of data in blocking mode
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData a 32-bit pointer to data buffer.
  * @param  Size number of data sample to be sent:
  * @note   When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *         configuration phase, the Size parameter means the number of 16-bit data length
  *         in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *         the Size parameter means the number of 16-bit data length.
  * @param  Timeout Timeout duration
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @note   In I2S Master Receiver mode, just after enabling the peripheral the clock will be generate
  *         in continuous way and as the I2S is not disabled at the end of the I2S transaction.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive(I2S_HandleTypeDef *hi2s, uint32_t *pData, uint32_t Size, uint32_t Timeout)
{
	uint32_t run_block;
	uint32_t remainder;
	uint32_t copy_len;
	uint32_t front_len, back_len;
	uint32_t *fifo_addr;
	uint32_t pre_offset;

	uint32_t w_lenth = DMA_TRANS_LEN;
	uint32_t loop = 0, done_cnt = 0;
	uint32_t dma_ch = hi2s->FlagDMAInc;

	if ((pData == NULL) || (Size == 0U))
	{
		return  HAL_ERROR;
	}

	/* Process Locked */
	__HAL_LOCK(hi2s);

	if (hi2s->State != HAL_I2S_STATE_READY)
	{
		__HAL_UNLOCK(hi2s);
		return HAL_BUSY;
	}

	/* Set state and reset error code */
	hi2s->State = HAL_I2S_STATE_BUSY_RX;
	hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
	hi2s->pRxBuffPtr = pData;

	fifo_addr = (uint32_t *)_i2s_get_fifo_addr(hi2s);

	run_block = Size / w_lenth;
	remainder = Size % w_lenth;
	if(Size % w_lenth)
		run_block ++;

	_i2s_trigger(hi2s);

	while(done_cnt < Size) {

		if(loop == (run_block - 1))
			remainder ? (copy_len = remainder) : (copy_len = w_lenth);
		else
			copy_len = w_lenth;

		while(_i2s_get_fifo_cnt(hi2s) < copy_len);

		while(_i2s_dma_check_trans_done(dma_ch) != 0);
		_i2s_dma_set_trans_length(copy_len);
		_i2s_dma_trans_start(dma_ch);
		while(_i2s_dma_check_trans_done(dma_ch) != 0);

		/* Consider the case that the size of channel (DRAM_PCM_BUF_CH_BYTES)
		 * is not multiple of transfer data (copy_len)
		 */
		pre_offset = hi2s->RxOffset + copy_len;
		if(pre_offset > DRAM_PCM_BUF_CH_BYTES) {
			back_len = pre_offset - DRAM_PCM_BUF_CH_BYTES;
			front_len = copy_len - back_len;
			//copy_data(hi2s->pRxBuffPtr + done_cnt / 4, fifo_addr + hi2s->RxOffset / 4, front_len / 4, hi2s->Init.DataFormat);
			//copy_data(hi2s->pRxBuffPtr + (done_cnt + front_len) / 4, fifo_addr, back_len / 4, hi2s->Init.DataFormat);
			HAL_DMA_Start((uint32_t)(fifo_addr + hi2s->RxOffset / 4), (uint32_t)(hi2s->pRxBuffPtr + done_cnt / 4), front_len, NULL, NULL);
			HAL_DMA_Start((uint32_t)(fifo_addr), (uint32_t)(hi2s->pRxBuffPtr + (done_cnt + front_len) / 4), back_len, NULL, NULL);
		} else {
			HAL_DMA_Start((uint32_t)(fifo_addr + hi2s->RxOffset / 4), (uint32_t)(hi2s->pRxBuffPtr + done_cnt / 4), copy_len, NULL, NULL);
			//copy_data(hi2s->pRxBuffPtr + done_cnt / 4, fifo_addr + hi2s->RxOffset / 4, copy_len / 4, hi2s->Init.DataFormat);
		}

		/* I2S C/P spend longer times than CPU execution code, So put the not-essential code here */
		done_cnt += copy_len;
		loop ++;

		/* Record the fifo buffer offset to avoid data overwriting */
		hi2s->RxOffset += copy_len;
		if (hi2s->RxOffset >= DRAM_PCM_BUF_CH_BYTES)
			hi2s->RxOffset -= DRAM_PCM_BUF_CH_BYTES;
	}

	hi2s->State = HAL_I2S_STATE_READY;
	__HAL_UNLOCK(hi2s);
	return HAL_OK;
}

/**
  * @brief  This function handles I2S Communication Timeout.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  Flag Flag checked
  * @param  State Value of the flag expected
  * @param  Timeout Duration of the timeout
  * @retval HAL status
  */
#define __HAL_I2S_GET_FLAG(__HANDLE__, __FLAG__) 1

#if 0
static HAL_StatusTypeDef I2S_WaitFlagStateUntilTimeout(I2S_HandleTypeDef *hi2s, uint32_t Flag, FlagStatus State, uint32_t Timeout)
{
	uint32_t tickstart;

	/* Get tick */
	tickstart = HAL_GetTick();

	/* Wait until flag is set to status*/
	while ((__HAL_I2S_GET_FLAG(hi2s, Flag) ? SET : RESET) != State)
	{
		if (Timeout != HAL_MAX_DELAY)
		{
			if (((HAL_GetTick() - tickstart) >= Timeout) || (Timeout == 0U))
			{
				/* Set the I2S State ready */
				hi2s->State = HAL_I2S_STATE_READY;

				/* Process Unlocked */
				__HAL_UNLOCK(hi2s);

				return HAL_TIMEOUT;
			}
		}
	}
	return HAL_OK;
}
#endif

#ifdef __cplusplus
}
#endif
