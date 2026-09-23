#include "KYRenderer.h"

namespace KYEight 
{
	KYRenderer::KYRenderer() 
	{
		
	}
	KYRenderer::~KYRenderer()
	{
		
	}

	bool KYRenderer::Initialize() 
	{
		CreateWindow();

		return true;
	}
	void KYRenderer::CreateWindow() 
	{
		// Init window and create rlImGUI context
		InitWindow(windowWidth, windowHeight, windowHandle);
		rlImGuiSetup(true);
	}

	void KYRenderer::RenderFrame()
	{
		BeginDrawing();
		rlImGuiBegin();

		ImGui::Begin("KYEightEngine");

		ImGui::Text("Work please");

		if (ImGui::Button("Hello"))
		{
			std::cout << "Works!\n";
		}

		ImGui::End();

		rlImGuiEnd();

		EndDrawing();
	}

	void KYRenderer::Shutdown() 
	{
		rlImGuiShutdown();

		CloseWindow();
	}
}