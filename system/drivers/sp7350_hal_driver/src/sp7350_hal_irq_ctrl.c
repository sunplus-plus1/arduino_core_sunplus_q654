#include "sp7350_hal_irq_ctrl.h"

/*
GROUP_0 	0:4  PREEMPT:0bit Sub:4bit
GROUP_1 	1:3  PREEMPT:1bit Sub:3bit
GROUP_2 	2:2  PREEMPT:2bit Sub:2bit
GROUP_3 	3:1  PREEMPT:3bit Sub:1bit
GROUP_4 	4:0  PREEMPT:4bit Sub:0bit
*/
#define NVIC_PRIORITY_GROUP        (NVIC_PRIORITYGROUP_2)
#define NVIC_PREEMPTPRIORITY       (2)  
#define NVIC_SUBPRIORITY_MAX       (4)

int32_t IRQ_Initialize (void)
{

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP);

	return 0;
}

int32_t IRQ_Unitialize(void)
{

}

int32_t IRQ_SetHandler (IRQn_ID_t irqn, IRQHandler_t handler)
{
	if ((irqn >= 0) && (irqn < MAX_IRQ_n))
	{
		HAL_NVIC_SetVector(irqn, (uint32_t)handler);
		return 0;
	}
	
	return -1;
}

IRQHandler_t IRQ_GetHandler (IRQn_ID_t irqn)
{
	IRQHandler_t h;
	if ((irqn >= 0) && (irqn < MAX_IRQ_n))
	{
		h = (IRQHandler_t)HAL_NVIC_GetVector(irqn);
	}
	else
	{
		h = (IRQHandler_t)0;
	}

	return h;
}
 
int32_t IRQ_Enable (IRQn_ID_t irqn)
{
	if ((irqn >=0) && (irqn < MAX_IRQ_n))
	{
		HAL_NVIC_EnableIRQ((IRQn_Type)irqn);
		return 0;
	}
	return -1;
}

int32_t IRQ_Disable (IRQn_ID_t irqn)
{
	if ((irqn >=0) && (irqn < MAX_IRQ_n))
	{
		HAL_NVIC_DisableIRQ((IRQn_Type)irqn);
		return 0;
	}
	return -1;
}

int32_t IRQ_SetPriority (IRQn_ID_t irqn, uint32_t priority)
{
	int32_t status;
	if ((irqn >= 0) && (irqn < MAX_IRQ_n) && (priority < NVIC_SUBPRIORITY_MAX))
	{
		HAL_NVIC_SetPriority(irqn, NVIC_PREEMPTPRIORITY,priority);
		status = 0;
	}
	
	return -1;
}

uint32_t IRQ_GetPriority (IRQn_ID_t irqn)
{
    uint32_t preemptpriority,subpriority=0;
	
	if ((irqn >= 0) && (irqn < MAX_IRQ_n))
	{
		HAL_NVIC_GetPriority(irqn,NVIC_PRIORITY_GROUP,&preemptpriority,&subpriority);
	}

	return subpriority;
}

int32_t IRQ_Clear(IRQn_ID_t irqn)
{
	return 0;
}

IRQn_ID_t IRQ_GetActiveIRQ (void)
{
	return 0;
}

IRQn_ID_t IRQ_GetActiveFIQ (void)
{
	return 0;
}

int32_t IRQ_SetMode (IRQn_ID_t irqn, uint32_t mode)
{
	return 0;
}

uint32_t IRQ_GetMode (IRQn_ID_t irqn)
{
	return 0;
}

