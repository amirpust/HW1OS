cmake_minimum_required(VERSION 3.14)
project(HW1)

set(CMAKE_CXX_STANDARD 11)

add_executable(HW1 main.cpp Commands.h Commands.cpp JobsList.cpp JobsList.h SmallShell.cpp SmallShell.h smash.cpp CommandExceptions.h libs.h Helpers.cpp Helpers.h)