add_library(KYEightEngine STATIC
	# Explicitly list .cpp's
	# Core
	source/core/KYEngine.cpp
	# Domain
	source/domain/KYDomainSerialiser.cpp
	source/domain/KYDomainConverter.cpp
	# ECS
	source/ecs/Registry.cpp
	# Renderer
	source/renderer/KYRenderer.cpp
	source/renderer/ViewportTarget.cpp
	source/renderer/WindowTarget.cpp
)

add_library(KYEight::Engine ALIAS KYEightEngine)

target_compile_features(KYEightEngine PUBLIC cxx_std_20)
target_include_directories(KYEightEngine PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)

# Public because engine headers expose raylib types
target_link_libraries(KYEightEngine PUBLIC raylib)
target_link_libraries(KYEightEngine PRIVATE nlohmann_json)

# Add jolt when used