#pragma once
#include "ecs/components/CoreComponents.h"

#include <optional>
#include <string>
#include <vector>

namespace KYEight 
{
	enum class KYDomainMeshSource
	{
		Primitive,
		File
	};
	struct KYDomainMesh
	{
		KYDomainMeshSource source_ = KYDomainMeshSource::Primitive;
		PrimitiveMeshData primitive_;
		std::string filepath_ = "";
		Color colour_ = RED;
	};

	/// <summary>
	/// Persistent representation of one scene ('domain'), contains serializable data
	/// </summary>
	struct KYDomainEntity
	{
		std::optional<NameComponent> name_;
		std::optional<Transform3D> transform_;
		std::optional<KYDomainMesh> mesh_;
	};

	/// <summary>
	/// Represents contents of a saved scene
	/// </summary>
	struct KYDomain 
	{
		static constexpr int currentVersion_ = 3;

		std::vector<KYDomainEntity> entities_;
	};
}