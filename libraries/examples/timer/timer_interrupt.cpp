#include "Arduino.h"
/*
	note to develop:
	only use timer2 and timer3 of every STC
	e.x. TIM0 is timer2 of STC
		 TIM1 is timer3 of STC
		 TIM2 is timer2 of STC_AV0
		 TIM3 is timer3 of STC_AV0 and so on....
*/
uint32_t start = 0;
uint32_t ticks = 2;

//IWatchdogClass *wdg1;

void tim0_callback(void)
{

	printf("@@@@@@@timer1 callback @@@@@@@, tick= %lu\n", HAL_GetTick());
	start = HAL_GetTick();
	//printf("start:%lu\n", start);

//	wdg1->reload();
//	printf("feed dog1!\n");

	//wdg0->get(&ticks, NULL);
	//printf("!!!get current timeout %d\n", ticks);
}

void setup()
{
#if 1
  	uint32_t u32Prescal = 0xFFFF;
 	//HAL_Init();
 	HardwareTimer *timer0 = new HardwareTimer(TIM1);
	timer0->setPrescaleFactor(60000);// 10kHz
	//timer0->setPrescaleFactor(1000);
	u32Prescal = timer0->getPrescaleFactor();
	printf("u32Prescal= 0x%x\n", u32Prescal);
	printf("freq = %d\n", timer0->getTimerClkFreq());
	timer0->setCount(15000, TICK_FORMAT); // 15000 / 10000 = 1.5s
	timer0->setOverflow(15000, TICK_FORMAT);
	//timer0->setCount(999, TICK_FORMAT);
	printf("counter = %d\n", timer0->getCount(TICK_FORMAT));
	timer0->attachInterrupt(tim0_callback);
	start = (uint32_t)HAL_GetTick();
	timer0->resume();
#endif

//	wdg1 = new IWatchdogClass(WDG1);
//	wdg1->begin(2, 0); // param 0 is unused

}


void loop()
{
	//printf("%s, %s\n", __FILE__, __FUNCTION__);
}

