file(GLOB_RECURSE BME280_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/third_party/hw/bme280/*.c
)

file(GLOB_RECURSE SSD1309_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/third_party/hw/stm32-ssd1306/ssd1306/*.c
)

file(GLOB_RECURSE PSM_INA219_STM32_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/third_party/hw/PSM_INA219_STM32/INA219.c
)

file(GLOB_RECURSE INA219_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/third_party/hw/ina219/src/driver_ina219.c
)
