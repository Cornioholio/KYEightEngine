#pragma once
#include "ecs/Registry.h"
#include "ecs/components/CoreComponents.h"

#include "raylib.h"

namespace KYEight 
{
	class MeshSystem 
	{
	public:
		void Render(const Registry& registry, const Camera3D& camera) const 
		{
			BeginMode3D(camera);

			for (Entity entity : registry.GetActiveEntities()) 
			{
				// Require both transform3d and mesh components
				if (!registry.HasComponent<Transform3D>(entity) || !registry.HasComponent<MeshComponent>(entity)) 
				{
					continue;
				}

				const Transform3D& transform = registry.GetComponent<Transform3D>(entity);

				const MeshComponent& mesh = registry.GetComponent<MeshComponent>(entity);

				DrawCubeV(transform.position_, transform.scale_, mesh.colour_);
			}

			EndMode3D();
		}
	};
}

