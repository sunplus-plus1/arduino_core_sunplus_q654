#ifndef __IWATCHDOG_H__
#define __IWATCHDOG_H__

#include "sunplus_def.h"

#if !defined(IWDG) && defined(WDG2)
  #define IWDG WDG2
#endif

#define STC_FREQ 90000

/* Maximal timeout in seconds */
//#define IWDG_TIMEOUT_MAX    (0xfffff / STC_FREQ)

/* Maximal timeout in milliseconds */
#define IWDG_TIMEOUT_MAX    (0xfffff * 1000 / STC_FREQ)

//#define IS_IWDG_TIMEOUT(X)  ((X) <= IWDG_TIMEOUT_MAX)
//#define IS_IWDG_TIMEOUT(X)  (((X) / 1000) <= IWDG_TIMEOUT_MAX)
#define IS_IWDG_TIMEOUT(X) (1)

typedef enum {
#if defined(WDG0_BASE)
	WDG0_INDEX,
#endif
#if defined(WDG1_BASE)
	WDG1_INDEX,
#endif
#if defined(WDG2_BASE)

	WDG2_INDEX,
#endif
	WDG_NUM,
	UNKNOWN_WDG = 0XFFFF
} watchdog_index_t;

typedef struct  {
	void *__this;
	uint16_t reload;
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

