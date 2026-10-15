add_library(KYGameLibrary STATIC
	# Explicitly list
	source/KYGame.cpp
)

add_library(KYEight::GameLib ALIAS KYGameLibrary)
target_compile_features(KYGameLibrary PUBLIC cxx_std_20)
target_include_directories(KYGameLibrary PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)

target_link_libraries(KYGameLibrary PUBLIC KYEight::Engine)
