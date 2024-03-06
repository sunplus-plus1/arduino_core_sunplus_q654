#include "sp7350_hal_adc.h"
#include "Arduino.h"

uint32_t HAL_ADC_Get_Vaule(int adc_chan)
{
	unsigned int temp_reg,temp_buf;
	volatile uint32_t stat;
	volatile struct dw_adc_regs *adc_regs = ADC_REG_AO;

	HAL_HW_Init(SAR12B);

	temp_reg = adc_regs->adc_cfg0b;
	temp_reg |= SP_ADC_EN;
	adc_regs->adc_cfg0b = temp_reg;		// adc enable

	temp_reg = adc_regs->adc_cfg02;
	temp_reg &= ~SP_ADC_CLK_DIV_MASK;
	adc_regs->adc_cfg02 = temp_reg;		// set clk div = 0

	temp_reg = adc_regs->adc_cfg0b;
	temp_reg &= ~SP_ADC_CHEN_MASK;

	switch (adc_chan) {
			case 0:
				temp_reg |= SP_ADC_CHAN0_SET;
			break;
			case 1:
				temp_reg |= SP_ADC_CHAN1_SET;
			break;
			case 2:
				temp_reg |= SP_ADC_CHAN2_SET;
			break;
			case 3:
				temp_reg |= SP_ADC_CHAN3_SET;
			break;
	}

	temp_reg &= ~SP_ADC_SRFS;
	adc_regs->adc_cfg0b = temp_reg; 	// adc start
	delay(1);
	temp_reg |= SP_ADC_SRFS;
	adc_regs->adc_cfg0b = temp_reg;		// adc start
	delay(1);

	temp_reg = adc_regs->adc_cfg02;
	temp_reg |=  SP_ADC_BYPASS;
	adc_regs->adc_cfg02 = temp_reg;

	stat =  adc_regs->adc_cfg0d;

	while((stat & SP_ADC_DATA_READY) != SP_ADC_DATA_READY)
	{
		stat =  adc_regs->adc_cfg0d;
	}

	temp_buf = adc_regs->adc_cfg0d;
	temp_buf &= SP_ADC_DATA_MASK;
	return temp_buf;

}

