#include "Inspector.h"

#include "imgui.h"

#include <filesystem>
#include <array>

namespace KYEightEditor
{
	Inspector::Inspector(const std::filesystem::path& assetRoot)
	{
		std::error_code error;

		assetRoot_ = std::filesystem::absolute(assetRoot, error).lexically_normal();

		if (!error) 
		{
			RefreshMeshAssets();
		}
	}	

	void Inspector::Draw(KYEight::Registry& registry, KYEight::Entity selectedEntity, KYEight::KYResourceManager& resourceManager)
	{
		ImGui::Begin("Inspector");

		if (!registry.IsValid(selectedEntity)) 
		{
			ImGui::TextDisabled("No entity selected");
			ImGui::End();
			return;
		}

		DrawName(registry, selectedEntity);

		ImGui::Separator();

		DrawTransform(registry, selectedEntity);

		ImGui::Separator();

		DrawMesh(registry, selectedEntity, resourceManager);

		ImGui::End();
	}

	void Inspector::DrawName(KYEight::Registry& registry, KYEight::Entity entity)
	{
		if (!registry.HasComponent<KYEight::NameComponent>(entity)) 
		{
			registry.AddComponent<KYEight::NameComponent>(entity, KYEight::NameComponent{});
		}

		auto& name = registry.GetComponent<KYEight::NameComponent>(entity);

		std::array<char, 256> nameBuffer{};

		std::strncpy(nameBuffer.data(), name.name_.c_str(), nameBuffer.size() - 1);

		ImGui::SetNextItemWidth(-1.f);

		if (ImGui::InputText("##EntityName", nameBuffer.data(), nameBuffer.size())) 
		{
			name.name_ = nameBuffer.data();
		}
	}
	void Inspector::DrawTransform(KYEight::Registry& registry, KYEight::Entity entity)
	{
		if (!registry.HasComponent<KYEight::Transform3D>(entity)) 
		{
			if (ImGui::Button("Add Transform3D")) 
			{
				registry.AddComponent<KYEight::Transform3D>(entity, KYEight::Transform3D{});
			}

			return;
		}

		auto& transform = registry.GetComponent<KYEight::Transform3D>(entity);

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			ImGui::DragFloat3("Position", &transform.position_.x, 0.1f);
			ImGui::DragFloat3("Rotation", &transform.rotation_.x, 0.1f);
			ImGui::DragFloat3("Scale", &transform.scale_.x, 0.1f);
		}
	}
	void Inspector::DrawMesh(KYEight::Registry& registry, KYEight::Entity entity, KYEight::KYResourceManager& resourceManager)
	{
		if (!registry.HasComponent<KYEight::MeshComponent>(entity)) 
		{
			if (ImGui::Button("Add Mesh Component")) 
			{
				registry.AddComponent<KYEight::MeshComponent>(entity, KYEight::MeshComponent{});
			}

			return;
		}

		auto& mesh = registry.GetComponent<KYEight::MeshComponent>(entity);

		if (!ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			return;
		}

		const char* preview = "Select OBJ";

		if (mesh.filepath_.has_value()) 
		{
			preview = mesh.filepath_->c_str();
		}

		if (ImGui::BeginCombo("Model", preview)) 
		{
			RefreshMeshAssets();

			for (const auto& asset : meshAssets_) 
			{
				const std::string relativePath = asset.lexically_relative(assetRoot_).generic_string();

				const bool selected = mesh.filepath_.has_value() && *mesh.filepath_ == relativePath;

				if (ImGui::Selectable(relativePath.c_str(), selected)) 
				{
					const KYEight::ModelHandle handle = resourceManager.LoadModel(relativePath);

					if (handle != 0) 
					{
						mesh.modelHandle_ = handle;
						mesh.filepath_ = relativePath;
						mesh.primitive_.reset();

						TraceLog(LOG_INFO, "Inspector: Loaded model: %s", asset.string().c_str());
					} else 
					{
						mesh.modelHandle_ = 0;
						mesh.filepath_.reset();

						TraceLog(LOG_INFO, "Inspector: Failed to load model: %s", asset.string().c_str());
					}
				}

				if (selected) 
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		float colour[4] =
		{
			mesh.colour_.r / 255.f,
			mesh.colour_.g / 255.f,
			mesh.colour_.b / 255.f,
			mesh.colour_.a / 255.f
		};

		if (ImGui::ColorEdit4("Colour", colour)) 
		{
			mesh.colour_.r = static_cast<unsigned char>(colour[0] * 255.f);
			mesh.colour_.g = static_cast<unsigned char>(colour[1] * 255.f);
			mesh.colour_.b = static_cast<unsigned char>(colour[2] * 255.f);
			mesh.colour_.a = static_cast<unsigned char>(colour[3] * 255.f);
		}
	}

	void Inspector::RefreshMeshAssets() 
	{
		meshAssets_.clear();

		const std::filesystem::path modelsDirectory = assetRoot_ / "assets";

		std::error_code error;

		if (!std::filesystem::exists(modelsDirectory, error) || 
			!std::filesystem::is_directory(modelsDirectory, error)) 
		{
			return;
		}

		// Search assets recursively for objs
		std::filesystem::recursive_directory_iterator iterator(modelsDirectory, std::filesystem::directory_options::skip_permission_denied, error);

		const std::filesystem::recursive_directory_iterator end;

		while (iterator != end) 
		{
			if (iterator->is_regular_file(error)) 
			{
				std::string extension = iterator->path().extension().string();

				std::transform(
					extension.begin(),
					extension.end(),
					extension.begin(),
					[](unsigned int character)
					{
						return static_cast<char>(std::tolower(character));
					});

				if (extension == ".obj") 
				{
					meshAssets_.push_back(iterator->path().lexically_normal());
				}
			}

			iterator.increment(error);

			if (error) 
			{
				error.clear();
			}
		}

		std::sort(
		meshAssets_.begin(),
		meshAssets_.end(),
		[](const std::filesystem::path& a, const std::filesystem::path& b)
		{
			return a.generic_string() < b.generic_string();
		});

	}
}