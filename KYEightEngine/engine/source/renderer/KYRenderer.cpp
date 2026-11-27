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

		// Load lighting shader
		lightingShader_ = LoadShader(
			"assets/shaders/lighting.vert",
			"assets/shaders/lighting.frag"
		);

		if (lightingShader_.id == 0)
		{
			std::cerr << "Failed to load lighting shader.";
			return false;
		}

		lightingShaderLoaded_ = true;

		// Standard raylib shader locations
		lightingShader_.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(lightingShader_, "mvp");
		lightingShader_.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(lightingShader_, "matModel");
		lightingShader_.locs[SHADER_LOC_MATRIX_NORMAL] = GetShaderLocation(lightingShader_, "matNormal");
		lightingShader_.locs[SHADER_LOC_COLOR_DIFFUSE] = GetShaderLocation(lightingShader_, "colDiffuse");
		lightingShader_.locs[SHADER_LOC_MAP_DIFFUSE] = GetShaderLocation(lightingShader_, "texture0");

		TraceLog(LOG_INFO, "[RENDERER] Lighting shader loaded");

		return true;
	}
	void KYRenderer::BeginFrame() 
	{
		renderTarget_.Begin();

		ClearBackground(RAYWHITE);
	}
	void KYRenderer::EndFrame() 
	{
		renderTarget_.End();
	}

	void KYRenderer::Shutdown() 
	{
		renderTarget_.Shutdown();
	}	

	const Camera3D& KYRenderer::GetCamera() const 
	{
		return camera_;
	}

	float KYRenderer::GetAspectRatio() const
	{
		return renderTarget_.GetAspectRatio();
	}
	RenderTexture2D KYRenderer::GetViewportTexture() const
	{
		return renderTarget_.GetViewportTexture();
	}
	const Shader& KYRenderer::GetLightingShader() const 
	{
		return lightingShader_;
	}
}