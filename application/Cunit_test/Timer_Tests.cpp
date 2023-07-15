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
#if 0
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

#ifndef SP7350
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
#endif
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
#ifndef SP7350
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

	//printf("%s, %x,%x, %d\n", __FUNCTION__, TIM0->control, READ_BIT(TIM0->control, TIMER_TRIG_SRC), HAL_TIM_GetCLKSrc(&(obj.handle)));
	CU_ASSERT_EQUAL(HAL_TIM_GetCLKSrc(&(obj.handle)), CLK_SLAVE_TRIG_SRC);
	HAL_TIM_DeInit(&(obj.handle));
}
#endif
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
#ifndef SP7350
    {"Test HAL_TIM_SetPrescaler TIM0", hal_tim_case_SetPrescaler0},
    {"Test HAL_TIM_SetPrescaler TIM0, read TIM1", hal_tim_case_SetPrescaler1},
    {"Test HAL_TIM_SetPrescalerHandle is TIM7+1", hal_tim_case_SetPrescaler2},
#endif
    {"Test hal_tim_case_GetCount0 TIM0", hal_tim_case_GetCount0},
    {"Test hal_tim_case_GetCount0 TIM0, error handle TIM7+1", hal_tim_case_GetCount1},
    {"Test hal_tim_case_GetCLKSrc0 TIM0", hal_tim_case_GetCLKSrc0},
    {"Test hal_tim_case_GetCLKSrc1 TIM0", hal_tim_case_GetCLKSrc1},
    {"Test hal_tim_case_GetCLKSrc2 TIM0", hal_tim_case_GetCLKSrc2},
#ifndef SP7350
    {"Test hal_tim_case_GetCLKSrc3 TIM0", hal_tim_case_GetCLKSrc3},
    {"Test hal_tim_case_GetCLKSrc4 TIM0", hal_tim_case_GetCLKSrc4},
#endif
    {"Test hal_tim_case_GetCLKSrc5 TIM0", hal_tim_case_GetCLKSrc5},

    CU_TEST_INFO_NULL
};
#else

/* Private Variables */
const static struct tim_hal_info sp_tim_info[] = {
	TIM_HAL_INFO(TIM0, TIM0_INDEX, STC_0, STC_TIMER0_IRQn),
	TIM_HAL_INFO(TIM1, TIM1_INDEX, STC_0, STC_TIMER1_IRQn),
	TIM_HAL_INFO(TIM2, TIM2_INDEX, STC_0, STC_TIMER2_IRQn),
	TIM_HAL_INFO(TIM3, TIM3_INDEX, STC_AV0, STC_AV0_TIMER0_IRQn),
	TIM_HAL_INFO(TIM4, TIM4_INDEX, STC_AV0, STC_AV0_TIMER1_IRQn),
	TIM_HAL_INFO(TIM5, TIM5_INDEX, STC_AV0, STC_AV0_TIMER2_IRQn),
	TIM_HAL_INFO(TIM6, TIM6_INDEX, STC_AV1, STC_AV1_TIMER0_IRQn),
	TIM_HAL_INFO(TIM7, TIM7_INDEX, STC_AV1, STC_AV1_TIMER1_IRQn),
	TIM_HAL_INFO(TIM8, TIM8_INDEX, STC_AV1, STC_AV1_TIMER2_IRQn),
	TIM_HAL_INFO(TIM9, TIM9_INDEX, STC_AV2, STC_AV2_TIMER0_IRQn),
	TIM_HAL_INFO(TIM10, TIM10_INDEX, STC_AV2, STC_AV2_TIMER1_IRQn),
	TIM_HAL_INFO(TIM11, TIM11_INDEX, STC_AV2, STC_AV2_TIMER2_IRQn),
	TIM_HAL_INFO(TIM12, TIM12_INDEX, STC_AV4, STC_AV4_TIMER0_IRQn),
	TIM_HAL_INFO(TIM13, TIM13_INDEX, STC_AV4, STC_AV4_TIMER1_IRQn),
	TIM_HAL_INFO(TIM14, TIM14_INDEX, STC_AV4, STC_AV4_TIMER2_IRQn),
};

const static struct tim64_hal_info sp_tim64_info[] = {
	TIM_HAL_INFO(TIM15, TIM15_INDEX, STC_0, STC_TIMER3_IRQn),
	TIM_HAL_INFO(TIM16, TIM16_INDEX, STC_AV0, STC_AV0_TIMER3_IRQn),
	TIM_HAL_INFO(TIM17, TIM17_INDEX, STC_AV1, STC_AV1_TIMER3_IRQn),
	TIM_HAL_INFO(TIM18, TIM18_INDEX, STC_AV2, STC_AV2_TIMER3_IRQn),
	TIM_HAL_INFO(TIM19, TIM19_INDEX, STC_AV4, STC_AV4_TIMER3_IRQn),
};

timerObj_t obj;
static int occur_interrupt = 0;
//HAL_StatusTypeDef eRet;

static uint32_t us_to_cnt(uint32_t freq, uint32_t us)
{
	return (freq / 1000000 * us);
}
static uint32_t ms_to_cnt(uint32_t freq, uint32_t ms)
{
	return (freq / 1000 * ms);
}

static void test_irq_handler(void)
{
	//_PASS("Entry irq\n");
	occur_interrupt ++;
	//if(IS_TIM64_INSTANCE(obj.handle.Instance64))
		//printf("%x\n", (uint32_t)obj.handle.Instance64);
	//else
		//printf("%x\n", (uint32_t)obj.handle.Instance);
}

static void hal_tim_case_32bit_counter(void)
{
	timerObj_t obj;
	HAL_StatusTypeDef eRet;
	uint32_t actual, expected, error;
	uint32_t freq = HAL_PLL_GetSystemFreq();
	uint32_t cnt_s = 0xFFFFFFFF; //start
	uint32_t us_delay = 1000;
	uint32_t us_error = 150;


	expected = cnt_s - us_to_cnt(freq, us_delay);
	error = us_to_cnt(freq, us_error);
	//printf("freq %d, expected 0x%x, error 0x%x \n", freq, expected, error);

	for(int i = 0; i < TIM15_INDEX; i++) {

		eRet = HAL_TIMEOUT;
		obj.handle.Instance = sp_tim_info[i].instance;
		obj.handle.State = HAL_TIM_STATE_RESET;
		obj.handle.Init.Prescaler = 0;
		obj.handle.Init.Counter = cnt_s;
		obj.handle.Init.ReloadCounter = cnt_s;
		obj.handle.Init.ClockSource = CLK_SYS_SRC;
		obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
		obj.handle.IrqHandle = NULL;

		eRet = HAL_TIM_Init(&(obj.handle));
		CU_ASSERT_EQUAL(eRet, HAL_OK);

		eRet = HAL_TIM_Start(&(obj.handle));
		CU_ASSERT_EQUAL(eRet, HAL_OK);

		delay_us(us_delay);//delay 1ms

		actual = HAL_TIM_GetCount(&(obj.handle));

		eRet = HAL_TIM_Stop(&(obj.handle));
		CU_ASSERT_EQUAL(eRet, HAL_OK);

		//printf("actual cnt 0x%x\n", actual);
		CU_ASSERT_DOUBLE_EQUAL(expected, actual, error);

		HAL_TIM_DeInit(&(obj.handle));
	}
}

static void hal_tim_case_64bit_counter(void)
{
	timerObj_t obj;
	HAL_StatusTypeDef eRet;
	uint32_t actual_l, expected, error, actual_h;
	uint32_t freq = HAL_PLL_GetSystemFreq();
	uint32_t cnt_s_h = 0xFFFFFFFF, cnt_s_l = 0xFFF; //start
	uint32_t us_delay = 1000;//consider the uint32_t overflow
	uint32_t us_error = 150;
	uint32_t temp;//to avoid overflow

	/* here only consider the case delay_cnt < 0xFFFF */
	if(us_to_cnt(freq, us_delay) > cnt_s_l) {
		/* pls avoid the case :
		 * us_to_cnt(freq, delay_us) < cnt_s_l ,
		 * us_to_cnt(freq, delay_us + error_us) > cnt_s_l */
		temp = us_to_cnt(freq, us_delay) - cnt_s_l;

		expected = 0xFFFFFFFF - temp;
	} else {
		expected = cnt_s_l - us_to_cnt(freq, us_delay);
	}
	error = us_to_cnt(freq, us_error);
	//printf("freq %d, expected 0x%x, error 0x%x, temp 0x%x\n",freq, expected, error, temp);

	for(int i = TIM15_INDEX; i < TIM_NUM; i++) {
		eRet = HAL_TIMEOUT;
		obj.handle.Instance64 = sp_tim64_info[i-15].instance;
		obj.handle.State = HAL_TIM_STATE_RESET;
		obj.handle.Init.Prescaler = 0;
		obj.handle.Init.Counter = cnt_s_l;
		obj.handle.Init.Counter_h = cnt_s_h;
		obj.handle.Init.ReloadCounter = cnt_s_l;
		obj.handle.Init.ReloadCounter_h = cnt_s_h;
		obj.handle.Init.ClockSource = CLK_SYS_SRC;
		obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
		obj.handle.IrqHandle = NULL;

		eRet = HAL_TIM_Init(&(obj.handle));
		CU_ASSERT_EQUAL(eRet, HAL_OK);

		eRet = HAL_TIM_Start(&(obj.handle));
		CU_ASSERT_EQUAL(eRet, HAL_OK);

		delay_us(us_delay);//delay 1ms

		actual_l = HAL_TIM_GetCount(&(obj.handle));
		actual_l = HAL_TIM_GetCount(&(obj.handle));
		actual_h = HAL_TIM_GetCount_h(&(obj.handle));

		eRet = HAL_TIM_Stop(&(obj.handle));
		CU_ASSERT_EQUAL(eRet, HAL_OK);

		//printf("actual cnt h 0x%x,l 0x%x\n", actual_h, actual);

		CU_ASSERT_DOUBLE_EQUAL(expected, actual_l, error);
		CU_ASSERT_EQUAL(actual_h, cnt_s_h - 1);//0xFFFFFFFE

		HAL_TIM_DeInit(&(obj.handle));
	}
}

static void hal_tim_case_intr(void)
{
	//timerObj_t obj;
	HAL_StatusTypeDef eRet;
	uint32_t cnt;
	uint32_t freq = HAL_PLL_GetSystemFreq();
	uint32_t ms_delay = 500;
	uint32_t ms_timing = 100;

	cnt = ms_to_cnt(freq, ms_timing);//counter start

	for(int i = TIM15_INDEX; i < TIM_NUM; i++) {
		eRet = HAL_TIMEOUT;
		obj.handle.State = HAL_TIM_STATE_RESET;
		if(i < TIM15_INDEX) {
			obj.handle.Instance = sp_tim_info[i].instance;
		} else {
			obj.handle.Instance64 = sp_tim64_info[i-15].instance;
		}

		obj.handle.Init.Counter = cnt;
		obj.handle.Init.Counter_h = 0;
		obj.handle.Init.ReloadCounter = cnt;
		obj.handle.Init.ReloadCounter_h = 0;
		obj.handle.Init.Prescaler = 0;
		obj.handle.Init.ClockSource = CLK_SYS_SRC;
		obj.handle.Init.AutoReloadPreload = REPEAT_MODE;
		obj.handle.IrqHandle = test_irq_handler;

		eRet = HAL_TIM_Init(&(obj.handle));
		CU_ASSERT_EQUAL(eRet, HAL_OK);

		eRet = HAL_TIM_Start(&(obj.handle));
		CU_ASSERT_EQUAL(eRet, HAL_OK);

		delay(ms_delay);//delay 0.5s

		eRet = HAL_TIM_Stop(&(obj.handle));
		CU_ASSERT_EQUAL(eRet, HAL_OK);

		//printf("entry %d\n", occur_interrupt);
		CU_ASSERT_EQUAL(occur_interrupt, ms_delay / ms_timing);
		occur_interrupt = 0;

		HAL_TIM_DeInit(&(obj.handle));
	}
}

#define STC_MAIN_0	((STC_TypeDef *)RF_GRP(11, 0))
#define STC_MAIN_1	((STC_TypeDef *)RF_GRP(12, 0))

STC_TypeDef *test_target[] = {STC0, STC1, STC2, STC3, STC4, STC_MAIN_0, STC_MAIN_1};

static void hal_stc_case_counter(void)
{
	uint32_t cnt_31_0, cnt_63_32, cnt_64;
	uint32_t cntl_31_0, cntl_32;

	for(int i = 0; i < 7; i++) {

		HAL_InitCommonSTC(test_target[i], 1000000);

		test_target[i]->stc_31_0 = 0xffff0000;
		test_target[i]->stc_63_32 = 0x33333333;

		delay(1000);
		/* read latch */
		cnt_31_0 = test_target[i]->stc_31_0;
		cnt_31_0 = 0;

		cnt_31_0 = test_target[i]->stc_31_0;
		cnt_63_32 = test_target[i]->stc_63_32;
		cnt_64 = test_target[i]->stc_64;

		/* write latch */
		test_target[i]->stcl_32 = 0x1234;
		cntl_31_0 = test_target[i]->stcl_31_0;
		cntl_32 = test_target[i]->stcl_32;

		printf("\n>>>>>>>>> STC%x:\n  stc_31_0 0x%x, stc_63_32 0x%x, stc_64 0x%x \nlatchreg:\n  31_0 0x%x, 32 0x%x\n",
			(uint32_t)test_target[i], cnt_31_0, cnt_63_32, cnt_64, cntl_31_0, cntl_32);
	}
}

CU_TestInfo timer_testcases[] =
{
	{"Test 32-bit conuter timer", hal_tim_case_32bit_counter},
	{"Test 64-bit conuter timer", hal_tim_case_64bit_counter},
	{"Test timer interrupt", hal_tim_case_intr},

	//{"Test 32-bit conuter STC", hal_stc_case_counter},
	CU_TEST_INFO_NULL
};

#endif

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
