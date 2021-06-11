#ifndef __SP7021_HAL_ICM_H
#define __SP7021_HAL_ICM_H

#ifdef __cplusplus
	extern "C" {
#endif

/* Include ------------------------------------------------------------------*/
#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "sp70xx.h"
#include "cmsis_compiler.h"
#include "common_all.h"

/* 
   icm callback function
   icm   : icm index icm0~3
   cnt   : internal counter
   fstate: fifo state
 */
typedef void (*sp_icm_cbf)(int icm, uint32_t cnt, uint32_t fstate);

typedef enum {
	ICM0=0,
	ICM1,
	ICM2,
	ICM3,
	ICM_NUM
}ICM_NUMTypeDef;

typedef struct
{
   __IOM uint32_t cfg0;
   __IOM uint32_t cfg1;
   __IOM uint32_t cntscl;
   __IOM uint32_t tstscl;
   __IOM uint32_t cnt;
   __IOM uint32_t pwh;
   __IOM uint32_t pwl;
}ICM_CTRL_REG;


typedef struct
{
	/* Set ICM regsiter address SP_ICM[0-3]_REG */
	ICM_CTRL_REG *instance;
	/* Set ICM index ICM[0-3] */
	int index;
	/* Set input signal source : 0:input0 1:input1 2:input2 3:input3 4:test */
	int muxsel;
	/* Set base signal clk : 0:extCLK0 1:extCLK1 2:extCLK2 3:extCLK3 4:SYSCLK 5:27M 6:32K */
	int clksel;
	/* Set input capture detect mode: 0:Rising Edge 1:Falling Edge 2:Edge Detection */
	int eemode;
	/* Set edge event timers(0-15).If set 7,interrupt will be triggered after 8times edge events */
	int etimes;
	/* Set debounce filter timers(0-7).The debounce times of input signal debounce filter */	
	int dtimes;
	
	uint32_t cntscl;

	uint32_t tstscl;

	/* set pin number: one of PINMUX_PIN1_00--PINMUX_PIN8_07  */
	uint32_t Pin_data;	//Used for input signals.
	uint32_t Pin_clk;	//if base signal clk set extCLK, the value be used.

	int pwl;
	int pwh;
	int cnt;
	int fstate;
}ICM_InitTypeDef;

#define CONVERT(X) (*(volatile unsigned long *)(&(X)))

#define MOON2_PIN_MUX_BASE RF_GRP(2, 0)
#define SP_PIN_MUX_CTRL2  ((GROUP2_Type*) MOON2_PIN_MUX_BASE)

#define ICM_CTRL_BASE RF_GRP(81, 0)

#define SP_ICM0_REG 	((ICM_CTRL_REG *)RF_GRP(81, 0))
#define SP_ICM1_REG 	((ICM_CTRL_REG *)RF_GRP(81, 7))
#define SP_ICM2_REG 	((ICM_CTRL_REG *)RF_GRP(81, 14))
#define SP_ICM3_REG 	((ICM_CTRL_REG *)RF_GRP(81, 21))

/* muxsel: input signal source select */
#define ICM_MUXSEL_INPUT0	0 // input signal 0
#define ICM_MUXSEL_INPUT1	1 // input signal 1
#define ICM_MUXSEL_INPUT2	2 // input signal 2
#define ICM_MUXSEL_INPUT3	3 // input signal 3
#define ICM_MUXSEL_TEST		4 // test signal

/* clksel: internal counter clock select */
#define ICM_CLKSEL_EXT0		0 // external clock 0
#define ICM_CLKSEL_EXT1		1 // external clock 1
#define ICM_CLKSEL_EXT2		2 // external clock 2
#define ICM_CLKSEL_EXT3		3 // external clock 3
#define ICM_CLKSEL_SYS		4 // system clock
#define ICM_CLKSEL_27M		5 // 27 MHz
#define ICM_CLKSEL_32K		6 // 32 KHz
	
/* eemode: edge event mode */
#define ICM_EEMODE_RISING	0 // rising edge
#define ICM_EEMODE_FALLING	1 // falling edge
#define ICM_EEMODE_BOTH		2 // both edges

/* icm fifo state */
#define ICM_FDDROP	0x1000 // fifo data dropped
#define ICM_FEMPTY	0x4000 // fifo empty
#define ICM_FFULL	0x8000 // fifo full

#define ICM_FCLEAR	0x20002000	// clear fifo, also fddrop=0, fempty=1, ffull=0
#define ICM_FMASK	(ICM_FDDROP | ICM_FEMPTY | ICM_FFULL)

// cfg0
#define ICM_ENABLE		0x00010001	// enable icm
#define ICM_DISABLE		0x00010000	// disable icm
#define ICM_RELOAD		0x00020002	// reload icm settings
#define ICM_INTCLR		0x00040004	// clear icm interrupt

#define MUXSEL_OFS	3
#define MUXSEL_BITS	3
#define CLKSEL_OFS	6
#define CLKSEL_BITS	3

// cfg1
#define EEMODE_OFS	0
#define EEMODE_BITS	2
#define ETIMES_OFS	2
#define ETIMES_BITS	4
#define DTIMES_OFS	6
#define DTIMES_BITS	3

#define ICM_MSK(field)  (((1 << field##_BITS) - 1) << field##_OFS)

/* pin mux */
#define	PINMUX_PIN1_00		1
#define	PINMUX_PIN1_01		2
#define	PINMUX_PIN1_02		3
#define	PINMUX_PIN1_03		4
#define	PINMUX_PIN1_04		5
#define	PINMUX_PIN1_05		6
#define	PINMUX_PIN1_06		7
#define	PINMUX_PIN1_07		8
#define	PINMUX_PIN2_00		9
#define	PINMUX_PIN2_01		10
#define	PINMUX_PIN2_02		11
#define	PINMUX_PIN2_03		12
#define	PINMUX_PIN2_04		13
#define	PINMUX_PIN2_05		14
#define	PINMUX_PIN2_06		15
#define	PINMUX_PIN2_07		16
#define	PINMUX_PIN3_00		17
#define	PINMUX_PIN3_01		18
#define	PINMUX_PIN3_02		19
#define	PINMUX_PIN3_03		20
#define	PINMUX_PIN3_04		21
#define	PINMUX_PIN3_05		22
#define	PINMUX_PIN3_06		23
#define	PINMUX_PIN3_07		24
#define	PINMUX_PIN4_00		25
#define	PINMUX_PIN4_01		26
#define	PINMUX_PIN4_02		27
#define	PINMUX_PIN4_03		28
#define	PINMUX_PIN4_04		29
#define	PINMUX_PIN4_05		30
#define	PINMUX_PIN4_06		31
#define	PINMUX_PIN4_07		32
#define	PINMUX_PIN5_00		33
#define	PINMUX_PIN5_01		34
#define	PINMUX_PIN5_02		35
#define	PINMUX_PIN5_03		36
#define	PINMUX_PIN5_04		37
#define	PINMUX_PIN5_05		38
#define	PINMUX_PIN5_06		39
#define	PINMUX_PIN5_07		40
#define	PINMUX_PIN6_00		41
#define	PINMUX_PIN6_01		42
#define	PINMUX_PIN6_02		43
#define	PINMUX_PIN6_03		44
#define	PINMUX_PIN6_04		45
#define	PINMUX_PIN6_05		46
#define	PINMUX_PIN6_06		47
#define	PINMUX_PIN6_07		48
#define	PINMUX_PIN7_00		49
#define	PINMUX_PIN7_01		50
#define	PINMUX_PIN7_02		51
#define	PINMUX_PIN7_03		52
#define	PINMUX_PIN7_04		53
#define	PINMUX_PIN7_05		54
#define	PINMUX_PIN7_06		55
#define	PINMUX_PIN7_07		56
#define	PINMUX_PIN8_00		57
#define	PINMUX_PIN8_01		58
#define	PINMUX_PIN8_02		59
#define	PINMUX_PIN8_03		60
#define	PINMUX_PIN8_04		61
#define	PINMUX_PIN8_05		62
#define	PINMUX_PIN8_06		63
#define	PINMUX_PIN8_07		64

/* function delaration */
void HAL_ICM_Init(ICM_InitTypeDef *pICM_Init);
void HAL_ICM_IRQHandler(ICM_InitTypeDef *pICM_Init);
void HAL_ICM_PINMUX(ICM_InitTypeDef *pICM_Init);

void ICM_DATA_SetPinMux(ICM_InitTypeDef *pICM_Init);
void ICM_CLK_SetPinMux(ICM_InitTypeDef *pICM_Init);

__STATIC_INLINE void ICM_Enable(ICM_InitTypeDef *pICM_Init)
{
	CONVERT(pICM_Init->instance->cfg0) = ICM_ENABLE;
}

__STATIC_INLINE void ICM_Disable(ICM_InitTypeDef *pICM_Init)
{
	CONVERT(pICM_Init->instance->cfg0) = ICM_DISABLE;
}

__STATIC_INLINE void ICM_Reload(ICM_InitTypeDef *pICM_Init)
{
	CONVERT(pICM_Init->instance->cfg0) = ICM_RELOAD;
}

__STATIC_INLINE void ICM_IntrClr(ICM_InitTypeDef *pICM_Init)
{
	CONVERT(pICM_Init->instance->cfg0) = ICM_INTCLR;
}

__STATIC_INLINE void ICM_ReadIntrCnt(ICM_InitTypeDef *pICM_Init, uint32_t *cnt)
{
	*cnt = CONVERT(pICM_Init->instance->cnt);
}

__STATIC_INLINE void ICM_GetFifoState(ICM_InitTypeDef *pICM_Init, uint32_t *state)
{
	*state = CONVERT(pICM_Init->instance->cfg1) & ICM_FMASK;
}

/* Get pulse width(waveform duty cycle)of test signal */
__STATIC_INLINE void ICM_GetSigPulseWidth(ICM_InitTypeDef *pICM_Init, uint32_t *pwh, uint32_t *pwl)
{
	*pwh = CONVERT(pICM_Init->instance->pwh);
	*pwl = CONVERT(pICM_Init->instance->pwl);
}

__STATIC_INLINE void ICM_Patch(ICM_InitTypeDef *pICM_Init)
{
	uint32_t temp;
	temp = CONVERT(pICM_Init->instance->cfg0) & ICM_MSK(CLKSEL);					// save the clk selection to temp var
	CONVERT(pICM_Init->instance->cfg0) = RF_MASK_V(ICM_MSK(CLKSEL), ICM_CLKSEL_SYS << CLKSEL_OFS);	// set clksel to SYSCLK
	ICM_IntrClr(pICM_Init); 														// clear interrupt flag
	CONVERT(pICM_Init->instance->cfg0) = RF_MASK_V(ICM_MSK(CLKSEL), 0) | temp;		// restore the clk selection
}


#define ICM_SETCFG(_cfg, field, val) \
	do { \
		CONVERT(pICM_Init->instance->cfg##_cfg) = ((val) << field##_OFS) | (ICM_MSK(field) << 16); \
	} while (0)
#define ICM_GETCFG(_cfg, field) \
	((CONVERT(pICM_Init->instance->cfg##_cfg) & ICM_MSK(field)) >> field##_OFS)

#if 0
__STATIC_INLINE void ICM_InputSigSrc(ICM_InitTypeDef *pICM_Init)
{
	CONVERT(pICM_Init->instance->cfg0) = RF_MASK_V(ICM_MSK(MUXSEL) , pICM_Init->muxsel << MUXSEL_OFS);
}

__STATIC_INLINE void ICM_ClkSel(ICM_InitTypeDef *pICM_Init)
{
	CONVERT(pICM_Init->instance->cfg0) = RF_MASK_V(ICM_MSK(CLKSEL), pICM_Init->clksel << CLKSEL_OFS);
}

__STATIC_INLINE void ICM_SetDetectMode(ICM_InitTypeDef *pICM_Init)
{
	CONVERT(pICM_Init->instance->cfg1) = RF_MASK_V(ICM_MSK(EEMODE), pICM_Init->eemode << EEMODE_OFS);
}

__STATIC_INLINE void ICM_SetDebounceTimes(ICM_InitTypeDef *pICM_Init)
{
	CONVERT(pICM_Init->instance->cfg1) = RF_MASK_V(ICM_MSK(DTIMES), pICM_Init->dtimes << DTIMES_OFS);
}

__STATIC_INLINE void ICM_TrigTimesEdgeEvent(ICM_InitTypeDef *pICM_Init)
{
	CONVERT(pICM_Init->instance->cfg1) = RF_MASK_V(ICM_MSK(ETIMES), pICM_Init->etimes << ETIMES_OFS);
}
#endif

#ifdef __cplusplus
}
#endif
#endif