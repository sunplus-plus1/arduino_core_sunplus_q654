//void SystemClock_Config(void);
#define SERIAL_UART_INSTANCE	1
#define SPI_SEL_INSTANCE	0    //select spi . SPI0--SPI4
#define I2C_SEL_INSTANCE	0	 //select i2c . I2C0--I2C3

#define PIN_SERIAL_TX	GPIO_P2_02
#define PIN_SERIAL_RX	GPIO_P2_00

#define PIN_SPI_MISO	GPIO_P1_07
#define PIN_SPI_MOSI	GPIO_P1_06
#define PIN_SPI_SCK 	GPIO_P1_04
#define PIN_SPI_CS 		GPIO_P1_05

#define PIN_I2C_SDA		GPIO_P1_07
#define PIN_I2C_SCL		GPIO_P1_06

#define PIN_ICM_DATA
#define PIN_ICM_CLK

#define PIN_EXT_INT0	GPIO_P4_07

#define NUM_DIGITAL_PINS	(GPIO_P8_07+1)









