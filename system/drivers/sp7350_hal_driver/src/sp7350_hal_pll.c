#include "sp7350_hal_pll.h"

/* SP7350 CM4 system clock is from AO domain, not main domain */
uint32_t HAL_PLL_GetSystemFreq(void)
{
	uint32_t clk_sel, clk_div, sys_freq;

	clk_sel = __HAL_PLL_GET_SYS_CLK_SEL();

	if (clk_sel == 0x0)
	{
		sys_freq = 200000000;
	}
	else if (clk_sel == 0x1)
	{
		sys_freq = 50000000;
	}
	else if (clk_sel == 0x3)
	{
		sys_freq = 25000000;
	}
	else if (clk_sel == 0x7)
	{
		sys_freq = 100000000;
	}

	return sys_freq;
}


uint32_t HAL_PLL_GetCortexM4Freq(void)
{
	uint32_t clk_sel, sys_freq;

	clk_sel = __HAL_PLL_GET_SYS_CM4_SEL();

	if (clk_sel == 0x0)
	{
		sys_freq = 400000000;
	}
	else if (clk_sel == 0x1)
	{
		sys_freq = 100000000;
	}
	else if (clk_sel == 0x3)
	{
		sys_freq = 200000000;
	}
	else if (clk_sel == 0x7)
	{
		sys_freq = 25000000;
	}
}

