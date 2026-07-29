cmake_minimum_required(VERSION 3.25)

project(wsudo
    VERSION 1.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

add_library(wsudo_warnings INTERFACE)

if(MSVC)
    target_compile_options(wsudo_warnings
        INTERFACE
            /W4
            /permissive-
            $<$<NOT:$<CONFIG:Debug>>:/WX>
    )
else()
    target_compile_options(wsudo_warnings
        INTERFACE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            $<$<NOT:$<CONFIG:Debug>>:-Werror>
    )
endif()

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
