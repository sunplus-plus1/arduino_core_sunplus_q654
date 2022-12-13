#include "sp645_hal_pll.h"

uint32_t HAL_PLL_GetSystemFreq(void)
{

	uint32_t clk_sel, clk_div, sys_freq;

	clk_sel = __HAL_PLL_GET_SYS_CLK_SEL();
	clk_div = __HAL_PLL_GET_SYS_DIV();

	if (clk_sel == 0x0)
	{
		sys_freq = 600000000 >> clk_div;
	}
	else if (clk_sel == 0x1)
	{
		sys_freq = 750000000 >> clk_div;
	}
	else
	{
		sys_freq = 500000000 >> clk_div;
	}

	return sys_freq;
}

uint32_t HAL_PLL_GetCortexM4Freq(void)
{
	return (HAL_PLL_GetSystemFreq() >> __HAL_PLL_GET_SYS_CM4_DIV());
}

/* Param val: the val 0b0/0b1/0b1x corresponding div 1/2/4 */
void HAL_PLL_SetCortexM4Freq(uint8_t val)
{
	__HAL_PLL_SET_SYS_CM4_DIV(val);
}
