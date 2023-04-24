#ifndef __SP7350_HAL_I2C_H
#define __SP7350_HAL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sp7350_cm4.h"
#include "sp7350_hal_def.h"
#include "irq_ctrl.h"
#include "sp7350xx.h"

#if 0
#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) (((((__HANDLE__)->Instance->interrupt) & \
                                                    (__FLAG__)) == (__FLAG__)) ? SET : RESET)
#define __HAL_DMA_GET_FLAG(__HANDLE__, __FLAG__) (((((__HANDLE__)->gdma->int_flag) & \
                                                    (__FLAG__)) == (__FLAG__)) ? SET : RESET)
#endif
#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) 0
#define __HAL_DMA_GET_FLAG(__HANDLE__, __FLAG__) 0


#define HAL_DCACHE_LINE_SIZE            32
#define CYG_MACRO_START			do{
#define CYG_MACRO_END			}while(0)

#define HAL_DCACHE_FLUSH( _base_ , _size_ )     \
CYG_MACRO_START                                 \
    HAL_DCACHE_STORE( _base_ , _size_ );        \
    HAL_DCACHE_INVALIDATE( _base_ , _size_ );   \
CYG_MACRO_END

// Invalidate cache lines in the given range without writing to memory.
#define HAL_DCACHE_INVALIDATE( _base_ , _size_ )

// Write dirty cache lines to memory for the given address range.
#define HAL_DCACHE_STORE( _base_ , _size_ )

/* Confirm the value by reading reg 0xf4(RX[15:8] TX[23:16]) */
#define I2C_FIFO_DEPTH	8
/* DMA transfer rate far more than I2C transfer rate, Therefore RX thresold > TX thresold */
/* For TX, FIFO data length less than or equal to watermark level (= TDRL), DMA request trigger */
#define I2C_TDRL	0//2
/* For RX, FIFO data length more than or equal to watermark level (= RDRL + 1), DMA request trigger */
#define I2C_RDRL	0//5

#define I2C_READ_DATA	0x100

/* I2C Transmit Modes */
#define I2C_MODE_BURST	0x0
#define I2C_MODE_INTR	0x1
#define I2C_MODE_DMA	0x2

/* I2C Frequency Modes */
#define I2C_MAX_STANDARD_MODE_FREQ	100000
#define I2C_MAX_FAST_MODE_FREQ		400000
#define I2C_MAX_FAST_MODE_PLUS_FREQ	3400000

#define SP_IC_CON_MASTER		0x1
#define SP_IC_CON_SPEED_STD		0x2
#define SP_IC_CON_SPEED_FAST		0x4
#define SP_IC_CON_SPEED_HIGH		0x6
#define SP_IC_CON_SPEED_MASK		0x6
#define SP_IC_CON_10BITADDR_SLAVE	0x8
#define SP_IC_CON_10BITADDR_MASTER	0x10
#define SP_IC_CON_RESTART_EN		0x20
#define SP_IC_CON_SLAVE_DISABLE		0x40
#define SP_IC_CON_STOP_DET_IFADDRESSED	0x80
#define SP_IC_CON_TX_EMPTY_CTRL		0x100
#define SP_IC_CON_RX_FIFO_FULL_HLD_CTRL	0x200

#define SP_IC_INTR_RX_UNDER	0x01
#define SP_IC_INTR_RX_OVER	0x02
#define SP_IC_INTR_RX_FULL	0x04
#define SP_IC_INTR_TX_OVER	0x08
#define SP_IC_INTR_TX_EMPTY	0x10
#define SP_IC_INTR_RD_REQ	0x20
#define SP_IC_INTR_TX_ABRT	0x40
#define SP_IC_INTR_RX_DONE	0x80
#define SP_IC_INTR_ACTIVITY	0x100
#define SP_IC_INTR_STOP_DET	0x200
#define SP_IC_INTR_START_DET	0x400
#define SP_IC_INTR_GEN_CALL	0x800
#define SP_IC_INTR_RESTART_DET	0x1000

#define SP_IC_RDMAE 		0x01
#define SP_IC_TDMAE 		0x02

#define SP_IC_STATUS_TFE	0x004
#define SP_IC_STATUS_RFNE	0x008
#define SP_IC_STATUS_MST_ACT	0x020

#define SP_IC_INTR_DEFAULT_MASK		(SP_IC_INTR_RX_FULL | \
					 SP_IC_INTR_TX_ABRT | \
					 SP_IC_INTR_STOP_DET)
#define SP_IC_INTR_MASTER_MASK		(SP_IC_INTR_DEFAULT_MASK | \
					 SP_IC_INTR_TX_EMPTY)
#define SP_IC_INTR_SLAVE_MASK		(SP_IC_INTR_DEFAULT_MASK | \
					 SP_IC_INTR_RX_DONE | \
					 SP_IC_INTR_RX_UNDER | \
					 SP_IC_INTR_RD_REQ)

#define ABRT_7B_ADDR_NOACK	0
#define ABRT_10ADDR1_NOACK	1
#define ABRT_10ADDR2_NOACK	2
#define ABRT_TXDATA_NOACK	3
#define ABRT_GCALL_NOACK	4
#define ABRT_GCALL_READ		5

#define SP_IC_TX_ABRT_7B_ADDR_NOACK	(1UL << ABRT_7B_ADDR_NOACK)
#define SP_IC_TX_ABRT_10ADDR1_NOACK	(1UL << ABRT_10ADDR1_NOACK)
#define SP_IC_TX_ABRT_10ADDR2_NOACK	(1UL << ABRT_10ADDR2_NOACK)
#define SP_IC_TX_ABRT_TXDATA_NOACK	(1UL << ABRT_TXDATA_NOACK)
#define SP_IC_TX_ABRT_GCALL_NOACK	(1UL << ABRT_GCALL_NOACK)

#define SP_IC_TX_ABRT_NOACK		(SP_IC_TX_ABRT_7B_ADDR_NOACK | \
					 SP_IC_TX_ABRT_10ADDR1_NOACK | \
					 SP_IC_TX_ABRT_10ADDR2_NOACK | \
					 SP_IC_TX_ABRT_TXDATA_NOACK | \
					 SP_IC_TX_ABRT_GCALL_NOACK)
/**
  * @}
  */

/** @defgroup HAL_state_structure_definition HAL state structure definition
  * @brief  HAL State structure definition
  * @note  HAL I2C State value coding follow below described bitmap :\n
  *          b7-b6  Error information\n
  *             00 : No Error\n
  *             01 : Abort (Abort user request on going)\n
  *             10 : Timeout\n
  *             11 : Error\n
  *          b5     Peripheral initialization status\n
  *             0  : Reset (peripheral not initialized)\n
  *             1  : Init done (peripheral initialized and ready to use. HAL I2C Init function called)\n
  *          b4     (not used)\n
  *             x  : Should be set to 0\n
  *          b3\n
  *             0  : Ready or Busy (No Listen mode ongoing)\n
  *             1  : Listen (peripheral in Address Listen Mode)\n
  *          b2     Intrinsic process state\n
  *             0  : Ready\n
  *             1  : Busy (peripheral busy with some configuration or internal operations)\n
  *          b1     Rx state\n
  *             0  : Ready (no Rx operation ongoing)\n
  *             1  : Busy (Rx operation ongoing)\n
  *          b0     Tx state\n
  *             0  : Ready (no Tx operation ongoing)\n
  *             1  : Busy (Tx operation ongoing)
  * @{
  */
typedef enum
{
	HAL_I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
	HAL_I2C_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
	HAL_I2C_STATE_BUSY              = 0x25U,   /*!< An internal process is ongoing            */
	HAL_I2C_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing      */
	HAL_I2C_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing         */
	HAL_I2C_STATE_BUSY_DMA_TX	= 0x23U,   /*!< Data DMA Transmission process is ongoing  */
	HAL_I2C_STATE_BUSY_DMA_RX       = 0x24U,   /*!< Data DMA Reception process is ongoing     */
	HAL_I2C_STATE_ABORT             = 0x60U,   /*!< Abort user request ongoing                */
	HAL_I2C_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state                             */
	HAL_I2C_STATE_ERROR             = 0xE0U    /*!< Error                                     */
} HAL_I2C_StateTypeDef;

/**
  * @}
  */

typedef enum
{
	HAL_I2C_ERR_NONE			= 0x000U,	/* successful */
	HAL_I2C_ERR_I2C_BUSY			= 0x001U,	/* I2C is busy */
	HAL_I2C_ERR_INVALID_DEVID		= 0x002U,	/* device id is invalid */
	HAL_I2C_ERR_INVALID_CNT			= 0x004U,	/* read or write count is invalid */
	HAL_I2C_ERR_TIMEOUT			= 0x008U,	/* wait timeout */
	HAL_I2C_ERR_RECEIVE_NACK		= 0x010U,	/* receive NACK */
	HAL_I2C_ERR_FIFO_EMPTY			= 0x020U,	/* FIFO empty */
	HAL_I2C_ERR_SCL_HOLD_TOO_LONG		= 0x040U,	/* SCL hlod too long */
	HAL_I2C_ERR_RDATA_OVERFLOW		= 0x080U,	/* rdata overflow */
	HAL_I2C_ERR_INVALID_STATE		= 0x100U,	/* read write state is invalid */
	HAL_I2C_ERR_REQUESET_IRQ		= 0x200U,	/* request irq failed */
	HAL_I2C_ERR_ADDRESS_NACK		= 0x400U	/* slave address NACK */
} I2C_Status_e;

typedef struct
{
	uint32_t Timing;              /*!< Specifies the I2C_TIMINGR_register value.
                                  This parameter calculated by referring to I2C initialization
                                         section in Reference manual */
} I2C_InitTypeDef;

/** @defgroup I2C_handle_Structure_definition I2C handle Structure definition
  * @brief  I2C handle Structure definition
  * @{
  */
typedef struct __I2C_HandleTypeDef
{
	__IO I2C_TypeDef           *Instance;      /*!< I2C registers base address                */
	I2C_InitTypeDef            Init;           /*!< I2C communication parameters              */
	HAL_LockTypeDef            Lock;           /*!< I2C locking object                        */
	__IO HAL_I2C_StateTypeDef  State;          /*!< I2C communication state                   */
	__IO uint32_t              ErrorCode;      /*!< I2C Error code                            */

	volatile GDMA_TypeDef      *gdma;/*!< Reserved it just to reduce the unnecessary marco definition for twi.c */

	uint8_t                    Index;
	uint32_t                   DataIndex;      /*!< Data index in buffer                      */
	uint32_t                   DataTotalLen;   /*!< I2C transfer size                         */
	uint32_t                   AbortSource;    /*!< I2C transfer source of abort              */
	uint32_t                   ReadLen;        /*!< Length of data which read form RX FIFO                           */
	uint32_t                   RxOutStanding;  /*!< Length of data which is left space in FIFO but not yet available */
	uint8_t                    XferAction;
	uint8_t                    XferWaitTxEnd;
	uint8_t                    *Buffer;
	uint32_t                   DMAIndex;
} I2C_HandleTypeDef;

void I2C_HAL_TEST_IRQHandler(void);

void HAL_I2C_PinMux(I2C_HandleTypeDef *hi2c, int sda_pinmux, int scl_pinmux);

HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);

HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c);

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t Size,
                                          uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t Size,
                                         uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
												   uint32_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
												  uint32_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
												 uint32_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
												uint32_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
                                              uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
                                             uint16_t Size);
HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef * hi2c);
void HAL_I2C_ClearError(I2C_HandleTypeDef * hi2c);
uint32_t HAL_I2C_GetError(I2C_HandleTypeDef * hi2c);

void HAL_I2C_IRQHandler(I2C_HandleTypeDef *hi2c);

void sp_i2c_en(I2C_HandleTypeDef *hi2c);
void HAL_I2C_TEST(void);
void HAL_I2C_SLAVE_TEST(void);

#ifdef __cplusplus
}
#endif

#endif
