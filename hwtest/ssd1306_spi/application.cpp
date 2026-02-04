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
#include "main.h"
#include "i2c.h"
#include "application.h"
#include "app/version.h"
#include "app/utils/delay.h"
#include "app/io/io_gpio.h"
#include "libs/third_party/hw/stm32-ssd1306/ssd1306/ssd1306.h"
#include "libs/third_party/hw/stm32-ssd1306/ssd1306/ssd1306_tests.h"
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
    bool led = false;
    ssd1306_Init();
    ssd1306_TestAll();
    while (1)
    {
        delay_ms(500);
        io_gpio_led(led);
        led ^= true;
        LOG_INFO("working...");
    }
}