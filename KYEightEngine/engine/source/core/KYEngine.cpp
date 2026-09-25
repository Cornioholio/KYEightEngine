#include "KYEngine.h"

namespace KYEight 
{
	KYEngine::KYEngine() 
	{
		renderer = new KYRenderer();
	}
	KYEngine::~KYEngine() 
	{
		delete renderer;
		renderer = nullptr;
	}


	bool KYEngine::Initialise() 
	{
		// Initialize systems
		if (renderer == nullptr) 
		{
			std::cout << "[ENGINE] No instance of renderer exists, could not intiailize renderer\n";
			return false;
		}
		if (!renderer->Initialise()) 
		{
			std::cout << "[ENGINE} Failed to intialize renderer\n";
			return false;
		}
		return true;

	}
	void KYEngine::Run() 
	{
		// Game loop
		while (!WindowShouldClose()) 
		{
			BeginDrawing();

			ClearBackground(RAYWHITE);

			Update();
			renderer->RenderFrame();
			// Input when implemented

			if (editorCallback) 
			{
				editorCallback();
			}

			EndDrawing();

		}
	}
	void KYEngine::Shutdown() 
	{
		// Shut down systems
		if (renderer == nullptr) 
		{
			std::cout << "[ENGINE] No instance of renderer exists, could not shut renderer down\n";
		}

		renderer->Shutdown();
	}
	void KYEngine::Update() 
	{
		
	}


	void KYEngine::SetEditorCallback(std::function<void()> callback)
	{
		editorCallback = callback;
	}
}