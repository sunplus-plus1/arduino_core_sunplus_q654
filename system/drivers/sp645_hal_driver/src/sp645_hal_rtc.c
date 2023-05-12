#include <stdlib.h>
#include "Arduino.h"
#include "sp645_hal_rtc.h"

IRQHandler_t periodic_cb = NULL;
IRQHandler_t alarm_cb = NULL;

#define LEAPS_THRU_END_OF(y) ((y) / 4 - (y) / 100 + (y) / 400)

static const uint8_t rtc_days_in_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static inline uint8_t is_leap_year(uint32_t year)
{
	return (!(year % 4) && (year % 100) || !(year % 400));
}

/* The number of days in the month. */
int rtc_month_days(uint32_t month, uint32_t year)
{
	return rtc_days_in_month[month] + (is_leap_year(year) && month == 1);
}

void rtc_sec_to_tm(uint32_t time, struct rtc_time *tm)
{
	uint32_t month, year, secs;
	uint32_t days;

	/* time must be positive */
	//days = timediv_s64_rem(time, 86400, &secs);
	days = time / 86400;
	secs = time % 86400;

	/* day of the week, 1970-01-01 was a Thursday */
	tm->tm_wday = (days + 4) % 7;

	year = 1970 + days / 365;
	days -= (year - 1970) * 365
		+ LEAPS_THRU_END_OF(year - 1)
		- LEAPS_THRU_END_OF(1970 - 1);

	tm->tm_year = year;
	tm->tm_yday = days + 1;

	for (month = 0; month < 11; month++) {
		int newdays;

		newdays = days - rtc_month_days(month, year);
		if (newdays < 0)
			break;
		days = newdays;
	}
	tm->tm_mon = month;
	tm->tm_mday = days + 1;

	tm->tm_hour = secs / 3600;
	secs -= tm->tm_hour * 3600;
	tm->tm_min = secs / 60;
	tm->tm_sec = secs - tm->tm_min * 60;
}

#if 0
/* Enable the interrupt */
static void _rtc_intr_en(uint32_t option)
{
	RTC_REGS->rtc_ctrl = option | (option << 16);
}
#endif
/* Enable the interrupt */
static void _rtc_intr_en(uint32_t option)
{
	uint32_t reg_val;

	reg_val = RTC_REGS->rtc_ctrl;
	reg_val |= option;
	RTC_REGS->rtc_ctrl = reg_val;
}

/* Disable the interrupt */
static void _rtc_intr_dis(uint32_t option)
{
	uint32_t reg_val;

	reg_val = RTC_REGS->rtc_ctrl;
	reg_val &= ~option;
	RTC_REGS->rtc_ctrl = reg_val;
}

/* Get current time */
static uint32_t _rtc_get_time(void)
{
	uint32_t timestamp;

	timestamp = RTC_REGS->rtc_timer;
	//printf("reg rtc_timer %d\n", timestamp);

	return timestamp;
}

/* Set the timestamp to trigger the interrupt */
static void _rtc_set_ontime(uint32_t ontime)
{
	RTC_REGS->rtc_ontime_set = ontime;
}

/* Set current time */
static void _rtc_update_clock(uint32_t timestamp)
{
	RTC_REGS->rtc_clock_set = timestamp;
	delay(1);
	//timestamp = RTC_REGS->rtc_clock_set;
	//printf("reg rtc_clock_set %d\n", RTC_REGS->rtc_clock_set);
}

/* Set periodic time */
static void _rtc_set_periodic(uint32_t timestamp)
{
	RTC_REGS->rtc_periodic_set = timestamp;
}

static uint8_t _rtc_int_status()
{
	return RTC_REGS->rtc_int_status ;
}

static void rtc_irq_handler(void)
{
	int irqn, status;

	irqn = __get_IPSR() - 16;

	//status = RTC_REGS->rtc_int_status;
	if (irqn == RTC_PERIODIC_IRQ)
		periodic_cb();
	else if (irqn == RTC_ALARM_IRQn)
		alarm_cb();
}

void HAL_RTC_Update(uint32_t sec)
{
	_rtc_update_clock(sec);
	//_rtc_intr_en(RTC_UPDATE_EN);
}

uint32_t HAL_RTC_Gettime(struct rtc_time *tm)
{
	uint32_t time;

	time = _rtc_get_time();
	//printf("\ntime sec %d\n", time);
	rtc_sec_to_tm(time, tm);

	//printf("\r\n%s:  RTC date/time to %d-%d-%d, %02d:%02d:%02d.\r\n",
	//	__func__, tm->tm_mday, tm->tm_mon + 1, tm->tm_year,
	//	tm->tm_hour, tm->tm_min, tm->tm_sec);
	return time;
}

void HAL_RTC_Periodic(uint32_t sec)
{
	_rtc_intr_dis(RTC_PERIODIC_EN);
	_rtc_set_periodic(sec);
	_rtc_intr_en(RTC_PERIODIC_EN);
}

void HAL_RTC_Periodic_Stop(void)
{
	_rtc_intr_dis(RTC_PERIODIC_EN);
}

void HAL_RTC_Alarm(uint32_t sec)
{
	_rtc_set_ontime(sec);
	_rtc_intr_en(RTC_ONTIME_EN);
}

void HAL_RTC_Init(uint32_t sec, IRQHandler_t cb_alarm, IRQHandler_t cb_periodic, IRQHandler_t cb_2hz)
{
	UNUSED(cb_2hz);
	//tm = malloc(sizeof(struct rtc_time));

	HAL_Module_Clock_enable(RTC, 1);
	HAL_Module_Clock_gate(RTC, 0);
	HAL_Module_Reset(RTC, 0);

	//99 periodic	RTC_PREIODIC_IRQ
	//141 sys rtc int	RTC_ALARM_IRQn
	if(cb_alarm != NULL) {
		alarm_cb = cb_alarm;
		IRQ_SetHandler(RTC_ALARM_IRQn, rtc_irq_handler);
		IRQ_Enable(RTC_ALARM_IRQn);
	} else if (cb_periodic != NULL) {
		periodic_cb = cb_periodic;
		IRQ_SetHandler(RTC_PERIODIC_IRQ, rtc_irq_handler);
		IRQ_Enable(RTC_PERIODIC_IRQ);
	}
	_rtc_update_clock(sec);
}

void HAL_RTC_DeInit(void)
{
	HAL_Module_Clock_enable(RTC, 0);
	HAL_Module_Clock_gate(RTC, 0);
	HAL_Module_Reset(RTC, 1);

	_rtc_intr_dis(RTC_PERIODIC_EN | RTC_UPDATE_EN | RTC_ONTIME_EN_PMC | RTC_ONTIME_EN);

	IRQ_Disable(RTC_ALARM_IRQn);
	IRQ_Disable(RTC_PERIODIC_IRQ);
}