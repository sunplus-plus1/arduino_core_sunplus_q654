
#include "irq_ctrl.h"
#include "sp7021_arm926.h"
#include "sp70xx.h"

#ifndef IRQ_GIC_LINE_COUNT
#define IRQ_GIC_LINE_COUNT      (200U)
#endif

static IRQHandler_t IRQTable[IRQ_GIC_LINE_COUNT] = { 0U };
static uint32_t	 IRQ_ID0 = 0;

__STATIC_INLINE void GIC_EnableIRQ(IRQn_Type IRQn){
	SP_IRQ_CTRL->mask[IRQn/32U] = 1 << (IRQn%32U);
}

__STATIC_INLINE void GIC_DisableIRQ(IRQn_Type IRQn){
	SP_IRQ_CTRL->mask[IRQn/32U] &= ~(1 << (IRQn%32U));
}

__STATIC_INLINE uint32_t GIC_GetEnableIRQ(IRQn_Type IRQn)
{
	return (SP_IRQ_CTRL->mask[IRQn / 32U] >> (IRQn % 32U)) & 1UL;
}

__STATIC_INLINE void GIC_SetIRQType(IRQn_Type IRQn, uint32_t type)
{
	if (type == IRQ_MODE_TRIG_EDGE_RISING)
	  SP_IRQ_CTRL->type[IRQn / 32U] = 1 << IRQn%32U;
	else{
		SP_IRQ_CTRL->type[IRQn / 32U] &= ~(1 << IRQn%32U);
		if (type == IRQ_MODE_TRIG_LEVEL_LOW)
			SP_IRQ_CTRL->polarity[IRQn / 32U] = 1 << IRQn%32U;
		else
			SP_IRQ_CTRL->polarity[IRQn / 32U] &= ~(1 << IRQn%32U);
	}
}

__STATIC_INLINE uint32_t GIC_GetIRQType(IRQn_Type IRQn)
{
	return (SP_IRQ_CTRL->type[IRQn / 32U] >> (IRQn % 32U)) & 1UL;
}

__STATIC_INLINE uint32_t GIC_GetIRQPolarity(IRQn_Type IRQn){

	return (SP_IRQ_CTRL->polarity[IRQn / 32U] >> (IRQn % 32U)) & 1UL;
}

__STATIC_INLINE uint32_t GIC_SetIRQPirority(IRQn_Type IRQn IRQn, uint32t priority)
{
	SP_IRQ_CTRL->priority[IRQn/32U] = 1 << (IRQn%32U);

}

__STATIC_INLINE uint32_t GIC_GetIRQPirority(IRQn_Type IRQn IRQn)
{
	return (SP_IRQ_CTRL->priority[IRQn / 32U] >> (IRQn % 32U)) & 1UL;

}

__STATIC_INLINE void GIC_ClearIRQ(IRQn_Type IRQn)
{
	SP_IRQ_CTRL->clear[IRQn/32U] &= ~(1 << (IRQn%32U));
}

__STATIC_INLINE uint32_t GIC_GetActiveIRQ(void)
{
	uint32_t i = 0;
	for (i=0;i < IRQ_GIC_LINE_COUNT; i++)
	{
		if ((SP_IRQ_CTRL->masked_irq[i / 32U] >> (i % 32U)) & 1UL)
			break;
	}

	return i;
}

__STATIC_INLINE uint32_t GIC_GetActiveFIQ(void)
{
	uint32_t i = 0;
	for (i=0;i < IRQ_GIC_LINE_COUNT; i++)
	{
		if ((SP_IRQ_CTRL->masked_fiq[i / 32U] >> (i % 32U)) & 1UL)
			break;
	}

	return i;
}

int32_t IRQ_Initialize (void) {
	uint32_t i = 0;

	for (i=0; i<IRQ_GIC_LINE_COUNT; i++ ){
		IRQTable[i] = (IRQHandler_t)NULL;
	}
	__enable_irq();
}

 int32_t IRQ_SetHandler (IRQn_ID_t irqn, IRQHandler_t handler)	{
	int32_t status = 0;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT)) {
    	IRQTable[irqn] = handler;
    	status =  0;
  	} else {
    	status = -1;
  	}

  return (status);
 }

 IRQHandler_t IRQ_GetHandler (IRQn_ID_t irqn){
	IRQHandler_t h;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT)) {
    	h = IRQTable[irqn];
  	} else {
    	h = (IRQHandler_t)0;
  	}

  	return (h);
	
 }
 
 int32_t IRQ_Enable (IRQn_ID_t irqn){
	 int32_t status;
	 if ((irqn >=0) && (irqn<(IRQn_ID_t)IRQ_GIC_LINE_COUNT)){
		GIC_EnableIRQ ((IRQn_Type)irqn);
		status = 0;
	 }else{
		status = -1;
	 }
	 return (status);
 }
 
 
 int32_t IRQ_Disable (IRQn_ID_t irqn){
	 int32_t status;
	 if ((irqn >=0) && (irqn<(IRQn_ID_t)IRQ_GIC_LINE_COUNT)){
		GIC_DisableIRQ ((IRQn_Type)irqn);
		status = 0;
	 }else{
		status = -1;
	 }
	 return (status);
 }
 
uint32_t IRQ_GetEnableState (IRQn_ID_t irqn){
	uint32_t enable;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT)){
		enable = GIC_GetEnableIRQ((IRQn_Type)irqn);
	}else{
		enable = 0;
	}

	return (enable);
}
 
 int32_t IRQ_SetMode (IRQn_ID_t irqn, uint32_t mode){
	int32_t status = 0;

	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT)){
		val = mode & 0x07;
		GIC_SetIRQType(mode);	
		status = 0;
	}else
		status = -1;
	return (status);
 }
 
uint32_t IRQ_GetMode (IRQn_ID_t irqn)
{
 	uint32_t val;
	uint32_t mode = IRQ_MODE_TRIG_LEVEL;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT)){
		val = GIC_GetIRQType(irqn);
		if (val == 1U){
			mode = IRQ_MODE_TRIG_EDGE_RISING;
			return mode;
		}

		val = GIC_GetIRQPolarity(irqn);
		if (val == 0){
			mode = IRQ_MODE_TRIG_LEVEL_HIGH;
		}else{
			mode = IRQ_MODE_TRIG_LEVEL_LOW;
		}

	}
	return (mode);
}
 


 
int32_t IRQ_SetPriority (IRQn_ID_t irqn, uint32_t priority)
{
	int32_t status;
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT)){
		GIC_SetIRQPirority(irqn, priority);
		status = 0;	
	}else
		status = 1;

	return (status);	

}
 

uint32_t IRQ_GetPriority (IRQn_ID_t irqn)
{
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT)){
		return GIC_GetIRQPirority(irqn);
	}

	return 0;
}
 
void IRQ_Clear(IRQn_ID_t irqn)
{
	if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT))
		GIC_ClearIRQ(irqn);
	
}


IRQn_ID_t IRQ_GetActiveIRQ (void)
{
	return GIC_GetActiveIRQ();
}

IRQn_ID_t IRQ_GetActiveFIQ (void)
{
	return GIC_GetActiveFIQ();
}


void IRQ_HANDLE(void)
{
	IRQHandler_t h = 0;
	IRQn_ID_t irqn = 0;
	ISR_SAVE_CONTEXT();
	irqn = GIC_GetActiveIRQ();
	h = IRQ_GetHandler(irqn);
	IRQ_Clear(irqn);
	if (h)
		(h)();
	ISR_RESTORE_CONTEXT
}

void FIQ_HANDLE(void)
{
	IRQHandler_t h = 0;
	IRQn_ID_t irqn = 0;
	ISR_SAVE_CONTEXT();
	irqn = GIC_GetActiveFIQ();
	h = IRQ_GetHandler(irqn);
	IRQ_Clear(irqn);
	if (h)
		(h)();
	ISR_RESTORE_CONTEXT
}

void RESET_HANDLE(void)
{
	return;
}

void UNDEF_INS_HANDLE( void )
{
	return;
}

void SWI_HANDLE( void )
{
    return;
}

void PREFE_ABORT_HANDLE( void )
{
	return;
}

void DATA_ABORT_HANDLE( void )
{
    return;
}

void NO_USED_HANDLE( void )
{
    return;
}


