#include "sp645_hal_pwm.h"
#define PWM_SRC_CLK         (200*1000*1000)

static uint32_t g_resolution_sel[PWM_MAX]={0}; // for freq set


/*
256: 3 ~ 781k
4096: 0.2 ~ 49k
*/
static int _PWM_Set(int pwm_num,uint32_t period,uint32_t duty)
{
	uint64_t duty_temp;
	uint32_t pwm_dd,pwm_resolution;

	if(duty == 0 || duty >= period)
		return HAL_ERROR;

	if(g_resolution_sel[pwm_num] == PWM_RESOLUTION_1_256 )
	{
		pwm_resolution = RESOLUTION_256;
	}
	else
	{
		pwm_resolution = RESOLUTION_4096;
	}
	duty_temp = ((uint64_t)duty*pwm_resolution)/period;

	/* pwm_dd = (1 / (sys clk * frequency)) * 4096 */
	pwm_dd = ((uint64_t)period * PWM_SRC_CLK )/ (1000*1000*1000) / pwm_resolution;

	if(pwm_dd == 0)
	{
		PWM_REG->pwm_bypass |= (1 << pwm_num);   // bypass enable
	}
	else
	{
		PWM_REG->pwm_bypass &= ~(1 << pwm_num);   // bypass disable
		PWM_REG->pwm_dd_ctrl[pwm_num].dd = pwm_dd;
	}

	/* set duty cycle */
	PWM_REG->pwm_chx_cfg[pwm_num].pwm_duty = duty_temp;

	return HAL_OK;
}

int HAL_PWM_INIT(PWM_InitTypeDef *PWM_Init)
{
	int pwm_num;

	if(!PWM_Init || !IS_PWM_NUM_VALID(PWM_Init->pwm_num) || !IS_PWM_RESOLUTION_SEL_VALID(PWM_Init->resolution_sel))
	{
		return HAL_ERROR;
	}

	pwm_num = PWM_Init->pwm_num;
	g_resolution_sel[pwm_num] = PWM_Init->resolution_sel;

	/* select pwm resolution  256/4095 */
	MODIFY_REG(PWM_REG->pwm_resolution,1<<(pwm_num),PWM_Init->resolution_sel<<(pwm_num));

	/* select PWM_DD, PWM_NUM_X match PWM_DD_X */
	PWM_REG->pwm_chx_cfg[pwm_num].pwm_du_dd_sel = pwm_num + ePWM_DD0;

	return _PWM_Set(pwm_num,PWM_Init->period_ns,PWM_Init->duty_ns);
}


void HAL_PWM_Start(int pwm_num)
{
	assert_param(IS_PWM_NUM_VALID(pwm_num));

	PWM_REG->pwm_en |=  (1 << pwm_num);     // set pwm enable
}

void HAL_PWM_Stop(int pwm_num)
{
	assert_param(IS_PWM_NUM_VALID(pwm_num));

	PWM_REG->pwm_en &= ~(1 << pwm_num);     // set pwm disable
}

void HAL_PWM_Period_Set(int pwm_num,uint32_t period,uint32_t duty)
{
	_PWM_Set(pwm_num,period,duty);
}

