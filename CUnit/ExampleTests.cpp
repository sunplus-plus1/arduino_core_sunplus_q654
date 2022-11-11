/*
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2001        Anil Kumar
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

#include "CUnit.h"
#include "sp645_hal_conf.h"



static void test_case_repeat(void) 
{ 
	CU_ASSERT_DOUBLE_NOT_EQUAL(5,1,2);  //5 - 1 > 2
}
static void test_case_on_shot(void) 
{ 
	CU_ASSERT_EQUAL(1,1); 
}

static void test_case_interrupt(void) 
{ 

	CU_ASSERT_EQUAL(2,0); 
	

	CU_ASSERT_DOUBLE_EQUAL(4,3,2);  //4 - 3 < 2
	
}

CU_TestInfo   timer_testcases[] =
{
    {"test_case_repeat：", test_case_repeat},
    {"test_case_on_shot：", test_case_on_shot},
	{"test_case_interrupt：", test_case_interrupt},
    CU_TEST_INFO_NULL
};
  
int  suite_success_init(void)
{
    return  0;
}

int  suite_success_clean(void)
{
    return  0;
}
 
static CU_SuiteInfo suites[] = 
{
    {"Testing the Timer：", suite_success_init, suite_success_clean, NULL,NULL,timer_testcases},
    CU_SUITE_INFO_NULL
};

int Add_Timer_Tests(void)
{
	assert(NULL != CU_get_registry());
    assert(!CU_is_test_running());
    if(CUE_SUCCESS  != CU_register_suites(suites))  //注册测试suite
    {
 
        fprintf(stderr, "Register suites failed - %s ", CU_get_error_msg());
        exit(EXIT_FAILURE);
    }

	return 0;
}
