/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "WInterrupts.h"

/*
  parameter id is 0~7 correspond exti0~exti7. The pin and 
  extern intr correspondence can be modified in interrupt.h. 
*/

//void attachInterrupt1(uint32_t pin, callback_function_t callback, uint32_t mode)
void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
	uint32_t id = pin;
	uint32_t it_mode = mode;
	if((id < 0) || (id > 7))
	{
		return;
	}
	
	switch (mode)
	{
	case FALLING :
	case LOW :
		it_mode = IRQ_MODE_TRIG_LEVEL_LOW;
		break;
	case RISING :
	case HIGH :
		it_mode = IRQ_MODE_TRIG_LEVEL_HIGH;
		break;
	default:
		it_mode = IRQ_MODE_TRIG_LEVEL_HIGH;
		break;
	}

	sunplus_interrupt_enable(id, callback, it_mode);
}

//void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
//{
//	std::function<void(void)> _c = callback;
//	attachInterrupt1(pin, _c, mode);
//}

void detachInterrupt(uint32_t pin)
{
	uint32_t id = pin;
	sunplus_interrupt_disable(id);
}
