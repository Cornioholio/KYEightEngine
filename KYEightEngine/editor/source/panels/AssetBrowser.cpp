	#include "AssetBrowser.h"

	#include "imgui.h"
	#include "nfd.h"

	#include <filesystem>
	#include <string>
	#include <algorithm>
	#include <system_error>

	namespace KYEightEditor 
	{
		AssetBrowser::AssetBrowser(const std::filesystem::path& assetRoot) 
		{
			std::error_code error;

			rootPath_ = std::filesystem::absolute(assetRoot, error).lexically_normal();

			if (error) 
			{
				errorMessage_ = "Could not resolve asset directory";
				return;
			}

			currentPath_ = rootPath_;

			RefreshDirectory();
		}

		void AssetBrowser::RefreshDirectory() 
		{
			entries_.clear();
			errorMessage_.clear();

			std::error_code error;

			if (!std::filesystem::exists(currentPath_, error) ||
				!std::filesystem::is_directory(currentPath_, error)) 
			{
				errorMessage_ = "Directory does not exist";
				return;
			}

			std::filesystem::directory_iterator iterator(currentPath_, error);

			if (error) 
			{
				errorMessage_ = "Could not read directory: " + error.message();
				return;
			}

			for (const auto& entry : iterator) 
			{
				std::error_code entryError;

				AssetEntry asset;
				asset.filepath_ = entry.path();
				asset.isDirectory_ = entry.is_directory(entryError);

				if (entryError) 
				{
					continue;
				}

				entries_.push_back(asset);
			}

			// Display folders before files
			std::sort(entries_.begin(), entries_.end(),
			[](const AssetEntry& a, const AssetEntry& b)
			{
				if (a.isDirectory_ != b.isDirectory_) 
				{
					return a.isDirectory_;
				}

				return a.filepath_.filename().string() < b.filepath_.filename().string();
			});
		}

		void AssetBrowser::NavigateTo(const std::filesystem::path& directory)
		{
			const std::filesystem::path target = std::filesystem::absolute(directory).lexically_normal();

			// Prevent navigation outside asset root
			const std::filesystem::path relative = target.lexically_relative(rootPath_);

			if (target != rootPath_ && (relative.empty() || *relative.begin() == "..")) 
			{
				return;
			}

			currentPath_ = target;
			selectedAsset_.clear();

			RefreshDirectory();
		}

		void AssetBrowser::Draw() 
		{
			// Navigation controls

			if (currentPath_ != rootPath_) 
			{
				if (ImGui::Button("Back")) 
				{
					NavigateTo(currentPath_.parent_path());
				}

				ImGui::SameLine();
			}

			ImGui::Text("Assets");

			ImGui::SameLine();

			ImGui::TextDisabled("%s", 
				currentPath_.lexically_relative(rootPath_).generic_string().c_str());
			// Import and refresh
			if (ImGui::Button("+ Add File")) 
			{
				ImportFile();
			}

			ImGui::SameLine();

			if (ImGui::Button("Refresh")) 
			{
				RefreshDirectory();
			}

			// Search
			ImGui::SetNextItemWidth(220.0f);
			ImGui::InputText("Search", searchBuffer_, sizeof(searchBuffer_));
			ImGui::Separator();

			if (!errorMessage_.empty())
			{
				ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), 
					"%s", errorMessage_.c_str());

				return;
			}

			// Asset grid
			std::filesystem::path pendingNavigation;

			if (ImGui::BeginTable("AssetGrid", 4, ImGuiTableFlags_SizingStretchSame)) 
			{
				int column = 0;

				for (const AssetEntry& asset : entries_) 
				{
					const std::string filename = asset.filepath_.filename().string();

					// Simple filename filtering
					if (searchBuffer_[0] != '\0' &&
						filename.find(searchBuffer_) == std::string::npos) 
					{
						continue;
					}

					if (column == 0) 
					{
						ImGui::TableNextRow();
					}

					ImGui::TableSetColumnIndex(column);

					ImGui::PushID(asset.filepath_.string().c_str());

					const std::string label = (asset.isDirectory_ ? "[Folder] " : "[File] ") + filename;

					const bool selected = selectedAsset_ == asset.filepath_;

					if (ImGui::Selectable(
						label.c_str(),
						selected,
						ImGuiSelectableFlags_AllowDoubleClick,
						ImVec2(0.0f, 45.0f))) 
					{
						if (asset.isDirectory_ &&
							ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) 
						{
							pendingNavigation = asset.filepath_;
						} else 
						{
							selectedAsset_ = asset.filepath_;
						}
					}

					ImGui::PopID();

					column = (column + 1) % 4;
				}

				ImGui::EndTable();
			}

			if (!pendingNavigation.empty()) 
			{
				NavigateTo(pendingNavigation);
			}

			ImGui::Separator();

			if (!selectedAsset_.empty()) 
			{
				ImGui::Text("Selected: %s",
					selectedAsset_.filename().string().c_str());
			}
		}

		void AssetBrowser::ImportFile() 
		{
			// Initialise native file dialog
			if (NFD_Init() != NFD_OKAY) 
			{
				importMessage_ = "Failed to initialise file dialog";
				importFailed_ = true;
				return;
			}

			// Convert current directory to UTF-8
			const std::u8string currentPathUTF8 = currentPath_.u8string();

			const std::string defaultPath(reinterpret_cast<const char*>(currentPathUTF8.data()), currentPathUTF8.size());

			// Config file picker
			nfdopendialogu8args_t arguments = {};

			arguments.defaultPath = defaultPath.c_str();

			nfdu8char_t* selectedPath = nullptr;

			const nfdresult_t result = NFD_OpenDialogU8_With(&selectedPath, &arguments);

			std::filesystem::path sourcePath;

			if (result == NFD_OKAY) 
			{
				sourcePath = std::filesystem::u8path(selectedPath);

				NFD_FreePathU8(selectedPath);
			} else if (result == NFD_ERROR) 
			{
				const char* error = NFD_GetError();

				importMessage_ = error != nullptr ? error : "Unknown file dialog error";

				importFailed_ = true;
			}

			NFD_Quit();

			// Cancelled or failed
			if (result != NFD_OKAY) 
			{
				return;
			}

			// Validate
			std::error_code error;

			if (!std::filesystem::exists(sourcePath, error) ||
				!std::filesystem::is_regular_file(sourcePath, error)) 
			{
				importMessage_ = "Selected file is invalid";
				importFailed_ = true;
				return;
			}

			// Destination is always current asset browser directory
			const std::filesystem::path destination = currentPath_ / sourcePath.filename();

			// Prevent importing a file onto itself
			if (std::filesystem::equivalent(sourcePath, destination, error)) 
			{
				importMessage_ = "File is already in this directory";
				importFailed_ = true;
				return;
			}

			// Copy without overwriting existing files
			error.clear();

			if (!std::filesystem::copy_file(
				sourcePath,
				destination,
				std::filesystem::copy_options::none,
				error)) 
			{
				importMessage_ = error.message();
				importFailed_ = true;
				return;
			}

			// Successful import
			selectedAsset_ = destination;

			importMessage_ = "Imported: " + destination.filename().string();
			importFailed_ = false;

			RefreshDirectory();
		}
	}