#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>

#include "Arduino.h"
#include "pm_common.h"


#define MAILBOX_IN_SUSPEND_VALUE   (0xaabb1234)  /* match in sp_remoteproc.c*/

volatile int	deep_sleep = 0;
volatile Suspend_Type	suspend_state = SUSPEND_OUT;

void resume_by_rtc(void)
{
	RTC_REGS->rtc_ontime_set = RTC_REGS->rtc_timer + 0x1;
	RTC_REGS->rtc_ctrl = 0x2;  // rtc irq to pmc
}
void wakeup_shortkey()
{
	printf("short key \n");
	if(suspend_state == SUSPEND_IN)
	{
		printf("resume by RTC \n");
		resume_by_rtc();
	}
	else
	{
		/* mailbox to ca55 in suspend */
		deep_sleep = 0;
		suspend_state = SUSPEND_START;
		CM4_TO_CA55_MAILBOX->direct_transation[7]=MAILBOX_IN_SUSPEND_VALUE;
	}
}

void wakeup_longkey()
{
	printf("long key \n");
	deep_sleep = 1;
	suspend_state = SUSPEND_START;
	CM4_TO_CA55_MAILBOX->direct_transation[7]=MAILBOX_IN_SUSPEND_VALUE;
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
