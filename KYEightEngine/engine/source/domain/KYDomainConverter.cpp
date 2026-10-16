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
		}
	}
}