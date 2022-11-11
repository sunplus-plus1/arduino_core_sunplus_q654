#ifndef __IWATCHDOG_H__
#define __IWATCHDOG_H__

#ifdef SP7021
#include "sp7021_hal_conf.h"
#elif defined(SP645)
#include "sp645_hal_conf.h"
#endif

#include "sunplus_def.h"

#if !defined(IWDG) && defined(WDG2)
  #define IWDG WDG2
#endif

#define STC_FREQ 90000
// Maximal timeout in microseconds
//#define IWDG_TIMEOUT_MAX    (0xfffff * 1000 / STC_FREQ)

//#define IS_IWDG_TIMEOUT(X)  (((X) / 1000) <= IWDG_TIMEOUT_MAX)
#define IWDG_TIMEOUT_MAX    (0xfffff / STC_FREQ)

#define IS_IWDG_TIMEOUT(X)  ((X) <= IWDG_TIMEOUT_MAX)


typedef enum {
	WDG0_INDEX,
	WDG1_INDEX,
	WDG2_INDEX,
	WDG_NUM,
	UNKNOWN_WDG = 0XFFFF
} watchdog_index_t;

typedef struct  {
	void *__this;
	WDG_HandleTypeDef handle;
	WdgCallbackFunc pfcallback;
} watchdogObj_t;

class IWatchdogClass {

public:
	IWatchdogClass(WDG_TypeDef *instance);
	void begin(uint32_t timeout, uint32_t window = IWDG_TIMEOUT_MAX);
	void set(uint32_t timeout, uint32_t window = IWDG_TIMEOUT_MAX);
	void get(uint32_t *timeout, uint32_t *window = NULL);
	void reload(void);
	bool isEnabled(void)
	{
		return _enabled;
	};
	bool isReset(bool clear = false);
	void clearReset(void);
	void attachInterrupt(void (*callback)(void));
	void detachInterrupt();

private:
   	static bool _enabled;
	watchdogObj_t _watchdogObj;
};

extern IWatchdogClass IWatchdog;
#endif /* __IWATCHDOG_H__ */

