#pragma once
#include "core/KYEngine.h"
#include "KYPlaySession.h"

#include "rlImGui.h"
#include "imgui_internal.h"

namespace KYEightEditor 
{
	class KYEditor 
	{
	public:
		KYEditor();
		~KYEditor();

		// Disable copy and move semantics
		KYEditor(const KYEditor&) = delete;
		KYEditor& operator=(const KYEditor&) = delete;

		KYEditor(KYEditor&&) = delete;
		KYEditor& operator=(KYEditor&&) = delete;

		bool Initialise();
		void Run();
		void Shutdown();

	private:
		void DrawEditorUI();

		void SetupDockSpace(ImGuiID dockID);

		void DrawToolbar();

		void DrawSceneHierarchy();
		void DrawViewport();
		void DrawInspector();
		void DrawBottomPanel();

		void DrawFiles();
		void DrawConsole();

		KYEight::KYEngine engine;
		KYPlaySession playSession;

		// Editor window
		unsigned int editorWindowHeight = 1080;
		unsigned int editorWindowWidth = 1920;

		const char* editorWindowTitle = "She's turned the weans against us";

	};
}