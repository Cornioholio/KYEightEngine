#pragma once
#include "raylib.h"

namespace KYEight 
{
	class RenderTarget 
	{
	public:
		virtual ~RenderTarget() = default;

		virtual bool Initialise() = 0;
		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Shutdown() = 0;

		virtual bool IsViewport() const = 0;

		virtual float GetAspectRatio() const = 0;
		virtual RenderTexture2D GetViewportTexture() const = 0;
	};
}	