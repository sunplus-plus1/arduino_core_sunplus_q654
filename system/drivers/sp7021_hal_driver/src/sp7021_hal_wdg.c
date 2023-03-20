#include "sp7021_hal_wdg.h"

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

static uint32_t _wdg_OverTimeOutBoundary(WDG_HandleTypeDef *hwdg, uint32_t timeout)
{
	uint32_t boundary;

	//boundary = MAX_TICKS * 1000 / hwdg->StcFreq;
	boundary = 0xffff0;

	return (timeout < boundary) ? 1UL : 0UL;
}

void HAL_WDG_IRQHandler(void *arg)
{
	assert_param(arg);

	WDG_HandleTypeDef *hwdg = (WDG_HandleTypeDef *)arg;

	assert_param(hwdg->Instance);

	_wdg_clearflag(hwdg->Instance);
}

void HAL_WDG_SetTimeout(WDG_HandleTypeDef *hwdg, uint32_t val)
{
	assert_param(hwdg->Instance);
	assert_param(_wdg_OverTimeOutBoundary(hwdg, val));

	_wdg_stop(hwdg->Instance);
	_wdg_unlock(hwdg->Instance);
	_wdg_setcnt(hwdg->Instance, val >> 4);
	_wdg_lock(hwdg->Instance);

}

uint32_t HAL_WDG_GetTimeout(WDG_HandleTypeDef *hwdg)
{
	assert_param(hwdg->Instance);

	uint32_t temp;

	temp = _wdg_getcnt(hwdg->Instance) << 4;

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
	STC_TypeDef *stcx;

	if (hwdg->Instance == WDG0)
	{
		irq = STC_TIMER_W_IRQn;
	}
	else if (hwdg->Instance == WDG1)
	{
		irq = STC_AV0_TIMER_W_IRQn;
	}

	_wdg_stop(hwdg->Instance);
	_wdg_cntmax(hwdg->Instance);
	_wdg_clearflag(hwdg->Instance);

	/* Config irq */
	IRQ_SetHandler(irq, hwdg->IrqHandle);
	IRQ_Enable(irq);

	/* Enable the RBUS to respond the system reset when watchdog timeout */
	MOON4_REG->sft_cfg[29] = 0x00020002;

	/* Enable the WDG to respond the system reset when watchdog timeout */
	if (hwdg->Instance == WDG0)
	{
		MOON4_REG->sft_cfg[29] = 0x00100010;
	}
	else if (hwdg->Instance == WDG1)
	{
		MOON4_REG->sft_cfg[29] = 0x00040004;
	}

	/* Return function status */
	return HAL_OK;
}

