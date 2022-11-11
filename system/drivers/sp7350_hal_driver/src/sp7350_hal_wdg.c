#include "sp7350_hal_wdg.h"
#if 0
HAL_StatusTypeDef HAL_WDG_Init(WDG_HandleTypeDef *hwdg)
{
	MODULE_ID_Type id;
	IRQn_Type irq;

	/* Check the WDG handle allocation */
	if (hwdg == NULL)
	{
		return HAL_ERROR;
	}

	/* Enable clock */
	HAL_Module_Clock_enable(id , 1);
	HAL_Module_Clock_gate(id , 1);
	HAL_Module_Reset(id , 0);

	HAL_WDG_STOP(hwdg->Instance);
	HAL_WDG_CNTMAX(hwdg->Instance);
	HAL_WDG_CLRF(hwdg->Instance);

	//while(1);
	/* 4.18 bit[9] MO1_RI_WATCHDOG_RST_EN */
	MOON4_REG->sft_cfg[18] = 0x02000200;

	if (hwdg->Instance == WDG0)
	{
		//printf("%d %s\n", __LINE__, __FUNCTION__);
		id = STC_0;
		irq = STC_TIMER_W_IRQn;
		MOON4_REG->sft_cfg[18] = 0x04000400;

	}
	else if (hwdg->Instance == WDG1)
	{
		id = STC_AV1;
		irq = STC_AV0_TIMER_W_IRQn;
		MOON4_REG->sft_cfg[18] = 0x10001000;
	}
	else if (hwdg->Instance == WDG2)
	{
		id = STC_AV2;
		irq = STC_AV1_TIMER_W_IRQn;
		MOON4_REG->sft_cfg[18] = 0x20002000;
	}

	/* Config irq */
	IRQ_SetHandler(irq, hwdg->IrqHandle);
	IRQ_Enable(irq);

	/* Return function status */
	return HAL_OK;
}
#endif
