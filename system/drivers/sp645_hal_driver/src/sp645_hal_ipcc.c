#include "sp645_hal_ipcc.h"


#ifdef __cplusplus
extern "C" {
#endif


HAL_StatusTypeDef HAL_IPCC_Init(IPCC_HandleTypeDef *hipcc)
{
	int i = 0;  
	IRQn_Type irqn =  MAX_IRQ_n;
	if (hipcc == NULL)
  	{
    	return HAL_ERROR;
  	}

	assert_param(IS_TIM_INSTANCE(hipcc->Instance));
	for (i=0; i < CHN_MAX; i++)
	{
		if (hipcc->szIRQHDL[i] != NULL)
		{
			irqn = IPC_CM42CA55_INT0_IRQn + i - DIRECT_CHN_0;
			IRQ_SetHandler(irqn, hipcc->szIRQHDL[i]);
			IRQ_Enable(irqn);
		}
	}
	return HAL_OK;
}

HAL_StatusTypeDef HAL_IPCC_DeInit(IPCC_HandleTypeDef *hipcc)
{
	int i = 0;  
	IRQn_Type irqn =  MAX_IRQ_n;
	if (hipcc == NULL)
  	{
    	return HAL_ERROR;
  	}

	assert_param(IS_TIM_INSTANCE(hipcc->Instance));
	for (i=0; i < CHN_MAX; i++)
	{
		irqn = IPC_CM42CA55_INT0_IRQn + i - DIRECT_CHN_0;
		IRQ_Disable(irqn);
	}
	return HAL_OK;
}

HAL_StatusTypeDef HAL_IPCC_UrgentNoify(IPCC_HandleTypeDef *hipcc, IPPC_CHN_type enChn, uint32_t data)
{
	uint32_t mask = 0;
	if (hipcc == NULL)
	{
		return HAL_ERROR;
	}
	assert_param(IS_TIM_INSTANCE(hipcc->Instance));

	if (enChn > DIRECT_CHN_7)
		return HAL_ERROR;

	mask = 1 <<(DIRECT_LOCK_DATA0_Pos+enChn);
	if (hipcc->Instance->M2C_over_write_monitor_status & mask)	
		return HAL_BUSY;
	hipcc->Instance->M2C_direct_data[enChn] = data;

	return HAL_OK;

}


HAL_StatusTypeDef HAL_IPCC_Noify(IPCC_HandleTypeDef *hipcc, uint32_t* pData, uint32_t seek, uint32_t size )
{
	uint32_t mask = 0;
	int i = 0;
	if ((hipcc == NULL) || (pData == NULL))
	{
		return HAL_ERROR;
	}

	if ((seek + size) > NORMAL_DATA_SIZE)
		return HAL_ERROR;

	assert_param(IS_TIM_INSTANCE(hipcc->Instance));

	for (i=0; i < size; i++)
	{
		mask += 1 <<(seek+i+NORMAL_LOCK_DATA0_Pos);
	}
	
	if (hipcc->Instance->M2C_over_write_monitor_status & mask)	
		return HAL_BUSY;

	memcpy((void *)(hipcc->Instance->M2C_normal_data+seek), pData, size);

	hipcc->Instance->M2C_normal_trigger = 1;
	return HAL_OK;

}


#ifdef __cplusplus
}
#endif


