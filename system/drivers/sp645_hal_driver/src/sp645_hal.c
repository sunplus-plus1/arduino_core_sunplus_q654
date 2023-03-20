#include "irq_ctrl.h"
#include "sp645_cm4.h"
#include "sp645_hal_def.h"
#include "sp64xx.h"
#include "cmsis_compiler.h"
#include "sp645_hal_stc.h"

/*
 * NOTE to develop:
 * 1.STC Group of Q645 CM4:
 *	 -----------------------------------------------------------------------
 *	|	|STC	 	|STC_AV0	|STC_AV1	|STC_AV2	|
 *	|-------|---------------|---------------|---------------|---------------|
 *	|STC	|STC0		|STC1	  	|STC2	   	|STC3		|
 *	|-------|---------------|---------------|---------------|---------------|
 *	|TIM	|TIM0/1		|TIM2/3		|TIM4/5		|TIM6/7		|
 *	|-------|---------------|---------------|---------------|---------------|
 *	|WDG	|WDG0		|WDG1		|WDG2		|	-	|
 *	 -----------------------------------------------------------------------
 * 2.only use timer2 and timer3 of every STC
 *	e.x. TIM0 is timer2 of STC
 *	TIM1 is timer3 of STC
 *	TIM2 is timer2 of STC_AV0
 *	TIM3 is timer3 of STC_AV0 and so on....
 */

#ifdef __cplusplus
extern "C" {
#endif


#define DEFAULT_SYS_STC_CLK	1000000			//1MHz
__IO uint32_t uwTick = 0;
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;//1MHz


STC_HandleTypeDef SysStandardTimeClk;

#ifdef  USE_FULL_ASSERT
void assert_failed(unsigned char *file, unsigned int line)
{
	printf("Wrong parameters value file %s on line %d\n", file, line);
	while(1);
}
#endif

HAL_StatusTypeDef HAL_InitCommonSTC(STC_TypeDef *STCx, uint32_t u32Freq)
{
	STC_HandleTypeDef stc;
	MODULE_ID_Type id;
	uint32_t u32Sysclk = HAL_PLL_GetSystemFreq();

	if (STCx == STC0) {
		id = STC_0;
	}
	else if (STCx == STC1) {
		id = STC_AV0;
	}
	else if (STCx == STC2) {
		id = STC_AV1;
	}
	else if (STCx == STC3) {
		id = STC_AV2;
	}

	HAL_Module_Clock_enable(id , 1);
	HAL_Module_Clock_gate(id , 1);
	HAL_Module_Reset(id , 0);

	stc.Instance = STCx;
	stc.ClockSource = 0; //sel system clk
	stc.Prescaler = u32Sysclk / u32Freq - 1;

	HAL_STC_Init(&stc);
}

HAL_StatusTypeDef HAL_Init(void)
{
	HAL_InitTick(STC3);
}

HAL_StatusTypeDef HAL_DeInit(void)
{

}
__weak HAL_StatusTypeDef HAL_InitTick (STC_TypeDef *STCx)
{
		SysStandardTimeClk.Instance = STCx;
		SysStandardTimeClk.ClockSource = 0;
		SysStandardTimeClk.ExtDiv = 0;
		/*the 1tick = 1us, 1MHz */
		SysStandardTimeClk.Prescaler = (SystemCoreClock/DEFAULT_SYS_STC_CLK);
		HAL_STC_Init(&SysStandardTimeClk);
}

void HAL_IncTick(void)
{


}

/*Delay ticks*/
__weak void HAL_Delay(uint32_t Delay)
{
	uint32_t tickstart = HAL_GetTick();
	uint32_t wait = 0;

	if (wait < HAL_MAX_DELAY)
	{
		wait += Delay;
	}

	while ((HAL_GetTick() - tickstart) < wait){
	}
}

uint32_t HAL_GetTick(void)
{
	uint32_t ticks = 0;
	ticks = (uint32_t)HAL_STC_GetCounter(&SysStandardTimeClk);
	return ticks;
}

HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq)
{

}

void HAL_PINMUX_Cfg(PINMUX_Type id, uint32_t pin)
{
	if(id & PINMUX_DFMP_2BIT)
	{
		id = id & 0xFFFF;
		MOON1_REG->sft_cfg[id/16] = RF_MASK_V(0x3 << (id%16),pin << (id%16));
	}
	else if(id & PINMUX_DFMP_3BIT)
	{
		id = id & 0xFFFF;
		MOON1_REG->sft_cfg[id/16] = RF_MASK_V(0x7 << (id%16),pin << (id%16));
	}
	else
	{
		MOON1_REG->sft_cfg[id/16] = RF_MASK_V_SET(1 << (id%16));
	}
}

void HAL_Module_Clock_enable(MODULE_ID_Type id, uint32_t enable)
{
	if (enable == 1)
	{
		if(READ_BIT(CLK_EN->clock_enable[id/16], id%16) == 0)
			CLK_EN->clock_enable[id/16] = RF_MASK_V_SET(1 << (id%16));
	}
	else
	{
		if(READ_BIT(CLK_EN->clock_enable[id/16], id%16) == 1)
			CLK_EN->clock_enable[id/16] = RF_MASK_V_CLR(1 << (id%16));
	}
}

void HAL_Module_Clock_gate(MODULE_ID_Type id, uint32_t enable)
{
	if (enable == 1)
	{
		if(READ_BIT(CLK_GATE->clock_gate_enable[id/16], id%16) == 0)
			CLK_GATE->clock_gate_enable[id/16] = RF_MASK_V_SET(1 << (id%16));
	}
	else
	{
		if(READ_BIT(CLK_GATE->clock_gate_enable[id/16], id%16) == 1)
			CLK_GATE->clock_gate_enable[id/16] = RF_MASK_V_CLR(1 << (id%16));
	}
}


void HAL_Module_Reset(MODULE_ID_Type id, uint32_t enable)
{
	if (enable == 1)
	{
		if(READ_BIT(MODULE_REST->reset[id/16], id%16) == 0)
			MODULE_REST->reset[id/16] = RF_MASK_V_SET(1 << (id%16));
	}
	else
	{
		if(READ_BIT(MODULE_REST->reset[id/16], id%16) == 1)
			MODULE_REST->reset[id/16] = RF_MASK_V_CLR(1 << (id%16));
	}
}

void HAL_lreg(int group)
{
	uint32_t i,  *base;

	base = (uint32_t *)(RF_GRP(group, 0));

	printf("lreg base addr 0x%x\n",base);
	for (i = 0; i < 32; i++) {
		printf( "group %d[%d] = %08x (%d)\n", group, i, base[i], base[i]);
	}
}

#ifdef __cplusplus
}
#endif

