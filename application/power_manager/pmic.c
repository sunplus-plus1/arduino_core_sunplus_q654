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


#define STI8070C_ADDR			0x60

#define STI8070C_VSET			0x00
#define STI8070C_REG3			0x03
#define STI8070C_REG3_VALUE		0x88

#define STI8070C_0D8V			7  /*((0.8-0.7125)/0.0125)*/

uint8_t tx_buf[2];
uint8_t rx_buf[2];

enum {
	PMIC_RT5759=0,
	PMIC_STI8070C,
	PMIC_MAX
};

int pmic_ic = PMIC_MAX;

int _pmic_i2c_write(uint8_t *buf)
{
	int ret;
	uint16_t i2c_addr = RT5759_ADDR;

	if(pmic_ic == PMIC_STI8070C)
		i2c_addr = STI8070C_ADDR;

	ret = HAL_I2C_Master_Transmit(test_handle, i2c_addr, tx_buf, sizeof(tx_buf)/sizeof(uint8_t), 0);

	if (ret) {
		printf("Transmit error code %d\n", ret);
		return -1;
	}
	return 0;
}

int get_pmic_ic(void)
{

#ifdef BOARD_SP7350_DM
	pmic_ic = PMIC_STI8070C;
#else
	pmic_ic = PMIC_RT5759;
#endif

	return pmic_ic;
}

void _pmic_enable(void)
{
	HAL_I2C_Init(test_handle);

	if(pmic_ic == PMIC_RT5759)
	{
		tx_buf[0]=RT5759_REG_DCDCCTRL;
		tx_buf[1]=0xA;
		_pmic_i2c_write(tx_buf);
	}
}

void _pmic_disable(void)
{
	HAL_I2C_Init(test_handle);
	get_pmic_ic();
	if(pmic_ic == PMIC_RT5759)
	{
		tx_buf[0]=RT5759_REG_DCDCCTRL;
		tx_buf[1]=0x8;
		_pmic_i2c_write(tx_buf);
	}
	else if(pmic_ic == PMIC_STI8070C)
	{
		tx_buf[0]=STI8070C_VSET;
		tx_buf[1]=0x0;
		_pmic_i2c_write(tx_buf);
	}
}

void _pmic_set_0d8_vol(void)
{
	if(pmic_ic == PMIC_RT5759)
	{
		tx_buf[0]=RT5759_REG_VSEL;
		tx_buf[1]=RT5759_0D8V;
		_pmic_i2c_write(tx_buf);
	}
	else if(pmic_ic == PMIC_STI8070C)
	{
		tx_buf[0]=STI8070C_VSET;
		tx_buf[1]=0x80 | STI8070C_0D8V;
		_pmic_i2c_write(tx_buf);
	}
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
			_pmic_disable();
			break;
		case CA55_0D8V_POWER:
			_pmic_enable();
			_pmic_set_0d8_vol();
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
	/*** This function will let PWR_NPU_CONTROL_PIN(GPIO_65) to be high after GPIO_65 is initialized.
	   Now NPU power is controlled by Linux kernel NPU driver. CM4 doesn't need to handle NPU power during system start up process.
	   CM4 just only handles the NPU power when system suspend and resume.
	   So it needs to cancel this GPIO_65 initialzation to let Linux kernel control the NPU power when system start up. ***/
	//pinMode(PWR_NPU_CONTROL_PIN, OUTPUT);

	/*** This function will let PWR_VCL_CONTROL_PIN(GPIO_66) to be high after GPIO_66 is initialized.
	   Now VCL power is controlled by Linux kernel VCL driver. CM4 doesn't need to handle VCL power during system start up process.
	   CM4 just only handles the VCL power when system suspend and resume.
	   So it needs to cancel this GPIO_66 initialzation to let Linux kernel control the VCL power when system start up. ***/
	//pinMode(PWR_VCL_CONTROL_PIN, OUTPUT);

	pinMode(MAIN_DOMAIN_CONTROL_PIN, OUTPUT);
	return 0;
}

#ifdef __cplusplus
}
#endif
