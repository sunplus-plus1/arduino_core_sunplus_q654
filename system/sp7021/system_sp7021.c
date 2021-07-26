#include "sp70xx.h"


uint32_t SystemCoreClock = HSI_VALUE;
extern uint8_t __vectors_start[];
extern uint8_t __vectors_end[];

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
#if 1
	L1C_DisableCaches();
	__set_TLBIALL(0);
  	__set_ICIALLU(0);
	L1C_InvalidateDCacheAll();
	MMU_CreateTranslationTable();
	MMU_Enable();
	//Enable Caches
	L1C_EnableCaches();
	
#endif	
   /*Interrupt  vector init*/	
    hal_memcpy(0x00000000, __vectors_start, (unsigned)__vectors_end - (unsigned)__vectors_start);
	IRQ_Initialize();
}


void SystemCoreClockUpdate (void)
{
	SystemCoreClock = HAL_PLL_Get_ARM926Freq();

}


