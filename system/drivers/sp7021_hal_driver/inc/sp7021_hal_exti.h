#ifndef __SP7021_HAL_EXTI_H
#define __SP7021_HAL_ExTI_H

#ifdef __cplusplus
	extern "C" {
#endif

/* Include ------------------------------------------------------------------*/
#include "sp7021_hal_gpio.h"
#include "sp7021_hal_irq_ctrl.h"

#define EXTI(X) (RF_GRP(3, (24+(X/2))))
#define REG_EXTI(X) (*((volatile unsigned int *)EXTI(X)))

#define RF_MASK_V(_mask, _val)          (((_mask) << 16) | (_val))

typedef struct
{
	IRQn_Type irqn;				//EXTI0_IRQn~EXTI7_IRQn
	uint32_t EXTIn;					//0~7
	uint32_t trig;
	IRQHandler_t handler;
	uint32_t pin;   					//GPIO selection
} EXTI_InitTypeDef;

void setGPIO_exti(EXTI_InitTypeDef *pEXTI_Init);
void HAL_EXTI_Init(EXTI_InitTypeDef *pEXTI_Init, IRQHandler_t pEXTI_callback);

#ifdef __cplusplus
}
#endif
#endif
