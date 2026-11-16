#pragma once
#include "renderer/KYRenderer.h"

#include "domain/KYDomain.h"
#include "domain/KYDomainConverter.h"
#include "domain/KYDomainSerialiser.h"

#include "ecs/Registry.h"
#include "ecs/systems/MeshSystem.h"

#include "resources/KYResourceManager.h"

#include <iostream>
#include <memory>

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
		KYEngine& operator=(const KYEngine&) = delete;

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

		bool SaveScene(const std::string& filepath, std::string* error = nullptr);
		bool LoadScene(const std::string& filepath, std::string* error = nullptr);

		// Getters for editor use.
		KYResourceManager& GetResourceManager();
		Registry& GetRegistry();
		const Registry& GetRegistry() const;
		float GetViewportAspectRatio() const;
		RenderTexture2D GetViewportTexture() const;
	private:
		// Systems
		std::unique_ptr<KYRenderer> renderer_;
		std::unique_ptr<KYResourceManager> resourceManager_;
		Registry registry_;
		MeshSystem meshSystem_;
	};
}