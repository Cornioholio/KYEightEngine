#pragma once
#include "raylib.h"

#include <iostream>

#include "RenderTarget.h"

/// <summary>
/// Renderer class responsible for managing rendering frames for viewport texture in editor, or in game executable.
/// </summary>
namespace KYEight 
{
	class KYRenderer
	{
	public:
		KYRenderer(RenderTarget& target);
		~KYRenderer();

		// Disable move and copy semantics
		KYRenderer(const KYRenderer&) = delete;
		KYRenderer& operator=(const KYRenderer&) = delete;

		KYRenderer(KYRenderer&&) = delete;
		KYRenderer& operator=(KYRenderer&&) = delete;

		bool Initialise();
		void BeginFrame();
		void EndFrame();
		void Shutdown();

		// Camera access
		const Camera3D& GetCamera() const;

		// Editor access
		float GetAspectRatio() const;
		RenderTexture2D GetViewportTexture() const;

	private:
		void RenderScene();

		RenderTarget& renderTarget_;

		Camera3D camera_{};
	};
}
