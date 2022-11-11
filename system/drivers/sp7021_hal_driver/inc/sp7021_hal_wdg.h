#ifndef __SP7021_HAL_WDG_H
#define __SP7021_HAL_WDG_H
#if 0
#ifdef __cplusplus
 extern "C" {
#endif

#include "sp7021_hal_def.h"
#include "sp70xx.h"

/**
  * @brief  IWDG Handle Structure definition
  */
typedef struct
{
	WDG_TypeDef		*Instance;	/*!< Register base address   */
	uint16_t		Reload;		/*!< WDG required parameters */
	HAL_LockTypeDef		Lock;		/*!< Locking object          */
	IRQHandler_t		IrqHandle;
} WDG_HandleTypeDef;

typedef void (*WdgCallbackFunc)(void);


#define WDG_CMD_UNLOCK		0xAB00		/*!< WDG conuter value can be written   */
#define WDG_CMD_LOCK		0xAB01		/*!< WDG conuter value can't be written */
#define WDG_CMD_STOP		0x3877		/*!< WDG conuter value will be kept     */
#define WDG_CMD_START		0x4A4B		/*!< WDG will be actived                */
#define WDG_CMD_CLRF		0x7482		/*!< Clear WDG interrupt flag           */
#define WDG_CMD_CNTMAX		0xDEAF		/*!< Set WDG conuter maximum value      */

#if 0
#define HAL_WDG_UNLOCK(__HANDLE__)		WRITE_REG((__HANDLE__)->Instance->control, WDG_CMD_UNLOCK)
#define HAL_WDG_LOCK(__HANDLE__)		WRITE_REG((__HANDLE__)->Instance->control, WDG_CMD_LOCK)
#define HAL_WDG_STOP(__HANDLE__)		WRITE_REG((__HANDLE__)->Instance->control, WDG_CMD_STOP)
#define HAL_WDG_START(__HANDLE__)		WRITE_REG((__HANDLE__)->Instance->control, WDG_CMD_START)
#define HAL_WDG_CLRF(__HANDLE__)		WRITE_REG((__HANDLE__)->Instance->control, WDG_CMD_CLRF)
#define HAL_WDG_CNTMAX(__HANDLE__)		WRITE_REG((__HANDLE__)->Instance->control, WDG_CMD_CNTMAX)
#endif

__STATIC_INLINE void HAL_WDG_UNLOCK(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_UNLOCK);
}

__STATIC_INLINE void HAL_WDG_LOCK(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_LOCK);
}

__STATIC_INLINE void HAL_WDG_STOP(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_STOP);
}

__STATIC_INLINE void HAL_WDG_START(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_START);
}

__STATIC_INLINE void HAL_WDG_CLRF(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_CLRF);
}

__STATIC_INLINE void HAL_WDG_CNTMAX(WDG_TypeDef *Instance)
{
	WRITE_REG(Instance->control, WDG_CMD_CNTMAX);
}

__STATIC_INLINE void HAL_WDG_SETCNT(WDG_TypeDef *Instance, uint16_t value)
{
	WRITE_REG(Instance->counter_val, value);
}

__STATIC_INLINE uint16_t HAL_WDG_GETCNT(WDG_TypeDef *Instance)
{
	return READ_REG(Instance->counter_val);
}

HAL_StatusTypeDef HAL_WDG_Init(WDG_HandleTypeDef *hwdg);

#ifdef __cplusplus
}
#endif
#endif
#endif
