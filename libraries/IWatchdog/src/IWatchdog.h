#ifndef __IWATCHDOG_H__
#define __IWATCHDOG_H__

#include "sunplus_def.h"

#if !defined(IWDG) && defined(WDG2)
  #define IWDG WDG2
#endif

/* Maximal timeout ticks*/
#define IWDG_TIMEOUT_MAX    0xffffffff

/* FIXME: Cal timeout by yourself */
//#define IS_IWDG_TIMEOUT(X)  ((X) <= IWDG_TIMEOUT_MAX)
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
#if defined(WDG3_BASE)
	WDG3_INDEX,
#endif
	WDG_NUM,
	UNKNOWN_WDG = 0XFFFF
} watchdog_index_t;

typedef enum {
	WDG_TICK_FMT, // default
	WDG_MICROSEC_FMT,
	WDG_MILLISEC_FMT,
	WDG_SEC_FMT
} WatchdogFormat_t;

typedef struct  {
	void *__this;
	uint32_t reload;
	WatchdogFormat_t format;
	WDG_HandleTypeDef handle;
	WdgCallbackFunc pfcallback;
} watchdogObj_t;

class IWatchdogClass {

public:
	IWatchdogClass(WDG_TypeDef *instance);
	void begin(uint32_t timeout, WatchdogFormat_t format = WDG_TICK_FMT);
	void set(uint32_t timeout, WatchdogFormat_t format = WDG_TICK_FMT);
	void get(uint32_t *timeout, WatchdogFormat_t format = WDG_TICK_FMT);
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

