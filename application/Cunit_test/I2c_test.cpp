#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CUnit.h"
#ifdef SP645
#include "sp645_hal.h"
#elif defined(SP7350)
#include "sp7350_hal.h"
#endif

#define I2C_FREQ	100

#define TEST_SLAVE_ADDRESS	0x2

#define IRQ_ENABLE	1
#define IRQ_DISABLE	0

//#define QUICK_TEST

//#define I2C_ALL_TEST
#ifndef I2C_ALL_TEST
#define I2C_SINGLE_TEST		I2C0_INDEX
#endif


static I2C_HandleTypeDef *gpHandle[I2C_NUM];
static uint8_t *buf;
static HAL_StatusTypeDef ret;

/****************************************** TEST CASE *************************************/
void test_case_i2c_tx_burst(void)
{
	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	buf = (uint8_t *)malloc(2);
	memset(buf, 0x55, sizeof(buf));

	test_handle->Instance = SP_I2CM0;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_BURST;

	HAL_I2C_Init(test_handle);
	ret = HAL_I2C_Master_Transmit(test_handle, TEST_SLAVE_ADDRESS, buf, 2, 0);
	HAL_I2C_DeInit(test_handle);

	CU_ASSERT_EQUAL(ret, HAL_OK);

	free(buf);
	free(test_handle);

}

void test_case_i2c_rx_burst(void)
{
	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	buf = (uint8_t *)malloc(2);
	memset(buf, 0, sizeof(buf));

	test_handle->Instance = SP_I2CM0;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_BURST;

	HAL_I2C_Init(test_handle);
	ret = HAL_I2C_Master_Receive(test_handle, TEST_SLAVE_ADDRESS, buf, 1, 0);
	HAL_I2C_DeInit(test_handle);

	CU_ASSERT_EQUAL(ret, HAL_OK);
	CU_ASSERT_EQUAL(*buf, 0x82);

	free(buf);
	free(test_handle);
}

void test_case_i2c_tx_intr(void)
{
	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	buf = (uint8_t *)malloc(2);
	memset(buf, 0x55, sizeof(buf));

	test_handle->Instance = SP_I2CM0;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_INTR;

	HAL_I2C_Init(test_handle);
	ret = HAL_I2C_Master_Transmit_IT(test_handle, TEST_SLAVE_ADDRESS, buf, 2);
	HAL_I2C_DeInit(test_handle);

	CU_ASSERT_EQUAL(ret, HAL_OK);

	free(buf);
	free(test_handle);
}

void test_case_i2c_rx_intr(void)
{
	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	buf = (uint8_t *)malloc(2);
	memset(buf, 0, sizeof(buf));

	test_handle->Instance = SP_I2CM0;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_INTR;

	HAL_I2C_Init(test_handle);
	ret = HAL_I2C_Master_Receive_IT(test_handle, TEST_SLAVE_ADDRESS, buf, 1);
	HAL_I2C_DeInit(test_handle);

	CU_ASSERT_EQUAL(ret, HAL_OK);
	CU_ASSERT_EQUAL(*buf, 0x82);

	free(buf);
	free(test_handle);
}

void test_case_i2c_tx_dma(void)
{
	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	buf = (uint8_t *)malloc(2);
	memset(buf, 0x55, sizeof(buf));

	test_handle->Instance = SP_I2CM0;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_DMA;

	HAL_I2C_Init(test_handle);
	ret = HAL_I2C_Master_Transmit_DMA(test_handle, TEST_SLAVE_ADDRESS, buf, 2);
	HAL_I2C_DeInit(test_handle);

	CU_ASSERT_EQUAL(ret, HAL_OK);

	free(buf);
	free(test_handle);
}

void test_case_i2c_rx_dma(void)
{
	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	buf = (uint8_t *)malloc(2);
	memset(buf, 0, sizeof(buf));

	test_handle->Instance = SP_I2CM0;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_DMA;

	HAL_I2C_Init(test_handle);
	ret = HAL_I2C_Master_Receive_DMA(test_handle, TEST_SLAVE_ADDRESS, buf, 1);
	HAL_I2C_DeInit(test_handle);

	CU_ASSERT_EQUAL(ret, HAL_OK);
	CU_ASSERT_EQUAL(*buf, 0x82);

	free(buf);
	free(test_handle);
}

/****************************************** TEST CASE *************************************/

CU_TestInfo i2c_testcases[] =
{
	{"test_i2c_transmit_burst_mode_case: ",	test_case_i2c_tx_burst},
	{"test_i2c_receive_burst_mode_case: ",	test_case_i2c_rx_burst},
	{"test_i2c_transmit_intr_mode_case: ",	test_case_i2c_tx_intr},
	{"test_i2c_receive_intr_mode_case: ",	test_case_i2c_rx_intr},
	{"test_i2c_transmit_dma_mode_case: ",	test_case_i2c_tx_dma},
	{"test_i2c_receive_dma_mode_case: ",	test_case_i2c_rx_dma},
	CU_TEST_INFO_NULL
};

int suite_success_i2c_init(void)
{
	return  0;
}

int suite_success_i2c_clean(void)
{
	return  0;
}

static CU_SuiteInfo suites[] =
{
	{"Testing the I2C", suite_success_i2c_init, suite_success_i2c_clean, NULL, NULL, i2c_testcases},
	CU_SUITE_INFO_NULL
};

int Add_I2C_Tests(void)
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

