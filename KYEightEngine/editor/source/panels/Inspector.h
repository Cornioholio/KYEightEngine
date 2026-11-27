#pragma once

#include "ecs/Registry.h"
#include "ecs/Entity.h"
#include "resources/KYResourceManager.h"

#include <filesystem>
#include <string>
#include <vector>
#include <cctype>
#include <system_error>
#include <algorithm>

namespace KYEightEditor 
{
	class Inspector 
	{
	public:
		explicit Inspector(const std::filesystem::path& assetRoot);

		void Draw(KYEight::Registry& registry, KYEight::Entity selectedEntity, KYEight::KYResourceManager& resourceManager);

	private:
		// Component inspectors
		void DrawName(KYEight::Registry& registry, KYEight::Entity entity);
		void DrawTransform(KYEight::Registry& registrty, KYEight::Entity entity);
		void DrawMesh(KYEight::Registry& registry, KYEight::Entity entity, KYEight::KYResourceManager& resourceManager);
		void DrawDirectionalLight(KYEight::Registry& registry, KYEight::Entity entity);

		void RefreshMeshAssets();

		std::filesystem::path assetRoot_;
		std::vector<std::filesystem::path> meshAssets_;

	};
}