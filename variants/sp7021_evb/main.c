#include <types.h>
#include <common.h>
#include <config.h>

#include "sp_interrupt.h"
#include "common_all.h"
#include "cache.h"
#include "stc.h"
#include "gpio_drv.h"
#include "metal/irq.h"

#ifdef ICM_TEST
#include "icm.h"
#endif

#include "HardwareTimer.h"

#define A_and_B_chip   //A and B chip running simultaneously
//#define A_chip_only       //A chip only

#ifdef NOC_TEST
// #include "display_pattern_384x240_nv12.inc"
extern void noc_initial_settings();
const unsigned char longlonglong_table[16];
#endif

#ifdef QCH_TEST
extern void qch_initial_settings();
#endif

extern void mmu_init();

extern char __vectors_start[];
extern char __vectors_end[];

void hw_init()
{
	unsigned int i;

	/* clken[all]  = enable */
	for (i = 0; i < sizeof(MOON0_REG->clken) / 4; i++)
		MOON0_REG->clken[i] = RF_MASK_V_SET(0xffff);
	/* gclken[all] = no */
	for (i = 0; i < sizeof(MOON0_REG->gclken) / 4; i++)
		MOON0_REG->gclken[i] = RF_MASK_V_CLR(0xffff);
	/* reset[all] = clear */
	for (i = 0; i < sizeof(MOON0_REG->reset) / 4; i++)
		MOON0_REG->reset[i] = RF_MASK_V_CLR(0xffff);

}

#ifdef A_chip_only
int main(void)
{

	printf("Build @%s, %s\n", __DATE__, __TIME__);

	hw_init();
	//AV1_STC_init();
	
	/*initial interrupt vector table*/
	int_memcpy(0x00000000, __vectors_start, (unsigned)__vectors_end - (unsigned)__vectors_start);

	mmu_init();
	HAL_DCACHE_ENABLE();

	ipc_init();
#ifdef NOC_TEST
	noc_initial_settings();
#endif

#ifdef QCH_TEST
	qch_initial_settings();
#endif

	//cbdma_test_init();
	timer_test_init();
#ifdef AXI_MON
	axi_mon_test_init();
#endif


#ifdef RS485_TEST
	//AV1_STC_init();
	rs485_init(10,11,12);	//G_MX[10]_TX --> DI, G_MX[11]_RX --> RO ,G_MX[12]_RTS
#endif 

	/* interrupt manager module init */
	sp_interrupt_setup();
	ipc_start();

	printf("NonOS boot OK!!!\n");
	task_dbg();
	while(1);

	//Never get here
	return 0;
}
#endif 

#ifdef A_and_B_chip
void uart_isr_init(void)
{
	//UART_REG->isc = (1 << 5); // RX interrupt enable
	//interrupt_register(53, "UART0", uart_isr, 1);
}

//#define TIMER_TEST
#ifdef TIMER_TEST
#include "sp7021_hal_irq_ctrl.h"

#define TIMER3_TICKS        (9 - 1)     /* 1s */
#define TIMER2_TICKS        (90 - 1)        /* 1s */

#define TIMER_CONFIG_SYS    (0 << 2)    /* src: system clock */
#define TIMER_CONFIG_STC    (1 << 2)    /* src: stc */
#define TIMER_ONESHOT       (0 << 1)    /* timer one-shot operation*/
#define TIMER_RELOAD        (1 << 1)    /* timer auto reload */
#define TIMER_RUN           (1 << 0)    /* timer run */
#define TIMER_STOP          (0 << 0)    /* timer stop */

#define TIMER_1MS_PRES_VALUE    (9)     // 1ms=1.1us*(TIMER_1MS_PRES_VALUE+1)*(TIMER2_TICKS+1)

#define TIMER3_INT  (154)
#define TIMER2_INT  (153)

static volatile unsigned int g_repeat_cnt = 0;

int timer3_callback(int vector, void *arg)
{
    printf("@Hello[%d]\n", ++g_repeat_cnt);
}

void timer_test_init()
{
#ifdef QCH_TEST
    isr_t isr = qch_timer_callback;
#else
    isr_t isr = timer3_callback;
#endif
    STC_REG->timer3_ctl = TIMER_CONFIG_STC | TIMER_RELOAD;
    STC_REG->timer3_pres_val = 999;
    STC_REG->timer3_reload = TIMER3_TICKS;
    STC_REG->timer3_cnt = TIMER3_TICKS;
#if 0
    IRQ_SetHandler(TIMER3_INT, timer3_callback);
    IRQ_SetMode(TIMER3_INT, IRQ_MODE_TRIG_EDGE_RISING);
    IRQ_Enable(TIMER3_INT);
#else
	extern int metal_a926_irq_init(void);
	metal_a926_irq_init();
	int a = TIMER3_INT;
	printf(">>>>>>>%d\n", a);
	metal_irq_register(TIMER3_INT, timer3_callback, NULL);
    IRQ_Enable(TIMER3_INT);
#endif
}

void timer_test()
{
    unsigned int dwMask;
    printf("Timer test start!\n");

    g_repeat_cnt = 0;

    STC_REG->timer3_ctl |= TIMER_RUN;

    while (g_repeat_cnt < 6);

    IRQ_Disable(TIMER3_INT);

    STC_REG->timer3_ctl &= ~TIMER_RUN;

    printf("Timer3 interrupt test finished\n");

}
#endif

int main(void)
{
#if 0 // MALLOC_TEST
	void *p1 = malloc(1 * 1024 * 1024);
	void *p2 = malloc(2 * 1024 * 1024);
	printf("MALLOC_TEST: %08x %08x\n", p1 , p2);
	free(p1);
	free(p2);
#endif

	printf("Build @%s, %s\n", __DATE__, __TIME__);
	hw_init();
	//mmu_init();
	//HAL_DCACHE_ENABLE();
	SystemInit();
	HAL_Init();

	//AV1_STC_init();
	/*initial interrupt vector table*/
	//int_memcpy(0x00000000, __vectors_start, (unsigned)__vectors_end - (unsigned)__vectors_start);

	IRQ_Initialize();

#ifdef TIMER_TEST
	timer_test_init();
	timer_test();
#endif

//#ifdef I2C_TEST
#ifdef I2C_TEST
	extern void i2c_test(void);
	i2c_test();
#endif

//#define ICM_TEST
#ifdef ICM_TEST
	ICM_Initialization();
#endif
	//cbdma_test_init();
	uart_isr_init();

//#define ARDUINO_INTR_SAMPLE
#ifdef ARDUINO_INTR_SAMPLE
	extern void arduino_intr_test(void);
	arduino_intr_test();
#endif
	
	printf("NonOS boot OK!!!\n");
#if 0
	extern openamp_test_main(void);
	openamp_test_main();
#endif
	extern void loop(void);
	
	//loop();
	void uart_test(void);
	uart_test();
	//task_dbg();
#if 0
    GPIO_F_SET(21,1);
    GPIO_M_SET(21,1);
    GPIO_E_SET(21,1);
    GPIO_O_SET(21,1);
#endif

	//Marlin_main();
	while(1);

	//Never get here
	return 0;
}
#endif 