#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif


void setup()
{
	printf("pwm test start \n");
	pwm_init(PWM0,20000000,10000000,0,0);
	pwm_start(PWM0);
	delay(10000);
	pwm_stop(PWM0);
	delay(5000);
	HAL_PWM_Period_Set(PWM0,47000,40000);
	pwm_start(PWM0);
	delay(10000);
	pwm_stop(PWM0);
	printf("pwm test end \n");
}

void loop()
{

}


#ifdef __cplusplus
}
#endif


