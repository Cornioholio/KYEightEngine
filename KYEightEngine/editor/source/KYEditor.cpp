#include "KYEditor.h"

// Editor UI
#include "rlImGui.h"
#include "imgui_internal.h"

// ECS
#include "ecs/Registry.h"
#include "ecs/components/CoreComponents.h"

#include <memory>


namespace KYEightEditor
{

	KYEditor::KYEditor() 
		: viewportTarget_(1280, 720), 
		assetBrowser_(std::filesystem::path(KYEIGHT_GAME_PROJECT_DIR)),
		entityInspector_(std::filesystem::path(KYEIGHT_GAME_PROJECT_DIR))
	{
		engine_ = std::make_unique<KYEight::KYEngine>(viewportTarget_);
	}

	KYEditor::~KYEditor() = default;

	bool KYEditor::Initialise()
	{
		// Set editor window properties, resizable and vsync, then create window
		SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
		InitWindow(editorWindowWidth_, editorWindowHeight_, editorWindowTitle_);
		MaximizeWindow();

		// Dont allow initialisation if window or engine arent both ready
		if (!IsWindowReady())
		{
			return false;
		}
		if (!engine_->Initialise())
		{
			return false;
		}

		// Start setup for raylib/imgui connection
		rlImGuiSetup(true);

		// Allow ui docking
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		return true;

	}
	void KYEditor::Run()
	{
		// Editor runtime
		while (!WindowShouldClose())
		{
			// Play session only updates when play is pressed and play session is active
			playSession_.Update(*engine_);

			// Rendeirng always happens
			engine_->Render();
			DrawEditorUI();
		}
	}
	void KYEditor::Shutdown()
	{
		// Stop gameplay befoer shutting systems it owns down
		playSession_.Stop(*engine_);

		rlImGuiShutdown();
		engine_->Shutdown();

		CloseWindow();
	}

	void KYEditor::DrawEditorUI()
	{
		BeginDrawing();

		ClearBackground(DARKGRAY);

		rlImGuiBegin();

		DrawToolbar();
		DrawSceneErrorPopup();

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		
		ImVec2 dockSpacePos = viewport->WorkPos;
		dockSpacePos.y += 45.f;

		ImVec2 dockSpaceSize = viewport->WorkSize;
		dockSpaceSize.y -= 45.f;
		

		ImGui::SetNextWindowPos(dockSpacePos);
		ImGui::SetNextWindowSize(dockSpaceSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGuiWindowFlags dockFlags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoBackground;

		ImGui::Begin("KYEight Editorm", nullptr, dockFlags);

		ImGui::PopStyleVar(2);

		ImGuiID dockID = ImGui::GetID("KYEightDockSpace");

		ImGui::DockSpace(dockID, ImVec2(0.f, 0.f), ImGuiDockNodeFlags_None);

		SetupDockSpace(dockID);

		ImGui::End();

		DrawSceneHierarchy();
		DrawViewport();
		DrawInspector();
		DrawBottomPanel();

		rlImGuiEnd();

		EndDrawing();
	}

	void KYEditor::SetupDockSpace(unsigned int dockID)
	{
		static bool layoutBuilt = false;

		if (layoutBuilt)
		{
			return;
		}

		layoutBuilt = true;

		ImGui::DockBuilderRemoveNode(dockID);

		ImGui::DockBuilderAddNode(dockID, ImGuiDockNodeFlags_DockSpace);

		ImGui::DockBuilderSetNodeSize(dockID, ImGui::GetMainViewport()->WorkSize);

		ImGuiID dockMain = dockID;

		// Left panel
		ImGuiID dockLeft;

		ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Left, 0.2f, &dockLeft, &dockMain);

		// Right panel
		ImGuiID dockRight;

		ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Right, 0.2f, &dockRight, &dockMain);

		// Bottom panel
		ImGuiID dockBottom;

		ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Down, 0.2f, &dockBottom, &dockMain);

		// Assign all windows
		ImGui::DockBuilderDockWindow("Objects", dockLeft);
		ImGui::DockBuilderDockWindow("Viewport", dockMain);
		ImGui::DockBuilderDockWindow("Inspector", dockRight);
		ImGui::DockBuilderDockWindow("File Explorer", dockBottom);

		ImGui::DockBuilderFinish(dockID);
	}

	void KYEditor::DrawViewport()
	{
		ImGui::Begin("Viewport");

		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		const float aspectRatio = engine_->GetViewportAspectRatio();

		// Fit rendered viewport inside the available panel while preserving aspect ratio.
		float viewportWidth = availableSize.x;
		float viewportHeight = viewportWidth / engine_->GetViewportAspectRatio();

		// If calculated height is too large, constrain by height instead
		if (viewportHeight > availableSize.y)
		{
			viewportHeight = availableSize.y;
			viewportWidth = viewportHeight * engine_->GetViewportAspectRatio();
		}

		// Center the viewport inside panel
		ImVec2 dockMiddle = ImGui::GetCursorPos();

		dockMiddle.x += (availableSize.x - viewportWidth) * 0.5f;
		dockMiddle.y += (availableSize.y - viewportHeight) * 0.5f;

		ImGui::SetCursorPos(dockMiddle);

		RenderTexture2D viewport = engine_->GetViewportTexture();

		// Flip vertical texture coordinates to account for render texture orientation.
		ImGui::Image((ImTextureID)(uintptr_t)viewport.texture.id, ImVec2(viewportWidth, viewportHeight), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}
	void KYEditor::DrawToolbar()
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);

		ImGui::SetNextWindowSize(ImVec2((float)GetScreenWidth(), 45.f), ImGuiCond_Always);

		ImGui::Begin("Tool Bar", nullptr, flags);

		if (!playSession_.isPlaying())
		{
			if (ImGui::Button("Play"))
			{
				playSession_.Start(*engine_);
			}
		}
		else
		{
			if (ImGui::Button("Stop"))
			{
				playSession_.Stop(*engine_);
			}
		}

		ImGui::SameLine();

		ImGui::Button("Pause");

		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();

		if (!playSession_.isPlaying())
		{
			if (ImGui::Button("Save Scene"))
			{
				SaveScene();
			}

			ImGui::SameLine();

			const std::filesystem::path& selectedAsset = assetBrowser_.GetSelectedAsset();
			const bool canLoadScene = !selectedAsset.empty() && selectedAsset.extension() == ".json" && std::filesystem::is_regular_file(selectedAsset);

			ImGui::BeginDisabled(!canLoadScene);

			if (ImGui::Button("Load Scene"))
			{
				LoadScene();
			}

			ImGui::EndDisabled();

			ImGui::SameLine();

			if (ImGui::Button("Save As")) 
			{
				ImGui::OpenPopup("Save Scene As");
			}
		}

		DrawSaveAsPopup();

		ImGui::End();
	}
	void KYEditor::DrawSceneHierarchy()
	{
		sceneHierarchy_.Draw(engine_->GetRegistry(), selectedEntity_);
	}
	void KYEditor::DrawBottomPanel()
	{
		ImGui::Begin("File Explorer");

		if (ImGui::BeginTabBar("BottomPanelTabs"))
		{
			if (ImGui::BeginTabItem("Files"))
			{
				DrawFiles();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Console"))
			{
				DrawConsole();

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}
	void KYEditor::DrawFiles()
	{
		assetBrowser_.Draw();
	}
	void KYEditor::DrawConsole()
	{

	}
	void KYEditor::DrawInspector()
	{
		if (engine_ == nullptr) 
		{
			return;
		}

		entityInspector_.Draw(engine_->GetRegistry(), selectedEntity_, engine_->GetResourceManager());
	}
	// Scene stuff
	void KYEditor::SaveScene() 
	{
		sceneError_.clear();

		const std::filesystem::path scenePath =
			std::filesystem::path(KYEIGHT_GAME_PROJECT_DIR) / sceneFilepath_;

		std::error_code filesystemError;

		std::filesystem::create_directories(
			scenePath.parent_path(),
			filesystemError
		);

		if (filesystemError)
		{
			sceneError_ = "Could not create scene directory: " +
				filesystemError.message();

			showSceneError_ = true;
			return;
		}

		if (!engine_->SaveScene(scenePath.string(), &sceneError_))
		{
			showSceneError_ = true;
			return;
		}

		TraceLog(LOG_INFO, "Scene saved: %s", scenePath.string().c_str());
	}
	void KYEditor::SaveSceneAs() 
	{
		sceneError_.clear();

		std::string filename = saveSceneName_;

		if (filename.empty()) 
		{
			sceneError_ = "Please enter a scene filename.";
			showSceneError_ = true;
			return;
		}

		std::filesystem::path filenamePath(filename);

		// Only allow a filename, not a dir path

		if (filenamePath.has_parent_path() ||
			filenamePath.filename() != filenamePath) 
		{
			sceneError_ = "Invalid scene filename.";
			showSceneError_ = true;
			return;
		}

		// Add JSON extension if it was not supplied
		if (filenamePath.extension().empty()) 
		{
			filenamePath += ".json";
		} else if (filenamePath.extension() != ".json")
		{
			sceneError_ = "Scene file must use the .json extension.";
			showSceneError_ = true;
			return;
		}

		// Save into the currently displayed asset browser directory.
		const std::filesystem::path scenePath = assetBrowser_.GetCurrentPath() / filenamePath;

		std::error_code filesystemError;

		// Do not overwrite an existing scene
		if (std::filesystem::exists(scenePath, filesystemError))
		{
			sceneError_ = "A file with that name already exists";
			showSceneError_ = true;

			return;
		}

		if (filesystemError) 
		{
			sceneError_ = "Could not check destination: " + filesystemError.message();
			showSceneError_ = true;
			return;
		}

		if (!engine_->SaveScene(scenePath.string(), &sceneError_)) 
		{
			showSceneError_ = true;
			return;
		}
			
		// Make new scene the currently active scene
		sceneFilepath_ = scenePath.lexically_relative(std::filesystem::path(KYEIGHT_GAME_PROJECT_DIR)).generic_string();

		assetBrowser_.Refresh();
		assetBrowser_.SelectAsset(scenePath);

		TraceLog(LOG_INFO, "New scene saved: %s", scenePath.string().c_str());

		ImGui::CloseCurrentPopup();
	}
	void KYEditor::LoadScene() 
	{
		sceneError_.clear();

		const std::filesystem::path scenePath =
			assetBrowser_.GetSelectedAsset();

		if (scenePath.empty() || scenePath.extension() != ".json")
		{
			sceneError_ = "Please select a valid JSON scene file.";
			showSceneError_ = true;
			return;
		}

		std::error_code filesystemError;

		if (!std::filesystem::is_regular_file(scenePath, filesystemError))
		{
			sceneError_ = "Selected scene file does not exist.";
			showSceneError_ = true;
			return;
		}

		if (!engine_->LoadScene(scenePath.string(), &sceneError_))
		{
			showSceneError_ = true;
			return;
		}

		// Update the active scene path after a successful load.
		sceneFilepath_ = scenePath.lexically_relative(
			std::filesystem::path(KYEIGHT_GAME_PROJECT_DIR)
		).generic_string();

		selectedEntity_ = KYEight::NullEntity;

		TraceLog(LOG_INFO, "Scene loaded: %s", scenePath.string().c_str());
	}
	void KYEditor::DrawSaveAsPopup() 
	{
		if (ImGui::BeginPopupModal("Save Scene As", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) 
		{
			ImGui::Text("Enter new name:");
			ImGui::InputText("Filename", saveSceneName_, sizeof(saveSceneName_));

			if (ImGui::Button("Save")) 
			{
				SaveSceneAs();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel")) 
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	void KYEditor::DrawSceneErrorPopup() 
	{
		if (showSceneError_) 
		{
			ImGui::OpenPopup("Scene Error");

			showSceneError_ = false;
		}

		if (ImGui::BeginPopupModal("Scene Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) 
		{
			ImGui::TextWrapped("%s", sceneError_.c_str());

			if (ImGui::Button("Ok")) 
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

	}
}
