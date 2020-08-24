cmake_minimum_required(VERSION 3.16)
project(ProgettoAPI C)

set(CMAKE_C_STANDARD 11)

set(GCC_COVERAGE_COMPILE_FLAGS "-DEVAL -std=gnu11 -O2 -pipe -static -lm")

set(CMAKE_C11_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(ProgettoAPI WIN32 main.c)