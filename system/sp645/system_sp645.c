#include "sp64xx.h"
#include "sp645_hal_conf.h"
#include "mem_sp645.h"


uint32_t SystemCoreClock = HSI_VALUE;



static inline void *hal_memcpy(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = (uint8_t*)dest;
    uint8_t *psrc = (uint8_t*)src;

	while(n--){
        *pdest++ = *psrc++;
    }

	return dest;
}

void SystemInit (void)
{
	/*system  init*/

}


void SystemCoreClockUpdate (void)
{
	//SystemCoreClock = HAL_PLL_Get_ARM926Freq();

}


