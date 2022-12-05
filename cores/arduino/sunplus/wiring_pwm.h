#ifndef __SUNPLUS_PWM_H
#define __SUNPLUS_PWM_H

#include "sunplus_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
param:
sp645/sp7350:   resolution 0:4096  1:256
sp7021:         Pin num
*/
static inline int pwm_init(int pwm_num, int period,int duty,int param)
{
	PWM_InitTypeDef PWM_Init;

	PWM_Init.pwm_num = (PWM_NUMTypeDef)pwm_num;
#ifndef SP7021
	PWM_Init.resolution_sel = (PWM_ResTypeDef)param;
#endif
	PWM_Init.period_ns = period;
	PWM_Init.duty_ns = duty;

#ifdef SP7021
	param = GPIO_TO_PINMUX(param);
	if(IS_VALID_PINMUX(param) == 0)
	{
		return HAL_ERROR;
	}
	/* set pwm pinmux */
	HAL_PINMUX_Cfg((PINMUX_Type)(PINMUX_PWM0 + pwm_num),param);
#else
	HAL_PINMUX_Cfg(PINMUX_DISP_PWM,1);
#endif

	return HAL_PWM_INIT(&PWM_Init);
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
