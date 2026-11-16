#include "KYResourceManager.h"

namespace KYEight 
{
	KYResourceManager::~KYResourceManager() 
	{
		Shutdown();
	}

	ModelHandle KYResourceManager::LoadModel(const std::string& filepath)
	{
		if (filepath.empty()) 
		{
			return 0;
		}

		auto iterator = modelPaths_.find(filepath);

		if (iterator != modelPaths_.end()) 
		{
			return iterator->second;
		}

		Model model = ::LoadModel(filepath.c_str());

		if (model.meshCount <= 0 || model.meshes == nullptr)
		{
			return 0;
		}

		ModelHandle handle = RegisterModel(std::move(model));

		if (handle == 0) 
		{
			return 0;
		}

		modelPaths_.emplace(filepath, handle);

		return handle;
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
		modelPaths_.clear();
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

		for (auto pathIterator = modelPaths_.begin(); pathIterator != modelPaths_.end();) 
		{
			if (pathIterator->second == handle) 
			{
				pathIterator = modelPaths_.erase(pathIterator);
			} else 
			{
				++pathIterator;
			}
		}
	}

	void KYResourceManager::Shutdown()
	{
		UnloadAll();
	}
}