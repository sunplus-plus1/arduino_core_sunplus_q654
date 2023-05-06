#include "sp7350_hal_spi.h"

#ifndef min
	#define min(a,b) ((a)<(b)?(a):(b))
#endif

uint32_t _spi_get_SrcClk(void)
{
	uint32_t temp, freq_Hz;
	temp = READ_BIT(MOON3_REG->sft_cfg[27], (0x3<<8));
	temp = temp>>8;
	if (temp == 0x3) {
		freq_Hz = 100*1000*1000;
	} else if (temp == 0x1) {
		freq_Hz = 200*1000*1000;
	} else if (temp == 0) {
		freq_Hz = 400*1000*1000;
	}

	return freq_Hz;
}

void _spi_receive_data(SPI_HandleTypeDef *hspi)
{
	uint32_t maxlen;
	maxlen = min(hspi->Instance->SP_SPI_RXFLR,hspi->RxXferCount);

	while(maxlen)
	{
		if(hspi->pRxBuffPtr)
		{
			*(uint8_t *)hspi->pRxBuffPtr = hspi->Instance->SP_SPI_DR;
			hspi->RxXferCount --;
			hspi->pRxBuffPtr ++;
		}
		maxlen --;
	}
}

void _spi_transmit_data(SPI_HandleTypeDef *hspi)
{
	uint32_t fifo_remain,maxlen;
	//remain fifo len
	fifo_remain = SPI_FIFO_MAX_LENGTH - hspi->Instance->SP_SPI_TXFLR;
	maxlen = min(fifo_remain,hspi->TxXferCount);

	while(maxlen)
	{
		if(hspi->pTxBuffPtr)
		{
			hspi->Instance->SP_SPI_DR = *(uint8_t *)(hspi->pTxBuffPtr);
			hspi->TxXferCount --;
			hspi->pTxBuffPtr ++;
		}
		maxlen --;
	}
}

void _spi_enable_chip(SPI_HandleTypeDef *hspi,int enable)
{
	hspi->Instance->SP_SPI_SSIENR = (enable ? 1:0);
}

void _spi_mask_intr(SPI_HandleTypeDef *hspi, uint32_t mask)
{
	volatile uint32_t new_mask = 0;
	new_mask = READ_REG(hspi->Instance->SP_SPI_IMR) & ~mask;
	WRITE_REG(hspi->Instance->SP_SPI_IMR, new_mask);
}

void _spi_unmask_intr(SPI_HandleTypeDef *hspi, uint32_t mask)
{
	volatile uint32_t new_mask = 0;
	new_mask = READ_REG(hspi->Instance->SP_SPI_IMR) | mask;
	WRITE_REG(hspi->Instance->SP_SPI_IMR, new_mask);
}

void _spi_dma_config(SPI_HandleTypeDef *hspi, int tmode,int txsize,int enable)
{
	hspi->Instance->SP_SPI_DMATDLR = txsize;
	hspi->Instance->SP_SPI_DMARDLR = 0;

	if(enable)
	{
		switch(tmode)
		{
			case SPI_TMOD_TR:
				hspi->Instance->SP_SPI_DMACR = SPI_DMA_TX_ENABLE | SPI_DMA_RX_ENABLE;
				break;
			case SPI_TMOD_TO:
				hspi->Instance->SP_SPI_DMACR = SPI_DMA_TX_ENABLE;
				break;
			case SPI_TMOD_RO:
				hspi->Instance->SP_SPI_DMACR = SPI_DMA_RX_ENABLE;
				break;
			default:
				break;
		}
	}
	else
	{
		hspi->Instance->SP_SPI_DMACR &= ~(SPI_DMA_TX_ENABLE | SPI_DMA_RX_ENABLE);
	}
}

void _spi_reg_ctrl0_set(SPI_HandleTypeDef *hspi,uint8_t tmode)
{
	uint32_t temp_reg=0;

	/* CTRLR0[ 3: 0] Data Frame Size */
	temp_reg |= (8 - 1);
	/* CTRLR0[ 9:8] Transfer Mode */
	temp_reg |= tmode << SPI_TMOD_OFFSET;
	/* CTRLR0[ 5: 4] Frame Format SPI mode */
	temp_reg |= 0 << SPI_FRF_OFFSET;
	/* CTRLR0[11] Shift Register Loop */
	temp_reg |= 0 << SPI_SRL_OFFSET;

	/* CPOL/CPHA select */
	switch(hspi->Init.spi_mode)
	{
		case SPI_MODE0:
			break;
		case SPI_MODE1:
			temp_reg |= CPHA;
			break;
		case SPI_MODE2:
			temp_reg |= CPOL;  
			break;
		case SPI_MODE3:
			temp_reg |= CPHA;
			temp_reg |= CPOL;  
			break;
		default:
			break;
	}

	/*config the spi_config register*/
	hspi->Instance->SP_SPI_CTRLR0 = temp_reg;
}

void _spi_config_set(SPI_HandleTypeDef *hspi,uint8_t tmode)
{
	_spi_enable_chip(hspi,0); //disable ssi before set

	_spi_reg_ctrl0_set(hspi,tmode);
	/*  set spi freq */
	hspi->Instance->SP_SPI_BAUDR = _spi_get_SrcClk() / hspi->Init.spiclk ;
	/*  set irq bit */
	_spi_mask_intr(hspi,0xff);

	_spi_enable_chip(hspi,1);  //enable ssi
}

void _spi_slave_select_enable(SPI_HandleTypeDef *hspi,int enable)
{
	/*  set slave sel and start to transmit */
	hspi->Instance->SP_SPI_SER = (enable ? 1:0);
}

void _spi_txrx_threshold_set(SPI_HandleTypeDef *hspi,uint32_t txsize)
{
	int level = 0;

	/* Originally Tx and Rx data lengths match. */
	level = min(SPI_FIFO_MAX_LENGTH/2,txsize);

	hspi->Instance->SP_SPI_TXFTLR = level;
	hspi->Instance->SP_SPI_RXFTLR = level-1;
}

//   set for receive only 
//   receive serial data until the number of data frames received is equal to this
//   register value plus 1
void _spi_receive_data_set(SPI_HandleTypeDef *hspi,uint32_t RxSize)
{
	_spi_enable_chip(hspi, 0);
	hspi->Instance->SP_SPI_CTRLR1 = RxSize-1; //set receive data size
	_spi_enable_chip(hspi, 1);
}

void _spi_reset(SPI_HandleTypeDef *hspi)
{
	char data;

	_spi_enable_chip(hspi, 0);
	// disable interrupt
	_spi_mask_intr(hspi, 0xff);
	// clear interrupt status
	data = hspi->Instance->SP_SPI_ICR;
	// clear slave sel
	_spi_slave_select_enable(hspi,0);

	_spi_enable_chip(hspi, 1);
	// disable DMA
	_spi_dma_config(hspi,0,0,0);
}

int _spi_check_status(SPI_HandleTypeDef *hspi, int raw)
{

	int irq_status;

	if(raw)
		irq_status = hspi->Instance->SP_SPI_SR;
	else
		irq_status = hspi->Instance->SP_SPI_ISR;

	if(((irq_status & SPI_INT_RXOI) == SPI_INT_RXOI) || ((irq_status & SPI_INT_RXUI) == SPI_INT_RXUI)|| \
		((irq_status & SPI_INT_TXOI) == SPI_INT_TXOI))
	{
		return -1;
	}

	return 0;
}

__weak void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	UNUSED(hspi);
}

__weak void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  UNUSED(hspi);
}

__weak void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	UNUSED(hspi);
}

__weak void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	UNUSED(hspi);
}

HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
	uint32_t div;

	if (hspi == NULL || !IS_SPI_ALL_INSTANCE(hspi->Instance) || !IS_SPI_MODE(hspi->Init.spi_mode) || !IS_VALID_FREQ(hspi->Init.spiclk) )
	{
		return HAL_ERROR;
	}

	hspi->ErrorCode = HAL_SPI_ERROR_NONE;
	hspi->State     = HAL_SPI_STATE_READY;

	hspi->TxCpltCallback       = HAL_SPI_TxCpltCallback;
	hspi->RxCpltCallback       = HAL_SPI_RxCpltCallback;
	hspi->TxRxCpltCallback     = HAL_SPI_TxRxCpltCallback;
	hspi->ErrorCallback     	= HAL_SPI_ErrorCallback;

	return HAL_OK;
}

HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi)
{
	if (hspi == NULL || !IS_SPI_ALL_INSTANCE(hspi->Instance))
	{
		return HAL_ERROR;
	}

	hspi->ErrorCode = HAL_SPI_ERROR_NONE;
	hspi->State = HAL_SPI_STATE_RESET;
	_spi_reset(hspi);

	__HAL_UNLOCK(hspi);

	return HAL_OK;
}

HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	uint32_t tickstart;

	HAL_StatusTypeDef errorcode = HAL_OK;

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto _end;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		errorcode = HAL_ERROR;
		goto _end;
	}

	__HAL_LOCK(hspi);

	tickstart = HAL_GetTick();

	hspi->State       = HAL_SPI_STATE_BUSY_TX;
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pData;
	hspi->TxXferCount = Size;

	_spi_config_set(hspi, SPI_TMOD_TO);

	_spi_txrx_threshold_set(hspi,Size);

	_spi_transmit_data(hspi); //fill data and transmit start

	_spi_slave_select_enable(hspi,1); // select slave and start transmit

	__HAL_UNLOCK(hspi);

	//databook page38,polling TFE first before BUSY Set
	while(hspi->Instance->SP_SPI_SR & SRI_TF_EMPT == SRI_TF_EMPT);
	while((hspi->Instance->SP_SPI_SR & SPI_BUSY) == SPI_BUSY)
	{
		if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
		{
			errorcode = HAL_TIMEOUT;
			goto _end;
		}

		if(_spi_check_status(hspi,true))
		{
			errorcode = HAL_ERROR;
		  goto _end;
		}

		// tx fifo empty,fill data
		if((hspi->TxXferCount > 0) && ((hspi->Instance->SP_SPI_RISR & SPI_INT_TXEI) == SPI_INT_TXEI))
		{
			_spi_transmit_data(hspi);
		}
	}
_end:
	_spi_reset(hspi);
	hspi->State = HAL_SPI_STATE_READY;
	return errorcode;

}

HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	uint32_t tickstart;
	HAL_StatusTypeDef errorcode = HAL_OK;

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto _end;
	}

	if ((pData == NULL) || (Size == 0U) || (Size > 0xFFFF))
	{
		errorcode = HAL_ERROR;
		goto _end;
	}

	/* Process Locked */
	__HAL_LOCK(hspi);

	/* Init tickstart for timeout management*/
	tickstart = HAL_GetTick();

	/* Set the transaction information */
	hspi->State       = HAL_SPI_STATE_BUSY_RX;
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pData;
	hspi->RxXferCount = Size;

	_spi_config_set(hspi, SPI_TMOD_RO);

	_spi_receive_data_set(hspi,Size);

	hspi->Instance->SP_SPI_DR = 0xFF; //write a dummy data to start

	_spi_slave_select_enable(hspi,1); // select slave and start to transmit

	__HAL_UNLOCK(hspi);

	while(hspi->Instance->SP_SPI_SR & SRI_TF_EMPT == SRI_TF_EMPT);

	while((hspi->Instance->SP_SPI_SR & SPI_BUSY) == SPI_BUSY)
	{
		if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
		{
			errorcode = HAL_TIMEOUT;
			goto _end;
		}
		if(_spi_check_status(hspi,true))
		{
			errorcode = HAL_ERROR;
			goto _end;
		}

		// rx fifo full,read data
		if((hspi->RxXferCount > 0) && ((hspi->Instance->SP_SPI_RISR & SPI_INT_RXFI) == SPI_INT_RXFI))
		{
			_spi_receive_data(hspi);
		}
	}
	//rx fifo have data not load
	while((hspi->RxXferCount > 0) && ((hspi->Instance->SP_SPI_SR & SRI_RF_NOT_EMPT) == SRI_RF_NOT_EMPT))
	{
		_spi_receive_data(hspi);
	}

_end:
	_spi_reset(hspi);
	hspi->State = HAL_SPI_STATE_READY;
	return errorcode;
}

HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t TxSize,
                                          uint16_t RxSize,uint32_t Timeout)
{
	uint32_t tickstart;

	HAL_StatusTypeDef errorcode = HAL_OK;

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto _end;
	}

	if ((pTxData == NULL) || (pRxData == NULL) || (TxSize == 0) | (RxSize == 0))
	{
		errorcode = HAL_ERROR;
		goto _end;
	}

	__HAL_LOCK(hspi);

	tickstart = HAL_GetTick();

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
	hspi->TxXferCount = TxSize;

	hspi->pRxBuffPtr  = (uint8_t *)pRxData;
	hspi->RxXferCount = RxSize;

	_spi_config_set(hspi, SPI_TMOD_TR);

	_spi_txrx_threshold_set(hspi,TxSize);

	_spi_transmit_data(hspi); //fill data

	_spi_slave_select_enable(hspi,1); // select slave and start to transmit

	__HAL_UNLOCK(hspi);

	//databook page38,polling TFE first before BUSY Set
	while(hspi->Instance->SP_SPI_SR & SRI_TF_EMPT == SRI_TF_EMPT);

	while((hspi->Instance->SP_SPI_SR & SPI_BUSY) == SPI_BUSY)
	{
		if ((((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
		{
			errorcode = HAL_TIMEOUT;
			goto _end;
		}

		if(_spi_check_status(hspi,true))
		{
			errorcode = HAL_ERROR;
			goto _end;
		}

		// tx fifo empty,fill data
		if((hspi->TxXferCount > 0) && ((hspi->Instance->SP_SPI_RISR & SPI_INT_TXEI) == SPI_INT_TXEI))
		{
			_spi_transmit_data(hspi);
		}

		// rx fifo full,read data
		if((hspi->RxXferCount > 0) && ((hspi->Instance->SP_SPI_RISR & SPI_INT_RXFI) == SPI_INT_RXFI))
		{
			_spi_receive_data(hspi);
		}
	}
	//rx fifo have data not load
	while((hspi->RxXferCount > 0) && ((hspi->Instance->SP_SPI_SR & SRI_RF_NOT_EMPT) == SRI_RF_NOT_EMPT))
	{
		_spi_receive_data(hspi);
	}

_end:
	_spi_reset(hspi);
	hspi->State = HAL_SPI_STATE_READY;
	return errorcode;

}

void HAL_SPI_IRQHandler(SPI_HandleTypeDef *arg)
{

	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)arg;
	uint8_t tmode,isrdone=false;

	if(!hspi){
		goto _error;
	}
	if (_spi_check_status(hspi,false)){
		goto _error;
	}

	tmode = (hspi->Instance->SP_SPI_CTRLR0 >> 8) & 0x3;
	if(tmode != SPI_TMOD_TO)
	{
		_spi_receive_data(hspi);
		if (!hspi->RxXferCount)
		{
			isrdone = true;
			_spi_mask_intr(hspi, 0xff);
		}
		else if (hspi->RxXferCount <= hspi->Instance->SP_SPI_RXFTLR)
		{
			hspi->Instance->SP_SPI_RXFTLR =  hspi->RxXferCount - 1;
		}
	}

	if (hspi->Instance->SP_SPI_ISR & SPI_INT_TXEI)
	{
		_spi_transmit_data(hspi);
		if (!hspi->TxXferCount)
		{
			_spi_mask_intr(hspi, SPI_INT_TXEI);
			if(tmode == SPI_TMOD_TO){
				isrdone = true;
			}
		}
	}

_fin_irq:
	if(isrdone)
	{
		_spi_reset(hspi);
		hspi->State = HAL_SPI_STATE_READY;
		if(tmode == SPI_TMOD_TR){
			hspi->TxRxCpltCallback(hspi);
		}
		else if(tmode == SPI_TMOD_TO){
			hspi->TxCpltCallback(hspi);
		}
		else if(tmode == SPI_TMOD_RO){
			hspi->RxCpltCallback(hspi);
		}
	}
	return;

_error:
	hspi->State = HAL_SPI_STATE_READY;
	hspi->ErrorCallback(hspi);

}

HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
	HAL_StatusTypeDef errorcode = HAL_OK;

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto _end;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		errorcode = HAL_ERROR;
		goto _end;
	}

	__HAL_LOCK(hspi);

	hspi->State       = HAL_SPI_STATE_BUSY_TX;
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pData;
	hspi->TxXferCount = Size;

	_spi_config_set(hspi, SPI_TMOD_TO);

	_spi_txrx_threshold_set(hspi,Size);

	_spi_transmit_data(hspi); //fill data and transmit start

	_spi_slave_select_enable(hspi,1); // select slave and start transmit

	_spi_unmask_intr(hspi, (SPI_INT_TXEI | SPI_INT_TXOI));

	__HAL_UNLOCK(hspi);

_end :
	return errorcode;
}

HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
	HAL_StatusTypeDef errorcode = HAL_OK;

	/* Process Locked */
	__HAL_LOCK(hspi);

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto _end;
	}

	if ((pData == NULL) || (Size == 0U) || (Size > 0xFFFF))
	{
		errorcode = HAL_ERROR;
		goto _end;
	}

	/* Set the transaction information */
	hspi->State       = HAL_SPI_STATE_BUSY_RX;
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pData;
	hspi->RxXferCount = Size;

	_spi_config_set(hspi, SPI_TMOD_RO);

	_spi_receive_data_set(hspi,Size);

	hspi->Instance->SP_SPI_DR = 0xFF; //write a dummy data to start

	_spi_slave_select_enable(hspi,1); // select slave and start to transmit

	_spi_unmask_intr(hspi, (SPI_INT_RXUI | SPI_INT_RXOI | SPI_INT_RXFI));

_end :
	__HAL_UNLOCK(hspi);
	return errorcode;
}

HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData,
                                             uint16_t TxSize,uint16_t RxSize)
{
	unsigned int imask;

	HAL_StatusTypeDef errorcode = HAL_OK;

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto _end;
	}

	if ((pTxData == NULL) || (pRxData == NULL) || (TxSize == 0) | (RxSize == 0))
	{
		errorcode = HAL_ERROR;
		goto _end;
	}

	__HAL_LOCK(hspi);

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
	hspi->TxXferCount = TxSize;

	hspi->pRxBuffPtr  = (uint8_t *)pRxData;
	hspi->RxXferCount = RxSize;

	imask = SPI_INT_TXEI | SPI_INT_TXOI | SPI_INT_RXUI | SPI_INT_RXOI | SPI_INT_RXFI;

	_spi_config_set(hspi, SPI_TMOD_TR);

	_spi_txrx_threshold_set(hspi,TxSize);

	_spi_transmit_data(hspi); //fill data

	_spi_slave_select_enable(hspi,1); // select slave and start to transmit

	_spi_unmask_intr(hspi, imask);

_end :
	__HAL_UNLOCK(hspi);
	return errorcode;
}

void HAL_SPI_DMA_IRQ_Callback(uint32_t err,void *param)
{
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)param;

	if(!hspi || err)
	{
		return;
	}

	if(err)
	{
		goto _error;
	}

	uint8_t tmode = (hspi->Instance->SP_SPI_CTRLR0 >> 8) & 0x3;

	if(tmode == SPI_TMOD_TR)
	{
		hspi->TxRxCpltCallback(hspi);
		while((hspi->Instance->SP_SPI_SR & SRI_TF_EMPT) != SRI_TF_EMPT);
	}
	else if(tmode == SPI_TMOD_TO)
	{
		hspi->TxCpltCallback(hspi);
		while((hspi->Instance->SP_SPI_SR & SRI_TF_EMPT) != SRI_TF_EMPT);
	}
	else if(tmode == SPI_TMOD_RO)
	{
		hspi->RxCpltCallback(hspi);
	}

	_spi_reset(hspi);
	hspi->State = HAL_SPI_STATE_READY;
	return;

_error:
	hspi->State = HAL_SPI_STATE_READY;
	hspi->ErrorCallback(hspi);
}

uint32_t _get_Spi_Index_By_Instance(SPI_HandleTypeDef *hspi)
{
	return (((uint32_t)(hspi->Instance) - SPI0_BASE)/0x1000 + PER_SPI_CB0);
}

HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
	HAL_StatusTypeDef errorcode = HAL_OK;

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto _end;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		errorcode = HAL_ERROR;
		goto _end;
	}

	__HAL_LOCK(hspi);

	hspi->State       = HAL_SPI_STATE_BUSY_TX;
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pData;
	hspi->TxXferCount = Size;

	_spi_config_set(hspi, SPI_TMOD_TO);

	_spi_unmask_intr(hspi, SPI_INT_TXOI);

	_spi_dma_config(hspi, SPI_TMOD_TO, Size, 1);

	_spi_slave_select_enable(hspi,1); // select slave and start transmit

	__HAL_UNLOCK(hspi);

	if (HAL_OK != HAL_DMA_Start((uint32_t)hspi->pTxBuffPtr, _get_Spi_Index_By_Instance(hspi),
								   hspi->TxXferCount,HAL_SPI_DMA_IRQ_Callback,(void *)hspi))
	{
	  /* Update SPI error code */
	  errorcode = HAL_ERROR;
	  hspi->State = HAL_SPI_STATE_READY;
	  goto _end;
	}

_end :
	return errorcode;
}

HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
	unsigned int temp_reg;
	HAL_StatusTypeDef errorcode = HAL_OK;

	/* Process Locked */
	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto _end;
	}

	if ((pData == NULL) || (Size == 0U) || (Size > 0xFFFF))
	{
		errorcode = HAL_ERROR;
		goto _end;
	}

	/* Set the transaction information */
	hspi->State       = HAL_SPI_STATE_BUSY_RX;
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pData;
	hspi->RxXferCount = Size;

	__HAL_LOCK(hspi);

	_spi_config_set(hspi, SPI_TMOD_RO);

	_spi_receive_data_set(hspi,Size);

	_spi_unmask_intr(hspi, (SPI_INT_RXUI | SPI_INT_RXOI));

	hspi->Instance->SP_SPI_DR = 0xFF; //write a dummy data to start

	_spi_dma_config(hspi, SPI_TMOD_RO, 0, 1);
	
	_spi_slave_select_enable(hspi,1); // select slave and start transmit

	__HAL_UNLOCK(hspi);

	if (HAL_OK != HAL_DMA_Start((uint32_t)_get_Spi_Index_By_Instance(hspi), (uint32_t)hspi->pRxBuffPtr,
								   hspi->RxXferCount,HAL_SPI_DMA_IRQ_Callback,(void *)hspi))
	{
	  /* Update SPI error code */
	  errorcode = HAL_ERROR;
	  goto _end;
	}

_end :
	return errorcode;
}

HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData,
                                             uint16_t TxSize,uint16_t RxSize)
{
	unsigned int imask;

	HAL_StatusTypeDef errorcode = HAL_OK;

	if (hspi->State != HAL_SPI_STATE_READY)
	{
		errorcode = HAL_BUSY;
		goto _end;
	}

	if ((pTxData == NULL) || (pRxData == NULL) || (TxSize == 0) | (RxSize == 0))
	{
		errorcode = HAL_ERROR;
		goto _end;
	}

	__HAL_LOCK(hspi);

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
	hspi->TxXferCount = TxSize;

	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pRxData;
	hspi->RxXferCount = RxSize;

	imask =  SPI_INT_TXOI | SPI_INT_RXUI | SPI_INT_RXOI;

	_spi_config_set(hspi, SPI_TMOD_TR);

	_spi_dma_config(hspi, SPI_TMOD_TR, TxSize, 1);

	_spi_unmask_intr(hspi, imask);

	_spi_slave_select_enable(hspi,1); // select slave and start to transmit

	__HAL_UNLOCK(hspi);
	/* transmit dma */
	if (HAL_OK != HAL_DMA_Start((uint32_t)hspi->pTxBuffPtr, _get_Spi_Index_By_Instance(hspi),
								   hspi->TxXferCount,HAL_SPI_DMA_IRQ_Callback,NULL))
	{
	  /* Update SPI error code */
	  errorcode = HAL_ERROR;
	  goto _end;
	}
	/* receive dma */
	if (HAL_OK != HAL_DMA_Start((uint32_t)_get_Spi_Index_By_Instance(hspi), (uint32_t)hspi->pRxBuffPtr,
								   hspi->RxXferCount,HAL_SPI_DMA_IRQ_Callback,(void *)hspi))
	{
	  /* Update SPI error code */
	  errorcode = HAL_ERROR;
	  goto _end;
	}

_end :
	return errorcode;
}

