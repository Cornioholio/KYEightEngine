#pragma once
#include "core/KYEngine.h"
#include "renderer/WindowTarget.h"	
#include "domain/KYDomain.h"
#include "domain/KYDomainSerialiser.h"
#include "domain/KYDomainConverter.h"

#include <memory>

/// <summary>
/// KYGame encapsulates the game loop and manages the game engine and window target for rendering.
/// </summary>
namespace KYEightGame 
{
	class KYGame 
	{
	public:
		KYGame();
		~KYGame();

		// Copy and move semantics again
		KYGame(KYGame&) = delete;
		KYGame& operator=(KYGame&) = delete;

		KYGame(KYGame&&) = delete;
		KYGame& operator=(KYGame&&) = delete;

		bool Initialise();

		void Run();

		void Shutdown();

	private:
		KYEight::WindowTarget windowTarget_;

		std::unique_ptr<KYEight::KYEngine> engine_ = nullptr;
	
	};
}