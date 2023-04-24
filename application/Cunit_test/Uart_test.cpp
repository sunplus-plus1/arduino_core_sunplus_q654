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

/*

UART1/2/3/6/7 RX connect to its own TX, send/receive by itself

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

static UART_HandleTypeDef huart_test;
static HAL_StatusTypeDef ret;

static char buf[100]={0};
static int rw_ret=0;

static void uart_callback(void)
{
	HAL_UART_IRQHandler(&huart_test);
}

static void uart_rxdma_callback(void)
{
	HAL_UART_Rxdma_IRQ_Handler(&huart_test);
}

static UART_CTRL_Type *uart_base;
static UART_Txdma *txdma_base;
static UART_Rxdma *rxdma_base;
static PINMUX_Type pinmux_idx;
static IRQn_Type irq_no,rxdma_irq_no;

static void _get_Uart_param(int uart_idx)
{
	switch(uart_idx)
	{
		case 1:
			uart_base = SP_UART1;
			irq_no = UART1_IRQn;
			txdma_base= UART_TXDMA1;
			pinmux_idx = PINMUX_UART1;
			break;
		case 2:
			uart_base = SP_UART2;
			irq_no = UART2_IRQn;
			txdma_base= UART_TXDMA2;
			pinmux_idx = PINMUX_UART2;
			break;
		case 3:
			uart_base = SP_UART3;
			irq_no = UART3_IRQn;
			txdma_base= UART_TXDMA3;
			pinmux_idx = PINMUX_UART3;
			break;
		case 6:
			uart_base = SP_UART6;
			irq_no = UART6_IRQn;
#ifdef SP645
			txdma_base= UART_TXDMA1;
#elif defined(SP7350)
			txdma_base= UART_TXDMA6;
#endif
			pinmux_idx = PINMUX_UART6;
			break;
		case 7:
			uart_base = SP_UART7;
			irq_no = UART7_IRQn;
#ifdef SP645
			txdma_base= UART_TXDMA2;
#elif defined(SP7350)
			txdma_base= UART_TXDMA7;
#endif
			pinmux_idx = PINMUX_UART7;
			break;
		default:
			CU_ASSERT_STRING_EQUAL("uart idx", "error");
			break;
	}

}

static void uart_test_Transmit_DMA(void)
{
	HAL_UART_DeInit(&huart_test);

	memset(&huart_test,0,sizeof(UART_HandleTypeDef));
	huart_test.Instance = uart_base;
	huart_test.Init.BaudRate = 115200;
	huart_test.txdma = txdma_base;
	HAL_UART_Init(&huart_test);

	/* set pinmux */
	HAL_PINMUX_Cfg(pinmux_idx,1);

	/* set interrupt */
	HAL_NVIC_SetVector(irq_no, (uint32_t)uart_callback);
	HAL_NVIC_EnableIRQ(irq_no);

	ret = HAL_UART_Transmit_DMA(&huart_test, (uint8_t*)"abcdefg123abc", 20);
	CU_ASSERT_EQUAL(ret, HAL_OK);

	/* wait for send data to complete*/
	delay(200);
	CU_ASSERT_EQUAL(rw_ret, 1);
}

static void uart_test_Receive_Polling(void);
static void uart_test_Receive_DMA(void)
{
#ifdef SP645

	HAL_UART_DeInit(&huart_test);
	memset(&huart_test,0,sizeof(UART_HandleTypeDef));
	huart_test.Instance = uart_base;
	huart_test.Init.BaudRate = 115200;
	huart_test.rxdma = UART_RXDMA1;
	HAL_UART_Init(&huart_test);

	/* set pinmux */
	HAL_PINMUX_Cfg(pinmux_idx,1);

	/* set interrupt */
	HAL_NVIC_SetVector(UADMA1_IRQn, (uint32_t)uart_rxdma_callback);
	HAL_NVIC_EnableIRQ(UADMA1_IRQn);

	ret = HAL_UART_Receive_DMA(&huart_test, (uint8_t*)buf, 6);
	CU_ASSERT_EQUAL(ret, HAL_OK);

	/* TX send data to RX */
	ret = HAL_UART_Transmit(&huart_test, (uint8_t*)"abcdefg123abc", 10,0xFFFFFF);
	CU_ASSERT_EQUAL(ret, HAL_OK);

	/* wait to receive data */
	delay(500);

	CU_ASSERT_EQUAL(rw_ret, 1);
	CU_ASSERT_STRING_EQUAL(buf, "abcdef");

#elif defined(SP7350)
    /* SP7350 no rxdma mode, use polling to replace */
	return uart_test_Receive_Polling();
#endif

}

static void uart_test_Transmit_IT(void)
{
	HAL_UART_DeInit(&huart_test);

	memset(&huart_test,0,sizeof(UART_HandleTypeDef));
	huart_test.Instance = uart_base;
	huart_test.Init.BaudRate = 115200;
	HAL_UART_Init(&huart_test);

	/* set interrupt */
	HAL_PINMUX_Cfg(pinmux_idx,1);

	/* set interrupt */
	HAL_NVIC_SetVector(irq_no, (uint32_t)uart_callback);
	HAL_NVIC_EnableIRQ(irq_no);

	rw_ret = 0;
	ret = HAL_UART_Transmit_IT(&huart_test, (uint8_t*)"abcdefg123456", 10);
	CU_ASSERT_EQUAL(ret, HAL_OK);

	/* wait for send data to complete*/
	delay(10);

	CU_ASSERT_EQUAL(rw_ret, 1);

}

static void uart_test_Receive_IT(void)
{
	/* clear data */
	memset(buf,0,sizeof(buf));
	rw_ret = 0;

	ret = HAL_UART_Receive_IT(&huart_test, (uint8_t*)buf, 6);
	CU_ASSERT_EQUAL(ret, HAL_OK);

	/* wait to receive data */
	delay(10);
	CU_ASSERT_EQUAL(rw_ret, 1);
	CU_ASSERT_STRING_EQUAL(buf, "abcdef");
}

/*  uart polling: uart3'TX connect to uart3's RX */
static void uart_test_Transmit_Polling(void)
{
	HAL_UART_DeInit(&huart_test);

	memset(&huart_test,0,sizeof(UART_HandleTypeDef));
	huart_test.Instance = uart_base;
	huart_test.Init.BaudRate = 115200;
	HAL_UART_Init(&huart_test);

	HAL_PINMUX_Cfg(pinmux_idx,1);

	ret = HAL_UART_Transmit(&huart_test, (uint8_t*)"abcdefg123456", 10,0xFFFF);

	CU_ASSERT_EQUAL(ret, HAL_OK);
}

static void uart_test_Receive_Polling(void)
{
	memset(&huart_test,0,sizeof(UART_HandleTypeDef));
	huart_test.Instance = uart_base;
	huart_test.Init.BaudRate = 115200;
	HAL_UART_Init(&huart_test);

	HAL_PINMUX_Cfg(pinmux_idx,1);

	memset(buf,0,sizeof(buf));

	ret = HAL_UART_Receive(&huart_test, (uint8_t*)buf, 6,0xFFFF);

	CU_ASSERT_EQUAL(ret, HAL_OK);
	CU_ASSERT_STRING_EQUAL(buf, "abcdef");
}

void uart1_test_Tx_DMA(void)	 {	_get_Uart_param(1); uart_test_Transmit_DMA();     }
void uart1_test_Rx_DMA(void)	 {	_get_Uart_param(1); uart_test_Receive_DMA();      }
void uart1_test_Tx_IT(void) 	 { 	_get_Uart_param(1); uart_test_Transmit_IT();      }
void uart1_test_Rx_IT(void) 	 {	_get_Uart_param(1); uart_test_Receive_IT();       }
void uart1_test_Tx_Polling(void) { 	_get_Uart_param(1); uart_test_Transmit_Polling(); }
void uart1_test_Rx_Polling(void) {	_get_Uart_param(1); uart_test_Receive_Polling();  }

void uart2_test_Tx_DMA(void)	 {	_get_Uart_param(2); uart_test_Transmit_DMA();     }
void uart2_test_Rx_DMA(void)	 {	_get_Uart_param(2); uart_test_Receive_DMA();      }
void uart2_test_Tx_IT(void) 	 { 	_get_Uart_param(2); uart_test_Transmit_IT();      }
void uart2_test_Rx_IT(void) 	 {	_get_Uart_param(2); uart_test_Receive_IT();       }
void uart2_test_Tx_Polling(void) { 	_get_Uart_param(2); uart_test_Transmit_Polling(); }
void uart2_test_Rx_Polling(void) {	_get_Uart_param(2); uart_test_Receive_Polling();  }

void uart3_test_Tx_DMA(void)	 {	_get_Uart_param(3); uart_test_Transmit_DMA();}
void uart3_test_Rx_DMA(void)	 {	_get_Uart_param(3); uart_test_Receive_DMA();      }
void uart3_test_Tx_IT(void) 	 { 	_get_Uart_param(3); uart_test_Transmit_IT();      }
void uart3_test_Rx_IT(void) 	 {	_get_Uart_param(3); uart_test_Receive_IT();       }
void uart3_test_Tx_Polling(void) { 	_get_Uart_param(3); uart_test_Transmit_Polling(); }
void uart3_test_Rx_Polling(void) {	_get_Uart_param(3); uart_test_Receive_Polling();  }

void uart6_test_Tx_DMA(void)	 {	_get_Uart_param(6); uart_test_Transmit_DMA();}
void uart6_test_Rx_DMA(void)	 {	_get_Uart_param(6); uart_test_Receive_DMA();      }
void uart6_test_Tx_IT(void) 	 { 	_get_Uart_param(6); uart_test_Transmit_IT();      }
void uart6_test_Rx_IT(void) 	 {	_get_Uart_param(6); uart_test_Receive_IT();       }
void uart6_test_Tx_Polling(void) { 	_get_Uart_param(6); uart_test_Transmit_Polling(); }
void uart6_test_Rx_Polling(void) {	_get_Uart_param(6); uart_test_Receive_Polling();  }

void uart7_test_Tx_DMA(void)	 {	_get_Uart_param(7); uart_test_Transmit_DMA();}
void uart7_test_Rx_DMA(void)	 {	_get_Uart_param(7); uart_test_Receive_DMA();      }
void uart7_test_Tx_IT(void) 	 { 	_get_Uart_param(7); uart_test_Transmit_IT();      }
void uart7_test_Rx_IT(void) 	 {	_get_Uart_param(7); uart_test_Receive_IT();       }
void uart7_test_Tx_Polling(void) { 	_get_Uart_param(7); uart_test_Transmit_Polling(); }
void uart7_test_Rx_Polling(void) {	_get_Uart_param(7); uart_test_Receive_Polling();  }

static CU_TestInfo   uart_testcases[] =
{
	{"test_uart1_transmit_DMA_case：",     uart1_test_Tx_DMA},
	{"test_uart1_receive_DMA_case：",      uart1_test_Rx_DMA},
	{"test_uart1_transmit_IT_case：",      uart1_test_Tx_IT},
	{"test_uart1_receive_IT_case：",       uart1_test_Rx_IT},
	{"test_uart1_transmit_polling_case：", uart1_test_Tx_Polling},
	{"test_uart1_receive_polling_case：",  uart1_test_Rx_Polling},

	{"test_uart2_transmit_DMA_case：",     uart2_test_Tx_DMA},
	{"test_uart2_receive_DMA_case：",      uart2_test_Rx_DMA},
	{"test_uart2_transmit_IT_case：",      uart2_test_Tx_IT},
	{"test_uart2_receive_IT_case：",       uart2_test_Rx_IT},
	{"test_uart2_transmit_polling_case：", uart2_test_Tx_Polling},
	{"test_uart2_receive_polling_case：",  uart2_test_Rx_Polling},

	{"test_uart3_transmit_DMA_case：",     uart3_test_Tx_DMA},
	{"test_uart3_receive_DMA_case：",      uart3_test_Rx_DMA},
	{"test_uart3_transmit_IT_case：",      uart3_test_Tx_IT},
	{"test_uart3_receive_IT_case：",       uart3_test_Rx_IT},
	{"test_uart3_transmit_polling_case：", uart3_test_Tx_Polling},
	{"test_uart3_receive_polling_case：",	uart3_test_Rx_Polling},

	{"test_uart6_transmit_DMA_case：",     uart6_test_Tx_DMA},
	{"test_uart6_receive_DMA_case：",      uart6_test_Rx_DMA},
	{"test_uart6_transmit_IT_case：",      uart6_test_Tx_IT},
	{"test_uart6_receive_IT_case：",       uart6_test_Rx_IT},
	{"test_uart6_transmit_polling_case：", uart6_test_Tx_Polling},
	{"test_uart6_receive_polling_case：",  uart6_test_Rx_Polling},

	{"test_uart7_transmit_DMA_case：",     uart7_test_Tx_DMA},
	{"test_uart7_receive_DMA_case：",      uart7_test_Rx_DMA},
	{"test_uart7_transmit_IT_case：",      uart7_test_Tx_IT},
	{"test_uart7_receive_IT_case：",       uart7_test_Rx_IT},
	{"test_uart7_transmit_polling_case：", uart7_test_Tx_Polling},
	{"test_uart7_receive_polling_case：",  uart7_test_Rx_Polling},

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
	{"Testing the Uart：", suite_success_init, suite_success_clean, NULL,NULL,uart_testcases},
	CU_SUITE_INFO_NULL
};

int Add_Uart_Tests(void)
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

static void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	rw_ret = 1;
}

static void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	rw_ret = 1;
}

static void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}

}


