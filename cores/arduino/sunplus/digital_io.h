
#ifndef __DIGITAL_IO_H
#define __DIGITAL_IO_H

/* Includes ------------------------------------------------------------------*/
#include "wiring_constants.h"
#include "sunplus_def.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void digital_io_write(uint32_t pin, uint32_t val)
{
  HAL_GPIO_WritePin(pin, val);
}


static inline uint32_t digital_io_read(uint32_t pin)
{
  return HAL_GPIO_ReadPin(pin);
}

static inline void digital_io_toggle( uint32_t pin)
{
  HAL_GPIO_TogglePin(pin);
}

static inline void digitalWriteFast(uint32_t pn, uint32_t ulVal)
{
  digital_io_write(pn, ulVal);
}


static inline int digitalReadFast(uint32_t pn)
{
  uint8_t level = 0;
  level = digital_io_read(pn);
  return (level) ? HIGH : LOW;
}

static inline void digitalToggleFast(uint32_t pn)
{
  digital_io_toggle(pn);
}

static inline void digitalModeSet(uint32_t ulPin, uint32_t ulMode)
{
    GPIO_InitTypeDef GPIO_Init;
    memset(&GPIO_Init,0,sizeof(GPIO_InitTypeDef));

    switch (ulMode) {
      case INPUT:
        GPIO_Init.Mode = GPIO_INPUT_MODE;
        break;
      case OUTPUT:
	  	#ifdef SP7021
        GPIO_Init.Mode = GPIO_OUTPUT_MODE;
		#elif defined(SP645) || defined(SP7350)
		GPIO_Init.out_value = 1; 
		GPIO_Init.Mode = GPIO_PP_OUTPUT_MODE;
		#endif
        break;
      case OUTPUT_OPEN_DRAIN:
	  	#ifdef SP7021
        GPIO_Init.opendrain = GPIO_OD_ENABLE;
		#elif defined(SP645) || defined(SP7350)
		GPIO_Init.out_value = 1; 
		GPIO_Init.Mode = GPIO_OD_OUTPUT_MODE;
		#endif
        break;
      default:
        return;
    }
  GPIO_Init.Pin = ulPin;
  HAL_GPIO_Init(&GPIO_Init);

}

static inline int digitalModeGet(uint32_t ulPin)
{
  return HAL_GPIO_Get_Mode(ulPin);
}


#ifdef __cplusplus
}
#endif

#endif 
