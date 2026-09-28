#include "KYGame.h"

namespace KYEightGame 
{
	KYGame::KYGame() 
	{

	}
	KYGame::~KYGame() 
	{

	}

	bool KYGame::Initialise()
	{
		SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
		InitWindow(1280, 720, "KYEight Game");

		if (!IsWindowReady()) 
		{
			return false;
		}
		if (!engine.Initialise()) 
		{
			return false;
		}

		return true;
	}

	void KYGame::RunFrame() 
	{
		engine.Update();
		engine.Render();
	}

	void KYGame::Shutdown() 
	{
		engine.Shutdown();
		CloseWindow();
	}
}