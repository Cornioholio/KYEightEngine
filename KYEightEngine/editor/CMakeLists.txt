add_executable(KYEightEditor
	# Explicitly list.cpp
	source/emain.cpp
	source/KYEditor.cpp
	source/PlaySession.cpp
	source/panels/AssetBrowser.cpp
)

# Absolute path to the game project source directory
target_compile_definitions(KYEightEditor PRIVATE
    KYEIGHT_GAME_PROJECT_DIR="${CMAKE_SOURCE_DIR}/gamelib"
)

target_compile_features(KYEightEditor PRIVATE cxx_std_20)

target_include_directories(KYEightEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

target_link_libraries(KYEightEditor PRIVATE KYEight::Engine KYEight::GameLib KYEightRlImGui)

