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
		assetBrowser_(std::filesystem::path(KYEIGHT_GAME_PROJECT_DIR) / "assets")
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

		CreateTestObjects();

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

		ImGui::Button("Step");

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

			if (ImGui::Button("Load Scene"))
			{
				LoadScene();
			}
		}

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
			// Only named entities are displayed in the hierarchy
			if (!registry.HasComponent<KYEight::NameComponent>(entity)) 
			{
				continue;
			}

			const KYEight::NameComponent& name = registry.GetComponent<KYEight::NameComponent>(entity);

			const bool selected = selectedEntity_ == entity;

			if (ImGui::Selectable(name.name_.c_str(), selected)) 
			{
				selectedEntity_ = entity;
			}
		}

		ImGui::End();
	}
	void KYEditor::DrawInspector()
	{
		ImGui::Begin("Inspector");

		KYEight::Registry& registry = engine_->GetRegistry();

		if (!registry.IsValid(selectedEntity_) ||
			!registry.HasComponent<KYEight::NameComponent>(selectedEntity_)) 
		{
			selectedEntity_ = KYEight::NullEntity;

			ImGui::Text("No entity selected");
			ImGui::End();

			return;
		}

		// Display name of entity
		KYEight::NameComponent& name = registry.GetComponent<KYEight::NameComponent>(selectedEntity_);
		ImGui::Text("%s", name.name_.c_str());

		ImGui::Separator();

		// Draw entity properties
		DrawTransforms(selectedEntity_);

		ImGui::End();
	}
	void KYEditor::DrawTransforms(KYEight::Entity entity) 
	{
		KYEight::Registry& registry = engine_->GetRegistry();

		if (!registry.HasComponent<KYEight::Transform3D>(entity))
		{
			return;
		}

		KYEight::Transform3D& transform = registry.GetComponent<KYEight::Transform3D>(entity);

		if (!ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			return;
		}

		ImGui::DragFloat3("Position", &transform.position_.x, 0.1f);
		ImGui::DragFloat3("Rotation", &transform.rotation_.x, 0.1f);
		ImGui::DragFloat3("Scale", &transform.scale_.x, 0.1f);

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
	void KYEditor::LoadScene() 
	{
		const std::filesystem::path scenePath =
			std::filesystem::path(KYEIGHT_GAME_PROJECT_DIR) / sceneFilepath_;

		if (!engine_->LoadScene(scenePath.string(), &sceneError_))
		{
			showSceneError_ = true;
			return;
		}

		selectedEntity_ = KYEight::NullEntity;
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
	void KYEditor::CreateTestObjects()
	{
		KYEight::Registry& registry = engine_->GetRegistry();

		// OBJ model
		KYEight::Entity objcube = registry.CreateEntity();
		KYEight::NameComponent objcubeName;
		objcubeName.name_ = "Suzanne";

		KYEight::Transform3D objcubeTransform;
		objcubeTransform.position_ = { 0.0f, 1.0f, 3.0f };
		objcubeTransform.rotation_ = { 0.0f, 0.0f, 0.0f };
		objcubeTransform.scale_ = { 1.0f, 1.0f, 1.0f };

		const std::filesystem::path modelPath =
			std::filesystem::path(KYEIGHT_GAME_PROJECT_DIR) /
			"assets/models/Suzanne.obj";

		KYEight::ModelHandle objcubeHandle =
			engine_->GetResourceManager().LoadModel(modelPath.string());

		if (objcubeHandle != 0)
		{
			KYEight::MeshComponent objcubeMesh;
			objcubeMesh.modelHandle_ = objcubeHandle;
			objcubeMesh.colour_ = BLUE;
			objcubeMesh.filepath_ = "assets/models/Suzanne.obj";

			registry.AddComponent<KYEight::NameComponent>(objcube, objcubeName);
			registry.AddComponent<KYEight::Transform3D>(objcube, objcubeTransform);
			registry.AddComponent<KYEight::MeshComponent>(objcube, objcubeMesh);
		}
		else
		{
			registry.DestroyEntity(objcube);
		}

		// Cube
		KYEight::Entity cube = registry.CreateEntity();

		KYEight::NameComponent cubeName;
		cubeName.name_ = "Test Cube";

		KYEight::Transform3D cubeTransform;
		cubeTransform.position_ = { 0.0f, 1.0f, 0.0f };
		cubeTransform.rotation_ = { 0.0f, 0.0f, 0.0f };
		cubeTransform.scale_ = { 1.0f, 1.0f, 1.0f };

		KYEight::MeshComponent cubeMesh;
		cubeMesh.modelHandle_ = engine_->GetResourceManager().RegisterModel(
			LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f))
		);
		cubeMesh.colour_ = RED;

		KYEight::PrimitiveMeshData cubePrimitive;
		cubePrimitive.type_ = KYEight::MeshPrimitive::Cube;
		cubePrimitive.dimensions_ = { 1.0f, 1.0f, 1.0f };

		cubeMesh.primitive_ = cubePrimitive;

		registry.AddComponent<KYEight::NameComponent>(cube, cubeName);
		registry.AddComponent<KYEight::Transform3D>(cube, cubeTransform);
		registry.AddComponent<KYEight::MeshComponent>(cube, cubeMesh);


		// Sphere
		KYEight::Entity sphere = registry.CreateEntity();

		KYEight::NameComponent sphereName;
		sphereName.name_ = "Test Sphere";

		KYEight::Transform3D sphereTransform;
		sphereTransform.position_ = { 2.0f, 1.0f, 0.0f };
		sphereTransform.rotation_ = { 0.0f, 0.0f, 0.0f };
		sphereTransform.scale_ = { 1.0f, 1.0f, 1.0f };

		KYEight::MeshComponent sphereMesh;
		sphereMesh.modelHandle_ = engine_->GetResourceManager().RegisterModel(
			LoadModelFromMesh(GenMeshSphere(0.5f, 16, 16))
		);
		sphereMesh.colour_ = BLUE;

		KYEight::PrimitiveMeshData spherePrimitive;
		spherePrimitive.type_ = KYEight::MeshPrimitive::Sphere;
		spherePrimitive.dimensions_ = { 0.5f, 0.5f, 0.5f };
		spherePrimitive.slices_ = 16;
		spherePrimitive.rings_ = 16;

		sphereMesh.primitive_ = spherePrimitive;

		registry.AddComponent<KYEight::NameComponent>(sphere, sphereName);
		registry.AddComponent<KYEight::Transform3D>(sphere, sphereTransform);
		registry.AddComponent<KYEight::MeshComponent>(sphere, sphereMesh);


		// Second cube, with a different transform
		KYEight::Entity secondCube = registry.CreateEntity();

		KYEight::NameComponent secondCubeName;
		secondCubeName.name_ = "Test Cube 2";

		KYEight::Transform3D secondCubeTransform;
		secondCubeTransform.position_ = { -2.0f, 1.0f, 0.0f };
		secondCubeTransform.rotation_ = { 0.0f, 45.0f, 0.0f };
		secondCubeTransform.scale_ = { 1.0f, 2.0f, 1.0f };

		KYEight::MeshComponent secondCubeMesh;
		secondCubeMesh.modelHandle_ = engine_->GetResourceManager().RegisterModel(
			LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f))
		);
		secondCubeMesh.colour_ = GREEN;

		KYEight::PrimitiveMeshData secondCubePrimitive;
		secondCubePrimitive.type_ = KYEight::MeshPrimitive::Cube;
		secondCubePrimitive.dimensions_ = { 1.0f, 1.0f, 1.0f };

		secondCubeMesh.primitive_ = secondCubePrimitive;

		registry.AddComponent<KYEight::NameComponent>(secondCube, secondCubeName);
		registry.AddComponent<KYEight::Transform3D>(secondCube, secondCubeTransform);
		registry.AddComponent<KYEight::MeshComponent>(secondCube, secondCubeMesh);
	}
}
