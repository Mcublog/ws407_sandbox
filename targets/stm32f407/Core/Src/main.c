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
#include "bme280.h"
#include "dwt.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME main
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (4)
#endif
#include "log_libs.h"
//<<----------------------
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SAMPLE_COUNT  UINT8_C(50)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static const uint32_t kI2CTimeoutMs = 10;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
BME280_INTF_RET_TYPE i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    HAL_StatusTypeDef err = HAL_I2C_Mem_Read((I2C_HandleTypeDef *)intf_ptr, (BME280_I2C_ADDR_PRIM << 1), reg_addr,
                                             1, reg_data, len, kI2CTimeoutMs);
    // LOG_DEBUG("read_i2c: %d", err);
    return err == HAL_OK ? BME280_OK : BME280_E_COMM_FAIL;
}

BME280_INTF_RET_TYPE i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len,
                               void *intf_ptr)
{
    HAL_StatusTypeDef err = HAL_I2C_Mem_Write(
        (I2C_HandleTypeDef *)intf_ptr, (BME280_I2C_ADDR_PRIM << 1), reg_addr, 1, (uint8_t *)reg_data, len, kI2CTimeoutMs);
    // LOG_DEBUG("i2c_write: %d", err);
    return err == HAL_OK ? BME280_OK : BME280_E_COMM_FAIL;
}

void delay_us_i2c(uint32_t period, void *intf_ptr)
{
    // LOG_DEBUG("delay_us_i2c: %d us", period);
    dwt_delay_us(period);
}

static int8_t get_temperature(uint32_t period, struct bme280_dev *dev)
{
    int8_t rslt = BME280_E_NULL_PTR;
    int8_t idx = 0;
    uint8_t status_reg;
    struct bme280_data comp_data;

    while (idx < SAMPLE_COUNT)
    {
        rslt = bme280_get_regs(BME280_REG_STATUS, &status_reg, 1, dev);
        LOG_INFO("bme280_get_regs", rslt);

        if (status_reg & BME280_STATUS_MEAS_DONE)
        {
            /* Measurement time delay given to read sample */
            dev->delay_us(period, dev->intf_ptr);

            /* Read compensated data */
            rslt = bme280_get_sensor_data(BME280_TEMP, &comp_data, dev);
            LOG_INFO("bme280_get_sensor_data", rslt);

            comp_data.temperature = comp_data.temperature / 100;
            LOG_DEBUG("Temperature[%d]:   %ld deg C\n", idx, (long int)comp_data.temperature);
            idx++;
        }
    }

    return rslt;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
struct bme280_dev bmp280 =
    {
        /*! Chip Id */
        .chip_id = BME280_CHIP_ID,

        /*! Interface Selection
         * For SPI, intf = BME280_SPI_INTF
         * For I2C, intf = BME280_I2C_INTF
         */
        .intf = BME280_I2C_INTF,

        /*!
         * The interface pointer is used to enable the user
         * to link their interface descriptors for reference during the
         * implementation of the read and write interfaces to the
         * hardware.
         */
        .intf_ptr = (void *)&hi2c1,

        /*! Variable to store result of read/write function */
        .intf_rslt = BME280_OK,

        /*! Read function pointer */
        .read = i2c_read,

        /*! Write function pointer */
        .write = i2c_write,

        /*! Delay function pointer */
        .delay_us = delay_us_i2c,

        // /*! Trim data */
        .calib_data = {0}
};
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    /* USER CODE BEGIN 2 */
    int8_t err = bme280_init(&bmp280);
    uint32_t period = 0;
    LOG_DEBUG("bme280_init: %d", err);
    struct bme280_settings settings;
    err = bme280_get_sensor_settings(&settings, &bmp280);
    LOG_INFO("bme280_get_sensor_settings", err);

    /* Always read the current settings before writing, especially when all the configuration is not modified */
    err = bme280_get_sensor_settings(&settings, &bmp280);
    LOG_INFO("bme280_get_sensor_settings", err);

    /* Configuring the over-sampling rate, filter coefficient and standby time */
    /* Overwrite the desired settings */
    settings.filter = BME280_FILTER_COEFF_2;

    /* Over-sampling rate for humidity, temperature and pressure */
    settings.osr_h = BME280_OVERSAMPLING_1X;
    settings.osr_p = BME280_OVERSAMPLING_1X;
    settings.osr_t = BME280_OVERSAMPLING_1X;

    /* Setting the standby time */
    settings.standby_time = BME280_STANDBY_TIME_0_5_MS;

    err = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &bmp280);
    LOG_INFO("bme280_set_sensor_settings", err);

    /* Always set the power mode after setting the configuration */
    err = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &bmp280);
    LOG_INFO("bme280_set_power_mode", err);

    /* Calculate measurement time in microseconds */
    err = bme280_cal_meas_delay(&period, &settings);
    LOG_INFO("bme280_cal_meas_delay", err);

    printf("\nTemperature calculation (Data displayed are compensated values)\n");
    printf("Measurement time : %lu us\n\n", (long unsigned int)period);

    err = get_temperature(period, &bmp280);
    LOG_INFO("get_temperature", err);

    while (1)
    {
        err = get_temperature(period, &bmp280);
        LOG_INFO("get_temperature", err);
        HAL_Delay(1000);
    }

    application();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
