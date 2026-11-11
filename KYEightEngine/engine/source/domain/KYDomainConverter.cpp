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

				if (mesh.primitive_.has_value())
				{
					KYDomainMesh domainMesh;
					domainMesh.primitive_ = *mesh.primitive_;
					domainMesh.colour_ = mesh.colour_;

					domainEntity.mesh_ = domainMesh;
				}
			}

			domain.entities_.push_back(domainEntity);
		}

		return domain;
	}

	void KYDomainConverter::ToRegistry(const KYDomain& domain, Registry& registry)
	{
		for (const KYDomainEntity& domainEntity : domain.entities_) 
		{
			Entity entity = registry.CreateEntity();

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

				MeshComponent mesh;

				mesh.model_ = CreatePrimitiveModel(domainMesh.primitive_);

				mesh.colour_ = domainMesh.colour_;

				mesh.primitive_ = domainMesh.primitive_;

				registry.AddComponent<MeshComponent>(entity, mesh);
			}
		}
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