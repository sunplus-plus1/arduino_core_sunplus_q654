#ifndef __AO_INTERRUPT_H__
#define __AO_INTERRUPT_H__

#include "sp7350_hal_exti.h"
#include "sp7350_hal.h"

void ao_interrupt_enable(uint16_t pin, void (*callback)(void), uint32_t mode);
void ao_interrupt_disable(uint16_t pin);

void GPIO_AO_INT0_IRQHandler(void);
void GPIO_AO_INT1_IRQHandler(void);
void GPIO_AO_INT2_IRQHandler(void);
void GPIO_AO_INT3_IRQHandler(void);
void GPIO_AO_INT4_IRQHandler(void);
void GPIO_AO_INT5_IRQHandler(void);
void GPIO_AO_INT6_IRQHandler(void);
void GPIO_AO_INT7_IRQHandler(void);
void GPIO_AO_INT8_IRQHandler(void);
void GPIO_AO_INT9_IRQHandler(void);
void GPIO_AO_INT10_IRQHandler(void);
void GPIO_AO_INT11_IRQHandler(void);
void GPIO_AO_INT12_IRQHandler(void);
void GPIO_AO_INT13_IRQHandler(void);
void GPIO_AO_INT14_IRQHandler(void);
void GPIO_AO_INT15_IRQHandler(void);
void GPIO_AO_INT16_IRQHandler(void);
void GPIO_AO_INT17_IRQHandler(void);
void GPIO_AO_INT18_IRQHandler(void);
void GPIO_AO_INT19_IRQHandler(void);
void GPIO_AO_INT20_IRQHandler(void);
void GPIO_AO_INT21_IRQHandler(void);
void GPIO_AO_INT22_IRQHandler(void);
void GPIO_AO_INT23_IRQHandler(void);
void GPIO_AO_INT24_IRQHandler(void);
void GPIO_AO_INT25_IRQHandler(void);
void GPIO_AO_INT26_IRQHandler(void);
void GPIO_AO_INT27_IRQHandler(void);
void GPIO_AO_INT28_IRQHandler(void);
void GPIO_AO_INT29_IRQHandler(void);
void GPIO_AO_INT30_IRQHandler(void);
void GPIO_AO_INT31_IRQHandler(void);

#endif
