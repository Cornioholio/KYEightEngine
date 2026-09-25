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
	};
}