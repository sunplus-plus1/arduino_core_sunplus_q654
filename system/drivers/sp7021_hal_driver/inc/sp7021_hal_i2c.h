#ifndef __SP7021_HAL_I2C_H
#define __SP7021_HAL_I2C_H

#include "core_armv5.h"
#include "sp7021_arm926.h"
#include "sp7021_hal_def.h"
#include "cache.h"

/* pinmux */
#define	PINMUX_PIN3_03		20
#define	PINMUX_PIN3_05		22
#define PINMUX_I2C_SCL		PINMUX_PIN3_03
#define PINMUX_I2C_SDA		PINMUX_PIN3_05

/****************************************
* I2C Master
****************************************/

//control0
#define I2C_CTL0_FREQ(x)                  (x<<24)  //bit[26:24]
#define I2C_CTL0_PREFETCH                 (1<<18)  //Now as read mode need to set high, otherwise don¡¦t care
#define I2C_CTL0_RESTART_EN               (1<<17)  //0:disable 1:enable
#define I2C_CTL0_SUBADDR_EN               (1<<16)  //For restart mode need to set high
#define I2C_CTL0_SW_RESET                 (1<<15)
#define I2C_CTL0_SLAVE_ADDR(x)            (x<<1)   //bit[7:1]

//control1
#define I2C_CTL1_ALL_CLR                  (0x3FF)
#define I2C_CTL1_EMPTY_CLR                (1<<9)
#define I2C_CTL1_SCL_HOLD_TOO_LONG_CLR    (1<<8)
#define I2C_CTL1_SCL_WAIT_CLR             (1<<7)
#define I2C_CTL1_EMPTY_THRESHOLD_CLR      (1<<6)
#define I2C_CTL1_DATA_NACK_CLR            (1<<5)
#define I2C_CTL1_ADDRESS_NACK_CLR         (1<<4)
#define I2C_CTL1_BUSY_CLR                 (1<<3)
#define I2C_CTL1_CLKERR_CLR               (1<<2)
#define I2C_CTL1_DONE_CLR                 (1<<1)
#define I2C_CTL1_SIFBUSY_CLR              (1<<0)

//control2
#define I2C_CTL2_FREQ_CUSTOM(x)           (x<<0)   //bit[10:0]
#define I2C_CTL2_SCL_DELAY(x)             (x<<24)  //bit[25:24]
#define I2C_CTL2_SDA_HALF_ENABLE          (1<<31)

//control5
#define I2C_CTL5_RING_VALUE(x)           (x>>21)   //bit[23:21]
#define I2C_CTL5_STATE(x)                (x>>17)   //bit[20:17]
#define I2C_CTL5_ROBE_MODE(x)            (x>>12)   //bit[15:12]
#define I2C_CTL5_SIFBUSY                 (1<<8)

//control6
#define I2C_CTL6_BURST_RDATA_CLR          I2C_EN1_BURST_RDATA_INT

//control7
#define I2C_CTL7_RDCOUNT(x)               (x<<16)  //bit[31:16]
#define I2C_CTL7_WRCOUNT(x)               (x<<0)   //bit[15:0]

//interrupt
#define I2C_INT_RINC_INDEX(x)             (x<<18)  //bit[20:18]
#define I2C_INT_WINC_INDEX(x)             (x<<15)  //bit[17:15]
#define I2C_INT_SCL_HOLD_TOO_LONG_FLAG    (1<<11)
#define I2C_INT_WFIFO_ENABLE              (1<<10)
#define I2C_INT_FULL_FLAG                 (1<<9)
#define I2C_INT_EMPTY_FLAG                (1<<8)
#define I2C_INT_SCL_WAIT_FLAG             (1<<7)
#define I2C_INT_EMPTY_THRESHOLD_FLAG      (1<<6)
#define I2C_INT_DATA_NACK_FLAG            (1<<5)
#define I2C_INT_ADDRESS_NACK_FLAG         (1<<4)
#define I2C_INT_BUSY_FLAG                 (1<<3)
#define I2C_INT_CLKERR_FLAG               (1<<2)
#define I2C_INT_DONE_FLAG                 (1<<1)
#define I2C_INT_SIFBUSY_FLAG              (1<<0)

//interrupt enable0
#define I2C_EN0_SCL_HOLD_TOO_LONG_INT     (1<<13)
#define I2C_EN0_NACK_INT                  (1<<12)
#define I2C_EN0_CTL_EMPTY_THRESHOLD(x)    (x<<9)  //bit[11:9]
#define I2C_EN0_EMPTY_INT                 (1<<8)
#define I2C_EN0_SCL_WAIT_INT              (1<<7)
#define I2C_EN0_EMPTY_THRESHOLD_INT       (1<<6)
#define I2C_EN0_DATA_NACK_INT             (1<<5)
#define I2C_EN0_ADDRESS_NACK_INT          (1<<4)
#define I2C_EN0_BUSY_INT                  (1<<3)
#define I2C_EN0_CLKERR_INT                (1<<2)
#define I2C_EN0_DONE_INT                  (1<<1)
#define I2C_EN0_SIFBUSY_INT               (1<<0)


#define I2C_CTL0_FREQ_MASK                  (0x7)     // 3 bit
#define I2C_CTL0_SLAVE_ADDR_MASK            (0x7F)    // 7 bit
#define I2C_CTL2_FREQ_CUSTOM_MASK           (0x7FF)   // 11 bit
#define I2C_CTL2_SCL_DELAY_MASK             (0x3)     // 2 bit
#define I2C_CTL7_RW_COUNT_MASK              (0xFFFF)  // 16 bit
#define I2C_EN0_CTL_EMPTY_THRESHOLD_MASK    (0x7)     // 3 bit
#define I2C_SG_DMA_LLI_INDEX_MASK           (0x1F)    // 5 bit

//interrupt enable1
#define I2C_EN1_BURST_RDATA_INT           (0x80008000)  //must sync with GET_BYTES_EACHTIME

//interrupt enable2
#define I2C_EN2_BURST_RDATA_OVERFLOW_INT  (0xFFFFFFFF)

//i2c master mode
#define I2C_MODE_DMA_MODE                 (1<<2)
#define I2C_MODE_MANUAL_MODE              (1<<1)  //0:trigger mode 1:auto mode
#define I2C_MODE_MANUAL_TRIG              (1<<0)

//i2c master status2
#define I2C_SW_RESET_DONE                 (1<<0)


#define I2C_BURST_RDATA_BYTES        16
#define I2C_BURST_RDATA_FLAG         0x80008000
#define I2C_BURST_RDATA_ALL_FLAG     0xFFFFFFFF

//burst write use
#define I2C_EMPTY_THRESHOLD_VALUE    4
/****************************************
* GDMA
****************************************/

//dma config
#define I2C_DMA_CFG_DMA_GO                (1<<8)
#define I2C_DMA_CFG_NON_BUF_MODE          (1<<2)
#define I2C_DMA_CFG_SAME_SLAVE            (1<<1)
#define I2C_DMA_CFG_DMA_MODE              (1<<0)

//dma interrupt flag
#define I2C_DMA_INT_LENGTH0_FLAG          (1<<6)
#define I2C_DMA_INT_THRESHOLD_FLAG        (1<<5)
#define I2C_DMA_INT_IP_TIMEOUT_FLAG       (1<<4)
#define I2C_DMA_INT_GDMA_TIMEOUT_FLAG     (1<<3)
#define I2C_DMA_INT_WB_EN_ERROR_FLAG      (1<<2)
#define I2C_DMA_INT_WCNT_ERROR_FLAG       (1<<1)
#define I2C_DMA_INT_DMA_DONE_FLAG         (1<<0)

//dma interrupt enable
#define I2C_DMA_EN_LENGTH0_INT            (1<<6)
#define I2C_DMA_EN_THRESHOLD_INT          (1<<5)
#define I2C_DMA_EN_IP_TIMEOUT_INT         (1<<4)
#define I2C_DMA_EN_GDMA_TIMEOUT_INT       (1<<3)
#define I2C_DMA_EN_WB_EN_ERROR_INT        (1<<2)
#define I2C_DMA_EN_WCNT_ERROR_INT         (1<<1)
#define I2C_DMA_EN_DMA_DONE_INT           (1<<0)

#define I2C_RESET(id, val)          ((1 << (16 + id)) | (val << id))
#define I2C_CLKEN(id, val)          ((1 << (16 + id)) | (val << id))
#define I2C_GCLKEN(id, val)         ((1 << (16 + id)) | (val << id))

#define I2C_MASTER_NUM			4
#define I2C_MSG_DATA_SIZE		255

#define I2C_CLK_SOURCE_FREQ		27000  // KHz(27MHz)

#define SET 	1
#define RESET 	0
#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) (((((__HANDLE__)->Instance->interrupt) & \
                                                    (__FLAG__)) == (__FLAG__)) ? SET : RESET)

typedef struct
{
	__IOM uint32_t control0;      /* 00 */
	__IOM uint32_t control1;      /* 01 */
	__IOM uint32_t control2;      /* 02 */
	__IOM uint32_t control3;      /* 03 */
	__IOM uint32_t control4;      /* 04 */
	__IOM uint32_t control5;      /* 05 */
	__IOM uint32_t i2cm_status0;  /* 06 */
	__IOM uint32_t interrupt;     /* 07 */
	__IOM uint32_t int_en0;       /* 08 */
	__IOM uint32_t i2cm_mode;     /* 09 */
	__IOM uint32_t i2cm_status1;  /* 10 */
	__IOM uint32_t i2cm_status2;  /* 11 */
	__IOM uint32_t control6;      /* 12 */
	__IOM uint32_t int_en1;       /* 13 */
	__IOM uint32_t i2cm_status3;  /* 14 */
	__IOM uint32_t i2cm_status4;  /* 15 */
	__IOM uint32_t int_en2;       /* 16 */
	__IOM uint32_t control7;      /* 17 */
	__IOM uint32_t control8;      /* 18 */
	__IOM uint32_t control9;      /* 19 */
	RESERVED(0[3], uint32_t)      /* 20~22 */
	__IOM uint32_t version;       /* 23 */
	__IOM uint32_t data00_03;     /* 24 */
	__IOM uint32_t data04_07;     /* 25 */
	__IOM uint32_t data08_11;     /* 26 */
	__IOM uint32_t data12_15;     /* 27 */
	__IOM uint32_t data16_19;     /* 28 */
	__IOM uint32_t data20_23;     /* 29 */
	__IOM uint32_t data24_27;     /* 30 */
	__IOM uint32_t data28_31;     /* 31 */
} I2C_TypeDef;

typedef struct
{
	__IOM uint32_t hw_version;                /* 00 */
	__IOM uint32_t dma_config;                /* 01 */
	__IOM uint32_t dma_length;                /* 02 */
	__IOM uint32_t dma_addr;                  /* 03 */
	__IOM uint32_t port_mux;                  /* 04 */
	__IOM uint32_t int_flag;                  /* 05 */
	__IOM uint32_t int_en;                    /* 06 */
	__IOM uint32_t sw_reset_state;            /* 07 */
	RESERVED(0[2], uint32_t)                  /* 08~09 */
	__IOM uint32_t sg_dma_index;              /* 10 */
	__IOM uint32_t sg_dma_config;             /* 11 */
	__IOM uint32_t sg_dma_length;             /* 12 */
	__IOM uint32_t sg_dma_addr;               /* 13 */
	RESERVED(1, uint32_t)                     /* 14 */
	__IOM uint32_t sg_setting;                /* 15 */
	__IOM uint32_t threshold;                 /* 16 */
	RESERVED(2, uint32_t)                     /* 17 */
	__IOM uint32_t gdma_read_timeout;         /* 18 */
	__IOM uint32_t gdma_write_timeout;        /* 19 */
	__IOM uint32_t ip_read_timeout;           /* 20 */
	__IOM uint32_t ip_write_timeout;          /* 21 */
	__IOM uint32_t write_cnt_debug;           /* 22 */
	__IOM uint32_t w_byte_en_debug;           /* 23 */
	__IOM uint32_t sw_reset_write_cnt_debug;  /* 24 */
	__IOM uint32_t sw_reset_read_cnt_debug;   /* 25 */
	RESERVED(3[6], uint32_t)                  /* 26~31 */
} I2C_GDMA_TypeDef;

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
	HAL_I2C_STATE_BUSY              = 0x24U,   /*!< An internal process is ongoing            */
	HAL_I2C_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing      */
	HAL_I2C_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing         */
	HAL_I2C_STATE_LISTEN            = 0x28U,   /*!< Address Listen Mode is ongoing            */
	HAL_I2C_STATE_BUSY_TX_LISTEN    = 0x29U,   /*!< Address Listen Mode and Data Transmission
                                               	   process is ongoing                         */
	HAL_I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,   /*!< Address Listen Mode and Data Reception
                                                   process is ongoing                         */
	HAL_I2C_STATE_ABORT             = 0x60U,   /*!< Abort user request ongoing                */
	HAL_I2C_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state                             */
	HAL_I2C_STATE_ERROR             = 0xE0U    /*!< Error                                     */

} HAL_I2C_StateTypeDef;

/** @defgroup HAL_mode_structure_definition HAL mode structure definition
  * @brief  HAL Mode structure definition
  * @note  HAL I2C Mode value coding follow below described bitmap :\n
  *          b7     (not used)\n
  *             x  : Should be set to 0\n
  *          b6\n
  *             0  : None\n
  *             1  : Memory (HAL I2C communication is in Memory Mode)\n
  *          b5\n
  *             0  : None\n
  *             1  : Slave (HAL I2C communication is in Slave Mode)\n
  *          b4\n
  *             0  : None\n
  *             1  : Master (HAL I2C communication is in Master Mode)\n
  *          b3-b2-b1-b0  (not used)\n
  *             xxxx : Should be set to 0000
  * @{
  */
typedef enum
{
  HAL_I2C_MODE_NONE               = 0x00U,   /*!< No I2C communication on going             */
  HAL_I2C_MODE_MASTER             = 0x10U,   /*!< I2C communication is in Master Mode       */
  HAL_I2C_MODE_SLAVE              = 0x20U,   /*!< I2C communication is in Slave Mode        */
  HAL_I2C_MODE_MEM                = 0x40U    /*!< I2C communication is in Memory Mode       */

} HAL_I2C_ModeTypeDef;
/**
  * @}
  */

typedef enum
{
	HAL_I2C_ERR_NONE,               /* successful */
	HAL_I2C_ERR_I2C_BUSY,           /* I2C is busy */
	HAL_I2C_ERR_INVALID_DEVID,      /* device id is invalid */
	HAL_I2C_ERR_INVALID_CNT,        /* read or write count is invalid */
	HAL_I2C_ERR_TIMEOUT_OUT,        /* wait timeout */
	HAL_I2C_ERR_RECEIVE_NACK,       /* receive NACK */
	HAL_I2C_ERR_FIFO_EMPTY,         /* FIFO empty */
	HAL_I2C_ERR_SCL_HOLD_TOO_LONG,  /* SCL hlod too long */
	HAL_I2C_ERR_RDATA_OVERFLOW,     /* rdata overflow */
	HAL_I2C_ERR_INVALID_STATE,      /* read write state is invalid */
	HAL_I2C_ERR_REQUESET_IRQ        /* request irq failed */
} I2C_Status_e;

typedef enum
{
	I2C_WRITE_STATE,  /* i2c is write */
	I2C_READ_STATE,   /* i2c is read */
	I2C_IDLE_STATE,   /* i2c is idle */
	I2C_DMA_WRITE_STATE,/* i2c is dma write */
	I2C_DMA_READ_STATE /* i2c is dma read */
} I2C_State_e;

typedef enum
{	
	I2C_DMA_WRITE_MODE,
	I2C_DMA_READ_MODE
} I2C_DMA_RW_Mode_e;

typedef enum
{
	I2C_WRITE_MODE,
	I2C_READ_MODE,
	I2C_RESTART_MODE
} I2C_RW_Mode_e;

typedef enum
{
	I2C_TRIGGER,
	I2C_AUTO
} I2C_Active_Mode_e;


typedef struct
{ 
	uint32_t freq;
	//uint16_t slave_addr;

	uint32_t *rdata_flag;

	uint32_t flag;
	uint32_t delay;
	/* decide how many byte to write/read */

	I2C_Active_Mode_e mode;
	uint32_t *rdata;
	uint32_t *wdata;
	I2C_RW_Mode_e rw_mode;
	/* interrupt enable */
	uint32_t int0;
	uint32_t rdata_en;
	uint32_t overflow_en;
	unsigned char threshold;

	uint32_t gdma_addr;
	uint32_t gdma_length;
	I2C_DMA_RW_Mode_e gdma_rw_mode;
	uint32_t gdma_int;
} I2C_InitTypeDef;

/** @defgroup I2C_handle_Structure_definition I2C handle Structure definition
  * @brief  I2C handle Structure definition
  * @{
  */
typedef struct __I2C_HandleTypeDef
{
	I2C_TypeDef                *Instance;      /*!< I2C registers base address                */
	I2C_GDMA_TypeDef		   *gdma;
	I2C_InitTypeDef            Init;           /*!< I2C communication parameters              */
	uint8_t 				   Index;
	uint16_t 				   RegDataIndex;
	uint8_t                    *pBuffPtr;      /*!< Pointer to I2C transfer buffer            */
	uint16_t                   XferSize;       /*!< I2C transfer size                         */
	__IO uint16_t              XferCount;      /*!< I2C transfer counter                      */
	__IO uint32_t              XferOptions;    /*!< I2C sequantial transfer options, this parameter can
                                                  be a value of @ref I2C_XFEROPTIONS */
	//__IO uint32_t              PreviousState;  /*!< I2C communication Previous state          */
	HAL_StatusTypeDef(*XferISR)(struct __I2C_HandleTypeDef *hi2c, uint32_t ITFlags, uint32_t ITSources);  /*!< I2C transfer IRQ handler function pointer */
	//DMA_HandleTypeDef          *hdmatx;        /*!< I2C Tx DMA handle parameters              */
	//DMA_HandleTypeDef          *hdmarx;        /*!< I2C Rx DMA handle parameters              */
	HAL_LockTypeDef            Lock;           /*!< I2C locking object                        */
	__IO HAL_I2C_StateTypeDef  State;          /*!< I2C communication state                   */
	__IO HAL_I2C_ModeTypeDef   Mode;           /*!< I2C communication mode                    */
	__IO uint32_t              ErrorCode;      /*!< I2C Error code                            */
	__IO uint32_t              AddrEventCount; /*!< I2C Address Event counter                 */
} I2C_HandleTypeDef;

/* pin mux */
#define	PINMUX_PIN2_02		11
#define	PINMUX_PIN2_03		12
#define PINMUX_I2C_SDA		PINMUX_PIN2_02
#define PINMUX_I2C_SCL		PINMUX_PIN2_03

#define MOON3_PIN_MUX_BASE RF_GRP(3, 0)
#define SP_PIN_MUX_CTRL3  ((GROUP3_Type*) MOON3_PIN_MUX_BASE)

#define SP_I2C0_REG 	((I2C_TypeDef *)RF_GRP(140, 0))
#define SP_GDMA0_REG 	((I2C_GDMA_TypeDef *)RF_GRP(141, 0))
#define SP_I2C1_REG 	((I2C_TypeDef *)RF_GRP(142, 0))
#define SP_GDMA1_REG 	((I2C_GDMA_TypeDef *)RF_GRP(143, 0))
#define SP_I2C2_REG 	((I2C_TypeDef *)RF_GRP(144, 0))
#define SP_GDMA2_REG 	((I2C_GDMA_TypeDef *)RF_GRP(145, 0))
#define SP_I2C3_REG 	((I2C_TypeDef *)RF_GRP(146, 0))
#define SP_GDMA3_REG 	((I2C_GDMA_TypeDef *)RF_GRP(147, 0))

void HAL_I2C_TEST(void);
void HAL_I2C_PinMux(I2C_HandleTypeDef *hi2c, int sda_pinmux, int scl_pinmux);
void HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size,
                                          uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size,
                                         uint32_t Timeout);
#endif
