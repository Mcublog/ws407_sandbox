#pragma once

#include <stdint.h>

/**
 * @brief Температура в сантиградусах Цельсия (0.01 °C)
 *
 * Датчики (NTC/MAX6675) выдают значения именно в сантиградусах:
 * 1000 сантиградусов == 10.00 °C.
 */
typedef int32_t temp_t;

/** @brief Перевод целых градусов в сантиградусы */
#define TEMP_FROM_C(x_c) ((temp_t)((x_c) * 100))

/** @brief Перевод сантиградусов в целые градусы */
#define TEMP_TO_C(t) ((t) / 100)

/** @brief Перевод миллиградусов в сантиградусы */
#define TEMP_FROM_MILLI(x_m) ((temp_t)((x_m) / 10))