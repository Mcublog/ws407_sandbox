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

uint8_t slaveTxData[] = "STM32 I2C2 OK";
uint8_t masterRxData[32];


#define kSlaveAddr (2U << 1U)

volatile bool m_data_rx = false;
volatile bool masterWaitData = false;
volatile bool slaveWaitData = false;

I2C_HandleTypeDef *i2cMasterHandler = &hi2c1;
I2C_HandleTypeDef *i2cSlaveHandler = &hi2c2;
//<<----------------------
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {
        LOG_INFO("HAL_I2C_SlaveRxCpltCallback");
        m_data_rx = true;
        slaveWaitData = false;
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if (hi2c == i2cMasterHandler)
  {
    LOG_INFO("HAL_I2C_MasterRxCpltCallback");
    masterWaitData = 0;
  }
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c,
                          uint8_t TransferDirection,
                          uint16_t AddrMatchCode)
{
    if(hi2c->Instance == I2C2)
    {
        LOG_INFO("HAL_I2C_AddrCallback: %d a: 0x%02x", TransferDirection, AddrMatchCode);
        if(TransferDirection == I2C_DIRECTION_RECEIVE)
        {
            // мастер читает у slave
            // HAL_I2C_Slave_Transmit(hi2c, slaveTxData, sizeof(slaveTxData), 10);
            HAL_I2C_Slave_Transmit_IT(hi2c,
                                      slaveTxData,
                                      sizeof(slaveTxData));
        }
        else
        {
            HAL_I2C_Slave_Receive_IT(hi2c, rxData, sizeof(rxData));
        }
    }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C2)
    {
        // можно обновить данные перед следующим чтением
        // например счётчик, регистр и т.п.
        LOG_INFO("HAL_I2C_SlaveTxCpltCallback");
    }
}

void master_read(bool &led)
{
    delay_ms(500);
    io_gpio_led(led);
    led ^= true;

    slaveWaitData = 1;
    HAL_I2C_Slave_Receive_IT(i2cSlaveHandler, rxData, sizeof(txData));
    HAL_I2C_Master_Transmit(&hi2c1, kSlaveAddr, txData, sizeof(txData), HAL_MAX_DELAY);
    while(slaveWaitData);
    LOG_INFO("slave rx done: %s", rxData);

    masterWaitData = 1;
    HAL_I2C_Master_Receive_IT(i2cMasterHandler, kSlaveAddr, masterRxData, sizeof(slaveTxData));
    HAL_I2C_Slave_Transmit_IT(i2cSlaveHandler, slaveTxData, sizeof(slaveTxData));
    while(masterWaitData);

    LOG_INFO("master rx done: %s", masterRxData);
    memset(masterRxData, 0, sizeof(masterRxData));

}

void master_write(bool &led)
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
    m_data_rx = false;
}

void application()
{
    LOG_INFO("Version: %s", FW_VERSION);
    bool led = false;

    while (1)
    {
        // master_read(led);
        master_write(led);
    }
}