#include "interrupt.h"
//#include "common.h"
typedef enum {
  EXTI0_INDEX,
  EXTI1_INDEX,
  EXTI2_INDEX,
  EXTI3_INDEX,
  EXTI4_INDEX,
  EXTI5_INDEX,
  EXTI6_INDEX,
  EXTI7_INDEX,
  EXTI_NUM
} exti_index_e;

static EXTI_HandleTypeDef exti_handles[EXTI_NUM];

#ifdef __cplusplus
extern "C" {
#endif

extern void HAL_PINMUX_Cfg(PINMUX_Type id, uint32_t pin);

#ifdef __cplusplus
}
#endif

static uint32_t get_pin_index(uint32_t pin)
{
	uint8_t index = 0;
	switch(pin)
	{
		case PIN_EXT_INT0:index = 0;break;//TODO
	}
	return index;
}

void sunplus_interrupt_enable(GPIO_TypeDef *port, uint16_t pin, void (*callback)(void), uint32_t mode)
{
	uint8_t index = 0;
	uint16_t exti_pin = 0;

	UNUSED(port);

	index = get_pin_index(pin);

	exti_handles[index].index = index;//0
	exti_handles[index].trigger = mode;//4
	exti_handles[index].priority = IRQ_TYPE_IRQ;//0

	exti_pin = GPIO_TO_PINMUX(pin);
	if(!IS_VALID_PINMUX(exti_pin))
	{
		printf("ERROR: [EXTI] pin error!\n");
		//core_debug("ERROR: [EXTI] pin error!\n");
		return;
	}
	HAL_PINMUX_Cfg(PINMUX_GPIO_INT0, exti_pin);

	exti_handles[index].callback = callback;

	if(index == EXTI0_INDEX)
	{
		exti_handles[EXTI0_INDEX].irqn = EXTI0_IRQn;
		/* Set mode level-high, level-low, edge-rising, edge-falling*/
		HAL_EXTI_SetMode(&exti_handles[EXTI0_INDEX]);
		/* Regsiter Handler, implement in HAL layer
		(system/drivers/sp7021_hal_driver/src/sp7021_hal_irq_ctrl.c) */
		IRQ_SetHandler(EXTI0_IRQn, EXTI0_IRQHandler);
		/* arm926 don't have NVIC, priority only irq and fiq */
		IRQ_SetPriority(EXTI0_IRQn, IRQ_TYPE_IRQ);
		/* enable irq */
		IRQ_Enable(EXTI0_IRQn);
	}
	else if(index == EXTI1_INDEX)
	{
		exti_handles[EXTI1_INDEX].irqn = EXTI1_IRQn;
		HAL_EXTI_SetMode(&exti_handles[index]);
		IRQ_SetHandler(EXTI1_IRQn, EXTI1_IRQHandler);
		IRQ_SetPriority(EXTI1_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI1_IRQn);
	}
	else if(index == EXTI2_INDEX)
	{
		exti_handles[EXTI2_INDEX].irqn = EXTI2_IRQn;
		HAL_EXTI_SetMode(&exti_handles[index]);
		IRQ_SetHandler(EXTI2_IRQn, EXTI2_IRQHandler);
		IRQ_SetPriority(EXTI2_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI2_IRQn);
	}
	else if(index == EXTI3_INDEX)
	{
		exti_handles[EXTI3_INDEX].irqn = EXTI3_IRQn;
		HAL_EXTI_SetMode(&exti_handles[index]);
		IRQ_SetHandler(EXTI3_IRQn, EXTI3_IRQHandler);
		IRQ_SetPriority(EXTI3_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI3_IRQn);
	}
	else if(index == EXTI4_INDEX)
	{
		exti_handles[EXTI4_INDEX].irqn = EXTI4_IRQn;
		HAL_EXTI_SetMode(&exti_handles[index]);
		IRQ_SetHandler(EXTI4_IRQn, EXTI4_IRQHandler);
		IRQ_SetPriority(EXTI4_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI4_IRQn);
	}
	else if(index == EXTI5_INDEX)
	{
		exti_handles[EXTI5_INDEX].irqn = EXTI5_IRQn;
		HAL_EXTI_SetMode(&exti_handles[index]);
		IRQ_SetHandler(EXTI5_IRQn, EXTI5_IRQHandler);
		IRQ_SetPriority(EXTI5_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI5_IRQn);
	}
	else if(index == EXTI6_INDEX)
	{
		exti_handles[EXTI6_INDEX].irqn = EXTI6_IRQn;
		HAL_EXTI_SetMode(&exti_handles[index]);
		IRQ_SetHandler(EXTI6_IRQn, EXTI6_IRQHandler);
		IRQ_SetPriority(EXTI6_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI6_IRQn);
	}
	else if(index == EXTI7_INDEX)
	{
		exti_handles[EXTI7_INDEX].irqn = EXTI7_IRQn;
		HAL_EXTI_SetMode(&exti_handles[index]);
		IRQ_SetHandler(EXTI7_IRQn, EXTI7_IRQHandler);
		IRQ_SetPriority(EXTI7_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI7_IRQn);
	}
}

void sunplus_interrupt_disable(GPIO_TypeDef *port, uint16_t pin)
{
	UNUSED(port);
	uint8_t index = get_pin_index(pin);
	exti_handles[index].callback = NULL;

	for(int i = 0; i < EXTI_NUM; i++)
	{
		if (exti_handles[index].irqn == exti_handles[i].irqn
			&& exti_handles[index].callback != NULL)
			{
				return;
			}
	}
	IRQ_Disable(exti_handles[index].irqn);
}

void EXTI0_IRQHandler(void)
{
	uint8_t index = EXTI0_INDEX;
	uint32_t irqn = EXTI0_IRQn;
	uint32_t it_mode = exti_handles[index].trigger;

	if(HAL_EXTI_EdgePatch(&exti_handles[index]))
	{
		if (exti_handles[index].callback != NULL)
		{
			exti_handles[index].callback();
		}
	}
}

void EXTI1_IRQHandler(void)
{
	uint8_t index = EXTI1_INDEX;
	uint32_t irqn = EXTI1_IRQn;
	uint32_t it_mode = exti_handles[index].trigger;

	if(HAL_EXTI_EdgePatch(&exti_handles[index]))
	{
		if (exti_handles[index].callback != NULL)
		{
			exti_handles[index].callback();
		}
	}
}

void EXTI2_IRQHandler(void)
{
	uint8_t index = EXTI2_INDEX;
	uint32_t irqn = EXTI2_IRQn;
	uint32_t it_mode = exti_handles[index].trigger;

	if(HAL_EXTI_EdgePatch(&exti_handles[index]))
	{
		if (exti_handles[index].callback != NULL)
		{
			exti_handles[index].callback();
		}
	}
}

void EXTI3_IRQHandler(void)
{
	uint8_t index = EXTI3_INDEX;
	uint32_t irqn = EXTI3_IRQn;
	uint32_t it_mode = exti_handles[index].trigger;

	if(HAL_EXTI_EdgePatch(&exti_handles[index]))
	{
		if (exti_handles[index].callback != NULL)
		{
			exti_handles[index].callback();
		}
	}
}

void EXTI4_IRQHandler(void)
{
	uint8_t index = EXTI4_INDEX;
	uint32_t irqn = EXTI4_IRQn;
	uint32_t it_mode = exti_handles[index].trigger;

	if(HAL_EXTI_EdgePatch(&exti_handles[index]))
	{
		if (exti_handles[index].callback != NULL)
		{
			exti_handles[index].callback();
		}
	}
}

void EXTI5_IRQHandler(void)
{
	uint8_t index = EXTI5_INDEX;
	uint32_t irqn = EXTI5_IRQn;
	uint32_t it_mode = exti_handles[index].trigger;

	if(HAL_EXTI_EdgePatch(&exti_handles[index]))
	{
		if (exti_handles[index].callback != NULL)
		{
			exti_handles[index].callback();
		}
	}
}

void EXTI6_IRQHandler(void)
{
	uint8_t index = EXTI6_INDEX;
	uint32_t irqn = EXTI6_IRQn;
	uint32_t it_mode = exti_handles[index].trigger;

	if(HAL_EXTI_EdgePatch(&exti_handles[index]))
	{
		if (exti_handles[index].callback != NULL)
		{
			exti_handles[index].callback();
		}
	}
}

void EXTI7_IRQHandler(void)
{
	uint8_t index = EXTI7_INDEX;
	uint32_t irqn = EXTI7_IRQn;
	uint32_t it_mode = exti_handles[index].trigger;

	if(HAL_EXTI_EdgePatch(&exti_handles[index]))
	{
		if (exti_handles[index].callback != NULL)
		{
			exti_handles[index].callback();
		}
	}
}
