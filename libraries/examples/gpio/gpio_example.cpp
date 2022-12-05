#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LEDpin	81  /*I2CM1_SCL*/

void setup()
{
	pinMode(LEDpin, OUTPUT);
	digitalWrite(LEDpin, LOW);
}

void loop()
{
	digitalWrite(LEDpin, HIGH);
	delay(200);
	digitalWrite(LEDpin, LOW);
	delay(200);
}


#ifdef __cplusplus
}
#endif


