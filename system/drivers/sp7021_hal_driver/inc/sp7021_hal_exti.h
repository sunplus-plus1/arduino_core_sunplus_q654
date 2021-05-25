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
	IRQn_Type irqn;				//intr num: EXTI0_IRQn~EXTI7_IRQn
	uint32_t id;				//exti index: 0~7
	uint32_t trig;				//triggle type (default high-level)
	uint32_t priority;			//IRQ/FIQ (default IRQ)
	IRQHandler_t handler;		//intr handle fuction
	uint32_t pin;   			//GPIO selection(pin_mux)
} EXTI_InitTypeDef;

void EXTI_SetPinMux(uint32_t pin, uint32_t id);
void HAL_EXTI_Init(EXTI_InitTypeDef *pEXTI_Init, IRQHandler_t pEXTI_callback);
void HAL_EXTI_Data(EXTI_InitTypeDef *pEXTI_Init);
#ifdef __cplusplus
}
#endif
#endif
