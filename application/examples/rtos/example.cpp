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

int freertos_main (void)
{
	printf("cm4 freertos + arduino system start!!! \n");
	HAL_DMA_Test();
	for(;;);
}
