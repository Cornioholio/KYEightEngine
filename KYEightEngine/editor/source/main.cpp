#include "KYEditor.h"
// TO DO:
/*
- ImGui engine UI for development
- Logger/profiler when engine ImGui is implemented. Will write to console in engine UI or somet
*/
int main() 
{
	// Create engine 
	KYEightEditor::KYEditor editor;

	// Create engine and all resources
	if (!editor.Initialise())
	{
		return -1;
	}

	// Game loop (Update, Render, Input)
	editor.Run();

	// Clean up resources
	editor.Shutdown();

	return 0;
}