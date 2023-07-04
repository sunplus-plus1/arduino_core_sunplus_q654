#ifndef __PM_PRIVATE_H__
#define __PM_PRIVATE_H__

#include <FreeRTOS.h>
#include<FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>

#include "Arduino.h"

#ifdef SP7350
#include "sp7350_hal_conf.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//#define PM_ZEBU_TEST   /* test in zebu, power down/up by uart */


#define MAIN_DOMAIN_PU_ACK_IRQ			(0)
#define MAIN_DOMAIN_PD_ACK_IRQ			(1)

#define PMC_RESET_VECTOR_ADDRESS		(0x100000)

#define PMC_WAIT_DDR_RETENTION_DONE_BIT (15)  /*G36.28 [15] ddr retention done statue */
#define PMC_CA55_TOP_RESET_STATUS_BIT   (12)  /*G36.28 [14:12] check ca55 top reset status */

#define TIMEOUT_MS						(10UL)

#define CA55_2_CM4_IRQ_INT0				(9)

/* PMIC */
#define PWR_NPU_CONTROL_PIN             (65)
#define PWR_VCL_CONTROL_PIN             (66)
#define MAIN_DOMAIN_CONTROL_PIN         (67)

#define MAIN_POWER_OFF		0x80
#define MAIN_POWER_ON		0x81

#define NPU_VCL_POWER_OFF	0x90
#define NPU_VCL_POWER_ON	0x91

#define BLOCKA_POWER_OFF	0xA0
#define BLOCKA_POWER_ON		0xA1

#define CA55_POWER_OFF		0xB0
#define CA55_0D7V_POWER     0xB1
#define CA55_0D8V_POWER     0xB2
#define CA55_1V_POWER       0xB3

/*wakeup key*/
#define WAKEUP_KEY_PIN		(90)
#define SHORT_PRESS         (1000)  /* short time: press time between 1s and 4.5s */
#define LONG_PRESS          (5000)  /* long time: press time between 5s and 8.5s */
#define OFFSET_PRESS        (3500)

extern volatile int	deep_sleep;
extern volatile int	in_suspend;
void wakeup_key_init(void);

/* power manager */
extern SemaphoreHandle_t xPowerDown_Semaphore;
void pm_set_power_down_bit(void);
void pm_set_retention_done_bit(void);
void Send_Cmd_To_PMIC(uint32_t cmd);
void vDoPowerdownTask( void *pvParameters );
void vDoPowerupTask( void *pvParameters );
void pm_save_data_before_ddr_retention(void);
void pm_restore_data_after_ddr_retention(void);
/* save data */
#define PM_DATA_SAVE_ADDRESS   0xFA29E000   /* Save the maindomain register and User data */

#define RGST_SECURE_REG       RF_GRP(502,0)
#define SECGRP1_MAIN_REG      RF_GRP(113,0)
#define SECGRP1_PAI_REG       RF_GRP(114,0)
#define SECGRP1_PAII_REG      RF_GRP(115,0)

typedef struct{
	uint32_t reg_Sec_Main[32];
	uint32_t reg_Sec_PAI[32];
	uint32_t reg_Sec_PAII[32];
	uint32_t reg_Sec_Group[32];
}reg_data;

/* ddr retention */
#define CM4_SRAM_RET_ADDRESS    0xFA29F000 /* Save the configuration info of ddr */
#define UMCTL2_REG_Base	   	    0xF8169000
#define DWC_PHY_REG_Base        0xF9000000

void ddr_retention_save(void);
void ddr_retention_load(void);

#ifdef __cplusplus
}
#endif
#endif
