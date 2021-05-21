#include "interrupt.h"

typedef struct{
	IRQn_Type irqnb;
	std::function<void(void)> callback;
}gpio_irq_conf_str;

#define NB_EXTI		(8)

static gpio_irq_conf_str gpio_irq_conf[NB_EXTI] = {
	{.irqnb = EXTI0_IRQn,		.callback = NULL},
	{.irqnb = EXTI1_IRQn,		.callback = NULL},
	{.irqnb = EXTI2_IRQn,		.callback = NULL},
	{.irqnb = EXTI3_IRQn,		.callback = NULL},
	{.irqnb = EXTI4_IRQn,		.callback = NULL},
	{.irqnb = EXTI5_IRQn,		.callback = NULL},
	{.irqnb = EXTI6_IRQn,		.callback = NULL},
	{.irqnb = EXTI7_IRQn,		.callback = NULL}
};

void sunplus_interrupt_enable(uint32_t id, callback_function_t callback, uint32_t mode)
{
	/* Seting Pin for extern interrupt to use. Param id is exti number */
	EXTI_SetPinMux(GPIO_EXTI(id), id);
	gpio_irq_conf[id].callback = callback;
	
	/* Regsiter Handler, implement in HAL layer
		(system/drivers/sp7021_hal_driver/src/sp7021_hal_irq_ctrl.c) */
	IRQ_SetHandler(gpio_irq_conf[id].irqnb, gpio_irq_conf[id].callback);
	/* Set mode level-high, level-low, edge-rising, edge-falling*/
	IRQ_SetMode(gpio_irq_conf[id].irqnb, mode);
	/* arm926 don't have NVIC, priority only irq and fiq */
	IRQ_SetPriority(gpio_irq_conf[id].irqnb, EXTI_IRQ);

	IRQ_Enable(gpio_irq_conf[id].irqnb);
}

void sunplus_interrupt_enable(uint32_t id, void (*callback)(void), uint32_t mode)
{
	std::function<void(void)> _c = callback;
	sunplus_interrupt_enable(id, priority, _c, mode);
}

void sunplus_interrupt_disable(uint32_t id)
{
	gpio_irq_conf[id].callback = NULL;
	
	for(int i = 0; i < NB_EXTI; i++)
	{
		if (gpio_irq_conf[id].irqnb == gpio_irq_conf[i].irqnb
			&& gpio_irq_conf[id].callback != NULL)
			{
				return;
			}
	}
	IRQ_Disable(gpio_irq_conf[id].irqnb);
}

