#ifndef __SP7350_HAL_EXTI_H
#define __SP7350_HAL_EXTI_H

#ifdef __cplusplus
	extern "C" {
#endif

/* Include ------------------------------------------------------------------*/
#include "sp7350_hal_irq_ctrl.h"

#define IRQ_MODE_TRIG_EDGE_NONE  	0x00
#define IRQ_MODE_TRIG_EDGE_ACTIVE  	0x10

typedef struct
{
	IRQn_Type irqn;				//intr num: EXTI0_IRQn~EXTI7_IRQn
	uint32_t trigger;			//triggle type (default high-level)
} EXTI_HandleTypeDef;

HAL_StatusTypeDef HAL_EXTI_SetMode(EXTI_HandleTypeDef *hexti);
int HAL_EXTI_EdgePatch(EXTI_HandleTypeDef *hexti);

#ifdef CONFIG_GPIO_AO_INT
typedef struct
{
	IRQn_Type irqn;
	uint32_t trigger;
} GPIO_AO_INT_HandleTypeDef;

HAL_StatusTypeDef HAL_GPIO_AO_INT_SetMode(int index, GPIO_AO_INT_HandleTypeDef *hexti);
int HAL_GPIO_AO_INT_EdgePatch(int index, GPIO_AO_INT_HandleTypeDef *hexti);
void HAL_GPIO_AO_INT_IRQ_Enable(int index);
void HAL_GPIO_AO_INT_IRQ_Disable(int index);
void HAL_GPIO_AO_INT_IRQ_Clear(int index);
void HAL_GPIO_AO_INT_IRQ_DEB_TimeSet(int pre_scale, int deb_time);
void HAL_GPIO_AO_INT_IRQ_DEB_Enable(int index);
#endif

#ifdef __cplusplus
}
#endif
#endif
