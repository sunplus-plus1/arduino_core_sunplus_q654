#include "sp7350_hal_dma.h"

#define DUMP(s, v)	printf("%s%08x\n", s, v);
#define DBG(s, v)	DUMP(s, v)
//#define DBG(...)

#define STAMP(v)                                   \
	do {                                       \
		*(volatile u32 *)0xf8000000UL = v; \
	} while (0)

#define BIT(x)		(1 << (x))
#define MIN(x, y)	((x) < (y) ? (x) : (y))

/* AHB_DMA Configure */
#define DMAH_NUM_MASTER_INT	4
#define DMAH_NUM_CHANNELS	8
#define DMAH_CH0_MAX_BLK_SIZE	4095
#define DMAH_M1_HDATA_WIDTH	32
#define MAX_BLK_BYTES		(DMAH_M1_HDATA_WIDTH / 8 * DMAH_CH0_MAX_BLK_SIZE)

/* Reg base of AHB_DMA */
#define REG_AHB_DMA0	0xf8820000UL
#define REG_AHB_DMA1	0xf8821000UL

/* Reg base of PER data register */
#define REG_SPI_DR	0xf8822060UL
#define REG_I2C_DR	0xf8828104UL

/* CTLx bit fields */
#define SMS		25
#define DMS		23
#define TT_FC		20
#define SINC		9
#define DINC		7
#define SRC_TR_WIDTH	4
#define DST_TR_WIDTH	1
#define INT_EN		0

/* CFGx bit fields */
#define DST_PER		(43 - 32)
#define SRC_PER		(39 - 32)
#define FIFO_MODE	(33 - 32)
#define HS_SEL_SRC	11
#define HS_SEL_DST	10
#define CH_PRIOR	5

#ifndef u32
#define u32	uint32_t
#endif

typedef struct {
	u32 l;
	u32 h;
} u64_t;

typedef struct {
	u64_t SAR;
	u64_t DAR;
	u64_t LLP;
	u64_t CTL;
	u64_t SSTAT;
	u64_t DSTAT;
	u64_t SSTATAR;
	u64_t DSTATAR;
	u64_t CFG;
	u64_t SGR;
	u64_t DSR;
} dmac_ch_reg_t;

typedef struct {
	/* Channel_x_Registers */
	dmac_ch_reg_t Ch[DMAH_NUM_CHANNELS];

	/* Interrupt Registers */
	u64_t RawTfr;
	u64_t RawBlock;
	u64_t RawSrcTran;
	u64_t RawDstTran;
	u64_t RawErr;
	u64_t StatusTfr;
	u64_t StatusBlock;
	u64_t StatusSrcTran;
	u64_t StatusDstTran;
	u64_t StatusErr;
	u64_t MaskTfr;
	u64_t MaskBlock;
	u64_t MaskSrcTran;
	u64_t MaskDstTran;
	u64_t MaskErr;
	u64_t ClearTfr;
	u64_t ClearBlock;
	u64_t ClearSrcTran;
	u64_t ClearDstTran;
	u64_t ClearErr;
	u64_t StatusInt;

	/* Software_Handshake Registers */
	u64_t ReqSrc;
	u64_t ReqDst;
	u64_t SglRqSrc;
	u64_t SglRqDst;
	u64_t LstSrc;
	u64_t LstDst;

	/* /Miscellaneous Registers */
	u64_t DmaCfg;
	u64_t ChEn;
	u64_t DmaId;
	u64_t DmaTest;
	u64_t DmaLpTimeout;
	u64_t Dma_Rsvd_3c0;
	u64_t DMA_COMP_PARAMS[6];
	u64_t DmaCompsID;
} dmac_reg_t;

typedef struct {
	volatile dmac_reg_t *reg;
	int IRQ[DMAH_NUM_CHANNELS];
        dma_callback_t callback[DMAH_NUM_CHANNELS];
	u32 ChEn; // channel bits
} dmac_t;

static dmac_t dmac[] = {
	{
		(void *)REG_AHB_DMA0,
		{
			151, // AHB_DMA0_CH0_INT
			152, // AHB_DMA0_CH1_INT
			153, // AHB_DMA0_CH2_INT
			154, // AHB_DMA0_CH3_INT
			155, // AHB_DMA0_CH4_INT
			156, // AHB_DMA0_CH5_INT
			160, // AHB_DMA0_CH6_INT
			161, // AHB_DMA0_CH7_INT
		}
	},
	{
		(void *)REG_AHB_DMA1,
		{
			178, // AHB_DMA1_CH0_INT
			180, // AHB_DMA1_CH1_INT
			192, // AHB_DMA1_CH2_INT
			193, // AHB_DMA1_CH3_INT
			194, // AHB_DMA1_CH4_INT
			195, // AHB_DMA1_CH5_INT
			196, // AHB_DMA1_CH6_INT
			199, // AHB_DMA1_CH7_INT
		}
	},
};

__STATIC_FORCEINLINE u32 local_irq_save(void)
{
#ifdef __CORTEX_M
	u32 flags = __get_PRIMASK();
#else
	u32 flags = __get_CPSR();
#endif

	__disable_irq();

	return flags;
}

__STATIC_FORCEINLINE void local_irq_restore(u32 flags)
{
#ifdef __CORTEX_M
	__set_PRIMASK(flags);
#else
	__set_cpsr(flags);
#endif
}

__STATIC_FORCEINLINE u32 test_and_set(volatile u32 *target)
{
	u32 flags = local_irq_save();
	u32 r = *target;

	*target = 1;
	local_irq_restore(flags);

	return r;
}

static u32 select_ch(u32 dma)
{
	u32 t, ch = 0;
	u32 flags = local_irq_save();

	t = dmac[dma].ChEn; //.reg->ChEn.l;
	/* find 1st zero bit */
	while ((ch < DMAH_NUM_CHANNELS) && (t & 1)) {
		t >>= 1;
		ch++;
	}
	dmac[dma].ChEn |= BIT(ch); // set channel bit
	local_irq_restore(flags);

	return ch;
}

static void dmac_irq_handler(u32 dma)
{
	volatile dmac_reg_t *reg = dmac[dma].reg;
	u32 tfr = reg->StatusTfr.l;
	u32 err = reg->StatusErr.l;
	u32 ch = 0;

	DBG("!Tfr: ", tfr);
	DBG("!Err: ", err);
	while (ch < DMAH_NUM_CHANNELS) {
		u32 ch_mask = BIT(ch);

		if ((tfr & ch_mask) || (err & ch_mask)) {
			if (dmac[dma].callback[ch])
				dmac[dma].callback[ch](err & ch_mask);
			dmac[dma].ChEn &= ~ch_mask; // clear channel bit
		}

		ch++;
	}

	/* clear interrupts */
	reg->ClearTfr.l = 0xffff;
	reg->ClearErr.l = 0xffff;
}

static void dma0_irq_handler(void)
{
	dmac_irq_handler(0);
}

static void dma1_irq_handler(void)
{
	dmac_irq_handler(1);
}

static volatile dmac_reg_t *dmac_init(u32 dma)
{
	volatile dmac_reg_t *reg = dmac[dma].reg;

	if (!test_and_set(&reg->DmaCfg.l)) {
		reg->MaskTfr.l = 0xffff;
		reg->MaskBlock.l = 0xff00;
		reg->MaskSrcTran.l = 0xff00;
		reg->MaskDstTran.l = 0xff00;
		reg->MaskErr.l = 0xffff;

		/* Register IRQs */
		for (int i = 0; i < DMAH_NUM_CHANNELS; i++) {
			int irqn = dmac[dma].IRQ[i];
			IRQ_SetHandler(irqn, dma ? dma1_irq_handler : dma0_irq_handler);
			IRQ_Enable(irqn);
		}
	}

	return reg;
}

/* Peripheral data register */
static u32 per_dr(u32 per, u32 offset)
{
	u32 r;

	if (per <= PER_I2C9) {	// I2C
		r = REG_I2C_DR + (0x1000 * per) + offset;
	} else {		// SPI_CB
		r = REG_SPI_DR + (0x1000 * (per - PER_SPI_CB0));
	}

	return r;
}

HAL_StatusTypeDef HAL_DMA_Start(u32 src, u32 dst, u32 len, dma_callback_t callback)
{
	static u32 m = 0; // master interface
	volatile dmac_reg_t *reg;
	volatile dmac_ch_reg_t *ch_reg;
	u32 dma, ch, ch_mask;
	u32 tt, sinc, dinc;
	u32 t;

	// src, dst, len must be 4-byte aligned

	/* transfer type */
	sinc = (src < PER_MAX) ? 3 : 0;
	dinc = (dst < PER_MAX) ? 3 : 0;
	if (sinc && dinc) {
		tt = 3; // Peripheral to Peripheral
		if ((src < PER_I2C8 && dst >= PER_I2C8) ||
		    (dst < PER_I2C8 && src >= PER_I2C8))
			return HAL_ERROR; // both PER should be on same DMAC
	} else if (sinc) {
		tt = 2; // Peripheral to Memory
	} else if (dinc) {
		tt = 1; // Memory to Peripheral
	} else {
		tt = 0; // Memory to Memory
	}
	DBG("tt     : ", tt);

	/* select dma & channel */
	if (tt) {
		dma = !(src < PER_I2C8 || dst < PER_I2C8);
		ch = select_ch(dma);
	} else {
		dma = 0;
		ch = select_ch(dma);
		if (ch >= DMAH_NUM_CHANNELS) {
			dma++;
			ch = select_ch(dma);
		}
	}
	if (ch >= DMAH_NUM_CHANNELS)
		return HAL_BUSY; // No free channel

	dmac[dma].callback[ch] = callback;
	DBG("dma    : ", dma);
	DBG("channel: ", ch);

	ch_mask = BIT(ch);
	ch_mask |= (ch_mask) << 8; // WE

	/* select master interface */
	m = (m + 1) % DMAH_NUM_MASTER_INT;
	DBG("master : ", m);

	reg = dmac_init(dma);

	/* Program channel registers */
	ch_reg = &reg->Ch[ch];
	ch_reg->SAR.l = sinc ? per_dr(src, 4) : src;
	ch_reg->DAR.l = dinc ? per_dr(dst, 0) : dst;
	ch_reg->LLP.l = 0;

	/* CFGx */
	t = 1 << CH_PRIOR;
	t |= (!sinc) << HS_SEL_SRC;
	t |= (!dinc) << HS_SEL_DST;
	ch_reg->CFG.l = t;
	t = 1 << FIFO_MODE;
	if (sinc)
		t |= ((src & 7) * 2 + 1) << SRC_PER; // RX
	if (dinc)
		t |= ((dst & 7) * 2) << DST_PER; // TX
	ch_reg->CFG.h = t;
	DBG("CFG0: ", ch_reg->CFG.l);
	DBG("CFG1: ", ch_reg->CFG.h);

	/* CTLx */
	t = 1 << INT_EN;
	t |= 2 << SRC_TR_WIDTH;
	t |= 2 << DST_TR_WIDTH;
	t |= tt << TT_FC;
	t |= sinc << SINC;
	t |= dinc << DINC;
	t |= m << SMS;
	t |= m << DMS;
	ch_reg->CTL.l = t;
	DBG("CTL0: ", ch_reg->CTL.l);

#ifdef HAL_DMA_TIMEOUT
	t = HAL_GetTick();
#endif
	while (len) {
		ch_reg->CTL.h = MIN(len, MAX_BLK_BYTES) / 4;
		DBG("CTL1: ", ch_reg->CTL.h);
		DBG("SAR : ", ch_reg->SAR.l);
		DBG("DAR : ", ch_reg->DAR.l);

		/* Start Transfer */
		reg->ChEn.l = ch_mask;

		if (callback)
			break;

		/* Wait for Transfer Complete */
		while (reg->ChEn.l & ch_mask) {
#ifdef HAL_DMA_TIMEOUT
			if ((HAL_GetTick() - t) > HAL_DMA_TIMEOUT)
				return HAL_TIMEOUT;
#endif
		}

		DBG("@Tfr: ", reg->StatusTfr.l);
		DBG("@Err: ", reg->StatusErr.l);

		len -= ch_reg->CTL.h * 4;
	}

	return HAL_OK;
}

#if 1 // TEST
static unsigned long src = 0xd2300000;
static unsigned long dst = 0xd0001000;
static unsigned long len = 0x100;

static void dma_test_callback(u32 err)
{
	DUMP("<<< ", *(volatile u32 *)(dst + len - 4));
	printf("DMA_Test result: %d\n", err);
}

void HAL_DMA_Test(void)
{
        DUMP("DMA_Test: polling mode ", src);
	STAMP(0xabcd1234); // DUMP START
	memset((void *)src, 0x5a, len);
	memset((void *)dst, 0x00, len);

	DUMP(">>> ", *(volatile u32 *)(dst + len - 4));
	HAL_DMA_Start(src, dst, len, NULL);
	DUMP("<<< ", *(volatile u32 *)(dst + len - 4));
	STAMP(0xabcddcba); // RID PASS

        DUMP("DMA_Test: interrupt mode ", src);
	memset((void *)src, 0xa5, len);
	memset((void *)dst, 0x00, len);

	DUMP(">>> ", *(volatile u32 *)(dst + len - 4));
	HAL_DMA_Start(src, dst, len, dma_test_callback);
}
#endif