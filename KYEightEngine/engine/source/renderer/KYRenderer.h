#pragma once
#include "raylib.h"

#include <iostream>

namespace KYEight 
{
	class KYRenderer
	{
	public:
		KYRenderer();
		~KYRenderer();

		// Disable move and copy semantics
		KYRenderer(const KYRenderer&) = delete;
		KYRenderer& operator=(const KYRenderer&) = delete;

		KYRenderer(const KYRenderer&&) = delete;
		KYRenderer& operator=(KYRenderer&&) = delete;

		bool Initialise();
		void RenderFrame();
		void Shutdown();

		RenderTexture2D GetViewportTexture() const;

	private:
		RenderTexture2D viewportTexture;

		unsigned int windowHeight = 1080;
		unsigned int windowWidth = 1920;

		const char* windowHandle = "She's turned the weans against us";

	};
}
