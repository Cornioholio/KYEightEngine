#include "KYRenderer.h"

namespace KYEight 
{
	KYRenderer::KYRenderer() 
	{
		
	}
	KYRenderer::~KYRenderer()
	{
		
	}

	bool KYRenderer::Initialise() 
	{
		viewportTexture = LoadRenderTexture(1280, 720);

		if (viewportTexture.id == 0) 
		{
			return false;
		}

		return true;
	}

	void KYRenderer::RenderFrame()
	{
		BeginTextureMode(viewportTexture);

		ClearBackground(RAYWHITE);

		// Game rendering

		EndTextureMode();
	}

	void KYRenderer::Shutdown() 
	{

	}

	RenderTexture2D KYRenderer::GetViewportTexture() const 
	{
		return viewportTexture;
	}
}