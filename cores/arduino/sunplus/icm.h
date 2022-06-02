#ifndef ICM_H_
#define ICM_H_

#ifdef SP7021

#include "sp7021_hal_icm.h"
#include "sp7021_hal_irq_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

void ICM_Initialization(void);
void ICM0_IRQHandler(void);
void ICM1_IRQHandler(void);
void ICM2_IRQHandler(void);
void ICM3_IRQHandler(void);

#endif


#ifdef __cplusplus
}
#endif

#endif

