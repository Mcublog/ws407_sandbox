/**
 * @file time_syscall.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-02-15
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/times.h>
#include <string.h>

#include "main.h"
#include "rtc.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME timesys
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (3)
#endif
#include "log_libs.h"
//<<----------------------

//>>---------------------- Global
extern RTC_HandleTypeDef hrtc;
//<<----------------------

//>>---------------------- Local function definition
static bool rtc_is_ready(void)
{
    return (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == SET);
}

static time_t get_current_time(void)
{
    RTC_TimeTypeDef rtc_time = {0};
    RTC_DateTypeDef rtc_date = {0};

    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

    struct tm time = {
            .tm_sec = rtc_time.Seconds,               // seconds [0,59]
            .tm_min = rtc_time.Minutes,               // min [0,59]
            .tm_hour = rtc_time.Hours,                // hour [0,23]
            .tm_mday = rtc_date.Date,                 // mday [1,31]
            .tm_mon = rtc_date.Month - 1,             // month [0,11]
            .tm_year = (rtc_date.Year + 2000) - 1900, // year (1900 == 0)
            .tm_wday = rtc_date.WeekDay - 1,          // week day (sunday = 0) [0,6]
            .tm_yday = 0,                                 // year day (1 january = 0) [0,365]
            .tm_isdst = 0,                                // flag Summer time (>0 - off)
        };
    return mktime(&time);
}

static void set_current_time(time_t t)
{
    struct tm *time = localtime(&t);
    RTC_TimeTypeDef rtc_time = {.Hours = time->tm_hour,
                                .Minutes = time->tm_min,
                                .Seconds = time->tm_sec};

    RTC_DateTypeDef rtc_date = {
        .Year = time->tm_year - (2000 - 1900),
        .Date = time->tm_mday,
        .Month = time->tm_mon + 1,
    };

    if (rtc_is_ready() == false)
        MX_RTC_Init();

    HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
}
//<<----------------------

/**
 * @brief
 *
 * @param r
 * @param t
 * @return clock_t
 */
__attribute__((__used__))
clock_t
_times_r(struct _reent *r, struct tms *t)
{
    clock_t tick = HAL_GetTick();
    t->tms_utime = tick; /* User CPU time.  */
    t->tms_stime = tick; /* System CPU time.  */

    t->tms_cutime = tick; /* User CPU time of dead children.  */
    t->tms_cstime = tick; /* System CPU time of dead children.  */
    return tick;
}

/**
 * @brief
 *
 * @param tv
 * @param tzvp
 * @return int
 */
__attribute__((__used__))
int _gettimeofday(struct timeval *tv, void *tzvp)
{
    tv->tv_sec = get_current_time();
    tv->tv_usec = 0;
    return 0;
}

/**
 * @brief
 *
 * @param tv
 * @param tz
 * @return int
 */
__attribute__((__used__))
int settimeofday(const struct timeval *tv , const struct timezone *tz)
{
    UNUSED(tz);

    set_current_time(tv->tv_sec);
    return 0;
}