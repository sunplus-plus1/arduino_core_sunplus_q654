#include "Arduino.h"

#include <FreeRTOS.h>
#include <task.h>


uint32_t start = 0;

void tim0_callback(void)
{
	
	printf("@@@@@@@[freertos]timer0 callback @@@@@@@, tick= %lu\n", HAL_GetTick());
	start = HAL_GetTick();
	//printf("start:%lu\n", start);
}


void vTask1(void const * argument)
{
	printf("%s, %d\n", __FUNCTION__, __LINE__);
	for(;;){
		printf("task1 run \n");
		vTaskDelay(1000);
	}

}

void vTask2(void const * argument)
{
	printf("%s, %d\n", __FUNCTION__, __LINE__);
	for(;;){
		printf("task2 run \n");
		vTaskDelay(1000);
	}

}


#include "HardwareSerial.h"
int freertos_main (void)
{
	printf("cm4 freertos + arduino system start!!! \n");

	HardwareTimer *timer0 = new HardwareTimer(TIM3);
	timer0->setPrescaleFactor(59999);
	timer0->setCount(10000-1, TICK_FORMAT);
	timer0->setOverflow(10000-1, TICK_FORMAT);
	printf("counter = %d\n", timer0->getCount(TICK_FORMAT));
	timer0->attachInterrupt(tim0_callback);
	start = (uint32_t)HAL_GetTick();
	printf("start:%lu\n", start);
	printf("freq = %d\n", timer0->getTimerClkFreq());
	timer0->resume();
	printf("%s, %d\n", __FUNCTION__, __LINE__);
	
	HardwareSerial uart;
	uart.begin(115200);

	uart.print("abcdef123456798abdefdefg");
	uart.print("9999");

	vTaskStartScheduler();
	printf("%s, %d\n", __FUNCTION__, __LINE__);
	for(;;);
}


























