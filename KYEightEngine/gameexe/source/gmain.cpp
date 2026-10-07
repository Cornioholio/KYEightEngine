#include "KYGame.h"

// Game entry point
int main() 
{
	KYEightGame::KYGame game;

	if (!game.Initialise()) 
	{
		return -1;
	}

	// Game loop
	game.Run();

	game.Shutdown();

	return 0;
}