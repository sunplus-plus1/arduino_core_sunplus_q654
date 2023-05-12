#ifndef __SP645_CM4_H
#define __SP645_CM4_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define __CM4_REV                 0x0001U  /*!< Core revision r0p1                            */
#define __MPU_PRESENT             0U       /*!< Q645 devices provide an MPU */
#define __NVIC_PRIO_BITS          4U       /*!< Q645 devices use 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0U       /*!< Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             1U       /*!< Q645 devices provide an FPU */

typedef enum
{
/******  Cortex-M4 Processor Exceptions Numbers ***********************************************/
  NonMaskableInt_IRQn              = -14,    /*!< 2 Non Maskable Interrupt                      */
  HardFault_IRQn                   = -13,    /*!< 3 Cortex-M4 Hard Fault Interrupt              */
  MemoryManagement_IRQn            = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt       */
  BusFault_IRQn                    = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt               */
  UsageFault_IRQn                  = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt             */
  SVCall_IRQn                      = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                */
  DebugMonitor_IRQn                = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt          */
  PendSV_IRQn                      = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                */
  SysTick_IRQn                     = -1,     /*!< 15 Cortex-M4 System Tick Interrupt            */
/******  SP645 specific Interrupt Numbers ***************************************************/
  EXTI0_IRQn                       = 0,      /*!< EXTI Line0 Interrupt                          */
  EXTI1_IRQn                       = 1,      /*!< EXTI Line1 Interrupt                          */
  EXTI2_IRQn                       = 2,      /*!< EXTI Line2 Interrupt                          */
  EXTI3_IRQn                       = 3,      /*!< EXTI Line3 Interrupt                          */
  EXTI4_IRQn                       = 4,      /*!< EXTI Line4 Interrupt                          */
  EXTI5_IRQn                       = 5,      /*!< EXTI Line5 Interrupt                          */
  EXTI6_IRQn                       = 6,      /*!< EXTI Line6 Interrupt                          */
  EXTI7_IRQn                       = 7,      /*!< EXTI Line7 Interrupt                          */
  I2C_MASTER0_IRQ                  = 8,      /*!< I2C 0  interrupt                              */
  IPC_CA552CM4_INT0_IRQn           = 9,      /*!< Mailbox CA55 notify to CM4 interrupt 0        */
  IPC_CA552CM4_INT1_IRQn           = 10,      /*!< Mailbox CA55 notify to CM4 interrupt 1       */
  CPIO_LEFT_IRQn                   = 11,      /*!< CPIO Left side interrupt                     */
  CPIO_RIGT_IRQn                   = 12,      /*!< CPIO Right side interrupt                    */
  IPC_CA552CM4_INT2_IRQn           = 13,      /*!< Mailbox CA55 notify to CM4 interrupt 2       */
  IPC_CA552CM4_INT3_IRQn           = 14,      /*!< Mailbox CA55 notify to CM4 interrupt 3       */
  SDIO_IRQn                        = 15,      /*!< SDIO Interrupt                               */
  CARD_IRQn                        = 16,      /*!< SD Card interrupt                            */
  EMMC_IRQn                        = 17,      /*!< EMMC Interrupt                               */
  SPI_FLASH_IRQn                   = 18,      /*!< SPI NOR Flash Interrupt                      */
  SPI_COMBO0_DMA_W_IRQn            = 19,      /*!< SPI Combo0 DMA write interrupt               */
  RESERVED_20                      = 20,      /*!< RESERVED interrupt                           */
  SPI_MASTER0_IRQn                 = 21,      /*!< SPI Combo0 Master interrupt                  */
  SPI_SLAVE0_IRQn                  = 22,      /*!< SPI Combo0 Slave interrupt                   */
  SPI_NAND_IRQn                    = 23,      /*!< SPI Nand interrupt                           */
  BCH_IRQn                         = 24,      /*!< BCH interrupt                                */
  IPC_CA552CM4_INT4_IRQn           = 25,      /*!< Mailbox CA55 notify to CM4 interrupt 4       */
  IPC_CA552CM4_INT5_IRQn           = 26,      /*!< Mailbox CA55 notify to CM4 interrupt 5       */
  SPI_COMBO1_DMA_W_IRQn            = 27,      /*!< SPI Combo1 DMA write interrupt               */
  SPI_MASTER1_IRQn                 = 28,      /*!< SPI Combo1 Master interrupt                  */
  SPI_SLAVE1_IRQn                  = 29,      /*!< SPI Combo1 Slave interrupt                   */
  MSI_UADMA3_IRQn                  = 30,      /*!< MSI Interrupt from PERI1 OCP2AXI Write       */
  MSI_UADMA2_IRQn                  = 31,      /*!< MSI Interrupt from PERI1 OCP2AXI Write       */
  MSI_UADMA0_IRQn                  = 32,      /*!< MSI Interrupt from PERI1 OCP2AXI Write       */
  MSI_UADMA1_IRQn                  = 33,      /*!< MSI Interrupt from PERI1 OCP2AXI Write       */
  RESERVED_34                      = 34,      /*!< RESERVED interrupt                           */
  UADBG_IRQn                       = 35,      /*!< UART to AXI for Debug Interrupt              */
  CPIO2CM4_INT0_IRQn               = 36,      /*!< CPIO to CM4 Interrupt 0                      */
  CPIO2CM4_INT1_IRQn               = 37,      /*!< CPIO to CM4 Interrupt 1                      */
  CPIO2CM4_INT2_IRQn               = 38,      /*!< CPIO to CM4 Interrupt 2                      */
  CPIO2CM4_INT3_IRQn               = 39,      /*!< CPIO to CM4 Interrupt 3                      */
  CPIO2CM4_INT4_IRQn               = 40,      /*!< CPIO to CM4 Interrupt 4                      */
  CPIO2CM4_INT5_IRQn               = 41,      /*!< CPIO to CM4 Interrupt 5                      */
  CPIO2CM4_INT6_IRQn               = 42,      /*!< CPIO to CM4 Interrupt 6                      */
  CPIO2CM4_INT7_IRQn               = 43,      /*!< CPIO to CM4 Interrupt 7                      */
  CPIO2CM4_INT8_IRQn               = 44,      /*!< CPIO to CM4 Interrupt 8                      */
  CPIO2CM4_INT9_IRQn               = 45,      /*!< CPIO to CM4 Interrupt 9                      */
  CPIO2CM4_INT10_IRQn              = 46,      /*!< CPIO to CM4 Interrupt 10                     */
  CPIO2CM4_INT11_IRQn              = 47,      /*!< CPIO to CM4 Interrupt 11                     */
  CPIO2CM4_INT12_IRQn              = 48,      /*!< CPIO to CM4 Interrupt 12                     */
  CPIO2CM4_INT13_IRQn              = 49,      /*!< CPIO to CM4 Interrupt 13                     */
  CPIO2CM4_INT14_IRQn              = 50,      /*!< CPIO to CM4 Interrupt 14                     */
  CPIO2CM4_INT15_IRQn              = 51,      /*!< CPIO to CM4 Interrupt 15                     */
  CPIO2CM4_INT16_IRQn              = 52,      /*!< CPIO to CM4 Interrupt 16                     */
  CPIO2CM4_INT17_IRQn              = 53,      /*!< CPIO to CM4 Interrupt 17                     */
  SECURIY_GROUP1_IRQn              = 54,      /*!< Security group 1 protect interrupt           */
  HSM_INT0_IRQn                    = 55,      /*!< HSM Interrupt 0                              */
  HSM_INT1_IRQn                    = 56,      /*!< HSM Interrupt 1                              */
  HSM_INT2_IRQn                    = 57,      /*!< HSM Interrupt 2                              */
  IOP_INT0_IRQn                    = 58,      /*!< IOP Interrupt 0                              */
  IOP_INT1_IRQn                    = 59,      /*!< IOP Interrupt 1                              */
  UART0_IRQn                       = 60,      /*!< UART0 Interrupt                              */
  UART1_IRQn                       = 61,      /*!< UART1 Interrupt                              */
  AXI_MON_TOP_IRQn                 = 62,      /*!< AXI Gloable Monitor Interrupt                */
  PWM_USER_INT0_IRQn               = 63,      /*!< PWM User Interrupt0                          */
  PWM_END_INT0_IRQn                = 64,      /*!< PWM End Interrupt0                           */
  PWM_USER_INT1_IRQn               = 65,      /*!< PWM User Interrupt1                          */
  PWM_END_INT1_IRQn                = 66,      /*!< PWM End Interrupt1                           */
  PWM_USER_INT2_IRQn               = 67,      /*!< PWM User Interrupt2                          */
  PWM_END_INT2_IRQn                = 68,      /*!< PWM End Interrupt2                           */
  PWM_USER_INT3_IRQn               = 69,      /*!< PWM User Interrupt3                          */
  PWM_END_INT3_IRQn                = 70,      /*!< PWM End Interrupt3                           */
  UADMA0_IRQn                      = 71,      /*!< UART DMA0 Interrupt                          */
  UADMA1_IRQn                      = 72,      /*!< UART DMA0 Interrupt                          */
  AXI_MON_PAI_IRQn                 = 73,      /*!< AXI Gloable Monitor Interrupt                */
  CBDMA0_IRQn                      = 74,      /*!< CBDMA0 Interrupt                             */
  AXI_MON_PAII_IRQn                = 75,      /*!< AXI Gloable Monitor Interrupt                */
  RBUS_IRQn                        = 76,      /*!< RBUS Interrupt                               */
  VC8000E_IRQn                     = 77,      /*!< Video Encoder Interrupt                      */
  IPC_CA552CM4_INT6_IRQn           = 78,      /*!< Mailbox CA55 notify to CM4 interrupt 6       */
  IPC_CA552CM4_INT7_IRQn           = 79,      /*!< Mailbox CA55 notify to CM4 interrupt 7       */
  CSI_FIELD_START_INT0_IRQn        = 80,      /*!< MIPI CSI Field Start Interrupt0              */
  CSI_FIELD_END_INT0_IRQn          = 81,      /*!< MIPI CSI Field End Interrupt0                */
  CSI_FIELD_START_INT1_IRQn        = 82,      /*!< MIPI CSI Field Start Interrupt1              */
  CSI_FIELD_END_INT1_IRQn          = 83,      /*!< MIPI CSI Field End Interrupt1                */
  CSI_FIELD_START_INT2_IRQn        = 84,      /*!< MIPI CSI Field Start Interrupt2              */
  CSI_FIELD_END_INT2_IRQn          = 85,      /*!< MIPI CSI Field End Interrupt2                */
  CSI_FIELD_START_INT3_IRQn        = 86,      /*!< MIPI CSI Field Start Interrupt3              */
  CSI_FIELD_END_INT3_IRQn          = 87,      /*!< MIPI CSI Field End Interrupt3                */
  VCL_INTR0_IRQn                   = 88,      /*!< VCL ISP Interrupt0                           */
  VCL_INTR1_IRQn                   = 89,      /*!< VCL ISP Interrupt1                           */
  VCL_INTR2_IRQn                   = 90,      /*!< VCL ISP Interrupt2                           */
  VCL_INTR3_IRQn                   = 91,      /*!< VCL ISP Interrupt3                           */
  VCL_INTR4_IRQn                   = 92,      /*!< VCL ISP Interrupt4                           */
  VCL_INTR5_IRQn                   = 93,      /*!< VCL ISP Interrupt5                           */
  I2C_MASTER1_IRQ                  = 94,      /*!< I2C 1  interrupt                             */
  I2C_MASTER2_IRQ                  = 95,      /*!< I2C 2  interrupt                             */
  I2C_MASTER3_IRQ                  = 96,      /*!< I2C 3  interrupt                             */
  I2C_MASTER4_IRQ                  = 97,      /*!< I2C 4  interrupt                             */
  I2C_MASTER5_IRQ                  = 98,      /*!< I2C 5  interrupt                             */
  RTC_PERIODIC_IRQ                 = 99,      /*!< RTC Periodic interrupt                      */
  AUD_LOSD_IRQ                     = 100,      /*!< Audio LOSD interrupt                        */
  AUD_FIFO_IRQ                     = 101,      /*!< Audio FIFO interrupt                        */
  AUD_TWS_LATCH_IRQ                = 102,      /*!< Audio TWS Latch interrupt                   */
  AUD_TWS_SAMPLE_IRQ               = 103,      /*!< Audio TWS Sample interrupt                  */
  RESERVED_104                     = 104,      /*!< RESERVED interrupt                          */
  RESERVED_105                     = 105,      /*!< RESERVED interrupt                          */
  SPI_COMBO2_DMA_W_IRQn            = 106,       /*!< SPI Combo2 DMA write interrupt             */
  SPI_MASTER2_IRQn                 = 107,       /*!< SPI Combo2 Master interrupt                */
  SPI_SLAVE2_IRQn                  = 108,       /*!< SPI Combo2 Slave interrupt                 */
  SPI_COMBO3_DMA_W_IRQn            = 109,       /*!< SPI Combo3 DMA write interrupt             */
  SPI_MASTER3_IRQn                 = 110,       /*!< SPI Combo3 Master interrupt                */
  SPI_SLAVE3_IRQn                  = 111,       /*!< SPI Combo3 Slave interrupt                   */
  SPI_COMBO4_DMA_W_IRQn            = 112,       /*!< SPI Combo4 DMA write interrupt               */
  SPI_MASTER4_IRQn                 = 113,       /*!< SPI Combo4 Master interrupt                  */
  SPI_SLAVE4_IRQn                  = 114,       /*!< SPI Combo4 Slave interrupt                   */
  SPI_COMBO5_DMA_W_IRQn            = 115,       /*!< SPI Combo5 DMA write interrupt               */
  SPI_MASTER5_IRQn                 = 116,       /*!< SPI Combo5 Master interrupt                  */
  SPI_SLAVE5_IRQn                  = 117,       /*!< SPI Combo5 Slave interrupt                   */
  IPC_CM42CA55_INT0_IRQn           = 118,       /*!< Mailbox CM4 notify to CA55 interrupt 0       */
  IPC_CM42CA55_INT1_IRQn           = 119,       /*!< Mailbox CM4 notify to CA55 interrupt 1       */
  IPC_CM42CA55_INT2_IRQn           = 120,       /*!< Mailbox CM4 notify to CA55 interrupt 2       */
  IPC_CM42CA55_INT3_IRQn           = 121,       /*!< Mailbox CM4 notify to CA55 interrupt 3       */
  IPC_CM42CA55_INT4_IRQn           = 122,       /*!< Mailbox CM4 notify to CA55 interrupt 4       */
  IPC_CM42CA55_INT5_IRQn           = 123,       /*!< Mailbox CM4 notify to CA55 interrupt 5       */
  IPC_CM42CA55_INT6_IRQn           = 124,       /*!< Mailbox CM4 notify to CA55 interrupt 6       */
  IPC_CM42CA55_INT7_IRQn           = 125,       /*!< Mailbox CM4 notify to CA55 interrupt 7       */
  IPC_CM42CA55_NORMAL_INT_IRQn     = 126,       /*!< Mailbox CM4 normal notify to CA55 interrupt  */
  IPC_CA552CM4_NORMAL_INT_IRQn     = 127,       /*!< Mailbox CA55 normal notify to CM4 interrupt  */
  STC_TIMER_W_IRQn                 = 128,       /*!< STC Watchdog interrupt        */
  STC_TIMER0_IRQn                  = 129,       /*!< STC timer0 interrupt          */
  STC_TIMER1_IRQn                  = 130,       /*!< STC timer1 interrupt          */
  STC_TIMER2_IRQn                  = 131,       /*!< STC timer2 interrupt          */
  STC_TIMER3_IRQn                  = 132,       /*!< STC timer3 interrupt          */
  STC_AV0_TIMER0_IRQn              = 133,       /*!< STC timer0 interrupt          */
  STC_AV0_TIMER1_IRQn              = 134,       /*!< STC timer1 interrupt          */
  STC_AV0_TIMER2_IRQn              = 135,       /*!< STC timer2 interrupt          */
  STC_AV0_TIMER3_IRQn              = 136,       /*!< STC timer3 interrupt          */
  STC_AV1_TIMER0_IRQn              = 137,       /*!< STC timer0 interrupt          */
  STC_AV1_TIMER1_IRQn              = 138,       /*!< STC timer1 interrupt          */
  STC_AV1_TIMER2_IRQn              = 139,       /*!< STC timer2 interrupt          */
  STC_AV1_TIMER3_IRQn              = 140,       /*!< STC timer3 interrupt          */
  RTC_ALARM_IRQn                   = 141,       /*!< RTC Alarm interrupt           */
  STC_AV2_TIMER0_IRQn              = 142,       /*!< STC timer0 interrupt          */
  STC_AV2_TIMER1_IRQn              = 143,       /*!< STC timer1 interrupt          */
  STC_AV2_TIMER2_IRQn              = 144,       /*!< STC timer2 interrupt          */
  STC_AV2_TIMER3_IRQn              = 145,       /*!< STC timer3 interrupt          */
  STC_AV0_TIMER_W_IRQn             = 146,       /*!< STC Watchdog interrupt        */
  STC_AV1_TIMER_W_IRQn             = 147,       /*!< STC Watchdog interrupt        */
  UPHY0_IRQn                       = 148,       /*!< USB Phy0 interrupt            */
  RESERVED_149                     = 149,       /*!< RESERVED interrupt            */
  RESERVED_150                     = 150,       /*!< RESERVED interrupt            */
  SAPCC_IRQn                       = 151,       /*!< SPACC interrupt               */
  RESERVED_152                     = 152,       /*!< RESERVED interrupt            */
  RESERVED_153                     = 153,       /*!< RESERVED interrupt            */
  RESERVED_154                     = 154,       /*!< RESERVED interrupt            */
  RESERVED_155                     = 155,       /*!< RESERVED interrupt            */
  RESERVED_156                     = 156,       /*!< RESERVED interrupt            */
  UART2_IRQn                       = 157,       /*!< UART2 Interrupt               */
  UART3_IRQn                       = 158,       /*!< UART3 Interrupt               */
  UART4_IRQn                       = 159,       /*!< UART4 Interrupt               */
  UADMA2_IRQn                      = 160,       /*!< UART DMA2 Interrupt           */
  UADMA3_IRQn                      = 161,       /*!< UART DMA3 Interrupt           */
  UART6_IRQn                       = 162,       /*!< UART6 Interrupt               */
  UART7_IRQn                       = 163,       /*!< UART7 Interrupt               */
  UART8_IRQn                       = 164,       /*!< UART8 Interrupt               */
  EVDN_IRQn                        = 165,       /*!< EVDN Interrupt                */
  RESERVED_166                     = 166,       /*!< RESERVED interrupt            */
  VC8000D_IRQn                     = 167,       /*!< Video Decoder Interrupt       */
  RESERVED_168                     = 168,       /*!< RESERVED interrupt            */
  GPU_EVENT_IRQn                   = 169,       /*!< G31 Event Interrupt           */
  GPU_JOB_IRQn                     = 170,       /*!< G31 Job Interrupt             */
  GPU_MMU_IRQn                     = 171,       /*!< G31 MMU Interrupt             */
  GPU_IRQn                         = 172,       /*!< G31 GPU Interrupt             */
  N78_JOB_IRQn                     = 173,       /*!< N78 Job Interrupt             */
  N78_DEBUG_IRQn                   = 174,       /*!< N78 Debug Interrupt           */
  N78_JOBNS_IRQn                   = 175,       /*!< N78 Job NS Interrupt          */
  N78_DEBUGNS_IRQn                 = 176,       /*!< N78 Debug NS Interrupt        */
  N78_ERR_IRQn                     = 177,       /*!< N78 Debug NS Interrupt        */
  N78_ERRNS_IRQn                   = 178,       /*!< N78 Debug NS Interrupt        */
  USB30C1_IRQn                     = 179,       /*!< USB30C1 Interrupt             */
  USB30C0_IRQn                     = 180,       /*!< USB30C0 Interrupt             */
  USBC0_OTG_IRQn                   = 181,       /*!< USBC0_OTG Interrupt           */
  USBC0_DEVIC_IRQn                 = 182,       /*!< USBC0_DEVIC interrupt         */
  USBC0_EHCI_IRQn                  = 183,       /*!< USBC0_EHCI Interrupt          */
  USBC0_OHCI_IRQn                  = 184,       /*!< USBC0_OHCI interrupt          */
  CPIO_INT_TO_CM4_58_IRQn          = 185,       /*!< cpio_int_to_cm4 Interrupt     */
  CPIO_INT_TO_CM4_59_IRQn          = 186,       /*!< cpio_int_to_cm4 Interrupt     */
  CPIO_INT_TO_CM4_60_IRQn          = 187,       /*!< cpio_int_to_cm4 Interrupt     */
  CPIO_INT_TO_CM4_61_IRQn          = 188,       /*!< cpio_int_to_cm4 Interrupt     */
  CPIO_INT_TO_CM4_62_IRQn          = 189,       /*!< cpio_int_to_cm4 Interrupt     */
  CPIO_INT_TO_CM4_63_IRQn          = 190,       /*!< cpio_int_to_cm4 Interrupt     */
  U3PHY0_IRQn                      = 191,       /*!< U3PHY0 Interrupt              */
  U3PHY1_IRQn                      = 192,       /*!< U3PHY1 Interrupt              */
  N78_ARADDRM0_IRQn                = 193,       /*!< from N78 Interrupt            */
  N78_AWADDRM0_IRQn                = 194,       /*!< from N78 Interrupt            */
  G31_ARADDRM0_IRQn                = 195,       /*!< from GPU G31 Interrupt        */
  G31_AWADDRM0_IRQn                = 196,       /*!< from GPU G31 Interrupt        */
  DDRPHY_IRQn                      = 197,       /*!< DDRPHY Interrupt              */
  UMCTL2_IRQn                      = 198,       /*!< UMCTL2 Interrupt              */
  MAX_IRQ_n
} IRQn_Type;

typedef enum {
/*G0.21*/
  SYSTEM                           = 0,        /*!< System bus moudle ID*/
  CA55CORE0                        = 1,        /*!< CA55 CORE 0 moudle ID */
  CA55CORE1                        = 2,        /*!< CA55 CORE 1 moudle ID */
  CA55CORE2                        = 3,        /*!< CA55 CORE 2 moudle ID */
  CA55CORE3                        = 4,        /*!< CA55 CORE 3 moudle ID */
  CA55SCUL3                        = 5,        /*!< CA55 SCUL 3 moudle ID */
  CA55                             = 6,        /*!< CA55 moudle ID */
  IOP                              = 7,        /*!< IOP moudle ID */
  PBUS0                            = 8,        /*!< PBUS0 moudle ID */
  PBUS1                            = 9,        /*!< PBUS1 moudle ID */
  PBUS2                            = 10,       /*!< PBUS2 moudle ID */
  PBUS3                            = 11,       /*!< PBUS3 moudle ID */
  BR0                              = 12,       /*!< Bootrom 0 moudle ID */
  CARD_CTL0                        = 13,       /*!< Emmc moudle ID */
  CARD_CTL1                        = 14,       /*!< SDCARD 0 moudle ID */
  CARD_CTL2                        = 15,       /*!< SDCARD 2 moudle ID */
/*G0.22*/
  CBDMA0                           = 16,       /*!< CBDMA moudle ID */
  CPIOL                            = 17,       /*!< CPIO L moudle ID */
  CPIOR                            = 18,       /*!< CPIO R moudle ID */
  DDR_PHY0                         = 19,       /*!< SDCTRL0 moudle ID */
  SDCTRL0                          = 20,       /*!< SDCTRL0 moudle ID */
  DUMMY_MASTER0                    = 21,       /*!< DUMMY_MASTER0 moudle ID */
  DUMMY_MASTER1                    = 22,       /*!< DUMMY_MASTER1 moudle ID */
  DUMMY_MASTER2                    = 23,       /*!< DUMMY_MASTER2 moudle ID */
  EVDN                             = 24,       /*!< EVDN moudle ID */
  SDPOART0                         = 25,       /*!< SDPOART0 moudle ID */
  UMCTL2                           = 26,       /*!< UMCTL2 moudle ID */
  GPU                              = 27,       /*!< GPU moudle ID */
  HSM                              = 28,       /*!< HSM moudle ID */
  RESERVE1                         = 29,       /*!< moudle ID */
  SPACC                            = 30,       /*!< SPACC moudle ID */
  INTERRUPT                        = 31,       /*!< INTERRUPT moudle ID */
/*G0.23*/
  N78                              = 32,       /*!< N78 moudle ID */
  SYSTOP                           = 33,       /*!< SYSTOP moudle ID */
  OTPRX                            = 34,       /*!< OTPRX moudle ID */
  PMC                              = 35,       /*!< PMC moudle ID */
  RBUS_BLOCKA                      = 36,       /*!< RBUS_BLOCKA moudle ID */
  RBUS_BLOCKB                      = 37,       /*!< RBUS_BLOCKB moudle ID */
  RESERVE2                         = 38,       /*!< moudle ID */
  RESERVE3                         = 39,       /*!< moudle ID */
  RTC                              = 40,       /*!< RTC moudle ID */
  MIPZ                             = 41,       /*!< MIPZ moudle ID */
  SPIFL                            = 42,       /*!< SPIFL moudle ID */
  BCH                              = 43,       /*!< BCH moudle ID */
  SPIND                            = 44,       /*!< SPIND moudle ID */
  UADMA01                          = 45,       /*!< UADMA01 moudle ID */
  UADMA23                          = 46,       /*!< UADMA23 moudle ID */
  UART0                            = 47,       /*!< UART0 moudle ID */
/*G0.24*/
  UART1                            = 48,        /*!< UART1 moudle ID */
  UART2                            = 49,        /*!< UART2 moudle ID */
  UART3                            = 50,        /*!< UART3 moudle ID */
  UART4                            = 51,        /*!< UART4 moudle ID */
  UART5                            = 52,        /*!< UART5 moudle ID */
  UART_DBG                         = 53,        /*!< UART_DBG moudle ID */
  UART2AXI                         = 54,        /*!< UART2AXI moudle ID */
  GDMAUA                           = 55,        /*!< GDMAUA moudle ID */
  UPHY0                            = 56,        /*!< UPHY0 moudle ID */
  USB30C0                          = 57,        /*!< USB30C0 moudle ID */
  USB30C1                          = 58,        /*!< USB30C1 moudle ID */
  U3PHY0                           = 59,        /*!< U3PHY0 moudle ID */
  U3PHY1                           = 60,        /*!< U3PHY1 moudle ID */
  USBC0                            = 61,        /*!< USBC0 moudle ID */
  VCD                              = 62,        /*!< VCD moudle ID */
  VCE                              = 63,        /*!< VCE moudle ID */
/*G0.25*/
  CM4                              = 64,        /*!< CM4 moudle ID */
  STC_0                            = 65,        /*!< STC0 moudle ID */
  STC_AV0                          = 66,        /*!< STC_AV0 moudle ID */
  STC_AV1                          = 67,        /*!< STC_AV1 moudle ID */
  STC_AV2                          = 68,        /*!< STC_AV2 SEL moudle ID */
  MAILBOX                          = 69,        /*!< MAILBOX moudle ID */
  PAI                              = 70,        /*!< PAI moudle ID */
  PAII                             = 71,        /*!< PAII moudle ID */
  DDRPHY                           = 72,        /*!< DDRPHY moudle ID */
  DDRCTL                           = 73,        /*!< DDRCTL moudle ID */
  I2CM0                            = 74,        /*!< I2CM0 moudle ID */
  SPICOMBO0                        = 75,        /*!< SPICOMBO0 moudle ID */
  SPICOMBO1                        = 76,        /*!< SPICOMBO1 moudle ID */
  SPICOMBO2                        = 77,        /*!< SPICOMBO2 moudle ID */
  SPICOMBO3                        = 78,        /*!< SPICOMBO3 moudle ID */
  SPICOMBO4                        = 79,        /*!< SPICOMBO4 moudle ID */
/*G0.26*/
  SPICOMBO5                        = 80,        /*!< SPICOMBO5 moudle ID */
  CSIIW0                           = 81,        /*!< CSIIW0 moudle ID */
  MIPICSI0                         = 82,        /*!< MIPICSI0 moudle ID */
  CSIIW1                           = 83,        /*!< CSIIW1 moudle ID */
  MIPICSI1                         = 84,        /*!< MIPICSI1 moudle ID */
  CSIIW2                           = 85,        /*!< CSIIW2 moudle ID */
  MIPICSI2                         = 86,        /*!< MIPICSI2 moudle ID */
  CSIIW3                           = 87,        /*!< CSIIW3 moudle ID */
  MIPICSI3                         = 88,        /*!< MIPICSI3 moudle ID */
  VCL                              = 89,        /*!< VCL moudle ID */
  DISP_PWM                         = 90,        /*!< DISP_PWM moudle ID */
  I2CM1                            = 91,        /*!< I2CM1 moudle ID */
  I2CM2                            = 92,        /*!< I2CM2 moudle ID */
  I2CM3                            = 93,        /*!< I2CM3 moudle ID */
  I2CM4                            = 94,        /*!< I2CM4 moudle ID */
  I2CM5                            = 95,        /*!< I2CM5 moudle ID */
/*G0.27*/
  UART6                            = 96,        /*!< UART6 moudle ID */
  UART7                            = 97,        /*!< UART7 moudle ID */
  UART8                            = 98,        /*!< UART8 moudle ID */
  AUD                              = 99,        /*!< AUD moudle ID */
  VIDEO_CODEC                      = 100,       /*!< VIDEO_CODEC moudle ID */
  AXIM_PAI                         = 101,       /*!< AXIM_PAI moudle ID */
  AXIM_PAII                        = 102,       /*!< STC AXIM_PAII moudle ID */
  AXIM_TOP                         = 103,       /*!< AXIM_TOP AV3 moudle ID */
}MODULE_ID_Type;

#define PINMUX_DFMP_2BIT            (0x10000)
#define PINMUX_DFMP_3BIT            (0x20000)

typedef enum{
   /*G1.1*/
  PINMUX_SPI_FLASH=16,
  PINMUX_UART_IOP,
  PINMUX_DISP_PWM,
  PINMUX_EMMC1,
  PINMUX_SPI_NAND=20|PINMUX_DFMP_2BIT,
  PINMUX_SDCARD=22,
  PINMUX_SDIO,
  PINMUX_UART0,
  PINMUX_UART1,
  PINMUX_UART2,
  PINMUX_UART3,
  PINMUX_UA2AXI,
  PINMUX_UART6,
  PINMUX_UART7,
  PINMUX_UART8,
  /*G1.2*/
  PINMUX_SPI_COM0_MST,
  PINMUX_SPI_COM0_SLV,
  PINMUX_CA55_JTAG,
  PINMUX_CM4_JTAG,
  PINMUX_HSM_JTAG,
  PINMUX_EVDN_JTAG,
  PINMUX_SPI_COM1_MST,
  PINMUX_SPI_COM1_SLV,
  PINMUX_SPI_COM2_MST,
  PINMUX_SPI_COM2_SLV,
  PINMUX_SPI_COM3_MST,
  PINMUX_SPI_COM3_SLV,
  PINMUX_SPI_COM4_MST,
  PINMUX_SPI_COM4_SLV,
  PINMUX_SPI_COM5_MST,
  PINMUX_SPI_COM5_SLV,
  /*G1.3*/
  PINMUX_I2C_0,
  PINMUX_I2C_1,
  PINMUX_I2C_2,
  PINMUX_I2C_3,
  PINMUX_I2C_4,
  PINMUX_I2C_5,
  PINMUX_AUD_TDMTX_XCK,
  PINMUX_AUD_DAC_CLK,
  PINMUX_AUD_AU2_DATA0,
  PINMUX_AUD_AU1_DATA0,
  PINMUX_AUD_AU2_CK,
  PINMUX_AUD_AU1_CK,
  PINMUX_AUD_AU_ADC_DATA0,
  PINMUX_AUD_ADC2_DATA0,
  PINMUX_AUD_ADC1_DATA0,
  PINMUX_AUD_TDM,
  /*G1.4*/
  PINMUX_SPDIF_IN=65|PINMUX_DFMP_3BIT,
  PINMUX_SPDIF_OUT=68|PINMUX_DFMP_3BIT,
  PINMUX_VCL=71,
  PINMUX_UAHSM,
  PINMUX_UAHSM_EXT,
  PINMUX_CPIOL_I2C,
  PINMUX_CPIOR_I2C,
  PINMUX_GPIO_INT0=75|PINMUX_DFMP_3BIT,
   /*G1.5*/
  PINMUX_GPIO_INT1=80|PINMUX_DFMP_3BIT,
  PINMUX_GPIO_INT2=83|PINMUX_DFMP_3BIT,
  PINMUX_GPIO_INT3=86|PINMUX_DFMP_3BIT,
  PINMUX_GPIO_INT4=89|PINMUX_DFMP_3BIT,
  PINMUX_GPIO_INT5=92|PINMUX_DFMP_3BIT,
   /*G1.6*/
  PINMUX_GPIO_INT6=96|PINMUX_DFMP_3BIT,
  PINMUX_GPIO_INT7=99|PINMUX_DFMP_3BIT,
  PINMUX_MO1_CLK27=102,
  PINMUX_U2PHY0_DEBUG,
  PINMUX_UPHY0_IF,
  PINMUX_U3PHY0_DEBUG,
  PINMUX_U3PHY1_DEBUG,
  PINMUX_PROBE_PORT
}PINMUX_Type;




/** Macro to write new value to the bits in register */

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))


#include "core_cm4.h"
#include <stdint.h>
#include "system_sp645.h"



typedef struct {
    __IOM uint32_t sft_cfg[32];
}MOON_REG_Type;

typedef struct{
    __IOM uint32_t clock_enable[10];   /*!< \brief devices clock enable bit*/
}Module_Clock_En_type;

typedef struct{
    __IOM uint32_t clock_gate_enable[10]; /*!< \brief devices clock gate enable bit*/
}Module_Clock_Gate_Type;

typedef struct{
    __IOM uint32_t reset[10];         /*!< \brief devices clock gate enable bit*/
}Module_Reset_Type;

typedef struct{
    __IOM uint32_t dr;                /* data register */
    __IOM uint32_t lsr;               /* line status register */
    __IOM uint32_t msr;               /* modem status register */
    __IOM uint32_t lcr;               /* line control register */
    __IOM uint32_t mcr;               /* modem control register */
    __IOM uint32_t div_l;
    __IOM uint32_t div_h;
    __IOM uint32_t isc;              /* interrupt status/control */
    __IOM uint32_t txr;              /* tx residue */
    __IOM uint32_t rxr;              /* rx residue */
    __IOM uint32_t thr;              /* rx threshold */
    __IOM uint32_t clk_src;
}UART_CTRL_Type;

typedef struct {
   __IOM uint32_t rxdma_enable_sel;
   __IOM uint32_t rxdma_start_addr;
   __IOM uint32_t rxdma_timeout_set;
   __IOM uint32_t rxdma_reserved;
   __IOM uint32_t rxdma_wr_adr;
   __IOM uint32_t rxdma_rd_adr;
   __IOM uint32_t rxdma_length_thr;
   __IOM uint32_t rxdma_end_addr;
   __IOM uint32_t rxdma_databytes;
   __IOM uint32_t rxdma_debug_info;
}UART_Rxdma;

typedef struct {
   __IOM uint32_t txdma_enable;
   __IOM uint32_t txdma_sel;
   __IOM uint32_t txdma_start_addr;
   __IOM uint32_t txdma_end_addr;
   __IOM uint32_t txdma_wr_adr;
   __IOM uint32_t txdma_rd_adr;
   __IOM uint32_t txdma_status;
   __IOM uint32_t txdma_tmr_unit;
   __IOM uint32_t txdma_tmr_cnt;
   __IOM uint32_t txdma_rst_done;
}UART_Txdma;

typedef struct {
   __IOM uint32_t gdma_hw_ver;
   __IOM uint32_t gdma_config;
   __IOM uint32_t gdma_length;
   __IOM uint32_t gdma_addr;
   __IOM uint32_t gdma_port_mux;
   __IOM uint32_t gdma_int_flag;
   __IOM uint32_t gdma_int_en;
   __IOM uint32_t gdma_software_reset_status;
   __IOM uint32_t txdma_tmr_cnt;
}UART_TxGdma;

typedef struct{
	__IOM uint32_t stc_15_0;				 /*!< \brief  standard time clock counter, 0~15 bit */
	__IOM uint32_t stc_31_16;				 /*!< \brief  standard time clock counter, 16~31 bit */
	__IOM uint32_t stc_47_32;
	__IOM uint32_t stc_63_48;
	__IOM uint32_t stc_64;					 /*!< \brief  standard time clock counter, the MSB 64 bit, when write the bit ,clear the stc counter at once */
	__IOM uint32_t stc_prescale_val;
	__IOM uint32_t stc_config;
	RESERVED(0[19], uint32_t)
	__IOM uint32_t stcl_15_0;
	__IOM uint32_t stcl_31_16;
	__IOM uint32_t stcl_32;
	__IOM uint32_t atc_15_0;
	__IOM uint32_t atc_31_16;
	__IOM uint32_t atc_33_32;
}STC_TypeDef;


typedef struct{
   // SPI_MASTER
   RESERVED(0[5], uint32_t);
   __IOM uint32_t dma_rxtx_size;
   __IOM uint32_t dma_tx_address;
   __IOM uint32_t dma_rx_address;
   __IOM uint32_t dma_config;
   RESERVED(1[4], uint32_t);
   __IOM uint32_t fifo_data;
   __IOM uint32_t spi_status;
   __IOM uint32_t spi_config;
   __IOM uint32_t spi_clk_inv;
   __IOM uint32_t spi_out_int_config;

}SPI_TypeDef;

typedef struct
{
	__IOM uint32_t control0;      /* 00 */
	__IOM uint32_t control1;      /* 01 */
	__IOM uint32_t control2;      /* 02 */
	__IOM uint32_t control3;      /* 03 */
	__IOM uint32_t control4;      /* 04 */
	__IOM uint32_t control5;      /* 05 */
	__IOM uint32_t i2cm_status0;  /* 06 */
	__IOM uint32_t interrupt;     /* 07 */
	__IOM uint32_t int_en0;       /* 08 */
	__IOM uint32_t i2cm_mode;     /* 09 */
	__IOM uint32_t i2cm_status1;  /* 10 */
	__IOM uint32_t i2cm_status2;  /* 11 */
	__IOM uint32_t control6;      /* 12 */
	__IOM uint32_t int_en1;       /* 13 */
	__IOM uint32_t i2cm_status3;  /* 14 */
	__IOM uint32_t i2cm_status4;  /* 15 */
	__IOM uint32_t int_en2;       /* 16 */
	__IOM uint32_t control7;      /* 17 */
	__IOM uint32_t control8;      /* 18 */
	__IOM uint32_t control9;      /* 19 */
	RESERVED(0[3], uint32_t)      /* 20~22 */
	__IOM uint32_t version;       /* 23 */
	__IOM uint32_t data00_03;     /* 24 */
	__IOM uint32_t data04_07;     /* 25 */
	__IOM uint32_t data08_11;     /* 26 */
	__IOM uint32_t data12_15;     /* 27 */
	__IOM uint32_t data16_19;     /* 28 */
	__IOM uint32_t data20_23;     /* 29 */
	__IOM uint32_t data24_27;     /* 30 */
	__IOM uint32_t data28_31;     /* 31 */
} I2C_TypeDef;

typedef struct
{
	__IOM uint32_t hw_version;                /* 00 */
	__IOM uint32_t dma_config;                /* 01 */
	__IOM uint32_t dma_length;                /* 02 */
	__IOM uint32_t dma_addr;                  /* 03 */
	__IOM uint32_t port_mux;                  /* 04 */
	__IOM uint32_t int_flag;                  /* 05 */
	__IOM uint32_t int_en;                    /* 06 */
	__IOM uint32_t sw_reset_state;            /* 07 */
	RESERVED(0[2], uint32_t)                  /* 08~09 */
	__IOM uint32_t sg_dma_index;              /* 10 */
	__IOM uint32_t sg_dma_config;             /* 11 */
	__IOM uint32_t sg_dma_length;             /* 12 */
	__IOM uint32_t sg_dma_addr;               /* 13 */
	RESERVED(1, uint32_t)                     /* 14 */
	__IOM uint32_t sg_setting;                /* 15 */
	__IOM uint32_t threshold;                 /* 16 */
	RESERVED(2, uint32_t)                     /* 17 */
	__IOM uint32_t gdma_read_timeout;         /* 18 */
	__IOM uint32_t gdma_write_timeout;        /* 19 */
	__IOM uint32_t ip_read_timeout;           /* 20 */
	__IOM uint32_t ip_write_timeout;          /* 21 */
	__IOM uint32_t write_cnt_debug;           /* 22 */
	__IOM uint32_t w_byte_en_debug;           /* 23 */
	__IOM uint32_t sw_reset_write_cnt_debug;  /* 24 */
	__IOM uint32_t sw_reset_read_cnt_debug;   /* 25 */
	RESERVED(3[6], uint32_t)                  /* 26~31 */
} GDMA_TypeDef;

typedef struct
{
    __IOM uint32_t control;           /*!< \brief timer control register for */
    __IOM uint32_t counter_val;       /*!< \brief 16-bit counter , it down to 0, the timer will generate the interrupt */
    __IOM uint32_t reload_val;        /*!< \brief when count reaches to 0, the value will reload to the count */
    __IOM uint32_t prescale_val;	  /*!< \brief 16-bit prescale for timer */
}TIM_TypeDef;

typedef struct
{
    __IOM uint32_t control;           /*!< \brief write command to this register to control the watchdog */
    __IOM uint32_t counter_val;       /*!< \brief 16-bit counter , it down to 0, the wathcdog will generate the interrupt */
}WDG_TypeDef;

typedef struct
{
  __IOM uint32_t type[7];              /*!< \brief  interrupt type 1:level-type, 0: edge-type */
  __IOM uint32_t polarity[7];          /*!< \brief  interrupt polarity 0:high-active(default) 1:low-active */
  __IOM uint32_t priority[7];          /*!< \brief  interrupt piority 0:fiq, 1:irq(default) */
  __IOM uint32_t mask[7];              /*!< \brief  mask interrupt bit 0:masked(default) 1:pass */
}IRQ_Ctrl_Type;

typedef struct {
	__IOM uint32_t C2M_normal_trigger;		//CA55 tiger the INT to CM4
	__IOM uint32_t C2M_write_monitor_status;
	__IOM uint32_t C2M_over_write_monitor_status;
	RESERVED(0, uint32_t);
	__IOM uint32_t C2M_normal_data[20];
	__IOM uint32_t C2M_direct_data[8];

	__IOM uint32_t M2C_normal_trigger;		//CM4 tiger the INT to CA55
	__IOM uint32_t M2C_write_monitor_status;
	__IOM uint32_t M2C_over_write_monitor_status;
	RESERVED(1, uint32_t);
	__IOM uint32_t M2C_normal_data[20];
	__IOM uint32_t M2C_direct_data[8];
}IPCC_Typedef;

#define STATIC_ASSERT(b) extern int _static_assert[b ? 1 : -1]

struct _PWM_DD_REG_ {
   union {
      struct {
         uint32_t dd             :18;//b(0-17)
         uint32_t :14;
      };
      __IOM uint32_t _pwm_dd_ctrl;
   };
};
STATIC_ASSERT(sizeof(struct _PWM_DD_REG_) == 4);

struct _PWM_DU_REG_ {
   union {
      struct {
         uint32_t pwm_duty       :12;   //b(0-11)
         uint32_t reserve_du     :4;    //b(12-15)
         uint32_t pwm_du_dd_sel  :2;    //b(16-17)
         uint32_t                :14;   //b(18-31)
      };
      __IOM uint32_t _pwm_chx_cfg;
   };
};
STATIC_ASSERT(sizeof(struct _PWM_DU_REG_) == 4);

typedef struct {
    union {
      struct {
         uint32_t pwm_en            :8;   //b(0-7)
         uint32_t pwm_bypass        :8;   //b(8-15)
         uint32_t reserve           :4;   //b(16-19)
         uint32_t pwm_resolution    :4;   //b(20-23)
         uint32_t :8;
      };
      __IOM uint32_t pwm_mode;
   };
    struct _PWM_DU_REG_ pwm_chx_cfg[4];
    __IOM uint32_t reserve_1[11];
    __IOM uint32_t pwm_version;
    struct _PWM_DD_REG_ pwm_dd_ctrl[4];
    __IOM uint32_t reserve_2[11];
}PWM_TypeDef;

#define REG_BASE                     0xF8000000

#define RF_GRP(_grp, _reg)           ((((_grp) * 32 + (_reg)) * 4) + REG_BASE)
#define RF_MASK_V(_mask, _val)       (((_mask) << 16) | (_val))
#define RF_MASK_V_SET(_mask)         (((_mask) << 16) | (_mask))
#define RF_MASK_V_CLR(_mask)         (((_mask) << 16) | 0)

/* MOON REG */
#define MOON0_REG                    ((volatile MOON_REG_Type *)RF_GRP(0, 0))
#define MOON1_REG                    ((volatile MOON_REG_Type *)RF_GRP(1, 0))
#define MOON2_REG                    ((volatile MOON_REG_Type *)RF_GRP(2, 0))
#define MOON3_REG                    ((volatile MOON_REG_Type *)RF_GRP(3, 0))
#define MOON4_REG                    ((volatile MOON_REG_Type *)RF_GRP(4, 0))


/********************  Bit definition for MON register  ********************/
#define CM4_FREQ_Pos (5)
#define CM4_FREQ_Msk (3U << CM4_FREQ_Pos) /*0x00000060*/



/* IRQ */
#define SP_IRQ_CTRL                  ((volatile IRQ_Ctrl_Type*) RF_GRP(9, 0))

/* IPCC Mailbox */
#define IPC_MAILBOX	                 ((volatile IPCC_Typedef *)RF_GRP(255, 0))

/*SP645 module clock enable bit*/
#define CLK_EN                       ((Module_Clock_En_type*)RF_GRP(0, 1))

/*SP645 module clock enable bit*/
#define CLK_GATE                     ((Module_Clock_Gate_Type*)RF_GRP(0, 11))

/*SP645 module rest bit*/
#define MODULE_REST                  ((Module_Reset_Type*)RF_GRP(0, 21))


/********************  Bit definition for STC register  ********************/
/*  STC register */
#define _STC_BASE                    RF_GRP(12, 0)
#define _STC0_BASE                   RF_GRP(96, 0)
#define _STC1_BASE                   RF_GRP(97, 0)
#define _STC2_BASE                   RF_GRP(99, 0)

#define STC0                         ((STC_TypeDef *)_STC_BASE)
#define STC1                         ((STC_TypeDef *)_STC0_BASE)
#define STC2                         ((STC_TypeDef *)_STC1_BASE)
#define STC3                         ((STC_TypeDef *)_STC2_BASE)
#define STC_PRESCALER_Pos            (0U)
#define STC_PRESCALER_Msk            (0x7FFFU << STC_PRESCALER_Pos)    /*0x000007FF*/
#define STC_PRESCALER                STC_PRESCALER_Msk                 /*STC prescale value */
#define STC_TRIG_SRC_Pos             (15U)
#define STC_TRIG_SRC_Msk             (1U << STC_TRIG_SRC_Pos)          /*0x00008000*/
#define STC_TRIG_SRC                 STC_TRIG_SRC_Msk                  /*The triger source of the STC counter. 0:system clock; 1:externl clock*/
#define STC_EXT_DIV_Pos              (0U)
#define STC_EXT_DIV_Msk              (0xFFU << STC_EXT_DIV_Pos)        /*0x000000FF*/
#define STC_EXT_DIV                  (STC_EXT_DIV_Msk)                 /*The external clock divder for STC*/

/****************** STC Instances : All supported instances *******************/
#define IS_STC_INSTANCE(INSTANCE)    (((INSTANCE) == STC0)   || \
                                      ((INSTANCE) == STC1)   || \
                                      ((INSTANCE) == STC2)   || \
                                      ((INSTANCE) == STC3))

#define IS_SPI_ALL_INSTANCE(__INSTANCE__) (((__INSTANCE__) == SPI0) || \
                                           ((__INSTANCE__) == SPI1) || \
                                           ((__INSTANCE__) == SPI2) || \
                                           ((__INSTANCE__) == SPI3))


/******************************************************************************/
/*                                                                            */
/*                        timer/counter                                       */
/*                                                                            */
/******************************************************************************/

/* Timer register */
#define _REG_GROUP_SIZE              (0x80)
#define _OFFSET_BETWEEN_TIMERS       (4*4)

#define TIM0_BASE                    RF_GRP(12, 18)
#define TIM1_BASE                    RF_GRP(12, 22)

/*TIMER2 & TIMER3 is a group*/
#define TIM2_BASE                    RF_GRP(96, 18)
#define TIM3_BASE                    RF_GRP(96, 22)

/*TIMER4 & TIMER5 is a group*/
#define TIM4_BASE                    RF_GRP(97, 18)
#define TIM5_BASE                    RF_GRP(97, 22)

/*TIMER6 & TIMER7 is a group*/
#define TIM6_BASE                    RF_GRP(99, 18)
#define TIM7_BASE                    RF_GRP(99, 22)

#define TIM0                         ((TIM_TypeDef *) TIM0_BASE)
#define TIM1                         ((TIM_TypeDef *) TIM1_BASE)
#define TIM2                         ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                         ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                         ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                         ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                         ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                         ((TIM_TypeDef *) TIM7_BASE)

/********************  Bit definition for timer register  ********************/
#define TIMER_GO_Pos                 (0)
#define TIMER_GO_Msk                 (1U << TIMER_GO_Pos)               /*0x00000001*/
#define TIMER_GO                     TIMER_GO_Msk                       /*timer go. 1:start;0:stop*/
#define TIMER_RPT_Pos                (1U)
#define TIMER_RPT_Msk                (1U << TIMER_RPT_Pos)              /*0x00000002*/
#define TIMER_RPT                    TIMER_RPT_Msk                      /*timer repeat operation. 0:one shot;1: repeation*/
#define TIMER_TRIG_SRC_Pos           (2U)
#define TIMER_TRIG_SRC_Msk           (0x7U << TIMER_TRIG_SRC_Pos)       /*0x00003000*/
#define TIMER_TRIG_SRC               TIMER_TRIG_SRC_Msk                 /*timer triger source selection. 0:systemclock triger;1:stc triger;2:rtc_triger;3:external clk triger;*/


/****************** TIM Instances : All supported instances *******************/
#define IS_TIM_INSTANCE(INSTANCE)    (((INSTANCE) == TIM0)   || ((INSTANCE) == TIM1) ||  ((INSTANCE) == TIM2)  ||  ((INSTANCE) == TIM3)    ||\
                                      ((INSTANCE) == TIM4)   || ((INSTANCE) == TIM5)  ||  ((INSTANCE) == TIM6)  ||  ((INSTANCE) == TIM7)   )

/******************************************************************************/
/*                                                                            */
/*                        watchdog module                                     */
/*                                                                            */
/******************************************************************************/
#define WDG0_BASE                   RF_GRP(12, 10)
#define WDG1_BASE                   RF_GRP(96, 10)
#define WDG2_BASE                   RF_GRP(97, 10)

#define WDG0                         ((WDG_TypeDef *) WDG0_BASE)
#define WDG1                         ((WDG_TypeDef *) WDG1_BASE)
#define WDG2                         ((WDG_TypeDef *) WDG2_BASE)

/****************** WDG Instances : All supported instances *******************/
#define IS_WDG_INSTANCE(INSTANCE)    (((INSTANCE) == WDG0)   || \
                                      ((INSTANCE) == WDG1)   || \
                                      ((INSTANCE) == WDG2))

/******************************************************************************/
/*                                                                            */
/*                        pwm module                                          */
/*                                                                            */
/******************************************************************************/

#define PWM_BASE                      RF_GRP(287, 0)
#define PWM_REG                       ((PWM_TypeDef *)PWM_BASE)

/******************************************************************************/
/*                        UART module                                          */
/******************************************************************************/

#define UART0_BASE      RF_GRP(18, 0)
#define UART1_BASE      RF_GRP(19, 0)
#define UART2_BASE      RF_GRP(16, 0)
#define UART3_BASE      RF_GRP(135, 0)
#define UART6_BASE      RF_GRP(261, 0)
#define UART7_BASE      RF_GRP(262, 0)


#define SP_UART0        ((UART_CTRL_Type *)UART0_BASE)
#define SP_UART1        ((UART_CTRL_Type *)UART1_BASE)
#define SP_UART2        ((UART_CTRL_Type *)UART2_BASE)
#define SP_UART3        ((UART_CTRL_Type *)UART3_BASE)
#define SP_UART4        ((UART_CTRL_Type *)0)/* define it for compile error, not used */
#define SP_UART6        ((UART_CTRL_Type *)UART6_BASE)
#define SP_UART7        ((UART_CTRL_Type *)UART7_BASE)

/*
UART LCR register BIT
*/
#define LCR_VOTE_BIT                (0x3 << 6)
#define LCR_BREAK_CONDITION_BIT     (0x1 << 5)
#define LCR_PARITY_BIT              (0x1 << 4)
#define LCR_PARITY_ENABLE_BIT       (0x1 << 3)
#define LCR_STOP_BIT_BIT            (0x1 << 2)
#define LCR_WORD_LEN_BIT            (0x3 << 0)

/* UART MCR register BIT */
#define MCR_AUTO_CALIBRATE_BIT         (0x1 << 7)
#define MCR_AUTO_CTS_BIT               (0x1 << 6)
#define MCR_AUTO_RTS_BIT               (0x1 << 5)
#define MCR_LOOP_BACK_BIT              (0x1 << 4)
#define MCR_RING_INDICATOR_BIT         (0x1 << 3)
#define MCR_DCD_BIT                    (0x1 << 2)
#define MCR_RTS_BIT                    (0x1 << 1)
#define MCR_DTS_BIT                    (0x1 << 0)

/* interrupt */
#define SP_UART_LSR_BC     (1 << 5) /* break condition */
#define SP_UART_LSR_FE     (1 << 4) /* frame error */
#define SP_UART_LSR_OE     (1 << 3) /* overrun error */
#define SP_UART_LSR_PE     (1 << 2) /* parity error */
#define SP_UART_LSR_RX     (1 << 1) /* 1: receive fifo not empty */
#define SP_UART_LSR_TX     (1 << 0) /* 1: transmit fifo is not full */

/* isc */
#define SP_UART_ISC_MSM    (1 << 7) /* Modem status ctrl */
#define SP_UART_ISC_LSM    (1 << 6) /* Line status interrupt */
#define SP_UART_ISC_RXM    (1 << 5) /* RX interrupt, when got some input data */
#define SP_UART_ISC_TXM    (1 << 4) /* TX interrupt, when trans start */
#define HAS_UART_ISC_FLAGMASK 0x0F
#define SP_UART_ISC_MS     (1 << 3)
#define SP_UART_ISC_LS     (1 << 2)
#define SP_UART_ISC_RX     (1 << 1)
#define SP_UART_ISC_TX     (1 << 0)

/* DMA-RX, dma_enable_sel */
#define DMA_INT            (1 << 31)
#define DMA_MSI_ID_SHIFT   12
#define DMA_MSI_ID_MASK    (0x7F << DMA_MSI_ID_SHIFT)
#define DMA_SEL_UARTX_SHIFT   8
#define DMA_SEL_UARTX_MASK (0x07 << DMA_SEL_UARTX_SHIFT)
#define DMA_SW_RST_B    (1 << 7)
#define DMA_INIT        (1 << 6)
#define DMA_GO          (1 << 5)
#define DMA_AUTO_ENABLE    (1 << 4)
#define DMA_TIMEOUT_INT_EN (1 << 3)
#define DMA_P_SAFE_DISABLE (1 << 2)
#define DMA_PBUS_DATA_SWAP (1 << 1)
#define DMA_ENABLE         (1 << 0)

#define UART_LSR_TRANSMIT_FIFO_STATUS        (1 << 0)
#define UART_LSR_RECEIVE_FIFO_STATUS         (1 << 1)



#define UART_TXDMA0        ((UART_Txdma *)RF_GRP(276, 0))
#define UART_TXDMA1        ((UART_Txdma *)RF_GRP(276, 16))
#define UART_TXDMA2        ((UART_Txdma *)RF_GRP(279, 0))
#define UART_TXDMA3        ((UART_Txdma *)RF_GRP(279, 16))


#define UART_TXGDMA0        ((UART_TxGdma *)RF_GRP(273, 0))
#define UART_TXGDMA1        ((UART_TxGdma *)RF_GRP(274, 0))
#define UART_TXGDMA2        ((UART_TxGdma *)RF_GRP(277, 0))
#define UART_TXGDMA3        ((UART_TxGdma *)RF_GRP(278, 0))

#define UART_RXDMA0        ((UART_Rxdma *)RF_GRP(275, 0))
#define UART_RXDMA1        ((UART_Rxdma *)RF_GRP(275, 16))
#define UART_RXDMA2        ((UART_Rxdma *)RF_GRP(270, 0))
#define UART_RXDMA3        ((UART_Rxdma *)RF_GRP(270, 16))

#define IS_UART_INSTANCE(__INSTANCE__) (((__INSTANCE__) == SP_UART0) || \
                                    ((__INSTANCE__) == SP_UART1) || \
                                    ((__INSTANCE__) == SP_UART2) || \
                                    ((__INSTANCE__) == SP_UART3) || \
                                    ((__INSTANCE__) == SP_UART6)  || \
                                    ((__INSTANCE__) == SP_UART7))

#define IS_UART_TXDMA_INSTANCE(__INSTANCE__) (((__INSTANCE__) == UART_TXDMA0) || \
                                    ((__INSTANCE__) == UART_TXDMA1) || \
                                    ((__INSTANCE__) == UART_TXDMA2) || \
                                    ((__INSTANCE__) == UART_TXDMA3))

#define IS_UART_TXGDMA_INSTANCE(__INSTANCE__) (((__INSTANCE__) == UART_TXGDMA0) || \
                                    ((__INSTANCE__) == UART_TXGDMA1) || \
                                    ((__INSTANCE__) == UART_TXGDMA2) || \
                                    ((__INSTANCE__) == UART_TXGDMA3))

#define IS_UART_RXDMA_INSTANCE(__INSTANCE__) (((__INSTANCE__) == UART_RXDMA0) || \
                                    ((__INSTANCE__) == UART_RXDMA1) || \
                                    ((__INSTANCE__) == UART_RXDMA2) || \
                                    ((__INSTANCE__) == UART_RXDMA3))

/******************************************************************************/
/*                        SPI module                                          */
/******************************************************************************/
#define SPI0_BASE      RF_GRP(89, 0)
#define SPI1_BASE      RF_GRP(91, 0)
#define SPI2_BASE      RF_GRP(93, 0)
#define SPI3_BASE      RF_GRP(128, 0)
#define SPI4_BASE      RF_GRP(130, 0)
#define SPI5_BASE      RF_GRP(132, 0)

#define SPI0        ((volatile SPI_TypeDef *)SPI0_BASE)
#define SPI1        ((volatile SPI_TypeDef *)SPI1_BASE)
#define SPI2        ((volatile SPI_TypeDef *)SPI2_BASE)
#define SPI3        ((volatile SPI_TypeDef *)SPI3_BASE)
#define SPI4        ((volatile SPI_TypeDef *)SPI4_BASE)
#define SPI5        ((volatile SPI_TypeDef *)SPI5_BASE)

#define CLK_DIVIDER(x)         (x<<16)
#define FINISH_FLAG_MASK       (1<<15)
#define RX_FULL_FLAG_MASK      (1<<14)
#define RX_EMP_FLAG_MASK       (1<<13)
#define TX_FULL_FLAG_MASK      (1<<12)
#define TX_EMP_FLAG_MASK       (1<<11)
#define WRITE_BYTE(x)          (x<<9)
#define READ_BYTE(x)           (x<<7)
#define FD_SEL                 (1<<6)
#define CS_SEL                 (1<<5)
#define LSB_SEL                (1<<4)
#define DELAY_ENABLE           (1<<3)
#define CPHA_W                 (1<<2)
#define CPHA_R                 (1<<1)
#define CPOL                   (1<<0)

#define CLEAR_MASTER_INT      (1<<6)

#define GET_LEN(x)            ((x>>9) & 0xFF)
#define GET_TX_LEN(x)         ((x>>1) & 0xFF)
#define GET_RX_CNT(x)         ((x>>14) & 0x0F)
#define GET_TX_CNT(x)         ((x>>8)  & 0x0F)

#define TOTAL_LENGTH(x)        (x<<9)
#define TX_LENGTH(x)           (x<<1)
#define RX_CNT                 (0x3F<<14)
#define TX_CNT                 (0x3F<<8)
#define SPI_BUSY               (1<<7)
#define FINISH_FLAG            (1<<6)
#define RX_FULL_FLAG           (1<<5)
#define RX_EMP_FLAG            (1<<4)
#define TX_FULL_FLAG           (1<<3)
#define TX_EMP_FLAG            (1<<2)
#define SPI_SW_RST             (1<<1)
#define SPI_START_FD           (1<<0)

#define SPI_TOTAL_SIZE         TOTAL_LENGTH(0xFF)
#define SPI_TX_SIZE            TX_LENGTH(0xFF)

/******************************************************************************/
/*                        I2C module                                          */
/******************************************************************************/
#define SP_I2CM0 	((I2C_TypeDef *)RF_GRP(140, 0))
#define SP_I2CM1 	((I2C_TypeDef *)RF_GRP(142, 0))
#define SP_I2CM2 	((I2C_TypeDef *)RF_GRP(144, 0))
#define SP_I2CM3 	((I2C_TypeDef *)RF_GRP(146, 0))
#define SP_I2CM4 	((I2C_TypeDef *)RF_GRP(508, 0))
#define SP_I2CM5 	((I2C_TypeDef *)RF_GRP(510, 0))

#define SP_GDMA0 	((GDMA_TypeDef *)RF_GRP(141, 0))
#define SP_GDMA1 	((GDMA_TypeDef *)RF_GRP(143, 0))
#define SP_GDMA2 	((GDMA_TypeDef *)RF_GRP(145, 0))
#define SP_GDMA3 	((GDMA_TypeDef *)RF_GRP(147, 0))
#define SP_GDMA4 	((GDMA_TypeDef *)RF_GRP(509, 0))
#define SP_GDMA5 	((GDMA_TypeDef *)RF_GRP(511, 0))
/******************************************************************************/
/*                                                                            */
/*                        gpio/pinmux define                                  */
/*                                                                            */
/******************************************************************************/
/* gpio register */

#define GPIO_I_PD(X)                 (RF_GRP(101, (0+X)))
#define GPIO_I_PU(X)                 (RF_GRP(102, (24+X)))
#define GPIO_O_DS(DS,X)              (RF_GRP(101, (4+DS*4+X)))

#define GPIO_FIRST(X)                (RF_GRP(101, (25+X)))
#define GPIO_MASTER(X)               (RF_GRP(5, (0+X)))
#define GPIO_OE(X)                   (RF_GRP(5, (13+X)))
#define GPIO_OUT(X)                  (RF_GRP(5, (26+X)))
#define GPIO_IN(X)                   (RF_GRP(6, (7+X)))
#define GPIO_I_INV(X)                (RF_GRP(6, (15+X)))
#define GPIO_O_INV(X)                (RF_GRP(6, (28+X)))
#define GPIO_OD(X)                   (RF_GRP(7, (9+X)))

#define GPIO_MIN                     0
#define GPIO_MAX                     107

#define GPIO(X)                      (X)

#define IS_GPIO_PIN(pin)             (((pin) >= GPIO_MIN) && ((pin) <= GPIO_MAX))
#define IS_PINMUX_PIN(pin)           (((pin) >= GPIO_MIN) && ((pin) <= GPIO_MAX))
#define IS_VALID_PINMUX(pin)         (((pin) >= GPIO_MIN) && ((pin) <= GPIO_MAX))
#define GPIO_TO_PINMUX(x)            IS_PINMUX_PIN(x)?(x):-1



/******************************************************************************/
/*                                                                            */
/*                       IPC Mailbox  		                              */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for IPC Mailbox register  ***************/


#define IS_IPCC_INSTANCE(INSTANCE)       (((INSTANCE) == IPC_MAILBOX))


#define NORMAL_LOCK_DATA0_Pos	(4)
#define NORMAL_LOCK_DATA0_Msk	(1<< NORMAL_LOCK_DATA0_Pos)
#define NORMAL_LOCK_DATA0		NORMAL_LOCK_DATA0_Msk

#define NORMAL_LOCK_DATA1_Pos	(5)
#define NORMAL_LOCK_DATA1_Msk	(1<< NORMAL_LOCK_DATA1_Pos)
#define NORMAL_LOCK_DATA1		NORMAL_LOCK_DATA1_Msk

#define NORMAL_LOCK_DATA2_Pos	(6)
#define NORMAL_LOCK_DATA2_Msk	(1<< NORMAL_LOCK_DATA2_Pos)
#define NORMAL_LOCK_DATA2		NORMAL_LOCK_DATA2_Msk

#define NORMAL_LOCK_DATA3_Pos	(7)
#define NORMAL_LOCK_DATA3_Msk	(1<< NORMAL_LOCK_DATA3_Pos)
#define NORMAL_LOCK_DATA3		NORMAL_LOCK_DATA3_Msk

#define NORMAL_LOCK_DATA4_Pos	(8)
#define NORMAL_LOCK_DATA4_Msk	(1<< NORMAL_LOCK_DATA4_Pos)
#define NORMAL_LOCK_DATA4		NORMAL_LOCK_DATA4_Msk

#define NORMAL_LOCK_DATA5_Pos	(9)
#define NORMAL_LOCK_DATA5_Msk	(1<< NORMAL_LOCK_DATA5_Pos)
#define NORMAL_LOCK_DATA5		NORMAL_LOCK_DATA5_Msk

#define NORMAL_LOCK_DATA6_Pos	(10)
#define NORMAL_LOCK_DATA6_Msk	(1<< NORMAL_LOCK_DATA6_Pos)
#define NORMAL_LOCK_DATA6		NORMAL_LOCK_DATA6_Msk

#define NORMAL_LOCK_DATA7_Pos	(11)
#define NORMAL_LOCK_DATA7_Msk	(1<< NORMAL_LOCK_DATA7_Pos)
#define NORMAL_LOCK_DATA7		NORMAL_LOCK_DATA7_Msk

#define NORMAL_LOCK_DATA8_Pos	(12)
#define NORMAL_LOCK_DATA8_Msk	(1<< NORMAL_LOCK_DATA8_Pos)
#define NORMAL_LOCK_DATA8		NORMAL_LOCK_DATA8_Msk

#define NORMAL_LOCK_DATA9_Pos	(13)
#define NORMAL_LOCK_DATA9_Msk	(1<< NORMAL_LOCK_DATA9_Pos)
#define NORMAL_LOCK_DATA9		NORMAL_LOCK_DATA9_Msk

#define NORMAL_LOCK_DATA10_Pos	(14)
#define NORMAL_LOCK_DATA10_Msk	(1<< NORMAL_LOCK_DATA10_Pos)
#define NORMAL_LOCK_DATA10		NORMAL_LOCK_DATA10_Msk

#define NORMAL_LOCK_DATA11_Pos	(15)
#define NORMAL_LOCK_DATA11_Msk	(1<< NORMAL_LOCK_DATA11_Pos)
#define NORMAL_LOCK_DATA11		NORMAL_LOCK_DATA11_Msk

#define NORMAL_LOCK_DATA12_Pos	(16)
#define NORMAL_LOCK_DATA12_Msk	(1<< NORMAL_LOCK_DATA12_Pos)
#define NORMAL_LOCK_DATA12		NORMAL_LOCK_DATA12_Msk

#define NORMAL_LOCK_DATA13_Pos	(17)
#define NORMAL_LOCK_DATA13_Msk	(1<< NORMAL_LOCK_DATA13_Pos)
#define NORMAL_LOCK_DATA13		NORMAL_LOCK_DATA13_Msk

#define NORMAL_LOCK_DATA14_Pos	(18)
#define NORMAL_LOCK_DATA14_Msk	(1<< NORMAL_LOCK_DATA14_Pos)
#define NORMAL_LOCK_DATA14		NORMAL_LOCK_DATA14_Msk

#define NORMAL_LOCK_DATA15_Pos	(19)
#define NORMAL_LOCK_DATA15_Msk	(1<< NORMAL_LOCK_DATA15_Pos)
#define NORMAL_LOCK_DATA15		NORMAL_LOCK_DATA15_Msk

#define NORMAL_LOCK_DATA16_Pos	(20)
#define NORMAL_LOCK_DATA16_Msk	(1<< NORMAL_LOCK_DATA16_Pos)
#define NORMAL_LOCK_DATA16		NORMAL_LOCK_DATA16_Msk

#define NORMAL_LOCK_DATA17_Pos	(21)
#define NORMAL_LOCK_DATA17_Msk	(1<< NORMAL_LOCK_DATA17_Pos)
#define NORMAL_LOCK_DATA17		NORMAL_LOCK_DATA17_Msk

#define NORMAL_LOCK_DATA18_Pos	(22)
#define NORMAL_LOCK_DATA18_Msk	(1<< NORMAL_LOCK_DATA18_Pos)
#define NORMAL_LOCK_DATA18		NORMAL_LOCK_DATA18_Msk

#define NORMAL_LOCK_DATA19_Pos	(23)
#define NORMAL_LOCK_DATA19_Msk	(1<< NORMAL_LOCK_DATA19_Pos)
#define NORMAL_LOCK_DATA19		NORMAL_LOCK_DATA19_Msk


#define DIRECT_LOCK_DATA0_Pos	(24)
#define DIRECT_LOCK_DATA0_Msk	(1<< DIRECT_LOCK_DATA0_Pos)
#define DIRECT_LOCK_DATA0		DIRECT_LOCK_DATA0_Msk

#define DIRECT_LOCK_DATA1_Pos	(25)
#define DIRECT_LOCK_DATA1_Msk	(1<< DIRECT_LOCK_DATA1_Pos)
#define DIRECT_LOCK_DATA1		DIRECT_LOCK_DATA1_Msk

#define DIRECT_LOCK_DATA2_Pos	(26)
#define DIRECT_LOCK_DATA2_Msk	(1<< DIRECT_LOCK_DATA2_Pos)
#define DIRECT_LOCK_DATA2		DIRECT_LOCK_DATA2_Msk

#define DIRECT_LOCK_DATA3_Pos	(27)
#define DIRECT_LOCK_DATA3_Msk	(1<< DIRECT_LOCK_DATA3_Pos)
#define DIRECT_LOCK_DATA3		DIRECT_LOCK_DATA3_Msk

#define DIRECT_LOCK_DATA4_Pos	(28)
#define DIRECT_LOCK_DATA4_Msk	(1<< DIRECT_LOCK_DATA4_Pos)
#define DIRECT_LOCK_DATA4		DIRECT_LOCK_DATA4_Msk

#define DIRECT_LOCK_DATA5_Pos	(29)
#define DIRECT_LOCK_DATA5_Msk	(1<< DIRECT_LOCK_DATA5_Pos)
#define DIRECT_LOCK_DATA5		DIRECT_LOCK_DATA5_Msk

#define DIRECT_LOCK_DATA6_Pos	(30)
#define DIRECT_LOCK_DATA6_Msk	(1<< DIRECT_LOCK_DATA6_Pos)
#define DIRECT_LOCK_DATA6		DIRECT_LOCK_DATA6_Msk

#define DIRECT_LOCK_DATA7_Pos	(31)
#define DIRECT_LOCK_DATA7_Msk	(1<< DIRECT_LOCK_DATA7_Pos)
#define DIRECT_LOCK_DATA7		DIRECT_LOCK_DATA7_Msk

#define NORMAL_OVERWRITE_DATA0_Pos	(4)
#define NORMAL_OVERWRITE_DATA0_Msk	(1<< NORMAL_OVERWRITE_DATA0_Pos)
#define NORMAL_OVERWRITE_DATA0		NORMAL_LOCK_DATA0_Msk

#define NORMAL_OVERWRITE_DATA1_Pos	(5)
#define NORMAL_OVERWRITE_DATA1_Msk	(1<< NORMAL_OVERWRITE_DATA1_Pos)
#define NORMAL_OVERWRITE_DATA1		NORMAL_LOCK_DATA1_Msk

#define NORMAL_OVERWRITE_DATA2_Pos	(6)
#define NORMAL_OVERWRITE_DATA2_Msk	(1<< NORMAL_OVERWRITE_DATA2_Pos)
#define NORMAL_OVERWRITE_DATA2		NORMAL_LOCK_DATA2_Msk

#define NORMAL_OVERWRITE_DATA3_Pos	(7)
#define NORMAL_OVERWRITE_DATA3_Msk	(1<< NORMAL_OVERWRITE_DATA3_Pos)
#define NORMAL_OVERWRITE_DATA3		NORMAL_LOCK_DATA3_Msk

#define NORMAL_OVERWRITE_DATA4_Pos	(8)
#define NORMAL_OVERWRITE_DATA4_Msk	(1<< NORMAL_OVERWRITE_DATA4_Pos)
#define NORMAL_OVERWRITE_DATA4		NORMAL_LOCK_DATA4_Msk

#define NORMAL_OVERWRITE_DATA5_Pos	(9)
#define NORMAL_OVERWRITE_DATA5_Msk	(1<< NORMAL_OVERWRITE_DATA5_Pos)
#define NORMAL_OVERWRITE_DATA5		NORMAL_LOCK_DATA5_Msk

#define NORMAL_OVERWRITE_DATA6_Pos	(10)
#define NORMAL_OVERWRITE_DATA6_Msk	(1<< NORMAL_OVERWRITE_DATA6_Pos)
#define NORMAL_OVERWRITE_DATA6		NORMAL_LOCK_DATA6_Msk

#define NORMAL_OVERWRITE_DATA7_Pos	(11)
#define NORMAL_OVERWRITE_DATA7_Msk	(1<< NORMAL_OVERWRITE_DATA7_Pos)
#define NORMAL_OVERWRITE_DATA7		NORMAL_LOCK_DATA7_Msk

#define NORMAL_OVERWRITE_DATA8_Pos	(12)
#define NORMAL_OVERWRITE_DATA8_Msk	(1<< NORMAL_OVERWRITE_DATA8_Pos)
#define NORMAL_OVERWRITE_DATA8		NORMAL_LOCK_DATA8_Msk

#define NORMAL_OVERWRITE_DATA9_Pos	(13)
#define NORMAL_OVERWRITE_DATA9_Msk	(1<< NORMAL_OVERWRITE_DATA9_Pos)
#define NORMAL_OVERWRITE_DATA9		NORMAL_LOCK_DATA9_Msk

#define NORMAL_OVERWRITE_DATA10_Pos	(14)
#define NORMAL_OVERWRITE_DATA10_Msk	(1<< NORMAL_OVERWRITE_DATA10_Pos)
#define NORMAL_OVERWRITE_DATA10		NORMAL_LOCK_DATA10_Msk

#define NORMAL_OVERWRITE_DATA11_Pos	(15)
#define NORMAL_OVERWRITE_DATA11_Msk	(1<< NORMAL_OVERWRITE_DATA11_Pos)
#define NORMAL_OVERWRITE_DATA11		NORMAL_LOCK_DATA11_Msk

#define NORMAL_OVERWRITE_DATA12_Pos	(16)
#define NORMAL_OVERWRITE_DATA12_Msk	(1<< NORMAL_OVERWRITE_DATA12_Pos)
#define NORMAL_OVERWRITE_DATA12		NORMAL_LOCK_DATA12_Msk

#define NORMAL_OVERWRITE_DATA13_Pos	(17)
#define NORMAL_OVERWRITE_DATA13_Msk	(1<< NORMAL_OVERWRITE_DATA13_Pos)
#define NORMAL_OVERWRITE_DATA13		NORMAL_LOCK_DATA13_Msk

#define NORMAL_OVERWRITE_DATA14_Pos	(18)
#define NORMAL_OVERWRITE_DATA14_Msk	(1<< NORMAL_OVERWRITE_DATA14_Pos)
#define NORMAL_OVERWRITE_DATA14		NORMAL_LOCK_DATA14_Msk

#define NORMAL_OVERWRITE_DATA15_Pos	(19)
#define NORMAL_OVERWRITE_DATA15_Msk	(1<< NORMAL_OVERWRITE_DATA15_Pos)
#define NORMAL_OVERWRITE_DATA15		NORMAL_LOCK_DATA15_Msk

#define NORMAL_OVERWRITE_DATA16_Pos	(20)
#define NORMAL_OVERWRITE_DATA16_Msk	(1<< NORMAL_OVERWRITE_DATA16_Pos)
#define NORMAL_OVERWRITE_DATA16		NORMAL_LOCK_DATA16_Msk

#define NORMAL_OVERWRITE_DATA17_Pos	(21)
#define NORMAL_OVERWRITE_DATA17_Msk	(1<< NORMAL_OVERWRITE_DATA17_Pos)
#define NORMAL_OVERWRITE_DATA17		NORMAL_LOCK_DATA17_Msk

#define NORMAL_OVERWRITE_DATA18_Pos	(22)
#define NORMAL_OVERWRITE_DATA18_Msk	(1<< NORMAL_OVERWRITE_DATA18_Pos)
#define NORMAL_OVERWRITE_DATA18		NORMAL_LOCK_DATA18_Msk

#define NORMAL_OVERWRITE_DATA19_Pos	(23)
#define NORMAL_OVERWRITE_DATA19_Msk	(1<< NORMAL_OVERWRITE_DATA19_Pos)
#define NORMAL_OVERWRITE_DATA19		NORMAL_LOCK_DATA19_Msk


#define DIRECT_OVERWITE_DATA0_Pos	(24)
#define DIRECT_OVERWITE_DATA0_Msk	(1<< DIRECT_OVERWITE_DATA0_Pos)
#define DIRECT_OVERWITE_DATA0		DIRECT_OVERWITE_DATA0_Msk

#define DIRECT_OVERWITE_DATA1_Pos	(25)
#define DIRECT_OVERWITE_DATA1_Msk	(1<< DIRECT_OVERWITE_DATA1_Pos)
#define DIRECT_OVERWITE_DATA1		DIRECT_OVERWITE_DATA1_Msk

#define DIRECT_OVERWITE_DATA2_Pos	(26)
#define DIRECT_OVERWITE_DATA2_Msk	(1<< DIRECT_OVERWITE_DATA2_Pos)
#define DIRECT_OVERWITE_DATA2		DIRECT_OVERWITE_DATA2_Msk

#define DIRECT_OVERWITE_DATA3_Pos	(27)
#define DIRECT_OVERWITE_DATA3_Msk	(1<< DIRECT_OVERWITE_DATA3_Pos)
#define DIRECT_OVERWITE_DATA3		DIRECT_OVERWITE_DATA3_Msk

#define DIRECT_OVERWITE_DATA4_Pos	(28)
#define DIRECT_OVERWITE_DATA4_Msk	(1<< DIRECT_OVERWITE_DATA4_Pos)
#define DIRECT_OVERWITE_DATA4		DIRECT_OVERWITE_DATA4_Msk

#define DIRECT_OVERWITE_DATA5_Pos	(29)
#define DIRECT_OVERWITE_DATA5_Msk	(1<< DIRECT_OVERWITE_DATA5_Pos)
#define DIRECT_OVERWITE_DATA5		DIRECT_OVERWITE_DATA5_Msk

#define DIRECT_OVERWITE_DATA6_Pos	(30)
#define DIRECT_OVERWITE_DATA6_Msk	(1<< DIRECT_OVERWITE_DATA6_Pos)
#define DIRECT_OVERWITE_DATA6		DIRECT_OVERWITE_DATA6_Msk

#define DIRECT_OVERWITE_DATA7_Pos	(31)
#define DIRECT_OVERWITE_DATA7_Msk	(1<< DIRECT_OVERWITE_DATA7_Pos)
#define DIRECT_OVERWITE_DATA7		DIRECT_OVERWITE_DATA7_Msk

/* MBOX */
#define MBOX_TO_NOTIFY		(*(volatile unsigned int *)RF_GRP(256, 31)) /* MBOX_CM4_TO_CA55_DIRECT[7] */
#define MBOX_NOTIFICATION	(*(volatile unsigned int *)RF_GRP(255, 31)) /* MBOX_CA55_TO_CM4_DIRECT[7] */
#define IRQ_NOTIFICATION	(IPC_CA552CM4_INT7_IRQn) /* ca55 to cm4 */

/******************************************************************************/
/*                                                                            */
/*                       Audio  		                              */
/*                                                                            */
/******************************************************************************/
typedef struct {
	// Group 060 : AUD
	__IOM uint32_t  audif_ctrl                            ; // 00
    	__IOM uint32_t  aud_enable                            ; // 01
    	__IOM uint32_t  pcm_cfg                               ; // 02
    	__IOM uint32_t  i2s_mute_flag_ctrl                    ; // 03
    	__IOM uint32_t  ext_adc_cfg                           ; // 04
    	__IOM uint32_t  int_dac_ctrl0                         ; // 05
    	__IOM uint32_t  int_adc_ctrl                          ; // 06
    	__IOM uint32_t  adc_in_path_switch                    ; // 07
    	__IOM uint32_t  int_adc_dac_cfg                       ; // 08
    	__IOM uint32_t  G060_reserved_9                       ; // 09
    	__IOM uint32_t  iec_cfg                               ; // 10
    	__IOM uint32_t  iec0_valid_out                        ; // 11
    	__IOM uint32_t  iec0_par0_out                         ; // 12
    	__IOM uint32_t  iec0_par1_out                         ; // 13
    	__IOM uint32_t  iec1_valid_out                        ; // 14
    	__IOM uint32_t  iec1_par0_out                         ; // 15
    	__IOM uint32_t  iec1_par1_out                         ; // 16
    	__IOM uint32_t  iec0_rx_debug_info                    ; // 17
    	__IOM uint32_t  iec0_valid_in                         ; // 18
    	__IOM uint32_t  iec0_par0_in                          ; // 19
    	__IOM uint32_t  iec0_par1_in                          ; // 20
   	__IOM uint32_t  iec1_rx_debug_info                    ; // 21
    	__IOM uint32_t  iec1_valid_in                         ; // 22
    	__IOM uint32_t  iec1_par0_in                          ; // 23
    	__IOM uint32_t  iec1_par1_in                          ; // 24
    	__IOM uint32_t  iec2_rx_debug_info                    ; // 25
    	__IOM uint32_t  iec2_valid_in                         ; // 26
    	__IOM uint32_t  iec2_par0_in                          ; // 27
    	__IOM uint32_t  iec2_par1_in                          ; // 28
    	__IOM uint32_t  G060_reserved_29                      ; // 29
    	__IOM uint32_t  iec_tx_user_wdata                     ; // 30
    	__IOM uint32_t  iec_tx_user_ctrl                      ; // 31

    	// Group 061 : AUD
    	__IOM uint32_t  adcp_ch_enable                        ; // 00, ADCPRC Configuration Group 1
    	__IOM uint32_t  adcp_fubypass                         ; // 01, ADCPRC Configuration Group 2
   	__IOM uint32_t  adcp_mode_ctrl                        ; // 02, ADCPRC Mode Control
    	__IOM uint32_t  adcp_init_ctrl                        ; // 03, ADCP Initialization Control
    	__IOM uint32_t  adcp_coeff_din                        ; // 04, Coefficient Data Input
    	__IOM uint32_t  adcp_agc_cfg                          ; // 05, ADCPRC AGC Configuration of Ch0/1
    	__IOM uint32_t  adcp_agc_cfg2                         ; // 06, ADCPRC AGC Configuration of Ch2/3
    	__IOM uint32_t  adcp_gain_0                           ; // 07, ADCP System Gain1
    	__IOM uint32_t  adcp_gain_1                           ; // 08, ADCP System Gain2
    	__IOM uint32_t  adcp_gain_2                           ; // 09, ADCP System Gain3
    	__IOM uint32_t  adcp_gain_3                           ; // 10, ADCP System Gain4
    	__IOM uint32_t  adcp_risc_gain                        ; // 11, ADCP RISC Gain
    	__IOM uint32_t  adcp_mic_l                            ; // 12, ADCPRC Microphone - in Left Channel Data
    	__IOM uint32_t  adcp_mic_r                            ; // 13, ADCPRC Microphone - in Right Channel Data
    	__IOM uint32_t  adcp_agc_gain                         ; // 14, ADCPRC AGC Gain
    	__IOM uint32_t  G061_reserved_15                      ; // 15, Reserved
    	__IOM uint32_t  aud_apt_mode                          ; // 16, Audio Playback Timer Mode
    	__IOM uint32_t  aud_apt_data                          ; // 17, Audio Playback Timer
    	__IOM uint32_t  aud_apt_parameter                     ; // 18, Audio Playback Timer Parameter
    	__IOM uint32_t  G061_reserved_19                      ; // 19, Reserved
    	__IOM uint32_t  aud_audhwya                           ; // 20, DRAM Base Address Offset
    	__IOM uint32_t  aud_inc_0                             ; // 21, DMA Counter Increment/Decrement
    	__IOM uint32_t  aud_delta_0                           ; // 22, Delta Value
    	__IOM uint32_t  aud_fifo_enable                       ; // 23, Audio FIFO Enable
    	__IOM uint32_t  aud_fifo_mode                         ; // 24, FIFO Mode Control
    	__IOM uint32_t  aud_fifo_support                      ; // 25, Supported FIFOs ( Debug Function )
    	__IOM uint32_t  aud_fifo_reset                        ; // 26, Host FIFO Reset
    	__IOM uint32_t  aud_chk_ctrl                          ; // 27, Checksum Control ( Debug Function )
    	__IOM uint32_t  aud_checksum_data                     ; // 28, Checksum Data ( Debug Function )
    	__IOM uint32_t  aud_chk_tcnt                          ; // 29, Target Count of Checksum ( Debug Function )
    	__IOM uint32_t  aud_embedded_input_ctrl               ; // 30, Embedded Input Control ( Debug Function )
    	__IOM uint32_t  aud_misc_ctrl                         ; // 31, Miscellaneous Control

    	// Group 062 : AUD
    	__IOM uint32_t  aud_ext_dac_xck_cfg                   ; // 00
    	__IOM uint32_t  aud_ext_dac_bck_cfg                   ; // 01
    	__IOM uint32_t  aud_iec0_bclk_cfg                     ; // 02
    	__IOM uint32_t  aud_ext_adc_xck_cfg                   ; // 03
    	__IOM uint32_t  aud_ext_adc_bck_cfg                   ; // 04
    	__IOM uint32_t  aud_int_adc_xck_cfg                   ; // 05
    	__IOM uint32_t  G062_reserved_6                       ; // 06
    	__IOM uint32_t  aud_int_dac_xck_cfg                   ; // 07
    	__IOM uint32_t  aud_int_dac_bck_cfg                   ; // 08
    	__IOM uint32_t  aud_iec1_bclk_cfg                     ; // 09
    	__IOM uint32_t  G062_reserved_10                      ; // 10
    	__IOM uint32_t  aud_pcm_iec_bclk_cfg                  ; // 11
    	__IOM uint32_t  aud_xck_osr104_cfg                    ; // 12
    	__IOM uint32_t  aud_hdmi_tx_mclk_cfg                  ; // 13
    	__IOM uint32_t  aud_hdmi_tx_bclk_cfg                  ; // 14
    	__IOM uint32_t  hdmi_tx_i2s_cfg                       ; // 15
    	__IOM uint32_t  hdmi_rx_i2s_cfg                       ; // 16
    	__IOM uint32_t  aud_aadc_agc01_cfg0                   ; // 17
    	__IOM uint32_t  aud_aadc_agc01_cfg1                   ; // 18
    	__IOM uint32_t  aud_aadc_agc01_cfg2                   ; // 19
    	__IOM uint32_t  aud_aadc_agc01_cfg3                   ; // 20
    	__IOM uint32_t  int_adc_ctrl3                         ; // 21
    	__IOM uint32_t  int_adc_ctrl2                         ; // 22
    	__IOM uint32_t  int_dac_ctrl2                         ; // 23
    	__IOM uint32_t  G062_reserved_24                      ; // 24
    	__IOM uint32_t  int_dac_ctrl1                         ; // 25
    	__IOM uint32_t  G062_reserved_26                      ; // 26
    	__IOM uint32_t  G062_reserved_27                      ; // 27
    	__IOM uint32_t  G062_reserved_28                      ; // 28
    	__IOM uint32_t  G062_reserved_29                      ; // 29
    	__IOM uint32_t  G062_reserved_30                      ; // 30
    	__IOM uint32_t  G062_reserved_31                      ; // 31

    	// Group 063 : AUD
    	__IOM uint32_t  aud_bt_ifx_cfg                        ; // 00
    	__IOM uint32_t  aud_bt_i2s_cfg                        ; // 01
    	__IOM uint32_t  aud_bt_xck_cfg                        ; // 02
    	__IOM uint32_t  aud_bt_bck_cfg                        ; // 03
    	__IOM uint32_t  aud_bt_sync_cfg                       ; // 04
    	__IOM uint32_t  G063_reserved_5                       ; // 05
    	__IOM uint32_t  G063_reserved_6                       ; // 06
    	__IOM uint32_t  G063_reserved_7                       ; // 07
    	__IOM uint32_t  aud_pwm_xck_cfg                       ; // 08
    	__IOM uint32_t  aud_pwm_bck_cfg                       ; // 09
    	__IOM uint32_t  G063_reserved_10                      ; // 10
    	__IOM uint32_t  G063_reserved_11                      ; // 11
    	__IOM uint32_t  G063_reserved_12                      ; // 12
   	__IOM uint32_t  G063_reserved_13                      ; // 13
    	__IOM uint32_t  G063_reserved_14                      ; // 14
    	__IOM uint32_t  G063_reserved_15                      ; // 15
    	__IOM uint32_t  G063_reserved_16                      ; // 16
    	__IOM uint32_t  G063_reserved_17                      ; // 17
    	__IOM uint32_t  G063_reserved_18                      ; // 18
    	__IOM uint32_t  G063_reserved_19                      ; // 19
    	__IOM uint32_t  aud_aadc_agc2_cfg0                    ; // 20
    	__IOM uint32_t  aud_aadc_agc2_cfg1                    ; // 21
    	__IOM uint32_t  aud_aadc_agc2_cfg2                    ; // 22
    	__IOM uint32_t  aud_aadc_agc2_cfg3                    ; // 23
    	__IOM uint32_t  aud_opt_test_pat                      ; // 24
    	__IOM uint32_t  aud_sys_status0                       ; // 25
    	__IOM uint32_t  aud_sys_status1                       ; // 26
    	__IOM uint32_t  int_adc_ctrl1                         ; // 27
    	__IOM uint32_t  bt_mute_flag                          ; // 28
    	__IOM uint32_t  cdrpll_losd_ctrl                      ; // 29
    	__IOM uint32_t  G063_reserved_30                      ; // 30
    	__IOM uint32_t  other_config                          ; // 31

    	// Group 064 : AUD
    	__IOM uint32_t  aud_a0_base                           ; //
    	__IOM uint32_t  aud_a0_length                         ; //
    	__IOM uint32_t  aud_a0_ptr                            ; //
    	__IOM uint32_t  aud_a0_cnt                            ; //
    	__IOM uint32_t  aud_a1_base                           ; //
    	__IOM uint32_t  aud_a1_length                         ; //
    	__IOM uint32_t  aud_a1_ptr                            ; //
    	__IOM uint32_t  aud_a1_cnt                            ; //
    	__IOM uint32_t  aud_a2_base                           ; //
    	__IOM uint32_t  aud_a2_length                         ; //
    	__IOM uint32_t  aud_a2_ptr                            ; //
    	__IOM uint32_t  aud_a2_cnt                            ; //
    	__IOM uint32_t  aud_a3_base                           ; //
    	__IOM uint32_t  aud_a3_length                         ; //
    	__IOM uint32_t  aud_a3_ptr                            ; //
    	__IOM uint32_t  aud_a3_cnt                            ; //
    	__IOM uint32_t  aud_a4_base                           ; //
    	__IOM uint32_t  aud_a4_length                         ; //
    	__IOM uint32_t  aud_a4_ptr                            ; //
    	__IOM uint32_t  aud_a4_cnt                            ; //
    	__IOM uint32_t  aud_a5_base                           ; //
    	__IOM uint32_t  aud_a5_length                         ; //
    	__IOM uint32_t  aud_a5_ptr                            ; //
    	__IOM uint32_t  aud_a5_cnt                            ; //
    	__IOM uint32_t  aud_a6_base                           ; //
    	__IOM uint32_t  aud_a6_length                         ; //
    	__IOM uint32_t  aud_a6_ptr                            ; //
    	__IOM uint32_t  aud_a6_cnt                            ; //
    	__IOM uint32_t  aud_a7_base                           ; //
    	__IOM uint32_t  aud_a7_length                         ; //
    	__IOM uint32_t  aud_a7_ptr                            ; //
    	__IOM uint32_t  aud_a7_cnt                            ; //

    	// Group 065 : AUD
    	__IOM uint32_t  aud_a8_base                           ; //
    	__IOM uint32_t  aud_a8_length                         ; //
    	__IOM uint32_t  aud_a8_ptr                            ; //
    	__IOM uint32_t  aud_a8_cnt                            ; //
    	__IOM uint32_t  aud_a9_base                           ; //
    	__IOM uint32_t  aud_a9_length                         ; //
    	__IOM uint32_t  aud_a9_ptr                            ; //
    	__IOM uint32_t  aud_a9_cnt                            ; //
    	__IOM uint32_t  aud_a10_base                          ; //
    	__IOM uint32_t  aud_a10_length                        ; //
    	__IOM uint32_t  aud_a10_ptr                           ; //
    	__IOM uint32_t  aud_a10_cnt                           ; //
    	__IOM uint32_t  aud_a11_base                          ; //
    	__IOM uint32_t  aud_a11_length                        ; //
    	__IOM uint32_t  aud_a11_ptr                           ; //
    	__IOM uint32_t  aud_a11_cnt                           ; //
    	__IOM uint32_t  aud_a12_base                          ; //
    	__IOM uint32_t  aud_a12_length                        ; //
    	__IOM uint32_t  aud_a12_ptr                           ; //
    	__IOM uint32_t  aud_a12_cnt                           ; //
    	__IOM uint32_t  aud_a13_base                          ; //
    	__IOM uint32_t  aud_a13_length                        ; //
    	__IOM uint32_t  aud_a13_ptr                           ; //
    	__IOM uint32_t  aud_a13_cnt                           ; //
    	__IOM uint32_t  aud_a14_base                          ; //
    	__IOM uint32_t  aud_a14_length                        ; //
    	__IOM uint32_t  aud_a14_ptr                           ; //
    	__IOM uint32_t  aud_a14_cnt                           ; //
    	__IOM uint32_t  aud_a15_base                          ; //
    	__IOM uint32_t  aud_a15_length                        ; //
    	__IOM uint32_t  aud_a15_ptr                           ; //
    	__IOM uint32_t  aud_a15_cnt                           ; //

    	// Group 066 : AUD
    	__IOM uint32_t  aud_a16_base                          ; //
    	__IOM uint32_t  aud_a16_length                        ; //
    	__IOM uint32_t  aud_a16_ptr                           ; //
    	__IOM uint32_t  aud_a16_cnt                           ; //
    	__IOM uint32_t  aud_a17_base                          ; //
    	__IOM uint32_t  aud_a17_length                        ; //
    	__IOM uint32_t  aud_a17_ptr                           ; //
    	__IOM uint32_t  aud_a17_cnt                           ; //
    	__IOM uint32_t  aud_a18_base                          ; //
    	__IOM uint32_t  aud_a18_length                        ; //
    	__IOM uint32_t  aud_a18_ptr                           ; //
    	__IOM uint32_t  aud_a18_cnt                           ; //
    	__IOM uint32_t  aud_a19_base                          ; //
    	__IOM uint32_t  aud_a19_length                        ; //
    	__IOM uint32_t  aud_a19_ptr                           ; //
    	__IOM uint32_t  aud_a19_cnt                           ; //
    	__IOM uint32_t  aud_a20_base                          ; //
    	__IOM uint32_t  aud_a20_length                        ; //
    	__IOM uint32_t  aud_a20_ptr                           ; //
    	__IOM uint32_t  aud_a20_cnt                           ; //
    	__IOM uint32_t  aud_a21_base                          ; //
    	__IOM uint32_t  aud_a21_length                        ; //
    	__IOM uint32_t  aud_a21_ptr                           ; //
    	__IOM uint32_t  aud_a21_cnt                           ; //
    	__IOM uint32_t  aud_a22_base                          ; // 24
    	__IOM uint32_t  aud_a22_length                        ; // 25
    	__IOM uint32_t  aud_a22_ptr                           ; // 26
    	__IOM uint32_t  aud_a22_cnt                           ; // 27
    	__IOM uint32_t  aud_a23_base                          ; // 28
    	__IOM uint32_t  aud_a23_length                        ; // 29
    	__IOM uint32_t  aud_a23_ptr                           ; // 30
    	__IOM uint32_t  aud_a23_cnt                           ; // 31

    	// Group 067 : AUD
    	__IOM uint32_t  aud_grm_master_gain                   ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_0                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_1                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_2                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_3                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_4                ; // Gain Control
    	__IOM uint32_t  aud_grm_mix_control_0                 ; // Mixer Setting
    	__IOM uint32_t  aud_grm_mix_control_1                 ; // Mixer Setting
    	__IOM uint32_t  aud_grm_mix_control_2                 ; // Mixer Setting
    	__IOM uint32_t  aud_grm_switch_0                      ; // Channel Switch
    	__IOM uint32_t  aud_grm_switch_1                      ; // Channel Switch
    	__IOM uint32_t  aud_grm_switch_int                    ; // Channel Switch
    	__IOM uint32_t  aud_grm_delta_volume                  ; // Gain Update
    	__IOM uint32_t  aud_grm_delta_ramp_pcm                ; // Gain Update
    	__IOM uint32_t  aud_grm_delta_ramp_risc               ; // Gain Update
    	__IOM uint32_t  aud_grm_delta_ramp_linein             ; // Gain Update
    	__IOM uint32_t  aud_grm_other                         ; // Other Setting
    	__IOM uint32_t  aud_grm_gain_control_5                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_6                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_7                ; // Gain Control
    	__IOM uint32_t  aud_grm_gain_control_8                ; // Gain Control
    	__IOM uint32_t  aud_grm_fifo_eflag                    ; // FIFO Error Flag
    	__IOM uint32_t  G067_reserved_22                      ; //
    	__IOM uint32_t  aud_grm_gain_control_10                      ; //
    	__IOM uint32_t  aud_grm_switch_hdmi_tx                ; // AUD_GRM_SWITCH_HDMI_TX
    	__IOM uint32_t  G067_reserved_25                      ; //
    	__IOM uint32_t  G067_reserved_26                      ; //
    	__IOM uint32_t  G067_reserved_27                      ; //
    	__IOM uint32_t  G067_reserved_28                      ; //
    	__IOM uint32_t  G067_reserved_29                      ; //
    	__IOM uint32_t  aud_grm_path_select                   ; //
    	__IOM uint32_t  G067_reserved_31                      ; //

    	// Group 068 : AUD
    	__IOM uint32_t  G068_AUD[32]                          ;

    	// Group 069 : Reserved
    	__IOM uint32_t  G069_reserved_00                      ;
    	__IOM uint32_t  G069_reserved_01                      ;
    	__IOM uint32_t  G069_reserved_02                      ;
    	__IOM uint32_t  G069_reserved_03                      ;
    	__IOM uint32_t  G069_reserved_04                      ;
    	__IOM uint32_t  G069_reserved_05                      ;
    	__IOM uint32_t  G069_reserved_06                      ;
    	__IOM uint32_t  G069_reserved_07                      ;
    	__IOM uint32_t  G069_reserved_08                      ;
    	__IOM uint32_t  G069_reserved_09                      ;
    	__IOM uint32_t  G069_reserved_10                      ;
    	__IOM uint32_t  G069_reserved_11                      ;
    	__IOM uint32_t  G069_reserved_12                      ;
    	__IOM uint32_t  G069_reserved_13                      ;
    	__IOM uint32_t  G069_reserved_14                      ;
    	__IOM uint32_t  I2S_PWM_CONTROL_1                     ;
    	__IOM uint32_t  I2S_PWM_CONTROL_2                     ;
    	__IOM uint32_t  I2S_PWM_CONTROL_3                     ;
    	__IOM uint32_t  I2S_PWM_CONTROL_4                     ;
    	__IOM uint32_t  CLASSD_MOS_CONTROL                    ;
    	__IOM uint32_t  G069_reserved_20                      ;
    	__IOM uint32_t  G069_reserved_21                      ;
    	__IOM uint32_t  G069_reserved_22                      ;
    	__IOM uint32_t  G069_reserved_23                      ;
    	__IOM uint32_t  G069_reserved_24                      ;
    	__IOM uint32_t  G069_reserved_25                      ;
    	__IOM uint32_t  G069_reserved_26                      ;
    	__IOM uint32_t  G069_reserved_27                      ;
    	__IOM uint32_t  G069_reserved_28                      ;
    	__IOM uint32_t  G069_reserved_29                      ;
    	__IOM uint32_t  G069_reserved_30                      ;
    	__IOM uint32_t  G069_reserved_31                      ;

    	// Group 070 : Reserved
    	__IOM uint32_t  G070_RESERVED[32]                     ;

    	// Group 071 : Reserved
    	__IOM uint32_t  aud_a24_base                          ; // 00
    	__IOM uint32_t  aud_a24_length                        ; // 01
    	__IOM uint32_t  aud_a24_ptr                           ; // 02
    	__IOM uint32_t  aud_a24_cnt                           ; // 03
    	__IOM uint32_t  aud_a25_base                          ; // 04
    	__IOM uint32_t  aud_a25_length                        ; // 05
    	__IOM uint32_t  aud_a25_ptr                           ; // 06
    	__IOM uint32_t  aud_a25_cnt                           ; // 07
    	__IOM uint32_t  aud_a26_base                          ; // 08
    	__IOM uint32_t  aud_a26_length                        ; // 09
    	__IOM uint32_t  aud_a26_ptr                           ; // 10
    	__IOM uint32_t  aud_a26_cnt                           ; // 11
    	__IOM uint32_t  aud_a27_base                          ; // 12
    	__IOM uint32_t  aud_a27_length                        ; // 13
    	__IOM uint32_t  aud_a27_ptr                           ; // 14
    	__IOM uint32_t  aud_a27_cnt                           ; // 15
    	__IOM uint32_t  aud_a28_base                          ; // 16
    	__IOM uint32_t  aud_a28_length                        ; // 17
    	__IOM uint32_t  aud_a28_ptr                           ; // 18
    	__IOM uint32_t  aud_a28_cnt                           ; // 19
    	__IOM uint32_t  aud_a29_base                          ; // 20
    	__IOM uint32_t  aud_a29_length                        ; // 21
    	__IOM uint32_t  aud_a29_ptr                           ; // 22
    	__IOM uint32_t  aud_a29_cnt                           ; // 23
    	__IOM uint32_t  G071_reserved_24                      ; // 24
    	__IOM uint32_t  G071_reserved_25                      ; // 25
    	__IOM uint32_t  G071_reserved_26                      ; // 26
    	__IOM uint32_t  G071_reserved_27                      ; // 27
    	__IOM uint32_t  G071_reserved_28                      ; // 28
    	__IOM uint32_t  G071_reserved_29                      ; // 29
    	__IOM uint32_t  G071_reserved_30                      ; // 30
    	__IOM uint32_t  G071_reserved_31                      ; // 31

    	// Group 072 : Reserved
    	__IOM uint32_t  tdm_rx_cfg0                           ;//0
    	__IOM uint32_t  tdm_rx_cfg1                           ;//1
    	__IOM uint32_t  tdm_rx_cfg2                           ;//2
    	__IOM uint32_t  tdm_rx_cfg3                           ;//3
    	__IOM uint32_t  G72_reserved_4                        ;//4
    	__IOM uint32_t  G72_reserved_5                        ;//5
    	__IOM uint32_t  tdm_tx_cfg0                           ;//6
    	__IOM uint32_t  tdm_tx_cfg1                           ;//7
    	__IOM uint32_t  tdm_tx_cfg2                           ;//8
    	__IOM uint32_t  tdm_tx_cfg3                           ;//9
    	__IOM uint32_t  tdm_tx_cfg4                           ;//10
    	__IOM uint32_t  G72_reserved_11                       ;//11
    	__IOM uint32_t  G72_reserved_12                       ;//12
    	__IOM uint32_t  G72_reserved_13                       ;//13
    	__IOM uint32_t  pdm_rx_cfg0                           ;//14
    	__IOM uint32_t  pdm_rx_cfg1                           ;//15
    	__IOM uint32_t  pdm_rx_cfg2                           ;//16
    	__IOM uint32_t  pdm_rx_cfg3                           ;//17
    	__IOM uint32_t  pdm_rx_cfg4                           ;//18
    	__IOM uint32_t  pdm_rx_cfg5                           ;//19
    	__IOM uint32_t  G72_reserved_20                       ;//20
    	__IOM uint32_t  G72_reserved_21                       ;//21
    	__IOM uint32_t  tdm_tx_xck_cfg                        ;//22
    	__IOM uint32_t  tdm_tx_bck_cfg                        ;//23
    	__IOM uint32_t  tdm_rx_xck_cfg                        ;//24
    	__IOM uint32_t  tdm_rx_bck_cfg                        ;//25 TDM_RX_BCK_CFG
    	__IOM uint32_t  pdm_rx_xck_cfg                        ;//26
    	__IOM uint32_t  pdm_rx_bck_cfg                        ;//27
    	__IOM uint32_t  G72_reserved_28                       ;//28
    	__IOM uint32_t  G72_reserved_29                       ;//29
    	__IOM uint32_t  tdmpdm_tx_sel                         ;//30
    	__IOM uint32_t  G72_reserved_31                       ;//31
} AUD_TypeDef;
#define AUD_REG ((volatile AUD_TypeDef *)RF_GRP(60, 0))

/******************************************************************************/
/*                                                                            */
/*                               RTC                                          */
/*                                                                            */
/******************************************************************************/
typedef struct{
	RESERVED(0[1], uint32_t);
	__IOM uint32_t rtc_ctrl;
	__IOM uint32_t rtc_timer;
	__IOM uint32_t rtc_ontime_set;
	__IOM uint32_t rtc_clock_set;
	__IOM uint32_t rtc_macro_ctrl;
	__IOM uint32_t rtc_periodic_set;
	__IOM uint32_t rtc_int_status;
	RESERVED(1[2], uint32_t); ;
	__IOM uint32_t rtc_system_cnt_31_0;
	__IOM uint32_t rtc_system_cnt_63_32;
	RESERVED(2[20], uint32_t);
}RTC_TypeDef;
#define RTC_REGS                    ((volatile RTC_TypeDef *)RF_GRP(116, 0))

/* -------------------  Start of section using anonymous unions  ------------------ */
#if defined ( __CC_ARM   )
  #pragma push
#pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler type
#endif

#ifdef __cplusplus
}
#endif

#endif


