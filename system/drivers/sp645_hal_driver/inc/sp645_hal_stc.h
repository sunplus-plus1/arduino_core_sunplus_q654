#ifndef __SP645_HAL_STC_H__
#define __SP645_HAL_STC_H__

#include "sp645_cm4.h"
#include "sp645_hal_def.h"
#include "sp64xx.h"
#include "sp645_hal.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct
{
	STC_TypeDef			 *Instance;     /*!< Register base address             */
	uint32_t			ClockSource;
	uint32_t 			Prescaler;
	uint32_t			ExtDiv;
} STC_HandleTypeDef;
	
HAL_StatusTypeDef HAL_STC_Init(STC_HandleTypeDef *Hstc);
HAL_StatusTypeDef HAL_STC_SetPrescaler(STC_HandleTypeDef *Hstc, uint32_t u32Prescaler);
HAL_StatusTypeDef HAL_STC_SetExtDiv(STC_HandleTypeDef *Hstc, uint32_t u32div);
uint64_t HAL_STC_GetCounter(STC_HandleTypeDef *Hstc);
uint32_t HAL_STC_GetPrescaler(STC_HandleTypeDef *Hstc);
uint32_t HAL_STC_GetExtDiv(STC_HandleTypeDef *Hstc);
uint32_t HAL_STC_GetClk(STC_TypeDef *STC_Instance);



#ifdef __cplusplus
}
#endif

#endif

