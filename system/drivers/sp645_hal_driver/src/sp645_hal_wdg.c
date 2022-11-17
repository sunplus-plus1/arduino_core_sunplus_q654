#include "sp645_hal_wdg.h"

__STATIC_INLINE void _wdg_unlock(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_UNLOCK);
}

__STATIC_INLINE void _wdg_lock(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_LOCK);
}

__STATIC_INLINE void _wdg_stop(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_STOP);
}

__STATIC_INLINE void _wdg_start(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_START);
}

__STATIC_INLINE void _wdg_clearflag(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_CLRF);
}

__STATIC_INLINE void _wdg_cntmax(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_CNTMAX);
}

__STATIC_INLINE void _wdg_setcnt(WDG_TypeDef *Instance, uint16_t value)
{
	WRITE_REG(Instance->counter_val, value);
}

__STATIC_INLINE uint16_t _wdg_getcnt(WDG_TypeDef *Instance)
{
	return READ_REG(Instance->counter_val);
}

void HAL_WDG_IRQHandler(void *arg)
{
	assert_param(arg);

	WDG_HandleTypeDef *hwdg = (WDG_HandleTypeDef *)arg;

	assert_param(hwdg->Instance);

	_wdg_clearflag(hwdg->Instance);
}

void HAL_WDG_SetTimeout(WDG_HandleTypeDef *hwdg, uint32_t val_ms)
{
	assert_param(hwdg->Instance);
	assert_param(val_ms);

	uint32_t temp;

	temp = val_ms * STC_FREQ / 1000;
	temp = temp >> 4;

	_wdg_stop(hwdg->Instance);
	_wdg_unlock(hwdg->Instance);
	_wdg_setcnt(hwdg->Instance, temp);
	_wdg_lock(hwdg->Instance);
}

uint32_t HAL_WDG_GetTimeout(WDG_HandleTypeDef *hwdg)
{
	assert_param(hwdg->Instance);

	uint32_t temp;

	temp = _wdg_getcnt(hwdg->Instance);
	printf("temp ticks 0x%x \n", temp);

	temp = temp << 4;
	temp = temp * 1000 / STC_FREQ ;

	return temp;
}

void HAL_WDG_Stop(WDG_HandleTypeDef *hwdg)
{
	assert_param(hwdg->Instance);

	_wdg_stop(hwdg->Instance);
	_wdg_cntmax(hwdg->Instance);
	_wdg_clearflag(hwdg->Instance);
}

void HAL_WDG_Start(WDG_HandleTypeDef *hwdg)
{
	assert_param(hwdg->Instance);

	_wdg_start(hwdg->Instance);
}


HAL_StatusTypeDef HAL_WDG_Init(WDG_HandleTypeDef *hwdg)
{
	assert_param(hwdg->Instance);

	MODULE_ID_Type id;
	IRQn_Type irq;

	/* Enable clock */
	HAL_Module_Clock_enable(id , 1);
	HAL_Module_Clock_gate(id , 1);
	HAL_Module_Reset(id , 0);

	_wdg_stop(hwdg->Instance);
	_wdg_cntmax(hwdg->Instance);
	//test cnt max
	int cnt = _wdg_getcnt(hwdg->Instance);
	printf("CNTMAX 0x%x\n", cnt);
	_wdg_clearflag(hwdg->Instance);

	//while(1);
	/* 4.18 bit[9] MO1_RI_WATCHDOG_RST_EN */
	MOON4_REG->sft_cfg[18] = 0x02000200;

	if (hwdg->Instance == WDG0)
	{
		//printf("%d %s\n", __LINE__, __FUNCTION__);
		id = STC_0;
		irq = STC_TIMER_W_IRQn;
		MOON4_REG->sft_cfg[18] = 0x04000400;

	}
	else if (hwdg->Instance == WDG1)
	{
		id = STC_AV1;
		irq = STC_AV0_TIMER_W_IRQn;
		MOON4_REG->sft_cfg[18] = 0x10001000;
	}
	else if (hwdg->Instance == WDG2)
	{
		id = STC_AV2;
		irq = STC_AV1_TIMER_W_IRQn;
		MOON4_REG->sft_cfg[18] = 0x20002000;
	}

	/* Config irq */
	IRQ_SetHandler(irq, hwdg->IrqHandle);
	IRQ_Enable(irq);

	/* Return function status */
	return HAL_OK;
}

