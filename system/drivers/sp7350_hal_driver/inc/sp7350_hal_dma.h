#ifndef __SP7350_HAL_DMA_H
#define __SP7350_HAL_DMA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7350_hal_conf.h"
#include "sp7350xx.h"

#define HAL_DMA_TIMEOUT	3000000

/* peripheral id */
enum {
        PER_I2C0,
        PER_I2C1,
        PER_I2C2,
        PER_I2C3,
        PER_I2C4,
        PER_I2C5,
        PER_I2C6,
        PER_I2C7,

        PER_I2C8,
        PER_I2C9,
        PER_SPI_CB0,
        PER_SPI_CB1,
        PER_SPI_CB2,
        PER_SPI_CB3,
        PER_SPI_CB4,
        PER_SPI_CB5,

        PER_MAX
};

typedef void (*dma_callback_t)(uint32_t err);

/**
  * @brief  Starts the DMA Transfer.
  * @param  src:	The source memory buffer address (4-byte aligned) or peripheral id
  * @param  dst:	The destination memory buffer address (4-byte aligned) or peripheral id
  * @param  len:	The length of data to be transferred from source to destination,
  *			In interrupt mode, this param can't exceeds MAX_BLK_SIZE(16380)
  * @param  callback:	NULL - polling mode
  *			Not NULL - The transfer complete callback function for interrupt mode
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Start(uint32_t src, uint32_t dst, uint32_t len, dma_callback_t callback);

void HAL_DMA_Test(void);

#ifdef __cplusplus
}
#endif

#endif
