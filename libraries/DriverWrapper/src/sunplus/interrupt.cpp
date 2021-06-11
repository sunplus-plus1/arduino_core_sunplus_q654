#include "interrupt.h"
//#include "common.h"

typedef struct{
	IRQn_Type irqnb;
//	callback_function_t callback;
	void (*callback)(void);
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

/* According to index of Exti0 ~ Exti7, get the corresponding pin value */
static uint32_t exti_index(uint32_t index)
{
	uint32_t id = 0;
	switch(index)
	{
		case 0:id = GPIO_EXTI(0);break;
		case 1:id = GPIO_EXTI(1);break;
		case 2:id = GPIO_EXTI(2);break;
		case 3:id = GPIO_EXTI(3);break;
		case 4:id = GPIO_EXTI(4);break;
		case 5:id = GPIO_EXTI(5);break;
		case 6:id = GPIO_EXTI(6);break;
		case 7:id = GPIO_EXTI(7);break;	
	}
	return id;
}

void sunplus_interrupt_enable(uint32_t id, void (*callback)(void), uint32_t mode)
{
	/* Seting Pin for extern interrupt to use. Param id is exti number */
	EXTI_SetPinMux(exti_index(id), id);
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