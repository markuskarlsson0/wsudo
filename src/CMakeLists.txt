add_executable(wsudo)

target_sources(wsudo
    PRIVATE
        main.cpp
)

add_subdirectory(cli)
add_subdirectory(utils)
add_subdirectory(app)
add_subdirectory(process)

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
    target_compile_options(wsudo
        PRIVATE
            /W4
            /permissive-
            $<$<NOT:$<CONFIG:Debug>>:/WX>
    )
else()
    target_compile_options(wsudo
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            $<$<NOT:$<CONFIG:Debug>>:-Werror>
    )
endif()
