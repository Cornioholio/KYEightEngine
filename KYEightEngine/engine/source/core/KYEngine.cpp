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

		// ECS Update, Timer Tick, Update function for all user scripts.
	}
	void KYEngine::Render() 
	{
		renderer->BeginFrame();

		meshSystem_.Render(registry_, renderer->GetCamera());

		renderer->EndFrame();

	}


	bool KYEngine::SaveScene(const std::string& filepath, std::string* error) 
	{
		KYDomain domain = KYDomainConverter::FromRegistry(registry_);

		return KYDomainSerialiser::Save(domain, filepath, error);
	}
	bool KYEngine::LoadScene(const std::string& filepath, std::string* error)  
	{
		KYDomain domain;

		if (!KYDomainSerialiser::Load(domain, filepath, error))
		{
			return false;
		}

		registry_.Clear();
	
		KYDomainConverter::ToRegistry(domain, registry_);

		return true;
	}


	// For editor use.
	Registry& KYEngine::GetRegistry() 
	{
		return registry_;
	}
	const Registry& KYEngine::GetRegistry() const 
	{
		return registry_;
	}
	float KYEngine::GetViewportAspectRatio() const
	{
		return renderer->GetAspectRatio();
	}
	RenderTexture2D KYEngine::GetViewportTexture() const
	{
		return renderer->GetViewportTexture();
	}
}