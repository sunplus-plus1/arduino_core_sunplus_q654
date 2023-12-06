#ifndef __SP7350_HAL_I2C_H
#define __SP7350_HAL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sp7350_cm4.h"
#include "sp7350_hal_def.h"
#include "irq_ctrl.h"
#include "sp7350xx.h"

#define STATUS_READ_IN_PROGRESS		0x1
#define STATUS_WRITE_IN_PROGRESS	0x2

/* Confirm the value by reading reg 0xf4(RX[15:8] TX[23:16]) */
#define I2C_FIFO_DEPTH	8
/* Data_cmd_reg (0x10) Master read cmd */
#define I2C_READ_DATA	0x100

#if 0
/* DMA transfer rate far more than I2C transfer rate, Therefore RX thresold > TX thresold */
/* For TX, FIFO data length less than or equal to watermark level (= TDRL), DMA request trigger */
#define I2C_TDRL	0//2
/* For RX, FIFO data length more than or equal to watermark level (= RDRL + 1), DMA request trigger */
#define I2C_RDRL	0//5
#endif

/* I2C Transmit Modes */
#define I2C_MODE_BURST	0x0
#define I2C_MODE_INTR	0x1
//#define I2C_MODE_DMA	0x2

/* I2C Frequency Modes */
#define I2C_MAX_STANDARD_MODE_FREQ	100000
#define I2C_MAX_FAST_MODE_FREQ		400000
#define I2C_MAX_FAST_MODE_PLUS_FREQ	1000000
#define I2C_MAX_HIGH_SPEED_MODE_FREQ	3400000

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

#define I2C_HAL_INFO(inst, idx, clk, pin, irqn, dma) \
	{ .instance = (inst), .index = (idx), \
	  .clk_id = (clk), .pinmux = (pin), \
	  .irq_num = (irqn), .dma_idx = (dma) }

typedef enum {
	I2C0_INDEX = 0UL,
	I2C1_INDEX,
	I2C2_INDEX,
	I2C3_INDEX,
	I2C4_INDEX,
	I2C5_INDEX,
	I2C6_INDEX,
	I2C7_INDEX,
	I2C8_INDEX,
	I2C9_INDEX,

	I2C_NUM
} i2c_index_e;

typedef enum
{
	I2C_M_WRITE	= 0x0UL,
	I2C_M_READ	= 0x1UL,
} HAL_I2C_Direction;

struct i2c_msg {
	uint16_t addr;
	uint32_t flag;
	uint32_t len;
	uint8_t *buf;
};

struct i2c_hal_info {
	volatile I2C_TypeDef *instance;
	i2c_index_e index;
	MODULE_ID_Type clk_id;
	PINMUX_Type pinmux;
	IRQn_Type irq_num;
	uint32_t dma_idx; /* DMA mode use */
};

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
	uint32_t Timing;
	uint32_t AddressingMode;
} I2C_InitTypeDef;

typedef struct __I2C_HandleTypeDef
{
	__IO I2C_TypeDef *Instance; /*!< I2C registers base address */
	I2C_InitTypeDef Init; /*!< I2C communication parameters */
	HAL_LockTypeDef Lock; /*!< I2C locking object */
	__IO HAL_I2C_StateTypeDef State; /*!< I2C communication state */
	__IO uint32_t ErrorCode; /*!< I2C Error code */
	uint8_t Index;
	uint8_t Mode;

	//uint32_t DMAIndex;
	struct i2c_msg *msgs;
	int msgs_num;
	int msg_write_idx;
	int msg_read_idx;
	uint32_t rx_outstanding; /*!< Length of data which is left space in FIFO but not yet available */
	uint32_t master_cfg;
	uint32_t abort_source; /*!< I2C transfer source of abort */
	uint32_t rx_buf_len;
	uint32_t tx_buf_len;
	uint8_t *rx_buf;
	uint8_t *tx_buf;
	uint8_t status;
	uint8_t msg_err;
	uint8_t completion;
} I2C_HandleTypeDef;

HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Master_Transfer(I2C_HandleTypeDef *hi2c, struct i2c_msg *msgs, int num);
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
					  uint8_t *pData, uint32_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
					 uint8_t *pData, uint32_t Size, uint32_t Timeout);
HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef * hi2c);
void HAL_I2C_ClearError(I2C_HandleTypeDef * hi2c);
uint32_t HAL_I2C_GetError(I2C_HandleTypeDef * hi2c);
void HAL_I2C_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif
