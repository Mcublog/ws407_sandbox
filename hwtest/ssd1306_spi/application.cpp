/**
 * @file application.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2026-02-04
 *
 * @copyright Viacheslav mcublog (c) 2026
 *
 */
#include "application.h"
#include "app/version.h"
#include "app/utils/delay.h"
#include "app/io/io_gpio.h"

#include "libs/hw/gmg12864/src/gmg12864lib.h"
#include "libs/hw/gmg12864/src/ssd1306spi_dev.h"
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

    const gmg_dev_t *dev = ssd1306spi_dev();
    GMG12864_Init(dev);
    GMG12864_logo_demonstration();
    GMG12864_Update();
    while (1)
    {
        delay_ms(2500);
        GMG12864_logo_demonstration();
        GMG12864_Update();
        io_gpio_led(led);
        led ^= true;
        LOG_INFO("working...");
    }
}