#include "Wire.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SLAVE_ADDR 0x44

static uint8_t tx_buff[32];
static uint8_t rx_buff[32];


static i2c_t *test1;

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
	Wire.endTransmission();
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

static uint8_t tx_buff1[32];
static uint8_t rx_buff1[32];

static i2c_t temptest;

void i2c_twi_test1(void)
{
	temptest.pin_sda = 15;
	temptest.pin_scl = 14;

	tx_buff1[0] = 0x24;
	tx_buff1[1] = 0x00;
	
	//printf("%d   %d\n", temptest.pin_sda, temptest.pin_scl);
	i2c_init(&temptest);
	i2c_master_write(&temptest, 0x44, tx_buff1, 2);
	//delay(200);
	//while(i2c_check(&test.handle));
	i2c_master_read(&temptest, 0x44, rx_buff1, 6);
	
	int temp = (rx_buff1[0]<<8) | rx_buff1[1];
	temp = temp * 17500/65536;
	int temp_L = temp%100;
	int temp_H = temp/100 -45;
	
	int RH = (rx_buff1[3] << 8) | rx_buff1[4];
	RH = RH * 10000/65536;
	int RH_L = RH % 100;
	int RH_H = RH / 100;
	printf("\n temp=%d.%d   %d.%d    \n",temp_H,temp_L,RH_H,RH_L);
}

extern void i2c_twi_test(void);

void i2c_test()
{
	do {
		delay(3000000);

		tx_buff[0] = 0x24;
		tx_buff[1] = 0x00;
#if 0
		test.pin_sda = 15;
		test.pin_scl = 14;
		i2c_init(&test);
		i2c_master_write(&test, 0x44, tx_buff, 2);
		i2c_master_read(&test, 0x44, rx_buff, 6);
	
		int temp = (rx_buff[0]<<8) | rx_buff[1];
		temp = temp * 17500/65536;
		int temp_L = temp%100;
		int temp_H = temp/100 -45;

		int RH = (rx_buff[3] << 8) | rx_buff[4];
		RH = RH * 10000/65536;
		int RH_L = RH % 100;
		int RH_H = RH / 100;
		printf("\n temp=%d.%d   %d.%d    \n",temp_H,temp_L,RH_H,RH_L);
#else
		i2c_setup();
		i2c_loop();
#endif
	} while(1);
}

#ifdef __cplusplus
}
#endif