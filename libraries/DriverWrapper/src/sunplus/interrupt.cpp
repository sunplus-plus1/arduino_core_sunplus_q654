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

struct irq_conf{
	IRQn_Type irqn;
	PINMUX_Type pinctrl;
	uint32_t priority;	/* arm926 don't have NVIC, priority only irq and fiq;
				 * cortex-m4 provide priority level 0~255
				 */
	void (*handler)(void);
} exti_config[8] = {
	{EXTI0_IRQn, PINMUX_GPIO_INT0, IRQ_TYPE_IRQ, EXTI0_IRQHandler},
	{EXTI1_IRQn, PINMUX_GPIO_INT1, IRQ_TYPE_IRQ, EXTI1_IRQHandler},
	{EXTI2_IRQn, PINMUX_GPIO_INT2, IRQ_TYPE_IRQ, EXTI2_IRQHandler},
	{EXTI3_IRQn, PINMUX_GPIO_INT3, IRQ_TYPE_IRQ, EXTI3_IRQHandler},
	{EXTI4_IRQn, PINMUX_GPIO_INT4, IRQ_TYPE_IRQ, EXTI4_IRQHandler},
	{EXTI5_IRQn, PINMUX_GPIO_INT5, IRQ_TYPE_IRQ, EXTI5_IRQHandler},
	{EXTI6_IRQn, PINMUX_GPIO_INT6, IRQ_TYPE_IRQ, EXTI6_IRQHandler},
	{EXTI7_IRQn, PINMUX_GPIO_INT7, IRQ_TYPE_IRQ, EXTI7_IRQHandler},
};

#if defined(SP645) || defined(SP7350)
namespace std {
	void __throw_bad_function_call()
	{
		while(1);
	};
}
#endif
typedef struct {
	EXTI_HandleTypeDef exti_handles;
	std::function<void(void)> callback;
	uint8_t active;
	uint16_t pin;
} gpio_irq_conf_str;

static gpio_irq_conf_str gpio_irq_conf[EXTI_NUM];

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

uint8_t get_idle_exti(void)
{
	uint8_t index, i;

	for(i = 0; i < 8; i++)
	{
		if (gpio_irq_conf[i].active == 0)
		{
			index = i;
			break;
		}
	}

	return index;
}

uint8_t get_current_exti(uint16_t pin)
{
	uint8_t index, i;

	for(i = 0; i < 8; i++)
	{
		if ((gpio_irq_conf[i].active == 1)&&(gpio_irq_conf[i].pin == pin))
		{
			index = i;
			break;
		}
	}

	return index;
}

void sunplus_interrupt_enable(uint16_t pin, callback_function_t callback, uint32_t mode)
//void sunplus_interrupt_enable(uint16_t pin, void (*callback)(void), uint32_t mode)
{
	uint16_t exti_pin = 0;
	uint8_t index = 0;

	exti_pin = GPIO_TO_PINMUX(pin);
	if(!IS_VALID_PINMUX(exti_pin))
	{
		printf("ERROR: [EXTI] pin error!\n");
		//core_debug("ERROR: [EXTI] pin error!\n");
		return;
	}

	index = get_idle_exti();

	gpio_irq_conf[index].active = 1;
	gpio_irq_conf[index].pin = pin;
	gpio_irq_conf[index].callback = callback;
	gpio_irq_conf[index].exti_handles.trigger = mode;

	HAL_PINMUX_Cfg(exti_config[index].pinctrl, exti_pin);
	gpio_irq_conf[index].exti_handles.irqn = exti_config[index].irqn;

	/* Set mode level-high, level-low, edge-rising, edge-falling*/
	HAL_EXTI_SetMode(&(gpio_irq_conf[index].exti_handles));

	/* Regsiter Handler, implement in HAL layer */
	IRQ_SetHandler(exti_config[index].irqn, exti_config[index].handler);

	IRQ_SetPriority(exti_config[index].irqn, exti_config[index].priority);

	/* enable irq */
	IRQ_Enable(exti_config[index].irqn);
}

void sunplus_interrupt_enable(uint16_t pin, void (*callback)(void), uint32_t mode)
{
  std::function<void(void)> _c = callback;
  sunplus_interrupt_enable(pin, _c, mode);
}

void sunplus_interrupt_disable(uint16_t pin)
{
	uint8_t index;

	index = get_current_exti(pin);

	gpio_irq_conf[index].active = 0;
	gpio_irq_conf[index].callback = NULL;

	IRQ_Disable(gpio_irq_conf[index].exti_handles.irqn);
}

void EXTI0_IRQHandler(void)
{
	uint8_t index = EXTI0_INDEX;;
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
	uint32_t it_mode = gpio_irq_conf[index].exti_handles.trigger;

	if(HAL_EXTI_EdgePatch(&(gpio_irq_conf[index].exti_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}
