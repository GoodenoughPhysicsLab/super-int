cmake_minimum_required(VERSION 3.5)
project(test)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} test_bitint.cc)
