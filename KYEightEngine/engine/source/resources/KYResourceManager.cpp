#include "KYResourceManager.h"
namespace Mask 
{
	constexpr uint32_t INDEX_MASK = 0x000FFFFF;
}
namespace KYEight 
{
	KYResourceManager::~KYResourceManager() 
	{
		Shutdown();
	}

	// Handle management
	ModelHandle KYResourceManager::CreateHandle(uint32_t index, uint32_t generation) 
	{
		// Reserve zero as invalid handle

		return (generation << 20) | ((index + 1) & Mask::INDEX_MASK);
	}

	uint32_t KYResourceManager::GetSlotIndex(ModelHandle handle) 
	{
		const uint32_t encodedIndex = handle & Mask::INDEX_MASK;

		if (encodedIndex == 0) 
		{
			return UINT32_MAX;
		}

		return encodedIndex - 1;
	}

	uint32_t KYResourceManager::GetGeneration(ModelHandle handle) 
	{
		return handle >> 20;
	}

	// Slot access
	KYResourceManager::ModelSlot* KYResourceManager::GetSlot(ModelHandle handle)
	{
		const uint32_t index = GetSlotIndex(handle);

		if (index >= models_.size()) 
		{
			return nullptr;
		}

		ModelSlot& slot = models_[index];

		if (!slot.loaded_ || slot.generation_ != GetGeneration(handle)) 
		{
			return nullptr;
		}

		return &slot;
	}

	const KYResourceManager::ModelSlot* KYResourceManager::GetSlot(ModelHandle handle) const 
	{
		const uint32_t index = GetSlotIndex(handle);

		if (index >= models_.size())
		{
			return nullptr;
		}

		const ModelSlot& slot = models_[index];

		if (!slot.loaded_ || slot.generation_ != GetGeneration(handle))
		{
			return nullptr;
		}

		return &slot;
	}

	// Path management

	std::string KYResourceManager::NormalisePath(const std::string& filepath) 
	{
		if (filepath.empty()) 
		{
			return {};
		}

		return std::filesystem::path(filepath).lexically_normal().generic_string();
	}

	// Allocating
	ModelHandle KYResourceManager::AllocateSlot(Model&& model, std::string filepath) 
	{
		uint32_t index;

		if (firstFreeSlot_ != 0) 
		{
			index = firstFreeSlot_ - 1;

			ModelSlot& slot = models_[index];

			firstFreeSlot_ = slot.nextFree_;

			slot.model_ = std::move(model);
			slot.filepath_ = std::move(filepath);
			slot.loaded_ = true;
			slot.nextFree_ = 0;
		} else 
		{
			index = static_cast<uint32_t>(models_.size());

			if (index >= 0x000FFFFF) 
			{
				return 0;
			}

			ModelSlot slot;
			slot.model_ = std::move(model);
			slot.filepath_ = std::move(filepath);
			slot.loaded_ = true;

			models_.push_back(std::move(slot));
		}

		return CreateHandle(index, models_[index].generation_);
	}

	// Loading
	ModelHandle KYResourceManager::LoadModel(const std::string& filepath)
	{
		if (filepath.empty()) 
		{
			return 0;
		}
		
		const std::string normalisedPath = NormalisePath(filepath);

		auto iterator = modelPaths_.find(normalisedPath);

		if (iterator != modelPaths_.end()) 
		{
			if (GetModel(iterator->second) != nullptr) 
			{
				return iterator->second;
			}

			modelPaths_.erase(iterator);
		}

		Model model = ::LoadModel(filepath.c_str());

		if (model.meshCount <= 0 || model.meshes == nullptr)
		{
			return 0;
		}

		ModelHandle handle = AllocateSlot(std::move(model), normalisedPath);

		if (handle == 0) 
		{
			::UnloadModel(model);
			return 0;
		}

		modelPaths_.emplace(normalisedPath, handle);

		return handle;
	}
	ModelHandle KYResourceManager::RegisterModel(Model&& model)
	{
		if (model.meshCount <= 0 || model.meshes == nullptr) 
		{
			return 0;
		}
		
		return AllocateSlot(std::move(model));
	}

	Model* KYResourceManager::GetModel(ModelHandle handle)
	{
		ModelSlot* slot = GetSlot(handle);

		if (slot == nullptr)
		{
			return nullptr;
		}

		return &slot->model_;
	}

	const Model* KYResourceManager::GetModel(ModelHandle handle) const
	{
		const ModelSlot* slot = GetSlot(handle);

		if (slot == nullptr)
		{
			return nullptr;
		}

		return &slot->model_;
	}

	// Unloading
	void KYResourceManager::UnloadAll() 
	{
		for (uint32_t index = 0; index < models_.size(); ++index) 
		{
			ModelSlot& slot = models_[index];

			if (!slot.loaded_) 
			{
				continue;
			}

			::UnloadModel(slot.model_);

			slot.model_ = {};
			slot.filepath_.clear();
			slot.loaded_ = false;

			slot.generation_ = (slot.generation_ + 1) & 0xFFF;

			if (slot.generation_ == 0) 
			{
				slot.generation_ = 1;
			}
		}

		modelPaths_.clear();

		firstFreeSlot_ = 0;

		for (uint32_t index = 0; index < models_.size(); ++index) 
		{
			models_[index].nextFree_ = firstFreeSlot_;

			firstFreeSlot_ = index + 1;
		}
	}
	void KYResourceManager::UnloadModel(ModelHandle handle)
	{
		ModelSlot* slot = GetSlot(handle);

		if (slot == nullptr) 
		{
			return;
		}

		if (!slot->filepath_.empty()) 
		{
			modelPaths_.erase(slot->filepath_);
		}

		::UnloadModel(slot->model_);

		slot->model_ = {};
		slot->filepath_.clear();
		slot->loaded_ = false;

		slot->generation_ = (slot->generation_ + 1) & 0xFFF;

		if (slot->generation_ == 0) 
		{
			slot->generation_ = 1;
		}

		const uint32_t index = GetSlotIndex(handle);

		slot->nextFree_ = firstFreeSlot_;
		firstFreeSlot_ = index + 1;
	}

	void KYResourceManager::Shutdown()
	{
		UnloadAll();
	}
}