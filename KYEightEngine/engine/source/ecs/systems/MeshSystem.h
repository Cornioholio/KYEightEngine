#pragma once
#include "ecs/Registry.h"
#include "ecs/components/CoreComponents.h"

#include "resources/KYResourceManager.h"

#include "raylib.h"
#include "raymath.h"

namespace KYEight 
{
	class MeshSystem 
	{
	public:
		void Render(Registry& registry, KYResourceManager& resourceManager, const Camera3D& camera) const 
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

				Model* model = resourceManager.GetModel(mesh.modelHandle_);

				if (model == nullptr) 
				{
					TraceLog(LOG_WARNING,
						"MeshSystem: Failed to retrieve model for entity %u. Handle: %u",
						entity, mesh.modelHandle_);
					continue;
				}

				Matrix rotation = MatrixRotateXYZ({ 
					transform.rotation_.x * DEG2RAD, 
					transform.rotation_.y * DEG2RAD, 
					transform.rotation_.z * DEG2RAD });

				Matrix scale = MatrixScale(
					transform.scale_.x,
					transform.scale_.y,
					transform.scale_.z );

				Matrix translation = MatrixTranslate(
					transform.position_.x, 
					transform.position_.y, 
					transform.position_.z );

				
				model->transform = MatrixMultiply(MatrixMultiply(scale, rotation), translation);

				DrawModel(*model, Vector3Zero(), 1.0f, mesh.colour_);
			}
			EndMode3D();
		}
	};
}

