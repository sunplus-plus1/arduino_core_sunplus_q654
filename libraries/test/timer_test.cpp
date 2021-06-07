#include "HardwareTimer.h"


#ifdef __cplusplus
extern "C" {
#endif


void tim0_callback(void)
{
	printf("@@@@@@@timer0 callback @@@@@@@\n");

}

void tim1_callback(void)
{
	printf("@@@@@@@timer1 callback @@@@@@@\n");

}


void loop(void)
{
	uint32_t u32Prescal = 0xFFFF;
	
	HardwareTimer *timer0 = new HardwareTimer(TIM0);
//	LL_TIM_SetClockSource(TIM0, CLK_SLAVE_WRAP_SRC);
	HardwareTimer *timer1 = new HardwareTimer(TIM1);
	//LL_TIM_SetPrescaler(TIM1, 1);
	timer1->setCount(2, TICK_FORMAT);
	timer0->setPrescaleFactor(202000);
	u32Prescal = timer0->getPrescaleFactor();
	printf("u32Prescal= 0x%x\n", u32Prescal);
	printf("freq = %d\n", timer0->getTimerClkFreq());
	timer0->setCount(1000, MICROSEC_FORMAT);
	printf("counter = %d\n", timer0->getCount(TICK_FORMAT));
	timer0->attachInterrupt(tim0_callback);
	timer0->resume();
	//timer1->setPrescaleFactor(202000);
	//timer1->setCount(1000, TICK_FORMAT);
	//timer1->attachInterrupt(tim1_callback);
	//timer1->resume();
}







#ifdef __cplusplus
}
#endif


