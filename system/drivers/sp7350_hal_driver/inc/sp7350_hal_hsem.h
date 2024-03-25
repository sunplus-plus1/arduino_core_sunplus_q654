#ifndef __SP7350_HAL_HSEM_H
#define __SP7350_HAL_HSEM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7350_hal_conf.h"
#include "sp7350xx.h"

#define HSEM_TOTAL_NUM					   (16)

#define IS_ADC_CHAN_VALID(num)              (((num) >= ADC_CHAN_0) && ((num) <= ADC_CHAN_3))

#define RLEASE_SEMAPHORE                   (0x554E4C4B)    /* free */

HAL_StatusTypeDef HAL_Hsem_Take(int lock_index);
uint32_t HAL_HSEM_IsSemTaken(uint32_t lock_index);
void HAL_Hsem_Release(int lock_index);
void HAL_Hsem_Release_All(void);

#ifdef __cplusplus
}
#endif

#endif

