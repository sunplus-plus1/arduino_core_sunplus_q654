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
	_watchdogObj.handle.IrqMode = 0;
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

	}

	_watchdogObj.pfcallback = NULL;
	HAL_WDG_Init(&_watchdogObj.handle);

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
	watchdog_index_t index = get_watchdog_index(_watchdogObj.handle.Instance);

	UNUSED(window);

	if (!IS_IWDG_TIMEOUT(timeout)) {
		return;
	}

	_watchdogObj.reload = timeout;

	//HAL_WDG_Init(&_watchdogObj.handle);

	_enabled = true;

	set(timeout, 0);

	HAL_WDG_Start(&_watchdogObj.handle);
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

	HAL_WDG_SetTimeout(&_watchdogObj.handle, timeout);

	/* test set timeout PASS */
	timeout = HAL_WDG_GetTimeout(&_watchdogObj.handle);
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

	// Timeout given in millisecond
	*timeout = HAL_WDG_GetTimeout(&_watchdogObj.handle);
}

/**
  * @brief  Reload the counter value with IWDG_RLR (IWDG_KR = 0x0000 AAAA)
  * @retval None
  */
void IWatchdogClass::reload(void)
{
	if (isEnabled()) {
		HAL_WDG_SetTimeout(&_watchdogObj.handle, _watchdogObj.reload);
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
	return index;
}
