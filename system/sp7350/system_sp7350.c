#include "sp7350xx.h"
#include "sp7350_hal_conf.h"
#include "mem_sp7350.h"


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
	HAL_Init();
}


void SystemCoreClockUpdate (void)
{


}


