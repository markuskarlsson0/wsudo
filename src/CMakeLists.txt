add_executable(wsudo)

target_sources(wsudo
    PRIVATE
        main.cpp
)

add_subdirectory(cli)
add_subdirectory(utils)
add_subdirectory(app)
add_subdirectory(process)
add_subdirectory(ipc)

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
        $<$<CONFIG:Debug>:WSUDO_DEBUG_BUILD>
)

target_link_libraries(wsudo
    PRIVATE
        wsudo_warnings
)
