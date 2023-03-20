#ifndef __sp7350_HAL_PLL_H
#define __sp7350_HAL_PLL_H

#include "sp7350xx.h"
#include "sp7350_cm4.h"
#include "sp7350_hal_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __HAL_PLL_GET_SYS_CLK_SEL() (READ_BIT(MOON3_REG->sft_cfg[28], GENMASK(8, 6)) >> 6)

#define __HAL_PLL_GET_SYS_CM4_SEL() (READ_BIT(MOON3_REG->sft_cfg[27], GENMASK(4, 2)) >> 2)

uint32_t HAL_PLL_GetSystemFreq(void);
uint32_t HAL_PLL_GetCortexM4Freq(void);

#ifdef __cplusplus
}
#endif

#endif
