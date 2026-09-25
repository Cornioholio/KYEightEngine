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
		KYRenderer(KYRenderer&) = delete;
		KYRenderer& operator=(const KYRenderer&) = delete;

		KYRenderer(KYRenderer&&) = delete;
		KYRenderer& operator=(KYRenderer&&) = delete;

		bool Initialise();
		void RenderFrame();
		void Shutdown();

		void ResizeViewport(unsigned int width, unsigned int height);
		RenderTexture2D GetViewportTexture() const;

	private:
		RenderTexture2D viewportTexture{};

		unsigned int viewportHeight = 1280;
		unsigned int viewportWidth = 720;

		Camera3D camera{};
	};
}
