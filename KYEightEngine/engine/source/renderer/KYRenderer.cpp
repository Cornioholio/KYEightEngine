#include "KYRenderer.h"

namespace KYEight 
{
	KYRenderer::KYRenderer(RenderTarget target) : renderTarget(target)
	{
		
	}
	KYRenderer::~KYRenderer()
	{
		
	}

	bool KYRenderer::Initialise() 
	{
		// Set initial dimensions for viewport texture.
		viewportTexture = LoadRenderTexture(viewportWidth, viewportHeight);

		// To keep window of a certain aspect ratio no matter viewport or window size, change to custom later on.
		aspectRatio = 16.0f / 9.0f;

		// Dont initialise if the viewport texture doesnt exist.
		if (viewportTexture.id == 0) 
		{
			return false;
		}

		// Set inital camera settings (temporary)
		camera.position = { 5.0f, 5.0f, 5.0f };
		camera.target = { 0.f, 0.f, 0.f };
		camera.up = { 0.f, 1.0f, 0.f };
		camera.fovy = 45.0f;
		camera.projection = CAMERA_PERSPECTIVE;

		return true;
	}

	void KYRenderer::RenderFrame()
	{
		// If the target is for a viewport
		if (renderTarget == RenderTarget::Viewport) 
		{
			BeginTextureMode(viewportTexture);

			ClearBackground(RAYWHITE);

			RenderScene();

			EndTextureMode();
		} 
		else  
		{
			// If the target is a window
			BeginDrawing();

			ClearBackground(RAYWHITE);

			RenderScene();

			EndDrawing();
		}
	}
	// Render 3D game scene 
	void KYRenderer::RenderScene() 
	{
		BeginMode3D(camera);

		DrawGrid(20, 1.0f);
		DrawCube({ 0.f, 1.0f, 0.f }, 2.0f, 2.0f, 2.0f, RED);

		EndMode3D();
	}
	// Dispose of viewport texture 
	void KYRenderer::Shutdown() 
	{
		if (viewportTexture.id != 0) 
		{
			UnloadRenderTexture(viewportTexture);
			viewportTexture = {};
		}
	}

	void KYRenderer::ResizeViewport(unsigned int width, unsigned int height) 
	{
		if (width == 0 || height == 0)
		{
			return;
		}
		if (viewportWidth == width || viewportHeight == height) 
		{
			return; 
		}

		if (viewportTexture.id != 0) 
		{
			UnloadRenderTexture(viewportTexture);
		}

		viewportWidth = width;
		viewportHeight = height;
		
		viewportTexture = LoadRenderTexture(viewportWidth, viewportHeight);
	}

	float KYRenderer::GetAspectRatio() const 
	{
		return aspectRatio;
	}
	RenderTexture2D KYRenderer::GetViewportTexture() const 
	{
		return viewportTexture;
	}

}