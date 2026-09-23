cmake_minimum_required(VERSION 3.25)

project(KYEightEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 5.5)

FetchContent_MakeAvailable(raylib)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS source/*.cpp source/*.h)

add_executable(KYEightEngine "source/main.cpp" ${ENGINE_SOURCES})

target_link_libraries(KYEightEngine PRIVATE raylib)