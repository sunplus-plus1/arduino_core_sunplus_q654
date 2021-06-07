
#include "irq_ctrl.h"
#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "sp70xx.h"
#include "cmsis_compiler.h"

__IO uint32_t uwTick = 0;
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;

HAL_StatusTypeDef HAL_Init(void)
{

}

HAL_StatusTypeDef HAL_DeInit(void)
{


}

HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{

}

void HAL_IncTick(void)
{


}

void HAL_Delay(uint32_t Delay)
{

}

uint32_t HAL_GetTick(void)
{

}

HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq)
{

}

HAL_TickFreqTypeDef HAL_GetTickFreq(void)
{

}
void HAL_SuspendTick(void)
{

}
void HAL_ResumeTick(void)
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

