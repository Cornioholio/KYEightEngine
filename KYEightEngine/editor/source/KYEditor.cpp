#include "KYEditor.h"

namespace KYEightEditor 
{
	bool KYEditor::Initialise() 
	{
		InitWindow(1920, 1080, "She's turned the weans against us");
		SetTargetFPS(60);

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

		ImGui::Begin("KYEight editor");

		RenderTexture2D viewport = engine.GetViewportTexture();

		ImGui::Image((ImTextureID)(uintptr_t)viewport.texture.id, ImVec2(1280, 720), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();

		rlImGuiEnd();

		EndDrawing();
	}
}
