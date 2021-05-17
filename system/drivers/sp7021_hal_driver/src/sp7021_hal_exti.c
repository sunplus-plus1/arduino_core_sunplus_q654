#include "sp7021_hal_exti.h"
#include "common_all.h"

void HAL_EXTI_Init(EXTI_InitTypeDef *pEXTI_Init, IRQHandler_t pEXTI_callback)
{
	pEXTI_Init->irqn = pEXTI_Init->EXTIn + 120;
	setGPIO_exti(pEXTI_Init);				//set PIN_MUX GPIO_EXIT
	IRQ_SetHandler(pEXTI_Init->irqn,pEXTI_callback);		//sethandler
	printf("exit 1\n");
	IRQ_SetMode(pEXTI_Init->irqn, pEXTI_Init->trig);		
	printf("exit 2\n");
	IRQ_Enable(pEXTI_Init->irqn);
	printf("exit 3\n");
}

void setGPIO_exti(EXTI_InitTypeDef *pEXTI_Init)
{   
    uint32_t reg_val = pEXTI_Init->pin - 7;         //G_MX = val(x) + 7
    printf("regval=%d irqn=%d\n", reg_val, pEXTI_Init->irqn);
	if((pEXTI_Init->EXTIn + 1) % 2)
    {   
        REG_EXTI(pEXTI_Init->EXTIn) = RF_MASK_V(0x7f, reg_val);
    }
    else
    {   
        REG_EXTI(pEXTI_Init->EXTIn) = RF_MASK_V((0x7f << 8), (reg_val << 8));
    }
}
