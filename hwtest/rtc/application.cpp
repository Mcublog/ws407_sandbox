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
#include "spi.h"
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

    struct timeval tv = {.tv_sec=1724350643, .tv_usec=0};
    settimeofday(&tv, NULL);
    bool led = false;

    while (1)
    {
        clock_t c = clock();
        LOG_INFO("clock: %d", c);
        delay_ms(1000);
        time_t t = time(NULL);
        LOG_INFO("time: %s", ctime(&t));
        io_gpio_led(led);
        led ^= true;
    }
}