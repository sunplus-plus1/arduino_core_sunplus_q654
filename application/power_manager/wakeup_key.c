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
	RTC_REGS->rtc_ontime_set = RTC_REGS->rtc_timer + 0x1;
	RTC_REGS->rtc_ctrl = 0x2;  // rtc irq to pmc
}
void freeze_zmem_resume_by_rtc(void)
{
	RTC_REGS->rtc_ontime_set = RTC_REGS->rtc_timer + 0x1;
	RTC_REGS->rtc_ctrl = 0x1;  // rtc irq to system
}
void wakeup_shortkey()
{
	printf("short key\n");
	if(suspend_state == FREEZE_MEM_IN)
	{
		printf("freeze/mem resume by RTC \n");
		freeze_zmem_resume_by_rtc();
		suspend_state == FREEZE_MEM_OUT;
	}
	else if(suspend_state == SUSPEND_IN)
	{
		printf("suspend resume by RTC \n");
		suspend_resume_by_rtc();
	}
	else
	{
		/* mailbox to ca55 in suspend */
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
	TickType_t xoffsetTimeout = pdMS_TO_TICKS( OFFSET_PRESS );
	TickType_t xshortTimeout = pdMS_TO_TICKS( SHORT_PRESS );
	TickType_t xlongTimeout = pdMS_TO_TICKS( LONG_PRESS );

	TickType_t xfirsttime,xsencondtime;
	bool xPress = false;
	for( ;; )
	{
	    //for freeze/mem wakeup by wakeupkey, change the suspend_state to freeze/mem.
		if(suspend_state == FREEZE_MEM_OUT && MBOX_RTC_WAKEUP == MBOX_RTC_SUSPEND_IN)
		{
			suspend_state = FREEZE_MEM_IN;
			MBOX_RTC_WAKEUP = 0;
		}
		else if(suspend_state == FREEZE_MEM_IN && MBOX_RTC_WAKEUP == MBOX_RTC_SUSPEND_OUT)
		{
			suspend_state = FREEZE_MEM_OUT;
			MBOX_RTC_WAKEUP = 0;
		}

		if((RTC_REGS->rtc_wakeupkey_int_status == 1) && !xPress)
		{
			xPress = true;
			RTC_REGS->rtc_wakeupkey_int_status = 1;
			xfirsttime = xTaskGetTickCount();
		}
		if((RTC_REGS->rtc_wakeupkey_int_status == 1) && xPress)
		{
			xsencondtime = xTaskGetTickCount();
			RTC_REGS->rtc_wakeupkey_int_status = 1;
		}
		if(xPress && (xTaskGetTickCount() - xsencondtime) > pdMS_TO_TICKS(200))
		{
			if((xsencondtime-xfirsttime) > xshortTimeout &&  (xsencondtime-xfirsttime) < (xshortTimeout + xoffsetTimeout))
			{
				wakeup_shortkey();
			}
			if((xsencondtime-xfirsttime) > xlongTimeout &&  (xsencondtime-xfirsttime) < (xlongTimeout + xoffsetTimeout))
			{
				wakeup_longkey();
			}
			xPress = false;
			xfirsttime = 0;
			xsencondtime = 0;
			RTC_REGS->rtc_wakeupkey_int_status = 1;
		}
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

void wakeup_key_init(void)
{
	pinMode(WAKEUP_KEY_PIN, INPUT);
	xTaskCreate( vWakeyupKeyTask, "wakeupkey", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
}
