//void SystemClock_Config(void);
#define SERIAL_UART_INSTANCE	1
/* select spi.(SPI0-SPI4) */
#define SPI_SEL_INSTANCE	3
/* select i2c.(I2C0-I2C3) */
#define I2C_SEL_INSTANCE	0
/* select icm.(ICM0--ICM3) */
#define ICM_SEL_INSTANCE    0
/* select ext_int.(EXT_INT0--EXT_INT7) */
#define EXT_INT_SEL_INSTANCE	0

#define PIN_SERIAL_TX	GPIO_(25)
#define PIN_SERIAL_RX	GPIO_(23)

#define PIN_SPI_MISO	GPIO_MAX
#define PIN_SPI_MOSI	GPIO_MAX
#define PIN_SPI_SCK 	GPIO_MAX
#define PIN_SPI_CS 		GPIO_MAX

#define PIN_I2C_SDA		GPIO_(13)
#define PIN_I2C_SCL		GPIO_(14)



/* EXT_INT0:  from  1 ~ 7 */

#define PIN_EXT_INT0	1

#define NUM_DIGITAL_PINS	(GPIO_MAX)



