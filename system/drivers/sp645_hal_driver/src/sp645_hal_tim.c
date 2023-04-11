
#include "sp645_hal_tim.h"



#ifdef __cplusplus
extern "C" {
#endif


static IRQn_Type HAL_TIM_GetIRQ(TIM_TypeDef *tim)
{
	IRQn_Type IRQn = MAX_IRQ_n;
	if ((tim == NULL) || (tim == (TIM_TypeDef*)0xFFFFFFFF))
		return IRQn;

	assert_param(IS_TIM_INSTANCE(tim));

	switch ((uint32_t)tim)
	{
		case  (uint32_t)TIM0 ... (uint32_t)TIM1:
			IRQn = STC_TIMER2_IRQn + ((uint32_t)tim - (uint32_t)TIM0)/_OFFSET_BETWEEN_TIMERS;
			break;
		case  (uint32_t)TIM2 ... (uint32_t)TIM3:
			IRQn = STC_AV0_TIMER2_IRQn + ((uint32_t)tim - (uint32_t)TIM2)/_OFFSET_BETWEEN_TIMERS;
			break;
		case  (uint32_t)TIM4 ... (uint32_t)TIM5:
			IRQn = STC_AV1_TIMER2_IRQn + ((uint32_t)tim - (uint32_t)TIM4)/_OFFSET_BETWEEN_TIMERS;
			break;
  		case  (uint32_t)TIM6 ... (uint32_t)TIM7:
			IRQn = STC_AV2_TIMER2_IRQn + ((uint32_t)tim - (uint32_t)TIM6)/_OFFSET_BETWEEN_TIMERS;
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

void TIM_SetConfig(TIM_TypeDef *TIMx, TIM_InitTypeDef *Structure)
{
	MODIFY_REG(TIMx->control, TIMER_TRIG_SRC, Structure->ClockSource<<TIMER_TRIG_SRC_Pos);
	MODIFY_REG(TIMx->control, TIMER_RPT, Structure->AutoReloadPreload<<TIMER_RPT_Pos);
	TIMx->counter_val = Structure->Counter;
	TIMx->reload_val = Structure->ReloadCounter;
}

HAL_StatusTypeDef HAL_TIM_Init(TIM_HandleTypeDef *htim)
{
	IRQn_Type irqn =  MAX_IRQ_n;

	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;

	if (htim->State == HAL_TIM_STATE_RESET)
	{
		htim->Lock = HAL_UNLOCKED;

		irqn = HAL_TIM_GetIRQ(htim->Instance);
		if (htim->IrqHandle != NULL){
			IRQ_SetHandler(irqn, htim->IrqHandle);
			IRQ_Enable(irqn);
		}else{
			IRQ_Disable(irqn);
		}
		//printf("set timer irq=%d  handler=%x \n",irqn,htim->IrqHandle);
	}
	htim->State = HAL_TIM_STATE_BUSY;
	htim->Instance->control = 0;
	htim->Instance->prescale_val = 0;
	htim->Instance->reload_val = 0;
	htim->Instance->counter_val = 0;
	TIM_SetConfig(htim->Instance, &htim->Init);

	/* Initialize the TIM state*/
	htim->State = HAL_TIM_STATE_READY;

	return HAL_OK;
}


HAL_StatusTypeDef HAL_TIM_DeInit(TIM_HandleTypeDef *htim)
{
  	/* Check the parameters */
	if (htim == NULL)
    	return HAL_ERROR;
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;

	htim->State = HAL_TIM_STATE_BUSY;

  	/* Disable the TIM Peripheral Clock */
  	MODIFY_REG(htim->Instance->control, TIMER_GO, 0<<TIMER_GO_Pos);

  	/* Change TIM state */
  	htim->State = HAL_TIM_STATE_RESET;

  	/* Release Lock */
  	__HAL_UNLOCK(htim);

	return HAL_OK;
}


HAL_TIM_StateTypeDef HAL_TIM_GetState(TIM_HandleTypeDef *htim)
{
  	/* Check the parameters */
	if (htim == NULL)
    	return HAL_ERROR;

	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;

  	return htim->State;
}

HAL_StatusTypeDef HAL_TIM_Start(TIM_HandleTypeDef *htim)
{
  	/* Check the parameters */
	if (htim == NULL)
    	return HAL_ERROR;

	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;
	/* Set the TIM state */
	htim->State = HAL_TIM_STATE_BUSY;

	MODIFY_REG(htim->Instance->control, TIMER_GO, 1<<TIMER_GO_Pos);

	/* Change the TIM state*/
	htim->State = HAL_TIM_STATE_READY;

	/* Return function status */
	return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_Stop(TIM_HandleTypeDef *htim)
{
  	/* Check the parameters */
	if (htim == NULL)
    	return HAL_ERROR;

	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;
	/* Set the TIM state */
	htim->State = HAL_TIM_STATE_BUSY;

	MODIFY_REG(htim->Instance->control, TIMER_GO, 0<<TIMER_GO_Pos);

	/* Change the TIM state*/
	htim->State = HAL_TIM_STATE_READY;

	/* Return function status */
	return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_SetPrescaler(TIM_HandleTypeDef *htim, uint32_t u32Prescaler)
{
  	/* Check the parameters */
	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;

	htim->Instance->prescale_val = u32Prescaler;
	htim->Init.Prescaler = u32Prescaler;
	return HAL_OK;
}

uint32_t HAL_TIM_GetPrescaler(TIM_HandleTypeDef *htim)
{
  	/* Check the parameters */
	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;

 	return htim->Instance->prescale_val;
}

HAL_StatusTypeDef HAL_TIM_setCount(TIM_HandleTypeDef *htim, uint32_t u32Count)
{
  	/* Check the parameters */
	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;

	htim->Instance->counter_val = u32Count;
	htim->Instance->reload_val = u32Count;
	return HAL_OK;
}

uint32_t HAL_TIM_GetCount(TIM_HandleTypeDef *htim)
{
  	/* Check the parameters */
	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;

	return htim->Instance->counter_val;
}

uint32_t HAL_TIM_GetCLKSrc(TIM_HandleTypeDef *htim)
{
	uint32_t u32Src = 0;
  	/* Check the parameters */
	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;

	return (READ_BIT(htim->Instance->control, TIMER_TRIG_SRC)>>TIMER_TRIG_SRC_Pos);
}

uint32_t HAL_TIM_GetMasterCLKFreq(TIM_HandleTypeDef *htim)
{
	TIM_TypeDef *hMtim = htim->Instance;
	uint32_t u32Feq = 0;
	uint32_t u32Src = 0;
	uint32_t u32Prescaler = 0;
	uint32_t u32Counter = 0;

  	/* Check the parameters */
	if (htim == NULL)
  	{
    	return HAL_ERROR;
  	}
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	if(!IS_TIM_INSTANCE(htim->Instance))
		return HAL_ERROR;

	if(HAL_TIM_GetCLKSrc(htim) == CLK_SLAVE_WRAP_SRC) /* time0 used timer1 clk src,get timer1's freq */
	{
		switch ((uint32_t)htim->Instance)
		{
			case  (uint32_t)TIM0:
			case  (uint32_t)TIM2:
			case  (uint32_t)TIM4:
			case  (uint32_t)TIM6:
				hMtim = htim->Instance + _OFFSET_BETWEEN_TIMERS;
				break;
			case  (uint32_t)TIM1:
			case  (uint32_t)TIM3:
			case  (uint32_t)TIM5:
			case  (uint32_t)TIM7:
				hMtim = htim->Instance - _OFFSET_BETWEEN_TIMERS;
				break;
			default:
			break;
		}
	}

	u32Src = READ_BIT(hMtim->control, TIMER_TRIG_SRC) >> TIMER_TRIG_SRC_Pos;
	u32Prescaler = hMtim->prescale_val;
	u32Counter = hMtim->counter_val;

	switch (u32Src){
		case CLK_SYS_SRC:
			u32Feq = SystemCoreClock;
			break;
		case CLK_STC_SRC:
		  	/*get stc base address by timer address */
		  	u32Feq = HAL_STC_GetClk((STC_TypeDef *)(((uint32_t)hMtim / _REG_GROUP_SIZE) * _REG_GROUP_SIZE));
			break;
		case CLK_RTC_SRC:
			break;
		case CLK_EXT_SRC:
			u32Feq = HSE_VALUE/2;
			break;
		default:
			u32Feq = SystemCoreClock;
			break;
	}

	u32Feq = u32Feq / (u32Prescaler + 1);
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
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	irqn = HAL_TIM_GetIRQ(htim->Instance);
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
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	irqn = HAL_TIM_GetIRQ(htim->Instance);
	if (IRQ_Disable(irqn) == 0)
		return HAL_OK;
	else
		return HAL_ERROR;
}

#ifdef __cplusplus
}
#endif

