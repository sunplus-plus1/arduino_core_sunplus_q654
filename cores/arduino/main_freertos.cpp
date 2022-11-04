#ifdef FREERTOS

#include "Arduino.h"

#include <FreeRTOS.h>
#include <task.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vAssertCalled(void);

	
void vApplicationMallocFailedHook( void )
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    taskDISABLE_INTERRUPTS();
    for( ;; ){};
}

void vAssertCalled( void )
{
    volatile unsigned long looping = 0;
    taskENTER_CRITICAL();
    {
        /* Use the debugger to set ul to a non-zero value in order to step out
                of this function to determine why it was called. */
        while( looping == 0LU )
        {
            portNOP();
        }
    }
    taskEXIT_CRITICAL();
}

#ifdef __cplusplus
}
#endif
/*-----------------------------------------------------------*/

extern int freertos_main (void);

void setup(void)
{
	freertos_main();
}

void loop(void)
{

}


#endif  // #ifdef FREERTOS

