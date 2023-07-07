#include "pm_common.h"
#include "pmic.h"

#define PM_TASK_STACK_SIZE				(configMINIMAL_STACK_SIZE)

SemaphoreHandle_t xPowerDown_Semaphore;


#ifdef PM_ZEBU_TEST
#define UART_LSR_TX_RDY2     (1 << 0)
#define UART_REG2            SP_UART1
#define UART_tx_rdy2()       (UART_REG2->lsr & UART_LSR_TX_RDY2)
#define UART_putc_nw2(c)     (UART_REG2->dr = (c))
#define UART_wait2()         do { while (!UART_tx_rdy2()) ;} while (0)
#define UART_putc2(c)        do { UART_wait2(); UART_putc_nw2(c); } while (0)
#endif

void Send_Cmd_To_PMIC(uint32_t cmd)
{
#ifdef PM_ZEBU_TEST
	UART_putc2(cmd);
#else
	pmic_do_cmd(cmd);
#endif
}

void vCA55_TO_CM4_Mailbox_ISR()
{
	static BaseType_t xHigherPriorityTaskWoken;
	int value = CA55_TO_CM4_MAILBOX->direct_transation[1];
	value = CA55_TO_CM4_MAILBOX->direct_transation[0];
	value = value ;

	if(xSemaphoreGiveFromISR( xPowerDown_Semaphore, &xHigherPriorityTaskWoken ) != pdTRUE)
	{
		printf("Semaphore Send ISR Fail!\r\n");
	}

	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void pm_main()
{

	xPowerDown_Semaphore = xSemaphoreCreateBinary( );

	wakeup_key_init(); /* init wakeup key */
	pmic_init(); /* pmic init */
	if( xPowerDown_Semaphore != NULL )
	{
		/* CM4 power down after CA55 into idle state*/
		NVIC_SetVector(IPC_CA552CM4_INT0_IRQn, (uint32_t)vCA55_TO_CM4_Mailbox_ISR); 
		NVIC_EnableIRQ(IPC_CA552CM4_INT0_IRQn);
		NVIC_SetPriority(IPC_CA552CM4_INT0_IRQn, 0x8); 
		xTaskCreate( vDoPowerdownTask, "powerdown", PM_TASK_STACK_SIZE, NULL, 1, NULL );
		xTaskCreate( vDoPowerupTask, "powerup", PM_TASK_STACK_SIZE, NULL, 1, NULL );
	}

//	CM4_TO_CA55_MAILBOX->direct_transation[7]=0xaabb1234;  // mailbox to ca55 to in suspend mode, test for replace wakeupkey

}

void pm_set_power_down_bit(void)
{
	/* used for romcode */
	PMC_REGS->pmc_ca55_reset_state = (0x1<<PMC_WAIT_DDR_RETENTION_DONE_BIT) | (0x1<<PMC_CA55_TOP_RESET_STATUS_BIT); // [15]=1,[14:12]=1 ca55 reset and wait ddr retention
	PMC_REGS->pmc_reset_vector = PMC_RESET_VECTOR_ADDRESS;
}

void pm_set_retention_done_bit(void)
{
	/* set ddr retention done bit [15]*/
	PMC_REGS->pmc_ca55_reset_state = 0x0; // [15]=0
}

void pm_save_data_before_ddr_retention(void)
{
	/* save maindomain register data to CM4_Sram */

}
void pm_restore_data_after_ddr_retention(void)
{
  // mantis #10541
#if 1
	for(int i = 0; i < 32;i++ )
	{
		// rewrite MOON0 ~ MOON5 register data. write through to main domain register. 
		MOON0_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON0_REG->sft_cfg[i]);
		MOON1_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON1_REG->sft_cfg[i]);
		MOON2_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON2_REG->sft_cfg[i]);
		MOON4_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON4_REG->sft_cfg[i]);
		MOON5_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON5_REG->sft_cfg[i]);
	}
	for(int i = 0; i < 32;i++ )
	{
		if(i < 5 || (i >=20 && i <=22)) //PLLA/S used for AO domain. 
			continue;
		MOON3_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON3_REG->sft_cfg[i]);
	}
#else
	//// MOON0
	for(int i = 0; i < 32;i++ )
	{
		if((i==6)||(i==13)|| (i==14)){
			printf("G0.%d\n",i);
			MOON0_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON0_REG->sft_cfg[i]);
			}
	}

	//// MOON1
	for(int i = 0; i < 32;i++ )
	{
		if((i >= 0 && i <= 2) || (i >=6 && i <=10)){
			printf("G1.%d\n",i);
			MOON1_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON1_REG->sft_cfg[i]);
			}
	}

	//// MOON3
	for(int i = 0; i < 32;i++ )
	{
		if((i >=5 && i <=19) || (i >=23 && i <=25) || (i >=28 && i <=30)){
			printf("G3.%d\n",i);
			MOON3_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON3_REG->sft_cfg[i]);
			}
	}

	//// MOON4
	for(int i = 0; i < 32;i++ )
	{
		if((i >=0 && i <= 12) || (i==14)|| (i==15)|| (i==23)|| (i==24) || (i==27)){
			printf("G4.%d\n",i);
			MOON4_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON4_REG->sft_cfg[i]);
			}
	}

	//// MOON5
	for(int i = 0; i < 32;i++ )
	{
		if((i >=0 && i <= 5) || (i >=13 && i <=19)|| (i==22)|| (i==23)){
			printf("G5.%d\n",i);
			MOON5_REG->sft_cfg[i] = RF_MASK_V(0xFFFF,MOON5_REG->sft_cfg[i]);
			}
	}
#endif
}
