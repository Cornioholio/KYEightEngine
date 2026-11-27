include(FetchContent)

# NFD - File dialog
FetchContent_Declare(nfd GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git GIT_TAG v1.3.0 GIT_SHALLOW TRUE)

set(NFD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(nfd)

# Raylib
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 5.5)
FetchContent_MakeAvailable(raylib)

# nlohmann - JSON
FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.12.0)
FetchContent_MakeAvailable(json)

# ImGui and rLImGui
if (KYEIGHT_BUILD_EDITOR)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.7-docking
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

target_compile_features(imgui PUBLIC cxx_std_11)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

FetchContent_Declare(rlimgui GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git GIT_TAG Raylib_5_5)
FetchContent_MakeAvailable(rlimgui)

add_library(KYEightRlImGui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)

target_include_directories(KYEightRlImGui PUBLIC ${rlimgui_SOURCE_DIR})
target_link_libraries(KYEightRlImGui PUBLIC raylib imgui)
endif()