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
	SystemCoreClockUpdate();

	HAL_Init();
}


void SystemCoreClockUpdate (void)
{
		
	uint8_t val = READ_BIT(MOON2_REG->sft_cfg[3], CM4_FREQ_Msk)>>CM4_FREQ_Pos;

	switch(val){
		case 0:
			SystemCoreClock = HSI_VALUE;
		break;

		case 1:
			SystemCoreClock = HSI_VALUE/2;
		break;

		case 2:
			SystemCoreClock = HSI_VALUE/4;
		break;

		default:
			SystemCoreClock = HSI_VALUE;
			break;
			
	}
	

}


