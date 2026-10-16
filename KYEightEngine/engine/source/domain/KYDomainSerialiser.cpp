#include "KYDomainSerialiser.h"

#include <cmath>
#include <exception>
#include <fstream>
#include <limits>
#include <string>
#include <utility>

#include "nlohmann/json.hpp"

namespace KYEight 
{
	// Helpers
	namespace 
	{
		void SetError(std::string* error, const std::string& message) 
		{
			if (error != nullptr) 
			{
				*error = message;
			}
		}

		bool ReadVector3(const nlohmann::json& object, const char* key, Vector3& vector, const std::string& entityDescription, std::string* error) 
		{
			const auto iterator = object.find(key);

			if (iterator == object.end() || !iterator->is_array()) 
			{
				SetError(error, entityDescription + ": missing or invalid '" + key + "' vector.");

				return false;
			}

			const nlohmann::json& values = *iterator;

			if (values.size() != 3) 
			{
				SetError(error, entityDescription + ": '" + key + "' must contain exactly 3 values.");

				return false;
			}

			float* components[] =
			{
				&vector.x,
				&vector.y,
				&vector.z
			};

			for (std::size_t i = 0; i < 3; ++i) 
			{
				if (!values[i].is_number()) 
				{
					SetError(error, entityDescription + ": '" + key + "' contains a non-numeric value.");

					return false;
				}

				const double value = values[i].get<double>();

				if (!std::isfinite(value) || value > std::numeric_limits<float>::max() || value < -std::numeric_limits<float>::max()) 
				{
					SetError(error, entityDescription + ": '" + key + "' contains an invalid number.");

					return false;
				}

				*components[i] = static_cast<float>(value);

			}

			return true;
		}
	}

	bool KYDomainSerialiser::Save(const KYDomain& domain, const std::string& filepath, std::string* error) 
	{
		if (error != nullptr) 
		{
			error->clear();
		}

		try 
		{
			nlohmann::json document;

			document["version"] = KYDomain::currentVersion_;
			document["entities"] = nlohmann::json::array();

			for (const KYDomainEntity& entity : domain.entities_) 
			{
				nlohmann::json savedEntity = nlohmann::json::object();

				if (entity.name_.has_value()) 
				{
					savedEntity["name"] =
					{
						{"value", entity.name_->name_}
					};
				}

				if (entity.transform_.has_value()) 
				{
					const Transform3D& transform = *entity.transform_;

					savedEntity["transform"] =
					{
						{
							"position",
							{
								transform.position_.x,
								transform.position_.y,
								transform.position_.z
							}
						},
						{
							"rotation",
							{
								transform.rotation_.x,
								transform.rotation_.y,
								transform.rotation_.z
							}
						},
						{
							"scale",
							{
								transform.scale_.x,
								transform.scale_.y,
								transform.scale_.z

							}
						}
					};
				}

				document["entities"].push_back(std::move(savedEntity));
			}


			std::ofstream file(filepath, std::ios::out | std::ios::trunc);

			if (!file.is_open()) 
			{
				SetError(error, "Could not open scene file for writing: " + filepath);

				return false;
			}

			file << document.dump(4);
			file.flush();

			if (!file) 
			{
				SetError(error, "Failed to write scene file: " + filepath);

				return false;
			}

			file.close();
			
			if (file.fail()) 
			{
				SetError(error, "Failed to close scene file: " + filepath);

				return false;
			}
			
			return true;
		}
		catch (const std::exception& exception) 
		{
			SetError(error, "Failed to save scene: " + std::string(exception.what()));
			return false;
		}
	}

	bool KYDomainSerialiser::Load(KYDomain& domain, const std::string& filepath, std::string* error) 
	{
		if (error != nullptr) 
		{
			error->clear();
		}
		try
		{
			std::ifstream file(filepath);

			if (!file.is_open())
			{
				SetError(error, "Could not open scene file for reading: " + filepath);
				return false;
			}

			nlohmann::json document;
			file >> document;

			if (!document.is_object())
			{
				SetError(error, "Invalid scene file: the root must be a JSON object.");
				return false;
			}

			const auto versionIterator = document.find("version");

			if (versionIterator == document.end() ||
				!versionIterator->is_number_integer())
			{
				SetError(error, "Invalid scene file: missing or invalid version.");
				return false;
			}

			const int version = versionIterator->get<int>();

			if (version != KYDomain::currentVersion_)
			{
				SetError(
					error,
					"Unsupported scene version: " + std::to_string(version));

				return false;
			}

			const auto entitiesIterator = document.find("entities");

			if (entitiesIterator == document.end() ||
				!entitiesIterator->is_array())
			{
				SetError(error, "Invalid scene file: 'entities' must be an array.");
				return false;
			}

			KYDomain loadedDomain;

			for (std::size_t i = 0; i < entitiesIterator->size(); ++i)
			{
				const nlohmann::json& savedEntity = (*entitiesIterator)[i];

				const std::string entityDescription =
					"Entity at index " + std::to_string(i);

				if (!savedEntity.is_object())
				{
					SetError(error, entityDescription + " must be a JSON object.");
					return false;
				}

				KYDomainEntity entity;

				const auto nameIterator = savedEntity.find("name");

				if (nameIterator != savedEntity.end())
				{
					if (!nameIterator->is_object())
					{
						SetError(error, entityDescription + ": 'name' must be an object.");
						return false;
					}

					const auto valueIterator = nameIterator->find("value");

					if (valueIterator == nameIterator->end() ||
						!valueIterator->is_string())
					{
						SetError(
							error,
							entityDescription + ": 'name.value' must be a string.");

						return false;
					}

					NameComponent name;
					name.name_ = valueIterator->get<std::string>();
					entity.name_ = std::move(name);
				}

				const auto transformIterator = savedEntity.find("transform");

				if (transformIterator != savedEntity.end())
				{
					if (!transformIterator->is_object())
					{
						SetError(
							error,
							entityDescription + ": 'transform' must be an object.");

						return false;
					}

					Transform3D transform{};

					if (!ReadVector3(
						*transformIterator,
						"position",
						transform.position_,
						entityDescription,
						error) ||
						!ReadVector3(
							*transformIterator,
							"rotation",
							transform.rotation_,
							entityDescription,
							error) ||
						!ReadVector3(
							*transformIterator,
							"scale",
							transform.scale_,
							entityDescription,
							error))
					{
						return false;
					}

					entity.transform_ = transform;
				}

				loadedDomain.entities_.push_back(std::move(entity));
			}

			domain = std::move(loadedDomain);
			return true;
		}
		catch (const nlohmann::json::exception& exception)
		{
			SetError(error, "Invalid scene JSON: " + std::string(exception.what()));
			return false;
		}
		catch (const std::exception& exception)
		{
			SetError(error, "Failed to load scene: " + std::string(exception.what()));
			return false;
		}
	}

}