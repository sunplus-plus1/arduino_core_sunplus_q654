#include "sp7350_hal_exti.h"
#include "sp7350_hal_conf.h"

#ifndef CONFIG_GPIO_AO_INT
HAL_StatusTypeDef HAL_EXTI_SetMode(EXTI_HandleTypeDef *hexti)
{
	int idx = 0;
	int mask = 0;
	
	if ((hexti->irqn < EXTI0_IRQn) || (hexti->irqn > EXTI7_IRQn))
	{
		return HAL_ERROR;
	}
	
	idx = hexti->irqn / 32;
	mask = (1 << (hexti->irqn % 32));
	
	if(hexti->trigger & IRQ_MODE_TRIG_EDGE_RISING)
	{
		SP_IRQ_CTRL->type[idx] |= mask;
		SP_IRQ_CTRL->polarity[idx] &= ~mask;
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_EDGE_FALLING)
	{
		SP_IRQ_CTRL->type[idx] |= mask;
		SP_IRQ_CTRL->polarity[idx] |= mask;
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_LEVEL_HIGH)
	{
		SP_IRQ_CTRL->type[idx] &= ~mask;
		SP_IRQ_CTRL->polarity[idx] &= ~mask;
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_LEVEL_LOW)
	{
		SP_IRQ_CTRL->type[idx] &= ~mask;
		SP_IRQ_CTRL->polarity[idx] |= mask;
	}
	return HAL_OK;
}

int HAL_EXTI_EdgePatch(EXTI_HandleTypeDef *hexti)
{
	int idx = 0;
	int mask = 0;
	
	idx = hexti->irqn / 32;
	mask = (1 << (hexti->irqn % 32));
	
	if(hexti->trigger & IRQ_MODE_TRIG_EDGE_ACTIVE)//second times entry irq
	{
		if(hexti->trigger & IRQ_MODE_TRIG_EDGE_RISING)
		{
			SP_IRQ_CTRL->polarity[idx] &= ~mask;//low->high
		}
		else
		{
			SP_IRQ_CTRL->polarity[idx] |= mask;//high->low
		}
		hexti->trigger &= ~IRQ_MODE_TRIG_EDGE_ACTIVE;
		return 0;
	}
	/* first time entry irq */
	else if(hexti->trigger == IRQ_MODE_TRIG_EDGE_RISING)
	{
		SP_IRQ_CTRL->polarity[idx] |= mask;//high->low
		hexti->trigger |= IRQ_MODE_TRIG_EDGE_ACTIVE;
	}
	else if(hexti->trigger == IRQ_MODE_TRIG_EDGE_FALLING)
	{
		SP_IRQ_CTRL->polarity[idx] &= ~mask;//low->high
		hexti->trigger |= IRQ_MODE_TRIG_EDGE_ACTIVE;	
	}
	return 1;
}
#else
HAL_StatusTypeDef HAL_EXTI_IRQ_Clear(EXTI_HandleTypeDef *hexti)
{
	int index = 0;
	int mask = 0;

	switch (hexti->irqn)
	{
	case EXTI0_IRQn...EXTI22_IRQn:
		index = hexti->irqn - EXTI0_IRQn;
		break;
	case EXTI23_IRQn...EXTI31_IRQn:
		index = hexti->irqn - EXTI23_IRQn + 23;
		break;
	default:
		return HAL_ERROR;
	}

	mask = 1 << index;
	GPIO_AO_IRQ_CTRL->int_clr = mask;

	return HAL_OK;
}

HAL_StatusTypeDef HAL_EXTI_SetMode(EXTI_HandleTypeDef *hexti)
{
	int index = 0;
	int mask = 0;

	switch (hexti->irqn)
	{
		case EXTI0_IRQn...EXTI22_IRQn:
			index = hexti->irqn - EXTI0_IRQn;
			break;
		case EXTI23_IRQn...EXTI31_IRQn:
			index = hexti->irqn - EXTI23_IRQn + 23;
			break;
			default:
			return HAL_ERROR;
	}

	mask = 1 << index;
	if(hexti->trigger & IRQ_MODE_TRIG_EDGE_RISING)
	{
		GPIO_AO_IRQ_CTRL->int_mode  &= ~mask;
		GPIO_AO_IRQ_CTRL->int_pol &= ~mask;
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_EDGE_FALLING)
	{
		GPIO_AO_IRQ_CTRL->int_mode  &= ~mask;
		GPIO_AO_IRQ_CTRL->int_pol |= mask;
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_LEVEL_HIGH)
	{
		GPIO_AO_IRQ_CTRL->int_mode  |= mask;
		GPIO_AO_IRQ_CTRL->int_pol &= ~mask;
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_LEVEL_LOW)
	{
		GPIO_AO_IRQ_CTRL->int_mode  |= mask;
		GPIO_AO_IRQ_CTRL->int_pol |= mask;
	}

	return HAL_OK;
}

int HAL_EXTI_EdgePatch(EXTI_HandleTypeDef *hexti)
{
	int index = 0, mask, value;

	switch (hexti->irqn)
	{
	case EXTI0_IRQn...EXTI22_IRQn:
		index = hexti->irqn - EXTI0_IRQn;
		break;
	case EXTI23_IRQn...EXTI31_IRQn:
		index = hexti->irqn - EXTI23_IRQn + 23;
		break;
	default:
		return 0;
	}

	mask = 1 << index;

	if(hexti->trigger & IRQ_MODE_TRIG_EDGE_BOTH)
	{
		value = GPIO_AO_IRQ_CTRL->int_pol & mask;
		if(value)
		{
			GPIO_AO_IRQ_CTRL->int_pol &= ~mask;//low->high
		}
		else
		{
			GPIO_AO_IRQ_CTRL->int_pol |= mask;//high->low
		}
	}

	return 1;
}


HAL_StatusTypeDef HAL_GPIO_AO_INT_SetMode(int index, GPIO_AO_INT_HandleTypeDef *hexti)
{
	if ((hexti->irqn < EXTI0_IRQn) || (hexti->irqn > EXTI22_IRQn && hexti->irqn < EXTI23_IRQn) || (hexti->irqn > EXTI31_IRQn))
	{
		return HAL_ERROR;
	}

	if(hexti->trigger & IRQ_MODE_TRIG_EDGE_RISING)
	{
		GPIO_AO_INT->int_mode  &= ~(1 << index);
		GPIO_AO_INT->int_pol &= ~(1 << index);
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_EDGE_FALLING)
	{
		GPIO_AO_INT->int_mode  &= ~(1 << index);
		GPIO_AO_INT->int_pol |= (1 << index);
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_LEVEL_HIGH)
	{
		GPIO_AO_INT->int_mode  |= (1 << index);
		GPIO_AO_INT->int_pol &= ~(1 << index);
	}
	else if(hexti->trigger & IRQ_MODE_TRIG_LEVEL_LOW)
	{
		GPIO_AO_INT->int_mode  |= (1 << index);
		GPIO_AO_INT->int_pol |= (1 << index);
	}

	return HAL_OK;
}

void HAL_GPIO_AO_INT_IRQ_Enable(int index)
{
	GPIO_AO_INT->int_en |= (1 << index);
}

void HAL_GPIO_AO_INT_IRQ_Disable(int index)
{
	GPIO_AO_INT->int_en &= ~(1 << index);
}

void HAL_GPIO_AO_INT_IRQ_Clear(int index)
{
	GPIO_AO_INT->int_clr = (1 << index);
}

void HAL_GPIO_AO_INT_IRQ_DEB_TimeSet(int pre_scale, int deb_time)
{
	GPIO_AO_INT->pre_scale = pre_scale;
	GPIO_AO_INT->deb_time = deb_time;
}
void HAL_GPIO_AO_INT_IRQ_DEB_Enable(int index)
{
	GPIO_AO_INT->deb_en |= (1 << index);
}

int HAL_GPIO_AO_INT_EdgePatch(int index, GPIO_AO_INT_HandleTypeDef *hexti)
{
#ifdef DOUBLE_EDGE_MODE
	if(hexti->trigger & IRQ_MODE_TRIG_EDGE_ACTIVE)//second times entry irq
	{
		if(hexti->trigger & IRQ_MODE_TRIG_EDGE_RISING)
		{
			GPIO_AO_INT->int_pol &= ~(1 << index);//low->high
		}
		else
		{
			GPIO_AO_INT->int_pol |= (1 << index);//high->low
		}
		hexti->trigger &= ~IRQ_MODE_TRIG_EDGE_ACTIVE;
		return 1;
	}
	/* first time entry irq */
	else if(hexti->trigger == IRQ_MODE_TRIG_EDGE_RISING)
	{
		GPIO_AO_INT->int_pol |= (1 << index);//high->low
		hexti->trigger |= IRQ_MODE_TRIG_EDGE_ACTIVE;
	}
	else if(hexti->trigger == IRQ_MODE_TRIG_EDGE_FALLING)
	{
		GPIO_AO_INT->int_pol &= ~(1 << index);//low->high
		hexti->trigger |= IRQ_MODE_TRIG_EDGE_ACTIVE;
	}
	return 1;
#else
	return 1;
#endif
}
#endif
