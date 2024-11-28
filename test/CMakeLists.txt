cmake_minimum_required(VERSION 3.15)

project(sint_test)
enable_testing()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

option(SINT_SIMD "Enable SINT_SIMD support" OFF)

if (MSVC)
    add_compile_options(/Zc:preprocessor /utf-8 /DNOMINMAX /D_USE_MATH_DEFINES /bigobj)
else()
    add_compile_options(-Wall -Wextra -Wno-parentheses)
    if (WIN32)
        add_compile_options(-finput-charset=utf-8 -fexec-charset=utf-8)
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/../include)

set(TEST_SI_BIGINT "test_si_bigint")
add_executable(
    ${TEST_SI_BIGINT}
    ${CMAKE_SOURCE_DIR}/test_si_bigint.c
    ${CMAKE_SOURCE_DIR}/../src/si_bigint.c
)
add_test(NAME ${TEST_SI_BIGINT} COMMAND ${CMAKE_BINARY_DIR}/${TEST_SI_BIGINT})

if(SINT_SIMD)
    add_compile_definitions(SINT_SIMD)
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86"
        OR CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64"
        OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        if (MSVC)
            target_compile_options(${TEST_SI_BIGINT} PRIVATE /arch:AVX2)
        else()
            target_compile_options(${TEST_SI_BIGINT} PRIVATE -mavx2)
        endif()
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        if (MSVC)
            target_compile_options(${TEST_SI_BIGINT} PRIVATE /arch:ARMv7VE) # no tested (win on arm)
        else()
            target_compile_options(${TEST_SI_BIGINT} PRIVATE -mfpu=neon)
        endif()
    endif()
endif()

