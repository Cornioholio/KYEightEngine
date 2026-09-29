#include "KYGame.h"

int main() 
{
	KYEightGame::KYGame game;

	if (!game.Initialise()) 
	{
		return -1;
	}

	game.RunFrame();

	game.Shutdown();

	return 0;
}