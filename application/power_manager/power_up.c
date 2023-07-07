#include "pm_common.h"

SemaphoreHandle_t xPowerUp_Semaphore;
extern volatile int do_npu_power ;
extern volatile int do_vcl_power ;

void _power_up_main_domain(void)
{
	Send_Cmd_To_PMIC(MAIN_POWER_ON);
	/*  power up finish */
	PMC_REGS->pmc_main_pwr_ctrl = 0x1;
	delay(50);
}

void _power_up_CA55_domain(void)
{

	Send_Cmd_To_PMIC(CA55_0D8V_POWER);
	delay(50);
	MOON0_REG->sft_cfg[1] = RF_MASK_V_SET(0x7E);  ///maindomain powerup will reset CA55
/// core0
	PMC_REGS->pmc_corepsw_en   &= 0xFFFFFFFE ;
	PMC_REGS->pmc_coremem_sden &= 0xFFFFFFFE ;
	PMC_REGS->pmc_iso_en       &= 0xFFFFFFFE ;
/// core1
	PMC_REGS->pmc_corepsw_en   &= 0xFFFFFFFD ;
	PMC_REGS->pmc_coremem_sden &= 0xFFFFFFFD ;
	PMC_REGS->pmc_iso_en       &= 0xFFFFFFFD ;
/// core2
	PMC_REGS->pmc_corepsw_en   &= 0xFFFFFFFB ;
	PMC_REGS->pmc_coremem_sden &= 0xFFFFFFFB ;
	PMC_REGS->pmc_iso_en       &= 0xFFFFFFFB ;
/// core3
	PMC_REGS->pmc_corepsw_en   &= 0xFFFFFFF7 ;
	PMC_REGS->pmc_coremem_sden &= 0xFFFFFFF7 ;
	PMC_REGS->pmc_iso_en       &= 0xFFFFFFF7 ;

	PMC_REGS->pmc_pctl_reg     |= 0x000003FF ; 

	PMC_REGS->pmc_lvs_pwd = 0x00AA55FF ;  /// LVS password
	PMC_REGS->pmc_lvs_disable = 0x0 ;     /// LVS disable
	MOON0_REG->sft_cfg[1] = RF_MASK_V_CLR(0x7E);//0x007E0000 ;  ///de-assert CA55 reset
}

void _power_up_npu_vcl_domain()
{
	if(do_npu_power){
		Send_Cmd_To_PMIC(NPU_POWER_ON);
	}
	if(do_vcl_power){
		Send_Cmd_To_PMIC(VCL_POWER_ON);
	}
	delay(50);
	if(do_npu_power){
		PMC_REGS->pmc_iso_pwd		= 0xFFAA5500;
		PMC_REGS->pmc_iso_en		&= 0xFFFFFFEF ;
		MOON0_REG->sft_cfg[7] = RF_MASK_V_CLR(0x4007);
	}

	if(do_vcl_power){
		PMC_REGS->pmc_iso_pwd		= 0xFFAA5500;
		PMC_REGS->pmc_iso_en		&= 0xFFFFFFDF ;
		MOON0_REG->sft_cfg[2] = RF_MASK_V_CLR(0x1FC0);
	}
}

void Main_Domain_PowerUP(void)
{
	_power_up_main_domain();

	_power_up_CA55_domain();

	_power_up_npu_vcl_domain();
	ddr_retention_load();

	pm_restore_data_after_ddr_retention();
	pm_set_retention_done_bit();
	in_suspend = 0;
}

/* trigger by send 0x82 cmd to PMIC */
void Main_Domain_PowerUP_REQ_Handler()
{
	static BaseType_t xHigherPriorityTaskWoken;
	printf("Power Up ISR ! \n");
	Main_Domain_PowerUP();

	/*semap will hang until ca55 restart.*/
	if(xSemaphoreGiveFromISR( xPowerUp_Semaphore, &xHigherPriorityTaskWoken ) != pdTRUE)
	{
		printf("Semaphore Send ISR Fail! \n");
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void power_up_maindomain(void)
{
	/* register interrupt */
	xPowerUp_Semaphore = xSemaphoreCreateBinary( );
	if(xPowerUp_Semaphore != NULL)
	{
		NVIC_SetVector(MAIN_PWR_UP_REQ_IRQn, (uint32_t)Main_Domain_PowerUP_REQ_Handler);
		NVIC_EnableIRQ(MAIN_PWR_UP_REQ_IRQn);
		NVIC_SetPriority(MAIN_PWR_UP_REQ_IRQn, 0x8); 
	}
}

void vDoPowerupTask( void *pvParameters )
{
	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	power_up_maindomain();

	for( ;; )
	{
		if( xPowerUp_Semaphore != NULL )
		{


			if(xSemaphoreTake( xPowerUp_Semaphore, portMAX_DELAY) == pdTRUE)
			{
				//Main_Domain_PowerUP();
			}
		}
	}
}
