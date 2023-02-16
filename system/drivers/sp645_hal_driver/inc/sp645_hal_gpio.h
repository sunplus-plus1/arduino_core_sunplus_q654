#ifndef __SP645_HAL_GPIO_H
#define __SP645_HAL_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp645_hal_def.h"
#include "sp645_hal_conf.h"
#include "sp64xx.h"

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */
typedef enum
{
  GPIO_OUT_LOW = 0,      // gpio set to low
  GPIO_OUT_HIGH          // gpio set to high
}GPIO_PinState;

typedef enum
{
  GPIO_INPUT_MODE = 0,      // set to input mode
  GPIO_PP_OUTPUT_MODE,      // set to PP-output mode
  GPIO_OD_OUTPUT_MODE       // set to OD-output mode
}GPIO_PinMode;

typedef enum
{
  GPIO_PULL_DISABLE = 0,	  // pull disable 
  GPIO_PULL_UP,			      // pull up
  GPIO_PULL_DOWN,		      // pull down
}GPIO_PULLState;

typedef enum
{
  GPIO_DS_DISABLE,         // driver selector disable
  GPIO_DS_0 = 0,          // driver selector level 0
  GPIO_DS_1,              // driver selector level 1
  GPIO_DS_2,              // driver selector level 2
  GPIO_DS_3,              // driver selector level 3
}GPIO_DSState;

typedef struct
{
  /* set input/output mode */
  GPIO_PinMode Mode;
  /* set default value.for iutput mode   */ 
  GPIO_PULLState pull;
  /* set output default value after gpio init */
  GPIO_PinState out_value;
  /* input/output invert?   0:nomarl  1: invert */
  uint8_t  invert;
  /* set pin number: one of GPIO_P0_00--GPIO_P8_07  */
  uint32_t Pin;
}GPIO_InitTypeDef;



#define IS_GPIO_PIN_ACTION(pin)   gpio_first_val_get(pin)

HAL_StatusTypeDef              HAL_GPIO_Init(GPIO_InitTypeDef *GPIO_Init);
HAL_StatusTypeDef              HAL_GPIO_DeInit(uint16_t GPIO_Pin);
GPIO_PinState     HAL_GPIO_ReadPin(uint16_t GPIO_Pin);
HAL_StatusTypeDef              HAL_GPIO_WritePin(uint16_t GPIO_Pin, GPIO_PinState PinState);
HAL_StatusTypeDef              HAL_GPIO_TogglePin(uint16_t GPIO_Pin);
int               HAL_GPIO_Get_Mode(uint16_t GPIO_Pin);


#ifdef __cplusplus
}
#endif
#endif
