#include "KYEngine.h"

namespace KYEight 
{
	// Create and dispose of systems here
	KYEngine::KYEngine(RenderTarget& target) 
	{
		renderer = new KYRenderer(target);
	}
	KYEngine::~KYEngine() 
	{
		delete renderer;
		renderer = nullptr;
	}
	bool KYEngine::Initialise() 
	{
		// Make sure renderer exists and initialises
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
	void KYEngine::Shutdown() 
	{
		// Renderer should exist to dispose
		if (renderer == nullptr) 
		{
			std::cout << "[ENGINE] No instance of renderer exists, could not shut renderer down\n";
		}

		renderer->Shutdown();
	}

	// Main game loop.
	void KYEngine::Update() 
	{
		std::cout << "Playing!\n";
	}
	void KYEngine::Render() 
	{
		renderer->RenderFrame();
	}

	// For editor use.
	float KYEngine::GetViewportAspectRatio() const
	{
		return renderer->GetAspectRatio();
	}
	RenderTexture2D KYEngine::GetViewportTexture() const
	{
		return renderer->GetViewportTexture();
	}
}