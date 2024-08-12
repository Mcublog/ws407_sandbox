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

#include "main.h"
#include "i2c.h"
#include "application.h"
#include "app/version.h"
#include "app/utils/delay.h"
#include "app/io/io_gpio.h"
#include "libs/third_party/hw/ina219/src/driver_ina219.h"
#include "libs/third_party/hw/ina219/interface/driver_ina219_interface.h"
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

    static ina219_handle_t gs_handle;

    /* link interface function */
    DRIVER_INA219_LINK_INIT(&gs_handle, ina219_handle_t);
    DRIVER_INA219_LINK_IIC_INIT(&gs_handle, ina219_interface_iic_init);
    DRIVER_INA219_LINK_IIC_DEINIT(&gs_handle, ina219_interface_iic_deinit);
    DRIVER_INA219_LINK_IIC_READ(&gs_handle, ina219_interface_iic_read);
    DRIVER_INA219_LINK_IIC_WRITE(&gs_handle, ina219_interface_iic_write);
    DRIVER_INA219_LINK_DELAY_MS(&gs_handle, ina219_interface_delay_ms);
    DRIVER_INA219_LINK_DEBUG_PRINT(&gs_handle, ina219_interface_debug_print);

    uint8_t res = ina219_set_addr_pin(&gs_handle, INA219_ADDRESS_0);
    if (res != 0)
        LOG_ERROR("ina219: set addr pin failed");

    res = ina219_set_resistance(&gs_handle, 0.1);
    if (res != 0)
        LOG_ERROR("ina219: set resistance");

    res = ina219_init(&gs_handle);
    if (res != 0)
        LOG_ERROR("ina219: ina219_init");

    res = ina219_set_bus_voltage_range(&gs_handle, INA219_BUS_VOLTAGE_RANGE_32V);
    if (res != 0)
        LOG_ERROR("ina219: ina219_set_bus_voltage_range");

    res = ina219_set_bus_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_64_SAMPLES);
    if (res != 0)
        LOG_ERROR("ina219: ina219_set_bus_voltage_adc_mode");

    res = ina219_set_shunt_voltage_adc_mode(&gs_handle, INA219_ADC_MODE_12_BIT_64_SAMPLES);
    if (res != 0)
        LOG_ERROR("ina219: ina219_set_shunt_voltage_adc_mode");

    res = ina219_set_mode(&gs_handle, INA219_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS);
    if (res != 0)
        LOG_ERROR("ina219: ina219_set_mode");

    res = ina219_set_pga(&gs_handle, INA219_PGA_320_MV);
    if (res != 0)
        LOG_ERROR("ina219: ina219_set_pga");

    uint16_t calibration = 4096;
    res = ina219_calculate_calibration(&gs_handle, (uint16_t *)&calibration);
    if (res != 0)
        LOG_ERROR("ina219: ina219_calculate_calibration");

    LOG_INFO("ina219: calibration: %d", calibration);

    res = ina219_set_calibration(&gs_handle, calibration);
    if (res != 0)
        LOG_ERROR("ina219: ina219_set_calibration");

    while (1)
    {
        delay_ms(1000);
        int16_t rawA = 0, rawV = 0;
        float current, voltage = 0.0f;
        res = ina219_read_current(&gs_handle, &rawA, &current);
        res |= ina219_read_shunt_voltage(&gs_handle, &rawV, &voltage);
        if (res != 0)
            LOG_ERROR("ina219: reading");
        LOG_INFO("raw[%d] current: %d mA", rawA, (int)roundf(current));
        LOG_INFO("raw[%d] voltage: %d mV", rawV, (int)roundf(voltage));
        io_gpio_led(led);
        led ^= true;
    }
}