#include "KYEditor.h"

namespace KYEightEditor 
{
	bool KYEditor::Initialise() 
	{
		SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
		InitWindow(editorWindowWidth, editorWindowHeight, editorWindowTitle);
		MaximizeWindow();

		if (!IsWindowReady()) 
		{
			return false;
		}

		
		if (!engine.Initialise()) 
		{
			return false;
		}
			
		rlImGuiSetup(true);

		engine.SetEditorCallback([this]()
		{
			DrawEditorUI();
		});

		return true;
		
	}
	void KYEditor::Run() 
	{
		engine.Run();
	}
	void KYEditor::Shutdown() 
	{
		rlImGuiShutdown();

		engine.Shutdown();

		CloseWindow();
	}

	void KYEditor::DrawEditorUI() 
	{
		BeginDrawing();

		ClearBackground(DARKGRAY);

		rlImGuiBegin();

		ImGui::Begin("KYEight Viewport");

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		RenderTexture2D viewport = engine.GetViewportTexture();

		ImGui::Image((ImTextureID)(uintptr_t)viewport.texture.id, viewportSize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();

		rlImGuiEnd();

		EndDrawing();
	}
}
