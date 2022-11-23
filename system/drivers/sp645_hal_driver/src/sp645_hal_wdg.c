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

	assert_param(hwdg->StcFreq);

	boundary = MAX_TICKS * 1000 / hwdg->StcFreq;

	return (timeout < boundary) ? 1UL : 0UL;
}

static int _wdg_StcInit(WDG_HandleTypeDef *hwdg, STC_TypeDef *stcx)
{
	assert_param(hwdg);
	assert_param(stcx);

	HAL_StatusTypeDef ret = HAL_OK;
	uint32_t freq;

	//printf("\n[DEBUG] STC freq %d before init\n", _wdg_GetStcFreq(stcx));

	/* TODO: Temporary use EXT_CLK(27M)
	 * Method of cal STC freq:
	 * TEMP = ext_clk / (2 * (ext_div + 1))
	 * STC_FREQ = TEMP / (prescaler + 1)
	 * or
	 * STC_FREQ = sys_clk / (prescaler + 1).
	 */
	hwdg->Stc.Instance = stcx;
	hwdg->Stc.ClockSource = 1;
	hwdg->Stc.ExtDiv = 0;

	if (hwdg->Stc.ClockSource == 1)
	{
		freq = HSE_VALUE / ((hwdg->Stc.ExtDiv + 1) * 2);
	}
	else
	{
		freq = HAL_PLL_GetSystemFreq();
	}

	if(hwdg->StcFreq == 0)
		return 1;

	hwdg->Stc.Prescaler = freq / hwdg->StcFreq - 1;

	ret = HAL_STC_Init(&hwdg->Stc);
	if(ret != HAL_OK)
		return 1;

	return 0;
}

void HAL_WDG_IRQHandler(void *arg)
{
	assert_param(arg);

	WDG_HandleTypeDef *hwdg = (WDG_HandleTypeDef *)arg;

	assert_param(IS_STC_INSTANCE(hwdg->Instance));

	_wdg_clearflag(hwdg->Instance);

	if(hwdg->IrqMode == 1)
	{
		MOON4_REG->sft_cfg[18] = 0x02000200 | (0x10001 << hwdg->BitShift);
		_wdg_unlock(hwdg->Instance);
		_wdg_setcnt(hwdg->Instance, 0x0);
		_wdg_lock(hwdg->Instance);
	}

}

void HAL_WDG_SetTimeout(WDG_HandleTypeDef *hwdg, uint32_t val_ms)
{
	assert_param(IS_STC_INSTANCE(hwdg->Instance));
	assert_param(_wdg_OverTimeOutBoundary(hwdg, val_ms));

	uint32_t temp;

	temp = val_ms * hwdg->StcFreq / 1000;
	temp = temp >> 4;

	_wdg_stop(hwdg->Instance);
	_wdg_unlock(hwdg->Instance);
	_wdg_setcnt(hwdg->Instance, temp);
	_wdg_lock(hwdg->Instance);

}

uint32_t HAL_WDG_GetTimeout(WDG_HandleTypeDef *hwdg)
{
	assert_param(IS_STC_INSTANCE(hwdg->Instance));

	uint32_t temp = 0;

	temp = _wdg_getcnt(hwdg->Instance);

	temp = temp << 4;
	temp = temp * 1000 / hwdg->StcFreq ;

	return temp;
}

void HAL_WDG_Stop(WDG_HandleTypeDef *hwdg)
{
	assert_param(IS_STC_INSTANCE(hwdg->Instance));

	_wdg_stop(hwdg->Instance);
	_wdg_cntmax(hwdg->Instance);
	_wdg_clearflag(hwdg->Instance);
}

void HAL_WDG_Start(WDG_HandleTypeDef *hwdg)
{
	assert_param(IS_STC_INSTANCE(hwdg->Instance));

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
		id = STC_0;
		irq = STC_TIMER_W_IRQn;
		stcx = STC0;
		hwdg->BitShift = 10;
	}
	else if (hwdg->Instance == WDG1)
	{
		id = STC_AV0;
		irq = STC_AV0_TIMER_W_IRQn;
		stcx = STC1;
		hwdg->BitShift = 12;
	}
	else if (hwdg->Instance == WDG2)
	{
		id = STC_AV1;
		irq = STC_AV1_TIMER_W_IRQn;
		stcx = STC2;
		hwdg->BitShift = 13;
	}

	/* Enable clock */
	HAL_Module_Clock_enable(id , 1);
	HAL_Module_Clock_gate(id , 1);
	HAL_Module_Reset(id , 0);

	ret = _wdg_StcInit(hwdg, stcx);
	if(ret)
		return HAL_ERROR;

	//printf("\n[DEBUG] STC freq %d after init\n", _wdg_GetStcFreq(stcx));

	_wdg_stop(hwdg->Instance);
	_wdg_cntmax(hwdg->Instance);
	_wdg_clearflag(hwdg->Instance);

	/* Config irq */
	IRQ_SetHandler(irq, hwdg->IrqHandle);
	IRQ_Enable(irq);

	/* Enable the RBUS/WDG to respond the system reset when watchdog timeout */
	if (hwdg->IrqMode == 1)
		MOON4_REG->sft_cfg[18] = 0x02000000 | (0x10000 << hwdg->BitShift);
	else
		MOON4_REG->sft_cfg[18] = 0x02000200 | (0x10001 << hwdg->BitShift);

	/* Return function status */
	return HAL_OK;
}

