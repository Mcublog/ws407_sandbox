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
#include <sys/times.h>
#include <string.h>

#include "main.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME timesys
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (3)
#endif
#include "log_libs.h"
//<<----------------------

//>>---------------------- Local function definition
static void _fill_hal_time(const struct tm *time, RTC_TimeTypeDef *rtc_time)
{
    rtc_time->Hours = time->tm_hour;
    rtc_time->Minutes = time->tm_min;
    rtc_time->Seconds = time->tm_sec;
}

static void _fill_hal_date(const struct tm *time, RTC_DateTypeDef *rtc_date)
{
    rtc_date->Year = time->tm_year - (2000 - 1900);
    rtc_date->Date = time->tm_mday;
    rtc_date->Month = time->tm_mon + 1;
}

static void _fill_hal_time_and_date(const struct tm *time, RTC_TimeTypeDef *rtc_time,
                                    RTC_DateTypeDef *rtc_date)
{
    _fill_hal_time(time, rtc_time);
    _fill_hal_date(time, rtc_date);
}

static void _fill_tm_from_hal_time(const RTC_TimeTypeDef *rtc_time, struct tm *time)
{
    time->tm_sec = rtc_time->Seconds; // seconds [0,59]
    time->tm_min = rtc_time->Minutes; // min [0,59]
    time->tm_hour = rtc_time->Hours;  // hour [0,23]
    time->tm_yday = 0;                // year day (1 january = 0) [0,365]
    time->tm_isdst = 0;               // flag Summer time (>0 - off)
}

static void _fill_tm_from_hal_date(const RTC_DateTypeDef *rtc_date, struct tm *time)
{
    time->tm_mday = rtc_date->Date;                 // mday [1,31]
    time->tm_mon = rtc_date->Month - 1;             // month [0,11]
    time->tm_year = (rtc_date->Year + 2000) - 1900; // year (1900 == 0)
    time->tm_wday = rtc_date->WeekDay - 1;          // week day (sunday = 0) [0,6]
    time->tm_yday = 0;                              // year day (1 january = 0) [0,365]
    time->tm_isdst = 0;                             // flag Summer time (>0 - off)
}

static void _fill_tm_from_hal_time_and_hal_date(const RTC_TimeTypeDef *rtc_time,
                                                const RTC_DateTypeDef *rtc_date,
                                                struct tm *time)
{
    _fill_tm_from_hal_time(rtc_time, time);
    _fill_tm_from_hal_date(rtc_date, time);
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
    t->tms_utime = 1; /* User CPU time.  */
    t->tms_stime = 2; /* System CPU time.  */

    t->tms_cutime = 3; /* User CPU time of dead children.  */
    t->tms_cstime = 4; /* System CPU time of dead children.  */
    return 1;
}

/**
 * @brief
 *
 * @param buf
 * @return clock_t
 */
__attribute__((__used__))
clock_t
_times(struct tms *buf)
{
    buf->tms_utime = 1; /* User CPU time.  */
    buf->tms_stime = 2; /* System CPU time.  */

    buf->tms_cutime = 3; /* User CPU time of dead children.  */
    buf->tms_cstime = 4; /* System CPU time of dead children.  */
    return 1;
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
    tv->tv_usec = 0;
    tv->tv_sec = 1676464882;
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
    LOG_INFO("_settimeofday");
    return 0;
}