#ifndef __SUNPLUS_PWM_H
#define __SUNPLUS_PWM_H

#include "sunplus_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
param:
sp645/sp7350:   resolution 0:4096  1:256
sp7021:         not used

pin:
sp645:          not used
sp7021:         pin num
sp7350:         value 1/2 is vaild, value 0/3 is disable
*/
static inline int pwm_init(int pwm_num, int period_ns,int duty_ns,int pin,int param)
{
	PWM_InitTypeDef PWM_Init;

	PWM_Init.pwm_num = (PWM_NUMTypeDef)pwm_num;
#ifndef SP7021
	PWM_Init.resolution_sel = (PWM_ResTypeDef)param;
#endif
	PWM_Init.period_ns = period_ns;
	PWM_Init.duty_ns = duty_ns;

#ifdef SP7021
	param = GPIO_TO_PINMUX(pin);
	if(IS_VALID_PINMUX(pin) == 0)
	{
		return HAL_ERROR;
	}
	/* set pwm pinmux */
	HAL_PINMUX_Cfg((PINMUX_Type)(PINMUX_PWM0 + pwm_num),pin);
#elif defined(SP645)
	HAL_PINMUX_Cfg(PINMUX_DISP_PWM,1);
#elif defined(SP7350)
	if(pin != 1 && pin != 2) /* value 1/2 is vaild, value 0/3 is disable */
	{
		return HAL_ERROR;
	}
	HAL_PINMUX_Cfg((PINMUX_Type)(PINMUX_PWM_CH0 + 2*pwm_num),pin);
#endif

	return HAL_PWM_Init(&PWM_Init);
}


static inline void pwm_start(int pwm_num)
{
	HAL_PWM_Start(pwm_num);
}

static inline void pwm_stop(int pwm_num)
{
	HAL_PWM_Stop(pwm_num);
}

static inline void pwm_set_period(int pwm_num,int period,int duty)
{
	HAL_PWM_Period_Set(pwm_num,period,duty);
}


#ifdef __cplusplus
}
#endif

#endif 
