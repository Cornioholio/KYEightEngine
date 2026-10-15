#include "KYDomainSerialiser.h"

namespace KYEight 
{
	bool KYDomainSerialiser::Save(const KYDomain& domain, const std::string& filepath, std::string* error) 
	{
		try 
		{
			nlohmann::json document;

			document["version"] = KYDomain::currentVersion_;
			document["entities"] = nlohmann::json::array();

			for (const KYDomainEntity& entity : domain.entities_) 
			{
				nlohmann::json savedEntity;

				if (entity.name_.has_value()) 
				{
					savedEntity["name"] = { { "value", entity.name_->name_ } };
				}

				if (entity.transform_.has_value()) 
				{
					const Transform3D& transform = *entity.transform_;

					savedEntity["transform"] = {
						{ "position", {
							transform.position_.x,
							transform.position_.y,
							transform.position_.z
						}},
						{ "rotation", {
							transform.rotation_.x,
							transform.rotation_.y,
							transform.rotation_.z
						}},
						{ "scale", {
							transform.scale_.x,
							transform.scale_.y,
							transform.scale_.z
						}}
					};
				}
				document["entities"].push_back(savedEntity);
			}

			std::ofstream file(filepath);

			if (!file.is_open()) 
			{
				if (error) 
				{
					*error = "Could not open scene file for writing: " + filepath;
				}

				return false;
			}

			file << document.dump(4);
			file.close();

			if (file.fail()) 
			{
				if (error) 
				{
					*error = "Failed to write scene file: " + filepath;
				}

				return false;
			}

			return true;

		}
		catch (const std::exception& exception) 
		{
			if (error) 
			{
				*error = exception.what();
			}

			return false;
		}
	}
	bool KYDomainSerialiser::Load(const KYDomain& domain, const std::string& filepath, std::string* error) 
	{
		return false;
	}
}