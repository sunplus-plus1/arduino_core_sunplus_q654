#ifndef __PMIC_H__
#define __PMIC_H__

#ifdef __cplusplus
extern "C" {
#endif
int pmic_init(void);
int pmic_do_cmd(uint32_t cmd);

#ifdef __cplusplus
}
#endif

#endif