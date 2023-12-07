cmake_minimum_required(VERSION 3.0.0)
project(PrintFileUtility VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(PrintFileUtility PrintFile.cpp PrintFile.h)

