#include "core_debug.h"
#include "Arduino.h"

#ifdef SP7021
#include "sp7021_hal_conf.h"
#elif defined(SP645)
#include "sp645_hal_conf.h"
#elif defined(SP7350)
#include "sp7350_hal_conf.h"
#endif

#include "twi.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SP7350//////////////
#define	I2C_MAX_FREQ		(I2C_MAX_FAST_MODE_PLUS_FREQ / 1000)

#ifndef I2C_TIMEOUT_TICK
#define I2C_TIMEOUT_TICK	0xffffffff
#endif


#else//////////////////


#ifdef SP7021
#define PINMUX_I2C_0 PINMUX_I2CM0_SCL
#define PINMUX_I2C_1 PINMUX_I2CM1_SCL
#define PINMUX_I2C_2 PINMUX_I2CM2_SCL
#define PINMUX_I2C_3 PINMUX_I2CM3_SCL
#endif
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
/* Private Defines */
/// @brief I2C timout in tick unit
#ifndef I2C_TIMEOUT_TICK
#define I2C_TIMEOUT_TICK	0xffffffff
#endif

#ifdef SP7350
#define	I2C_MAX_FREQ		(I2C_MAX_FAST_MODE_PLUS_FREQ / 1000)
#else
#define I2C_MAX_FREQ		400
#endif
#define I2C_INIT_FREQ		100

#define I2C_INFO(inst, idx, clk, pin, irqn, cb, dma) \
	{ .instance = (inst), .index = (idx), \
	  .clk_id = (clk), .pinmux = (pin), \
	  .irq_num = (irqn), .irq_callback = (cb), \
	  .dma_idx = (dma) }

struct i2c_info {
	volatile I2C_TypeDef *instance;
	i2c_index_e index;
	MODULE_ID_Type clk_id;
	PINMUX_Type pinmux;
	IRQn_Type irq_num;
	IRQHandler_t irq_callback;
	uint32_t dma_idx; /* DMA mode use */
};

/* Private Variables */
const static struct i2c_info sp_i2c_info[] = {
	I2C_INFO(SP_I2CM0, I2C0_INDEX, I2CM0, PINMUX_I2C_0, I2C_MASTER0_IRQ, &I2C0_IRQHandler, PER_I2C0),
	I2C_INFO(SP_I2CM1, I2C1_INDEX, I2CM1, PINMUX_I2C_1, I2C_MASTER1_IRQ, &I2C1_IRQHandler, PER_I2C1),
	I2C_INFO(SP_I2CM2, I2C2_INDEX, I2CM2, PINMUX_I2C_2, I2C_MASTER2_IRQ, &I2C2_IRQHandler, PER_I2C2),
	I2C_INFO(SP_I2CM3, I2C3_INDEX, I2CM3, PINMUX_I2C_3, I2C_MASTER3_IRQ, &I2C3_IRQHandler, PER_I2C3),
#if defined (SP645) || defined (SP7350)
	I2C_INFO(SP_I2CM4, I2C4_INDEX, I2CM4, PINMUX_I2C_4, I2C_MASTER4_IRQ, &I2C4_IRQHandler, PER_I2C4),
	I2C_INFO(SP_I2CM5, I2C5_INDEX, I2CM5, PINMUX_I2C_5, I2C_MASTER5_IRQ, &I2C5_IRQHandler, PER_I2C5),
#endif
#ifdef SP7350
	I2C_INFO(SP_I2CM6, I2C6_INDEX, I2CM6, PINMUX_I2C_6, I2C_MASTER6_IRQ, &I2C6_IRQHandler, PER_I2C6),
	I2C_INFO(SP_I2CM7, I2C7_INDEX, I2CM7, PINMUX_I2C_7, I2C_MASTER7_IRQ, &I2C7_IRQHandler, PER_I2C7),
	I2C_INFO(SP_I2CM8, I2C8_INDEX, I2CM8, PINMUX_I2C_8, I2C_MASTER8_IRQ, &I2C8_IRQHandler, PER_I2C8),
	I2C_INFO(SP_I2CM9, I2C9_INDEX, I2CM9, PINMUX_I2C_9, I2C_MASTER9_IRQ, &I2C9_IRQHandler, PER_I2C9),
#endif
};

/* Just for IRQHandler() */
I2C_HandleTypeDef *gpHandle[I2C_NUM];

static void i2c_detect(I2C_HandleTypeDef *handle)
{
	int i;

	for(i = 0; i < I2C_NUM; i++) {
		if(handle->Instance == sp_i2c_info[i].instance) {
			handle->Index = sp_i2c_info[i].index;
#ifdef SP7350
			handle->DMAIndex = sp_i2c_info[i].dma_idx;
#endif
			break;
		}
	}
}

static void i2c_irq_config(IRQn_Type irqn, IRQHandler_t callback)
{
	//IRQ_Disable(irqn);
	IRQ_SetMode(irqn, IRQ_MODE_TRIG_LEVEL_HIGH);
	IRQ_SetHandler(irqn, callback);
#if defined (SP7350) && defined (INTR_MODE)
	IRQ_Enable(irqn);
#endif
}

static void i2c_clken_config(MODULE_ID_Type clk_id)
{
	HAL_Module_Clock_enable(clk_id, 1);
	HAL_Module_Clock_gate(clk_id, 0);
	HAL_Module_Reset(clk_id, 0);
}

/* TODO: refine the code of pinmux config */
static void i2c_pinmux_config(PINMUX_Type pinmux, i2c_t *obj)
{
#ifdef SP7021
	uint32_t i2c_sda, i2c_scl;

	i2c_sda = GPIO_TO_PINMUX(obj->pin_sda);
	if (!IS_VALID_PINMUX(i2c_sda)) {
		core_debug("ERROR: [I2C] SDA pin error!\n");
		return;
	}

	i2c_scl = GPIO_TO_PINMUX(obj->pin_scl);
	if (!IS_VALID_PINMUX(i2c_scl)) {
		core_debug("ERROR: [I2C] SCL pin error!\n");
		return;
	}

	HAL_PINMUX_Cfg(pinmux + 1, i2c_sda);
	HAL_PINMUX_Cfg(pinmux, i2c_scl);
#else
	UNUSED(obj);
	HAL_PINMUX_Cfg(pinmux, 1);
#endif
}
#endif
/**
* @brief Compute I2C timing according current I2C clock source and
* required I2C clock.
* @param  obj : pointer to i2c_t structure
* @param frequency : Required I2C clock in kHz.
* @retval I2C timing or 0 in case of error.which set in freq_custom bit.
*/
static uint32_t i2c_getTiming(i2c_t * obj, uint32_t frequency)
{
	UNUSED(obj);

	if (frequency > I2C_MAX_FREQ) {
		frequency = I2C_MAX_FREQ;
	}

	return frequency;
}

/**
  * @brief  Default init and setup GPIO and I2C peripheral
  * @param  obj : pointer to i2c_t structure
  * @retval none
  */
void i2c_init(i2c_t *obj)
{
	//printf("%d   %d\n", temptest.pin_sda, temptest.pin_scl);
	i2c_custom_init(obj, 100, 0, 0x33);
}

/**
  * @brief  Initialize and setup GPIO and I2C peripheral
  * @param  obj : pointer to i2c_t structure
  * @param  timing : one of the i2c_timing_e
  * @param  addressingMode : I2C_ADDRESSINGMODE_7BIT or I2C_ADDRESSINGMODE_10BIT
  * @param  ownAddress : device address
  * @retval none
  */
void i2c_custom_init(i2c_t *obj, uint32_t timing, uint32_t addressingMode, uint32_t ownAddress)
{
	uint32_t index;

	UNUSED(addressingMode);
	UNUSED(ownAddress);
	if (obj == NULL)
		return;

	I2C_HandleTypeDef *handle = &(obj->handle);
#ifndef SP7350
	/* Match the i2c information */
	i2c_detect(handle);
	/* Set irq trigger mode and irq callback */
	i2c_irq_config(sp_i2c_info[handle->Index].irq_num, sp_i2c_info[handle->Index].irq_callback);
	/* Enable clock */
	i2c_clken_config(sp_i2c_info[handle->Index].clk_id);
	/* Config pinmux */
	i2c_pinmux_config(sp_i2c_info[handle->Index].pinmux, obj);
	/* Just for i2c_deinit() */
	obj->irq = sp_i2c_info[handle->Index].irq_num;
	/* Just for IRQHandler() */
	gpHandle[handle->Index] = handle;
#endif
	handle->Init.Timing = i2c_getTiming(obj, timing);
	handle->State = HAL_I2C_STATE_RESET;

	/* Init the I2C */
	HAL_I2C_Init(handle);
}

/**
  * @brief  Initialize and setup GPIO and I2C peripheral
  * @param  obj : pointer to i2c_t structure
  * @retval none
  */
void i2c_deinit(i2c_t * obj)
{
	if (obj == NULL) {
		return;
	}
	IRQ_Disable(obj->irq);
	HAL_I2C_DeInit(&(obj->handle));
}

/**
  * @brief  Setup transmission speed. I2C must be configured before.
  * @param  obj : pointer to i2c_t structure
  * @param  frequency : i2c transmission speed
  * @retval none
  */
void i2c_setTiming(i2c_t * obj, uint32_t frequency)
{
	obj->handle.Init.Timing = i2c_getTiming(obj, frequency);
	HAL_I2C_Init(&(obj->handle));
}

/**
  * @brief  Write bytes at a given address
  * @param  obj : pointer to i2c_t structure
  * @param  dev_address: specifies the address of the device.
  * @param  data: pointer to data to be write
  * @param  size: number of bytes to be write.
  * @retval read status
  */
i2c_status_e i2c_master_write(i2c_t * obj, uint8_t dev_address, uint8_t * data, uint16_t size)
{
	i2c_status_e ret = I2C_OK;
	uint32_t tickstart = HAL_GetTick();
	uint32_t delta = 0;
	uint32_t err = 0;

#if defined(BURST_MODE)
	if (HAL_I2C_Master_Transmit(&(obj->handle), dev_address, data, size, 0xffff) == HAL_OK) {
#elif defined (INTR_MODE)
	if (HAL_I2C_Master_Transmit_IT(&(obj->handle), dev_address, data, size) == HAL_OK) {
#elif defined(DMA_MODE)
	if (HAL_I2C_Master_Transmit_DMA(&(obj->handle), dev_address, data, size) == HAL_OK) {
#elif defined(DMA_IT_MODE)
	if (HAL_I2C_Master_Transmit_DMA_IT(&(obj->handle), dev_address, data, size) == HAL_OK) {
#endif
		// wait for transfer completion
		while ((HAL_I2C_GetState(&(obj->handle)) != HAL_I2C_STATE_READY)
		       && (delta < I2C_TIMEOUT_TICK)) {
			delta = (HAL_GetTick() - tickstart);
			if (HAL_I2C_GetError(&(obj->handle)) != HAL_I2C_ERR_NONE)
				break;
		}
	}

	err = HAL_I2C_GetError(&(obj->handle));

	if ((delta >= I2C_TIMEOUT_TICK) || (err & HAL_I2C_ERR_TIMEOUT)) {
		ret = I2C_TIMEOUT;
	} else {
		if (err & HAL_I2C_ERR_ADDRESS_NACK) {
			ret = I2C_NACK_ADDR;
		} else if (err & HAL_I2C_ERR_RECEIVE_NACK) {
			ret = I2C_NACK_DATA;
		} else if (err != HAL_I2C_ERR_NONE) {
			ret = I2C_ERROR;
		}
	}

	HAL_I2C_ClearError(&(obj->handle));

	return ret;
}

/**
  * @brief  read bytes in master mode at a given address
  * @param  obj : pointer to i2c_t structure
  * @param  dev_address: specifies the address of the device.
  * @param  data: pointer to data to be read
  * @param  size: number of bytes to be read.
  * @retval read status
  */
i2c_status_e i2c_master_read(i2c_t * obj, uint8_t dev_address, uint8_t * data, uint16_t size)
{
	i2c_status_e ret = I2C_OK;
	uint32_t tickstart = HAL_GetTick();
	uint32_t delta = 0;
	uint32_t err = 0;

#if defined(BURST_MODE)
	if (HAL_I2C_Master_Receive(&(obj->handle), dev_address, data, size, 0xffff) == HAL_OK) {
#elif defined(INTR_MODE)
	if (HAL_I2C_Master_Receive_IT(&(obj->handle), dev_address, data, size) == HAL_OK) {
#elif defined(DMA_MODE)
	if (HAL_I2C_Master_Receive_DMA(&(obj->handle), dev_address, data, size) == HAL_OK) {
#elif defined(DMA_IT_MODE)
	if (HAL_I2C_Master_Receive_DMA_IT(&(obj->handle), dev_address, data, size) == HAL_OK) {
#endif
		uint32_t state = obj->handle.State;
		/* wait for transfer completion */
		while ((HAL_I2C_GetState(&(obj->handle)) != HAL_I2C_STATE_READY)
		       && (delta < I2C_TIMEOUT_TICK)) {
			delta = (HAL_GetTick() - tickstart);
			if (HAL_I2C_GetError(&(obj->handle)) != HAL_I2C_ERR_NONE) {
				break;
			}
		}
	}

	err = HAL_I2C_GetError(&(obj->handle));

	if ((delta >= I2C_TIMEOUT_TICK)
	    || ((err & HAL_I2C_ERR_TIMEOUT) == HAL_I2C_ERR_TIMEOUT)) {
		ret = I2C_TIMEOUT;
	} else {
		if ((err & HAL_I2C_ERR_ADDRESS_NACK) == HAL_I2C_ERR_ADDRESS_NACK) {
			ret = I2C_NACK_ADDR;
		}
		if ((err & HAL_I2C_ERR_RECEIVE_NACK) == HAL_I2C_ERR_RECEIVE_NACK) {
			ret = I2C_NACK_DATA;
		} else if (err != HAL_I2C_ERR_NONE) {
			ret = I2C_ERROR;
		}
	}

	HAL_I2C_ClearError(&(obj->handle));

	return ret;
}

/* Aim of the function is to get i2c_s pointer using hi2c pointer */
/* Highly inspired from magical linux kernel's "container_of" */
/* (which was not directly used since not compatible with IAR toolchain) */
i2c_t *get_i2c_obj(I2C_HandleTypeDef * hi2c)
{
	struct i2c_s *obj_s;
	i2c_t *obj;

	obj_s = (struct i2c_s *)((char *)hi2c - offsetof(struct i2c_s, handle));
	obj = (i2c_t *) ((char *)obj_s - offsetof(i2c_t, handle));

	return (obj);
}
#ifndef SP7350
void I2C0_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C0_INDEX]);
}

void I2C1_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C1_INDEX]);
}

void I2C2_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C2_INDEX]);
}

void I2C3_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C3_INDEX]);
}
#if defined(SP645) || defined(SP7350)
void I2C4_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C4_INDEX]);
}

void I2C5_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C5_INDEX]);
}
#endif
#ifdef SP7350
void I2C6_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C6_INDEX]);
}

void I2C7_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C7_INDEX]);
}

void I2C8_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C8_INDEX]);
}

void I2C9_IRQHandler()
{
	HAL_I2C_IRQHandler(gpHandle[I2C9_INDEX]);
}
#endif
#endif
#ifdef __cplusplus
}
#endif
