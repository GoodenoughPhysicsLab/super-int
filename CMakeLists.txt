cmake_minimum_required(VERSION 3.15)

project(sint C)
if (MSVC)
    add_compile_options(/std:c11)
else()
    set(CAMKE_C_STANDARD 99)
endif()
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

if (MSVC)
    add_compile_options(/Zc:preprocessor /utf-8 /DNOMINMAX /D_USE_MATH_DEFINES /bigobj)
else()
    add_compile_options(-w)
    if (WIN32)
        add_compile_options(-finput-charset=utf-8 -fexec-charset=utf-8)
    endif()
endif()

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.c)

# use BUILD_SHARED_LIBS to control STATIC / SHARED
add_library(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "SI_export=__declspec(dllexport)")
endif()
