cmake_minimum_required(VERSION 3.15)

project(sint_test)
enable_testing()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(SINT_SIMD "Enable SINT_SIMD support" OFF)

if (MSVC)
    add_compile_options(/Zc:preprocessor /utf-8 /DNOMINMAX /D_USE_MATH_DEFINES /EHsc /bigobj)
else()
    if (WIN32)
        add_compile_options(-finput-charset=utf-8 -fexec-charset=utf-8)
    endif()
    add_compile_options(-Wall -Wextra)
endif()

# Fetch and build GoogleTest
# https://github.com/google/googletest/tree/main/googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCES main.cc ${CMAKE_SOURCE_DIR}/../sint/si_bigint/si_bigint.c)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

if(SINT_SIMD)
    add_compile_definitions(SINT_SIMD)
    if (MSVC)
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /arch:AVX2)
    else()
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -mavx2)
    endif()
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} gtest_main)
add_test(NAME ${CMAKE_PROJECT_NAME} COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME})
