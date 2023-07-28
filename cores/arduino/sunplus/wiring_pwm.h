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
static inline HAL_StatusTypeDef pwm_init(PWM_InitTypeDef *PWM_Init) 
{

#ifdef SP7021
	if(IS_VALID_PINMUX(PWM_Init->pin) == 0)
	{
		return HAL_ERROR;
	}
	/* set pwm pinmux */
	HAL_PINMUX_Cfg((PINMUX_Type)(PINMUX_PWM0 + PWM_Init->pwm_num),PWM_Init->pin);
#elif defined(SP645)
	HAL_PINMUX_Cfg(PINMUX_DISP_PWM,1);
#elif defined(SP7350)
	if(PWM_Init->pin != 1 && PWM_Init->pin != 2) /* value 1/2 is vaild, value 0/3 is disable */
	{
		return HAL_ERROR;
	}
	HAL_PINMUX_Cfg((PINMUX_Type)(PINMUX_PWM_CH0 + 2*PWM_Init->pwm_num),PWM_Init->pin);
#endif

	return HAL_PWM_Init(PWM_Init);
}


static inline void pwm_start(int pwm_num)
{
	HAL_PWM_Start(pwm_num);
}

static inline void pwm_stop(int pwm_num)
{
	HAL_PWM_Stop(pwm_num);
}

static inline HAL_StatusTypeDef pwm_set_period(int pwm_num,int period,int duty)
{
	return HAL_PWM_Period_Set(pwm_num,period,duty);
}


#ifdef __cplusplus
}
#endif

#endif 
