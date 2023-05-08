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
		NVIC_SetPriority(IPC_CA552CM4_INT0_IRQn, 0xE0); 
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
	reg_data *save_data = (reg_data *)PM_DATA_SAVE_ADDRESS; 
	memcpy((void *)save_data->reg_Sec_Group,(void *)RGST_SECURE_REG, sizeof(uint32_t)*32);
	memcpy((void *)save_data->reg_Sec_Main, (void *)SECGRP1_MAIN_REG,sizeof(uint32_t)*32);
	memcpy((void *)save_data->reg_Sec_PAI,  (void *)SECGRP1_PAI_REG, sizeof(uint32_t)*32);
	memcpy((void *)save_data->reg_Sec_PAII, (void *)SECGRP1_PAII_REG,sizeof(uint32_t)*32);
}
void pm_restore_data_after_ddr_retention(void)
{
	/* restore maindomain register data from CM4_Sram */
	reg_data *save_data = (reg_data *)PM_DATA_SAVE_ADDRESS; 
	memcpy((void *)RGST_SECURE_REG, (void *)save_data->reg_Sec_Group,sizeof(uint32_t)*32);
	memcpy((void *)SECGRP1_MAIN_REG,(void *)save_data->reg_Sec_Main, sizeof(uint32_t)*32);
	memcpy((void *)SECGRP1_PAI_REG, (void *)save_data->reg_Sec_PAI,  sizeof(uint32_t)*32);
	memcpy((void *)SECGRP1_PAII_REG,(void *)save_data->reg_Sec_PAII, sizeof(uint32_t)*32);
}
