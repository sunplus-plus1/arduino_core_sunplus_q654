#include "sp7021_hal_i2c.h"

#define MAX_NBYTE_SIZE		32U

static int get_instance_index(uint32_t instance);
static I2C_GDMA_TypeDef *get_gdma_index(uint32_t instance);
static void _i2c_status_clear(I2C_HandleTypeDef *hi2c, uint32_t flag);
static void _i2c_dma_int_flag_clear(I2C_HandleTypeDef *hi2c, uint32_t flag);
static void _i2c_reset(I2C_HandleTypeDef *hi2c);
static void _i2c_data0_set(I2C_HandleTypeDef *hi2c, uint32_t *wdata);
static void _i2c_int_en0_disable(I2C_HandleTypeDef *hi2c, uint32_t int0);
static void _i2c_rdata_flag_get(I2C_HandleTypeDef *hi2c,uint32_t *flag);
static void _i2c_data_get(I2C_HandleTypeDef *hi2c, int index, uint32_t *rdata);
static void _i2c_rdata_flag_clear(I2C_HandleTypeDef *hi2c, uint32_t flag);
static void _i2c_clock_freq_set(I2C_HandleTypeDef *hi2c, uint32_t freq);
static void _i2c_slave_addr_set(I2C_HandleTypeDef *hi2c, uint16_t slave_addr);
static void _i2c_scl_delay_set(I2C_HandleTypeDef *hi2c, uint32_t delay);
static void _i2c_trans_cnt_set(I2C_HandleTypeDef *hi2c, uint16_t write_cnt, uint16_t read_cnt);
static void _i2c_active_mode_set(I2C_HandleTypeDef *hi2c, I2C_Active_Mode_e mode);
static void _i2c_data_set(I2C_HandleTypeDef *hi2c, uint32_t *wdata);
static void _i2c_rw_mode_set(I2C_HandleTypeDef *hi2c, I2C_RW_Mode_e rw_mode);
static void _i2c_int_en0_set(I2C_HandleTypeDef *hi2c, uint32_t int0);
static void _i2c_int_en1_set(I2C_HandleTypeDef *hi2c, uint32_t int1);
static void _i2c_int_en2_set(I2C_HandleTypeDef *hi2c, uint32_t int2);
static void _i2c_enable(I2C_HandleTypeDef *hi2c);
static void _i2c_manual_trigger(I2C_HandleTypeDef *hi2c);
static void _i2c_int_en0_with_thershold_set(I2C_HandleTypeDef *hi2c, uint32_t int0, uint8_t threshold);
static void _i2c_dma_mode_enable(I2C_HandleTypeDef *hi2c);
static void _i2c_dma_addr_set(I2C_HandleTypeDef *hi2c,  uint32_t addr);
static void _i2c_dma_length_set(I2C_HandleTypeDef *hi2c, uint32_t len);
static void _i2c_dma_rw_mode_set(I2C_HandleTypeDef *hi2c, I2C_DMA_RW_Mode_e mode);
static void _i2c_dma_int_en_set(I2C_HandleTypeDef *hi2c,  uint32_t int0);
static void _i2c_dma_go_set(I2C_HandleTypeDef *hi2c);

uint8_t tx_buffer[I2C_MASTER_NUM][I2C_MSG_DATA_SIZE];
uint8_t rx_buffer[I2C_MASTER_NUM][I2C_MSG_DATA_SIZE];

void HAL_I2C_TEST()
{
	printf("Build I2C @%s,%s\n", __DATE__, __TIME__);

	I2C_HandleTypeDef i2c_test;
	i2c_test.Instance = SP_I2C0_REG;
	i2c_test.Init.freq = 100;

	HAL_I2C_Init(&i2c_test);
	//HAL_I2C_PinMux(&i2c_test, PINMUX_PIN3_05, PINMUX_PIN3_03);//	29,27
	HAL_PINMUX_Cfg(PINMUX_I2CM0_CK, PINMUX_PIN3_03);//27
	HAL_PINMUX_Cfg(PINMUX_I2CM0_DAT, PINMUX_PIN3_05);//29

	*tx_buffer[i2c_test.Index] = 0x2400;

	HAL_I2C_Master_Transmit(&i2c_test, 0x44, tx_buffer, 2, 0);// dev addr 0x44, write 2 bytes of data
	while(!(i2c_test.State == HAL_I2C_STATE_READY));
	HAL_I2C_Master_Receive(&i2c_test, 0x44, rx_buffer, 6, 0);// dev addr 0x44, read 6 bytes of data

	int temp = (rx_buffer[i2c_test.Index][0] << 8) | rx_buffer[i2c_test.Index][1];
	temp = temp * 17500/65536;
	int temp_L = temp % 100;
	int temp_H = temp/100 - 45;

	int RH = (rx_buffer[i2c_test.Index][3] << 8) | rx_buffer[i2c_test.Index][4];
	RH = RH * 10000/65536;
	int RH_L = RH % 100;
	int RH_H = RH / 100;
	printf("temperature:%d, humidity:%d\n ", (temp_H<<8)|temp_L, (RH_H<<8)|RH_L);
}
#if 0 
/* 
  SP7021 have the function of Pin Multiplex. Here set pin for an external interrupt to use
  by configuring the Pinmux control register.
*/
void HAL_I2C_PinMux(I2C_HandleTypeDef *hi2c, int sda_pinmux, int scl_pinmux)
{
	SP_PIN_MUX_CTRL3->pinmux_i2cm[hi2c->Index] = RF_MASK_V(0x7f, scl_pinmux);
	SP_PIN_MUX_CTRL3->pinmux_i2cm[hi2c->Index] = RF_MASK_V(0x7f << 8, sda_pinmux << 8);
}
#endif
void HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
	if (hi2c == NULL)
  	{
    	return HAL_ERROR;
  	}
  	hi2c->State = HAL_I2C_STATE_BUSY;

	hi2c->Index = get_instance_index(hi2c->Instance);
	hi2c->gdma = get_gdma_index(hi2c->Instance);

	hi2c->ErrorCode = HAL_I2C_ERR_NONE;
  	hi2c->State = HAL_I2C_STATE_READY;
  	hi2c->Mode = HAL_I2C_MODE_NONE;

	return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size,
                                          uint32_t Timeout)
{
	uint32_t i,j;
	uint16_t burst_cnt = 0;
	uint32_t int0 = 0;
	uint8_t w_data[32] = {0};

	/* WRCOUNT 15:0 */
	if (Size > 0xFFFF)
	{
		printf("I2C write count is invalid !! Size=%d\n", Size);
		return HAL_ERROR;
	}
	/* 
		handle first 32 byte data and the remaining bytes 
		every 4 bytes count as a burst, calculate the burst_cnt.
	*/
	if (Size > MAX_NBYTE_SIZE)
	{
		burst_cnt = (Size - MAX_NBYTE_SIZE) / 4;
		if ((Size - MAX_NBYTE_SIZE) % 4)
		{
			burst_cnt += 1;
		}

		for (i = 0; i < 32; i++)
		{
			w_data[i] = hi2c->pBuffPtr[i];
		}
	} 
	else
	{
		for(i = 0; i < Size; i++)
		{
			w_data[i] = hi2c->pBuffPtr[i];
		}
	}

	int0 = (I2C_EN0_SCL_HOLD_TOO_LONG_INT | I2C_EN0_EMPTY_INT | I2C_EN0_DATA_NACK_INT
			| I2C_EN0_ADDRESS_NACK_INT | I2C_EN0_DONE_INT );

	if (burst_cnt)
	    int0 |= I2C_EN0_EMPTY_THRESHOLD_INT;

	hi2c->pBuffPtr   	= pData;
	hi2c->State 		= HAL_I2C_STATE_BUSY_TX;
	hi2c->XferCount 	= i;

	_i2c_clock_freq_set(hi2c, hi2c->Init.freq);
	_i2c_slave_addr_set(hi2c, DevAddress);
	//printf("BaseAddress:%p\n", hi2c->Instance);
	//printf("control0_addr:0x%x,control0_data:0x%x\n", &(hi2c->Instance->control0), hi2c->Instance->control0);

	_i2c_trans_cnt_set(hi2c, Size, 0);
	_i2c_active_mode_set(hi2c, I2C_TRIGGER);
	_i2c_rw_mode_set(hi2c, I2C_WRITE_MODE);
	_i2c_data_set(hi2c, (uint32_t *)w_data);//store first 32 byte to i2c sram.
	_i2c_int_en0_set(hi2c, int0);
	_i2c_int_en0_with_thershold_set(hi2c, int0, I2C_EMPTY_THRESHOLD_VALUE);		//burst write use
	_i2c_manual_trigger(hi2c);

	while((hi2c->Instance->interrupt & I2C_INT_DONE_FLAG) != I2C_INT_DONE_FLAG){	//INT_DONE = 0 
		
	    //printf("hi2c->Instance->interrupt 0x%x\n",hi2c->Instance->interrupt);

        if((hi2c->Instance->interrupt & I2C_INT_ADDRESS_NACK_FLAG) == I2C_INT_ADDRESS_NACK_FLAG){
        	//printf("hi2c->Instance->interrupt 0x%x\n",hi2c->Instance->interrupt);
		    printf("I2C slave address NACK !!\n");
            break;
		}else if((hi2c->Instance->interrupt & I2C_INT_DATA_NACK_FLAG) == I2C_INT_DATA_NACK_FLAG){
		    printf("I2C slave data NACK !!\n");
            break;
		}else if((hi2c->Instance->interrupt & I2C_INT_SCL_HOLD_TOO_LONG_FLAG) == I2C_INT_SCL_HOLD_TOO_LONG_FLAG){
		    printf("I2C SCL hold too long occur !!\n");
            break;
		}
		
        if((hi2c->Instance->interrupt & I2C_INT_EMPTY_THRESHOLD_FLAG) == I2C_INT_EMPTY_THRESHOLD_FLAG){
		    for (i = 0; i < I2C_EMPTY_THRESHOLD_VALUE; i++) {
			    for (j = 0; j < 4; j++) {
				    if (hi2c->XferCount >= Size) {  //write_cnt <= 32, DataIndex = DataTotalLen = write_cnt
					    w_data[j] = 0;
					} else {
						w_data[j] = pData[hi2c->XferCount];
					}
					hi2c->XferCount++;
				}
				_i2c_data0_set(hi2c, (uint32_t *)w_data);//unsigned char *->unsigned int *, 4 bytes, so using data00_03 is enough  
				burst_cnt--;
								
				if (burst_cnt == 0) {
					_i2c_int_en0_disable(hi2c, (I2C_EN0_EMPTY_THRESHOLD_INT | I2C_EN0_EMPTY_INT));
				    break;
				}
			}
			_i2c_status_clear(hi2c, I2C_CTL1_EMPTY_THRESHOLD_CLR);
	    }
	}
	_i2c_reset(hi2c);  // reset 
    hi2c->State = HAL_I2C_STATE_READY;
    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size,
                                         uint32_t Timeout)
{
	uint32_t i,j,k;
	uint16_t read_cnt = 0;
	uint16_t burst_cnt = 0;
	uint16_t burst_remain = 0;
	uint32_t status3, bit_index;	
	uint32_t int0 = 0, int1 = 0, int2 = 0;
	uint32_t tickstart;

	if (hi2c->State == HAL_I2C_STATE_READY)
	{
	  	/* Process Locked */
	    __HAL_LOCK(hi2c);
	
	    /* Init tickstart for timeout management*/
	    tickstart = HAL_GetTick();
	
	    /* Process Locked */
	    __HAL_LOCK(hi2c);
		_i2c_reset(hi2c);
	
	

		burst_cnt = Size / I2C_BURST_RDATA_BYTES;
		burst_remain = Size % I2C_BURST_RDATA_BYTES;
		
		int0 = (I2C_EN0_SCL_HOLD_TOO_LONG_INT | I2C_EN0_EMPTY_INT | I2C_EN0_DATA_NACK_INT
				| I2C_EN0_ADDRESS_NACK_INT | I2C_EN0_DONE_INT );	
		int1 = I2C_BURST_RDATA_FLAG;
		int2 = I2C_BURST_RDATA_ALL_FLAG;
	
		hi2c->pBuffPtr			= pData;
		hi2c->XferCount 		= 0;	
		hi2c->RegDataIndex 		= 0;
		hi2c->State 			= HAL_I2C_STATE_BUSY_RX;
	
	  	_i2c_clock_freq_set(hi2c, hi2c->Init.freq);
	    _i2c_slave_addr_set(hi2c, DevAddress);			// set slave address

	    _i2c_trans_cnt_set(hi2c, 0, hi2c->XferCount);			// set read writer count
		_i2c_active_mode_set(hi2c, I2C_TRIGGER);			// Trigger mode
		_i2c_rw_mode_set(hi2c, I2C_READ_MODE);			// set read mode	
	
		_i2c_int_en0_set(hi2c, int0);
		_i2c_int_en1_set(hi2c, int1);
		_i2c_int_en2_set(hi2c, int2);
	
		_i2c_manual_trigger(hi2c);						// start Trigger 
	
		while((hi2c->Instance->interrupt & I2C_INT_DONE_FLAG) != I2C_INT_DONE_FLAG)
		{
		    //printf("i2c_regs->interrupt00 0x%x\n",i2c_regs->interrupt);

	        if((hi2c->Instance->interrupt & I2C_INT_ADDRESS_NACK_FLAG)== I2C_INT_ADDRESS_NACK_FLAG){
			    printf("I2C slave address NACK !!\n");
	            break;
			}else if((hi2c->Instance->interrupt & I2C_INT_DATA_NACK_FLAG)== I2C_INT_DATA_NACK_FLAG){
			    printf("I2C slave data NACK !!\n");
	            break;
			}else if((hi2c->Instance->interrupt & I2C_INT_SCL_HOLD_TOO_LONG_FLAG)== I2C_INT_SCL_HOLD_TOO_LONG_FLAG){
			    printf("I2C SCL hold too long occur !!\n");
	            break;
			}
	
	        if(burst_cnt)
	        {
			    _i2c_rdata_flag_get(hi2c, &status3);
				for(i = 0; i < (32 / I2C_BURST_RDATA_BYTES); i++)//I2C_BURST_RDATA_BYTES = 16
				{    
					bit_index = (I2C_BURST_RDATA_BYTES - 1) + (I2C_BURST_RDATA_BYTES * i); // 15  31
			    	if(status3 & (1 << bit_index)) 
			    	{
						for (j = 0; j < (I2C_BURST_RDATA_BYTES / 4); j++)
						{
							k = burst_remain + j;
							if (k >= 8)
							{
								k -= 8;
							}
							_i2c_data_get(hi2c, k, (uint32_t *)(&pData[hi2c->XferCount]));
							hi2c->XferCount += 4;
						}
						_i2c_rdata_flag_clear(hi2c, (((1 << I2C_BURST_RDATA_BYTES) - 1) << (I2C_BURST_RDATA_BYTES * i)));
						hi2c->RegDataIndex += (I2C_BURST_RDATA_BYTES / 4);
						if (hi2c->RegDataIndex >= 8)
						{
							hi2c->RegDataIndex -= 8;
						}
						burst_cnt --;
			    	}
	        	}
			}
		}
	
		if ((hi2c->Instance->interrupt & I2C_INT_DONE_FLAG) == I2C_INT_DONE_FLAG) {
	
			//printf("I2C_INT_DONE_FLAG00 ");
			if ((burst_remain)&&(hi2c->State == HAL_I2C_STATE_BUSY_RX)) {
				j = 0;
				for (i = 0; i < (I2C_BURST_RDATA_BYTES / 4); i++) {
					k = hi2c->RegDataIndex + i;
					if (k >= 8) {
						k -= 8;
					}
					_i2c_data_get(hi2c, k, (unsigned int *)(&rx_buffer[hi2c->Index][j]));
					j += 4;
				}
		
				for (i = 0; i < burst_remain; i++) {
					hi2c->XferCount = hi2c->XferCount + i;
					pData[hi2c->XferCount] = rx_buffer[hi2c->Index][i];			
				}
				//printf("I2C_data i %d ",i);
			}
	     }
	
	    _i2c_reset(hi2c);  // reset
	    hi2c->State = HAL_I2C_STATE_READY;
	    return HAL_OK;
	}
	else
	{
		return HAL_BUSY;
	}
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
                                             uint16_t Size)
{

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
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{

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
                                              uint16_t Size)
{
	uint32_t burst_cnt = 0;
	uint32_t int0;

	if (hi2c->State == HAL_I2C_STATE_READY)
	{
		if (__HAL_I2C_GET_FLAG(hi2c, I2C_INT_BUSY_FLAG) == SET)
		{
			return HAL_BUSY;
		}
		/* Process Locked */
    	__HAL_LOCK(hi2c);
   		
   		_i2c_reset(hi2c);

    	hi2c->Mode        = HAL_I2C_MODE_MASTER;
    	hi2c->ErrorCode   = HAL_I2C_ERR_NONE;
    	//hi2c->XferISR     = I2C_Master_ISR_DMA;

    	int0 = (I2C_EN0_SCL_HOLD_TOO_LONG_INT | I2C_EN0_EMPTY_INT | I2C_EN0_DATA_NACK_INT
			| I2C_EN0_ADDRESS_NACK_INT | I2C_EN0_DONE_INT );

		if (burst_cnt)
	   		int0 |= I2C_EN0_EMPTY_THRESHOLD_INT;

    	hi2c->State       = HAL_I2C_STATE_BUSY_TX;
    	hi2c->XferCount   = Size;
    	hi2c->pBuffPtr    = pData;

    	_i2c_dma_mode_enable(hi2c);

    	_i2c_clock_freq_set(hi2c, hi2c->Init.freq);
	    _i2c_slave_addr_set(hi2c, DevAddress);			// set slave address

	    _i2c_trans_cnt_set(hi2c, hi2c->XferCount, 0);	// set read/write count
		_i2c_active_mode_set(hi2c, I2C_AUTO);			// Auto mode
		_i2c_rw_mode_set(hi2c, I2C_WRITE_MODE);			// set write mode	

		_i2c_int_en0_set(hi2c, int0);

		HAL_DCACHE_FLUSH(hi2c->pBuffPtr, hi2c->XferCount);   // cache data map to dram

		if(hi2c->XferCount < 4)
			_i2c_dma_length_set(hi2c, 4);	
		else
			_i2c_dma_length_set(hi2c, hi2c->XferCount);

		_i2c_dma_addr_set(hi2c, (uint32_t)hi2c->pBuffPtr);
		_i2c_dma_rw_mode_set(hi2c, I2C_DMA_WRITE_MODE);
		_i2c_dma_int_en_set(hi2c, I2C_DMA_EN_DMA_DONE_INT);
		_i2c_dma_go_set(hi2c);

		while(((hi2c->Instance->interrupt & I2C_INT_DONE_FLAG) != I2C_INT_DONE_FLAG) && ((hi2c->gdma->int_flag & I2C_DMA_INT_DMA_DONE_FLAG) != I2C_DMA_INT_DMA_DONE_FLAG))
		{
		    printf("hi2c->gdma->int_flag 0x%x\n",hi2c->gdma->int_flag);
	
	        if((hi2c->gdma->int_flag & I2C_DMA_INT_WCNT_ERROR_FLAG) == I2C_DMA_INT_WCNT_ERROR_FLAG){
			    printf("I2C DMA WCNT ERR !!\n");
	            break;
			}else if((hi2c->gdma->int_flag & I2C_DMA_INT_WB_EN_ERROR_FLAG) == I2C_DMA_INT_WB_EN_ERROR_FLAG){
			    printf("I2C DMA WB EN ERR !!\n");
	            break;
			}else if((hi2c->gdma->int_flag & I2C_DMA_INT_GDMA_TIMEOUT_FLAG) == I2C_DMA_INT_GDMA_TIMEOUT_FLAG){
			    printf("I2C DMA timeout !!\n");
	            break;
			}else if((hi2c->gdma->int_flag & I2C_DMA_INT_IP_TIMEOUT_FLAG) == I2C_DMA_INT_IP_TIMEOUT_FLAG){
			    printf("I2C IP timeout !!\n");
	            break;
			}else if((hi2c->gdma->int_flag & I2C_DMA_INT_THRESHOLD_FLAG) == I2C_DMA_INT_THRESHOLD_FLAG){
			    printf("I2C Length is zero !!\n");
	            break;
			}
	
		}
	
		while((hi2c->Instance->control5 & I2C_CTL5_SIFBUSY) == I2C_CTL5_SIFBUSY)////////////////////////
		{
	
	        if((hi2c->Instance->interrupt & I2C_INT_ADDRESS_NACK_FLAG)== I2C_INT_ADDRESS_NACK_FLAG){
			    printf("I2C slave address NACK !!\n");
	            break;
			}else if((hi2c->Instance->interrupt & I2C_INT_DATA_NACK_FLAG)== I2C_INT_DATA_NACK_FLAG){
			    printf("I2C slave data NACK !!\n");
	            break;
			}else if((hi2c->Instance->interrupt & I2C_INT_SCL_HOLD_TOO_LONG_FLAG)== I2C_INT_SCL_HOLD_TOO_LONG_FLAG){
			    printf("I2C SCL hold too long occur !!\n");
	            break;
			}
	
		}
	
	    _i2c_reset(hi2c);  // reset 
	    hi2c->State = HAL_I2C_STATE_READY;
	}
	else
	{
		return HAL_BUSY;
	}
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
                                             uint16_t Size)
{
	uint32_t temp_reg = 0;
	uint32_t i,j,k;
    uint32_t data_len = 0;	
	uint32_t freq;
	uint32_t int0 = 0, int1 = 0, int2 = 0;
	uint32_t burst_cnt = 0;
	uint32_t burst_remain = 0;

	if (hi2c->State == HAL_I2C_STATE_READY)
  	{
    	if (__HAL_I2C_GET_FLAG(hi2c, I2C_INT_BUSY_FLAG) == SET)
    	{
      		return HAL_BUSY;
    	}
#if 0
    	tickstart = HAL_GetTick();
    	if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY, tickstart) != HAL_OK)
		{
			return HAL_ERROR;
		}
#endif
	    _i2c_reset(hi2c);  // reset 
	    //delay_1ms(1);
	
		int0 = (I2C_EN0_SCL_HOLD_TOO_LONG_INT | I2C_EN0_EMPTY_INT | I2C_EN0_DATA_NACK_INT
				| I2C_EN0_ADDRESS_NACK_INT | I2C_EN0_DONE_INT );	
	
		int1 = I2C_BURST_RDATA_FLAG;
		int2 = I2C_BURST_RDATA_ALL_FLAG;
	
		burst_cnt			= Size / I2C_BURST_RDATA_BYTES;
		burst_remain		= Size % I2C_BURST_RDATA_BYTES;

		hi2c->RegDataIndex	= 0;
	    hi2c->State 		= HAL_I2C_STATE_BUSY_RX;
	    hi2c->XferCount 	= Size;
	    hi2c->pBuffPtr  	= pData;
	
		_i2c_dma_mode_enable(hi2c);								// enable DMA
	
	    _i2c_clock_freq_set(hi2c, hi2c->Init.freq);					// 27M/270 =  100k hz
	
		_i2c_scl_delay_set(hi2c, 0x01);

	    _i2c_slave_addr_set(hi2c, DevAddress);					// set slave address	
	
	    _i2c_active_mode_set(hi2c, I2C_AUTO);					//set AUTO mode
	
	    _i2c_rw_mode_set(hi2c, I2C_READ_MODE);                                  // set read mode
		
		_i2c_int_en0_set(hi2c, int0);
		_i2c_int_en1_set(hi2c, int1);
		_i2c_int_en2_set(hi2c, int2);
	
	    HAL_DCACHE_INVALIDATE(hi2c->pBuffPtr, hi2c->XferCount);   // cache data map to dram   
		
		if(hi2c->XferCount < 4)
	    	_i2c_dma_length_set(hi2c, 4);		
	    else
	    	_i2c_dma_length_set(hi2c, hi2c->XferCount); 

		_i2c_dma_addr_set(hi2c, (uint32_t)hi2c->pBuffPtr);
		_i2c_dma_rw_mode_set(hi2c, I2C_DMA_READ_MODE);
		_i2c_dma_int_en_set(hi2c, I2C_DMA_EN_DMA_DONE_INT);
		_i2c_dma_go_set(hi2c);

		//printf("dma_config 0x%x \n",i2c_dma_regs->dma_config);
		//printf("i2cm_control2 0x%x \n",i2c_regs->control2);
		//printf("i2cm_control0 0x%x \n",i2c_regs->control0);
		//printf("i2cm_mode 0x%x \n",i2c_regs->i2cm_mode);
		//printf("dma_addr 0x%x \n",i2c_dma_regs->dma_addr);		
		//printf("dma_length 0x%x \n",i2c_dma_regs->dma_length);
		//printf("int_flag 0x%x \n",i2c_regs->interrupt);
		//printf("dma_int_flag 0x%x \n",i2c_dma_regs->int_flag);
	
		while((hi2c->Instance->control5 & I2C_CTL5_SIFBUSY) == I2C_CTL5_SIFBUSY)
		{
	
	        if((hi2c->Instance->interrupt & I2C_INT_ADDRESS_NACK_FLAG)== I2C_INT_ADDRESS_NACK_FLAG){
			    printf("I2C slave address NACK !!\n");
	            break;
			}else if((hi2c->Instance->interrupt & I2C_INT_DATA_NACK_FLAG)== I2C_INT_DATA_NACK_FLAG){
			    printf("I2C slave data NACK !!\n");
	            break;
			}else if((hi2c->Instance->interrupt & I2C_INT_SCL_HOLD_TOO_LONG_FLAG)== I2C_INT_SCL_HOLD_TOO_LONG_FLAG){
			    printf("I2C SCL hold too long occur !!\n");
	            break;
			}	   
	
		}
	
		while(((hi2c->Instance->interrupt & I2C_INT_DONE_FLAG) != I2C_INT_DONE_FLAG) && ((hi2c->gdma->int_flag & I2C_DMA_INT_DMA_DONE_FLAG) != I2C_DMA_INT_DMA_DONE_FLAG))
		{
		    //printf("hi2c->gdma->int_flag 0x%x\n",hi2c->gdma->int_flag);
	
	        if((hi2c->gdma->int_flag & I2C_DMA_INT_WCNT_ERROR_FLAG) == I2C_DMA_INT_WCNT_ERROR_FLAG){
			    printf("I2C DMA WCNT ERR !!\n");
	            break;
			}else if((hi2c->gdma->int_flag & I2C_DMA_INT_WB_EN_ERROR_FLAG) == I2C_DMA_INT_WB_EN_ERROR_FLAG){
			    printf("I2C DMA WB EN ERR !!\n");
	            break;
			}else if((hi2c->gdma->int_flag & I2C_DMA_INT_GDMA_TIMEOUT_FLAG) == I2C_DMA_INT_GDMA_TIMEOUT_FLAG){
			    printf("I2C DMA timeout !!\n");
	            break;
			}else if((hi2c->gdma->int_flag & I2C_DMA_INT_IP_TIMEOUT_FLAG) == I2C_DMA_INT_IP_TIMEOUT_FLAG){
			    printf("I2C IP timeout !!\n");
	            break;
			}else if((hi2c->gdma->int_flag & I2C_DMA_INT_THRESHOLD_FLAG) == I2C_DMA_INT_THRESHOLD_FLAG){
			    printf("I2C Length is zero !!\n");
	            break;
			}
		}
	
	    //HAL_DCACHE_INVALIDATE(data_buf,len);   // cache data map to dram   
		_i2c_reset(hi2c);// reset 
	    hi2c->State = HAL_I2C_STATE_READY;
	}
    else
    {
    	return HAL_BUSY;
    }
}
#if 0
/**
  * @brief  This function handles I2C Communication Timeout.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  Flag Specifies the I2C flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status,
                                                    uint32_t Timeout, uint32_t Tickstart)
{
	while (__HAL_I2C_GET_FLAG(hi2c, Flag) == Status)
	{
		/* Check for the Timeout */
		if (Timeout != HAL_MAX_DELAY)
		{
			if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
			{
				hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
				hi2c->State = HAL_I2C_STATE_READY;
				hi2c->Mode = HAL_I2C_MODE_NONE;

				/* Process Unlocked */
				__HAL_UNLOCK(hi2c);
				return HAL_ERROR;
			}
		}
	}
	return HAL_OK;
}

/**
  * @brief  This function handles I2C Communication Timeout for specific usage of TXIS flag.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnTXISFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{
  while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXIS) == RESET)
  {
    /* Check if a NACK is detected */
    if (I2C_IsAcknowledgeFailed(hi2c, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
      {
        hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);

        return HAL_ERROR;
      }
    }
  }
  return HAL_OK;
}
#endif
/* private function */
static int get_instance_index(uint32_t instance)
{
	int index;

	switch (instance)
	{
		case (uint32_t)SP_I2C0_REG:
			index = 0;
			break;
		case (uint32_t)SP_I2C1_REG:
			index = 1;
			break;
		case (uint32_t)SP_I2C2_REG:
			index = 2;
			break;
		case (uint32_t)SP_I2C3_REG:
			index = 3;
			break;
	}
	return index;
}

static I2C_GDMA_TypeDef *get_gdma_index(uint32_t instance)
{
	I2C_GDMA_TypeDef *gdma;

	switch (instance)
	{
		case (uint32_t)SP_I2C0_REG:
			gdma = SP_GDMA0_REG;
			break;
		case (uint32_t)SP_I2C1_REG:
			gdma = SP_GDMA1_REG;
			break;
		case (uint32_t)SP_I2C2_REG:
			gdma = SP_GDMA2_REG;
			break;
		case (uint32_t)SP_I2C3_REG:
			gdma = SP_GDMA3_REG;
			break;
	}
	return gdma;
}

__STATIC_INLINE void _i2c_status_clear(I2C_HandleTypeDef *hi2c, uint32_t flag)
{
	hi2c->Instance->control1 |= flag;
	hi2c->Instance->control1 &= (~flag);
}

__STATIC_INLINE void _i2c_dma_int_flag_clear(I2C_HandleTypeDef *hi2c, uint32_t flag)
{
	hi2c->gdma->int_flag |= flag;
}

__STATIC_INLINE void _i2c_reset(I2C_HandleTypeDef *hi2c)
{
	hi2c->Instance->control0 |= I2C_CTL0_SW_RESET;
}

__STATIC_INLINE void _i2c_data0_set(I2C_HandleTypeDef *hi2c, uint32_t *wdata)
{
	hi2c->Instance->data00_03 = *wdata;
}

__STATIC_INLINE void _i2c_int_en0_disable(I2C_HandleTypeDef *hi2c, uint32_t int0)
{
	hi2c->Instance->int_en0 &= ~(int0);
}

__STATIC_INLINE void _i2c_rdata_flag_get(I2C_HandleTypeDef *hi2c,uint32_t *flag)
{
	*flag = hi2c->Instance->i2cm_status3;
}

static void _i2c_data_get(I2C_HandleTypeDef *hi2c, int index, uint32_t *rdata)
{
	switch (index) {
		case 0:
			*rdata = hi2c->Instance->data00_03;
			break;

		case 1:
			*rdata = hi2c->Instance->data04_07;
			break;

		case 2:
			*rdata = hi2c->Instance->data08_11;
			break;

		case 3:
			*rdata = hi2c->Instance->data12_15;
			break;

		case 4:
			*rdata = hi2c->Instance->data16_19;
			break;

		case 5:
			*rdata = hi2c->Instance->data20_23;
			break;

		case 6:
			*rdata = hi2c->Instance->data24_27;
			break;

		case 7:
			*rdata = hi2c->Instance->data28_31;
			break;

		default:
			break;
	}
}

__STATIC_INLINE void _i2c_rdata_flag_clear(I2C_HandleTypeDef *hi2c, uint32_t flag)
{
	hi2c->Instance->control6 = flag;
	hi2c->Instance->control6 = 0;
}

static void _i2c_clock_freq_set(I2C_HandleTypeDef *hi2c, uint32_t freq)
{
	uint32_t div;

	div = I2C_CLK_SOURCE_FREQ / freq;
	div -= 1;
	if (0 != I2C_CLK_SOURCE_FREQ % freq)
	{
		div += 1;
	}

	if (div > I2C_CTL2_FREQ_CUSTOM_MASK)
	{
		div = I2C_CTL2_FREQ_CUSTOM_MASK;
	}

	hi2c->Instance->control0 &= (~I2C_CTL0_FREQ(I2C_CTL0_FREQ_MASK));

	hi2c->Instance->control2 &= (~I2C_CTL2_FREQ_CUSTOM(I2C_CTL2_FREQ_CUSTOM_MASK));
	hi2c->Instance->control2 |= I2C_CTL2_FREQ_CUSTOM(div);

}

__STATIC_INLINE void _i2c_slave_addr_set(I2C_HandleTypeDef *hi2c, uint16_t slave_addr)
{
	uint16_t temp_addr = slave_addr & I2C_CTL0_SLAVE_ADDR_MASK;

	hi2c->Instance->control0 &= (~I2C_CTL0_SLAVE_ADDR(I2C_CTL0_SLAVE_ADDR_MASK));//clear previous slave addr
	hi2c->Instance->control0 |= I2C_CTL0_SLAVE_ADDR(temp_addr);
}

__STATIC_INLINE void _i2c_scl_delay_set(I2C_HandleTypeDef *hi2c, uint32_t delay)
{
	hi2c->Instance->control2 &= (~I2C_CTL2_SCL_DELAY(I2C_CTL2_SCL_DELAY_MASK));
	hi2c->Instance->control2 |= I2C_CTL2_SCL_DELAY(delay);

	hi2c->Instance->control2 &= (~(I2C_CTL2_SDA_HALF_ENABLE));
}

/* decide how many byte to write/read */
__STATIC_INLINE void _i2c_trans_cnt_set(I2C_HandleTypeDef *hi2c, uint16_t write_cnt, uint16_t read_cnt)
{
	uint32_t t_write = write_cnt & I2C_CTL7_RW_COUNT_MASK;
	uint32_t t_read = read_cnt & I2C_CTL7_RW_COUNT_MASK;

	hi2c->Instance->control7 = I2C_CTL7_WRCOUNT(t_write) | I2C_CTL7_RDCOUNT(t_read);
}

static void _i2c_active_mode_set(I2C_HandleTypeDef *hi2c, I2C_Active_Mode_e mode)
{
	hi2c->Instance->i2cm_mode &= (~(I2C_MODE_MANUAL_MODE | I2C_MODE_MANUAL_TRIG));
	switch (mode)
	{
		default:
		case I2C_TRIGGER:
			break;

		case I2C_AUTO:
			hi2c->Instance->i2cm_mode |= I2C_MODE_MANUAL_MODE;
			break;
	}
}

static void _i2c_data_set(I2C_HandleTypeDef *hi2c, uint32_t *wdata)
{
	hi2c->Instance->data00_03 = wdata[0];
	hi2c->Instance->data04_07 = wdata[1];
	hi2c->Instance->data08_11 = wdata[2];
	hi2c->Instance->data12_15 = wdata[3];
	hi2c->Instance->data16_19 = wdata[4];
	hi2c->Instance->data20_23 = wdata[5];
	hi2c->Instance->data24_27 = wdata[6];
	hi2c->Instance->data28_31 = wdata[7];
}

static void _i2c_rw_mode_set(I2C_HandleTypeDef *hi2c, I2C_RW_Mode_e rw_mode)
{
	switch(rw_mode) {
		default:
		case I2C_WRITE_MODE:
			hi2c->Instance->control0 &= (~(I2C_CTL0_PREFETCH | I2C_CTL0_RESTART_EN | I2C_CTL0_SUBADDR_EN));
			break;

		case I2C_READ_MODE:
			hi2c->Instance->control0 &= (~(I2C_CTL0_RESTART_EN | I2C_CTL0_SUBADDR_EN));
			hi2c->Instance->control0 |= I2C_CTL0_PREFETCH;
			break;

		case I2C_RESTART_MODE:
			hi2c->Instance->control0 |= (I2C_CTL0_PREFETCH | I2C_CTL0_RESTART_EN | I2C_CTL0_SUBADDR_EN);
			break;
	}
}
/* I2C interrupt enable */
__STATIC_INLINE void _i2c_int_en0_set(I2C_HandleTypeDef *hi2c, uint32_t int0)
{
	hi2c->Instance->int_en0 = int0;
}

/* I2C burst read interrupt enable */
__STATIC_INLINE void _i2c_int_en1_set(I2C_HandleTypeDef *hi2c, uint32_t int1)
{
	hi2c->Instance->int_en1 = int1; //rdata_en
}

/* I2C burst read the data buffer overflow interrupt enable */
__STATIC_INLINE void _i2c_int_en2_set(I2C_HandleTypeDef *hi2c, uint32_t int2)
{
	hi2c->Instance->int_en2 = int2;	//overflow_en
}

/* The most basic I2C module hardware initialization with MOON register */
__STATIC_INLINE void _i2c_enable(I2C_HandleTypeDef *hi2c)
{
	MODULE_REST->reset[3] = I2C_RESET(hi2c->Index, 0);
	CLK_EN->clock_enable[3] = I2C_CLKEN(hi2c->Index, 1);
	CLK_GATE->clock_gate_enable[3] = I2C_GCLKEN(hi2c->Index, 0);
}

/* Set the I2C master manual active when manual_mode is triggler mode */
__STATIC_INLINE void _i2c_manual_trigger(I2C_HandleTypeDef *hi2c)
{
	hi2c->Instance->i2cm_mode |= I2C_MODE_MANUAL_TRIG;
}

/* I2C mster ring fifo tmpty threshold control(sw control) */
__STATIC_INLINE void _i2c_int_en0_with_thershold_set(I2C_HandleTypeDef *hi2c, uint32_t int0, uint8_t threshold)
{
	hi2c->Instance->int_en0 = (int0 | I2C_EN0_CTL_EMPTY_THRESHOLD(threshold));
}

/* select dma mode */
__STATIC_INLINE void _i2c_dma_mode_enable(I2C_HandleTypeDef *hi2c)
{
	hi2c->Instance->i2cm_mode |= I2C_MODE_DMA_MODE;
}

__STATIC_INLINE void _i2c_dma_addr_set(I2C_HandleTypeDef *hi2c, uint32_t addr)
{
	hi2c->gdma->dma_addr = addr;
}

__STATIC_INLINE void _i2c_dma_length_set(I2C_HandleTypeDef *hi2c, uint32_t len)
{
	len &= (0xFFFF);  //only support 16 bit
	hi2c->gdma->dma_length = len;
}

static void _i2c_dma_rw_mode_set(I2C_HandleTypeDef *hi2c, I2C_DMA_RW_Mode_e mode)
{
	switch (mode)
	{
		default:
		case I2C_DMA_WRITE_MODE:
			hi2c->gdma->dma_config |= I2C_DMA_CFG_DMA_MODE;
			break;

		case I2C_DMA_READ_MODE:
			hi2c->gdma->dma_config &= (~I2C_DMA_CFG_DMA_MODE);
			break;
	}
}

__STATIC_INLINE void _i2c_dma_int_en_set(I2C_HandleTypeDef *hi2c, uint32_t int0)
{
	hi2c->gdma->int_en = int0;
}

__STATIC_INLINE void _i2c_dma_go_set(I2C_HandleTypeDef *hi2c)
{
	hi2c->gdma->dma_config |= I2C_DMA_CFG_DMA_GO;
}
