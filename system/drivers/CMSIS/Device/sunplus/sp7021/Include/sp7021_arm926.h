
#ifndef __SP7021_ARM926_H
#define __SP7021_ARM926_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/**
  * @brief Bit position definition inside a 32 bits registers
  */
#define  B(x) \
        ((uint32_t) 1 << x)

typedef enum IRQn
 {
                                */
 /******  SP7021 Q628 specific Interrupt Numbers ************************************************************************/
   TEGN_INIT_FIELD_START_IRQn  = 0,     /*!< TEGN START                                           			      */
   TEGN_INIT_FIELD_END_IRQn    = 1,     /*!< TEGN END                                    						  */ 
   RESERVED_2 				   = 2, 	/*!< RESERVED interrupt                                                   */
   RESERVED_3         		   = 3, 	/*!< RESERVED interrupt                                                   */
   TEGN_INI1_USER_IRQn         = 4,     /*!< TEGN For USER                              						  */
   TEGN_INI2_USER_IRQn         = 5,     /*!< TEGN For USER         												  */
   RESERVED_6                  = 6,     /*!< RESERVED interrupt                                                   */
   RESERVED_7                  = 7,     /*!< RESERVED interrupt                                                   */
   RESERVED_8                  = 8,     /*!< RESERVED interrupt                                                   */
   RESERVED_9                  = 9,     /*!< RESERVED interrupt                                                   */
   USBC1_OTG_IRQn              = 10,    /*!< USB Controller 1 OTG interrupt                                      */
   USBC0_OTG_IRQn              = 11,    /*!< USB Controller 0 OTG interrupt                                      */
   RESERVED_12                 = 12,    /*!< RESERVED interrupt                                                  */
   USBC0_DEVICE_IRQn           = 13,    /*!< USB Controller 0 Device Interrupt                                   */
   USBC0_EHCI_IRQn             = 14,    /*!< USB Controller 0 EHCI Interrupt                                     */
   USBC0_OHCI_IRQn             = 15,    /*!< USB Controller 0 OHCI Interrupt                                     */
   USBC1_DEVICE_IRQn		   = 16,	/*!< USB Controller 1 Device Interrupt								   */
   USBC1_EHCI_IRQn 			   = 17,	/*!< USB Controller 1 EHCI Interrupt									   */
   USBC1_OHCI_IRQn 			   = 18,	/*!< USB Controller 1 OHCI Interrupt									   */
   RESERVED_19                 = 19,    /*!< RESERVED interrupt                                                  */
   EMCC_IRQn                   = 20,    /*!< eMMC interrupt                                                  */
   SDCARD_IRQn                 = 21,    /*!< SD Card interrupt                                                  */
   SDIO_IRQn                   = 22,    /*!< SDIO interrupt                                                  */
   RESERVED_23                 = 23,    /*!< RESERVED interrupt                                                  */
   RESERVED_24                 = 24,    /*!< RESERVED interrupt                                                  */
   RBUS_IRQn                   = 25,    /*!< RBUS interrupt                                                  */
   NOC_IRQn                    = 26,    /*!< NoC interrupt                                                  */
   RESERVED_27                 = 27,    /*!< RESERVED interrupt                                                  */
   SDPORT_IRQn                 = 28,    /*!< Security for protect address                                                  */
   PI_EXTI0_IRQn               = 29,     /*!< EXTI Line0 from FPGA(PIO) Interrupt                                                 */
   PI_EXTI1_IRQn               = 30,     /*!< EXTI Line1  from FPGA(PIO) Interrupt                                                 */
   RESERVED_31                 = 31,    /*!< RESERVED interrupt                                                  */
   RESERVED_32                 = 32,    /*!< RESERVED interrupt                                                  */
   RESERVED_33                 = 33,    /*!< RESERVED interrupt                                                  */
   RESERVED_34                 = 34,    /*!< RESERVED interrupt                                                  */
   RESERVED_35                 = 35,    /*!< RESERVED interrupt                                                  */
   UPHY0_IRQn                  = 36,    /*!< USB PHY 0 interrupt                                                  */
   UPHY1_IRQn                  = 37,     /*!< USB PHY 0 Interrupt                                                 */
   RESERVED_38                 = 38,    /*!< RESERVED interrupt                                                  */
   PFCNT_SENSOR_IRQn           = 39,    /*!< P-chip Sensor interrupt                                                  */
   RESERVED_40                 = 40,    /*!< RESERVED interrupt                                                  */
   IOP_INT0_IRQn               = 41,    /*!< IOP INT0 interrupt                                                  */
   IOP_INT1_IRQn               = 42,    /*!< IOP INT0 interrupt                                                  */
   RESERVED_43                 = 43,    /*!< RESERVED interrupt                                                  */
   RESERVED_44                 = 44,    /*!< RESERVED interrupt                                                  */
   RESERVED_45                 = 45,    /*!< RESERVED interrupt                                                  */
   REMOVE_MBUS_IRQn            = 46,    /*!< MBUS Remove interrupt                                                  */   
   RESERVED_47                 = 47,    /*!< RESERVED interrupt                                                  */
   RESERVED_48                 = 48,    /*!< RESERVED interrupt                                                  */
   CSI0_INT_FIELD_START_IRQn   = 49,    /*!< MIPI CSI0 FIELD Start interrupt                                                  */
   CSI0_INT_FIELD_END_IRQn     = 50,    /*!< MIPI CSI0 FIELD End interrupt                                                  */
   CSI1_INT_FIELD_START_IRQn   = 51,    /*!< MIPI CSI1 FIELD Start interrupt                                                  */
   CSI1_INT_FIELD_END_IRQn     = 52,    /*!< MIPI CSI1 FIELD End interrupt                                                  */
   UART0_IRQn                  = 53,    /*!< UART0 interrupt                                                  */
   UART1_IRQn                  = 54,    /*!< UART1 interrupt                                                  */
   UART2_IRQn                  = 55,    /*!< UART2 interrupt                                                  */
   UART3_IRQn                  = 56,    /*!< UART3 interrupt                                                  */
   RESERVED_57                 = 57,    /*!< RESERVED interrupt                                                  */
   BCH_IRQ_IRQn                = 58,    /*!< SPI Nand BCH interrupt                                                  */
   LOSD_IRQn                   = 59,    /*!< Audio LOSD interrupt                                                  */
   AUD_FIFO_IRQn               = 60,    /*!< Audio FIFO interrupt                                                  */
   RESERVED_61                 = 61,    /*!< RESERVED interrupt                                                  */
   RESERVED_62                 = 62,    /*!< RESERVED interrupt                                                  */
   RESERVED_63                 = 63,    /*!< RESERVED interrupt                                                  */
   RESERVED_64                 = 64,    /*!< RESERVED interrupt                                                  */
   RESERVED_65                 = 65,    /*!< RESERVED interrupt                                                  */
   L2SW_IRQn                   = 66,    /*!< Ethernet Switch interrupt                                                  */
   SPI_COMBO1_DMA_W_IRQn       = 67,    /*!< SPI Combo1 DMA write interrupt                                                  */
   SPI_SLAVE1_IRQn             = 68,    /*!< SPI Slave1 interrupt                                                  */
   SPI_MASTER1_IRQn            = 69,    /*!< SPI Master1 interrupt                                                  */
   SPI_COMBO2_DMA_W_IRQn       = 70,    /*!< SPI Combo2 DMA write interrupt                                                  */
   SPI_SLAVE2_IRQn             = 71,    /*!< SPI Slave2 interrupt                                                  */
   SPI_MASTER2_IRQn            = 72,    /*!< SPI Master2 interrupt                                                  */
   SPI_COMBO3_DMA_W_IRQn	   = 73,	 /*!< SPI Combo3 DMA write interrupt												  */
   SPI_SLAVE3_IRQn 			   = 74,	 /*!< SPI Slave3 interrupt													*/
   SPI_MASTER3_IRQn			   = 75,	 /*!< SPI Master3 interrupt 												 */
   RESERVED_76                 = 76,    /*!< RESERVED interrupt                                                  */	
   RESERVED_77                 = 77,    /*!< RESERVED interrupt                                                  */	
   RESERVED_78                 = 78,    /*!< RESERVED interrupt                                                  */	
   RESERVED_79                 = 79,    /*!< RESERVED interrupt                                                  */	
   RESERVED_80                 = 80,    /*!< RESERVED interrupt                                                  */	
   RESERVED_81                 = 81,    /*!< RESERVED interrupt                                                  */	
   RESERVED_82                 = 82,    /*!< RESERVED interrupt                                                  */	
   RESERVED_83                 = 83,    /*!< RESERVED interrupt                                                  */	
   RESERVED_84                 = 84,    /*!< RESERVED interrupt                                                  */	
   RESERVED_85                 = 85,    /*!< RESERVED interrupt                                                  */	
   RESERVED_86                 = 86,    /*!< RESERVED interrupt                                                  */	
   RESERVED_87                 = 87,    /*!< RESERVED interrupt                                                  */	
   RESERVED_88                 = 88,    /*!< RESERVED interrupt                                                  */	
   RESERVED_89                 = 89,    /*!< RESERVED interrupt                                                  */	
   RESERVED_90                 = 90,    /*!< RESERVED interrupt                                                  */	
   RESERVED_91                 = 91,    /*!< RESERVED interrupt                                                  */	
   ICM0_IRQn                   = 92,    /*!< Input capture 0 interrupt                                                  */	
   ICM1_IRQn                   = 93,    /*!< Input capture 1 interrupt                                                  */	
   ICM2_IRQn                   = 94,    /*!< Input capture 2 interrupt                                                  */	
   ICM3_IRQn                   = 95,    /*!< Input capture 3 interrupt                                                  */	
   RESERVED_96                 = 96,    /*!< RESERVED interrupt                                                  */	
   RESERVED_97                 = 97,    /*!< RESERVED interrupt                                                  */	
   MSI_UART_DMA0_IRQn          = 98,    /*!< MSI UART DMA0 interrupt                                                  */	
   MSI_UART_DMA1_IRQn          = 99,    /*!< MSI UART DMA1 interrupt                                                  */	
   RESERVED_100                = 100,    /*!< RESERVED interrupt                                                  */	
   RESERVED_101                = 101,    /*!< RESERVED interrupt                                                  */	
   RESERVED_102                = 102,    /*!< RESERVED interrupt                                                  */	
   AXI_MON_IRQn                = 103,    /*!< AXI Clobal Monitor interrupt                                                  */	
   RESERVED_104                = 104,    /*!< RESERVED interrupt                                                  */	
   HDMI_TX_IRQn                = 105,    /*!< HDMI TX interrupt                                                  */	
   RESERVED_106                = 106,    /*!< RESERVED interrupt                                                  */	
   RESERVED_107                = 107,    /*!< RESERVED interrupt                                                  */	
   RESERVED_108                = 108,    /*!< RESERVED interrupt                                                  */	
   RESERVED_109                = 109,    /*!< RESERVED interrupt                                                  */	
   RESERVED_110                = 110,    /*!< RESERVED interrupt                                                  */	
   RESERVED_111                = 111,    /*!< RESERVED interrupt                                                  */	
   RESERVED_112                = 112,    /*!< RESERVED interrupt                                                  */	
   RESERVED_113                = 113,    /*!< RESERVED interrupt                                                  */	
   RESERVED_114                = 114,    /*!< RESERVED interrupt                                                  */	
   RESERVED_115                = 115,    /*!< RESERVED interrupt                                                  */	
   RESERVED_116                = 116,    /*!< RESERVED interrupt                                                  */	
   RESERVED_117                = 117,    /*!< RESERVED interrupt                                                  */	
   RESERVED_118                = 118,    /*!< RESERVED interrupt                                                  */	
   RESERVED_119                = 119,    /*!< RESERVED interrupt                                                  */	
   EXTI0_IRQn                  = 120,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI1_IRQn                  = 121,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI2_IRQn                  = 122,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI3_IRQn                  = 123,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI4_IRQn                  = 124,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI5_IRQn                  = 125,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI6_IRQn                  = 126,    /*!< GPIO Externl  interrupt                                                  */	
   EXTI7_IRQn                  = 127,    /*!< GPIO Externl  interrupt                                                  */	
   DMA0_IRQn                   = 128,    /*!< CBDMA0  interrupt                                                  */	
   DMA1_IRQn                   = 129,    /*!< CBDMA1  interrupt                                                  */	
   RESERVED_130                = 130,    /*!< RESERVED interrupt                                                  */	
   RESERVED_131                = 131,    /*!< RESERVED interrupt                                                  */	
   RESERVED_132                = 132,    /*!< RESERVED interrupt                                                  */	
   RESERVED_133                = 133,    /*!< RESERVED interrupt                                                  */	
   UART4_IRQn                  = 134,    /*!< UART4 interrupt                                                  */	
   RESERVED_135                = 135,    /*!< RESERVED interrupt                                                  */	
   RESERVED_136                = 136,    /*!< RESERVED interrupt                                                  */	
   RESERVED_137                = 137,    /*!< RESERVED interrupt                                                  */	
   UART_DMA0_IRQn              = 138,    /*!< UART DMA0  interrupt                                                  */	
   UART_DMA1_IRQn              = 139,    /*!< UART DMA1  interrupt                                                  */	
   RESERVED_140                = 140,    /*!< RESERVED interrupt                                                  */	
   RESERVED_141                = 141,    /*!< RESERVED interrupt                                                  */	
   UADBG_IRQn                  = 142,    /*!< UART to AXI  interrupt                                                  */	
   SPI_FLASH_IRQn              = 143,    /*!< SPI Flash  interrupt                                                  */	
   SPI_COMBO0_DMA_W_IRQn	   = 144,	 /*!< SPI Combo0 DMA write interrupt												  */
   SPI_SLAVE0_IRQn 			   = 145,	 /*!< SPI Slave0 interrupt													*/
   SPI_MASTER0_IRQn			   = 146,	 /*!< SPI Master0 interrupt 												 */
   RESERVED_147                = 147,    /*!< RESERVED interrupt                                                  */	
   SECURIYT_IRQn               = 148,    /*!< Securiyt interrupt                                                  */	
   SPI_NAND_IRQn               = 149,    /*!< SPI Nand interrupt                                                  */	
   STC_TIMER_W_IRQn            = 150,    /*!< STC TIMER W interrupt                                                  */	
   STC_TIMER0_IRQn             = 151,    /*!< STC TIMER 0 interrupt                                                  */	
   STC_TIMER1_IRQn             = 152,    /*!< STC TIMER 1 interrupt                                                  */	
   STC_TIMER2_IRQn             = 153,    /*!< STC TIMER 2 interrupt                                                  */	
   STC_TIMER3_IRQn             = 154,    /*!< STC TIMER 2 interrupt                                                  */	
   STC_AV0_TIMER0_IRQn 		   = 155,	  /*!< STC TIMER 0 interrupt												  */ 
   STC_AV0_TIMER1_IRQn 		   = 156,	  /*!< STC TIMER 1 interrupt												  */ 
   STC_AV0_TIMER2_IRQn 		   = 157,	  /*!< STC TIMER 2 interrupt												  */ 
   STC_AV0_TIMER3_IRQn 		   = 158,	  /*!< STC TIMER 2 interrupt												  */ 
   STC_AV1_TIMER0_IRQn 		   = 159,	   /*!< STC TIMER 0 interrupt												   */ 
   STC_AV1_TIMER1_IRQn 		   = 160,	   /*!< STC TIMER 1 interrupt												   */ 
   STC_AV1_TIMER2_IRQn 		   = 161,	   /*!< STC TIMER 2 interrupt												   */ 
   STC_AV1_TIMER3_IRQn 		   = 162,	   /*!< STC TIMER 2 interrupt												   */ 
   RTC_IRQn 		           = 163,	   /*!< RTC TIMER  interrupt												   */ 							
   STC_AV2_TIMER0_IRQn 		   = 164,		/*!< STC TIMER 0 interrupt													*/ 
   STC_AV2_TIMER1_IRQn 		   = 165,		/*!< STC TIMER 1 interrupt													*/ 
   STC_AV2_TIMER2_IRQn 		   = 166,		/*!< STC TIMER 2 interrupt													*/ 
   STC_AV2_TIMER3_IRQn 		   = 167,		/*!< STC TIMER 2 interrupt													*/ 
   STC_AV0_TIMER_W_IRQn		   = 168,	  /*!< STC TIMER W interrupt													*/
   RESERVED_169                = 169,    /*!< RESERVED interrupt                                                  */	
   RESERVED_170                = 170    /*!< RESERVED interrupt                                                  */	
   RESERVED_171                = 171,    /*!< RESERVED interrupt                                                  */	
   RESERVED_172                = 172,    /*!< RESERVED interrupt                                                  */	
   RESERVED_173                = 173,    /*!< RESERVED interrupt                                                  */	
   I2C_MASTER0_IRQ             = 174,    /*!< I2C 0  interrupt                                                  */	
   I2C_MASTER1_IRQ             = 175,    /*!< I2C 1  interrupt                                                  */	
   I2C_MASTER2_IRQ             = 176,    /*!< I2C 2  interrupt                                                  */	
   I2C_MASTER3_IRQ             = 177,    /*!< I2C 3  interrupt                                                  */	
   I2C_DDC0_IRQ                = 178,    /*!< I2C DDC0 interrupt                                                  */	
   RESERVED_179                = 179,    /*!< RESERVED interrupt                                                  */	
   RESERVED_180                = 180,    /*!< RESERVED interrupt                                                  */	
   RESERVED_181                = 181,    /*!< RESERVED interrupt                                                  */	
   C_CHIP_INT0_IRQn            = 182,    /*!< C Chip Direct  interrupt                                                  */	
   C_CHIP_INT1_IRQn            = 183,    /*!< C Chip Direct interrupt                                                  */	  
   C_CHIP_INT2_IRQn            = 184,    /*!< C Chip Direct interrupt                                                  */	  
   C_CHIP_INT3_IRQn            = 185,    /*!< C Chip Direct interrupt                                                  */	  
   C_CHIP_INT4_IRQn            = 186,    /*!< C Chip Direct interrupt                                                  */	  
   C_CHIP_INT5_IRQn            = 187,    /*!< C Chip Direct interrupt                                                  */	  
   C_CHIP_INT6_IRQn            = 188,    /*!< C Chip Direct interrupt                                                  */	  
   C_CHIP_INT7_IRQn            = 189,    /*!< C Chip Direct interrupt                                                  */	  
   p_CHIP_INT0_IRQn            = 190,    /*!< P Chip to C chip interrupt                                                  */	
   p_CHIP_INT1_IRQn			   = 192,	  /*!< P Chip to C chip interrupt 												 */  
   P_CHIP_INT2_IRQn			   = 193,	  /*!< P Chip to C chip interrupt													*/	
   P_CHIP_INT3_IRQn			   = 194,	  /*!< P Chip to C chip interrupt													*/	
   P_CHIP_INT4_IRQn			   = 195,	  /*!< P Chip to C chip interrupt													*/	
   P_CHIP_INT5_IRQn			   = 196,	  /*!< P Chip to C chip interrupt													*/	
   P_CHIP_INT6_IRQn			   = 197,	  /*!< P Chip to C chip interrupt													*/	
   P_CHIP_INT7_IRQn			   = 198,	  /*!< P Chip to C chip interrupt													*/	
   C_CHIP_CPU0_IRQn			   = 199,	  /*!< C Chip Direct interrupt													*/		
   P_CHIP_IRQn			       = 200,	  /*!< C Chip Direct interrupt													*/		
   MAX_IRQ_n

 } IRQn_Type;

 
#define __FPU_PRESENT             0       
#define __TIM_PRESENT 			  0		  
#define __GIC_PRESENT             0
#define __L2C_PRESENT	          0

#define IRQ_CTRL_BASE    0x9C000480

#include "core_armv5.h"

#include <stdint.h>


typedef struct
{
  __IOM uint32_t type[7];              /*!< \brief  interrupt type 0:level-type, 1: edge-type */
  __IOM uint32_t polarity[7];          /*!< \brief  interrupt polarity 0:high-active(default) 1:low-active*/
  __IOM uint32_t priority[7];          /*!< \brief  interrupt piority 0:fiq(default), 1:irq */
  __IOM  uint32_t mask[7];              /*!< \brief  mask interrupt bit 0:masked(default) 1:pass */
  RESERVED(1[4], uint32_t)
  	__OM uint32_t clear[7]; 		   /*!< \brief  clear interrupt bit */
  __IM uint32_t masked_fiq[7];		   /*!< \brief  read masked fiq bit */
  __IM uint32_t masked_irq[7];		   /*!< \brief  read masked irq bit */
  __IOM uint32_t dsp_mask[7];		   /*!< \brief  mask dsp interrupt bit 0:masked(default) 1:pass */
  RESERVED(2[3], uint32_t)
  __IM uint32_t group; 
}  IRQ_Ctrl_Type;

#define SP_IRQ_CTRL  (*(IRQ_Ctrl_Type*) IRQ_CTRL_BASE)




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SP7021_ARM926_H */


