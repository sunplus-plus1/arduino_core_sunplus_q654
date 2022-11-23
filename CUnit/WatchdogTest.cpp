#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CUnit.h"
#include "sp645_hal_conf.h"

typedef enum {
	WDG0_INDEX,
	WDG1_INDEX,
	WDG2_INDEX,
	WDG_NUM,
	UNKNOWN_WDG = 0XFFFF
} watchdog_index_t;

#define TEST_TARGET	WDG1
#define WDG_FREQ	90000

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

	return index;
}

void callback(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);
	CU_PASS("Timeout and enter the callback\n");

	/* reset if not stop counter */
	HAL_WDG_Stop(&hwdg[index]);
	HAL_WDG_IRQHandler(&hwdg[index]);
}

static void test_case_GetTimeout(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);

	uint32_t actual, expected = 0xfffff * 1000 / WDG_FREQ;//ms

	/* init */
	hwdg[index].Instance = TEST_TARGET;
	hwdg[index].StcFreq = WDG_FREQ;
	hwdg[index].IrqMode = 0;
	hwdg[index].IrqHandle = NULL;

	HAL_WDG_Stop(&hwdg[index]);
	actual = HAL_WDG_GetTimeout(&hwdg[index]);
	CU_ASSERT_EQUAL(actual, expected);

	hwdg[index].Instance = NULL;
	HAL_WDG_GetTimeout(&hwdg[index]);
	CU_PASS("Para Instance");

	hwdg[index].StcFreq = 0;
	HAL_WDG_GetTimeout(&hwdg[index]);
	CU_PASS("Para StcFreq");
}

static void test_case_SetTimeout(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);

	uint32_t input = 2000, output;

	/* Init */
	hwdg[index].Instance = TEST_TARGET;
	hwdg[index].StcFreq = WDG_FREQ;
	hwdg[index].IrqMode = 0;
	hwdg[index].IrqHandle = NULL;

	/* Normal flow */
	HAL_WDG_SetTimeout(&hwdg[index], input);
	output = HAL_WDG_GetTimeout(&hwdg[index]);;
	CU_ASSERT_EQUAL(input, output);

	/* Struct varible / Parameter */
	hwdg[index].Instance = NULL;
	HAL_WDG_SetTimeout(&hwdg[index], input);
	CU_PASS("Struct varible \Instance = NULL");

	hwdg[index].StcFreq = 0;
	HAL_WDG_SetTimeout(&hwdg[index], input);
	CU_PASS("Struct varible \StcFreq = 0");

	input = 0xffffffff;
	HAL_WDG_SetTimeout(&hwdg[index], input);
	CU_PASS("Parameter \input = 0xffffffff");
}

static void test_case_Reload(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);

	/* Init */
	hwdg[index].Instance = TEST_TARGET;
	hwdg[index].StcFreq = WDG_FREQ;
	hwdg[index].IrqMode = 0;
	hwdg[index].IrqHandle = NULL;
#if 0
	HAL_WDG_Stop(&hwdg[index]);
	HAL_WDG_SetTimeout(&hwdg[index], 0);
	HAL_WDG_Start(&hwdg[index]);
#endif
}

static void test_case_FeedDog(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);

	uint32_t timeout = 1000;//ms
	int i;

	/* Init */
	hwdg[index].Instance = TEST_TARGET;
	hwdg[index].StcFreq = WDG_FREQ;
	hwdg[index].IrqMode = 0;
	hwdg[index].IrqHandle = NULL;

	HAL_WDG_Init(&hwdg[index]);
	HAL_WDG_SetTimeout(&hwdg[index], timeout);
	HAL_WDG_Start(&hwdg[index]);

	for(i=0; i++; i<10)
	{
		HAL_Delay(500000);//us
		HAL_WDG_SetTimeout(&hwdg[index], timeout);
		CU_PASS("Feed dog");
	}
	HAL_WDG_Stop(&hwdg[index]);
}

static void test_case_IrqMode(void)
{
	watchdog_index_t index = get_watchdog_index(TEST_TARGET);

	uint32_t timeout = 1000;//ms
	int i;

	/* Init */
	hwdg[index].Instance = TEST_TARGET;
	hwdg[index].StcFreq = WDG_FREQ;
	hwdg[index].IrqMode = 1;
	hwdg[index].IrqHandle = callback;

	HAL_WDG_Init(&hwdg[index]);
	HAL_WDG_SetTimeout(&hwdg[index], timeout);
	HAL_WDG_Start(&hwdg[index]);
}

CU_TestInfo watchdog_testcases[] =
{
	{"test_case_GetTimeout", test_case_GetTimeout},
	{"test_case_SetTimeout", test_case_SetTimeout},
	{"test_case_FeedDog", test_case_FeedDog},
	{"test_case_IrqMode", test_case_IrqMode},
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

