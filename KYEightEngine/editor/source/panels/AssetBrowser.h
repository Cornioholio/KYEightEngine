#pragma once
#include <filesystem>
#include <vector>
#include <string>

namespace KYEightEditor 
{
	class AssetBrowser 
	{
	public:
		explicit AssetBrowser(const std::filesystem::path& assetRoot);

		void Draw();
	private:
		struct AssetEntry 
		{
			std::filesystem::path filepath_;
			bool isDirectory_ = false;
		};

		void RefreshDirectory();
		void NavigateTo(const std::filesystem::path& directory);

		std::filesystem::path rootPath_;
		std::filesystem::path currentPath_;	

		std::vector<AssetEntry> entries_;

		std::filesystem::path selectedAsset_;

		char searchBuffer_[256] = {};

		std::string errorMessage_;
	};
}