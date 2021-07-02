#include "icm.h"

#define TEST_PWM

ICM_InitTypeDef ICM_Init[ICM_NUM];

static int get_instance_index(uint32_t instance)
{
	int index;
	switch(instance)
	{
		case (uint32_t)SP_ICM0_REG:index=ICM0;break;
		case (uint32_t)SP_ICM1_REG:index=ICM1;break;
		case (uint32_t)SP_ICM2_REG:index=ICM2;break;
		case (uint32_t)SP_ICM3_REG:index=ICM3;break;
	}
	return index;
}

void ICM_Initialization()
{
	printf("icm Build @%s, %s\n", __DATE__, __TIME__);
	
	ICM_CTRL_REG* instance;
	int index, muxsel, clksel, eemode, etimes, dtimes, cntscl, tstscl, Pin_data, Pin_clk;
	muxsel = clksel = eemode = etimes = dtimes = cntscl = tstscl = Pin_data = Pin_clk = -1;
	
	//get_para_val();
	muxsel = ICM_MUXSEL_INPUT0;
	clksel = ICM_CLKSEL_27M;
	eemode = ICM_EEMODE_BOTH;
	etimes = 4;
	dtimes = 3;

	instance = SP_ICM0_REG;
	index = get_instance_index((uint32_t)instance);

	ICM_Init[index].index = index;
	ICM_Init[index].instance = instance;
	
	/* set icm configure */
	HAL_ICM_GetConfig(&ICM_Init[index]);
	if (muxsel != -1) ICM_Init[index].muxsel = muxsel;
	if (clksel != -1) ICM_Init[index].clksel = clksel;
	if (eemode != -1) ICM_Init[index].eemode = eemode;
	if (etimes != -1) ICM_Init[index].etimes = etimes;
	if (dtimes != -1) ICM_Init[index].dtimes = dtimes;
	if (cntscl != -1) ICM_Init[index].cntscl = cntscl;
	if (tstscl != -1) ICM_Init[index].tstscl = tstscl;
	HAL_ICM_Init(&ICM_Init[index]);

	/* select pinmux :HAL_ICM_PINMUX must be called after setting clksel*/
	if (Pin_data != -1)
		ICM_Init[index].Pin_data = Pin_data;
	if (Pin_clk != -1)
		ICM_Init[index].Pin_clk = Pin_clk;
	HAL_ICM_PINMUX(&ICM_Init[index], PINMUX_ICM_DATA, PINMUX_ICM_CLK);

	/* config interrupt*/
	IRQn_Type irqn = ICM_Init[index].index + 92;
	switch(irqn)
	{
		case ICM0_IRQn:IRQ_SetHandler(ICM0_IRQn, &ICM0_IRQHandler);break;
		case ICM1_IRQn:IRQ_SetHandler(ICM1_IRQn, &ICM1_IRQHandler);break;
		case ICM2_IRQn:IRQ_SetHandler(ICM2_IRQn, &ICM2_IRQHandler);break;
		case ICM3_IRQn:IRQ_SetHandler(ICM3_IRQn, &ICM3_IRQHandler);break;
	}
	IRQ_SetMode(irqn, IRQ_MODE_TRIG_EDGE_RISING);
	IRQ_Enable(irqn);
#ifdef TEST_PWM
	/* config pwm */
	PWM_InitTypeDef PWM_Init={0};
	PWM_Init.Pin = PINMUX_PIN2_01;
	PWM_Init.pwm_num = PWM0;
	PWM_Init.pwm_freq = 8000;
	PWM_Init.duty_cycle = 50;
	HAL_PWM_INIT(&PWM_Init);
	HAL_PWM_ENABLE(PWM0);
#endif 
}

void ICM0_IRQHandler()
{
	HAL_ICM_IRQHandler(&ICM_Init[ICM0]);
}

void ICM1_IRQHandler()
{
	HAL_ICM_IRQHandler(&ICM_Init[ICM1]);
}

void ICM2_IRQHandler()
{
	HAL_ICM_IRQHandler(&ICM_Init[ICM2]);
}

void ICM3_IRQHandler()
{
	HAL_ICM_IRQHandler(&ICM_Init[ICM3]);
}
