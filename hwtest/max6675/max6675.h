#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "app/data/temperature.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Преобразовать 16-битное слово MAX6675 в температуру
     *
     * Раскладка кадра (даташит MAX6675, Figure 2 "SO output"):
     * - D15 — фиктивный знаковый бит, всегда 0;
     * - D14..D3 — значение АЦП, 12 бит с шагом 0.25 °C
     *   (диапазон 0…1023.75 °C);
     * - D2 — идентификатор устройства, у MAX6675 всегда 0;
     * - D1 — состояние входа термопары: 1 — обрыв или КЗ;
     * - D0 — three-state, не используется.
     *
     * @param word 16-битное слово, считанное с MAX6675
     * @param out указатель на результат (сотые доли градуса, сантиградусы)
     * @return bool true - успешно, false - битый кадр, чужое устройство
     *         или авария термопары
     */
    bool max6675_convert(uint16_t word, temp_t *out);

#ifdef __cplusplus
}
#endif
