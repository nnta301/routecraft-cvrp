add_library(routecraft_core STATIC
    src/cvrp.cpp
    src/distance.cpp
    src/nearest_neighbor.cpp
    src/result_export.cpp
    src/validator.cpp
    src/version.cpp
    src/vrp_parser.cpp
)

target_include_directories(routecraft_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(routecraft_core PUBLIC cxx_std_17)
set_target_properties(routecraft_core PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(routecraft_core
    PRIVATE ROUTECRAFT_VERSION="${PROJECT_VERSION}"
)

if(MSVC)
    target_compile_options(routecraft_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(routecraft_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(routecraft
    cli/main.cpp
)
target_link_libraries(routecraft PRIVATE routecraft_core)
target_compile_features(routecraft PRIVATE cxx_std_17)
set_target_properties(routecraft PROPERTIES
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)

if(MSVC)
    target_compile_options(routecraft PRIVATE /W4 /permissive-)
else()
    target_compile_options(routecraft PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
