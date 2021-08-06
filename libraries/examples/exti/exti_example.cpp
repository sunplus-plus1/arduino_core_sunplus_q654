#include "WInterrupts.h"
#include "sp7021_hal_gpio.h"
#include "wiring_time.h"
#include "irq_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

GPIO_InitTypeDef gpio;
static int trigger;

void gpio_int_irq_callback()
{
	
    printf("IRQ callback\n");
/* 
	if external interrupt trigger mode is level,
	clear the interrupt signal of device when exit.
	here setting gpio low imitate clearing intr signal of the waveform.
 */
	if (trigger == HIGH)
	{
		HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
	}
	
}

void temp_output_waveform(uint32_t trigger)
{
	int i = 0;
	
	gpio.Pin = GPIO_P4_06;
	gpio.opendrain = GPIO_OD_DISABLE;
	gpio.Mode = GPIO_OUTPUT_MODE;
	
	HAL_GPIO_Init(&gpio);

	if(trigger == RISING)
	{
		/* 5 pulses , 3 seconds per pulse*/
		i = 5;
		while(i)
		{
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
			delay(3000);
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_HIGH);
			delay(3000);
			i--;
		}
		HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
	}

	if(trigger == HIGH)
	{
		/* 1 pulses , 1 seconds high level*/
		i = 2;
		while(i)
		{
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
			delay(3000);
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_HIGH);
			delay(1000);
			HAL_GPIO_WritePin(GPIO_P4_06, GPIO_OUT_LOW);
			i--;
		}
	}
}

void arduino_intr_test(void)
{
	trigger = HIGH;
	
	attachInterrupt(PIN_EXT_INT0, &gpio_int_irq_callback, trigger);
	
	temp_output_waveform(trigger);
	
	detachInterrupt(PIN_EXT_INT0);
}

#ifdef __cplusplus
}
#endif