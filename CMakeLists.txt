cmake_minimum_required(VERSION 3.10)

project(HelloWorld LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL3 CONFIG REQUIRED)

add_executable(hello
        src/main.cpp
)

target_link_libraries(hello PRIVATE SDL3::SDL3)
