#ifndef __sp645_HAL_PLL_H
#define __sp645_HAL_PLL_H

#include "sp64xx.h"
#include "sp645_cm4.h"
#include "sp645_hal_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __HAL_PLL_GET_SYS_CLK_SEL() (READ_BIT(MOON2_REG->sft_cfg[0], GENMASK(1, 0)) >> 0)

#define __HAL_PLL_GET_SYS_DIV() (READ_BIT(MOON2_REG->sft_cfg[0], GENMASK(5, 2)) >> 2)

#define __HAL_PLL_GET_SYS_CM4_DIV() (READ_BIT(MOON2_REG->sft_cfg[3], GENMASK(6, 5)) >> 5)

#define __HAL_PLL_SET_SYS_CM4_DIV(VAL) (WRITE_REG(MOON2_REG->sft_cfg[3], (VAL << 5) | (GENMASK(6, 5) << 16)))

uint32_t HAL_PLL_GetSystemFreq(void);
uint32_t HAL_PLL_GetCortexM4Freq(void);
void HAL_PLL_SetCortexM4Freq(uint8_t val);

#ifdef __cplusplus
}
#endif

#endif
