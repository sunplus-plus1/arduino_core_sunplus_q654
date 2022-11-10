
#ifdef SP7021
#include "sp7021_hal_irq_ctrl.c"
#elif defined(SP645)
#include "sp645_hal_cortex.c"
#include "sp645_hal_irq_ctrl.c"
#elif defined(SP7350)
#include "sp7350_hal_cortex.c"
#include "sp7350_hal_irq_ctrl.c"
#endif