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
    bool led = false;
    static constexpr uint8_t kI2CTimeoutMs = 10;
    uint8_t addr = 0;
    LOG_INFO("Version: %s", FW_VERSION);
    while (1)
    {
        delay_ms(100);
        io_gpio_led(led);
        led ^= true;
        HAL_StatusTypeDef err =
            HAL_I2C_IsDeviceReady(&hi2c1, (addr << 1), 1, kI2CTimeoutMs);
        if (err == HAL_StatusTypeDef::HAL_OK)
            LOG_INFO("find addr[0x%02x[0x%02x]]: %d", addr, addr>>1, err);
        addr++;
        LOG_INFO("working...");
    }
}