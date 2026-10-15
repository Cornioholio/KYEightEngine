#pragma once
#include <fstream>
#include <exception>
#include <string>

#include "nlohmann/json.hpp"

#include "KYDomain.h"

namespace KYEight 
{
	class KYDomainSerialiser 
	{
	public:
		static bool Save(const KYDomain& domain, const std::string& filepath, std::string* error = nullptr);
		static bool Load(const KYDomain& domain, const std::string& filepath, std::string* error = nullptr);

	private:
	};
}