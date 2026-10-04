#include "ViewportTarget.h"

namespace KYEight
{
	ViewportTarget::ViewportTarget(unsigned int width, unsigned int height) : viewportWidth_(width), viewportHeight_(height)
	{

	}
	ViewportTarget::~ViewportTarget() 
	{
		Shutdown();
	}
	bool ViewportTarget::Initialise() 
	{
		viewportTexture_ = LoadRenderTexture(viewportWidth_, viewportHeight_);
		if (viewportTexture_.id == 0) 
		{
			return false;
		}

		aspectRatio_ = static_cast<float>(viewportWidth_) / static_cast<float>(viewportHeight_);

		return true;
	}
	void ViewportTarget::Begin()  
	{
		BeginTextureMode(viewportTexture_);
	}
	void ViewportTarget::End()  
	{
		EndTextureMode();
	}
	void ViewportTarget::Shutdown() 
	{
		if (viewportTexture_.id != 0) 
		{
			UnloadRenderTexture(viewportTexture_);
			viewportTexture_ = {};
		}
	}
	void ViewportTarget::Resize(unsigned int width, unsigned int height) 
	{
		if (width == 0 || height == 0)
		{
			return;
		}

		if (viewportWidth_ == width && viewportHeight_ == height)
		{
			return;
		}

		if (viewportTexture_.id != 0)
		{
			UnloadRenderTexture(viewportTexture_);
			viewportTexture_ = {};
		}

		viewportWidth_ = width;
		viewportHeight_ = height;

		viewportTexture_ = LoadRenderTexture(viewportWidth_, viewportHeight_);

		if (viewportTexture_.id != 0)
		{
			aspectRatio_ =
				static_cast<float>(viewportWidth_) /
				static_cast<float>(viewportHeight_);
		}
	}

	// Getters

	bool ViewportTarget::IsViewport() const
	{
		return true;
	}

	RenderTexture2D ViewportTarget::GetViewportTexture() const
	{
		return viewportTexture_;
	}
	float ViewportTarget::GetAspectRatio() const 
	{
		return aspectRatio_;
	}
}