include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/mcu.cmake)
include(${PROJECT_SOURCE_DIR}/hwtest/common.cmake)

function(w407_hwtest_app_v1 target includes sources defines)

    # Removed unused files
    list(REMOVE_ITEM APP_SOURCES ${APP_DIR}/application.c)

    # Add sources
    file(GLOB_RECURSE HWTEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

    # Build lib
    add_executable(${target}
        ${HWTEST_SOURCES}
        #
        ${HAL_SOURCES}
        ${APP_SOURCES}
        ${RTT_SOURCES}
        #
        ${sources}
    )

    # Include paths
    target_include_directories(${target}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}
        ${HAL_INCLUDE_DIRS}
        ${APP_DIRS}
        ${RTT_INCLUDE_DIRS}
        #
        ${includes}
    )

    # Project symbols
    target_compile_definitions(${target} PRIVATE ${symbols_SYMB} ${defines})

    # Compiler options
    target_compile_options(${target} PRIVATE ${ARM_NONE_EABI_FLAGS})

    # Linker options
    target_link_options(${target} PRIVATE ${STM32F407_LINKER_OPTION})

    # # Convert output to hex and binary
    add_custom_command(TARGET ${target}
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -I elf32-littlearm ${target} ${target}.hex
        COMMAND ${CMAKE_OBJCOPY} ARGS -O binary -I elf32-littlearm ${target} ${target}.bin
    )

    build_jlink_script(${target})

endfunction(w407_hwtest_app_v1)
