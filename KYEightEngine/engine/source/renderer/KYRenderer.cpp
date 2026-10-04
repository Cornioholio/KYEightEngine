#include "KYRenderer.h"

namespace KYEight 
{
	KYRenderer::KYRenderer(RenderTarget& target) : renderTarget_(target)
	{
		
	}
	KYRenderer::~KYRenderer()
	{
		
	}

	bool KYRenderer::Initialise() 
	{
		if (!renderTarget_.Initialise()) 
		{
			std::cerr << "Failed to initialise render target." << std::endl;
			return false;
		}

		// Set inital camera settings (temporary)
		camera_.position = { 5.0f, 5.0f, 5.0f };
		camera_.target = { 0.f, 0.f, 0.f };
		camera_.up = { 0.f, 1.0f, 0.f };
		camera_.fovy = 45.0f;
		camera_.projection = CAMERA_PERSPECTIVE;

		return true;
	}

	void KYRenderer::RenderFrame()
	{
		renderTarget_.Begin();

		ClearBackground(RAYWHITE);

		RenderScene();

		renderTarget_.End();
	}
	// Render 3D game scene 
	void KYRenderer::RenderScene() 
	{
		BeginMode3D(camera_);

		DrawGrid(20, 1.0f);
		DrawCube({ 0.f, 1.0f, 0.f }, 2.0f, 2.0f, 2.0f, RED);

		EndMode3D();
	}

	void KYRenderer::Shutdown() 
	{
		renderTarget_.Shutdown();
	}	

	float KYRenderer::GetAspectRatio() const
	{
		return renderTarget_.GetAspectRatio();
	}
	RenderTexture2D KYRenderer::GetViewportTexture() const
	{
		return renderTarget_.GetViewportTexture();
	}
}