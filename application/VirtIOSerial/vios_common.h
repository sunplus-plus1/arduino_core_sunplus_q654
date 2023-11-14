#ifndef __VIOS_PRIVATE_H__
#define __VIOS_PRIVATE_H__

#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>

#include "Arduino.h"
#include "sp7350_hal_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

extern SemaphoreHandle_t xVioAction_Semaphore;
void vDoVIOreadwriteTask( void *pvParameters );

#ifdef __cplusplus
}
#endif
#endif