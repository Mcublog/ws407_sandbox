/**
 * @file application.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-08-10
 *
 * @copyright Viacheslav mcublog (c) 2024
 *
 */
#include <cmath>
#include <ctime>
#include <sys/time.h>

#include "main.h"
#include "i2c.h"
#include "application.h"
#include "app/version.h"
#include "app/utils/delay.h"
#include "app/io/io_gpio.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME app
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (4)
#endif
#include "log_libs.h"
//<<----------------------

void application()
{
    LOG_INFO("Version: %s", FW_VERSION);

    struct timeval tv = {};
    struct timezone tz ={};
    settimeofday(&tv, &tz);

    time_t t = time(NULL);
    clock_t c = clock();

    LOG_INFO("time: %d clock: %d", t, c);

    bool led = false;

    while (1)
    {
        delay_ms(1000);
        io_gpio_led(led);
        led ^= true;
    }
}