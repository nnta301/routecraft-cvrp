add_test(
    NAME routecraft_cli_smoke
    COMMAND "${CMAKE_COMMAND}"
        "-DROUTECRAFT_EXECUTABLE=$<TARGET_FILE:routecraft>"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cli_smoke.cmake"
)

add_executable(routecraft_core_tests
    cvrp_core_tests.cpp
)

target_link_libraries(routecraft_core_tests PRIVATE routecraft_core)

add_test(
    NAME routecraft_core_tests
    COMMAND routecraft_core_tests
)

add_executable(routecraft_parser_tests
    vrp_parser_tests.cpp
)

target_link_libraries(routecraft_parser_tests PRIVATE routecraft_core)

add_test(
    NAME routecraft_parser_tests
    COMMAND routecraft_parser_tests
)
