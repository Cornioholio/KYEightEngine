#pragma once

#include "KYDomain.h"

#include "resources/KYResourceManager.h"

#include "ecs/Registry.h"

namespace KYEight 
{
	class KYDomainConverter 
	{
	public:
		static KYDomain FromRegistry(const Registry& registry);

		static bool ToRegistry(const KYDomain& domain, Registry& registry, KYResourceManager& resourceManager, std::string* error = nullptr);

	private:
		static Model CreatePrimitiveModel(const PrimitiveMeshData& primitive);

	};
}