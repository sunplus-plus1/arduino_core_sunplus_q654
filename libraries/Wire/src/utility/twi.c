#include "core_debug.h"
#include "Arduino.h"
#include "sp7350_hal_conf.h"
#include "twi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	I2C_MAX_FREQ		(I2C_MAX_FAST_MODE_PLUS_FREQ / 1000)

#ifndef I2C_TIMEOUT_TICK
#define I2C_TIMEOUT_TICK	0xffffffff
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

	handle->Init.Timing = i2c_getTiming(obj, timing) * 1000;
	handle->State = HAL_I2C_STATE_RESET;
	handle->Mode = I2C_MODE_INTR;//I2C_MODE_BURST

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

	if (HAL_I2C_Master_Transmit(&(obj->handle), dev_address, data, size, 0xffff) == HAL_OK) {
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

	if (HAL_I2C_Master_Receive(&(obj->handle), dev_address, data, size, 0xffff) == HAL_OK) {
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

#ifdef __cplusplus
}
#endif
