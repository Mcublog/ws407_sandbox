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
#include "libs/third_party/hw/PSM_INA219_STM32/INA219.h"
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

    INA219_t ina219 = {};
    int err = INA219_Init(&ina219, &hi2c1, INA219_ADDRESS);
    if (err == 0)
        LOG_ERROR("INA219_Init");

    while (1)
    {
        delay_ms(200);
        uint16_t voltage = INA219_ReadShuntVolage_mV(&ina219);
        int16_t current = INA219_ReadCurrent_raw(&ina219);
        LOG_INFO("current: %d", current);
        io_gpio_led(led);
        led ^= true;
    }
}