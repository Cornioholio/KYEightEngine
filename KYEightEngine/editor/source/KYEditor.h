#pragma once

// Engine and rendering
#include "core/KYEngine.h"
#include "renderer/ViewportTarget.h"
#include "PlaySession.h"

// Entities
#include "ecs/Entity.h"

// Panels
#include "panels/AssetBrowser.h"
#include "panels/SceneHierarchy.h"

#include <memory>
#include <filesystem>

namespace KYEightEditor
{
	/// <summary>
	/// Owns an engine instance aswell as managing the editor window lifetime. And coordinates the viewport, scene hierarchy, inspector, and file explorer / console.
	///	Delegates the gameplay session control to PlaySession.
	/// 
	/// Editor is uncopyable and unmoveable because it owns runtime resources and maintains references between its rendering systems.
	/// </summary>
	class KYEditor
	{
	public:
		/// <summary>
		/// Constructs the editor's viewport target and engine instance.
		/// 
		/// The engine renders into 'viewportTarget_', which is owned by the editor.
		/// </summary>
		KYEditor();

		/// <summary>
		/// Releases the editor object, runtime systems are shut down explicitly through shutdown methods.
		/// </summary>
		~KYEditor();

		// Editor owns runtime resources, no copy or move
		KYEditor(const KYEditor&) = delete;
		KYEditor& operator=(const KYEditor&) = delete;

		KYEditor(KYEditor&&) = delete;
		KYEditor& operator=(KYEditor&&) = delete;

		// Application lifecycle

		/// <summary>
		/// Initialises the editor window, engine and ImGui integration.
		/// 
		/// Intiialisation order follows: Window, Engine, ImGui integration.
		/// </summary>
		/// <returns>True if initialistion was successful</returns>
		bool Initialise();

		/// <summary>
		/// Executes editor's main loop.
		/// 
		/// Gameplay updates are controlled by PlaySession. 
		/// Engine rendering and editor UI rendering continue while the editor is in edit mode.
		/// </summary>
		void Run();

		/// <summary>
		/// Shuts down play session, engine and editor window.
		/// 
		/// Always call after method 'Run' has finished and before destroying the editor.
		/// </summary>
		void Shutdown();

	private:
		// Editor UI
		void DrawEditorUI();
		void SetupDockSpace(unsigned int dockID);
		void DrawToolbar();

		// Editor Panels
		void DrawSceneHierarchy();
		void DrawViewport();
		void DrawInspector();
		void DrawBottomPanel();

		// Bottom Panel tabs
		void DrawFiles();
		void DrawConsole();

		// Inspector possible entity properties
		void DrawTransforms(KYEight::Entity entity);

		// Scene Tools
		void SaveScene();
		void LoadScene();
		void DrawSceneErrorPopup();

		// Testing
		void CreateTestObjects();
		
		// Editor state
		KYEight::Entity selectedEntity_ = KYEight::NullEntity;

		// Owned runtime systems
		KYEight::ViewportTarget viewportTarget_;
		std::unique_ptr<KYEight::KYEngine> engine_ = nullptr;
		PlaySession playSession_;

		// Panels
		AssetBrowser assetBrowser_;
		SceneHierarchy sceneHierarchy_;

		// Editor viewport confiuration
		const unsigned int editorWindowHeight_ = 1080;
		const unsigned int editorWindowWidth_ = 1920;
		const char* editorWindowTitle_ = "KYEightEditor - Try Unity!";

		//Scene configuration
		std::string sceneFilepath_ = "assets/scenes/MainScene.json";
		std::string sceneError_;
		bool showSceneError_ = false;
	};
}