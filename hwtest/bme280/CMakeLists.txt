include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/mcu.cmake)
include(${PROJECT_SOURCE_DIR}/hwtest/common.cmake)
include(${PROJECT_SOURCE_DIR}/libs/hw/list.cmake)
include(${PROJECT_SOURCE_DIR}/libs/third_party/hw/list.cmake)

set(HWTEST_TARGET hwtest_bme280)

# Removed unused files
list(REMOVE_ITEM HAL_SOURCES ${PROJECT_SOURCE_DIR}/targets/stm32f407/Core/Src/main.c)

# Build lib
add_executable(${HWTEST_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/bme280_test.c
    ${HAL_SOURCES}
    ${APP_SOURCES}
    ${RTT_SOURCES}
    ${BME280_SOURCES}
    ${DWT_SOURCES}
)

# Include paths
target_include_directories(${HWTEST_TARGET}
    PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${HAL_INCLUDE_DIRS}
    ${APP_DIRS}
    ${RTT_INCLUDE_DIRS}
)

# Project symbols
target_compile_definitions(${HWTEST_TARGET} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${HWTEST_TARGET} PRIVATE ${ARM_NONE_EABI_FLAGS})

# Linker options
target_link_options(${HWTEST_TARGET} PRIVATE ${STM32F407_LINKER_OPTION})

# # Convert output to hex and binary
add_custom_command(TARGET ${HWTEST_TARGET}
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -I elf32-littlearm ${HWTEST_TARGET} ${HWTEST_TARGET}.hex
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary -I elf32-littlearm ${HWTEST_TARGET} ${HWTEST_TARGET}.bin
)

build_jlink_script(${HWTEST_TARGET})