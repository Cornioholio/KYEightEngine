cmake_minimum_required(VERSION 3.25)

project(KYEightEngine VERSION 0.1.0 DESCRIPTION "C++ Game Engine and Editor" LANGUAGES CXX)

# Language config
set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build 
option(KYEIGHT_BUILD_EDITOR "Build KYEight Editor" ON)
option(KYEIGHT_BUILD_GAME "Build example game" ON)

# Shared build config
include(CTest)

# Dependencies
include(cmake/Dependencies.cmake)

# Engine must be defined before its consumers
add_subdirectory(engine)

# End-user game code
if (KYEIGHT_BUILD_GAME OR KYEIGHT_BUILD_EDITOR) 
	add_subdirectory(gamelib)
endif()

# Applications
if (KYEIGHT_BUILD_EDITOR)
	add_subdirectory(editor)
endif()

if (KYEIGHT_BUILD_GAME)
	add_subdirectory(gameexe)
endif()

