#include "core_debug.h"
#include "Arduino.h"

#ifdef SP7021
#include "sp7021_hal_conf.h"
#include "sp7021_ll_spi.h"
#elif defined(SP645)
#include "sp645_hal_conf.h"
#include "sp645_ll_spi.h"
#endif

#include "spi_com.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private Functions */
/**
  * @brief  return clock freq of an SPI instance
  * @param  spi_inst : SPI instance
  * @retval clock freq of the instance else SystemCoreClock
  */
uint32_t spi_getClkFreqInst(SPI_TypeDef *spi_inst)
{
    return 0;
}


uint32_t spi_getClkFreq(spi_t *obj)
{
    return 0;
}

/**
  * @brief  SPI initialization function
  * @param  obj : pointer to spi_t structure
  * @param  speed : spi output speed
  * @param  mode : one of the spi modes
  * @param  msb : set to 1 in msb first
  * @retval None
  */
void spi_init(spi_t *obj, uint32_t speed, spi_mode_e mode, uint8_t msb)
{
    uint32_t spi_mosi,spi_miso,spi_sclk,spi_ssel;
    
    if (obj == NULL) {
      return;
    }
   
    SPI_HandleTypeDef *handle = &(obj->handle);

#if SPI_SEL_INSTANCE == 0
    handle->Instance  = SPI0;
#elif SPI_SEL_INSTANCE == 1
   handle->Instance  = SPI1;
#elif SPI_SEL_INSTANCE == 2
    handle->Instance  = SPI2;
#elif SPI_SEL_INSTANCE == 3
    handle->Instance  = SPI3;
#ifdef SP645
#elif SPI_SEL_INSTANCE == 4
    handle->Instance  = SPI4;
#elif SPI_SEL_INSTANCE == 5
    handle->Instance  = SPI5;
#endif 
#endif 

    handle->Init.spiclk             = speed;
    handle->Init.spi_mode           = mode;

    if (msb == 0) {
      handle->Init.FirstBit         = SPI_FIRSTBIT_LSB;
    } else {
      handle->Init.FirstBit         = SPI_FIRSTBIT_MSB;
    }

#ifdef SP7021
    /* Configure SPI GPIO pins  gpio to pinmux set */
    spi_mosi = GPIO_TO_PINMUX(obj->pin_mosi);
    if(!IS_VALID_PINMUX(spi_mosi))
    {
      core_debug("ERROR: [SPI] MOSI pin error!\n");
      return;
    }
    spi_miso = GPIO_TO_PINMUX(obj->pin_miso);
    if(!IS_VALID_PINMUX(spi_miso))
    {
      core_debug("ERROR: [SPI] MISO pin error!\n");
      return;
    }
    spi_sclk = GPIO_TO_PINMUX(obj->pin_sclk);
    if(!IS_VALID_PINMUX(spi_sclk))
    {
      core_debug("ERROR: [SPI] clk pin error!\n");
      return;
    }
    if (obj->pin_ssel != -1)
    {
        spi_ssel = GPIO_TO_PINMUX(obj->pin_ssel);
        if(!IS_VALID_PINMUX(spi_ssel))
        {
          core_debug("ERROR: [SPI] SSEL pin error!\n");
          return;
        }
    }
    if (SPI_SEL_INSTANCE >= 0 && SPI_SEL_INSTANCE <= 3)
    {
        HAL_Module_Clock_enable(SPI_COMBO0 + SPI_SEL_INSTANCE, 1);
        HAL_Module_Clock_gate(SPI_COMBO0 + SPI_SEL_INSTANCE, 1);
        HAL_Module_Reset(SPI_COMBO0 + SPI_SEL_INSTANCE, 0);

        HAL_PINMUX_Cfg(PINMUX_SPIM0_CLK + 5*SPI_SEL_INSTANCE,spi_sclk);
        HAL_PINMUX_Cfg(PINMUX_SPIM0_DO + 5*SPI_SEL_INSTANCE ,spi_mosi);
        HAL_PINMUX_Cfg(PINMUX_SPIM0_DI + 5*SPI_SEL_INSTANCE,spi_miso);
        if (obj->pin_ssel != -1)
        HAL_PINMUX_Cfg(PINMUX_SPIM0_EN + 5*SPI_SEL_INSTANCE,spi_ssel);
    }
#elif defined(SP645)
    if (SPI_SEL_INSTANCE >=0 && SPI_SEL_INSTANCE <= 5)
    {
        HAL_Module_Clock_enable(SPICOMBO0 + SPI_SEL_INSTANCE, 1);
        HAL_Module_Clock_gate(SPICOMBO0 + SPI_SEL_INSTANCE, 1);
        HAL_Module_Reset(SPICOMBO0 + SPI_SEL_INSTANCE, 0);
        if(SPI_SEL_INSTANCE == 0)
            HAL_PINMUX_Cfg(PINMUX_SPI_COM0_MST,1);
        else
            HAL_PINMUX_Cfg(PINMUX_SPI_COM1_MST + 2*(SPI_SEL_INSTANCE - 1),1);
    }
#endif

    HAL_SPI_Init(handle);
}

/**
  * @brief This function is implemented to deinitialize the SPI interface
  *        (IOs + SPI block)
  * @param  obj : pointer to spi_t structure
  * @retval None
  */
void spi_deinit(spi_t *obj)
{
    if (obj == NULL) {
      return;
    }

    SPI_HandleTypeDef *handle = &(obj->handle);

    HAL_SPI_DeInit(handle);
#ifdef SP7021
    if (SPI_SEL_INSTANCE >= 0 && SPI_SEL_INSTANCE <= 3)
    {
        HAL_Module_Clock_enable(SPI_COMBO0 + SPI_SEL_INSTANCE, 0);
        HAL_Module_Clock_gate(SPI_COMBO0 + SPI_SEL_INSTANCE, 0);
    }
#elif defined(SP645)
    if (SPI_SEL_INSTANCE >=0 && SPI_SEL_INSTANCE <= 5)
    {
        HAL_Module_Clock_enable(SPICOMBO0 + SPI_SEL_INSTANCE, 0);
        HAL_Module_Clock_gate(SPICOMBO0 + SPI_SEL_INSTANCE, 0);
    }
#endif
}


spi_status_e spi_send(spi_t *obj, uint8_t *Data, uint16_t len, uint32_t Timeout)
{
    return spi_transfer(obj, Data, Data, len, Timeout, 1 /* SPI_TRANSMITONLY */);
}


spi_status_e spi_transfer(spi_t *obj, uint8_t *tx_buffer, uint8_t *rx_buffer,
                          uint16_t len, uint32_t Timeout, uint8_t skipReceive)
{
    spi_status_e ret = SPI_OK;
    uint32_t tickstart, size = len;
    volatile SPI_TypeDef *_SPI = obj->handle.Instance;

    if ((obj == NULL) || (len == 0) || (Timeout == 0U)) {
      return Timeout > 0U ? SPI_ERROR : SPI_TIMEOUT;
    }

    return HAL_SPI_TransmitReceive(&obj->handle,tx_buffer,rx_buffer,len,1,Timeout);

#if 0
    tickstart = HAL_GetTick();

    /* spi config set */
    LL_SPI_Config_Set(_SPI,obj->handle.Init.spiclk,obj->handle.Init.spi_mode,obj->handle.Init.FirstBit);

    /* Start transfer */
    LL_SPI_SetTransferSize(_SPI, size);
    LL_SPI_StartMasterTransfer(_SPI);

    while (size--) {
      while (!LL_SPI_IsActiveFlag_TXE(_SPI));
      LL_SPI_TransmitData8(_SPI, *tx_buffer++);

      if (!skipReceive) {
        while (!LL_SPI_IsActiveFlag_RXNE(_SPI));
        *rx_buffer++ = LL_SPI_ReceiveData8(_SPI);
      }
      if ((Timeout != HAL_MAX_DELAY) && (HAL_GetTick() - tickstart >= Timeout)) {
        ret = SPI_TIMEOUT;
        break;
      }
    }
    return ret;
#endif 
}

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
