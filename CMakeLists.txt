cmake_minimum_required(VERSION 3.20)
project(MainProject LANGUAGES CXX)

option(WINBUILD "Enable /clr mode" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set(CMAKE_OBJECT_PATH_MAX 2000)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (MSVC)
    add_compile_options(
        /Zc:__cplusplus
        /permissive-
        /std:c++latest
        /EHsc
    )

    if (WINBUILD)
        add_compile_options(/clr)
    endif()
endif()

file(
    GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/*.cpp"
)

add_executable(main ${SOURCES})
