#ifndef __SP7350_HAL_ADC_H
#define __SP7350_HAL_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7350_hal_conf.h"
#include "sp7350xx.h"

#define SP_ADC_CLK_DIV_MASK		0x700
#define SP_ADC_DATA_MASK		0xFFF

#define SP_ADC_EN				(0x1<<1)
#define SP_ADC_SRFS				(0x2<<1)
#define SP_ADC_DATA_READY		(0x1<<15)  

typedef enum {
	ADC_CHAN_0=0,
	ADC_CHAN_1,
	ADC_CHAN_2,
	ADC_CHAN_3,
	ADC_CHAN_MAX
}ADC_NUMTypeDef;


#define IS_ADC_CHAN_VALID(num)               (((num) >= ADC_CHAN_0) && ((num) <= ADC_CHAN_3))

void HAL_ADC_Start(int pwm_num);
void HAL_ADC_Stop(int pwm_num);
uint32_t HAL_ADC_Get_Vaule(int adc_chan);

#ifdef __cplusplus
}
#endif

#endif

