#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CUnit.h"
#ifdef SP645
#include "sp645_hal.h"
#elif defined(SP7350)
#include "sp7350_hal.h"
#endif

/*
 * NOTE to develop SP7350:
 * 1.STC Group of CM4:
 *	 ---------------------------------------------------------------------------------------
 *	|	|STC	 	|STC_AV0	|STC_AV1	|STC_AV2	|STC_AV4	|
 *	|-------|---------------|---------------|---------------|---------------|---------------|
 *	|STC	|STC0		|STC1	  	|STC2	   	|STC3		|STC4		|
 *	|-------|---------------|---------------|---------------|---------------|---------------|
 *	|TIM	|TIM0/1/2	|TIM3/4/5	|TIM6/7/8	|TIM9/10/11	|TIM12/13/14	|
 *	|-------|---------------|---------------|---------------|---------------|---------------|
 *	|WDG	|WDG0		|WDG1		|WDG2		|	-	|WDG3		|
 *	 ---------------------------------------------------------------------------------------
 * 2.only use timer 0/1/2 of every STC
 *	e.x. TIM0 is timer0 of STC
 *	TIM1 is timer1 of STC
 *	TIM2 is timer2 of STC
 *	TIM3 is timer0 of STC_AV0 and so on....
 *
 * 3.Real chip test need modify the count of ticks.
 */


#define STAMP(v)                                   \
	do {                                       \
		*(volatile uint32_t *)0xf8000000UL = v; \
	} while (0)

typedef enum {
	WDG0_INDEX,
	WDG1_INDEX,
	WDG2_INDEX,
	WDG_NUM,
	UNKNOWN_WDG = 0XFFFF
} watchdog_index_t;

#define TEST_TARGET	WDG0
#define WDG_FREQ	1000000

WDG_HandleTypeDef hwdg[WDG_NUM];

static watchdog_index_t get_watchdog_index(WDG_TypeDef *instance)
{
	watchdog_index_t index = UNKNOWN_WDG;

	if (instance == WDG0) {
		index = WDG0_INDEX;
	}
	else if (instance == WDG1) {
		index = WDG1_INDEX;
	}
	else if (instance == WDG2) {
		index = WDG2_INDEX;
	}
#ifdef SP7350
	else if (instance == WDG3) {
		index = WDG2_INDEX;
	}
#endif
	return index;
}

static int occur_interrupt = 0;
void callback(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);

	occur_interrupt++;
	CU_PASS("cb\n");

	/* reset if not stop counter */
	HAL_WDG_IRQHandler(&hwdg[index]);
}

static void test_case_GetTimeout(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);

	uint32_t actual, expected;

	HAL_InitCommonSTC(STC0, 1000000);
	expected = 0xffffffff;//max ticks (us)

	/* init */
	hwdg[index].Instance = TEST_TARGET;
	hwdg[index].IrqMode = WDG_RST;
	hwdg[index].IrqHandle = NULL;

	HAL_WDG_Stop(&hwdg[index]);
	WRITE_REG(hwdg[index].Instance->control, WDG_CMD_CNTMAX);
	actual = HAL_WDG_GetTimeout(&hwdg[index]);
	CU_ASSERT_EQUAL(actual, expected);

	/* Test whether function return correctly if paramter is wrong */
	hwdg[index].Instance = NULL;
	HAL_WDG_GetTimeout(&hwdg[index]);
	CU_PASS("Struct varible \Instance = NULL");
}

static void test_case_SetTimeout(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);

	uint32_t input, output;

	HAL_InitCommonSTC(STC0, 1000000);
	input = 2000000;//2000000 ticks(us)

	/* Init */
	hwdg[index].Instance = TEST_TARGET;
	hwdg[index].IrqMode = WDG_RST;
	hwdg[index].IrqHandle = NULL;

	/* Normal flow */
	HAL_WDG_SetTimeout(&hwdg[index], input);
	output = HAL_WDG_GetTimeout(&hwdg[index]);
	CU_ASSERT_EQUAL(input, output);

	/* Struct varible / Parameter */
	hwdg[index].Instance = NULL;
	HAL_WDG_SetTimeout(&hwdg[index], input);
	CU_PASS("Struct varible \Instance = NULL");
}

static void test_case_FeedDog(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);

	uint32_t timeout = 4000000;//ticks = us
	uint32_t feedog = 2000000;
	uint32_t fdloop = 5;
	int i;

	HAL_InitCommonSTC(STC0, 1000000);
	/* Init */
	hwdg[index].Instance = TEST_TARGET;
#ifdef SP7350
	hwdg[index].IrqMode = WDG_INTR_RST;
	hwdg[index].IrqHandle = callback;
	hwdg[index].IrqTicks = 2500000;//pretimeout
#else
	hwdg[index].IrqMode = WDG_RST;
	hwdg[index].IrqHandle = NULL;
#endif
	HAL_WDG_Init(&hwdg[index]);
	HAL_WDG_SetTimeout(&hwdg[index], timeout);
	HAL_WDG_Start(&hwdg[index]);

	for(i = 0; i < fdloop; i++)
	{
		HAL_Delay(feedog);//us
		HAL_WDG_SetTimeout(&hwdg[index], timeout);
		//printf("fd\n");
		CU_PASS("Feed dog");
	}

	printf("occur_interrupt %d\n", occur_interrupt);
	CU_ASSERT_EQUAL(occur_interrupt, fdloop);
	occur_interrupt = 0;

	HAL_WDG_Stop(&hwdg[index]);
}

CU_TestInfo watchdog_testcases[] =
{
	{"test_case_GetTimeout", test_case_GetTimeout},
	{"test_case_SetTimeout", test_case_SetTimeout},
	{"test_case_FeedDog", test_case_FeedDog},
	CU_TEST_INFO_NULL
};

int suite_success_wdg_init(void)
{
	return  0;
}

int suite_success_wdg_clean(void)
{
	return  0;
}

static CU_SuiteInfo suites[] =
{
	{"Testing the Watchdog", suite_success_wdg_init, suite_success_wdg_clean, NULL, NULL, watchdog_testcases},
	CU_SUITE_INFO_NULL
};

int Add_Watchdog_Tests(void)
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
