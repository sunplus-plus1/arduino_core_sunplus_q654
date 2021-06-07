#include "sp7021_hal_exti.h"

/* HAL layer port for external interrupts, you can call  this function to 
   implement the use of EXTI. e.g in main.c  */
void HAL_EXTI_Init(EXTI_InitTypeDef *pEXTI_Init, IRQHandler_t pEXTI_callback)
{
	pEXTI_Init->irqn = pEXTI_Init->id + 120;
	EXTI_SetPinMux(pEXTI_Init->pin, pEXTI_Init->id);
	IRQ_SetHandler(pEXTI_Init->irqn, pEXTI_callback);
	IRQ_SetPriority(pEXTI_Init->irqn, pEXTI_Init->priority);
	IRQ_SetMode(pEXTI_Init->irqn, pEXTI_Init->trig);
	IRQ_Enable(pEXTI_Init->irqn);
}

void HAL_EXTI_Data(EXTI_InitTypeDef *pEXTI_Init)
{
	pEXTI_Init->trig = IRQ_MODE_TRIG_LEVEL_HIGH;
	pEXTI_Init->priority = 1;
}

/* 
  SP7021 have the function of Pin Multiplex. Here set pin for an external interrupt to use
  by configuring the Pinmux control register.
*/
void EXTI_SetPinMux(uint32_t pin, uint32_t id)
{   
    uint32_t reg_val = pin - 7;         //G_MX = reg_val(x) + 7
	if((id + 1) % 2)
    {   
        REG_EXTI(id) = RF_MASK_V(0x7f, reg_val);
    }
    else
    {   
        REG_EXTI(id) = RF_MASK_V((0x7f << 8), (reg_val << 8));
    }
}
