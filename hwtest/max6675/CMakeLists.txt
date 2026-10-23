include(${PROJECT_SOURCE_DIR}/hwtest/build.cmake)
include(${PROJECT_SOURCE_DIR}/libs/hw/list.cmake)
include(${PROJECT_SOURCE_DIR}/libs/third_party/hw/list.cmake)

file(GLOB_RECURSE CURRENT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

set(CURRENT_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
)

w407_hwtest_app_v1("hwtest_max6675" "${CURRENT_INCLUDES}" "${CURRENT_SOURCES}" "")
