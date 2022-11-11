#include "IWatchdog.h"

static watchdogObj_t *IWatchdog_Handle[WDG_NUM] = {NULL};// used for irq handle

static void WDG0_IRQHandler(void);
static void WDG1_IRQHandler(void);
static void WDG2_IRQHandler(void);
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
	_watchdogObj.__this = (void *)this;

	switch(index)
	{
		case WDG0_INDEX:
			_watchdogObj.handle.IrqHandle = &WDG0_IRQHandler;
			break;

		case WDG1_INDEX:
			_watchdogObj.handle.IrqHandle = &WDG1_IRQHandler;
			break;

		case WDG2_INDEX:
			_watchdogObj.handle.IrqHandle = &WDG2_IRQHandler;
			break;
	}

	_watchdogObj.pfcallback = NULL;
	HAL_WDG_Init(&(_watchdogObj.handle));

}


/**
  * @brief  Enable IWDG, must be called once
  * @param  timeout: value in microseconds
  * @param  window: optional value in microseconds
  *         Default: IWDG_TIMEOUT_MAX
  * @retval None
  */
void IWatchdogClass::begin(uint32_t timeout, uint32_t window)
{
	UNUSED(window);

	if (!IS_IWDG_TIMEOUT(timeout)) {
		return;
	}

	uint32_t value = 0;
#if 0
	float temp = STC_FREQ / 1000000;
	value = timeout * temp;
	value = value >> 4;
#else
	value = timeout * STC_FREQ;
	value = value >> 4;
#endif
	_watchdogObj.handle.Reload = value;

	//HAL_WDG_Init(&_watchdogObj.handle);

	_enabled = true;

	set(timeout, 0);

	HAL_WDG_START(_watchdogObj.handle.Instance);
}

/**
  * @brief  Set the timeout and window values
  * @param  timeout: value in microseconds
  * @param  window: optional value in microseconds
  *         Default: IWDG_TIMEOUT_MAX
  * @retval None
  */
void IWatchdogClass::set(uint32_t timeout, uint32_t window)
{
	UNUSED(window);

	if ((isEnabled()) && (!IS_IWDG_TIMEOUT(timeout))) {
		return;
	}

	uint32_t value = 0;

#if 0
	// Convert timeout to seconds

	float temp = STC_FREQ / 1000000;
	value = timeout * temp;
	value = value >> 4;
#else
	value = timeout * STC_FREQ;
	value = value >> 4;
#endif


	HAL_WDG_STOP(_watchdogObj.handle.Instance);
	HAL_WDG_UNLOCK(_watchdogObj.handle.Instance);
	HAL_WDG_SETCNT(_watchdogObj.handle.Instance, value);
	HAL_WDG_LOCK(_watchdogObj.handle.Instance);
}

/**
  * @brief  Get the current timeout and window values
  * @param  timeout: pointer to the get the value in microseconds
  * @param  window: optional pointer to the get the value in microseconds
  * @retval None
  */
void IWatchdogClass::get(uint32_t *timeout, uint32_t *window)
{
	UNUSED(window);

	if (timeout == NULL) {
		return;
	}

	uint32_t value;

	// Timeout given in microseconds
	value = HAL_WDG_GETCNT(_watchdogObj.handle.Instance);
#if 0
	value = _watchdogObj.handle.Reload - value;
	float temp = STC_FREQ / 1000000;
	*timeout = (value << 4) / temp;
#else
	value = _watchdogObj.handle.Reload - value;
	*timeout = value;
#endif
}

/**
  * @brief  Reload the counter value with IWDG_RLR (IWDG_KR = 0x0000 AAAA)
  * @retval None
  */
void IWatchdogClass::reload(void)
{
	if (isEnabled()) {
		HAL_WDG_UNLOCK(_watchdogObj.handle.Instance);
		HAL_WDG_SETCNT(_watchdogObj.handle.Instance, _watchdogObj.handle.Reload);
		HAL_WDG_LOCK(_watchdogObj.handle.Instance);
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

	HAL_WDG_STOP(_watchdogObj.handle.Instance);
	_watchdogObj.pfcallback = NULL;

}

static void WDG0_IRQHandler(void)
{
	if ((IWatchdog_Handle[WDG0_INDEX] != NULL)
		&& (IWatchdog_Handle[WDG0_INDEX]->pfcallback))
	{
		 IWatchdog_Handle[WDG0_INDEX]->pfcallback();
	}
	HAL_WDG_CLRF(IWatchdog_Handle[WDG0_INDEX]->handle.Instance);
}

static void WDG1_IRQHandler(void)
{
	if ((IWatchdog_Handle[WDG1_INDEX] != NULL)
		&& (IWatchdog_Handle[WDG1_INDEX]->pfcallback))
	{
		 IWatchdog_Handle[WDG1_INDEX]->pfcallback();
	}
	HAL_WDG_CLRF(IWatchdog_Handle[WDG1_INDEX]->handle.Instance);
}

static void WDG2_IRQHandler(void)
{
	if ((IWatchdog_Handle[WDG2_INDEX] != NULL)
		&& (IWatchdog_Handle[WDG2_INDEX]->pfcallback))
	{
		 IWatchdog_Handle[WDG2_INDEX]->pfcallback();
	}
	HAL_WDG_CLRF(IWatchdog_Handle[WDG2_INDEX]->handle.Instance);
}

static watchdog_index_t get_watchdog_index(WDG_TypeDef *instance)
{
	watchdog_index_t index = UNKNOWN_WDG;

	if (instance == WDG0) {
		index = WDG0_INDEX;
	}

	if (instance == WDG1) {
		index = WDG1_INDEX;
	}

	if (instance == WDG2) {
		index = WDG2_INDEX;
	}

	return index;
}
