#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CUnit.h"
#include "Arduino.h"
#include "sp7350_hal.h"

//#define TEST_SLAVE_ADDRESS	0x2//ZEBU XTOR
#define TEST_SLAVE_ADDRESS	0x44//SHT3x
#define I2C_SINGLE_TEST		SP_I2CM0
#define SLAVE_ADDRESS_SHT3x	0x44

void SHT3x_cal_temperature_humidity(uint8_t *buf)
{
	int temp = (buf[0] << 8) | buf[1];
	temp = temp * 17500 / 65536;
	int temp_L = temp % 100;
	int temp_H = temp / 100 - 45;

	int RH = (buf[3] << 8) | buf[4];
	RH = RH * 10000 / 65536;
	int RH_L = RH % 100;
	int RH_H = RH / 100;

	printf("\n temp=%d.%d   %d.%d    \n", temp_H, temp_L, RH_H, RH_L);
}

void test_case_i2c_SHT3x_single_shot(void)
{
	uint8_t *tx_buf, *rx_buf;
	static HAL_StatusTypeDef ret;

	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	tx_buf = (uint8_t *)malloc(8);
	memset(tx_buf, 0, 8);
	rx_buf = (uint8_t *)malloc(8);
	memset(rx_buf, 0, 8);
	tx_buf[0] = 0x24;
	tx_buf[1] = 0x00;

	test_handle->Instance = I2C_SINGLE_TEST;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_INTR;//I2C_MODE_BURST

	HAL_I2C_Init(test_handle);

	struct i2c_msg msg_set_oneshot[] = {
		{
			.addr = SLAVE_ADDRESS_SHT3x,
			.flag = I2C_M_WRITE,
			.len  = 2,
			.buf  = tx_buf,
		},
	};
	struct i2c_msg msg_read[] = {
		{
			.addr = SLAVE_ADDRESS_SHT3x,
			.flag = I2C_M_READ,
			.len  = 6,
			.buf  = rx_buf,
		},
	};

	while(1) {
		test_handle->State = HAL_I2C_STATE_READY;
		ret = HAL_I2C_Master_Transfer(test_handle, msg_set_oneshot, 1);
		CU_ASSERT_EQUAL(ret, HAL_OK);
		delay_us(100);
		ret = HAL_I2C_Master_Transfer(test_handle, msg_read, 1);
		CU_ASSERT_EQUAL(ret, HAL_OK);

		SHT3x_cal_temperature_humidity(rx_buf);

		delay(2000);
	}
	HAL_I2C_DeInit(test_handle);

	free(tx_buf);
	free(rx_buf);
	free(test_handle);
}

void test_case_i2c_SHT3x_periodic(void)
{
	uint8_t *tx_buf, *rx_buf;
	static HAL_StatusTypeDef ret;

	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	tx_buf = (uint8_t *)malloc(8);
	memset(tx_buf, 0, 8);
	rx_buf = (uint8_t *)malloc(8);
	memset(rx_buf, 0, 8);

	tx_buf[0] = 0x21;
	tx_buf[1] = 0x30;
	tx_buf[2] = 0xe0;
	tx_buf[3] = 0x00;
	/* soft reset */
	tx_buf[4] = 0x30;
	tx_buf[5] = 0xa2;

	test_handle->Instance = I2C_SINGLE_TEST;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_INTR;//I2C_MODE_BURST

	HAL_I2C_Init(test_handle);

	struct i2c_msg msg_set_periodic[] = {
		{
			.addr = SLAVE_ADDRESS_SHT3x,
			.flag = I2C_M_WRITE,
			.len  = 2,
			.buf  = tx_buf,
		},
	};
	struct i2c_msg msg_get_periodic[] = {
		{
			.addr = SLAVE_ADDRESS_SHT3x,
			.flag = I2C_M_WRITE,
			.len  = 2,
			.buf  = tx_buf + 2,
		},
		{
			.addr = SLAVE_ADDRESS_SHT3x,
			.flag = I2C_M_READ,
			.len  = 6,
			.buf  = rx_buf,
		},
	};
	struct i2c_msg msg_reset[] = {
		{
			.addr = SLAVE_ADDRESS_SHT3x,
			.flag = I2C_M_WRITE,
			.len  = 2,
			.buf  = tx_buf + 4,
		},
	};

	HAL_I2C_Master_Transfer(test_handle, msg_set_periodic, 1);
	HAL_I2C_Master_Transfer(test_handle, msg_reset, 1);

	while(1) {
		test_handle->State = HAL_I2C_STATE_READY;
		ret = HAL_I2C_Master_Transfer(test_handle, msg_get_periodic, 2);
		CU_ASSERT_EQUAL(ret, HAL_OK);
		if (test_handle->ErrorCode == HAL_I2C_ERR_ADDRESS_NACK) {
			HAL_I2C_Master_Transfer(test_handle, msg_reset, 1);
			test_handle->ErrorCode = HAL_I2C_ERR_NONE;
		}
		SHT3x_cal_temperature_humidity(rx_buf);
		delay(2000);
	}
	HAL_I2C_DeInit(test_handle);

	free(tx_buf);
	free(rx_buf);
	free(test_handle);
}

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
	static HAL_StatusTypeDef ret;

	I2C_HandleTypeDef *test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	tx_buf = (uint8_t *)malloc(8);
	memset(tx_buf, 0, 8);
	rx_buf = (uint8_t *)malloc(8);
	memset(rx_buf, 0, 8);

	test_handle->Instance = I2C_SINGLE_TEST;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_BURST;

	HAL_I2C_Init(test_handle);

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

	free(tx_buf);
	free(rx_buf);
	free(test_handle);
}

CU_TestInfo i2c_testcases[] =
{
	{"test_case_i2c_SHT3x_single_shot: ",	test_case_i2c_SHT3x_single_shot},
	//{"test_case_i2c_SHT3x_periodic ",	test_case_i2c_SHT3x_periodic},
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
	printf(" build @ " __DATE__ " " __TIME__ "\n");

#if 0	/* Test I2CX pinmux SDA SCL */
	#define TEST_SCL 91
	#define TEST_SDA 90

	printf("[DEBUG] Set pinmux.\n"); //xt temp add
	GPIO_InitTypeDef gpio_i2c0data;
	memset(&gpio_i2c0data, 0, sizeof(gpio_i2c0data));
	gpio_i2c0data.Mode = GPIO_PP_OUTPUT_MODE;
	gpio_i2c0data.Pin = GPIO(TEST_SCL);
	gpio_i2c0data.out_value = GPIO_OUT_LOW;
	HAL_GPIO_Init(&gpio_i2c0data);

	GPIO_InitTypeDef gpio_i2c0clk;
	memset(&gpio_i2c0clk, 0, sizeof(gpio_i2c0clk));
	gpio_i2c0clk.Mode = GPIO_PP_OUTPUT_MODE;
	gpio_i2c0clk.Pin = GPIO(TEST_SDA);
	gpio_i2c0clk.out_value = GPIO_OUT_LOW;
	HAL_GPIO_Init(&gpio_i2c0clk);

	printf("[DEBUG] Output the level.\n"); //xt temp add
	while (1) {
		HAL_GPIO_WritePin(GPIO(TEST_SCL), GPIO_OUT_HIGH);
		delay(1);
		HAL_GPIO_WritePin(GPIO(TEST_SCL), GPIO_OUT_LOW);
		delay(1);
		HAL_GPIO_WritePin(GPIO(TEST_SDA), GPIO_OUT_HIGH);
		delay(2);
		HAL_GPIO_WritePin(GPIO(TEST_SDA), GPIO_OUT_LOW);
		delay(2);
	}
#endif
	assert(NULL != CU_get_registry());
	assert(!CU_is_test_running());
	if(CUE_SUCCESS != CU_register_suites(suites))  //注册测试suite
	{
		fprintf(stderr, "Register suites failed - %s ", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}

	return 0;
}

