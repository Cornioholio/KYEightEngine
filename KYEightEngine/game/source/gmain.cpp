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
	game.RunFrame();

	game.Shutdown();

	return 0;
}