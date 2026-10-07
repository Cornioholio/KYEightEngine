#pragma once
#include "renderer/KYRenderer.h"

#include "ecs/Registry.h"
#include "ecs/systems/MeshSystem.h"

#include <iostream>

/// <summary>
/// KYEngine encapsulates all core game systems and provides the game loop (update, render and input).
/// </summary>
namespace KYEight
{
	class KYEngine
	{
	public:
		KYEngine(RenderTarget& target);
		~KYEngine();

		// Disable move and copy semantics
		KYEngine(KYEngine&) = delete;
		KYEngine& operator=(KYEngine&) = delete;

		KYEngine(KYEngine&&) = delete;
		KYEngine& operator=(KYEngine&&) = delete;

		// Sets up all game systems (render, input etc.)
		bool Initialise();

		// Update game logic.
		void Update();
		// Render game graphics, either viewport or game (Render target dependent).
		void Render();
		// Clean up 
		void Shutdown();

		// Getters for editor use.
		Registry& GetRegistry();
		const Registry& GetRegistry() const;
		float GetViewportAspectRatio() const;
		RenderTexture2D GetViewportTexture() const;
	private:
		// Systems
		KYRenderer* renderer = nullptr;

		Registry registry_;
		MeshSystem meshSystem_;

		Entity cubeEntity_ = NullEntity;
	};
}