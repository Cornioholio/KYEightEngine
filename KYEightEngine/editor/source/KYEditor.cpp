#include "KYEditor.h"

namespace KYEightEditor
{
	KYEditor::KYEditor() : viewportTarget_(1280, 720)
	{
		engine_ = new KYEight::KYEngine(viewportTarget_);
	}
	KYEditor::~KYEditor()
	{
		delete engine_;
		engine_ = nullptr;
	}

	bool KYEditor::Initialise()
	{
		// Set editor window properties, resizable and vsync 
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

		// Start setup for raylib <=> imgui connection
		rlImGuiSetup(true);

		// Allow docking
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Set callback for the UIDrawing

		return true;

	}
	void KYEditor::Run()
	{
		// Editor runtime
		while (!WindowShouldClose())
		{
			// Play session only updates when play is pressed, engine & ui rendering always happens 
			playSession_.Update(*engine_);

			engine_->Render();

			DrawEditorUI();
		}
	}
	void KYEditor::Shutdown()
	{
		// Shutdown editor and clean up
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

	void KYEditor::SetupDockSpace(ImGuiID dockID)
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

		// Real time resizing letterboxing, based on aspect ratio.
		ImVec2 availableSize = ImGui::GetContentRegionAvail();

		float viewportWidth = availableSize.x;
		float viewportHeight = viewportWidth / engine_->GetViewportAspectRatio();

		// If calculated height is too large, constrain by height instead
		if (viewportHeight > availableSize.y)
		{
			viewportHeight = availableSize.y;
			viewportWidth = viewportHeight * engine_->GetViewportAspectRatio();
		}

		ImVec2 dockMiddle = ImGui::GetCursorPos();

		dockMiddle.x += (availableSize.x - viewportWidth) * 0.5f;
		dockMiddle.y += (availableSize.y - viewportHeight) * 0.5f;

		ImGui::SetCursorPos(dockMiddle);

		RenderTexture2D viewport = engine_->GetViewportTexture();

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

		ImGui::Button("Step");

		ImGui::End();
	}
	void KYEditor::DrawSceneHierarchy()
	{
		ImGui::Begin("Objects");

		ImGui::Text("Scene");

		ImGui::Separator();

		const KYEight::Registry& registry = engine_->GetRegistry();

		for (KYEight::Entity entity : registry.GetActiveEntities()) 
		{
			if (!registry.HasComponent<KYEight::NameComponent>(entity)) 
			{
				continue;
			}

			const KYEight::NameComponent& name = registry.GetComponent<KYEight::NameComponent>(entity);

			ImGui::Text("%s", name.name_.c_str());
		}

		ImGui::End();
	}
	void KYEditor::DrawInspector()
	{
		ImGui::Begin("Inspector");

		ImGui::TextDisabled("Select an Entity or actor");

		ImGui::End();
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

	}
	void KYEditor::DrawConsole()
	{

	}
}
