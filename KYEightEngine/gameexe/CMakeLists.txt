add_executable(KYGame 
	# List explicitly
	source/gmain.cpp
	)

target_compile_features(KYGame PRIVATE cxx_std_20)

target_include_directories(KYGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

target_link_libraries(KYGame PRIVATE KYEight::GameLib)

# Stage game assets alongside the standalone game executable on every build
add_custom_target(KYGameAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/gamelib/assets"
        "$<TARGET_FILE_DIR:KYGame>/assets"
    VERBATIM
)

add_dependencies(KYGame KYGameAssets)