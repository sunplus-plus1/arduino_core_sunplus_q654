
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
   STC_TIMER_W_IRQn            = 150,    /*!< STC Wathcdog interrupt                                                  */	
   STC_TIMER0_IRQn             = 151,    /*!< STC TIMER 0 interrupt                                                  */	
   STC_TIMER1_IRQn             = 152,    /*!< STC TIMER 1 interrupt                                                  */	
   STC_TIMER2_IRQn             = 153,    /*!< STC TIMER 2 interrupt                                                  */	
   STC_TIMER3_IRQn             = 154,    /*!< STC TIMER 3 interrupt                                                  */	
   STC_AV0_TIMER0_IRQn 		   = 155,	  /*!< STC TIMER 0 interrupt												  */ 
   STC_AV0_TIMER1_IRQn 		   = 156,	  /*!< STC TIMER 1 interrupt												  */ 
   STC_AV0_TIMER2_IRQn 		   = 157,	  /*!< STC TIMER 2 interrupt												  */ 
   STC_AV0_TIMER3_IRQn 		   = 158,	  /*!< STC TIMER 3 interrupt												  */ 
   STC_AV1_TIMER0_IRQn 		   = 159,	   /*!< STC TIMER 0 interrupt												   */ 
   STC_AV1_TIMER1_IRQn 		   = 160,	   /*!< STC TIMER 1 interrupt												   */ 
   STC_AV1_TIMER2_IRQn 		   = 161,	   /*!< STC TIMER 2 interrupt												   */ 
   STC_AV1_TIMER3_IRQn 		   = 162,	   /*!< STC TIMER 3 interrupt												   */ 
   RTC_IRQn 		           = 163,	   /*!< RTC TIMER  interrupt												   */ 							
   STC_AV2_TIMER0_IRQn 		   = 164,		/*!< STC TIMER 0 interrupt													*/ 
   STC_AV2_TIMER1_IRQn 		   = 165,		/*!< STC TIMER 1 interrupt													*/ 
   STC_AV2_TIMER2_IRQn 		   = 166,		/*!< STC TIMER 2 interrupt													*/ 
   STC_AV2_TIMER3_IRQn 		   = 167,		/*!< STC TIMER 3 interrupt													*/ 
   STC_AV0_TIMER_W_IRQn		   = 168,	  /*!< STC TWatchdog interrupt													*/
   RESERVED_169                = 169,    /*!< RESERVED interrupt                                                  */	
   RESERVED_170                = 170,    /*!< RESERVED interrupt                                                  */	
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


typedef enum {
	/*group 0*/
	SYSTEM 			= 0,							/* !< System bus ID*/
	RESERVED0 		= 1,
	RTC				= 2,							/* !< RTC moudle ID */
	IOCTL 			= 3,							/* !< IO Ctrl module ID */ 
	IOP				= 4,							/* !< IOP module ID */ 
	OTPRX			= 5, 							/* !< OTP RX moudle ID */
	NOC				= 6, 							/* !< NOC moudle ID */
	BR				= 7, 							/* !< BootROM Wrap moudle ID */
	RBUS_L00		= 8,							/* !< RBUS Level 0 moudle ID */ 
	SPI_FLASH		= 9, 							/* !< SPI Flash moudle ID */
	SDCTRL0			= 10, 							/* !< SD Ctrl moudle ID */
	PERI0			= 11, 							/* !< PREI0 moudle ID */
	RESERVED1		= 12,
	ARM926			= 13, 							/* !< ARM926 moudle ID */			
	UMCTL2			= 14, 							/* !< UMCTRL2 moudle ID */
	PERI1			= 15, 							/* !< PERI1 moudle ID */
	/*group 1*/
	DDR_PHY0		= 16, 							/* !< DDR PHY moudle ID */
	RESERVED2		= 17, 
	C_CHIP			= 18,							/* !< For C Chip module ID */
	RESERVED3		= 19, 									
	STC0			= 20, 							/* !< Standard time clock 0 moudle ID */
	STC_AV0			= 21, 							/* !< standard time clock AV0 moudle ID */
	STC_AV1			= 22, 							/* !< standard time clock AV1 moudle ID */
	STC_AV2			= 23, 							/* !< standard time clock AV2 module ID */
	UART0			= 24, 							/* !< UART0 moudle ID */
	UART1 			= 25, 							/* !< UART1 moudle ID */
	UART2			= 26, 							/* !< UART2 moudle ID */ 
	UART3 			= 27, 							/* !< UART3 module ID */
	UART4			= 28, 							/* !< UART4 moudle ID */
	HWUA			= 29, 							/* !< Hardware uart moudle ID */
	DDC0			= 30, 							/* !< DDC0 moudle ID */
	UADMA			= 31, 
	/*group 2*/
	CBDMA0			= 32, 							/* !< CBDMA0 module ID*/
	CBDMA1			= 33,							/* !< CBDMA1 module ID*/
	SPI_COMBO0		= 34, 							/* !< SPI Combo 0 moudle ID */
	SPI_COMBO1		= 35, 							/* !< SPI Combo 1 moudle ID */
	SPI_COMBO2		= 36, 							/* !< SPI Combo 2 moudle ID */
	SPI_COMBO3		= 37, 							/* !< SPI Combo 3 moudle ID */
	AUD				= 38, 							/* !< Audio moudle ID	*/
	RESERVED4       = 39, 
	RESERVED5		= 40, 
	RESERVED6		= 41, 							
	USBCTRL0		= 42, 							/* !< USB Controller 0 moudle ID */	
	USBCTRL1		= 43,							/* !< USB Controller 1 moudle ID */
	RESERVED7		= 44, 						
	USBPHY0			= 45, 							/* !< USB Phy 0 moudle ID */
	USBPHY1			= 46,							/* !< USB Phy 1 moudle ID */
	RESERVED8		= 47,
	/*group 3*/
	I2CM0			= 48, 							/* !< I2C Master0 moudle ID */
	I2CM1			= 49, 							/* !< I2C Master1 moudle ID */
	I2CM2			= 50, 							/* !< I2C Master2 moudle ID */
	I2CM3			= 51, 							/* !< I2C Master3 moudle ID */
	RESERVED9		= 52,
	RESERVED10		= 53,
	RESERVED11		= 54,
	RESERVED12		= 55,
	RESERVED13		= 56,
	RESERVED14		= 57,
	RESERVED15		= 58,
	RESERVED16		= 59,
	RESERVED17		= 60,
	PMC_QCHANEL		= 61,							/* !< PMC Q-chanel moudle ID */
	CARD_CTL0		= 62,							/* !< SD Card Controller moudle ID */
	CARD_CTL1		= 63,							/* !< SD Card Controller moudle ID */
	/*group 4*/
	RESERVED18		= 64,
	RESERVED19		= 65,						
	CARD_CTL4		= 66,							/* !< SD Card Controller moudle ID */					
	RESERVED20		= 67,
	BCH				= 68,							/* !< BCH for spi nand moudle ID */
	RESERVED21		= 69,
	RESERVED22		= 70,
	RESERVED23		= 71,
	RESERVED24		= 72,
	RESERVED25		= 73,
	RESERVED26		= 74,
	DDFCH			= 75,							/* !< DDFCH module ID */
	CSIIW0			= 76, 							/* !< CSI IW 0 moudle ID */
	CSIIW1			= 77, 							/* !< CSI IW 0 moudle ID */
	MIPICSI0 		= 78, 							/* !< MIPICSI 0 moudle ID */
	MIPICSI1		= 79, 							/* !< MIPICSI 1 moudle ID */
	/*group 5*/
	HDMI_TX			= 80, 							/* !< HDMI TX moudle ID */
	RESERVED27		= 81, 
	RESERVED28		= 82,
	RESERVED29		= 83,
	RESERVED30		= 84,
	VPOST			= 85, 							/* !< Video POST moudle ID*/
	RESERVED31		= 86,
	RESERVED32		= 87,
	RESERVED33		= 88,
	RESERVED34		= 89,
	RESERVED35		= 90,
	RESERVED36		= 91,
	RESERVED37		= 92,
	RESERVED38		= 93,
	RESERVED39		= 94,
	RESERVED40		= 95,
	/*group 6*/
	TGEN			= 96, 							/* !< TGEN module ID */
	DMIX			= 97, 							/* !< DMIX moudle ID */
	RESERVED41		= 98,
	RESERVED42		= 99,
	RESERVED43		= 100,
	RESERVED44		= 101,
	RESERVED45		= 102,
	RESERVED46		= 103,
	RESERVED47		= 104,
	RESERVED48		= 105,
	TCON			= 106,							/* !< TCON moudle ID */
	RESERVED49		= 107,
	RESERVED50		= 108,
	RESERVED51		= 109,
	RESERVED52		= 110,
	IRQCTRL			= 111,
	/*group 7*/
	RGST			= 112, 							/* !< Register bus moudle ID */
	RESERVED53		= 113,
	RESERVED54		= 114,
	GPIO			= 115, 							/* !< GPIO moudle ID */
	RBUS_TOP		= 116, 							/* !< Register top bus moudle ID */
	RESERVED55		= 117,
	RESERVED56		= 118,
	RESERVED57		= 119,
	RESERVED58		= 120,
	RESERVED59		= 121,
	RESERVED60		= 122,
	RESERVED61		= 123,
	RESERVED62		= 124,
	RESERVED63		= 125,
	RESERVED64		= 126,
	RESERVED65		= 127,
	/*group 8*/
	RESERVED66		= 128,
	RESERVED67		= 129,
	RESERVED68		= 130,
	RESERVED69		= 131,
	RESERVED70		= 132,
	RESERVED71		= 133,
	MAILBOX			= 134, 							/* !< Mailbox module ID */
	RESERVED72		= 135,
	RESERVED73		= 136,
	RESERVED74		= 137,
	SPI_NAND		= 138, 							/* !< SPI Nand moudle ID */
	I2C2CBUS		= 139, 							/* !< I2C to Bus moudule ID */
	GDMA			= 140,							/* !< GDMA module ID */
	SECURIYT		= 141, 							/* !< Securiyt module ID */
	DVE				= 142, 							/* !< DVE moudle ID */
	GPOST0			= 143, 							/* !< GPOST0 module ID */
	/*group 9*/
	OSD0 			= 144, 							/* !< OSD 0 module ID */
	RESERVED75		= 145,
	PWM				= 146,							/* !< PWM moudle ID */
	UADBG			= 147,							/* !< UART Debug module ID */
	DUMMY_MASTER	= 148,							/* !< Dummy master module ID */
	FIO_CTRL		= 149, 							/* !< FIO controller module ID */
	FPGA			= 150,							/* !< FPGA controller module ID */
	L2SW			= 151, 							/* !< Ethernet switch module ID */
	ICM				= 152, 							/* !< Input capture module ID */
	AXI_GLOBAL		= 153,							/* !< AXI moniter module ID */
	RESERVED76		= 154,	
	RESERVED77		= 155,
	RESERVED78		= 156,
	RESERVED79		= 157,
	RESERVED80		= 158,
	RESERVED81		= 159,
	
}MODULE_ID_Type;
 
#define __FPU_PRESENT             0       
#define __TIM_PRESENT 			  0		  
#define __GIC_PRESENT             0
#define __L2C_PRESENT	          0

#define IRQ_CTRL_BASE    0x9C000480

#include "core_armv5.h"

#include <stdint.h>




typedef struct{
	__IOM uint32_t clock_enable[10];				/*!< \brief devices clock enable bit*/
}Module_Clock_En_type;

typedef struct{
	__IOM uint32_t clock_gate_enable[10];			/*!< \brief devices clock gate enable bit*/
}Module_Clock_Gate_Type;

typedef struct{
	__IOM uint32_t reset[10];			/*!< \brief devices clock gate enable bit*/
}Module_Reset_Type;



typedef struct{
	__IM uint32_t stamp_id; 						   /*!< \brief chip reversion and stamp id*/
	__IOM uint32_t clock_enable[10]; 				/*!< \brief devices clock enable bit*/
	__IOM uint32_t clock_gate_enable[10];			/*!< \brief devices clock gate enable bit*/
	__IOM uint32_t reset[10];						   /*!< \brief devices reset bit*/
	__IOM uint32_t hw_config;							/*!< \brief hardware config for boot mode*/
}GROUP0_Type;

typedef struct{
	__IM uint32_t test_mode_config; 				  /*!< \brief chip test mode config*/
	__IOM uint32_t pinmux_cfg[4]; 				  /*!< \brief pinmux config */
	RESERVED(0[17], uint32_t)                    /*!< reserved */
}GROUP1_Type;

typedef struct{
   __IOM uint32_t pinmux_net[11];             /*!< \brief ethernet pinmux config */
   __IOM uint32_t pinmux_sdio[3];             /*!< \brief sdio pinmux config */
   __IOM uint32_t pinmux_pwm[4];              /*!< \brief pwm pinmux config */
   __IOM uint32_t pinmux_icm_data[2];         /*!< \brief icm data pinmux config */
   __IOM uint32_t pinmux_icm_clk[2];          /*!< \brief icm clk pinmux config */
   __IOM uint32_t pinmux_spim[10];            /*!< \brief spi master pinmux config */
}GROUP2_Type;

typedef struct{
   __IOM uint32_t pinmux_spis[10];             /*!< \brief spi salver pinmux config */
   __IOM uint32_t pinmux_i2cm[4];              /*!< \brief i2cm pinmux config */
   __IOM uint32_t pinmux_ua1[2];               /*!< \brief uart1 pinmux config */
   __IOM uint32_t pinmux_ua2[2];               /*!< \brief uart2 pinmux config */
   __IOM uint32_t pinmux_ua3[2];               /*!< \brief uart3 pinmux config */
   __IOM uint32_t pinmux_ua4[2];               /*!< \brief uart4 pinmux config */
   __IOM uint32_t pinmux_timer_int[2];         /*!< \brief timer interrupt master pinmux config */
   __IOM uint32_t pinmux_gpio_int[4];          /*!< \brief gpio interrupt pinmux config */
   RESERVED(0[14], uint32_t)                    /*!< reserved */
}GROUP3_Type;

typedef struct
{
  __IOM uint32_t type[7];              /*!< \brief  interrupt type 0:level-type, 1: edge-type */
  __IOM uint32_t polarity[7];          /*!< \brief  interrupt polarity 0:high-active(default) 1:low-active*/
  __IOM uint32_t priority[7];          /*!< \brief  interrupt piority 0:fiq(default), 1:irq */
  __IOM  uint32_t mask[7];              /*!< \brief  mask interrupt bit 0:masked(default) 1:pass */
  RESERVED(0[4], uint32_t)
  	__OM uint32_t clear[7]; 		   /*!< \brief  clear interrupt bit */
  __IM uint32_t masked_fiq[7];		   /*!< \brief  read masked fiq bit */
  __IM uint32_t masked_irq[7];		   /*!< \brief  read masked irq bit */
  __IOM uint32_t dsp_mask[7];		   /*!< \brief  mask dsp interrupt bit 0:masked(default) 1:pass */
  RESERVED(1[3], uint32_t)
  __IM uint32_t group; 
}IRQ_Ctrl_Type;

typedef struct{
	__IOM uint32_t stc_15_0;				 /*!< \brief  standard time clock counter, 0~15 bit */
	__IOM uint32_t stc_16_31;				 /*!< \brief  standard time clock counter, 16~31 bit */
	__IOM uint32_t stc_64;					 /*!< \brief  standard time clock counter, the MSB 64 bit, when write the bit ,clear the stc counter at once */
	__IOM uint32_t stc_prescale_val;		
	RESERVED(0[3], uint32_t)
	__IOM uint32_t stc_config;	
	RESERVED(1[6], uint32_t)
	__IOM uint32_t stc_47_32;	
	__IOM uint32_t stc_48_63;
}STC_TypeDef;


typedef struct 
{
	__IOM uint32_t control;   			/*!< \brief timer control register for >*/
	__IOM uint32_t prescale_val;			/*!< \brief 16-bit prescale for timer>*/
	__IOM uint32_t reload_val;			/*!< \brief when count reaches to 0, the value will reload to the count >*/
	__IOM uint32_t counter_val;					/*!< \brief 16-bit counter , it down to 0, the timer will generate the interrupt*/
}TIM_TypeDef;


#define SP_IRQ_CTRL  ((IRQ_Ctrl_Type*) IRQ_CTRL_BASE)


#define DRAM_BASE 			0x00000000
#define EXTERNAL_ROM_BASE 	0x98000000
#define PERIPH_BASE 		0x9C000000
#define RUBS_BASE			(PERIPH_BASE+0)
#define AXI_BUS_BASE		(PERIPH_BASE+0x101000)
#define INTERNEL_ROM_BASE 	0x9E000000
#define SRAM_BASE 			0x9E800000

#define RF_GRP(_grp, _reg) ((((_grp) * 32 + (_reg)) * 4) + RUBS_BASE)
#define RF_AXI_GRP(_grp, _reg) ((((_grp) * 4096 + (_reg)) * 4) + AXI_BUS_BASE)

#define RF_MASK_V(_mask, _val)       (((_mask) << 16) | (_val))
#define RF_MASK_V_SET(_mask)         (((_mask) << 16) | (_mask))
#define RF_MASK_V_CLR(_mask)         (((_mask) << 16) | 0)

/*APB RBUS register group*/
#define MOON0_GROUP_NO 		(0)
#define MOON1_GROUP_NO     (1)
#define MOON2_GROUP_NO		(2)
#define MOON3_GROUP_NO		(3)
#define MOON4_GROUP_NO		(4)
#define MOON5_GROUP_NO		(5)
#define GPIOXT0_GROUP_NO	(6)
#define GPIOXT1_GROUP_NO	(7)
#define IOP_GROUP_NO		(8)
#define INT_CTRL0_GROUP_NO	(9)  //for ARM926
#define INT_CTRL1_GROUP_NO	(10)
#define TSENT_GROUP_NO		(11)

#define INT_CTRL2_GROUP_NO	(15) //for C-Chip
#define INT_CTRL3_GROUP_NO	(23)


/*SP7021 module clock enable bit*/
#define MODULE_CLK_EN_BASE	RF_GRP(0, 1)

/*SP7021 module clock enable bit*/
#define MODULE_CLK_GATE_BASE RF_GRP(0, 11)

/*SP7021 module rest bit*/
#define MODULE_RESET_BASE RF_GRP(0, 21)



/*TIMER0 & TIMER1 is a group*/
#define TIM0_BASE RF_GRP(12, 16)
#define TIM1_BASE RF_GRP(12, 20)

/*TIMER2 & TIMER3 is a group*/
#define TIM2_BASE RF_GRP(96, 16)
#define TIM3_BASE RF_GRP(96, 20)

/*TIMER4 & TIMER5 is a group*/
#define TIM4_BASE RF_GRP(97, 16)
#define TIM5_BASE RF_GRP(97, 20)

/*TIMER6 & TIMER7 is a group*/
#define TIM6_BASE RF_GRP(99, 16)
#define TIM7_BASE RF_GRP(99, 20)


#define CLK_EN			((Module_Clock_En_type*) MODULE_CLK_EN_BASE)
#define CLK_GATE		((Module_Clock_Gate_Type*) MODULE_CLK_GATE_BASE)
#define MODULE_REST		((Module_Reset_Type*) MODULE_RESET_BASE)
#define TIM0			((TIM_TypeDef *) TIM0_BASE)
#define TIM1			((TIM_TypeDef *) TIM1_BASE)
#define TIM2			((TIM_TypeDef *) TIM2_BASE)
#define TIM3			((TIM_TypeDef *) TIM3_BASE)
#define TIM4			((TIM_TypeDef *) TIM4_BASE)
#define TIM5			((TIM_TypeDef *) TIM5_BASE)
#define TIM6			((TIM_TypeDef *) TIM6_BASE)
#define TIM7			((TIM_TypeDef *) TIM7_BASE)




/******************************************************************************/
/*                                                                            */
/*                        Standard time clock                                 */
/*                                                                            */
/******************************************************************************/


/********************  Bit definition for STC register  ********************/

#define STC_PRESCALER_Pos 	(0U)
#define STC_PRESCALER_Msk	(0x7FFU << STC_PRESCALER_Pos) 	/*0x000007FF*/
#define STC_PRESCALER		STC_PRESCALER_Msk				/*STC prescale value */
#define STC_TRIG_SRC_Pos 	(15U)							
#define STC_TRIG_SRC_Msk	(1U << STC_TRIG_SRC_Pos)		/*0x00008000*/
#define STC_TRIG_SRC		STC_TRIG_SRC_Msk				/*The triger source of the STC counter. 0:system clock; 1:externl clock*/
#define STC_EXT_DIV_Pos		(0U)	
#define STC_EXT_DIV_Msk		(0xFFU << STC_EXT_DIV_Pos)		/*0x000000FF*/
#define STC_EXT_DIV			(STC_EXT_DIV_Msk)				/*The external clock divder for STC*/		


/******************************************************************************/
/*                                                                            */
/*                        timer/counter			                              */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for timer register  ********************/
#define TIMER_GO_Pos 			(0)
#define TIMER_GO_Msk 			(1U << TIMER_GO_Pos)			/*0x00000001*/
#define TIMER_GO				TIMER_GO_Msk					/*timer go. 1:start;0:stop*/	
#define TIMER_RPT_pos			(1U)
#define TIMER_RPT_Msk			(1U << TIMER_RPT_pos)			/*0x00000002*/	
#define TIMER_RPT				TIMER_RPT_Msk					/*timer repeat operation. 0:one shot;1: repeation*/
#define TIMER_TRIG_SRC_Pos		(2U)
#define TIMER_TRIG_SRC_Msk		(0xF << TIMER_TRIG_SRC_Pos)		/*0x0000003C*/
#define TIMER_TRIG_SRC			TIMER_TRIG_SRC_Msk				/*timer triger source selection. 0:systemclock triger;1:stc triger;2:rtc_triger;3:external clk triger;4:in a group timer prescale triger;5:in a group timer triger*/




/****************** TIM Instances : All supported instances *******************/
#define IS_TIM_INSTANCE(INSTANCE)       (((INSTANCE) == TIM0)   || \
										 ((INSTANCE) == TIM1)   || \
                                         ((INSTANCE) == TIM2)   || \
                                         ((INSTANCE) == TIM3)   || \
                                         ((INSTANCE) == TIM4)   || \
                                         ((INSTANCE) == TIM5)   || \
                                         ((INSTANCE) == TIM6)   || \
                                         ((INSTANCE) == TIM7))


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SP7021_ARM926_H */


