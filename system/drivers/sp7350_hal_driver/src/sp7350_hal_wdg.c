#include "sp7350_hal_wdg.h"

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

__STATIC_INLINE void _wdg_setmode(WDG_TypeDef *Instance, HAL_WDG_ModeTypeDef mode)
{
	WRITE_REG(Instance->mode, mode);
}

__STATIC_INLINE void _wdg_set_intrcnt(WDG_TypeDef *Instance, uint32_t value)
{
	WRITE_REG(Instance->intr_counter_val, value);
}

__STATIC_INLINE uint32_t _wdg_get_intrcnt(WDG_TypeDef *Instance)
{
	return READ_REG(Instance->intr_counter_val);
}

void HAL_WDG_IRQHandler(void *arg)
{
	WDG_HandleTypeDef *hwdg = (WDG_HandleTypeDef *)arg;

	assert_param(IS_WDG_INSTANCE(hwdg->Instance));

	_wdg_clearflag(hwdg->Instance);
}

/* STC_FREQ = 1000000 */
void HAL_WDG_SetTimeout(WDG_HandleTypeDef *hwdg, uint32_t u32Ticks)
{
	assert_param(IS_WDG_INSTANCE(hwdg->Instance));
	assert_param(IS_WDG_TICKS(u32Ticks));

	if(hwdg->IrqMode == WDG_INTR_RST) {
		u32Ticks -= hwdg->IrqTicks;
	}

	_wdg_stop(hwdg->Instance);
	_wdg_unlock(hwdg->Instance);
	_wdg_setcnt(hwdg->Instance, u32Ticks);
	if(hwdg->IrqMode == WDG_INTR_RST) {
		_wdg_set_intrcnt(hwdg->Instance, hwdg->IrqTicks);
	}
	_wdg_lock(hwdg->Instance);
}

uint32_t HAL_WDG_GetTimeout(WDG_HandleTypeDef *hwdg)
{
	assert_param(IS_WDG_INSTANCE(hwdg->Instance));

	uint32_t temp = 0, debug = 0;

	temp = _wdg_getcnt(hwdg->Instance);
	debug = temp;
	if(hwdg->IrqMode == WDG_INTR_RST) {
		temp += _wdg_get_intrcnt(hwdg->Instance);
	}

	//printf("debug. 0x%x temp ticks 0x%x \n", debug, temp);
	return temp;
}

void HAL_WDG_Stop(WDG_HandleTypeDef *hwdg)
{
	assert_param(IS_WDG_INSTANCE(hwdg->Instance));

	_wdg_stop(hwdg->Instance);
	_wdg_cntmax(hwdg->Instance);
	_wdg_clearflag(hwdg->Instance);
}

void HAL_WDG_Start(WDG_HandleTypeDef *hwdg)
{
	assert_param(IS_WDG_INSTANCE(hwdg->Instance));

	_wdg_start(hwdg->Instance);
}


HAL_StatusTypeDef HAL_WDG_Init(WDG_HandleTypeDef *hwdg)
{
	if(hwdg == NULL)
		return HAL_ERROR;

	IRQn_Type irq;
	int ret;

	if (hwdg->Instance == WDG0)
	{
		irq = STC_TIMER_W_IRQn;
		hwdg->BitShift = 1;
	}
	else if (hwdg->Instance == WDG1)
	{
		irq = STC_AV0_TIMER_W_IRQn;
		hwdg->BitShift = 2;
	}
	else if (hwdg->Instance == WDG2)
	{
		irq = STC_AV1_TIMER_W_IRQn;
		hwdg->BitShift = 3;
	}
	else if (hwdg->Instance == WDG3)
	{
		irq = STC_AV4_TIMERW_IRQn;
		hwdg->BitShift = 5;
	}

	_wdg_setmode(hwdg->Instance, hwdg->IrqMode);
	_wdg_stop(hwdg->Instance);
	_wdg_cntmax(hwdg->Instance);
	_wdg_clearflag(hwdg->Instance);

	/* Config irq */
	if(hwdg->IrqMode != WDG_RST) {
		IRQ_SetHandler(irq, hwdg->IrqHandle);
		IRQ_Enable(irq);
	}

	/* Enable the WDG to respond the system reset when watchdog timeout */
	MOON4_REG->sft_cfg[22] =  (0x10001 << hwdg->BitShift);

	/* Return function status */
	return HAL_OK;
}

