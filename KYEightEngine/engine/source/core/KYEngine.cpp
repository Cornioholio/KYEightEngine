#include "KYEngine.h"

namespace KYEight 
{
	// Create and dispose of systems here
	KYEngine::KYEngine(RenderTarget& target) 
	{
		renderer_ = std::make_unique<KYRenderer>(target);
		resourceManager_ = std::make_unique<KYResourceManager>();
	}
	KYEngine::~KYEngine() 
	{

	}
	bool KYEngine::Initialise() 
	{
		// Make sure renderer_ exists and initialises
		if (renderer_ == nullptr) 
		{
			std::cout << "[ENGINE] No instance of renderer_ exists, could not intiailize renderer_\n";
			return false;
		}
		if (!renderer_->Initialise()) 
		{
			std::cout << "[ENGINE} Failed to intialize renderer_\n";
			return false;
		}
		
		return true;
	}
	void KYEngine::Shutdown() 
	{
		// Clean up systems
		if (resourceManager_) 
		{
			resourceManager_->Shutdown();
		} else 
		{
			std::cout << "[ENGINE] No instance of resourceManager_ exists, could not shit down resourceManager_ down\n";
		}

		if (renderer_) 
		{
			renderer_->Shutdown();
		} else 
		{
			std::cout << "[ENGINE] No instance of renderer_ exists, could not shut renderer_ down\n";
		
		}

	}

	// Main game loop.
	void KYEngine::Update() 
	{
		std::cout << "Playing!\n";

		// ECS Update, Timer Tick, Update function for all user scripts.
	}
	void KYEngine::Render() 
	{
		renderer_->BeginFrame();

		meshSystem_.Render(registry_, *resourceManager_, renderer_->GetCamera());

		renderer_->EndFrame();

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
		resourceManager_->UnloadAll();

		if (!KYDomainConverter::ToRegistry(domain, registry_, *resourceManager_))
		{
			return false;
		}

		return true;
	}


	// For editor use.
	KYResourceManager& KYEngine::GetResourceManager() 
	{
		return *resourceManager_;
	}
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
		return renderer_->GetAspectRatio();
	}
	RenderTexture2D KYEngine::GetViewportTexture() const
	{
		return renderer_->GetViewportTexture();
	}
}