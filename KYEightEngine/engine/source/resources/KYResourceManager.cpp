#include "KYResourceManager.h"

namespace KYEight 
{
	KYResourceManager::~KYResourceManager() 
	{
		Shutdown();
	}

	ModelHandle KYResourceManager::LoadModel(const std::string& filepath)
	{
		Model model = ::LoadModel(filepath.c_str());

		if (model.meshCount <= 0 || model.meshes == nullptr)
		{
			return 0;
		}

		return RegisterModel(std::move(model));
	}
	ModelHandle KYResourceManager::RegisterModel(Model&& model)
	{
		if (model.meshCount <= 0 || model.meshes == nullptr) 
		{
			return 0;
		}

		ModelHandle handle = nextHandle_++; // Exception thrown. read access violation (this was nullptr)

		models_.emplace(handle, std::move(model));

		return handle;
	}

	Model* KYResourceManager::GetModel(ModelHandle handle)
	{
		auto iterator = models_.find(handle);

		if (iterator == models_.end())
		{
			return nullptr;
		}

		return &iterator->second;
	}
	void KYResourceManager::UnloadAll() 
	{
		for (auto& [handle, model] : models_) 
		{
			::UnloadModel(model);
		}

		models_.clear();
	}
	void KYResourceManager::UnloadModel(ModelHandle handle)
	{
		auto iterator = models_.find(handle);

		if (iterator == models_.end()) 
		{
			return;
		}

		::UnloadModel(iterator->second);

		models_.erase(iterator);
	}

	void KYResourceManager::Shutdown()
	{
		UnloadAll();
	}
}