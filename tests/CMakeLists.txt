include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(wsudo_tests)

target_sources(wsudo_tests
    PRIVATE
        cli/parser_test.cpp
        ipc/pipe_test.cpp
        ${PROJECT_SOURCE_DIR}/src/cli/parser.cpp
        ${PROJECT_SOURCE_DIR}/src/ipc/data/data.cpp
        ${PROJECT_SOURCE_DIR}/src/ipc/pipe/pipe.cpp
        ${PROJECT_SOURCE_DIR}/src/ipc/pipe/host.cpp
        ${PROJECT_SOURCE_DIR}/src/ipc/pipe/client.cpp
        ${PROJECT_SOURCE_DIR}/src/utils/exception.cpp
        ${PROJECT_SOURCE_DIR}/src/utils/handle.cpp
)

target_include_directories(wsudo_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(wsudo_tests
    PRIVATE
        GTest::gtest_main
        wsudo_warnings
)

include(GoogleTest)
gtest_discover_tests(wsudo_tests)
