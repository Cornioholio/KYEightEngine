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

		camera.position = { 5.0f, 5.0f, 5.0f };
		camera.target = { 0.f, 0.f, 0.f };
		camera.up = { 0.f, 1.0f, 0.f };
		camera.fovy = 45.0f;
		camera.projection = CAMERA_PERSPECTIVE;

		return true;
	}

	void KYRenderer::RenderFrame()
	{
		BeginTextureMode(viewportTexture);

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		DrawGrid(20, 1.0f);
		DrawCube({ 0.f, 1.0f, 0.f }, 2.0f, 2.0f, 2.0f, RED);
		DrawCubeWires({ 0.f, 1.0f, 0.f }, 2.0f, 2.0f, 2.0f, BLUE);

		EndMode3D();

		EndTextureMode();
	}

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

	RenderTexture2D KYRenderer::GetViewportTexture() const 
	{
		return viewportTexture;
	}
}