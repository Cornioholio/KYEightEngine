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
		
		std::string error;

		if (!engine_->LoadScene("assets/scenes/MainScene.json", &error)) 
		{
			TraceLog(LOG_ERROR, "Failed to load game scene: %s", error.c_str());
			return false;
		}

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