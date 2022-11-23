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

//#define WDG_TEST

#ifdef WDG_TEST
uint32_t wdg_remain_time = 2;
IWatchdogClass *wdg1;
#endif

void tim0_callback(void)
{

	printf("@@@@@@@timer1 callback @@@@@@@, tick= %lu\n", HAL_GetTick());
	start = HAL_GetTick();
	//printf("start:%lu\n", start);

#ifdef WDG_TEST
	wdg1->get(&wdg_remain_time, NULL);
	printf("wdg_remain_time %d ms\n", wdg_remain_time);
	wdg1->reload();
#endif
}


#ifdef WDG_TEST
void wdg1_callback(void)
{
	printf("feed dog timeout!\n");
}
#endif

void setup()
{
#if 1
	//printf("[2]%s %s \n", __DATE__, __TIME__);
  	uint32_t u32Prescal = 0xFFFF;
 	//HAL_Init();

	uint32_t sys_freq = HAL_PLL_GetSystemFreq();
	printf("sys_freq = %d\n", sys_freq);

 	HardwareTimer *timer0 = new HardwareTimer(TIM1);
	timer0->setPrescaleFactor(sys_freq / 10000);// 10kHz
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
#ifdef WDG_TEST
	wdg1 = new IWatchdogClass(WDG2);
	wdg1->attachInterrupt(wdg1_callback);
	wdg1->begin(1000, 0); // param 0 is unused
#endif
}


void loop()
{
	//printf("%s, %s\n", __FILE__, __FUNCTION__);
}

