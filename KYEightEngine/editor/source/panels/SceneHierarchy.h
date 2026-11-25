#pragma once
#include "ecs/Registry.h"

namespace KYEightEditor 
{
	/// <summary>
	/// Displays and manages entities belonging to the current scene.
	/// Handles entity creation, deletion and selection
	/// </summary>
	class SceneHierarchy 
	{
	public:
		void Draw(KYEight::Registry& registry, KYEight::Entity& selectedEntity);

	private:
		void CreateEntity(KYEight::Registry& registry, KYEight::Entity& selectedEntity);
		void DeleteEntity(KYEight::Registry& registry, KYEight::Entity entity, KYEight::Entity& selectedEntity);

		std::string GenerateEntityName(const KYEight::Registry& registry) const;
	};
}