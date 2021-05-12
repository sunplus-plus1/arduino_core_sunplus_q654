#ifndef __SP7021_HAL_GPIO_H
#define __SP7021_HAL_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7021_hal_def.h"

typedef struct
{
  u8 Mode;             /* 0:input          1:output                                     */  
  u8 opendrain;        /* 0:disable        1:enable ;set opendrain the Mode is unused   */
  u8 default_value;    /* 0:low            1:high                                       */
  u32 Pin;             /* pin number: one of pin_0--pin_71                              */ 
}GPIO_InitTypeDef;

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
  GPIO_OUTPUT_MODE           // set to output mode
}GPIO_PinMode;

typedef enum
{
  GPIO_OD_DISABLE = 0,      // open drain disable 
  GPIO_OD_ENABLE             // open drain enable 
}GPIO_ODState;


typedef void          GPIO_TypeDef;

#define GPIO_P0_00    0
#define GPIO_P0_01    1
#define GPIO_P0_02    2
#define GPIO_P0_03    3
#define GPIO_P0_04    4
#define GPIO_P0_05    5
#define GPIO_P0_06    6
#define GPIO_P0_07    7
#define GPIO_P1_00    8
#define GPIO_P1_01    9
#define GPIO_P1_02    10
#define GPIO_P1_03    11
#define GPIO_P1_04    12
#define GPIO_P1_05    13
#define GPIO_P1_06    14
#define GPIO_P1_07    15
#define GPIO_P2_00    16
#define GPIO_P2_01    17
#define GPIO_P2_02    18
#define GPIO_P2_03    19
#define GPIO_P2_04    20
#define GPIO_P2_05    21
#define GPIO_P2_06    22
#define GPIO_P2_07    23
#define GPIO_P3_00    24
#define GPIO_P3_01    25
#define GPIO_P3_02    26
#define GPIO_P3_03    27
#define GPIO_P3_04    28
#define GPIO_P3_05    29
#define GPIO_P3_06    30
#define GPIO_P3_07    31
#define GPIO_P4_00    32
#define GPIO_P4_01    33
#define GPIO_P4_02    34
#define GPIO_P4_03    35
#define GPIO_P4_04    36
#define GPIO_P4_05    37
#define GPIO_P4_06    38
#define GPIO_P4_07    39
#define GPIO_P5_00    40
#define GPIO_P5_01    41
#define GPIO_P5_02    42
#define GPIO_P5_03    43
#define GPIO_P5_04    44
#define GPIO_P5_05    45
#define GPIO_P5_06    46
#define GPIO_P5_07    47
#define GPIO_P6_00    48
#define GPIO_P6_01    49
#define GPIO_P6_02    50
#define GPIO_P6_03    51
#define GPIO_P6_04    52
#define GPIO_P6_05    53
#define GPIO_P6_06    54
#define GPIO_P6_07    55
#define GPIO_P7_00    56
#define GPIO_P7_01    57
#define GPIO_P7_02    58
#define GPIO_P7_03    59
#define GPIO_P7_04    60
#define GPIO_P7_05    61
#define GPIO_P7_06    62
#define GPIO_P7_07    63
#define GPIO_P8_00    64
#define GPIO_P8_01    65
#define GPIO_P8_02    66
#define GPIO_P8_03    67
#define GPIO_P8_04    68
#define GPIO_P8_05    69
#define GPIO_P8_06    70
#define GPIO_P8_07    71

  
#define IS_GPIO_PIN_ACTION(pin)   gpio_first_val_get(pin)
#define IS_GPIO_PIN(pin)  (((pin) >= GPIO_P0_00) && ((pin) <= GPIO_P8_07))

#define gpio_assert_param(expr)   ((expr) ? (void)0 : (printf("[ERROR] %s %d\n",__FUNCTION__, __LINE__)))


struct g6_regs {
  unsigned int sft_cfg[32];
};

struct g7_regs {
  unsigned int sft_cfg[32];
};

struct g101_regs {
  unsigned int sft_cfg[32];
};

#define G6_REG ((volatile struct g6_regs *)RF_GRP(6, 0))
#define G7_REG ((volatile struct g7_regs *)RF_GRP(7, 0))
#define G101_REG ((volatile struct g101_regs *)RF_GRP(101, 0))

#define GPIO_FIRST(X)   (RF_GRP(101, (25+X)))
#define GPIO_MASTER(X)  (RF_GRP(6, (0+X)))
#define GPIO_OE(X)      (RF_GRP(6, (8+X)))
#define GPIO_OUT(X)     (RF_GRP(6, (16+X)))
#define GPIO_IN(X)      (RF_GRP(6, (24+X)))
#define GPIO_I_INV(X)   (RF_GRP(7, (0+X)))
#define GPIO_O_INV(X)   (RF_GRP(7, (8+X)))
#define GPIO_OD(X)      (RF_GRP(7, (16+X)))

void              HAL_GPIO_Init(GPIO_InitTypeDef *GPIO_Init);
void              HAL_GPIO_DeInit(u16 GPIO_Pin);
GPIO_PinState     HAL_GPIO_ReadPin(u16 GPIO_Pin);
void              HAL_GPIO_WritePin(u16 GPIO_Pin, GPIO_PinState PinState);
void              HAL_GPIO_TogglePin(u16 GPIO_Pin);
HAL_StatusTypeDef HAL_GPIO_LockPin(u16 GPIO_Pin);
void              HAL_GPIO_EXTI_IRQHandler(u16 GPIO_Pin);
void              HAL_GPIO_EXTI_Callback(u16 GPIO_Pin);

#ifdef __cplusplus
}
#endif
#endif
