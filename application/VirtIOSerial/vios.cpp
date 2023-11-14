#include "vios_common.h"
#include "VirtIOSerial.h"

#define VIOS_TASK_STACK_SIZE				(200)

SemaphoreHandle_t xVioAction_Semaphore;

#ifdef __cplusplus
extern "C" {
#endif

void vios_setup()
{
	xVioAction_Semaphore = xSemaphoreCreateBinary( );
	SerialVirtIO.begin();
	uart1.begin();
	uart2.begin();

	if( xVioAction_Semaphore != NULL )
	{
		xTaskCreate( vDoVIOreadwriteTask, "viorw", VIOS_TASK_STACK_SIZE, NULL, 1, NULL );
	}	
}

#ifdef __cplusplus
}
#endif

