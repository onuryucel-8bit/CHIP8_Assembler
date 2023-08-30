cmake_minimum_required(VERSION 3.0)

project(Chip8_Assembler VERSION 0.6)

set(CMAKE_CXX_STANDARD 11)

add_executable(AssemblerCH8 Main.cpp Assembler_ch8.cpp)

target_include_directories(AssemblerCH8 PRIVATE ${CMAKE_SOURCE_DIR})