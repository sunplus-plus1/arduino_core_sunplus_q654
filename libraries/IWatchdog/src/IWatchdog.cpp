#include "IWatchdog.h"

static watchdogObj_t *IWatchdog_Handle[WDG_NUM] = {NULL};// used for irq handle

#if defined(WDG0_BASE)
static void WDG0_IRQHandler(void);
#endif

#if defined(WDG1_BASE)
static void WDG1_IRQHandler(void);
#endif

#if defined(WDG2_BASE)
static void WDG2_IRQHandler(void);
#endif

#if defined(WDG3_BASE)
static void WDG3_IRQHandler(void);
#endif

static watchdog_index_t get_watchdog_index(WDG_TypeDef *instance);

// Initialize static variable
bool IWatchdogClass::_enabled = false;

IWatchdogClass::IWatchdogClass(WDG_TypeDef *instance)
{
	watchdog_index_t index = get_watchdog_index(instance);

	if (index == UNKNOWN_WDG)
	{
		Error_Handler();
	}

	IWatchdog_Handle[index] = &_watchdogObj;
	_watchdogObj.handle.Instance = instance;

	//_watchdogObj.handle.IrqMode = WDG_RST;
	_watchdogObj.handle.IrqMode = WDG_INTR_RST;

	_watchdogObj.handle.IrqTicks = 1500;//us

	_watchdogObj.__this = (void *)this;

	switch(index)
	{
#if defined(WDG0_BASE)
		case WDG0_INDEX:
			_watchdogObj.handle.IrqHandle = &WDG0_IRQHandler;
			break;
#endif

#if defined(WDG1_BASE)
		case WDG1_INDEX:
			_watchdogObj.handle.IrqHandle = &WDG1_IRQHandler;
			break;
#endif

#if defined(WDG2_BASE)
		case WDG2_INDEX:
			_watchdogObj.handle.IrqHandle = &WDG2_IRQHandler;
			break;
#endif

#if defined(WDG3_BASE)
		case WDG3_INDEX:
			_watchdogObj.handle.IrqHandle = &WDG3_IRQHandler;
			break;
#endif

	}

	_watchdogObj.pfcallback = NULL;

	HAL_WDG_Init(&_watchdogObj.handle);
}


/**
  * @brief  Enable IWDG, must be called once
  * @param  timeout: value in microseconds
  * @param  format: The unit of the timeout value
  * @retval None
  */
void IWatchdogClass::begin(uint32_t timeout, WatchdogFormat_t format)
{
	watchdog_index_t index = get_watchdog_index(_watchdogObj.handle.Instance);

	if (!IS_IWDG_TIMEOUT(timeout)) {
		return;
	}

	_watchdogObj.reload = timeout;
	_watchdogObj.format = format;
	_enabled = true;

	set(timeout, format);
	HAL_WDG_Start(&_watchdogObj.handle);
}

/**
  * @brief  Set the timeout and window values
  * @param  timeout: value in microseconds
  * @param  format: the unit of timeout
  * @retval None
  */
void IWatchdogClass::set(uint32_t timeout, WatchdogFormat_t format)
{
	uint32_t u32Count_val = 0;
	uint32_t u32Freq = 0;

	if ((isEnabled()) && (!IS_IWDG_TIMEOUT(timeout))) {
		return;
	}

	u32Freq = HAL_STC_GetClk((STC_TypeDef *)(((uint32_t)_watchdogObj.handle.Instance / _REG_GROUP_SIZE) * _REG_GROUP_SIZE));

	switch(format){
		case WDG_MICROSEC_FMT:
			u32Count_val = timeout * (u32Freq / 1000000);
			break;
		case WDG_MILLISEC_FMT:
			u32Count_val = timeout * (u32Freq / 1000);
			break;
		case WDG_SEC_FMT:
			u32Count_val = timeout * u32Freq;
			break;
		case WDG_TICK_FMT:
		default:
			u32Count_val = timeout;
			break;
	}

	HAL_WDG_SetTimeout(&_watchdogObj.handle, u32Count_val);
}

/**
  * @brief  Get the current timeout and window values
  * @param  timeout: pointer to the get the value in microseconds
  * @retval None
  */
void IWatchdogClass::get(uint32_t *timeout, WatchdogFormat_t format)
{
	uint32_t u32Count_val = 0;
	uint32_t u32Freq = 0;

	if (timeout == NULL) {
		return;
	}

	u32Freq = HAL_STC_GetClk((STC_TypeDef *)(((uint32_t)_watchdogObj.handle.Instance / _REG_GROUP_SIZE) * _REG_GROUP_SIZE));

	u32Count_val = HAL_WDG_GetTimeout(&_watchdogObj.handle);

	switch(format){
		case WDG_MICROSEC_FMT:
			*timeout = u32Count_val / (u32Freq / 1000000);
			break;
		case WDG_MILLISEC_FMT:
			*timeout = u32Count_val / (u32Freq / 1000);
			break;
		case WDG_SEC_FMT:
			*timeout = u32Count_val / u32Freq;
			break;
		case WDG_TICK_FMT:
		default:
			*timeout = u32Count_val;
			break;
	}

}

/**
  * @brief  Reload the counter value with IWDG_RLR (IWDG_KR = 0x0000 AAAA)
  * @retval None
  */
void IWatchdogClass::reload(void)
{
	if (isEnabled()) {
		set(_watchdogObj.reload, _watchdogObj.format);
		HAL_WDG_Start(&_watchdogObj.handle);
	}
}

/**
  * @brief  Check if the system has resumed from IWDG reset
  * @param  clear: if true clear IWDG reset flag. Default false
  * @retval return reset flag status
  */
bool IWatchdogClass::isReset(bool clear)
{
	//TODO:
}

/**
  * @brief  Clear IWDG reset flag
  * @retval None
  */
void IWatchdogClass::clearReset(void)
{
	//TODO:
}

void IWatchdogClass::attachInterrupt(void (*callback)(void))
{
	uint32_t index = get_watchdog_index(_watchdogObj.handle.Instance);
	if (index == UNKNOWN_WDG)
		return;

	_watchdogObj.pfcallback = callback;
}

void IWatchdogClass:: detachInterrupt()
{
	uint32_t index = get_watchdog_index(_watchdogObj.handle.Instance);
	if (index == UNKNOWN_WDG)
		return;

	HAL_WDG_Stop(&_watchdogObj.handle);
	_watchdogObj.pfcallback = NULL;
}

#if defined(WDG0_BASE)
static void WDG0_IRQHandler(void)
{
	if ((IWatchdog_Handle[WDG0_INDEX] != NULL)
		&& (IWatchdog_Handle[WDG0_INDEX]->pfcallback))
	{
		IWatchdog_Handle[WDG0_INDEX]->pfcallback();
		HAL_WDG_IRQHandler(&IWatchdog_Handle[WDG0_INDEX]->handle);
	}
}
#endif

#if defined(WDG1_BASE)
static void WDG1_IRQHandler(void)
{
	if ((IWatchdog_Handle[WDG1_INDEX] != NULL)
		&& (IWatchdog_Handle[WDG1_INDEX]->pfcallback))
	{
		IWatchdog_Handle[WDG1_INDEX]->pfcallback();
		HAL_WDG_IRQHandler(&IWatchdog_Handle[WDG1_INDEX]->handle);
	}
}
#endif

#if defined(WDG2_BASE)
static void WDG2_IRQHandler(void)
{
	if ((IWatchdog_Handle[WDG2_INDEX] != NULL)
		&& (IWatchdog_Handle[WDG2_INDEX]->pfcallback))
	{
		IWatchdog_Handle[WDG2_INDEX]->pfcallback();
		HAL_WDG_IRQHandler(&IWatchdog_Handle[WDG2_INDEX]->handle);
	}
}
#endif

#if defined(WDG3_BASE)
static void WDG3_IRQHandler(void)
{
	if ((IWatchdog_Handle[WDG3_INDEX] != NULL)
		&& (IWatchdog_Handle[WDG3_INDEX]->pfcallback))
	{
		IWatchdog_Handle[WDG3_INDEX]->pfcallback();
		HAL_WDG_IRQHandler(&IWatchdog_Handle[WDG3_INDEX]->handle);
	}
}
#endif

static watchdog_index_t get_watchdog_index(WDG_TypeDef *instance)
{
	watchdog_index_t index = UNKNOWN_WDG;

#if defined(WDG0_BASE)
	if (instance == WDG0) {
		index = WDG0_INDEX;
	}
#endif

#if defined(WDG1_BASE)
	if (instance == WDG1) {
		index = WDG1_INDEX;
	}
#endif

#if defined(WDG2_BASE)
	if (instance == WDG2) {
		index = WDG2_INDEX;
	}
#endif

#if defined(WDG3_BASE)
	if (instance == WDG3) {
		index = WDG3_INDEX;
	}
#endif

	return index;
}
