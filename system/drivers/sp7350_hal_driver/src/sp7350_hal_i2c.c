#include "sp7350_hal_i2c.h"
#include "sp7350_hal_dma.h"
#include <stdlib.h>

#define HAL_I2C_DEBUG
#ifdef HAL_I2C_DEBUG
#define debug_print(format, ...) printf("[%s:%d] "format"", __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define debug_print(format, ...)
#endif

#define STAMP(v)                                   \
	do {                                       \
		*(volatile uint32_t *)0xf8000000UL = v; \
	} while (0)

static void i2c_sp_xfer_prepare(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t *buf, uint32_t len, uint8_t mode);
static void i2c_sp_xfer(I2C_HandleTypeDef *hi2c);
static void i2c_sp_read(I2C_HandleTypeDef *hi2c);
static void i2c_sp_wait_bus_not_busy(I2C_HandleTypeDef *hi2c, uint32_t timeout, uint8_t mode);
static uint32_t i2c_sp_read_clear_intrbits(I2C_HandleTypeDef *hi2c);
static uint32_t i2c_sp_timeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);
static void i2c_sp_dump_reg(I2C_HandleTypeDef *hi2c);
void i2c_sp_handle_tx_abort(I2C_HandleTypeDef *hi2c);

HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{

	if(hi2c == NULL)
	{
		return HAL_ERROR;
	}

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

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size, I2C_MODE_BURST);

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

	i2c_sp_wait_bus_not_busy(hi2c, 2560, I2C_MODE_BURST);
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
	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size, I2C_MODE_BURST);

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

	i2c_sp_wait_bus_not_busy(hi2c, 2560, I2C_MODE_BURST);

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

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size, I2C_MODE_DMA);

	HAL_DMA_Start((uint32_t)pData, hi2c->DMAIndex, Size, NULL);

	i2c_sp_wait_bus_not_busy(hi2c, 2560, I2C_MODE_BURST);

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

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size, I2C_MODE_DMA);

	hi2c->Instance->ic_data_cmd = I2C_READ_DATA;

	//i2c_sp_wait_bus_not_busy(hi2c, 2560, I2C_MODE_DMA);

	HAL_DMA_Start(hi2c->DMAIndex, (uint32_t)pData, Size, NULL);

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

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size, I2C_MODE_INTR);

	i2c_sp_wait_bus_not_busy(hi2c, 1000000, I2C_MODE_INTR);

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

	i2c_sp_xfer_prepare(hi2c, DevAddress, pData, Size, I2C_MODE_INTR);

	i2c_sp_wait_bus_not_busy(hi2c, 1000000, I2C_MODE_INTR);

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
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
                                              uint16_t Size)
{
	if (hi2c->State != HAL_I2C_STATE_READY)
		return HAL_BUSY;

	__HAL_LOCK(hi2c);



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
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
                                             uint16_t Size)
{
	if (hi2c->State != HAL_I2C_STATE_READY)
		return HAL_BUSY;

	__HAL_LOCK(hi2c);



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

static void i2c_sp_wait_bus_not_busy(I2C_HandleTypeDef *hi2c, uint32_t timeout, uint8_t mode)
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

		if(mode == I2C_MODE_BURST) {
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

static void i2c_sp_xfer_prepare(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t *buf, uint32_t len, uint8_t mode)
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
	temp_reg |= SP_IC_CON_SPEED_STD;

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
	if(mode == I2C_MODE_DMA) {
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
	uint32_t temp;

	intr_mask = SP_IC_INTR_MASTER_MASK;

	tx_free = I2C_FIFO_DEPTH - hi2c->Instance->ic_txflr;
	rx_free = I2C_FIFO_DEPTH - hi2c->Instance->ic_rxflr;
	//debug_print("tx_free %d, rx_free %d\n", tx_free, rx_free);
	while(hi2c->DataTotalLen > 0 && tx_free > 0 && rx_free > 0) {
		if(hi2c->State == HAL_I2C_STATE_BUSY_TX) {
			temp = *hi2c->Buffer++;
			//debug_print("data 0x%x\n", temp);
			hi2c->Instance->ic_data_cmd = temp;
			tx_free--;
		} else {
			if(hi2c->RxOutStanding >= rx_free)
				break;
			hi2c->Instance->ic_data_cmd = I2C_READ_DATA;
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
	int i;

	if (abort_source & SP_IC_TX_ABRT_NOACK) {
		for(i = 0; i <= 4; i++)
			printf("NACK fail \n");
		return;
	}

	for(i = 5; i <= 13; i++)
		printf("NACK fail 02\n");
}

#ifdef HAL_I2C_DEBUG

I2C_HandleTypeDef *slave_hi2c;
#define XTOR_SLAVE_ADDRESS	0x02
#define I2C_SLAVE_ADDRESS	0x55

void sp_i2c_slave_irq(void)
{
	uint32_t read_clr;

	read_clr = slave_hi2c->Instance->ic_clr_rd_req;

	for(int i = 1; i < 7; i++)
		slave_hi2c->Instance->ic_data_cmd = (i << 4) | i;
}

void sp_i2c_slave(I2C_HandleTypeDef *hi2c)
{
	hi2c->Instance->ic_enable = 0;
	hi2c->Instance->ic_sar = I2C_SLAVE_ADDRESS;
	hi2c->Instance->ic_con = 0x22;
	hi2c->Instance->ic_intr_mask = 0x20;
	hi2c->Instance->ic_enable = 0x1;
}

/* Only for HAL DEBUG */
void sp_i2c_en(I2C_HandleTypeDef *hi2c)
{
	switch (hi2c->Index) {
		case 0:
			hi2c->Instance = SP_I2CM0;
			HAL_PINMUX_Cfg(PINMUX_I2C_0, 1); // cfg3[4:3]=1  i2c1 pin
		break;
		case 1:
			hi2c->Instance = SP_I2CM1;
			HAL_PINMUX_Cfg(PINMUX_I2C_1, 1);
		break;
		case 2:
			hi2c->Instance = SP_I2CM2;
			HAL_PINMUX_Cfg(PINMUX_I2C_2, 1);
		break;
		case 3:
			hi2c->Instance = SP_I2CM3;
			HAL_PINMUX_Cfg(PINMUX_I2C_3, 1);
		break;
		case 4:
			hi2c->Instance = SP_I2CM4;
			HAL_PINMUX_Cfg(PINMUX_I2C_4, 1);
		break;
		case 5:
			hi2c->Instance = SP_I2CM5;
			HAL_PINMUX_Cfg(PINMUX_I2C_5, 1);
		break;
		case 6:
			hi2c->Instance = SP_I2CM6;
			HAL_PINMUX_Cfg(PINMUX_I2C_6, 1);
		break;
		case 7:
			hi2c->Instance = SP_I2CM7;
			HAL_PINMUX_Cfg(PINMUX_I2C_7, 1);
		break;
		case 8:
			hi2c->Instance = SP_I2CM8;
			HAL_PINMUX_Cfg(PINMUX_I2C_8, 1);
		break;
		case 9:
			hi2c->Instance = SP_I2CM9;
			HAL_PINMUX_Cfg(PINMUX_I2C_9, 1);
		break;
	}
	HAL_Module_Clock_enable(I2CM0 + hi2c->Index, 1);
	HAL_Module_Clock_gate(I2CM0 + hi2c->Index, 0);
	HAL_Module_Reset(I2CM0 + hi2c->Index, 0);

	printf("i2c addr 0x%08x\n", (unsigned int)hi2c->Instance);
	printf("pinmux 0x%08x(1<<3)\n", *(unsigned int *)(0xf880008c));
	printf("clken  0x%08x(1<<9)\n", *(unsigned int *)(0xf8800124));
	printf("gclken 0x%08x(0<<9)\n", *(unsigned int *)(0xf880015c));
	printf("reset  0x%08x(0<<9)\n", *(unsigned int *)(0xf8800024));
}

void HAL_I2C_SLAVE_TEST(void)
{
	uint8_t buf[2];
	uint8_t *rx_buf = malloc(8);
	memset(rx_buf, 0, 8);

	I2C_HandleTypeDef hi2c0;
	I2C_HandleTypeDef hi2c1;

	hi2c0.Index = 0;
	hi2c1.Index = 1;//I2C 1

	IRQ_SetMode(I2C_MASTER1_IRQ, IRQ_MODE_TRIG_LEVEL_HIGH);
	IRQ_SetHandler(I2C_MASTER1_IRQ, sp_i2c_slave_irq);
	IRQ_Enable(I2C_MASTER1_IRQ);
	sp_i2c_en(&hi2c1);
	sp_i2c_slave(&hi2c1);

	slave_hi2c = &hi2c1;

	sp_i2c_en(&hi2c0);
	HAL_I2C_Init(&hi2c0);
	HAL_I2C_Master_Receive(&hi2c0, I2C_SLAVE_ADDRESS, rx_buf, 6, 0);
	for(int i = 0; i < 6; i++) {
		printf("0x%x\n",*(rx_buf + 1));
	}
}

void HAL_I2C_TEST(void)
{
	I2C_HandleTypeDef hi2c;
	uint8_t buf[2];
	uint8_t *rx_buf = malloc(4);

	buf[0] = 0x11;
	memset(rx_buf, 0, 4);

	hi2c.Index = 0;

	sp_i2c_en(&hi2c);
	HAL_I2C_Init(&hi2c);
	HAL_I2C_Master_Transmit(&hi2c, XTOR_SLAVE_ADDRESS, buf, 1, 0);
	HAL_I2C_Master_Receive(&hi2c, XTOR_SLAVE_ADDRESS, rx_buf, 1, 0);
	printf("2!!!! 0x%x \n", *rx_buf);
	if(*rx_buf == 0x82)
		buf[0] = 0x22;
		buf[1] = 0x33;
		HAL_I2C_Master_Transmit(&hi2c, XTOR_SLAVE_ADDRESS, buf, 2, 0);
	printf("!!!! i2c test end \n");
}
#endif
