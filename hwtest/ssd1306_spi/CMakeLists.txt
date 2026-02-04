include(${PROJECT_SOURCE_DIR}/hwtest/build.cmake)
include(${PROJECT_SOURCE_DIR}/libs/hw/list.cmake)
include(${PROJECT_SOURCE_DIR}/libs/third_party/hw/list.cmake)

file(GLOB_RECURSE CURRENT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    #
    ${gmg12864lib}
    ${PROJECT_SOURCE_DIR}/libs/hw/gmg12864/src/ssd1306spi_dev.c
    #
)

set(CURRENT_INCLUDES
    ${gmg12864_includes}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CURRENT_DEFINES
    # "GMG12864_OLED"
)

w407_hwtest_app_v1("hwtest_ssd1306_spi" "${CURRENT_INCLUDES}" "${CURRENT_SOURCES}" "${CURRENT_DEFINES}")