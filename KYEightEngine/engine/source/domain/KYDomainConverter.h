#pragma once

#include "KYDomain.h"
#include "ecs/Registry.h"

namespace KYEight 
{
class KYDomainConverter 
{
public:
	static KYDomain FromRegistry(const Registry& registry);

	static void ToRegistry(const KYDomain& domain, Registry& registry);
};
}