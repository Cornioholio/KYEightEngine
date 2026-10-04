#pragma once
#include "RenderTarget.h"

#include "raylib.h"

namespace KYEight 
{
	class WindowTarget : public RenderTarget 
	{
	public:
		virtual bool Initialise() override;
		virtual void Begin() override;
		virtual void End() override;
		virtual void Shutdown() override;

		bool IsViewport() const override;

		float GetAspectRatio() const override;
		RenderTexture2D GetViewportTexture() const override;
	};
}