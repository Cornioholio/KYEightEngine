#pragma once
#include "ecs/components/CoreComponents.h"
#include <unordered_map>

namespace KYEight
{
	class KYResourceManager
	{
	public:
		KYResourceManager() = default;
		~KYResourceManager();

		KYResourceManager(const KYResourceManager&) = delete;
		KYResourceManager& operator=(const KYResourceManager) = delete;

		ModelHandle LoadModel(const std::string& filepath);
		ModelHandle RegisterModel(Model&& model);

		Model* GetModel(ModelHandle handle);

		void UnloadModel(ModelHandle handle);
		void UnloadAll();
		void Shutdown();

	private:
		std::unordered_map<ModelHandle, Model> models_;
		ModelHandle nextHandle_ = 1;

	};

}