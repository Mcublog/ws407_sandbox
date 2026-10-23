/**
 * @file application.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2026-04-07
 *
 * @copyright Viacheslav mcublog (c) 2026
 *
 */
#include <cstring>

#include "main.h"

#include "application.h"
#include "app/version.h"
#include "app/utils/delay.h"
#include "app/io/io_gpio.h"

#include "max6675.h"

#include "spi.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME app
#define LOG_MODULE_LEVEL (4)
#include "log_libs.h"
//<<----------------------

//>>----------------------  Global
extern SPI_HandleTypeDef hspi2;
//<<----------------------

//>>---------------------- Locals
/**
 * @brief Таймаут обмена по шине SPI, мс
 */
static constexpr uint32_t kSpiTimeoutMs = 100;
//<<----------------------
static void cs_select()
{
    HAL_GPIO_WritePin(CS_MAX6674_GPIO_Port, CS_MAX6674_Pin, GPIO_PIN_RESET);
}

static void cs_release()
{
    HAL_GPIO_WritePin(CS_MAX6674_GPIO_Port, CS_MAX6674_Pin, GPIO_PIN_SET);
}

int ispi_read(uint8_t *rx, uint16_t len)
{
    if ((rx == nullptr) || (len == 0))
    {
        LOG_WARNING("spi: bad args");
        return -1;
    }

    cs_select();
    // Master + 2 lines: HAL_SPI_Receive читает MISO и одновременно выдаёт
    // в MOSI содержимое буфера (MAX6675 MOSI не используется)
    const HAL_StatusTypeDef status = HAL_SPI_Receive(&hspi2, rx, len, kSpiTimeoutMs);
    cs_release();

    if (status != HAL_OK)
    {
        LOG_WARNING("spi: read %d bytes failed, status %d", len, status);
        return -1;
    }

    return 0;
}

void application()
{
    LOG_INFO("Version: %s", FW_VERSION);
    bool led = false;

    uint8_t m_rx[2] = {};

    while (1)
    {
        delay_ms(500);

        if (ispi_read(m_rx, sizeof(m_rx)) == 0)
        {
            const uint16_t kWord = ((uint16_t)m_rx[0] << 8) | m_rx[1];
            temp_t out = 0;
            max6675_convert(kWord, &out);
            LOG_INFO("temp: %d", out);
        }

        io_gpio_led(led ^= true);
    }
}