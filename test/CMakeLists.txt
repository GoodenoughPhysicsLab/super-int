cmake_minimum_required(VERSION 3.15)

project(test_sint C)
set(CAMKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
enable_testing()

option(SINT_SIMD "Enable SINT_SIMD support" OFF)

if (MSVC)
    add_compile_options(/Zc:preprocessor /utf-8 /DNOMINMAX /D_USE_MATH_DEFINES /bigobj)
else()
    add_compile_options(-Wall -Wextra -Wno-parentheses)
    if (WIN32)
        add_compile_options(-finput-charset=utf-8 -fexec-charset=utf-8)
    endif()
endif()

if(SINT_SIMD)
    add_compile_definitions(SINT_SIMD)
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86"
        OR CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64"
        OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        if (MSVC)
            add_compile_options(/arch:AVX2)
        else()
            add_compile_options(-mavx2)
        endif()
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        if (MSVC)
            add_compile_options(/arch:ARMv7VE) # no tested (win on arm)
        else()
            add_compile_options(-mfpu=neon)
        endif()
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/../include)

file(GLOB_RECURSE TEST_SRCS ${CMAKE_SOURCE_DIR}/*.c)

foreach(a_test IN LISTS TEST_SRCS)
    get_filename_component(filename ${a_test} NAME_WE)
    add_executable(${filename} ${a_test} ${CMAKE_SOURCE_DIR}/../src/si_bigint.c)
    add_test(NAME ${filename} COMMAND ${CMAKE_BINARY_DIR}/${filename})
endforeach()
