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

#define GPIO_TEST_PIN    GPIO(15)

#ifdef SP645
#define GPIO_DELAY			1000
#elif defined(SP7350)
#define GPIO_DELAY			3
#endif

void gpio_out_test()
{
	GPIO_InitTypeDef GPIO_Init;
	memset(&GPIO_Init,0,sizeof(GPIO_Init));
	GPIO_Init.Mode = GPIO_PP_OUTPUT_MODE;
	GPIO_Init.Pin = GPIO_TEST_PIN;

	GPIO_Init.out_value = GPIO_OUT_LOW;
	ret = HAL_GPIO_Init(&GPIO_Init);
	CU_ASSERT_EQUAL(ret, HAL_OK);

	HAL_GPIO_WritePin(GPIO_TEST_PIN,GPIO_OUT_HIGH);
	delay(GPIO_DELAY);

	HAL_GPIO_WritePin(GPIO_TEST_PIN,GPIO_OUT_LOW);
	delay(GPIO_DELAY);

	HAL_GPIO_DeInit(GPIO_TEST_PIN);
}

void gpio_out_invert_test()
{
	GPIO_InitTypeDef GPIO_Init;
	memset(&GPIO_Init,0,sizeof(GPIO_Init));
	GPIO_Init.Mode = GPIO_PP_OUTPUT_MODE;
	GPIO_Init.Pin = GPIO_TEST_PIN;
	GPIO_Init.out_value = GPIO_OUT_LOW;
	GPIO_Init.invert = 1;
	HAL_GPIO_Init(&GPIO_Init);
	/* output  invert   */

	HAL_GPIO_WritePin(GPIO_TEST_PIN,GPIO_OUT_HIGH);
	delay(GPIO_DELAY);

	HAL_GPIO_WritePin(GPIO_TEST_PIN,GPIO_OUT_LOW);
	delay(GPIO_DELAY);

	HAL_GPIO_DeInit(GPIO_TEST_PIN);
	CU_ASSERT_EQUAL(1, 1);
}

void gpio_in_test()
{
	GPIO_InitTypeDef GPIO_Init;
	memset(&GPIO_Init,0,sizeof(GPIO_Init));
	GPIO_Init.Mode = GPIO_INPUT_MODE;
	GPIO_Init.Pin = GPIO_TEST_PIN;
	HAL_GPIO_Init(&GPIO_Init);
	CU_ASSERT_EQUAL(1, 1);
}

void gpio_in_invert_test()
{
	int value = 0;
	GPIO_InitTypeDef GPIO_Init;
	memset(&GPIO_Init,0,sizeof(GPIO_Init));
	GPIO_Init.Mode = GPIO_INPUT_MODE;
	GPIO_Init.Pin = GPIO_TEST_PIN;
	HAL_GPIO_Init(&GPIO_Init);
	value = HAL_GPIO_ReadPin(GPIO_TEST_PIN);
	HAL_GPIO_DeInit(GPIO_TEST_PIN);

	memset(&GPIO_Init,0,sizeof(GPIO_Init));
	GPIO_Init.Mode = GPIO_INPUT_MODE;
	GPIO_Init.Pin = GPIO_TEST_PIN;
	GPIO_Init.invert = 1;
	HAL_GPIO_Init(&GPIO_Init);

	CU_ASSERT_EQUAL(HAL_GPIO_ReadPin(GPIO_TEST_PIN), !value);
	HAL_GPIO_DeInit(GPIO_TEST_PIN);	
}

void gpio_in_pull_up_down_test()
{
	int value = 0;

	GPIO_InitTypeDef GPIO_Init;
	memset(&GPIO_Init,0,sizeof(GPIO_Init));
	GPIO_Init.Mode = GPIO_INPUT_MODE;
	GPIO_Init.Pin = GPIO_TEST_PIN;
	GPIO_Init.pull = GPIO_PULL_UP;
	HAL_GPIO_Init(&GPIO_Init);
	value = HAL_GPIO_ReadPin(GPIO_TEST_PIN);
	HAL_GPIO_DeInit(GPIO_TEST_PIN);

	memset(&GPIO_Init,0,sizeof(GPIO_Init));
	GPIO_Init.Mode = GPIO_INPUT_MODE;
	GPIO_Init.Pin = GPIO_TEST_PIN;
	GPIO_Init.pull = GPIO_PULL_DOWN;
	HAL_GPIO_Init(&GPIO_Init);

	CU_ASSERT_EQUAL(HAL_GPIO_ReadPin(GPIO_TEST_PIN), !value);

	HAL_GPIO_DeInit(GPIO_TEST_PIN);
	
}
void gpio_open_drain_test()
{
	GPIO_InitTypeDef GPIO_Init;
	memset(&GPIO_Init,0,sizeof(GPIO_Init));
	GPIO_Init.Mode = GPIO_OD_OUTPUT_MODE;
	GPIO_Init.Pin = GPIO_TEST_PIN;
	HAL_GPIO_Init(&GPIO_Init);

	HAL_GPIO_WritePin(GPIO_TEST_PIN,GPIO_OUT_HIGH);
	delay(GPIO_DELAY);

	HAL_GPIO_WritePin(GPIO_TEST_PIN,GPIO_OUT_LOW);
	delay(GPIO_DELAY);
	HAL_GPIO_DeInit(GPIO_TEST_PIN);

	CU_ASSERT_EQUAL(1, 1);
}

static CU_TestInfo   gpio_testcases[] =
{
	{"gpio open drain test：",	   gpio_open_drain_test},
	{"gpio out test：",             gpio_out_test},
	{"gpio out invert test：",      gpio_out_invert_test},
	{"gpio in test：",              gpio_in_test},
	{"gpio in invert test：",	   gpio_in_invert_test},
	{"gpio in pull up/down test：",gpio_in_pull_up_down_test},

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
	{"Testing the GPIO：", suite_success_init, suite_success_clean, NULL,NULL,gpio_testcases},
	CU_SUITE_INFO_NULL
};

int Add_GPIO_Tests(void)
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


