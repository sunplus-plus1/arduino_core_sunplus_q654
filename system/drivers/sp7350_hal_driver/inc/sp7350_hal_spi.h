#ifndef SP7350_HAL_SPI_H
#define SP7350_HAL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif


#include "sp7350_hal_def.h"
#include "sp7350_cm4.h"
#include "sp7350xx.h" 	


typedef enum
{
  HAL_SPI_STATE_RESET      = 0x00U,
  HAL_SPI_STATE_READY      = 0x01U,
  HAL_SPI_STATE_BUSY       = 0x02U,
  HAL_SPI_STATE_BUSY_TX    = 0x03U,
  HAL_SPI_STATE_BUSY_RX    = 0x04U,
  HAL_SPI_STATE_BUSY_TX_RX = 0x05U,
  HAL_SPI_STATE_ERROR      = 0x06U,
  HAL_SPI_STATE_ABORT      = 0x07U
} HAL_SPI_StateTypeDef;

typedef enum {
  SPI_MODE0 = 0x00,
  SPI_MODE1 = 0x01,
  SPI_MODE2 = 0x02,
  SPI_MODE3 = 0x03
} spi_mode;

typedef struct
{
  uint32_t spiclk;
  uint32_t spi_mode;
  uint32_t FirstBit;
} SPI_InitTypeDef;

typedef struct __SPI_HandleTypeDef
{
  /* user set */
  volatile SPI_TypeDef       *Instance;      /*!< SPI registers base address               */

  SPI_InitTypeDef            Init;           /*!< SPI communication parameters             */

  /* driver used */
  uint8_t                    *pTxBuffPtr;    /*!< Pointer to SPI Tx transfer Buffer        */

  __IO uint16_t              TxXferCount;    /*!< SPI Tx Transfer Counter                  */

  uint8_t                    *pRxBuffPtr;    /*!< Pointer to SPI Rx transfer Buffer        */

  __IO uint16_t              RxXferCount;    /*!< SPI Rx Transfer Counter                  */

  HAL_LockTypeDef            Lock;           /*!< Locking object                           */

  __IO HAL_SPI_StateTypeDef  State;          /*!< SPI communication state                  */

  __IO uint32_t              ErrorCode;      /*!< SPI Error code                           */

  void (* TxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI Tx Completed callback          */
  void (* RxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI Rx Completed callback          */
  void (* TxRxCpltCallback)(struct __SPI_HandleTypeDef *hspi);           /*!< SPI TxRx Completed callback        */
  void (* ErrorCallback)(struct __SPI_HandleTypeDef *hspi);           	 /*!< SPI error callback        */

} SPI_HandleTypeDef;


#define HAL_SPI_ERROR_NONE              (0x00000000U)
#define HAL_SPI_ERROR_MODF              (0x00000001U)
#define HAL_SPI_ERROR_CRC               (0x00000002U)
#define HAL_SPI_ERROR_OVR               (0x00000004U)
#define HAL_SPI_ERROR_FRE               (0x00000008U)
#define HAL_SPI_ERROR_DMA               (0x00000010U)
#define HAL_SPI_ERROR_FLAG              (0x00000020U)
#define HAL_SPI_ERROR_ABORT             (0x00000040U)
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
#define HAL_SPI_ERROR_INVALID_CALLBACK  (0x00000080U)
#endif

#define SPI_FIRSTBIT_MSB                (0x00000000U)
#define SPI_FIRSTBIT_LSB                (0x00000001U)

#define SPI_FREQ_MIN                    (40000)     /* spi clk  min is 40k */
#define SPI_FREQ_MAX                    (50000000)  /* spi clk  max is 50M */

/* full duplex, fifo length is config 32 byte */
#define SPI_FIFO_MAX_LENGTH     (32)

#define SPI_TMOD_TR			    0x0		/* xmit & recv */
#define SPI_TMOD_TO			    0x1		/* xmit only */
#define SPI_TMOD_RO			    0x2		/* recv only */

#define SPI_DMA_TX_ENABLE		(0x1<<1)
#define SPI_DMA_RX_ENABLE		(0x1<<0)

#define IS_SPI_MODE(__MODE__)      (((__MODE__) >= SPI_MODE0) &&\
                                    ((__MODE__) <= SPI_MODE3))

#define IS_VALID_FREQ(__freq__)  (((__freq__) >= SPI_FREQ_MIN) && \
                                    ((__freq__) <= SPI_FREQ_MAX))

HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t TxSize,
                                           uint16_t RxSize,uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData,  uint16_t TxSize,uint16_t RxSize);

HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData,
                                             uint16_t TxSize,uint16_t RxSize);

HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi);
HAL_SPI_StateTypeDef HAL_SPI_GetState(SPI_HandleTypeDef *hs);

void HAL_SPI_IRQHandler(SPI_HandleTypeDef *arg);

#ifdef __cplusplus
}
#endif

#endif