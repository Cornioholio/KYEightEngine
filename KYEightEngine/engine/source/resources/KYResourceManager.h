#pragma once
#include "ecs/components/CoreComponents.h"
#include <unordered_map>
#include <vector>
#include <filesystem>

namespace KYEight
{

	/// <summary>
	/// Sensible for runtime resource loading
	/// </summary>
	class KYResourceManager
	{
	public:
		KYResourceManager() = default;
		~KYResourceManager();

		KYResourceManager(const KYResourceManager&) = delete;
		KYResourceManager& operator=(const KYResourceManager&) = delete;

		ModelHandle LoadModel(const std::string& filepath);
		ModelHandle RegisterModel(Model&& model);

		Model* GetModel(ModelHandle handle);
		const Model* GetModel(ModelHandle handle) const;

		void UnloadModel(ModelHandle handle);
		void UnloadAll();
		void Shutdown();

	private:
		struct ModelSlot 
		{
			Model model_{};
			std::string filepath_ = "";

			uint32_t generation_ = 1;
			uint32_t nextFree_ = 0;

			bool loaded_ = false;
		};

		static std::string NormalisePath(const std::string& filepath);

		static ModelHandle CreateHandle(uint32_t index, uint32_t generation);

		static uint32_t GetSlotIndex(ModelHandle handle);
		static uint32_t GetGeneration(ModelHandle handle);

		ModelSlot* GetSlot(ModelHandle handle);
		const ModelSlot* GetSlot(ModelHandle handle) const;

		ModelHandle AllocateSlot(Model&& model, std::string filepath = "");

		std::vector<ModelSlot> models_;

		std::unordered_map<std::string, ModelHandle> modelPaths_;

		uint32_t firstFreeSlot_ = 0;
	};

}