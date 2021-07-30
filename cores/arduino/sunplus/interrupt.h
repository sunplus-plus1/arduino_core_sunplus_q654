#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "sp7021_hal_exti.h"
#include "sp7021_hal.h"

#define GPIO_EXTI(x) GPIO_EXTI##x

#define GPIO_EXTI0 39
#define GPIO_EXTI1 38
#define GPIO_EXTI2 37
#define GPIO_EXTI3 36
#define GPIO_EXTI4 35
#define GPIO_EXTI5 34
#define GPIO_EXTI6 33
#define GPIO_EXTI7 32

typedef struct
{;}
GPIO_TypeDef;

void sunplus_interrupt_enable(GPIO_TypeDef *port, uint16_t pin, void (*callback)(void), uint32_t mode);
void sunplus_interrupt_disable(GPIO_TypeDef *port, uint16_t pin);

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI5_IRQHandler(void);
void EXTI6_IRQHandler(void);
void EXTI7_IRQHandler(void);

#endif

