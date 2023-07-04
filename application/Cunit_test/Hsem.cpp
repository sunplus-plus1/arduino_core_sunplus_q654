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

static void hsem_IP_test(void)
{
	HAL_StatusTypeDef ret;
	printf("\n");
	for(int i = 0; i < 16;i++)
	{
		printf("first read lock[%d]=%d \n",i,HSEM_REG_AO->lock[i]);
		printf("second read lock[%d]=%d \n",i,HSEM_REG_AO->lock[i]);
		HSEM_REG_AO->lock[i] = 0x554E4C4B;
		printf("after write, third read lock[%d]=%d \n",i,HSEM_REG_AO->lock[i]);
		printf("\n");
	}
	HAL_Hsem_Release_All();
}

static void hsem_read_test(void)
{
	HAL_StatusTypeDef ret;
	int value;
	for(int i = 0; i < 16;i++)
	{
		ret = HAL_Hsem_Take(i);            // first read, return HAL_OK
		CU_ASSERT_EQUAL(ret, HAL_OK);
		value = HAL_HSEM_IsSemTaken(i);      //hsem have taken
		CU_ASSERT_EQUAL(value, 1);
		ret = HAL_Hsem_Take(i);            //read again, return HAL_ERROR
		CU_ASSERT_EQUAL(ret, HAL_ERROR);
	}
	
	HAL_Hsem_Release_All();
}

static void hsem_write_test(void)
{
	HAL_StatusTypeDef ret;
	for(int i = 0; i < 16;i++)
	{
		ret = HAL_Hsem_Take(i);             // first read, return HAL_OK
		CU_ASSERT_EQUAL(ret, HAL_OK);
		ret = HAL_Hsem_Take(i);             //read again, return HAL_ERROR
		CU_ASSERT_EQUAL(ret, HAL_ERROR);
		HAL_Hsem_Release(i);                //write magic to release lock
		ret = HAL_Hsem_Take(i);             // read again, return HAL_OK
		CU_ASSERT_EQUAL(ret, HAL_OK);
	}
}

static CU_TestInfo   hsem_testcases[] =
{
	{"HSEM IP test：",    hsem_IP_test},
	{"HSEM read test：",    hsem_read_test},
	{"HSEM write test：",    hsem_write_test},
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
	{"Testing the Hsem", suite_success_init, suite_success_clean, NULL,NULL,hsem_testcases},
	CU_SUITE_INFO_NULL
};

int Add_Hsem_Tests(void)
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


