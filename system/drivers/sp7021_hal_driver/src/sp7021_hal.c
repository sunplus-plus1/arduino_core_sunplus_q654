
#include "irq_ctrl.h"
#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "sp70xx.h"
#include "cmsis_compiler.h"
#include "sp7021_hal_stc.h";



#define DEFAULT_SYS_STC_CLK	1000000			//1MHz
__IO uint32_t uwTick = 0;
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;//1MHz



STC_HandleTypeDef SysStandardTimeClk;



HAL_StatusTypeDef HAL_Init(void)
{
	HAL_InitTick(STC0);
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
		SysStandardTimeClk.Prescaler = (HSI_VALUE/DEFAULT_SYS_STC_CLK);
		HAL_STC_Init(&SysStandardTimeClk);
}

void HAL_IncTick(void)
{


}

/*Delay ticks*/
__weak void HAL_Delay(uint32_t Delay)
{
	uint32_t tickstart = HAL_GetTick();
	uint32_t wait = Delay;

	if (wait < HAL_MAX_DELAY)
	{
		wait += Delay;
	}

	while ((HAL_GetTick() - tickstart) < wait){
	}
}

uint32_t HAL_GetTick(void)
{
	return HAL_STC_GetCounter(&SysStandardTimeClk);
}

HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq)
{

}



void HAL_Module_Clock_enable(MODULE_ID_Type id, uint32_t enable)
{
	if (enable == 1)
		CLK_EN->clock_enable[id/16] = (1<<(id%16)<<16)| (1<<(id%16));
	else
		CLK_EN->clock_enable[id/16] = (1<<(id%16)<<16)| (0<<(id%16));
	
}

void HAL_Module_Clock_gate(MODULE_ID_Type id, uint32_t enable)
{
	if (enable == 1)
		CLK_GATE->clock_gate_enable[id/16] = (1<<(id%16)<<16)| (1<<(id%16));
	else
		CLK_GATE->clock_gate_enable[id/16] = (1<<(id%16)<<16)| (0<<(id%16));
	
}


void HAL_Module_Reset(MODULE_ID_Type id, uint32_t enable)
{
	if (enable == 1)
		MODULE_REST->reset[id/16] = (1<<(id%16)<<16)| (1<<(id%16));
	else
		MODULE_REST->reset[id/16] = (1<<(id%16)<<16)| (0<<(id%16));
	
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


