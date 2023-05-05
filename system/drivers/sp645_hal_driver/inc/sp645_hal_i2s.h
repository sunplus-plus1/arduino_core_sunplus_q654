/**
  ******************************************************************************
  * @file    stm32g0xx_hal_i2s.h
  * @author  MCD Application Team
  * @brief   Header file of I2S HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SP645_HAL_I2S_H
#define SP645_HAL_I2S_H

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_I2S_SUPPORT
#if defined(SPI_I2S_SUPPORT)
/* Includes ------------------------------------------------------------------*/
#include "sp645_cm4.h"
#include "sp645_hal_def.h"
#include "sp64xx.h"

#define DRAM_PCM_BUF_CH_BYTES	(20 * 1024)
#define DMA_TRANS_LEN		(2 * 1024)

#define PLLA_147M	147456000U
#define PLLA_135M	135475200U

#define ROUNDUP(v, size)	((v + size - 1) & (~(size - 1)))

#define BIT(n)		(1U << n)

#define DEBUG_OFF	42

#define I2S0_P		BIT(0)
#define I2S1_P		BIT(1)
#define I2S2_P		BIT(2)

#define I2S0_C		BIT(3)
#define I2S1_C		BIT(4)
#define I2S2_C		BIT(5)

/* I2S TX, Active high to increment/decrement DMA counter */
#define I2S_TX_INC0		(0x1 << 0)
#define I2S_TX_INC1		(0x1 << 6)
#define I2S_TX_INC2		(0x1 << 19)
/* I2S RX */
#define I2S_RX_INC0    		(0x1 << 11)
#define I2S_RX_INC1    		(0x1 << 16)
#define I2S_RX_INC2    		(0x1 << 10)

/* I2S FIFO Enable */
#define I2S_TX_FIFO_EN0		(0x1 << 0)
#define I2S_TX_FIFO_EN1		(0x1 << 6)
#define I2S_TX_FIFO_EN2		(0x1 << 19)

#define I2S_RX_FIFO_EN0    	(0x1 << 11)
#define I2S_RX_FIFO_EN1    	(0x1 << 16)
#define I2S_RX_FIFO_EN2    	(0x1 << 10)

/* I2S Audio Enable */
#define I2S_TX_AUD_EN0		(0x01 | (0x1df << 16))
#define I2S_TX_AUD_EN1		(0x01 << 13)
#define I2S_TX_AUD_EN2		(0x01 << 15)
#define I2S_RX_AUD_EN0    	(0x01 << 3)
#define I2S_RX_AUD_EN1    	(0x01 << 11)
#define I2S_RX_AUD_EN2    	(0x01 << 5)

////////////////////////////////////////
// G61.30 debug function: i2s tx test
#define I2S2_DEBUG_BIT		(14U)
#define I2S1_DEBUG_BIT		(10U)
#define I2S0_DEBUG_BIT		(8U)

////////////////////////////////data_config
#define I2S_MODE_MASK		GENMASK(9, 8)
#define I2S_MODE_BIT		(8U)
#define I2S_LRCYCLE_MASK	GENMASK(6, 5)
#define I2S_LRCYCLE_BIT		(5U)
#define I2S_DPARITY_BIT		(4U)
#define I2S_DWIDTH_MASK		GENMASK(3, 2)
#define I2S_DWIDTH_BIT		(2U)
#define I2S_DELAY_BIT		(0U)

#define LRCK_CYCLE_24		(1U)
#define LRCK_CYCLE_32		(2U)
#define LRCK_CYCLE_16		(3U)

//PCM data to ext DAC delay one BCK cycle after LRCK transition bit[0]
#define i2s_delay_mask		GENMASK(0, 0)
#define i2s_delay_nocyc		(0x0 << 0)
#define i2s_delay_onecyc	(0x1 << 1)
/////////////////////////////////////
#define XCK_GET_EN_BIT		(15U)
#define XCK_EN_MSK		GENMASK(14, 14)
#define XCK_EN_BIT		(14U)
#define XCK_OE_MSK		GENMASK(13, 13)
#define XCK_OE_BIT		(13U)
#define XCK_SRC_MSK		GENMASK(12, 11)
#define XCK_SRC_BIT		(11U)
#define XCK_DIV_MSK		GENMASK(10, 0)
#define XCK_DIV_BIT		(0U)

#define BCK_GET_EN_BIT		(15U)
#define BCK_EN_MSK		GENMASK(14, 14)
#define BCK_EN_BIT		(14U)
#define BCK_SRC_MSK		GENMASK(13, 11)
#define BCK_SRC_BIT		(11U)
#define BCK_DIV_MSK		GENMASK(10, 0)
#define BCK_DIV_BIT		(0U)

#define XBCK_DISABLE		(0U)
#define XBCK_ENABLE		(1U)

#define XBCK_SRC_CDRPLL		(0U)
#define XBCK_SRC_PLLA		(1U)
#define XBCK_SRC_DPLL		(2U)
#define XBCK_SRC_PIXTAL		(3U)
#define BCK_SRC_XCK		(4U)

/** @defgroup I2S_Error I2S Error
  * @{
  */
#define HAL_I2S_ERROR_NONE               (0x00000000U)  /*!< No error                    */
#define HAL_I2S_ERROR_TIMEOUT            (0x00000001U)  /*!< Timeout error               */

/** @defgroup I2S_Mode I2S Mode
  * @{
  */
#define I2S_MODE_SLAVE_TX                (0x0U)
#define I2S_MODE_SLAVE_RX                (0x1U)
#define I2S_MODE_MASTER_TX               (0x2U)
#define I2S_MODE_MASTER_RX               (0x3U)

/** @defgroup I2S_Data_Format I2S Data Format
  * @{
  */
#define I2S_DATAFORMAT_16B		(0x0U)
#define I2S_DATAFORMAT_18B		(0x1U)
#define I2S_DATAFORMAT_20B		(0x2U)
#define I2S_DATAFORMAT_24B		(0x3U)

/** @defgroup I2S_Audio_Frequency I2S Audio Frequency
  * @{
  */
#define I2S_AUDIOFREQ_192K               (192000U)
#define I2S_AUDIOFREQ_96K                (96000U)
#define I2S_AUDIOFREQ_48K                (48000U)
#define I2S_AUDIOFREQ_44K                (44100U)
#define I2S_AUDIOFREQ_32K                (32000U)
#define I2S_AUDIOFREQ_22K                (22050U)
#define I2S_AUDIOFREQ_16K                (16000U)
#define I2S_AUDIOFREQ_11K                (11025U)
#define I2S_AUDIOFREQ_8K                 (8000U)
#define I2S_AUDIOFREQ_DEFAULT            (2U)

/* I2S TX */
#define I2S_P_INC0		(0x1 << 0)
#define I2S_P_INC1		(0x1 << 6)
#define I2S_P_INC2		(0x1 << 19)
/* I2S RX */
#define I2S_C_INC0    		(0x1 << 11)
#define I2S_C_INC1    		(0x1 << 16)
#define I2S_C_INC2    		(0x1 << 10)
#define TDMPDM_C_INC0 		((0x1 << 14) | (0x3 << 17) | (0x1 << 21) | (0xf << 22))
#define TDM_P_INC0    		((0x1 << 20) | (0x3 << 26) | 0x1f)

#define aud_enable_i2stdm_p	(0x01 | (0x1df << 16))
#define aud_enable_i2s1_p	(0x01 << 13)
#define aud_enable_i2s2_p	(0x01 << 15)
#define aud_enable_i2s0_c    	(0x01 << 3)
#define aud_enable_i2s1_c    	(0x01 << 11)
#define aud_enable_i2s2_c    	(0x01 << 5)
#define aud_enable_tdmpdm_c 	(0x01 << 12)

typedef enum _SampleRateSetting_e
{
	FS_22 = 22050,
	FS_32 = 32000,
	FS_44 = 44100,
	FS_48 = 48000,
	FS_64 = 64000,
	FS_88 = 88200,
	FS_96 = 96000,
	FS_128 = 128000,
	FS_176 = 176400,
	FS_192 = 192000,
} SampleRateSetting_e;
#define SAMPLE_RATE FS_48

#define	ONEHOT_B10 0x00000400
#define	ONEHOT_B11 0x00000800
#define	ONEHOT_B12 0x00001000

/** @addtogroup STM32G0xx_HAL_Driver
  * @{
  */

/** @addtogroup I2S
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup I2S_Exported_Types I2S Exported Types
  * @{
  */
typedef struct
{
	uint8_t flag_gen_xck_bck;
} I2S_Priv;

/**
  * @brief I2S Init structure definition
  */
typedef struct
{
  uint32_t Mode;                /*!< Specifies the I2S operating mode.
                                     This parameter can be a value of @ref I2S_Mode */

  uint32_t Standard;            /*!< Specifies the standard used for the I2S communication.
                                     This parameter can be a value of @ref I2S_Standard */

  uint32_t DataFormat;          /*!< Specifies the data format for the I2S communication.
                                     This parameter can be a value of @ref I2S_Data_Format */

  uint32_t MCLKOutput;          /*!< Specifies whether the I2S MCLK output is enabled or not.
                                     This parameter can be a value of @ref I2S_MCLK_Output */

  uint32_t AudioFreq;           /*!< Specifies the frequency selected for the I2S communication.
                                     This parameter can be a value of @ref I2S_Audio_Frequency */
} I2S_InitTypeDef;

/**
  * @brief  HAL State structures definition
  */
typedef enum
{
  HAL_I2S_STATE_RESET      = 0x00U,  /*!< I2S not yet initialized or disabled                */
  HAL_I2S_STATE_READY      = 0x01U,  /*!< I2S initialized and ready for use                  */
  HAL_I2S_STATE_BUSY       = 0x02U,  /*!< I2S internal process is ongoing                    */
  HAL_I2S_STATE_BUSY_TX    = 0x03U,  /*!< Data Transmission process is ongoing               */
  HAL_I2S_STATE_BUSY_RX    = 0x04U,  /*!< Data Reception process is ongoing                  */
  HAL_I2S_STATE_TIMEOUT    = 0x06U,  /*!< I2S timeout state                                  */
  HAL_I2S_STATE_ERROR      = 0x07U   /*!< I2S error state                                    */
} HAL_I2S_StateTypeDef;

typedef enum
{
	INDEX_I2S0	= 0x0U,
	INDEX_I2S1	= 0x1U,
	INDEX_I2S2	= 0x2U
} HAL_I2S_IndexTypeDef;

typedef enum
{
	I2S_RX	= 0x0U,
	I2S_TX	= 0x1U
} HAL_I2S_RxTxTypeDef;

typedef enum
{
	I2S_INT_XCK_BCK	= 0x0U,
	I2S_EXT_XCK_BCK	= 0x1U
} HAL_I2S_XckBckTypeDef;

typedef enum
{
	I2S_MASTER	= 0x0U,
	I2S_SLAVE	= 0x2U
} HAL_I2S_ModeTypeDef;

typedef enum
{
	I2S_DEBUG_OFF	= 0x0U,
	I2S_DEBUG_0DB	= 0x1U,
	I2S_DEBUG_60DB	= 0x3U
} HAL_I2S_DebugTypeDef;

typedef struct
{
	volatile uint32_t *data_cfg;	/* Config the master/slave mode, LRCK and PCM data */
	volatile uint32_t *bck;		/* Config the BCK source and division */
	volatile uint32_t *xck;		/* Config the XCK source and division */
	volatile uint32_t *fifo_base;	/* Set the fifo buffer base address */
	volatile uint32_t *fifo_len;	/* Set the fifo buffer length */
	volatile uint32_t *fifo_ptr;	/* Record the position of I2S hardware access the FIFO */
	volatile uint32_t *fifo_cnt;	/* Get the current data count */
} HAL_I2S_RegTypeDef;

/**
  * @brief I2S handle Structure definition
  */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1)
typedef struct __I2S_HandleTypeDef
#else
typedef struct
#endif
{
	SPI_TypeDef                *Instance;    /*!< I2S registers base address */

	I2S_InitTypeDef            Init;         /*!< I2S communication parameters */

	uint32_t                   *pTxBuffPtr;  /*!< Pointer to I2S Tx transfer buffer */

	__IO uint16_t              TxXferSize;   /*!< I2S Tx transfer size */

	__IO uint16_t              TxXferCount;  /*!< I2S Tx transfer Counter */

	uint32_t                   *pRxBuffPtr;  /*!< Pointer to I2S Rx transfer buffer */

	__IO uint16_t              RxXferSize;   /*!< I2S Rx transfer size */

	__IO uint16_t              RxXferCount;  /*!< I2S Rx transfer counter
	                                      (This field is initialized at the
	                                       same value as transfer size at the
	                                       beginning of the transfer and
	                                       decremented when a sample is received
	                                       NbSamplesReceived = RxBufferSize-RxBufferCount) */

	__IO HAL_LockTypeDef       Lock;         /*!< I2S locking object */

	__IO HAL_I2S_StateTypeDef  State;        /*!< I2S communication state */

	__IO uint32_t              ErrorCode;    /*!< I2S Error code
	                                        This parameter can be a value of @ref I2S_Error */
	HAL_I2S_IndexTypeDef	Index;
	HAL_I2S_XckBckTypeDef 	FlagSrcXckBck;
	HAL_I2S_RxTxTypeDef	FlagTxRx;
	HAL_I2S_ModeTypeDef	FlagMode;
	HAL_I2S_DebugTypeDef	FlagDebug;
	uint32_t		FlagFifoEn;
	uint32_t		FlagAudEn;
	uint32_t		FlagDMAInc;
	HAL_I2S_RegTypeDef	Reg;
	uint32_t		TxOffset;	/* Record the ptr offset of TX fifo buffer */
	uint32_t		RxOffset;	/* Record the ptr offset of RX fifo buffer */
	uint32_t		PllSrc;

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  void (* TxCpltCallback)(struct __I2S_HandleTypeDef *hi2s);             /*!< I2S Tx Completed callback          */
  void (* RxCpltCallback)(struct __I2S_HandleTypeDef *hi2s);             /*!< I2S Rx Completed callback          */
  void (* TxHalfCpltCallback)(struct __I2S_HandleTypeDef *hi2s);         /*!< I2S Tx Half Completed callback     */
  void (* RxHalfCpltCallback)(struct __I2S_HandleTypeDef *hi2s);         /*!< I2S Rx Half Completed callback     */
  void (* ErrorCallback)(struct __I2S_HandleTypeDef *hi2s);              /*!< I2S Error callback                 */
  void (* MspInitCallback)(struct __I2S_HandleTypeDef *hi2s);            /*!< I2S Msp Init callback              */
  void (* MspDeInitCallback)(struct __I2S_HandleTypeDef *hi2s);          /*!< I2S Msp DeInit callback            */

#endif  /* USE_HAL_I2S_REGISTER_CALLBACKS */
} I2S_HandleTypeDef;

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
/**
  * @brief  HAL I2S Callback ID enumeration definition
  */
typedef enum
{
  HAL_I2S_TX_COMPLETE_CB_ID             = 0x00U,    /*!< I2S Tx Completed callback ID         */
  HAL_I2S_RX_COMPLETE_CB_ID             = 0x01U,    /*!< I2S Rx Completed callback ID         */
  HAL_I2S_TX_HALF_COMPLETE_CB_ID        = 0x03U,    /*!< I2S Tx Half Completed callback ID    */
  HAL_I2S_RX_HALF_COMPLETE_CB_ID        = 0x04U,    /*!< I2S Rx Half Completed callback ID    */
  HAL_I2S_ERROR_CB_ID                   = 0x06U,    /*!< I2S Error callback ID                */
  HAL_I2S_MSPINIT_CB_ID                 = 0x07U,    /*!< I2S Msp Init callback ID             */
  HAL_I2S_MSPDEINIT_CB_ID               = 0x08U     /*!< I2S Msp DeInit callback ID           */

} HAL_I2S_CallbackIDTypeDef;

/**
  * @brief  HAL I2S Callback pointer definition
  */
typedef  void (*pI2S_CallbackTypeDef)(I2S_HandleTypeDef *hi2s); /*!< pointer to an I2S callback function */

#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup I2S_Exported_macros I2S Exported Macros
  * @{
  */

/** @brief  Reset I2S handle state
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
#define __HAL_I2S_RESET_HANDLE_STATE(__HANDLE__)                do{                                                  \
                                                                    (__HANDLE__)->State = HAL_I2S_STATE_RESET;       \
                                                                    (__HANDLE__)->MspInitCallback = NULL;            \
                                                                    (__HANDLE__)->MspDeInitCallback = NULL;          \
                                                                  } while(0)
#else
#define __HAL_I2S_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_I2S_STATE_RESET)
#endif

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup I2S_Exported_Functions
  * @{
  */

/** @addtogroup I2S_Exported_Functions_Group1
  * @{
  */
/* Initialization/de-initialization functions  ********************************/
void HAL_I2S_Default_Init(void);
HAL_StatusTypeDef HAL_I2S_Init(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_DeInit(I2S_HandleTypeDef *hi2s);
void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s);
void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s);

/* Callbacks Register/UnRegister functions  ***********************************/
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
HAL_StatusTypeDef HAL_I2S_RegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID, pI2S_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_I2S_UnRegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup I2S_Exported_Functions_Group2
  * @{
  */
/* I/O operation functions  ***************************************************/
//HAL_StatusTypeDef HAL_I2S_Debug_Tx_Sine(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_Debug_Tx_Sine(HAL_I2S_IndexTypeDef index, HAL_I2S_DebugTypeDef db);

/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_I2S_Transmit(I2S_HandleTypeDef *hi2s, uint32_t *pData, uint32_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2S_Receive(I2S_HandleTypeDef *hi2s, uint32_t *pData, uint32_t Size, uint32_t Timeout);

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_I2S_Transmit_IT(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2S_Receive_IT(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size);
void HAL_I2S_IRQHandler(I2S_HandleTypeDef *hi2s);

/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_I2S_Transmit_DMA(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2S_Receive_DMA(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size);

HAL_StatusTypeDef HAL_I2S_DMAPause(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_DMAResume(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_DMAStop(I2S_HandleTypeDef *hi2s);

/* Callbacks used in non blocking modes (Interrupt and DMA) *******************/
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s);
/**
  * @}
  */

/** @addtogroup I2S_Exported_Functions_Group3
  * @{
  */
/* Peripheral Control and State functions  ************************************/
HAL_I2S_StateTypeDef HAL_I2S_GetState(I2S_HandleTypeDef *hi2s);
uint32_t HAL_I2S_GetError(I2S_HandleTypeDef *hi2s);

/* Private macros ------------------------------------------------------------*/
/** @brief  Checks if I2S Mode parameter is in allowed range.
  * @param  __MODE__ specifies the I2S Mode.
  *         This parameter can be a value of @ref I2S_Mode
  * @retval None
  */
#define IS_I2S_MODE(__MODE__) (((__MODE__) == I2S_MODE_SLAVE_TX)  || \
                               ((__MODE__) == I2S_MODE_SLAVE_RX)  || \
                               ((__MODE__) == I2S_MODE_MASTER_TX) || \
                               ((__MODE__) == I2S_MODE_MASTER_RX))

#define IS_I2S_STANDARD(__STANDARD__) (((__STANDARD__) == I2S_STANDARD_PHILIPS)   || \
                                       ((__STANDARD__) == I2S_STANDARD_MSB)       || \
                                       ((__STANDARD__) == I2S_STANDARD_LSB)       || \
                                       ((__STANDARD__) == I2S_STANDARD_PCM_SHORT) || \
                                       ((__STANDARD__) == I2S_STANDARD_PCM_LONG))

#define IS_I2S_DATA_FORMAT(__FORMAT__) (((__FORMAT__) == I2S_DATAFORMAT_16B)          || \
                                        ((__FORMAT__) == I2S_DATAFORMAT_16B_EXTENDED) || \
                                        ((__FORMAT__) == I2S_DATAFORMAT_24B)          || \
                                        ((__FORMAT__) == I2S_DATAFORMAT_32B))

#define IS_I2S_MCLK_OUTPUT(__OUTPUT__) (((__OUTPUT__) == I2S_MCLKOUTPUT_ENABLE) || \
                                        ((__OUTPUT__) == I2S_MCLKOUTPUT_DISABLE))

#define IS_I2S_AUDIO_FREQ(__FREQ__) ((((__FREQ__) >= I2S_AUDIOFREQ_8K)    && \
                                      ((__FREQ__) <= I2S_AUDIOFREQ_192K)) || \
                                      ((__FREQ__) == I2S_AUDIOFREQ_DEFAULT))

#endif /* SPI_I2S_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif /* SP645_HAL_I2S_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

