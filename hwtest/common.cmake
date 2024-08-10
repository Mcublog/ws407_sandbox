file(GLOB_RECURSE HAL_SOURCES
    ${PROJECT_SOURCE_DIR}/targets/stm32f407/*.c
    ${PROJECT_SOURCE_DIR}/targets/stm32f407/*.s
)
# Removed unused files
list(REMOVE_ITEM HAL_SOURCES ${PROJECT_SOURCE_DIR}/targets/stm32f407/Core/Src/main.c)
