
#include "sp7350_hal_tim.h"


#ifdef __cplusplus
extern "C" {
#endif

static IRQn_Type HAL_TIM_GetIRQ(TIM_HandleTypeDef *htim)
{
	IRQn_Type IRQn = MAX_IRQ_n;
	uint32_t tim;

	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));

	if (IS_TIM64_INSTANCE(htim->Instance64)) {
		tim = (uint32_t)htim->Instance64;
	} else {
		tim = (uint32_t)htim->Instance;
	}

	switch (tim)
	{
		case  (uint32_t)TIM0 ... (uint32_t)TIM2:
			IRQn = STC_TIMER0_IRQn + ((uint32_t)tim - (uint32_t)TIM0)/_OFFSET_BETWEEN_TIMERS;
			break;
		case  (uint32_t)TIM3 ... (uint32_t)TIM5:
			IRQn = STC_AV0_TIMER0_IRQn + ((uint32_t)tim - (uint32_t)TIM3)/_OFFSET_BETWEEN_TIMERS;
			break;
		case  (uint32_t)TIM6 ... (uint32_t)TIM8:
			IRQn = STC_AV1_TIMER0_IRQn + ((uint32_t)tim - (uint32_t)TIM6)/_OFFSET_BETWEEN_TIMERS;
			break;
  		case  (uint32_t)TIM9 ... (uint32_t)TIM11:
			IRQn = STC_AV2_TIMER0_IRQn + ((uint32_t)tim - (uint32_t)TIM9)/_OFFSET_BETWEEN_TIMERS;
			break;
		case  (uint32_t)TIM12 ... (uint32_t)TIM14:
			IRQn = STC_AV4_TIMER0_IRQn + ((uint32_t)tim - (uint32_t)TIM12)/_OFFSET_BETWEEN_TIMERS;
			break;
		case  (uint32_t)TIM15:
			IRQn = STC_TIMER3_IRQn;
			break;
		case  (uint32_t)TIM16:
			IRQn = STC_AV0_TIMER3_IRQn;
			break;
		case  (uint32_t)TIM17:
			IRQn = STC_AV1_TIMER3_IRQn;
			break;
		case  (uint32_t)TIM18:
			IRQn = STC_AV2_TIMER3_IRQn;
			break;
		case  (uint32_t)TIM19:
			IRQn = STC_AV4_TIMER3_IRQn;
			break;
		default:
			break;
	}
	return IRQn;
}

timerObj_t *HAL_TIM_Get_timer_obj(TIM_HandleTypeDef *htim)
{
	timerObj_t *obj;
	assert_param(htim);

	obj = (timerObj_t *)((char *)htim - offsetof(timerObj_t, handle));
	return (obj);
}

void TIM_SetConfig(TIM_HandleTypeDef *htim)
{
	TIM_InitTypeDef *Structure = &htim->Init;

	if(IS_TIM64_INSTANCE(htim->Instance64)) {
		MODIFY_REG(htim->Instance64->control, TIMER_TRIG_SRC, Structure->ClockSource << TIMER_TRIG_SRC_Pos);
		MODIFY_REG(htim->Instance64->control, TIMER_RPT, Structure->AutoReloadPreload << TIMER_RPT_Pos);
		htim->Instance64->counter_l = Structure->Counter;
		htim->Instance64->counter_h = Structure->Counter_h;
		htim->Instance64->reload_l = Structure->ReloadCounter;
		htim->Instance64->reload_h = Structure->ReloadCounter_h;
	}else{
		MODIFY_REG(htim->Instance->control, TIMER_TRIG_SRC, Structure->ClockSource << TIMER_TRIG_SRC_Pos);
		MODIFY_REG(htim->Instance->control, TIMER_RPT, Structure->AutoReloadPreload << TIMER_RPT_Pos);
		htim->Instance->counter_val = Structure->Counter;
		htim->Instance->reload_val = Structure->ReloadCounter;
	}
}

void HAL_TIM_HW_Set(TIM_HandleTypeDef *htim,int onoff)
{
	if((htim->Instance == TIM0) || (htim->Instance == TIM1) || (htim->Instance == TIM2) || (htim->Instance64 == TIM15)){
		(onoff == 1)?HAL_HW_Init(STC_0):HAL_HW_DeInit(STC_0);
	}
	else if(htim->Instance == TIM3 || htim->Instance == TIM4 || htim->Instance == TIM5 || htim->Instance64 == TIM16){
		(onoff == 1)?HAL_HW_Init(STC_AV0):HAL_HW_DeInit(STC_AV0);
	}
	else if(htim->Instance == TIM6 || htim->Instance == TIM7 || htim->Instance == TIM8 || htim->Instance64 == TIM17){
		(onoff == 1)?HAL_HW_Init(STC_AV1):HAL_HW_DeInit(STC_AV1);
	}
	else if(htim->Instance == TIM9 || htim->Instance == TIM10 || htim->Instance == TIM11 || htim->Instance64 == TIM18){
		(onoff == 1)?HAL_HW_Init(STC_AV2):HAL_HW_DeInit(STC_AV2);
	}
	else if(htim->Instance == TIM12 || htim->Instance == TIM13 || htim->Instance == TIM14 || htim->Instance64 == TIM19){
		(onoff == 1)?HAL_HW_Init(STC_AV4):HAL_HW_DeInit(STC_AV4);
	}
}

HAL_StatusTypeDef HAL_TIM_Init(TIM_HandleTypeDef *htim)
{
	IRQn_Type irqn =  MAX_IRQ_n;

	if (htim == NULL)
  	{
        return HAL_ERROR;
  	}

	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));

	HAL_TIM_HW_Set(htim,1);

	if (htim->State == HAL_TIM_STATE_RESET)
	{
		htim->Lock = HAL_UNLOCKED;
		irqn = HAL_TIM_GetIRQ(htim);
		IRQ_SetHandler(irqn, htim->IrqHandle);
		IRQ_Enable(irqn);
	}
	htim->State = HAL_TIM_STATE_BUSY;

	if (IS_TIM64_INSTANCE(htim->Instance64)) {
		htim->Instance64->control = 0;
		htim->Instance64->reload_l = 0;
		htim->Instance64->reload_h = 0;
		htim->Instance64->counter_l = 0;
		htim->Instance64->counter_h = 0;
	} else {
		htim->Instance->control = 0;
		htim->Instance->reload_val = 0;
		htim->Instance->counter_val = 0;
	}

	TIM_SetConfig(htim);

	/* Initialize the TIM state*/
	htim->State = HAL_TIM_STATE_READY;

	return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_DeInit(TIM_HandleTypeDef *htim)
{
	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));

	htim->State = HAL_TIM_STATE_BUSY;

	/* Disable the TIM Peripheral Clock */
	if(IS_TIM64_INSTANCE(htim->Instance64)) {
		MODIFY_REG(htim->Instance64->control, TIMER_GO, 0 << TIMER_GO_Pos);
	} else {
		MODIFY_REG(htim->Instance->control, TIMER_GO, 0 << TIMER_GO_Pos);
	}

	/* Change TIM state */
	htim->State = HAL_TIM_STATE_RESET;

	/* Release Lock */
	__HAL_UNLOCK(htim);

	return HAL_OK;
}


HAL_TIM_StateTypeDef HAL_TIM_GetState(TIM_HandleTypeDef *htim)
{
  	return htim->State;
}

HAL_StatusTypeDef HAL_TIM_Start(TIM_HandleTypeDef *htim)
{
	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));

	/* Set the TIM state */
	htim->State = HAL_TIM_STATE_BUSY;

	if(IS_TIM64_INSTANCE(htim->Instance64)) {
		MODIFY_REG(htim->Instance64->control, TIMER_GO, 1 << TIMER_GO_Pos);
		//printf("0x%x 0x%x", &htim->Instance64->control, htim->Instance64->control);
	} else {
		MODIFY_REG(htim->Instance->control, TIMER_GO, 1 << TIMER_GO_Pos);
	}

	/* Change the TIM state*/
	htim->State = HAL_TIM_STATE_READY;

	/* Return function status */
	return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_Stop(TIM_HandleTypeDef *htim)
{
	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));

	/* Set the TIM state */
	htim->State = HAL_TIM_STATE_BUSY;

	if(IS_TIM64_INSTANCE(htim->Instance64))
		MODIFY_REG(htim->Instance64->control, TIMER_GO, 0 << TIMER_GO_Pos);
	else
		MODIFY_REG(htim->Instance->control, TIMER_GO, 0 << TIMER_GO_Pos);

	/* Change the TIM state*/
	htim->State = HAL_TIM_STATE_READY;

	/* Return function status */
	return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_setCount(TIM_HandleTypeDef *htim, uint32_t u32Count, uint32_t u32Count_h)
{
	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));

	if(IS_TIM64_INSTANCE(htim->Instance64)) {
		htim->Instance64->counter_h = u32Count_h;
		htim->Instance64->counter_l = u32Count;
		htim->Instance64->reload_h = u32Count_h;
		htim->Instance64->reload_l = u32Count;
	} else {
		UNUSED(u32Count_h);
		htim->Instance->counter_val = u32Count;
		htim->Instance->reload_val = u32Count;
	}

	return HAL_OK;
}

uint32_t HAL_TIM_GetCount(TIM_HandleTypeDef *htim)
{
	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));

	if(IS_TIM64_INSTANCE(htim->Instance64)) {
		return htim->Instance64->counter_l;
	} else {
		return htim->Instance->counter_val;
	}
}

uint32_t HAL_TIM_GetCount_h(TIM_HandleTypeDef *htim)
{
	assert_param(IS_TIM64_INSTANCE(htim->Instance64));

	return htim->Instance64->counter_h;
}
uint32_t HAL_TIM_GetCLKSrc(TIM_HandleTypeDef *htim)
{
	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));

	if(IS_TIM64_INSTANCE(htim->Instance64)) {
		return (READ_BIT(htim->Instance64->control, TIMER_TRIG_SRC)>>TIMER_TRIG_SRC_Pos);
	} else {
		return (READ_BIT(htim->Instance->control, TIMER_TRIG_SRC)>>TIMER_TRIG_SRC_Pos);
	}
}

HAL_StatusTypeDef HAL_TIM_SetPrescaler(TIM_HandleTypeDef *htim, uint32_t u32Prescaler)
{
	return HAL_OK;
}
uint32_t HAL_TIM_GetPrescaler(TIM_HandleTypeDef *htim)
{
	return 0;
}

uint32_t HAL_TIM_GetMasterCLKFreq(TIM_HandleTypeDef *htim)
{
	TIM_TypeDef *hMtim = htim->Instance;
	uint32_t u32Feq = 0;
	uint32_t u32Src = 0;
	uint32_t u32Counter = 0;

	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(HAL_TIM_GetCLKSrc(htim) == CLK_SLAVE_WRAP_SRC) /* time0 used timer1 clk src,get timer1's freq */
	{
		switch ((uint32_t)htim->Instance)
		{
			case  (uint32_t)TIM0:
			case  (uint32_t)TIM3:
			case  (uint32_t)TIM6:
			case  (uint32_t)TIM9:
			case  (uint32_t)TIM12:
				hMtim = htim->Instance + _OFFSET_BETWEEN_TIMERS;
				break;
			case  (uint32_t)TIM1:
			case  (uint32_t)TIM4:
			case  (uint32_t)TIM7:
			case  (uint32_t)TIM10:
			case  (uint32_t)TIM13:
				hMtim = htim->Instance - _OFFSET_BETWEEN_TIMERS;
				break;
			default:
			break;
		}
	}

	u32Src = READ_BIT(hMtim->control, TIMER_TRIG_SRC)>>TIMER_TRIG_SRC_Pos;

	u32Counter = hMtim->counter_val;

	switch (u32Src){
		case CLK_SYS_SRC:
			u32Feq = HAL_PLL_GetSystemFreq();
			break;
		case CLK_STC_SRC:
			/* get stc base address by timer address */
		  	u32Feq = HAL_STC_GetClk((STC_TypeDef *)(((uint32_t)hMtim / _REG_GROUP_SIZE) * _REG_GROUP_SIZE));
			break;
		case CLK_RTC_SRC:
			break;
		default:
			u32Feq = HSI_VALUE;
			break;
	}

	if (HAL_TIM_GetCLKSrc(htim) == CLK_SLAVE_WRAP_SRC)
	{
		if(u32Counter != 0)
			u32Feq /= u32Counter;
	}
	return u32Feq;
}

HAL_StatusTypeDef HAL_TIM_Enable_Interrupt(TIM_HandleTypeDef *htim)
{
	IRQn_Type irqn =  MAX_IRQ_n;

	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}
	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));
	irqn = HAL_TIM_GetIRQ(htim);
	if (IRQ_Enable(irqn) == 0)
		return HAL_OK;
	else
		return HAL_ERROR;
}

HAL_StatusTypeDef HAL_TIM_Disable_Interrupt(TIM_HandleTypeDef *htim)
{
	IRQn_Type irqn =  MAX_IRQ_n;

	if (htim == NULL)
	{
		return HAL_ERROR;
	}
	assert_param(IS_TIM_INSTANCE(htim->Instance) || IS_TIM64_INSTANCE(htim->Instance64));
	irqn = HAL_TIM_GetIRQ(htim);
	if (IRQ_Disable(irqn) == 0)
		return HAL_OK;
	else
		return HAL_ERROR;
}

#ifdef __cplusplus
}
#endif
