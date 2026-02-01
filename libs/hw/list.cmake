file(GLOB_RECURSE DWT_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/hw/dwt/*.c
)

set(gmg12864_includes
    ${PROJECT_SOURCE_DIR}/libs/hw/gmg12864/src
)

file(GLOB_RECURSE gmg12864_gmgdev
    ${PROJECT_SOURCE_DIR}/libs/hw/gmg12864/src/gmg12864lib.c
    ${PROJECT_SOURCE_DIR}/libs/hw/gmg12864/src/gmg12964dev.c
)
