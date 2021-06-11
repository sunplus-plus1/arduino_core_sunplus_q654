#include "sp7021_hal.h"
#include "sp7021_hal_stc.h"


#ifdef __cplusplus
extern "C" {
#endif


HAL_StatusTypeDef HAL_STC_Init(STC_HandleTypeDef *Hstc)
{
	if (Hstc == NULL)
		return HAL_ERROR;
	assert_param(IS_STC_INSTANCE(Hstc->Instance));
	Hstc->Instance->stc_prescale_val = 0;
	Hstc->Instance->stc_config = 0;
	/*External Clock source*/
	if (Hstc->ClockSource){
		Hstc->Instance->stc_config = Hstc->ExtDiv;
	}
 	MODIFY_REG(Hstc->Instance->stc_prescale_val, STC_PRESCALER, Hstc->Prescaler<<STC_PRESCALER_Pos);
 	Hstc->Instance->stc_63_48 = 0;
	Hstc->Instance->stc_47_32 = 0;
	Hstc->Instance->stc_31_16 = 0;
	Hstc->Instance->stc_15_0 = 0;
	return HAL_OK;

}

HAL_StatusTypeDef HAL_STC_SetPrescaler(STC_HandleTypeDef *Hstc, uint32_t u32Prescaler)
{
	if (Hstc == NULL)
		return HAL_ERROR;
	assert_param(IS_STC_INSTANCE(Hstc->Instance));

	MODIFY_REG(Hstc->Instance->stc_prescale_val, STC_PRESCALER, Hstc->Prescaler<<STC_PRESCALER_Pos);
	Hstc->Prescaler = u32Prescaler;
	return HAL_OK;
}


HAL_StatusTypeDef HAL_STC_SetExtDiv(STC_HandleTypeDef *Hstc, uint32_t u32div)
{

	if (Hstc == NULL)
		return HAL_ERROR;
	assert_param(IS_STC_INSTANCE(Hstc->Instance));
	
	MODIFY_REG(Hstc->Instance->stc_config, STC_EXT_DIV, Hstc->Instance->stc_config << STC_EXT_DIV_Pos);
	Hstc->ExtDiv = u32div;
	
	return HAL_OK;
}

uint64_t HAL_STC_GetCounter(STC_HandleTypeDef *Hstc)
{
	if (Hstc == NULL)
		return 0;
	assert_param(IS_STC_INSTANCE(Hstc->Instance));
	#if 0
	return	(((uint64_t)Hstc->Instance->stc_63_48<<48)| \
					 ((uint64_t)Hstc->Instance->stc_47_32<<32)| \
					 ((uint64_t)Hstc->Instance->stc_31_16<<16)| \
					 ((uint64_t)Hstc->Instance->stc_15_0));
	#endif
	Hstc->Instance->stcl_32 = 0;
	return (Hstc->Instance->stcl_31_16<<16|Hstc->Instance->stcl_15_0);

	
	


}

uint32_t HAL_STC_GetPrescaler(STC_HandleTypeDef *Hstc)
{

	if (Hstc == NULL)
		return 0;
	assert_param(IS_STC_INSTANCE(Hstc->Instance));

	return (Hstc->Instance->stc_prescale_val&STC_PRESCALER_Msk)>>STC_PRESCALER_Pos;

}


uint32_t HAL_STC_GetExtDiv(STC_HandleTypeDef *Hstc)
{

	if (Hstc == NULL)
		return 0;
	assert_param(IS_STC_INSTANCE(Hstc->Instance));

	return (Hstc->Instance->stc_config&STC_EXT_DIV_Msk)>>STC_EXT_DIV_Pos;

}












#ifdef __cplusplus
}
#endif





