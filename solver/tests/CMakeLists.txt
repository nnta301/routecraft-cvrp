add_test(
    NAME routecraft_cli_smoke
    COMMAND "${CMAKE_COMMAND}"
        "-DROUTECRAFT_EXECUTABLE=$<TARGET_FILE:routecraft>"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cli_smoke.cmake"
)
