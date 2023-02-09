#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cunit_main.h"
#include "Basic.h"


static void test_case1(void)
{
	CU_ASSERT(1);
}
static void test_case2(void)
{
	CU_ASSERT_EQUAL(1, 2);
}
static void test_case3(void)
{
	CU_ASSERT_STRING_EQUAL("abc", "edf");
}

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */

int main(void)
{

	CU_ErrorAction error_action = CUEA_IGNORE;
	CU_pSuite pSuite;
	int test_return = 0;

	if (CU_initialize_registry()) {
		printf("\nInitialization of Test Registry failed.");
		return -1;
	}
	#if 0

	pSuite = CU_add_suite("suite_for_test", NULL, NULL);
	if (NULL == pSuite) {
		return -1;
	}

	CU_add_test(pSuite, "test_case1", test_case1);
	CU_add_test(pSuite, "test_case2", test_case2);
	CU_add_test(pSuite, "test_case3", test_case3);
	#else

	//Add_Timer_Tests();
	//Add_Watchdog_Tests();
	Add_Uart_Tests();
	Add_Pwm_Tests();
	#endif

	CU_basic_set_mode(CU_BRM_VERBOSE);

	CU_set_error_action(error_action);

	test_return = CU_basic_run_tests();
	printf("\nTests completed with return value %d.\n", test_return);

	CU_cleanup_registry();

	return 0;
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
