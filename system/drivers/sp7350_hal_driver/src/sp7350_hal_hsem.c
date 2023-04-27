#include "sp7350_hal_hsem.h"

#define HSEM_R_UNLOCK                      (0)     /* free */
#define HSEM_R_LOCK                        (1)     /* locked */

#define RLEASE_SEMAPHORE                   (0x554E4C4B)    /* free */

volatile int hsem_status[HSEM_TOTAL_NUM] = {0};
HAL_StatusTypeDef HAL_Hsem_Take(int lock_index)
{
	if(lock_index >= HSEM_TOTAL_NUM)
		return HAL_ERROR;

	if((hsem_status[lock_index] == 0) && (HSEM_REG_AO->lock[lock_index] == HSEM_R_UNLOCK))
	{
		hsem_status[lock_index] = 1;
		return HAL_OK;
	}
	return HAL_ERROR;
}

uint32_t HAL_HSEM_IsSemTaken(uint32_t lock_index)
{
	if(lock_index >= HSEM_TOTAL_NUM)
		return 1;

  return hsem_status[lock_index];
}

void HAL_Hsem_Release(int lock_index)
{
	if(lock_index >= HSEM_TOTAL_NUM)
		return;

	HSEM_REG_AO->lock[lock_index] = RLEASE_SEMAPHORE;
	hsem_status[lock_index] = 0;
}

void HAL_Hsem_Release_All(void)
{
	for(int i = 0; i < HSEM_TOTAL_NUM; i++)
	{
		HSEM_REG_AO->lock[i] = RLEASE_SEMAPHORE;
		hsem_status[i] = 0;
	}
}

