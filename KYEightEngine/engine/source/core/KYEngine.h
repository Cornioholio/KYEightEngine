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
		KYEngine(KYEngine&) = delete;
		KYEngine& operator=(KYEngine&) = delete;

		KYEngine(KYEngine&&) = delete;
		KYEngine& operator=(KYEngine&&) = delete;

		bool Initialise();

		void Update();
		void Render();

		void Shutdown();

		float GetViewportAspectRatio() const;
		RenderTexture2D GetViewportTexture() const;
	private:

		KYRenderer* renderer;
	};
}