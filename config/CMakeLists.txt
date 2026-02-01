configure_file("version.h.in"
    ${PROJECT_SOURCE_DIR}/app/version.h)


configure_file("fw_burn.jlink.in"
    ${PROJECT_SOURCE_DIR}/scripts/flash_burn.jlink)