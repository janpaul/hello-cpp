cmake_minimum_required(VERSION 3.25)

project(HelloWorld LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 CONFIG REQUIRED)

add_executable(
        hello
        src/main.cpp
)

target_link_libraries(hello PRIVATE SDL3::SDL3)

if (MSVC)
    target_compile_options(hello PRIVATE /W4 /permissive-)
else()
    target_compile_options(hello PRIVATE -Wall -Wextra -Wpedantic)
endif()
