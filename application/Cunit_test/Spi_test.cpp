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

static SPI_HandleTypeDef hspi_test;
static HAL_StatusTypeDef ret;

static unsigned char rxbuf[100]={0},txbuf[100]={0};
static int rw_ret=0;

volatile SPI_TypeDef *spi_base ;
static PINMUX_Type pinmux_idx;
static IRQn_Type irq_no;

static void spi_callback(void)
{
	HAL_SPI_IRQHandler(&hspi_test);
}


static void _get_Spi_param(int spi_idx) 
{ 
	switch(spi_idx)
	{
		case 0:
			spi_base = SPI0;
			irq_no = SPI_MASTER0_IRQn;
			pinmux_idx = PINMUX_SPI_COM0_MST;
			break;
		case 1:
			spi_base = SPI1;
			irq_no = SPI_MASTER1_IRQn;
			pinmux_idx = PINMUX_SPI_COM1_MST;
			break;
		case 2:
			spi_base = SPI2;
			irq_no = SPI_MASTER2_IRQn;
			pinmux_idx = PINMUX_SPI_COM2_MST;
			break;
		case 3:
			spi_base = SPI3;
			irq_no = SPI_MASTER3_IRQn;
			pinmux_idx = PINMUX_SPI_COM3_MST;
			break;
		case 4:
			spi_base = SPI4;
			irq_no = SPI_MASTER4_IRQn;
			pinmux_idx = PINMUX_SPI_COM4_MST;
			break;
		case 5:
			spi_base = SPI5;
			irq_no = SPI_MASTER5_IRQn;
			pinmux_idx = PINMUX_SPI_COM5_MST;
			break;
		default:
			CU_ASSERT_STRING_EQUAL("uart idx", "error");
			break;
	}

}

static void _spi_init(void)
{
	memset(&hspi_test,0,sizeof(SPI_HandleTypeDef));
	hspi_test.Instance = spi_base;
	hspi_test.Init.spiclk = 1500000;
	hspi_test.Init.spi_mode = SPI_MODE0;
	hspi_test.Init.FirstBit = MSBFIRST;
	ret = HAL_SPI_Init(&hspi_test);

	/* set pinmux */
	HAL_PINMUX_Cfg(pinmux_idx,1);

	for(int i=0;i < 10;i++)
	{
		txbuf[i] = 0x44+i;
	}
}

static void spi_test_Transmit_DMA(void) 
{ 
	_spi_init();
	
	rw_ret = 0;
	ret = HAL_SPI_Transmit_DMA(&hspi_test, txbuf, 10);

	CU_ASSERT_EQUAL(ret, HAL_OK);

	/* wait for send data to complete*/

	CU_ASSERT_EQUAL(rw_ret, 1);
}

static void spi_test_Receive_DMA(void) 
{ 
	_spi_init();

	rw_ret = 0;
	ret = HAL_SPI_Receive_DMA(&hspi_test, rxbuf,10);
	CU_ASSERT_EQUAL(ret, HAL_OK);

	CU_ASSERT_EQUAL(rw_ret, 1);
}

static void spi_test_TransmitReceive_DMA(void) 
{ 
	_spi_init();

	rw_ret = 0;
	ret = HAL_SPI_TransmitReceive_DMA(&hspi_test, txbuf,rxbuf,10, 10);
	CU_ASSERT_EQUAL(ret, HAL_OK);

	CU_ASSERT_EQUAL(rw_ret, 1);

}

static void spi_test_Transmit_IT(void) 
{ 
	_spi_init();

	/* set interrupt */
	HAL_NVIC_SetVector(irq_no, (uint32_t)spi_callback);
	HAL_NVIC_EnableIRQ(irq_no);

	rw_ret = 0;
	ret = HAL_SPI_Transmit_IT(&hspi_test, txbuf, 10);

	CU_ASSERT_EQUAL(ret, HAL_OK);

	/* wait for send data to complete*/

	CU_ASSERT_EQUAL(rw_ret, 1);
}

static void spi_test_Receive_IT(void) 
{ 
	_spi_init();

	/* set interrupt */
	HAL_NVIC_SetVector(irq_no, (uint32_t)spi_callback);
	HAL_NVIC_EnableIRQ(irq_no);

	rw_ret = 0;
	ret = HAL_SPI_Receive_IT(&hspi_test, rxbuf, 10);
	CU_ASSERT_EQUAL(ret, HAL_OK);
	printf(" spi rx it rw_ret=%d \n",rw_ret);
	CU_ASSERT_EQUAL(rw_ret, 1);
}

static void spi_test_TransmitReceive_IT(void) 
{ 
	_spi_init();

	/* set interrupt */
	HAL_NVIC_SetVector(irq_no, (uint32_t)spi_callback);
	HAL_NVIC_EnableIRQ(irq_no);

	rw_ret = 0;
	ret = HAL_SPI_TransmitReceive_IT(&hspi_test, txbuf,rxbuf,10, 10);
	CU_ASSERT_EQUAL(ret, HAL_OK);

	CU_ASSERT_EQUAL(rw_ret, 1);
}

static void spi_test_Transmit_Polling(void) 
{
	_spi_init();

	ret = HAL_SPI_Transmit(&hspi_test, txbuf, 10,0xFFFFF);

	CU_ASSERT_EQUAL(ret, HAL_OK);

	/* wait for send data to complete*/

}

static void spi_test_Receive_Polling(void) 
{
	_spi_init();

	ret = HAL_SPI_Receive(&hspi_test, rxbuf, 10,0xFFFFF);

	CU_ASSERT_EQUAL(ret, HAL_OK);
}

static void spi_test_TransmitReceive_Polling(void) 
{
	_spi_init();

	ret = HAL_SPI_TransmitReceive(&hspi_test, txbuf,rxbuf,10,10,0xFFFFF);

	CU_ASSERT_EQUAL(ret, HAL_OK);
}

void spi_test_Tx_DMA(void)       {_get_Spi_param(0); spi_test_Transmit_DMA();     }
void spi_test_Rx_DMA(void)       {_get_Spi_param(0); spi_test_Receive_DMA();      }
void spi_test_TxRx_DMA(void)     {_get_Spi_param(0); spi_test_TransmitReceive_DMA();      }
void spi_test_Tx_IT(void)        {_get_Spi_param(0); spi_test_Transmit_IT();      }
void spi_test_Rx_IT(void)        {_get_Spi_param(0); spi_test_Receive_IT();       }
void spi_test_TxRx_IT(void)      {_get_Spi_param(0); spi_test_TransmitReceive_IT();       }
void spi_test_Tx_Polling(void)   {_get_Spi_param(0); spi_test_Transmit_Polling(); }
void spi_test_Rx_Polling(void)   {_get_Spi_param(0); spi_test_Receive_Polling();  }
void spi_test_TxRx_Polling(void) {_get_Spi_param(0); spi_test_TransmitReceive_Polling();  }

static CU_TestInfo   spi_testcases[] =
{
	{"test_spi_Tx_IT_case：",        spi_test_Tx_IT},
	{"test_spi_Rx_IT_case：",        spi_test_Rx_IT},
	{"test_spi_TxRx_IT_case：", 	  	spi_test_TxRx_IT},
	{"test_spi_Tx_DMA_case：",       spi_test_Tx_DMA},
	{"test_spi_Rx_DMA_case：",       spi_test_Rx_DMA},
	{"test_spi_TxRx_DMA_case：",		spi_test_TxRx_DMA},
	{"test_spi_Tx_polling_case：",   spi_test_Tx_Polling},
	{"test_spi_Rx_polling_case：",   spi_test_Rx_Polling},
	{"test_spi_TxRx_polling_case：",spi_test_TxRx_Polling},
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
	{"Testing the SPI:", suite_success_init, suite_success_clean, NULL,NULL,spi_testcases},
	CU_SUITE_INFO_NULL
};

int Add_Spi_Tests(void)
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



extern "C" {

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *huart)
{
	rw_ret = 1;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *huart)
{
	rw_ret = 1;
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *huart)
{
	rw_ret = 1;
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *huart)
{

}

}


