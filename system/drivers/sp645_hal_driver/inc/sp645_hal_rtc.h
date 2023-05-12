#ifndef SP645_HAL_RTC_H
#define SP645_HAL_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sp645_cm4.h"
#include "sp645_hal_def.h"
#include "sp64xx.h"

#define RTC_SYS_RST_DIS		(1 << 4)
#define RTC_PERIODIC_EN		(1 << 3)
#define RTC_UPDATE_EN		(1 << 2)
#define RTC_ONTIME_EN_PMC	(1 << 1)
#define RTC_ONTIME_EN		(1 << 0)

typedef struct {

} RTC_HandleTypeDef;

struct rtc_time {
        /*
         * the number of seconds after the minute, normally in the range
         * 0 to 59, but can be up to 60 to allow for leap seconds
         */
        int tm_sec;
        /* the number of minutes after the hour, in the range 0 to 59*/
        int tm_min;
        /* the number of hours past midnight, in the range 0 to 23 */
        int tm_hour;
        /* the day of the month, in the range 1 to 31 */
        int tm_mday;
        /* the number of months since January, in the range 0 to 11 */
        int tm_mon;
        /* the number of years */
        long tm_year;
        /* the number of days since Sunday, in the range 0 to 6 */
        int tm_wday;
        /* the number of days since January 1, in the range 0 to 365 */
        int tm_yday;
};

void HAL_RTC_Init(uint32_t sec, IRQHandler_t cb_alarm, IRQHandler_t cb_periodic, IRQHandler_t cb_2hz);
void HAL_RTC_Update(uint32_t sec);
uint32_t HAL_RTC_Gettime(struct rtc_time *tm);
void HAL_RTC_Periodic(uint32_t sec);
void HAL_RTC_Periodic_Stop(void);
void HAL_RTC_Alarm(uint32_t sec);
void HAL_RTC_DeInit(void);

#ifdef __cplusplus
}
#endif

#endif
