add_executable(KYGame 
	# List explicitly
	source/gmain.cpp
	)

target_compile_features(KYGame PRIVATE cxx_std_20)

target_include_directories(KYGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

target_link_libraries(KYGame PRIVATE KYEight::GameLib)