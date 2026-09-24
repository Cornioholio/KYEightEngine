#include "core/KYEngine.h"

// TO DO:
/*
- ImGui engine UI for development
- Logger/profiler when engine ImGui is implemented. Will write to console in engine UI or somet
*/
int main() 
{
	// Create engine 
	KYEight::KYEngine engine;

	// Create engine and all resources
	if (!engine.Initialize()) 
	{
		return -1;
	}

	// Game loop (Update, Render, Input)
	engine.Run();

	// Clean up resources
	engine.Shutdown();

	return 0;
}