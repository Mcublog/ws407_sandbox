function(LOG_INFO text)
    message("[INFO] " ${ARGV})
endfunction()

function(LOG_DEBUG text)
    message("[DEBUG] " ${ARGV})
endfunction()

function(LOG_WARN text)
    message("[WARN] " ${ARGV})
endfunction()

function(build_jlink_script target)
    add_custom_command(TARGET ${target}
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy
        ARGS ${target}.hex ${PROJECT_SOURCE_DIR}/build/output/${target}.hex)

    configure_file(${PROJECT_SOURCE_DIR}/config/burn.jlink.in
        ${CMAKE_CURRENT_BINARY_DIR}/${target}.jlink
        ESCAPE_QUOTES)
endfunction()

function(get_git_hash hash_value)
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(${hash_value} ${GIT_HASH} PARENT_SCOPE)
    if(GIT_HASH STREQUAL "")
        set(GIT_HASH "unknown")
    endif()
endfunction()

function(set_build_mark output_dir hash_value timestamp)

    set(GIT_HASH ${hash_value})
    set(BUILD_TIMESTAMP ${timestamp})

    configure_file("get_build_marks.c.in"
        ${output_dir}/get_build_marks.c
        ESCAPE_QUOTES)

endfunction()
