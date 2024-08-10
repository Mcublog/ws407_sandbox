/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "i2c.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
    LOG_INFO("Version: %s", FW_VERSION);
    while (1)
    {
        delay_ms(1000);
        io_gpio_led(led);
        led ^= true;
        LOG_INFO("working...");
    }
}