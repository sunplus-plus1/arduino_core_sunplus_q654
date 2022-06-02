#include "sp645_hal_pwm.h"
#define PWM_SRC_CLK         (200*1000*1000)

static uint32_t g_resolution_sel[PWM_MAX]={0};

/*frequency = (1 / sys clk) * (1~262142)PWM DD0(18bit) * 4096 */
static int __PWM_Set_by_freq(int pwm_num,uint32_t freq,uint32_t duty_cycle)
{
	uint32_t pwm_dd;

	if(g_resolution_sel[pwm_num] == PWM_RESOLUTION_1_256 && duty_cycle >= RESOLUTION_256)
	{
		duty_cycle = (RESOLUTION_256-1);
	}
	else if(duty_cycle >= RESOLUTION_4096)
	{
		duty_cycle = (RESOLUTION_4096-1);
	}

	pwm_dd = PWM_SRC_CLK / freq / HAL_PWM_Get_Resolution_By_Num(pwm_num);

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
	PWM_REG->pwm_chx_cfg[pwm_num].pwm_duty = duty_cycle;

	return HAL_OK;
}

int HAL_PWM_INIT(PWM_InitTypeDef *PWM_Init)
{
	int pwm_num;
	
	assert_param(PWM_Init);
	assert_param(IS_PWM_NUM_VALID(pwm_num));
	assert_param(IS_PWM_DUTY_VALID(PWM_Init->pwm_duty));
	assert_param(IS_PWM_RESOLUTION_SEL_VALID(PWM_Init->pwm_resolution_sel));

	pwm_num = PWM_Init->pwm_num;
	g_resolution_sel[pwm_num] = PWM_Init->pwm_resolution_sel;
	
	/* set pwm pinmux */
	HAL_PINMUX_Cfg(PINMUX_DISP_PWM,1);
	
	/* select pwm resolution  256/4095 */
	MODIFY_REG(PWM_REG->pwm_resolution,1<<(pwm_num),PWM_Init->pwm_resolution_sel<<(pwm_num));

	/* select PWM_DD, PWM_NUM_X match PWM_DD_X */
	PWM_REG->pwm_chx_cfg[pwm_num].pwm_du_dd_sel = pwm_num + ePWM_DD0;

	return __PWM_Set_by_freq(pwm_num,PWM_Init->pwm_freq,PWM_Init->pwm_duty);
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

int HAL_PWM_Get_Resolution_By_Num(int pwm_num)
{
	uint32_t pwm_resolution;

	if(g_resolution_sel[pwm_num] == PWM_RESOLUTION_1_256)
		pwm_resolution = RESOLUTION_256;
	else
		pwm_resolution = RESOLUTION_4096;

	return pwm_resolution;
}

void HAL_PWM_Freq_Set(int pwm_num,uint32_t freq,uint32_t duty_cycle)
{
	__PWM_Set_by_freq(pwm_num,freq,duty_cycle);
}

