cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

project (cc4e LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_executable(itox itox.c)
add_executable(uniq uniq.c)
add_executable(calc calc.c)
add_executable(ctof ctof.c)
add_executable(bump bump.c)
add_executable(simple_machine simple_machine.c)
add_executable(touring touring.c)