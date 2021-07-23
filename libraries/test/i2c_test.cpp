#include "Wire.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SLAVE_ADDR 0x44

static uint8_t tx_buff[32];
static uint8_t rx_buff[32];

void i2c_setup()
{
	uint8_t sda = 15;
	uint8_t scl = 14;
	Wire.begin(sda, scl);
}

void i2c_loop()
{
	int i = 0;
	
	Wire.beginTransmission(SLAVE_ADDR);
	Wire.write(tx_buff, 2);
	printf("%s(%d)\n", __FUNCTION__, __LINE__);
	Wire.endTransmission();
	printf("%s(%d)\n", __FUNCTION__, __LINE__);
	Wire.requestFrom(SLAVE_ADDR, 6);
	while(Wire.available())
	{
		rx_buff[i] = Wire.read();
		i++;
	}
	
	int temp = (rx_buff[0]<<8) | rx_buff[1];
	temp = temp * 17500/65536;
	int temp_L = temp%100;
	int temp_H = temp/100 -45;
	
	int RH = (rx_buff[3] << 8) | rx_buff[4];
	RH = RH * 10000/65536;
	int RH_L = RH % 100;
	int RH_H = RH / 100;
	printf("\n temp=%d.%d   %d.%d    \n", temp_H, temp_L, RH_H, RH_L);
	
}

void i2c_test()
{
	do {
	
		tx_buff[0] = 0x24;
		tx_buff[1] = 0x00;
	
		i2c_setup();
		i2c_loop();
		delay(3000000);
	} while(1);
}

#ifdef __cplusplus
}
#endif