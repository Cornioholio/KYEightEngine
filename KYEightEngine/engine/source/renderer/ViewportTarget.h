#pragma once
#include "RenderTarget.h"

#include "raylib.h"

namespace KYEight 
{
	class ViewportTarget : public RenderTarget
	{
	public:
		ViewportTarget(unsigned int width, unsigned int height);
		~ViewportTarget();

		bool Initialise() override;
		void Begin() override;
		void End() override;
		void Shutdown() override;

		bool IsViewport() const override;

		void Resize(unsigned int width, unsigned int height);

		float GetAspectRatio() const;
		RenderTexture2D GetViewportTexture() const;

	private:
		RenderTexture2D viewportTexture_{};

		unsigned int viewportWidth_ = 1280;
		unsigned int viewportHeight_ = 720;

		float aspectRatio_ = 16.0f / 9.0f;
	};
}