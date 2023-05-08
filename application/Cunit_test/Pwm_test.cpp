/*
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2001		Anil Kumar
 *  Copyright (C) 2004, 2005  Anil Kumar, Jerry St.Clair
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Arduino.h"
#include "CUnit.h"

#ifdef SP645
#include "sp645_hal_conf.h"
#elif defined(SP7350)
#include "sp7350_hal_conf.h"
#endif

static HAL_StatusTypeDef ret;

void _pwm_set(int pwm)
{
	ret = pwm_init(pwm,500000000,200000000,1,PWM_RESOLUTION_0_4096);

	CU_ASSERT_EQUAL(ret, HAL_OK);
	
	pwm_start(pwm);
	delay(3000);
	pwm_stop(pwm);
	delay(3000);
	ret = pwm_set_period(pwm,80000,40000);

	CU_ASSERT_EQUAL(ret, HAL_OK);

	pwm_start(pwm);
	delay(3000);
	pwm_stop(pwm);
}

void pwm0_test()
{
	_pwm_set(PWM0);
}

void pwm1_test()
{
	_pwm_set(PWM1);
}

void pwm2_test()
{
	_pwm_set(PWM2);
}

void pwm3_test()
{
	_pwm_set(PWM3);
}

void pwm_Init_test()
{
	ret = pwm_init(PWM0,20000000,10000000,1,PWM_RESOLUTION_0_4096);
	CU_ASSERT_EQUAL(ret, HAL_OK);


	ret = pwm_init(PWM0,0,10000000,1,PWM_RESOLUTION_0_4096);
	CU_ASSERT_EQUAL(ret, HAL_ERROR);
	
	ret = pwm_init(PWM0,10000000,20000000,1,PWM_RESOLUTION_0_4096);
	CU_ASSERT_EQUAL(ret, HAL_ERROR);

#ifdef SP7350
	/* pinmux must set to 1/2 */
	ret = pwm_init(PWM0,10000000,20000000,3,PWM_RESOLUTION_0_4096);
	CU_ASSERT_EQUAL(ret, HAL_ERROR);

	ret = pwm_init(PWM0,10000000,20000000,0,PWM_RESOLUTION_0_4096);
	CU_ASSERT_EQUAL(ret, HAL_ERROR);
#endif

}

void pwm_Start_test()
{
	pwm_start(PWM0);
	CU_ASSERT_EQUAL((PWM_REG->pwm_en) & (1<<PWM0), (1<<PWM0));
}

void pwm_Stop_test()
{
	pwm_stop(PWM0);
	CU_ASSERT_EQUAL((PWM_REG->pwm_en) & (1<<PWM0), (0<<PWM0));
}

void pwm_Set_test()
{
	ret = pwm_set_period(PWM0,0,40000);
	CU_ASSERT_EQUAL(ret, HAL_ERROR);


	ret = pwm_set_period(PWM0,40000,80000);
	CU_ASSERT_EQUAL(ret, HAL_ERROR);
}

static CU_TestInfo   pwm_testcases[] =
{
	{"PWM0 test：",     pwm0_test},
	{"PWM1 test：",     pwm1_test},
	{"PWM2 test：",	   pwm2_test},
	{"PWM3 test：",	   pwm3_test},

	{"PWM Init test：",     pwm_Init_test},
	{"PWM Start test：",    pwm_Start_test},
	{"PWM Stop test：",	   pwm_Stop_test},
	{"PWM Set test：",	   pwm_Set_test},
	CU_TEST_INFO_NULL
};

static int  suite_success_init(void)
{
	return  0;
}

static int  suite_success_clean(void)
{
	return  0;
}
 
static CU_SuiteInfo suites[] =
{
	{"Testing the PWM：", suite_success_init, suite_success_clean, NULL,NULL,pwm_testcases},
	CU_SUITE_INFO_NULL
};

int Add_Pwm_Tests(void)
{

	assert(NULL != CU_get_registry());
	assert(!CU_is_test_running());
	if(CUE_SUCCESS  != CU_register_suites(suites))
	{
		printf("Register suites failed - %s ", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}

	return 0;
}


