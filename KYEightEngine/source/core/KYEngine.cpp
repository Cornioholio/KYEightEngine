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


	bool KYEngine::Initialize() 
	{
		// Initialize systems
		if (renderer == nullptr) 
		{
			std::cout << "[ENGINE] No instance of renderer exists, could not intiailize renderer\n";
			return false;
		}
		if (!renderer->Initialize()) 
		{
			std::cout << "[ENGINE} Failed to intialize renderer\n";
			return false;
		}
		return true;

	}
	void KYEngine::Run() 
	{
		// Game loop
		while(!WindowShouldClose()) 
		{
			renderer->RenderFrame();

			Update();

			// Input when implemented
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
}