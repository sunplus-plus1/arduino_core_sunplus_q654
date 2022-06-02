#ifndef __SP645_HAL_PWM_H
#define __SP645_HAL_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp645_hal_conf.h"
#include "sp64xx.h"

#define RESOLUTION_4096         (4096)
#define RESOLUTION_256		    (256)

typedef enum {
	PWM0=0,
	PWM1,
	PWM2,
	PWM3,
	PWM_MAX
}PWM_NUMTypeDef;

enum {
	ePWM_DD0=0,
	ePWM_DD1,
	ePWM_DD2,
	ePWM_DD3,
	ePWM_DD_MAX
};

typedef enum {
	PWM_RESOLUTION_0_4096=0,
	PWM_RESOLUTION_1_256,
	PWM_RESOLUTION_MAX
}PWM_ResTypeDef;

typedef struct
{
  /* set pwm number. the value set from PWM0 to PWM3 */
  PWM_NUMTypeDef pwm_num;
  /* select pwm resolution. 256 or 4096 */
  PWM_ResTypeDef pwm_resolution_sel;
  /* set pwm freq. unit is HZ */
  uint32_t pwm_freq;
  /* set pwm duty. the value set from  1 to 255/4095 */ 
  uint32_t pwm_duty;
}PWM_InitTypeDef;


#define IS_PWM_NUM_VALID(num)               (((num) >= PWM0) && ((num) <= PWM3))
#define IS_PWM_DUTY_VALID(duty)             (((duty) >= 0) && ((duty) <= RESOLUTION_4096))
#define IS_PWM_RESOLUTION_SEL_VALID(res)    (((res) == PWM_RESOLUTION_0_4096) || ((res) <= PWM_RESOLUTION_1_256))


int HAL_PWM_INIT(PWM_InitTypeDef *PWM_Init);
void HAL_PWM_ENABLE(int pwm_num);
void HAL_PWM_DISABLE(int pwm_num);
void HAL_PWM_Freq_Set(int pwm_num,uint32_t freq,uint32_t duty_cycle);
int HAL_PWM_Get_Resolution_By_Num(int pwm_num);

#ifdef __cplusplus
}
#endif

#endif

