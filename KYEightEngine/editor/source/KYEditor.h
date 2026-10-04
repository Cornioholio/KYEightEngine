#pragma once
#include "core/KYEngine.h"
#include "renderer/ViewportTarget.h"

#include "PlaySession.h"

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

		KYEight::ViewportTarget viewportTarget_;

		KYEight::KYEngine* engine_ = nullptr;

		PlaySession playSession_;

		// Editor window
		unsigned int editorWindowHeight_ = 1080;
		unsigned int editorWindowWidth_ = 1920;

		const char* editorWindowTitle_ = "Choaklit creem";

	};
}