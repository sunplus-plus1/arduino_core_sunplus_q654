#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "sp7021_hal_exti.h"

#define LOW     0x0
#define HIGH    0x1
#define CHANGE  0x2
#define FALLING 0x3
#define RISING  0x4

#define GPIO_EXTI(x) GPIO_EXTI##x

#define GPIO_EXTI0 39
#define GPIO_EXTI1 38
#define GPIO_EXTI2 37
#define GPIO_EXTI3 36
#define GPIO_EXTI4 35
#define GPIO_EXTI5 34
#define GPIO_EXTI6 33
#define GPIO_EXTI7 32

#define EXTI_IRQ IRQ_TYPE_IRQ

#ifdef __cplusplus
	#include <functional>
	
	typedef std::function<void(void)> callback_function_t;
	void sunplus_interrupt_enable(uint32_t id, callback_function_t callback, uint32_t mode);
#endif

void sunplus_interrupt_enable(uint32_t id, void (*callback)(void), uint32_t mode);
void sunplus_interrupt_disable(uint32_t id);

#endif
