#include "sp7350_hal_i2c.h"
#include "sp7350_hal_dma.h"
#include <stdlib.h>

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

static void i2c_sp_xfer_prepare(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t *buf, uint32_t len);
static void i2c_sp_xfer(I2C_HandleTypeDef *hi2c);
static void i2c_sp_read(I2C_HandleTypeDef *hi2c);
static void i2c_sp_wait_bus_not_busy(I2C_HandleTypeDef *hi2c, uint32_t timeout);
static uint32_t i2c_sp_read_clear_intrbits(I2C_HandleTypeDef *hi2c);
static uint32_t i2c_sp_timeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);
static void i2c_sp_dump_reg(I2C_HandleTypeDef *hi2c);
static void i2c_sp_handle_tx_abort(I2C_HandleTypeDef *hi2c);
static void i2c_irq_handler(void);

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
			handle->DMAIndex = sp_i2c_info[i].dma_idx;
			break;
		}
	}
}

static void i2c_irq_config(IRQn_Type irqn)
{
	IRQ_SetMode(irqn, IRQ_MODE_TRIG_LEVEL_HIGH);
	IRQ_SetHandler(irqn, i2c_irq_handler);
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

static uint8_t i2c_speed_config(I2C_HandleTypeDef *hi2c, uint32_t speed)
{
	uint8_t temp_reg;

	MOON3_REG->sft_cfg[27] = (3 << 22) | (0 << 6);
	if(speed < I2C_MAX_STANDARD_MODE_FREQ) {
		temp_reg = SP_IC_CON_SPEED_STD;
		hi2c->Instance->ic_ss_scl_hcnt = 397;
		hi2c->Instance->ic_ss_scl_lcnt = 496;
	} else if(speed < I2C_MAX_FAST_MODE_FREQ) {
		temp_reg = SP_IC_CON_SPEED_FAST;
		hi2c->Instance->ic_fs_scl_hcnt = 57;
		hi2c->Instance->ic_fs_scl_lcnt = 129;
	} else if(speed < I2C_MAX_FAST_MODE_PLUS_FREQ) {
		temp_reg = SP_IC_CON_SPEED_HIGH;
	}
	return temp_reg;
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

static void i2c_deinit(i2c_index_e i2c_index)
{

}

void i2c_irq_handler(void)
{
	uint32_t irqn, index;

	irqn = __get_IPSR() - 16;
	index = i2c_irq_get_index(irqn);

	HAL_I2C_IRQHandler(gpHandle[index]);
}

HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
	//printf("%s\n",__TIME__);
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
	/* Speed config */
	hi2c->Speed = i2c_speed_config(hi2c, hi2c->Init.Timing);
	/* Set irq trigger mode and irq callback */
	if(hi2c->Mode == I2C_MODE_INTR) {
		//i2c_irq_config(sp_i2c_info[hi2c->Index].irq_num, sp_i2c_info[hi2c->Index].irq_callback);
		i2c_irq_config(sp_i2c_info[hi2c->Index].irq_num);
		gpHandle[hi2c->Index] = hi2c;
	}

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
	uint32_t stat;
	//printf("%s\n", __TIME__);
	if (hi2c->State != HAL_I2C_STATE_READY)
		return HAL_BUSY;

	__HAL_LOCK(hi2c);

	i2c_sp_xfer_prepare(hi2c, msgs[0].addr , NULL, 0);
	//i2c_sp_dump_reg(hi2c);

	for(int i = 0; i < num; i++) {
		if (msgs[i].flag & I2C_M_RESTART)
			hi2c->NeedRestart = 1;

		if (msgs[i].flag & I2C_M_STOP)
			hi2c->NeedStop = 1;

		if (msgs[i].flag & I2C_M_READ) {
			hi2c->State = HAL_I2C_STATE_BUSY_RX;
			hi2c->ReadLen = msgs[i].len;
		} else {
			hi2c->State = HAL_I2C_STATE_BUSY_TX;
			hi2c->ReadLen = 0;
		}

		hi2c->DataTotalLen = msgs[i].len;
		hi2c->Buffer = msgs[i].buf;

		debug_print("i %d State 0x%x DataTotalLen %d ReadLen %d \n", i, hi2c->State, hi2c->DataTotalLen, hi2c->ReadLen);
		while(hi2c->DataTotalLen || hi2c->ReadLen) {

			i2c_sp_xfer(hi2c);

			stat = hi2c->Instance->ic_status;
			if (stat & SP_IC_STATUS_RFNE) {
				debug_print("r ne\n");
				i2c_sp_read(hi2c);
			}

			stat = i2c_sp_read_clear_intrbits(hi2c);
			if (stat & SP_IC_INTR_TX_ABRT) {
				i2c_sp_handle_tx_abort(hi2c);
				hi2c->XferAction = 0;
				hi2c->State = HAL_I2C_STATE_ABORT;
				break;
			}
		}

	}
	i2c_sp_wait_bus_not_busy(hi2c, 2560);
	hi2c->Instance->ic_intr_mask = 0;
	stat = hi2c->Instance->ic_clr_intr;
	hi2c->Instance->ic_enable = 0;

	__HAL_UNLOCK(hi2c);
	return HAL_OK;
}


HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t Size,
                                          uint32_t Timeout)
{
	uint32_t stat;

	UNUSED(Timeout);

	if (hi2c->State != HAL_I2C_STATE_READY)
		return HAL_BUSY;

	__HAL_LOCK(hi2c);

	hi2c->State = HAL_I2C_STATE_BUSY_TX;

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size);

	while(hi2c->DataTotalLen) {

		i2c_sp_xfer(hi2c);

		stat = i2c_sp_read_clear_intrbits(hi2c);
		if (stat & SP_IC_INTR_TX_ABRT) {
			i2c_sp_handle_tx_abort(hi2c);
			hi2c->XferAction = 0;
			hi2c->State = HAL_I2C_STATE_ABORT;
			break;
		}
	}

	i2c_sp_wait_bus_not_busy(hi2c, 2560);
	//debug_print("t\n");
	hi2c->Instance->ic_intr_mask = 0;
	stat = hi2c->Instance->ic_clr_intr;
	hi2c->Instance->ic_enable = 0;

	__HAL_UNLOCK(hi2c);
	return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t Size,
                                         uint32_t Timeout)
{
	UNUSED(Timeout);

	if (hi2c->State != HAL_I2C_STATE_READY)
		return HAL_BUSY;

	__HAL_LOCK(hi2c);
	uint32_t stat;

	hi2c->State = HAL_I2C_STATE_BUSY_RX;
	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size);

	while(hi2c->ReadLen) {

		i2c_sp_xfer(hi2c);

		stat = hi2c->Instance->ic_status;
		if (stat & SP_IC_STATUS_RFNE)
			i2c_sp_read(hi2c);

		stat = i2c_sp_read_clear_intrbits(hi2c);
		if (stat & SP_IC_INTR_TX_ABRT) {
			i2c_sp_handle_tx_abort(hi2c);
			hi2c->XferAction = 0;
			hi2c->State = HAL_I2C_STATE_ABORT;
			break;
		}
	}

	i2c_sp_wait_bus_not_busy(hi2c, 2560);

	hi2c->Instance->ic_intr_mask = 0;
	stat = hi2c->Instance->ic_clr_intr;
	hi2c->Instance->ic_enable = 0;

	__HAL_UNLOCK(hi2c);
	return HAL_OK;
}


/**
  * @brief  Transmit in master mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
                                              uint32_t Size)
{
	uint32_t stat;
	if (hi2c->State != HAL_I2C_STATE_READY)
		return HAL_BUSY;

    	__HAL_LOCK(hi2c);
	hi2c->State = HAL_I2C_STATE_BUSY_DMA_TX;

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size);

	HAL_DMA_Start((uint32_t)pData, hi2c->DMAIndex, Size, NULL, NULL);

	i2c_sp_wait_bus_not_busy(hi2c, 2560);

	/* Diable all the interrupt */
	hi2c->Instance->ic_intr_mask = 0;
	/* Clear all the interrupt*/
	stat = hi2c->Instance->ic_clr_intr;
	/* Disable the I2C */
	hi2c->Instance->ic_enable = 0;

	hi2c->State = HAL_I2C_STATE_READY;
	__HAL_UNLOCK(hi2c);

	return HAL_OK;
}

/**
  * @brief  Receive in master mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
                                             uint32_t Size)
{
	uint32_t stat;
	if (hi2c->State != HAL_I2C_STATE_READY)
  		return HAL_BUSY;

	/* Process Locked */
    	__HAL_LOCK(hi2c);
	hi2c->State = HAL_I2C_STATE_BUSY_DMA_RX;

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size);

	hi2c->Instance->ic_data_cmd = I2C_READ_DATA;

	//i2c_sp_wait_bus_not_busy(hi2c, 2560);

	HAL_DMA_Start(hi2c->DMAIndex, (uint32_t)pData, Size, NULL, NULL);

	hi2c->Instance->ic_intr_mask = 0;
	stat = hi2c->Instance->ic_clr_intr;
	hi2c->Instance->ic_enable = 0;

	hi2c->State = HAL_I2C_STATE_READY;
	__HAL_UNLOCK(hi2c);

	return HAL_OK;
}

/**
  * @brief  Transmit in master mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
                                             uint32_t Size)
{
	uint32_t stat;
	if (hi2c->State != HAL_I2C_STATE_READY)
		return HAL_BUSY;

	__HAL_LOCK(hi2c);

	hi2c->State = HAL_I2C_STATE_BUSY_TX;

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size);

	i2c_sp_wait_bus_not_busy(hi2c, 1000000);

	hi2c->Instance->ic_intr_mask = 0;
	stat = hi2c->Instance->ic_clr_intr;
	hi2c->Instance->ic_enable = 0;

	__HAL_UNLOCK(hi2c);

	return HAL_OK;
}

/**
  * @brief  Receive in master mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t Size)
{
	uint32_t stat;
	if (hi2c->State != HAL_I2C_STATE_READY)
		return HAL_BUSY;

	__HAL_LOCK(hi2c);

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size);

	i2c_sp_wait_bus_not_busy(hi2c, 1000000);

	hi2c->Instance->ic_intr_mask = 0;
	stat = hi2c->Instance->ic_clr_intr;
	hi2c->Instance->ic_enable = 0;

	__HAL_UNLOCK(hi2c);

	return HAL_OK;
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

void HAL_I2C_IRQHandler(I2C_HandleTypeDef *hi2c)
{
	uint8_t enabled;
	uint32_t stat;
	//printf("h\n");
	enabled = hi2c->Instance->ic_enable;//0x6c
	stat = hi2c->Instance->ic_raw_intr_stat;//0x34
	/* End the hanlder: 1.I2C disable 2.Interrupt which I2C activity generate */
	if (!enabled || !(stat & ~SP_IC_INTR_ACTIVITY))
		return;

	stat = i2c_sp_read_clear_intrbits(hi2c);
	if (stat & SP_IC_INTR_TX_ABRT) {
		//i2c_sp_handle_tx_abort(hi2c);
		hi2c->Instance->ic_intr_mask = 0;
		hi2c->State = HAL_I2C_STATE_ABORT;
		return;
	}
	if (stat & SP_IC_INTR_RX_FULL)
		i2c_sp_read(hi2c);

	if (stat & SP_IC_INTR_TX_EMPTY)
		i2c_sp_xfer(hi2c);

	if (stat & (SP_IC_INTR_STOP_DET | SP_IC_INTR_TX_ABRT)) {
		hi2c->XferAction = 0;
		hi2c->State = HAL_I2C_STATE_READY;
	}
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

static uint32_t i2c_sp_read_clear_intrbits(I2C_HandleTypeDef *hi2c)
{
	unsigned int stat;
	unsigned int temp;

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

	/*0x2C
	 *
	 */
	stat = (uint32_t)hi2c->Instance->ic_intr_stat;
	temp = stat;
	/*
	 * Do not use the IC_CLR_INTR register to clear interrupts, or
	 * you'll miss some interrupts, triggered during the period from
	 * readl(IC_INTR_STAT) to readl(IC_CLR_INTR).
	 *
	 * Instead, use the separately-prepared IC_CLR_* registers.
	 */
	if (temp & SP_IC_INTR_RX_UNDER)
		temp = (uint32_t)hi2c->Instance->ic_clr_rx_under;
	if (stat & SP_IC_INTR_RX_OVER)
		temp = (uint32_t)hi2c->Instance->ic_clr_rx_over;
	if (stat & SP_IC_INTR_TX_OVER)
		temp = (uint32_t)hi2c->Instance->ic_clr_tx_over;
	if (stat & SP_IC_INTR_RD_REQ)
		temp = (uint32_t)hi2c->Instance->ic_clr_rd_req;
	if (stat & SP_IC_INTR_TX_ABRT) {
		/*
		 * The IC_TX_ABRT_SOURCE register is cleared whenever
		 * the IC_CLR_TX_ABRT is read.  Preserve it beforehand.
		 */
		hi2c->AbortSource = (unsigned int)hi2c->Instance->ic_tx_abrt_source;
		temp = (uint32_t)hi2c->Instance->ic_clr_tx_abrt;
	}
	if (stat & SP_IC_INTR_RX_DONE)
		temp = (uint32_t)hi2c->Instance->ic_clr_rx_done;
	if (stat & SP_IC_INTR_ACTIVITY)
		temp = (uint32_t)hi2c->Instance->ic_clr_tx_activity;
	if (stat & SP_IC_INTR_STOP_DET)
		temp = (uint32_t)hi2c->Instance->ic_clr_stop_det;
	if (stat & SP_IC_INTR_START_DET)
		temp = (uint32_t)hi2c->Instance->ic_clr_start_dft;
	if (stat & SP_IC_INTR_GEN_CALL)
		temp = (uint32_t)hi2c->Instance->ic_clr_gen_call;

	return stat;
}

static void i2c_sp_wait_bus_not_busy(I2C_HandleTypeDef *hi2c, uint32_t timeout)
{
	uint32_t stat;
	uint32_t tickstart;

	tickstart= HAL_GetTick();
	while(hi2c->XferAction)
	{
		/* Timeout function */
		if (i2c_sp_timeout(hi2c, timeout, tickstart)) {
			/* 640us = fifo_depth / 100kbit/s and the STC freq = 1MHz*/
			printf("[I2C]Timeout!\n");
			hi2c->ErrorCode |= HAL_I2C_ERR_TIMEOUT;
			break;
		}

		stat = hi2c->Instance->ic_status;
		if ((stat & SP_IC_STATUS_MST_ACT) != SP_IC_STATUS_MST_ACT) {
			hi2c->XferAction = 0;
			hi2c->State = HAL_I2C_STATE_READY;
		}

		if(hi2c->Mode == I2C_MODE_BURST) {
			stat = i2c_sp_read_clear_intrbits(hi2c);
			if (stat & SP_IC_INTR_TX_ABRT) {
				i2c_sp_handle_tx_abort(hi2c);
				hi2c->XferAction = 0;
				hi2c->State = HAL_I2C_STATE_ABORT;
				break;
			}
		}
	}
}

static void i2c_sp_xfer_prepare(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t *buf, uint32_t len)
{
	uint32_t temp_reg, stat;

	hi2c->Buffer = buf;
	hi2c->DataIndex = 0;
	hi2c->AbortSource =  0;
	hi2c->DataTotalLen = len;
	hi2c->ReadLen = len;
	hi2c->RxOutStanding = 0;
	hi2c->XferAction = 1;
	hi2c->XferWaitTxEnd = 0;
	hi2c->NeedRestart = 0;
	hi2c->NeedStop = 0;

	//xtdebug
#if 0
	printf("ic_con 0x%x\n", hi2c->Instance->ic_con);
	printf("ic_tar 0x%x\n", hi2c->Instance->ic_tar);
	printf("ic_intr_mask 0x%x\n", hi2c->Instance->ic_intr_mask);
	printf("ic_tx_tl 0x%x\n", hi2c->Instance->ic_tx_tl);
	printf("ic_rx_tl 0x%x\n", hi2c->Instance->ic_rx_tl);
	printf("ic_enable 0x%x\n", hi2c->Instance->ic_enable);
#endif
/******** TODO: put the master initialization code in HAL_init()*********/
	uint32_t timeout = 100;
	do {
		hi2c->Instance->ic_enable = 0;
		/*
		 * The enable status register may be unimplemented, but
		 * in that case this test reads zero and exits the loop.
		 */
		temp_reg = hi2c->Instance->ic_enable_status;
		if ((temp_reg & 1) == 0)
			break;
		/*
		 * Wait 10 times the signaling period of the highest I2C
		 * transfer supported by the driver (for 400KHz this is
		 * 25us) as described in the DesignWare I2C databook.
		 */
		HAL_Delay(25);
	} while (timeout--);

	/*0x0 CON
	 *1.bit[0]   set 1:Master mode enable
	 *2.bit[6]   set 1:Slave mode disable
	 *3.bit[5]   set 1:restart enable, send start bit
	 *4.bit[2:1] set 1:standard 100kbit/s
	 */
	/* Init master */
	temp_reg = SP_IC_CON_MASTER | SP_IC_CON_SLAVE_DISABLE | SP_IC_CON_RESTART_EN;
	temp_reg |= hi2c->Speed;

	hi2c->Instance->ic_con = temp_reg;
/******** TODO: put the master initialization code in init_master() *********/

	/*0x4 TAR
	 *bit[9:0] set	:address
	 */
	hi2c->Instance->ic_tar = addr;

	/* Enforce disabled interrupts (due to HW issues) */
	hi2c->Instance->ic_intr_mask = 0;

	/*0x9C ENABLE STATUS RO
	 *bit[0] get 1:I2C EN
	 *bit[1] get 1:receive NACK
	 *bit[1] get 1:data lost //??? here slave is board?
	 *0x40 CLR INTR RO
	 *bit[0] :clear the interrupt
	 */
	/* Dummy read to avoid the register getting stuck on Bay Trail */
	temp_reg = hi2c->Instance->ic_enable_status;

	/* Clear and enable interrupts */
	temp_reg = hi2c->Instance->ic_clr_intr;

	/*0x30 INTR MASK
	 *bit[2] set 1:RX FULL
	 *bit[6] set 1:TX ABRT
	 *bit[9] set 1:STOP DET
	 *
	 *bit[4] set 1:TX EMPTY
	 */
	temp_reg = SP_IC_INTR_MASTER_MASK;
	hi2c->Instance->ic_intr_mask = temp_reg;

	/* control the thresold of trigger the RX_FULL/TX_EMPTY interrupt */
	hi2c->Instance->ic_tx_tl = I2C_FIFO_DEPTH / 2;	/* < 4, trig intr */
	hi2c->Instance->ic_rx_tl = 0;			/* > 0, trig intr */

	/* Use DMA fifo */
	if(hi2c->Mode == I2C_MODE_DMA) {
		hi2c->Instance->ic_dma_cr = SP_IC_TDMAE | SP_IC_RDMAE;
		hi2c->Instance->ic_dma_tdlr = I2C_TDRL;
		hi2c->Instance->ic_dma_rdlr = I2C_RDRL;
	}

	/*0x6C ENABLE
	 *bit[0]   set 1:i2c moudle enable
	 */
	hi2c->Instance->ic_enable = 1;

	/* ???why clear intr here */
	//i2c_sp_read_clear_intrbits(hi2c);
#if 0
	//xtdebug
	printf("ic_con 0x%x\n", hi2c->Instance->ic_con);
	printf("ic_tar 0x%x\n", hi2c->Instance->ic_tar);
	printf("ic_intr_mask 0x%x\n", hi2c->Instance->ic_intr_mask);
	printf("ic_tx_tl 0x%x\n", hi2c->Instance->ic_tx_tl);
	printf("ic_rx_tl 0x%x\n", hi2c->Instance->ic_rx_tl);
	printf("ic_enable 0x%x\n", hi2c->Instance->ic_enable);
#endif
}

static void i2c_sp_read(I2C_HandleTypeDef *hi2c)
{
	uint8_t rx_valid;
	uint32_t tmp;

	rx_valid = hi2c->Instance->ic_rxflr;//0x78

	while(hi2c->ReadLen > 0 && rx_valid > 0) {
		tmp = hi2c->Instance->ic_data_cmd;
		*hi2c->Buffer++ = tmp;
		hi2c->RxOutStanding--;
		hi2c->ReadLen--;
		rx_valid--;
	}

	if(hi2c->ReadLen == 0) {
		hi2c->Instance->ic_intr_mask = 0;
		tmp = hi2c->Instance->ic_clr_intr;
		hi2c->Instance->ic_enable = 0;
		hi2c->State = HAL_I2C_STATE_READY;
	}
}

static void i2c_sp_xfer(I2C_HandleTypeDef *hi2c)
{
	uint32_t tx_free, rx_free;
	uint32_t intr_mask;
	uint32_t temp = 0;

	intr_mask = SP_IC_INTR_MASTER_MASK;

	tx_free = I2C_FIFO_DEPTH - hi2c->Instance->ic_txflr;
	rx_free = I2C_FIFO_DEPTH - hi2c->Instance->ic_rxflr;
	debug_print("tx_free %d, rx_free %d\n", tx_free, rx_free);
	while(hi2c->DataTotalLen > 0 && tx_free > 0 && rx_free > 0) {
		debug_print("11\n");
		temp = 0;
		if (hi2c->NeedRestart) {
			temp |= BIT(10);
			hi2c->NeedRestart = 0;
		}

		if (hi2c->NeedStop && hi2c->DataTotalLen == 1) {
			temp |= BIT(9);
			hi2c->NeedStop = 0;
		}

		if (hi2c->State == HAL_I2C_STATE_BUSY_TX) {
			debug_print("22\n");
			temp |= *hi2c->Buffer++;
			debug_print("data 0x%x\n", temp);
			hi2c->Instance->ic_data_cmd = temp;
			tx_free--;
		} else if (hi2c->State == HAL_I2C_STATE_BUSY_RX){
			debug_print("33\n");
			if(hi2c->RxOutStanding >= rx_free)
				break;
			temp |= I2C_READ_DATA;
			hi2c->Instance->ic_data_cmd = temp;
			hi2c->RxOutStanding++;
			rx_free--;
		}
		hi2c->DataTotalLen--;
	}

	if (hi2c->DataTotalLen == 0) {
		intr_mask &= ~SP_IC_INTR_TX_EMPTY;
		hi2c->Instance->ic_intr_mask = intr_mask;
		if(hi2c->State == HAL_I2C_STATE_BUSY_TX) {
			hi2c->XferWaitTxEnd = 1;
		}
	}
}

void i2c_sp_dump_reg(I2C_HandleTypeDef *hi2c)
{
	printf("ic_tx_tl 0x%08x\n", (unsigned int)&hi2c->Instance->ic_tx_tl);
	printf("0x%08x\n", hi2c->Instance->ic_tx_tl);
	printf("ic_rx_tl 0x%08x\n", (unsigned int)&hi2c->Instance->ic_rx_tl);
	printf("0x%08x\n", hi2c->Instance->ic_rx_tl);
	printf("ic_con 0x%08x\n", (unsigned int)&hi2c->Instance->ic_con);
	printf("0x%08x\n", hi2c->Instance->ic_con);
	printf("ic_tar 0x%08x\n", (unsigned int)&hi2c->Instance->ic_tar);
	printf("0x%08x\n", hi2c->Instance->ic_tar);
	printf("ic_enable 0x%08x\n", (unsigned int)&hi2c->Instance->ic_enable);
	printf("0x%08x\n", hi2c->Instance->ic_enable);
	printf("ic_enable_status 0x%08x\n", (unsigned int)&hi2c->Instance->ic_enable_status);
	printf("0x%08x\n", hi2c->Instance->ic_enable_status);
	printf("ic_clr_intr 0x%08x\n", (unsigned int)&hi2c->Instance->ic_clr_intr);
	printf("0x%08x\n", hi2c->Instance->ic_clr_intr);
	printf("ic_intr_mask 0x%08x\n", (unsigned int)&hi2c->Instance->ic_intr_mask);
	printf("0x%08x\n", hi2c->Instance->ic_intr_mask);
}

/* 0x80 TX_ABRT_SOURCE */
void i2c_sp_handle_tx_abort(I2C_HandleTypeDef *hi2c)
{
	unsigned int abort_source = hi2c->AbortSource;
	int i, temp_code;

	if (abort_source & SP_IC_TX_ABRT_NOACK) {
		for (i = 0; i <= 4; i++) {
			if (abort_source & ( 1 << i )) {
				printf("NACK fail %d\n", i);
				temp_code = i;
			}

		}
		return;
	}

	for(i = 5; i <= 13; i++)
		printf("NACK fail 02\n");
	if (temp_code == 1)
		hi2c->ErrorCode = HAL_I2C_ERR_ADDRESS_NACK;
}
