#pragma once
#include "renderer/KYRenderer.h"

#include <iostream>
namespace KYEight 
{
	class KYEngine
	{
	public:
		KYEngine();
		~KYEngine();

		// Disable move and copy semantics
		KYEngine(const KYEngine&) = delete;
		KYEngine& operator=(const KYEngine&) = delete;

		KYEngine(const KYEngine&&) = delete;
		KYEngine& operator=(KYEngine&&) = delete;

		bool Initialize();
		void Run();
		void Shutdown();

	private:
		void Update();

		KYRenderer* renderer;

	};
}