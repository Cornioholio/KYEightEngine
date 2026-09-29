#pragma once
#include "raylib.h"

#include <iostream>

namespace KYEight 
{
	class KYRenderer
	{
	public:
		enum class RenderTarget 
		{
			Viewport,
			Window
		};

		KYRenderer();
		~KYRenderer();

		// Disable move and copy semantics
		KYRenderer(KYRenderer&) = delete;
		KYRenderer& operator=(const KYRenderer&) = delete;

		KYRenderer(KYRenderer&&) = delete;
		KYRenderer& operator=(KYRenderer&&) = delete;

		bool Initialise();
		void RenderFrame(RenderTarget target = RenderTarget::Viewport);
		void Shutdown();

		void ResizeViewport(unsigned int width, unsigned int height);

		RenderTexture2D GetViewportTexture() const;
		float GetAspectRatio() const;

	private:
		void RenderScene();

		RenderTexture2D viewportTexture{};

		float aspectRatio = 0.f;

		unsigned int viewportHeight = 720;
		unsigned int viewportWidth = 1280;

		Camera3D camera{};
	};
}
