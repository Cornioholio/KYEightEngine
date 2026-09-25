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
		rlImGuiBegin();

		ImGui::Begin("KYEight editor");

		ImGui::Text("I work!");

		ImGui::End();

		rlImGuiEnd();
	}
}
