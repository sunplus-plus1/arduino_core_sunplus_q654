#include "core_debug.h"
#include "Arduino.h"

#ifdef SP7021
#include "sp7021_hal_conf.h"
#elif defined(SP645)
#include "sp645_hal_conf.h"
#endif

#include "twi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private Defines */
/// @brief I2C timout in tick unit
#ifndef I2C_TIMEOUT_TICK
#define I2C_TIMEOUT_TICK        1000 /* 1ms */
#endif

#define I2C_MAX_FREQ			400
#define I2C_INIT_FREQ			100
#define I2C_FREQ			27000

/*  Family specific description for I2C */
typedef enum {
	I2C0_INDEX,
	I2C1_INDEX,
	I2C2_INDEX,
	I2C3_INDEX,
	I2C4_INDEX,
	I2C5_INDEX,
	I2C_NUM
} i2c_index_e;

/* Private Variables */
static I2C_HandleTypeDef *i2c_handles[I2C_NUM];

void func_print(uint8_t data)
{
	printf("0x%x\n", data);
}

/**
* @brief Compute I2C timing according current I2C clock source and
* required I2C clock.
* @param  obj : pointer to i2c_t structure
* @param frequency : Required I2C clock in kHz.
* @retval I2C timing or 0 in case of error.which set in freq_custom bit.
*/
static uint32_t i2c_getTiming(i2c_t * obj, uint32_t frequency)
{

	if (frequency > I2C_MAX_FREQ) {
		frequency = I2C_MAX_FREQ;
	}

	UNUSED(obj);

	return frequency;
}

/**
  * @brief  Default init and setup GPIO and I2C peripheral
  * @param  obj : pointer to i2c_t structure
  * @retval none
  */
void i2c_init(i2c_t * obj)
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
	uint32_t i2c_sda, i2c_scl;

	UNUSED(addressingMode);
	UNUSED(ownAddress);
	if (obj == NULL)
		return;

	I2C_HandleTypeDef *handle = &(obj->handle);
#ifdef SP7021
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
#endif
	if (handle->Instance == SP_I2CM0) {
		IRQ_SetMode(I2C_MASTER0_IRQ, IRQ_MODE_TRIG_LEVEL_HIGH);
		IRQ_SetHandler(I2C_MASTER0_IRQ, I2C0_IRQHandler);

		obj->irq = I2C_MASTER0_IRQ;
		i2c_handles[I2C0_INDEX] = handle;
		i2c_handles[I2C0_INDEX]->Index = I2C0_INDEX;
		i2c_handles[I2C0_INDEX]->gdma = SP_GDMA0;
	}
	/* Enable I2C1 clock if not done */
	else if (handle->Instance == SP_I2CM1) {
		IRQ_SetMode(I2C_MASTER1_IRQ, IRQ_MODE_TRIG_LEVEL_HIGH);
		IRQ_SetHandler(I2C_MASTER1_IRQ, I2C1_IRQHandler);

		obj->irq = I2C_MASTER1_IRQ;
		i2c_handles[I2C1_INDEX] = handle;
		i2c_handles[I2C1_INDEX]->Index = I2C1_INDEX;
		i2c_handles[I2C1_INDEX]->gdma = SP_GDMA1;
	}
	/* Enable I2C2 clock if not done */
	else if (handle->Instance == SP_I2CM2) {
		IRQ_SetMode(I2C_MASTER2_IRQ, IRQ_MODE_TRIG_LEVEL_HIGH);
		IRQ_SetHandler(I2C_MASTER2_IRQ, I2C2_IRQHandler);

		obj->irq = I2C_MASTER2_IRQ;
		i2c_handles[I2C2_INDEX] = handle;
		i2c_handles[I2C2_INDEX]->Index = I2C2_INDEX;
		i2c_handles[I2C2_INDEX]->gdma = SP_GDMA2;
	}
	/* Enable I2C3 clock if not done */
	else if (handle->Instance == SP_I2CM3) {
		IRQ_SetMode(I2C_MASTER3_IRQ, IRQ_MODE_TRIG_LEVEL_HIGH);
		IRQ_SetHandler(I2C_MASTER3_IRQ, I2C3_IRQHandler);

		obj->irq = I2C_MASTER3_IRQ;
		i2c_handles[I2C3_INDEX] = handle;
		i2c_handles[I2C3_INDEX]->Index = I2C3_INDEX;
		i2c_handles[I2C3_INDEX]->gdma = SP_GDMA3;
	}
#ifdef SP645			/* Enable I2C2 clock if not done */
	else if (handle->Instance == SP_I2CM4) {
		IRQ_SetMode(I2C_MASTER4_IRQ, IRQ_MODE_TRIG_LEVEL_HIGH);
		IRQ_SetHandler(I2C_MASTER4_IRQ, I2C4_IRQHandler);

		obj->irq = I2C_MASTER4_IRQ;
		i2c_handles[I2C4_INDEX] = handle;
		i2c_handles[I2C4_INDEX]->Index = I2C4_INDEX;
		i2c_handles[I2C4_INDEX]->gdma = SP_GDMA4;

	}
	/* Enable I2C3 clock if not done */
	else if (handle->Instance == SP_I2CM5) {
		IRQ_SetMode(I2C_MASTER5_IRQ, IRQ_MODE_TRIG_LEVEL_HIGH);
		IRQ_SetHandler(I2C_MASTER5_IRQ, I2C5_IRQHandler);

		obj->irq = I2C_MASTER5_IRQ;
		i2c_handles[I2C5_INDEX] = handle;
		i2c_handles[I2C5_INDEX]->Index = I2C5_INDEX;
		i2c_handles[I2C5_INDEX]->gdma = SP_GDMA5;
	}
#endif
	//FIXME:enable config error,
	HAL_Module_Clock_enable(I2CM0 + handle->Index, 1);
	HAL_Module_Clock_gate(I2CM0 + handle->Index, 1);
	HAL_Module_Reset(I2CM0 + handle->Index, 0);
#ifdef SP7021
	HAL_PINMUX_Cfg(PINMUX_I2CM0_SDA + handle->Index * 2, i2c_sda);
	HAL_PINMUX_Cfg(PINMUX_I2CM0_SCL + handle->Index * 2, i2c_scl);
#elif defined(SP645)
	HAL_PINMUX_Cfg(PINMUX_I2C_0 + handle->Index, 1);
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
#elif defined (IT_MODE)
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
#elif defined (IT_MODE)
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

void I2C0_IRQHandler()
{
	HAL_I2C_IRQHandler(i2c_handles[I2C0_INDEX]);
}

void I2C1_IRQHandler()
{
	HAL_I2C_IRQHandler(i2c_handles[I2C1_INDEX]);
}

void I2C2_IRQHandler()
{
	HAL_I2C_IRQHandler(i2c_handles[I2C2_INDEX]);
}

void I2C3_IRQHandler()
{
	HAL_I2C_IRQHandler(i2c_handles[I2C3_INDEX]);
}

void I2C4_IRQHandler()
{
	HAL_I2C_IRQHandler(i2c_handles[I2C4_INDEX]);
}

void I2C5_IRQHandler()
{
	HAL_I2C_IRQHandler(i2c_handles[I2C5_INDEX]);
}

#ifdef __cplusplus
}
#endif
