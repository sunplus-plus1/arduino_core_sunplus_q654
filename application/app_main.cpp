#include "Arduino.h"
#include "app_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void setup()
{
	#ifdef FREERTOS
		freertos_main();
	#else
		arduino_setup();
	#endif
}

void loop()
{
	#ifndef FREERTOS
		arduino_loop();
	#endif
}

#ifdef __cplusplus
}
#endif