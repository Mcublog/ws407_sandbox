/**
 * @file max6675.cpp
 * @brief Конвертация слов MAX6675 в температуру (термопара K)
 * @version 0.1
 * @date 2026-10-07
 *
 */
#include "max6675.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME max6675
#define LOG_MODULE_LEVEL (3)
#include "log_libs.h"
//<<----------------------

//>>---------------------- Local
/**
 * @brief D15 — фиктивный знаковый бит, всегда 0
 */
static constexpr uint16_t kMax6675DummySignBit = (1u << 15);

/**
 * @brief D2 — идентификатор устройства, у MAX6675 всегда 0
 */
static constexpr uint16_t kMax6675DeviceIdBit = (1u << 2);

/**
 * @brief D1 — состояние входа термопары: 1 — обрыв или КЗ
 */
static constexpr uint16_t kMax6675ThermocoupleBit = (1u << 1);

/**
 * @brief D3..D14 — значение АЦП: 12 бит с шагом 0.25 °C (0…1023.75 °C)
 */
static constexpr uint16_t kMax6675ValueMask = 0x0FFFu;
//<<----------------------

//>>---------------------- Public
bool max6675_convert(uint16_t word, temp_t *out)
{
    LOG_DEBUG("max6675: 0x%04X", word);

    if (out == nullptr)
        return false;

    // D15 — всегда 0, иначе битый кадр
    if ((word & kMax6675DummySignBit) != 0)
    {
        LOG_WARNING("max6675: bad frame 0x%04X", word);
        return false;
    }

    // D2 — идентификатор устройства, у MAX6675 всегда 0
    if ((word & kMax6675DeviceIdBit) != 0)
    {
        LOG_WARNING("max6675: bad device id 0x%04X", word);
        return false;
    }

    // D1 — обрыв или КЗ термопары
    if ((word & kMax6675ThermocoupleBit) != 0)
    {
        LOG_WARNING("max6675: thermocouple fault");
        return false;
    }

    // D3..D14 — значение АЦП в единицах 0.25 °C
    const uint16_t quarter_deg = (word >> 3) & kMax6675ValueMask;
    *out = (temp_t)quarter_deg * 25;
    return true;
}
//<<----------------------
