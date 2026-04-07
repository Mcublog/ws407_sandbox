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

#include "i2c.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME app
#define LOG_MODULE_LEVEL (4)
#include "log_libs.h"
//<<----------------------

//>>----------------------  Global
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
//<<----------------------

//>>---------------------- Locals
uint8_t txData[] = "Hello from I2C1";
uint8_t rxData[32] = {};

#define kSlaveAddr (2U << 1U)

volatile bool m_data_rx = false;
//<<----------------------
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {
        m_data_rx = true;
        // if(TransferDirection == I2C_DIRECTION_TRANSMIT)
        // {
        //     // мастер пишет в slave
        //     HAL_I2C_Slave_Receive_IT(hi2c, rxData, sizeof(rxData));
        // }
        // else
        // {
        //     // мастер читает из slave
        //     HAL_I2C_Slave_Transmit_IT(hi2c, rxData, sizeof(rxData));
        // }
    }
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {
        // обязательно снова включить listen
        HAL_I2C_EnableListen_IT(hi2c);
    }
}

void application()
{
    LOG_INFO("Version: %s", FW_VERSION);
    bool led = false;

    while (1)
    {
        HAL_I2C_Slave_Receive_IT(&hi2c2, rxData, sizeof(txData));

        delay_ms(500);
        io_gpio_led(led);
        led ^= true;

        auto s = HAL_I2C_Master_Transmit(&hi2c1, kSlaveAddr, txData, sizeof(txData), HAL_MAX_DELAY);
        LOG_INFO("i2c send status: %d", s);
        while (!m_data_rx)
        {
            /* code */
        }
        // здесь данные уже в rxData
        LOG_INFO("rx: %s", rxData);
        memset(rxData, 0, sizeof(rxData));

        // перезапустить прием
        // HAL_I2C_DeInit(&hi2c2);
        // MX_I2C2_Init();
        //HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
        // HAL_I2C_Slave_Receive_IT(&hi2c2, rxData, sizeof(rxData));
        m_data_rx = false;
    }
}