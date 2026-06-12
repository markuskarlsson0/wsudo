add_executable(wsudo)

target_sources(wsudo
    PRIVATE
        main.cpp
)

add_subdirectory(cli)

target_include_directories(wsudo
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_definitions(wsudo
    PRIVATE
        WSUDO_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        WSUDO_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        WSUDO_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        WSUDO_VERSION_STRING="${PROJECT_VERSION}"
        WSUDO_TARGET_ARCH="${WSUDO_TARGET_ARCH}"
)

if(MSVC)
    target_compile_options(wsudo PRIVATE /W4 $<$<NOT:$<CONFIG:Debug>>:/WX> /permissive-)
else()
    target_compile_options(wsudo PRIVATE -Wall -Wextra -Wpedantic $<$<NOT:$<CONFIG:Debug>>:-Werror> -Wshadow -Wconversion -Wsign-conversion)
endif()
