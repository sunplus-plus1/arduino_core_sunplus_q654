#include <types.h>
#include <common.h>
#include <config.h>

#include "sp_interrupt.h"
#include "common_all.h"
#include "cache.h"
#include "stc.h"
#include "gpio_drv.h"

#ifdef ICM_TEST
#include "icm.h"
#endif

#include "HardwareTimer.h"

//#define I2C_TEST
#ifdef I2C_TEST
#include "sp7021_hal_i2c.h"
#endif

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


//#ifdef I2C_TEST
//u8	data_buf[255];
//u8	tx_buf[255];
//#endif

//#define ARDUINO_INTR_SAMPLE
#ifdef ARDUINO_INTR_SAMPLE

#include "WInterrupts.h"

void gpio_int_irq_callback(void)
{
    printf("IRQ callback\n");
}

void arduino_intr_test(void)
{
	attachInterrupt(0, &gpio_int_irq_callback, HIGH);
}
#endif

//#define INTR_SAMPLE
#ifdef INTR_SAMPLE

#include "sp7021_hal_exti.h"
static cnt = 0;
void gpio_int_fiq_callback(void)
{
	printf("FIQ callback\n");
}

void gpio_int_irq_callback(void)
{
    printf("IRQ callback\n");
    cnt++;
    printf("cnt=%d\n", cnt);
    if(6 == cnt)
    {
		printf("121 TRIG=%d\n", IRQ_GetMode(121));
		IRQ_Disable(121);
    }
}

void create_IRQ()
{
	EXTI_InitTypeDef *pEXTI_IRQ;
	//HAL_EXTI_Data(pEXTI_IRQ);
	pEXTI_IRQ->priority = 1;
	pEXTI_IRQ->pin = 38;
	pEXTI_IRQ->id = 1;		//use EXTI1,irqn=121
	pEXTI_IRQ->trig = IRQ_MODE_TRIG_EDGE_RISING;  //EDGE_RISING
	HAL_EXTI_Init(pEXTI_IRQ, &gpio_int_irq_callback);
}

void create_FIQ()
{
	EXTI_InitTypeDef *pEXTI_FIQ;
	HAL_EXTI_Data(pEXTI_FIQ);    // default LEVEL_HIGH
	pEXTI_FIQ->pin = 39;
	pEXTI_FIQ->id = 0;		//use EXTI0,irqn=120
	pEXTI_FIQ->priority = 0;    //FIQ
	HAL_EXTI_Init(pEXTI_FIQ, &gpio_int_fiq_callback);
}

void EXTI_TEST()
{
	int flag1 = 2;
	int flag2 = 2;
	printf("entry EXTI_TEST\n");
	create_IRQ();
	create_FIQ();
	while(1)
	{
		if(cnt > 6)
		{
			flag1 = IRQ_GetEnableState(121);
			IRQ_Disable(121);
			IRQ_Clear(121);
			flag2 = IRQ_GetEnableState(121);
			printf("previous flag = %d,flag =%d\n", flag1, flag2);
			while(1);
		}
	}
}
#endif

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

//#ifdef I2C_TEST
//   unsigned int test;
//#endif



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

#ifdef INTR_SAMPLE
	gpio_intr_test_init();
#endif

#ifdef RS485_TEST
	//AV1_STC_init();
	rs485_init(10,11,12);	//G_MX[10]_TX --> DI, G_MX[11]_RX --> RO ,G_MX[12]_RTS
#endif 

	/* interrupt manager module init */
	sp_interrupt_setup();
	ipc_start();

//#ifdef I2C_TEST
//	sp_i2c_master_init();
//	sp_i2c_master_set_freq_khz(0, 100);
//#endif

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

void timer3_callback(int vector)
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
    IRQ_SetHandler(TIMER3_INT, timer3_callback);
    IRQ_SetMode(TIMER3_INT, IRQ_MODE_TRIG_EDGE_RISING);
    IRQ_Enable(TIMER3_INT);
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

#if 0
typedef struct
{
	volatile uint32_t cfg0;
	volatile uint32_t b;
	volatile uint32_t c;
}ICM_CTRL_REG;
//9C002880
#define SP_ICM0_REG ((ICM_CTRL_REG *) 0x9C002880)

typedef struct
{
	ICM_CTRL_REG *instance;
}ICM_InitTypeDef;

HAL_TEST(ICM_InitTypeDef *test)
{
	//printf("entry\n");
	int a = test->instance->cfg0;
	//printf("exit\n");
}

void icm_test()
{
	printf("icm Build @%s, %s\n", __DATE__, __TIME__);
	ICM_InitTypeDef test;
	test.instance = SP_ICM0_REG;
	printf(">>>test.instance\n");
	test.instance->cfg0 = 0x01c00100;
	printf("(%d)cfg0=%x\n", __LINE__, test.instance->cfg0);
	test.instance->cfg0 = 0x01c00140;
	printf("(%d)cfg0=%x\n", __LINE__, test.instance->cfg0);
	test.instance->cfg0 = 0x01c00180;
	printf("(%d)cfg0=%x\n", __LINE__, test.instance->cfg0);

	HAL_TEST(&test);
}
#endif

#ifdef I2C_TEST
extern void sp_i2c_read(unsigned int i2c_no, u8  slave_addr , u8  *data_buf , unsigned int len);
extern void sp_i2c_write(unsigned int i2c_no, u8  slave_addr , u8  *data_buf , unsigned int len);
extern void sp_i2c_dma_read(unsigned int i2c_no, u8  slave_addr , u8  *data_buf , unsigned int len);
extern void sp_i2c_dma_write(unsigned int i2c_no, u8  slave_addr , u8  *data_buf , unsigned int len);
extern void sp_i2c_master_set_freq_khz(unsigned int i2c_no, unsigned int freq);
extern void sp_i2c_master_init(void);
extern int i2c_check(unsigned int i2c_no);





void i2c_test()
{	
	printf("I2C_TEST!!!!!!!!!\n");
	uint8_t tx_buff[20];
	uint8_t rx_buff[20];
	tx_buff[0] = 0x24;
	tx_buff[1] = 0x00;
int a = 100000000;
	while(a--);
	sp_i2c_master_init();
	//MOON3_REG->sft_cfg[10] = RF_MASK_V(0x7f, 14);//scl
	//MOON3_REG->sft_cfg[10] = RF_MASK_V(0x7f << 8, 16 << 8);//sda
	sp_i2c_master_set_freq_khz(0, 100);
	sp_i2c_dma_write(0, 0x44, tx_buff, 2);
	while(i2c_check(0));
	sp_i2c_dma_read(0, 0x44, rx_buff, 6);

	//sp_i2c_dma_write(0, 0x44, tx_buff, 2);
	//sp_i2c_dma_read(0, 0x44, rx_buff, 6);

	int temp = (rx_buff[0] << 8) | rx_buff[1];
	temp = temp * 17500/65536;
	int temp_L = temp % 100;
	int temp_H = temp/100 - 45;

	int RH = (rx_buff[3] << 8) | rx_buff[4];
	RH = RH * 10000/65536;
	int RH_L = RH % 100;
	int RH_H = RH / 100;
	printf("temperature%d:%d, humidity%d:%d\n ", temp_H,temp_L, RH_H,RH_L);
}
#endif

//#define TEMPWAVE
#ifdef TEMPWAVE
void temp_output_waveform(void)
{
	gpio_first_val_set(32, 1);
	gpio_master_val_set(32, 1);
	gpio_oe_val_set(32, 1);
	while(1)
	{
		gpio_out_val_set(32, 1);
		while(27000);
		gpio_out_val_set(32, 0);
		while(27000);
	}
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

//#ifdef I2C_TEST
//    unsigned int test;
//#endif
	printf("Build @%s, %s\n", __DATE__, __TIME__);
	hw_init();
	//mmu_init();
	//HAL_DCACHE_ENABLE();
	SystemInit();


	//AV1_STC_init();
	/*initial interrupt vector table*/
	//int_memcpy(0x00000000, __vectors_start, (unsigned)__vectors_end - (unsigned)__vectors_start);

	//IRQ_Initialize();

#ifdef TIMER_TEST
	//timer_test_init();
	//timer_test();
#endif

#ifdef I2C_TEST
	//HAL_I2C_TEST();
	//i2c_test();
	while(1)
	{
		i2c_test();
	}
#endif

#ifdef TEMPWAVE
	temp_output_waveform();
#endif

//#define ICM_TEST
#ifdef ICM_TEST
	ICM_Initialization();
#endif
	//cbdma_test_init();
	uart_isr_init();
#ifdef INTR_SAMPLE
	EXTI_TEST();
#endif

#ifdef ARDUINO_INTR_SAMPLE
	arduino_intr_test();
#endif

	

	printf("NonOS boot OK!!!\n");
	
	extern void loop(void);
	
	loop();
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
