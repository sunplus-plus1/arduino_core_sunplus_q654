#ifndef __SP7021_LL_STC_H__
#define __SP7021_LL_STC_H__

#ifdef __cplusplus
extern "C" {
#endif

__STATIC_INLINE void LL_SPI_StartMasterTransfer(SPI_TypeDef *SPIx)
{
	MODIFY_REG(SPIx->spi_status, SPI_START_FD, SPI_START_FD);
}

__STATIC_INLINE void LL_SPI_SetTransferSize(SPI_TypeDef *SPIx, uint32_t Count)
{
  MODIFY_REG(SPIx->spi_status, SPI_TOTAL_SIZE, TOTAL_LENGTH(Count));
  MODIFY_REG(SPIx->spi_status, SPI_TX_SIZE, TX_LENGTH(Count));
}

__STATIC_INLINE void LL_SPI_TransmitData8(SPI_TypeDef *SPIx, uint8_t TxData)
{
  *((__IO uint32_t *)&SPIx->fifo_data) = TxData;
}

__STATIC_INLINE uint8_t LL_SPI_ReceiveData8(SPI_TypeDef *SPIx)
{
  return (*((__IO uint8_t *)&SPIx->fifo_data));
}

__STATIC_INLINE uint32_t LL_SPI_IsActiveFlag_RXNE(SPI_TypeDef *SPIx)
{
  return ((READ_BIT(SPIx->spi_status, RX_CNT)) ? 1UL : 0UL);
}

__STATIC_INLINE uint32_t LL_SPI_IsActiveFlag_TXE(SPI_TypeDef *SPIx)
{
  return ((READ_BIT(SPIx->spi_status, TX_EMP_FLAG) == (TX_EMP_FLAG)) ? 1UL : 0UL);
}

#ifdef __cplusplus
}
#endif

#endif