include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/mcu.cmake)
include(${PROJECT_SOURCE_DIR}/libs/hw/list.cmake)
include(${PROJECT_SOURCE_DIR}/libs/third_party/hw/list.cmake)

# Add sources
file(GLOB_RECURSE HAL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.s
)

file(GLOB_RECURSE APP_SOURCES ${APP_DIR}/*.c)

# Build lib
add_executable(${STM32F407F_TARGET}
    ${HAL_SOURCES}
    ${APP_SOURCES}
    ${RTT_SOURCES}
    ${BME280_SOURCES}
    ${DWT_SOURCES}
)

# Include paths
target_include_directories(${STM32F407F_TARGET}
    PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${HAL_INCLUDE_DIRS}
    ${APP_DIRS}
    ${RTT_INCLUDE_DIRS}
)

# Project symbols
target_compile_definitions(${STM32F407F_TARGET} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${STM32F407F_TARGET} PRIVATE ${ARM_NONE_EABI_FLAGS})

# Linker options
target_link_options(${STM32F407F_TARGET} PRIVATE ${STM32F407_LINKER_OPTION})

# # Convert output to hex and binary
add_custom_command(TARGET ${STM32F407F_TARGET}
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -I elf32-littlearm ${STM32F407F_TARGET} ${STM32F407F_TARGET}.hex
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary -I elf32-littlearm ${STM32F407F_TARGET} ${STM32F407F_TARGET}.bin
)

build_jlink_script(${STM32F407F_TARGET})