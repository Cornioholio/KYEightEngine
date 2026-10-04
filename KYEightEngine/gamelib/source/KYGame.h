#pragma once
#include "core/KYEngine.h"
#include "renderer/WindowTarget.h"	

namespace KYEightGame 
{
	class KYGame 
	{
	public:
		KYGame();
		~KYGame();

		KYGame(KYGame&) = delete;
		KYGame& operator=(KYGame&) = delete;

		KYGame(KYGame&&) = delete;
		KYGame& operator=(KYGame&&) = delete;

		bool Initialise();

		void RunFrame();

		void Shutdown();

	private:
		KYEight::WindowTarget windowTarget;

		KYEight::KYEngine* engine = nullptr;
	
	};
}