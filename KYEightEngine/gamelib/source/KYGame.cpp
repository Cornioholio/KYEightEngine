#include "KYGame.h"

namespace KYEightGame 
{
	KYGame::KYGame() 
	{
		engine_ = std::make_unique<KYEight::KYEngine>(windowTarget_);
	}
	KYGame::~KYGame() = default;

	bool KYGame::Initialise()
	{
		SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
		InitWindow(1280, 720, "KYEight Game");

		if (!IsWindowReady()) 
		{
			return false;
		}
		if (!engine_->Initialise()) 
		{
			return false;
		}

		KYEight::Registry originalRegistry;

		KYEight::Entity entity = originalRegistry.CreateEntity();

		originalRegistry.AddComponent<KYEight::NameComponent>(
			entity, "Serialization round-trip test");

		KYEight::Transform3D transform;
		transform.position_ = { 1.f, 2.f, 3.f };
		transform.rotation_ = { 0.f, 90.f, 0.f };
		transform.scale_ = { 1.f, 3.f, 4.f };

		originalRegistry.AddComponent<KYEight::Transform3D>(
			entity, transform);

		// Convert the registry into persistent scene data.
		KYEight::KYDomain domain =
			KYEight::KYDomainConverter::FromRegistry(originalRegistry);

		// Save the scene.
		std::string error;

		if (!KYEight::KYDomainSerialiser::Save(
			domain, "KYEightSceneTest.json", &error))
		{
			std::cout << "Save failed: " << error << '\n';
			return false;
		}

		// Load the scene back from disk.
		KYEight::KYDomain loadedDomain;

		if (!KYEight::KYDomainSerialiser::Load(
			loadedDomain, "KYEightSceneTest.json", &error))
		{
			std::cout << "Load failed: " << error << '\n';
			return false;
		}

		// Reconstruct the ECS into a fresh registry.
		KYEight::Registry loadedRegistry;

		KYEight::KYDomainConverter::ToRegistry(
			loadedDomain, loadedRegistry);

		// Verify the reconstructed data.
		if (loadedRegistry.GetActiveEntities().size() != 1)
		{
			std::cout << "Test failed: incorrect entity count\n";
			return false;
		}

		KYEight::Entity loadedEntity =
			*loadedRegistry.GetActiveEntities().begin();

		const auto& loadedName =
			loadedRegistry.GetComponent<KYEight::NameComponent>(loadedEntity);

		const auto& loadedTransform =
			loadedRegistry.GetComponent<KYEight::Transform3D>(loadedEntity);

		std::cout << "Loaded entity count: "
			<< loadedRegistry.GetActiveEntities().size() << '\n';

		std::cout << "Name: " << loadedName.name_ << '\n';

		std::cout << "Position: "
			<< loadedTransform.position_.x << ", "
			<< loadedTransform.position_.y << ", "
			<< loadedTransform.position_.z << '\n';

		std::cout << "Rotation: "
			<< loadedTransform.rotation_.x << ", "
			<< loadedTransform.rotation_.y << ", "
			<< loadedTransform.rotation_.z << '\n';

		std::cout << "Scale: "
			<< loadedTransform.scale_.x << ", "
			<< loadedTransform.scale_.y << ", "
			<< loadedTransform.scale_.z << '\n';
		return true;
	}

	void KYGame::Run()
	{
		while (!WindowShouldClose())
		{
			engine_->Update();
			engine_->Render();
		}
	}

	void KYGame::Shutdown() 
	{
		engine_->Shutdown();
		CloseWindow();
	}
}