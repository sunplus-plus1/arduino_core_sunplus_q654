#include "sp7350_hal_i2c.h"
#include "sp7350_hal_dma.h"
#include <stdlib.h>
#include <Arduino.h>

//#define HAL_I2C_DEBUG
#ifdef HAL_I2C_DEBUG
#define debug_print(format, ...) printf("[%s:%d] "format"", __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define debug_print(format, ...)
#endif

#define STAMP(v)                                   \
	do {                                       \
		*(volatile uint32_t *)0xf8000000UL = v; \
	} while (0)

#define DIV_ROUND_CLOSEST(x, divisor)	(((x)+ ((divisor) / 2)) / (divisor))

/* Private Variables */
const static struct i2c_hal_info sp_i2c_info[] = {
	I2C_HAL_INFO(SP_I2CM0, I2C0_INDEX, I2CM0, PINMUX_I2C_0, I2C_MASTER0_IRQ, PER_I2C0),
	I2C_HAL_INFO(SP_I2CM1, I2C1_INDEX, I2CM1, PINMUX_I2C_1, I2C_MASTER1_IRQ, PER_I2C1),
	I2C_HAL_INFO(SP_I2CM2, I2C2_INDEX, I2CM2, PINMUX_I2C_2, I2C_MASTER2_IRQ, PER_I2C2),
	I2C_HAL_INFO(SP_I2CM3, I2C3_INDEX, I2CM3, PINMUX_I2C_3, I2C_MASTER3_IRQ, PER_I2C3),
	I2C_HAL_INFO(SP_I2CM4, I2C4_INDEX, I2CM4, PINMUX_I2C_4, I2C_MASTER4_IRQ, PER_I2C4),
	I2C_HAL_INFO(SP_I2CM5, I2C5_INDEX, I2CM5, PINMUX_I2C_5, I2C_MASTER5_IRQ, PER_I2C5),
	I2C_HAL_INFO(SP_I2CM6, I2C6_INDEX, I2CM6, PINMUX_I2C_6, I2C_MASTER6_IRQ, PER_I2C6),
	I2C_HAL_INFO(SP_I2CM7, I2C7_INDEX, I2CM7, PINMUX_I2C_7, I2C_MASTER7_IRQ, PER_I2C7),
	I2C_HAL_INFO(SP_I2CM8, I2C8_INDEX, I2CM8, PINMUX_I2C_8, I2C_MASTER8_IRQ, PER_I2C8),
	I2C_HAL_INFO(SP_I2CM9, I2C9_INDEX, I2CM9, PINMUX_I2C_9, I2C_MASTER9_IRQ, PER_I2C9),
};

static I2C_HandleTypeDef *gpHandle[I2C_NUM];

static void i2c_detect(I2C_HandleTypeDef *handle)
{
	int i;

	for(i = 0; i < I2C_NUM; i++) {
		if(handle->Instance == sp_i2c_info[i].instance) {
			handle->Index = sp_i2c_info[i].index;
			//handle->DMAIndex = sp_i2c_info[i].dma_idx;
			break;
		}
	}
}

static void i2c_irq_config(IRQn_Type irqn)
{
	IRQ_SetMode(irqn, IRQ_MODE_TRIG_LEVEL_HIGH);
	IRQ_SetHandler(irqn, HAL_I2C_IRQHandler);
	IRQ_Enable(irqn);
}

static void i2c_clken_config(MODULE_ID_Type clk_id)
{
	HAL_Module_Clock_enable(clk_id, 1);
	HAL_Module_Clock_gate(clk_id, 0);
	HAL_Module_Reset(clk_id, 0);
}

static void i2c_pinmux_config(PINMUX_Type pinmux)
{
	HAL_PINMUX_Cfg(pinmux, 1);
}

static uint32_t i2c_irq_get_index(uint32_t irqn)
{
	int i, index;

	for(i = 0; i < I2C_NUM; i++) {
		if(irqn == sp_i2c_info[i].irq_num) {
			index = sp_i2c_info[i].index;
			break;
		}
	}

	return index;
}

static void __i2c_disable(I2C_HandleTypeDef *hi2c)
{
	uint32_t status;
	uint32_t timeout = 100;

	do {
		hi2c->Instance->ic_enable = 0;
		/*
		 * The enable status register may be unimplemented, but
		 * in that case this test reads zero and exits the loop.
		 */
		status = hi2c->Instance->ic_enable_status;
		if ((status & 1) == 0)
			return;
		/*
		 * Wait 10 times the signaling period of the highest I2C
		 * transfer supported by the driver (for 400KHz this is
		 * 25us) as described in the DesignWare I2C databook.
		 */
		HAL_Delay(25);
	} while (timeout--);

	printf("Timeout to disable the I2C module\n");
}

static void __i2c_disable_nowait(I2C_HandleTypeDef *hi2c)
{
	hi2c->Instance->ic_enable = 0;
}

static void __i2c_cal_scl_cnt(uint32_t clk_rate_khz, uint32_t tHIGH, uint32_t tLOW, uint32_t scl_falling_time,
			 uint32_t *scl_hcnt, uint32_t *scl_lcnt)
{
	*scl_hcnt = DIV_ROUND_CLOSEST(clk_rate_khz * tHIGH, 1000000) - 8;
	*scl_lcnt = DIV_ROUND_CLOSEST(clk_rate_khz * (tLOW + scl_falling_time), 1000000) - 1;
}

static void __i2c_timing_set(I2C_HandleTypeDef *hi2c)
{
	uint32_t clk_rate_khz;
	uint32_t scl_hcnt, scl_lcnt;
	uint32_t scl_falling_time = 300; /* ns */

	/* I2C clk tree: 0->100M, 0b1->50M, 0b11->25M */
	clk_rate_khz = 100000;
	MOON3_REG->sft_cfg[27] = (3 << 22) | (0 << 6);

	/* Set SCL hcnt/lcnt */
	if(hi2c->Init.Timing <= I2C_MAX_STANDARD_MODE_FREQ) {
		hi2c->master_cfg = SP_IC_CON_SPEED_STD;
		__i2c_cal_scl_cnt(clk_rate_khz, 4000, 4700, scl_falling_time, &scl_hcnt, &scl_lcnt);
		hi2c->Instance->ic_ss_scl_hcnt = scl_hcnt;
		hi2c->Instance->ic_ss_scl_lcnt = scl_lcnt;
		debug_print("ss_scl_h %d \n", hi2c->Instance->ic_ss_scl_hcnt);
		debug_print("ss_scl_l %d \n", hi2c->Instance->ic_ss_scl_lcnt);
	} else if(hi2c->Init.Timing <= I2C_MAX_FAST_MODE_FREQ) {
		hi2c->master_cfg = SP_IC_CON_SPEED_FAST;
		__i2c_cal_scl_cnt(clk_rate_khz, 600, 1300, scl_falling_time, &scl_hcnt, &scl_lcnt);
		hi2c->Instance->ic_fs_scl_hcnt = scl_hcnt;
		hi2c->Instance->ic_fs_scl_lcnt = scl_lcnt;
		debug_print("fs_scl_h %d \n", hi2c->Instance->ic_fs_scl_hcnt);
		debug_print("fs_scl_l %d \n", hi2c->Instance->ic_fs_scl_lcnt);
	} else if(hi2c->Init.Timing <= I2C_MAX_FAST_MODE_PLUS_FREQ) {
		hi2c->master_cfg = SP_IC_CON_SPEED_FAST;
		__i2c_cal_scl_cnt(clk_rate_khz, 260, 500, scl_falling_time, &scl_hcnt, &scl_lcnt);
		hi2c->Instance->ic_fs_scl_hcnt = scl_hcnt;
		hi2c->Instance->ic_fs_scl_lcnt = scl_lcnt;
		debug_print("fs_scl_h %d \n", hi2c->Instance->ic_fs_scl_hcnt);
		debug_print("fs_scl_l %d \n", hi2c->Instance->ic_fs_scl_lcnt);
	} else if (hi2c->Init.Timing <= I2C_MAX_HIGH_SPEED_MODE_FREQ) {
		hi2c->master_cfg = SP_IC_CON_SPEED_HIGH;
		__i2c_cal_scl_cnt(clk_rate_khz, 160, 320, scl_falling_time, &scl_hcnt, &scl_lcnt);
		hi2c->Instance->ic_hs_scl_hcnt = scl_hcnt;
		hi2c->Instance->ic_hs_scl_lcnt = scl_lcnt;
		debug_print("hs_scl_h %d \n", hi2c->Instance->ic_hs_scl_hcnt);
		debug_print("hs_scl_l %d \n", hi2c->Instance->ic_hs_scl_lcnt);
	}

	/* Set SDA hold time , ic_sda_hold <= scl_lcnt - 2 */
	hi2c->Instance->ic_sda_hold = scl_lcnt - 2;
	debug_print("ic_sda_hold %d \n", hi2c->Instance->ic_sda_hold);
}

static void __i2c_init_master(I2C_HandleTypeDef *hi2c)
{
	__i2c_timing_set(hi2c);

	hi2c->master_cfg |= SP_IC_CON_MASTER | SP_IC_CON_SLAVE_DISABLE | SP_IC_CON_RESTART_EN;

	/* control the thresold of trigger the RX_FULL/TX_EMPTY interrupt */
	hi2c->Instance->ic_tx_tl = I2C_FIFO_DEPTH / 2;	/* < 4, trig intr */
	hi2c->Instance->ic_rx_tl = 0;			/* > 0, trig intr */

	hi2c->Instance->ic_con = hi2c->master_cfg;
}

static void i2c_sp_dump_reg(I2C_HandleTypeDef *hi2c)
{
	debug_print("ic_tx_tl 0x%08x\n", (unsigned int)&hi2c->Instance->ic_tx_tl);
	debug_print("0x%08x\n", hi2c->Instance->ic_tx_tl);
	debug_print("ic_rx_tl 0x%08x\n", (unsigned int)&hi2c->Instance->ic_rx_tl);
	debug_print("0x%08x\n", hi2c->Instance->ic_rx_tl);
	debug_print("ic_con 0x%08x\n", (unsigned int)&hi2c->Instance->ic_con);
	debug_print("0x%08x\n", hi2c->Instance->ic_con);
	debug_print("ic_tar 0x%08x\n", (unsigned int)&hi2c->Instance->ic_tar);
	debug_print("0x%08x\n", hi2c->Instance->ic_tar);
	debug_print("ic_enable 0x%08x\n", (unsigned int)&hi2c->Instance->ic_enable);
	debug_print("0x%08x\n", hi2c->Instance->ic_enable);
	debug_print("ic_enable_status 0x%08x\n", (unsigned int)&hi2c->Instance->ic_enable_status);
	debug_print("0x%08x\n", hi2c->Instance->ic_enable_status);
	debug_print("ic_clr_intr 0x%08x\n", (unsigned int)&hi2c->Instance->ic_clr_intr);
	debug_print("0x%08x\n", hi2c->Instance->ic_clr_intr);
	debug_print("ic_intr_mask 0x%08x\n", (unsigned int)&hi2c->Instance->ic_intr_mask);
	debug_print("0x%08x\n", hi2c->Instance->ic_intr_mask);
}

static uint32_t i2c_sp_timeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{

	if (((HAL_GetTick() - Tickstart) > Timeout) && (Timeout != 0U)) {
		hi2c->State = HAL_I2C_STATE_READY;
		/* Process Unlocked */
		__HAL_UNLOCK(hi2c);

		return 1;
	}

	return 0;
}

static int i2c_sp_wait_bus_not_busy(I2C_HandleTypeDef *hi2c, uint32_t timeout)
{
	uint32_t stat, tickstart;
	int ret = 0;

	tickstart= HAL_GetTick();
	while (1) {
		/* Timeout function */
		/* 640us = fifo_depth / 100kbit/s and the STC freq = 1MHz*/
		if (i2c_sp_timeout(hi2c, timeout, tickstart)) {
			printf("[I2C]Timeout!\n");
			hi2c->ErrorCode |= HAL_I2C_ERR_TIMEOUT;
			ret = -1;
		}

		stat = hi2c->Instance->ic_status;
		if ((stat & SP_IC_STATUS_MST_ACT) != SP_IC_STATUS_MST_ACT) {
			hi2c->State = HAL_I2C_STATE_READY;
			break;
		}
	}
	return ret;
}

static uint32_t i2c_sp_read_clear_intrbits(I2C_HandleTypeDef *hi2c)
{
	unsigned int stat;
	unsigned int dummy;

	/*
	 * The IC_INTR_STAT register just indicates "enabled" interrupts.
	 * The unmasked raw version of interrupt status bits is available
	 * in the IC_RAW_INTR_STAT register.
	 *
	 * That is,
	 *   stat = readl(IC_INTR_STAT);
	 * equals to,
	 *   stat = readl(IC_RAW_INTR_STAT) & readl(IC_INTR_MASK);
	 *
	 * The raw version might be useful for debugging purposes.
	 */
	stat = (uint32_t)hi2c->Instance->ic_intr_stat;
	/*
	 * Do not use the IC_CLR_INTR register to clear interrupts, or
	 * you'll miss some interrupts, triggered during the period from
	 * readl(IC_INTR_STAT) to readl(IC_CLR_INTR).
	 *
	 * Instead, use the separately-prepared IC_CLR_* registers.
	 */
	if (stat & SP_IC_INTR_RX_UNDER)
		dummy = (uint32_t)hi2c->Instance->ic_clr_rx_under;
	if (stat & SP_IC_INTR_RX_OVER)
		dummy = (uint32_t)hi2c->Instance->ic_clr_rx_over;
	if (stat & SP_IC_INTR_TX_OVER)
		dummy = (uint32_t)hi2c->Instance->ic_clr_tx_over;
	if (stat & SP_IC_INTR_RD_REQ)
		dummy = (uint32_t)hi2c->Instance->ic_clr_rd_req;
	if (stat & SP_IC_INTR_TX_ABRT) {
		/*
		 * The IC_TX_ABRT_SOURCE register is cleared whenever
		 * the IC_CLR_TX_ABRT is read.  Preserve it beforehand.
		 */
		hi2c->abort_source = (unsigned int)hi2c->Instance->ic_tx_abrt_source;
		dummy = (uint32_t)hi2c->Instance->ic_clr_tx_abrt;
	}
	if (stat & SP_IC_INTR_RX_DONE)
		dummy = (uint32_t)hi2c->Instance->ic_clr_rx_done;
	if (stat & SP_IC_INTR_ACTIVITY)
		dummy = (uint32_t)hi2c->Instance->ic_clr_tx_activity;
	if (stat & SP_IC_INTR_STOP_DET)
		dummy = (uint32_t)hi2c->Instance->ic_clr_stop_det;
	if (stat & SP_IC_INTR_START_DET)
		dummy = (uint32_t)hi2c->Instance->ic_clr_start_dft;
	if (stat & SP_IC_INTR_GEN_CALL)
		dummy = (uint32_t)hi2c->Instance->ic_clr_gen_call;
	//*(volatile unsigned int *)0xF8803338 = 0x14;//xtdebug
	return stat;
}

/* 0x80 TX_ABRT_SOURCE */
void i2c_sp_handle_tx_abort(I2C_HandleTypeDef *hi2c)
{
	unsigned int abort_source = hi2c->abort_source;
	int i, temp_code;

	if (abort_source & SP_IC_TX_ABRT_NOACK) {
		for (i = 0; i <= 4; i++) {
			if (abort_source & (1 << i)) {
				printf("NACK fail %d\n", i);
				temp_code = i;
			}

		}
		return;
	}

	for(i = 5; i <= 13; i++) {
		if (abort_source & (1 << i)) {
			if (i == 12)
				printf("lost arbition and again\n");
			debug_print("NACK fail 02\n");
		}
	}
	
	if (temp_code == 1)
		hi2c->ErrorCode = HAL_I2C_ERR_ADDRESS_NACK;
}

static void i2c_sp_xfer_prepare(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t *buf, uint32_t len)
{
	uint32_t dummy, stat;

	hi2c->abort_source =  0;
	hi2c->rx_outstanding = 0;
	hi2c->completion = 1;
	hi2c->msg_write_idx = 0;
	hi2c->msg_read_idx = 0;
	hi2c->msg_err = 0;

	/* Debug */
	//i2c_sp_dump_reg(hi2c);

	__i2c_disable(hi2c);

	 //TODO: Consider the case of 10-bit address slave device
	hi2c->Instance->ic_tar = addr;

#if 0 /* Not support DMA mode, see cm4 remapping address for more details */
	/* Use DMA fifo */
	if (hi2c->Mode == I2C_MODE_DMA) {
		hi2c->Instance->ic_dma_cr = SP_IC_TDMAE | SP_IC_RDMAE;
		hi2c->Instance->ic_dma_tdlr = I2C_TDRL;
		hi2c->Instance->ic_dma_rdlr = I2C_RDRL;
	}
#endif
	/* Enforce disabled interrupts (due to HW issues) */
	hi2c->Instance->ic_intr_mask = 0;

	hi2c->Instance->ic_enable = 1; // XTTEMP

	/* Dummy read to avoid the register getting stuck on Bay Trail */
	dummy = hi2c->Instance->ic_enable_status;

	/* Clear and enable interrupts */
	dummy = hi2c->Instance->ic_clr_intr;
	hi2c->Instance->ic_intr_mask = SP_IC_INTR_MASTER_MASK;

	/* Debug */
	//i2c_sp_dump_reg(hi2c);
}

static void i2c_sp_read(I2C_HandleTypeDef *hi2c)
{
	uint8_t rx_valid;
	uint32_t data;
	struct i2c_msg *msgs = hi2c->msgs;

	for (; hi2c->msg_read_idx < hi2c->msgs_num; hi2c->msg_read_idx++) {
		uint32_t len;
		uint8_t *buf;

		if (!(msgs[hi2c->msg_read_idx].flag & I2C_M_READ))
			continue;

		if (!(hi2c->status & STATUS_READ_IN_PROGRESS)) {
			len = msgs[hi2c->msg_read_idx].len;
			buf = msgs[hi2c->msg_read_idx].buf;
		} else {
			len = hi2c->rx_buf_len;
			buf = hi2c->rx_buf;
		}

		rx_valid = hi2c->Instance->ic_rxflr;//0x78
		for (; len > 0 && rx_valid > 0; len--, rx_valid--) {
			data = hi2c->Instance->ic_data_cmd;
			debug_print("rx_data 0x%x\n", data);
			*buf++ = data;
			hi2c->rx_outstanding--;
		}

		/* RX is full. But this msg read don't end. Save the
		 * buffer address and remaining length to continue
		 * the read in next RX empty.
		 */
		if (len > 0) {
			hi2c->status |= STATUS_READ_IN_PROGRESS;
			hi2c->rx_buf_len = len;
			hi2c->rx_buf = buf;
			return;
		} else {
			hi2c->status &= ~STATUS_READ_IN_PROGRESS;
		}
	}
}

static void i2c_sp_xfer(I2C_HandleTypeDef *hi2c)
{
	uint32_t tx_free, rx_free;
	uint32_t intr_mask;
	struct i2c_msg *msgs = hi2c->msgs;
	uint32_t addr = msgs[hi2c->msg_write_idx].addr;
	uint32_t buf_len = hi2c->tx_buf_len;
	uint8_t *buf = hi2c->tx_buf;
	uint32_t temp = 0;

	intr_mask = SP_IC_INTR_MASTER_MASK;

	for (; hi2c->msg_write_idx < hi2c->msgs_num; hi2c->msg_write_idx++) {

		if (msgs[hi2c->msg_write_idx].addr != addr) {
			printf("%s: invalid target address\n", __func__);
			hi2c->msg_err = 1;
			break;
		}
		if (!(hi2c->status & STATUS_WRITE_IN_PROGRESS)) {
			debug_print("write progress %d\n", hi2c->msg_write_idx);
			/* new i2c_msg */
			buf = msgs[hi2c->msg_write_idx].buf;
			buf_len = msgs[hi2c->msg_write_idx].len;
		}

		tx_free = I2C_FIFO_DEPTH - hi2c->Instance->ic_txflr;
		rx_free = I2C_FIFO_DEPTH - hi2c->Instance->ic_rxflr;
		debug_print("tx_free %d, rx_free %d\n", tx_free, rx_free);
		while (buf_len > 0 && tx_free > 0 && rx_free > 0) {

			if (msgs[hi2c->msg_write_idx].flag & I2C_M_READ) {
				debug_print("11\n");
				/* Avoid rx buffer overrun */
				if (hi2c->rx_outstanding >= I2C_FIFO_DEPTH)
					break;

				temp |= I2C_READ_DATA;
				hi2c->Instance->ic_data_cmd = temp;
				rx_free--;
				hi2c->rx_outstanding++;
			} else {
				debug_print("22\n");
				temp |= *buf++;
				debug_print("data 0x%x\n", temp);
				hi2c->Instance->ic_data_cmd = temp;
			}
			temp = 0;
			tx_free--; buf_len--;
		}

		hi2c->tx_buf = buf;
		hi2c->tx_buf_len = buf_len;

		if (buf_len > 0) {
			hi2c->status |= STATUS_WRITE_IN_PROGRESS;
			break;
		} else {
			hi2c->status &= ~STATUS_WRITE_IN_PROGRESS;
		}
	}

	/*
	 * If i2c_msg index search is completed, we don't need TX_EMPTY
	 * interrupt any more.
	 */
	if (hi2c->msg_write_idx == hi2c->msgs_num)
		intr_mask &= ~SP_IC_INTR_TX_EMPTY;

	if (hi2c->msg_err)
		intr_mask = 0;

	hi2c->Instance->ic_intr_mask = intr_mask;
}

void i2c_irq_handler(I2C_HandleTypeDef *hi2c)
{
	uint8_t enabled;
	uint32_t stat;
		
	enabled = hi2c->Instance->ic_enable;//0x6c
	stat = hi2c->Instance->ic_raw_intr_stat;//0x34
	/* End the hanlder: 1.I2C disable 2.Interrupt which I2C activity generate */
	debug_print("stat %x\n", stat);
	if (!enabled || !(stat & ~SP_IC_INTR_ACTIVITY))
		return;

	stat = i2c_sp_read_clear_intrbits(hi2c);
	debug_print("stat2 %x\n", stat);
	if (stat & SP_IC_INTR_TX_ABRT) {
		i2c_sp_handle_tx_abort(hi2c);
		hi2c->Instance->ic_intr_mask = 0;
		hi2c->State = HAL_I2C_STATE_ABORT;
		goto tx_abort;
	}
	if (stat & SP_IC_INTR_RX_FULL)
		i2c_sp_read(hi2c);
	if (stat & SP_IC_INTR_TX_EMPTY)
		i2c_sp_xfer(hi2c);
tx_abort:
	if (stat & (SP_IC_INTR_STOP_DET | SP_IC_INTR_TX_ABRT) || hi2c->msg_err) {
		hi2c->completion = 0;
		hi2c->State = HAL_I2C_STATE_READY;
	}
}

HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
	printf("\n%s %s\n", __FUNCTION__, __TIME__);
	if(hi2c == NULL) {
		return HAL_ERROR;
	}
	hi2c->State = HAL_I2C_STATE_RESET;

	/* Fill the private data */
	i2c_detect(hi2c);
	/* Clock enable  */
	i2c_clken_config(sp_i2c_info[hi2c->Index].clk_id);
	/* Pinmux config  */
	i2c_pinmux_config(sp_i2c_info[hi2c->Index].pinmux);

	/* Set irq trigger mode and irq callback */
	if(hi2c->Mode == I2C_MODE_INTR) {
		//i2c_irq_config(sp_i2c_info[hi2c->Index].irq_num, sp_i2c_info[hi2c->Index].irq_callback);
		i2c_irq_config(sp_i2c_info[hi2c->Index].irq_num);
		gpHandle[hi2c->Index] = hi2c;
	}

	__i2c_init_master(hi2c);

	//HAL_Delay(1000);

	//xtdebug
	//i2c_sp_dump_reg(hi2c);

	hi2c->ErrorCode = HAL_I2C_ERR_NONE;

	hi2c->State = HAL_I2C_STATE_READY;
	return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c)
{
	if (hi2c == NULL)
	{
		return HAL_ERROR;
	}

	if(hi2c->Mode == I2C_MODE_INTR) {
		IRQ_Disable(sp_i2c_info[hi2c->Index].irq_num);
	}

	return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Master_Transfer(I2C_HandleTypeDef *hi2c, struct i2c_msg *msgs, int num)
{
	uint32_t stat, enabled, tickstart;
	int ret;

	if (hi2c->State != HAL_I2C_STATE_READY)
		return HAL_BUSY;

	__HAL_LOCK(hi2c);

	ret = i2c_sp_wait_bus_not_busy(hi2c, 2560);
	if (ret < 0)
		goto done;

	hi2c->msgs = msgs;
	hi2c->msgs_num = num;

	i2c_sp_xfer_prepare(hi2c, msgs[0].addr, NULL, 0);

	if (hi2c->Mode == I2C_MODE_BURST) {
		tickstart= HAL_GetTick();
		while (hi2c->completion) {
			/* 2s */
			if (i2c_sp_timeout(hi2c, 2000000, tickstart)) {
				printf("[I2C]Xfer Timeout!\n");
				hi2c->ErrorCode |= HAL_I2C_ERR_TIMEOUT;
				goto done;
			}

			i2c_irq_handler(hi2c);
		}
	}

	if (hi2c->Mode == I2C_MODE_INTR) {
		tickstart= HAL_GetTick();
		while (hi2c->completion) {
			/* 2s */
			if (i2c_sp_timeout(hi2c, 2000000, tickstart)) {
				printf("[I2C]Xfer Timeout!\n");
				hi2c->ErrorCode |= HAL_I2C_ERR_TIMEOUT;
				goto done;
			}
		}
	}

	hi2c->Instance->ic_intr_mask = 0;
	stat = hi2c->Instance->ic_clr_intr;

	/* Disable IC nowait */
	hi2c->Instance->ic_enable = 0;
done:
	__HAL_UNLOCK(hi2c);
	return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t Size,
                                          uint32_t Timeout)
{
	UNUSED(Timeout);
	struct i2c_msg msg[] = {
		{
			.addr = DevAddress,
			.flag = I2C_M_WRITE,
			.len  = Size,
			.buf  = pData,
		},
	};
	return HAL_I2C_Master_Transfer(hi2c, msg, 1);
}

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t Size,
                                         uint32_t Timeout)
{
	UNUSED(Timeout);
	struct i2c_msg msg[] = {
		{
			.addr = DevAddress,
			.flag = I2C_M_READ,
			.len  = Size,
			.buf  = pData,
		},
	};
	return HAL_I2C_Master_Transfer(hi2c, msg, 1);
}

void HAL_I2C_IRQHandler(void)
{
	uint32_t irqn, index;

	irqn = __get_IPSR() - 16;
	index = i2c_irq_get_index(irqn);

	i2c_irq_handler(gpHandle[index]);
}

HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef *hi2c)
{
	/* Return I2C handle state */
	return hi2c->State;
}

uint32_t HAL_I2C_GetError(I2C_HandleTypeDef *hi2c)
{
	return hi2c->ErrorCode;
}

void HAL_I2C_ClearError(I2C_HandleTypeDef * hi2c)
{
	hi2c->ErrorCode = HAL_I2C_ERR_NONE;
}
