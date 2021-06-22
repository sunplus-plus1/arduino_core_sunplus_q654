#ifndef SP7021_HAL_SPI_H
#define SP7021_HAL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif


#include "sp7021_hal_def.h"
#include "sp7021_arm926.h"
#include "sp70xx.h" 	




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


typedef struct
{
  uint32_t freq;
  uint32_t CLKPolarity;
  uint32_t CLKPhase;
  uint32_t FirstBit;
} SPI_InitTypeDef;

typedef struct __SPI_HandleTypeDef
{
  SPI_TypeDef                *Instance;      /*!< SPI registers base address               */

  SPI_InitTypeDef            Init;           /*!< SPI communication parameters             */

  uint8_t                    *pTxBuffPtr;    /*!< Pointer to SPI Tx transfer Buffer        */

  uint16_t                   TxXferSize;     /*!< SPI Tx Transfer size                     */

  __IO uint16_t              TxXferCount;    /*!< SPI Tx Transfer Counter                  */

  uint8_t                    *pRxBuffPtr;    /*!< Pointer to SPI Rx transfer Buffer        */

  uint16_t                   RxXferSize;     /*!< SPI Rx Transfer size                     */

  __IO uint16_t              RxXferCount;    /*!< SPI Rx Transfer Counter                  */


  HAL_LockTypeDef            Lock;           /*!< Locking object                           */

  __IO HAL_SPI_StateTypeDef  State;          /*!< SPI communication state                  */

  __IO uint32_t              ErrorCode;      /*!< SPI Error code                           */

  void (* TxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI Tx Completed callback          */
  void (* RxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI Rx Completed callback          */
  void (* TxRxCpltCallback)(struct __SPI_HandleTypeDef *hspi);           /*!< SPI TxRx Completed callback        */

} SPI_HandleTypeDef;

#define min(X,Y) ((X) < (Y) ? (X) : (Y))

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


#define SPI_POLARITY_LOW                (0x00000000U)
#define SPI_POLARITY_HIGH               (0x00000001U)

#define SPI_PHASE_1EDGE                 (0x00000000U)
#define SPI_PHASE_2EDGE                 (0x00000001U)

#define SPI_FIRSTBIT_MSB                (0x00000000U)
#define SPI_FIRSTBIT_LSB                (0x00000001U)

#define SPI_FREQ_MIN                    (40000)     /* spi clk  min is 40k */
#define SPI_FREQ_MAX                    (50000000)  /* spi clk  max is 50M */
/* SPI MST DMA_SIZE */
#define MST_DMA_RSIZE(x)       (x<<16)
#define MST_DMA_WSIZE(x)       (x<<0)

/* SPI MST DMA config */
#define MST_DMA_EN             (1<<1)
#define MST_DMA_START          (1<<0)

/* full duplex, fifo length is config 16 byte */
#define SPI_FIFO_MAX_LENGTH     (16)

/* SPI MST STATUS */

#define GET_LEN(x)     ((x>>24) & 0xFF)
#define GET_TX_LEN(x)  ((x>>16) & 0xFF)
#define GET_RX_CNT(x)  ((x>>12) & 0x0F)
#define GET_TX_CNT(x)  ((x>>8)  & 0x0F)

#define TOTAL_LENGTH(x)        (x<<24)
#define TX_LENGTH(x)           (x<<16)
#define RX_CNT                 (0x0F<<12)
#define TX_CNT                 (0x0F<<12)
#define SPI_BUSY               (1<<7)
#define FINISH_FLAG            (1<<6)
#define RX_FULL_FLAG           (1<<5)
#define RX_EMP_FLAG            (1<<4)
#define TX_FULL_FLAG           (1<<3)
#define TX_EMP_FLAG            (1<<2)
#define SPI_SW_RST             (1<<1)
#define SPI_START_FD           (1<<0)

#define CLEAN_FLUG_MASK (~0xF800)


/* SPI MST CONFIG */
#define CLK_DIVIDER(x)         (x<<16)
#define FINISH_FLAG_MASK       (1<<15)
#define RX_FULL_FLAG_MASK      (1<<14)
#define RX_EMP_FLAG_MASK       (1<<13)
#define TX_FULL_FLAG_MASK      (1<<12)
#define TX_EMP_FLAG_MASK       (1<<11)
#define WRITE_BYTE(x)          (x<<9)
#define READ_BYTE(x)           (x<<7)
#define FD_SEL                 (1<<6)
#define CS_SEL                 (1<<5)
#define LSB_SEL                (1<<4)
#define DELAY_ENABLE           (1<<3)
#define CPHA_W                 (1<<2)
#define CPHA_R                 (1<<1)
#define CPOL                   (1<<0)

#define CLEAR_MASTER_INT (1<<6)

#define CLEAR_IRQ_MASK         (~0xF800)
#define CLEAN_RW_BYTE          (~0x780)
#define CLEAN_SPI_MODE         (~0x7)

#define SPI_FD_INTR            (1<<7)
#define SPI_MASTER_NUM         (4)
#define SPI_MSG_DATA_SIZE      (255)

#define SPI_CLK_RATE           202500000
#define SPI_INIT_FREQ          5000000
#define SPI_MAX_FREQ           25000000


#define IS_SPI_CPOL(__CPOL__)      (((__CPOL__) == SPI_POLARITY_LOW) || \
                                    ((__CPOL__) == SPI_POLARITY_HIGH))

#define IS_SPI_CPHA(__CPHA__)      (((__CPHA__) == SPI_PHASE_1EDGE) || \
                                    ((__CPHA__) == SPI_PHASE_2EDGE))

#define IS_SPI_FIRST_BIT(__BIT__)  (((__BIT__) == SPI_FIRSTBIT_MSB) || \
                                    ((__BIT__) == SPI_FIRSTBIT_LSB))

#define IS_VALID_FREQ(__freq__)  (((__freq__) >= SPI_FREQ_MIN) && \
                                    ((__freq__) <= SPI_FREQ_MAX))

HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t RxSize,
                                           uint16_t TxSize,uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData,  uint16_t RxSize,uint16_t TxSize);

HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi);
HAL_SPI_StateTypeDef HAL_SPI_GetState(SPI_HandleTypeDef *hs);

#ifdef __cplusplus
}
#endif

#endif