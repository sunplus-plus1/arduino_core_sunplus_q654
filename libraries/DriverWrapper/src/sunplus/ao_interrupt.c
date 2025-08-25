#ifdef CONFIG_GPIO_AO_INT

#include "ao_interrupt.h"

typedef enum {
	GPIO_AO_INT0_INDEX,
	GPIO_AO_INT1_INDEX,
	GPIO_AO_INT2_INDEX,
	GPIO_AO_INT3_INDEX,
	GPIO_AO_INT4_INDEX,
	GPIO_AO_INT5_INDEX,
	GPIO_AO_INT6_INDEX,
	GPIO_AO_INT7_INDEX,
	GPIO_AO_INT8_INDEX,
	GPIO_AO_INT9_INDEX,
	GPIO_AO_INT10_INDEX,
	GPIO_AO_INT11_INDEX,
	GPIO_AO_INT12_INDEX,
	GPIO_AO_INT13_INDEX,
	GPIO_AO_INT14_INDEX,
	GPIO_AO_INT15_INDEX,
	GPIO_AO_INT16_INDEX,
	GPIO_AO_INT17_INDEX,
	GPIO_AO_INT18_INDEX,
	GPIO_AO_INT19_INDEX,
	GPIO_AO_INT20_INDEX,
	GPIO_AO_INT21_INDEX,
	GPIO_AO_INT22_INDEX,
	GPIO_AO_INT23_INDEX,
	GPIO_AO_INT24_INDEX,
	GPIO_AO_INT25_INDEX,
	GPIO_AO_INT26_INDEX,
	GPIO_AO_INT27_INDEX,
	GPIO_AO_INT28_INDEX,
	GPIO_AO_INT29_INDEX,
	GPIO_AO_INT30_INDEX,
	GPIO_AO_INT31_INDEX,
	GPIO_AO_INT_NUM
} gpio_ao_int_index_e;

struct irq_conf{
	IRQn_Type irqn;
	PINMUX_Type pinctrl;
	uint32_t priority;	/* arm926 don't have NVIC, priority only irq and fiq;
				 * cortex-m4 provide priority level 0~255
				 */
	void (*handler)(void);
} gpio_ao_int_config[32] = {
	{EXTI0_IRQn, PINMUX_GPIO_AO_INT0, IRQ_TYPE_IRQ, GPIO_AO_INT0_IRQHandler},
	{EXTI1_IRQn, PINMUX_GPIO_AO_INT0, IRQ_TYPE_IRQ, GPIO_AO_INT1_IRQHandler},
	{EXTI2_IRQn, PINMUX_GPIO_AO_INT0, IRQ_TYPE_IRQ, GPIO_AO_INT2_IRQHandler},
	{EXTI3_IRQn, PINMUX_GPIO_AO_INT0, IRQ_TYPE_IRQ, GPIO_AO_INT3_IRQHandler},
	{EXTI4_IRQn, PINMUX_GPIO_AO_INT0, IRQ_TYPE_IRQ, GPIO_AO_INT4_IRQHandler},
	{EXTI5_IRQn, PINMUX_GPIO_AO_INT0, IRQ_TYPE_IRQ, GPIO_AO_INT5_IRQHandler},
	{EXTI6_IRQn, PINMUX_GPIO_AO_INT0, IRQ_TYPE_IRQ, GPIO_AO_INT6_IRQHandler},
	{EXTI7_IRQn, PINMUX_GPIO_AO_INT0, IRQ_TYPE_IRQ, GPIO_AO_INT7_IRQHandler},
	{EXTI8_IRQn, PINMUX_GPIO_AO_INT1, IRQ_TYPE_IRQ, GPIO_AO_INT8_IRQHandler},
	{EXTI9_IRQn, PINMUX_GPIO_AO_INT1, IRQ_TYPE_IRQ, GPIO_AO_INT9_IRQHandler},
	{EXTI10_IRQn, PINMUX_GPIO_AO_INT1, IRQ_TYPE_IRQ, GPIO_AO_INT10_IRQHandler},
	{EXTI11_IRQn, PINMUX_GPIO_AO_INT1, IRQ_TYPE_IRQ, GPIO_AO_INT11_IRQHandler},
	{EXTI12_IRQn, PINMUX_GPIO_AO_INT1, IRQ_TYPE_IRQ, GPIO_AO_INT12_IRQHandler},
	{EXTI13_IRQn, PINMUX_GPIO_AO_INT1, IRQ_TYPE_IRQ, GPIO_AO_INT13_IRQHandler},
	{EXTI14_IRQn, PINMUX_GPIO_AO_INT1, IRQ_TYPE_IRQ, GPIO_AO_INT14_IRQHandler},
	{EXTI15_IRQn, PINMUX_GPIO_AO_INT1, IRQ_TYPE_IRQ, GPIO_AO_INT15_IRQHandler},
	{EXTI16_IRQn, PINMUX_GPIO_AO_INT2, IRQ_TYPE_IRQ, GPIO_AO_INT16_IRQHandler},
	{EXTI17_IRQn, PINMUX_GPIO_AO_INT2, IRQ_TYPE_IRQ, GPIO_AO_INT17_IRQHandler},
	{EXTI18_IRQn, PINMUX_GPIO_AO_INT2, IRQ_TYPE_IRQ, GPIO_AO_INT18_IRQHandler},
	{EXTI19_IRQn, PINMUX_GPIO_AO_INT2, IRQ_TYPE_IRQ, GPIO_AO_INT19_IRQHandler},
	{EXTI20_IRQn, PINMUX_GPIO_AO_INT2, IRQ_TYPE_IRQ, GPIO_AO_INT20_IRQHandler},
	{EXTI21_IRQn, PINMUX_GPIO_AO_INT2, IRQ_TYPE_IRQ, GPIO_AO_INT21_IRQHandler},
	{EXTI22_IRQn, PINMUX_GPIO_AO_INT2, IRQ_TYPE_IRQ, GPIO_AO_INT22_IRQHandler},
	{EXTI23_IRQn, PINMUX_GPIO_AO_INT2, IRQ_TYPE_IRQ, GPIO_AO_INT23_IRQHandler},
	{EXTI24_IRQn, PINMUX_GPIO_AO_INT3, IRQ_TYPE_IRQ, GPIO_AO_INT24_IRQHandler},
	{EXTI25_IRQn, PINMUX_GPIO_AO_INT3, IRQ_TYPE_IRQ, GPIO_AO_INT25_IRQHandler},
	{EXTI26_IRQn, PINMUX_GPIO_AO_INT3, IRQ_TYPE_IRQ, GPIO_AO_INT26_IRQHandler},
	{EXTI27_IRQn, PINMUX_GPIO_AO_INT3, IRQ_TYPE_IRQ, GPIO_AO_INT27_IRQHandler},
	{EXTI28_IRQn, PINMUX_GPIO_AO_INT3, IRQ_TYPE_IRQ, GPIO_AO_INT28_IRQHandler},
	{EXTI29_IRQn, PINMUX_GPIO_AO_INT3, IRQ_TYPE_IRQ, GPIO_AO_INT29_IRQHandler},
	{EXTI30_IRQn, PINMUX_GPIO_AO_INT3, IRQ_TYPE_IRQ, GPIO_AO_INT30_IRQHandler},
	{EXTI31_IRQn, PINMUX_GPIO_AO_INT3, IRQ_TYPE_IRQ, GPIO_AO_INT31_IRQHandler},
};
int32_t pin_to_index[2][32] = {
	{52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83},
	{68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 91, 92, 93, 94, 95, 96, 97, 98}
};



typedef struct {
	GPIO_AO_INT_HandleTypeDef gpio_ao_int_handles;
	void (*callback)(void);
	uint8_t active;
	uint16_t pin;
} gpio_irq_conf_str;

static gpio_irq_conf_str gpio_irq_conf[GPIO_AO_INT_NUM];

int32_t get_idle_gpio_ao_int(uint16_t pin, uint32_t* pin_group)
{
	uint8_t i, j;
	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 32; j++){
			if (pin_to_index[i][j] == pin) {
				*pin_group = i + 1;
				return j;
			}
		}
	}
	return -1;
}

uint8_t get_current_gpio_ao_int(uint16_t pin)
{
	uint8_t index, i;

	for(i = 0; i < 32; i++)
	{
		if ((gpio_irq_conf[i].active == 1)&&(gpio_irq_conf[i].pin == pin))
		{
			index = i;
			break;
		}
	}

	return index;
}

void ao_interrupt_enable(uint16_t pin, void (*callback)(void), uint32_t mode)
{
	uint32_t pin_group;
	int32_t index = get_idle_gpio_ao_int(pin, &pin_group);

	if (index == -1)
		return;

	gpio_irq_conf[index].active = 1;
	gpio_irq_conf[index].pin = pin;
	gpio_irq_conf[index].callback = callback;
	gpio_irq_conf[index].gpio_ao_int_handles.trigger = mode;

	HAL_PINMUX_Cfg(gpio_ao_int_config[index].pinctrl, pin_group);
	gpio_irq_conf[index].gpio_ao_int_handles.irqn = gpio_ao_int_config[index].irqn;

	/* Set mode level-high, level-low, edge-rising, edge-falling*/
	HAL_GPIO_AO_INT_SetMode(index, &(gpio_irq_conf[index].gpio_ao_int_handles));

	/* Regsiter Handler, implement in HAL layer */
	IRQ_SetHandler(gpio_ao_int_config[index].irqn, gpio_ao_int_config[index].handler);

	IRQ_SetPriority(gpio_ao_int_config[index].irqn, gpio_ao_int_config[index].priority);

	/* enable debounce */
	// int pre_scale = 0x20;
	// int deb_time = 0x20;
	// HAL_GPIO_AO_INT_IRQ_DEB_TimeSet(pre_scale, deb_time);
	// HAL_GPIO_AO_INT_IRQ_DEB_Enable(index);

	/* enable irq */
	IRQ_Enable(gpio_ao_int_config[index].irqn);
	HAL_GPIO_AO_INT_IRQ_Enable(index);
}

void ao_interrupt_disable(uint16_t pin)
{
	uint32_t pin_group;
	int32_t index = get_idle_gpio_ao_int(pin, &pin_group);
	if (index == -1) {
		return;
	}
	gpio_irq_conf[index].active = 0;
	gpio_irq_conf[index].callback = NULL;

	IRQ_Disable(gpio_irq_conf[index].gpio_ao_int_handles.irqn);
	HAL_GPIO_AO_INT_IRQ_Disable(index);
}

void GPIO_AO_INT0_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT0_INDEX;;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT1_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT1_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT2_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT2_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT3_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT3_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT4_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT4_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT5_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT5_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT6_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT6_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT7_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT7_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT8_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT8_INDEX;;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT9_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT9_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT10_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT10_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);


	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT11_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT11_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);


	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT12_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT12_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT13_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT13_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT14_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT14_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT15_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT15_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT16_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT16_INDEX;;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT17_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT17_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT18_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT18_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT19_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT19_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT20_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT20_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT21_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT21_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT22_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT22_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT23_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT23_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT24_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT24_INDEX;;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT25_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT25_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT26_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT26_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT27_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT27_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT28_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT28_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT29_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT29_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT30_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT30_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}

void GPIO_AO_INT31_IRQHandler(void)
{
	uint8_t index = GPIO_AO_INT31_INDEX;
	HAL_GPIO_AO_INT_IRQ_Clear(index);

	if(HAL_GPIO_AO_INT_EdgePatch(index, &(gpio_irq_conf[index].gpio_ao_int_handles)))
	{
		if (gpio_irq_conf[index].callback != NULL)
		{
			gpio_irq_conf[index].callback();
		}
	}
}
#endif
