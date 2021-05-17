#include "sp7021_hal_exti.h"

void HAL_EXTI_Init(EXTI_InitTypeDef *pEXTI_Init, IRQHandler_t pEXTI_callback)
{
	pEXTI_Init->irqn = pEXTI_Init->EXTIn + 120;
	setGPIO_exti(pEXTI_Init);				//set PIN_MUX GPIO_EXIT
	IRQ_SetHandler(pEXTI_Init->irqn,pEXTI_callback);		//sethandler
	IRQ_SetMode(pEXTI_Init->irqn, pEXTI_Init->trig);		
	IRQ_Enable(pEXTI_Init->irqn);
}

void setGPIO_exti(EXTI_InitTypeDef *pEXTI_Init)
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
