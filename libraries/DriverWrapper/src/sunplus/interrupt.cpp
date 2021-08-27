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

//static EXTI_HandleTypeDef exti_handles[EXTI_NUM];

typedef struct {
	EXTI_HandleTypeDef exti_handles;
	std::function<void(void)> callback;
} gpio_irq_conf_str;

static gpio_irq_conf_str gpio_irq_conf[EXTI_NUM];

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

void sunplus_interrupt_enable(uint16_t pin, callback_function_t callback, uint32_t mode)
//void sunplus_interrupt_enable(uint16_t pin, void (*callback)(void), uint32_t mode)
{

	uint8_t index = 0;
	uint16_t exti_pin = 0;

	index = get_pin_index(pin);

	gpio_irq_conf[index].exti_handles.index = index;//0
	gpio_irq_conf[index].exti_handles.trigger = mode;//4
	gpio_irq_conf[index].exti_handles.priority = IRQ_TYPE_IRQ;//0

	exti_pin = GPIO_TO_PINMUX(pin);
	if(!IS_VALID_PINMUX(exti_pin))
	{
		printf("ERROR: [EXTI] pin error!\n");
		//core_debug("ERROR: [EXTI] pin error!\n");
		return;
	}
	HAL_PINMUX_Cfg(PINMUX_GPIO_INT0, exti_pin);

	gpio_irq_conf[index].callback = callback;

	if(index == EXTI0_INDEX)
	{
		gpio_irq_conf[EXTI0_INDEX].exti_handles.irqn = EXTI0_IRQn;
		/* Set mode level-high, level-low, edge-rising, edge-falling*/
		HAL_EXTI_SetMode(&(gpio_irq_conf[index].exti_handles));
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
		gpio_irq_conf[EXTI1_INDEX].exti_handles.irqn = EXTI1_IRQn;
		HAL_EXTI_SetMode(&(gpio_irq_conf[index].exti_handles));
		IRQ_SetHandler(EXTI1_IRQn, EXTI1_IRQHandler);
		IRQ_SetPriority(EXTI1_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI1_IRQn);
	}
	else if(index == EXTI2_INDEX)
	{
		gpio_irq_conf[EXTI2_INDEX].exti_handles.irqn = EXTI2_IRQn;
		HAL_EXTI_SetMode(&(gpio_irq_conf[index].exti_handles));
		IRQ_SetHandler(EXTI2_IRQn, EXTI2_IRQHandler);
		IRQ_SetPriority(EXTI2_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI2_IRQn);
	}
	else if(index == EXTI3_INDEX)
	{
		gpio_irq_conf[EXTI3_INDEX].exti_handles.irqn = EXTI3_IRQn;
		HAL_EXTI_SetMode(&(gpio_irq_conf[index].exti_handles));
		IRQ_SetHandler(EXTI3_IRQn, EXTI3_IRQHandler);
		IRQ_SetPriority(EXTI3_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI3_IRQn);
	}
	else if(index == EXTI4_INDEX)
	{
		gpio_irq_conf[EXTI4_INDEX].exti_handles.irqn = EXTI4_IRQn;
		HAL_EXTI_SetMode(&(gpio_irq_conf[index].exti_handles));
		IRQ_SetHandler(EXTI4_IRQn, EXTI4_IRQHandler);
		IRQ_SetPriority(EXTI4_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI4_IRQn);
	}
	else if(index == EXTI5_INDEX)
	{
		gpio_irq_conf[EXTI5_INDEX].exti_handles.irqn = EXTI5_IRQn;
		HAL_EXTI_SetMode(&(gpio_irq_conf[index].exti_handles));
		IRQ_SetHandler(EXTI5_IRQn, EXTI5_IRQHandler);
		IRQ_SetPriority(EXTI5_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI5_IRQn);
	}
	else if(index == EXTI6_INDEX)
	{
		gpio_irq_conf[EXTI6_INDEX].exti_handles.irqn = EXTI6_IRQn;
		HAL_EXTI_SetMode(&(gpio_irq_conf[index].exti_handles));
		IRQ_SetHandler(EXTI6_IRQn, EXTI6_IRQHandler);
		IRQ_SetPriority(EXTI6_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI6_IRQn);
	}
	else if(index == EXTI7_INDEX)
	{
		gpio_irq_conf[EXTI7_INDEX].exti_handles.irqn = EXTI7_IRQn;
		HAL_EXTI_SetMode(&(gpio_irq_conf[index].exti_handles));
		IRQ_SetHandler(EXTI7_IRQn, EXTI7_IRQHandler);
		IRQ_SetPriority(EXTI7_IRQn, IRQ_TYPE_IRQ);
		IRQ_Enable(EXTI7_IRQn);
	}
}

void sunplus_interrupt_enable(uint16_t pin, void (*callback)(void), uint32_t mode)
{
  std::function<void(void)> _c = callback;
  sunplus_interrupt_enable(pin, _c, mode);

}

void sunplus_interrupt_disable(uint16_t pin)
{
	uint8_t index = get_pin_index(pin);
	gpio_irq_conf[index].callback = NULL;

	for(int i = 0; i < EXTI_NUM; i++)
	{
		if (gpio_irq_conf[index].exti_handles.irqn == gpio_irq_conf[i].exti_handles.irqn
			&& gpio_irq_conf[index].callback != NULL)
			{
				return;
			}
	}
	IRQ_Disable(gpio_irq_conf[index].exti_handles.irqn);
}

void EXTI0_IRQHandler(void)
{
	uint8_t index = EXTI0_INDEX;
	uint32_t irqn = EXTI0_IRQn;
	uint32_t it_mode = gpio_irq_conf[index].exti_handles.trigger;

	if(HAL_EXTI_EdgePatch(&(gpio_irq_conf[index].exti_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void EXTI1_IRQHandler(void)
{
	uint8_t index = EXTI1_INDEX;
	uint32_t irqn = EXTI1_IRQn;
	uint32_t it_mode = gpio_irq_conf[index].exti_handles.trigger;

	if(HAL_EXTI_EdgePatch(&(gpio_irq_conf[index].exti_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void EXTI2_IRQHandler(void)
{
	uint8_t index = EXTI2_INDEX;
	uint32_t irqn = EXTI2_IRQn;
	uint32_t it_mode = gpio_irq_conf[index].exti_handles.trigger;

	if(HAL_EXTI_EdgePatch(&(gpio_irq_conf[index].exti_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void EXTI3_IRQHandler(void)
{
	uint8_t index = EXTI3_INDEX;
	uint32_t irqn = EXTI3_IRQn;
	uint32_t it_mode = gpio_irq_conf[index].exti_handles.trigger;

	if(HAL_EXTI_EdgePatch(&(gpio_irq_conf[index].exti_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void EXTI4_IRQHandler(void)
{
	uint8_t index = EXTI4_INDEX;
	uint32_t irqn = EXTI4_IRQn;
	uint32_t it_mode = gpio_irq_conf[index].exti_handles.trigger;

	if(HAL_EXTI_EdgePatch(&(gpio_irq_conf[index].exti_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void EXTI5_IRQHandler(void)
{
	uint8_t index = EXTI5_INDEX;
	uint32_t irqn = EXTI5_IRQn;
	uint32_t it_mode = gpio_irq_conf[index].exti_handles.trigger;

	if(HAL_EXTI_EdgePatch(&(gpio_irq_conf[index].exti_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void EXTI6_IRQHandler(void)
{
	uint8_t index = EXTI6_INDEX;
	uint32_t irqn = EXTI6_IRQn;
	uint32_t it_mode = gpio_irq_conf[index].exti_handles.trigger;

	if(HAL_EXTI_EdgePatch(&(gpio_irq_conf[index].exti_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void EXTI7_IRQHandler(void)
{
	uint8_t index = EXTI7_INDEX;
	uint32_t irqn = EXTI7_IRQn;
	uint32_t it_mode = gpio_irq_conf[index].exti_handles.trigger;

	if(HAL_EXTI_EdgePatch(&(gpio_irq_conf[index].exti_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}
