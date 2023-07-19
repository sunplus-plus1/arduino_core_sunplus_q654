#ifndef __SP7350_HAL_ADC_H
#define __SP7350_HAL_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7350_hal_conf.h"
#include "sp7350xx.h"

#define SP_ADC_CLK_DIV_MASK     0x700
#define SP_ADC_DATA_MASK        0xFFF
#define SP_ADC_CHEN_MASK        0x38	//bit3 ~ bit5

#define SP_ADC_EN               0x02
#define SP_ADC_SRFS             0x04
#define SP_ADC_BYPASS           0x20
#define SP_ADC_DATA_READY       0x8000
#define SP_ADC_CHAN_SET         0x688  //  011010001000 = 0x688
#define SP_ADC_CHAN0_SET        0x00
#define SP_ADC_CHAN1_SET        0x08
#define SP_ADC_CHAN2_SET        0x10
#define SP_ADC_CHAN3_SET        0x18


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

