cmake_minimum_required(VERSION 3.21)
project(8080_emulator C)

set(CMAKE_C_STANDARD 23)

add_executable(8080_emulator
        src/main.c
        src/cpu.c
        src/ram.c
)

target_include_directories(8080_emulator PRIVATE include)
