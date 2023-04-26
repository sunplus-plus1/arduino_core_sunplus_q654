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

#define I2C_INFO(inst, idx, clk, pin, irqn, cb, dma) \
	{ .instance = (inst), .index = (idx), \
	  .clk_id = (clk), .pinmux = (pin), \
	  .irq_num = (irqn), .irq_callback = (cb), \
	  .dma_idx = (dma) }

#ifndef SP7350
enum {
        PER_I2C0,
        PER_I2C1,
        PER_I2C2,
        PER_I2C3,
        PER_I2C4,
        PER_I2C5,
        PER_I2C6,
        PER_I2C7,
        PER_I2C8,
        PER_I2C9,
};
#endif

typedef enum {
	I2C0_INDEX = 0UL,
	I2C1_INDEX,
	I2C2_INDEX,
	I2C3_INDEX,
#if defined (SP645) || defined (SP7350)
	I2C4_INDEX,
	I2C5_INDEX,
#endif
#ifdef SP7350
	I2C6_INDEX,
	I2C7_INDEX,
	I2C8_INDEX,
	I2C9_INDEX,
#endif
	I2C_NUM
} i2c_index_e;

struct i2c_info {
	volatile I2C_TypeDef *instance;
	i2c_index_e index;
	MODULE_ID_Type clk_id;
	PINMUX_Type pinmux;
	IRQn_Type irq_num;
	IRQHandler_t irq_callback;
	uint32_t dma_idx; /* DMA mode use */
};

static void CUNIT_I2C0_IRQHandler(void);
static void CUNIT_I2C1_IRQHandler(void);
static void CUNIT_I2C2_IRQHandler(void);
static void CUNIT_I2C3_IRQHandler(void);
#if defined (SP645) || defined(SP7350)
static void CUNIT_I2C4_IRQHandler(void);
static void CUNIT_I2C5_IRQHandler(void);
#endif
#ifdef SP7350
static void CUNIT_I2C6_IRQHandler(void);
static void CUNIT_I2C7_IRQHandler(void);
static void CUNIT_I2C8_IRQHandler(void);
static void CUNIT_I2C9_IRQHandler(void);
#endif

/* Private Variables */
const static struct i2c_info sp_i2c_info[] = {
	I2C_INFO(SP_I2CM0, I2C0_INDEX, I2CM0, PINMUX_I2C_0, I2C_MASTER0_IRQ, &CUNIT_I2C0_IRQHandler, PER_I2C0),
	I2C_INFO(SP_I2CM1, I2C1_INDEX, I2CM1, PINMUX_I2C_1, I2C_MASTER1_IRQ, &CUNIT_I2C1_IRQHandler, PER_I2C1),
	I2C_INFO(SP_I2CM2, I2C2_INDEX, I2CM2, PINMUX_I2C_2, I2C_MASTER2_IRQ, &CUNIT_I2C2_IRQHandler, PER_I2C2),
	I2C_INFO(SP_I2CM3, I2C3_INDEX, I2CM3, PINMUX_I2C_3, I2C_MASTER3_IRQ, &CUNIT_I2C3_IRQHandler, PER_I2C3),
#if defined (SP645) || defined (SP7350)
	I2C_INFO(SP_I2CM4, I2C4_INDEX, I2CM4, PINMUX_I2C_4, I2C_MASTER4_IRQ, &CUNIT_I2C4_IRQHandler, PER_I2C4),
	I2C_INFO(SP_I2CM5, I2C5_INDEX, I2CM5, PINMUX_I2C_5, I2C_MASTER5_IRQ, &CUNIT_I2C5_IRQHandler, PER_I2C5),
#endif
#ifdef SP7350
	I2C_INFO(SP_I2CM6, I2C6_INDEX, I2CM6, PINMUX_I2C_6, I2C_MASTER6_IRQ, &CUNIT_I2C6_IRQHandler, PER_I2C6),
	I2C_INFO(SP_I2CM7, I2C7_INDEX, I2CM7, PINMUX_I2C_7, I2C_MASTER7_IRQ, &CUNIT_I2C7_IRQHandler, PER_I2C7),
	I2C_INFO(SP_I2CM8, I2C8_INDEX, I2CM8, PINMUX_I2C_8, I2C_MASTER8_IRQ, &CUNIT_I2C8_IRQHandler, PER_I2C8),
	I2C_INFO(SP_I2CM9, I2C9_INDEX, I2CM9, PINMUX_I2C_9, I2C_MASTER9_IRQ, &CUNIT_I2C9_IRQHandler, PER_I2C9),
#endif
};

static I2C_HandleTypeDef *gpHandle[I2C_NUM];
static uint8_t *buf;
static HAL_StatusTypeDef ret;

static void i2c_irq_config(IRQn_Type irqn, IRQHandler_t callback, int irq_enable)
{
	IRQ_SetMode(irqn, IRQ_MODE_TRIG_LEVEL_HIGH);
	IRQ_SetHandler(irqn, callback);
#if defined (SP7350)
	if(irq_enable == 1)
		IRQ_Enable(irqn);
#endif
}

static void i2c_clken_config(MODULE_ID_Type clk_id)
{
	HAL_Module_Clock_enable(clk_id, 1);
	HAL_Module_Clock_gate(clk_id, 0);
	HAL_Module_Reset(clk_id, 0);
}

static void i2c_pinmux_config(PINMUX_Type pinmux)
{
	HAL_PINMUX_Cfg(pinmux, 1);
}

static void i2c_init(i2c_index_e i2c_index, int irq_enable)
{
	gpHandle[i2c_index] = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));

	/* Fill the register base address data */
	gpHandle[i2c_index]->Instance = sp_i2c_info[i2c_index].instance;
#ifdef SP7350
	gpHandle[i2c_index]->DMAIndex = sp_i2c_info[i2c_index].dma_idx;
#endif
	/* Set irq trigger mode and irq callback */
	i2c_irq_config(sp_i2c_info[i2c_index].irq_num, sp_i2c_info[i2c_index].irq_callback, irq_enable);
	/* Enable clock */
	i2c_clken_config(sp_i2c_info[i2c_index].clk_id);
	/* Config pinmux */
	i2c_pinmux_config(sp_i2c_info[i2c_index].pinmux);

	gpHandle[i2c_index]->Init.Timing = 100;
	gpHandle[i2c_index]->State = HAL_I2C_STATE_RESET;
#if 0
	printf("i2c addr 0x%08x\n", (unsigned int)gpHandle[i2c_index]->Instance);
	printf("pinmux 0x%08x(1<<3)\n", *(unsigned int *)(0xf880008c));
	printf("clken  0x%08x(1<<9)\n", *(unsigned int *)(0xf8800124));
	printf("gclken 0x%08x(0<<9)\n", *(unsigned int *)(0xf880015c));
	printf("reset  0x%08x(0<<9)\n", *(unsigned int *)(0xf8800024));
#endif
	/* Init the I2C */
	HAL_I2C_Init(gpHandle[i2c_index]);
}

static void i2c_deinit(i2c_index_e i2c_index)
{
	IRQ_Disable(sp_i2c_info[i2c_index].irq_num);
	free(gpHandle[i2c_index]);
}

static void CUNIT_I2C0_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C0_INDEX]);
}

static void CUNIT_I2C1_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C1_INDEX]);
}

static void CUNIT_I2C2_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C2_INDEX]);
}

static void CUNIT_I2C3_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C3_INDEX]);
}
#if defined(SP645) || defined(SP7350)
static void CUNIT_I2C4_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C4_INDEX]);
}

static void CUNIT_I2C5_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C5_INDEX]);
}
#endif
#ifdef SP7350
static void CUNIT_I2C6_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C6_INDEX]);
}

static void CUNIT_I2C7_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C7_INDEX]);
}

static void CUNIT_I2C8_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C8_INDEX]);
}

static void CUNIT_I2C9_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C9_INDEX]);
}
#endif
/****************************************** TEST CASE *************************************/
void test_case_i2c_tx_burst(void)
{
	buf = (uint8_t *)malloc(2);

	memset(buf, 0x55, sizeof(buf));
	i2c_init(I2C_SINGLE_TEST, IRQ_DISABLE);
	ret = HAL_I2C_Master_Transmit(gpHandle[I2C_SINGLE_TEST], TEST_SLAVE_ADDRESS, buf, 2, 0);
	i2c_deinit(I2C_SINGLE_TEST);
#ifndef QUICK_TEST
	CU_ASSERT_EQUAL(ret, HAL_OK);
#else
	if(ret != HAL_OK)
		printf(" ERR %s (%d)\n", __FUNCTION__, __LINE__);
#endif
	free(buf);
}

void test_case_i2c_rx_burst(void)
{
	buf = (uint8_t *)malloc(2);

	memset(buf, 0, sizeof(buf));
	i2c_init(I2C_SINGLE_TEST, IRQ_DISABLE);
	ret = HAL_I2C_Master_Receive(gpHandle[I2C_SINGLE_TEST], TEST_SLAVE_ADDRESS, buf, 1, 0);
	i2c_deinit(I2C_SINGLE_TEST);
#ifndef QUICK_TEST
	CU_ASSERT_EQUAL(ret, HAL_OK);
	CU_ASSERT_EQUAL(*buf, 0x82);
#else
	if(ret != HAL_OK)
		printf(" ERR %s (%d)\n", __FUNCTION__, __LINE__);
	if(*buf != 0x82)
		printf(" ERR %s (%d)\n", __FUNCTION__, __LINE__);
#endif
	free(buf);
}

void test_case_i2c_tx_intr(void)
{
	buf = (uint8_t *)malloc(2);

	memset(buf, 0x55, sizeof(buf));
	i2c_init(I2C_SINGLE_TEST, IRQ_ENABLE);
	ret = HAL_I2C_Master_Transmit_IT(gpHandle[I2C_SINGLE_TEST], TEST_SLAVE_ADDRESS, buf, 2);
	i2c_deinit(I2C_SINGLE_TEST);
#ifndef QUICK_TEST
	CU_ASSERT_EQUAL(ret, HAL_OK);
#else
	if(ret != HAL_OK)
		printf(" ERR %s (%d)\n", __FUNCTION__, __LINE__);
#endif
	free(buf);
}

void test_case_i2c_rx_intr(void)
{
	buf = (uint8_t *)malloc(2);

	memset(buf, 0, sizeof(buf));

	i2c_init(I2C_SINGLE_TEST, IRQ_ENABLE);
	ret = HAL_I2C_Master_Receive_IT(gpHandle[I2C_SINGLE_TEST], TEST_SLAVE_ADDRESS, buf, 1);
	i2c_deinit(I2C_SINGLE_TEST);
#ifndef QUICK_TEST
	CU_ASSERT_EQUAL(ret, HAL_OK);
	CU_ASSERT_EQUAL(*buf, 0x82);
#else
	if(ret != HAL_OK)
		printf(" ERR %s (%d)\n", __FUNCTION__, __LINE__);
	if(*buf != 0x82)
		printf(" ERR %s (%d)\n", __FUNCTION__, __LINE__);
#endif
	free(buf);
}

void test_case_i2c_tx_dma(void)
{
	buf = (uint8_t *)malloc(2);

	memset(buf, 0x55, sizeof(buf));
	i2c_init(I2C_SINGLE_TEST, IRQ_DISABLE);
	ret = HAL_I2C_Master_Transmit_DMA(gpHandle[I2C_SINGLE_TEST], TEST_SLAVE_ADDRESS, buf, 2);
	i2c_deinit(I2C_SINGLE_TEST);
#ifndef QUICK_TEST
	CU_ASSERT_EQUAL(ret, HAL_OK);
#else
	if(ret != HAL_OK)
		printf(" ERR %s (%d)\n", __FUNCTION__, __LINE__);
#endif
	free(buf);
}

void test_case_i2c_rx_dma(void)
{
	buf = (uint8_t *)malloc(2);

	memset(buf, 0, sizeof(buf));

	i2c_init(I2C_SINGLE_TEST, IRQ_DISABLE);
	ret = HAL_I2C_Master_Receive_DMA(gpHandle[I2C_SINGLE_TEST], TEST_SLAVE_ADDRESS, buf, 1);
	i2c_deinit(I2C_SINGLE_TEST);
#ifndef QUICK_TEST
	CU_ASSERT_EQUAL(ret, HAL_OK);
	CU_ASSERT_EQUAL(*buf, 0x82);
#else
	if(ret != HAL_OK)
		printf(" ERR %s (%d)\n", __FUNCTION__, __LINE__);
	if(*buf != 0x82)
		printf(" ERR %s (%d)\n", __FUNCTION__, __LINE__);
#endif
	free(buf);
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

