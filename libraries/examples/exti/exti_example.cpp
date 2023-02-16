/*
 *	Test the high-level and edge-rising.
 *		pin set:
 *			output: GPIO simulates PWM waves
 *			input : Externel interrupt0
 *		expected result:
 *			HIGH: 	print every 1 seconds.
 *			RISING: print every 2 seconds.
 *
 *	if high-level trigger, pull down the level in irq_handler (simulate clearing the signal from  externel device)
 *
 *		Notice: Edge trigger is simulated by software.
 */
#include "Arduino.h"

GPIO_InitTypeDef gpio;
static int trigger;

#define INPUT_PIN	 GPIO(26)
void gpio_int_irq_callback()
{
/*
 *	if external interrupt trigger mode is level,
 *	clear the interrupt signal of device when exit.
 *	here setting gpio low imitate clearing intr signal of the waveform.
 */
	if (trigger == HIGH)
	{
		printf("exti high-level!\n");
		digitalWrite(INPUT_PIN, LOW);
	}
	else if (trigger == RISING)
	{
		printf("exti edge-rising!\n");
	}
}

void setup()
{
	trigger = RISING;
	pinMode(INPUT_PIN, OUTPUT);
	digitalWrite(INPUT_PIN, LOW);
	attachInterrupt(PIN_EXT_INT0, &gpio_int_irq_callback, trigger);
}

void loop()
{
	int i = 0;

	if(trigger == RISING)
	{
		/* 5 pulses , 2 seconds per pulse*/
		i = 5;
		while(i)
		{
			digitalWrite(INPUT_PIN, LOW);
			delay(2000);
			digitalWrite(INPUT_PIN, HIGH);
			/* no delay to simulate pulse */
			i--;
		}
		digitalWrite(INPUT_PIN, LOW);
	}

	if(trigger == HIGH)
	{
		/* 2 pulses , 1 seconds high level*/
		i = 2;
		while(1)
		{
			digitalWrite(INPUT_PIN, LOW);
			delay(1000);
			digitalWrite(INPUT_PIN, HIGH);
			delay(1000);
			i--;
		}
	}
	detachInterrupt(PIN_EXT_INT0);
}
