#include "sp7350_hal_gpio.h"

int gpio_input_invert_val_set(uint32_t bit,int invert_set)
{
	uint32_t idx;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	if(invert_set)
		*((volatile unsigned int *) (GPIO_I_INV(idx))) = RF_MASK_V_SET(1 << (bit & 0x0f));
	else
		*((volatile unsigned int *) (GPIO_I_INV(idx))) = RF_MASK_V_CLR(1 << (bit & 0x0f));
	return 0;
}

int gpio_input_invert_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_I_INV(idx)));

	return ((reg_val & value) ? 1 : 0);
}


int gpio_output_invert_val_set(uint32_t bit,int invert_set)
{
	uint32_t idx;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	if(invert_set)
		*((volatile unsigned int *) (GPIO_O_INV(idx))) = RF_MASK_V_SET(1 << (bit & 0x0f));
	else
		*((volatile unsigned int *) (GPIO_O_INV(idx))) = RF_MASK_V_CLR(1 << (bit & 0x0f));

	return 0;
}

int gpio_output_invert_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_O_INV(idx)));
	return ((reg_val & value) ? 1 : 0);
}

int gpio_open_drain_val_set(uint32_t bit,int od_set)
{
	uint32_t idx;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	if(od_set)
		*((volatile unsigned int *) (GPIO_OD(idx))) = RF_MASK_V_SET(1 << (bit & 0x0f));
	else
		*((volatile unsigned int *) (GPIO_OD(idx))) = RF_MASK_V_CLR(1 << (bit & 0x0f));

	return 0;
}

int gpio_open_drain_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_OD(idx)));
	return ((reg_val & value) ? 1 : 0);
}

/* set first register value , enable
   1: pin set in GPIO mode
   0: pin set in pinmux mode
*/
int gpio_first_val_set(uint32_t bit,int firset_set)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 5;
	if (idx > 4) {
		return -1;
	}

	value = 1 << (bit & 0x1f);

	reg_val = *((volatile unsigned int *)(GPIO_FIRST(idx)));
	if(firset_set)
	{
		reg_val |= value;
	}
	else
	{
		reg_val &= (~value);
	}
	
	*((volatile unsigned int *) (GPIO_FIRST(idx))) = reg_val;

	return 0;
}

uint32_t gpio_first_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 5;
	if (idx > 5) {
		return -1;
	}
	
	value = 1 << (bit & 0x1f);
	
	reg_val = *((volatile unsigned int *)(GPIO_FIRST(idx)));

	return((reg_val & value) ? 1 : 0);
}

/* set master value 
   1: gpio control by gpio register
   0: gpio control by IOP
*/
int gpio_master_val_set(uint32_t bit,int master_set)
{
	uint32_t idx;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	if(master_set)
		*((volatile unsigned int *) (GPIO_MASTER(idx))) = RF_MASK_V_SET(1 << (bit & 0x0f));
	else
		*((volatile unsigned int *) (GPIO_MASTER(idx))) = RF_MASK_V_CLR(1 << (bit & 0x0f));

	return 0;
}

/* get GPIO control mode register value 
   1: GPIO Group control mode 
   0: IOP control mode
*/
int gpio_master_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	value = 1 << (bit & 0x0f);
	reg_val = *((volatile unsigned int *)(GPIO_MASTER(idx)));

	return ((reg_val & value) ? 1 : 0);
}

/* set output enable value 
   1: output mode
   0: input mode
*/
int gpio_oe_val_set(uint32_t bit,int oe_set)
{
	uint32_t idx;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	if(oe_set)
		*((volatile unsigned int *) (GPIO_OE(idx))) = RF_MASK_V_SET(1 << (bit & 0x0f));
	else
		*((volatile unsigned int *) (GPIO_OE(idx))) = RF_MASK_V_CLR(1 << (bit & 0x0f));

	return 0;
}

/* get output enable value 
   1: output mode
   0: input mode
*/
int gpio_oe_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_OE(idx)));

	return ((reg_val & value) ? 1 : 0);
}

/****** gpio output mode, set output value ****/
int gpio_out_val_set(uint32_t bit, uint32_t gpio_out_value)
{
	uint32_t idx;

	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}

	if(gpio_out_value)
		*((volatile unsigned int *) (GPIO_OUT(idx))) = RF_MASK_V_SET(1 << (bit & 0x0f));
	else
		*((volatile unsigned int *) (GPIO_OUT(idx))) = RF_MASK_V_CLR(1 << (bit & 0x0f));

	return 0;
}

/****** gpio output mode, get output value ****/
int gpio_out_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 4;
	if (idx > 8) {
		return -1;
	}
	
	value = 1 << (bit & 0x0f);
	
	reg_val = *((volatile unsigned int *)(GPIO_OUT(idx)));

	return ((reg_val & value) ? 1 : 0);
}

/****** gpio input mode, get input value ****/
int gpio_in_val_get(uint32_t bit)
{
	uint32_t idx, value, reg_val;
	
	idx = bit >> 5;
	if (idx > 5) {
		return -1;
	}
	
	value = 1 << (bit & 0x1f);
	
	reg_val = *((volatile unsigned int *)(GPIO_IN(idx)));

	return ((reg_val & value) ? 1 : 0);
}


void dvio_pull_up_down_set(uint32_t bit, uint32_t pull)
{
	uint32_t idx;

	if(bit >= GPIO(20) && bit <= GPIO(49))
	{
		idx = 0;
		bit -= GPIO(20);
	}
	else if(bit >= GPIO(50) && bit <= GPIO(79))
	{
		idx = 1;
		bit -= GPIO(50);
	}
	/* clear bit */
	*((volatile unsigned int *)(GPIO_I_PD(idx))) &= ~(1 << bit);
	*((volatile unsigned int *)(GPIO_I_PU(idx))) &= ~(1 << bit);

	/* set bit */
	if(pull == GPIO_PULL_DOWN)
	{
		*((volatile unsigned int *)(GPIO_I_PD(idx))) |=  1 << bit;
	}
	else if(pull == GPIO_PULL_UP)
	{
		*((volatile unsigned int *)(GPIO_I_PU(idx))) |= 1 << bit;
	}

}


void gpio_pull_up_down_set(uint32_t bit, uint32_t pull)
{
	uint32_t idx;
	
	if(bit < GPIO(20))
	{
		idx = 0;
		bit -= GPIO(0);
	}
	else if (bit >= GPIO(80) && bit <= GPIO(91))
	{
		idx = 0;
		bit -= GPIO(60);
	}
	else if(bit >= GPIO(92) && bit <= GPIO(105))
	{
		idx = 1;
		bit -= GPIO(92);
	}
	/* disable pull up/down */
	*((volatile unsigned int *)(GPIO_I_PE(idx))) &= ~(1 << bit);

	/* set bit */
	if(pull == GPIO_PULL_DOWN)
	{
		*((volatile unsigned int *)(GPIO_I_PE(idx))) |= 1 << bit;
		*((volatile unsigned int *)(GPIO_I_PS(idx))) &= ~(1 << bit);
	}
	else if(pull == GPIO_PULL_UP)
	{
		*((volatile unsigned int *)(GPIO_I_PE(idx))) |= 1 << bit;
		*((volatile unsigned int *)(GPIO_I_PS(idx))) |= 1 << bit;
		*((volatile unsigned int *)(GPIO_I_SPU(idx))) |= 1 << bit;
	}
}

void gpio_pull_up_down_val_set(uint32_t bit, uint32_t pull)
{

	if(bit < GPIO(20) || bit > GPIO(79))
	{
		gpio_pull_up_down_set(bit,pull);
	}
	else
	{
		dvio_pull_up_down_set(bit,pull);
	}
}

uint8_t gpio_is_output(uint32_t GPIO_Pin)
{
	if(gpio_open_drain_val_get(GPIO_Pin) || gpio_oe_val_get(GPIO_Pin))
		return 1;

	return 0;
}

/*****************  HAL interface ************************/
HAL_StatusTypeDef HAL_GPIO_Init(GPIO_InitTypeDef *GPIO_Init)
{ 

	if(!GPIO_Init || !IS_GPIO_PIN(GPIO_Init->Pin))
		return HAL_ERROR;

	/* set pin to gpio mode */
	gpio_first_val_set(GPIO_Init->Pin,1);
	/* set pin control by gpio group register */
	gpio_master_val_set(GPIO_Init->Pin,1);

	switch(GPIO_Init->Mode)
	{
		case GPIO_OD_OUTPUT_MODE:
			/*	pin in opendrain mode */
			gpio_open_drain_val_set(GPIO_Init->Pin,1);
			gpio_out_val_set(GPIO_Init->Pin,GPIO_Init->out_value);
		break;
		case GPIO_PP_OUTPUT_MODE:
			gpio_oe_val_set(GPIO_Init->Pin,1);
			gpio_output_invert_val_set(GPIO_Init->Pin,GPIO_Init->invert);
			gpio_out_val_set(GPIO_Init->Pin,GPIO_Init->out_value);
		break;
		case GPIO_INPUT_MODE:
			gpio_pull_up_down_val_set(GPIO_Init->Pin,GPIO_Init->pull);
			gpio_input_invert_val_set(GPIO_Init->Pin,GPIO_Init->invert);
			gpio_oe_val_set(GPIO_Init->Pin,0);
		break;
		default:
			break;
	}

	return HAL_OK;
}

HAL_StatusTypeDef HAL_GPIO_DeInit(uint16_t GPIO_Pin)
{
	if(!IS_GPIO_PIN(GPIO_Pin) || !IS_GPIO_PIN_ACTION(GPIO_Pin))
		return HAL_ERROR;

	gpio_first_val_set(GPIO_Pin,0);

	gpio_master_val_set(GPIO_Pin,1);

	gpio_open_drain_val_set(GPIO_Pin,0);

	gpio_output_invert_val_set(GPIO_Pin,1);

	gpio_input_invert_val_set(GPIO_Pin,0);

	gpio_oe_val_set(GPIO_Pin,0);

	gpio_out_val_set(GPIO_Pin,0);

	return HAL_OK;
}

GPIO_PinState  HAL_GPIO_ReadPin(uint16_t GPIO_Pin)
{

	GPIO_PinState bitstatus;

	if(!IS_GPIO_PIN(GPIO_Pin) || !IS_GPIO_PIN_ACTION(GPIO_Pin))
		return HAL_ERROR;

	if(gpio_is_output(GPIO_Pin))
	{
		bitstatus = gpio_out_val_get(GPIO_Pin)? GPIO_OUT_HIGH:GPIO_OUT_LOW;
	}
	else
	{
		bitstatus = gpio_in_val_get(GPIO_Pin)? GPIO_OUT_HIGH:GPIO_OUT_LOW;
	}
	return bitstatus;
}

HAL_StatusTypeDef HAL_GPIO_WritePin(uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	if(!IS_GPIO_PIN(GPIO_Pin) || !IS_GPIO_PIN_ACTION(GPIO_Pin))
		return HAL_ERROR;

	/* get input/output mode. write data in output mode only 
		1:output mode  0:input mode 
	*/
	if(gpio_is_output(GPIO_Pin)) 
	{
		gpio_out_val_set(GPIO_Pin,PinState);
	}
	return HAL_OK;
}

HAL_StatusTypeDef HAL_GPIO_TogglePin(uint16_t GPIO_Pin)
{
	if(!IS_GPIO_PIN(GPIO_Pin) || !IS_GPIO_PIN_ACTION(GPIO_Pin))
		return HAL_ERROR;
	/* get input/output mode. write data in output mode only 
	   1:output mode  0:input mode 
	*/
	if(gpio_is_output(GPIO_Pin)) 
	{
		gpio_out_val_set(GPIO_Pin,!(gpio_out_val_get(GPIO_Pin)));
	}
	return HAL_OK;
}

int HAL_GPIO_Get_Mode(uint16_t GPIO_Pin)
{
	if(!IS_GPIO_PIN(GPIO_Pin) || !IS_GPIO_PIN_ACTION(GPIO_Pin))
		return HAL_ERROR;
	/* get input/output mode. write data in output mode only 
	   1:output mode  0:input mode 
	*/
	return gpio_oe_val_get(GPIO_Pin);
}

