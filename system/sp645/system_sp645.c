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
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
        SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
	SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
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


