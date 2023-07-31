#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CUnit.h"
#include "Arduino.h"
#ifdef SP645
#include "sp645_hal.h"
#elif defined(SP7350)
#include "sp7350_hal.h"
#endif

#define I2C_FREQ	100

//#define TEST_SLAVE_ADDRESS	0x2//ZEBU XTOR
#define TEST_SLAVE_ADDRESS	0x44//SHT3x

#define IRQ_ENABLE	1
#define IRQ_DISABLE	0

//#define QUICK_TEST

//#define I2C_ALL_TEST
#ifndef I2C_ALL_TEST
#define I2C_SINGLE_TEST		SP_I2CM7
#endif


static I2C_HandleTypeDef *gpHandle[I2C_NUM];
static uint8_t *buf;
static HAL_StatusTypeDef ret;

/****************************************** TEST CASE *************************************/
void test_case_i2c_tx_burst(void)
{
	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	buf = (uint8_t *)malloc(6);
	memset(buf, 0x55, sizeof(buf));

	test_handle->Instance = I2C_SINGLE_TEST;
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

	buf = (uint8_t *)malloc(6);
	memset(buf, 0, sizeof(buf));

	test_handle->Instance = I2C_SINGLE_TEST;
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

	buf = (uint8_t *)malloc(6);
	memset(buf, 0x55, sizeof(buf));

	test_handle->Instance = I2C_SINGLE_TEST;
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

	buf = (uint8_t *)malloc(6);
	memset(buf, 0, sizeof(buf));

	test_handle->Instance = I2C_SINGLE_TEST;
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

	buf = (uint8_t *)malloc(6);
	memset(buf, 0x55, sizeof(buf));

	test_handle->Instance = I2C_SINGLE_TEST;
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

	buf = (uint8_t *)malloc(6);
	memset(buf, 0, sizeof(buf));

	test_handle->Instance = I2C_SINGLE_TEST;
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

void test_case_i2c_SHT3x(void)
{
	uint8_t *tx_buf, *rx_buf;

	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	tx_buf = (uint8_t *)malloc(8);
	memset(tx_buf, 0, 8);
	rx_buf = (uint8_t *)malloc(8);
	memset(rx_buf, 0, 8);
	tx_buf[0] = 0x24;
	tx_buf[1] = 0x00;

	test_handle->Instance = I2C_SINGLE_TEST;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_BURST;

	HAL_I2C_Init(test_handle);

	//for(int i = 0; i < 5; i++) {
	while(1) {
		test_handle->State = HAL_I2C_STATE_READY;
		ret = HAL_I2C_Master_Transmit(test_handle, TEST_SLAVE_ADDRESS, tx_buf, 2, 0);
		CU_ASSERT_EQUAL(ret, HAL_OK);
		ret = HAL_I2C_Master_Receive(test_handle, TEST_SLAVE_ADDRESS, rx_buf, 6, 0);
		CU_ASSERT_EQUAL(ret, HAL_OK);

		int temp = (rx_buf[0] << 8) | rx_buf[1];
		temp = temp * 17500 / 65536;
		int temp_L = temp % 100;
		int temp_H = temp / 100 - 45;

		int RH = (rx_buf[3] << 8) | rx_buf[4];
		RH = RH * 10000 / 65536;
		int RH_L = RH % 100;
		int RH_H = RH / 100;

		printf("\n temp=%d.%d   %d.%d    \n", temp_H, temp_L, RH_H, RH_L);
		delay(2000);
	}
	HAL_I2C_DeInit(test_handle);

	free(buf);
	free(test_handle);
}
/****************************************** TEST CASE *************************************/

////////////////////
#define RT5759_ADDR             0x62

#define RT5759_REG_VENDORINFO   0x00
#define RT5759_REG_FREQ         0x01
#define RT5759_REG_VSEL         0x02
#define RT5759_REG_DCDCCTRL     0x03
#define RT5759_REG_STATUS       0x04
#define RT5759_REG_DCDCSET      0x05

#define RT5759_0D8V             ((0.8-0.6)*100)
#define RT5759_MANUFACTURER_ID  0x82

void test_case_i2c_pmic(void)
{
	uint8_t *tx_buf, *rx_buf;

	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	tx_buf = (uint8_t *)malloc(8);
	memset(tx_buf, 0, 8);
	rx_buf = (uint8_t *)malloc(8);
	memset(rx_buf, 0, 8);

	test_handle->Instance = I2C_SINGLE_TEST;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_BURST;

	HAL_I2C_Init(test_handle);

	//for(int i = 0; i < 5; i++) {
	do {
		tx_buf[0] = RT5759_REG_VENDORINFO;
		ret = HAL_I2C_Master_Transmit(test_handle, RT5759_ADDR, tx_buf, 1, 0);
		CU_ASSERT_EQUAL(ret, HAL_OK);
		ret = HAL_I2C_Master_Receive(test_handle, RT5759_ADDR, rx_buf, 1, 0);
		CU_ASSERT_EQUAL(ret, HAL_OK);
		CU_ASSERT_EQUAL(rx_buf[0], RT5759_MANUFACTURER_ID);
		printf("version 0x%x\n", rx_buf[0]);

		tx_buf[0]=RT5759_REG_VSEL;
		tx_buf[1]=RT5759_0D8V;
		ret = HAL_I2C_Master_Transmit(test_handle, RT5759_ADDR, tx_buf, 2, 0);
		CU_ASSERT_EQUAL(ret, HAL_OK);
	} while (0);

	HAL_I2C_DeInit(test_handle);

	free(buf);
	free(test_handle);
}

/////////////////////
#if 0
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

#endif
CU_TestInfo i2c_testcases[] =
{
	{"test_case_i2c_SHT3x: ",	test_case_i2c_SHT3x},
	//{"test_case_i2c_pmic: ",	test_case_i2c_pmic},
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

