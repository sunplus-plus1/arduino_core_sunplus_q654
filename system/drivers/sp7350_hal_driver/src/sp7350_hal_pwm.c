#include "sp7350_hal_pwm.h"

static uint32_t g_resolution_sel[PWM_MAX]={0}; // for freq set

// 4096/25M, max_freq=6K  min_period=164000ns
// 4096/50M, max_freq=50K min_period=20000ns

// 256/25M, max_freq=90K  min_period= 11000ns
// 256/50M, max_freq=200K min_period=5100ns
static uint32_t _PWM_Get_SrcClk(void)
{
	uint32_t temp, freq_Hz;
	temp = READ_BIT(MOON3_REG->sft_cfg[27], (0x1<<5));
	if (temp == (0x1<<5)) {
		freq_Hz = 200000000;
	} else {
		freq_Hz = HSE_VALUE;
	}
	return freq_Hz;
}

static HAL_StatusTypeDef _PWM_Set(int pwm_num,uint32_t period,uint32_t duty)
{
	uint64_t duty_temp;
	uint32_t pwm_dd,pwm_resolution;
	
	volatile PWM_TypeDef *pwm_reg = PWM_REG;

	if(duty == 0 || duty >= period || (!IS_PWM_NUM_VALID(pwm_num)))
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
	pwm_dd = ((uint64_t)period * _PWM_Get_SrcClk() )/ (1000*1000*1000) / pwm_resolution;

	if(pwm_dd == 0)
	{
		pwm_reg->pwm_bypass |= (1 << pwm_num);   // bypass enable
	}
	else
	{
		pwm_reg->pwm_bypass &= ~(1 << pwm_num);   // bypass disable
		pwm_reg->pwm_dd_ctrl[pwm_num].dd = pwm_dd;
	}
	/* set duty cycle */
	pwm_reg->pwm_chx_cfg[pwm_num].pwm_duty = duty_temp;

	return HAL_OK;
}

HAL_StatusTypeDef HAL_PWM_Init(PWM_InitTypeDef *PWM_Init)
{
	int pwm_num;
	volatile  PWM_TypeDef *pwm_reg = PWM_REG;

	if(!PWM_Init || !IS_PWM_NUM_VALID(PWM_Init->pwm_num) || !IS_PWM_RESOLUTION_SEL_VALID(PWM_Init->resolution_sel))
	{
		return HAL_ERROR;
	}

	HAL_HW_Init(DISP_PWM); //hw init

	pwm_num = PWM_Init->pwm_num;
	g_resolution_sel[pwm_num] = PWM_Init->resolution_sel;

	/* select pwm resolution  256/4095 */
	MODIFY_REG(pwm_reg->pwm_resolution,1<<(pwm_num),PWM_Init->resolution_sel<<(pwm_num));

	/* select PWM_DD, PWM_NUM_X match PWM_DD_X */
	pwm_reg->pwm_chx_cfg[pwm_num].pwm_du_dd_sel = pwm_num + ePWM_DD0;

	return _PWM_Set(pwm_num,PWM_Init->period_ns,PWM_Init->duty_ns);
}

void HAL_PWM_Start(int pwm_num)
{
	volatile  PWM_TypeDef *pwm_reg = PWM_REG;
	if(IS_PWM_NUM_VALID(pwm_num))
	{
		pwm_reg->pwm_en |=  (1 << pwm_num);     // set pwm enable
	}
}

void HAL_PWM_Stop(int pwm_num)
{
	volatile  PWM_TypeDef *pwm_reg = PWM_REG;
	if(IS_PWM_NUM_VALID(pwm_num))
	{
		pwm_reg->pwm_en &= ~(1 << pwm_num);     // set pwm disable
	}
}

HAL_StatusTypeDef HAL_PWM_Period_Set(int pwm_num,uint32_t period,uint32_t duty)
{
	return _PWM_Set(pwm_num,period,duty);
}

