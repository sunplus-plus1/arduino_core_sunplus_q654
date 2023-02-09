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
#include "Arduino.h"
#include "sunplus_ll.h"
#include "sunplus_def.h"
#ifdef SP645
#include "sp645_hal.h"
#elif defined(SP7350)
#include "sp7350_hal.h"
#endif


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

static void hal_tim_case_init0(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM0;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_init1(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM1;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_init2(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM2;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_init3(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM3;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_init4(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM4;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_init5(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM5;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_init6(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM6;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_init7(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM7;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_init8(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM7+2;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_NOT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_init9(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = NULL;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_NOT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_deinit0(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;

	eRet = HAL_TIM_DeInit(NULL);
	CU_ASSERT_NOT_EQUAL(eRet, HAL_OK);
}

static void hal_tim_case_deinit1(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	obj.handle.Instance = TIM7+1;
	eRet = HAL_TIM_DeInit(&(obj.handle));
	CU_ASSERT_NOT_EQUAL(eRet, HAL_OK);
}


static void CUINIT_TIM_IRQHandler(void){

}


static void hal_tim_case_Start0(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_Start(&(obj.handle));
	CU_ASSERT_EQUAL(eRet, HAL_OK);
	HAL_TIM_Stop(&(obj.handle));
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_Start1(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_Start(&(obj.handle));
	CU_ASSERT_EQUAL(((READ_REG(TIM0->control)) & (TIMER_GO))>>(TIMER_GO_Pos), 1);
	HAL_TIM_Stop(&(obj.handle));
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_Start2(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM7;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_Start(&(obj.handle));
	CU_ASSERT_EQUAL(((READ_REG(TIM7->control)) & (TIMER_GO))>>(TIMER_GO_Pos), 1);
	HAL_TIM_Stop(&(obj.handle));
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_Start3(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM2;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_Start(&(obj.handle));
	CU_ASSERT_NOT_EQUAL(((READ_REG(TIM6->control)) & (TIMER_GO))>>(TIMER_GO_Pos), 1);
	
	HAL_TIM_Stop(&(obj.handle));
	HAL_TIM_DeInit(&(obj.handle));
}


static void hal_tim_case_Stop0(void)
{
	
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_Start(&(obj.handle));CU_ASSERT_NOT_EQUAL(((READ_REG(TIM6->control)) & (TIMER_GO))>>(TIMER_GO_Pos), 1);
	HAL_TIM_Stop(&(obj.handle));
	CU_ASSERT_EQUAL(((READ_REG(TIM0->control)) & (TIMER_GO))>>(TIMER_GO_Pos), 0);
	HAL_TIM_DeInit(&(obj.handle));

}

static void hal_tim_case_Stop1(void)
{
	
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_Start(&(obj.handle));CU_ASSERT_NOT_EQUAL(((READ_REG(TIM6->control)) & (TIMER_GO))>>(TIMER_GO_Pos), 1);
	obj.handle.Instance = TIM7+2;
	eRet = HAL_TIM_Stop(&(obj.handle));
	CU_ASSERT_NOT_EQUAL(eRet, HAL_OK);
	obj.handle.Instance = TIM0;
	HAL_TIM_DeInit(&(obj.handle));

}

static void hal_tim_case_SetPrescaler0(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_SetPrescaler(&(obj.handle), 0xFFFF);
	CU_ASSERT_EQUAL(READ_REG(TIM0->prescale_val), 0xFFFF);
	HAL_TIM_DeInit(&(obj.handle));
}


static void hal_tim_case_SetPrescaler1(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_SetPrescaler(&(obj.handle), 0xFFFF);
	CU_ASSERT_NOT_EQUAL(READ_REG(TIM1->prescale_val), 0xFFFF);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_SetPrescaler2(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	obj.handle.Instance = TIM7+2;
	eRet = HAL_TIM_SetPrescaler(&(obj.handle), 0xFFFF);
	CU_ASSERT_NOT_EQUAL(eRet, HAL_OK);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_GetCount0(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_SetPrescaler(&(obj.handle), 0xFFFF);
	CU_ASSERT_EQUAL(HAL_TIM_GetCount(&(obj.handle)), 0xFFFF);
	HAL_TIM_DeInit(&(obj.handle));


}

static void hal_tim_case_GetCount1(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	eRet = HAL_TIM_SetPrescaler(&(obj.handle), 0xFFFF);
	obj.handle.Instance = TIM7+1;
	CU_ASSERT_NOT_EQUAL(HAL_TIM_GetCount(&(obj.handle)), 0xFFFF);
	HAL_TIM_DeInit(&(obj.handle));
}


static void hal_tim_case_GetCLKSrc0(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SYS_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(HAL_TIM_GetCLKSrc(&(obj.handle)), CLK_SYS_SRC);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_GetCLKSrc1(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_STC_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(HAL_TIM_GetCLKSrc(&(obj.handle)), CLK_STC_SRC);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_GetCLKSrc2(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_RTC_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(HAL_TIM_GetCLKSrc(&(obj.handle)), CLK_RTC_SRC);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_GetCLKSrc3(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_EXT_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(HAL_TIM_GetCLKSrc(&(obj.handle)), CLK_EXT_SRC);
	HAL_TIM_DeInit(&(obj.handle));
}


static void hal_tim_case_GetCLKSrc4(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SLAVE_TRIG_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	
	printf("%s, %x,%x, %d\n", __FUNCTION__, TIM0->control, READ_BIT(TIM0->control, TIMER_TRIG_SRC), HAL_TIM_GetCLKSrc(&(obj.handle)));
	CU_ASSERT_EQUAL(HAL_TIM_GetCLKSrc(&(obj.handle)), CLK_SLAVE_TRIG_SRC);
	HAL_TIM_DeInit(&(obj.handle));
}

static void hal_tim_case_GetCLKSrc5(void)
{
	HAL_StatusTypeDef eRet = HAL_TIMEOUT;
	timerObj_t obj;
	memset(&obj, 0, sizeof(timerObj_t));
	obj.handle.Instance = TIM0;
	//obj.handle.IrqHandle = CUINIT_TIM_IRQHandler;
	obj.handle.State = HAL_TIM_STATE_RESET;
	obj.handle.Init.Prescaler = 0;
	obj.handle.Init.Counter = 0xFFFF;
	obj.handle.Init.ReloadCounter = 0xFFFF;
	obj.handle.Init.ClockSource = CLK_SLAVE_WRAP_SRC;
	obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
	obj.pfcallback = NULL;
	eRet = HAL_TIM_Init(&(obj.handle));
	CU_ASSERT_EQUAL(HAL_TIM_GetCLKSrc(&(obj.handle)), CLK_SLAVE_WRAP_SRC);
	HAL_TIM_DeInit(&(obj.handle));
}



CU_TestInfo   timer_testcases[] =
{
    {"Test HAL_TIM_Init TIM0", hal_tim_case_init0},
    {"Test HAL_TIM_Init TIM1", hal_tim_case_init1},
    {"Test HAL_TIM_Init TIM2", hal_tim_case_init2},
    {"Test HAL_TIM_Init TIM3", hal_tim_case_init3},
    {"Test HAL_TIM_Init TIM4", hal_tim_case_init4},
    {"Test HAL_TIM_Init TIM5", hal_tim_case_init5},
    {"Test HAL_TIM_Init TIM6", hal_tim_case_init6},
    {"Test HAL_TIM_Init TIM7", hal_tim_case_init7},
    {"Test HAL_TIM_Init the Handle is error", hal_tim_case_init8},
    {"Test HAL_TIM_Init the Handle is NULL", hal_tim_case_init9},
    {"Test HAL_TIM_DeInit the Handle is NULL", hal_tim_case_deinit0},
    {"Test HAL_TIM_DeInit the Handle is TIM7+1", hal_tim_case_deinit1},
    {"Test HAL_TIM_Start TIM", hal_tim_case_Start0},
    {"Test HAL_TIM_Start TIM0", hal_tim_case_Start1},
    {"Test HAL_TIM_Start TIM7", hal_tim_case_Start2},
    {"Test HAL_TIM_Start TIM2 read TIM6", hal_tim_case_Start3},
    {"Test HAL_TIM_Stop TIM0", hal_tim_case_Stop0},
    {"Test HAL_TIM_Stop the handle is error", hal_tim_case_Stop1},
    {"Test HAL_TIM_SetPrescaler TIM0", hal_tim_case_SetPrescaler0},
    {"Test HAL_TIM_SetPrescaler TIM0, read TIM1", hal_tim_case_SetPrescaler1},
    {"Test HAL_TIM_SetPrescalerHandle is TIM7+1", hal_tim_case_SetPrescaler2},
    {"Test hal_tim_case_GetCount0 TIM0", hal_tim_case_GetCount0},
    {"Test hal_tim_case_GetCount0 TIM0, error handle TIM7+1", hal_tim_case_GetCount1},
    {"Test hal_tim_case_GetCLKSrc0 TIM0", hal_tim_case_GetCLKSrc0},
    {"Test hal_tim_case_GetCLKSrc1 TIM0", hal_tim_case_GetCLKSrc1},
    {"Test hal_tim_case_GetCLKSrc2 TIM0", hal_tim_case_GetCLKSrc2},
    {"Test hal_tim_case_GetCLKSrc3 TIM0", hal_tim_case_GetCLKSrc3},
    {"Test hal_tim_case_GetCLKSrc4 TIM0", hal_tim_case_GetCLKSrc4},
    {"Test hal_tim_case_GetCLKSrc5 TIM0", hal_tim_case_GetCLKSrc5},
    
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
