#pragma once
#include "ecs/components/CoreComponents.h"

#include <optional>
#include <string>
#include <vector>

namespace KYEight 
{
	/// <summary>
	/// Persistent representation of one scene ('domain'), contains serializable data
	/// </summary>
	struct KYDomainEntity
	{
		std::optional<NameComponent> name_;
		std::optional<Transform3D> transform_;
	};

	/// <summary>
	/// Represents contents of a saved scene
	/// </summary>
	struct KYDomain 
	{
		static constexpr int currentVersion_ = 1;

		std::vector<KYDomainEntity> entities_;
	};
}