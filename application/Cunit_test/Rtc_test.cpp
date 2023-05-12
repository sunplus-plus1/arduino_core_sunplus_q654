/* FIXME: test_case_Periodic(): sec_trg > new_sec_start, always generate intrrupt */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Arduino.h"
#include "CUnit.h"
#ifdef SP645
#include "sp645_hal.h"
#elif defined(SP7350)
#include "sp7350_hal.h"
#endif

#define STAMP(v)                                   \
	do {                                       \
		*(volatile uint32_t *)0xf8000000UL = v; \
	} while (0)

uint32_t sec_start = 0;
volatile int flag_p = 0;
volatile int flag_a = 0;
volatile int flag_r = 0;

void irq_handler_alarm(void)
{
	//CU_PASS("alarm irq ack\n");
	printf(" alarm irq ack ");
	flag_a = 1;
}

void irq_handler_periodic(void)
{
	//CU_PASS("periodic irq ack\n");
	printf(" periodic irq ack ");
	flag_p = 1;
}

void irq_handler_rtc2hz(void)
{
	//CU_PASS("alarm irq ack\n");
	printf(" rtc2hz irq ack ");
	flag_r ++;
}

static void test_case_Gettime(void)
{
	uint32_t time;
	uint32_t actual_secs, per_secs = 1;//read time every 2 second
	int i;
	struct rtc_time *tm = (struct rtc_time *)malloc(sizeof(struct rtc_time));

	HAL_RTC_Init(sec_start, NULL, NULL, NULL);

	for(i = 0; i < 1; i++) {
		actual_secs += per_secs;
		delay(per_secs * 1000); //unit ms
		time = HAL_RTC_Gettime(tm);

		//printf("\r\n%s:  RTC date/time to %d/%d/%d, %02d:%02d:%02d.\r\n",
		//       __func__, tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
		//       tm->tm_hour, tm->tm_min, tm->tm_sec);
		CU_ASSERT_EQUAL(time, actual_secs);
		CU_ASSERT_EQUAL(tm->tm_sec, actual_secs);
	}
	free(tm);
}

static void test_case_Update(void)
{
	uint32_t time;
	uint32_t new_sec_start = 3600;//1 hour
	struct rtc_time *tm = (struct rtc_time *)malloc(sizeof(struct rtc_time));

	HAL_RTC_Init(sec_start, NULL, NULL, NULL);

	HAL_RTC_Update(new_sec_start);

	time = HAL_RTC_Gettime(tm);

	//printf("\r\n%s:  RTC date/time to %d/%d/%d, %02d:%02d:%02d.\r\n",
		// __func__, tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
		 //tm->tm_hour, tm->tm_min, tm->tm_sec);

	CU_ASSERT_EQUAL(time, 3600);
	CU_ASSERT_EQUAL(tm->tm_hour, 1);//1 hour
	free(tm);
}

static void test_case_Alarm(void)
{
	uint32_t new_sec_start = 3600;
	uint32_t sec_trg = 3601;

	HAL_RTC_Init(sec_start, irq_handler_alarm, NULL, NULL);
	HAL_RTC_Update(new_sec_start);
	HAL_RTC_Alarm(sec_trg);
	while(flag_a < 1);
	flag_a = 0;
}

/* FIXME: sec_trg > new_sec_start, always generate intrrupt */
static void test_case_Periodic(void)
{
	struct rtc_time *tm = (struct rtc_time *)malloc(sizeof(struct rtc_time));
	uint32_t time = 0;
	uint32_t new_sec_start = 2;
	uint32_t sec_trg = 32000;

	HAL_RTC_Init(sec_start, NULL, irq_handler_periodic, NULL);
	HAL_RTC_Update(new_sec_start);
	HAL_RTC_Periodic(sec_trg);

	while(flag_p < 1) {
	}
	flag_p = 0;
	HAL_RTC_Periodic_Stop();

	free(tm);
}
#ifdef SP7350
static void test_case_Rtc2hz(void)
{
	HAL_RTC_Init(sec_start, NULL, NULL, irq_handler_rtc2hz);
	while(flag_r < 5);
	flag_r = 0;
	HAL_RTC_DeInit();
}
#endif
CU_TestInfo rtc_testcases[] =
{
	{"test_case_Gettime", test_case_Gettime},
	{"test_case_Update", test_case_Update},
	{"test_case_Alarm", test_case_Alarm},
	{"test_case_Periodic", test_case_Periodic},
#ifdef SP7350
	{"test_case_Rtc2hz", test_case_Rtc2hz},
#endif
	CU_TEST_INFO_NULL
};

int suite_success_rtc_init(void)
{
	printf("%s %s", __DATE__, __TIME__);
	return  0;
}

int suite_success_rtc_clean(void)
{
	return  0;
}

static CU_SuiteInfo suites[] =
{
	{"Testing the RTC", suite_success_rtc_init, suite_success_rtc_clean, NULL, NULL, rtc_testcases},
	CU_SUITE_INFO_NULL
};

int Add_Rtc_Tests(void)
{
	assert(NULL != CU_get_registry());
	assert(!CU_is_test_running());
	if(CUE_SUCCESS != CU_register_suites(suites))  //注册测试suite
	{
		fprintf(stderr, "Register suites failed - %s ", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}

	return 0;
}

