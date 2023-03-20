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

/* This function used for DEBUG */
static uint32_t _wdg_GetStcFreq(STC_TypeDef *Instance)
{
	uint8_t stc_src_sel;
	uint32_t stc_prescale_val, sys_clk, ext_clk, stc_freq , stc_ext_div;

	assert_param(IS_STC_INSTANCE(Instance));

	stc_prescale_val = (Instance->stc_prescale_val & STC_PRESCALER_Msk) >> STC_PRESCALER_Pos;
	stc_src_sel = (Instance->stc_prescale_val & STC_TRIG_SRC_Msk) >> STC_TRIG_SRC_Pos;

	if (stc_src_sel == 0) //SYS_CLK
	{
		sys_clk = HAL_PLL_GetSystemFreq();
		stc_freq = sys_clk / (stc_prescale_val + 1);
	}
	else if (stc_src_sel == 1) //EXT_CLK
	{
		stc_ext_div = (Instance->stc_config & STC_EXT_DIV_Msk) >> STC_EXT_DIV_Pos;
		stc_freq = HSE_VALUE / (2 * (stc_ext_div + 1));
		stc_freq = stc_freq / (stc_prescale_val + 1);
	}
	return stc_freq;
}

static uint32_t _wdg_OverTimeOutBoundary(WDG_HandleTypeDef *hwdg, uint32_t timeout)
{
	uint32_t boundary;

	//assert_param(hwdg->StcFreq);

	boundary = 0xffff0;

	return (timeout < boundary) ? 1UL : 0UL;
}

void HAL_WDG_IRQHandler(void *arg)
{
	//assert_param(arg);

	WDG_HandleTypeDef *hwdg = (WDG_HandleTypeDef *)arg;

	assert_param(IS_WDG_INSTANCE(hwdg->Instance));

	_wdg_clearflag(hwdg->Instance);

	if(hwdg->IrqMode == 1)
	{
		MOON4_REG->sft_cfg[18] = 0x02000200 | (0x10001 << hwdg->BitShift);
		_wdg_unlock(hwdg->Instance);
		_wdg_setcnt(hwdg->Instance, 0x0);
		_wdg_lock(hwdg->Instance);
	}

}

void HAL_WDG_SetTimeout(WDG_HandleTypeDef *hwdg, uint32_t val)
{
	assert_param(IS_WDG_INSTANCE(hwdg->Instance));
	assert_param(IS_WDG_TICKS(val));

	_wdg_stop(hwdg->Instance);
	_wdg_unlock(hwdg->Instance);
	_wdg_setcnt(hwdg->Instance, val >> 4);
	_wdg_lock(hwdg->Instance);
}

uint32_t HAL_WDG_GetTimeout(WDG_HandleTypeDef *hwdg)
{
	assert_param(IS_WDG_INSTANCE(hwdg->Instance));

	uint32_t temp = 0;

	temp = _wdg_getcnt(hwdg->Instance) << 4;

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

	MODULE_ID_Type id;
	IRQn_Type irq;
	STC_TypeDef *stcx;
	int ret;

	if (hwdg->Instance == WDG0)
	{
		irq = STC_TIMER_W_IRQn;
		hwdg->BitShift = 10;
	}
	else if (hwdg->Instance == WDG1)
	{
		irq = STC_AV0_TIMER_W_IRQn;
		hwdg->BitShift = 12;
	}
	else if (hwdg->Instance == WDG2)
	{
		irq = STC_AV1_TIMER_W_IRQn;
		hwdg->BitShift = 13;
	}

	_wdg_stop(hwdg->Instance);
	_wdg_cntmax(hwdg->Instance);
	_wdg_clearflag(hwdg->Instance);

	/* Config irq */
	//IRQ_SetHandler(irq, hwdg->IrqHandle);
	//IRQ_Enable(irq);

	/* Enable the RBUS/WDG to respond the system reset when watchdog timeout */
	if (hwdg->IrqMode == 1)
		MOON4_REG->sft_cfg[18] = 0x02000000 | (0x10000 << hwdg->BitShift);
	else
		MOON4_REG->sft_cfg[18] = 0x02000200 | (0x10001 << hwdg->BitShift);

	/* Return function status */
	return HAL_OK;
}

