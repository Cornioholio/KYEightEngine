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
		void Render(Registry& registry, KYResourceManager& resourceManager, const Camera3D& camera, const Shader& lightingShader) const 
		{
			// Default lighting when no directional light exists
			Vector3 lightDir = { 0.f, 1.f, 0.f };
			Vector3 lightCol = { 1.f, 1.f, 1.f };
			float lightIntensity = 0.f;

			constexpr float ambientStrength = 0.15f;

			for (Entity entity : registry.GetActiveEntities()) 
			{
				if (!registry.HasComponent<DirectionalLight>(entity) ||
					!registry.HasComponent<Transform3D>(entity)) 
				{
					continue;
				}

				const DirectionalLight& light = registry.GetComponent<DirectionalLight>(entity);
				const Transform3D& transform = registry.GetComponent<Transform3D>(entity);

				Matrix rotation = MatrixRotateXYZ({
					transform.rotation_.x * DEG2RAD,
					transform.rotation_.y * DEG2RAD,
					transform.rotation_.z * DEG2RAD
					});

				// Entity forward direction is local -z
				// Shader needs direction towards light
				Vector3 forward = Vector3Transform({ 0.f, 0.f, -1.f }, rotation);
				lightDir = Vector3Scale(Vector3Normalize(forward), -1.f);

				lightCol =
				{
					light.colour_.r / 255.f,
					light.colour_.g / 255.f,
					light.colour_.b / 255.f
				};

				lightIntensity = light.intensity_;

				break;
			}

			// Upload lighting info
			SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "lightDirection"), &lightDir, SHADER_UNIFORM_VEC3);

			SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "lightColour"), &lightCol, SHADER_UNIFORM_VEC3);

			SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "lightIntensity"), &lightIntensity, SHADER_UNIFORM_FLOAT);

			SetShaderValue(lightingShader, GetShaderLocation(lightingShader, "ambientStrength"), &ambientStrength, SHADER_UNIFORM_FLOAT);


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

				// Temporarily replace material shaders, restore after so resource manager remains unchanged
				std::vector<Shader> originalShaders;

				if (model->materials != nullptr) 
				{
					originalShaders.reserve(model->materialCount);

					for (int i = 0; i < model->materialCount; ++i) 
					{
						originalShaders.push_back(model->materials[i].shader);
						model->materials[i].shader = lightingShader;
					}
				}

				DrawModel(*model, Vector3Zero(), 1.0f, mesh.colour_);

				// Restore
				if (model->materials != nullptr) 
				{
					for (int i = 0; i < model->materialCount; ++i) 
					{
						model->materials[i].shader = originalShaders[i];
					}
				}
			}
			EndMode3D();
		}
	};
}

