# AGENTS.md

`ws407_sandbox` — песочница для STM32F407VE: общий код в `app/`, host-цель и
unit-тест, отдельные прошивки-примеры в `hwtest/*`. CMake, C11 (C++20 для части
hwtest), STM32 HAL/CubeMX.

## Host: сборка и проверка

CI (`.github/workflows/cmake.yml`) запускается только на push в `develop` и
проверяет только host-ветку:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc
cmake --build build --target common_types_test
ctest --test-dir build --output-on-failure
```

- Единственный тест — `test/common_types_test.c`, CTest-имя `common_types_test`.
  Фреймворка нет: тест падает через `exit(1)`. Запуск только его:
  `ctest --test-dir build -R common_types_test --output-on-failure`.
- Desktop: `cmake --build build --target ws407_desktop`; бинарь —
  `build/targets/desktop/ws407_desktop`.
- Линтеров, форматтеров и typecheck в репозитории нет. Проверка = сборка + ctest.
- **Не запускайте `cmake --build build` без `--target`**: в дерево всегда
  добавляются `hwtest_*`, и host-gcc падает на ARM-флагах (`-mthumb`, `-mfpu`,
  `-mfloat-abi`). Это ожидаемо.
- `set(CMAKE_C_COMPILER ...)` внутри `targets/desktop/CMakeLists.txt` и
  `test/CMakeLists.txt` не работает — компилятор выбирается только при configure.
  Поэтому host и ARM собираются в разных build-деревьях.

## MCU / hwtest

```sh
cmake -S . -B build-arm -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=arm-none-eabi-gcc \
  -DCMAKE_CXX_COMPILER=arm-none-eabi-g++
cmake --build build-arm --target hwtest_bme280
```

- `ws407_fw` сейчас **не цель сборки**: в `targets/CMakeLists.txt` включён только
  `add_subdirectory(desktop)`, строка для `stm32f407` закомментирована.
- Каждый `hwtest/<name>` — отдельная прошивка `hwtest_<name>`. Новый каталог
  нужно добавить в `hwtest/CMakeLists.txt`. Для нового теста используйте
  `hwtest/build.cmake` → `w407_hwtest_app_v1(target includes sources defines)`;
  пример — `hwtest/i2c_slave/CMakeLists.txt`.
- HAL-слой не хранится в git: `targets/stm32f407/Core/*`, `Drivers/*`,
  `startup_*.s`, `*.ld`, `Makefile`, `.ioc` перечислены в `.gitignore` и
  генерируются CubeMX. Свежий клон без генерации MCU не соберёт.
- В каждом `hwtest/*/README.MD` рецепт: скопировать `cube/STM32F407VE.ioc` в
  `targets/stm32f407` и перегенерировать. Часть `cube/copy.sh` устарела
  (ссылается на несуществующий `hwtest/ssd1309`) — копируйте вручную.
- Текущее локальное состояние CubeMX не собирается ARM-тулчейном: `spi.c`
  компилируется, но `HAL_SPI_MODULE_ENABLED` выключен; на arm-none-eabi 14.2
  линковка дополнительно падает на duplicate `_write_r` (RTT syscalls vs newlib).
  Перед работой с MCU проверяйте, что выбранный `.ioc` включает нужные периферии.
- Прошивка: `JLink -CommanderScript <target>.jlink` (device `STM32F407VE`),
  J-Link должен быть в `PATH`. Настройка тулчейна — в `README.md`.
- `build_jlink_script()` копирует hex в `<repo>/build/output`, а сгенерированный
  `.jlink` читает `${CMAKE_BINARY_DIR}/output`. Пути совпадают только если
  ARM-дерево называется `build`; с `build-arm` скрипт прошивки будет ссылаться
  на несуществующий файл.

## Структура

- `app/` — аппаратно-независимый код, общий для MCU и desktop.
  `app/io/io_gpio.h`, `app/utils/delay.h`, `app/utils/timer.h` реализуются дважды:
  `targets/stm32f407/src/` (HAL) и `targets/desktop/mocks/` (host). Новая
  аппаратная функция требует обе реализации, иначе не соберётся desktop/тест.
- `targets/desktop/` — host entrypoint и моки; `targets/stm32f407/` — HAL и
  board layer.
- `libs/hw/` — локальные драйверы, `libs/third_party/hw/` — вендорные. Списки
  исходников централизованы в `libs/hw/list.cmake` и
  `libs/third_party/hw/list.cmake`; новый драйвер добавляйте туда и подключайте
  его переменную к нужной цели.
- `libs/hw/gmg12864` — git submodule со своим `AGENTS.md`; после клона нужен
  `git submodule update --init --recursive`. Второй submodule — `ina219`.

## Соглашения

- Логирование — обязательный модульный шаблон: `#define LOG_MODULE_NAME`,
  затем `LOG_MODULE_LEVEL`, и только потом `#include "log_libs.h"`.
  Уровни: 1 — error, 2 — warning, 3 — info, 4 — debug; в существующем коде
  чаще стоит 3.
- CMake собирает исходники через `GLOB_RECURSE`: после добавления нового `.c`
  или `.cpp` нужно повторно запустить configure, иначе файл не попадёт в сборку.
- `app/version.h` генерируется из `project(... VERSION ...)` в корневом
  `CMakeLists.txt` и перезаписывается при configure — версию меняйте в CMake,
  не в заголовке. `scripts/flash_burn.jlink` тоже генерируется.
- Рабочая ветка — `develop`.
