#include "Arduino.h"
/*
 * NOTE to develop SP7350:
 * 1.STC Group of CM4:
 *	 ---------------------------------------------------------------------------------------
 *	|	|STC	 	|STC_AV0	|STC_AV1	|STC_AV2	|STC_AV4	|
 *	|-------|---------------|---------------|---------------|---------------|---------------|
 *	|STC	|STC0		|STC1	  	|STC2	   	|STC3		|STC4		|
 *	|-------|---------------|---------------|---------------|---------------|---------------|
 *	|TIM	|TIM0/1/2	|TIM3/4/5	|TIM6/7/8	|TIM9/10/11	|TIM12/13/14	|
 *	|-------|---------------|---------------|---------------|---------------|---------------|
 *	|WDG	|WDG0		|WDG1		|WDG2		|	-	|WDG3		|
 *	 ---------------------------------------------------------------------------------------
 * 2.only use timer 0/1/2 of every STC
 *	e.x. TIM0 is timer0 of STC
 *	TIM1 is timer1 of STC
 *	TIM2 is timer2 of STC
 *	TIM3 is timer0 of STC_AV0 and so on....
 *
 * 3.Real chip test need modify the count of ticks.
 */

#define STAMP(v)                                   \
	do {                                       \
		*(volatile uint32_t *)0xf8000000UL = v; \
	} while (0)

//#define WDG_TEST
#define MHz 1000000

#ifdef WDG_TEST
uint32_t timer_cnt =1;
uint32_t wdg_cnt = 2;
uint32_t wdg_intr_cnt = 3;
IWatchdogClass *wdg1;
#endif

HardwareTimer *timer0;
uint32_t start = 0;

void tim0_callback(void)
{
#ifdef WDG_TEST
#ifdef SP7350
	start = HAL_GetTick();
	timer_cnt = *(uint32_t *)(0xf8800ba8);
	wdg_cnt = *(uint32_t *)(0xf8800ba0);
	wdg_intr_cnt = *(uint32_t *)(0xf8800bf0);
	STAMP(0x33);
	STAMP(start);
	STAMP(timer_cnt);
	STAMP(wdg_cnt);
	STAMP(wdg_intr_cnt);
	//STAMP(0xabcddcba);
#else
	wdg1->get(&wdg_cnt, WDG_TICK_FMT);
	printf("%d\n", wdg_cnt);
	wdg1->reload();
#endif

#else
	printf("@@@@@@@timer1 callback @@@@@@@, tick= %lu\n", HAL_GetTick());
	start = HAL_GetTick();
	//printf("start:%lu\n", start);
#endif
}

#ifdef WDG_TEST

void wdg1_callback(void)
{
#ifdef SP7350
	start = HAL_GetTick();
	timer_cnt = *(uint32_t *)(0xf8800ba8);
	wdg_cnt = *(uint32_t *)(0xf8800ba0);
	wdg_intr_cnt = *(uint32_t *)(0xf8800bf0);
	STAMP(0x22);
	STAMP(start);
	STAMP(timer_cnt);
	STAMP(wdg_cnt);
	STAMP(wdg_intr_cnt);
#else
	printf("h\n");
#endif
	//printf("@@@@@@@watchdog callback @@@@@@@, tick= %lu\n", HAL_GetTick());
}
#endif

/* Test reload mode for all the timers */
void temp_test(STC_TypeDef *STC, TIM_TypeDef *TIM)
{
	uint32_t u32Freq, u32Src;

	HAL_InitCommonSTC(STC, 1000000);
	HardwareTimer *timer_test = new HardwareTimer(TIM);

	timer_test->setClockSource(CLK_STC_SRC);
	u32Src = timer_test->getClockSource();
	u32Freq = timer_test->getTimerClkFreq();
	timer_test->setCount(500, TICK_FORMAT); // 500us when sysclk = 1MHz
	timer_test->setOverflow(500, TICK_FORMAT);

	printf("Timer SRC Select = %d(0:sys 1:stc)\n", u32Src);
	printf("Timer STC Frequence = %d Hz\n", u32Freq);

	printf("STC count = %d\n", timer_test->getCount(TICK_FORMAT));
	printf("STC overflow = %d\n", timer_test->getOverflow(TICK_FORMAT));
	timer_test->attachInterrupt(tim0_callback);
	timer_test->resume();

	delay_us(2000);

	timer_test->pause();
}

void setup()
{
#if 0
	temp_test(STC0, TIM0);
#endif

	uint32_t u32Freq, u32Src;
	uint32_t u32Prescal = 0xFFFF;
	printf("[TIMER]%s %s \n", __DATE__, __TIME__);
	HAL_InitCommonSTC(STC0, 1000000);//STC0 set 1MHz
	//HAL_InitCommonSTC(STC1, 100000);//STC1 set 100kHz

	timer0 = new HardwareTimer(TIM0);
#ifdef SP7350
	timer0->setClockSource(CLK_STC_SRC);
	timer0->setCount(1000, TICK_FORMAT); // 500us when sysclk = 1MHz
	timer0->setOverflow(1000, TICK_FORMAT);
#else
	timer0->setClockSource(CLK_SYS_SRC);
	u32Freq = HAL_PLL_GetSystemFreq();
	timer0->setPrescaleFactor(u32Freq / 10000);// 10kHz
	timer0->setCount(15000, TICK_FORMAT); // 15000 / 10000 = 1.5s
	timer0->setOverflow(15000, TICK_FORMAT);
#endif
	/* DEBUG */
	printf("Timer SRC Select = %d(0:sys 1:stc)\n", timer0->getClockSource());
	printf("Timer Frequence = %d Hz\n", timer0->getTimerClkFreq());
	printf("Timer prescal= 0x%x\n", timer0->getPrescaleFactor());
	printf("Timer count = %d\n", timer0->getCount(TICK_FORMAT));
	printf("Timer overflow = %d\n", timer0->getOverflow(TICK_FORMAT));

	timer0->attachInterrupt(tim0_callback);
	/* Go */
#ifndef WDG_TEST
	timer0->resume();
#endif
#ifdef WDG_TEST
	/* The clock freq on the ZEBU is 1/2000 of the clk on the real chip */
#ifdef SP7350
	//STC set 1MHz
	//STAMP(0xabcd1234);
	wdg1 = new IWatchdogClass(WDG0);
	wdg1->attachInterrupt(wdg1_callback);
	wdg1->begin(2000, WDG_TICK_FMT); // param 0 is unused
	timer0->resume();
	start = HAL_GetTick();
	timer_cnt = *(uint32_t *)(0xf8800ba8);
	wdg_cnt = *(uint32_t *)(0xf8800ba0);
	wdg_intr_cnt = *(uint32_t *)(0xf8800bf0);
	STAMP(0x11);
	STAMP(start);
	STAMP(timer_cnt);
	STAMP(wdg_cnt);
	STAMP(wdg_intr_cnt);
#else
	wdg1 = new IWatchdogClass(WDG1);
	wdg1->attachInterrupt(wdg1_callback);
	timer0->resume();
	wdg1->begin(200000, WDG_TICK_FMT); // param 0 is unused
#endif
#endif
}

void loop()
{
	//printf("%s, %s\n", __FILE__, __FUNCTION__);
}
