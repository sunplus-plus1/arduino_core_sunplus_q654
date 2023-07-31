/*
 *	Test i2s.
 *	Q654 evb board I2S1 is used for wifi module.
 *	So following special changes is required:
 *		1.Modify the circuit: R455 = 0 (disable wifi module)
 *		2.GPIO56,57 first is set 0 (disable GPIO mode)
 *
 *	Q654 evb board I2S2 is used for UA6 (CM4 console).
 *	So following special changes is required:
 *		1.MOON UA6 pinmux is set 0 (disable GPIO80 ua6 function)
 */

#include "Arduino.h"
#ifndef SP7021
#define PCM_ADDRESS_16BIT	((uint32_t *)0x24000000)
#define PCM_ADDRESS_24BIT	((uint32_t *)0x25000000)
#define TEST_BUF_SIZE		(4 * 1024)

#define STAMP(v)                                   \
	do {                                       \
		*(volatile uint32_t *)0xf8000000UL = v; \
	} while (0)

extern uint32_t single_swap_4bytes(uint32_t single);
extern void dump_common_reg(I2S_HandleTypeDef *hi2s);

uint32_t pcmdata[96];

void addr_to_group_print(uint32_t addr)
{
	uint32_t temp, group, reg;

	temp = addr - 0xf8000000;

	group = temp / 128;
	reg = (temp % 128) / 4;

	printf("# G%02d.%02d  0x%08x val 0x%08x\n", group, reg, addr, *(uint32_t *)addr);
}

void dump_common_reg(I2S_HandleTypeDef *hi2s)
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

void dump_audio_reg()
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

uint32_t single_swap_4bytes(uint32_t single)
{
	return ((single&0x000000ff)<<24|\
	(single&0x0000ff00)<<8|\
	(single&0x00ff0000)>>8|\
	(single&0xff000000)>>24);
}

void init_pcmdata(void)
{
   //1.5k 0db tone, 64 samples (stereo)
	pcmdata[0]=0x00000000;
   	pcmdata[1]=0x000018f9;
   	pcmdata[2]=0x000c7c80;
   	pcmdata[3]=0x30fb0018;
   	pcmdata[4]=0x7d80471c;
   	pcmdata[5]=0x00238e00;
   	pcmdata[6]=0x5a82002d;
   	pcmdata[7]=0x41006a6d;
   	pcmdata[8]=0x00353680;
   	pcmdata[9]=0x7641003b;
   	pcmdata[10]=0x20807d8a;
  	pcmdata[11]=0x003ec500;
   	pcmdata[12]=0x7fff003f;
   	pcmdata[13]=0xff807d8b;
   	pcmdata[14]=0x003ec580;
   	pcmdata[15]=0x7642003b;
   	pcmdata[16]=0x21006a6f;
   	pcmdata[17]=0x00353780;
   	pcmdata[18]=0x5a84002d;
   	pcmdata[19]=0x4200471f;
   	pcmdata[20]=0x00238f80;
   	pcmdata[21]=0x30fe0018;
   	pcmdata[22]=0x7f0018fc;
   	pcmdata[23]=0x000c7e00;
   	pcmdata[24]=0x00030000;
   	pcmdata[25]=0x0180e70a;
   	pcmdata[26]=0x00f38500;
   	pcmdata[27]=0xcf0700e7;
   	pcmdata[28]=0x8380b8e6;
   	pcmdata[29]=0x00dc7300;
   	pcmdata[30]=0xa58000d2;
   	pcmdata[31]=0xc0009595;
   	pcmdata[32]=0x00caca80;
   	pcmdata[33]=0x89c000c4;
   	pcmdata[34]=0xe0008276;
   	pcmdata[35]=0x00c13b00;
   	pcmdata[36]=0x800000c0;
   	pcmdata[37]=0x00008275;
   	pcmdata[38]=0x00c13a80;
   	pcmdata[39]=0x89bc00c4;
   	pcmdata[40]=0xde009590;
   	pcmdata[41]=0x00cac800;
   	pcmdata[42]=0xa57a00d2;
  	pcmdata[43]=0xbd00b8de;
   	pcmdata[44]=0x00dc6f00;
   	pcmdata[45]=0xceff00e7;
   	pcmdata[46]=0x7f80e702;
   	pcmdata[47]=0x00f38100;
   	pcmdata[48]=0x00000000;
   	pcmdata[49]=0x000018f9;
   	pcmdata[50]=0x000c7c80;
   	pcmdata[51]=0x30fb0018;
   	pcmdata[52]=0x7d80471c;
   	pcmdata[53]=0x00238e00;
   	pcmdata[54]=0x5a82002d;
   	pcmdata[55]=0x41006a6d;
   	pcmdata[56]=0x00353680;
   	pcmdata[57]=0x7641003b;
   	pcmdata[58]=0x20807d8a;
   	pcmdata[59]=0x003ec500;
   	pcmdata[60]=0x7fff003f;
   	pcmdata[61]=0xff807d8b;
   	pcmdata[62]=0x003ec580;
   	pcmdata[63]=0x7642003b;
   	pcmdata[64]=0x21006a6f;
   	pcmdata[65]=0x00353780;
   	pcmdata[66]=0x5a84002d;
   	pcmdata[67]=0x4200471f;
   	pcmdata[68]=0x00238f80;
   	pcmdata[69]=0x30fe0018;
   	pcmdata[70]=0x7f0018fc;
   	pcmdata[71]=0x000c7e00;
   	pcmdata[72]=0x00030000;
   	pcmdata[73]=0x0180e70a;
   	pcmdata[74]=0x00f38500;
   	pcmdata[75]=0xcf0700e7;
   	pcmdata[76]=0x8380b8e6;
   	pcmdata[77]=0x00dc7300;
   	pcmdata[78]=0xa58000d2;
   	pcmdata[79]=0xc0009595;
   	pcmdata[80]=0x00caca80;
   	pcmdata[81]=0x89c000c4;
   	pcmdata[82]=0xe0008276;
   	pcmdata[83]=0x00c13b00;
   	pcmdata[84]=0x800000c0;
   	pcmdata[85]=0x00008275;
   	pcmdata[86]=0x00c13a80;
   	pcmdata[87]=0x89bc00c4;
   	pcmdata[88]=0xde009590;
   	pcmdata[89]=0x00cac800;
   	pcmdata[90]=0xa57a00d2;
   	pcmdata[91]=0xbd00b8de;
   	pcmdata[92]=0x00dc6f00;
   	pcmdata[93]=0xceff00e7;
   	pcmdata[94]=0x7f80e702;
   	pcmdata[95]=0x00f38100;

	for(int i = 0; i < 96; i++)
		pcmdata[i] = single_swap_4bytes(pcmdata[i]);

}

/* TEST1:INTERNAL DEBUG, I2S2 output signal(1k 0db sine) */
void i2s_test_debug_signal(void)
{
	HAL_I2S_Debug_Tx_Sine(INDEX_I2S2, I2S_DEBUG_0DB);
}

/* TEST2:TX TEST, I2S2 output pcmdata[96] (1.5k 0db sine) */
void i2s_test_tx_pcmdata(void)
{
	uint32_t *data_buf;
	uint32_t data_len;
	I2S_HandleTypeDef hi2s;

	hi2s.Index = INDEX_I2S2;
	hi2s.FlagDebug = I2S_DEBUG_OFF;
	hi2s.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s.Init.DataFormat = I2S_DATAFORMAT_24B;
	hi2s.Init.AudioFreq = I2S_AUDIOFREQ_48K;

	HAL_I2S_Init(&hi2s);

	//dump_audio_reg();
	//debug:config the reg same as old test

	init_pcmdata();
	data_len = 384;
	data_buf = (uint32_t *)pcmdata;

	dump_common_reg(&hi2s);

	while(1) {
		HAL_I2S_Transmit(&hi2s, data_buf, data_len, 0);
	}
}

/* TEST3:2 TX TEST, I2S1 I2S2 output pcmdata[96] (1.5k 0db sine) */
void i2s_test_tx_multi_pcmdata(void)
{
	uint32_t *data_buf;
	uint32_t data_len;
	I2S_HandleTypeDef hi2s1;
	I2S_HandleTypeDef hi2s2;

	hi2s1.Index = INDEX_I2S1;
	hi2s1.FlagDebug = I2S_DEBUG_OFF;
	hi2s1.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s1.Init.DataFormat = I2S_DATAFORMAT_24B;
	hi2s1.Init.AudioFreq = I2S_AUDIOFREQ_48K;

	hi2s2.Index = INDEX_I2S2;
	hi2s2.FlagDebug = I2S_DEBUG_OFF;
	hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s2.Init.DataFormat = I2S_DATAFORMAT_24B;
	hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_48K;

	HAL_I2S_Init(&hi2s1);
	HAL_I2S_Init(&hi2s2);

	//dump_audio_reg();
	//debug:config the reg same as old test

	init_pcmdata();
	data_len = 384;
	data_buf = (uint32_t *)pcmdata;

	while(1) {
		HAL_I2S_Transmit(&hi2s1, data_buf, data_len, 0);
		HAL_I2S_Transmit(&hi2s2, data_buf, data_len, 0);
	}
}

/* TEST4: TX TEST, I2S2 output B185.bin (60s 24bit 8000Hz) */
void i2s_test_data_width(void)
{
	uint32_t *data_buf;
	uint32_t data_len;
	I2S_HandleTypeDef hi2s;

	hi2s.Index = INDEX_I2S2;
	hi2s.FlagDebug = I2S_DEBUG_OFF;
	hi2s.Init.Mode = I2S_MODE_MASTER_TX;

	#if 0//24bit
	hi2s.Init.DataFormat = I2S_DATAFORMAT_24B;
	hi2s.Init.AudioFreq = I2S_AUDIOFREQ_8K;
	data_len = 2880000;//60s B185.bin 24bit
	data_buf = PCM_ADDRESS_24BIT;
	#else//16bit
	hi2s.Init.DataFormat = I2S_DATAFORMAT_16B;
	hi2s.Init.AudioFreq = I2S_AUDIOFREQ_48K;
	//data_len = 905736;//48k16bit.pcm
	data_len = 905216;//48k16bit.pcm
	data_buf = PCM_ADDRESS_16BIT;
	#endif

	HAL_I2S_Init(&hi2s);

	dump_common_reg(&hi2s);

	while(1) {
		HAL_I2S_Transmit(&hi2s, data_buf, data_len, 0);
	}
}

/* TEST5: RX TEST, TX1 output debug 1kHz sine, RX0 receive and TX2 output,
 * compare the analog waveform.
 */
void i2s_test_rx_sine(void)
{
	uint32_t *data_buf;
	uint32_t data_len;
	uint32_t offset;

	I2S_HandleTypeDef hi2s0;
	I2S_HandleTypeDef hi2s2;

	//HAL_I2S_Default_Init();

	HAL_I2S_Debug_Tx_Sine(INDEX_I2S1, I2S_DEBUG_0DB);
	hi2s0.Index = INDEX_I2S0;
	hi2s0.Init.Mode = I2S_MODE_SLAVE_RX;
	//hi2s0.Init.Mode = I2S_MODE_MASTER_RX;
	hi2s0.Init.DataFormat = I2S_DATAFORMAT_24B;
	hi2s0.Init.AudioFreq = I2S_AUDIOFREQ_48K;

	hi2s2.Index = INDEX_I2S2;
	hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s2.Init.DataFormat = I2S_DATAFORMAT_24B;
	hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_48K;

	HAL_I2S_Init(&hi2s0);
	HAL_I2S_Init(&hi2s2);

	offset = 0;
	data_buf = (uint32_t *)malloc(TEST_BUF_SIZE);

	while(1) {
		HAL_I2S_Receive(&hi2s0, data_buf + offset / 4, TEST_BUF_SIZE / 2, 0);
		HAL_I2S_Transmit(&hi2s2, data_buf + offset / 4, TEST_BUF_SIZE / 2, 0);
		offset += TEST_BUF_SIZE / 2;
		if(offset == TEST_BUF_SIZE)
			offset = 0;
	}
}

extern int gpio_first_val_set(uint32_t bit,int firset_set);

void setup()
{
	printf("i2s example %s\n", __TIME__);

#if 0//TEST PIN is connected
	GPIO_InitTypeDef GPIO_I2S1BCK;
	memset(&GPIO_I2S1BCK,0,sizeof(GPIO_I2S1BCK));
	GPIO_I2S1BCK.Mode = GPIO_PP_OUTPUT_MODE;
	GPIO_I2S1BCK.Pin = GPIO(56);
	GPIO_I2S1BCK.out_value = GPIO_OUT_LOW;
	HAL_GPIO_Init(&GPIO_I2S1BCK);

	GPIO_InitTypeDef GPIO_I2S2BCK;
	memset(&GPIO_I2S2BCK,0,sizeof(GPIO_I2S2BCK));
	GPIO_I2S2BCK.Mode = GPIO_PP_OUTPUT_MODE;
	GPIO_I2S2BCK.Pin = GPIO(80);
	GPIO_I2S2BCK.out_value = GPIO_OUT_LOW;
	HAL_GPIO_Init(&GPIO_I2S2BCK);

	while (1) {
		HAL_GPIO_WritePin(GPIO(56),GPIO_OUT_HIGH);
		delay(10);
		HAL_GPIO_WritePin(GPIO(56),GPIO_OUT_LOW);
		delay(10);
		HAL_GPIO_WritePin(GPIO(80),GPIO_OUT_HIGH);
		delay(10);
		HAL_GPIO_WritePin(GPIO(80),GPIO_OUT_LOW);
		delay(10);
	}
#endif
#ifdef SP7350
	/* Invalid I2S1 pinmux*/
	gpio_first_val_set(GPIO(56), 0);
	gpio_first_val_set(GPIO(57), 0);
	/* Invalid I2S2 pinmux*/
	HAL_PINMUX_Cfg(PINMUX_UART6, 0);

	//HAL_I2S_Debug_Tx_Sine(INDEX_I2S1, I2S_DEBUG_0DB);
	//HAL_I2S_Debug_Tx_Sine(INDEX_I2S2, I2S_DEBUG_0DB);
#endif
#if 0
	i2s_test_debug_signal();
#endif

#if 0
	i2s_test_tx_pcmdata();
#endif

#if 0
	i2s_test_tx_multi_pcmdata();
#endif

#if 0
	i2s_test_data_width();
#endif

#if 1
	i2s_test_rx_sine();
#endif

}

void loop()
{

}
#else
void setup()
{

}

void loop()
{

}
#endif
