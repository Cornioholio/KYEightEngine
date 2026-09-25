#pragma once
#include "core/KYEngine.h"
#include "rlImGui.h"
#include "imgui.h"

namespace KYEightEditor 
{
	class KYEditor 
	{
	public:
		bool Initialise();
		void Run();
		void Shutdown();

	private:
		void DrawEditorUI();

		KYEight::KYEngine engine;
	
		unsigned int editorWindowHeight = 1080;
		unsigned int editorWindowWidth = 1920;

		const char* editorWindowTitle = "She's turned the weans against us";

	};
}