cmake_minimum_required(VERSION 3.25)

project(HelloWorld LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 CONFIG REQUIRED)
if (NOT SDL3_FOUND)
    message(STATUS "SDL3 not found, fetching from source…")
    FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG        release-3.2.22
    )
    FetchContent_MakeAvailable(SDL3)
endif()

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
