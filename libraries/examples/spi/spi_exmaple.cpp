#include "SPI.h"
#include "Arduino.h"
#ifdef __cplusplus
extern "C" {
#endif
void spi_test(void)
{
	char buf[6]={0};
	SPIClass SPI_1(MOSI,MISO,SCK,SSEL);
	SPI_1.beginTransaction(SPISettings(1500000, MSBFIRST, SPI_MODE3));
	printf("spi_test\n");

	SPI_1.transfer(0x0);
	uint8_t ch = SPI_1.transfer(0x0);
	if(ch != 0xE5)
	{
		printf("[ADXL345] read data error ch=%x   !!!\n",ch);
	}
	else
	{
		printf("[ADXL345] read data 0xe5 ok  !!!\n");
	}

		SPI_1.transfer16(0x310b);
		SPI_1.transfer16(0x2d08);
		SPI_1.transfer16(0x2c0b);
		SPI_1.transfer16(0x2e00);
		SPI_1.transfer16(0x3800);
		SPI_1.transfer16(0x1e00);
		SPI_1.transfer16(0x1f00);
		SPI_1.transfer16(0x2000);
	while(1)
	{
		delay(1000);
		memset(buf,0,6);

	for(int i=0;i<6;i++)
	{
			buf[i] =SPI_1.transfer((0x32+i) | 0x80);
			buf[i] = SPI_1.transfer(0x0);
		}
		printf("\n data : ");;
		for(int i=0;i<6;i++)
		{
			printf(" %x ",buf[i]);;
		}
	}
}

void setup()
{
	spi_test();

}

void loop()
{

}


#ifdef __cplusplus
}
#endif


