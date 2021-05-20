#include "sp7021_hal_exti.h"

void HAL_EXTI_Init(EXTI_InitTypeDef *pEXTI_Init, IRQHandler_t pEXTI_callback)
{
	pEXTI_Init->irqn = pEXTI_Init->EXTIn + 120;
	EXTI_set_gpio(pEXTI_Init);							//set GPIO pin
	IRQ_SetHandler(pEXTI_Init->irqn, pEXTI_callback);	//set handler
	IRQ_SetPriority(pEXTI_Init->irqn, pEXTI_Init->priority);
	IRQ_SetMode(pEXTI_Init->irqn, pEXTI_Init->trig);	//set trig type
	IRQ_Enable(pEXTI_Init->irqn);						//enable 
}

void HAL_EXTI_Data(EXTI_InitTypeDef *pEXTI_Init)
{
	pEXTI_Init->trig = IRQ_MODE_TRIG_LEVEL_HIGH;
	pEXTI_Init->priority = 1;
}

void EXTI_set_gpio(EXTI_InitTypeDef *pEXTI_Init)
{   
    uint32_t reg_val = pEXTI_Init->pin - 7;         //G_MX = val(x) + 7
	if((pEXTI_Init->EXTIn + 1) % 2)
    {   
        REG_EXTI(pEXTI_Init->EXTIn) = RF_MASK_V(0x7f, reg_val);
    }
    else
    {   
        REG_EXTI(pEXTI_Init->EXTIn) = RF_MASK_V((0x7f << 8), (reg_val << 8));
    }
}
