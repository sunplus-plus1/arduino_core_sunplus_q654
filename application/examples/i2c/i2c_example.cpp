/*
 *	Test i2c on a temperature sensor(SHT3X).
 *	The transmission of Arduino layer(twi.c) is implemented as INTR MODE.
 *	if want to use other mode. Please assgin the varible in i2c_custom_init() of twi.c.
 *	e.x.
 *		i2c_custom_init()
 *		{
 *			...
 *			handle->Mode = I2C_MODE_INTR;//I2C_MODE_BURST
 *			...
 *		}
 *
 */

#include "Arduino.h"
#include "Wire.h"

#define SLAVE_ADDR 0x44

static uint8_t tx_buff[32];
static uint8_t rx_buff[32];

TwoWire *i2cm_test;

void setup()
{
	printf("i2c example %s\n", __TIME__);

	memset(&tx_buff, 0x0, 32);
	memset(&rx_buff, 0x0, 32);

	i2cm_test = new TwoWire(SP_I2CM0);
	i2cm_test->begin();
}

void loop()
{
	int i = 0;

	tx_buff[0] = 0x24;//24
	tx_buff[1] = 0x00;//00

	i2cm_test->beginTransmission(SLAVE_ADDR);
	i2cm_test->write(tx_buff, 2);

	uint8_t ret = i2cm_test->endTransmission();
	if (ret) {
		printf("Transmit error code %d\n", ret);
	}

	i2cm_test->requestFrom(SLAVE_ADDR, 6);

	while(i2cm_test->available())
	{
		rx_buff[i] = i2cm_test->read();
		i++;
	}

	int temp = (rx_buff[0] << 8) | rx_buff[1];
	temp = temp * 17500 / 65536;
	int temp_L = temp % 100;
	int temp_H = temp / 100 - 45;

	int RH = (rx_buff[3] << 8) | rx_buff[4];
	RH = RH * 10000 / 65536;
	int RH_L = RH % 100;
	int RH_H = RH / 100;

	printf("\n temp=%d.%d   %d.%d    \n", temp_H, temp_L, RH_H, RH_L);

	delay(2000);
}
