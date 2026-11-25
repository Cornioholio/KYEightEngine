#include "SceneHierarchy.h"

#include "ecs/components/CoreComponents.h"

#include "imgui.h"

#include <algorithm>
#include <string>
#include <vector>

namespace KYEightEditor 
{
	void SceneHierarchy::Draw(KYEight::Registry& registry, KYEight::Entity& selectedEntity) 
	{
		ImGui::Begin("Objects");
		ImGui::Text("Scene");
		ImGui::SameLine();

		bool createRequested = false;

		if (ImGui::Button("+")) 
		{
			ImGui::OpenPopup("CreateEntityPopup");
		}

		if (ImGui::BeginPopup("CreateEntityPopup")) 
		{
			if (ImGui::MenuItem("Empty Entity")) 
			{
				createRequested = true;
			}

			ImGui::EndPopup();
		}

		ImGui::Separator();

		// Clear invalid selection
		if (!registry.IsValid(selectedEntity)) 
		{
			selectedEntity = KYEight::NullEntity;
		}

		// Take snapshot so registry change doesnt invalidate iteration
		std::vector<KYEight::Entity> entities;

		for (KYEight::Entity entity : registry.GetActiveEntities()) 
		{
			if (registry.HasComponent<KYEight::NameComponent>(entity)) 
			{
				entities.push_back(entity);
			}
		}

		std::sort(entities.begin(), entities.end(),
			[&registry](KYEight::Entity a, KYEight::Entity b)
			{
				return registry.GetComponent<KYEight::NameComponent>(a).name_ < registry.GetComponent<KYEight::NameComponent>(b).name_;
			});

		KYEight::Entity pendingDeletion = KYEight::NullEntity;

		// Draw entities 
		for (KYEight::Entity entity : entities) 
		{
			// Only named entities appear in hierarchy
			if (!registry.HasComponent<KYEight::NameComponent>(entity)) 
			{
				continue;
			}

			const auto& name = registry.GetComponent<KYEight::NameComponent>(entity);

			const bool selected = selectedEntity == entity;

			ImGui::PushID(static_cast<int>(entity));

			if (ImGui::Selectable(name.name_.c_str(), selected)) 
			{
				selectedEntity = entity;
			}

			// Entity context menu
			if (ImGui::BeginPopupContextItem("EntityContextMenu")) 
			{
				if (ImGui::MenuItem("Delete")) 
				{
					pendingDeletion = entity;
				}

				ImGui::EndPopup();
			}

			ImGui::PopID();

		}

		ImGui::Separator();


		// Delete selected entity

		if (registry.IsValid(selectedEntity)) 
		{
			if (ImGui::Button("Delete Selected")) 
			{
				pendingDeletion = selectedEntity;
			}
		}

		// Process operations after entity iteration 
		if (pendingDeletion != KYEight::NullEntity) 
		{
			DeleteEntity(registry, pendingDeletion, selectedEntity);
		}

		if (createRequested) 
		{
			CreateEntity(registry, selectedEntity);
		}

		ImGui::End();
	}

	void SceneHierarchy::CreateEntity(KYEight::Registry& registry, KYEight::Entity& selectedEntity) 
	{
		const KYEight::Entity entity = registry.CreateEntity();

		KYEight::NameComponent name;
		name.name_ = GenerateEntityName(registry);

		KYEight::Transform3D transform;

		registry.AddComponent<KYEight::NameComponent>(entity, std::move(name));

		registry.AddComponent<KYEight::Transform3D>(entity, transform);

		// Automatically select newly created entity.
		selectedEntity = entity;
	}
	void SceneHierarchy::DeleteEntity(KYEight::Registry& registry, KYEight::Entity entity, KYEight::Entity& selectedEntity)
	{
		if (!registry.IsValid(entity))
		{
			return;
		}
		registry.DestroyEntity(entity);

		// Clear selection if the deleted entity was selected.
		if (selectedEntity == entity)
		{
			selectedEntity = KYEight::NullEntity;
		}
	}

	std::string SceneHierarchy::GenerateEntityName(const KYEight::Registry& registry) const 
	{
		const std::string baseName = "New Entity";

		bool nameExists = false;

		for (KYEight::Entity entity : registry.GetActiveEntities())
		{
			if (!registry.HasComponent<KYEight::NameComponent>(entity))
			{
				continue;
			}

			if (registry.GetComponent<KYEight::NameComponent>(entity).name_
				== baseName)
			{
				nameExists = true;
				break;
			}
		}

		if (!nameExists)
		{
			return baseName;
		}

		unsigned int suffix = 2;

		while (true)
		{
			const std::string candidate =
				baseName + " (" + std::to_string(suffix) + ")";

			bool exists = false;

			for (KYEight::Entity entity : registry.GetActiveEntities())
			{
				if (!registry.HasComponent<KYEight::NameComponent>(entity))
				{
					continue;
				}

				if (registry.GetComponent<KYEight::NameComponent>(entity).name_
					== candidate)
				{
					exists = true;
					break;
				}
			}

			if (!exists)
			{
				return candidate;
			}

			++suffix;
		}
	}
}