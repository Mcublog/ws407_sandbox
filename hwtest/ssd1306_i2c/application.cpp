/**
 * @file application.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2026-02-01
 *
 * @copyright Viacheslav mcublog (c) 2026
 *
 */
#include "main.h"

#include "application.h"
#include "app/version.h"
#include "app/utils/delay.h"
#include "app/io/io_gpio.h"

#include "libs/hw/gmg12864/src/gmg12864lib.h"
#include "libs/hw/gmg12864/src/ssd1306dev.h"
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

    GMG12864_Init(ssd1306_dev());
    // ssd1306_Init();
    // GMG12864_Puts(0, 20, "Text");
    GMG12864_logo_demonstration();
    GMG12864_Update();
    // ssd1306_TestAll();
    while (1)
    {
        delay_ms(500);
        io_gpio_led(led);
        led ^= true;
        LOG_INFO("working...");
    }
}