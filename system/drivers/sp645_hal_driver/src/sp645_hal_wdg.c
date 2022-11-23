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

static uint32_t _wdg_GetStcFreq(STC_TypeDef *Instance)
{
	uint8_t stc_src_sel;
	uint32_t stc_prescale_val, sys_clk, ext_clk, stc_freq , stc_ext_div;

	assert_param(IS_STC_INSTANCE(Instance));

	stc_prescale_val = (Instance->stc_prescale_val & STC_PRESCALER_Msk) >> STC_PRESCALER_Pos;
	stc_src_sel = (Instance->stc_prescale_val & STC_TRIG_SRC_Msk) >> STC_TRIG_SRC_Pos;

	if (stc_src_sel == 0) { //SYS_CLK
		sys_clk = HAL_PLL_GetSystemFreq();
		stc_freq = sys_clk / (stc_prescale_val + 1);
		stc_freq = stc_freq * 1000;
	} else if (stc_src_sel == 1) { //EXT_CLK
		stc_ext_div = (Instance->stc_config & STC_EXT_DIV_Msk) >> STC_EXT_DIV_Pos;
		stc_freq = HSE_VALUE / (2 * (stc_ext_div + 1));
		stc_freq = stc_freq / (stc_prescale_val + 1);
	}
	return stc_freq;
}

static uint32_t _wdg_OverTimeOutBoundary(WDG_HandleTypeDef *hwdg, uint32_t timeout)
{
	uint32_t boundary;

	boundary = MAX_TICKS * 1000 / hwdg->StcFreq;

	return (timeout < boundary) ? 1UL : 0UL;
}

static void _wdg_StcInit(STC_HandleTypeDef *stc, STC_TypeDef *stcx)
{
	uint32_t freq;

	/* get the previous corresponding stc freq */
	freq = _wdg_GetStcFreq(stcx);
	printf("[WDG] Get the stc freq %d before init\n", freq);

	/* TODO: Temporary use EXT_CLK(27M)
	 * Method of cal STC freq:
	 * TEMP = ext_clk / (2 * (ext_div + 1))
	 * STC_FREQ = TEMP / (prescaler + 1)
	 * or
	 * STC_FREQ = sys_clk / (prescaler + 1).
	 */
	stc->Instance = stcx;
	stc->ClockSource = 1;
	stc->ExtDiv = 0;
	stc->Prescaler = 149;

	HAL_STC_Init(stc);
}

void HAL_WDG_IRQHandler(void *arg)
{
	assert_param(arg);

	WDG_HandleTypeDef *hwdg = (WDG_HandleTypeDef *)arg;

	assert_param(hwdg->Instance);

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
	assert_param(hwdg->Instance);
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
	assert_param(hwdg->Instance);

	uint32_t temp = 0;

	temp = _wdg_getcnt(hwdg->Instance);

	temp = temp << 4;
	temp = temp * 1000 / hwdg->StcFreq ;

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
	printf("[WDG] %s %s\n", __DATE__, __TIME__);

	MODULE_ID_Type id;
	IRQn_Type irq;
	STC_TypeDef *stcx;

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

	_wdg_StcInit(&hwdg->Stc, stcx);

	/* get the corresponding stc freq */
	hwdg->StcFreq = _wdg_GetStcFreq(stcx);
	printf("[WDG] Get the stc freq %d after init\n", hwdg->StcFreq);

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

