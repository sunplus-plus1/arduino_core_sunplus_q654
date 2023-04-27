#include "sp7350_hal_adc.h"

void HAL_ADC_Start(int adc_chan)
{
	unsigned int temp_reg;
	volatile struct dw_adc_regs *adc_regs = ADC_REG_AO;

	if((IS_ADC_CHAN_VALID(adc_chan)))
	{

		temp_reg = adc_regs->adc_cfg0b;
		temp_reg |= adc_chan << 3;
		temp_reg |= SP_ADC_EN;
		adc_regs->adc_cfg0b = temp_reg;		// adc enable

		temp_reg = adc_regs->adc_cfg02;
		temp_reg &= ~SP_ADC_CLK_DIV_MASK;
		adc_regs->adc_cfg02 = temp_reg;		// set clk div = 0

		adc_regs->adc_cfg0e = adc_chan << 3*adc_chan; 	// set adc chan

		temp_reg = adc_regs->adc_cfg0b;
		temp_reg |= SP_ADC_SRFS;
		adc_regs->adc_cfg0b = temp_reg;		// adc start
	}
}

void HAL_ADC_Stop(int adc_chan)
{
	unsigned int temp_reg;
	volatile struct dw_adc_regs *adc_regs = ADC_REG_AO;
	if((IS_ADC_CHAN_VALID(adc_chan)))
	{
		temp_reg = adc_regs->adc_cfg0b;
		temp_reg &= ~SP_ADC_EN;
		adc_regs->adc_cfg0b = temp_reg;		// adc disable
	}
}

uint32_t HAL_ADC_Get_Vaule(int adc_chan)
{
	unsigned int temp_buf=0;
	volatile struct dw_adc_regs *adc_regs = ADC_REG_AO;
	uint32_t state =  adc_regs->adc_cfg0d;

	while((state & SP_ADC_DATA_READY) != SP_ADC_DATA_READY)
	{
		state =  adc_regs->adc_cfg0d;
	}
	switch (adc_chan)
	{
		case ADC_CHAN_0:
			temp_buf = adc_regs->adc_cfg11;
			temp_buf &= SP_ADC_DATA_MASK;
		break;
		case ADC_CHAN_1:
			temp_buf = adc_regs->adc_cfg12;
			temp_buf &= SP_ADC_DATA_MASK;
		break;
		case ADC_CHAN_2:
			temp_buf = adc_regs->adc_cfg13;
			temp_buf &= SP_ADC_DATA_MASK;
		break;
		case ADC_CHAN_3:
			temp_buf = adc_regs->adc_cfg14;
			temp_buf &= SP_ADC_DATA_MASK;
		break;

	}
	return temp_buf;
}

