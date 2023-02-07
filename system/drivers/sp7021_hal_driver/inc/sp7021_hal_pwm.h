
#ifndef __SP7021_HAL_PWM_H
#define __SP7021_HAL_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7021_hal_def.h"
#include "sp7021_arm926.h"

typedef enum {
	PWM0=0,
	PWM1,
	PWM2,
	PWM3,
	PWM4,
	PWM5,
	PWM6,
	PWM7,
	PWM_MAX
}PWM_NUMTypeDef;

enum {
	ePWM_DD0=0,
	ePWM_DD1,
	ePWM_DD2,
	ePWM_DD3,
	ePWM_DD_MAX
};


typedef struct
{
  /* set pwm number. the value set from PWM0 to PWM7 */
  PWM_NUMTypeDef pwm_num;
  /* set pwm period. unit is ns */
  uint32_t period_ns;
  /* set pwm duty. unit is ns */ 
  uint32_t duty_ns;
}PWM_InitTypeDef;


#define PWM_DEBUG    /* enable pwm debug  log */

#define IS_PWM_NUM_VALID(num)  (((num) >= PWM0) && ((num) <= PWM7))

#define pwm_assert_param(expr)   ((expr) ? (void)0 : (printf("[ERROR]: file %s on line %d\r\n",__FUNCTION__, __LINE__)))


int HAL_PWM_Init(PWM_InitTypeDef *PWM_Init);
void HAL_PWM_Start(int pwm_num);
void HAL_PWM_Stop(int pwm_num);

void HAL_PWM_Period_Set(int pwm_num,uint32_t period,uint32_t duty);

#ifdef __cplusplus
}
#endif

#endif
