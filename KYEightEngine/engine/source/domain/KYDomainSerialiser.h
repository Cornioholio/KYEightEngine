#pragma once
#include "KYDomain.h"

namespace KYEight 
{
	class KYDomainSerialiser 
	{
	public:
		static bool Save(const KYDomain& domain, const std::string& filepath, std::string* error = nullptr);
		static bool Load(KYDomain& domain, const std::string& filepath, std::string* error = nullptr);

	};
}