#include "KYEditor.h"

namespace KYEightEditor 
{
	bool KYEditor::Initialise() 
	{
		// Set editor window properties, resizable and vsync 
		SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
		InitWindow(editorWindowWidth, editorWindowHeight, editorWindowTitle);
		MaximizeWindow();

		// Dont allow initialisation if window or engine arent both ready
		if (!IsWindowReady()) 
		{
			return false;
		}
		if (!engine.Initialise()) 
		{
			return false;
		}
		
		// Start setup for raylib <=> imgui connection
		rlImGuiSetup(true);

		// Set callback for the UIDrawing
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
		// Shutdown editor and clean up
		rlImGuiShutdown();

		engine.Shutdown();

		CloseWindow();
	}
	void KYEditor::DrawViewport() 
	{
		ImGui::Begin("KYEight Viewport");

		// Real time resizing letterboxing, based on aspect ratio.
		ImVec2 availableSize = ImGui::GetContentRegionAvail();

		float viewportWidth = availableSize.x;
		float viewportHeight = viewportWidth / engine.GetViewportAspectRatio();

		// If calculated height is too large, constrain by height instead
		if (viewportHeight > availableSize.y) 
		{
			viewportHeight = availableSize.y;
			viewportWidth = viewportHeight * engine.GetViewportAspectRatio();
		}

		ImVec2 dockMiddle = ImGui::GetCursorPos();

		dockMiddle.x += (availableSize.x - viewportWidth) * 0.5f;
		dockMiddle.y += (availableSize.y - viewportHeight) * 0.5f;

		ImGui::SetCursorPos(dockMiddle);
		
		RenderTexture2D viewport = engine.GetViewportTexture();

		ImGui::Image((ImTextureID)(uintptr_t)viewport.texture.id, ImVec2(viewportWidth, viewportHeight), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}
	void KYEditor::DrawEditorUI() 
	{
		BeginDrawing();

		ClearBackground(DARKGRAY);

		rlImGuiBegin();

		DrawViewport();

		rlImGuiEnd();

		EndDrawing();
	}
}
