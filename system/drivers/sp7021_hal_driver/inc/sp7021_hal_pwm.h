
#ifndef __SP7021_HAL_PWM_H
#define __SP7021_HAL_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sp7021_hal_def.h"
#include "sp7021_arm926.h"

typedef enum {
	PWM0=0,
	PWM1,
	PWM2,
	PWM3,
	PWM4,
	PWM5,
	PWM6,
	PWM7,
	PWM_MAX
}PWM_NUMTypeDef;

enum {
	ePWM_DD0=0,
	ePWM_DD1,
	ePWM_DD2,
	ePWM_DD3,
	ePWM_DD_MAX
};

#define STATIC_ASSERT(b) extern int _static_assert[b ? 1 : -1]

struct _PWM_DD_REG_ {
	union {
		struct {
			uint32_t dd					:16;//b(0-15)
			uint32_t :16;
		};
		uint32_t idx_all;
	};
};
STATIC_ASSERT(sizeof(struct _PWM_DD_REG_) == 4);

struct _PWM_DU_REG_ {
	union {
		struct {
			uint32_t pwm_du				:8;	//b(0-7)
			uint32_t pwm_du_dd_sel		:2;	//b(8-9)
			uint32_t						:6;	//b(10-15)
			uint32_t :16;
		};
		uint32_t idx_all;
	};
};
STATIC_ASSERT(sizeof(struct _PWM_DU_REG_) == 4);

struct moon244_regs {
	//GROUP 244
	union {
		struct {
			uint32_t pwm_en				:8;	//b(0-7)
			uint32_t pwm_bypass			:8;	//b(8-15)
			uint32_t :16;
		};
		uint32_t grp244_0;
	};
	union {
		struct {
			uint32_t pwm_cnt0_en			:1;	//b(0)
			uint32_t pwm_cnt1_en			:1;	//b(1)
			uint32_t pwm_cnt2_en			:1;	//b(2)
			uint32_t pwm_cnt3_en			:1;	//b(3)
			uint32_t pwm_clk54_en		:1;	//b(4)
			uint32_t						:3;	//b(5-7)
			uint32_t pwm_dd0_sync_off	:1;	//b(8)
			uint32_t pwm_dd1_sync_off	:1;	//b(9)
			uint32_t pwm_dd2_sync_off	:1;	//b(10)
			uint32_t pwm_dd3_sync_off	:1;	//b(11)
			uint32_t						:4;	//b(12-15)
			uint32_t :16;
		};
		uint32_t grp244_1;
	};
	struct _PWM_DD_REG_ pwm_dd[4];		//G244.2~5
	struct _PWM_DU_REG_ pwm_du[8];		//G244.6~13
	uint32_t grp244_14_31[18];
};


typedef struct
{
  /* set pwm number. the value set from PWM0 to PWM7 */
  PWM_NUMTypeDef pwm_num;
  /* set pwm freq. unit is HZ */
  uint32_t pwm_freq;
  /* set pwm duty. the value set from  1 to 99 */ 
  uint32_t duty_cycle;
  /* set pin number: one of PINMUX_PIN1_00--PINMUX_PIN8_07  */
  uint32_t Pin;
}PWM_InitTypeDef;


#define PINMUX_CFG_2_REG ((volatile GROUP2_Type *) RF_GRP(2, 0))

#define PWM_CTRL_REG ((volatile struct moon244_regs *)RF_GRP(244, 0))

#define PWM_DEBUG    /* enable pwm debug  log */


#define	PINMUX_PIN1_00		1
#define	PINMUX_PIN1_01		2
#define	PINMUX_PIN1_02		3
#define	PINMUX_PIN1_03		4
#define	PINMUX_PIN1_04		5
#define	PINMUX_PIN1_05		6
#define	PINMUX_PIN1_06		7
#define	PINMUX_PIN1_07		8
#define	PINMUX_PIN2_00		9
#define	PINMUX_PIN2_01		10
#define	PINMUX_PIN2_02		11
#define	PINMUX_PIN2_03		12
#define	PINMUX_PIN2_04		13
#define	PINMUX_PIN2_05		14
#define	PINMUX_PIN2_06		15
#define	PINMUX_PIN2_07		16
#define	PINMUX_PIN3_00		17
#define	PINMUX_PIN3_01		18
#define	PINMUX_PIN3_02		19
#define	PINMUX_PIN3_03		20
#define	PINMUX_PIN3_04		21
#define	PINMUX_PIN3_05		22
#define	PINMUX_PIN3_06		23
#define	PINMUX_PIN3_07		24
#define	PINMUX_PIN4_00		25
#define	PINMUX_PIN4_01		26
#define	PINMUX_PIN4_02		27
#define	PINMUX_PIN4_03		28
#define	PINMUX_PIN4_04		29
#define	PINMUX_PIN4_05		30
#define	PINMUX_PIN4_06		31
#define	PINMUX_PIN4_07		32
#define	PINMUX_PIN5_00		33
#define	PINMUX_PIN5_01		34
#define	PINMUX_PIN5_02		35
#define	PINMUX_PIN5_03		36
#define	PINMUX_PIN5_04		37
#define	PINMUX_PIN5_05		38
#define	PINMUX_PIN5_06		39
#define	PINMUX_PIN5_07		40
#define	PINMUX_PIN6_00		41
#define	PINMUX_PIN6_01		42
#define	PINMUX_PIN6_02		43
#define	PINMUX_PIN6_03		44
#define	PINMUX_PIN6_04		45
#define	PINMUX_PIN6_05		46
#define	PINMUX_PIN6_06		47
#define	PINMUX_PIN6_07		48
#define	PINMUX_PIN7_00		49
#define	PINMUX_PIN7_01		50
#define	PINMUX_PIN7_02		51
#define	PINMUX_PIN7_03		52
#define	PINMUX_PIN7_04		53
#define	PINMUX_PIN7_05		54
#define	PINMUX_PIN7_06		55
#define	PINMUX_PIN7_07		56
#define	PINMUX_PIN8_00		57
#define	PINMUX_PIN8_01		58
#define	PINMUX_PIN8_02		59
#define	PINMUX_PIN8_03		60
#define	PINMUX_PIN8_04		61
#define	PINMUX_PIN8_05		62
#define	PINMUX_PIN8_06		63
#define	PINMUX_PIN8_07		64



#define IS_PWM_NUM_VALID(num)  (((num) >= PWM0) && ((num) <= PWM7))
#define IS_PWM_PIN_VALID(pin)  (((pin) >= PINMUX_PIN1_00) && ((pin) <= PINMUX_PIN8_07))
#define IS_PWM_DUTY_VALID(duty)  (((duty) >= 1) && ((duty) <= 99))

#define pwm_assert_param(expr)   ((expr) ? (void)0 : (printf("[ERROR]: file %s on line %d\r\n",__FUNCTION__, __LINE__)))




int HAL_PWM_INIT(PWM_InitTypeDef *PWM_Init);
void HAL_PWM_ENABLE(int pwm_num);
void HAL_PWM_DISABLE(int pwm_num);

#ifdef __cplusplus
}
#endif

#endif