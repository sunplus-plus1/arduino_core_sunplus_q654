#include "pm_common.h"
#include "Wire.h"

TwoWire *pmic_i2c;

#define RT5759_ADDR             0x60

#define RT5759_REG_VENDORINFO   0x00
#define RT5759_REG_FREQ         0x01
#define RT5759_REG_VSEL         0x02
#define RT5759_REG_DCDCCTRL     0x03
#define RT5759_REG_STATUS       0x04
#define RT5759_REG_DCDCSET      0x05

#define RT5759_0D8V             ((0.8-0.6)*100)
#define RT5759_MANUFACTURER_ID  0x82

uint8_t tx_buf[2];


//#define USE_I2C_READ

int _pmic_i2c_write(uint8_t tx_buf[])
{
	pmic_i2c->beginTransmission(RT5759_ADDR);
	pmic_i2c->write(tx_buf,sizeof(tx_buf)/sizeof(tx_buf[0]));

	uint8_t ret = pmic_i2c->endTransmission();
	if (ret) {
		printf("Transmit error code %d\n", ret);
		return -1;
	}
	return 0;
}

#ifdef USE_I2C_READ
uint8_t _pmic_i2c_read(uint8_t reg)
{
	uint8_t data = 0;
	pmic_i2c->beginTransmission(RT5759_ADDR);
	pmic_i2c->write(reg); 
	pmic_i2c->endTransmission(0); // end transmission,not send stop bit

	pmic_i2c->requestFrom(RT5759_ADDR, (size_t)1);// send data n-bytes read
	pmic_i2c->available();
	data = pmic_i2c->read(); // receive DATA
	return 	data;
}
#endif

void _rt5759_enable(void)
{
#ifdef USE_I2C_READ
	uint8_t data = _pmic_i2c_read(RT5759_REG_DCDCCTRL);
	data |=  (0x1<<1);
	tx_buf[0]=RT5759_REG_DCDCCTRL;
	tx_buf[1]=data;
#else
	tx_buf[0]=RT5759_REG_DCDCCTRL;
	tx_buf[1]=0xA;
#endif
	_pmic_i2c_write(tx_buf);
}

void _rt5759_disable(void)
{
#ifdef USE_I2C_READ
	uint8_t data = _pmic_i2c_read(RT5759_REG_DCDCCTRL);
	data &= ~(0x1<<1);
	tx_buf[0]=RT5759_REG_DCDCCTRL;
	tx_buf[1]=data;
#else
	tx_buf[0]=RT5759_REG_DCDCCTRL;
	tx_buf[1]=0x8;
#endif
	_pmic_i2c_write(tx_buf);
}

void _rt5759_set_0d8_vol(void)
{
	tx_buf[0]=RT5759_REG_VSEL;
	tx_buf[1]=RT5759_0D8V;
	_pmic_i2c_write(tx_buf);
}


#ifdef __cplusplus
extern "C" {
#endif 

int pmic_do_cmd(uint32_t cmd)
{
	int ret = 0;
	switch(cmd)
	{
		case CA55_POWER_OFF:
			_rt5759_disable();
			break;
		case CA55_0D8V_POWER:
			_rt5759_enable();
			_rt5759_set_0d8_vol();
			break;
		default:
		break;
	}
	return ret;
}

int pmic_init(void)
{
	uint8_t version = 0;
	pmic_i2c = new TwoWire(SP_I2CM3);
	pmic_i2c->begin();
#ifdef USE_I2C_READ
	version = _pmic_i2c_read(RT5759_REG_VENDORINFO);
	if(version != RT5759_MANUFACTURER_ID)
	{
		printf("pmic device init error \n");
		return -1;
	}
#endif
	return 0;
}

#ifdef __cplusplus
}
#endif
