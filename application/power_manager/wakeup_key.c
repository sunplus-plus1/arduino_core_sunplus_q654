#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>

#include "Arduino.h"
#include "pm_common.h"


#define MAILBOX_IN_SUSPEND_VALUE   (0xaabb1234)  /* match in sp_remoteproc.c*/
#define MBOX_RTC_SUSPEND_IN        (0x11225566)
#define MBOX_RTC_SUSPEND_OUT       (0x33447788)

volatile int	deep_sleep = 0;
volatile Suspend_Type	suspend_state = SUSPEND_OUT;

void suspend_resume_by_rtc(void)
{
#ifdef POWER_MAINDOMAIN_ALIVE
	RTC_REGS->rtc_ontime_set = RTC_REGS->rtc_timer + 0x1;
	RTC_REGS->rtc_ctrl = 0x2;  // rtc irq to pmc
#else
	system_PowerUP();
#endif
}
void freeze_resume_by_rtc(void)
{
	RTC_REGS->rtc_ontime_set = RTC_REGS->rtc_timer + 0x1;
	RTC_REGS->rtc_ctrl = 0x1;  // rtc irq to system
}
void wakeup_shortkey()
{
	if(suspend_state == FREEZE_CMD_IN)
	{
		printf("freeze resume by RTC \n");
		freeze_resume_by_rtc();
		suspend_state == FREEZE_CMD_OUT;
	}
	else if(suspend_state == SUSPEND_IN)
	{
		printf("suspend resume by RTC \n");
		suspend_resume_by_rtc();
	}
	else if(suspend_state == SUSPEND_OUT)
	{
		/* mailbox to ca55 in suspend */
	    printf("wakeupkey in suspend \n");
		deep_sleep = 0;
		suspend_state = SUSPEND_START;
		MBOX_TO_NOTIFY=MAILBOX_IN_SUSPEND_VALUE;
	}
}

void wakeup_longkey()
{
	printf("long key \n");
	deep_sleep = 1;
	suspend_state = SUSPEND_START;
	MBOX_TO_NOTIFY=MAILBOX_IN_SUSPEND_VALUE;
}

void vWakeyupKeyTask( void *pvParameters )
{
	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;
	TickType_t xoffsetTimeout = OFFSET_PRESS;
	TickType_t x1000msTimeout = T1000MS_PRESS;
	TickType_t x300msTimeout =  T300MS_PRESS;

	TickType_t xfirsttime,xsencondtime;
	TickType_t intervaltime; // time interval between 300ms/1000ms
	bool xPress = false;

	for( ;; )
	{
	    //for freeze wakeup by wakeupkey, change the suspend_state to freeze/mem.
		if(suspend_state == FREEZE_CMD_OUT && MBOX_RTC_WAKEUP == MBOX_RTC_SUSPEND_IN)
		{
			suspend_state = FREEZE_CMD_IN;
			MBOX_RTC_WAKEUP = 0;
		}
		else if(suspend_state == FREEZE_CMD_IN && MBOX_RTC_WAKEUP == MBOX_RTC_SUSPEND_OUT)
		{
			suspend_state = FREEZE_CMD_OUT;
			MBOX_RTC_WAKEUP = 0;
		}

		if((RTC_REGS->rtc_wakeupkey_int_status == 1) && !xPress)
		{
			xPress = true;
			RTC_REGS->rtc_wakeupkey_int_status = 1;
			xfirsttime = millis();
		}
		if((RTC_REGS->rtc_wakeupkey_int_status == 1) && xPress)
		{
			xsencondtime = millis();
			RTC_REGS->rtc_wakeupkey_int_status = 1;
		}

		if(xPress && (millis() - xfirsttime) > (100)) //check per 100ms
		{
			if((millis()-xfirsttime) > x300msTimeout && !intervaltime && ((suspend_state == FREEZE_CMD_IN) || (suspend_state == SUSPEND_IN)))
			{
				wakeup_shortkey();
				xfirsttime = 0;
				intervaltime = millis();
			}
			else if((millis()-xfirsttime) > x1000msTimeout && !intervaltime)
			{
				wakeup_shortkey();
				xfirsttime = 0;
				intervaltime = millis();
			}
		}
		if((millis() - xsencondtime) > 300) //check is unpress?
		{
			intervaltime = 0;
			xsencondtime = 0;
			xPress = false;
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void wakeup_key_init(void)
{
	pinMode(WAKEUP_KEY_PIN, INPUT);
	xTaskCreate( vWakeyupKeyTask, "wakeupkey", 200, NULL, 1, NULL );
}
