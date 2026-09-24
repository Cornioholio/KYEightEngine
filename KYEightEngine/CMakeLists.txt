cmake_minimum_required(VERSION 3.25)

project(KYEightEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Raylib
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 5.5)
FetchContent_MakeAvailable(raylib)

# Dear ImGUI
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.92.7)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/imgui_demo.cpp
	${imgui_SOURCE_DIR}/imgui_draw.cpp
	${imgui_SOURCE_DIR}/imgui_tables.cpp
	${imgui_SOURCE_DIR}/imgui_widgets.cpp)
	
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# rlImGUI
FetchContent_Declare(rlimgui GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git GIT_TAG Raylib_5_5)
FetchContent_MakeAvailable(rlimgui)

add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_include_directories(rlimgui PUBLIC ${rlimgui_SOURCE_DIR})
target_link_libraries(rlimgui PUBLIC raylib imgui)

# Engine
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS engine/source/*.cpp engine/source/*.h)

add_library(KYEightEngine STATIC ${ENGINE_SOURCES})

target_include_directories(KYEightEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine/source)

target_link_libraries(KYEightEngine PUBLIC raylib imgui rlimgui)

# Editor
add_executable(KYEightEditor editor/source/main.cpp)
target_link_libraries(KYEightEditor PRIVATE KYEightEngine)