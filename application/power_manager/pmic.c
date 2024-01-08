#include "pm_common.h"
#include "sp7350_hal.h"

I2C_HandleTypeDef *test_handle;

#define RT5759_ADDR             0x62

#define RT5759_REG_VENDORINFO   0x00
#define RT5759_REG_FREQ         0x01
#define RT5759_REG_VSEL         0x02
#define RT5759_REG_DCDCCTRL     0x03
#define RT5759_REG_STATUS       0x04
#define RT5759_REG_DCDCSET      0x05

#define RT5759_0D8V             ((0.8-0.6)*100)
#define RT5759_MANUFACTURER_ID  0x82

uint8_t tx_buf[2];
uint8_t rx_buf[2];

int _pmic_i2c_write(uint8_t *buf)
{
	int ret;
	ret = HAL_I2C_Master_Transmit(test_handle, RT5759_ADDR, tx_buf, sizeof(tx_buf)/sizeof(uint8_t), 0);
	if (ret) {
		printf("Transmit error code %d\n", ret);
		return -1;
	}
	return 0;
}

void _rt5759_enable(void)
{
	HAL_I2C_Init(test_handle);

	tx_buf[0]=RT5759_REG_DCDCCTRL;
	tx_buf[1]=0xA;

	_pmic_i2c_write(tx_buf);
}

void _rt5759_disable(void)
{
	HAL_I2C_Init(test_handle);

	tx_buf[0]=RT5759_REG_DCDCCTRL;
	tx_buf[1]=0x8;

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
		case MAIN_POWER_OFF:
			digitalWrite(MAIN_DOMAIN_CONTROL_PIN, LOW);
			break;
		case MAIN_POWER_ON:
			digitalWrite(MAIN_DOMAIN_CONTROL_PIN, HIGH);
			break;
		case NPU_POWER_OFF:
			digitalWrite(PWR_NPU_CONTROL_PIN, LOW);
			break;
		case NPU_POWER_ON:
			digitalWrite(PWR_NPU_CONTROL_PIN, HIGH);
			break;
		case VCL_POWER_OFF:
			digitalWrite(PWR_VCL_CONTROL_PIN, LOW);
			break;
		case VCL_POWER_ON:
			digitalWrite(PWR_VCL_CONTROL_PIN, HIGH);
			break;
		default:
		break;
	}
	return ret;
}

int pmic_init(void)
{
	uint8_t version = 0;

	test_handle = (I2C_HandleTypeDef *)malloc(sizeof(I2C_HandleTypeDef));
	memset(test_handle,0,sizeof(I2C_HandleTypeDef));

	test_handle->Instance = SP_I2CM7;
	test_handle->Init.Timing = I2C_MAX_STANDARD_MODE_FREQ;
	test_handle->Mode = I2C_MODE_BURST;

	pinMode(PWR_NPU_CONTROL_PIN, OUTPUT);
	pinMode(PWR_VCL_CONTROL_PIN, OUTPUT);
	pinMode(MAIN_DOMAIN_CONTROL_PIN, OUTPUT);
	return 0;
}

#ifdef __cplusplus
}
#endif
