#ifdef FREERTOS

#include "Arduino.h"

#include <FreeRTOS.h>
#include <task.h>
#include <string.h>

#ifdef SP7350
#include "pm.h"
#define PVT_TEST
#endif

#ifdef PVT_TEST
void isr_127(void)
{
	int value = CA55_TO_CM4_MAILBOX->normal_transation[0];
	CA55_TO_CM4_MAILBOX->cpu_to_cpu_int_trigger = 0;
	printf("ISR_127 %d\n",value);
	if(value == 0x77) {
		CM4_TO_CA55_MAILBOX->normal_transation[0]=0x99;
		CM4_TO_CA55_MAILBOX->cpu_to_cpu_writelock_flag=0x10;
		CM4_TO_CA55_MAILBOX->cpu_to_cpu_int_trigger = 1;
	}
}
#endif

int freertos_main (void)
{
	printf("\nFreeRTOS (build @pm_main "__TIME__")\n\n");


	#ifdef SP7350
	pm_main();
	#endif
	/* Start the tasks and timer running. */

#ifdef PVT_TEST
	NVIC_SetVector(IPC_CA552CM4_NORMAL_INT_IRQn, (uint32_t)isr_127);
	NVIC_EnableIRQ(IPC_CA552CM4_NORMAL_INT_IRQn);
#endif

	vTaskStartScheduler();
	for( ;; );
}


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
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ;; ){};
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeHeapSpace;

    /* This is just a trivial example of an idle hook.  It is called on each
    cycle of the idle task.  It must *NOT* attempt to block.  In this case the
    idle task just queries the amount of FreeRTOS heap that remains.  See the
    memory management section on the https://www.FreeRTOS.org web site for memory
    management options.  If there is a lot of heap memory free then the
    configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
    RAM. */
}

/*-----------------------------------------------------------*/

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
/*-----------------------------------------------------------*/
void vLoggingPrintf( const char *pcFormat, ... )
{
        va_list arg;

        va_start( arg, pcFormat );
        vprintf( pcFormat, arg );
        va_end( arg );
}

#ifdef __cplusplus
}
#endif
/*-----------------------------------------------------------*/

#endif  // #ifdef FREERTOS

