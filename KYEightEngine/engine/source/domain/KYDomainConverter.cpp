#include "KYDomainConverter.h"

namespace KYEight
{
	// Creating a domain from registry
	KYDomain KYDomainConverter::FromRegistry(const Registry& registry) 
	{
		KYDomain domain;

		for (Entity entity : registry.GetActiveEntities()) 
		{
			KYDomainEntity domainEntity;

			if (registry.HasComponent<NameComponent>(entity)) 
			{
				domainEntity.name_ = registry.GetComponent<NameComponent>(entity);
			}
			if (registry.HasComponent<Transform3D>(entity)) 
			{
				domainEntity.transform_ = registry.GetComponent<Transform3D>(entity);
			}
			if (registry.HasComponent<MeshComponent>(entity))
			{
				const MeshComponent& mesh =
					registry.GetComponent<MeshComponent>(entity);

				KYDomainMesh domainMesh;
				domainMesh.colour_ = mesh.colour_;

				if (mesh.primitive_.has_value())
				{
					domainMesh.primitive_ = *mesh.primitive_;
					domainMesh.colour_ = mesh.colour_;

					domainEntity.mesh_ = domainMesh;
				} else if (mesh.filepath_.has_value()) 
				{
					domainMesh.source_ = KYDomainMeshSource::File;
					domainMesh.filepath_ = *mesh.filepath_;

					domainEntity.mesh_ = domainMesh;
				}
			}
			if (registry.HasComponent<DirectionalLight>(entity)) 
			{
				domainEntity.directionalLight_ = registry.GetComponent<DirectionalLight>(entity);
			}

			domain.entities_.push_back(domainEntity);
		}

		return domain;
	}

	bool KYDomainConverter::ToRegistry(const KYDomain& domain, Registry& registry, KYResourceManager& resourceManager, std::string* error)
	{
		if (error != nullptr) 
		{
			error->clear();
		}

		std::vector<Entity> createdEntities;
		std::vector<ModelHandle> registeredModels;
		
		auto cleanup = [&]()
		{
			for (Entity entity : createdEntities)
			{
				registry.DestroyEntity(entity);
			}
			for (ModelHandle handle : registeredModels)
			{
				resourceManager.UnloadModel(handle);
			}
		};

		for (const KYDomainEntity& domainEntity : domain.entities_) 
		{
			Entity entity = registry.CreateEntity();
			createdEntities.push_back(entity);

			if (domainEntity.name_.has_value()) 
			{
				registry.AddComponent<NameComponent>(entity, *domainEntity.name_);
			}

			if (domainEntity.transform_.has_value()) 
			{
				registry.AddComponent<Transform3D>(entity, *domainEntity.transform_);
			}

			if (domainEntity.mesh_.has_value()) 
			{
				const KYDomainMesh& domainMesh = *domainEntity.mesh_;
				
				ModelHandle handle = 0;

				if (domainMesh.source_ == KYDomainMeshSource::Primitive) 
				{
					Model model = CreatePrimitiveModel(domainMesh.primitive_);

					if (model.meshCount <= 0 || model.meshes == nullptr) 
					{
						if (error != nullptr) 
						{
							*error = "[ENGINE] Failed to create primitive model\n";
						}

						cleanup();
						return false;
					}
				
					handle = resourceManager.RegisterModel(std::move(model));
					
					if (handle == 0) 
					{
						if (error != nullptr) 
						{
							*error = "[ENGINE] Failed to register primitive model\n";
						}

						cleanup();
						return false;
					}

					registeredModels.push_back(handle);
				}
				else if (domainMesh.source_ == KYDomainMeshSource::File)
				{
					handle = resourceManager.LoadModel(domainMesh.filepath_);

					if (handle == 0)
					{
						if (error != nullptr)
						{
							*error = "[ENGINE] Failed to load model from file\n";
						}

						cleanup();

						return false;
					}
				} else 
				{
					if (error != nullptr) 
					{
						*error = "[ENGINE] Unsupported mesh source\n";
					}

					cleanup();
					return false;
				}

				MeshComponent mesh;
				mesh.modelHandle_ = handle;
				mesh.colour_ = domainMesh.colour_;

				if (domainMesh.source_ == KYDomainMeshSource::Primitive)
				{
					mesh.primitive_ = domainMesh.primitive_;
				}
				else
				{
					mesh.filepath_ = domainMesh.filepath_;
				}

				registry.AddComponent<MeshComponent>(entity, mesh);

			}

			if (domainEntity.directionalLight_.has_value()) 
			{
				if (registry.HasComponent<MeshComponent>(entity)) 
				{
					if (error != nullptr) 
					{
						*error = "[ENGINE] Entity cannot have both mesh component and directional light\n";
					}

					cleanup();
					return false;
				}

				registry.AddComponent<DirectionalLight>(entity, *domainEntity.directionalLight_);
			}
		}

		return true;
	}


	Model KYDomainConverter::CreatePrimitiveModel(const PrimitiveMeshData& primitive) 
	{
		switch (primitive.type_) 
		{
			case MeshPrimitive::Cube:
			{
				Mesh mesh = GenMeshCube(
					primitive.dimensions_.x,
					primitive.dimensions_.y,
					primitive.dimensions_.z);

				return LoadModelFromMesh(mesh);
			}
			case MeshPrimitive::Sphere: 
			{
				Mesh mesh = GenMeshSphere(
					primitive.dimensions_.x,
					primitive.rings_, 
					primitive.slices_);

				return LoadModelFromMesh(mesh);
			}
		}

		return {};
	}
}